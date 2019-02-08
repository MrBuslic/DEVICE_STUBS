#include "MBK07.h"

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

MBK07_widg::MBK07_widg()
{
	/*for (int i = 0; i < 3; i++)
	{
		mvku_modules << MV_MODULE(2, i);
		mvmk_modules << MV_MODULE(3, i);
	}*/
	widg = new QWidget(this);
//	this->setFixedSize(1910, 1130);
	setWindowTitle("МБК-07");
/*	MU1 = new QPushButton("МУ 1", this);
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
	MVMK2_gb = new QGroupBox("МВМК 2", this);*/
	FSMU_gb = new QGroupBox("ФСМУ", this);
	FSVU_gb = new QGroupBox("ФСВУ", this);
	for (int i = 0; i < 3; i++)
	{	
		QString numb = QString::number(i);
		FSMU_blocks << new QPushButton(numb, this);
		FSVU_canals << new QPushButton(numb, this);
		/*main_MVKU << new QPushButton("Основной", this);
		reserve_MVKU << new QPushButton("Резервный", this);
		main_MVMK << new QPushButton("Основной", this);
		reserve_MVMK << new QPushButton("Резервный", this);*/
	}
	All_vblayout = new QHBoxLayout();
	FSMUFSVU_vblayout = new QVBoxLayout();
	FSMU_hblayout = new QHBoxLayout();
	FSVU_hblayout = new QHBoxLayout();
	SubGrid_glayout = new QGridLayout();
	All_vblayout->addLayout(FSMUFSVU_vblayout);
	All_vblayout->addLayout(SubGrid_glayout);
	FSMUFSVU_vblayout->addLayout(FSMU_hblayout);
	FSMUFSVU_vblayout->addLayout(FSVU_hblayout);
	for (int i = 0; i < 3; i++)
	{
		FSMU_hblayout->addWidget(FSMU_blocks[0]);
		FSVU_hblayout->addWidget(FSVU_canals[0]);
	}
	QLabel* Mode_lb;
	Mode_lb->text = ("Режим");
	QLabel* Lit_lb;
	Lit_lb->text = ("Литера");
	QLabel* Stab_lb;
	Stab_lb->text = ("Стабильность");
	QLabel* Ann_lb;
	Ann_lb->text = ("Антенна");
	QLabel* submode;
	submode->text = ("ИМ");
	QList<QLabel*> sub_lb_list;
	sub_lb_list << Mode_lb;
	sub_lb_list << Lit_lb;
	sub_lb_list << Stab_lb;
	sub_lb_list << Ann_lb;
	sub_lb_list << submode;
	QList<QLabel*> submodepi8;
	for (int i = 0; i < 4; i++)
	{
		QString numb = QString::number(i);
		submodepi8 << new QLabel("ПСП-" + numb);
	}
	QLineEdit* Mode_le = new QLineEdit(this);
	Mode_le->setReadOnly(true);
	QLineEdit* Lit_le = new QLineEdit(this);
	Lit_le->setReadOnly(true);
	QLineEdit* Stab_le = new QLineEdit(this);
	Stab_le->setReadOnly(true);
	QLineEdit* Ann_le = new QLineEdit(this);
	Ann_le->setReadOnly(true);
	QList<QLineEdit*> sub_le_list;
	sub_le_list << Mode_le;
	sub_le_list << Lit_le;
	sub_le_list << Stab_le;
	sub_le_list << Ann_le;
	QList<Sub_tmp*> Sub_wid;
	for (int i = 0; i < 3; i++)
	{
		Sub_tmp tmp_obj;
		tmp_obj.set_lab(sub_lb_list[i]);
		tmp
	}

	//SubGrid_glayout->addWidget();
	//Logs = new QPlainTextEdit();
	//Logs->setReadOnly(true);



	/*QHBoxLayout *MVKU0_hlayout = new QHBoxLayout();
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
	v_l->addLayout(MU_glayout);*/

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
	connect(MU1, &QPushButton::clicked, this, &MBK07_widg::choose_dialog);
	connect(MU2, &QPushButton::clicked, this, &MBK07_widg::choose_dialog);

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));
//	choose_dialog();
	//(1040 2040 2140 2240  3040 3140 3240) в начале все модули имеют основной канал и му1
	//нужно обработать входящие (первые 4 знака) для таблицы 6, для какого модуля пришло слово
	
	paint_buttons();
	//set_new_tm();
}

//LKA05_widg::~LKA05_widg()
//{

//}



void MBK07_widg::choose_dialog()
{
	
//	emit btnClicked(ind);
//	QLabel *label_1;
//	QCheckBox *check_1;
	dlg = new QDialog(this, /*Qt::WindowSystemMenuHint |*/ Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
//	QPushButton * okBut;
	QPushButton *clBut;
	//	okBut.setFlat(true);
	MU1_set(words);
	

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
	connect(okBut, &QPushButton::clicked, this, &MBK07_widg::save_choose_set);
	dlg->show();
}

QCheckBox* MBK07_widg::add_set(QString name, QString data, bool is_main)
{
	QCheckBox* cb = new QCheckBox(this);
	cb->setText(name);
	cb->setProperty("ind", set_list.count());
	cb->setProperty("name", data);
	cb->setProperty("is_main", is_main);
	set_list.push_back(cb);
	return cb;
}


void MBK07_widg::MU1_set(QVariantList words)
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

void MBK07_widg::save_choose_set()
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
void MBK07_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
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
			//set_new_tm();
		}
		if (tmp_cwd.subadr == 28) //MK
		{
			int max_p;
			max_p = 0;
			int num_vertic;
			//QVariantList pshm_list;
			
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int mshm = (itr->toInt()) >> 12;
				if (mshm >=12)
				{
					QMessageBox::critical(0, "Больше 11", "Ошибка СД");
					break;
				}
				MV_DEV& param_mshm = mvmk_modules[mshm / 4].get_settings();
				for (int pshm = 0; pshm <= 11; pshm++)
				{
					num_vertic = (itr->toInt()&(1<<pshm));
					if (num_vertic != 0)
					{
						if (max_p <= 4)
						{
							//pshm_list << pshm;
							MV_DEV& param_pshm = mvmk_modules[pshm / 4].get_settings();
							emit new_mk(mshm, pshm, param_mshm.length_kom, param_pshm.length_kom, param_mshm.u_kom, param_pshm.u_kom, std::abs(param_mshm.dt_kom-param_pshm.dt_kom));
							max_p++;
						}
						else 
						{
							QMessageBox::critical(0, "Больше 4", "Ошибка СД");
							break;
						}
						
					}
				}
			}
		}
		if (tmp_cwd.subadr == 29) //KU
		{
			int ku;
			int max_ku;
			max_ku = 0;
			for (QVariantList::iterator itr = words.begin(); itr != words.end(); itr++)
			{
				int nim = (itr->toInt() & 0x0700) >> 8;
				for (int num_ku = 0; num_ku <= 7; num_ku++)
				{
					ku = (itr->toInt()&(1 << num_ku));
					if (ku != 0)
					{
						if (max_ku <= 4)
						{
							MV_DEV& param_ku = mvku_modules[nim].get_settings();
							emit new_ku(num_ku+nim*8, param_ku.length_kom, param_ku.u_kom);
							max_ku++;
						}
					}
					else
					{
						QMessageBox::critical(0, "Больше 4", "Ошибка СД");
						break;
					}
				}
			}
		}

	}
}

void MBK07_widg::paint_buttons()
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

/*void MBK07_widg::set_new_tm()
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
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, tm_words);
}*/

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