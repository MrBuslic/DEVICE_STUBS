#include "AOS.h"

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

AOS_widg::AOS_widg(QWidget *parent)
{
	setWindowTitle("АОС");

	slot_thr.set_connection_params("127.0.0.1", OMNIBUS_SLOT);
	slot_thr.start(); 

	signal_thr.set_connection_params("127.0.0.1", OMNIBUS_SIGNAL);
	signal_thr.start();

	if (!slot_thr.wait_connected(3) || !signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с rpc_omnibus");
		this->deleteLater();
		return;
	}

	mku_slot_thr.set_connection_params("127.0.0.1", MKU_SLOT);
	mku_slot_thr.start();

	mku_signal_thr.set_connection_params("127.0.0.1", MKU_SIGNAL);
	mku_signal_thr.start();

	if (!mku_slot_thr.wait_connected(3) || !mku_signal_thr.wait_connected(3))
	{
		QMessageBox::critical(0, "Нет соединения", "Ошибка соединения с mku");
		this->deleteLater();
		return;
	}

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

	connect(signal_thr.get_obj().get(), SIGNAL(new_message(QVariant, int, int, int, QVariantList, int)), this, SLOT(new_message(QVariant, int, int, int, QVariantList, int)));

	slot_thr.get_omnibus_obj()->switch_ab(MKO, adr, false);

	connect(mku_signal_thr.get_obj().get(), SIGNAL(new_ku_mk(int, int, double, int)), this, SLOT(new_ku_mk(int, int, double, int)));
	connect(power_signal_thr.get_obj().get(), SIGNAL(u_on_k1(double)), this, SLOT(get_power(double)));

	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	restoreGeometry(settings.value("aos_geometry").toByteArray());

	kontr_dev = AOS_KP_OFF;
	cgo_dev = AOS_KP_OFF;
	gpfm2_dev = AOS_KP_OFF;
	gpfm1_dev = AOS_KP_OFF;

}


void AOS_widg::imit_off()
{

}

void AOS_widg::imit_on()
{

}

void AOS_widg::new_ku_mk(int ku, int length_ku, double u_ku, int line_ku)
{

}


void AOS_widg::new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os)
{
	MKOWord tmp_cwd;
	tmp_cwd.com_word = cwd;
	if (os == -1)
		return;
	if ((mko == MKO) && (tmp_cwd.adr == adr) && (tmp_cwd.trans_dir == 0))
	{
	
		update_graphics();
		set_new_tm();
	}
}


void AOS_widg::update_graphics()
{

}

void AOS_widg::set_new_tm()
{
	unsigned short _word = 0;
	QVariantList tmp_list;

	tmp_list.push_back(_word);
	slot_thr.get_omnibus_obj()->set_new_data(MKO, adr, 1, tmp_list);
}

AOS_widg::~AOS_widg()
{
	slot_thr.quit();
	signal_thr.quit();

	mku_slot_thr.quit();
	mku_signal_thr.quit();

	power_slot_thr.quit();
	power_signal_thr.quit();
}

void AOS_widg::closeEvent(QCloseEvent *event)
{
	QSettings settings(QApplication::applicationDirPath() + "/positions.ini", QSettings::IniFormat);
	settings.setValue("aos_geometry", saveGeometry());
	QWidget::closeEvent(event);
}