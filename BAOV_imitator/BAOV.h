#ifndef BAOV_H
#define BAOV_H

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
#include "../buses_imitator/interrupt_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "sensor.h"
#include "functional_control_mapping_item.h"

using namespace std;

enum LKA
{
	LKA_1 = 0,
	LKA_2 = 1,
	LKA_OFF = 3
};
enum OG
{
	OG_1 = 0,
	OG_2 = 1,
	OG_3 = 2,
	OG_ERR = 3
};
enum FINIK
{
	FINIK_1 = 0,
	FINIK_2 = 1,
	FINIK_3 = 2,
	FINIK_ERR = 3
};
enum FINIK_REZH
{
	FINIK_REZH_PI15 = 0,
	FINIK_REZH_PI8 = 1,
	FINIK_REZH_ERR = 2
};
enum KP
{
	KP_1 = 0,
	KP_2 = 1,
	KP_OFF = 3
};

// BAO
enum RC
{
	RC_1 = 0,
	RC_2 = 1,
	RC_3 = 2,
	RC_4 = 3,
	RC_OFF = 4
};
enum UCO
{
	UCO_1 = 1,
	UCO_2 = 2,
	UCO_OFF = 3
};

union ResponseWord {
	quint16 com_word;
	struct {
		quint16	device_error : 1,
			interface_managemenent_accepted: 1,
			subscriber_error : 1,
			subscriber_busy : 1,
			group_command_accepted : 1,
			reserve : 3,
			service_request : 1,
			response_word_transmission : 1,
			message_error : 1,
			device_address : 5;
	};
};

union FirstOKWord {
	quint16 com_word;
	struct {
		quint16 sis_fk_ready : 1,
			time_request : 1,
			kv_ku_cbk_ok : 1,
			kv_ku_mkp_ok : 1,
			incorrect_time_code : 1,
			incorrect_command_control_code : 1,
			no_sm3_signal : 1,
			no_program : 1,
			uco_1_ok : 1,
			uco_2_ok : 1,
			cm3_period_not_ok : 1,
			test_mode_ok : 1,
			temperature_mn_uco_ok : 1,
			temperature_pl_uco_ok : 1,
			start_bo_ok : 1,
			bao_v_ok : 1;
	};
};
union SecondOKWord {
	quint16 com_word;
	struct {
		quint16 brgi_1_1_ok : 1,
			brgi_1_2_ok : 1,
			brgi_2_1_ok : 1,
			brgi_2_2_ok : 1,
			brgi_3_1_ok : 1,
			brgi_3_2_ok : 1,
			brgi_4_1_ok : 1,
			brgi_4_2_ok : 1,
			dri_1_1_ok : 1,
			dri_1_2_ok : 1,
			dni_drs_1_ok : 1,
			seconds_counter_4 : 1,
			dri_2_1_ok : 1,
			dri_2_2_ok : 2,
			dni_drs_2_ok : 1,
			seconds_counter_5_ok : 1;
	};
};

union ThirdOKWord {
	quint16 com_word;
	struct {
		quint16 temperature_mn_brgi_1_ok : 1,
			temperature_pl_brgi_1_ok : 1,
			temperature_mn_brgi_2_ok : 1,
			temperature_pl_brgi_2_ok : 1,
			temperature_mn_brgi_3_ok : 1,
			temperature_pl_brgi_3_ok : 1,
			temperature_mn_brgi_4_ok : 1,
			temperature_pl_brgi_4_ok : 1,
			temperature_mn_bo_1_ok : 1,
			temperature_pl_bo_1_ok : 1,
			temperature_mn_bo_2_ok : 1,
			temperature_pl_bo_2_ok : 1,
			seconds_counter_0 : 1,
			seconds_counter_1 : 1,
			seconds_counter_2 : 1,
			seconds_counter_3 : 1;
	};
};

class BAOV_widg : public QWidget
{
    Q_OBJECT

public:
	BAOV_widg(QWidget *parent = 0);
	~BAOV_widg();
	
private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	OG current_OG = OG_ERR;
	FINIK current_FINIK = FINIK_ERR;
	FINIK_REZH current_FINIK_REZH = FINIK_REZH_ERR;
	KP current_KP = KP_OFF;
	LKA current_LKA = LKA_OFF;

