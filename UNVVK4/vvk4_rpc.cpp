#include "vvk4_rpc.h"

void RPC_vvk4_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("vvk4 slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("vvk4 slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("vvk4 slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_vvk4_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("vvk4 signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("vvk4 signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("vvk4 signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_vvk4_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_vvk4_SLOT_Object>(new RPC_vvk4_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_vvk4_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_vvk4_SIGNAL_Object>(new RPC_vvk4_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_vvk4_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_vvk4_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_vvk4_SIGNAL_Object::foi_interrupt)) {
		SRPCSignalClass::Instance().toLog("foi_interrupt connected");
		emit connect_signal("foi_interrupt(int, short, double, double)", true);
	}
}

void RPC_vvk4_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_vvk4_SIGNAL_Object::foi_interrupt)) {
		SRPCSignalClass::Instance().toLog("foi_interrupt disconnected");
		//emit connect_signal("foi_interrupt(int, short, double, double)", false);
	}
}

void RPC_vvk4_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("vvk4 new signal " + op_name);

			if (op_name == "foi_interrupt(int, short, double, double)")
			{
				int _n;
				tmp_stream >> _n;
				SRPCSignalClass::Instance().toLog("vvk4 " + op_name +" call_number "+ QString::number(call_number) + " _n = "+RPCSignalClass::QVariantToString(_n));
				short _chan;
				tmp_stream >> _chan;
				SRPCSignalClass::Instance().toLog("vvk4 " + op_name +" call_number "+ QString::number(call_number) + " _chan = "+RPCSignalClass::QVariantToString(_chan));
				double _u;
				tmp_stream >> _u;
				SRPCSignalClass::Instance().toLog("vvk4 " + op_name +" call_number "+ QString::number(call_number) + " _u = "+RPCSignalClass::QVariantToString(_u));
				double _t;
				tmp_stream >> _t;
				SRPCSignalClass::Instance().toLog("vvk4 " + op_name +" call_number "+ QString::number(call_number) + " _t = "+RPCSignalClass::QVariantToString(_t));
				emit foi_interrupt(_n, _chan, _u, _t);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("vvk4 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_vvk4_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("vvk4 dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("vvk4 dynamic_call finished log_timer_ontimer");
}
int RPC_vvk4_SLOT_Object::unfoi_run()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("vvk4 dynamic_call unfoi_run %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unfoi_run()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("vvk4 dynamic_call finished unfoi_run %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_vvk4_SLOT_Object::unvvk4_commut_ListOutput(int _line, QString _masOn, QString _masOff)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_line);
	tmp_list << QVariant(_masOn);
	tmp_list << QVariant(_masOff);
	SRPCSignalClass::Instance().toLog(QString("vvk4 dynamic_call unvvk4_commut_ListOutput %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unvvk4_commut_ListOutput(int, QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("vvk4 dynamic_call finished unvvk4_commut_ListOutput %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

