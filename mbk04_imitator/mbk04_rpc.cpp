#include "mbk04_rpc.h"

void RPC_mbk04_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mbk04 slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("mbk04 slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mbk04 slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mbk04_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mbk04 signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("mbk04 signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mbk04 signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mbk04_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mbk04_SLOT_Object>(new RPC_mbk04_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mbk04_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mbk04_SIGNAL_Object>(new RPC_mbk04_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mbk04_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_mbk04_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mbk04_SIGNAL_Object::new_tm)) {
		SRPCSignalClass::Instance().toLog("new_tm connected");
		emit connect_signal("new_tm(int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mbk04_SIGNAL_Object::state_changed_signal)) {
		SRPCSignalClass::Instance().toLog("state_changed_signal connected");
		emit connect_signal("state_changed_signal()", true);
	}
}

void RPC_mbk04_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mbk04_SIGNAL_Object::new_tm)) {
		SRPCSignalClass::Instance().toLog("new_tm disconnected");
		//emit connect_signal("new_tm(int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mbk04_SIGNAL_Object::state_changed_signal)) {
		SRPCSignalClass::Instance().toLog("state_changed_signal disconnected");
		//emit connect_signal("state_changed_signal()", false);
	}
}

void RPC_mbk04_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mbk04 new signal " + op_name);

			if (op_name == "new_tm(int)")
			{
				int nw;
				tmp_stream >> nw;
				SRPCSignalClass::Instance().toLog("mbk04 " + op_name +" call_number "+ QString::number(call_number) + " nw = "+RPCSignalClass::QVariantToString(nw));
				emit new_tm(nw);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mbk04 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "state_changed_signal()")
			{
				emit state_changed_signal();
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mbk04 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_mbk04_SLOT_Object::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("mbk04 dynamic_call new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mbk04 dynamic_call finished new_message");
}
void RPC_mbk04_SLOT_Object::new_ku(int ku_n, int length, double u)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	SRPCSignalClass::Instance().toLog(QString("mbk04 dynamic_call new_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku(int, int, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("mbk04 dynamic_call finished new_ku");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

