#include "omnibus_rpc.h"

void RPC_omnibus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_omnibus_SLOT_Object>(new RPC_omnibus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_omnibus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_omnibus_SIGNAL_Object>(new RPC_omnibus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_omnibus_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_omnibus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_omnibus_SIGNAL_Object::new_message)) {
		SRPCSignalClass::Instance().toLog("new_message connected");
		emit connect_signal("new_message(QVariant, int, int, int, QVariantList, int)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_omnibus_SIGNAL_Object::message_to_log)) {
		SRPCSignalClass::Instance().toLog("message_to_log connected");
		emit connect_signal("message_to_log(QString)", true);
	}
}

void RPC_omnibus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_omnibus_SIGNAL_Object::new_message)) {
		SRPCSignalClass::Instance().toLog("new_message disconnected");
		//emit connect_signal("new_message(QVariant, int, int, int, QVariantList, int)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_omnibus_SIGNAL_Object::message_to_log)) {
		SRPCSignalClass::Instance().toLog("message_to_log disconnected");
		//emit connect_signal("message_to_log(QString)", false);
	}
}

void RPC_omnibus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("omnibus new signal " + op_name);

			if (op_name == "new_message(QVariant, int, int, int, QVariantList, int)")
			{
				QVariant dt;
				tmp_stream >> dt;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " dt = "+RPCSignalClass::QVariantToString(dt));
				int mko;
				tmp_stream >> mko;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " mko = "+RPCSignalClass::QVariantToString(mko));
				int line;
				tmp_stream >> line;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " line = "+RPCSignalClass::QVariantToString(line));
				int cwd;
				tmp_stream >> cwd;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " cwd = "+RPCSignalClass::QVariantToString(cwd));
				QVariantList words;
				tmp_stream >> words;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " words = "+RPCSignalClass::QVariantToString(words));
				int os;
				tmp_stream >> os;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " os = "+RPCSignalClass::QVariantToString(os));
				emit new_message(dt, mko, line, cwd, words, os);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("omnibus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "message_to_log(QString)")
			{
				QString _msg;
				tmp_stream >> _msg;
				SRPCSignalClass::Instance().toLog("omnibus " + op_name +" call_number "+ QString::number(call_number) + " _msg = "+RPCSignalClass::QVariantToString(_msg));
				emit message_to_log(_msg);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("omnibus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_omnibus_SLOT_Object::switch_ab_os(int mko, int addr, int _os)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mko);
	tmp_list << QVariant(addr);
	tmp_list << QVariant(_os);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call switch_ab_os %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("switch_ab_os(int, int, int)", tmp_list);
	SRPCSignalClass::Instance().toLog("omnibus dynamic_call finished switch_ab_os");
}
void RPC_omnibus_SLOT_Object::switch_ab(int mko, int addr, bool _on)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mko);
	tmp_list << QVariant(addr);
	tmp_list << QVariant(_on);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call switch_ab %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("switch_ab(int, int, bool)", tmp_list);
	SRPCSignalClass::Instance().toLog("omnibus dynamic_call finished switch_ab");
}
void RPC_omnibus_SLOT_Object::set_new_data(int mko, int addr, int saddr, QVariantList words)
{
	QVariantList tmp_list;
	tmp_list << QVariant(mko);
	tmp_list << QVariant(addr);
	tmp_list << QVariant(saddr);
	tmp_list << QVariant(words);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call set_new_data %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_new_data(int, int, int, QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("omnibus dynamic_call finished set_new_data");
}
void RPC_omnibus_SLOT_Object::send_msg(int mko, int line, int cwd, QVariantList& words, int& os)
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(mko);
	tmp_list << QVariant(line);
	tmp_list << QVariant(cwd);
	tmp_list << QVariant(words);
	tmp_list << QVariant(os);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call send_msg %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("send_msg(int, int, int, QVariantList&, int&)", tmp_list);
	words = tmp_list.at(3).toList();
	tmp_ret_params += " words="+RPCSignalClass::QVariantToString(tmp_list.at(3));
	os = tmp_list.at(4).toInt();
	tmp_ret_params += " os="+RPCSignalClass::QVariantToString(tmp_list.at(4));
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call finished send_msg %1").arg(tmp_ret_params));
}
int RPC_omnibus_SLOT_Object::unomnibus_map_channels_setup(int _n, int _chan)
{
	if(!connected) return 1;
	QVariantList tmp_list;
	QString tmp_ret_params;
	tmp_list << QVariant(_n);
	tmp_list << QVariant(_chan);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call unomnibus_map_channels_setup %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("unomnibus_map_channels_setup(int, int)", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call finished unomnibus_map_channels_setup %1").arg(tmp_ret_params));
	return res.toInt();
}
QVariant RPC_omnibus_SLOT_Object::get_dt()
{
	QVariantList tmp_list;
	QString tmp_ret_params;
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call get_dt %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("get_dt()", tmp_list);
	tmp_ret_params += " return="+RPCSignalClass::QVariantToString(res);
	SRPCSignalClass::Instance().toLog(QString("omnibus dynamic_call finished get_dt %1").arg(tmp_ret_params));
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

