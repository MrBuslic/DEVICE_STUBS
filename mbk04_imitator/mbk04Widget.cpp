#include "mbk04Widget.h"
#include "rpc_ports.h"
#include <bitset>
//#include "MonitorDBController.hpp"
//#include "ProtocolDBController.hpp"
//#include "FrameDBController.hpp"
//#include "instruments.h"
#include "mbk04_socket_rpc.h"
//#include "frame_bus_rpc"
#include <QTimer>
//структура командного слова сообщения МКО
//
union MKOWord
{
	quint16 com_word;				 // командное слово целиком
	struct
	{
		quint16 count : 5,    // число сл.данных / команда
	subadr : 5,   // подадрес
		 tr : 1,       // направление передачи(1-чт.ОУ)
		  adr : 5;
	};
};


MainWidget::MainWidget()  
{
	widg = new QWidget(this);
	//this->setFixedSize(1910, 1130);
	setWindowTitle("Имитатор МБК04");

	o_rez_btn = new QPushButton("О", this);
	o_rez_btn->setCheckable(true);
	o_rez_btn->setFixedWidth(200);
	o_rez_btn->setFixedHeight(150);
	r_rez_btn = new QPushButton("Р", this);
	r_rez_btn->setCheckable(true);
	r_rez_btn->setFixedWidth(200);
	r_rez_btn->setFixedHeight(150);
	ik15_btn = new QPushButton("ИК15", this);
	ik15_btn->setCheckable(true);
	ik15_btn->setFixedHeight(70);
	ik8_btn = new QPushButton("ИК8", this);
	ik8_btn->setCheckable(true);
	ik8_btn->setFixedHeight(70);
	vtf_btn = new QPushButton("ВТФ", this);
	vtf_btn->setCheckable(true);
	vtf_btn->setFixedHeight(70);



	QVBoxLayout* v_l = new QVBoxLayout(this);
	QHBoxLayout* h_l = new QHBoxLayout();
	QHBoxLayout* h_2 = new QHBoxLayout();

	h_l->addWidget(o_rez_btn);
	h_l->addWidget(r_rez_btn);
	
	h_2->addWidget(ik15_btn);
	h_2->addWidget(ik8_btn);
	h_2->addWidget(vtf_btn);
	

	v_l->addLayout(h_l);
	v_l->addLayout(h_2);

	main_widg = new QMainWindow;
	QWidget* central = new QWidget;
	central->setFixedWidth(1);
	main_widg->setCentralWidget(central);
	v_l->addWidget(main_widg);
	this->setLayout(v_l);
	o_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	r_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	ik15_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	ik8_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	vtf_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
	devices.insert(CURRENT_DEV::MAIN, MV_DEV());
	devices.insert(CURRENT_DEV::RESERVE, MV_DEV());

	current_dev = CURRENT_DEV::OFF;
	current_rezh = REZH_FRAME::OFF_REZH;
	frame_slot_obj = new RPC_frame_bus_SLOT_Object("127.0.0.1", FRAME_SLOT);
	frame_slot_thr.set_connection_params("127.0.0.1", FRAME_SLOT);
	frame_slot_thr.start();

	frame_signal_thr.set_connection_params("127.0.0.1", FRAME_SIGNAL);
	frame_signal_thr.start();

	QString ip_str = "127.0.0.1"; 
	int slot_port = MBK04_SLOT;
	int signal_port = MBK04_SIGNAL;
	Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new Socket_RPC_SLOT_Server_Thread;
	rpc_slot_srv->set_app(this);
	rpc_slot_srv->set_params(ip_str, slot_port);
	rpc_slot_srv->start();
	Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new Socket_RPC_SIGNAL_Thread;
	rpc_signal_srv->set_app(this);
	rpc_signal_srv->set_params(ip_str, signal_port);
	rpc_signal_srv->start();
	connect(this, &MainWidget::state_changed_signal, this, &MainWidget::state_changed);
	timer = new QTimer(this);
	
	connect(timer,&QTimer::timeout, this, &MainWidget::send_frame);
	timer->start(4000);
}



MainWidget::~MainWidget()
{

}



void MainWidget::new_ku(int ku_n, int length, double u)
{

	//реакция на новую КУ
	//если команда == 16 то зеленым горит основной канал
	//если команда 17 == то оба горят серым (выключено)
	//если канал == 18, то горит зеленым резервный
	switch (ku_n)
	{
	case 16: current_dev = CURRENT_DEV::MAIN; emit state_changed_signal(); break;
	case 17: current_dev = CURRENT_DEV::OFF; emit state_changed_signal(); break;
	case 18: current_dev = CURRENT_DEV::RESERVE; emit state_changed_signal(); break;

	default:
		break;
	};
	

}


void MainWidget::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		if (tmp_cwd.subadr == 2)
		{
			switch (words.at(0).toInt())
			{
			case 1: current_rezh = REZH_FRAME::PI15; break;
			case 2: current_rezh = REZH_FRAME::PI8; break;
			case 4: current_rezh = REZH_FRAME::VTF; break;
			case 7: current_rezh = REZH_FRAME::OFF_REZH; break;
			default:
				break;
			};
		}
		else if (tmp_cwd.subadr == 6)
		{
			new_SCHBK(words);
		}

	}
	emit state_changed_signal();




}

