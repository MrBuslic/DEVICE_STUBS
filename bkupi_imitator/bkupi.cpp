#include "bkupi.h"

#include <QMessageBox>
#include "instruments.h"

AB_data::AB_data(int addr, QList<int> saddrs)
{
	os = addr << 11;
	for (QList<int>::iterator saddr = saddrs.begin(); saddr != saddrs.end(); saddr++)
	{
		QList<unsigned short> tmp_words;
		for (int i = 0; i < 32; i++)
		{
			tmp_words << 0;
		}

		words.insert(*saddr, tmp_words);
	}
}

BKUPI_widg::BKUPI_widg(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::bkupi_widg)
{
    ui->setupUi(this);
	parse_xml();

	server_thr.set_connection_params(instr::GetIpFromSettings("main_server"), 20001);
	server_thr.start();
	if (!server_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с main_server");
		this->deleteLater();
		return;
	}
	ui->_14r733_upi1->setCheckable(true);
	ui->_14r733_upi2->setCheckable(true);
	ui->_14r733_upi3->setCheckable(true);
	ui->_14r733_upi4->setCheckable(true);

    ui->_14r733_vchm1->setCheckable(true);
    ui->_14r733_vchm2->setCheckable(true);
    ui->_14r733_vchm3->setCheckable(true);
    ui->_14r733_vchm4->setCheckable(true);
    ui->_14r732_vchm1->setCheckable(true);
    ui->_14r732_vchm2->setCheckable(true);
    ui->_14r732_vchm3->setCheckable(true);
    ui->_14r732_vchm4->setCheckable(true);
    ui->cbk_on_vm1->setCheckable(true);
    ui->cbk_on_vm2->setCheckable(true);
    ui->cbk_on_vm3->setCheckable(true);
    ui->cbk_on_vm4->setCheckable(true);
    ui->cbk_off_vm1->setCheckable(true);
    ui->cbk_off_vm2->setCheckable(true);
    ui->cbk_off_vm3->setCheckable(true);
    ui->cbk_off_vm4->setCheckable(true);
    ui->cbk_backup_vm1->setCheckable(true);
    ui->cbk_backup_vm2->setCheckable(true);
    ui->cbk_backup_vm3->setCheckable(true);
    ui->cbk_backup_vm4->setCheckable(true);
    ui->cbk_wrong_vm1->setCheckable(true);
    ui->cbk_wrong_vm2->setCheckable(true);
    ui->cbk_wrong_vm3->setCheckable(true);
    ui->cbk_wrong_vm4->setCheckable(true);
    ui->mbk04_chanel->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ВЫКЛ"<< "ОШИБКА"  );
    ui->mbk04_mode->addItems(QStringList() << "ВТФ" << "ИК-8"<< "ИК-15"<< "ОШИБКА"  );
   //
    ui->beh_og->addItems(QStringList() << "1" << "2"<< "3"<< "ОШИБКА"  );
    ui->beh_finik->addItems(QStringList() << "1" << "2"<< "3"<< "ОШИБКА"  );
    ui->beh_lka->addItems(QStringList() << "1" << "2" );
    ui->beh_kp->addItems(QStringList() << "1" << "2" );
    ui->acn_k->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ОШИБКА(Нет ОС)"<< "ОШИБКА(Нет ОС)"  );
    ui->acn_pi->addItems(QStringList() << "ИСПРАВНА" << "НЕИСПРАВНА"  );
    ui->acn_rns->addItems(QStringList() << "ГЛОНАСС" << "GPS"<< "ГЛОНАС+GPS"  );

    ui->lka05_km_2->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ОШИБКА"  );
    ui->_14p733_rku->addItems(QStringList() << "1" << "2"<< "3"<< "4" );
	ui->_14P733_ch->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ" << "ОШИБКА");
	ui->_14p733_regim->addItems(QStringList() << "ВТФ" << "ПИ-15" << "ПИ-8");
    ui->mbk07_fsmy->addItems(QStringList() << "1" << "2"<< "3"<< "ВЫКЛ"<< "ОШИБКА"  );
    ui->mbk07_fsvy->addItems(QStringList() << "1" << "2"<< "3"<< "ВЫКЛ"<< "ОШИБКА"  );
	ui->mbk07_lt->addItems(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" );
    ui->mbk07_mode->addItems(QStringList() << "ВТФ" << "ИМ"<< "ПИ-15"<< "ПИ8-15"<< "ПИ8-1.5"<< "ОШИБКА"  );
    ui->mbk07_psp->addItems(QStringList() << "1" << "2"<< "3"<< "4"  );
    ui->mbk07_ant->addItems(QStringList() << "МНА+Y" << "МНА-Y"<< "ОНА"<< "ОШИБКА"  );
    ui->mbk07_stab->addItems(QStringList() << "ВЫСОКАЯ" << "НИЗКАЯ КГ1"<< "НИЗКАЯ КГ2"<< "ОШИБКА"  );

    ui->mbk02_osn->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ВЫКЛ"<< "ОШИБКА"  );
	ui->mbk02_lit->addItems(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7");
    ui->mbk02_ant->addItems(QStringList() << "МНА1+Y" << "МНА1-Y"<< "МНА2+Y"<< "МНА2-Y"  );
    ui->mbk02_vcs->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ОТСУТСТВУЕТ"  );
    ui->_14P732_km->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ"<< "ОШИБКА"  );

    //ui->BYP_kanal->addItems(QStringList() << "ОСНОВНОЙ" << "РЕЗЕРВНЫЙ" );
    //ui->BYP_pitanie_shdU->addItems(QStringList() << "ЕСТЬ" << "НЕТ" );
    //ui->BYP_pitanie_shdF->addItems(QStringList() << "ЕСТЬ" << "НЕТ" );
    //ui->BYP_pitanie_DUU->addItems(QStringList() << "ЕСТЬ" << "НЕТ" );
    //ui->BYP_pitanie_DUF->addItems(QStringList() << "ЕСТЬ" << "НЕТ" );
    //ui->BYP_isprav->addItems(QStringList() << "ИСПРАВЕН" << "НЕ ИСПРАВЕН" );

	//QList<unsigned short> words_6_17;
	//QList<unsigned short> words_6_1;
	//QList<unsigned short> words_2_30;
	//QList<unsigned short> words_5_1;
	//QList<unsigned short> words_3_8;
	//QList<unsigned short> words_4_17;
	//QList<unsigned short> words_4_1;
	//QList<unsigned short> words_7_1;
	//QList<unsigned short> words_2_1;
	//QList<unsigned short> words_2_17;
	//QList<unsigned short> words_9_2;
	server_thr.get_main_server_obj()->USTANOVIT_OBMEN_MKO(1);

	ab_data.insert(2, AB_data(2, QList<int>() << 1 << 4 << 17));
	server_thr.get_main_server_obj()->MKPA_AB(2, 1);
	ab_data.insert(3, AB_data(3, QList<int>() << 8));
	server_thr.get_main_server_obj()->MKPA_AB(3, 1);
	ab_data.insert(4, AB_data(4, QList<int>() << 1 << 17));
	server_thr.get_main_server_obj()->MKPA_AB(4, 1);
	ab_data.insert(5, AB_data(5, QList<int>() << 1));
	server_thr.get_main_server_obj()->MKPA_AB(5, 1);
	ab_data.insert(6, AB_data(6, QList<int>() << 1 << 2 << 17));
	server_thr.get_main_server_obj()->MKPA_AB(6, 1);
	ab_data.insert(7, AB_data(7, QList<int>() << 1));
	server_thr.get_main_server_obj()->MKPA_AB(7, 1);
	ab_data.insert(9, AB_data(9, QList<int>() << 2));
	server_thr.get_main_server_obj()->MKPA_AB(9, 1);

	connect(ui->_14r733_vchm1, SIGNAL(clicked()), this, SLOT(vchm1_clicked()));
    connect(ui->_14r733_vchm2, SIGNAL(clicked()), this, SLOT(vchm2_clicked()));
    connect(ui->_14r733_vchm3, SIGNAL(clicked()), this, SLOT(vchm3_clicked()));
    connect(ui->_14r733_vchm4, SIGNAL(clicked()), this, SLOT(vchm4_clicked()));
	connect(ui->_14r733_upi1, SIGNAL(clicked()), this, SLOT(_733_upi_clicked()));
	connect(ui->_14r733_upi2, SIGNAL(clicked()), this, SLOT(_733_upi_clicked()));
	connect(ui->_14r733_upi3, SIGNAL(clicked()), this, SLOT(_733_upi_clicked()));
	connect(ui->_14r733_upi4, SIGNAL(clicked()), this, SLOT(_733_upi_clicked()));
	
	
	
	connect(ui->_14r732_vchm1, SIGNAL(clicked()), this, SLOT(_732_vchm1_clicked()));
    connect(ui->_14r732_vchm2, SIGNAL(clicked()), this, SLOT(_732_vchm2_clicked()));
    connect(ui->_14r732_vchm3, SIGNAL(clicked()), this, SLOT(_732_vchm3_clicked()));
    connect(ui->_14r732_vchm4, SIGNAL(clicked()), this, SLOT(_732_vchm4_clicked()));


	//эмит коннектов
	ui->_14r733_vchm1->setChecked(true);
	emit ui->_14r733_vchm1->clicked();
	/*emit ui->_14r733_vchm2->clicked();
	emit ui->_14r733_vchm3->clicked();
	emit ui->_14r733_vchm4->clicked();*/
	ui->_14r733_upi1->setChecked(true);
	emit ui->_14r733_upi1->clicked();
	/*emit ui->_14r733_upi2->clicked();
	emit ui->_14r733_upi3->clicked();
	emit ui->_14r733_upi4->clicked();*/
	ui->_14r732_vchm1->setChecked(true);
	emit ui->_14r732_vchm1->clicked();
	/*emit ui->_14r732_vchm2->clicked();
	emit ui->_14r732_vchm3->clicked();
	emit ui->_14r732_vchm4->clicked();*/





    //connect(ui->cbk_on_vm1, SIGNAL(clicked()), this, SLOT(cbk1on_clicked()));
    //connect(ui->cbk_on_vm2, SIGNAL(clicked()), this, SLOT(cbk2on_clicked()));
    //connect(ui->cbk_on_vm3, SIGNAL(clicked()), this, SLOT(cbk3on_clicked()));
    //connect(ui->cbk_on_vm4, SIGNAL(clicked()), this, SLOT(cbk4on_clicked()));
    //connect(ui->cbk_off_vm1, SIGNAL(clicked()), this, SLOT(cbk1off_clicked()));
    //connect(ui->cbk_off_vm2, SIGNAL(clicked()), this, SLOT(cbk2off_clicked()));
    //connect(ui->cbk_off_vm3, SIGNAL(clicked()), this, SLOT(cbk3off_clicked()));
    //connect(ui->cbk_off_vm4, SIGNAL(clicked()), this, SLOT(cbk4off_clicked()));
    //connect(ui->cbk_backup_vm1, SIGNAL(clicked()), this, SLOT(cbk1bc_clicked()));
    //connect(ui->cbk_backup_vm2, SIGNAL(clicked()), this, SLOT(cbk2bc_clicked()));
    //connect(ui->cbk_backup_vm3, SIGNAL(clicked()), this, SLOT(cbk3bc_clicked()));
    //connect(ui->cbk_backup_vm4, SIGNAL(clicked()), this, SLOT(cbk4bc_clicked()));
    //connect(ui->cbk_wrong_vm1, SIGNAL(clicked()), this, SLOT(cbk1wr_clicked()));
    //connect(ui->cbk_wrong_vm2, SIGNAL(clicked()), this, SLOT(cbk2wr_clicked()));
    //connect(ui->cbk_wrong_vm3, SIGNAL(clicked()), this, SLOT(cbk3wr_clicked()));
    //connect(ui->cbk_wrong_vm4, SIGNAL(clicked()), this, SLOT(cbk4wr_clicked()));

    connect(ui->mbk04_chanel, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk04c(const QString&)));
    connect(ui->mbk04_mode, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk04m(const QString&)));
   //
    connect(ui->beh_og, SIGNAL(currentTextChanged(const QString&)), this, SLOT(og(const QString&)));
    connect(ui->beh_finik, SIGNAL(currentTextChanged(const QString&)), this, SLOT(finik(const QString&)));
    connect(ui->beh_lka, SIGNAL(currentTextChanged(const QString&)), this, SLOT(lka(const QString&)));
    connect(ui->beh_kp, SIGNAL(currentTextChanged(const QString&)), this, SLOT(kp(const QString&)));
    connect(ui->acn_k, SIGNAL(currentTextChanged(const QString&)), this, SLOT(k(const QString&)));  
    connect(ui->acn_pi, SIGNAL(currentTextChanged(const QString&)), this, SLOT(pi(const QString&)));
    connect(ui->acn_rns, SIGNAL(currentTextChanged(const QString&)), this, SLOT(rns(const QString&)));
    connect(ui->lka05_km_2, SIGNAL(currentTextChanged(const QString&)), this, SLOT(km(const QString&)));
    connect(ui->_14p733_rku, SIGNAL(currentTextChanged(const QString&)), this, SLOT(rku(const QString&)));
	connect(ui->_14P733_ch, SIGNAL(currentTextChanged(const QString&)), this, SLOT(_733_ch(const QString&)));
	connect(ui->_14p733_regim, SIGNAL(currentTextChanged(const QString&)), this, SLOT(_733_rg(const QString&)));
    connect(ui->mbk07_fsmy, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_fsmy(const QString&)));
    connect(ui->mbk07_fsvy, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_fsvy(const QString&)));
    connect(ui->mbk07_lt, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_lt(const QString&)));
    connect(ui->mbk07_mode, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_mode(const QString&)));
    connect(ui->mbk07_psp, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_psp(const QString&)));
    connect(ui->mbk07_ant, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_ant(const QString&)));
    connect(ui->mbk07_stab, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk07_stab(const QString&)));
    connect(ui->mbk02_osn, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk02_osn(const QString&)));
	connect(ui->mbk02_lit, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk02_lit(const QString&)));
    connect(ui->mbk02_ant, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk02_ant(const QString&)));
    connect(ui->mbk02_vcs, SIGNAL(currentTextChanged(const QString&)), this, SLOT(mbk02_vcs(const QString&)));
    connect(ui->_14P732_km, SIGNAL(currentTextChanged(const QString&)), this, SLOT(p732_km(const QString&)));
   /* connect(ui->BYP_kanal, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_kan(const QString&)));
    connect(ui->BYP_pitanie_shdU, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_pitan_shdU(const QString&)));
    connect(ui->BYP_pitanie_shdF, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_pitan_shdF(const QString&)));
    connect(ui->BYP_pitanie_DUU, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_pitan_DUU(const QString&)));
    connect(ui->BYP_pitanie_DUF, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_pitan_DUF(const QString&)));
    connect(ui->BYP_isprav, SIGNAL(currentTextChanged(const QString&)), this, SLOT(BYP_ispr(const QString&)));
*/
	// emit start
	emit  ui->mbk04_chanel->currentTextChanged("ОСНОВНОЙ");
	emit ui->mbk04_mode->currentTextChanged("ВТФ");
	//
	emit ui->beh_og->currentTextChanged("1");
	emit ui->beh_finik->currentTextChanged("1");
	emit ui->beh_lka->currentTextChanged("1");
	emit ui->beh_kp->currentTextChanged("1");
	emit ui->acn_k->currentTextChanged("ОСНОВНОЙ");
	emit ui->acn_pi->currentTextChanged("ИСПРАВНА");
	emit ui->acn_rns->currentTextChanged("ГЛОНАСС");
	emit ui->lka05_km_2->currentTextChanged("ОСНОВНОЙ");
	emit ui->_14p733_rku->currentTextChanged("1");
	emit ui->_14P733_ch->currentTextChanged("ОСНОВНОЙ");
	emit ui->_14p733_regim->currentTextChanged("ВТФ");
	emit ui->mbk07_fsmy->currentTextChanged("1");
	emit ui->mbk07_fsvy->currentTextChanged("1");
	emit ui->mbk07_lt->currentTextChanged("1");
	emit ui->mbk07_mode->currentTextChanged("ВТФ");
	emit ui->mbk07_psp->currentTextChanged("1");
	emit ui->mbk07_ant->currentTextChanged("МНА+Y");
	emit ui->mbk07_stab->currentTextChanged("ВЫСОКАЯ");
	emit ui->mbk02_osn->currentTextChanged("ОСНОВНОЙ");
	emit ui->mbk02_lit->currentTextChanged("1");
	emit ui->mbk02_ant->currentTextChanged("МНА1+Y");
	emit ui->mbk02_vcs->currentTextChanged("ОСНОВНОЙ");
	emit ui->_14P732_km->currentTextChanged("ОСНОВНОЙ");




}

