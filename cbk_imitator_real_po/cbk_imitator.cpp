#include "cbk_imitator.h"
#include "rpc_ports.h"

#include "cbk_socket_rpc.h"
#include "mko_module.h"


CBK_MainWindow::CBK_MainWindow() : QMainWindow()
{
	QVBoxLayout* v_lay = new QVBoxLayout(this);
	QGridLayout *grid = new QGridLayout;
	set_str_combo();

	grid->addWidget(createVMGroup(0), 0, 0);
	grid->addWidget(createVMGroup(1), 0, 1);
	grid->addWidget(createVMGroup(2), 0, 2);
	grid->addWidget(createVMGroup(3), 0, 3);

	/////////
	Get_Time = new QPushButton("Время");
	////////
	edit = new QTextEdit(this);
	setMinimumSize(490, 300);
	v_lay->addLayout(grid);
	v_lay->addWidget(Get_Time);
	v_lay->addWidget(edit);

	QWidget *window = new QWidget();
	window->setLayout(v_lay);
	setCentralWidget(window);
	
	
	m_settings = new QSettings(QSettings::IniFormat, QSettings::SystemScope, tr("Комета"), tr("ЦБК"), this);
	QFile file("C:\\ProgramData\\Комета\\ЦБК.ini");
	if (file.exists())
	{
		read_settings();
	}


	for (int i = 0; i < 4; i++)
	{
		QTimer* tmp_timer = new QTimer;
		tmp_timer->setInterval(SPO_START_TIME);
		tmp_timer->setSingleShot(true);
		connect(tmp_timer, &QTimer::timeout, this, &CBK_MainWindow::on_timeout);

		on_timers.insert(tmp_timer, i);
		on_timers_.insert(i, tmp_timer);
	}

	//QObject::connect(this, SIGNAL(vm_is_on(int)), this, SLOT(slot_vm_is_on(int)));
	//QObject::connect(this, SIGNAL(vm_is_off(int)), this, SLOT(slot_vm_is_off(int)));
	QObject::connect(Get_Time, SIGNAL(clicked()), this, SLOT(show_time()));


	power_slot_thr.set_connection_params("127.0.0.1", POWER_SLOT);
	power_slot_thr.start();

	power_signal_thr.set_connection_params("127.0.0.1", POWER_SIGNAL);
	power_signal_thr.start();

	if (!power_slot_thr.wait_connected(3) || !power_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с power_bus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start();

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku_bus");
		this->deleteLater();
		return;
	}

	MkoImitObject& mko_imit(SMkoImitObject::Instance());
	mko_imit.create_slot_thread();
	QObject::connect(&mko_imit, SIGNAL(signal_send_msg_mko(int, int, int, QVariantList&, int&)), mko_imit.get_mko_slot_thread()->get_omnibus_obj().get(), SLOT(send_msg(int, int, int, QVariantList&, int&)), Qt::DirectConnection);
	interrupt_thread.start();

	//QString ip_str = "127.0.0.1";
	//int slot_port = CBK_SLOT;
	//int signal_port = CBK_SIGNAL;
	//cbk_Socket_RPC_SLOT_Server_Thread* rpc_slot_srv = new cbk_Socket_RPC_SLOT_Server_Thread;
	//rpc_slot_srv->set_app(this);
	//rpc_slot_srv->set_params(ip_str, slot_port);
	//rpc_slot_srv->start();
	//cbk_Socket_RPC_SIGNAL_Thread* rpc_signal_srv = new cbk_Socket_RPC_SIGNAL_Thread;
	//rpc_signal_srv->set_app(this);
	//rpc_signal_srv->set_params(ip_str, signal_port);
	//rpc_signal_srv->start();


	VM_init();
	WorkState_init();

	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));
	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku_cbk(int, int, double, int)), this, SLOT(new_ku(int, int, double, int)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("cbk_geometry").toByteArray());
}

