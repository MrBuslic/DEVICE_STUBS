#include "N736.h"
#include "N736_socket_rpc.h"
#include <QMessageBox>
#include "rpc_ports.h"
union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count_word : 5,    // число сл.данных / команда
			subadr : 5,   // подадрес
			trans_dir : 1,       // направление передачи(1-чт.ќ”)
			adr : 5;
	};
};

N736_widg::N736_widg() : flag_on(false)
{
	
	widg = new QWidget(this);
	this->setFixedSize(382, 280);
	setWindowTitle("14Н736");
	main_comp = new QPushButton("Основной", this);
	//main_comp-> setFixedSize(180,50);
	reserve_comp = new QPushButton("Резервный", this);
	//reserve_comp->setFixedSize(180, 50);

	line_mko_0 = new QPushButton("0", this);
	line_mko_1 = new QPushButton("1", this);

	QStringList name_btn_lst;
	name_btn_lst <<  "БЛОКИРОВКА" << "ВКЛЮЧИТЬ" << "ПОДКЛЮЧИТЬ" << "РАБОТА" 
		<< "ГОТОВ К ЦУ" << "БЛОКИРОВКА УСТАНОВЛЕНА" << "ВКЛЮЧЕНО" << "ИСПРАВНО" << "ПОДКЛЮЧЕНО" << "РАБОТА УСТАНОВЛЕНА";

	QVBoxLayout *on_chan_hlayout = new QVBoxLayout();
	for (int i = 0; i < 4; i++)
	{
		main_btn << new QPushButton((QString("%1").arg(name_btn_lst[i])), this);
		on_chan_hlayout->addWidget(main_btn[i]);
	}
	QVBoxLayout *on_sgnl_hlayout = new QVBoxLayout();
	for (int i = 4 ; i<10; i++)
	{
		main_btn << new QPushButton((QString("%1").arg(name_btn_lst[i])), this);
		on_sgnl_hlayout->addWidget(main_btn[i]);
	}

	on_chan_gb = new QGroupBox("Каналы включения", this);
	on_chan_gb->setLayout(on_chan_hlayout);
	on_sgnl_gb = new QGroupBox("Сигналы подтверждения включения", this);
	on_sgnl_gb->setLayout(on_sgnl_hlayout);

	QHBoxLayout * h_layout = new QHBoxLayout();
	h_layout->addWidget(main_comp);
	h_layout->addWidget(reserve_comp);

	QHBoxLayout * line_hlayout = new QHBoxLayout();
	line_hlayout->addWidget(line_mko_0);
	line_hlayout->addWidget(line_mko_1);

	on_kit_gb = new QGroupBox("Комплект", this);
	on_kit_gb->setLayout(h_layout);
	kit_glayout = new QGridLayout;
	kit_glayout->addWidget(on_kit_gb, 0, 0);

	on_line_gb = new QGroupBox("Линия МКО", this);
	on_line_gb->setLayout(line_hlayout);
	line_glayout = new QGridLayout;
	line_glayout->addWidget(on_line_gb);

	inf_glayout = new QGridLayout;
	inf_glayout->addWidget(on_chan_gb, 1, 0);
	inf_glayout_2 = new QGridLayout;
	inf_glayout_2->addWidget(on_sgnl_gb, 1, 1);

	QHBoxLayout* kit_line_hlayout = new QHBoxLayout();
	kit_line_hlayout->addLayout(kit_glayout);
	kit_line_hlayout->addLayout(line_glayout);

	QHBoxLayout* h_l = new QHBoxLayout();
	h_l->addLayout(inf_glayout);
	h_l->addLayout(inf_glayout_2);

	QVBoxLayout* v_l = new QVBoxLayout(this);
	v_l->addLayout(kit_line_hlayout);
	v_l->addLayout(h_l);

	AbOn_tmr = new QTimer(this);
	AbOn_tmr->setSingleShot(true);
	//connect(AbOn_tmr, &QTimer::timeout, this, &MPR_widg::omni_connect);


	///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
	//slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	//slot_thr.start(); // вот тут падает

	//signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	//signal_thr.start(); // вот тут падает

	/*if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}*/


	QString ip_str = "127.0.0.1";
	int slot_port = N736_SLOT;
	int signal_port = N736_SIGNAL;
	n736_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new n736_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	n736_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new n736_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();


	if (!isset(24, 24))
	{
		//kpi_list << FREQ_P_code;
		qDebug() << "P";

	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	//if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	//{
	//	QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
	//	this->deleteLater();
	//	return;
	//}

	
	ols_slot_thr.set_connection_params("127.0.0.1", OLS_SLOT);
	ols_slot_thr.start();

	ols_signal_thr.set_connection_params("127.0.0.1", OLS_SIGNAL);
	ols_signal_thr.start();

	if (!ols_slot_thr.wait_connected(3) || !ols_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с ols");
		//this->deleteLater();
		return;
	}

	connect(static_cast<RPC_ols_SIGNAL_Object*>(ols_signal_thr.get_obj().get()), &RPC_ols_SIGNAL_Object::new_ols_data, this, &N736_widg::dataIn);


	//MKO = 1;
	//adr = 4;

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(signal_thr.get_obj().get(), SIGNAL(new_message_mpko(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	//connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku_732(int, int, double, int)), this, SLOT(new_ku_732(int, int, double, int)));

	
	//paint_buttons();


	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("n736_geometry").toByteArray());
//	set_new_tm();
}

bool N736_widg::isset(qulonglong x, qulonglong n)
{
	return (x & ((qulonglong)1 << n)) != 0;
}


void N736_widg::dataIn(QVariantList dataList, QVariantList maskList)
{
	if (maskList.isEmpty() || dataList.isEmpty())
		return;
	//KPIString = "";
	QVariantList kpi_list;

	for (auto const& i : boost::combine(dataList, maskList)) // range based
	{
		QVariant MASKVar, DATAVar;
		boost::tie(DATAVar, MASKVar) = i;
		qulonglong MASK = MASKVar.toULongLong();
		qulonglong DATA = DATAVar.toULongLong();
		qulonglong res = MASK & DATA;
	}
}

void N736_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("n736_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

N736_widg::~N736_widg()
{

}