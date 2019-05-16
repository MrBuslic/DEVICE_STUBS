#include "cbk_imitator.h"

enum eControlChan
{
	controlOnCommon = 15,
	controlOnVM0 = 17,
	controlOnVM1 = 18,
	controlOnVM2 = 19,
	controlOnVM3 = 20,

	controlOffCommon = 16,
	controlOffVM0 = 21,
	controlOffVM1 = 22,
	controlOffVM2 = 23,
	controlOffVM3 = 24,

#ifdef OLD_CRATE
	controlHoldVM0 = 1,
	controlHoldVM1 = 2,
	controlHoldVM2 = 3,
	controlHoldVM3 = 4,
#else
	controlHoldVM0 = 21,
	controlHoldVM1 = 22,
	controlHoldVM2 = 23,
	controlHoldVM3 = 24,
#endif
};

CBK_MainWindow::CBK_MainWindow() : QMainWindow()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	QGridLayout *grid = new QGridLayout;
	set_str_combo();

	grid->addWidget(createVM1Group(), 0, 0);
	grid->addWidget(createVM2Group(), 0, 1);
	grid->addWidget(createVM3Group(), 0, 2);
	grid->addWidget(createVM4Group(), 0, 3);

	QGridLayout *vip_grid = new QGridLayout;
	vip_grid->addWidget(createMDS32Group(), 0, 0);
	vip_grid->addWidget(createMFSK24Group(), 0, 1);

	/////////
	Get_Time = new QPushButton("Время");
	////////
	edit = new QTextEdit(this);
	setMinimumSize(490, 500);
	v_lay->addLayout(grid);
	v_lay->addLayout(vip_grid);
	v_lay->addWidget(Get_Time);
	v_lay->addWidget(edit);

	QWidget *window = new QWidget();
	window->setLayout(v_lay);
	setCentralWidget(window);
	
	mds32_imit = new mds32_exchange();
	mfsk24_imit = new mfsk24_exchange();
	
	m_settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, tr("Комета"), tr("ЦБК"), this);
	QFile file("C:\\ProgramData\\Комета\\ЦБК.ini");
	if (file.exists())
	{
		read_settings();
	}

	QObject::connect(VM1_ON, SIGNAL(clicked()), this, SLOT(set_VM1_ON()));
	QObject::connect(VM2_ON, SIGNAL(clicked()), this, SLOT(set_VM2_ON()));
	QObject::connect(VM3_ON, SIGNAL(clicked()), this, SLOT(set_VM3_ON()));
	QObject::connect(VM4_ON, SIGNAL(clicked()), this, SLOT(set_VM4_ON()));
	QObject::connect(VM1_OFF, SIGNAL(clicked()), this, SLOT(set_VM1_OFF()));
	QObject::connect(VM2_OFF, SIGNAL(clicked()), this, SLOT(set_VM2_OFF()));
	QObject::connect(VM3_OFF, SIGNAL(clicked()), this, SLOT(set_VM3_OFF()));
	QObject::connect(VM4_OFF, SIGNAL(clicked()), this, SLOT(set_VM4_OFF()));
	QObject::connect(VM1_CRASH, SIGNAL(clicked()), this, SLOT(set_VM1_CRASH()));
	QObject::connect(VM2_CRASH, SIGNAL(clicked()), this, SLOT(set_VM2_CRASH()));
	QObject::connect(VM3_CRASH, SIGNAL(clicked()), this, SLOT(set_VM3_CRASH()));
	QObject::connect(VM4_CRASH, SIGNAL(clicked()), this, SLOT(set_VM4_CRASH()));
	QObject::connect(VM1_Combo, SIGNAL(currentIndexChanged(int)), this, SLOT(change_PO_VM1(int)));
	QObject::connect(VM2_Combo, SIGNAL(currentIndexChanged(int)), this, SLOT(change_PO_VM2(int)));
	QObject::connect(VM3_Combo, SIGNAL(currentIndexChanged(int)), this, SLOT(change_PO_VM3(int)));
	QObject::connect(VM4_Combo, SIGNAL(currentIndexChanged(int)), this, SLOT(change_PO_VM4(int)));
	QObject::connect(MDS_ON, SIGNAL(clicked()), this, SLOT(connectMDS()));
	QObject::connect(MFSK_ON, SIGNAL(clicked()), this, SLOT(connectMFSK()));
	QObject::connect(MDS_RE, SIGNAL(clicked()), this, SLOT(reconnectMDS()));
	QObject::connect(MFSK_RE, SIGNAL(clicked()), this, SLOT(reconnectMFSK()));
	QObject::connect(this, SIGNAL(vm_is_on(int)), this, SLOT(slot_vm_is_on(int)));
	QObject::connect(this, SIGNAL(vm_is_off(int)), this, SLOT(slot_vm_is_off(int)));
	QObject::connect(Get_Time, SIGNAL(clicked()), this, SLOT(show_time()));
	VM_init();
	WorkState_init();

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("cbk_geometry").toByteArray());
}

