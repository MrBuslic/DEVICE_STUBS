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

union BLKIIH_Word		//БЛК, внутренний интерфейс и нагреватели subadr 1
{
	quint16 ok_tm_word[9];
	struct
	{
		quint16					  // word 20 
			s_main_blk_on : 1,    // Включен основной БЛК, счёт идёт от 4-го бита
			s_res_blk_on : 1,     // Включен резервный БЛК 
			s_main_internal_interface_work : 1,	//Работает основной внутренний интерфейс
			: 2,			//Резерв
			s_internal_interface_OK : 1;	//Исправен внутренний интерфейс
		quint16					  // word 21 
			: 5,			//Резерв
			s_11_heater_work : 1, //Включен нагреватель 11
			s_10_heater_work : 1; //Включен нагреватель 10
		quint16					  // word 22
			: 7,	//Резерв
			s_9_heater_work : 1, //Включен нагреватель 9
			: 3,	//Резерв
			s_17_heater_work : 1; //Включен нагреватель 17
		quint16					  // word 23
			: 8,	//Резерв
			s_25_heater_work : 1, //Включен нагреватель 25
			s_24_heater_work : 1, //Включен нагреватель 24
			s_23_heater_work : 1, //Включен нагреватель 23
			s_22_heater_work : 1, //Включен нагреватель 22
			s_21_heater_work : 1, //Включен нагреватель 21
			s_20_heater_work : 1, //Включен нагреватель 20
			s_19_heater_work : 1, //Включен нагреватель 19
			s_18_heater_work : 1; //Включен нагреватель 18
		quint16					  // word 24
			: 11,	//Резерв
			s_34_heater_work : 1, //Включен нагреватель 34
			s_33_heater_work : 1; //Включен нагреватель 33
		quint16					  // word 25
			s_8_res_pyro_worked : 1, //Сработал восьмой пиропатрон, резервная нить
			s_8_main_pyro_worked : 1, //Сработал восьмой пиропатрон, основная нить
			s_7_res_pyro_worked : 1, //Сработал седьмой пиропатрон, резервная нить
			s_7_main_pyro_worked : 1, //Сработал седьмой пиропатрон, основная нить
			s_6_res_pyro_worked : 1, //Сработал шестой пиропатрон, резервная нить
			s_6_main_pyro_worked : 1, //Сработал шестой пиропатрон, основная нить
			s_5_res_pyro_worked : 1, //Сработал пятый пиропатрон, резервная нить
			s_5_main_pyro_worked : 1, //Сработал пятый пиропатрон, основная нить
			s_4_res_pyro_worked : 1, //Сработал четвёртый пиропатрон, резервная нить
			s_4_main_pyro_worked : 1, //Сработал четвёртый пиропатрон, основная нить
			s_3_res_pyro_worked : 1, //Сработал третий пиропатрон, резервная нить
			s_3_main_pyro_worked : 1, //Сработал третий пиропатрон, основная нить
			s_2_res_pyro_worked : 1, //Сработал второй пиропатрон, резервная нить
			s_2_main_pyro_worked : 1, //Сработал второй пиропатрон, основная нить
			s_1_res_pyro_worked : 1, //Сработал первый пиропатрон, резервная нить
			s_1_main_pyro_worked : 1; //Сработал первый пиропатрон, основная нить
		quint16					  // word 26
			: 1,	//Резерв
			s_main_pyro_power : 1, //Есть основное питание 27 В пиропатронов
			: 1,	//Резерв
			s_res_pyro_power : 1, //Есть резервное питание 27 В пиропатронов
			s_14_res_pyro_worked : 1, //Сработал четырнадцатый пиропатрон, резервная нить
			s_14_main_pyro_worked : 1, //Сработал четырнадцатый пиропатрон, основная нить
			s_13_res_pyro_worked : 1, //Сработал тринадцатый пиропатрон, резервная нить
			s_13_main_pyro_worked : 1, //Сработал тринадцатый пиропатрон, основная нить
			s_12_res_pyro_worked : 1, //Сработал двенадцатый пиропатрон, резервная нить
			s_12_main_pyro_worked : 1, //Сработал двенадцатый пиропатрон, основная нить
			s_11_res_pyro_worked : 1, //Сработал одиннадцатый пиропатрон, резервная нить
			s_11_main_pyro_worked : 1, //Сработал одиннадцатый пиропатрон, основная нить
			s_10_res_pyro_worked : 1, //Сработал десятый пиропатрон, резервная нить
			s_10_main_pyro_worked : 1, //Сработал десятый пиропатрон, основная нить
			s_9_res_pyro_worked : 1, //Сработал девятый пиропатрон, резервная нить
			s_9_main_pyro_worked : 1; //Сработал девятый пиропатрон, основная нить
		quint16					  // word 27
			s_8_res_pyro_worked_repeat : 1, //Сработал восьмой пиропатрон, резервная нить (повтор)
			s_8_main_pyro_worked_repeat : 1, //Сработал восьмой пиропатрон, основная нить (повтор)
			s_7_res_pyro_worked_repeat : 1, //Сработал седьмой пиропатрон, резервная нить (повтор)
			s_7_main_pyro_worked_repeat : 1, //Сработал седьмой пиропатрон, основная нить (повтор)
			s_6_res_pyro_worked_repeat : 1, //Сработал шестой пиропатрон, резервная нить (повтор)
			s_6_main_pyro_worked_repeat : 1, //Сработал шестой пиропатрон, основная нить (повтор)
			s_5_res_pyro_worked_repeat : 1, //Сработал пятый пиропатрон, резервная нить (повтор)
			s_5_main_pyro_worked_repeat : 1, //Сработал пятый пиропатрон, основная нить (повтор)
			s_4_res_pyro_worked_repeat : 1, //Сработал четвёртый пиропатрон, резервная нить (повтор)
			s_4_main_pyro_worked_repeat : 1, //Сработал четвёртый пиропатрон, основная нить (повтор)
			s_3_res_pyro_worked_repeat : 1, //Сработал третий пиропатрон, резервная нить (повтор)
			s_3_main_pyro_worked_repeat : 1, //Сработал третий пиропатрон, основная нить (повтор)
			s_2_res_pyro_worked_repeat : 1, //Сработал второй пиропатрон, резервная нить (повтор)
			s_2_main_pyro_worked_repeat : 1, //Сработал второй пиропатрон, основная нить (повтор)
			s_1_res_pyro_worked_repeat : 1, //Сработал первый пиропатрон, резервная нить (повтор)
			s_1_main_pyro_worked_repeat : 1; //Сработал первый пиропатрон, основная нить (повтор)
		quint16					  // word 28
			: 4,	//Резерв
			s_14_res_pyro_worked_repeat : 1, //Сработал четырнадцатый пиропатрон, резервная нить (повтор)
			s_14_main_pyro_worked_repeat : 1, //Сработал четырнадцатый пиропатрон, основная нить (повтор)
			s_13_res_pyro_worked_repeat : 1, //Сработал тринадцатый пиропатрон, резервная нить (повтор)
			s_13_main_pyro_worked_repeat : 1, //Сработал тринадцатый пиропатрон, основная нить (повтор)
			s_12_res_pyro_worked_repeat : 1, //Сработал двенадцатый пиропатрон, резервная нить (повтор)
			s_12_main_pyro_worked_repeat : 1, //Сработал двенадцатый пиропатрон, основная нить (повтор)
			s_11_res_pyro_worked_repeat : 1, //Сработал одиннадцатый пиропатрон, резервная нить (повтор)
			s_11_main_pyro_worked_repeat : 1, //Сработал одиннадцатый пиропатрон, основная нить (повтор)
			s_10_res_pyro_worked_repeat : 1, //Сработал десятый пиропатрон, резервная нить (повтор)
			s_10_main_pyro_worked_repeat : 1, //Сработал десятый пиропатрон, основная нить (повтор)
			s_9_res_pyro_worked_repeat : 1, //Сработал девятый пиропатрон, резервная нить (повтор)
			s_9_main_pyro_worked_repeat : 1; //Сработал девятый пиропатрон, основная нить (повтор)
	};
};
/*
union AST_Word			//Температуры аналоговых датчиков subadr 18
{
	quint16 ok_tm_word[7];
	struct
	{
		quint16					  // word 21 
			: 4,			//Резерв
			s_1_heater_work : 1, //Включен нагреватель 1
			s_2_heater_work : 1, //Включен нагреватель 2
			s_3_heater_work: 1, //Включен нагреватель 3
			s_4_heater_work : 1, //Включен нагреватель 4
			s_5_heater_work: 1, //Включен нагреватель 5
			s_7_heater_work : 1, //Включен нагреватель 7
			s_8_heater_work : 1, //Включен нагреватель 8
			s_39_heater_work : 1, //Включен нагреватель 39
			s_40_heater_work : 1; //Включен нагреватель 40
		quint16					  // word 22
			: 7,	//Резерв
			s_9_heater_work : 1, //Включен нагреватель 11
			: 3,	//Резерв
			s_17_heater_work : 1; //Включен нагреватель 10
		quint16					  // word 23
			: 8,	//Резерв
			s_25_heater_work : 1, //Включен нагреватель 25
			s_24_heater_work : 1, //Включен нагреватель 24
			s_23_heater_work : 1, //Включен нагреватель 23
			s_22_heater_work : 1, //Включен нагреватель 22
			s_21_heater_work : 1, //Включен нагреватель 21
			s_20_heater_work : 1, //Включен нагреватель 20
			s_19_heater_work : 1, //Включен нагреватель 19
			s_18_heater_work : 1; //Включен нагреватель 18
		quint16					  // word 24
			: 11,	//Резерв
			s_33_heater_work : 1, //Включен нагреватель 33
			s_34_heater_work : 1; //Включен нагреватель 34
	};
};
*/
class BKIS_widg : public QWidget
{
    Q_OBJECT
	
public:
	BKIS_widg(QWidget *parent = 0);
	~BKIS_widg();
	
private:
	const int MKO = 0;//проверить номер шины
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
	void set_pyro_bus_state(int bus_num, bool state);
signals:
	void send_pyro_group_activation(int group_num);
private:
	QString name = "BKIS";//Имя устройства
	int bus  = 1;//номер шины для шины питания(power_bus): НК
	int volt;//Принятое напряжение
	
	BLKIIH_Word BLKIIH_word; //подумать над другим названием!!!
	int blk_state;
	bool interface_state;
	QMap<int, int> pyro_buses_state;
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

