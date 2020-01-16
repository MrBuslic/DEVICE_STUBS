#include "BAU.h"

#include "rpc_ports.h"

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

union KBRTK_A_DataWords
{
	quint16 data_words;

	KBRTK_A_DataWords(QVariantList raw_words)
	{
		this->data_words = static_cast<quint16>(raw_words.at(0).toInt());
	}

	struct
	{
		quint16
			rez : 4,
			BUFAR_com : 3,
			LPch_com : 3,
			MFS_com : 3,
			ALPS_com : 3;
	};
};

union BOCH_com
{
	quint16 data_words;

	struct
	{
		quint16
			BOCH_b3 : 1,
			BOCH_b2 : 1,
			BOCH_b1 : 1,
			rez : 13;

	};
};

union KBAU_DataWords
{
	quint16 data_words;

	KBAU_DataWords(QVariantList raw_words)
	{
		this->data_words = static_cast<quint16>(raw_words.at(0).toInt());
	}

	struct
	{
		quint16
			Cont_MT : 2,
			Cont_MK : 2,
			RUK : 1,
			rez : 10,
			AK : 1;
	};
};

quint16 BAU_widg::sum_bits(Serv_Union& bits)
{
	return bits.first_bit + bits.second_bit + bits.third_bit;
}

union KBRTK_M_DataWords
{
	quint16 data_words[3];

	KBRTK_M_DataWords(QVariantList raw_words)
	{
		for (int i = 0; i < raw_words.size(); i++)
			this->data_words[i] = static_cast<quint16>(raw_words.at(i).toInt());
	}

	struct
	{
		quint16
			BOCH_second_com : 1,
			BOCH_first_com : 1,
			SGTS_ant_A_com : 2,
			SGTS_ant_one_com : 2,
			PRM_ant_A_com : 2,
			PRM_ant_one_com : 2,
			LBV_com : 3,
			UM_com : 3;
		quint16
			BOCH_ch_D1 : 1,
			BOCH_ch_D2 : 1,
			BOCH_ch_D3 : 1,
			BOCH_ch_D4 : 1,
			BOCH_ch_D5 : 1,
			BOCH_ch_D6 : 1,
			BOCH_ch_D7 : 1,
			UPCH_com : 2,
			AOS_com : 3,
			FOS_com : 3,
			BOCH_third_com : 1;
		quint16
			rez : 1,
			UPCH_att : 5,
			FOS_PS : 1,
			FOS_att1 : 4,
			FOS_att2 : 4,
			BOCH_ch_D0 : 1;
	};
};

