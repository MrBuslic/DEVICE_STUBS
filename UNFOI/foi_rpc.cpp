#include "foi_rpc.h"

void RPC_foi_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_foi_SLOT_Object>(new RPC_foi_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_foi_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_foi_SIGNAL_Object>(new RPC_foi_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_foi_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_foi_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_foi_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_foi_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("foi new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int RPC_foi_SLOT_Object::unfoi_map_channels_setup(int _n, short _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call unfoi_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unfoi_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call finished unfoi_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_foi_SLOT_Object::unfoi_chan_setup(int _n, short _chan, double _u, double _t)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	tmp_list << QVariant(_u);
	tmp_list << QVariant(_t);
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call unfoi_chan_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unfoi_chan_setup(int, short, double, double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call finished unfoi_chan_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_foi_SLOT_Object::unfoi_run()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call unfoi_run %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unfoi_run()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("foi dynamic_call finished unfoi_run %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

