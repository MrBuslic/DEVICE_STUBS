#include "ols_rpc.h"

void RPC_ols_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ols_SLOT_Object>(new RPC_ols_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ols_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_ols_SIGNAL_Object>(new RPC_ols_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_ols_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::receive_data)) {
		connect_signal("receive_data(QVariantList&)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::new_ols_data)) {
		connect_signal("new_ols_data(QVariantList, QVariantList)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::packet_ready)) {
		connect_signal("packet_ready(QVariantList)", true);
	}
}

void RPC_ols_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::receive_data)) {
		connect_signal("receive_data(QVariantList&)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::new_ols_data)) {
		connect_signal("new_ols_data(QVariantList, QVariantList)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_ols_SIGNAL_Object::packet_ready)) {
		connect_signal("packet_ready(QVariantList)", false);
	}
}

void RPC_ols_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("ols new signal " + op_name);

			if (op_name == "receive_data(QVariantList&)")
			{
				QVariantList data_buffer;
				tmp_stream >> data_buffer;
				SRPCSignalClass::Instance().toLog("ols " + op_name +" call_number "+ QString::number(call_number) + " data_buffer = "+RPCSignalClass::QVariantToString(data_buffer));
				emit receive_data(data_buffer);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QVariantList return_list;
				return_list << QVariant(data_buffer);
				tmp_stream2 << return_list;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("ols signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_ols_data(QVariantList, QVariantList)")
			{
				QVariantList data_buffer;
				tmp_stream >> data_buffer;
				SRPCSignalClass::Instance().toLog("ols " + op_name +" call_number "+ QString::number(call_number) + " data_buffer = "+RPCSignalClass::QVariantToString(data_buffer));
				QVariantList mask_buffer;
				tmp_stream >> mask_buffer;
				SRPCSignalClass::Instance().toLog("ols " + op_name +" call_number "+ QString::number(call_number) + " mask_buffer = "+RPCSignalClass::QVariantToString(mask_buffer));
				emit new_ols_data(data_buffer, mask_buffer);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("ols signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "packet_ready(QVariantList)")
			{
				QVariantList data_buffer;
				tmp_stream >> data_buffer;
				SRPCSignalClass::Instance().toLog("ols " + op_name +" call_number "+ QString::number(call_number) + " data_buffer = "+RPCSignalClass::QVariantToString(data_buffer));
				emit packet_ready(data_buffer);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("ols signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

int RPC_ols_SLOT_Object::unols_write_data_kf(QVariantList data_buffer, QVariantList mask_buffer)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	tmp_list << QVariant(mask_buffer);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call unols_write_data_kf %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_write_data_kf(QVariantList, QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call finished unols_write_data_kf %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_ols_SLOT_Object::unols_trigger_imm(int devise)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(devise);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call unols_trigger_imm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_trigger_imm(int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call finished unols_trigger_imm %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_ols_SLOT_Object::unols_read_data_kr(QVariantList& data_buffer)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(data_buffer);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call unols_read_data_kr %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_read_data_kr(QVariantList&)", tmp_list);
	data_buffer = tmp_list.at(0).toList();
	tmp_ret_params += " data_buffer="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call finished unols_read_data_kr %1").arg(tmp_ret_params));
}
int RPC_ols_SLOT_Object::unols_mStart()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call unols_mStart %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unols_mStart()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("ols dynamic_call finished unols_mStart %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