QGroupBox *CBK_MainWindow::createVMGroup(int n_vm)
{
	QGroupBox *groupBox = new QGroupBox(tr("Управление ВМ 1"));
	QLabel* VM_label = new QLabel("ВМ 1");
	VM_label->setFixedSize(70, 35);
	VM_label->setAlignment(Qt::AlignCenter);


	QComboBox* VM_Combo = new QComboBox();
	VM_Combo->addItems(str_combo);
	VM_Combo->setCurrentIndex(-1);
	VM_Combo->setFixedWidth(70);
	connect(VM_Combo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &CBK_MainWindow::change_PO_VM);


	QPushButton* VM_ON = new QPushButton("ВМ1 ВКЛ");
	VM_ON->setFixedWidth(70);
	connect(VM_ON, &QPushButton::clicked, this, &CBK_MainWindow::VM_ON_clicked);

	QPushButton* VM_OFF = new QPushButton("ВМ1 ВЫКЛ");
	VM_OFF->setFixedWidth(70);
	connect(VM_OFF, &QPushButton::clicked, this, &CBK_MainWindow::VM_OFF_clicked);

	QPushButton* VM_CRASH = new QPushButton("ВМ1 Авария");
	VM_CRASH->setFixedWidth(70);
	connect(VM_CRASH, &QPushButton::clicked, this, &CBK_MainWindow::VM_CRASH_clicked);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(VM_label);
	VM_Labels << VM_label;
	vbox->addWidget(VM_Combo);
	VM_Combos << VM_Combo;
	vbox->addWidget(VM_ON);
	ON_btns << VM_ON;
	vbox->addWidget(VM_OFF);
	OFF_btns << VM_OFF;
	vbox->addWidget(VM_CRASH);
	CRASH_btns << VM_CRASH;
	vbox->addSpacing(2);
	vbox->setAlignment(Qt::AlignCenter);
	groupBox->setLayout(vbox);
	return groupBox;
}


void CBK_MainWindow::VM_init()
{
	w_state.VM = -1;
	w_state.PO = -1;
	for (int i = 0; i < 4; i++)
	{
		VMS.VMPowerState[i] = OFF;
		VMS.running[i] = false;
	}
	set_VM_OFF(0);
	set_VM_OFF(1);
	set_VM_OFF(2);
	set_VM_OFF(3);

	VMS.VMPOState[0] = VM_Combos[0]->currentIndex();
	VMS.VMPOState[1] = VM_Combos[1]->currentIndex();
	VMS.VMPOState[2] = VM_Combos[2]->currentIndex();
	VMS.VMPOState[3] = VM_Combos[3]->currentIndex();

	for (int i = 0; i < 5; i++)
	{
		pitanie[i] = false;
	}
	set_tm_state();
}

void CBK_MainWindow::WorkState_init()
{

}

void CBK_MainWindow::set_str_combo()
{
	str_combo.append("СПО БУ");
	str_combo.append("ТПО");
	str_combo.append("Свободно");
}

void CBK_MainWindow::VM_ON_clicked()
{
	int n_vm = -1;
	for (int i = 0; i < 4; i++)
	{
		if (ON_btns[i] == sender())
			n_vm = i;
	}
	if (n_vm == -1)
		return;
	set_VM_ON(n_vm);
}

void CBK_MainWindow::set_VM_ON(int n_vm)
{
	VMS.VMPowerState[n_vm] = ON;
	pitanie[n_vm+1] = true;
	VM_Labels[n_vm]->setStyleSheet("QLabel { background-color : green; color : black; }");
	//emit(vm_is_on(n_vm));
	VMS.StartTime[n_vm] = QDateTime::currentMSecsSinceEpoch();
	on_timers_[n_vm]->start();

	set_tm_state();
}

void CBK_MainWindow::VM_OFF_clicked()
{
	int n_vm = -1;
	for (int i = 0; i < 4; i++)
	{
		if (OFF_btns[i] == sender())
			n_vm = i;
	}
	if (n_vm == -1)
		return;
	set_VM_OFF(n_vm);
}

void CBK_MainWindow::set_VM_OFF(int n_vm)
{
	VMS.VMPowerState[n_vm] = OFF;
	VMS.running[n_vm] = false;
	pitanie[n_vm+1] = false;
	VM_Labels[n_vm]->setStyleSheet("QLabel { background-color : grey; color : black; }");
	if (STimeThread::Instance().cbk_conf.vm & (1 << n_vm))
		STimeThread::Instance().cbk_conf.vm ^= (1 << n_vm);

	STimeThread::Instance().cbk_conf.of |= (1 << n_vm);

	STimeThread::Instance().cbk_conf.er |= (1 << n_vm);

	if ((STimeThread::Instance().cbk_conf.vm == 0) && (w_state.PO != -1))
	{
		shutdown_PO(w_state.PO);
		STimeThread::Instance().cbk_conf.rs = 0;
		STimeThread::Instance().cbk_conf.of = 0xF;
		STimeThread::Instance().cbk_conf.er = 0xF;
	}
	on_timers_[n_vm]->stop();
	set_tm_state();
}

void CBK_MainWindow::VM_CRASH_clicked()
{
	int n_vm = -1;
	for (int i = 0; i < 4; i++)
	{
		if (CRASH_btns[i] == sender())
			n_vm = i;
	}
	if (n_vm == -1)
		return;
	set_VM_CRASH(n_vm);
}

