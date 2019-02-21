#include "BECH.h"

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

BECH_widg::BECH_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("БЭЧ");

	mode_names.insert(0, "ПИ-15");
	mode_names.insert(1, "ПИ-8");
	mode_names.insert(2, "Ошибка");

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		OG_pbut << new QPushButton(numb, this);
		FINIK_pbut << new QPushButton(numb, this);
		OG_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		FINIK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (i < 2)
		{
			QString numb = QString::number(i + 1);
			LKA_pbut << new QPushButton(numb, this);
			KP_pbut << new QPushButton(numb, this);
			LKA_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			KP_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		}
	}
	LKA_gb = new QGroupBox("ЛКА", this);
	OG_gb = new QGroupBox("ОГ", this);
	FINIK_gb = new QGroupBox("ФИНИК", this);
	KP_gb = new QGroupBox("КП", this);

	All_vblay = new QVBoxLayout(this);
	LKA_hlay = new QHBoxLayout(LKA_gb);
	OG_hlay = new QHBoxLayout(OG_gb);
	FINIK_hlay = new QHBoxLayout(FINIK_gb);
	KP_hlay = new QHBoxLayout(KP_gb);
	FINIK_REZH_hlay = new QHBoxLayout();

	for (int i = 0; i < 3; i++)
	{
		OG_hlay->addWidget(OG_pbut[i]);
		FINIK_hlay->addWidget(FINIK_pbut[i]);
		if (i < 2)
		{
			LKA_hlay->addWidget(LKA_pbut[i]);
			KP_hlay->addWidget(KP_pbut[i]);
		}
	}

	LKA_gb->setAlignment(Qt::AlignHCenter);
	FINIK_gb->setAlignment(Qt::AlignHCenter);;
	KP_gb->setAlignment(Qt::AlignHCenter);;
	OG_gb->setAlignment(Qt::AlignHCenter);;

	QLabel* Mode_lb = new QLabel("Режим");

	Mode_le = new QLineEdit(this);
	Mode_le->setReadOnly(true);

	FINIK_REZH_hlay->addWidget(Mode_lb);
	FINIK_REZH_hlay->addWidget(Mode_le);

	All_vblay->addWidget(LKA_gb);
	All_vblay->addWidget(OG_gb);
	All_vblay->addWidget(FINIK_gb);
	All_vblay->addWidget(KP_gb);
	All_vblay->addLayout(FINIK_REZH_hlay);

	//QVBoxLayout *logs_lay = new QVBoxLayout(this);
	//edit = new QTextEdit(this);
	//_scroll_bar = edit->verticalScrollBar();
	//_doc = new QTextDocument();
	//_cursor = new QTextCursor(_doc);
	//edit->setDocument(_doc);
	//edit->setReadOnly(true);
	//_doc->setMaximumBlockCount(1000);
	//setMinimumSize(490, 300);
	//auto_scroll_box = new QCheckBox(this);
	//auto_scroll_box->setText("Автопрокрутка");
	//auto_scroll_box->setChecked(true);
	//connect(auto_scroll_box, &QCheckBox::stateChanged, this, &BECH_widg::auto_scroll_clicked);
	//logs_lay->addWidget(edit);
	//logs_lay->addWidget(auto_scroll_box);


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

	lka05_slot_thr.set_connection_params("127.0.0.1", 50061);
	lka05_slot_thr.start(); // вот тут падает

	lka05_signal_thr.set_connection_params("127.0.0.1", 50062);
	lka05_signal_thr.start(); // вот тут падает

	if (!lka05_slot_thr.wait_connected(3) || !lka05_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с lka05");
		this->deleteLater();
		return;
	}

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));


	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	flag = true;

	connect(lka05_signal_thr.get_obj().get(), SIGNAL(new_mk(int, int, int, int, double, double, int)), this, SLOT(new_mk(int, int, int, int, double, double, int)));


	log_filename = QString("d:/logs/%1_%2.log").arg(QCoreApplication::applicationName()).arg(QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss"));
	QDir dir("d:/logs");
	if (!dir.exists())
		QDir().mkdir("d:/logs");
	connect(&log_timer, &QTimer::timeout, this, &BECH_widg::log_timer_ontimer);
	log_timer.start(200);

	//set_new_tm();
}


void BECH_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt)
{

	int uu = 0;
	//	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
	//	msg_to_log(_msg);
		int tmp_mshm = mshm;
		int tmp_pshm = pshm;
	
		switch (tmp_mshm)
		{
		case 3:
			current_LKA = LKA_1;
			break;
		case 4:
			current_LKA = LKA_2;
			break;
		case 8:
			current_KP = KP_1; 
			break;
		case 9:
			current_KP = KP_2; 
			break;
		}
		write_words();
		paint_buttons();
	//	set_new_tm();
}