QGroupBox *CBK_MainWindow::createVM1Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление ВМ 1"));
	VM1_Label = new QLabel("ВМ 1");
	VM1_Label->setFixedSize(70, 35);
	VM1_Label->setAlignment(Qt::AlignCenter);

	VM1_Combo = new QComboBox();
	VM1_Combo->addItems(str_combo);
	VM1_Combo->setCurrentIndex(-1);
	VM1_Combo->setFixedWidth(70);

	VM1_ON = new QPushButton("ВМ1 ВКЛ");
	VM1_ON->setFixedWidth(70);
	VM1_OFF = new QPushButton("ВМ1 ВЫКЛ");
	VM1_OFF->setFixedWidth(70);
	VM1_CRASH = new QPushButton("ВМ1 Авария");
	VM1_CRASH->setFixedWidth(70);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(VM1_Label);
	vbox->addWidget(VM1_Combo);
	vbox->addWidget(VM1_ON);
	vbox->addWidget(VM1_OFF);
	vbox->addWidget(VM1_CRASH);
	vbox->addSpacing(2);
	vbox->setAlignment(Qt::AlignCenter);
	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *CBK_MainWindow::createVM2Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление ВМ 2"));
	VM2_Label = new QLabel("ВМ 2");
	VM2_Label->setFixedSize(70, 35);
	VM2_Label->setAlignment(Qt::AlignCenter);

	VM2_Combo = new QComboBox();
	VM2_Combo->addItems(str_combo);
	VM2_Combo->setCurrentIndex(-1);
	VM2_Combo->setFixedWidth(70);

	VM2_ON = new QPushButton("ВМ2 ВКЛ");
	VM2_ON->setFixedWidth(70);
	VM2_OFF = new QPushButton("ВМ2 ВЫКЛ");
	VM2_OFF->setFixedWidth(70);
	VM2_CRASH = new QPushButton("ВМ2 Авария");
	VM2_CRASH->setFixedWidth(70);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(VM2_Label);
	vbox->addWidget(VM2_Combo);
	vbox->addWidget(VM2_ON);
	vbox->addWidget(VM2_OFF);
	vbox->addWidget(VM2_CRASH);
	vbox->addSpacing(2);
	vbox->setAlignment(Qt::AlignCenter);
	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *CBK_MainWindow::createVM3Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление ВМ 3"));
	VM3_Label = new QLabel("ВМ 3");
	VM3_Label->setFixedSize(70, 35);
	VM3_Label->setAlignment(Qt::AlignCenter);

	VM3_Combo = new QComboBox();
	VM3_Combo->addItems(str_combo);
	VM3_Combo->setCurrentIndex(-1);
	VM3_Combo->setFixedWidth(70);

	VM3_ON = new QPushButton("ВМ3 ВКЛ");
	VM3_ON->setFixedWidth(70);
	VM3_OFF = new QPushButton("ВМ3 ВЫКЛ");
	VM3_OFF->setFixedWidth(70);
	VM3_CRASH = new QPushButton("ВМ3 Авария");
	VM3_CRASH->setFixedWidth(70);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(VM3_Label);
	vbox->addWidget(VM3_Combo);
	vbox->addWidget(VM3_ON);
	vbox->addWidget(VM3_OFF);
	vbox->addWidget(VM3_CRASH);
	vbox->addSpacing(2);
	vbox->setAlignment(Qt::AlignCenter);
	groupBox->setLayout(vbox);
	return groupBox;
}

