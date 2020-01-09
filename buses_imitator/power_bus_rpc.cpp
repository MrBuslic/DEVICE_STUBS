#include "power_bus_rpc.h"

void RPC_power_bus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_power_bus_SLOT_Object>(new RPC_power_bus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_power_bus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_power_bus_SIGNAL_Object>(new RPC_power_bus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_power_bus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_nk)) {
		connect_signal("u_on_nk(double)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_k1)) {
		connect_signal("u_on_k1(double)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_k2)) {
		connect_signal("u_on_k2(double)", true);
	}
}

void RPC_power_bus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_nk)) {
		connect_signal("u_on_nk(double)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_k1)) {
		connect_signal("u_on_k1(double)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_power_bus_SIGNAL_Object::u_on_k2)) {
		connect_signal("u_on_k2(double)", false);
	}
}

void RPC_power_bus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("power_bus new signal " + op_name);

			if (op_name == "u_on_nk(double)")
			{
				double volt;
				tmp_stream >> volt;
				SRPCSignalClass::Instance().toLog("power_bus " + op_name +" call_number "+ QString::number(call_number) + " volt = "+RPCSignalClass::QVariantToString(volt));
				emit u_on_nk(volt);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("power_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "u_on_k1(double)")
			{
				double volt;
				tmp_stream >> volt;
				SRPCSignalClass::Instance().toLog("power_bus " + op_name +" call_number "+ QString::number(call_number) + " volt = "+RPCSignalClass::QVariantToString(volt));
				emit u_on_k1(volt);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("power_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "u_on_k2(double)")
			{
				double volt;
				tmp_stream >> volt;
				SRPCSignalClass::Instance().toLog("power_bus " + op_name +" call_number "+ QString::number(call_number) + " volt = "+RPCSignalClass::QVariantToString(volt));
				emit u_on_k2(volt);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("power_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_power_bus_SLOT_Object::set_u(int bus, double volt)
{
	QVariantList tmp_list;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(volt);
	SRPCSignalClass::Instance().toLog(QString("power_bus dynamic_call set_u %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_u(int, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("power_bus dynamic_call finished set_u");
}
void RPC_power_bus_SLOT_Object::get_i(int bus, double& curr)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(curr);
	SRPCSignalClass::Instance().toLog(QString("power_bus dynamic_call get_i %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_i(int, double&)", tmp_list);
	curr = tmp_list.at(1).toDouble();
	tmp_ret_params += " curr="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("power_bus dynamic_call finished get_i %1").arg(tmp_ret_params));
}
void RPC_power_bus_SLOT_Object::set_i(int bus, QString name, double curr)
{
	QVariantList tmp_list;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(name);
	tmp_list << QVariant(curr);
	SRPCSignalClass::Instance().toLog(QString("power_bus dynamic_call set_i %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_i(int, QString, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("power_bus dynamic_call finished set_i");
}
void RPC_power_bus_SLOT_Object::set_bus_state(int bus, int state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("power_bus dynamic_call set_bus_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_bus_state(int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("power_bus dynamic_call finished set_bus_state");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