MT_widg::MT_widg(QWidget *parent)
{
	setWindowTitle("Аппаратура МТ");

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		UM_pbut << new QPushButton(numb, this);
		LBV_pbut << new QPushButton(numb, this);
		BOCH_pbut << new QPushButton(numb, this);
		FOS_pbut << new QPushButton(numb, this);
		AOS_pbut << new QPushButton(numb, this);

		UM_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		LBV_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		BOCH_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		FOS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		AOS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	for (int i = 0; i < 2; i++)
	{
		QString numb = QString::number(i + 1);
		PRM_Ant_1_pbut << new QPushButton(numb, this);
		PRM_Ant_A_pbut << new QPushButton(numb, this);
		SGTS_Ant_1_pbut << new QPushButton(numb, this);
		SGTS_Ant_A_pbut << new QPushButton(numb, this);
		UPCH_pbut << new QPushButton(numb, this);

		PRM_Ant_1_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		PRM_Ant_A_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		SGTS_Ant_1_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		SGTS_Ant_A_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		UPCH_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	App_gb = new QWidget(this);
	UM_gb = new QGroupBox("УМ", this);
	LBV_gb = new QGroupBox("ЛБВ", this);
	BOCH_gb = new QGroupBox("БОЧ", this);
	FOS_gb = new QGroupBox("ФОС", this);
	AOS_gb = new QGroupBox("АОС", this);
	UPCH_gb = new QGroupBox("УПЧ", this);
	PRM_Ant_1_gb = new QGroupBox("Комплект антенн первый", this);
	PRM_Ant_2_gb = new QGroupBox("Комплект антенн А", this);
	SGTS_Ant_1_gb = new QGroupBox("Комплект антенн первый", this);
	SGTS_Ant_2_gb = new QGroupBox("Комплект антенн А", this);
	PRM_gb = new QGroupBox("ПРМ", this);
	SGTS_gb = new QGroupBox("СГС/СТС", this);

	All_vlay = new QVBoxLayout(this);
	App_all_vlay = new QVBoxLayout(App_gb);
	App_glay = new QGridLayout();
	App_PRM_SGTS_hlay = new QHBoxLayout();
	UM_hlay = new QHBoxLayout(UM_gb);
	LBV_hlay = new QHBoxLayout(LBV_gb);
	BOCH_hlay = new QHBoxLayout(BOCH_gb);
	FOS_hlay = new QHBoxLayout(FOS_gb);
	AOS_hlay = new QHBoxLayout(AOS_gb);
	UPCH_hlay = new QHBoxLayout(UPCH_gb);
	PRM_Ant_hlay = new QHBoxLayout(PRM_gb);
	PRM_Ant_1_hlay = new QHBoxLayout(PRM_Ant_1_gb);
	PRM_Ant_2_hlay = new QHBoxLayout(PRM_Ant_2_gb);
	SGTS_Ant_hlay = new QHBoxLayout(SGTS_gb);
	SGTS_Ant_1_hlay = new QHBoxLayout(SGTS_Ant_1_gb);
	SGTS_Ant_2_hlay = new QHBoxLayout(SGTS_Ant_2_gb);

	for (int i = 0; i < 3; i++)
	{
		UM_hlay->addWidget(UM_pbut[i]);
		LBV_hlay->addWidget(LBV_pbut[i]);
		BOCH_hlay->addWidget(BOCH_pbut[i]);
		FOS_hlay->addWidget(FOS_pbut[i]);
		AOS_hlay->addWidget(AOS_pbut[i]);
	}

	for (int i = 0; i < 2; i++)
	{
		UPCH_hlay->addWidget(UPCH_pbut[i]);
		PRM_Ant_1_hlay->addWidget(PRM_Ant_1_pbut[i]);
		PRM_Ant_2_hlay->addWidget(PRM_Ant_A_pbut[i]);
		SGTS_Ant_1_hlay->addWidget(SGTS_Ant_1_pbut[i]);
		SGTS_Ant_2_hlay->addWidget(SGTS_Ant_A_pbut[i]);
	}

	UM_gb->setAlignment(Qt::AlignHCenter);
	LBV_gb->setAlignment(Qt::AlignHCenter);
	BOCH_gb->setAlignment(Qt::AlignHCenter);
	FOS_gb->setAlignment(Qt::AlignHCenter);
	AOS_gb->setAlignment(Qt::AlignHCenter);
	UPCH_gb->setAlignment(Qt::AlignHCenter);
	PRM_gb->setAlignment(Qt::AlignHCenter);
	PRM_Ant_1_gb->setAlignment(Qt::AlignHCenter);
	PRM_Ant_2_gb->setAlignment(Qt::AlignHCenter);
	SGTS_gb->setAlignment(Qt::AlignHCenter);
	SGTS_Ant_1_gb->setAlignment(Qt::AlignHCenter);
	SGTS_Ant_2_gb->setAlignment(Qt::AlignHCenter);

	All_vlay->addWidget(App_gb);
	App_all_vlay->addLayout(App_glay);
	App_all_vlay->addLayout(App_PRM_SGTS_hlay);
	App_glay->addWidget(UM_gb, 0, 0);
	App_glay->addWidget(LBV_gb, 0, 1);
	App_glay->addWidget(BOCH_gb, 0, 2);
	App_glay->addWidget(FOS_gb, 1, 0);
	App_glay->addWidget(AOS_gb, 1, 1);
	App_glay->addWidget(UPCH_gb, 1, 2);
	App_PRM_SGTS_hlay->addWidget(PRM_gb);
	PRM_Ant_hlay->addWidget(PRM_Ant_1_gb);
	PRM_Ant_hlay->addWidget(PRM_Ant_2_gb);
	App_PRM_SGTS_hlay->addWidget(SGTS_gb);
	SGTS_Ant_hlay->addWidget(SGTS_Ant_1_gb);
	SGTS_Ant_hlay->addWidget(SGTS_Ant_2_gb);
}

AT_widg::AT_widg(QWidget *parent)
{
	setWindowTitle("Аппаратура АТ");

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		ALPS_pbut << new QPushButton(numb, this);
		MFS_pbut << new QPushButton(numb, this);
		LPch_pbut << new QPushButton(numb, this);
		BUFAR_pbut << new QPushButton(numb, this);

		ALPS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		MFS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		LPch_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		BUFAR_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	ALPS_gb = new QGroupBox("АЛПС", this);
	MFS_gb = new QGroupBox("МФС", this);
	LPch_gb = new QGroupBox("ЛПЧ", this);
	BUFAR_gb = new QGroupBox("БУФАР", this);

	App_glay = new QGridLayout(this);
	ALPS_hlay = new QHBoxLayout(ALPS_gb);
	MFS_hlay = new QHBoxLayout(MFS_gb);
	LPch_hlay = new QHBoxLayout(LPch_gb);
	BUFAR_hlay = new QHBoxLayout(BUFAR_gb);

	for (int i = 0; i < 3; i++)
	{
		ALPS_hlay->addWidget(ALPS_pbut[i]);
		MFS_hlay->addWidget(MFS_pbut[i]);
		LPch_hlay->addWidget(LPch_pbut[i]);
		BUFAR_hlay->addWidget(BUFAR_pbut[i]);
	}

	ALPS_gb->setAlignment(Qt::AlignHCenter);
	MFS_gb->setAlignment(Qt::AlignHCenter);
	LPch_gb->setAlignment(Qt::AlignHCenter);
	BUFAR_gb->setAlignment(Qt::AlignHCenter);

	App_glay->addWidget(ALPS_gb, 0, 0);
	App_glay->addWidget(MFS_gb, 0, 1);
	App_glay->addWidget(LPch_gb, 1, 0);
	App_glay->addWidget(BUFAR_gb, 1, 1);
}

BAU_widg::BAU_widg(QWidget *parent)
{
	//widg = new QWidget(this);

	setWindowTitle("БАУ");

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		MU_pbut << new QPushButton(numb, this);
		MT_pbut << new QPushButton(numb, this);
		MK_pbut << new QPushButton(numb, this);

		MU_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		MK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		MT_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	BAU_gb = new QWidget(this);
	MU_gb = new QGroupBox("МУ", this);
	MT_gb = new QGroupBox("МТ", this);
	MK_gb = new QGroupBox("МК", this);

	All_vlay = new QVBoxLayout(this);
	All_BAU_vlay = new QVBoxLayout(BAU_gb);
	MU_hlay = new QHBoxLayout(MU_gb);
	MT_hlay = new QHBoxLayout(MT_gb);
	MK_hlay = new QHBoxLayout(MK_gb);
	MTMK_hlay = new QHBoxLayout();

	for (int i = 0; i < 3; i++)
	{
		MU_hlay->addWidget(MU_pbut[i]);
		MT_hlay->addWidget(MT_pbut[i]);
		MK_hlay->addWidget(MK_pbut[i]);
	}

	MU_gb->setAlignment(Qt::AlignHCenter);
	MT_gb->setAlignment(Qt::AlignHCenter);
	MK_gb->setAlignment(Qt::AlignHCenter);

	All_vlay->addWidget(BAU_gb);
	All_BAU_vlay->addWidget(MU_gb);
	All_BAU_vlay->addLayout(MTMK_hlay);
	MTMK_hlay->addWidget(MT_gb);
	MTMK_hlay->addWidget(MK_gb);

	K_CBK_timer = new QTimer(this);
	K_CBK_timer->setSingleShot(true);
	connect(K_CBK_timer, &QTimer::timeout, this, &BAU_widg::imit_off);

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start(); // вот тут падает

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start(); // вот тут падает

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с МКУ");
		this->deleteLater();
		return;
	}

	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start(); // вот тут падает

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start(); // вот тут падает

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	omnibus_slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	omnibus_slot_thr.start(); // вот тут падает

	omnibus_signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	omnibus_signal_thr.start(); // вот тут падает

	if (!omnibus_slot_thr.wait_connected(3) || !omnibus_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_nk(double)), this, SLOT(get_power(double)));
	connect(omnibus_signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);

	MT_widget.hide();
	AT_widget.hide();

	ZOB.zob_word = 0;
}

