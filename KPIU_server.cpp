#include "KPIU_server.h"
#include "socket_rpc.h"
#include "rpc_ports.h"
#include "kpiu_socket_rpc.h"
#include "mds32_rpc.h"
#include "mfsk24_rpc.h"
#include "vvk4_rpc.h"
#include "is4_rpc.h"
#include "ols_rpc.h"
#include "unis4.h"

KPIUServer::KPIUServer(QWidget* parent, QString platform) : QWidget(parent), rm_type(platform), bufar_chan(0)
{
	QString ip_str = "127.0.0.1";
	QVBoxLayout* main_lay = new QVBoxLayout(this);
	QTabWidget* tab_widget = new QTabWidget(this);
	main_lay->addWidget(tab_widget);

	QWidget* bus_widget = new QWidget(this);


	QGridLayout* bus_grd_layout = new QGridLayout(bus_widget);

	bus_grd_layout->setVerticalSpacing(10);
	bus_grd_layout->setHorizontalSpacing(10);

	tab_widget->addTab(bus_widget, "Buses");
	
	omnibus_widget = new RpcOmnibusWidget(this);
	interrupt_widget = new InterruptWidget(this);
	kpi_widget = new KPIWidget(this);
	mku_widget = new MKUWidget(this);
	frame_widget = new FrameBusWidget(this);
	power_widget = new PowerWidget(this);


	QLabel* omnibus_label = new QLabel("OMNIBUS");
	QLabel* interrupt_label = new QLabel("Interrupt");
	QLabel* kpi_label = new QLabel("KPI");
	QLabel* mku_label = new QLabel("MKU");
	QLabel* frame_label = new QLabel("Frame bus");
	QLabel* power_label = new QLabel("Power bus");

	bus_grd_layout->addWidget(omnibus_label, 0, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(interrupt_label, 0, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(kpi_label, 0, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(omnibus_widget, 1, 0);
	bus_grd_layout->addWidget(interrupt_widget, 1, 1);
	bus_grd_layout->addWidget(kpi_widget, 1, 2);
	bus_grd_layout->addWidget(mku_label, 2, 0, Qt::AlignCenter);
	bus_grd_layout->addWidget(frame_label, 2, 1, Qt::AlignCenter);
	bus_grd_layout->addWidget(power_label, 2, 2, Qt::AlignCenter);
	bus_grd_layout->addWidget(mku_widget, 3, 0);
	bus_grd_layout->addWidget(frame_widget, 3, 1);	
	bus_grd_layout->addWidget(power_widget, 3, 2, Qt::AlignTop);
	if ((rm_type == "bkupi") || (rm_type == "mca"))
	{

		QWidget* ols_widget = new QWidget(this);
		QWidget* device_1_widget = new QWidget(this);
		QWidget* device_2_widget = new QWidget(this);
		QWidget* device_3_widget = new QWidget(this);
		QWidget* device_4_widget = new QWidget(this);
		QWidget* mfsk24_widget = new QWidget(this);
		QGridLayout* ols_grd_layout = new QGridLayout(ols_widget);
		QGridLayout* device_1_grd_layout = new QGridLayout(device_1_widget);
		QGridLayout* device_2_grd_layout = new QGridLayout(device_2_widget);
		QGridLayout* device_3_grd_layout = new QGridLayout(device_3_widget);
		QGridLayout* device_4_grd_layout = new QGridLayout(device_4_widget);
		QGridLayout* mfsk24_grd_layout = new QGridLayout(mfsk24_widget);
		ols_grd_layout->setVerticalSpacing(10);
		ols_grd_layout->setHorizontalSpacing(10);

		device_1_grd_layout->setVerticalSpacing(10);
		device_1_grd_layout->setHorizontalSpacing(10);
		tab_widget->addTab(ols_widget, "OLS Imitators");
		tab_widget->addTab(device_1_widget, "Devices 1 page");
		tab_widget->addTab(device_2_widget, "Devices 2 page");
		tab_widget->addTab(device_3_widget, "Devices 3 page");
		tab_widget->addTab(device_4_widget, "Devices 4 page");
		tab_widget->addTab(mfsk24_widget, "MFSK24 Imitators");


		QLabel* kprd_label = new QLabel("KPRD");
		QLabel* foi_label = new QLabel("Foi");
		QLabel* kp50_label = new QLabel("KP50");
		QLabel* ols_0_label = new QLabel("Ols_0");
		QLabel* ols_1_label = new QLabel("Ols_1");
		QLabel* ols_2_label = new QLabel("Ols_2");
		QLabel* ols_3_label = new QLabel("Ols_3");
		QLabel* ols_4_label = new QLabel("Ols_4");
		QLabel* mds32_0_label = new QLabel("mds32_0");
		QLabel* mds32_1_label = new QLabel("mds32_1");
		QLabel* mfsk24_0_label = new QLabel("mfsk24_0");
		QLabel* mfsk24_1_label = new QLabel("mfsk24_1");
		QLabel* mfsk24_2_label = new QLabel("mfsk24_2");
		QLabel* mfsk24_3_label = new QLabel("mfsk24_3");
		QLabel* mfsk24_4_label = new QLabel("mfsk24_4");
		QLabel* mfsk24_5_label = new QLabel("mfsk24_5");
		QLabel* mfsk24_6_label = new QLabel("mfsk24_6");
		QLabel* mfsk24_7_label = new QLabel("mfsk24_7");
		QLabel* mfsk24_8_label = new QLabel("mfsk24_8");
		QLabel* mt8k4l_0_label = new QLabel("mt8k4l_0");
		QLabel* mt8k4l_1_label = new QLabel("mt8k4l_1");
		QLabel* mt8k4l_2_label = new QLabel("mt8k4l_2");
		QLabel* ads128_0_label = new QLabel("ads128_0");
		QLabel* ads128_1_label = new QLabel("ads128_1");
		QLabel* mn8i_label = new QLabel("mn8i");
		QLabel* vvk4_label = new QLabel("vvk4");
		QLabel* is4_label = new QLabel("is4");
		QLabel* rm_mbk07_label = new QLabel("rm_mbk07");
		QLabel* mkprm_label = new QLabel("mkprm");
		QLabel* sorensen_label = new QLabel("sorensen");
		QLabel* n6707_label = new QLabel("n6707");
		QLabel* mki_label = new QLabel("MKI");
		QLabel* ibep_label = new QLabel("IBEP");

		ols_0_widget = new RpcOlsWidget(0);
		ols_1_widget = new RpcOlsWidget(0, 1);
		ols_2_widget = new RpcOlsWidget(0, 2);
		ols_3_widget = new RpcOlsWidget(0, 3);
		ols_4_widget = new RpcOlsWidget(0, 4);
		QThread::currentThread()->msleep(2000);
		kprd_widget = new KPRD_imitator(this);
		foi_widget = new RpcFoiWidget(this);
		kp50_widget = new RpcKP50Widget();
		mds32_0_widget = new RpcMDS32Widget(0);
		mds32_1_widget = new RpcMDS32Widget(1);
		mfsk24_0_widget = new RpcMFSK24Widget(0);
		mfsk24_1_widget = new RpcMFSK24Widget(1);
		mfsk24_2_widget = new RpcMFSK24Widget(2);
		mfsk24_3_widget = new RpcMFSK24Widget(3);
		mfsk24_4_widget = new RpcMFSK24Widget(4);
		mfsk24_5_widget = new RpcMFSK24Widget(5);
		mfsk24_6_widget = new RpcMFSK24Widget(6);
		mfsk24_7_widget = new RpcMFSK24Widget(7);
		mfsk24_8_widget = new RpcMFSK24Widget(8);
		ads128_0_widget = new RpcADS128Widget(0);
		ads128_1_widget = new RpcADS128Widget(1);

		mt8k4l_0_widget = new RpcMT8K4LWidget(MT8K4L_SLOT, MT8K4L_SIGNAL);
		mt8k4l_1_widget = new RpcMT8K4LWidget(MT8K4L_SLOT + 1, MT8K4L_SIGNAL + 1);
		mt8k4l_2_widget = new RpcMT8K4LWidget(MT8K4L_SLOT + 2, MT8K4L_SIGNAL + 2);

		mn8i_widget = new RpcMN8IWidget(0);
		vvk4_widget = new RpcVvk4Widget;
		is4_widget = new RpcIS4Widget(0);
		rm_mbk07_widget = new RM_MBK07_imitator;
		mkprm_widget = new RpcMKPRMWidget;
		sorensen_widget = new SORENSENWidget;
		n6705_widget = new N6705Widget;
		ibep_widget = new IBEP_imitator;

		ols_grd_layout->addWidget(ols_0_label, 0, 0, Qt::AlignCenter);
		ols_grd_layout->addWidget(ols_1_label, 0, 1, Qt::AlignCenter);
		ols_grd_layout->addWidget(ols_2_label, 0, 2, Qt::AlignCenter);
		ols_grd_layout->addWidget(ols_0_widget, 1, 0, Qt::AlignTop);
		ols_grd_layout->addWidget(ols_1_widget, 1, 1, Qt::AlignTop);
		ols_grd_layout->addWidget(ols_2_widget, 1, 2, Qt::AlignTop);
		ols_grd_layout->addWidget(ols_3_label, 2, 0, Qt::AlignCenter);
		ols_grd_layout->addWidget(ols_4_label, 2, 1, Qt::AlignCenter);
		ols_grd_layout->addWidget(kprd_label, 2, 2, Qt::AlignCenter);
		ols_grd_layout->addWidget(ols_3_widget, 3, 0, Qt::AlignTop);
		ols_grd_layout->addWidget(ols_4_widget, 3, 1, Qt::AlignTop);
		ols_grd_layout->addWidget(kprd_widget, 3, 2, Qt::AlignTop);

		device_1_grd_layout->addWidget(mds32_0_label, 0, 0, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mds32_1_label, 0, 1, Qt::AlignCenter);
		device_1_grd_layout->addWidget(foi_label, 0, 2, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mds32_0_widget, 1, 0, Qt::AlignTop);
		device_1_grd_layout->addWidget(mds32_1_widget, 1, 1, Qt::AlignTop);
		device_1_grd_layout->addWidget(foi_widget, 1, 2);
		device_1_grd_layout->addWidget(mt8k4l_0_label, 2, 0, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mt8k4l_1_label, 2, 1, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mt8k4l_2_label, 2, 2, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mt8k4l_0_widget, 3, 0, Qt::AlignCenter);
		device_1_grd_layout->addWidget(mt8k4l_1_widget, 3, 1);
		device_1_grd_layout->addWidget(mt8k4l_2_widget, 3, 2);

		device_2_grd_layout->addWidget(ads128_0_label, 0, 0, Qt::AlignCenter);
		device_2_grd_layout->addWidget(ads128_1_label, 0, 1, Qt::AlignCenter);
		device_2_grd_layout->addWidget(mn8i_label, 0, 2, Qt::AlignCenter);
		device_2_grd_layout->addWidget(ads128_0_widget, 1, 0);
		device_2_grd_layout->addWidget(ads128_1_widget, 1, 1);
		device_2_grd_layout->addWidget(mn8i_widget, 1, 2);
		device_2_grd_layout->addWidget(is4_label, 2, 0, Qt::AlignCenter);
		device_2_grd_layout->addWidget(vvk4_label, 2, 1, Qt::AlignCenter);
		//device_2_grd_layout->addWidget(rm_mbk07_label, 2, 2, Qt::AlignCenter);
		device_2_grd_layout->addWidget(is4_widget, 3, 0);
		device_2_grd_layout->addWidget(vvk4_widget, 3, 1);
		//device_2_grd_layout->addWidget(rm_mbk07_widget, 3, 2, Qt::AlignTop);

		device_3_grd_layout->addWidget(sorensen_label, 0, 0, Qt::AlignCenter);
		device_3_grd_layout->addWidget(n6707_label, 0, 1, Qt::AlignCenter);
		device_3_grd_layout->addWidget(ibep_label, 0, 2);
		device_3_grd_layout->addWidget(sorensen_widget, 1, 0, Qt::AlignCenter);
		device_3_grd_layout->addWidget(n6705_widget, 1, 1, Qt::AlignTop);
		device_3_grd_layout->addWidget(ibep_widget, 1, 2, Qt::AlignCenter);
		device_3_grd_layout->addWidget(kp50_label, 2, 0, Qt::AlignCenter);
		device_3_grd_layout->addWidget(kp50_widget, 3, 0, Qt::AlignCenter);

		device_4_grd_layout->addWidget(rm_mbk07_label, 0, 0, Qt::AlignCenter);
		device_4_grd_layout->addWidget(mkprm_label, 0, 1, Qt::AlignCenter);
		device_4_grd_layout->addWidget(mki_label, 0, 2, Qt::AlignCenter);
		device_4_grd_layout->addWidget(rm_mbk07_widget, 1, 0, Qt::AlignTop);
		device_4_grd_layout->addWidget(mkprm_widget, 1, 1, Qt::AlignTop);
		//device_4_grd_layout->addWidget(mki_widget, 1, 2);

		mfsk24_grd_layout->addWidget(mfsk24_0_label, 0, 0, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_1_label, 0, 1, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_2_label, 0, 2, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_0_widget, 1, 0);//отладить 
		mfsk24_grd_layout->addWidget(mfsk24_1_widget, 1, 1);
		mfsk24_grd_layout->addWidget(mfsk24_2_widget, 1, 2);
		mfsk24_grd_layout->addWidget(mfsk24_3_label, 2, 0, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_4_label, 2, 1, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_5_label, 2, 2, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_3_widget, 3, 0);
		mfsk24_grd_layout->addWidget(mfsk24_4_widget, 3, 1);
		mfsk24_grd_layout->addWidget(mfsk24_5_widget, 3, 2, Qt::AlignTop);
		mfsk24_grd_layout->addWidget(mfsk24_6_label, 4, 0, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_7_label, 4, 1, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_8_label, 4, 2, Qt::AlignCenter);
		mfsk24_grd_layout->addWidget(mfsk24_6_widget, 5, 0);//отладить 
		mfsk24_grd_layout->addWidget(mfsk24_7_widget, 5, 1);
		mfsk24_grd_layout->addWidget(mfsk24_8_widget, 5, 2);
	}

	QProcess::execute(QApplication::applicationDirPath() + "/mongodb_export db");
	QProcess::execute(QApplication::applicationDirPath() + "/db_starter");


	if (rm_type == "afar")
	{
		QProcess::startDetached(QApplication::applicationDirPath() + "/ag7972");
		QProcess::startDetached(QApplication::applicationDirPath() + "/n6705");
		QProcess::startDetached(QApplication::applicationDirPath() + "/afar_server");
		QProcess::startDetached(QApplication::applicationDirPath() + "/afar_mko_server");
		QThread::currentThread()->msleep(2000);
		QProcess::startDetached(QApplication::applicationDirPath() + "/BUFAR_imitator");
		//QProcess::startDetached(QApplication::applicationDirPath() + "/client");
		ip_state.insert(1, false);
		ip_state.insert(2, false);
		ip_state.insert(3, false);
	}
	if ((rm_type == "bkupi")|| (rm_type == "mca"))
	{
		QThread::currentThread()->msleep(3000);
		QProcess::startDetached(QApplication::applicationDirPath() + "/cbk_imitator_real_po");
		QProcess::startDetached(QApplication::applicationDirPath() + "/mbk04_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/MBK-02_imitator");
		QThread::currentThread()->msleep(2000);
		QProcess::startDetached(QApplication::applicationDirPath() + "/R732_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/R733_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/BOOP_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/MBK-07_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/BECH_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/ASN_imitator");
		QProcess::startDetached(QApplication::applicationDirPath() + "/LKA-05_imitator");


		if (rm_type == "mca")
		{
			QProcess::startDetached(QApplication::applicationDirPath() + "/N736_imitator");
			QProcess::startDetached(QApplication::applicationDirPath() + "/N737_imitator");
			QProcess::startDetached(QApplication::applicationDirPath() + "/BAU_imitator");
			QProcess::startDetached(QApplication::applicationDirPath() + "/BAO");
			QProcess::startDetached(QApplication::applicationDirPath() + "/BKIS_imitator");
		}

		QProcess::startDetached(QApplication::applicationDirPath() + "/common");
		QProcess::startDetached(QApplication::applicationDirPath() + "/monitor");
		QProcess::startDetached(QApplication::applicationDirPath() + "/comapp1");
		QProcess::startDetached(QApplication::applicationDirPath() + "/MBK07");
		QProcess::startDetached(QApplication::applicationDirPath() + "/comapp2");
		QProcess::startDetached(QApplication::applicationDirPath() + "/comappFrame");
		QThread::currentThread()->msleep(5000);
		QProcess::startDetached(QApplication::applicationDirPath() + "/client --imit");


		connect(mds32_0_widget, SIGNAL(mds32_get_sample(uint&, bool&)), this, SLOT(mds_1_get_sample(uint&, bool&)), Qt::DirectConnection);
		connect(mds32_1_widget, SIGNAL(mds32_get_sample(uint&, bool&)), this, SLOT(mds_2_get_sample(uint&, bool&)), Qt::DirectConnection);

		connect(mfsk24_1_widget, SIGNAL(mfsk24_impulse_change(QVariantList)), this, SLOT(mfsk_2_impulse(QVariantList)), Qt::DirectConnection);

		mds1_chans.chans = 0xFFFFFFFF;
		mds2_chans.chans = 0xFFFFFFFF;
	
		///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
		lka05_slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
		lka05_slot_thr.start(); // вот тут падает

		lka05_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
		lka05_signal_thr.start(); // вот тут падает

		if (!lka05_slot_thr.wait_connected(3) || !lka05_signal_thr.wait_connected(3))
		{
			QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka_05");
			this->deleteLater();
			return;
		}

		connect(this, SLOT(LKA05_MK_USTANOVIT_SOSTOYANIE(int, int, bool)), lka05_slot_thr.get_obj().get(), SLOT(set_mk_working(int, int, bool)));


		connect(is4_widget, SIGNAL(is4_measure(uint, QVariant&)), this, SLOT(get_resistance(uint, QVariant&)), Qt::DirectConnection);

		
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


		connect(ols_2_widget, &RpcOlsWidget::new_ols_data, this, &KPIUServer::mshr_data_in, Qt::DirectConnection);
		connect(ols_2_widget, &RpcOlsWidget::receive_data, this, &KPIUServer::mshr_data_out, Qt::DirectConnection);

	}

	connect(power_widget, &PowerWidget::u_on_bus, this, &KPIUServer::power_bus_state_changed);

	kpiu_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new kpiu_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, KPIU_SERVER_SLOT);
	rpc_slot_srv->start();
	kpiu_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new kpiu_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, KPIU_SERVER_SIGNAL);
	rpc_signal_srv->start();
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

void KPIUServer::mfsk_2_impulse(QVariantList channels)
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
	mku_widget->get_tm("ONA_TM", tmp_buf);
	mds1_chans.ONA_chans = (~tmp_buf.toUInt()) & 0x3;

	mku_widget->get_tm("732_TM", tmp_buf);
	mds1_chans.chans_732 = ((~tmp_buf.toUInt()) & 0x1F);

	buf = mds1_chans.chans;
}

void KPIUServer::mds_2_get_sample(uint& buf, bool& flag)
{
	flag = true;
	QVariant tmp_buf;
	mku_widget->get_tm("CBK_TM", tmp_buf);
	mds2_chans.CBK_chans = (~tmp_buf.toUInt()) & 0x1FF;

	mku_widget->get_tm("733_TM", tmp_buf);
	mds2_chans.chans_733 = ((~tmp_buf.toUInt()) & 0x1F);

	buf = mds2_chans.chans;
}

void KPIUServer::get_resistance(uint NProcess, QVariant& resistance)
{
	QVariantList ei_chanels_list;
	QVariantList sum_chanels_list;
	int connection_state;
	QVariantList channels;
	vvk4_widget->get_commut_chanels_list(ei_chanels_list, sum_chanels_list);


	if (NProcess == IS4_MeasVoltDC)
	{

		if (ei_chanels_list.contains(101) || sum_chanels_list.contains(102))
		{
			resistance = 0.3;
			if (ei_chanels_list.contains(101) && sum_chanels_list.contains(102))
				resistance = 27.0;
		}
			
	}

	else
	{
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

			if ((bau_tm.toUInt() & (1 << bau_ind)) != 0)
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
}
	//get_commut_chanels_list(ei_list, sum_list);

int KPIUServer::get_connection_state(QVariantList& _chans)
{
	QList<pyro_chan_state> connections = pyro_state.values();
	QList<pyro_chan_state>::iterator it = connections.begin();
	for (; it != connections.end(); it++)
	{
		if (((it->chan_num[0] == _chans[0]) && (it->chan_num[1] == _chans[1])) || ((it->chan_num[1] == _chans[0]) && (it->chan_num[0] == _chans[1])))
			//((it->chan_num[0] == _chans[0]) || (it->chan_num[1] == _chans[0]) || (it->chan_num[0] == _chans[1]) || (it->chan_num[1] == _chans[1]))//todo ??????
			return it->state;
	}
	SRPCSignalClass::Instance().toLog(QString("Не найдено соединения каналов пиропатронов %1 %2").arg(_chans[0].toString()).arg(_chans[1].toString()));
	return 0; //возвращать -1?
}

void KPIUServer::mshr_data_in(QVariantList dataList, QVariantList maskList)
{
	mku_widget->make_data_737(dataList, maskList);
	mku_widget->make_data_736(dataList, maskList);
}


void KPIUServer::mshr_data_out(QVariantList& data)
{
	data.clear();
	qulonglong chans = 0;
	QVariant mpsh_chans;
	mku_widget->get_tm("736_TM", mpsh_chans);
	QVariant mpr_chans;
	mku_widget->get_tm("737_TM", mpr_chans);
	chans = mpsh_chans.toULongLong() + mpr_chans.toULongLong();
	data << QVariant(chans);
}

void KPIUServer::power_bus_state_changed(QString name, double u)
{
	if (rm_type == "afar")
	{
		if (name == "AG7972")
		{
			power_widget->set_u(1, u);
			power_widget->set_u(2, u);
			power_widget->set_u(3, u);
		}
		if (name.contains("N6705"))
		{
			int ip_n = name.right(1).toInt();

			ip_state[ip_n] = (u > 1.0);

			int new_bufar_chan = 0;
			for (int i = 1; i < 4; i++)
			{
				if (ip_state[i])
					new_bufar_chan += i;
			}
			if (bufar_chan != new_bufar_chan)
			{
				bufar_chan = new_bufar_chan;
				mku_widget->make_mt_at_state(BAU_at_mt(BUFAR_st), bufar_chan);
			}
		}
	}
}


//LKA05
int KPIUServer::LKA05_KU_USTANOVIT_SOSTOYANIE(int module_num, int _dev, bool _flag)
{
	lka05_slot_thr.get_lka05_obj()->set_ku_working(module_num, _dev, _flag);
	return 0;
}

int KPIUServer::LKA05_MK_USTANOVIT_SOSTOYANIE(int module_num, int _dev, bool _flag)
{
	lka05_slot_thr.get_lka05_obj()->set_mk_working(module_num, _dev, _flag);
	return 0;
}

int KPIUServer::LKA05_MPVN_USTANOVIT_SOSTOYANIE(int _dev, bool _flag)
{
	lka05_slot_thr.get_lka05_obj()->set_mpvn_working(_dev, _flag);
	return 0; 
}

int KPIUServer::LKA05_MU_USTANOVIT_SOSTOYANIE(int _dev, bool _flag)
{
	lka05_slot_thr.get_lka05_obj()->set_mu_working(_dev, _flag);
	return 0;
}
