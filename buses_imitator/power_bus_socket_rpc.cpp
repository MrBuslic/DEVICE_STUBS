	#include "power_bus_socket_rpc.h"

int power_bus_Socket_RPC_SLOT_Object::obj_num = 0;
int power_bus_Socket_RPC_SIGNAL_Object::obj_num = 0;
int power_bus_Socket_RPC_SIGNAL_Object::call_number = 0;

	power_bus_Socket_RPC_SIGNAL_Thread::power_bus_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("power_bus_Socket_RPC_SIGNAL_Thread");
	}

	void power_bus_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new power_bus_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("power_bus signal thread started");
		exec();
	}

	power_bus_Socket_RPC_SIGNAL_Server::power_bus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void power_bus_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("power_bus signal client connected");
		std::shared_ptr<power_bus_Socket_RPC_SIGNAL_Object> tmp_obj(new power_bus_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	power_bus_Socket_RPC_SLOT_Server_Thread::power_bus_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	power_bus_Socket_RPC_SLOT_Server::power_bus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, PowerWidget* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void power_bus_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new power_bus_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("power_bus slot thread started");
		exec();
	}

	void power_bus_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<power_bus_Socket_RPC_SLOT_Object>(new power_bus_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	power_bus_Socket_RPC_SLOT_Thread::power_bus_Socket_RPC_SLOT_Thread(PowerWidget* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{}

	void power_bus_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("power_bus slot client connected");
		std::shared_ptr<power_bus_Socket_RPC_SLOT_Thread> tmp_obj(new power_bus_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	power_bus_Socket_RPC_SLOT_Object::power_bus_Socket_RPC_SLOT_Object(PowerWidget* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("power_bus_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &power_bus_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["set_u(int, double)"] = &power_bus_Socket_RPC_SLOT_Object::set_u;
		operators_map["get_i(int, double&)"] = &power_bus_Socket_RPC_SLOT_Object::get_i;
		operators_map["set_i(int, QString, double)"] = &power_bus_Socket_RPC_SLOT_Object::set_i;
		operators_map["set_bus_state(int, int)"] = &power_bus_Socket_RPC_SLOT_Object::set_bus_state;
		operators_map["set_bus_u(QString, double)"] = &power_bus_Socket_RPC_SLOT_Object::set_bus_u;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	power_bus_Socket_RPC_SIGNAL_Object::power_bus_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("power_bus_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void power_bus_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void power_bus_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void power_bus_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void power_bus_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(u_on_bus(QString, double)), this, SLOT(u_on_bus(QString, double)));
		disconnect(app, SIGNAL(u_on_nk(double)), this, SLOT(u_on_nk(double)));
		disconnect(app, SIGNAL(u_on_k1(double)), this, SLOT(u_on_k1(double)));
		disconnect(app, SIGNAL(u_on_k2(double)), this, SLOT(u_on_k2(double)));
	}
	void power_bus_Socket_RPC_SIGNAL_Object::set_app(PowerWidget* _app)
	{
		app = _app;
		connect(app, SIGNAL(u_on_bus(QString, double)), this, SLOT(u_on_bus(QString, double)), Qt::DirectConnection);
		data_map.insert("u_on_bus(QString, double)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(u_on_nk(double)), this, SLOT(u_on_nk(double)), Qt::DirectConnection);
		data_map.insert("u_on_nk(double)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(u_on_k1(double)), this, SLOT(u_on_k1(double)), Qt::DirectConnection);
		data_map.insert("u_on_k1(double)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(u_on_k2(double)), this, SLOT(u_on_k2(double)), Qt::DirectConnection);
		data_map.insert("u_on_k2(double)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void power_bus_Socket_RPC_SLOT_Object::read_data()
	{
		//LARGE_INTEGER _freq;
		//LARGE_INTEGER tmp1;
		//LARGE_INTEGER tmp2;
		int tmp_size;
		int tmp_bytes = rpc_socket->bytesAvailable();
		if (tmp_bytes < 4)
			return;
		QByteArray tmp_size_arr = rpc_socket->read(4);
		QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
		tmp_size_stream >> tmp_size;
		while (1)
		{
			tmp_bytes = rpc_socket->bytesAvailable();
			if (tmp_bytes >= tmp_size)
			break;
			rpc_socket->waitForReadyRead(500);
		}
		SRPCSignalClass::Instance().toLog(QString(" %1 new data %2 bytes").arg(objectName()).arg(tmp_size));
		QByteArray tmp_arr = rpc_socket->read(tmp_size);
		QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
		int call_n;
		tmp_stream >> call_n;
		QString op_name;
		tmp_stream >> op_name;
		SRPCSignalClass::Instance().toLog(QString("%1 op_name %2 call_n %3").arg(objectName()).arg(op_name).arg(call_n));
		OPERATORS_MAP::iterator op_itr = operators_map.find(op_name);
		QVariant res = 1;
		with_return = false;
		QVariantList var_list;
		tmp_stream >> var_list;
		if (op_itr != operators_map.end())
			res = (this->*op_itr.value())(var_list);
		//QueryPerformanceCounter(&tmp2);
		//double tmp_time = (tmp2.QuadPart-tmp1.QuadPart)*1e6/_freq.QuadPart;
		//qDebug() << tmp_time;
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		if (!with_return)
		{
			tmp_stream2 << QString("response");
			tmp_stream2 << res;
		}
		else
		{
			tmp_stream2 << QString("response_return");
			tmp_stream2 << res;
			tmp_stream2 << var_list;
		}
		QByteArray tmp_arr3;
		QDataStream tmp_stream3(&tmp_arr3, QIODevice::WriteOnly);
		tmp_stream3 << tmp_arr2.size();
		rpc_socket->write(tmp_arr3+tmp_arr2);
		for (int i = 0; i<3; ++i)
		{
			if (rpc_socket->waitForBytesWritten(3000))
				break; 
			else
				SRPCSignalClass::Instance().toLog(QString(" == == == == == == == == == == == == == = %1 response wait failed %2").arg(objectName()).arg(rpc_socket->errorString()));
		}
		SRPCSignalClass::Instance().toLog(QString(" %1 %2 call_n %3 response sent %4 %5 %6").arg(objectName()).arg(op_name).arg(call_n).arg(tmp_arr2.size()).arg(tmp_arr3.size()).arg(tmp_arr3.data())); 
	}

	void power_bus_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void power_bus_Socket_RPC_SIGNAL_Object::read_data()
	{
		int tmp_size;
		while (rpc_socket->bytesAvailable())
		{
			int tmp_bytes = rpc_socket->bytesAvailable();
			while (tmp_bytes < 4)
			{
				rpc_socket->waitForReadyRead(100);
				tmp_bytes = rpc_socket->bytesAvailable();
			}
			QByteArray tmp_size_arr = rpc_socket->read(4);
			QDataStream tmp_size_stream(&tmp_size_arr, QIODevice::ReadOnly);
			tmp_size_stream >> tmp_size;
			while (1)
			{
				tmp_bytes = rpc_socket->bytesAvailable();
				if (tmp_bytes >= tmp_size)
					break;
				rpc_socket->waitForReadyRead(100);
			}
			QByteArray tmp_arr = rpc_socket->read(tmp_size);
			QDataStream tmp_stream(&tmp_arr, QIODevice::ReadOnly);
			QVariantList signal_data;
			QString signal_name;
			tmp_stream >> signal_name;
			if (signal_name == "connect")
			{
				tmp_stream >> signal_name;
				SRPCSignalClass::Instance().toLog(QString("%1 connect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = true;
			}
			else
			if (signal_name == "disconnect")
			{
				tmp_stream >> signal_name;
				SRPCSignalClass::Instance().toLog(QString("%1 disconnect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = false;
			}
			else
			{
				auto& descriptor = *data_map[signal_name].get();
				SRPCSignalClass::Instance().toLog(QString("%1 signal %2 received data").arg(objectName()).arg(signal_name));
				if (signal_name.toLocal8Bit().size() != tmp_size)
					tmp_stream >> signal_data;
				descriptor.signal_data = signal_data;
//				data_map[signal_name]->cond.wakeAll();
				descriptor.mutex.unlock();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	void power_bus_Socket_RPC_SIGNAL_Object::u_on_bus(QString name, double volt)
	{
		auto& descriptor = *data_map["u_on_bus(QString, double)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("u_on_bus(QString, double)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal u_on_bus  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << name;
		SRPCSignalClass::Instance().toLog(QString("u_on_bus  call_number %2 name =  %1").arg(RPCSignalClass::QVariantToString(name)).arg(call_number));
		tmp_stream << volt;
		SRPCSignalClass::Instance().toLog(QString("u_on_bus  call_number %2 volt =  %1").arg(RPCSignalClass::QVariantToString(volt)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_bus sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_bus finished").arg(objectName()));
	}
	void power_bus_Socket_RPC_SIGNAL_Object::u_on_nk(double volt)
	{
		auto& descriptor = *data_map["u_on_nk(double)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("u_on_nk(double)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal u_on_nk  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << volt;
		SRPCSignalClass::Instance().toLog(QString("u_on_nk  call_number %2 volt =  %1").arg(RPCSignalClass::QVariantToString(volt)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_nk sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_nk finished").arg(objectName()));
	}
	void power_bus_Socket_RPC_SIGNAL_Object::u_on_k1(double volt)
	{
		auto& descriptor = *data_map["u_on_k1(double)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("u_on_k1(double)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal u_on_k1  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << volt;
		SRPCSignalClass::Instance().toLog(QString("u_on_k1  call_number %2 volt =  %1").arg(RPCSignalClass::QVariantToString(volt)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_k1 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_k1 finished").arg(objectName()));
	}
	void power_bus_Socket_RPC_SIGNAL_Object::u_on_k2(double volt)
	{
		auto& descriptor = *data_map["u_on_k2(double)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("u_on_k2(double)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal u_on_k2  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << volt;
		SRPCSignalClass::Instance().toLog(QString("u_on_k2  call_number %2 volt =  %1").arg(RPCSignalClass::QVariantToString(volt)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_k2 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal u_on_k2 finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant power_bus_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant power_bus_Socket_RPC_SLOT_Object::set_u(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int bus = _values.at(0).value<int>();
			double volt = _values.at(1).value<double>();
			app->set_u(bus, volt);
			return 0;
		}
		catch(const std::exception &)
		{
			return 0;
		}
		catch(...)
		{
			return 0;
		}
	}
	QVariant power_bus_Socket_RPC_SLOT_Object::get_i(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int bus = _values.at(0).value<int>();
			double curr = _values.at(1).value<double>();
			app->get_i(bus, curr);
			_values[1] = curr;
			SRPCSignalClass::Instance().toLog(QString("%1 curr = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			return 0;
		}
		catch(const std::exception &)
		{
			return 0;
		}
		catch(...)
		{
			return 0;
		}
	}
	QVariant power_bus_Socket_RPC_SLOT_Object::set_i(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int bus = _values.at(0).value<int>();
			QString name = _values.at(1).value<QString>();
			double curr = _values.at(2).value<double>();
			app->set_i(bus, name, curr);
			return 0;
		}
		catch(const std::exception &)
		{
			return 0;
		}
		catch(...)
		{
			return 0;
		}
	}
	QVariant power_bus_Socket_RPC_SLOT_Object::set_bus_state(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int bus = _values.at(0).value<int>();
			int state = _values.at(1).value<int>();
			app->set_bus_state(bus, state);
			return 0;
		}
		catch(const std::exception &)
		{
			return 0;
		}
		catch(...)
		{
			return 0;
		}
	}
	QVariant power_bus_Socket_RPC_SLOT_Object::set_bus_u(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString name = _values.at(0).value<QString>();
			double volt = _values.at(1).value<double>();
			app->set_bus_u(name, volt);
			return 0;
		}
		catch(const std::exception &)
		{
			return 0;
		}
		catch(...)
		{
			return 0;
		}
	}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