BKUPI_widg::~BKUPI_widg()
{
   // delete ui;
}

QVariantList BKUPI_widg::toVariant(QList<unsigned short> _words)
{
	QVariantList tmp_list;
	for (QList<unsigned short>::iterator itr = _words.begin(); itr != _words.end(); itr++)
		tmp_list << *itr;
	return tmp_list;
}

void BKUPI_widg::change_ok_data(int ok_num, int new_value)
{
	//QMessageBox::warning\(0, \"Новое значение\", QString\(\"0x%1\"\)\.arg\(ab_data\[(\d+)\]\.words\[(\d+)\]\[(\d+)\], 4, 16, QChar\(\'0\'\)\)\)\;
	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[$1].words[$2][$3], 4, 16, QChar('0'))); \n 
	//words_(.)_(\d + )\[
	//ab_data[$1].words[$2][
}


void BKUPI_widg::parse_xml()
{
	///TODO
}

void BKUPI_widg::vchm1_clicked()
{
	
	short new_ok1;
	short new_ok2;
    short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
	if (ui->_14r733_vchm1->isChecked())
	{
		new_ok1 = 1;
		new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
	}
	else
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 1;
	}

    ab_data[6].words[17][5] = ab_data[6].words[17][5] & 0xFEFF | (new_ok1 << 8);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFFFE | (new_ok2 << 0);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFEFF | (new_ok3 << 8);
	ab_data[6].words[17][5] = ab_data[6].words[17][5] & 0xF7FF | (new_ok4 << 11);
	ab_data[6].words[17][5] = ab_data[6].words[17][5] & 0xEFFF | (new_ok5 << 12);
	ab_data[6].words[17][5] = ab_data[6].words[17][5] & 0xDFFF | (new_ok6 << 13);
	//change_ok_data(12221, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][9], 4, 16, QChar('0'))); 
  //  QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][5], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 17, toVariant(ab_data[6].words[17]));

}


