#include "kpiu_rpc.h"

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

void RPC_kpiu_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::string_msg)) {
		connect_signal("string_msg(QString)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_msg)) {
		connect_signal("int_msg(int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_return_signal)) {
		connect_signal("int_return_signal(int&)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toLogs)) {
		connect_signal("toLogs(QString)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toProtocols)) {
		connect_signal("toProtocols(QString)", true);
	}
}

void RPC_kpiu_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::string_msg)) {
		connect_signal("string_msg(QString)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_msg)) {
		connect_signal("int_msg(int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::int_return_signal)) {
		connect_signal("int_return_signal(int&)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toLogs)) {
		connect_signal("toLogs(QString)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kpiu_SIGNAL_Object::toProtocols)) {
		connect_signal("toProtocols(QString)", false);
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
			if (op_name == "toLogs(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toLogs(message);
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
			if (op_name == "toProtocols(QString)")
			{
				QString message;
				tmp_stream >> message;
				SRPCSignalClass::Instance().toLog("kpiu " + op_name +" call_number "+ QString::number(call_number) + " message = "+RPCSignalClass::QVariantToString(message));
				emit toProtocols(message);
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

int RPC_kpiu_SLOT_Object::KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call KU_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("KU_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished KU_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(mshm);
	tmp_list << QVariant(line_m);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call MSHM_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished MSHM_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(pshm);
	tmp_list << QVariant(line_p);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call PSHM_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished PSHM_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call FOI_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished FOI_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList data_buffer, QVariantList mask_buffer)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	tmp_list << QVariant(mask_buffer);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call OLS_ZAPIS_DANNIH_FORMIROVANIYA %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList, QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished OLS_ZAPIS_DANNIH_FORMIROVANIYA %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList data_buffer)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call OLS_ZAPIS_DANNIH_REGISTRACII %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished OLS_ZAPIS_DANNIH_REGISTRACII %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::OLS_BISTRIY_START(int devise)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(devise);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call OLS_BISTRIY_START %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OLS_BISTRIY_START(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished OLS_BISTRIY_START %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& data_buffer)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call OLS_CHTENIE_DANNICH_REGISTRACII %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList&)", tmp_list);
	data_buffer = tmp_list.at(0).toList();
	tmp_ret_params += " data_buffer="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished OLS_CHTENIE_DANNICH_REGISTRACII %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(antenna_name);
	tmp_list << QVariant(connected_antenna_name);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call ANTENNA_USTANOVKA_KOMMUTACII %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("ANTENNA_USTANOVKA_KOMMUTACII(QString, QString)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished ANTENNA_USTANOVKA_KOMMUTACII %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(bus);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call USTANOVIT_SOSTOYANIE_SHINI_PITANIYA %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished USTANOVIT_SOSTOYANIE_SHINI_PITANIYA %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_kpiu_SLOT_Object::PYRO_USTANOVIT_SOSTOYANIE(QString name, int state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(name);
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call PYRO_USTANOVIT_SOSTOYANIE %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("PYRO_USTANOVIT_SOSTOYANIE(QString, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished PYRO_USTANOVIT_SOSTOYANIE %1").arg(tmp_ret_params));
	return res.toInt();
}
QString RPC_kpiu_SLOT_Object::getXML()
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call getXML %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("getXML()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished getXML %1").arg(tmp_ret_params));
	return res.toString();
}
void RPC_kpiu_SLOT_Object::mfsk_2_impulse(QVariantList channels)
{
	QVariantList tmp_list;
	tmp_list << QVariant(channels);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mfsk_2_impulse %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mfsk_2_impulse(QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpiu dynamic_call finished mfsk_2_impulse");
}
void RPC_kpiu_SLOT_Object::mds_1_get_sample(uint& buf, bool& flag)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(buf);
	tmp_list << QVariant(flag);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mds_1_get_sample %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mds_1_get_sample(uint&, bool&)", tmp_list);
	buf = tmp_list.at(0).toUInt();
	tmp_ret_params += " buf="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	flag = tmp_list.at(1).toBool();
	tmp_ret_params += " flag="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished mds_1_get_sample %1").arg(tmp_ret_params));
}
void RPC_kpiu_SLOT_Object::mds_2_get_sample(uint& buf, bool& flag)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(buf);
	tmp_list << QVariant(flag);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mds_2_get_sample %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mds_2_get_sample(uint&, bool&)", tmp_list);
	buf = tmp_list.at(0).toUInt();
	tmp_ret_params += " buf="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	flag = tmp_list.at(1).toBool();
	tmp_ret_params += " flag="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished mds_2_get_sample %1").arg(tmp_ret_params));
}
void RPC_kpiu_SLOT_Object::get_resistance(uint NProcess, QVariant& resistance)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(NProcess);
	tmp_list << QVariant(resistance);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call get_resistance %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_resistance(uint, QVariant&)", tmp_list);
	resistance = tmp_list.at(1);
	tmp_ret_params += " resistance="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished get_resistance %1").arg(tmp_ret_params));
}
int RPC_kpiu_SLOT_Object::get_connection_state(QVariantList& _chans)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_chans);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call get_connection_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_connection_state(QVariantList&)", tmp_list);
	_chans = tmp_list.at(0).toList();
	tmp_ret_params += " _chans="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished get_connection_state %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_kpiu_SLOT_Object::mshr_data_in(QVariantList dataList, QVariantList maskList)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dataList);
	tmp_list << QVariant(maskList);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mshr_data_in %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mshr_data_in(QVariantList, QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpiu dynamic_call finished mshr_data_in");
}
void RPC_kpiu_SLOT_Object::mshr_data_out(QVariantList& data)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call mshr_data_out %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("mshr_data_out(QVariantList&)", tmp_list);
	data = tmp_list.at(0).toList();
	tmp_ret_params += " data="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call finished mshr_data_out %1").arg(tmp_ret_params));
}
void RPC_kpiu_SLOT_Object::power_bus_state_changed(QString name, double u)
{
	QVariantList tmp_list;
	tmp_list << QVariant(name);
	tmp_list << QVariant(u);
	SRPCSignalClass::Instance().toLog(QString("kpiu dynamic_call power_bus_state_changed %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("power_bus_state_changed(QString, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpiu dynamic_call finished power_bus_state_changed");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

