#ifndef BKIS_H
#define BKIS_H

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

class BKIS_widg : public QWidget
{
    Q_OBJECT
	
public:
	BKIS_widg(QWidget *parent = 0);
	~BKIS_widg();
	
private:
	const int MKO = 1;//проверить номер шины
	const int adr = 22;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов

	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void make_ku(int ku_n, int length, double u, int line);
	void get_power(double _volt);
	void set_blk_state(int state);
	void set_interface_state(bool state);
	int set_electric_heater_state(int name, bool state);
	int set_pyro_group_state(int group_num, bool state);
	void set_power_back();
	void omni_connect();
	void set_pyro_bus_state(int bus_num);
signals:
	void send_pyro_group_activation(int group_num);
private:
	QString name = "BKIS";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	int blk_state;
	bool interface_state;
	int pyro_buses_state;
	QMap<int, int> pyro_groups_states;
	QMap<int, int> electric_heaters_states;
	
	QPushButton* main_blk;
	QPushButton* reserve_blk;
	QPushButton* main_interface;
	QPushButton* reserve_interface;
	
	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // BKIS_H

