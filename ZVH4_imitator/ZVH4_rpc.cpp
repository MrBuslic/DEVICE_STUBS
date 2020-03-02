#include "ZVH4_rpc.h"

void RPC_ZVH4_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ZVH4_SLOT_Object>(new RPC_ZVH4_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ZVH4_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ZVH4_SIGNAL_Object>(new RPC_ZVH4_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ZVH4_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_ZVH4_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_ZVH4_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_ZVH4_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("ZVH4 new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_ZVH4_SLOT_Object::error_Slot(QAbstractSocket::SocketError socketError)
{
	QVariantList tmp_list;
	tmp_list << QVariant(socketError);
	SRPCSignalClass::Instance().toLog(QString("ZVH4 dynamic_call error_Slot %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("error_Slot(QAbstractSocket::SocketError)", tmp_list);
	SRPCSignalClass::Instance().toLog("ZVH4 dynamic_call finished error_Slot");
}
void RPC_ZVH4_SLOT_Object::read()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("ZVH4 dynamic_call read %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("read()", tmp_list);
	SRPCSignalClass::Instance().toLog("ZVH4 dynamic_call finished read");
}
void RPC_ZVH4_SLOT_Object::connect_ag()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("ZVH4 dynamic_call connect_ag %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("connect_ag()", tmp_list);
	SRPCSignalClass::Instance().toLog("ZVH4 dynamic_call finished connect_ag");
}
void RPC_ZVH4_SLOT_Object::read_ag()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("ZVH4 dynamic_call read_ag %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("read_ag()", tmp_list);
	SRPCSignalClass::Instance().toLog("ZVH4 dynamic_call finished read_ag");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

