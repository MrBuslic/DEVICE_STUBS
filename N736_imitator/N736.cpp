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
	_state.state = 0;
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


	name_lst_0 << CHAN_BLOCK_1 << CHAN_INCLUDE_1 << CHAN_CONNECT_1 << CHAN_WORK_1; // << CHAN_READY_CU_1 << CHAN_SET_BLOCK_1 << CHAN_SET_INCLUDE_1 << CHAN_SET_OK_1 << CHAN_SET_CONNECT_1 << CHAN_SET_WORK_1;
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
	active_device = false;
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

void N736_widg::imit_on()
{
	AbOn_tmr->start(20000);
}


void N736_widg::omni_connect()
{
	//off_device = false;
	active_device = true;
	AbOn_tmr->stop();
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr_device, true);

	new_tm(0xFFFF);
	
}

void N736_widg::new_tm(int tm)
{
	QVariantList tm_words;
	tm_words << tm;

	//slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 2, tm_words); // уточнить подадрес 
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 2, tm_words);
}

void N736_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	//сделать проверку cwd
	// Adr14H736_KR1	0x0A
	// Adr14H736_KR2	0x1A
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	qDebug() << "tmp_cwd.subadr = " << tmp_cwd.subadr <<" tmp_cwd.trans_dir = " << tmp_cwd.trans_dir;
	ZaprSinch word;
	word_SP synchr_pos;
	format_SCHBK schbk_word;
	if ((tmp_cwd.adr == adr_0) || (tmp_cwd.adr == adr_1)) /*&& (tmp_cwd.trans_dir == 0)) //запись */
	{
		adr_device = tmp_cwd.adr;
		if (tmp_cwd.trans_dir == 0) //запись 
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
			//QApplication::processEvents();

			//step 0
			if (tmp_cwd.subadr == 1)
			{
				for (int i = 0; i < (words.count() - 1); i++)
					word.Zapr_word[i] = words[i].toInt();

				/*
				WORD Num_Synxr_2;
				Num_Synxr_2 = (word.NumVxW2 << 6);

				DWORD num_sync;
				num_sync = word.NumVxW1 || Num_Synxr_2; */

				for (int i = 1; i < 11; i++)
				{
					synchr_strct.schzk[i - 1] = i;
				}
				synchr_strct.pr = word.pRegim; //признак режима ПК1 ПК2
				synchr_strct.nkp = word.NumKP; //номер КП
				synchr_strct.niis = word.NumSI; //номер ИИС  //номер ключа 
				synchr_strct.nsync = word.NumVxW1 || (word.NumVxW2 << 6);
				qDebug() << "synchr_strct.schzk = " << synchr_strct.schzk[0] << synchr_strct.schzk[1] << synchr_strct.schzk[2] << synchr_strct.schzk[3];
				qDebug() << "synchr_strct.pr = " << synchr_strct.pr << "word.pRegim =" << word.pRegim;
				//CreateSync_new(synchr_strct.schzk, word.pRegim, word.NumKP, word.NumSI, synchr_strct.nsync, synchr_strct.sync);
				
				
				CreateSync_new(synchr_strct.schzk, synchr_strct.pr, synchr_strct.nkp, synchr_strct.niis, synchr_strct.nsync, synchr_strct.sync);
				qDebug() << "synchr_strct.sync" << synchr_strct.sync;
				msg_syn(synchr_strct.sync);
				_state.pRSP = 1;
				set_state();
			}
			//step 2
			if (tmp_cwd.subadr == 2)
			{
				for (int i = 0; i < words.count(); i++)
				{
					synchr_pos.SP[i] = words[i].toInt();
				}
				_state.pRSSP = 1;

				//if (synchr_strct.pr == 2) //ПК 2 СЧБК не нужен?

				set_state();
			}
			//step 4  СЧБК
			if (tmp_cwd.subadr == 3)
			{
				unsigned char* schbk[2];
				for (int i = 0; i < (words.count() - 1); i++)
				{
					schbk_word.SCHBK_word[i] = words[i].toInt();
					schbk[0] = (unsigned char*)schbk_word.SCHBK_word[i];
				}

				//unsigned int* schbk_1 = (unsigned int*)schbk_word.scbk_1;
				//unsigned int* schbk_2 = (unsigned int*)schbk_word.scbk_2;


				//unsigned char* schbk_1_char = (unsigned char*)schbk_word.scbk_1;
				//unsigned char* schbk_2_char = (unsigned char*)schbk_word.scbk_2;
				//приходит два шорта, собрать в один и конвертировать
				//unsigned int schbk;
				//schbk = (*schbk_1 << 16) || *schbk_2;
				//BYTE* tmp_arr = (BYTE*)(&schbk);
				PrepareSCHBK_new((BYTE*)(&schbk));

			
				/*schbk[0] = (unsigned char*)schbk_word.scbk_1;
				schbk[1] = (unsigned char*)schbk_word.scbk_2;
				PrepareSCHBK_new((unsigned char*)schbk);*/

				_state.pRSSCHBK = 1;
				set_state();

				//CreateZKPI_new();
			}

			if (tmp_cwd.subadr == 21)
			{
				_state.state = 0;
				//slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 21, QVariantList() << _state.state);
				set_state();
			}
		}
		//if (tmp_cwd.trans_dir == 1) //чтение
		//{
		//	
		//}

	}
	
}

