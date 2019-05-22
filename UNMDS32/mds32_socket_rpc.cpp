	#include "mds32_socket_rpc.h"

int mds32_Socket_RPC_SLOT_Object::obj_num = 0;
int mds32_Socket_RPC_SIGNAL_Object::obj_num = 0;
int mds32_Socket_RPC_SIGNAL_Object::call_number = 0;

	mds32_Socket_RPC_SIGNAL_Thread::mds32_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("mds32_Socket_RPC_SIGNAL_Thread");
	}

	void mds32_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new mds32_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("mds32 signal thread started");
		exec();
	}

	mds32_Socket_RPC_SIGNAL_Server::mds32_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void mds32_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("mds32 signal client connected");
		std::shared_ptr<mds32_Socket_RPC_SIGNAL_Object> tmp_obj(new mds32_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	mds32_Socket_RPC_SLOT_Server_Thread::mds32_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	mds32_Socket_RPC_SLOT_Server::mds32_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, RpcMDS32Widget* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void mds32_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new mds32_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("mds32 slot thread started");
		exec();
	}

	void mds32_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<mds32_Socket_RPC_SLOT_Object>(new mds32_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	mds32_Socket_RPC_SLOT_Thread::mds32_Socket_RPC_SLOT_Thread(RpcMDS32Widget* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{}

	void mds32_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("mds32 slot client connected");
		std::shared_ptr<mds32_Socket_RPC_SLOT_Thread> tmp_obj(new mds32_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	mds32_Socket_RPC_SLOT_Object::mds32_Socket_RPC_SLOT_Object(RpcMDS32Widget* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("mds32_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &mds32_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["auto_scroll_clicked(int)"] = &mds32_Socket_RPC_SLOT_Object::auto_scroll_clicked;
		operators_map["log_timer_ontimer()"] = &mds32_Socket_RPC_SLOT_Object::log_timer_ontimer;
		operators_map["unmds32_input_trigger(bool)"] = &mds32_Socket_RPC_SLOT_Object::unmds32_input_trigger;
		operators_map["unmds32_read_sample(uint&, uint&, uint&)"] = &mds32_Socket_RPC_SLOT_Object::unmds32_read_sample;
		operators_map["unmds32_start()"] = &mds32_Socket_RPC_SLOT_Object::unmds32_start;
		operators_map["check_box_clicked()"] = &mds32_Socket_RPC_SLOT_Object::check_box_clicked;
		operators_map["line_edit_changed(QString&)"] = &mds32_Socket_RPC_SLOT_Object::line_edit_changed;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	mds32_Socket_RPC_SIGNAL_Object::mds32_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("mds32_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void mds32_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void mds32_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void mds32_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void mds32_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(mds32_get_sample(int, uint&, int&)), this, SLOT(mds32_get_sample(int, uint&, int&)));
	}
	void mds32_Socket_RPC_SIGNAL_Object::set_app(RpcMDS32Widget* _app)
	{
		app = _app;
		connect(app, SIGNAL(mds32_get_sample(int, uint&, int&)), this, SLOT(mds32_get_sample(int, uint&, int&)), Qt::DirectConnection);
		data_map.insert("mds32_get_sample(int, uint&, int&)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void mds32_Socket_RPC_SLOT_Object::read_data()
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

	void mds32_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void mds32_Socket_RPC_SIGNAL_Object::read_data()
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

	void mds32_Socket_RPC_SIGNAL_Object::mds32_get_sample(int channel, uint& buf, int& flag)
	{
		auto& descriptor = *data_map["mds32_get_sample(int, uint&, int&)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("mds32_get_sample(int, uint&, int&)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal mds32_get_sample  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << channel;
		SRPCSignalClass::Instance().toLog(QString("mds32_get_sample  call_number %2 channel =  %1").arg(RPCSignalClass::QVariantToString(channel)).arg(call_number));
		tmp_stream << buf;
		SRPCSignalClass::Instance().toLog(QString("mds32_get_sample  call_number %2 buf =  %1").arg(RPCSignalClass::QVariantToString(buf)).arg(call_number));
		tmp_stream << flag;
		SRPCSignalClass::Instance().toLog(QString("mds32_get_sample  call_number %2 flag =  %1").arg(RPCSignalClass::QVariantToString(flag)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal mds32_get_sample sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		buf = data_map["mds32_get_sample(int, uint&, int&)"]->signal_data.at(1).toUInt();
		flag = data_map["mds32_get_sample(int, uint&, int&)"]->signal_data.at(2).toInt();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal mds32_get_sample finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant mds32_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant mds32_Socket_RPC_SLOT_Object::auto_scroll_clicked(QVariantList& _values)
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
	QVariant mds32_Socket_RPC_SLOT_Object::log_timer_ontimer(QVariantList& _values)
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
	QVariant mds32_Socket_RPC_SLOT_Object::unmds32_input_trigger(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			bool state = _values.at(0).value<bool>();
			int res = app->unmds32_input_trigger(state);
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
	QVariant mds32_Socket_RPC_SLOT_Object::unmds32_read_sample(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint _buf = _values.at(0).value<uint>();
			uint _firstTime = _values.at(1).value<uint>();
			uint _lasteTime = _values.at(2).value<uint>();
			int res = app->unmds32_read_sample(_buf, _firstTime, _lasteTime);
			_values[0] = _buf;
			SRPCSignalClass::Instance().toLog(QString("%1 _buf = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			_values[1] = _firstTime;
			SRPCSignalClass::Instance().toLog(QString("%1 _firstTime = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
			_values[2] = _lasteTime;
			SRPCSignalClass::Instance().toLog(QString("%1 _lasteTime = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[2])));
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
	QVariant mds32_Socket_RPC_SLOT_Object::unmds32_start(QVariantList& _values)
	{
		try
		{
			int res = app->unmds32_start();
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
	QVariant mds32_Socket_RPC_SLOT_Object::check_box_clicked(QVariantList& _values)
	{
		try
		{
			app->check_box_clicked();
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
	QVariant mds32_Socket_RPC_SLOT_Object::line_edit_changed(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString _text = _values.at(0).value<QString>();
			app->line_edit_changed(_text);
			_values[0] = _text;
			SRPCSignalClass::Instance().toLog(QString("%1 _text = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
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
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////