void BKUPI_widg::vchm2_clicked()
{

    short new_ok1;
    short new_ok2;
    short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
    if (ui->_14r733_vchm2->isChecked())
	{
		new_ok1 = 1;
		new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
	}
	else
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 1;
	}

    ab_data[6].words[17][6] = ab_data[6].words[17][6] & 0xFEFF | (new_ok1 << 8);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFFFD | (new_ok2 << 1);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFDFF | (new_ok3 << 9);
	ab_data[6].words[17][6] = ab_data[6].words[17][6] & 0xF7FF | (new_ok4 << 11);
	ab_data[6].words[17][6] = ab_data[6].words[17][6] & 0xEFFF | (new_ok5 << 12);
	ab_data[6].words[17][6] = ab_data[6].words[17][6] & 0xDFFF | (new_ok6 << 13);

    //change_ok_data(12221, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][9], 4, 16, QChar('0'))); 
   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][6], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 17, toVariant(ab_data[6].words[17]));

}


void BKUPI_widg::vchm3_clicked()
{

    short new_ok1;
    short new_ok2;
    short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
    if (ui->_14r733_vchm3->isChecked())
	{
		new_ok1 = 1;
		new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
	}
	else
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 1;
	}

    ab_data[6].words[17][7] = ab_data[6].words[17][6] & 0xFEFF | (new_ok1 << 8);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFFFB | (new_ok2 << 2);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFBFF | (new_ok3 << 10);
	ab_data[6].words[17][7] = ab_data[6].words[17][7] & 0xF7FF | (new_ok4 << 11);
	ab_data[6].words[17][7] = ab_data[6].words[17][7] & 0xEFFF | (new_ok5 << 12);
	ab_data[6].words[17][7] = ab_data[6].words[17][7] & 0xDFFF | (new_ok6 << 13);
    //change_ok_data(12221, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][9], 4, 16, QChar('0'))); 
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][7], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 17, toVariant(ab_data[6].words[17]));

}

void BKUPI_widg::vchm4_clicked()
{

    short new_ok1;
    short new_ok2;
    short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
    if (ui->_14r733_vchm4->isChecked())
	{
		new_ok1 = 1;
		new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
	}
	else
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 1;
	}

    ab_data[6].words[17][8] = ab_data[6].words[17][6] & 0xFEFF | (new_ok1 << 8);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xFFF7 | (new_ok2 << 3);
    ab_data[6].words[17][9] = ab_data[6].words[17][9] & 0xF7FF | (new_ok3 << 11);
	ab_data[6].words[17][8] = ab_data[6].words[17][8] & 0xF7FF | (new_ok4 << 11);
	ab_data[6].words[17][8] = ab_data[6].words[17][8] & 0xEFFF | (new_ok5 << 12);
	ab_data[6].words[17][8] = ab_data[6].words[17][8] & 0xDFFF | (new_ok6 << 13);
    //change_ok_data(12221, new_ok);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][9], 4, 16, QChar('0'))); 
   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][8], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 17, toVariant(ab_data[6].words[17]));

}


void BKUPI_widg::_733_upi_clicked()
{

	short new_ok1;
	short new_ok2;
	short new_ok3;
	short new_ok4;

	if (ui->_14r733_upi1->isChecked())
	{
		new_ok1 = 0;

	}
	else
	{
		new_ok1 = 1;

	}
	if (ui->_14r733_upi2->isChecked())
	{
		new_ok2 = 0;

	}
	else
	{
		new_ok2 = 1;

	}
	if (ui->_14r733_upi3->isChecked())
	{
		new_ok3 = 0;

	}
	else
	{
		new_ok3 = 1;

	}
	if (ui->_14r733_upi4->isChecked())
	{
		new_ok4 = 0;

	}
	else
	{
		new_ok4 = 1;

	}

	ab_data[6].words[1][0] = ab_data[6].words[1][0] & 0xFFFE | (new_ok1 << 0);
	ab_data[6].words[1][0] = ab_data[6].words[1][0] & 0xFEFF | (new_ok2 << 8);
	ab_data[6].words[1][1] = ab_data[6].words[1][1] & 0xFFFE | (new_ok3 << 0);
	ab_data[6].words[1][1] = ab_data[6].words[1][1] & 0xFEFF | (new_ok4 << 8);

	//change_ok_data(12221, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][9], 4, 16, QChar('0'))); 
	//  QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[17][5], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 1, toVariant(ab_data[6].words[1]));

}





