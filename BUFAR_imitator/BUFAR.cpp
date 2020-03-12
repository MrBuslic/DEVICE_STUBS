#include "BUFAR.h"

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

union I_1_DataWords
{
	quint16 data_words[3];

	I_1_DataWords(QVariantList raw_words)
	{
		for (int i = 0; i < raw_words.size(); i++)
			this->data_words[i] = static_cast<quint16>(raw_words.at(i).toInt());
	}

	struct
	{
		quint16
			AFAR_mode : 4,
			rez : 12;
		quint16
			OY_AFAR : 15,
			OY_pl_mn : 1;
		quint16
			OZ_AFAR : 15,
			OZ_pl_mn : 1;
	};
};

union I_2_DataWords
{
	quint16 data_words[15];
	I_2_DataWords(QVariantList raw_words)
	{
		for (int i = 0; i < raw_words.size(); i++)
			this->data_words[i] = static_cast<quint16>(raw_words.at(i).toInt());
	}
	struct
	{
		quint16
			BUM1_16 : 16;
		quint16
			BUM_17_25 : 9,
			rez : 3,
			AFAR_mode : 4;
		quint16
			BUM_1_Power_1_4 : 4,
			BUM_Power : 1,
			rez_1 : 3,
			BUM_2_Power_2_4 : 4,
			rez_2 : 3;
			;
	};

};

//void BUM_chan :: default_button_state()
//{
//	BUM_chan_pbut->setText("");
//	BUM_chan_pbut->setMaximumWidth(16);
//	BUM_chan_pbut->setMaximumHeight(16);
//	BUM_chan_pbut->setStyleSheet("background-color: rgb(204, 204, 204);");
//	BUM_chan_pbut->setCheckable(false);
//}

void BUM_cl::create_BUMs()
{
	BUMs_gbox = new QGroupBox("");
	BUMs_glay = new QGridLayout(BUMs_gbox);

	for (int n = 0; n < 2; n ++)
	{
		for (int i = 0; i < 2; i ++)
		{
			QString numb = QString::number(i + n * 2);
			BUMs_pbut << new QPushButton(numb);
			BUMs_pbut[i + n * 2]->setText("");
			BUMs_pbut[i + n * 2]->setMaximumWidth(16);
			BUMs_pbut[i + n * 2]->setMaximumHeight(16);
			BUMs_pbut[i + n * 2]->setStyleSheet("background-color: rgb(204, 204, 204);");
			BUMs_pbut[i + n * 2]->setCheckable(false);
			BUMs_glay->addWidget(BUMs_pbut[i + n * 2], i, n);
		}
	}
}

BUFAR_widg::BUFAR_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("БУФАР");

	All_vlay = new QVBoxLayout(this);
	BUFAR_cm_vlay = new QVBoxLayout();
	BUFAR_compl_hlay = new QHBoxLayout();
	BUFAR_mode_hlay = new QHBoxLayout();
	BUMs_glay = new QGridLayout();

	BUFAR_comp_lb = new QLabel(this);
	BUFAR_comp_le = new QLineEdit(this);
	BUFAR_mode_lb = new QLabel(this);
	BUFAR_mode_le = new QLineEdit(this);

	BUFAR_comp_lb->setText("Комплект");
	BUFAR_comp_le->setReadOnly(true);
	BUFAR_comp_le->setMaximumWidth(150);
	BUFAR_comp_le->setMinimumWidth(150);
	BUFAR_mode_lb->setText("Режим");
	BUFAR_mode_le->setReadOnly(true);
	BUFAR_mode_le->setMaximumWidth(150);
	BUFAR_mode_le->setMinimumWidth(150);

	All_vlay->addLayout(BUFAR_cm_vlay);
	BUFAR_cm_vlay->addLayout(BUFAR_compl_hlay);
	BUFAR_cm_vlay->addLayout(BUFAR_mode_hlay);
	All_vlay->addLayout(BUMs_glay);

	BUFAR_compl_hlay->addWidget(BUFAR_comp_lb);
	BUFAR_compl_hlay->addWidget(BUFAR_comp_le);
	BUFAR_mode_hlay->addWidget(BUFAR_mode_lb);
	BUFAR_mode_hlay->addWidget(BUFAR_mode_le);
	
	for (int i = 0; i < 25; i++)
	{
		BUM_current tmp_BUM;
		BUM_current_list.push_back(tmp_BUM);
	}

	//spaceitem - нужно ли?

	for (int n = 0; n < 5; n++)
	{
		for (int i = 0; i < 5; i++)
		{
			BUM_cl tmp_BUM_cl;
			tmp_BUM_cl.create_BUMs();
			BUM_cl_list.push_back(tmp_BUM_cl);
			BUMs_glay->addWidget(BUM_cl_list.at(i+n*5).BUMs_gbox, i, n);
		}
	}

	//BUM_cl_list.at(1).BUMs_pbut.at(1)->setStyleSheet();

	//slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	//slot_thr.start(); // вот тут падает

	//signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	//signal_thr.start(); // вот тут падает

	//if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	//{
	//	QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
	//	this->deleteLater();
	//	return;
	//}

	//power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	//power_slot_thr.start(); // вот тут падает

	//power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	//power_signal_thr.start(); // вот тут падает

	//if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	//{
	//	QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
	//	this->deleteLater();
	//	return;
	//}

	//connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	//slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	//connect(mku_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int, int, int)), this, SLOT(new_mk(int, int, int, int, double, double, int, int, int)));
	//connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	//log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	//QDir dir("d:/logs");
	//if (!dir.exists())
	//	QDir().mkdir("d:/logs");
	////connect(&log_timer, &QTimer::timeout, this, &BECH_widg::log_timer_ontimer);
	//log_timer.start(200);


	//edit = new QTextEdit(this);
	//_scroll_bar = edit->verticalScrollBar();
	//_doc = new QTextDocument();
	//_cursor = new QTextCursor(_doc);
	//edit->setDocument(_doc);
	//edit->setReadOnly(true);
	//_doc->setMaximumBlockCount(1000);
	//setMinimumSize(360, 300);
	//auto_scroll_box = new QCheckBox(this);
	//auto_scroll_box->setText("Автопрокрутка");
	//auto_scroll_box->setChecked(true);
	////connect(auto_scroll_box, &QCheckBox::stateChanged, this, &BECH_widg::auto_scroll_clicked);

	//QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	//restoreGeometry(settings.value("bech_geometry").toByteArray());
}

