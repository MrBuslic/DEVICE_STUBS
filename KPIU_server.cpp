#include "KPIU_server.h"
#include "socket_rpc.h"
#include "rpc_ports.h"
#include "kpiu_socket_rpc.h"
#include "mds32_rpc.h"
#include "mfsk24_rpc.h"
#include "vvk4_rpc.h"
#include "is4_rpc.h"


KPIUServer::KPIUServer(QWidget* parent) : QWidget(parent)
{
	QString ip_str = "127.0.0.1";
	QGridLayout* bus_grd_layout = new QGridLayout(this);
	bus_grd_layout->setVerticalSpacing(10);
	bus_grd_layout->setHorizontalSpacing(10);
	omnibus_widget = new RpcOmnibusWidget(this);
	interrupt_widget = new InterruptWidget(this);
	kpi_widget = new KPIWidget(this);
	mku_widget = new MKUWidget(this);
	frame_widget = new FrameBusWidget(this);
	power_widget = new PowerWidget(this);
	ols_widget = new RpcOlsWidget(this, 0);
	QThread::currentThread()->sleep(2);
	kprd_widget = new KPRD_imitator(this);
	foi_widget = new RpcFoiWidget(this);
	QLabel* omnibus_label = new QLabel("OMNIBUS");
	QLabel* interrupt_label = new QLabel("Interrupt");
	QLabel* kpi_label = new QLabel("KPI");
	QLabel* mku_label = new QLabel("MKU");
	QLabel* frame_label = new QLabel("Frame bus");
	QLabel* power_label = new QLabel("Power bus");
	QLabel* ols_label = new QLabel("Ols");
	QLabel* kprd_label = new QLabel("KPRD");
	QLabel* foi_label = new QLabel("Foi");
	//connect(ols_widget, &RpcOlsWidget::new_ols_data, kprd_widget, &KPRD_imitator::dataIn);
	bus_grd_layout->addWidget(omnibus_label, 0, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(interrupt_label, 0, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(kpi_label, 0, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(omnibus_widget, 1, 0);//отладить 
	bus_grd_layout->addWidget(interrupt_widget, 1, 1);
	bus_grd_layout->addWidget(kpi_widget, 1, 2);
	bus_grd_layout->addWidget(mku_label, 2, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(frame_label, 2, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(power_label, 2, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(mku_widget, 3, 0);
	bus_grd_layout->addWidget(frame_widget, 3, 1);
	bus_grd_layout->addWidget(power_widget, 3, 2, Qt::AlignTop);
	bus_grd_layout->addWidget(ols_label, 4, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(kprd_label, 4, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(foi_label, 4, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(ols_widget, 5, 0, Qt::AlignTop);	
	bus_grd_layout->addWidget(kprd_widget, 5, 1, Qt::AlignTop);
	bus_grd_layout->addWidget(foi_widget, 5, 2);
	
	kpiu_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kpiu_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);

	QProcess::execute(QApplication::applicationDirPath() + "/mongodb_export db");
	QProcess::execute(QApplication::applicationDirPath() + "/db_starter");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_kp50");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_ols 1");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_ols 2");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mds32 0");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mds32 1");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 0");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 1");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 2");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 3");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 5");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 6");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 7");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mfsk24 8");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_ads128 0");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_ads128 1");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mn8i");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_vvk4");
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_is4");
	QProcess::startDetached(QApplication::applicationDirPath() + "/RM_MBK07_imitator");
	QThread::currentThread()->sleep(3);
	QProcess::startDetached(QApplication::applicationDirPath() + "/rpc_mkprm");
	QProcess::startDetached(QApplication::applicationDirPath() + "/sorensen");
	QProcess::startDetached(QApplication::applicationDirPath() + "/IBEP_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/n6705");
	QProcess::startDetached(QApplication::applicationDirPath() + "/cbk_imitator_real_po");
	QThread::currentThread()->sleep(2);
	QProcess::startDetached(QApplication::applicationDirPath() + "/mbk04_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/MBK-02_imitator");
	QThread::currentThread()->sleep(2);
	QProcess::startDetached(QApplication::applicationDirPath() + "/R732_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/R733_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/BOOP_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/MBK-07_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/BECH_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/ASN_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/LKA-05_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/BAU_imitator");
	QProcess::startDetached(QApplication::applicationDirPath() + "/UNVVK4");
	QProcess::startDetached(QApplication::applicationDirPath() + "/UNIS4");
	QProcess::startDetached(QApplication::applicationDirPath() + "/common");
	QProcess::startDetached(QApplication::applicationDirPath() + "/comapp1");
	QProcess::startDetached(QApplication::applicationDirPath() + "/MBK07");
	QProcess::startDetached(QApplication::applicationDirPath() + "/comapp2");
	QProcess::startDetached(QApplication::applicationDirPath() + "/comappFrame");
	QThread::currentThread()->sleep(5);
	QProcess::startDetached(QApplication::applicationDirPath() + "/client --imit");


	rpc_slot_srv->set_params(ip_str, KPIU_SERVER_SLOT);
	rpc_slot_srv->start();
	kpiu_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kpiu_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, KPIU_SERVER_SIGNAL);
	rpc_signal_srv->start();


	RPC_mds32_SLOT_Thread* mds1_slot_thr = new RPC_mds32_SLOT_Thread;
	mds1_slot_thr->set_connection_params("127.0.0.1", MDS_SLOT);
	mds1_slot_thr->start();

	RPC_mds32_SIGNAL_Thread* mds1_signal_thr = new RPC_mds32_SIGNAL_Thread;
	mds1_signal_thr->set_connection_params("127.0.0.1", MDS_SIGNAL);
	mds1_signal_thr->start();

	if (!mds1_slot_thr->wait_connected(3) || !mds1_signal_thr->wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mds32 1");
		this->deleteLater();
		return;
	}
	connect(mds1_signal_thr->get_obj().get(), SIGNAL(mds32_get_sample(uint&, bool&)), this, SLOT(mds_1_get_sample(uint&, bool&)), Qt::DirectConnection);



	RPC_mds32_SIGNAL_Thread* mds2_signal_thr = new RPC_mds32_SIGNAL_Thread;
	mds2_signal_thr->set_connection_params("127.0.0.1", MDS_SIGNAL+1);
	mds2_signal_thr->start();

	if (!mds2_signal_thr->wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mds32 2");
		this->deleteLater();
		return;
	}
	connect(mds2_signal_thr->get_obj().get(), SIGNAL(mds32_get_sample(uint&, bool&)), this, SLOT(mds_2_get_sample(uint&, bool&)), Qt::DirectConnection);


	RPC_mfsk24_SLOT_Thread* mfsk1_slot_thr = new RPC_mfsk24_SLOT_Thread;
	mfsk1_slot_thr->set_connection_params("127.0.0.1", MFSK_SLOT);
	mfsk1_slot_thr->start();

	RPC_mfsk24_SIGNAL_Thread* mfsk1_signal_thr = new RPC_mfsk24_SIGNAL_Thread;
	mfsk1_signal_thr->set_connection_params("127.0.0.1", MFSK_SIGNAL);
	mfsk1_signal_thr->start();

	if (!mfsk1_slot_thr->wait_connected(3) || !mfsk1_signal_thr->wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mfsk24 1");
		this->deleteLater();
		return;
	}
	connect(mfsk1_signal_thr->get_obj().get(), SIGNAL(mfsk24_impulse_change(QVariantList)), this, SLOT(mfsk_1_impulse(QVariantList)), Qt::DirectConnection);

	mds1_chans.chans = 0xFFFFFFFF;
	mds2_chans.chans = 0xFFFFFFFF;

	vvk4_slot_thr = new RPC_vvk4_SLOT_Thread;
	vvk4_slot_thr->set_connection_params("127.0.0.1", VVK_SLOT);
	vvk4_slot_thr->start();

	RPC_vvk4_SIGNAL_Thread* vvk4_signal_thr = new RPC_vvk4_SIGNAL_Thread;
	vvk4_signal_thr->set_connection_params("127.0.0.1", VVK_SIGNAL);
	vvk4_signal_thr->start();

	if (!vvk4_slot_thr->wait_connected(3) || !vvk4_signal_thr->wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с vvk4");
		this->deleteLater();
		return;
	}
	//connect(mfsk1_signal_thr->get_obj().get(), SIGNAL(vvk4_get_channels_list(QVariantList& ei_list, QVariantList& sum_list)), this, SLOT(get_commut_chanels_list(QVariantList& ei_list, QVariantList& sum_list)), Qt::DirectConnection);

	RPC_is4_SLOT_Thread* is4_slot_thr = new RPC_is4_SLOT_Thread;
	is4_slot_thr->set_connection_params("127.0.0.1", IS4_SLOT);
	is4_slot_thr->start();
	RPC_is4_SIGNAL_Thread* is4_signal_thr = new RPC_is4_SIGNAL_Thread;
	is4_signal_thr->set_connection_params("127.0.0.1", IS4_SIGNAL);
	is4_signal_thr->start();

	if (!is4_slot_thr->wait_connected(3) || !is4_signal_thr->wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с is4");
		this->deleteLater();
		return;
	}

	connect(is4_signal_thr->get_obj().get(), SIGNAL(is4_measure(uint, QVariant&)), this, SLOT(get_resistance(uint, QVariant&)), Qt::DirectConnection);

	pyro_state.insert("ПП1_О",  pyro_chan_state(QList<int>() << 111 << 112, 0));
	pyro_state.insert("ПП1_Р",  pyro_chan_state(QList<int>() << 113 << 114, 0));
	pyro_state.insert("ПП2_О",  pyro_chan_state(QList<int>() << 115 << 116, 0));
	pyro_state.insert("ПП2_Р",  pyro_chan_state(QList<int>() << 117 << 118, 0));
	pyro_state.insert("ПП3_О",  pyro_chan_state(QList<int>() << 119 << 120, 0));
	pyro_state.insert("ПП3_Р",  pyro_chan_state(QList<int>() << 121 << 122, 0));
	pyro_state.insert("ПП4_О",  pyro_chan_state(QList<int>() << 123 << 124, 0));
	pyro_state.insert("ПП4_Р",  pyro_chan_state(QList<int>() << 125 << 126, 0));

	pyro_state.insert("ПП5_О",  pyro_chan_state(QList<int>() << 161 << 162, 0));
	pyro_state.insert("ПП5_Р",  pyro_chan_state(QList<int>() << 163 << 164, 0));
	pyro_state.insert("ПП6_О",  pyro_chan_state(QList<int>() << 165 << 166, 0));
	pyro_state.insert("ПП6_Р",  pyro_chan_state(QList<int>() << 167 << 168, 0));

	pyro_state.insert("ПП7_О",  pyro_chan_state(QList<int>() << 169 << 170, 0));
	pyro_state.insert("ПП7_Р",  pyro_chan_state(QList<int>() << 171 << 172, 0));
	pyro_state.insert("ПП8_О",  pyro_chan_state(QList<int>() << 173 << 174, 0));
	pyro_state.insert("ПП8_Р",  pyro_chan_state(QList<int>() << 175 << 176, 0));
	pyro_state.insert("ПП9_О",  pyro_chan_state(QList<int>() << 177 << 178, 0));
	pyro_state.insert("ПП9_Р",  pyro_chan_state(QList<int>() << 179 << 180, 0));
	pyro_state.insert("ПП10_О",  pyro_chan_state(QList<int>() << 181 << 182, 0));
	pyro_state.insert("ПП10_Р",  pyro_chan_state(QList<int>() << 183 << 184, 0));

	pyro_state.insert("ПП11_О",  pyro_chan_state(QList<int>() << 185 << 186, 0));
	pyro_state.insert("ПП11_Р",  pyro_chan_state(QList<int>() << 187 << 188, 0));
	pyro_state.insert("ПП12_О",  pyro_chan_state(QList<int>() << 189 << 190, 0));
	pyro_state.insert("ПП12_Р",  pyro_chan_state(QList<int>() << 191 << 192, 0));
	pyro_state.insert("ПП13_О",  pyro_chan_state(QList<int>() << 193 << 194, 0));
	pyro_state.insert("ПП13_Р",  pyro_chan_state(QList<int>() << 195 << 196, 0));
	pyro_state.insert("ПП14_О",  pyro_chan_state(QList<int>() << 197 << 198, 0));
	pyro_state.insert("ПП14_Р",  pyro_chan_state(QList<int>() << 199 << 200, 0));

	bau_chans.clear();
	bau_chans << 141 << 142 << 143 << 144 << 145 << 146;
}
//mku_bus_setup
int KPIUServer::KU_NASTROYKA_CELOSTNOSTI_KANALOV(int ku_n, int line)
{
	mku_widget->ku_map_channels_setup(ku_n, line);
	return 0;
}
int KPIUServer::MSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int mshm, int line_m)
{
	mku_widget->mshm_map_channels_setup(mshm, line_m);
	return 0;
}
int KPIUServer::PSHM_NASTROYKA_CELOSTNOSTI_KANALOV(int pshm, int line_p)
{
	mku_widget->pshm_map_channels_setup(pshm, line_p);
	return 0;
}