void BAU_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1) imit_off();
}

void BAU_widg::imit_on()
{
	//msg_to_log("Питание включено");
	//current_MT = MT_1;
	update_graphics_BAU();
	omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
}

//void BAU_widg::set_power_back()
//{
//	double curr;
//	if (volt >= 1)
//		curr = (double)power / volt;
//	else
//		curr = 0.0;
//	power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
//}

void BAU_widg::imit_off()
{
	current_MK = MK_OFF;
	current_MT = MT_OFF;
	current_MU = MU_OFF;
	MT_widget.current_UM = UM_OFF;
	MT_widget.current_LBV = LBV_OFF;
	MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
	MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
	MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
	MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
	MT_widget.current_BOCH = BOCH_OFF;
	MT_widget.current_FOS = FOS_OFF;
	MT_widget.current_AOS = AOS_OFF;
	MT_widget.current_UPCH = UPCH_OFF;
	AT_widget.current_ALPS = ALPS_OFF;
	AT_widget.current_MFS = MFS_OFF;
	AT_widget.current_LPch = LPch_OFF;
	AT_widget.current_BUFAR = BUFAR_OFF;
	MT_widget.hide();
	AT_widget.hide();
	kcbk = false;
	//msg_to_log("Питание отключено");
	tm_data.VBAU_1 = 0;
	tm_data.VBAU_2 = 0;
	tm_data.VBAU_3 = 0;
	tm_data.VPBAU_1 = 0;
	tm_data.VPBAU_2 = 0;
	tm_data.VPBAU_3 = 0;
	omnibus_slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	update_graphics_BAU();
	set_tm_state();
}

