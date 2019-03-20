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

MBK07_widg::MBK07_widg(QWidget *parent)
{
	widg = new QWidget(this);
	setWindowTitle("МБК-07");
	FSMU_gb = new QGroupBox("ФСМУ", this);
	FSVU_gb = new QGroupBox("ФСВУ", this);

	mode_names.insert(full_mode::PI15, "ПИ15");
	mode_names.insert(full_mode::PI8, "ПИ8");
	mode_names.insert(full_mode::WTF8, "ВТФ8");

	stab_names.insert(STAB::LOW_STAB, "НС");
	stab_names.insert(STAB::HIGH_STAB, "ВС");
	stab_names.insert(STAB::KG1_STAB, "КГ 1");
	stab_names.insert(STAB::KG2_STAB, "КГ 2");

	ant_names.insert(ANTENNA::OHA, "OHA");
	ant_names.insert(ANTENNA::MHAPY, "MHA+Y");
	ant_names.insert(ANTENNA::MHAMY, "MHA-Y");

	for (int i = 0; i < 9; i++)
	{
		int tmp_d = 1 << i;
		lit_map.insert(tmp_d, LITERA(i + 1));
	}

	for (int i = 0; i < 3; i++)
	{
		QString numb = QString::number(i + 1);
		FSMU_blocks << new QPushButton(numb, this);
		FSVU_canals << new QPushButton(numb, this);
	}
	All_vblayout = new QHBoxLayout();
	QVBoxLayout *logs_lay = new QVBoxLayout(this);
	FSMUFSVU_vblayout = new QVBoxLayout();
	FSMU_hblayout = new QHBoxLayout(FSMU_gb);
	FSVU_hblayout = new QHBoxLayout(FSVU_gb);
	SubGrid_glayout = new QGridLayout();
	logs_lay->addLayout(All_vblayout);
	All_vblayout->addLayout(FSMUFSVU_vblayout);
	All_vblayout->addLayout(SubGrid_glayout);
	FSMUFSVU_vblayout->addWidget(FSMU_gb);
	FSMUFSVU_vblayout->addWidget(FSVU_gb);
	FSMU_gb->setAlignment(Qt::AlignHCenter);
	FSVU_gb->setAlignment(Qt::AlignHCenter);
	for (int i = 0; i < 3; i++)
	{
		FSMU_hblayout->addWidget(FSMU_blocks[i]);
		FSVU_hblayout->addWidget(FSVU_canals[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		FSMU_blocks[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		FSVU_canals[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
	}
	QLabel* Mode_lb = new QLabel("Режим");
	QLabel* Lit_lb = new QLabel("Литера");
	QLabel* Stab_lb = new QLabel("Стабильность");
	QLabel* Ann_lb = new QLabel("Антенна");
	QList<QLabel*> sub_lb_list;
	sub_lb_list << Mode_lb;
	sub_lb_list << Lit_lb;
	sub_lb_list << Stab_lb;
	sub_lb_list << Ann_lb;

	QLineEdit* Mode_le = new QLineEdit(this);
	Mode_le->setReadOnly(true);
	QLineEdit* Lit_le = new QLineEdit(this);
	Lit_le->setReadOnly(true);
	QLineEdit* Stab_le = new QLineEdit(this);
	Stab_le->setReadOnly(true);
	QLineEdit* Ann_le = new QLineEdit(this);
	Ann_le->setReadOnly(true);

	sub_le_list << Mode_le;
	sub_le_list << Lit_le;
	sub_le_list << Stab_le;
	sub_le_list << Ann_le;
	for (int i = 0; i < 4; i++)
	{
		SubGrid_glayout->addWidget(sub_lb_list[i], i, 0);
		SubGrid_glayout->addWidget(sub_le_list[i], i, 1);
	}

	edit = new QTextEdit(this);
	_scroll_bar = edit->verticalScrollBar();
	_doc = new QTextDocument();
	_cursor = new QTextCursor(_doc);
	edit->setDocument(_doc);
	edit->setReadOnly(true);
	_doc->setMaximumBlockCount(1000);
	setMinimumSize(490, 300);
	auto_scroll_box = new QCheckBox(this);
	auto_scroll_box->setText("Автопрокрутка");
	auto_scroll_box->setChecked(true);
	connect(auto_scroll_box, &QCheckBox::stateChanged, this, &MBK07_widg::auto_scroll_clicked);
	logs_lay->addWidget(edit);
	logs_lay->addWidget(auto_scroll_box);

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
	connect(&log_timer, &QTimer::timeout, this, &MBK07_widg::log_timer_ontimer);
	log_timer.start(200);

	set_new_tm();
}
void MBK07_widg::new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt)
{
	QString _msg = QString("%1 принял МК МШ%2 ПШ%3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(mshm).arg(pshm);
	msg_to_log(_msg);

	int tmp_mshm = mshm;
	int tmp_pshm = pshm - 8;

	switch (tmp_mshm)
	{
	case 0:
		current_FSMU = FSMU_numbB(tmp_pshm);
		break;
	case 1:
		current_stab = STAB(tmp_pshm);
		break;
	case 2:
		current_FSVU = FSVU_numbB(tmp_pshm); 
		break;
	case 3:
		current_antenna = ANTENNA(tmp_pshm); 
		break;
	}
	write_words();
	paint_buttons();
	set_new_tm();
}


void MBK07_widg::msg_to_log(const QString& _msg)
{
	{
		QMutexLocker lock(&log_mutex);
		log_buffer << _msg;
	}
	_cursor->insertText(_msg + "\n");
	if (auto_scroll)
		_scroll_bar->setValue(_scroll_bar->maximum());
}

void MBK07_widg::auto_scroll_clicked(int _state)
{
	auto_scroll = (_state != 0);
}

void MBK07_widg::log_timer_ontimer()
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

void MBK07_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr))
	{
		QString _msg = QString("%1 принял сигнал на подадресе %2 c КС %3").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(tmp_cwd.subadr).arg(tmp_cwd.com_word);
		msg_to_log(_msg);

		int tmp_word = words[0].toInt();
		QMap<int, QString>::iterator mode_itr;
		QMap<int, LITERA>::iterator lit_itr;
		QMap<int, QString>::iterator stab_itr;
		QMap<int, QString>::iterator ant_intr;

		switch (tmp_cwd.subadr)
		{
		case 2:


			char rezh;
			rezh = tmp_word & 7;



			mode_itr = mode_names.find(rezh);
			if (mode_itr == mode_names.end())
			{
				current_mode = ERR;
				break;
			}
			current_mode = full_mode(mode_itr.key());

			if (current_mode == PI8)
			{
				current_PSP = PSP(PSP_OFF);
			}

			pi8_fast = ((tmp_word & 0x40) != 0);

			IM = ((tmp_word & 0x10) != 0);
			break;
		case 3:

			byte liter;
			liter = tmp_word & 0xFF;
			lit_itr = lit_map.find(liter);
			if (lit_itr == lit_map.end())
			{
				current_lit = 0;
				break;
			}

			current_lit = LITERA(lit_itr.value());
			break;
		case 4:

			char psp_ch = tmp_word & 3;

			current_PSP = PSP(psp_ch + 1);

			break;
		};
		write_words();
		set_new_tm();
	}
}
void MBK07_widg::write_words()
{
	QString res_mode;
	if (mode_names.contains(current_mode))
	{
		res_mode = mode_names[current_mode];
		if (current_mode == PI8)
		{
			res_mode += QString("F%1 ПСП%2").arg((pi8_fast) ? "15" : "1.5").arg(current_PSP);
			//?все что перед - если, : -все что перед иначе.
		}
		if (IM == true)
			res_mode += " ИМ";
	}

	sub_le_list[0]->setText(res_mode);
	QString lit_num = QString::number(current_lit);
	sub_le_list[1]->setText(lit_num);
	sub_le_list[2]->setText(stab_names[current_stab]);
	sub_le_list[3]->setText(ant_names[current_antenna]);
}
void MBK07_widg::paint_buttons()
{
	for (int i = 0; i < 3; i++)
	{
		FSMU_blocks[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_FSMU == i)
			FSMU_blocks[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
	for (int i = 0; i < 3; i++)
	{
		FSVU_canals[i]->setStyleSheet("background-color: rgb(204, 204, 204);");
		if (current_FSVU == i)
			FSVU_canals[i]->setStyleSheet("background-color: rgb(142, 198, 156);");
	}
}

void MBK07_widg::set_new_tm()
{
	if ((current_FSMU == FSMU_OFF) || (current_stab == LOW_STAB))
	{
		if (ab_state)
		{
			ab_state = false;
			slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);
		}
		return;
	}
	else
	{
		if (!ab_state)
		{
			ab_state = true;
			slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, true);
		}
	}

	QVariantList tm_words;
	unsigned short f_word = 0;
	unsigned short s_word = 0;
	if (current_FSVU != FSVU_OFF)
		switch (current_FSVU)
		{
		case FSVU_One:
			f_word += 0x3000;
			break;
		case FSVU_Two:
			f_word += 0x2800;
			break;
		case FSVU_Three:
			f_word += 0x1800;
			break;
		}
	switch (current_FSMU)
	{
	case FSMU_One:
		f_word += 0x600;
		break;
	case FSMU_Two:
		f_word += 0x500;
		break;
	case FSMU_Three:
		f_word += 0x300;
		break;
	}
	if (pi8_fast)
		f_word += 0x40;
	if (IM)
		f_word += 0x10;
	if (current_mode != ERR)
		f_word += 1 << full_mode(current_mode) - 1;
	tm_words.push_back(f_word);


	if (current_lit != 0)
		s_word += 0x100 << current_lit - 1;
	s_word += 0x80 << STAB(current_stab) - 1;
	if (current_PSP != PSP_OFF)
		s_word += PSP(current_PSP) - 1;
	switch (current_antenna)
	{
	case OHA:
		s_word += 0xC;
		break;
	case MHAPY:
		s_word += 0x14;
		break;
	case MHAMY:
		s_word += 0x18;
		break;
	}
	tm_words.push_back(s_word);
	//Отправка 
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tm_words);
}