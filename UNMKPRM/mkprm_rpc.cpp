#include "mkprm_rpc.h"

void RPC_mkprm_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mkprm slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("mkprm slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mkprm slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mkprm_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mkprm signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("mkprm signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mkprm signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mkprm_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mkprm_SLOT_Object>(new RPC_mkprm_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mkprm_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mkprm_SIGNAL_Object>(new RPC_mkprm_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mkprm_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_mkprm_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_mkprm_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_mkprm_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mkprm new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int RPC_mkprm_SLOT_Object::unmkprm_get_strings(int strings, QVariantList& string_data)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(strings);
	tmp_list << QVariant(string_data);
	SRPCSignalClass::Instance().toLog(QString("mkprm dynamic_call unmkprm_get_strings %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmkprm_get_strings(int, QVariantList&)", tmp_list);
	string_data = tmp_list.at(1).toList();
	tmp_ret_params += " string_data="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mkprm dynamic_call finished unmkprm_get_strings %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_mkprm_SLOT_Object::unmkprm_start()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mkprm dynamic_call unmkprm_start %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmkprm_start()", tmp_list);
	SRPCSignalClass::Instance().toLog("mkprm dynamic_call finished unmkprm_start");
}
void RPC_mkprm_SLOT_Object::unmkprm_stop()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mkprm dynamic_call unmkprm_stop %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmkprm_stop()", tmp_list);
	SRPCSignalClass::Instance().toLog("mkprm dynamic_call finished unmkprm_stop");
}
void RPC_mkprm_SLOT_Object::new_frame(QString mode, QVariant _frame_data)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mode);
	tmp_list << QVariant(_frame_data);
	SRPCSignalClass::Instance().toLog(QString("mkprm dynamic_call new_frame %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_frame(QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("mkprm dynamic_call finished new_frame");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

