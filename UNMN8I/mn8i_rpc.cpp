#include "mn8i_rpc.h"

void RPC_mn8i_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mn8i slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("mn8i slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mn8i slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mn8i_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("mn8i signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("mn8i signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("mn8i signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_mn8i_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mn8i_SLOT_Object>(new RPC_mn8i_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mn8i_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_mn8i_SIGNAL_Object>(new RPC_mn8i_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_mn8i_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_mn8i_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mn8i_SIGNAL_Object::packet_ready)) {
		SRPCSignalClass::Instance().toLog("packet_ready connected");
		emit connect_signal("packet_ready()", true);
	}
}

void RPC_mn8i_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_mn8i_SIGNAL_Object::packet_ready)) {
		SRPCSignalClass::Instance().toLog("packet_ready disconnected");
		//emit connect_signal("packet_ready()", false);
	}
}

void RPC_mn8i_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("mn8i new signal " + op_name);

			if (op_name == "packet_ready()")
			{
				emit packet_ready();
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("mn8i signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_mn8i_SLOT_Object::auto_scroll_clicked(int _state)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_state);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call auto_scroll_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished auto_scroll_clicked");
}
void RPC_mn8i_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call log_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished log_timer_ontimer");
}
void RPC_mn8i_SLOT_Object::measurement_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call measurement_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("measurement_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished measurement_timer_ontimer");
}
void RPC_mn8i_SLOT_Object::infin_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call infin_timer_ontimer %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("infin_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished infin_timer_ontimer");
}
void RPC_mn8i_SLOT_Object::new_ku(int ku_n, int length, double u)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call new_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku(int, int, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished new_ku");
}
void RPC_mn8i_SLOT_Object::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mshm);
	tmp_list << QVariant(pshm);
	tmp_list << QVariant(length_m);
	tmp_list << QVariant(length_p);
	tmp_list << QVariant(u_m);
	tmp_list << QVariant(u_p);
	tmp_list << QVariant(dt);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call new_mk %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_mk(int, int, int, int, double, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished new_mk");
}
int RPC_mn8i_SLOT_Object::unmn8i_start()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_start %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_start()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_start %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_input_trigger(bool state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_input_trigger %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_input_trigger(bool)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_input_trigger %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_sample_width_q(uint& frame_width, uint& width_in_bytes)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(frame_width);
	tmp_list << QVariant(width_in_bytes);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_sample_width_q %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_sample_width_q(uint&, uint&)", tmp_list);
	frame_width = tmp_list.at(0).toUInt();
	tmp_ret_params += " frame_width="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	width_in_bytes = tmp_list.at(1).toUInt();
	tmp_ret_params += " width_in_bytes="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_sample_width_q %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_buf);
	tmp_list << QVariant(_firstTime);
	tmp_list << QVariant(_thisTime);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_read_sample %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_read_sample(uint&, uint&, uint&)", tmp_list);
	_buf = tmp_list.at(0).toUInt();
	tmp_ret_params += " _buf="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	_firstTime = tmp_list.at(1).toUInt();
	tmp_ret_params += " _firstTime="+RPCSignalClass::QVariantToString(tmp_list.at(1));
	_thisTime = tmp_list.at(2).toUInt();
	tmp_ret_params += " _thisTime="+RPCSignalClass::QVariantToString(tmp_list.at(2));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_read_sample %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(isHot);
	tmp_list << QVariant(numSamples);
	tmp_list << QVariant(buf);
	tmp_list << QVariant(realNumSamples);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_read_packet %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_read_packet(bool, uint, QVariantList&, uint&)", tmp_list);
	buf = tmp_list.at(2).toList();
	tmp_ret_params += " buf="+RPCSignalClass::QVariantToString(tmp_list.at(2));
	realNumSamples = tmp_list.at(3).toUInt();
	tmp_ret_params += " realNumSamples="+RPCSignalClass::QVariantToString(tmp_list.at(3));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_read_packet %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_sample_period(double _periodS)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_periodS);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_sample_period %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_sample_period(double)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_sample_period %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_mode_cycle(uint _size)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_size);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_mode_cycle %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_mode_cycle(uint)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_mode_cycle %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_num_ready_data(uint& _num)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_num);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_num_ready_data %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_num_ready_data(uint&)", tmp_list);
	_num = tmp_list.at(0).toUInt();
	tmp_ret_params += " _num="+RPCSignalClass::QVariantToString(tmp_list.at(0));
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_num_ready_data %1").arg(tmp_ret_params));
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_stop()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call unmn8i_stop %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unmn8i_stop()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call finished unmn8i_stop %1").arg(tmp_ret_params));
	return res.toInt();
}
void RPC_mn8i_SLOT_Object::button_clicked()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("mn8i dynamic_call button_clicked %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("button_clicked()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished button_clicked");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

