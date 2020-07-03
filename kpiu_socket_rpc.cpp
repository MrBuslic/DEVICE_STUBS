	#include "kpiu_socket_rpc.h"

int kpiu_Socket_RPC_SLOT_Object::obj_num = 0;
int kpiu_Socket_RPC_SIGNAL_Object::obj_num = 0;
int kpiu_Socket_RPC_SIGNAL_Object::call_number = 0;
int kpiu_Socket_RPC_SLOT_Thread::obj_num = 0;

	kpiu_Socket_RPC_SIGNAL_Thread::kpiu_Socket_RPC_SIGNAL_Thread() : QThread()
	{
		setObjectName("kpiu_Socket_RPC_SIGNAL_Thread");
	}

	void kpiu_Socket_RPC_SIGNAL_Thread::run()
	{
		rpc_srv = new kpiu_Socket_RPC_SIGNAL_Server(conn_ip, conn_port);
		rpc_srv->set_app(app);
		SRPCSignalClass::Instance().toLog("kpiu signal thread started");
		exec();
	}

	kpiu_Socket_RPC_SIGNAL_Server::kpiu_Socket_RPC_SIGNAL_Server(QString _conn_ip, int _conn_port)
	{
		rpc_server = new QTcpServer;
		connect(rpc_server, SIGNAL(newConnection()), this, SLOT(tcp_slot()));
		rpc_server->listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("signal server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));

	}

	void kpiu_Socket_RPC_SIGNAL_Server::tcp_slot()
	{
		SRPCSignalClass::Instance().toLog("kpiu signal client connected");
		std::shared_ptr<kpiu_Socket_RPC_SIGNAL_Object> tmp_obj(new kpiu_Socket_RPC_SIGNAL_Object);
		tmp_obj->set_app(app);
		tmp_obj->set_socket(rpc_server->nextPendingConnection());
		rpc_objects << tmp_obj;
	}

	kpiu_Socket_RPC_SLOT_Server_Thread::kpiu_Socket_RPC_SLOT_Server_Thread() : QThread()
	{
		setObjectName("Socket_RPC_SLOT_Server_Thread");
	}

	kpiu_Socket_RPC_SLOT_Server::kpiu_Socket_RPC_SLOT_Server(QString _conn_ip, int _conn_port, KPIUServer* _app) : QTcpServer(), app(_app)
	{
		listen(((_conn_ip == "") ? QHostAddress::Any : QHostAddress(_conn_ip)), _conn_port);
		SRPCSignalClass::Instance().toLog(QString("slot server started listen ip %1 port %2").arg(_conn_ip).arg(_conn_port));
	}


	void kpiu_Socket_RPC_SLOT_Server_Thread::run()
	{
		rpc_srv = new kpiu_Socket_RPC_SLOT_Server(conn_ip, conn_port, app);
		SRPCSignalClass::Instance().toLog("kpiu slot thread started");
		exec();
	}

	void kpiu_Socket_RPC_SLOT_Thread::run()
	{
		rpc_obj = std::shared_ptr<kpiu_Socket_RPC_SLOT_Object>(new kpiu_Socket_RPC_SLOT_Object(app, socketDescriptor));
		exec();
	}

	kpiu_Socket_RPC_SLOT_Thread::kpiu_Socket_RPC_SLOT_Thread(KPIUServer* _app, int _socketDescriptor) : app(_app), socketDescriptor(_socketDescriptor)
	{
	setObjectName(QString("kpiu_Socket_RPC_SLOT_Thread_%1").arg(obj_num++));
	}

	void kpiu_Socket_RPC_SLOT_Server::incomingConnection(qintptr socketDescriptor)
	{
		SRPCSignalClass::Instance().toLog("kpiu slot client connected");
		std::shared_ptr<kpiu_Socket_RPC_SLOT_Thread> tmp_obj(new kpiu_Socket_RPC_SLOT_Thread(app, socketDescriptor));
		tmp_obj->start();
		rpc_objects << tmp_obj;
	}

	kpiu_Socket_RPC_SLOT_Object::kpiu_Socket_RPC_SLOT_Object(KPIUServer* _app, int socketDescriptor) : QObject(), with_return(false), app(_app)
	{
	setObjectName(QString("kpiu_SLOT_Object_%1").arg(obj_num++));
		operators_map["QuerySlots()"] = &kpiu_Socket_RPC_SLOT_Object::QuerySlots;
		///////////////////////////////////////////////////////////////////////
		operators_map["KU_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::KU_NASTROYKA_CELOSTNOSTI_KANALOV;
		operators_map["MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::MSHM_NASTROYKA_CELOSTNOSTI_KANALOV;
		operators_map["PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::PSHM_NASTROYKA_CELOSTNOSTI_KANALOV;
		operators_map["FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::FOI_NASTROYKA_CELOSTNOSTI_KANALOV;
		operators_map["ANTENNA_USTANOVKA_KOMMUTACII(QString, QString)"] = &kpiu_Socket_RPC_SLOT_Object::ANTENNA_USTANOVKA_KOMMUTACII;
		operators_map["OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV;
		operators_map["USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int, int)"] = &kpiu_Socket_RPC_SLOT_Object::USTANOVIT_SOSTOYANIE_SHINI_PITANIYA;
		operators_map["PYRO_USTANOVIT_SOSTOYANIE(QString, int)"] = &kpiu_Socket_RPC_SLOT_Object::PYRO_USTANOVIT_SOSTOYANIE;
		operators_map["LKA05_KU_USTANOVIT_SOSTOYANIE(int, int, bool)"] = &kpiu_Socket_RPC_SLOT_Object::LKA05_KU_USTANOVIT_SOSTOYANIE;
		operators_map["LKA05_MK_USTANOVIT_SOSTOYANIE(int, int, bool)"] = &kpiu_Socket_RPC_SLOT_Object::LKA05_MK_USTANOVIT_SOSTOYANIE;
		operators_map["LKA05_MPVN_USTANOVIT_SOSTOYANIE(int, bool)"] = &kpiu_Socket_RPC_SLOT_Object::LKA05_MPVN_USTANOVIT_SOSTOYANIE;
		operators_map["LKA05_MU_USTANOVIT_SOSTOYANIE(int, bool)"] = &kpiu_Socket_RPC_SLOT_Object::LKA05_MU_USTANOVIT_SOSTOYANIE;
		operators_map["getXML()"] = &kpiu_Socket_RPC_SLOT_Object::getXML;
		operators_map["mfsk_2_impulse(QVariantList)"] = &kpiu_Socket_RPC_SLOT_Object::mfsk_2_impulse;
		operators_map["mds_1_get_sample(uint&, bool&)"] = &kpiu_Socket_RPC_SLOT_Object::mds_1_get_sample;
		operators_map["mds_2_get_sample(uint&, bool&)"] = &kpiu_Socket_RPC_SLOT_Object::mds_2_get_sample;
		operators_map["get_resistance(uint, QVariant&)"] = &kpiu_Socket_RPC_SLOT_Object::get_resistance;
		operators_map["get_connection_state(QVariantList&)"] = &kpiu_Socket_RPC_SLOT_Object::get_connection_state;
		operators_map["mshr_data_in(QVariantList, QVariantList)"] = &kpiu_Socket_RPC_SLOT_Object::mshr_data_in;
		operators_map["mshr_data_out(QVariantList&)"] = &kpiu_Socket_RPC_SLOT_Object::mshr_data_out;
		operators_map["power_bus_state_changed(QString, double)"] = &kpiu_Socket_RPC_SLOT_Object::power_bus_state_changed;
		operators_map["activate_pyro(int)"] = &kpiu_Socket_RPC_SLOT_Object::activate_pyro;
		///////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////
		rpc_socket = new QTcpSocket();
		rpc_socket->setSocketDescriptor(socketDescriptor);
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	kpiu_Socket_RPC_SIGNAL_Object::kpiu_Socket_RPC_SIGNAL_Object() : QObject()
	{
		setObjectName(QString("kpiu_SIGNAL_Object_%1").arg(obj_num++));
		connect(this, SIGNAL(send_signal(QByteArray*)), this, SLOT(send_signal_slot(QByteArray*)), Qt::BlockingQueuedConnection);
	}

	void kpiu_Socket_RPC_SIGNAL_Object::send_signal_func(QByteArray* _arr)
	{
		QMutexLocker locker(&signal_mutex);
		emit send_signal(_arr);
	}

	void kpiu_Socket_RPC_SLOT_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SLOT SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
	}

	void kpiu_Socket_RPC_SIGNAL_Object::set_socket(QTcpSocket* _rpc_socket)
	{
		rpc_socket = _rpc_socket;
		connect(rpc_socket, SIGNAL(readyRead()), this, SLOT(read_data()));
		connect(rpc_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(sock_error(QAbstractSocket::SocketError)));
	}

	void kpiu_Socket_RPC_SIGNAL_Object::sock_error(QAbstractSocket::SocketError _err)
	{
		SRPCSignalClass::Instance().toLog(QString("%1 SIGNAL SOCK ERROR!!! %2").arg(this->objectName()).arg(_err));
		if (_err == QAbstractSocket::SocketError::SocketTimeoutError)
			return;
		disconnect(app, SIGNAL(string_msg(QString)), this, SLOT(string_msg(QString)));
		disconnect(app, SIGNAL(int_msg(int)), this, SLOT(int_msg(int)));
		disconnect(app, SIGNAL(int_return_signal(int&)), this, SLOT(int_return_signal(int&)));
		disconnect(app, SIGNAL(toLogs(QString)), this, SLOT(toLogs(QString)));
		disconnect(app, SIGNAL(toProtocols(QString)), this, SLOT(toProtocols(QString)));
	}
	void kpiu_Socket_RPC_SIGNAL_Object::set_app(KPIUServer* _app)
	{
		app = _app;
		connect(app, SIGNAL(string_msg(QString)), this, SLOT(string_msg(QString)), Qt::DirectConnection);
		data_map.insert("string_msg(QString)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(int_msg(int)), this, SLOT(int_msg(int)), Qt::DirectConnection);
		data_map.insert("int_msg(int)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(int_return_signal(int&)), this, SLOT(int_return_signal(int&)), Qt::DirectConnection);
		data_map.insert("int_return_signal(int&)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(toLogs(QString)), this, SLOT(toLogs(QString)), Qt::DirectConnection);
		data_map.insert("toLogs(QString)", std::shared_ptr<SignalData>(new SignalData()));
		connect(app, SIGNAL(toProtocols(QString)), this, SLOT(toProtocols(QString)), Qt::DirectConnection);
		data_map.insert("toProtocols(QString)", std::shared_ptr<SignalData>(new SignalData()));

	}

	void kpiu_Socket_RPC_SLOT_Object::read_data()
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

	void kpiu_Socket_RPC_SIGNAL_Object::send_signal_slot(QByteArray* _arr)
	{
		rpc_socket->write(*_arr);
		rpc_socket->waitForBytesWritten(3000);
	}

	void kpiu_Socket_RPC_SIGNAL_Object::read_data()
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

	void kpiu_Socket_RPC_SIGNAL_Object::string_msg(QString _msg)
	{
		auto& descriptor = *data_map["string_msg(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("string_msg(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal string_msg  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << _msg;
		SRPCSignalClass::Instance().toLog(QString("string_msg  call_number %2 _msg =  %1").arg(RPCSignalClass::QVariantToString(_msg)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal string_msg sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal string_msg finished").arg(objectName()));
	}
	void kpiu_Socket_RPC_SIGNAL_Object::int_msg(int _msg)
	{
		auto& descriptor = *data_map["int_msg(int)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("int_msg(int)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal int_msg  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << _msg;
		SRPCSignalClass::Instance().toLog(QString("int_msg  call_number %2 _msg =  %1").arg(RPCSignalClass::QVariantToString(_msg)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal int_msg sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal int_msg finished").arg(objectName()));
	}
	void kpiu_Socket_RPC_SIGNAL_Object::int_return_signal(int& ret_int)
	{
		auto& descriptor = *data_map["int_return_signal(int&)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("int_return_signal(int&)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal int_return_signal  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << ret_int;
		SRPCSignalClass::Instance().toLog(QString("int_return_signal  call_number %2 ret_int =  %1").arg(RPCSignalClass::QVariantToString(ret_int)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal int_return_signal sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		ret_int = data_map["int_return_signal(int&)"]->signal_data.at(0).toInt();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal int_return_signal finished").arg(objectName()));
	}
	void kpiu_Socket_RPC_SIGNAL_Object::toLogs(QString message)
	{
		auto& descriptor = *data_map["toLogs(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("toLogs(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal toLogs  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << message;
		SRPCSignalClass::Instance().toLog(QString("toLogs  call_number %2 message =  %1").arg(RPCSignalClass::QVariantToString(message)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toLogs sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toLogs finished").arg(objectName()));
	}
	void kpiu_Socket_RPC_SIGNAL_Object::toProtocols(QString message)
	{
		auto& descriptor = *data_map["toProtocols(QString)"].get();
		if (!descriptor.signal_needed)
			return;
		QByteArray tmp_arr;
		QDataStream tmp_stream(&tmp_arr, QIODevice::WriteOnly);
		tmp_stream << QString("toProtocols(QString)");
		tmp_stream << (++call_number);
		SRPCSignalClass::Instance().toLog(QString("%1 from thread %2 send_signal toProtocols  call_number %3").arg(objectName()).arg(QThread::currentThread()->objectName()).arg(call_number));
		tmp_stream << message;
		SRPCSignalClass::Instance().toLog(QString("toProtocols  call_number %2 message =  %1").arg(RPCSignalClass::QVariantToString(message)).arg(call_number));
		QByteArray tmp_arr2;
		QDataStream tmp_stream2(&tmp_arr2, QIODevice::WriteOnly);
		tmp_stream2 << tmp_arr.size();
		tmp_arr2 += tmp_arr;
		descriptor.mutex.lock();
		send_signal_func(&tmp_arr2);
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toProtocols sended").arg(objectName()));
		descriptor.mutex.lock();
		descriptor.mutex.unlock();
		SRPCSignalClass::Instance().toLog(QString("%1 send_signal toProtocols finished").arg(objectName()));
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	QVariant kpiu_Socket_RPC_SLOT_Object::QuerySlots(QVariantList& _values)
	{
		QString tmp_string;
		int _count = operators_map.keys().count();
		for (int i = 0; i < _count; ++i)
			tmp_string += operators_map.keys().at(i);
		return tmp_string;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	QVariant kpiu_Socket_RPC_SLOT_Object::KU_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int ku_n = _values.at(0).value<int>();
			int line = _values.at(1).value<int>();
			int res = app->KU_NASTROYKA_CELOSTNOSTI_KANALOV(ku_n, line);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int mshm = _values.at(0).value<int>();
			int line_m = _values.at(1).value<int>();
			int res = app->MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(mshm, line_m);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int pshm = _values.at(0).value<int>();
			int line_p = _values.at(1).value<int>();
			int res = app->PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(pshm, line_p);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::FOI_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _n = _values.at(0).value<int>();
			int _chan = _values.at(1).value<int>();
			int res = app->FOI_NASTROYKA_CELOSTNOSTI_KANALOV(_n, _chan);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::ANTENNA_USTANOVKA_KOMMUTACII(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString antenna_name = _values.at(0).value<QString>();
			QString connected_antenna_name = _values.at(1).value<QString>();
			int res = app->ANTENNA_USTANOVKA_KOMMUTACII(antenna_name, connected_antenna_name);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _n = _values.at(0).value<int>();
			int _chan = _values.at(1).value<int>();
			int res = app->OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(_n, _chan);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int bus = _values.at(0).value<int>();
			int state = _values.at(1).value<int>();
			int res = app->USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(bus, state);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::PYRO_USTANOVIT_SOSTOYANIE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString name = _values.at(0).value<QString>();
			int state = _values.at(1).value<int>();
			int res = app->PYRO_USTANOVIT_SOSTOYANIE(name, state);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::LKA05_KU_USTANOVIT_SOSTOYANIE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int module_num = _values.at(0).value<int>();
			int _dev = _values.at(1).value<int>();
			bool _flag = _values.at(2).value<bool>();
			int res = app->LKA05_KU_USTANOVIT_SOSTOYANIE(module_num, _dev, _flag);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::LKA05_MK_USTANOVIT_SOSTOYANIE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int module_num = _values.at(0).value<int>();
			int _dev = _values.at(1).value<int>();
			bool _flag = _values.at(2).value<bool>();
			int res = app->LKA05_MK_USTANOVIT_SOSTOYANIE(module_num, _dev, _flag);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::LKA05_MPVN_USTANOVIT_SOSTOYANIE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _dev = _values.at(0).value<int>();
			bool _flag = _values.at(1).value<bool>();
			int res = app->LKA05_MPVN_USTANOVIT_SOSTOYANIE(_dev, _flag);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::LKA05_MU_USTANOVIT_SOSTOYANIE(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int _dev = _values.at(0).value<int>();
			bool _flag = _values.at(1).value<bool>();
			int res = app->LKA05_MU_USTANOVIT_SOSTOYANIE(_dev, _flag);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::getXML(QVariantList& _values)
	{
		try
		{
			QString res = app->getXML();
			SRPCSignalClass::Instance().toLog(QString("%1 return = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(res)));
			return res;
		}
		catch(const std::exception &)
		{
			return QString();
		}
		catch(...)
		{
			return QString();
		}
	}
	QVariant kpiu_Socket_RPC_SLOT_Object::mfsk_2_impulse(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList channels = _values.at(0).value<QVariantList>();
			app->mfsk_2_impulse(channels);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::mds_1_get_sample(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint buf = _values.at(0).value<uint>();
			bool flag = _values.at(1).value<bool>();
			app->mds_1_get_sample(buf, flag);
			_values[0] = buf;
			SRPCSignalClass::Instance().toLog(QString("%1 buf = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			_values[1] = flag;
			SRPCSignalClass::Instance().toLog(QString("%1 flag = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
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
	QVariant kpiu_Socket_RPC_SLOT_Object::mds_2_get_sample(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint buf = _values.at(0).value<uint>();
			bool flag = _values.at(1).value<bool>();
			app->mds_2_get_sample(buf, flag);
			_values[0] = buf;
			SRPCSignalClass::Instance().toLog(QString("%1 buf = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
			_values[1] = flag;
			SRPCSignalClass::Instance().toLog(QString("%1 flag = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
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
	QVariant kpiu_Socket_RPC_SLOT_Object::get_resistance(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			uint NProcess = _values.at(0).value<uint>();
			QVariant resistance = _values.at(1).value<QVariant>();
			app->get_resistance(NProcess, resistance);
			_values[1] = resistance;
			SRPCSignalClass::Instance().toLog(QString("%1 resistance = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[1])));
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
	QVariant kpiu_Socket_RPC_SLOT_Object::get_connection_state(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList _chans = _values.at(0).value<QVariantList>();
			int res = app->get_connection_state(_chans);
			_values[0] = _chans;
			SRPCSignalClass::Instance().toLog(QString("%1 _chans = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
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
	QVariant kpiu_Socket_RPC_SLOT_Object::mshr_data_in(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList dataList = _values.at(0).value<QVariantList>();
			QVariantList maskList = _values.at(1).value<QVariantList>();
			app->mshr_data_in(dataList, maskList);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::mshr_data_out(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QVariantList data = _values.at(0).value<QVariantList>();
			app->mshr_data_out(data);
			_values[0] = data;
			SRPCSignalClass::Instance().toLog(QString("%1 data = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values[0])));
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
	QVariant kpiu_Socket_RPC_SLOT_Object::power_bus_state_changed(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			QString name = _values.at(0).value<QString>();
			double u = _values.at(1).value<double>();
			app->power_bus_state_changed(name, u);
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
	QVariant kpiu_Socket_RPC_SLOT_Object::activate_pyro(QVariantList& _values)
	{
		try
		{
			SRPCSignalClass::Instance().toLog(QString("%1 _values = %2").arg(objectName()).arg(RPCSignalClass::QVariantToString(_values)));
			int group_num = _values.at(0).value<int>();
			app->activate_pyro(group_num);
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

