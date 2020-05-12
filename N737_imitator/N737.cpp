#include "N737.h"
#include "N737_socket_rpc.h"
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

N737_widg::N737_widg() : flag_on(false)
{
	
	widg = new QWidget(this);
	this->setFixedSize(382, 360);
	setWindowTitle("14Н737");
	main_comp = new QPushButton("Основной", this);
	//main_comp-> setFixedSize(180,50);
	reserve_comp = new QPushButton("Резервный", this);
	//reserve_comp->setFixedSize(180, 50);

	line_mko_0 = new QPushButton("0", this);
	line_mko_1 = new QPushButton("1", this);
	QStringList name_btn_lst;
	name_btn_lst <<  "БЛОКИРОВКА" << "ВКЛЮЧИТЬ" << "ПОДКЛЮЧИТЬ" << "РАБОТА ФК"	<< "РАБОТА" 
		<< "ГОТОВ К ЦУ" << "БЛОКИРОВКА УСТАНОВЛЕНА" << "ВКЛЮЧЕНО" << "ИСПРАВНО" << "ПОДКЛЮЧЕНО" << "РАБОТА ФК УСТАНОВЛЕНА" << "РАБОТА УСТАНОВЛЕНА" << "ЗАХВАТ ПСП";

	QVBoxLayout *on_chan_hlayout = new QVBoxLayout();
	for (int i = 0; i < 5; i++)
	{
		main_btn << new QPushButton((QString("%1").arg(name_btn_lst[i])), this);
		on_chan_hlayout->addWidget(main_btn[i]);
	}
	QVBoxLayout *on_sgnl_hlayout = new QVBoxLayout();
	for (int i = 5 ; i<13; i++)
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
	
	
	QString ip_str = "127.0.0.1";
	int slot_port = N737_SLOT;
	int signal_port = N737_SIGNAL;

	n737_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new n737_Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	n737_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new n737_Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
	
	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start(); // вот тут падает

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start(); // вот тут падает

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}


	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		this->deleteLater();
		return;
	}




	//MKO = 1;
	//adr = 4;

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	//connect(signal_thr.get_obj().get(), SIGNAL(new_message_mpko(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_data_737(QVariantList, QVariantList)), this, SLOT(dataIn(QVariantList, QVariantList)));

	num_chnl = 0;
	current_dev_7 = CURRENT_COMP_7::OFF;
	current_mko_7 = CURRENT_MKO_7::OFF;

	paint_buttons();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("n737_geometry").toByteArray());
//	set_new_tm();
}

void N737_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
}
void N737_widg::paint_buttons()
{
	switch (current_dev_7)
	{
	case CURRENT_COMP_7::MAIN:
		main_comp->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		reserve_comp->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		QApplication::processEvents();
		break;
	case CURRENT_COMP_7::OFF:
		main_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case CURRENT_COMP_7::RESERVE:
		main_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_comp->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	default:
		break;
	}
	switch (current_mko_7)
	{
	case CURRENT_MKO_7::MAIN:
		line_mko_0->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		line_mko_1->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		QApplication::processEvents();
		break;
	case CURRENT_MKO_7::OFF:
		line_mko_0->setStyleSheet("background-color: rgb(204, 204, 204);");
		line_mko_1->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case CURRENT_MKO_7::RESERVE:
		line_mko_0->setStyleSheet("background-color: rgb(204, 204, 204);");
		line_mko_1->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	default:
		break;
	}
}

void N737_widg::set_tm_state()
{
	qDebug() << "num_chnl=" << num_chnl;
	mku_slot_thr.get_mku_bus_obj()->set_tm("737_TM", (uint)num_chnl);
}


bool N737_widg::isset(qulonglong x, qulonglong n)
{
	qDebug() << "isset=" << ((qulonglong)1 << n) << "n = " << n;
	return (x & ((qulonglong)1 << n)) != 0;
}


void N737_widg::dataIn(QVariantList dataList, QVariantList maskList)
{
	//num_chnl = 0;
	if (maskList.isEmpty() || dataList.isEmpty())
		return;
	//KPIString = "";
	QVariantList kpi_list;
	
	//name_btn_lst << CHAN_BLOCK_1 << CHAN_INCLUDE_1 << CHAN_READY_CU_1 << CHAN_SET_BLOCK_1 << CHAN_SET_INCLUDE_1 << CHAN_SET_OK_1 << CHAN_CONNECT_1 << CHAN_WORK_1 << CHAN_SET_CONNECT_1 << CHAN_SET_WORK_1;


	in_lst << CHAN_R_BLOCK_1 << CHAN_R_INCLUDE_1 << CHAN_R_CONNECT_1 << CHAN_R_WORK_FK_1 << CHAN_R_WORK_1;
	out_lst << CHAN_R_READY_CU_1 << CHAN_R_SET_BLOCK_1 << CHAN_R_SET_INCLUDE_1 << CHAN_R_SET_OK_1 << CHAN_R_SET_CONNECT_1 << CHAN_R_SET_WORK_FK_1 << CHAN_R_SET_WORK_1 << CHAN_R_PSP_1;
	
	//name_lst_1 << CHAN_R_BLOCK_2 << CHAN_R_INCLUDE_2 << CHAN_R_CONNECT_2 << CHAN_R_WORK_FK_2 << CHAN_R_WORK_2 << CHAN_R_READY_CU_2 << CHAN_R_SET_BLOCK_2 << CHAN_R_SET_INCLUDE_2 << CHAN_R_SET_OK_2 << CHAN_R_SET_CONNECT_2 << CHAN_R_SET_WORK_FK_2 << CHAN_R_SET_WORK_2 << CHAN_R_PSP_2;
	for (auto const& i : boost::combine(dataList, maskList)) // range based
	{
		QVariant MASKVar, DATAVar;
		boost::tie(DATAVar, MASKVar) = i;
		qulonglong MASK = MASKVar.toULongLong();
		qulonglong DATA = DATAVar.toULongLong();
		qulonglong res = MASK & DATA;
		qDebug() << "res=" << res;

		if (isset(res, CHAN_R_BLOCK_1 - 1))
		{
		}


		for (int i = 0; i < in_lst.count(); i++)
		{
			num_chnl |= (1i64 << (in_lst[i] - 1));
			qDebug() << "num_chnl=" << num_chnl << (1i64 << (in_lst[i] - 1)) << (1i64 << (in_lst[i]));
		}
	}
	set_tm_state();
}


void N737_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("n737_geometry", saveGeometry());
	QWidget::closeEvent(event);
}

N737_widg::~N737_widg()
{

}