void BAU_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;
		if ((pshm == 5) && (mshm >= 0) && (mshm <= 5))
		{
			//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
				//msg_to_log(_msg);
			switch (tmp_mshm)
			{
			case 0:
				current_MU = MU_1;
				current_MK = MK_1;
				current_MT = MT_1;
				MT_widget.show();
				AT_widget.show();
				tm_data.VBAU_1 = 1;
				tm_data.VBAU_2 = 0;
				tm_data.VBAU_3 = 0;
				tm_data.VPBAU_1 = 1;
				tm_data.VPBAU_2 = 0;
				tm_data.VPBAU_3 = 0;
				break;
			case 1:
				current_MU = MU_2;
				current_MK = MK_2;
				current_MT = MT_2;
				MT_widget.show();
				AT_widget.show();
				tm_data.VBAU_1 = 0;
				tm_data.VBAU_2 = 1;
				tm_data.VBAU_3 = 0;
				tm_data.VPBAU_1 = 0;
				tm_data.VPBAU_2 = 1;
				tm_data.VPBAU_3 = 0;
				break;
			case 2:
				current_MU = MU_3;
				current_MK = MK_3;
				current_MT = MT_3;
				MT_widget.show();
				AT_widget.show();
				tm_data.VBAU_1 = 0;
				tm_data.VBAU_2 = 0;
				tm_data.VBAU_3 = 1;
				tm_data.VPBAU_1 = 0;
				tm_data.VPBAU_2 = 0;
				tm_data.VPBAU_3 = 1;
				break;
			//case 3:
			//	if (current_MU = MU_1)
			//	{
			//		current_MU = MU_OFF;
			//		current_MT = MT_OFF;
			//		current_MK = MK_OFF;
			//		MT_widget.current_UM = UM_OFF;
			//		MT_widget.current_LBV = LBV_OFF;
			//		MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
			//		MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
			//		MT_widget.current_BOCH = BOCH_OFF;
			//		MT_widget.current_FOS = FOS_OFF;
			//		MT_widget.current_AOS = AOS_OFF;
			//		MT_widget.current_UPCH = UPCH_OFF;
			//		AT_widget.current_ALPS = ALPS_OFF;
			//		AT_widget.current_MFS = MFS_OFF;
			//		AT_widget.current_LPch = LPch_OFF;
			//		AT_widget.current_BUFAR = BUFAR_OFF;
			//		tm_data.VBAU_1 = 0;
			//		tm_data.VPBAU_1 = 0;
			//		MT_widget.hide();
			//		AT_widget.hide();
			//	}
			//	break;
			//case 4:
			//	if (current_MU = MU_2)
			//	{
			//		current_MU = MU_OFF;
			//		current_MT = MT_OFF;
			//		current_MK = MK_OFF;
			//		MT_widget.current_UM = UM_OFF;
			//		MT_widget.current_LBV = LBV_OFF;
			//		MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
			//		MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
			//		MT_widget.current_BOCH = BOCH_OFF;
			//		MT_widget.current_FOS = FOS_OFF;
			//		MT_widget.current_AOS = AOS_OFF;
			//		MT_widget.current_UPCH = UPCH_OFF;
			//		AT_widget.current_ALPS = ALPS_OFF;
			//		AT_widget.current_MFS = MFS_OFF;
			//		AT_widget.current_LPch = LPch_OFF;
			//		AT_widget.current_BUFAR = BUFAR_OFF;
			//		tm_data.VBAU_2 = 0;
			//		tm_data.VPBAU_2 = 0;
			//		MT_widget.hide();
			//		AT_widget.hide();
			//	}
			//	break;
			//case 5:
			//	if (current_MU = MU_3)
			//	{
			//		current_MU = MU_OFF;
			//		current_MT = MT_OFF;
			//		current_MK = MK_OFF;
			//		MT_widget.current_UM = UM_OFF;
			//		MT_widget.current_LBV = LBV_OFF;
			//		MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
			//		MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
			//		MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
			//		MT_widget.current_BOCH = BOCH_OFF;
			//		MT_widget.current_FOS = FOS_OFF;
			//		MT_widget.current_AOS = AOS_OFF;
			//		MT_widget.current_UPCH = UPCH_OFF;
			//		AT_widget.current_ALPS = ALPS_OFF;
			//		AT_widget.current_MFS = MFS_OFF;
			//		AT_widget.current_LPch = LPch_OFF;
			//		AT_widget.current_BUFAR = BUFAR_OFF;
			//		tm_data.VBAU_3 = 0;
			//		tm_data.VPBAU_3 = 0;
			//		MT_widget.hide();
			//		AT_widget.hide();
			//	}
			//	break;
			default:
				if ((tmp_mshm >= 3) && (tmp_mshm <= 5))
				{
					current_MU = MU_OFF;
					current_MT = MT_OFF;
					current_MK = MK_OFF;
					MT_widget.current_UM = UM_OFF;
					MT_widget.current_LBV = LBV_OFF;
					MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
					MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
					MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
					MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
					MT_widget.current_BOCH = BOCH_OFF;
					MT_widget.current_FOS = FOS_OFF;
					MT_widget.current_AOS = AOS_OFF;
					MT_widget.current_UPCH = UPCH_OFF;
					AT_widget.current_ALPS = ALPS_OFF;
					AT_widget.current_MFS = MFS_OFF;
					AT_widget.current_LPch = LPch_OFF;
					AT_widget.current_BUFAR = BUFAR_OFF;
					tm_data.VBAU_1 = 0;
					tm_data.VBAU_2 = 0;
					tm_data.VBAU_3 = 0;
					tm_data.VPBAU_1 = 0;
					tm_data.VPBAU_2 = 0;
					tm_data.VPBAU_3 = 0;
					MT_widget.hide();
					AT_widget.hide();
				}
			}
			update_graphics_BAU();
			ZTM_create();
			set_tm_state();
		}
	}
}

void BAU_widg::K_BAU(QVariantList words)
{
	KBAU_DataWords d_words = words;
	//AK
	if (d_words.AK == 1)
		kcbk = false;
	else
		kcbk = true;
	if (current_MU != MU_OFF)
	{
		if (d_words.RUK != 0)
		{
			//МТ
			//if (current_MT == MT_OFF)
			current_MT = MT(d_words.Cont_MT);
			//	else
			if (d_words.Cont_MT == 0)
			{
				AT_widget.current_ALPS = ALPS_OFF;
				AT_widget.current_MFS = MFS_OFF;
				AT_widget.current_LPch = LPch_OFF;
				AT_widget.current_BUFAR = BUFAR_OFF;
				AT_widget.hide();
			}
			else
				AT_widget.show();

			//МК
			//if (current_MK == MK_OFF)
			current_MK = MK(d_words.Cont_MK);
			//else
			if (d_words.Cont_MK == 0)
			{
				//current_MK = MK_OFF;
				MT_widget.current_LBV = LBV_OFF;
				MT_widget.current_PRM_Ant_1 = PRM_Ant_OFF;
				MT_widget.current_PRM_Ant_A = PRM_Ant_OFF;
				MT_widget.current_SGTS_Ant_1 = SGTS_Ant_OFF;
				MT_widget.current_SGTS_Ant_A = SGTS_Ant_OFF;
				MT_widget.current_BOCH = BOCH_OFF;
				MT_widget.current_FOS = FOS_OFF;
				MT_widget.current_AOS = AOS_OFF;
				MT_widget.current_UPCH = UPCH_OFF;
				MT_widget.hide();
			}
			else
				MT_widget.show();
		}
	}
}