//unfoi_setup
int KPIUServer::FOI_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	foi_widget->unfoi_map_channels_setup(_n, _chan);
	return 0;
}

//unols_setup
int KPIUServer::OLS_ZAPIS_DANNIH_FORMIROVANIYA(QVariantList data_buffer, QVariantList mask_buffer)
{
	ols_widget->unols_write_data_kf(data_buffer, mask_buffer);
	return 0;
}

int KPIUServer::OLS_ZAPIS_DANNIH_REGISTRACII(QVariantList data_buffer)
{
	ols_widget->unols_read_data_kr(data_buffer);
	return 0;
}

int KPIUServer::OLS_BISTRIY_START(int devise)
{
	ols_widget->unols_trigger_imm(devise);
	return 0;
}

int KPIUServer::OLS_CHTENIE_DANNICH_REGISTRACII(QVariantList& data_buffer)
{
	ols_widget->unols_read_data_kr(data_buffer);
	return 0;
}

//KPRD_setup
int KPIUServer::ANTENNA_USTANOVKA_KOMMUTACII(QString antenna_name, QString connected_antenna_name)
{
	kprd_widget->set_antenna_connection(antenna_name, connected_antenna_name);
	return 0;
}

//OMNIBUS_setup
int KPIUServer::OMNIBUS_NASTROYKA_CELOSTNOSTI_KANALOV(int _n, int _chan)
{
	omnibus_widget->unomnibus_map_channels_setup(_n, _chan);
	SRPCSignalClass::Instance().toLog(QString("Присваиваю каналу %1 значение %2").arg(_n).arg(_chan));
	return 0;
}

