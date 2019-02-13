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
#include "../LKA-05_imitator/lka05_rpc.h"

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
	HIGH_STAB = 1,
	KG1_STAB = 2,
	KG2_STAB = 3
};
enum ANTENNA
{
	OHA = 0,
	MHA1Y = 1,
	MHA0Y = 2
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

/*class MU_MODULE
{
public:
	MU_MODULE();
	virtual unsigned short get_tm();
	void set_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
			return;

		working[_dev] = _flag;
	}
	void set_ab_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
			return;

		ab_working[_dev] = _flag;
	}
	bool get_working()
	{
		return working[current_dev];
	}
	bool get_ab_working()
	{
		return ab_working[current_dev];
	}
	void switch_cur_dev(CURRENT_DEV _dev)
	{
		if (_dev == OFF)
			return;
		current_dev = _dev;
	}
	CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
private:
	QMap<CURRENT_DEV, bool> working;
	QMap<CURRENT_DEV, bool> ab_working;
	CURRENT_DEV current_dev;
};


struct MV_DEV
{
	double u_kom = 27.0;
	int dt_kom = 1;
	int length_kom = 200;
	bool working = true;
};

class MV_MODULE : public LKA05_MODULE
{
public:
	MV_MODULE(int _com, int _nim);
	virtual unsigned short get_tm();
	bool get_working()
	{
		return devices[current_dev].working;
	}
	void set_working(CURRENT_DEV _dev, bool _flag)
	{
		if (_dev == OFF)
			return;

		devices[_dev].working = _flag;
	}
	void switch_cur_dev(CURRENT_DEV _dev)
	{
		current_dev = _dev;
	}
	CURRENT_DEV get_current_dev()
	{
		return current_dev;
	}
	MV_DEV& get_settings()
	{
		return devices[current_dev];
	}
//	void set_settings()
//	{	
//	}

private:
	QMap<CURRENT_DEV, MV_DEV> devices;
	CURRENT_DEV current_dev;
	int com;
	int nim;
};
*/
/*class Sub_tmp
{
public:
	void set_lab(QLabel* lab)
	{
		sub_lab = lab;
	}
	void set_line(QLineEdit* line)
	{
		sub_lined = line;
	}
	void order()
	{
		sub_just->addWidget(sub_lab);
		sub_just->addWidget(sub_lined);
	}
private:
	QLabel* sub_lab;
	QLineEdit* sub_lined;
	QVBoxLayout* sub_just = new QVBoxLayout();
};*/
class MBK07_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	MBK07_widg(QWidget *parent = 0);
	//~MBK07_widg();
	

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
	STAB current_stab;
	ANTENNA current_antenna;


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
	bool flag;
	QPushButton * okBut;
	QDialog *dlg;

	QList<QCheckBox*> set_list;
	QList<QCheckBox*> set_list_mu2;
	QList<QLineEdit*> sub_le_list;

	void paint_buttons();
	void write_words();
	void set_new_tm();
protected:
	
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);

	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);

//	void current_kom(int mshm_numb, int pshm_numb);
	void auto_scroll_clicked(int _state);
	void log_timer_ontimer();
//	void mbk04(const QString& new_text);
//	void save_choose_set();
private:
//	MU_MODULE mu_module;
//	QList<MV_MODULE> mvku_modules;
//	QList<MV_MODULE> mvmk_modules;

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
	void msg_to_log(const QString& _msg);

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_lka05_SLOT_Thread lka05_slot_thr;
	RPC_lka05_SIGNAL_Thread lka05_signal_thr;


	QMap<int, QString> mode_names;
	QMap<int, LITERA> lit_map;
	QMap<int, QString> stab_names;
	QMap<int, QString> ant_names;
	bool ab_state = true;
};

#endif // MBK07_H