//
//
//// Блок ЦБК Включеные ВЧМ
//
//
//void BKUPI_widg::cbk1on_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_on_vm1->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFFE | (new_ok << 0);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk2on_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_on_vm2->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFFD | (new_ok << 1);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk3on_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_on_vm3->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFFB | (new_ok << 2);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk4on_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_on_vm4->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFF7 | (new_ok << 3);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//// Блок ЦБК Выключеные ВЧМ
//
//void BKUPI_widg::cbk1off_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_off_vm1->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xEFFF | (new_ok << 12);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk2off_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_off_vm2->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xDFFF | (new_ok << 13);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk3off_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_off_vm3->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xBFFF | (new_ok << 13);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk4off_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_off_vm4->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0x7FFF | (new_ok << 14);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//
//// Блок ЦБК Резервные ВМ
//
//void BKUPI_widg::cbk1bc_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_backup_vm1->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFEF | (new_ok << 4);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk2bc_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_backup_vm2->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFDF | (new_ok << 5);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk3bc_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_backup_vm3->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFFBF | (new_ok << 6);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk4bc_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_backup_vm4->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFF7F | (new_ok << 7);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//
////Блок ЦБК Неисправные ВМ
//
//void BKUPI_widg::cbk1wr_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_wrong_vm1->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFEFF | (new_ok << 8);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//
//void BKUPI_widg::cbk2wr_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_wrong_vm2->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFDFF | (new_ok << 9);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//void BKUPI_widg::cbk3wr_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_wrong_vm3->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xFBFF | (new_ok << 10);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//void BKUPI_widg::cbk4wr_clicked()
//{
//
//    short new_ok;
//    if (ui->cbk_wrong_vm4->isChecked())
//    {
//        new_ok = 1;
//
//    }
//    else
//    {
//        new_ok = 0;
//    }
//
//    ab_data[2].words[30][6] = ab_data[2].words[30][6] & 0xF7FF | (new_ok << 11);
//
//
//    //change_ok_data(12221, new_ok);
//
//    QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[30][6], 4, 16, QChar('0'))); 
//	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 30, toVariant(ab_data[2].words[30]));
//
//}
//



void BKUPI_widg::og(const QString& new_text)
{
    short new_ok;
    if (new_text == "1")
        new_ok = 1;
    else
    if (new_text == "2")
        new_ok = 2;
    else
    if (new_text == "3")
        new_ok = 4;
    else
    if (new_text == "ОШИБКА")
        new_ok = 3;




 ab_data[5].words[1][0] = ab_data[5].words[1][0] & 0xFFF8 | (new_ok << 0);

    //change_ok_data(305, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[5].words[1][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(5, 1, toVariant(ab_data[5].words[1]));

}
void BKUPI_widg::finik(const QString& new_text)
{
    short new_ok;
    if (new_text == "1")
        new_ok = 1 ;
    else
    if (new_text == "2")
        new_ok = 2;
    else
    if (new_text == "3")
        new_ok = 4;
    else
    if (new_text == "ОШИБКА")
        new_ok = 3;




 ab_data[5].words[1][0] = ab_data[5].words[1][0] & 0xFFC7 | (new_ok << 3);

    //change_ok_data(308, new_ok);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[5].words[1][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(5, 1, toVariant(ab_data[5].words[1]));

}
void BKUPI_widg::lka(const QString& new_text)
{
    short new_ok;
    if (new_text == "1")
        new_ok = 1;
    else
    if (new_text == "2")
        new_ok = 2;

 ab_data[5].words[1][0] = ab_data[5].words[1][0] & 0xFFBF | (new_ok << 6);

    //change_ok_data(, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[5].words[1][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(5, 1, toVariant(ab_data[5].words[1]));

}
void BKUPI_widg::kp(const QString& new_text)
{
    short new_ok;
    if (new_text == "1")
        new_ok = 1;
    else
    if (new_text == "2")
        new_ok = 0;


 ab_data[5].words[1][0] = ab_data[5].words[1][0] & 0xFF7F | (new_ok << 7);

    //change_ok_data(, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[5].words[1][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(5, 1, toVariant(ab_data[5].words[1]));

}
void BKUPI_widg::k(const QString& new_text)
{
    short new_ok;
	if (new_text == "ОСНОВНОЙ")
		server_thr.get_main_server_obj()->ZAPIS_AB_NEP(4, 28, QVariantList() << 0x1001);
    else
    if (new_text == "РЕЗЕРВНЫЙ")
		server_thr.get_main_server_obj()->ZAPIS_AB_NEP(4, 28, QVariantList() << 0x1002);
  //  else
  //  if (new_text == "ВЫКЛ")
		//server_thr.get_main_server_obj()->ZAPIS_AB_NEP(4, 28, QVariantList() << 0x1004);
 //   else
	//if (new_text == "ОШИБКА1")
	//	//server_thr.get_main_server_obj()->MKPA_UDAL_AB(3, 1);
 //   else
 //   if (new_text == "ОШИБКА2")
 //       new_ok = 228;



 //ab_data[5].words[1][0] = ab_data[5].words[1][0] & 0xFFF8 | (new_ok << 0);

 //   //change_ok_data(, new_ok);

 // //  QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[5].words[1][0], 4, 16, QChar('0'))); 
	//server_thr.get_main_server_obj()->ZADAT_DANNYE(5, 1, toVariant(ab_data[5].words[1]));

}
void BKUPI_widg::pi(const QString& new_text)
{
    short new_ok;
    if (new_text == "ИСПРАВНА")
        new_ok = 1;
    else
    if (new_text == "НЕИСПРАВНА")
        new_ok = 0;



 ab_data[3].words[8][0] = ab_data[3].words[8][0] & 0xFFFE | (new_ok << 0);

    //change_ok_data(145, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[3].words[8][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(3, 8, toVariant(ab_data[3].words[8]));

}
void BKUPI_widg::rns(const QString& new_text)
{
    short new_ok;
    if (new_text == "ГЛОНАСС")
        new_ok = 1;
    else
    if (new_text == "GPS")
        new_ok = 0;
    else
    if (new_text == "ГЛОНАС+GPS")
        new_ok = 2;




 ab_data[3].words[8][0] = ab_data[3].words[8][0] & 0xE7FF | (new_ok << 11);

    //change_ok_data(156, new_ok);

   // QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[3].words[8][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(3, 8, toVariant(ab_data[3].words[8]));

}

void BKUPI_widg::km(const QString& new_text)
{
		short new_ok1=0;
        short new_ok2=0;
		if (new_text == "ОСНОВНОЙ")
        {
        new_ok1 = 1;
        new_ok2 = 0;
        }
    else
	if (new_text == "РЕЗЕРВНЫЙ")
        {
        new_ok1 = 0;
        new_ok2 = 1;
        }
    else
	if (new_text == "ОШИБКА")
        {
        new_ok1 = 0;
        new_ok2 = 0;
        }



 ab_data[4].words[17][0] = ab_data[4].words[17][0] & 0xFFBF | (new_ok1 << 6);
 ab_data[4].words[17][0] = ab_data[4].words[17][0] & 0xFF7F | (new_ok2 << 7);

    //change_ok_data(199, new_ok1,200,new_ok2 = ;);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[4].words[17][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(4, 17, toVariant(ab_data[4].words[17]));

}