//power_bus_setup
int KPIUServer::USTANOVIT_SOSTOYANIE_SHINI_PITANIYA(int bus, int state)
{
	power_widget->set_bus_state(bus, state);
	return 0;
}

//pyro
int KPIUServer::PYRO_USTANOVIT_SOSTOYANIE(QString _name, int _state)//Производить установку по названию пир-на?
{
	QMap<QString, pyro_chan_state>::iterator itr = pyro_state.find(_name);

	if (itr == pyro_state.end())
	{
		SRPCSignalClass::Instance().toLog(QString("Соединение %1 не найдено!").arg(_name));
		return 1;
	}
	itr->state = _state;
	return 0;
}

QString KPIUServer::getXML()
{
	QFile file(QCoreApplication::applicationDirPath() + "/kpiu_directives.xml");

	if (file.open(QFile::ReadOnly | QFile::Text))
		return file.readAll();
	else
		return "";
}

enum eControlChan
{
	controlOnCommon = 15,
	controlOnVM0 = 17,
	controlOnVM1 = 18,
	controlOnVM2 = 19,
	controlOnVM3 = 20,

	controlOffCommon = 16,
	controlOffVM0 = 21,
	controlOffVM1 = 22,
	controlOffVM2 = 23,
	controlOffVM3 = 24,

#ifdef OLD_CRATE
	controlHoldVM0 = 1,
	controlHoldVM1 = 2,
	controlHoldVM2 = 3,
	controlHoldVM3 = 4,
#else
	controlHoldVM0 = 21,
	controlHoldVM1 = 22,
	controlHoldVM2 = 23,
	controlHoldVM3 = 24,
#endif
};

