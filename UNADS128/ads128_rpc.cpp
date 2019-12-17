#include "ads128_rpc.h"

void RPC_ads128_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ads128_SLOT_Object>(new RPC_ads128_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ads128_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ads128_SIGNAL_Object>(new RPC_ads128_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ads128_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_ads128_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_ads128_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("ads128 new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_ads128_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("ads128 dynamic_call finished auto_scroll_clicked");
}
void RPC_ads128_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("ads128 dynamic_call finished log_timer_ontimer");
}
void RPC_ads128_SLOT_Object::ads_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("ads128 dynamic_call finished ads_timer_ontimer");
}
void RPC_ads128_SLOT_Object::new_ku(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call new_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("ads128 dynamic_call finished new_ku");
}
void RPC_ads128_SLOT_Object::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
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
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call new_mk %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_mk(int, int, int, int, double, double, int, int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("ads128 dynamic_call finished new_mk");
}
int RPC_ads128_SLOT_Object::ads128_conf_analog(uint group, double level_0, double level_1)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(group);
	tmp_list << QVariant(level_0);
	tmp_list << QVariant(level_1);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads128_conf_analog %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads128_conf_analog(uint, double, double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call finished ads128_conf_analog %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_ads128_SLOT_Object::ads128_start()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads128_start %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads128_start()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call finished ads128_start %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_ads128_SLOT_Object::ads128_read_data(QVariantList& thisbuf, QVariantList& firstbuf)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(thisbuf);
	tmp_list << QVariant(firstbuf);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads128_read_data %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads128_read_data(QVariantList&, QVariantList&)", tmp_list);
	thisbuf = tmp_list.at(0).toList();
	tmp_ret_params += " thisbuf="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	firstbuf = tmp_list.at(1).toList();
	tmp_ret_params += " firstbuf="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call finished ads128_read_data %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_ads128_SLOT_Object::ads128_stop()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads128_stop %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads128_stop()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call finished ads128_stop %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_ads128_SLOT_Object::ads128_analog_q(uint group_, double& lev0, double& lev1)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(group_);
	tmp_list << QVariant(lev0);
	tmp_list << QVariant(lev1);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call ads128_analog_q %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ads128_analog_q(uint, double&, double&)", tmp_list);
	lev0 = tmp_list.at(1).toDouble();
	tmp_ret_params += " lev0="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	lev1 = tmp_list.at(2).toDouble();
	tmp_ret_params += " lev1="+RPCSignalClass::QVariantToString(tmp_list.at(2));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ads128 dynamic_call finished ads128_analog_q %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