QGroupBox *CBK_MainWindow::createVM4Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление ВМ 4"));
	VM4_Label = new QLabel("ВМ 4");
	VM4_Label->setFixedSize(70, 35);
	VM4_Label->setAlignment(Qt::AlignCenter);

	VM4_Combo = new QComboBox();
	VM4_Combo->addItems(str_combo);
	VM4_Combo->setCurrentIndex(-1);
	VM4_Combo->setFixedWidth(70);

	VM4_ON = new QPushButton("ВМ4 ВКЛ");
	VM4_ON->setFixedWidth(70);
	VM4_OFF = new QPushButton("ВМ4 ВЫКЛ");
	VM4_OFF->setFixedWidth(70);
	VM4_CRASH = new QPushButton("ВМ4 Авария");
	VM4_CRASH->setFixedWidth(70);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(VM4_Label);
	vbox->addWidget(VM4_Combo);
	vbox->addWidget(VM4_ON);
	vbox->addWidget(VM4_OFF);
	vbox->addWidget(VM4_CRASH);
	vbox->addSpacing(2);
	vbox->setAlignment(Qt::AlignCenter);
	groupBox->setLayout(vbox);
	return groupBox;
}
QGroupBox *CBK_MainWindow::createMDS32Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление МДС-32"));
	ipMDS_Label = new QLabel("IP сервера МДС-32:");
	ipMDS_Label->setFixedSize(130, 25);
	ipMDS_Label->setAlignment(Qt::AlignLeft);
	portMDS_Label = new QLabel("Порт сервера МДС-32:");
	portMDS_Label->setFixedSize(130, 25);
	portMDS_Label->setAlignment(Qt::AlignLeft);

	ipMDS_Edit = new QLineEdit();
	ipMDS_Edit->setFixedSize(70, 25);
	portMDS_Edit = new QLineEdit();
	portMDS_Edit->setFixedSize(70, 25);


	MDS_ON = new QPushButton("Connect");
	MDS_ON->setFixedWidth(100);
	MDS_RE = new QPushButton("Reconnect");
	MDS_RE->setFixedWidth(100);

	QGridLayout *gLay = new QGridLayout;
	gLay->addWidget(ipMDS_Label, 0, 0);
	gLay->addWidget(ipMDS_Edit, 0, 1);
	gLay->addWidget(portMDS_Label, 1, 0);
	gLay->addWidget(portMDS_Edit, 1, 1);
	gLay->addWidget(MDS_ON, 2, 0);
	gLay->addWidget(MDS_RE, 2, 1);
	groupBox->setLayout(gLay);
	return groupBox;
}

QGroupBox *CBK_MainWindow::createMFSK24Group()
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление МФСК-24"));
	ipMFSK_Label = new QLabel("IP сервера МФСК-24:");
	ipMFSK_Label->setFixedSize(130, 25);
	ipMFSK_Label->setAlignment(Qt::AlignLeft);
	portMFSK_Label = new QLabel("Порт сервера МФСК-24:");
	portMFSK_Label->setFixedSize(130, 25);
	portMFSK_Label->setAlignment(Qt::AlignLeft);

	ipMFSK_Edit = new QLineEdit();
	ipMFSK_Edit->setFixedSize(70, 25);
	portMFSK_Edit = new QLineEdit();
	portMFSK_Edit->setFixedSize(70, 25);


	MFSK_ON = new QPushButton("Connect");
	MFSK_ON->setFixedWidth(100);
	MFSK_RE = new QPushButton("Reconnect");
	MFSK_RE->setFixedWidth(100);

	QGridLayout *gLay = new QGridLayout;
	gLay->addWidget(ipMFSK_Label, 0, 0);
	gLay->addWidget(ipMFSK_Edit, 0, 1);
	gLay->addWidget(portMFSK_Label, 1, 0);
	gLay->addWidget(portMFSK_Edit, 1, 1);
	gLay->addWidget(MFSK_ON, 2, 0);
	gLay->addWidget(MFSK_RE, 2, 1);
	groupBox->setLayout(gLay);
	return groupBox;
}

