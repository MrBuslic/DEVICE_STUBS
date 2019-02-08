#ifndef LKA05_H
#define LKA05_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include "LKA05Modules.h"

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../mbk04_imitator/mbk04_rpc.h"

class LKA05_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	LKA05_widg();
	~LKA05_widg();
	void set_new_tm();
	
private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;
	QPushButton *MU1;
	QPushButton *MU2;


	QList<QPushButton*> main_MVKU;
	QList<QPushButton*> reserve_MVKU;
	QList<QPushButton*> main_MVMK;
	QList<QPushButton*> reserve_MVMK;

	QGroupBox *MVKU0_gb;
	QGroupBox *MVKU1_gb;
	QGroupBox *MVKU2_gb;
	QGroupBox *MVMK0_gb;
	QGroupBox *MVMK1_gb;
	QGroupBox *MVMK2_gb;
	int MKO;
	int adr;
	QVariantList words;
	QGridLayout *MB_glayout;
	QGridLayout *MU_glayout;
	bool flag;
	//// window choose settings
	QPushButton * okBut;
	QDialog *dlg;
	void MU1_set(QVariantList words);
	QGroupBox* CheckButtonsBox;
	QCheckBox* add_set(QString name, QString data, bool is_main = true);
	void choose_dialog();
	QCheckBox cb;
	QList<QCheckBox*> set_list;
	QList<QCheckBox*> set_list_mu2;

	void paint_buttons();

public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os); 

	void save_choose_set();
	void new_tm(int tm); 
signals:
	void new_ku(int ku_n, int length, double u);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);
private:
	MU_MODULE mu_module;
	QList<MV_MODULE> mvku_modules;
	QList<MV_MODULE> mvmk_modules;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mbk04_SLOT_Thread mbk04_slot_thr;
	RPC_mbk04_SIGNAL_Thread mbk04_signal_thr;
};

#endif // LKA05_H

