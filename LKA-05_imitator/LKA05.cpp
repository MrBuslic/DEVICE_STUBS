#include "LKA05.h"

#include <QMessageBox>

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

LKA05_widg::LKA05_widg()
{
	for (int i = 0; i < 3; i++)
	{
		mvku_modules << MV_MODULE(2, i);
		mvmk_modules << MV_MODULE(3, i);
	}
	widg = new QWidget(this);
//	this->setFixedSize(1910, 1130);
	setWindowTitle("ЛКА-05");
	MU1 = new QPushButton("МУ 1", this);
	MU1 -> setFixedSize(300,100);
	MU1->setProperty("type", 1);
	MU2 = new QPushButton("МУ 2", this);
	MU2->setFixedSize(300, 100);
	MU2->setProperty("type", 2);
	MVKU0_gb = new QGroupBox("МВКУ 0", this);
	MVKU1_gb = new QGroupBox("МВКУ 1", this);
	MVKU2_gb = new QGroupBox("МВКУ 2", this);
	MVMK0_gb = new QGroupBox("МВМК 0", this);
	MVMK1_gb = new QGroupBox("МВМК 1", this);
	MVMK2_gb = new QGroupBox("МВМК 2", this);
	for (int i = 0; i < 3; i++)
	{
		main_MVKU << new QPushButton("Основной", this);
		reserve_MVKU << new QPushButton("Резервный", this);
		main_MVMK << new QPushButton("Основной", this);
		reserve_MVMK << new QPushButton("Резервный", this);
	}
	QHBoxLayout *MVKU0_hlayout = new QHBoxLayout();
	MVKU0_hlayout->addWidget(main_MVKU[0]);
	MVKU0_hlayout->addWidget(reserve_MVKU[0]);
	MVKU0_gb->setLayout(MVKU0_hlayout);
	QHBoxLayout *MVKU1_hlayout = new QHBoxLayout();
	MVKU1_hlayout->addWidget(main_MVKU[1]);
	MVKU1_hlayout->addWidget(reserve_MVKU[1]);
	MVKU1_gb->setLayout(MVKU1_hlayout);
	QHBoxLayout *MVKU2_hlayout = new QHBoxLayout();
	MVKU2_hlayout->addWidget(main_MVKU[2]);
	MVKU2_hlayout->addWidget(reserve_MVKU[2]);
	MVKU2_gb->setLayout(MVKU2_hlayout);
	QHBoxLayout *MVMK0_hlayout = new QHBoxLayout();
	MVMK0_hlayout->addWidget(main_MVMK[0]);
	MVMK0_hlayout->addWidget(reserve_MVMK[0]);
	MVMK0_gb->setLayout(MVMK0_hlayout);
	QHBoxLayout *MVMK1_hlayout = new QHBoxLayout();
	MVMK1_hlayout->addWidget(main_MVMK[1]);
	MVMK1_hlayout->addWidget(reserve_MVMK[1]);
	MVMK1_gb->setLayout(MVMK1_hlayout);
	QHBoxLayout *MVMK2_hlayout = new QHBoxLayout();
	MVMK2_hlayout->addWidget(main_MVMK[2]);
	MVMK2_hlayout->addWidget(reserve_MVMK[2]);
	MVMK2_gb->setLayout(MVMK2_hlayout);

	QHBoxLayout * h_layout_MU = new QHBoxLayout();
	h_layout_MU->addWidget(MU1);
	h_layout_MU->addWidget(MU2);

	MU_glayout = new QGridLayout;
//	MU_glayout->addWidget(MU1,0,0);
//	MU_glayout->addWidget(MU2, 0, 1);
	MU_glayout->addWidget(MVKU0_gb, 1, 0);
	MU_glayout->addWidget(MVKU1_gb, 1, 1);
	MU_glayout->addWidget(MVKU2_gb, 1, 2);
	MU_glayout->addWidget(MVMK0_gb, 2, 0);
	MU_glayout->addWidget(MVMK1_gb, 2, 1);
	MU_glayout->addWidget(MVMK2_gb, 2, 2);

	QVBoxLayout* v_l = new QVBoxLayout(this);
	v_l->addLayout(h_layout_MU);
	v_l->addLayout(MU_glayout);

	///slot_thr.set_connection_params(instr::GetIpFromSettings("rpc_omnibus"), 50001); FIX!!!!!
	slot_thr.set_connection_params("127.0.0.1", 50001);
	slot_thr.start(); // вот тут падает

	signal_thr.set_connection_params("127.0.0.1", 50002);
	signal_thr.start(); // вот тут падает

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}
	MKO = 1;
	adr = 4;
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag = true;
	connect(MU1, &QPushButton::clicked, this, &LKA05_widg::choose_dialog);
	connect(MU2, &QPushButton::clicked, this, &LKA05_widg::choose_dialog);

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
//	choose_dialog();
	//(1040 2040 2140 2240  3040 3140 3240) в начале все модули имеют основной канал и му1
	//нужно обработать входящие (первые 4 знака) для таблицы 6, для какого модуля пришло слово
	
	paint_buttons();
	set_new_tm();
}

