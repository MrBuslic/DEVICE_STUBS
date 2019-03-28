	#include "omnibus_socket_rpc.h"

int Socket_RPC_SLOT_Object::obj_num = 0;
int Socket_RPC_SIGNAL_Object::obj_num = 0;
int Socket_RPC_SIGNAL_Object::call_number = 0;

	Socket_RPC_SIGNAL_Thread::Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SIGNAL_Thread");
	}

	void Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("omnibus signal thread started");
		exec();
	}

	Socket_RPC_SIGNAL_Server::Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("omnibus signal client connected");
		std::shared_ptr<Socket_RPC_SIGNAL_Object> tmp_obj(new Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	Socket_RPC_SLOT_Server_Thread::Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	Socket_RPC_SLOT_Server::Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcOmnibusWidget* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("omnibus slot thread started");
		exec();
	}

	void Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<Socket_RPC_SLOT_Object>(new Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	Socket_RPC_SLOT_Thread::Socket_RPC_SLOT_Thread(RpcOmnibusWidget* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{}

	void Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("omnibus slot client connected");
		std::shared_ptr<Socket_RPC_SLOT_Thread> tmp_obj(new Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	Socket_RPC_SLOT_Object::Socket_RPC_SLOT_Object(RpcOmnibusWidget* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("omnibus_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["auto_scroll_clicked(int)"] = &Socket_RPC_SLOT_Object::auto_scroll_clicked;
		operators_map["log_timer_ontimer()"] = &Socket_RPC_SLOT_Object::log_timer_ontimer;
		operators_map["switch_ab(int, int, bool)"] = &Socket_RPC_SLOT_Object::switch_ab;
		operators_map["set_new_data(int, int, int, QVariantList)"] = &Socket_RPC_SLOT_Object::set_new_data;
		operators_map["send_msg(int, int, int, QVariantList&, int&)"] = &Socket_RPC_SLOT_Object::send_msg;
		operators_map["get_dt()"] = &Socket_RPC_SLOT_Object::get_dt;
		operators_map["message_to_log_slot(QString)"] = &Socket_RPC_SLOT_Object::message_to_log_slot;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	Socket_RPC_SIGNAL_Object::Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("omnibus_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
		disconnect(app, SIGNAL(message_to_log(QString)), this, SLOT(message_to_log(QString)));
	}
	void Socket_RPC_SIGNAL_Object::set_app(RpcOmnibusWidget* _app)
	{
		app = _app;
		connect(app, SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)), Qt::DirectConnection);
		data_map.insert("new_message(QVariant, int, int, int, QVariantList, int)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(message_to_log(QString)), this, SLOT(message_to_log(QString)), Qt::DirectConnection);
		data_map.insert("message_to_log(QString)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void Socket_RPC_SLOT_Object::read_data()
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

	void Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void Socket_RPC_SIGNAL_Object::read_data()
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

	void Socket_RPC_SIGNAL_Object::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
	{
		auto& descriptor = *data_map["new_message(QVariant, int, int, int, QVariantList, int)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("new_message(QVariant, int, int, int, QVariantList, int)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal new_message  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << dt;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 dt =  %1").arg(RPCSignalClass::QVariantToString(dt)).arg(call_number));
		tmp_stream << mko;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 mko =  %1").arg(RPCSignalClass::QVariantToString(mko)).arg(call_number));
		tmp_stream << line;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 line =  %1").arg(RPCSignalClass::QVariantToString(line)).arg(call_number));
		tmp_stream << cwd;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 cwd =  %1").arg(RPCSignalClass::QVariantToString(cwd)).arg(call_number));
		tmp_stream << words;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 words =  %1").arg(RPCSignalClass::QVariantToString(words)).arg(call_number));
		tmp_stream << os;
		SRPCSignalClass::Instance().toLog(QString("new_message  call_number %2 os =  %1").arg(RPCSignalClass::QVariantToString(os)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_message sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal new_message finished").arg(objectName()));
	}
	void Socket_RPC_SIGNAL_Object::message_to_log(QString _msg)
	{
		auto& descriptor = *data_map["message_to_log(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("message_to_log(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal message_to_log  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << _msg;
		SRPCSignalClass::Instance().toLog(QString("message_to_log  call_number %2 _msg =  %1").arg(RPCSignalClass::QVariantToString(_msg)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal message_to_log sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal message_to_log finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant Socket_RPC_SLOT_Object::auto_scroll_clicked(QVariantList& _values)
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
	QVariant Socket_RPC_SLOT_Object::log_timer_ontimer(QVariantList& _values)
	{
		try
		{
			app->log_timer_ontimer();
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
	QVariant Socket_RPC_SLOT_Object::switch_ab(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int mko = _values.at(0).value<int>();
			int addr = _values.at(1).value<int>();
			bool _on = _values.at(2).value<bool>();
			app->switch_ab(mko, addr, _on);
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
	QVariant Socket_RPC_SLOT_Object::set_new_data(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int mko = _values.at(0).value<int>();
			int addr = _values.at(1).value<int>();
			int saddr = _values.at(2).value<int>();
			QVariantList words = _values.at(3).value<QVariantList>();
			app->set_new_data(mko, addr, saddr, words);
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
	QVariant Socket_RPC_SLOT_Object::send_msg(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int mko = _values.at(0).value<int>();
			int line = _values.at(1).value<int>();
			int cwd = _values.at(2).value<int>();
			QVariantList words = _values.at(3).value<QVariantList>();
			int os = _values.at(4).value<int>();
			app->send_msg(mko, line, cwd, words, os);
			_values[3] = words;
			SRPCSignalClass::Instance().toLog(QString("%1 words = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[3])));
			_values[4] = os;
			SRPCSignalClass::Instance().toLog(QString("%1 os = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[4])));
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
	QVariant Socket_RPC_SLOT_Object::get_dt(QVariantList& _values)
	{
		try
		{
			QVariant res = app->get_dt();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return QVariant();
		}
		catch(...)
		{
			return QVariant();
		}
	}
	QVariant Socket_RPC_SLOT_Object::message_to_log_slot(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _msg = _values.at(0).value<QString>();
			app->message_to_log_slot(_msg);
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

