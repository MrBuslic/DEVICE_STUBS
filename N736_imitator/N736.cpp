#include "N736.h"
#include "N736_socket_rpc.h"
#include <QMessageBox>
#include "rpc_ports.h"
extern "C"
{
#include "mkpa.h"
}
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


	name_lst_0 << CHAN_BLOCK_1 << CHAN_INCLUDE_1 << CHAN_CONNECT_1 << CHAN_WORK_1 << CHAN_READY_CU_1 << CHAN_SET_BLOCK_1 << CHAN_SET_INCLUDE_1 << CHAN_SET_OK_1 << CHAN_SET_CONNECT_1 << CHAN_SET_WORK_1;
	//name_lst_1 << CHAN_BLOCK_2 << CHAN_INCLUDE_2 << CHAN_CONNECT_2 << CHAN_WORK_2 << CHAN_READY_CU_2 << CHAN_SET_BLOCK_2 << CHAN_SET_INCLUDE_2 << CHAN_SET_OK_2 << CHAN_SET_CONNECT_2 << CHAN_SET_WORK_2;

	AbOn_tmr = new QTimer(this);
	AbOn_tmr->setSingleShot(true);
	connect(AbOn_tmr, &QTimer::timeout, this, &N736_widg::omni_connect);
	

	///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
	
	connect(main_comp, &QPushButton::clicked, this, &N736_widg::example_but);
	
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

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start(); 

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start(); 


	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); 

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); 

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		this->deleteLater();
		return;
	}

	//connect(static_cast<RPC_ols_SIGNAL_Object*>(ols_signal_thr.get_obj().get()), &RPC_ols_SIGNAL_Object::new_ols_data, this, &N736_widg::dataIn);

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	//connect(signal_thr.get_obj().get(), SIGNAL(new_message_mpko(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
	
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_data_736(QVariantList, QVariantList)), this, SLOT(dataIn(QVariantList, QVariantList)));
	
	num_chnl = 0x0; 
	current_dev = CURRENT_COMP::OFF;
	current_mko = CURRENT_MKO::OFF;
	paint_buttons();
	off_device = true;
	kr = true;
	adr_0 = 10;
	adr_1 = 26;
	MKO = 0; //??? на 0
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("n736_geometry").toByteArray());

//	set_new_tm();
}

void N736_widg::example_but() {
	
}


void N736_widg::omni_connect()
{
	AbOn_tmr->stop();
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr_device, true);

	new_tm(0xFFFF);
	
}

void N736_widg::new_tm(int tm)
{
	QVariantList tm_words;
	tm_words << tm;
	//slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 2, tm_words);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_0, 2, tm_words);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_1, 2, tm_words);
}

void N736_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	//сделать проверку cwd
	// Adr14H736_KR1	0x0A
	// Adr14H736_KR2	0x1A
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	ZaprSinch word;
	
	if ((tmp_cwd.adr == adr_0) || (tmp_cwd.adr == adr_1))
	{
		switch (line)
		{
		case 0:
			current_mko = CURRENT_MKO::MAIN;
			break;
		case 1:
			current_mko = CURRENT_MKO::RESERVE;
			break;
		default:
			break;
		}
		paint_buttons();
		QApplication::processEvents();
	}
	//step 0
	if (tmp_cwd.subadr == 1)
	{
		for (int i = 0; i < 4; i++)
			word.Zapr_word = words[i].toInt();
				
		/*
		WORD Num_Synxr_2;
		Num_Synxr_2 = (word.NumVxW2 << 6);

		DWORD num_sync;
		num_sync = word.NumVxW1 || Num_Synxr_2; */

		synchr_strct.pr = word.pRegim; //признак режима ПК1 ПК2
		synchr_strct.nkp = word.NumKP; //номер КП
		synchr_strct.niis = word.NumSI; //номер ИИС
		synchr_strct.nsync = word.NumVxW1 || (word.NumVxW2 << 6);
		
		//CreateSync_new(SCHZK, PR, N KP, NKL, NSYNC, SYNC);
		CreateSync_new(synchr_strct.schzk, synchr_strct.pr, synchr_strct.nkp, synchr_strct.niis, synchr_strct.nsync, synchr_strct.sync);
		msg_syn(synchr_strct.sync);

	}
	//step 2
	if (tmp_cwd.subadr == 2)
	{
		//PrepareSCHBK_new();
		//make_schbk(const QString& com_chan_mnem)
		//{
		//	if ((com_chan_mnem == "ПК1") && (omnibusKPI->getSCHBKReceived()))
		
				unsigned int schbk;
				BYTE* tmp_arr = (BYTE*)(&schbk);
				/*if (word.pRegim)
					PrepareSCHBK_new((BYTE*)(&schbk));
				else
					PrepareSCHBK((BYTE*)(&schbk));*/
	}
	//step 3
	if (tmp_cwd.subadr == 3)
	{
		//CreateZKPI_new();
	}
	QVariantList synchr_w;
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_0, tmp_cwd.subadr, synchr_w);
	
}

