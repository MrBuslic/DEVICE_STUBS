#include "n736_rpc.h"

void RPC_n736_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_n736_SLOT_Object>(new RPC_n736_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_n736_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_n736_SIGNAL_Object>(new RPC_n736_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_n736_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_n736_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", true);
	}
}

void RPC_n736_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_n736_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", false);
	}
}

void RPC_n736_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("n736 new signal " + op_name);

			if (op_name == "new_ku(int, int, double, int)")
			{
				int ku_n;
				tmp_stream >> ku_n;
				SRPCSignalClass::Instance().toLog("n736 " + op_name +" call_number "+ QString::number(call_number) + " ku_n = "+RPCSignalClass::QVariantToString(ku_n));
				int length;
				tmp_stream >> length;
				SRPCSignalClass::Instance().toLog("n736 " + op_name +" call_number "+ QString::number(call_number) + " length = "+RPCSignalClass::QVariantToString(length));
				double u;
				tmp_stream >> u;
				SRPCSignalClass::Instance().toLog("n736 " + op_name +" call_number "+ QString::number(call_number) + " u = "+RPCSignalClass::QVariantToString(u));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("n736 " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				emit new_ku(ku_n, length, u, line);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("n736 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