void MainWidget::state_changed()
{
	int tmp_new_tm = 0;
	short new_ok0;
	short new_ok1;
	short new_ok2;
	short new_ok3;
	short new_ok4;
	
	//сделать инверсию "текущего режима"
	switch (current_dev)
	{
	case CURRENT_DEV::MAIN :
		o_rez_btn->setStyleSheet("background-color: rgb(142, 198, 156);"); //green light - working
		r_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);"); //grey light - off
		new_ok0 = 0;
		new_ok1 = 1;
		break;
	case CURRENT_DEV::OFF:
		o_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
		r_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
		new_ok0 = 1;
		new_ok1 =1;
		break;
	case CURRENT_DEV::RESERVE:
		o_rez_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
		r_rez_btn->setStyleSheet("background-color: rgb(142, 198, 156);");
		new_ok0 = 1;
		 new_ok1 = 0;
		break;

	default:
		break;
	}
	tmp_new_tm = tmp_new_tm & 0xFFFE | (new_ok0 << 0);
	tmp_new_tm = tmp_new_tm & 0xFFFD | (new_ok1 << 1);
	if (current_dev != CURRENT_DEV::OFF)
	{


		switch (current_rezh)
		{
		case REZH_FRAME::VTF:
			new_ok2 = 0;
			new_ok3 = 0;
			new_ok4 = 1;
			clean_frame_data("vtf");
			ik15_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			ik8_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			vtf_btn->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;
		case REZH_FRAME::PI8:
			new_ok2 = 0;
			new_ok3 = 1;
			new_ok4 = 0;
			clean_frame_data("pi8");
			ik15_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			ik8_btn->setStyleSheet("background-color: rgb(142, 198, 156);");
			vtf_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case REZH_FRAME::PI15:
			new_ok2 = 1;
			new_ok3 = 0;
			new_ok4 = 0;
			clean_frame_data("pi15");
			ik15_btn->setStyleSheet("background-color: rgb(142, 198, 156);");
			ik8_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			vtf_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case REZH_FRAME::OFF_REZH:
			new_ok2 = 1;
			new_ok3 = 1;
			new_ok4 = 1;
			ik15_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			ik8_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			vtf_btn->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;

		default:
			break;
		}
		
		tmp_new_tm = tmp_new_tm & 0xFFEF | (new_ok4 << 4);
		tmp_new_tm = tmp_new_tm & 0xFFF7 | (new_ok3 << 3);
		tmp_new_tm = tmp_new_tm & 0xFFFB | (new_ok2 << 2);
	}
	emit new_tm(tmp_new_tm);
}

//void MainWidget::run()
//{
//
////задаем таймер, который каждые четыре секунды отгружает кадр, посмотреть как во втором серваке это реализовано
//Скушать печеньку
//}
//void MainWidget::new_KVIT(int _kvit, int line_num)
//{
//	data_includer.includeKVIT(frame.get(), _kvit, line_num);
//}
//
void MainWidget::new_SCHBK(QVariantList words)
{
	 
	//std::bitset<32> schbk_set(SCHBK);
	BYTE_ARRAY schbk_arr = BYTE_ARRAY(new BYTE[32]);
	for (int i = 0; i < 16; i++)
	{

			schbk_arr[i] = words.at(i).toInt() & 0xF0;
		
			schbk_arr[2*i] = words.at(i).toInt() & 0xF;
	}
	data_includer.includeSCHBK((unsigned char*)(frame.data()), schbk_arr.get(), 32);
}

void MainWidget::clean_frame_data(QString REZH)
{
	int i = 0;
	QFile frame_file(QCoreApplication::applicationDirPath() + "/" + REZH + ".dat"); //уточнить имя файла и путь к нему!
	
	if (!frame_file.open(QIODevice::ReadOnly | QIODevice::Text))
		return ;

	QStringList tmp_list;
	QTextStream in_frame(&frame_file);
	tmp_list = in_frame.readAll().split("\n");
	clean_frame = QByteArray();
	clean_frame.resize(tmp_list.count());
	i = 0;
	for (QStringList::iterator itr = tmp_list.begin(); itr != tmp_list.end(); itr++, i++)
	{
		frame[i] = itr->toInt();
	}
	frame_file.close();
	frame = clean_frame;
}

void MainWidget::send_frame()
{
	if (current_rezh == REZH_FRAME::OFF_REZH)
		return;

	QString mode;
	switch (current_rezh)
	{
	case REZH_FRAME::VTF:
		mode = "vtf";
		break;

	case REZH_FRAME::PI8:
		mode = "pi8";
		break;
	case REZH_FRAME::PI15:
		mode = "pi15";
		break;
	}
		emit frame_slot_obj->make_new_frame(mode, frame);
}