void N736_widg::msg_syn(unsigned char* msg)
{
	unsigned int* msg_int = (unsigned int*)msg;
	QVariantList synchr_w;

}

void N736_widg::paint_buttons()
{
	switch (current_dev)
	{
	case CURRENT_COMP::MAIN:
		main_comp->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		reserve_comp->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		QApplication::processEvents();
		break;
	case CURRENT_COMP::OFF:
		main_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case CURRENT_COMP::RESERVE:
		main_comp->setStyleSheet("background-color: rgb(204, 204, 204);");
		reserve_comp->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	default:
		break;
	}
	switch (current_mko)
	{
	case CURRENT_MKO::MAIN:
		line_mko_0->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		line_mko_1->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		QApplication::processEvents();
		break;
	case CURRENT_MKO::OFF:
		line_mko_0->setStyleSheet("background-color: rgb(204, 204, 204);");
		line_mko_1->setStyleSheet("background-color: rgb(204, 204, 204);");
		break;
	case CURRENT_MKO::RESERVE:
		line_mko_0->setStyleSheet("background-color: rgb(204, 204, 204);");
		line_mko_1->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;

	default:
		break;
	}
	//switch (current_connect)
	//{
	//case CURRENT_CONNECT::MAIN:
	//	main_btn[0]->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
	//	main_btn[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	QApplication::processEvents();
	//	QThread::currentThread()->msleep(60000);
	//	main_btn[4]->setStyleSheet("background-color: rgb(142, 198, 156);"); 
	//	main_btn[5]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	main_btn[6]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	main_btn[7]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	QApplication::processEvents();
	//	QThread::currentThread()->msleep(100);
	//	main_btn[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	main_btn[3]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	QApplication::processEvents();
	//	QThread::currentThread()->msleep(100);
	//	main_btn[8]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	main_btn[9]->setStyleSheet("background-color: rgb(142, 198, 156);");
	//	break;
	//case CURRENT_CONNECT::OFF:
	//	main_btn[0]->setStyleSheet("background-color: rgb(204, 204, 204);");
	//	main_btn[1]->setStyleSheet("background-color: rgb(204, 204, 204);");
	//	break;
	//
	//default:
	//	break;
	//}

}


// ответ в ОЛС через МКУ 
void N736_widg::set_tm_state()
{
	mku_slot_thr.get_mku_bus_obj()->set_tm("736_TM", QVariant(num_chnl));
}


bool N736_widg::isset(qulonglong x, qulonglong n)
{
	return (x & ((qulonglong)1 << n)) != 0;
}


