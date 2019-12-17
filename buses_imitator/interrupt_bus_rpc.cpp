#include "interrupt_bus_rpc.h"

void RPC_interrupt_bus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_interrupt_bus_SLOT_Object>(new RPC_interrupt_bus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_interrupt_bus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_interrupt_bus_SIGNAL_Object>(new RPC_interrupt_bus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_interrupt_bus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_interrupt_bus_SIGNAL_Object::new_interrupt)) {
		connect_signal("new_interrupt(int, short, double, double)", true);
	}
}

void RPC_interrupt_bus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_interrupt_bus_SIGNAL_Object::new_interrupt)) {
		connect_signal("new_interrupt(int, short, double, double)", false);
	}
}

void RPC_interrupt_bus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("interrupt_bus new signal " + op_name);

			if (op_name == "new_interrupt(int, short, double, double)")
			{
				int _n;
				tmp_stream >> _n;
				SRPCSignalClass::Instance().toLog("interrupt_bus " + op_name +" call_number "+ QString::number(call_number) + " _n = "+RPCSignalClass::QVariantToString(_n));
				short _chan;
				tmp_stream >> _chan;
				SRPCSignalClass::Instance().toLog("interrupt_bus " + op_name +" call_number "+ QString::number(call_number) + " _chan = "+RPCSignalClass::QVariantToString(_chan));
				double _u;
				tmp_stream >> _u;
				SRPCSignalClass::Instance().toLog("interrupt_bus " + op_name +" call_number "+ QString::number(call_number) + " _u = "+RPCSignalClass::QVariantToString(_u));
				double _t;
				tmp_stream >> _t;
				SRPCSignalClass::Instance().toLog("interrupt_bus " + op_name +" call_number "+ QString::number(call_number) + " _t = "+RPCSignalClass::QVariantToString(_t));
				emit new_interrupt(_n, _chan, _u, _t);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("interrupt_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_interrupt_bus_SLOT_Object::make_interrupt(int _n, short _chan, double _u, double _t)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	tmp_list << QVariant(_u);
	tmp_list << QVariant(_t);
	SRPCSignalClass::Instance().toLog(QString("interrupt_bus dynamic_call make_interrupt %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_interrupt(int, short, double, double)", tmp_list);
	SRPCSignalClass::Instance().toLog("interrupt_bus dynamic_call finished make_interrupt");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

