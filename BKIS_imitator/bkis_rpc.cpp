#include "bkis_rpc.h"

void RPC_bkis_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_bkis_SLOT_Object>(new RPC_bkis_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_bkis_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_bkis_SIGNAL_Object>(new RPC_bkis_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_bkis_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_bkis_SIGNAL_Object::send_pyro_group_activation)) {
		connect_signal("send_pyro_group_activation(int)", true);
	}
}

void RPC_bkis_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_bkis_SIGNAL_Object::send_pyro_group_activation)) {
		connect_signal("send_pyro_group_activation(int)", false);
	}
}

void RPC_bkis_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("bkis new signal " + op_name);

			if (op_name == "send_pyro_group_activation(int)")
			{
				int group_num;
				tmp_stream >> group_num;
				SRPCSignalClass::Instance().toLog("bkis " + op_name +" call_number "+ QString::number(call_number) + " group_num = "+RPCSignalClass::QVariantToString(group_num));
				emit send_pyro_group_activation(group_num);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("bkis signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_bkis_SLOT_Object::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished new_message");
}
void RPC_bkis_SLOT_Object::make_ku(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call make_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_ku(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished make_ku");
}
void RPC_bkis_SLOT_Object::get_power(double _volt)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_volt);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call get_power %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_power(double)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished get_power");
}
void RPC_bkis_SLOT_Object::set_blk_state(int state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_blk_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_blk_state(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished set_blk_state");
}
void RPC_bkis_SLOT_Object::set_interface_state(bool state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_interface_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_interface_state(bool)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished set_interface_state");
}
int RPC_bkis_SLOT_Object::set_electric_heater_state(int name, bool state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(name);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_electric_heater_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_electric_heater_state(int, bool)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call finished set_electric_heater_state %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_bkis_SLOT_Object::set_pyro_group_state(int group_num, bool state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(group_num);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_pyro_group_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_pyro_group_state(int, bool)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call finished set_pyro_group_state %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_bkis_SLOT_Object::set_power_back()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_power_back %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_power_back()", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished set_power_back");
}
void RPC_bkis_SLOT_Object::omni_connect()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call omni_connect %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("omni_connect()", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished omni_connect");
}
void RPC_bkis_SLOT_Object::set_pyro_bus_state(int bus_num)
{
	QVariantList tmp_list;
	tmp_list << QVariant(bus_num);
	SRPCSignalClass::Instance().toLog(QString("bkis dynamic_call set_pyro_bus_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_pyro_bus_state(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("bkis dynamic_call finished set_pyro_bus_state");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

