#include "mfsk24_rpc.h"

void RPC_mfsk24_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mfsk24_SLOT_Object>(new RPC_mfsk24_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mfsk24_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mfsk24_SIGNAL_Object>(new RPC_mfsk24_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mfsk24_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_mfsk24_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_)) {
		SRPCSignalClass::Instance().toLog("mfsk24_ connected");
		emit connect_signal("mfsk24_()", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_state_change)) {
		SRPCSignalClass::Instance().toLog("mfsk24_state_change connected");
		emit connect_signal("mfsk24_state_change(int, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_impulse_change)) {
		SRPCSignalClass::Instance().toLog("mfsk24_impulse_change connected");
		emit connect_signal("mfsk24_impulse_change(QVariantList)", true);
	}
}

void RPC_mfsk24_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_)) {
		SRPCSignalClass::Instance().toLog("mfsk24_ disconnected");
		//emit connect_signal("mfsk24_()", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_state_change)) {
		SRPCSignalClass::Instance().toLog("mfsk24_state_change disconnected");
		//emit connect_signal("mfsk24_state_change(int, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_mfsk24_SIGNAL_Object::mfsk24_impulse_change)) {
		SRPCSignalClass::Instance().toLog("mfsk24_impulse_change disconnected");
		//emit connect_signal("mfsk24_impulse_change(QVariantList)", false);
	}
}

void RPC_mfsk24_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mfsk24 new signal " + op_name);

			if (op_name == "mfsk24_()")
			{
				emit mfsk24_();
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mfsk24 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "mfsk24_state_change(int, int)")
			{
				int channel;
				tmp_stream >> channel;
				SRPCSignalClass::Instance().toLog("mfsk24 " + op_name +" call_number "+ QString::number(call_number) + " channel = "+RPCSignalClass::QVariantToString(channel));
				int state;
				tmp_stream >> state;
				SRPCSignalClass::Instance().toLog("mfsk24 " + op_name +" call_number "+ QString::number(call_number) + " state = "+RPCSignalClass::QVariantToString(state));
				emit mfsk24_state_change(channel, state);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mfsk24 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "mfsk24_impulse_change(QVariantList)")
			{
				QVariantList channels;
				tmp_stream >> channels;
				SRPCSignalClass::Instance().toLog("mfsk24 " + op_name +" call_number "+ QString::number(call_number) + " channels = "+RPCSignalClass::QVariantToString(channels));
				emit mfsk24_impulse_change(channels);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mfsk24 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_mfsk24_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mfsk24 dynamic_call finished auto_scroll_clicked");
}
void RPC_mfsk24_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mfsk24 dynamic_call finished log_timer_ontimer");
}
int RPC_mfsk24_SLOT_Object::unmfsk24_manual_group_cmd(int _state_chan, QVariantList _vec)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_state_chan);
	tmp_list << QVariant(_vec);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call unmfsk24_manual_group_cmd %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmfsk24_manual_group_cmd(int, QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call finished unmfsk24_manual_group_cmd %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mfsk24_SLOT_Object::unmfsk24_state(QVariantList& _state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call unmfsk24_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmfsk24_state(QVariantList&)", tmp_list);
	_state = tmp_list.at(0).toList();
	tmp_ret_params += " _state="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call finished unmfsk24_state %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mfsk24_SLOT_Object::unmfsk24_manual_cmd(int _chan, int _state_chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_chan);
	tmp_list << QVariant(_state_chan);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call unmfsk24_manual_cmd %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmfsk24_manual_cmd(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call finished unmfsk24_manual_cmd %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mfsk24_SLOT_Object::unmfsk24_start(QVariantList _state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call unmfsk24_start %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmfsk24_start(QVariantList)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call finished unmfsk24_start %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mfsk24_SLOT_Object::unmfsk24_set_cmd_time(int _chan, int _time)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_chan);
	tmp_list << QVariant(_time);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call unmfsk24_set_cmd_time %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmfsk24_set_cmd_time(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mfsk24 dynamic_call finished unmfsk24_set_cmd_time %1").arg(tmp_ret_params));
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

