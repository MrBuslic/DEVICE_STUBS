#include "kpiu_rpc.h"

void RPC_kpiu_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kpiu slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("kpiu slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kpiu slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kpiu_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kpiu signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("kpiu signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kpiu signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kpiu_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kpiu_SLOT_Object>(new RPC_kpiu_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kpiu_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kpiu_SIGNAL_Object>(new RPC_kpiu_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kpiu_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_kpiu_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::string_msg)) {
		SRPCSignalClass::Instance().toLog("string_msg connected");
		emit connect_signal("string_msg(QString)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_msg)) {
		SRPCSignalClass::Instance().toLog("int_msg connected");
		emit connect_signal("int_msg(int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_return_signal)) {
		SRPCSignalClass::Instance().toLog("int_return_signal connected");
		emit connect_signal("int_return_signal(int&)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toLog)) {
		SRPCSignalClass::Instance().toLog("toLog connected");
		emit connect_signal("toLog(QString)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toProtocol)) {
		SRPCSignalClass::Instance().toLog("toProtocol connected");
		emit connect_signal("toProtocol(QString)", true);
	}
}

void RPC_kpiu_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::string_msg)) {
		SRPCSignalClass::Instance().toLog("string_msg disconnected");
		//emit connect_signal("string_msg(QString)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_msg)) {
		SRPCSignalClass::Instance().toLog("int_msg disconnected");
		//emit connect_signal("int_msg(int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_return_signal)) {
		SRPCSignalClass::Instance().toLog("int_return_signal disconnected");
		//emit connect_signal("int_return_signal(int&)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toLog)) {
		SRPCSignalClass::Instance().toLog("toLog disconnected");
		//emit connect_signal("toLog(QString)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toProtocol)) {
		SRPCSignalClass::Instance().toLog("toProtocol disconnected");
		//emit connect_signal("toProtocol(QString)", false);
	}
}

void RPC_kpiu_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("kpiu new signal " + op_name);

			if (op_name == "string_msg(QString)")
			{
				QString _msg;
				tmp_stream >> _msg;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " _msg = "+RPCSignalClass::QVariantToString(_msg));
				emit string_msg(_msg);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpiu signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "int_msg(int)")
			{
				int _msg;
				tmp_stream >> _msg;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " _msg = "+RPCSignalClass::QVariantToString(_msg));
				emit int_msg(_msg);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpiu signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "int_return_signal(int&)")
			{
				int ret_int;
				tmp_stream >> ret_int;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " ret_int = "+RPCSignalClass::QVariantToString(ret_int));
				emit int_return_signal(ret_int);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QVariantList return_list;
				return_list << QVariant(ret_int);
				tmp_stream2 << return_list;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpiu signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "toLog(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toLog(message);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpiu signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "toProtocol(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toProtocol(message);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpiu signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int RPC_kpiu_SLOT_Object::ku_map_channels_setup(int ku_n, short line)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call ku_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ku_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished ku_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::mshm_map_channels_setup(int mshm, short line_m)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(mshm);
	tmp_list << QVariant(line_m);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mshm_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mshm_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished mshm_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::pshm_map_channels_setup(int pshm, short line_p)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(pshm);
	tmp_list << QVariant(line_p);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call pshm_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("pshm_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished pshm_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::unfoi_map_channels_setup(int _n, short _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call unfoi_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unfoi_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished unfoi_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	tmp_list << QVariant(mask_buffer);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call unols_write_data_kf %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_write_data_kf(QVariantList, QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished unols_write_data_kf %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::unols_trigger_imm(int devise)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(devise);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call unols_trigger_imm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_trigger_imm(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished unols_trigger_imm %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_kpiu_SLOT_Object::unols_read_data_kr(QVariantList& data_buffer)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call unols_read_data_kr %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_read_data_kr(QVariantList&)", tmp_list);
	data_buffer = tmp_list.at(0).toList();
	tmp_ret_params += " data_buffer="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished unols_read_data_kr %1").arg(tmp_ret_params));
}
void RPC_kpiu_SLOT_Object::set_antenna_connection(QString antenna_name, QString connected_antenna_name)
{
	QVariantList tmp_list;
	tmp_list << QVariant(antenna_name);
	tmp_list << QVariant(connected_antenna_name);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call set_antenna_connection %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_antenna_connection(QString, QString)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpiu dynamic_call finished set_antenna_connection");
}
int RPC_kpiu_SLOT_Object::unomnibus_map_channels_setup(int _n, short _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call unomnibus_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unomnibus_map_channels_setup(int, short)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished unomnibus_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_kpiu_SLOT_Object::set_bus_state(int bus, int state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call set_bus_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_bus_state(int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpiu dynamic_call finished set_bus_state");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

