#include "mku_bus_rpc.h"

void RPC_mku_bus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mku_bus_SLOT_Object>(new RPC_mku_bus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mku_bus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mku_bus_SIGNAL_Object>(new RPC_mku_bus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mku_bus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku_732)) {
		connect_signal("new_ku_732(int, int, double, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku_cbk)) {
		connect_signal("new_ku_cbk(int, int, double, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_mk)) {
		connect_signal("new_mk(int, int, int, int, double, double, int, int, int)", true);
	}
}

void RPC_mku_bus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku_732)) {
		connect_signal("new_ku_732(int, int, double, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_ku_cbk)) {
		connect_signal("new_ku_cbk(int, int, double, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mku_bus_SIGNAL_Object::new_mk)) {
		connect_signal("new_mk(int, int, int, int, double, double, int, int, int)", false);
	}
}

void RPC_mku_bus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mku_bus new signal " + op_name);

			if (op_name == "new_ku(int, int, double, int)")
			{
				int ku_n;
				tmp_stream >> ku_n;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " ku_n = "+RPCSignalClass::QVariantToString(ku_n));
				int length;
				tmp_stream >> length;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " length = "+RPCSignalClass::QVariantToString(length));
				double u;
				tmp_stream >> u;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " u = "+RPCSignalClass::QVariantToString(u));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				emit new_ku(ku_n, length, u, line);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mku_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_ku_732(int, int, double, int)")
			{
				int ku_n;
				tmp_stream >> ku_n;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " ku_n = "+RPCSignalClass::QVariantToString(ku_n));
				int length;
				tmp_stream >> length;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " length = "+RPCSignalClass::QVariantToString(length));
				double u;
				tmp_stream >> u;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " u = "+RPCSignalClass::QVariantToString(u));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				emit new_ku_732(ku_n, length, u, line);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mku_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_ku_cbk(int, int, double, int)")
			{
				int ku_n;
				tmp_stream >> ku_n;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " ku_n = "+RPCSignalClass::QVariantToString(ku_n));
				int length;
				tmp_stream >> length;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " length = "+RPCSignalClass::QVariantToString(length));
				double u;
				tmp_stream >> u;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " u = "+RPCSignalClass::QVariantToString(u));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				emit new_ku_cbk(ku_n, length, u, line);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mku_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_mk(int, int, int, int, double, double, int, int, int)")
			{
				int mshm;
				tmp_stream >> mshm;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " mshm = "+RPCSignalClass::QVariantToString(mshm));
				int pshm;
				tmp_stream >> pshm;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " pshm = "+RPCSignalClass::QVariantToString(pshm));
				int length_m;
				tmp_stream >> length_m;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " length_m = "+RPCSignalClass::QVariantToString(length_m));
				int length_p;
				tmp_stream >> length_p;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " length_p = "+RPCSignalClass::QVariantToString(length_p));
				double u_m;
				tmp_stream >> u_m;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " u_m = "+RPCSignalClass::QVariantToString(u_m));
				double u_p;
				tmp_stream >> u_p;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " u_p = "+RPCSignalClass::QVariantToString(u_p));
				int dt;
				tmp_stream >> dt;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " dt = "+RPCSignalClass::QVariantToString(dt));
				int line_m;
				tmp_stream >> line_m;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " line_m = "+RPCSignalClass::QVariantToString(line_m));
				int line_p;
				tmp_stream >> line_p;
				SRPCSignalClass::Instance().toLog("mku_bus " + op_name +" call_number "+ QString::number(call_number) + " line_p = "+RPCSignalClass::QVariantToString(line_p));
				emit new_mk(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mku_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_mku_bus_SLOT_Object::make_ku(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call make_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_ku(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mku_bus dynamic_call finished make_ku");
}
void RPC_mku_bus_SLOT_Object::make_ku_732(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call make_ku_732 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_ku_732(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mku_bus dynamic_call finished make_ku_732");
}
void RPC_mku_bus_SLOT_Object::make_ku_cbk(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call make_ku_cbk %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_ku_cbk(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mku_bus dynamic_call finished make_ku_cbk");
}
void RPC_mku_bus_SLOT_Object::make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
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
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call make_mk %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_mk(int, int, int, int, double, double, int, int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mku_bus dynamic_call finished make_mk");
}
int RPC_mku_bus_SLOT_Object::ku_map_channels_setup(int ku_n, int line)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call ku_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ku_map_channels_setup(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call finished ku_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mku_bus_SLOT_Object::mshm_map_channels_setup(int mshm, int line_m)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(mshm);
	tmp_list << QVariant(line_m);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call mshm_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mshm_map_channels_setup(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call finished mshm_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mku_bus_SLOT_Object::pshm_map_channels_setup(int pshm, int line_p)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(pshm);
	tmp_list << QVariant(line_p);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call pshm_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("pshm_map_channels_setup(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call finished pshm_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_mku_bus_SLOT_Object::get_tm(QString tm_name, QVariant& tm_val)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(tm_name);
	tmp_list << QVariant(tm_val);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call get_tm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_tm(QString, QVariant&)", tmp_list);
	tm_val = tmp_list.at(1);
	tmp_ret_params += " tm_val="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call finished get_tm %1").arg(tmp_ret_params));
}
void RPC_mku_bus_SLOT_Object::set_tm(QString tm_name, QVariant tm_val)
{
	QVariantList tmp_list;
	tmp_list << QVariant(tm_name);
	tmp_list << QVariant(tm_val);
	SRPCSignalClass::Instance().toLog(QString("mku_bus dynamic_call set_tm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_tm(QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("mku_bus dynamic_call finished set_tm");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