LKA05_widg::~LKA05_widg()
{

}

/*LKA05_widg::choose_dialog(QWidget* parent) :QDialog(parent)
{
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);

	QLabel* lab = new QLabel("Выберите конфигурацию:");
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	v_lay->addWidget(lab);
	QHBoxLayout* h_lay = new QHBoxLayout;
	MU1_set();
	accept_btn = new QPushButton("Подтвердить");
	reject_btn = new QPushButton("Отмена");

	h_lay->addWidget(CheckButtonsBox);
	v_lay->addLayout(h_lay);
	QHBoxLayout* h_lay2 = new QHBoxLayout;
	h_lay2->addWidget(accept_btn, 0, Qt::AlignLeft);
	h_lay2->addWidget(reject_btn, 0, Qt::AlignRight);
	v_lay->addLayout(h_lay2);

	connect(accept_btn, SIGNAL(clicked()), this, SLOT(accept()));
	connect(reject_btn, SIGNAL(clicked()), this, SLOT(reject()));
}*/


void LKA05_widg::choose_dialog()
{
	
//	emit btnClicked(ind);
//	QLabel *label_1;
//	QCheckBox *check_1;
	dlg = new QDialog(this, /*Qt::WindowSystemMenuHint |*/ Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
//	QPushButton * okBut;
	QPushButton *clBut;
	//	okBut.setFlat(true);
	MU1_set(words);
	
//	if (ind = 2)
//	{
//		if ()
//	}

	QVBoxLayout* v_lay = new QVBoxLayout(dlg);
	okBut = new QPushButton("ok", dlg);
	clBut = new QPushButton("close", dlg);
	QHBoxLayout* h_lay = new QHBoxLayout;
	h_lay->addWidget(CheckButtonsBox);
	v_lay->addLayout(h_lay);
	QHBoxLayout* h_lay2 = new QHBoxLayout;
	h_lay2->addWidget(okBut, 0, Qt::AlignLeft);
	h_lay2->addWidget(clBut, 0, Qt::AlignRight);
	v_lay->addLayout(h_lay2);
//	connect(okBut, SIGNAL(clicked()), dlg, SLOT(save_choose_set()));
	connect(clBut, SIGNAL(clicked()), dlg, SLOT(reject()));
	connect(okBut, SIGNAL(clicked()), dlg, SLOT(accept()));
	connect(okBut, &QPushButton::clicked, this, &LKA05_widg::save_choose_set);
	dlg->show();
}

QCheckBox* LKA05_widg::add_set(QString name, QString data, bool is_main)
{
	QCheckBox* cb = new QCheckBox(this);
	cb->setText(name);
	cb->setProperty("ind", set_list.count());
	cb->setProperty("name", data);
	cb->setProperty("is_main", is_main);
	set_list.push_back(cb);
	return cb;
}


void LKA05_widg::MU1_set(QVariantList words)
{
	QVBoxLayout *vbox = new QVBoxLayout;
	CheckButtonsBox = new QGroupBox(dlg);
	CheckButtonsBox->setTitle("Settings:");
	CheckButtonsBox->setFlat(true);
//	set_list.clear();
	int ind = static_cast<QPushButton*>(sender())->property("type").toInt();
	if (ind == 1)
	{
		if (flag == true)
		{
			flag = false;
			add_set("Неисправн №1", "1");
	//		words[0] = 0x1080;
//			QVBoxLayout *vbox = new QVBoxLayout;
			foreach(QCheckBox* cb, set_list)
				vbox->addWidget(cb);
		}
	}
	if (ind == 2)
	{
		add_set("Неисправн №2", "2");
		add_set("Неисправн №3", "3");
//		QVBoxLayout *vbox = new QVBoxLayout;
		foreach(QCheckBox* cb, set_list)
			vbox->addWidget(cb);
	}

//	QVBoxLayout *vbox = new QVBoxLayout;
//	foreach(QCheckBox* cb, set_list)
//		vbox->addWidget(cb);

	vbox->addStretch(1);
	CheckButtonsBox->setLayout(vbox);
//	test(MKO, adr, words);

}

void LKA05_widg::save_choose_set()
{
	foreach(QCheckBox* cb, set_list)
		{
			if (cb->isChecked())
			{
				words[0] = 0x1080;
			}
		}

	//test(MKO, adr, words);
		
	
}

void LKA05_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		if (tmp_cwd.subadr == 17)
		{
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int switch_dev = (itr->toInt() & 0xC0) >> 6;
				int com = (itr->toInt() & 0x3000) >> 12;
				int nim = (itr->toInt() & 0x0300) >> 8;
				switch (com)
				{
				case 1:
					continue;
				case 2:
					if (switch_dev)
						mvku_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
					break;
				case 3:
					if (switch_dev)
						mvmk_modules[nim].switch_cur_dev(CURRENT_DEV(switch_dev));
					break;
				};
			}
			paint_buttons();
			set_new_tm();
		}



	}
}

