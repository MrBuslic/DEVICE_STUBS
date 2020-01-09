#include "lka05_rpc.h"

void RPC_lka05_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_lka05_SLOT_Object>(new RPC_lka05_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_lka05_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_lka05_SIGNAL_Object>(new RPC_lka05_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_lka05_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_lka05_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_lka05_SIGNAL_Object::new_mk)) {
		connect_signal("new_mk(int, int, int, int, double, double, int, int, int)", true);
	}
}

void RPC_lka05_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_lka05_SIGNAL_Object::new_ku)) {
		connect_signal("new_ku(int, int, double, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_lka05_SIGNAL_Object::new_mk)) {
		connect_signal("new_mk(int, int, int, int, double, double, int, int, int)", false);
	}
}

void RPC_lka05_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("lka05 new signal " + op_name);

			if (op_name == "new_ku(int, int, double, int)")
			{
				int ku_n;
				tmp_stream >> ku_n;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " ku_n = "+RPCSignalClass::QVariantToString(ku_n));
				int length;
				tmp_stream >> length;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " length = "+RPCSignalClass::QVariantToString(length));
				double u;
				tmp_stream >> u;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " u = "+RPCSignalClass::QVariantToString(u));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				emit new_ku(ku_n, length, u, line);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("lka05 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "new_mk(int, int, int, int, double, double, int, int, int)")
			{
				int mshm;
				tmp_stream >> mshm;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " mshm = "+RPCSignalClass::QVariantToString(mshm));
				int pshm;
				tmp_stream >> pshm;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " pshm = "+RPCSignalClass::QVariantToString(pshm));
				int length_m;
				tmp_stream >> length_m;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " length_m = "+RPCSignalClass::QVariantToString(length_m));
				int length_p;
				tmp_stream >> length_p;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " length_p = "+RPCSignalClass::QVariantToString(length_p));
				double u_m;
				tmp_stream >> u_m;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " u_m = "+RPCSignalClass::QVariantToString(u_m));
				double u_p;
				tmp_stream >> u_p;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " u_p = "+RPCSignalClass::QVariantToString(u_p));
				int dt;
				tmp_stream >> dt;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " dt = "+RPCSignalClass::QVariantToString(dt));
				int line_m;
				tmp_stream >> line_m;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " line_m = "+RPCSignalClass::QVariantToString(line_m));
				int line_p;
				tmp_stream >> line_p;
				SRPCSignalClass::Instance().toLog("lka05 " + op_name +" call_number "+ QString::number(call_number) + " line_p = "+RPCSignalClass::QVariantToString(line_p));
				emit new_mk(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("lka05 signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_lka05_SLOT_Object::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dt);
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("lka05 dynamic_call new_message %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_message(QVariant, int, int, int, QVariantList, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("lka05 dynamic_call finished new_message");
}
void RPC_lka05_SLOT_Object::new_tm(int tm)
{
	QVariantList tmp_list;
	tmp_list << QVariant(tm);
	SRPCSignalClass::Instance().toLog(QString("lka05 dynamic_call new_tm %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_tm(int)", tmp_list);
	SRPCSignalClass::Instance().toLog("lka05 dynamic_call finished new_tm");
}
void RPC_lka05_SLOT_Object::new_ku_732(int ku_n, int length, double u, int line)
{
	QVariantList tmp_list;
	tmp_list << QVariant(ku_n);
	tmp_list << QVariant(length);
	tmp_list << QVariant(u);
	tmp_list << QVariant(line);
	SRPCSignalClass::Instance().toLog(QString("lka05 dynamic_call new_ku_732 %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("new_ku_732(int, int, double, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("lka05 dynamic_call finished new_ku_732");
}
void RPC_lka05_SLOT_Object::get_power(double _volt)
{
	QVariantList tmp_list;
	tmp_list << QVariant(_volt);
	SRPCSignalClass::Instance().toLog(QString("lka05 dynamic_call get_power %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_power(double)", tmp_list);
	SRPCSignalClass::Instance().toLog("lka05 dynamic_call finished get_power");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

