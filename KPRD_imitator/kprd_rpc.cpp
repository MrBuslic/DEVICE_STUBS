#include "kprd_rpc.h"

void RPC_kprd_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kprd slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("kprd slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kprd slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kprd_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kprd signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("kprd signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kprd signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kprd_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kprd_SLOT_Object>(new RPC_kprd_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kprd_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kprd_SIGNAL_Object>(new RPC_kprd_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kprd_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_kprd_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kprd_SIGNAL_Object::test)) {
		SRPCSignalClass::Instance().toLog("test connected");
		emit connect_signal("test(QVariantList, QVariantList)", true);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kprd_SIGNAL_Object::sendKPI)) {
		SRPCSignalClass::Instance().toLog("sendKPI connected");
		emit connect_signal("sendKPI(QVariantList)", true);
	}
}

void RPC_kprd_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kprd_SIGNAL_Object::test)) {
		SRPCSignalClass::Instance().toLog("test disconnected");
		//emit connect_signal("test(QVariantList, QVariantList)", false);
	}
	else
	if (signal == QMetaMethod::fromSignal(&RPC_kprd_SIGNAL_Object::sendKPI)) {
		SRPCSignalClass::Instance().toLog("sendKPI disconnected");
		//emit connect_signal("sendKPI(QVariantList)", false);
	}
}

void RPC_kprd_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("kprd new signal " + op_name);

			if (op_name == "test(QVariantList, QVariantList)")
			{
				QVariantList maskList;
				tmp_stream >> maskList;
				SRPCSignalClass::Instance().toLog("kprd " + op_name +" call_number "+ QString::number(call_number) + " maskList = "+RPCSignalClass::QVariantToString(maskList));
				QVariantList dataList;
				tmp_stream >> dataList;
				SRPCSignalClass::Instance().toLog("kprd " + op_name +" call_number "+ QString::number(call_number) + " dataList = "+RPCSignalClass::QVariantToString(dataList));
				emit test(maskList, dataList);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kprd signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
			if (op_name == "sendKPI(QVariantList)")
			{
				QVariantList kpiList;
				tmp_stream >> kpiList;
				SRPCSignalClass::Instance().toLog("kprd " + op_name +" call_number "+ QString::number(call_number) + " kpiList = "+RPCSignalClass::QVariantToString(kpiList));
				emit sendKPI(kpiList);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kprd signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_kprd_SLOT_Object::dataIn(QVariantList dataList, QVariantList maskList)
{
	QVariantList tmp_list;
	tmp_list << QVariant(dataList);
	tmp_list << QVariant(maskList);
	SRPCSignalClass::Instance().toLog(QString("kprd dynamic_call dataIn %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("dataIn(QVariantList, QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("kprd dynamic_call finished dataIn");
}
void RPC_kprd_SLOT_Object::set_antenna_connection(QString antenna_name, QString connected_antenna_name)
{
	QVariantList tmp_list;
	tmp_list << QVariant(antenna_name);
	tmp_list << QVariant(connected_antenna_name);
	SRPCSignalClass::Instance().toLog(QString("kprd dynamic_call set_antenna_connection %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("set_antenna_connection(QString, QString)", tmp_list);
	SRPCSignalClass::Instance().toLog("kprd dynamic_call finished set_antenna_connection");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

