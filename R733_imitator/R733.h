#ifndef R733_H
#define R733_H

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include "R733Modules.h"
#include "LKAModules.h"
#include "instruments.h"

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"
//#include "../MBK-02_imitator/MBK02_rpc.h"
#include "../mbk04_imitator/mbk04_rpc.h"
#include "../buses_imitator/frame_bus_rpc.h"

union TM_733
{
	quint16 tm_data;
	struct
	{
		quint16 PP : 1,
			GO : 1,
			VP_O : 1,
			VP_R : 1,
			GR : 1,
			res : 11;
	};
};

class R733_widg : public LKA06_MODULE
{
    Q_OBJECT
public:
	R733_widg();
	~R733_widg() {}
private:
	TM_733 tm_data;

	QPushButton *MU1;
	QPushButton *MU2;
	QPushButton *main_MPVN;
	QPushButton *reserve_MPVN;
	QPushButton* main_MVKU;
	QPushButton* reserve_MVKU;
	QPushButton* regime_mod_upi;
	QCheckBox* auto_scroll_box;

	QList<QPushButton*>  VCH_list;
	QList<QPushButton*>  UPI_list;
	QGroupBox *MVKU_gb;
	QGroupBox *MPVN_gb;
	QGroupBox *VCH_gb;
	QGroupBox *UPI_gb;
	QGroupBox *MU_gb;
	QGroupBox *REGIME_MOD_UPI;
	QGridLayout *block_glayout;

	REGIME regime_upi;

	UPI_MODULE upi_module;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;

	bool flag_on;
	int num_ku;
	
	int rrr;
	int rpk;
	int rkm;
	int rbk;

	QTextDocument* _doc;
	QTextEdit* edit_info;
	QStringList log_buffer;
	QMutex log_mutex;
	QTextCursor* _cursor;
	QScrollBar* _scroll_bar;
	bool auto_scroll;

	void paint_buttons();
	void new_data_mv();
	void set_new_tm();

	void set_rsk();
	void set_rrr_rsh();

	void msg_to_log(const QString& _msg);

	QMap<int, QString> mode_names;
	QMap<int, QString> major_names;
	QMap<int, QString> channel_names;

	QList<int> channels_upi;
	
	QString name = "14P733";
	int bus = 3;
	int power = 0;
	double volt;
	void imit_off();
	void imit_on();
	void change_power();
	void set_power_back();

	QList<int> upi_state_channels;
	QList<int> rkm_channels;
	QList<int> rbk_channels;

	QString regime;
	QString msg;

	void set_tm_state();
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void auto_scroll_clicked(int _state);
	void get_power(double volt);
	void new_frame_04(QString mode, QVariant frame_data);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);

protected:
	void closeEvent(QCloseEvent *event);
signals:
	void new_ku(int ku_n, int length, double u, int line);
};

#endif // R733_H

