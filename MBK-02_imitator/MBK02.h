#ifndef MBK02_H
#define MBK02_H

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

enum CHANEL
{
	CHANEL_1 = 0,
	CHANEL_2 = 1,
	CHANEL_OFF = 2,
	CHANEL_ERR = 3
};
enum ANTENNA_CH1
{
	MHA1PY = 0,
	MHA1MY = 1,
	MHA1ERR = 3
};
enum ANTENNA_CH2
{
	MHA2PY = 0,
	MHA2MY = 1,
	MHA2ERR = 3
};
class MBK02_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	MBK02_widg(QWidget *parent = 0);
	//~MBK02_widg();
	

private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	//Хранимые текущие

	int current_lit = 0;

	QPlainTextEdit* Logs;

	const int MKO = 1;
	const int adr = 2;
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

#endif // MBK02_H