void CBK_MainWindow::set_VM_CRASH(int n_vm)
{
	VMS.VMPowerState[n_vm] = CRASH;
	VM_Labels[n_vm]->setStyleSheet("QLabel { background-color : red; color : black; }");
}

void CBK_MainWindow::change_PO_VM(int index)
{
	int n_vm = -1;
	for (int i = 0; i < 4; i++)
	{
		if (VM_Combos[i] == sender())
			n_vm = i;
	}
	if (n_vm == -1)
		return;

	QString str;
	str = QString("ВМ%1\n%2").arg(n_vm+1).arg(VM_Combos[n_vm]->currentText());
	VM_Labels[n_vm]->setText(str);
	VM_Labels[n_vm]->setAlignment(Qt::AlignCenter);
	switch (index)
	{
	case 0:
		VMS.VMPOState[n_vm] = SPOBU;
		break;
	case 1:
		VMS.VMPOState[n_vm] = TPO;
		break;
	case 2:
		VMS.VMPOState[n_vm] = VACANT;
		break;
	}
}

void CBK_MainWindow::write_settings()
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("cbk_geometry", saveGeometry());

	m_settings->beginGroup("CBK_State");
	m_settings->setValue("VM1_PO", VM_Combos[0]->currentIndex());
	m_settings->setValue("VM1_Lab", VM_Labels[0]->text());
	m_settings->setValue("VM2_PO", VM_Combos[1]->currentIndex());
	m_settings->setValue("VM2_Lab", VM_Labels[1]->text());
	m_settings->setValue("VM3_PO", VM_Combos[2]->currentIndex());
	m_settings->setValue("VM3_Lab", VM_Labels[2]->text());
	m_settings->setValue("VM4_PO", VM_Combos[3]->currentIndex());
	m_settings->setValue("VM4_Lab", VM_Labels[3]->text());
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
	VM_Combos[0]->setCurrentIndex(index);
	VM_Labels[0]->setText(str);
	VM_Labels[1]->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM2_PO", c_ind).toInt();
	str = m_settings->value("VM2_Lab", l_str).toString();
	VM_Combos[1]->setCurrentIndex(index);
	VM_Labels[1]->setText(str);
	VM_Labels[1]->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM3_PO", c_ind).toInt();
	str = m_settings->value("VM3_Lab", l_str).toString();
	VM_Combos[2]->setCurrentIndex(index);
	VM_Labels[2]->setText(str);
	VM_Labels[2]->setAlignment(Qt::AlignCenter);
	index = m_settings->value("VM4_PO", c_ind).toInt();
	str = m_settings->value("VM4_Lab", l_str).toString();
	VM_Combos[3]->setCurrentIndex(index);
	VM_Labels[3]->setText(str);
	VM_Labels[3]->setAlignment(Qt::AlignCenter);
	m_settings->endGroup();
}

