#include "mbk04Widget.h"
#include "MonitorDBController.hpp"
#include "ProtocolDBController.hpp"
#include "FrameDBController.hpp"
w#include "instruments.h"

//структура командного слова сообщения МКО
//
union MkoWord
{
	quint16 cw;				 // командное слово целиком
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
		
}



MainWidget::~MainWidget()
{

}