void KPIUServer::mfsk_1_impulse(QVariantList channels)
{
	if ((channels.count() % 2) != 0)
	{
		return;
	}
	QMap<int, int> channels_map;
	for (int i = 0; i < channels.count(); i += 2)
	{
		channels_map.insert(channels[i].toInt(), channels[i + 1].toInt());
	}
	if (channels_map.contains(controlOnCommon))
	{
		if ((channels_map[controlOnCommon] < 300) && (channels_map[controlOnCommon] > 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOnVM0 + i) && (channels_map[controlOnVM0 + i] < 300) && (channels_map[controlOnVM0 + i] > 100))
				{
					mku_widget->make_ku_cbk(i, channels_map[controlOnVM0 + i], 27, 3);
				}
			}
		}
	}

	if (channels_map.contains(controlOffCommon))
	{
		if ((channels_map[controlOffCommon] < 300) && (channels_map[controlOffCommon] > 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOffVM0 + i) && (channels_map[controlOffVM0 + i] < 300) && (channels_map[controlOffVM0 + i] > 100))
				{
					mku_widget->make_ku_cbk(i+4, channels_map[controlOffVM0 + i], 27, 3);
				}
			}
		}
	}
}

void KPIUServer::mds_1_get_sample(uint& buf, bool& flag)
{
	flag = true;
	QVariant tmp_buf;
	mku_widget->get_tm("CBK_TM", tmp_buf);
	mds1_chans.CBK_chans = (~tmp_buf.toUInt()) & 0x1FF;

	mku_widget->get_tm("733_TM", tmp_buf);
	mds1_chans.chans_733 = ((~tmp_buf.toUInt()) & 0x1F);

	buf = mds1_chans.chans;
}

