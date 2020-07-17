	#include "frame_bus_socket_rpc.h"

int frame_bus_Socket_RPC_SLOT_Object::obj_num = 0;
int frame_bus_Socket_RPC_SIGNAL_Object::obj_num = 0;
int frame_bus_Socket_RPC_SIGNAL_Object::call_number = 0;
int frame_bus_Socket_RPC_SLOT_Thread::obj_num = 0;

	frame_bus_Socket_RPC_SIGNAL_Thread::frame_bus_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("frame_bus_Socket_RPC_SIGNAL_Thread");
	}

	void frame_bus_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new frame_bus_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("frame_bus signal thread started");
		exec();
	}

	frame_bus_Socket_RPC_SIGNAL_Server::frame_bus_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void frame_bus_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("frame_bus signal client connected");
		std::shared_ptr<frame_bus_Socket_RPC_SIGNAL_Object> tmp_obj(new frame_bus_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	frame_bus_Socket_RPC_SLOT_Server_Thread::frame_bus_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	frame_bus_Socket_RPC_SLOT_Server::frame_bus_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, FrameBusWidget* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void frame_bus_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new frame_bus_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("frame_bus slot thread started");
		exec();
	}

	void frame_bus_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<frame_bus_Socket_RPC_SLOT_Object>(new frame_bus_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	frame_bus_Socket_RPC_SLOT_Thread::frame_bus_Socket_RPC_SLOT_Thread(FrameBusWidget* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{
	setObjectName(QString("frame_bus_Socket_RPC_SLOT_Thread_%1").arg(obj_num++));
	}

	void frame_bus_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("frame_bus slot client connected");
		std::shared_ptr<frame_bus_Socket_RPC_SLOT_Thread> tmp_obj(new frame_bus_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	frame_bus_Socket_RPC_SLOT_Object::frame_bus_Socket_RPC_SLOT_Object(FrameBusWidget* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("frame_bus_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &frame_bus_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["make_new_frame_04(QString, QVariant)"] = &frame_bus_Socket_RPC_SLOT_Object::make_new_frame_04;
		operators_map["make_new_frame_733(QString, QVariant)"] = &frame_bus_Socket_RPC_SLOT_Object::make_new_frame_733;
		operators_map["make_new_frame_07(QString, int, int, int, QString, QVariant)"] = &frame_bus_Socket_RPC_SLOT_Object::make_new_frame_07;
		operators_map["make_new_frame_rm07(QString, QVariant)"] = &frame_bus_Socket_RPC_SLOT_Object::make_new_frame_rm07;
		operators_map["make_new_frame_n737(QString, QVariant)"] = &frame_bus_Socket_RPC_SLOT_Object::make_new_frame_n737;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	frame_bus_Socket_RPC_SIGNAL_Object::frame_bus_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("frame_bus_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void frame_bus_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void frame_bus_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void frame_bus_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void frame_bus_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(new_frame_04(QString, QVariant)), this, SLOT(new_frame_04(QString, QVariant)));
		disconnect(app, SIGNAL(new_frame_733(QString, QVariant)), this, SLOT(new_frame_733(QString, QVariant)));
		disconnect(app, SIGNAL(new_frame_07(QString, int, int, int, QString, QVariant)), this, SLOT(new_frame_07(QString, int, int, int, QString, QVariant)));
		disconnect(app, SIGNAL(new_frame_rm07(QString, QVariant)), this, SLOT(new_frame_rm07(QString, QVariant)));
		disconnect(app, SIGNAL(new_frame_n737(QString, QVariant)), this, SLOT(new_frame_n737(QString, QVariant)));
	}
	void frame_bus_Socket_RPC_SIGNAL_Object::set_app(FrameBusWidget* _app)
	{
		app = _app;
		connect(app, SIGNAL(new_frame_04(QString, QVariant)), this, SLOT(new_frame_04(QString, QVariant)), Qt::DirectConnection);
		data_map.insert("new_frame_04(QString, QVariant)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(new_frame_733(QString, QVariant)), this, SLOT(new_frame_733(QString, QVariant)), Qt::DirectConnection);
		data_map.insert("new_frame_733(QString, QVariant)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(new_frame_07(QString, int, int, int, QString, QVariant)), this, SLOT(new_frame_07(QString, int, int, int, QString, QVariant)), Qt::DirectConnection);
		data_map.insert("new_frame_07(QString, int, int, int, QString, QVariant)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(new_frame_rm07(QString, QVariant)), this, SLOT(new_frame_rm07(QString, QVariant)), Qt::DirectConnection);
		data_map.insert("new_frame_rm07(QString, QVariant)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(new_frame_n737(QString, QVariant)), this, SLOT(new_frame_n737(QString, QVariant)), Qt::DirectConnection);
		data_map.insert("new_frame_n737(QString, QVariant)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void frame_bus_Socket_RPC_SLOT_Object::read_data()
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

	void frame_bus_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void frame_bus_Socket_RPC_SIGNAL_Object::read_data()
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
				if (!data_map.contains(signal_name))
					return;
				SRPCSignalClass::Instance().toLog(QString("%1 connect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = true;
			}
			else
			if (signal_name == "disconnect")
			{
				tmp_stream >> signal_name;
				if (!data_map.contains(signal_name))
					return;
				SRPCSignalClass::Instance().toLog(QString("%1 disconnect signal %2").arg(objectName()).arg(signal_name));
				data_map[signal_name]->signal_needed = false;
			}
			else
			{
				if (!data_map.contains(signal_name))
					return;
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

	void frame_bus_Socket_RPC_SIGNAL_Object::new_frame_04(QString mode, QVariant frame_data)
	{
		auto& descriptor = *data_map["new_frame_04(QString, QVariant)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_frame_04(QString, QVariant)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_frame_04  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << mode;
		SRPCSignalClass::Instance().toLog(QString("new_frame_04  call_number %2 mode =  %1").arg(RPCSignalClass::QVariantToString(mode)).arg(call_number));
		tmp_stream << frame_data;
		SRPCSignalClass::Instance().toLog(QString("new_frame_04  call_number %2 frame_data =  %1").arg(RPCSignalClass::QVariantToString(frame_data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_04 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_04 finished").arg(objectName()));
	}
	void frame_bus_Socket_RPC_SIGNAL_Object::new_frame_733(QString mode, QVariant frame_data)
	{
		auto& descriptor = *data_map["new_frame_733(QString, QVariant)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_frame_733(QString, QVariant)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_frame_733  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << mode;
		SRPCSignalClass::Instance().toLog(QString("new_frame_733  call_number %2 mode =  %1").arg(RPCSignalClass::QVariantToString(mode)).arg(call_number));
		tmp_stream << frame_data;
		SRPCSignalClass::Instance().toLog(QString("new_frame_733  call_number %2 frame_data =  %1").arg(RPCSignalClass::QVariantToString(frame_data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_733 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_733 finished").arg(objectName()));
	}
	void frame_bus_Socket_RPC_SIGNAL_Object::new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data)
	{
		auto& descriptor = *data_map["new_frame_07(QString, int, int, int, QString, QVariant)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_frame_07(QString, int, int, int, QString, QVariant)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_frame_07  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << mode;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 mode =  %1").arg(RPCSignalClass::QVariantToString(mode)).arg(call_number));
		tmp_stream << psp;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 psp =  %1").arg(RPCSignalClass::QVariantToString(psp)).arg(call_number));
		tmp_stream << lit;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 lit =  %1").arg(RPCSignalClass::QVariantToString(lit)).arg(call_number));
		tmp_stream << fm;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 fm =  %1").arg(RPCSignalClass::QVariantToString(fm)).arg(call_number));
		tmp_stream << ant;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 ant =  %1").arg(RPCSignalClass::QVariantToString(ant)).arg(call_number));
		tmp_stream << frame_data;
		SRPCSignalClass::Instance().toLog(QString("new_frame_07  call_number %2 frame_data =  %1").arg(RPCSignalClass::QVariantToString(frame_data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_07 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_07 finished").arg(objectName()));
	}
	void frame_bus_Socket_RPC_SIGNAL_Object::new_frame_rm07(QString mode, QVariant frame_data)
	{
		auto& descriptor = *data_map["new_frame_rm07(QString, QVariant)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_frame_rm07(QString, QVariant)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_frame_rm07  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << mode;
		SRPCSignalClass::Instance().toLog(QString("new_frame_rm07  call_number %2 mode =  %1").arg(RPCSignalClass::QVariantToString(mode)).arg(call_number));
		tmp_stream << frame_data;
		SRPCSignalClass::Instance().toLog(QString("new_frame_rm07  call_number %2 frame_data =  %1").arg(RPCSignalClass::QVariantToString(frame_data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_rm07 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_rm07 finished").arg(objectName()));
	}
	void frame_bus_Socket_RPC_SIGNAL_Object::new_frame_n737(QString mode, QVariant frame_data)
	{
		auto& descriptor = *data_map["new_frame_n737(QString, QVariant)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_frame_n737(QString, QVariant)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_frame_n737  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << mode;
		SRPCSignalClass::Instance().toLog(QString("new_frame_n737  call_number %2 mode =  %1").arg(RPCSignalClass::QVariantToString(mode)).arg(call_number));
		tmp_stream << frame_data;
		SRPCSignalClass::Instance().toLog(QString("new_frame_n737  call_number %2 frame_data =  %1").arg(RPCSignalClass::QVariantToString(frame_data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_n737 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_frame_n737 finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant frame_bus_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant frame_bus_Socket_RPC_SLOT_Object::make_new_frame_04(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString mode = _values.at(0).value<QString>();
			QVariant frame_data = _values.at(1).value<QVariant>();
			app->make_new_frame_04(mode, frame_data);
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
	QVariant frame_bus_Socket_RPC_SLOT_Object::make_new_frame_733(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString mode = _values.at(0).value<QString>();
			QVariant frame_data = _values.at(1).value<QVariant>();
			app->make_new_frame_733(mode, frame_data);
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
	QVariant frame_bus_Socket_RPC_SLOT_Object::make_new_frame_07(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString mode = _values.at(0).value<QString>();
			int psp = _values.at(1).value<int>();
			int lit = _values.at(2).value<int>();
			int fm = _values.at(3).value<int>();
			QString ant = _values.at(4).value<QString>();
			QVariant frame_data = _values.at(5).value<QVariant>();
			app->make_new_frame_07(mode, psp, lit, fm, ant, frame_data);
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
	QVariant frame_bus_Socket_RPC_SLOT_Object::make_new_frame_rm07(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString mode = _values.at(0).value<QString>();
			QVariant frame_data = _values.at(1).value<QVariant>();
			app->make_new_frame_rm07(mode, frame_data);
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
	QVariant frame_bus_Socket_RPC_SLOT_Object::make_new_frame_n737(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString mode = _values.at(0).value<QString>();
			QVariant frame_data = _values.at(1).value<QVariant>();
			app->make_new_frame_n737(mode, frame_data);
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

