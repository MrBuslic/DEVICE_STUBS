#include "BOOP_rpc.h"

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

void RPC_BOOP_SLOT_Object::new_message(QVariant dt, int MKO, int line, int command_word, QVariantList words, int respond_word)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(MKO);
	tmp_list << QVariant(line);
	tmp_list << QVariant(command_word);
	tmp_list << QVariant(words);
	tmp_list << QVariant(respond_word);
	SRPCSignalClass::Instance().toLog(QString("BOOP dynamic_call new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("BOOP dynamic_call finished new_message");
}
void RPC_BOOP_SLOT_Object::new_matrix_command(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mshm);
	tmp_list << QVariant(pshm);
	tmp_list << QVariant(length_m);
	tmp_list << QVariant(length_p);
	tmp_list << QVariant(u_m);
	tmp_list << QVariant(u_p);
	tmp_list << QVariant(dt);
	tmp_list << QVariant(line_m);
	tmp_list << QVariant(line_p);
	SRPCSignalClass::Instance().toLog(QString("BOOP dynamic_call new_matrix_command %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_matrix_command(int, int, int, int, double, double, int, int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("BOOP dynamic_call finished new_matrix_command");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