void BAU_widg::K_BRTK_A(QVariantList words)
{
	ZOB.zob_word = 0;
	KBRTK_A_DataWords d_words(words);
	Serv_Union tmp_ALPS_dw;
	Serv_Union tmp_MFS_dw;
	Serv_Union tmp_LPch_dw;
	Serv_Union tmp_BUFAR_dw;

	tmp_ALPS_dw.bits = d_words.ALPS_com;
	tmp_MFS_dw.bits = d_words.MFS_com;
	tmp_LPch_dw.bits = d_words.LPch_com;
	tmp_BUFAR_dw.bits = d_words.BUFAR_com;

	if (d_words.rez != 0)
		ZOB.NKK = 1;
	if ((sum_bits(tmp_ALPS_dw) > 4) && (sum_bits(tmp_MFS_dw) > 4) && (sum_bits(tmp_LPch_dw) > 4) && (sum_bits(tmp_BUFAR_dw) > 4))
		ZOB.KMT = 1;
	///АЛПС
	if (AT_widget.current_ALPS == ALPS_OFF)
	{
		if (d_words.ALPS_com != d_words.LPch_com)
			ZOB.LPch_ALPS = 1;
	}
	else
	{
		if ((AT_widget.current_ALPS != ALPS(d_words.LPch_com)) && (d_words.ALPS_com != 0))
			ZOB.KAT = 1;
	}
	///МФС
	if (AT_widget.current_MFS != MFS_OFF)
		if (AT_widget.current_MFS != MFS(d_words.MFS_com) && (d_words.MFS_com != 0))
			ZOB.KAT = 1;
	///ЛПЧ
	if (AT_widget.current_LPch == LPch_OFF)
	{
		if (d_words.LPch_com != d_words.ALPS_com)
			ZOB.LPch_ALPS = 1;
	}
	else
	{
		if ((AT_widget.current_LPch != LPch(d_words.LPch_com)) && (d_words.LPch_com != 0))
			ZOB.KAT = 1;
	}
	///БУФАР
	if (AT_widget.current_BUFAR != BUFAR_OFF)
		if ((AT_widget.current_BUFAR == BUFAR(d_words.BUFAR_com)) && (d_words.BUFAR_com == 0))
			ZOB.KAT = 1;
	if (ZOB.zob_word == 0)
	{
		AT_widget.current_ALPS = ALPS(d_words.ALPS_com);
		AT_widget.current_MFS = MFS(d_words.MFS_com);
		AT_widget.current_LPch = LPch(d_words.LPch_com);
		AT_widget.current_BUFAR = BUFAR(d_words.BUFAR_com);
	}

	QVariantList tmp_list;
	tmp_list.push_back(ZOB.zob_word);

	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 16, tmp_list);
}

