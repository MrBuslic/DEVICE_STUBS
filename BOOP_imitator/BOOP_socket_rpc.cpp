	#include "BOOP_socket_rpc.h"

int BOOP_Socket_RPC_SLOT_Object::obj_num = 0;
int BOOP_Socket_RPC_SIGNAL_Object::obj_num = 0;
int BOOP_Socket_RPC_SIGNAL_Object::call_number = 0;

	BOOP_Socket_RPC_SIGNAL_Thread::BOOP_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("BOOP_Socket_RPC_SIGNAL_Thread");
	}

	void BOOP_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new BOOP_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("BOOP signal thread started");
		exec();
	}

	BOOP_Socket_RPC_SIGNAL_Server::BOOP_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void BOOP_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("BOOP signal client connected");
		std::shared_ptr<BOOP_Socket_RPC_SIGNAL_Object> tmp_obj(new BOOP_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	BOOP_Socket_RPC_SLOT_Server_Thread::BOOP_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	BOOP_Socket_RPC_SLOT_Server::BOOP_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, BOOP* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void BOOP_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new BOOP_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("BOOP slot thread started");
		exec();
	}

	void BOOP_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<BOOP_Socket_RPC_SLOT_Object>(new BOOP_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	BOOP_Socket_RPC_SLOT_Thread::BOOP_Socket_RPC_SLOT_Thread(BOOP* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{}

	void BOOP_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("BOOP slot client connected");
		std::shared_ptr<BOOP_Socket_RPC_SLOT_Thread> tmp_obj(new BOOP_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	BOOP_Socket_RPC_SLOT_Object::BOOP_Socket_RPC_SLOT_Object(BOOP* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("BOOP_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &BOOP_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["new_message(QVariant, int, int, int, QVariantList, int)"] = &BOOP_Socket_RPC_SLOT_Object::new_message;
		operators_map["new_matrix_command(int, int, int, int, double, double, int, int, int)"] = &BOOP_Socket_RPC_SLOT_Object::new_matrix_command;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	BOOP_Socket_RPC_SIGNAL_Object::BOOP_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("BOOP_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void BOOP_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void BOOP_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void BOOP_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void BOOP_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
	}
	void BOOP_Socket_RPC_SIGNAL_Object::set_app(BOOP* _app)
	{
		app = _app;

	}

	void BOOP_Socket_RPC_SLOT_Object::read_data()
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

	void BOOP_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void BOOP_Socket_RPC_SIGNAL_Object::read_data()
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
	QVariant BOOP_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant BOOP_Socket_RPC_SLOT_Object::new_message(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariant dt = _values.at(0).value<QVariant>();
			int MKO = _values.at(1).value<int>();
			int line = _values.at(2).value<int>();
			int command_word = _values.at(3).value<int>();
			QVariantList words = _values.at(4).value<QVariantList>();
			int respond_word = _values.at(5).value<int>();
			app->new_message(dt, MKO, line, command_word, words, respond_word);
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
	QVariant BOOP_Socket_RPC_SLOT_Object::new_matrix_command(QVariantList& _values)
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
			app->new_matrix_command(mshm, pshm, length_m, length_p, u_m, u_p, dt, line_m, line_p);
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