void N736_widg::dataIn(QVariantList dataList, QVariantList maskList)
{
	
	if (maskList.isEmpty() || dataList.isEmpty())
		return;
	
	for (auto const& i : boost::combine(dataList, maskList)) // range based
	{
		QVariant MASKVar, DATAVar;
		boost::tie(DATAVar, MASKVar) = i;
		qulonglong MASK = MASKVar.toULongLong();
		qulonglong DATA = DATAVar.toULongLong();
		qulonglong res = MASK & DATA;
		qDebug() << "res=" << res;
		
		if (off_device == true)
		{
			if (isset(res, CHAN_BLOCK_1 - 1))
			{
				adr_device = adr_0;
				qDebug() << "1комплект ";
				current_dev = CURRENT_COMP::MAIN;
				kr = true;
			}
			else if (isset(res, CHAN_BLOCK_2 - 1))
			{
				adr_device = adr_1;
				qDebug() << "2комплект ";
				current_dev = CURRENT_COMP::RESERVE;
				kr = false;
			}
		}
		if (kr == false)
		{
			res = res >> 32;
			num_chnl = num_chnl >> 32;
		}


		if (off_device == false)
		{
			for (int i = 0; i < 4; i++)
			//for (int i = 3; i >= 0; i--)
			{
				if (!isset(res, ((name_lst_0[i]) - 1)))
				{
					main_btn[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
					qDebug() << "Отключение  i =" << i << "off_device = " << off_device << " num_chnl ====" << num_chnl;
					num_chnl &= ~(1i64 << (name_lst_0[i] - 1));

					switch (i)
					{
					case 0: 
						main_btn[4]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[4] - 1));
						main_btn[5]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[5] - 1));
						break;
					case 1:
						main_btn[6]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[6] - 1));
						main_btn[7]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[7] - 1));
						break;
					case 2:
						main_btn[8]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[8] - 1));
						break;
					case 3:
						main_btn[9]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (name_lst_0[9] - 1));
						break;
					default:
						break;
					}
				}
			}
			if (kr == false)
				num_chnl = num_chnl << 32;
			set_tm_state();
			qDebug() << "res=====" << res << num_chnl;
			//if ((res & 0xf) == 0x0)
			if (num_chnl == 0)
			{
				current_dev = CURRENT_COMP::OFF;
				//current_mko = CURRENT_MKO::OFF; мко тоже выключать?
				paint_buttons();
				off_device = true;
				qDebug() << "Комплект выключен ";
			}
			return;
		}

			for (int i = 0; i < 4; i++)
			{

				if (isset(res, ((name_lst_0[i]) - 1)))
				{
					num_chnl |= (1i64 << (name_lst_0[i] - 1));
					main_btn[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

					//num_chnl = 0x7e00;

					switch (i)
					{
					case 0:
						main_btn[5]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[5] - 1));
						break;
					case 1:
						main_btn[6]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[6] - 1));
						main_btn[7]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[7] - 1));
						break;
					case 2:
						main_btn[8]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[8] - 1));
						break;
					case 3:
						main_btn[4]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[4] - 1));
						main_btn[9]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (name_lst_0[9] - 1));
						break;
					default:
						break;
					}
					/*if (num_chnl & 0xA) {
						for (int i = 5; i < 8; i++)
						{
							main_btn[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
							num_chnl |= (1i64 << (name_lst_0[i] - 1));
							qDebug() << "num_chnl в 0xA = " << num_chnl << res << i  ;
						}
					}
					if (num_chnl & 0x5) {
						for (int i = 8; i < 10; i++)
						{
							main_btn[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
							num_chnl |= (1i64 << (name_lst_0[i] - 1));
							qDebug() << "num_chnl в  = 0x5" << num_chnl;
						}
						main_btn[4]->setStyleSheet("background-color: rgb(142, 198, 156);");	
					}*/
					if ((res & 0xF) == 0xF) /// сделать везде как здесь
					{
						qDebug() << "(res & 0xF) = " << (res & 0xF) << (res && 0xF);
						off_device = false;
					}
				}
			}		
	}
	if (kr == false)
		num_chnl = num_chnl << 32;
	set_tm_state();
	qDebug() << "set_tm_state" << num_chnl;
	paint_buttons();
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