#include "BOOP_rpc.h"

void RPC_BOOP_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("BOOP slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("BOOP slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("BOOP slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_BOOP_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("BOOP signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("BOOP signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("BOOP signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_BOOP_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_BOOP_SLOT_Object>(new RPC_BOOP_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_BOOP_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_BOOP_SIGNAL_Object>(new RPC_BOOP_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_BOOP_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_BOOP_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_BOOP_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_BOOP_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("BOOP new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

 RPC_BOOP_SLOT_Object::void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("BOOP dynamic_call void new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("void new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("BOOP dynamic_call finished void new_message %1").arg(tmp_ret_params));
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

