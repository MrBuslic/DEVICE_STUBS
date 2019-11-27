#include "cbk_rpc.h"

void RPC_cbk_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_cbk_SLOT_Object>(new RPC_cbk_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_cbk_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_cbk_SIGNAL_Object>(new RPC_cbk_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_cbk_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_cbk_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_is_on)) {
		SRPCSignalClass::Instance().toLog("vm_is_on connected");
		emit connect_signal("vm_is_on(int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_is_off)) {
		SRPCSignalClass::Instance().toLog("vm_is_off connected");
		emit connect_signal("vm_is_off(int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_change_po)) {
		SRPCSignalClass::Instance().toLog("vm_change_po connected");
		emit connect_signal("vm_change_po(int)", true);
	}
}

void RPC_cbk_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_is_on)) {
		SRPCSignalClass::Instance().toLog("vm_is_on disconnected");
		//emit connect_signal("vm_is_on(int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_is_off)) {
		SRPCSignalClass::Instance().toLog("vm_is_off disconnected");
		//emit connect_signal("vm_is_off(int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_cbk_SIGNAL_Object::vm_change_po)) {
		SRPCSignalClass::Instance().toLog("vm_change_po disconnected");
		//emit connect_signal("vm_change_po(int)", false);
	}
}

void RPC_cbk_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("cbk new signal " + op_name);

			if (op_name == "vm_is_on(int)")
			{
				int n_vm;
				tmp_stream >> n_vm;
				SRPCSignalClass::Instance().toLog("cbk " + op_name +" call_number "+ QString::number(call_number) + " n_vm = "+RPCSignalClass::QVariantToString(n_vm));
				emit vm_is_on(n_vm);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("cbk signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "vm_is_off(int)")
			{
				int n_vm;
				tmp_stream >> n_vm;
				SRPCSignalClass::Instance().toLog("cbk " + op_name +" call_number "+ QString::number(call_number) + " n_vm = "+RPCSignalClass::QVariantToString(n_vm));
				emit vm_is_off(n_vm);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("cbk signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "vm_change_po(int)")
			{
				int n_vm;
				tmp_stream >> n_vm;
				SRPCSignalClass::Instance().toLog("cbk " + op_name +" call_number "+ QString::number(call_number) + " n_vm = "+RPCSignalClass::QVariantToString(n_vm));
				emit vm_change_po(n_vm);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("cbk signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_cbk_SLOT_Object::set_tm_state()
{
	QVariantList tmp_list;
	SRPCSignalClass::Instance().toLog(QString("cbk dynamic_call set_tm_state %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_tm_state()", tmp_list);
	SRPCSignalClass::Instance().toLog("cbk dynamic_call finished set_tm_state");
}
void RPC_cbk_SLOT_Object::new_ku(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("cbk dynamic_call new_ku %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("cbk dynamic_call finished new_ku");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

