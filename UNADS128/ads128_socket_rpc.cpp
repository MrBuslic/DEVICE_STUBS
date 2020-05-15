	#include "ads128_socket_rpc.h"

int ads128_Socket_RPC_SLOT_Object::obj_num = 0;
int ads128_Socket_RPC_SIGNAL_Object::obj_num = 0;
int ads128_Socket_RPC_SIGNAL_Object::call_number = 0;
int ads128_Socket_RPC_SLOT_Thread::obj_num = 0;

	ads128_Socket_RPC_SIGNAL_Thread::ads128_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("ads128_Socket_RPC_SIGNAL_Thread");
	}

	void ads128_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new ads128_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("ads128 signal thread started");
		exec();
	}

	ads128_Socket_RPC_SIGNAL_Server::ads128_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void ads128_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("ads128 signal client connected");
		std::shared_ptr<ads128_Socket_RPC_SIGNAL_Object> tmp_obj(new ads128_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	ads128_Socket_RPC_SLOT_Server_Thread::ads128_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	ads128_Socket_RPC_SLOT_Server::ads128_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcADS128Widget* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void ads128_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new ads128_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("ads128 slot thread started");
		exec();
	}

	void ads128_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<ads128_Socket_RPC_SLOT_Object>(new ads128_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	ads128_Socket_RPC_SLOT_Thread::ads128_Socket_RPC_SLOT_Thread(RpcADS128Widget* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{
	setObjectName(QString("ads128_Socket_RPC_SLOT_Thread_%1").arg(obj_num++));
	}

	void ads128_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("ads128 slot client connected");
		std::shared_ptr<ads128_Socket_RPC_SLOT_Thread> tmp_obj(new ads128_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	ads128_Socket_RPC_SLOT_Object::ads128_Socket_RPC_SLOT_Object(RpcADS128Widget* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("ads128_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &ads128_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["ads_timer_ontimer()"] = &ads128_Socket_RPC_SLOT_Object::ads_timer_ontimer;
		operators_map["new_ku(int, int, double, int)"] = &ads128_Socket_RPC_SLOT_Object::new_ku;
		operators_map["new_mk(int, int, int, int, double, double, int, int, int)"] = &ads128_Socket_RPC_SLOT_Object::new_mk;
		operators_map["ads128_conf_analog(uint, double, double)"] = &ads128_Socket_RPC_SLOT_Object::ads128_conf_analog;
		operators_map["ads128_start()"] = &ads128_Socket_RPC_SLOT_Object::ads128_start;
		operators_map["ads128_read_data(QVariantList&, QVariantList&)"] = &ads128_Socket_RPC_SLOT_Object::ads128_read_data;
		operators_map["ads128_stop()"] = &ads128_Socket_RPC_SLOT_Object::ads128_stop;
		operators_map["ads128_analog_q(uint, double&, double&)"] = &ads128_Socket_RPC_SLOT_Object::ads128_analog_q;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	ads128_Socket_RPC_SIGNAL_Object::ads128_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("ads128_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void ads128_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void ads128_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void ads128_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void ads128_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
	}
	void ads128_Socket_RPC_SIGNAL_Object::set_app(RpcADS128Widget* _app)
	{
		app = _app;

	}

	void ads128_Socket_RPC_SLOT_Object::read_data()
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

	void ads128_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void ads128_Socket_RPC_SIGNAL_Object::read_data()
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant ads128_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant ads128_Socket_RPC_SLOT_Object::ads_timer_ontimer(QVariantList& _values)
	{
		try
		{
			app->ads_timer_ontimer();
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
	QVariant ads128_Socket_RPC_SLOT_Object::new_ku(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int ku_n = _values.at(0).value<int>();
			int length = _values.at(1).value<int>();
			double u = _values.at(2).value<double>();
			int line = _values.at(3).value<int>();
			app->new_ku(ku_n, length, u, line);
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
	QVariant ads128_Socket_RPC_SLOT_Object::new_mk(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int mshm = _values.at(0).value<int>();
			int pshm = _values.at(1).value<int>();
			int length_m = _values.at(2).value<int>();
			int length_p = _values.at(3).value<int>();
			double u_m = _values.at(4).value<double>();
			double u_p = _values.at(5).value<double>();
			int dt = _values.at(6).value<int>();
			int line_m = _values.at(7).value<int>();
			int line_p = _values.at(8).value<int>();
			app->new_mk(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
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
	QVariant ads128_Socket_RPC_SLOT_Object::ads128_conf_analog(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint group = _values.at(0).value<uint>();
			double level_0 = _values.at(1).value<double>();
			double level_1 = _values.at(2).value<double>();
			int res = app->ads128_conf_analog(group, level_0, level_1);
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return 1;
		}
		catch(...)
		{
			return 1;
		}
	}
	QVariant ads128_Socket_RPC_SLOT_Object::ads128_start(QVariantList& _values)
	{
		try
		{
			int res = app->ads128_start();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return 1;
		}
		catch(...)
		{
			return 1;
		}
	}
	QVariant ads128_Socket_RPC_SLOT_Object::ads128_read_data(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList thisbuf = _values.at(0).value<QVariantList>();
			QVariantList firstbuf = _values.at(1).value<QVariantList>();
			int res = app->ads128_read_data(thisbuf, firstbuf);
			_values[0] = thisbuf;
			SRPCSignalClass::Instance().toLog(QString("%1 thisbuf = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			_values[1] = firstbuf;
			SRPCSignalClass::Instance().toLog(QString("%1 firstbuf = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return 1;
		}
		catch(...)
		{
			return 1;
		}
	}
	QVariant ads128_Socket_RPC_SLOT_Object::ads128_stop(QVariantList& _values)
	{
		try
		{
			int res = app->ads128_stop();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return 1;
		}
		catch(...)
		{
			return 1;
		}
	}
	QVariant ads128_Socket_RPC_SLOT_Object::ads128_analog_q(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint group_ = _values.at(0).value<uint>();
			double lev0 = _values.at(1).value<double>();
			double lev1 = _values.at(2).value<double>();
			int res = app->ads128_analog_q(group_, lev0, lev1);
			_values[1] = lev0;
			SRPCSignalClass::Instance().toLog(QString("%1 lev0 = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			_values[2] = lev1;
			SRPCSignalClass::Instance().toLog(QString("%1 lev1 = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[2])));
			with_return = true;
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return 1;
		}
		catch(...)
		{
			return 1;
		}
	}
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

