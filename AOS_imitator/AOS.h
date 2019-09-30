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

enum KP
{
	KP_1 = 0,
	KP_2 = 1,
	KP_OFF = 3
};

class AOS_widg : public QWidget
{
    Q_OBJECT

public:
	AOS_widg(QWidget *parent = 0);
	~AOS_widg();
	
private:
	KP current_KP = KP_OFF;


	const int MKO = 1;
	const int adr = 21;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов
	void omni_connect();//почключение к omnibus спустя n-ое количество времени
	void update_time();//Расчет времени прогрева ОГ в зависимости от времени включения и окончания прогрева
	void set_warm_og();//сохранение окончания прогревания и обьявление о прогретости/непрогретости
	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku(int ku, int length_ku, double u_ku, int line_ku);

private:
	QString name = "АОС";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // AOS_H

