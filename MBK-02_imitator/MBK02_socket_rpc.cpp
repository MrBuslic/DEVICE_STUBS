	#include "MBK02_socket_rpc.h"

int MBK02_Socket_RPC_SLOT_Object::obj_num = 0;
int MBK02_Socket_RPC_SIGNAL_Object::obj_num = 0;
int MBK02_Socket_RPC_SIGNAL_Object::call_number = 0;

	MBK02_Socket_RPC_SIGNAL_Thread::MBK02_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("MBK02_Socket_RPC_SIGNAL_Thread");
	}

	void MBK02_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new MBK02_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("MBK02 signal thread started");
		exec();
	}

	MBK02_Socket_RPC_SIGNAL_Server::MBK02_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void MBK02_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("MBK02 signal client connected");
		std::shared_ptr<MBK02_Socket_RPC_SIGNAL_Object> tmp_obj(new MBK02_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	MBK02_Socket_RPC_SLOT_Server_Thread::MBK02_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	MBK02_Socket_RPC_SLOT_Server::MBK02_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, MBK02_widg* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void MBK02_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new MBK02_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("MBK02 slot thread started");
		exec();
	}

	void MBK02_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<MBK02_Socket_RPC_SLOT_Object>(new MBK02_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	MBK02_Socket_RPC_SLOT_Thread::MBK02_Socket_RPC_SLOT_Thread(MBK02_widg* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{}

	void MBK02_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("MBK02 slot client connected");
		std::shared_ptr<MBK02_Socket_RPC_SLOT_Thread> tmp_obj(new MBK02_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	MBK02_Socket_RPC_SLOT_Object::MBK02_Socket_RPC_SLOT_Object(MBK02_widg* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("MBK02_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &MBK02_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["new_message(QVariant, int, int, int, QVariantList, int)"] = &MBK02_Socket_RPC_SLOT_Object::new_message;
		operators_map["new_ku_732(int, int, double, int)"] = &MBK02_Socket_RPC_SLOT_Object::new_ku_732;
		operators_map["set_new_mbk02_tm()"] = &MBK02_Socket_RPC_SLOT_Object::set_new_mbk02_tm;
		operators_map["new_KPI(QVariantList)"] = &MBK02_Socket_RPC_SLOT_Object::new_KPI;
		operators_map["auto_scroll_clicked(int)"] = &MBK02_Socket_RPC_SLOT_Object::auto_scroll_clicked;
		operators_map["update_tm(int)"] = &MBK02_Socket_RPC_SLOT_Object::update_tm;
		operators_map["get_power(double)"] = &MBK02_Socket_RPC_SLOT_Object::get_power;
		operators_map["update_graphics()"] = &MBK02_Socket_RPC_SLOT_Object::update_graphics;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	MBK02_Socket_RPC_SIGNAL_Object::MBK02_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("MBK02_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void MBK02_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void MBK02_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void MBK02_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void MBK02_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(msg_to_14R732(QVariantList)), this, SLOT(msg_to_14R732(QVariantList)));
		disconnect(app, SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_tm(int, int)));
		disconnect(app, SIGNAL(set_new_power_tm(int, QVariantList)), this, SLOT(set_new_power_tm(int, QVariantList)));
		disconnect(app, SIGNAL(emit_update_graphics()), this, SLOT(emit_update_graphics()));
	}
	void MBK02_Socket_RPC_SIGNAL_Object::set_app(MBK02_widg* _app)
	{
		app = _app;
		connect(app, SIGNAL(msg_to_14R732(QVariantList)), this, SLOT(msg_to_14R732(QVariantList)), Qt::DirectConnection);
		data_map.insert("msg_to_14R732(QVariantList)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(set_new_tm(int, int)), this, SLOT(set_new_tm(int, int)), Qt::DirectConnection);
		data_map.insert("set_new_tm(int, int)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(set_new_power_tm(int, QVariantList)), this, SLOT(set_new_power_tm(int, QVariantList)), Qt::DirectConnection);
		data_map.insert("set_new_power_tm(int, QVariantList)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(emit_update_graphics()), this, SLOT(emit_update_graphics()), Qt::DirectConnection);
		data_map.insert("emit_update_graphics()", std::shared_ptr<SignalData>(new SignalData()));

	}

	void MBK02_Socket_RPC_SLOT_Object::read_data()
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

	void MBK02_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void MBK02_Socket_RPC_SIGNAL_Object::read_data()
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

	void MBK02_Socket_RPC_SIGNAL_Object::msg_to_14R732(QVariantList data)
	{
		auto& descriptor = *data_map["msg_to_14R732(QVariantList)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("msg_to_14R732(QVariantList)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal msg_to_14R732  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << data;
		SRPCSignalClass::Instance().toLog(QString("msg_to_14R732  call_number %2 data =  %1").arg(RPCSignalClass::QVariantToString(data)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal msg_to_14R732 sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal msg_to_14R732 finished").arg(objectName()));
	}
	void MBK02_Socket_RPC_SIGNAL_Object::set_new_tm(int sadr, int word)
	{
		auto& descriptor = *data_map["set_new_tm(int, int)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("set_new_tm(int, int)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal set_new_tm  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << sadr;
		SRPCSignalClass::Instance().toLog(QString("set_new_tm  call_number %2 sadr =  %1").arg(RPCSignalClass::QVariantToString(sadr)).arg(call_number));
		tmp_stream << word;
		SRPCSignalClass::Instance().toLog(QString("set_new_tm  call_number %2 word =  %1").arg(RPCSignalClass::QVariantToString(word)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal set_new_tm sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal set_new_tm finished").arg(objectName()));
	}
	void MBK02_Socket_RPC_SIGNAL_Object::set_new_power_tm(int sadr, QVariantList words)
	{
		auto& descriptor = *data_map["set_new_power_tm(int, QVariantList)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("set_new_power_tm(int, QVariantList)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal set_new_power_tm  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << sadr;
		SRPCSignalClass::Instance().toLog(QString("set_new_power_tm  call_number %2 sadr =  %1").arg(RPCSignalClass::QVariantToString(sadr)).arg(call_number));
		tmp_stream << words;
		SRPCSignalClass::Instance().toLog(QString("set_new_power_tm  call_number %2 words =  %1").arg(RPCSignalClass::QVariantToString(words)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal set_new_power_tm sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal set_new_power_tm finished").arg(objectName()));
	}
	void MBK02_Socket_RPC_SIGNAL_Object::emit_update_graphics()
	{
		auto& descriptor = *data_map["emit_update_graphics()"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("emit_update_graphics()");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal emit_update_graphics  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal emit_update_graphics sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal emit_update_graphics finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant MBK02_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant MBK02_Socket_RPC_SLOT_Object::new_message(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariant dt = _values.at(0).value<QVariant>();
			int mko = _values.at(1).value<int>();
			int line = _values.at(2).value<int>();
			int cwd = _values.at(3).value<int>();
			QVariantList words = _values.at(4).value<QVariantList>();
			int os = _values.at(5).value<int>();
			app->new_message(dt, mko, line, cwd, words, os);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::new_ku_732(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int ku_n = _values.at(0).value<int>();
			int length = _values.at(1).value<int>();
			double u = _values.at(2).value<double>();
			int line = _values.at(3).value<int>();
			app->new_ku_732(ku_n, length, u, line);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::set_new_mbk02_tm(QVariantList& _values)
	{
		try
		{
			app->set_new_mbk02_tm();
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
	QVariant MBK02_Socket_RPC_SLOT_Object::new_KPI(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList KPI_list = _values.at(0).value<QVariantList>();
			app->new_KPI(KPI_list);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::auto_scroll_clicked(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _state = _values.at(0).value<int>();
			app->auto_scroll_clicked(_state);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::update_tm(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int sadr = _values.at(0).value<int>();
			app->update_tm(sadr);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::get_power(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			double volt = _values.at(0).value<double>();
			app->get_power(volt);
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
	QVariant MBK02_Socket_RPC_SLOT_Object::update_graphics(QVariantList& _values)
	{
		try
		{
			app->update_graphics();
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