void KPIUServer::mds_2_get_sample(uint& buf, bool& flag)
{
	flag = true;
	QVariant tmp_buf;
	mku_widget->get_tm("ONA_TM", tmp_buf);
	mds2_chans.ONA_chans = (~tmp_buf.toUInt()) & 0x3;

	mku_widget->get_tm("732_TM", tmp_buf);
	mds2_chans.chans_732 = ((~tmp_buf.toUInt()) & 0x1F);

	buf = mds2_chans.chans;
}

void KPIUServer::get_resistance(uint NProcess, QVariant& resistance)
{
	QVariantList ei_chanels_list;
	QVariantList sum_chanels_list;
	int connection_state;
	QVariantList channels;
	vvk4_slot_thr->get_vvk4_obj()->get_commut_chanels_list(ei_chanels_list, sum_chanels_list);

	if (sum_chanels_list.contains(bau_ground))
	{
		resistance = 13000000;
		if (ei_chanels_list.empty())
			return;
		int ei_chan = ei_chanels_list.at(0).toInt();

		int bau_ind = bau_chans.indexOf(ei_chan);
		if (bau_ind == -1)
			return;

		QVariant bau_tm;
		mku_widget->get_tm("BAU_TM", bau_tm);

		if ( (bau_tm.toUInt() & (1 << bau_ind)) != 0)
			resistance = 0.8;
	}
	else
	{
		channels = ei_chanels_list + sum_chanels_list;
		// вставить функцию опроса ВВК
		connection_state = get_connection_state(channels);
		switch (connection_state)
		{
		case 1:
			resistance = 10;
		case 2:
			resistance = 0.8;
		case 3:
			resistance = 13000000;
		}
	}
}
	//get_commut_chanels_list(ei_list, sum_list);

int KPIUServer::get_connection_state(QVariantList& _chans)
{
	QList<pyro_chan_state> connections = pyro_state.values();
	QList<pyro_chan_state>::iterator it = connections.begin();
	for (; it != connections.end(); it++)
	{
		if (((it->chan_num[0] == _chans[0]) && (it->chan_num[1] == _chans[1])) || ((it->chan_num[1] == _chans[0]) && (it->chan_num[0] == _chans[1])))
			//((it->chan_num[0] == _chans[0]) || (it->chan_num[1] == _chans[0]) || (it->chan_num[0] == _chans[1]) || (it->chan_num[1] == _chans[1]))//todo убрать
			return it->state;
	}
	SRPCSignalClass::Instance().toLog(QString("Не найдено соединения каналов пиропатронов %1 %2").arg(_chans[0].toString()).arg(_chans[1].toString()));
	return 0; //возвращать -1?
}