void BUFAR_widg::get_power(double _volt)
{
	volt = _volt;
	if (volt >= 20.0)
		imit_on();
	else
		if (volt < 1)
			imit_off();
}

void BUFAR_widg::set_power_back()
{
	double curr;
	if (power_vt >= 1)
	{
		curr = (double)power_vt / volt;
	}
	else
		curr = 0.0;
	//power_slot_thr.get_power_bus_obj()->set_i(bus, name, curr);
}

void BUFAR_widg::imit_off()
{
	msg_to_log("Питание отключено");
	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	update_graphics();
}

void BUFAR_widg::imit_on()
{
	change_power(true);
	update_graphics();
	set_new_tm();
}

void BUFAR_widg::change_power(bool)
{

}

void BUFAR_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p)
{
	if (volt != 0)
	{
		int uu = 0;
		//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
		//	msg_to_log(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;

		update_graphics();
		set_new_tm();
	}
}


void BUFAR_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void BUFAR_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void BUFAR_widg::log_timer_ontimer()
{
	QStringList tmp_buffer;
	{
		QMutexLocker lock(&log_mutex);
		tmp_buffer = log_buffer;
		log_buffer.clear();
	}
	if (tmp_buffer.isEmpty())
		return;
	QFile log_file(log_filename);
	QTextStream log_stream(&log_file);
	log_file.open(QIODevice::Append);
	for (QStringList::iterator itr = tmp_buffer.begin(); itr != tmp_buffer.end(); itr++)
		log_stream << *itr << "\n";
	log_file.close();
}

void BUFAR_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		//	QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//	msg_to_log(_msg);
		switch (tmp_cwd.subadr)
		{
		case 1:
			I_1(words);
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		}
		update_graphics();
		set_new_tm();
	}
}

void BUFAR_widg::change_BUMs(int num, bool ch_1, bool ch_2, bool ch_3, bool ch_4)
{
	BUM_current_list[num].chan_1 = ch_1;
	BUM_current_list[num].chan_2 = ch_2;
	BUM_current_list[num].chan_3 = ch_3;
	BUM_current_list[num].chan_4 = ch_4;
}

void BUFAR_widg::change_BUMs(int num, bool all_ch)
{
	BUM_current_list[num].chan_1 = all_ch;
	BUM_current_list[num].chan_2 = all_ch;
	BUM_current_list[num].chan_3 = all_ch;
	BUM_current_list[num].chan_4 = all_ch;
}

void BUFAR_widg::I_1(QVariantList words)
{
	I_1_DataWords d_words = words;
	switch (d_words.AFAR_mode)
	{
	case 0:
		for (int i = 0; i < 25; i++)
		{
			change_BUMs(i, false);
		}
		break;
	case 1:
		for (int i = 0; i < 25; i++)
		{
			change_BUMs(i, false);
		}
		change_BUMs(1,true);
		change_BUMs(2, true);
		change_BUMs(6, true);
		change_BUMs(7, true);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		for (int i = 0; i < 25; i++)
		{
			change_BUMs(i, false);
		}
		break;
	}
	if (d_words.OY_pl_mn != OY_faze)
		OY_faze = -1 * OY_faze;
	if (d_words.OZ_pl_mn != OZ_faze)
		OZ_faze = -1 * OZ_faze;
}

void BUFAR_widg::update_graphics()
{
	
}

void BUFAR_widg::set_new_tm()
{
	//omnibus_slot_thr.get_omnibus_obj()->switch_ab_os()
	// slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tmp_list);
}

BUFAR_widg::~BUFAR_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void BUFAR_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("bech_geometry", saveGeometry());
	QWidget::closeEvent(event);
}