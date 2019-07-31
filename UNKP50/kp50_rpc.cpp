#include "kp50_rpc.h"

void RPC_kp50_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kp50_SLOT_Object>(new RPC_kp50_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kp50_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kp50_SIGNAL_Object>(new RPC_kp50_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kp50_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_kp50_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_kp50_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_kp50_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("kp50 new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_kp50_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("kp50 dynamic_call finished auto_scroll_clicked");
}
void RPC_kp50_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("kp50 dynamic_call finished log_timer_ontimer");
}
void RPC_kp50_SLOT_Object::set_u_in(double _u)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_u);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call set_u_in %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_u_in(double)", tmp_list);
	SRPCSignalClass::Instance().toLog("kp50 dynamic_call finished set_u_in");
}
int RPC_kp50_SLOT_Object::unkp50_switch_channel(int n, bool on)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(n);
	tmp_list << QVariant(on);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call unkp50_switch_channel %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unkp50_switch_channel(int, bool)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call finished unkp50_switch_channel %1").arg(tmp_ret_params));
	return res.toInt();
}
bool RPC_kp50_SLOT_Object::unkp50_channel_state_Q(int n)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(n);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call unkp50_channel_state_Q %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unkp50_channel_state_Q(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call finished unkp50_channel_state_Q %1").arg(tmp_ret_params));
	return res.toBool();
}
double RPC_kp50_SLOT_Object::unkp50_meas_I(int n)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(n);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call unkp50_meas_I %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unkp50_meas_I(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call finished unkp50_meas_I %1").arg(tmp_ret_params));
	return res.toDouble();
}
double RPC_kp50_SLOT_Object::unkp50_meas_Uin(int n)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(n);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call unkp50_meas_Uin %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unkp50_meas_Uin(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call finished unkp50_meas_Uin %1").arg(tmp_ret_params));
	return res.toDouble();
}
double RPC_kp50_SLOT_Object::unkp50_meas_Uout(int n)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(n);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call unkp50_meas_Uout %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unkp50_meas_Uout(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kp50 dynamic_call finished unkp50_meas_Uout %1").arg(tmp_ret_params));
	return res.toDouble();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

