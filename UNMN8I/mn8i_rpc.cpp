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
				SRPCSignalClass::Instance().toLog("mn8i signal finished " + op_name);
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
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call auto_scroll_clicked");
	dynamic_call("auto_scroll_clicked(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished auto_scroll_clicked");
}
void RPC_mn8i_SLOT_Object::log_timer_ontimer()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call log_timer_ontimer");
	dynamic_call("log_timer_ontimer()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished log_timer_ontimer");
}
int RPC_mn8i_SLOT_Object::unmn8i_start()
{
	if(!connected) return 1;
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call unmn8i_start");
	dynamic_call("unmn8i_start()", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished unmn8i_start");
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_input_trigger(bool state)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(state);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call unmn8i_input_trigger");
	dynamic_call("unmn8i_input_trigger(bool)", tmp_list);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished unmn8i_input_trigger");
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_sample_width_q(uint& frame_width, uint& width_in_bytes)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(frame_width);
	tmp_list << QVariant(width_in_bytes);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call unmn8i_sample_width_q");
	dynamic_call("unmn8i_sample_width_q(uint&, uint&)", tmp_list);
	frame_width = tmp_list.at(0).toUInt();
	width_in_bytes = tmp_list.at(1).toUInt();
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished unmn8i_sample_width_q");
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_read_sample(uint& _buf, uint& _firstTime, uint& _thisTime)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(_buf);
	tmp_list << QVariant(_firstTime);
	tmp_list << QVariant(_thisTime);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call unmn8i_read_sample");
	dynamic_call("unmn8i_read_sample(uint&, uint&, uint&)", tmp_list);
	_buf = tmp_list.at(0).toUInt();
	_firstTime = tmp_list.at(1).toUInt();
	_thisTime = tmp_list.at(2).toUInt();
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished unmn8i_read_sample");
	return res.toInt();
}
int RPC_mn8i_SLOT_Object::unmn8i_read_packet(bool isHot, uint numSamples, QVariantList& buf, uint& realNumSamples)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	tmp_list << QVariant(isHot);
	tmp_list << QVariant(numSamples);
	tmp_list << QVariant(buf);
	tmp_list << QVariant(realNumSamples);
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call unmn8i_read_packet");
	dynamic_call("unmn8i_read_packet(bool, uint, QVariantList&, uint&)", tmp_list);
	buf = tmp_list.at(2).toList();
	realNumSamples = tmp_list.at(3).toUInt();
	SRPCSignalClass::Instance().toLog("mn8i dynamic_call finished unmn8i_read_packet");
	return res.toInt();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