void BAU_widg::K_BRTK_M(QVariantList words)
{
	ZOB.zob_word = 0;
	KBRTK_M_DataWords d_words(words);

	BOCH_com BOCH_full;
	BOCH_full.data_words = 0;
	BOCH_full.BOCH_b1 = d_words.BOCH_first_com;
	BOCH_full.BOCH_b2 = d_words.BOCH_second_com;
	BOCH_full.BOCH_b3 = d_words.BOCH_third_com;
	
	Serv_Union tmp_UM_dw;
	Serv_Union tmp_LBV_dw;
	Serv_Union tmp_PRM_Ant_1_dw;
	Serv_Union tmp_PRM_Ant_A_dw;
	Serv_Union tmp_SGTS_Ant_1_dw;
	Serv_Union tmp_SGTS_Ant_A_dw;
	Serv_Union tmp_BOCH_dw;
	Serv_Union tmp_FOS_dw;
	Serv_Union tmp_AOS_dw;
	Serv_Union tmp_UPCH_dw;

	tmp_UM_dw.bits = d_words.UM_com;
	tmp_LBV_dw.bits = d_words.LBV_com;
	tmp_PRM_Ant_1_dw.bits = d_words.PRM_ant_one_com;
	tmp_PRM_Ant_A_dw.bits = d_words.PRM_ant_A_com;
	tmp_SGTS_Ant_1_dw.bits = d_words.SGTS_ant_one_com;
	tmp_SGTS_Ant_A_dw.bits = d_words.SGTS_ant_A_com;
	tmp_BOCH_dw.bits = BOCH_full.data_words;
	tmp_FOS_dw.bits = d_words.FOS_com;
	tmp_AOS_dw.bits = d_words.AOS_com;
	tmp_UPCH_dw.bits = d_words.UPCH_com;

	if (d_words.rez != 0)
		ZOB.NKK = 1;
	if (!((sum_bits(tmp_UM_dw) <= 1) && (sum_bits(tmp_LBV_dw) <= 1) && (sum_bits(tmp_PRM_Ant_1_dw) <= 1) && (sum_bits(tmp_PRM_Ant_A_dw) <= 1) && (sum_bits(tmp_SGTS_Ant_1_dw) <= 1)
		&& (sum_bits(tmp_SGTS_Ant_A_dw) <= 1) && (sum_bits(tmp_BOCH_dw) <= 1) && (sum_bits(tmp_FOS_dw) <= 1) && (sum_bits(tmp_AOS_dw) <= 1) && (sum_bits(tmp_UPCH_dw) <= 1)))
		ZOB.KMT = 1;

	///УМ
	if (MT_widget.current_UM == UM_OFF)
	{
		if (d_words.UM_com != d_words.LBV_com)
			ZOB.UM_LBV = 1;
	}
	else
	{
		if (MT_widget.current_UM != UM(d_words.UM_com) && (d_words.UM_com != 0))
			ZOB.KAT = 1;
	}
	///ЛБВ
	if (MT_widget.current_LBV == LBV_OFF)
	{
		if (d_words.LBV_com != d_words.UM_com)
			ZOB.UM_LBV = 1;
	}
	else
	{
		if (MT_widget.current_LBV != LBV(d_words.LBV_com) && (d_words.LBV_com != 0))
			ZOB.KAT = 1;
	}
	///БОЧ
	if (MT_widget.current_BOCH == BOCH_OFF)
	{
		if ((MT_widget.current_AOS != AOS_OFF) && (MT_widget.current_FOS != FOS_OFF) && (MT_widget.current_LBV != LBV_OFF))
			ZOB.BOCH_OOFF = 1;
	}
	else
	{
		if (BOCH_full.data_words == 0)
		{
			if ((MT_widget.current_AOS == AOS_OFF) || (MT_widget.current_FOS == FOS_OFF) || (MT_widget.current_LBV == LBV_OFF))
				ZOB.BOCH_OON;
		}
		else
			if (MT_widget.current_BOCH != BOCH(BOCH_full.data_words))
				ZOB.KAT = 1;
	}
	///ФОС
	if (MT_widget.current_FOS == FOS_OFF)
	{
		if (MT_widget.current_LBV != LBV_OFF)
			ZOB.FOS_OON = 1;
	}
	else
	{
		if (MT_widget.current_FOS != FOS(d_words.FOS_com) && (d_words.FOS_com != 0))
			ZOB.KAT = 1;
	}
	///АОС
	if (MT_widget.current_BOCH == BOCH_OFF)
	{
		if (BOCH_full.data_words == 0)
			ZOB.AOS_OON = 1;
	}
	else
	{
		if (BOCH_full.data_words == 0)
			ZOB.AOS_OON = 1;
	}
	///Аттенюаторы ФОС 1
	if ((d_words.FOS_att1 > 0xD) && (d_words.FOS_att1 < 0xF))
		ZOB.NZP = 1;
	///Аттенюаторы ФОС 2
	if ((d_words.FOS_att2 > 0xD) && (d_words.FOS_att2 < 0xF))
		ZOB.NZP = 1;
	///Аттенюаторы УПЧ
	if ((d_words.UPCH_att > 0xF) && (d_words.UPCH_att < 0x1F))
		ZOB.NZP = 1;

	if (ZOB.zob_word == 0)
	{
		MT_widget.current_UM = UM(d_words.UM_com);
		MT_widget.current_LBV = LBV(d_words.LBV_com);
		MT_widget.current_BOCH = BOCH(BOCH_full.data_words);
		MT_widget.current_FOS = FOS(d_words.FOS_com);
		MT_widget.current_AOS = AOS(d_words.AOS_com);
		MT_widget.current_UPCH = UPCH(d_words.UPCH_com);

		///ПРМ, антена 1
		MT_widget.current_PRM_Ant_1 = PRM(d_words.PRM_ant_one_com);
		///ПРМ, антена А
		MT_widget.current_PRM_Ant_A = PRM(d_words.PRM_ant_A_com);
		/// СТС СГС,антена 1 
		MT_widget.current_SGTS_Ant_1 = SGTS(d_words.SGTS_ant_one_com);
		/// СТС СГС,антена A
		MT_widget.current_SGTS_Ant_A = SGTS(d_words.SGTS_ant_A_com);

		///Частота генератора БОЧ
		MT_widget.current_BOCH_ch.BOCH_ch_D0 = d_words.BOCH_ch_D0;
		MT_widget.current_BOCH_ch.BOCH_ch_D1 = d_words.BOCH_ch_D1;
		MT_widget.current_BOCH_ch.BOCH_ch_D2 = d_words.BOCH_ch_D2;
		MT_widget.current_BOCH_ch.BOCH_ch_D3 = d_words.BOCH_ch_D3;
		MT_widget.current_BOCH_ch.BOCH_ch_D4 = d_words.BOCH_ch_D4;
		MT_widget.current_BOCH_ch.BOCH_ch_D5 = d_words.BOCH_ch_D5;
		MT_widget.current_BOCH_ch.BOCH_ch_D6 = d_words.BOCH_ch_D6;
		MT_widget.current_BOCH_ch.BOCH_ch_D7 = d_words.BOCH_ch_D7;

		MT_widget.current_FOS_att1 = d_words.FOS_att1;
		MT_widget.current_FOS_att2 = d_words.FOS_att2;
		///Ключ ПС в ФОС
		MT_widget.current_PS = FOS_PS(d_words.FOS_PS);
		MT_widget.current_UPCH_att = d_words.UPCH_att;
	}

	QVariantList tmp_list;
	tmp_list.push_back(ZOB.zob_word);

	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 16, tmp_list);
}

