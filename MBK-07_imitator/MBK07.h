#ifndef MBK07_H
#define MBK07_H

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
#include <qplaintextedit.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"
#include "../buses_imitator/frame_bus_rpc.h"

enum FSMU_numbB
{
	FSMU_One = 0,
	FSMU_Two = 1,
	FSMU_Three = 2,
	FSMU_OFF = 3
};
enum FSVU_numbB
{
	FSVU_One = 0,
	FSVU_Two = 1,
	FSVU_Three = 2,
	FSVU_OFF = 3
};
enum STAB
{
	LOW_STAB = 0,
	KG1_STAB = 1,
	KG2_STAB = 2,
	HIGH_STAB = 3,
	OFF_STAB = 4

};
enum ANTENNA
{
	OHA = 0,
	MHAPY = 1,
	MHAMY = 2,
	ANT_OFF = 3
};
enum full_mode
{
	ERR = 0,
	PI15 = 1,
	PI8 = 2,
	WTF8 = 3	
};
enum PSP
{
	PSP_OFF = 0,
	PSP1 = 1,
	PSP2 = 2,
	PSP3 = 3,
	PSP4 = 4
};

enum LITERA
{
	LIT0 = 0,
	LIT1 = 1,
	LIT2 = 2,
	LIT3 = 3,
	LIT4 = 4,
	LIT5 = 5,
	LIT6 = 6,
	LIT7 = 7,
	LIT8 = 8
};

class MBK07_widg : public QWidget
{
    Q_OBJECT

public:
	MBK07_widg(QWidget *parent = 0);
	~MBK07_widg();
	

private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	int current_lit = 0;
	full_mode current_mode = ERR;
	PSP current_PSP = PSP_OFF;
	FSMU_numbB current_FSMU = FSMU_OFF;
	FSVU_numbB current_FSVU = FSVU_OFF;
	STAB current_stab = OFF_STAB;
	ANTENNA current_antenna = ANT_OFF;

	bool IM = false;
	bool pi8_fast = false;

	QList<QPushButton*> FSMU_blocks;
	QList<QPushButton*> FSVU_canals;

	QPlainTextEdit* Logs;

	QGroupBox *FSMU_gb;
	QGroupBox *FSVU_gb;
	const int MKO = 1;
	const int adr = 7;
	QVariantList words;
	QGridLayout *SubGrid_glayout;
	QVBoxLayout *FSMUFSVU_vblayout;
	QHBoxLayout *FSVU_hblayout;
	QHBoxLayout *FSMU_hblayout;
	QHBoxLayout *All_vblayout;
	QPushButton * okBut;
	QDialog *dlg;
	bool flag_on;

	QList<QCheckBox*> set_list;
	QList<QCheckBox*> set_list_mu2;
	QList<QLineEdit*> sub_le_list;

	void set_new_tm();
	void imit_on();
	void imit_off();
	void update_graphics();
	void set_power_back();
	void change_power();
protected:
	
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
	void get_power(double volt);
	void get_frame(QString mode, QVariant frame_data);
protected:
	void closeEvent(QCloseEvent *event);
signals:
	void push_frame();
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
	bool ab_state = true;
	QString name = "МБК-07";
	int bus = 1;
	int power = 0;
	int fsmu_v = 0x033B;
	int fsvu_v = 0x0292;
	int volt;

	QMap<int, QString> mode_names;
	QMap<int, LITERA> lit_map;
	QMap<int, QString> stab_names;
	QMap<int, QString> ant_names;
	
	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	RPC_frame_bus_SLOT_Thread frame_slot_thr;
	RPC_frame_bus_SIGNAL_Thread frame_signal_thr;
};

#endif // MBK07_H

