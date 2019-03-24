#include "kpi_bus_rpc.h"

void RPC_kpi_bus_SLOT_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kpi_bus slot connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		SRPCSignalClass::Instance().toLog("kpi_bus slot connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kpi_bus slot connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kpi_bus_SIGNAL_Object::connect_to_server()
{
	SRPCSignalClass::Instance().toLog(QString("kpi_bus signal connecting %1 %2").arg(addr).arg(port));
	_sock = std::shared_ptr<QTcpSocket>(new QTcpSocket);
	_sock->connectToHost(addr,port);
	if (_sock->waitForConnected(3000))
	{
		connected = true;
		connect(_sock.get(), SIGNAL(readyRead()), this, SLOT(read_data()));
		SRPCSignalClass::Instance().toLog("kpi_bus signal connected");
	}
	else
	{
		connected = false;
		SRPCSignalClass::Instance().toLog(QString("kpi_bus signal connection failed %1 %2").arg(_sock->error()).arg(_sock->errorString()));
	}
}

void RPC_kpi_bus_SLOT_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kpi_bus_SLOT_Object>(new RPC_kpi_bus_SLOT_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kpi_bus_SIGNAL_Thread::run()
{
	rpc_obj = std::shared_ptr<RPC_kpi_bus_SIGNAL_Object>(new RPC_kpi_bus_SIGNAL_Object(addr, port));
	rpc_obj->connect_to_server();
	exec();
}

void RPC_kpi_bus_SIGNAL_Object::send_connect(QString signal_name, bool _connect)
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

void RPC_kpi_bus_SIGNAL_Object::connectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpi_bus_SIGNAL_Object::new_KPI)) {
		SRPCSignalClass::Instance().toLog("new_KPI connected");
		emit connect_signal("new_KPI(QVariantList)", true);
	}
}

void RPC_kpi_bus_SIGNAL_Object::disconnectNotify(const QMetaMethod & signal)
{
	if (signal == QMetaMethod::fromSignal(&RPC_kpi_bus_SIGNAL_Object::new_KPI)) {
		SRPCSignalClass::Instance().toLog("new_KPI disconnected");
		//emit connect_signal("new_KPI(QVariantList)", false);
	}
}

void RPC_kpi_bus_SIGNAL_Object::read_data()
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

			SRPCSignalClass::Instance().toLog("kpi_bus new signal " + op_name);

			if (op_name == "new_KPI(QVariantList)")
			{
				QVariantList KPI_list;
				tmp_stream >> KPI_list;
				SRPCSignalClass::Instance().toLog("kpi_bus " + op_name +" call_number "+ QString::number(call_number) + " KPI_list = "+RPCSignalClass::QVariantToString(KPI_list));
				emit new_KPI(KPI_list);
				QByteArray tmp_arr2;
				QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
				tmp_stream2 << op_name;
				QByteArray tmp_arr3;
				QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
				tmp_stream3 << tmp_arr2.size();
				_sock->write(tmp_arr3 + tmp_arr2);
				_sock->waitForBytesWritten(3000);
				SRPCSignalClass::Instance().toLog("kpi_bus signal finished " + op_name +" call_number "+ QString::number(call_number));
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

void RPC_kpi_bus_SLOT_Object::make_KPI(QVariantList KPI_list)
{
	QVariantList tmp_list;
	tmp_list << QVariant(KPI_list);
	SRPCSignalClass::Instance().toLog(QString("kpi_bus dynamic_call make_KPI %1").arg(RPCSignalClass::QVariantToString(tmp_list)));
	dynamic_call("make_KPI(QVariantList)", tmp_list);
	SRPCSignalClass::Instance().toLog("kpi_bus dynamic_call finished make_KPI");
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