void BKUPI_widg::_733_ch(const QString& new_text)
{
	short new_ok;
	short new_ok1;
	if (new_text == "ОСНОВНОЙ")
	{
	
		new_ok = 1;
		new_ok1 = 0;
	}
	else
	if (new_text == "РЕЗЕРВНЫЙ")
	{
		new_ok = 0;
		new_ok1 = 1;
	}
	else
	if (new_text == "ОШИБКА")
	{
		new_ok = 0;
		new_ok1 = 0;
	}
	ab_data[6].words[17][0] = ab_data[6].words[17][0] & 0xFFBF | (new_ok << 6);
	ab_data[6].words[17][0] = ab_data[6].words[17][0] & 0xFF7F | (new_ok1 << 7);

	//change_ok_data(39, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[17][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 17, toVariant(ab_data[6].words[17]));
}


void BKUPI_widg::_733_rg(const QString& new_text)
{
	short new_ok;
	short new_ok1;
	short new_ok2;
	short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
	short new_ok7;
	short new_ok8;
	short new_ok9;
	short new_ok10;
	short new_ok11;
	if (new_text == "ПИ-15")
	{

		new_ok = 1;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 1;
		new_ok7 = 0;
		new_ok8 = 0;
		new_ok9 = 1;
		new_ok10 = 0;
		new_ok11 = 0;
		
	}
	else
	if (new_text == "ПИ-8")
	{
		new_ok = 0;
		new_ok1 = 1;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 0;
		new_ok6 = 0;
		new_ok7 = 1;
		new_ok8 = 0;
		new_ok9 = 0;
		new_ok10 = 1;
		new_ok11 = 0;
	}
	else
	if (new_text == "ВТФ")
	{
		new_ok = 0;
		new_ok1 = 0;
		new_ok2 = 1;
		new_ok3 = 0;
		new_ok4 = 0;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 0;
		new_ok8 = 1;
		new_ok9 = 0;
		new_ok10 = 0;
		new_ok11 = 1;
	}
	else
	
	{
		new_ok = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
		new_ok7 = 0;
		new_ok8 = 0;
		new_ok9 = 0;
		new_ok10 = 0;
		new_ok11 = 0;
	}

	ab_data[6].words[2][0] = ab_data[6].words[2][0] & 0xFFFE | (new_ok << 0);
	ab_data[6].words[2][0] = ab_data[6].words[2][0] & 0xFFFD | (new_ok1 << 1);
	ab_data[6].words[2][0] = ab_data[6].words[2][0] & 0xFFFB | (new_ok2 << 2);
	ab_data[6].words[2][1] = ab_data[6].words[2][1] & 0xFFFE | (new_ok3 << 0);
	ab_data[6].words[2][1] = ab_data[6].words[2][1] & 0xFFFD | (new_ok4 << 1);
	ab_data[6].words[2][1] = ab_data[6].words[2][1] & 0xFFFB | (new_ok5 << 2);
	ab_data[6].words[2][2] = ab_data[6].words[2][2] & 0xFFFE | (new_ok6 << 0);
	ab_data[6].words[2][2] = ab_data[6].words[2][2] & 0xFFFD | (new_ok7 << 1);
	ab_data[6].words[2][2] = ab_data[6].words[2][2] & 0xFFFB | (new_ok8 << 2);
	ab_data[6].words[2][3] = ab_data[6].words[2][3] & 0xFFFE | (new_ok9 << 0);
	ab_data[6].words[2][3] = ab_data[6].words[2][3] & 0xFFFD | (new_ok10 << 1);
	ab_data[6].words[2][3] = ab_data[6].words[2][3] & 0xFFFB | (new_ok11 << 2);


	//change_ok_data(39, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[17][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 2, toVariant(ab_data[6].words[2]));
}


void BKUPI_widg::rku(const QString& new_text)
{
	short new_ok1;
	short new_ok2;
	short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
	short new_ok7;
	short new_ok8;
    if (new_text == "1")
      {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
        new_ok4 = 0;
        new_ok5 = 0;
        new_ok6 = 0;
        new_ok7 = 0;
        new_ok8 = 0;
    }
    else
    if (new_text == "2")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 0;
        new_ok4 = 1;
        new_ok5 = 0;
        new_ok6 = 1;
        new_ok7 = 0;
        new_ok8 = 1;
    }
    else
    if (new_text == "3")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 1;
        new_ok4 = 0;
        new_ok5 = 1;
        new_ok6 = 0;
        new_ok7 = 1;
        new_ok8 = 0;
    }
    else
    if (new_text == "4")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 1;
        new_ok4 = 1;
        new_ok5 = 1;
        new_ok6 = 1;
        new_ok7 = 1;
        new_ok8 = 1;
    }




 ab_data[6].words[1][2] = ab_data[6].words[1][2] & 0xFFFD | (new_ok1 << 1);
 ab_data[6].words[1][2] = ab_data[6].words[1][2] & 0xFFFE | (new_ok2 << 0);
 ab_data[6].words[1][2] = ab_data[6].words[1][2] & 0xFDFF | (new_ok3 << 9);
 ab_data[6].words[1][2] = ab_data[6].words[1][2] & 0xFEFF | (new_ok4 << 8);
 ab_data[6].words[1][3] = ab_data[6].words[1][3] & 0xFFFD | (new_ok5 << 1);
 ab_data[6].words[1][3] = ab_data[6].words[1][3] & 0xFFFE | (new_ok6 << 0);
 ab_data[6].words[1][3] = ab_data[6].words[1][3] & 0xFDFF | (new_ok7 << 9);
 ab_data[6].words[1][3] = ab_data[6].words[1][3] & 0xFEFF | (new_ok8 << 8);


    //change_ok_data(498, new_ok1,497,new_ok2;506, new_ok3,505,new_ok4, ;514, new_ok5,513,new_ok6;522, new_ok7,521,new_ok8, ;);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[1][2], 4, 16, QChar('0'))); 
	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[6].words[1][3], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(6, 1, toVariant(ab_data[6].words[1]));

}

void BKUPI_widg::mbk04c(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    if (new_text == "ОСНОВНОЙ")
    {
        new_ok1 = 0;
        new_ok2 = 1;
    }
    else
    if (new_text == "РЕЗЕРВНЫЙ")
    {
        new_ok1 = 1;
        new_ok2 = 0;
    }
    else
	//if (new_text == "ВЫКЛ")
	//{
	//	new_ok1 = 1;
	//	new_ok2 = 1;
	//}
    //else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 0;
    }




    ab_data[4].words[1][0] =  ab_data[4].words[1][0]  & 0xFFFE | (new_ok1 << 0);
    ab_data[4].words[1][0] =  ab_data[4].words[1][0]  & 0xFFFD | (new_ok2 << 1);

    //change_ok_data(177, new_ok1);
    //change_ok_data(178, new_ok2);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[4].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(4, 1, toVariant(ab_data[4].words[1]));
}