void AT_widg::update_graphics_AT()
{
	for (int i = 0; i < 3; i++)
	{
		ALPS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		MFS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		LPch_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		BUFAR_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	switch (current_ALPS)
	{
	case 1:
		ALPS_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		ALPS_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		ALPS_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_MFS)
	{
	case 1:
		MFS_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		MFS_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		MFS_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_LPch)
	{
	case 1:
		LPch_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		LPch_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		LPch_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_BUFAR)
	{
	case 1:
		BUFAR_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		BUFAR_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		BUFAR_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
}

void MT_widg::update_graphics_MT()
{
	for (int i = 0; i < 3; i++)
	{
		UM_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		LBV_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		BOCH_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		FOS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		AOS_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}

	switch (current_UM)
	{
	case 1:
		UM_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		UM_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		UM_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_LBV)
	{
	case 1:
		LBV_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		LBV_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		LBV_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_BOCH)
	{
	case 1:
		BOCH_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		BOCH_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		BOCH_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_FOS)
	{
	case 1:
		FOS_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		FOS_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		FOS_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_AOS)
	{
	case 1:
		AOS_pbut[2]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		AOS_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 4:
		AOS_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	for (int i = 0; i < 2; i++)
	{
		UPCH_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		PRM_Ant_1_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		PRM_Ant_A_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		SGTS_Ant_1_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		SGTS_Ant_A_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	switch (current_UPCH)
	{
	case 1:
		UPCH_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		UPCH_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_PRM_Ant_1)
	{
	case 1:
		PRM_Ant_1_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		PRM_Ant_1_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_PRM_Ant_A)
	{
	case 1:
		PRM_Ant_A_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		PRM_Ant_A_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_SGTS_Ant_1)
	{
	case 1:
		SGTS_Ant_1_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		SGTS_Ant_1_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
	switch (current_SGTS_Ant_A)
	{
	case 1:
		SGTS_Ant_A_pbut[1]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	case 2:
		SGTS_Ant_A_pbut[0]->setStyleSheet("background-color: rgb(142, 198, 156);");
		break;
	}
}

void BAU_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
		switch (tmp_cwd.subadr)
		{
		case 10:
			K_BAU(words);
			break;
		case 13:
			K_CBK();
		case 16:
			K_BRTK_M(words);
			MT_widget.update_graphics_MT();
			break;
		case 17:
			K_BRTK_A(words);
			AT_widget.update_graphics_AT();
			break;
		}
		if (ZOB.zob_word == 0)
		{
			ZTK_create();
			ZTM_create();
		}
		//Отправление ЗТК
		update_graphics_BAU();
	}

}

void BAU_widg::K_CBK()
{
	if (kcbk)
		K_CBK_timer->start(300000);
}

void BAU_widg::ZTM_create()
{
	for (int i = 0; i < 25; i++)
	{
		ZTM.data_words[i] = 0;
	}
	//1
	ZTM.OK_ALPS = AT_widget.current_ALPS;
	ZTM.OK_MFS = AT_widget.current_MFS;
	ZTM.TM_og = 1;
	ZTM.TM_STS_o = 0;
	ZTM.TM_STS_k = 0;
	ZTM.TM_SGS_o = 0;
	ZTM.TM_SGS_k = 0;
	ZTM.OK_LPch = AT_widget.current_LPch;
	ZTM.TM_LPch_k = 1;
	ZTM.TM_MFS_OZU = 1;
	//2
	ZTM.TM_MFS_PZU = 1;
	ZTM.TM_MFS_OB = 1;
	ZTM.TM_MFS_BS = 1;
	ZTM.TM_MFS_KS = 1;
	ZTM.TM_MFS_I = 1;
	ZTM.TM_MFS_STCH = 1;
	ZTM.TM_BFS = 1;
	ZTM.OK_BUFAR = AT_widget.current_BUFAR;
	ZTM.TM_PRM_ant_1 = MT_widget.current_PRM_Ant_1;
	ZTM.TM_PRM_ant_A = MT_widget.current_PRM_Ant_A;
	ZTM.TM_SGTS_ant_A = MT_widget.current_SGTS_Ant_A;
	//3
	ZTM.TM_SGTS_ant_1 = MT_widget.current_SGTS_Ant_1;
	ZTM.TM_UPCH = MT_widget.current_UPCH;
	ZTM.TM_FOS = MT_widget.current_FOS;
	ZTM.TM_PRM_ant_1_AP1 = 0;
	ZTM.TM_PRM_ant_a_AP2 = 0;
	ZTM.PNP_APD_MT = 0;
	ZTM.PNP_APD_AT = 0;
	ZTM.VNP_APD_MT = 0;
	//4,5
	ZTM.VNP_APD_AT = 0;
	ZTM.PP_CBA = 0;
	ZTM.VP_CBA = 0;
	ZTM.GOT_CBA = 0;
	ZTM.TM_BOCH = MT_widget.current_BOCH;
	switch (MT_widget.current_LBV)
	{
	case LBV_OFF:
		ZTM.TM_LBV_FP_1 = 0;
		ZTM.TM_LBV_FP_2 = 0;
		ZTM.TM_LBV_FP_3 = 0;
		break;
	case LBV_1:
		ZTM.TM_LBV_FP_1 = 1;
		ZTM.TM_LBV_FP_2 = 0;
		ZTM.TM_LBV_FP_3 = 0;
		break;
	case LBV_2:
		ZTM.TM_LBV_FP_1 = 0;
		ZTM.TM_LBV_FP_2 = 1;
		ZTM.TM_LBV_FP_3 = 0;
		break;
	case LBV_3:
		ZTM.TM_LBV_FP_1 = 0;
		ZTM.TM_LBV_FP_2 = 0;
		ZTM.TM_LBV_FP_3 = 1;
		break;
	}
	//5
	ZTM.TM_AOS = MT_widget.current_AOS;
	ZTM.TM_RBU_AOS = 0;
	ZTM.TM_RAOS = 0;
	ZTM.rez5 = 0;
	ZTM.SAK = 0;//Таблица 25
	if (kcbk)
	ZTM.TAK = 1;
	else
		ZTM.TAK = 1;
	//6
	ZTM.TM_ARU_UPch_1 = 0;
	ZTM.TM_ARU_UPch_2 = 0;
	//7
	ZTM.TM_Rout_FOS_1 = 0;
	ZTM.TM_Rout_FOS_2 = 0;
	//8
	ZTM.TM_Rout_FOS_3 = 0;
	ZTM.TM_Rout_LBV = 0;
	//9
	ZTM.TM_Ic_LBV = 0;
	ZTM.TM_Uc_LBV_1 = 0;
	//10
	ZTM.TM_Uc_LBV_2 = 0;
	ZTM.TM_Uc_LBV_3 = 0;
	//11
	ZTM.T_VIP_BAU = 0;
	ZTM.T_rez = 0;
	//12
	ZTM.TM_T1_LBV1 = 0;
	ZTM.TM_T2_LBV2 = 0;
	//13
	ZTM.TM_T3_LBV3 = 0;
	ZTM.TM_TD = 0;
	//14
	ZTM.KMK_123 = 0;
	ZTM.KMT_123 = 0;
	ZTM.rez14 = 0;
	ZTM.MK = current_MK;
	ZTM.MT = current_MT;
	//15
	ZTM.K15 = 0x2AA;//1010101010
	ZTM.rez15 = 0;
	//16
	ZTM.rez16 = 0;
	ZTM.KAD_A_17 = 0;
	ZTM.KAD_17 = 0;
	//17
	ZTM.rez17 = 0;
	ZTM.KAD_A_814 = 0;
	ZTM.KAD_814 = 0;
	//18
	ZTM.rez18 = 0;
	ZTM.KTD_A_13 = 0;
	ZTM.KTD13 = 0;
	//19
	ZTM.rez19 = 0;
	ZTM.KTD_A_46 = 0;
	ZTM.KTD46 = 0;
	//20
	ZTM.AD_rez_1 = 0;
	ZTM.AD_rez_2 = 0;
	//21
	ZTM.AD_rez_3 = 0;
	ZTM.AD_rez_4 = 0;
	//22
	ZTM.DD_rez_1 = 0;
	ZTM.DD_rez_3 = 0;
	ZTM.DD_rez_5 = 0;
	ZTM.DD_rez_7 = 0;
	ZTM.rez22 = 0;
	ZTM.Sch_TM = 0;
	//23
	ZTM.Sch_10ms = 0;
	//24
	ZTM.REsch = 0;
	ZTM.rezR = 0;
	ZTM.SBANK = 0;
	ZTM.rez2S = 0;
	ZTM.COPY = 0;
	ZTM.rezC = 0;
	ZTM.KP12 = 0;
	ZTM.KP11 = 0;
	ZTM.KP02 = 0;
	ZTM.KP01 = 0;
	ZTM.KS1 = 0;
	ZTM.KS0 = 0;
	//25
	ZTM.ABANK = 0;
	ZTM.PITB2 = 0;
	ZTM.PITB1 = 0;
	ZTM.Poff = 0;
	ZTM.Ptok = 0;
	ZTM.Pnapr = 0;
	ZTM.Ptime = 0;
	ZTM.PRon = 0;
	ZTM.PRtok = 0;
	ZTM.PRnapr = 0;
	ZTM.PRtime = 0;
	ZTM.rez25 = 0;

	QVariantList tmp_list;
	for (int i = 0; i < 25; i++)
		tmp_list.push_back(ZTM.data_words[i]);

	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 17, tmp_list);
}

void BAU_widg::ZTK_create()
{
	for (int i = 0; i < 3; i++)
	{
		ZTK.data_words[i] = 0;
	}

	Serv_Union tmp_LPch;
	tmp_LPch.bits = AT_widget.current_LPch;

	Serv_Union tmp_FOS_att2;
	tmp_FOS_att2.bits = MT_widget.current_FOS_att2;

	//1
	ZTK.UM = MT_widget.current_UM;
	ZTK.LBV = MT_widget.current_LBV;
	ZTK.PRM_ant_one_com = MT_widget.current_PRM_Ant_1;
	ZTK.PRM_ant_A_com = MT_widget.current_PRM_Ant_A;
	ZTK.SGTS_ant_one_com = MT_widget.current_SGTS_Ant_1;
	ZTK.SGTS_ant_A_com = MT_widget.current_SGTS_Ant_A;
	//2
	ZTK.BOCH = MT_widget.current_BOCH;
	ZTK.FOS = MT_widget.current_FOS;
	ZTK.AOS = MT_widget.current_AOS;
	ZTK.ALPS = AT_widget.current_ALPS;
	ZTK.MFS = AT_widget.current_MFS;
	ZTK.LPch_first_bit = tmp_LPch.first_bit;
	//3
	ZTK.LPch_sec_b = tmp_LPch.second_bit;
	ZTK.LPch_third_b = tmp_LPch.third_bit;
	ZTK.BUFAR = AT_widget.current_BUFAR;
	ZTK.UPCH = MT_widget.current_UPCH;
	ZTK.BOCH_CH = MT_widget.current_BOCH_ch.data_words;
	//4
	ZTK.FOS_att2_D0 = tmp_FOS_att2.first_bit;
	ZTK.FOS_att2_D1 = tmp_FOS_att2.second_bit;
	ZTK.FOS_att2_D2 = tmp_FOS_att2.third_bit;
	ZTK.FOS_att2_D3 = tmp_FOS_att2.fourth_bit;
	ZTK.FOS_att1 = MT_widget.current_FOS_att1;
	ZTK.FOS_PS = MT_widget.current_PS;
	ZTK.UPCH_att = MT_widget.current_UPCH_att;

	QVariantList tmp_list;
	for (int i = 0; i < 3; i++)
		tmp_list.push_back(ZTK.data_words[i]);

	omnibus_slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 18, tmp_list);
}

void BAU_widg::update_graphics_BAU()
{
	for (int i = 0; i < 3; i++)
	{
		MU_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_MU == i + 1)
			MU_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		MT_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_MT == i + 1)
			MT_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		MK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_MK == i + 1)
			MK_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
}

void BAU_widg::set_tm_state()
{
	mku_slot_thr.get_mku_bus_obj()->set_tm("BAU_TM", (uint)(tm_data.tm_data));
}

BAU_widg::~BAU_widg()
{
	//slot_thr.quit();
	//signal_thr.quit();

	//mku_slot_thr.quit();
	//mku_signal_thr.quit();
}

MT_widg::~MT_widg()
{

}

AT_widg::~AT_widg()
{

}