void LKA05_widg::paint_buttons()
{
	if (mu_module.get_current_dev() == MAIN)
	{
		MU1->setStyleSheet("background-color: rgb(142, 198, 156);");
		MU2->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	else
	{
		MU1->setStyleSheet("background-color: rgb(204, 204, 204);");
		MU2->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
	for (int i = 0; i < 3; i++)
	{
		switch (mvku_modules[i].get_current_dev())
		{
		case OFF: 
			main_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case MAIN:
			main_MVKU[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case RESERVE:
			main_MVKU[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVKU[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;

		};

		switch (mvmk_modules[i].get_current_dev())
		{
		case OFF:
			main_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case MAIN:
			main_MVMK[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			break;
		case RESERVE:
			main_MVMK[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			reserve_MVMK[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			break;

		};

	}
}

void LKA05_widg::set_new_tm()
{
	QVariantList tm_words;
	tm_words << mu_module.get_tm();
	for (int i = 0; i < 3; i++)
	{
		tm_words << mvku_modules[i].get_tm();
	}
	for (int i = 0; i < 3; i++)
	{
		tm_words << mvmk_modules[i].get_tm();
	}
	slot_thr.get_omnibus_obj()->set_new_data(1, 4, 17, tm_words);
}

MU_MODULE::MU_MODULE() : current_dev(MAIN)
{
	working.insert(MAIN, true);
	working.insert(RESERVE, true);

	ab_working.insert(MAIN, true);
	ab_working.insert(RESERVE, true);

}

MV_MODULE::MV_MODULE(int _com, int _nim) : com(_com), nim(_nim), current_dev(MAIN)
{
	devices.insert(MAIN, MV_DEV());
	devices.insert(RESERVE, MV_DEV());
}

unsigned short MU_MODULE::get_tm()
{
	unsigned short _word = 0x1000;
	if (!get_working())
		_word += 4;
	_word += 0x20 << current_dev;
	return _word;
}

unsigned short MV_MODULE::get_tm()
{
	unsigned short _word = (com << 12) + (nim << 8);
	if (!get_working())
		_word += 2 << current_dev;
	if (current_dev == OFF)
		_word += 0xC0;
	else
		_word += 0x20 << current_dev;
	return _word;
}