void BKUPI_widg::mbk04m(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
	short new_ok3;
    if (new_text == "ВТФ")
    {
        new_ok1 = 0;
        new_ok2 = 0;
		new_ok3 = 1;
    }
    else
    if (new_text == "ИК-8")
    {
        new_ok1 = 0;
        new_ok2 = 1;
		new_ok3 = 0;
    }
    else
    if (new_text == "ИК-15")
    {
        new_ok1 = 1;
        new_ok2 = 0;
		new_ok3 = 0;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 1;
		new_ok3 = 1;
    }



    ab_data[4].words[1][0] = ab_data[4].words[1][0] & 0xFFEF | (new_ok3 << 4);
    ab_data[4].words[1][0] = ab_data[4].words[1][0] & 0xFFF7 | (new_ok2 << 3);
	ab_data[4].words[1][0] = ab_data[4].words[1][0] & 0xFFFB | (new_ok1 << 2);
    //change_ok_data(12221, new_ok);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[4].words[1][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(4, 1, toVariant(ab_data[4].words[1]));

}


// МБК 07


void BKUPI_widg::mbk07_fsmy(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    if (new_text == "1")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 1;
    }
    else
    if (new_text == "2")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 1;
    }
    else
    if (new_text == "3")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 0;
    }
    else
    if (new_text == "ВЫКЛ")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 1;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
    }



    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFEFF | (new_ok1 << 8);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFDFF | (new_ok2 << 9);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFBFF | (new_ok3 << 10);

    //change_ok_data(713, new_ok1);
    //change_ok_data(714, new_ok2);
    //change_ok_data(715, new_ok2)
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[7].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));

}

void BKUPI_widg::mbk07_fsvy(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    if (new_text == "1")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 1;
    }
    else
    if (new_text == "2")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 1;
    }
    else
    if (new_text == "3")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 0;
    }
    else
    if (new_text == "ВЫКЛ")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 1;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
    }



    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xF7FE | (new_ok1 << 11);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xEFFF | (new_ok2 << 12);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xDFFF | (new_ok3 << 13);

    
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));
}


void BKUPI_widg::mbk07_lt(const QString& new_text)
{
short new_ok0;
short new_ok1;
short new_ok2;
short new_ok3;
short new_ok4;
short new_ok5;
short new_ok6;
short new_ok7;

if (new_text == "1")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 0;
	new_ok4 = 0;
	new_ok5 = 1;
	new_ok6 = 0;
	new_ok7 = 0;
}
else
if (new_text == "2")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 1;
	new_ok4 = 0;
	new_ok5 = 1;
	new_ok6 = 0;
	new_ok7 = 0;
}
else
if (new_text == "3")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 0;
	new_ok4 = 1;
	new_ok5 = 1;
	new_ok6 = 0;
	new_ok7 = 0;
}
else
if (new_text == "4")
{
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 1;
	new_ok4 = 1;
	new_ok5 = 1;
	new_ok6 = 0;
	new_ok7 = 0;
}
else
if (new_text == "5")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 0;
	new_ok4 = 0;
	new_ok5 = 0;
	new_ok6 = 1;
	new_ok7 = 0;
}
else
if (new_text == "6")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 1;
	new_ok4 = 0;
	new_ok5 = 0;
	new_ok6 = 1;
	new_ok7 = 0;
}
else
if (new_text == "7")
{
	new_ok0 = 0;
	new_ok1 = 0;
	new_ok2 = 0;
	new_ok3 = 0;
	new_ok4 = 0;
	new_ok5 = 1;
	new_ok6 = 0;
	new_ok7 = 1;
}


ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xFEFF | (new_ok0 << 8);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xFFDF | (new_ok1 << 9);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xFBFF | (new_ok2 << 10);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xF7FF | (new_ok3 << 11);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xEFFF | (new_ok4 << 12);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xDFFF | (new_ok5 << 13);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0xBFFF | (new_ok6 << 14);
ab_data[7].words[1][1] = ab_data[7].words[1][1] & 0x7FFF | (new_ok7 << 15);

server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));

 }

void BKUPI_widg::mbk07_mode(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    short new_ok4;
	short new_ok5;
    if (new_text == "ВТФ")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 1;
        new_ok4 = 0;
		new_ok5 = 0;
    }
    else
    if (new_text == "ИМ")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
        new_ok4 = 1;
		new_ok5 = 0;
    }
    else
    if (new_text == "ПИ-15")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 0;
        new_ok4 = 0;
		new_ok5 = 0;
    }
    else
    if (new_text == "ПИ8-15")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 0;
        new_ok4 = 0;
		new_ok5 = 0;

    }
    else
    if (new_text == "ПИ8-1.5")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 0;
        new_ok4 = 0;
		new_ok5 = 1;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
        new_ok4 = 0;
		new_ok5 = 0;
    }



    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFFFE | (new_ok1 << 0);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFFFD | (new_ok2 << 1);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFFFB | (new_ok3 << 2);
    ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFFEF | (new_ok4 << 4);
	ab_data[7].words[1][0] =  ab_data[7].words[1][0]  & 0xFFBF | (new_ok5 << 6);
    //change_ok_data(705, new_ok1);
    //change_ok_data(706, new_ok2);
    //change_ok_data(707, new_ok3);
    //change_ok_data(709, new_ok4);
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[7].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));
}

void BKUPI_widg::mbk07_psp(const QString& new_text)
{
    short new_ok;
	short new_ok1;
	if (new_text == "1")
	{
		new_ok = 0;
	    new_ok1 = 0;
	}
    else
    if (new_text == "2")
	{
		new_ok = 1;
		new_ok1 = 0;
	}
    else
    if (new_text == "3")
	{
		new_ok = 0;
		new_ok1 = 1;
	}
    else
    if (new_text == "4")
	{
		new_ok = 1;
		new_ok1 = 1;
	}

    ab_data[7].words[1][1] = ab_data[7].words[1][1]  & 0xFFFE | (new_ok << 0);
	ab_data[7].words[1][1] = ab_data[7].words[1][1]	 & 0xFFFD | (new_ok1 << 1);

    //change_ok_data(721, new_ok);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[7].words[1][1], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));
}

void BKUPI_widg::mbk07_ant(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    if (new_text == "МНА+Y")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 1;
    }
    else
    if (new_text == "МНА-Y")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 1;
    }
    else
    if (new_text == "ОНА")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 0;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 1;
    }




    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFFFB | (new_ok1 << 2);
    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFFF7 | (new_ok2 << 3);
    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFFEF | (new_ok3 << 4);

    //change_ok_data(723, new_ok1);
    //change_ok_data(724, new_ok2);
    //change_ok_data(725, new_ok3);
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[7].words[1][1], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));
}

void BKUPI_widg::mbk07_stab(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    if (new_text == "ВЫСОКАЯ")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 1;
    }
    else
    if (new_text == "НИЗКАЯ КГ1")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 0;
    }
    else
    if (new_text == "НИЗКАЯ КГ2")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 0;
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 1;
        new_ok2 = 1;
        new_ok3 = 1;
    }




    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFFDF | (new_ok1 << 5);
    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFFBF | (new_ok2 << 6);
    ab_data[7].words[1][1] =  ab_data[7].words[1][1]  & 0xFF7F | (new_ok3 << 7);

    //change_ok_data(726, new_ok1);
    //change_ok_data(727, new_ok2);
    //change_ok_data(728, new_ok3);
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[7].words[1][1], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(7, 1, toVariant(ab_data[7].words[1]));
}