void CBK_MainWindow::VM_init()
{
	for (int i = 0; i < 4; i++)
	{
		VMS.VMPowerState[i] = OFF;
	}
	set_VM1_OFF();
	set_VM2_OFF();
	set_VM3_OFF();
	set_VM4_OFF();
	VMS.VMPOState[0] = VM1_Combo->currentIndex();
	VMS.VMPOState[1] = VM2_Combo->currentIndex();
	VMS.VMPOState[2] = VM3_Combo->currentIndex();
	VMS.VMPOState[3] = VM4_Combo->currentIndex();
	for (int i = 0; i < 5; i++)
	{
		pitanie[i] = false;
	}
}

void CBK_MainWindow::WorkState_init()
{
	w_state.VM = -1;
	w_state.PO = -1;
}

void CBK_MainWindow::set_str_combo()
{
	str_combo.append("СПО БУ");
	str_combo.append("ТПО");
	str_combo.append("Свободно");
}

void CBK_MainWindow::set_VM1_ON()
{
	VMS.VMPowerState[0] = ON;
	VM1_Label->setStyleSheet("QLabel { background-color : green; color : black; }");
	emit(vm_is_on(1));
}

void CBK_MainWindow::set_VM2_ON()
{
	VMS.VMPowerState[1] = ON;
	VM2_Label->setStyleSheet("QLabel { background-color : green; color : black; }");
	emit(vm_is_on(2));
}

void CBK_MainWindow::set_VM3_ON()
{
	VMS.VMPowerState[2] = ON;
	VM3_Label->setStyleSheet("QLabel { background-color : green; color : black; }");
	emit(vm_is_on(3));
}

void CBK_MainWindow::set_VM4_ON()
{
	VMS.VMPowerState[3] = ON;
	VM4_Label->setStyleSheet("QLabel { background-color : green; color : black; }");
	emit(vm_is_on(4));
}

void CBK_MainWindow::set_VM1_OFF()
{
	VMS.VMPowerState[0] = OFF;
	VM1_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
	emit(vm_is_off(1));
}

void CBK_MainWindow::set_VM2_OFF()
{
	VMS.VMPowerState[1] = OFF;
	VM2_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
	emit(vm_is_off(2));
}

void CBK_MainWindow::set_VM3_OFF()
{
	VMS.VMPowerState[2] = OFF;
	VM3_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
	emit(vm_is_off(3));
}

void CBK_MainWindow::set_VM4_OFF()
{
	VMS.VMPowerState[3] = OFF;
	VM4_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
	emit(vm_is_off(4));
}

void CBK_MainWindow::set_VM1_CRASH()
{
	VMS.VMPowerState[0] = CRASH;
	VM1_Label->setStyleSheet("QLabel { background-color : red; color : black; }");
}

void CBK_MainWindow::set_VM2_CRASH()
{
	VMS.VMPowerState[1] = CRASH;
	VM2_Label->setStyleSheet("QLabel { background-color : red; color : black; }");
}

void CBK_MainWindow::set_VM3_CRASH()
{
	VMS.VMPowerState[2] = CRASH;
	VM3_Label->setStyleSheet("QLabel { background-color : red; color : black; }");
}

void CBK_MainWindow::set_VM4_CRASH()
{
	VMS.VMPowerState[3] = CRASH;
	VM4_Label->setStyleSheet("QLabel { background-color : red; color : black; }");
}

void CBK_MainWindow::change_PO_VM1(int index)
{
	QString str;
	str = "ВМ1";
	str += "\n";
	str += VM1_Combo->currentText();
	VM1_Label->setText(str);
	VM1_Label->setAlignment(Qt::AlignCenter);
	switch (index)
	{
	case 0:
		VMS.VMPOState[0] = SPOBU;
		break;
	case 1:
		VMS.VMPOState[0] = TPO;
		break;
	case 2:
		VMS.VMPOState[0] = VACANT;
		break;
	}
}

void CBK_MainWindow::change_PO_VM2(int index)
{
	QString str;
	str = "ВМ2";
	str += "\n";
	str += VM2_Combo->currentText();
	VM2_Label->setText(str);
	VM2_Label->setAlignment(Qt::AlignCenter);
	switch (index)
	{
	case 0:
		VMS.VMPOState[1] = SPOBU;
		break;
	case 1:
		VMS.VMPOState[1] = TPO;
		break;
	case 2:
		VMS.VMPOState[1] = VACANT;
		break;
	}
}