void N736_widg::set_state()
{
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 21, QVariantList() << _state.state);
	qDebug() << "_state.state = " << _state.state;
}

void N736_widg::msg_syn(unsigned char* msg)
{
	unsigned int* msg_int = (unsigned int*)msg;
	QString tmp_str;
	for (int i = 0; i < 16; i+=2)
	{
		synchr_w.push_back(msg_int[i] << 8 + msg_int[i+1]);
	}
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr_device, 1, synchr_w);
	qDebug() << "synchr_w = " << synchr_w;
}

void N736_widg::paint_buttons()
{
	switch (current_dev)
	{
	case CURRENT_COMP::MAIN:
		main_comp->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		reserve_comp->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		//QApplication::processEvents();
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
		//QApplication::processEvents();
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
	// перебор приход информации 
	for (auto const& i : boost::combine(dataList, maskList)) // range based
	{
		QVariant MASKVar, DATAVar;
		boost::tie(DATAVar, MASKVar) = i;
		qulonglong MASK = MASKVar.toULongLong();
		qulonglong DATA = DATAVar.toULongLong();
		qulonglong res = MASK & DATA;
		
		//проверка комплекта (основной/резервный) по сигналу блокировки
		//num_chnl - ответный сигнал 

		//флаг off_device- устройство выключено
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
		// сдвигаем на 32 (бит) позиций при резервном комплекте (тк это такой же основной комплект только сдвинутый)
		if (kr == false)
		{
			res = res >> 32;
			num_chnl = num_chnl >> 32;
		}

		// отключение устройства 
		if (off_device == false)
		{
			// name_lst_0 << CHAN_BLOCK_1 << CHAN_INCLUDE_1 << CHAN_CONNECT_1 << CHAN_WORK_1;
			// зависимость выходн сигналов от входных 
			for (int i = 0; i < 4; i++)
			{
				if (!isset(res, ((name_lst_0[i]) - 1)))
				{
					main_btn[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
					
					num_chnl &= ~(1i64 << (name_lst_0[i] - 1));

					//отключение - это обратный порядок включения
					// в num_chnl -1 потому что отсчет начинается с 1 (а не с 0)
					switch (i)
					{
					case 0: 
						main_btn[4]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_READY_CU_1 - 1));
						main_btn[5]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_SET_BLOCK_1 - 1));
						break;
					case 1:
						main_btn[6]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_SET_INCLUDE_1 - 1));
						main_btn[7]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_SET_OK_1 - 1));
						break;
					case 2:
						main_btn[8]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_SET_CONNECT_1 - 1));
						break;
					case 3:
						main_btn[9]->setStyleSheet("background-color: rgb(204, 204, 204);");
						num_chnl &= ~(1i64 << (CHAN_SET_WORK_1 - 1));
						break;
					default:
						break;
					}
				}
			}
			if (kr == false)
				num_chnl = num_chnl << 32;
			set_tm_state();
			
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
			// включение устройства по вход сигналу		
			for (int i = 0; i < 4; i++)
			{
				// зависимость выходн сигналов от входных 
				// при 0 - блокировка - блокировка установлена, при 1 - включить - включено, исправно и тд
				if (isset(res, ((name_lst_0[i]) - 1)))
				{
					num_chnl |= (1i64 << (name_lst_0[i] - 1));
					main_btn[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

					// в num_chnl -1 потому что отсчет начинается с 1. (а не с 0)
					switch (i)
					{
					case 0:
						main_btn[5]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_SET_BLOCK_1 - 1));
						break;
					case 1:
						main_btn[6]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_SET_INCLUDE_1 - 1));
						main_btn[7]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_SET_OK_1 - 1));
						break;
					case 2:
						main_btn[8]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_SET_CONNECT_1 - 1));
						break;
					case 3:
						main_btn[4]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_READY_CU_1 - 1));
						main_btn[9]->setStyleSheet("background-color: rgb(142, 198, 156);");
						num_chnl |= (1i64 << (CHAN_SET_WORK_1 - 1));
						break;
					default:
						break;
					}


					if ((res & 0xA) == 0xA)
					{
						if (!active_device)
							imit_on();
					}


					if ((res & 0xF) == 0xF) /// сделать везде как здесь
					{
						//if (off_device)
						//	imit_on();
						off_device = false;
					}
				}
			}		
	}
	// при резервном комплекте выходной сигнал сдвигаем обратно на 32 позициии
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