// МБК 02
void BKUPI_widg::mbk02_osn(const QString& new_text)
{
    short new_ok1;
	short new_ok2;
	short new_ok3;
    short new_ok4;
    
    if (new_text == "ОСНОВНОЙ")
    {
        new_ok1 = 0;
        new_ok2 = 1;
		new_ok3 = 0;
		new_ok4 = 1;

    }
    else
    if (new_text == "РЕЗЕРВНЫЙ")
    {
        new_ok1 = 1;
        new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
    }
    else
    if (new_text == "ВЫКЛ")
    {
        new_ok1 = 1;
        new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 1;
       
    }
    else
    if (new_text == "ОШИБКА")
    {
        new_ok1 = 0;
        new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 0;
        
    }




	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xFFEF | (new_ok1 << 4);
	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xFFDF | (new_ok2 << 5);
	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xEFFF | (new_ok3 << 12);
	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xDFFF | (new_ok4 << 13);

    //change_ok_data(726, new_ok1);
    //change_ok_data(727, new_ok2);
    //change_ok_data(728, new_ok3);
   // //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 1, toVariant(ab_data[2].words[1]));
}

void BKUPI_widg::mbk02_lit(const QString& new_text)
{
	short new_ok0;
	short new_ok1;
	short new_ok2;
	short new_ok3;
	short new_ok4;
	short new_ok5;
	short new_ok6;
	short new_ok7;
	short new_ok_0;
	short new_ok_1;
	short new_ok_2;
	short new_ok_3;
	short new_ok_4;
	short new_ok_5;
	short new_ok_6;
	short new_ok_7;

	if (new_text == "1")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 0;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 0;
		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 0;
		new_ok_4 = 0;
		new_ok_5 = 1;
		new_ok_6 = 0;
		new_ok_7 = 0;
	}
	else
	if (new_text == "2")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 0;

		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 1;
		new_ok_4 = 0;
		new_ok_5 = 1;
		new_ok_6 = 0;
		new_ok_7 = 0;
	}
	else
	if (new_text == "3")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 0;

		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 0;
		new_ok_4 = 1;
		new_ok_5 = 1;
		new_ok_6 = 0;
		new_ok_7 = 0;
	}
	else
	if (new_text == "4")
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 0;

		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 1;
		new_ok_4 = 1;
		new_ok_5 = 1;
		new_ok_6 = 0;
		new_ok_7 = 0;
	}
	else
	if (new_text == "5")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 1;
		new_ok7 = 0;

		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 0;
		new_ok_4 = 0;
		new_ok_5 = 0;
		new_ok_6 = 1;
		new_ok_7 = 0;
	}
	else
	if (new_text == "6")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 1;
		new_ok7 = 0;

		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 1;
		new_ok_4 = 0;
		new_ok_5 = 0;
		new_ok_6 = 1;
		new_ok_7 = 0;
	}
	else
	if (new_text == "7")
	{
		new_ok0 = 0;
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 0;
		new_ok5 = 1;
		new_ok6 = 0;
		new_ok7 = 1;

		new_ok_0 = 0;
		new_ok_1 = 0;
		new_ok_2 = 0;
		new_ok_3 = 0;
		new_ok_4 = 0;
		new_ok_5 = 1;
		new_ok_6 = 0;
		new_ok_7 = 1;
	}


	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFEFF | (new_ok0 << 8);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFDF | (new_ok1 << 9);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFBFF | (new_ok2 << 10);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xF7FF | (new_ok3 << 11);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xEFFF | (new_ok4 << 12);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xDFFF | (new_ok5 << 13);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xBFFF | (new_ok6 << 14);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0x7FFF | (new_ok7 << 15);

	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFFE | (new_ok_0 << 0);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFFD | (new_ok_1 << 1);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFFB | (new_ok_2 << 2);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFF7 | (new_ok_3 << 3);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFEF | (new_ok_4 << 4);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFDF | (new_ok_5 << 5);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFFBF | (new_ok_6 << 6);
	ab_data[2].words[4][0] = ab_data[2].words[4][0] & 0xFF7F | (new_ok_7 << 7);

	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 4, toVariant(ab_data[2].words[4]));

}



void BKUPI_widg::mbk02_ant(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
	short new_ok3;
	short new_ok4;

    if (new_text == "МНА1+Y")
    {
        new_ok1 = 1;
		new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
    }
    else
    if (new_text == "МНА1-Y")
    {
        new_ok1 = 0;
		new_ok2 = 1;
		new_ok3 = 0;
		new_ok4 = 1;
    }
    else
    if (new_text == "МНА2+Y")
    {

		new_ok1 = 1;
        new_ok2 = 0;
		new_ok3 = 1;
		new_ok4 = 0;
    }
    else
    if (new_text == "МНА2-Y")
    {
		new_ok1 = 1;
        new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 1;
    }


    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFFFB | (new_ok1 << 2);
    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFFF7 | (new_ok2 << 3);
	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xFBFF | (new_ok1 << 10);
	ab_data[2].words[1][0] = ab_data[2].words[1][0] & 0xF7FF | (new_ok2 << 11);
    //change_ok_data(3, new_ok1);
    //change_ok_data(4, new_ok2);

   // //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 1, toVariant(ab_data[2].words[1]));
}

void BKUPI_widg::mbk02_vcs(const QString& new_text)
{
    short new_ok1;
    short new_ok2;
    short new_ok3;
    short new_ok4;
    if (new_text == "ОСНОВНОЙ")
    {
        new_ok1 = 1;
        new_ok2 = 0;
        new_ok3 = 1;
        new_ok4 = 0;
    }
    else
    if (new_text == "РЕЗЕРВНЫЙ")
    {
        new_ok1 = 0;
        new_ok2 = 1;
        new_ok3 = 0;
        new_ok4 = 1;
    }
    else
    if (new_text == "ОТСУТСТВУЕТ")
    {
        new_ok1 = 0;
        new_ok2 = 0;
        new_ok3 = 0;
        new_ok4 = 0;
    }


    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFFFE | (new_ok1 << 0);
    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFFFD | (new_ok2 << 1);
    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFEFF | (new_ok3 << 8);
    ab_data[2].words[1][0] =  ab_data[2].words[1][0]  & 0xFDFF | (new_ok4 << 9);

    //change_ok_data(1, new_ok1);
    //change_ok_data(2, new_ok2);
    //change_ok_data(9, new_ok3);
    //change_ok_data(10, new_ok4);
   // //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[1][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 1, toVariant(ab_data[2].words[1]));
}
void BKUPI_widg::p732_km(const QString& new_text)
{
    short new_ok;
    if (new_text == "ОСНОВНОЙ")
        new_ok = 1;

    else
    if (new_text == "РЕЗЕРВНЫЙ")
        new_ok = 2;

    else
    if (new_text == "ОШИБКА")
        new_ok = 0;

    ab_data[2].words[17][0] =  ab_data[2].words[17][0]  & 0xFF3F | (new_ok << 6);


    //change_ok_data(39, new_ok);

    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg( ab_data[2].words[17][0], 4, 16, QChar('0')));
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 17, toVariant(ab_data[2].words[17]));
}
void BKUPI_widg::_732_vchm1_clicked()
{

	short new_ok1;
	short new_ok2;
	 short new_ok3;
	 short new_ok4;
	 short new_ok5;
	  short new_ok6;
	if (ui->_14r732_vchm1->isChecked())
	{
		new_ok1 = 1;
		new_ok2 = 1;
		new_ok3 = 1;
		new_ok4 = 0;
		new_ok5 = 0;
		new_ok6 = 0;
	}
	else
	{
		new_ok1 = 0;
		new_ok2 = 0;
		new_ok3 = 0;
		new_ok4 = 1;
		new_ok5 = 1;
		new_ok6 = 1;
	}

    ab_data[2].words[17][5] = ab_data[2].words[17][5] & 0xFEFF | (new_ok1 << 8);
    ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFFFE | (new_ok2 << 0);
    ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFEFF | (new_ok3 << 8);
    ab_data[2].words[17][5] = ab_data[2].words[17][5] & 0xF7FF | (new_ok4 << 11);
    ab_data[2].words[17][5] = ab_data[2].words[17][5] & 0xEFFF | (new_ok5 << 12);
    ab_data[2].words[17][5] = ab_data[2].words[17][5] & 0xDFFF | (new_ok6 << 13);


        //change_ok_data(12221, new_ok);


    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][5], 4, 16, QChar('0'))); 
    //QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][9], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 17, toVariant(ab_data[2].words[17]));

}