	const int DEFAULT_DETECTION_THRESHOLD = 5;

	bool sensors_enabled = false;
	RC curr_RC = RC_OFF;
	UCO curr_UCO = UCO_OFF;
	int sis_fk_counter = 0;
	int sis_counter = 0;
	std::map<int, sensor> addressToSensorMap;
	std::map<int, sensor> addressToEnabledSensorMap;
	std::map<int, functional_control_mapping_item> addressToFKWordMap;
	unsigned short curr_first_time_word;
	unsigned short curr_second_time_word;
	ResponseWord responseWord;
	FirstOKWord firstOKWord;
	SecondOKWord secondOKWord;
	ThirdOKWord thirdOKWord;

	QList<QPushButton*> rc_pbut;
	QList<QPushButton*> uco_pbut;
	QList<QPushButton*> active_sensors_info_pbut;
	QPushButton* sis_btn;

	QVBoxLayout *All_vblay;
	QHBoxLayout *rc_hlay;
	QHBoxLayout *uco_hlay;
	QHBoxLayout *sis_btn_hlay;

	QGroupBox *rc_gb;
	QGroupBox *uco_gb;
	QGroupBox *active_sensors_info_gb;
	QGridLayout *active_sensors_info_grid;
	QGroupBox *sis_btn_gb;

	QPlainTextEdit* Logs;
	QLineEdit* Mode_le;
	const int MKO = 1;
	const int adr = 10;
	QPushButton * okBut;
	QDialog *dlg;
	bool ready_og;
	bool flag_on;

	void update_graphics();//обновление графики
	void omni_connect();//почключение к omnibus спустя n-ое количество времени
	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
	void change_power(bool switch_og);//изменение мощности
	void set_change_power();//передает в change_power параметр false, означающий, что функция вызвана таймером
	void set_power_back();//возврат силы тока в зависимости от мощности
protected:
	void closeEvent(QCloseEvent *event);
	bool checkIfActiveSensorsMapDoenstContainsBothSensors(int currAddr, int firstActiveAddr, int secondActiveAddr);
	void parseInitSensorsData();
	void doFunctionalControl();
	void parseMappingForFunctionalControl();
	void turnOnSensors();
	void turnOffSensors(bool isNextActionTurningOffBAOV);
	void sendOperationalControl();
	sensor getSensorForFK(int address, bool isEnabled);
	void setKU_CBK_OK();
	void setKU_MKP_OK();
	void updateSensorStatusInOK(sensor sensor, int value);
	void setDetectionThreshold(QVariantList words);
	void setActiveSensors(QVariantList words);
	void addSensorToActive(int addr, bool isActive);
	void printTime();
	void generateSIS(bool isImit);
	void generateSIS(bool isImit, bool isSISNII);
public slots:
	void generateSIS();
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void BAOV_interrupt_setup();
	void BAOV_interrupt_run();
	void get_power(double volt);
private:
	void msg_to_log(const QString& _msg);

	QTextEdit* edit;
	QScrollBar* _scroll_bar;
	QTextDocument* _doc;
	QTextCursor* _cursor;
	QString log_filename;
	QCheckBox* auto_scroll_box;
	bool auto_scroll;
	QTimer log_timer;
	QStringList log_buffer;
	QMutex log_mutex;
	QTimer *inter_tmr;
	QTimer *warm_og_tmr;
	QTimer *AbOn_tmr;
	QTimer *Power_tmr;
	QSettings LKA_sett;

	QMap<int, QString> mode_names;
	QMap<OG, qint64> OG_start_warm;//Мап старта прогрева
	QMap<OG, qint64> OG_finish_warm;//Мап окончания прогрева

	int standart_tm = 120000;//стандартное время прогрева
	int tm_towarm;//текущее время прогрева, с растчетом прогретости
	int cooling_cof = 4;//коэфициент охлаждения (во сколько раз ОГ быстрее нагревается чем охлаждается)
	int power_vt = 0;//мощность
	int warm_er = 500;//погрешность нагрева
	int n;
	short chan;
	double u;
	double t;
	QString name = "БЭЧ";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // BAOV_H

