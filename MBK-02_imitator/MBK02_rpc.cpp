#include "MBK02_rpc.h"

void RPC_MBK02_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("MBK02 slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("MBK02 slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("MBK02 slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_MBK02_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("MBK02 signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("MBK02 signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("MBK02 signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_MBK02_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_MBK02_SLOT_Object>(new RPC_MBK02_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_MBK02_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_MBK02_SIGNAL_Object>(new RPC_MBK02_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_MBK02_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
{
	QByteArray tmp_arr2;
	QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
	if (_connect) tmp_stream2 << QString("connect"); else tmp_stream2 << QString("disconnect");
	tmp_stream2 << signal_name;
	QByteArray tmp_arr3;
	QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
	tmp_stream3 << tmp_arr2.size();
	_sock->write(tmp_arr3 + tmp_arr2);
	_sock->waitForBytesWritten(3000);
}

void RPC_MBK02_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::msg_to_14R732)) {
		SRPCSignalClass::Instance().toLog("msg_to_14R732 connected");
		emit connect_signal("msg_to_14R732(QVariantList)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::set_new_tm)) {
		SRPCSignalClass::Instance().toLog("set_new_tm connected");
		emit connect_signal("set_new_tm(int, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::set_new_power_tm)) {
		SRPCSignalClass::Instance().toLog("set_new_power_tm connected");
		emit connect_signal("set_new_power_tm(int, QVariantList)", true);
	}
}

void RPC_MBK02_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::msg_to_14R732)) {
		SRPCSignalClass::Instance().toLog("msg_to_14R732 disconnected");
		//emit connect_signal("msg_to_14R732(QVariantList)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::set_new_tm)) {
		SRPCSignalClass::Instance().toLog("set_new_tm disconnected");
		//emit connect_signal("set_new_tm(int, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_MBK02_SIGNAL_Object::set_new_power_tm)) {
		SRPCSignalClass::Instance().toLog("set_new_power_tm disconnected");
		//emit connect_signal("set_new_power_tm(int, QVariantList)", false);
	}
}

void RPC_MBK02_SIGNAL_Object::read_data()
{
	int tmp_size;
	while (_sock->bytesAvailable())
	{
		int tmp_bytes = _sock->bytesAvailable();
		while (tmp_bytes < 4)
		{
			_sock->waitForReadyRead(100);
			tmp_bytes = _sock->bytesAvailable();
		}
		QByteArray tmp_size_arr = _sock->read(4);
		QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
		tmp_size_stream >> tmp_size;
		while (1)
		{
			tmp_bytes = _sock->bytesAvailable();
			if (tmp_bytes >= tmp_size)
				break;
			_sock->waitForReadyRead(500);
		}
		SRPCSignalClass::Instance().toLog(QString("%1 signal new data %2 bytes").arg(objectName()).arg(tmp_size));
		QByteArray tmp_arr = _sock->read(tmp_size);
		QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
		while(!tmp_stream.atEnd())
		{
			QString op_name;
			tmp_stream >> op_name;
			int call_number;
			tmp_stream >> call_number;

			SRPCSignalClass::Instance().toLog("MBK02 new signal " + op_name);

			if (op_name == "msg_to_14R732(QVariantList)")
			{
				QVariantList data;
				tmp_stream >> data;
				SRPCSignalClass::Instance().toLog("MBK02 " + op_name +" call_number "+ QString::number(call_number) + " data = "+RPCSignalClass::QVariantToString(data));
				emit msg_to_14R732(data);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("MBK02 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "set_new_tm(int, int)")
			{
				int sadr;
				tmp_stream >> sadr;
				SRPCSignalClass::Instance().toLog("MBK02 " + op_name +" call_number "+ QString::number(call_number) + " sadr = "+RPCSignalClass::QVariantToString(sadr));
				int word;
				tmp_stream >> word;
				SRPCSignalClass::Instance().toLog("MBK02 " + op_name +" call_number "+ QString::number(call_number) + " word = "+RPCSignalClass::QVariantToString(word));
				emit set_new_tm(sadr, word);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("MBK02 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "set_new_power_tm(int, QVariantList)")
			{
				int sadr;
				tmp_stream >> sadr;
				SRPCSignalClass::Instance().toLog("MBK02 " + op_name +" call_number "+ QString::number(call_number) + " sadr = "+RPCSignalClass::QVariantToString(sadr));
				QVariantList words;
				tmp_stream >> words;
				SRPCSignalClass::Instance().toLog("MBK02 " + op_name +" call_number "+ QString::number(call_number) + " words = "+RPCSignalClass::QVariantToString(words));
				emit set_new_power_tm(sadr, words);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("MBK02 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_MBK02_SLOT_Object::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished new_message");
}
void RPC_MBK02_SLOT_Object::new_ku_732(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call new_ku_732 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku_732(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished new_ku_732");
}
void RPC_MBK02_SLOT_Object::set_new_mbk02_tm()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call set_new_mbk02_tm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_new_mbk02_tm()", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished set_new_mbk02_tm");
}
void RPC_MBK02_SLOT_Object::new_KPI(QVariantList KPI_list)
{
	QVariantList tmp_list;
	tmp_list << QVariant(KPI_list);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call new_KPI %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_KPI(QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished new_KPI");
}
void RPC_MBK02_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished auto_scroll_clicked");
}
void RPC_MBK02_SLOT_Object::update_tm(int sadr)
{
	QVariantList tmp_list;
	tmp_list << QVariant(sadr);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call update_tm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("update_tm(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished update_tm");
}
void RPC_MBK02_SLOT_Object::get_power(double volt)
{
	QVariantList tmp_list;
	tmp_list << QVariant(volt);
	SRPCSignalClass::Instance().toLog(QString("MBK02 dynamic_call get_power %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_power(double)", tmp_list);
	SRPCSignalClass::Instance().toLog("MBK02 dynamic_call finished get_power");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