void CBK_MainWindow::change_PO_VM3(int index)
{
	QString str;
	str = "ВМ3";
	str += "\n";
	str += VM3_Combo->currentText();
	VM3_Label->setText(str);
	VM3_Label->setAlignment(Qt::AlignCenter);
	switch (index)
	{
	case 0:
		VMS.VMPOState[2] = SPOBU;
		break;
	case 1:
		VMS.VMPOState[2] = TPO;
		break;
	case 2:
		VMS.VMPOState[2] = VACANT;
		break;
	}
}

void CBK_MainWindow::change_PO_VM4(int index)
{
	QString str;
	str = "ВМ4";
	str += "\n";
	str += VM4_Combo->currentText();
	VM4_Label->setText(str);
	VM4_Label->setAlignment(Qt::AlignCenter);
	switch (index)
	{
	case 0:
		VMS.VMPOState[3] = SPOBU;
		break;
	case 1:
		VMS.VMPOState[3] = TPO;
		break;
	case 2:
		VMS.VMPOState[3] = VACANT;
		break;
	}
}

void CBK_MainWindow::write_settings()
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("cbk_geometry", saveGeometry());

	m_settings->beginGroup("CBK_State");
	m_settings->setValue("VM1_PO", VM1_Combo->currentIndex());
	m_settings->setValue("VM1_Lab", VM1_Label->text());
	m_settings->setValue("VM2_PO", VM2_Combo->currentIndex());
	m_settings->setValue("VM2_Lab", VM2_Label->text());
	m_settings->setValue("VM3_PO", VM3_Combo->currentIndex());
	m_settings->setValue("VM3_Lab", VM3_Label->text());
	m_settings->setValue("VM4_PO", VM4_Combo->currentIndex());
	m_settings->setValue("VM4_Lab", VM4_Label->text());
	m_settings->endGroup();
	m_settings->beginGroup("Connection_Settings");
	m_settings->setValue("MDS_Server_IP", ipMDS_Edit->text());
	m_settings->setValue("MDS_Server_Port", portMDS_Edit->text());
	m_settings->setValue("MFSK_Server_IP", ipMFSK_Edit->text());
	m_settings->setValue("MFSK_Server_Port", portMFSK_Edit->text());
	m_settings->endGroup();
	m_settings->sync();
}

void CBK_MainWindow::read_settings()
{
	QVariant c_ind = 0;
	QVariant l_str = "";
	m_settings->beginGroup("CBK_State");
	int index = m_settings->value("VM1_PO", c_ind).toInt();
	QString str = m_settings->value("VM1_Lab", l_str).toString();
	VM1_Combo->setCurrentIndex(index);
	VM1_Label->setText(str);
	VM1_Label->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM2_PO", c_ind).toInt();
	str = m_settings->value("VM2_Lab", l_str).toString();
	VM2_Combo->setCurrentIndex(index);
	VM2_Label->setText(str);
	VM2_Label->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM3_PO", c_ind).toInt();
	str = m_settings->value("VM3_Lab", l_str).toString();
	VM3_Combo->setCurrentIndex(index);
	VM3_Label->setText(str);
	VM3_Label->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM4_PO", c_ind).toInt();
	str = m_settings->value("VM4_Lab", l_str).toString();
	VM4_Combo->setCurrentIndex(index);
	VM4_Label->setText(str);
	VM4_Label->setAlignment(Qt::AlignCenter);
	m_settings->endGroup();
	m_settings->beginGroup("Connection_Settings");
	str = m_settings->value("MDS_Server_IP", l_str).toString();
	ipMDS_Edit->setText(str);
	str = m_settings->value("MDS_Server_Port", l_str).toString();
	portMDS_Edit->setText(str);
	str = m_settings->value("MFSK_Server_IP", l_str).toString();
	ipMFSK_Edit->setText(str);
	str = m_settings->value("MFSK_Server_Port", l_str).toString();
	portMFSK_Edit->setText(str);
	m_settings->endGroup();
}

