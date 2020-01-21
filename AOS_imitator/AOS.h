#ifndef AOS_H
#define AOS_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <qplaintextedit.h>
#include <qsettings.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

enum DEVICE_STATES
{
	AOS_KP_OFF = 0, 
	AOS_KP_1 = 1,
	AOS_KP_2 = 2,
	AOS_OKP_ERR = 3
};

union AOS_modes
{
	quint16 _word;
	struct
	{
		quint16 kontr : 2, //AOS_modes
			cgo : 2, //AOS_modes
			gpfm2 : 2, //AOS_modes
			gpfm1 : 2, //AOS_modes
			sig_96 : 1, //0 - сигнал от УПИ, 1- сигнал от 15Э1827
			dev_on : 1, //1 - включать устр-ва,1 - игнорировать первые биты
			res : 4,
			kontr_mode : 1; // 0 - режим контроля, 1 - рабочий режим

	};
};

class AOS_widg : public QWidget
{
    Q_OBJECT

public:
	AOS_widg(QWidget *parent = 0);
	~AOS_widg();
	
private:
	//AOS_KP current_KP = AOS_OFF;


	const int MKO = 1;
	const int adr = 21;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов

	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku_mk(int ku, int length_ku, double u_ku, int line_ku);

private:
	QString name = "АОС";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	DEVICE_STATES kontr_dev;
	DEVICE_STATES cgo_dev;
	DEVICE_STATES gpfm2_dev;
	DEVICE_STATES gpfm1_dev;
	
	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // AOS_H

