#include "is4_rpc.h"

void RPC_is4_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_is4_SLOT_Object>(new RPC_is4_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_is4_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_is4_SIGNAL_Object>(new RPC_is4_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_is4_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_is4_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
}

void RPC_is4_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
}

void RPC_is4_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("is4 new signal " + op_name);

		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_is4_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("is4 dynamic_call finished log_timer_ontimer");
}
void RPC_is4_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("is4 dynamic_call finished auto_scroll_clicked");
}
void RPC_is4_SLOT_Object::is4_clicked(int state_is4)
{
	QVariantList tmp_list;
	tmp_list << QVariant(state_is4);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call is4_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("is4_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("is4 dynamic_call finished is4_clicked");
}
void RPC_is4_SLOT_Object::measure(uint NProcess, QVariant& value)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(NProcess);
	tmp_list << QVariant(value);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call measure %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("measure(uint, QVariant&)", tmp_list);
	value = tmp_list.at(1);
	tmp_ret_params += " value="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished measure %1").arg(tmp_ret_params));
}
int RPC_is4_SLOT_Object::unis4_SetTypeProcess(uint EProcess)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(EProcess);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call unis4_SetTypeProcess %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unis4_SetTypeProcess(uint)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished unis4_SetTypeProcess %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_is4_SLOT_Object::unis4_StartACP()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call unis4_StartACP %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unis4_StartACP()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished unis4_StartACP %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_is4_SLOT_Object::unis4_ResultMeas(double& ResMeas, uint& NumRes)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(ResMeas);
	tmp_list << QVariant(NumRes);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call unis4_ResultMeas %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unis4_ResultMeas(double&, uint&)", tmp_list);
	ResMeas = tmp_list.at(0).toDouble();
	tmp_ret_params += " ResMeas="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	NumRes = tmp_list.at(1).toUInt();
	tmp_ret_params += " NumRes="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished unis4_ResultMeas %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_is4_SLOT_Object::unis4_RangeMeas(uint range)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(range);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call unis4_RangeMeas %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unis4_RangeMeas(uint)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished unis4_RangeMeas %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_is4_SLOT_Object::unis4_StartCalibr()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call unis4_StartCalibr %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unis4_StartCalibr()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("is4 dynamic_call finished unis4_StartCalibr %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