void CBK_MainWindow::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::question(this, tr("Подтввердите"), tr("Завершить работу?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		write_settings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void CBK_MainWindow::connectMDS()
{
	QString ip_str = ipMDS_Edit->text();
	int port = portMDS_Edit->text().toInt();
	mds32_imit->create_signal_thread(ip_str, port);
	QObject::connect(mds32_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mds32_get_sample(int, uint&, int&)), this, SLOT(mds32_send_sample(int, uint&, int&)), Qt::BlockingQueuedConnection);
}

void CBK_MainWindow::reconnectMDS()
{
	return;
}

void CBK_MainWindow::connectMFSK()
{
	QString ip_str = ipMFSK_Edit->text();
	int port = portMFSK_Edit->text().toInt();
	mfsk24_imit->create_signal_thread(ip_str, port);
	QObject::connect(mfsk24_imit->get_mds32_exchange_thread()->get_obj().get(), SIGNAL(mfsk24_impulse_change(QVariantList)), this, SLOT(slot_mfsk24_impulse_change(QVariantList)), Qt::BlockingQueuedConnection);
}

void CBK_MainWindow::reconnectMFSK()
{
	return;
}

void CBK_MainWindow::mds32_send_sample(int channel, uint& buf, int& flag)
{
	switch (channel)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	{
		flag = 1;
		if (pitanie[channel - 1] == false)
		{
			buf = 1;
		}
		else
		{
			buf = 0;
		}
		break;
	}

	default:
		edit->append("The specified channel is not for us!");
	}
}

void CBK_MainWindow::slot_mfsk24_impulse_change(QVariantList channels)
{
	if ((channels.count() % 2) != 0)
	{
		edit->append("Error in size of channels array!");
		return;
	}
	QMap<int, int> channels_map;
	for (int i = 0; i < channels.count(); i += 2)
	{
		channels_map.insert(channels[i].toInt(), channels[i + 1].toInt());
	}
	if (channels_map.contains(controlOnCommon))
	{
		if ((channels_map[controlOnCommon] > 300) && (channels_map[controlOnCommon] < 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOnVM0 + i) && (channels_map[controlOnVM0 + i] > 300) && (channels_map[controlOnVM0 + i] < 100))
				{
					VMS.VMPowerState[i] = ON;
					pitanie[i] = true;
					edit->append(QString("Turning VM%1 ON").arg(i));
					switch (i)
					{
					case 0:
						set_VM1_ON();
						break;
					case 1:
						set_VM2_ON();
						break;
					case 2:
						set_VM3_ON();
						break;
					case 3:
						set_VM4_ON();
						break;
					default:
						break;
					}
				}
			}
		}
	}

	if (channels_map.contains(controlOffCommon))
	{
		if ((channels_map[controlOffCommon] > 300) && (channels_map[controlOffCommon] < 100))
		{
			for (int i = 0; i < 4; i++)
			{
				if (channels_map.contains(controlOffVM0 + i) && (channels_map[controlOffVM0 + i] > 300) && (channels_map[controlOffVM0 + i] < 100))
				{
					VMS.VMPowerState[i] = OFF;
					pitanie[i] = false;
					edit->append(QString("Turning VM%1 OFF").arg(i));
					switch (i)
					{
					case 0:
						set_VM1_OFF();
						break;
					case 1:
						set_VM2_OFF();
						break;
					case 2:
						set_VM3_OFF();
						break;
					case 3:
						set_VM4_OFF();
						break;
					default:
						break;
					}
				}
			}
		}
	}

}