void BECH_widg::msg_to_log(const QString& _msg)
{
	//{
	//	QMutexLocker lock(&log_mutex);
	//	log_buffer << _msg;
	//}
	//_cursor->insertText(_msg + "\n");
	//if (auto_scroll)
	//	_scroll_bar->setValue(_scroll_bar->maximum());
}

void BECH_widg::auto_scroll_clicked(int _state)
{
	//auto_scroll = (_state != 0);
}

void BECH_widg::log_timer_ontimer()
{
	/*QStringList tmp_buffer;
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
	log_file.close();*/
}

void BECH_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		//	QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		//	msg_to_log(_msg);

		int tmp_word = words[0].toInt();
		
		char reset = tmp_word & 0x1F;
		if (reset != 15)
		{
			char finik_rezh_ch = tmp_word & 0x30;
			//	char lka_ch = 1; мат ком
			//	char kp_ch = 1; мат ком
			char tmp = tmp_word & 3;
			if ((tmp_word & 3) > 0)
				current_OG = OG((tmp_word & 3) - 1);
			if ((tmp_word & 0xC) >> 2 > 0)
				current_FINIK = FINIK(((tmp_word & 0xC) >> 2) - 1);
			switch (finik_rezh_ch)
			{
			case 16:
				current_FINIK_REZH = FINIK_REZH_PI8;
				break;
			case 32:
				current_FINIK_REZH = FINIK_REZH_PI15;
				break;
			}
		}
		else
		{
			current_OG = OG_1;
			current_FINIK = FINIK_1;
			current_FINIK_REZH = FINIK_REZH_PI8;
		}
			write_words();
			paint_buttons();
		//	set_new_tm();
	}
}
void BECH_widg::write_words()
{
		if (mode_names.contains(current_FINIK_REZH))
			Mode_le->setText(mode_names[current_FINIK_REZH]);
}
void BECH_widg::paint_buttons()
{
	for (int i = 0; i < 3; i++)
	{
		OG_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_OG == i)
			OG_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		FINIK_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_FINIK == i)
			FINIK_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");

		if (i < 2)
		{
			LKA_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			if (current_LKA == i)
				LKA_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
			KP_pbut[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
			if (current_KP == i)
				KP_pbut[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
		}
	}
}

void BECH_widg::set_new_tm()
{
	//if ((current_FSMU == FSMU_OFF) || (current_stab == LOW_STAB))
	//{
	//	if (ab_state)
	//	{
	//		ab_state = false;
	//		slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
	//	}
	//	return;
	//}
	//else
	//{
	//	if (!ab_state)
	//	{
	//		ab_state = true;
	//		slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
	//	}
	//}

	//QVariantList tm_words;
	//unsigned short f_word = 0;
	//unsigned short s_word = 0;
	//if (current_FSVU != FSVU_OFF)
	//	switch (current_FSVU)
	//	{
	//	case FSVU_One:
	//		f_word += 0x3000;
	//		break;
	//	case FSVU_Two:
	//		f_word += 0x2800;
	//		break;
	//	case FSVU_Three:
	//		f_word += 0x1800;
	//		break;
	//	}
	//switch (current_FSMU)
	//{
	//case FSMU_One:
	//	f_word += 0x600;
	//	break;
	//case FSMU_Two:
	//	f_word += 0x500;
	//	break;
	//case FSMU_Three:
	//	f_word += 0x300;
	//	break;
	//}
	////шта?
	//if (pi8_fast)
	//	f_word += 0x40;
	//if (IM)
	//	f_word += 0x10;
	//if (current_mode != ERR)
	//	f_word += 1 << full_mode(current_mode) - 1;
	//tm_words.push_back(f_word);


	//if (current_lit != 0)
	//	s_word += 0x100 << current_lit - 1;
	//s_word += 0x80 << STAB(current_stab) - 1;
	//if (current_PSP != PSP_OFF)
	//	s_word += PSP(current_PSP) - 1;
	//switch (current_antenna)
	//{
	//case OHA:
	//	s_word += 0xC;
	//	break;
	//case MHA1Y:
	//	s_word += 0x14;
	//	break;
	//case MHA0Y:
	//	s_word += 0x18;
	//	break;
	//}
	//tm_words.push_back(s_word);
	////Отправка 
	//slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tm_words);
}