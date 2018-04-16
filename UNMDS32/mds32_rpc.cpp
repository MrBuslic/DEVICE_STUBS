#include "mds32_rpc.h"

void RPC_mds32_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mds32 slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("mds32 slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mds32 slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mds32_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mds32 signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("mds32 signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mds32 signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mds32_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mds32_SLOT_Object>(new RPC_mds32_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mds32_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mds32_SIGNAL_Object>(new RPC_mds32_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mds32_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_mds32_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_mds32_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_mds32_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mds32 new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_mds32_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call auto_scroll_clicked");
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call finished auto_scroll_clicked");
}
void RPC_mds32_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call log_timer_ontimer");
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call finished log_timer_ontimer");
}
int RPC_mds32_SLOT_Object::unmds32_input_trigger(bool state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call unmds32_input_trigger");
	dynamic_call("unmds32_input_trigger(bool)", tmp_list);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call finished unmds32_input_trigger");
	return res.toInt();
}
int RPC_mds32_SLOT_Object::unmds32_read_sample(unsigned int&, int& _lasteTime)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(int&);
	tmp_list << QVariant(_lasteTime);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call unmds32_read_sample");
	dynamic_call("unmds32_read_sample(unsigned, int&)", tmp_list);
	_lasteTime = tmp_list.at(1).toInt();
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call finished unmds32_read_sample");
	return res.toInt();
}
int RPC_mds32_SLOT_Object::unmds32_start()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call unmds32_start");
	dynamic_call("unmds32_start()", tmp_list);
	SRPCSignalClass::Instance().toLog("mds32 dynamic_call finished unmds32_start");
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