void CBK_MainWindow::closeEvent(QCloseEvent* event)
{
	if (QMessageBox::question(this, tr("Подтвердите"), tr("Завершить работу?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
	{
		write_settings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void CBK_MainWindow::set_tm_state()
{
	mku_slot_thr.get_mku_bus_obj()->set_tm("CBK_TM", (uint)(pitanie.to_ulong()));
}

void CBK_MainWindow::new_ku(int ku_n, int length, double u, int line)
{
	if (pitanie[0] == 0)
		return;
	if (u < 20)
		return;

	if (ku_n < 4)
		set_VM_ON(ku_n);
	else
		set_VM_OFF(ku_n-4);
}
	



//void CBK_MainWindow::slot_vm_is_on(int n_vm)
//{
//	if (w_state.VM == -1)
//	{
//		w_state.VM = n_vm;
//		w_state.PO = VMS.VMPOState[n_vm];
//		switch (VMS.VMPOState[n_vm])
//		{
//		case(SPOBU):
//			run_PO(SPOBU);
//			break;
//		case(TPO):
//			run_PO(TPO);
//			break;
//		case(VACANT):
//			break;
//		}
//		
//		return;
//	}
//	if ((w_state.PO == VMS.VMPOState[n_vm])&&(w_state.VM != n_vm))
//	{
//		//VMS.VMPOState[n_vm] = VACANT;
//		//VM_Combos[n_vm]->setCurrentIndex(VACANT);
//		//QMessageBox::information(0, QString("VM -%1 is ON").arg(n_vm), "PO is changed to VACANT");
//		//return;
//	}
//	if (((w_state.PO == SPOBU) && (VMS.VMPOState[n_vm] == TPO))&&(w_state.VM != n_vm))
//	{
//		VMS.VMPowerState[n_vm] = OFF;
//		VM_Labels[n_vm]->setStyleSheet("QLabel { background-color : grey; color : black; }");
//		return;
//	}
//	if (((w_state.PO == TPO) && (VMS.VMPOState[n_vm] == SPOBU)) && (w_state.VM != n_vm))
//	{
//		VMS.VMPowerState[w_state.VM] = OFF;
//		VM_Labels[w_state.VM]->setStyleSheet("QLabel { background-color : grey; color : black; }");
//		shutdown_PO(TPO);
//		w_state.VM = n_vm;
//		w_state.PO = VMS.VMPOState[n_vm];
//		run_PO(SPOBU);
//	}
//
//}
//
//void CBK_MainWindow::slot_vm_is_off(int n_vm)
//{
//	bool need_shutdown = TRUE;
//	if (w_state.VM == n_vm)
//	{
//		for (int i = 0; i < 4; i++)
//		{
//			if ((VMS.VMPowerState[i] == ON) && (i != (n_vm)) && (VMS.VMPOState[i] == VACANT))
//			{
//				w_state.VM = i;
//				w_state.PO = VMS.VMPOState[i];
//				VM_Combos[i]->setCurrentIndex(w_state.PO);
//				need_shutdown = FALSE;
//				break;
//			}
//		}
//		if (need_shutdown)
//		{
//			shutdown_PO(w_state.PO);
//			w_state.VM = -1;
//			w_state.PO = -1;
//		}
//		else
//		{
//			return;
//		}	
//	}
//	else
//	{
//		return;
//	}
//}

void CBK_MainWindow::opo_loaded(int n_vm)
{
	VMS.running[n_vm] = true;
	STimeThread::Instance().cbk_conf.vm += (1 << n_vm);
	if (STimeThread::Instance().cbk_conf.of & (1 << n_vm))
		STimeThread::Instance().cbk_conf.of ^= (1 << n_vm);
	if (STimeThread::Instance().cbk_conf.er & (1 << n_vm))
		STimeThread::Instance().cbk_conf.er ^= (1 << n_vm);

	bool need_po_start = true;
	bool need_shutdown = true;
	for (int i = 0; i < 4; i++)
	{
		if ((n_vm != i) && (VMS.VMPowerState[i] == ON) && (VMS.running[i]))
		{
			if (qAbs(VMS.StartTime[n_vm] - VMS.StartTime[i]) > 1000)
			{
				set_VM_OFF(i);
			}
			else
				need_po_start = false;
		}

		need_shutdown &= VMS.running[i];
	}


	if (need_shutdown)
	{
		set_VM_OFF(3);
		STimeThread::Instance().cbk_conf.rs += (1 << 3);
		if (STimeThread::Instance().cbk_conf.er & (1 << 3))
			STimeThread::Instance().cbk_conf.er ^= (1 << 3);
	}


	if (need_po_start)
	{
		switch (VMS.VMPOState[n_vm])
		{
		case(SPOBU):
			run_PO(SPOBU);
			break;
		case(TPO):
			run_PO(TPO);
			break;
		case(VACANT):
			break;
		};
		w_state.PO = VMS.VMPOState[n_vm];
		STimeThread::Instance().cbk_conf.m0 = n_vm + 1;
		STimeThread::Instance().cbk_conf.m1 = n_vm + 1;
		STimeThread::Instance().cbk_conf.dv = n_vm + 1;

	}

}


void CBK_MainWindow::on_timeout()
{
	QTimer* tmp_timer = static_cast<QTimer*>(sender());
	tmp_timer->stop();
	opo_loaded(on_timers[tmp_timer]);
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
		tpo_thread.terminate();
		tpo_thread.wait(5000);
		STimeThread::Instance().quit();
		STimeThread::Instance().wait(5000);
		break;
	}
	w_state.PO = -1;
}

void CBK_MainWindow::show_time()
{
	int tmp_time = 0;
	tmp_time = STimeThread::Instance().getCurTime();
	edit->append(QString("Current time: %1").arg(tmp_time));
}


void CBK_MainWindow::get_power(double _volt)
{
	if (_volt >= 23.0)
	{
		pitanie[0] = true;
		STimeThread::Instance().cbk_conf.vm = 0;
		STimeThread::Instance().cbk_conf.rs = 0;
		STimeThread::Instance().cbk_conf.of = 0xF;
		STimeThread::Instance().cbk_conf.er = 0xF;

		set_tm_state();
		for (int i = 0; i < 4; i++)
		{
			if (VMS.VMPOState[i] == TPO)
				set_VM_ON(i);
		}

	}
	else
	{
		pitanie[0] = false;
		set_VM_OFF(0);
		set_VM_OFF(1);
		set_VM_OFF(2);
		set_VM_OFF(3);
	}
}