void CBK_MainWindow::slot_vm_is_on(int n_vm)
{
	if (w_state.VM == -1)
	{
		w_state.VM = n_vm;
		w_state.PO = VMS.VMPOState[n_vm - 1];
		switch (VMS.VMPOState[n_vm - 1])
		{
		case(SPOBU):
			run_PO(SPOBU);
			break;
		case(TPO):
			run_PO(TPO);
			break;
		case(VACANT):
			break;
		}
		
		return;
	}
	if ((w_state.PO == VMS.VMPOState[n_vm - 1])&&(w_state.VM != n_vm))
	{
		VMS.VMPOState[n_vm - 1] = VACANT;
		switch (n_vm)
		{
		case 1:
			VM1_Combo->setCurrentIndex(VACANT);
			break;
		case 2:
			VM2_Combo->setCurrentIndex(VACANT);
			break;
		case 3:
			VM3_Combo->setCurrentIndex(VACANT);
			break;
		case 4:
			VM4_Combo->setCurrentIndex(VACANT);
			break;
		}
		QMessageBox::information(0, QString("VM -%1 is ON").arg(n_vm), "PO is changed to VAVANT");
		return;
	}
	if (((w_state.PO == SPOBU) && (VMS.VMPOState[n_vm - 1] == TPO))&&(w_state.VM != n_vm))
	{
		switch (n_vm)
		{
		case 1:
			VMS.VMPowerState[0] = OFF;
			VM1_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			break;
		case 2:
			VMS.VMPowerState[1] = OFF;
			VM2_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			break;
		case 3:
			VMS.VMPowerState[2] = OFF;
			VM3_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			break;
		case 4:
			VMS.VMPowerState[3] = OFF;
			VM4_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			break;
		}
		return;
	}
	if (((w_state.PO == TPO) && (VMS.VMPOState[n_vm - 1] == SPOBU)) && (w_state.VM != n_vm))
	{
		switch (w_state.VM)
		{
		case 1:
			VMS.VMPowerState[0] = OFF;
			VM1_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			shutdown_PO(TPO);
			break;
		case 2:
			VMS.VMPowerState[1] = OFF;
			VM2_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			shutdown_PO(TPO);
			break;
		case 3:
			VMS.VMPowerState[2] = OFF;
			VM3_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			shutdown_PO(TPO);
			break;
		case 4:
			VMS.VMPowerState[3] = OFF;
			VM4_Label->setStyleSheet("QLabel { background-color : grey; color : black; }");
			shutdown_PO(TPO);
			break;
		}
		w_state.VM = n_vm;
		w_state.PO = VMS.VMPOState[n_vm - 1];
		run_PO(SPOBU);
	}
}

void CBK_MainWindow::slot_vm_is_off(int n_vm)
{
	bool need_shutdown = TRUE;
	if (w_state.VM == n_vm)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((VMS.VMPowerState[i] == ON) && (i != (n_vm - 1)) && (VMS.VMPOState[i] == VACANT))
			{
				w_state.VM = i + 1;
				w_state.PO = VMS.VMPOState[i];
				switch (i)
				{
				case 0:
					VM1_Combo->setCurrentIndex(w_state.PO);
					break;
				case 1:
					VM2_Combo->setCurrentIndex(w_state.PO);
					break;
				case 2:
					VM3_Combo->setCurrentIndex(w_state.PO);
					break;
				case 3:
					VM4_Combo->setCurrentIndex(w_state.PO);
					break;
				}
				need_shutdown = FALSE;
				break;
			}
		}
		if (need_shutdown)
		{
			shutdown_PO(w_state.PO);
			w_state.VM = -1;
			w_state.PO = -1;
		}
		else
		{
			return;
		}	
	}
	else
	{
		return;
	}
}

void CBK_MainWindow::run_PO(int PO)
{
	switch (PO)
	{
	case(SPOBU):
		edit->append("Starting SPOBU");
		break;
	case(TPO):
		edit->append("Starting TPO");
		TimeThread& b_time(STimeThread::Instance());
		b_time.start();
		tpo_thread.start();
		interrupt_thread.start();
		break;
	}
}

void CBK_MainWindow::shutdown_PO(int PO)
{
	switch (PO)
	{
	case(SPOBU):
		edit->append("Shutdown SPOBU");
		break;
	case(TPO):
		edit->append("Shutdown TPO");
		interrupt_thread.stop_thread();
		interrupt_thread.wait(5000);
		tpo_thread.terminate();
		tpo_thread.wait(5000);
		STimeThread::Instance().quit();
		STimeThread::Instance().wait(5000);
		break;
	}
}

void CBK_MainWindow::show_time()
{
	int tmp_time = 0;
	tmp_time = STimeThread::Instance().getCurTime();
	edit->append(QString("Current time: %1").arg(tmp_time));
}