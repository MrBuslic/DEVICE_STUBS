#include "frame_bus_rpc.h"

void RPC_frame_bus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_frame_bus_SLOT_Object>(new RPC_frame_bus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_frame_bus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_frame_bus_SIGNAL_Object>(new RPC_frame_bus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_frame_bus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_04)) {
		connect_signal("new_frame_04(QString, QVariant)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_733)) {
		connect_signal("new_frame_733(QString, QVariant)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_07)) {
		connect_signal("new_frame_07(QString, int, int, int, QString, QVariant)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_rm07)) {
		connect_signal("new_frame_rm07(QString, QVariant)", true);
	}
}

void RPC_frame_bus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_04)) {
		connect_signal("new_frame_04(QString, QVariant)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_733)) {
		connect_signal("new_frame_733(QString, QVariant)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_07)) {
		connect_signal("new_frame_07(QString, int, int, int, QString, QVariant)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_frame_bus_SIGNAL_Object::new_frame_rm07)) {
		connect_signal("new_frame_rm07(QString, QVariant)", false);
	}
}

void RPC_frame_bus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("frame_bus new signal " + op_name);

			if (op_name == "new_frame_04(QString, QVariant)")
			{
				QString mode;
				tmp_stream >> mode;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " mode = "+RPCSignalClass::QVariantToString(mode));
				QVariant frame_data;
				tmp_stream >> frame_data;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " frame_data = "+RPCSignalClass::QVariantToString(frame_data));
				emit new_frame_04(mode, frame_data);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("frame_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_frame_733(QString, QVariant)")
			{
				QString mode;
				tmp_stream >> mode;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " mode = "+RPCSignalClass::QVariantToString(mode));
				QVariant frame_data;
				tmp_stream >> frame_data;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " frame_data = "+RPCSignalClass::QVariantToString(frame_data));
				emit new_frame_733(mode, frame_data);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("frame_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_frame_07(QString, int, int, int, QString, QVariant)")
			{
				QString mode;
				tmp_stream >> mode;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " mode = "+RPCSignalClass::QVariantToString(mode));
				int psp;
				tmp_stream >> psp;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " psp = "+RPCSignalClass::QVariantToString(psp));
				int lit;
				tmp_stream >> lit;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " lit = "+RPCSignalClass::QVariantToString(lit));
				int fm;
				tmp_stream >> fm;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " fm = "+RPCSignalClass::QVariantToString(fm));
				QString ant;
				tmp_stream >> ant;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " ant = "+RPCSignalClass::QVariantToString(ant));
				QVariant frame_data;
				tmp_stream >> frame_data;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " frame_data = "+RPCSignalClass::QVariantToString(frame_data));
				emit new_frame_07(mode, psp, lit, fm, ant, frame_data);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("frame_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_frame_rm07(QString, QVariant)")
			{
				QString mode;
				tmp_stream >> mode;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " mode = "+RPCSignalClass::QVariantToString(mode));
				QVariant frame_data;
				tmp_stream >> frame_data;
				SRPCSignalClass::Instance().toLog("frame_bus " + op_name +" call_number "+ QString::number(call_number) + " frame_data = "+RPCSignalClass::QVariantToString(frame_data));
				emit new_frame_rm07(mode, frame_data);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("frame_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_frame_bus_SLOT_Object::make_new_frame_04(QString mode, QVariant frame_data)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mode);
	tmp_list << QVariant(frame_data);
	SRPCSignalClass::Instance().toLog(QString("frame_bus dynamic_call make_new_frame_04 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_new_frame_04(QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("frame_bus dynamic_call finished make_new_frame_04");
}
void RPC_frame_bus_SLOT_Object::make_new_frame_733(QString mode, QVariant frame_data)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mode);
	tmp_list << QVariant(frame_data);
	SRPCSignalClass::Instance().toLog(QString("frame_bus dynamic_call make_new_frame_733 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_new_frame_733(QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("frame_bus dynamic_call finished make_new_frame_733");
}
void RPC_frame_bus_SLOT_Object::make_new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mode);
	tmp_list << QVariant(psp);
	tmp_list << QVariant(lit);
	tmp_list << QVariant(fm);
	tmp_list << QVariant(ant);
	tmp_list << QVariant(frame_data);
	SRPCSignalClass::Instance().toLog(QString("frame_bus dynamic_call make_new_frame_07 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_new_frame_07(QString, int, int, int, QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("frame_bus dynamic_call finished make_new_frame_07");
}
void RPC_frame_bus_SLOT_Object::make_new_frame_rm07(QString mode, QVariant frame_data)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mode);
	tmp_list << QVariant(frame_data);
	SRPCSignalClass::Instance().toLog(QString("frame_bus dynamic_call make_new_frame_rm07 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_new_frame_rm07(QString, QVariant)", tmp_list);
	SRPCSignalClass::Instance().toLog("frame_bus dynamic_call finished make_new_frame_rm07");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