void BKUPI_widg::_732_vchm2_clicked()
{

  short new_ok1;
  short new_ok2;
   short new_ok3;
   short new_ok4;
   short new_ok5;
    short new_ok6;
  if (ui->_14r732_vchm2->isChecked())
  {
          new_ok1 = 1;
          new_ok2 = 1;
          new_ok3 = 1;
          new_ok4 = 0;
          new_ok5 = 0;
          new_ok6 = 0;
  }
  else
  {
          new_ok1 = 0;
          new_ok2 = 0;
          new_ok3 = 0;
		  new_ok4 = 1;
		  new_ok5 = 1;
		  new_ok6 = 1;
  }

ab_data[2].words[17][6] = ab_data[2].words[17][6] & 0xFEFF | (new_ok1 << 8);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFFFD | (new_ok2 << 1);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFDFF | (new_ok3 << 9);
ab_data[2].words[17][6] = ab_data[2].words[17][6] & 0xF7FF | (new_ok4 << 11);
ab_data[2].words[17][6] = ab_data[2].words[17][6] & 0xEFFF | (new_ok5 << 12);
ab_data[2].words[17][6] = ab_data[2].words[17][6] & 0xDFFF | (new_ok6 << 13);


  //change_ok_data(12221, new_ok);


//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][6], 4, 16, QChar('0'))); 
//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][9], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 17, toVariant(ab_data[2].words[17]));

}


void BKUPI_widg::_732_vchm3_clicked()
{

  short new_ok1;
  short new_ok2;
   short new_ok3;
   short new_ok4;
   short new_ok5;
    short new_ok6;
  if (ui->_14r732_vchm3->isChecked())
  {
          new_ok1 = 1;
          new_ok2 = 1;
          new_ok3 = 1;
		  new_ok4 = 0;
          new_ok5 = 0;
          new_ok6 = 0;
  }
  else
  {
          new_ok1 = 0;
          new_ok2 = 0;
          new_ok3 = 0;
		  new_ok4 = 1;
		  new_ok5 = 1;
		  new_ok6 = 1;
  }

ab_data[2].words[17][7] = ab_data[2].words[17][7] & 0xFEFF | (new_ok1 << 8);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFFFB | (new_ok2 << 2);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFBFF | (new_ok3 << 10);
ab_data[2].words[17][7] = ab_data[2].words[17][7] & 0xF7FF | (new_ok4 << 11);
ab_data[2].words[17][7] = ab_data[2].words[17][7] & 0xEFFF | (new_ok5 << 12);
ab_data[2].words[17][7] = ab_data[2].words[17][7] & 0xDFFF | (new_ok6 << 13);


  //change_ok_data(12221, new_ok);


//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][7], 4, 16, QChar('0'))); 
//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][9], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 17, toVariant(ab_data[2].words[17]));

}

void BKUPI_widg::_732_vchm4_clicked()
{

  short new_ok1;
  short new_ok2;
   short new_ok3;
   short new_ok4;
   short new_ok5;
    short new_ok6;
  if (ui->_14r732_vchm4->isChecked())
  {
          new_ok1 = 1;
          new_ok2 = 1;
          new_ok3 = 1;
          new_ok4 = 0;
          new_ok5 = 0;
          new_ok6 = 0;
  }
  else
  {
          new_ok1 = 0;
          new_ok2 = 0;
          new_ok3 = 0;
		  new_ok4 = 1;
		  new_ok5 = 1;
		  new_ok6 = 1;
  }

ab_data[2].words[17][8] = ab_data[2].words[17][7] & 0xFEFF | (new_ok1 << 8);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xFFF7 | (new_ok2 << 3);
ab_data[2].words[17][9] = ab_data[2].words[17][9] & 0xF7FF | (new_ok3 << 11);
ab_data[2].words[17][8] = ab_data[2].words[17][7] & 0xF7FF | (new_ok4 << 11);
ab_data[2].words[17][8] = ab_data[2].words[17][7] & 0xEFFF | (new_ok5 << 12);
ab_data[2].words[17][8] = ab_data[2].words[17][7] & 0xDFFF | (new_ok6 << 13);


  //change_ok_data(12221, new_ok);


//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][8], 4, 16, QChar('0'))); 
//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[2].words[17][9], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(2, 17, toVariant(ab_data[2].words[17]));

}
void BKUPI_widg::BYP_kan(const QString& new_text)
{
	short new_ok;
	if (new_text == "ОСНОВНОЙ")
		new_ok = 0;
	else
	if (new_text == "РЕЗЕРВНЫЙ")
		new_ok = 1;


	ab_data[9].words[2][0] = ab_data[9].words[2][0] & 0xFF7F | (new_ok << 7);



	//change_ok_data(9016, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
void BKUPI_widg::BYP_pitan_shdU(const QString& new_text)
{
	short new_ok;
	if (new_text == "ЕСТЬ")
		new_ok = 1;
	else
	if (new_text == "НЕТ")
		new_ok = 0;


	ab_data[9].words[2][0] = ab_data[9].words[2][0] & 0xFFFE | (new_ok << 0);



	//change_ok_data(9031, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
void BKUPI_widg::BYP_pitan_shdF(const QString& new_text)
{
	short new_ok;
	if (new_text == "ЕСТЬ")
		new_ok = 1;
	else
	if (new_text == "НЕТ")
		new_ok = 0;


	ab_data[9].words[2][0] = ab_data[9].words[2][0] & 0xFFFD | (new_ok << 1);



	//change_ok_data(9030, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
void BKUPI_widg::BYP_pitan_DUU(const QString& new_text)
{
	short new_ok;
	if (new_text == "ЕСТЬ")
		new_ok = 1;
	else
	if (new_text == "НЕТ")
		new_ok = 0;


	ab_data[9].words[2][0] = ab_data[9].words[2][0] & 0xFFFB | (new_ok << 2);



	//change_ok_data(9029, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
void BKUPI_widg::BYP_pitan_DUF(const QString& new_text)
{
	short new_ok;
	if (new_text == "ЕСТЬ")
		new_ok = 1;
	else
	if (new_text == "НЕТ")
		new_ok = 0;


	ab_data[9].words[2][0] = ab_data[9].words[2][0] & 0xFFF7 | (new_ok << 3);



	//change_ok_data(9028, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][0], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
void BKUPI_widg::BYP_ispr(const QString& new_text)
{
	short new_ok;
	if (new_text == "ИСПРАВЕН")
		new_ok = 1;
	else
	if (new_text == "НЕ ИСПРАВЕН")
		new_ok = 0;


	ab_data[9].words[2][9] = ab_data[9].words[2][9] & 0x7FFF | (new_ok << 15);



	//change_ok_data(9015, new_ok);

	//QMessageBox::warning(0, "Новое значение", QString("0x%1").arg(ab_data[9].words[2][9], 4, 16, QChar('0'))); 
	server_thr.get_main_server_obj()->ZADAT_DANNYE(9, 2, toVariant(ab_data[9].words[2]));
}
