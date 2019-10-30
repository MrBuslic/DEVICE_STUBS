#ifndef BOOP_H
#define BOOP_H

#include <qwidget.h>
#include <qmainwindow.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qgroupbox.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"

class Angle
{
public:
	Angle() : cur_pos(0), null_pos(0), plus_stop(0), minus_stop(0), saw_n(0) {}
	void calc_angle(int steps = 0);
	int cur_pos;
	int null_pos;
	int plus_stop;
	int minus_stop;
	double _angle;
	QString angle_string;
	int saw_n;
	static const double ShD_bit;
	static const double DU_bit;
};

union BOOPDataWords
{
	BOOPDataWords()
	{
		// Form data words
	}

	quint16 data_words[13];

	struct {
		quint16 nu_servo_power_status : 1,
			phi_servo_power_status : 1,
			nu_angle_sensor_power_status : 1,
			phi_angle_sensor_power_status : 1,
			nu_channel_work_status : 1,
			phi_channel_work_status : 1,
			previous_message_error : 1,
			half_set_engage : 1,
			temperature : 8;

		quint16 nu_pulse_amount : 15,
			nu_rotation_direction : 1;

		quint16 phi_pulse_amount : 15,
			phi_rotation_direction : 1;

		quint16 nu_pulse_frequency : 9,
			: 7;

		quint16 phi_pulse_frequency : 9,
			: 7;

		quint16 nu_angle;

		quint16 phi_angle;

		quint16 nu_pulse_summ : 15,
			nu_summ_sign : 1;

		quint16 phi_pulse_summ : 15,
			phi_summ_sign : 1;

		// Self-control bits
		quint16 bit19 : 1,
			bit18 : 1,
			bit17 : 1,
			bit16 : 1,
			bit15 : 1,
			bit14 : 1,
			bit13 : 1,
			bit12 : 1,
			: 7,
			bit4 : 1;

		quint16 nu_min_angle;

		quint16 phi_min_angle;

		quint16 checksum;
	};
};

class BOOP : public QWidget
{
	Q_OBJECT
public:
	BOOP();
	~BOOP();
	void new_data(int mko, int address, int subaddress, QVariantList words);

private:
	// Main Window
	QMainWindow* mainWindow;

	// Main widget
	QWidget* widget;

	// Temperature indicator
	QLineEdit *temperatureEdit;

	// Hals-set buttons
	QPushButton *mainSetButton;
	QPushButton *reserveSetButton;

	// Self-control checkboxes
	QCheckBox *bit19;
	QCheckBox *bit18;
	QCheckBox *bit17;
	QCheckBox *bit16;
	QCheckBox *bit15;
	QCheckBox *bit14;
	QCheckBox *bit13;
	QCheckBox *bit12;
	QCheckBox *bit4;

	// Uplilon channel servo controls
	QLabel *nuAngleValue;
	QLabel *nuAnglePiValue;
	QLabel *nuSawNumber;
	QCheckBox *nuAngleSensorPower;
	QCheckBox *nuAngleServoPower;
	QPushButton *increaseNuAngle;
	QPushButton *decreaseNuAngle;

	// Phi channel servo controls
	QLabel *phiAngleValue;
	QLabel *phiAnglePiValue;
	QLabel *phiSawNumber;
	QCheckBox *phiAngleSensorPower;
	QCheckBox *phiAngleServoPower;
	QPushButton *increasePhiAngle;
	QPushButton *decreasePhiAngle;
	QTimer *timer_for_msg = new QTimer();
	// Log area
	QTextEdit *logArea;
	BOOPDataWords word_for_cbk;
	int nu_sec = 0, phi_sec = 0;
	int nu_angl_amount;
	int phi_angl_amount;
	bool need_to_move;
	int timer_count;
	quint16 nu_rotation_direction, phi_rotation_direction;

	Angle nu_angle;
	Angle phi_angle;

	//QString pos_nu, pos_phi;

protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int MKO, int line, int command_word, QVariantList words, int respond_word);
	void new_matrix_command(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void move_boop();

signals:
	

private:
	void new_tm();
	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	const int MKO = 1;
	const int address = 9;


};

#endif
