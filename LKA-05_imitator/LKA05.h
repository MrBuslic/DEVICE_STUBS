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

#include "../OMNIBUSBOX/omnibus_rpc.h"

class LKA05_MODULE
{
public:
	virtual unsigned short get_tm() = 0;
};

enum CURRENT_DEV
{
	MAIN = 1,
	RESERVE = 2,
	OFF = 3
};

class MU_MODULE : public LKA05_MODULE
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
protected:
	
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
//	void mbk04(const QString& new_text);
	void save_choose_set();
signals:
	void new_ku(int ku_n, int length, double u);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt);
private:
	MU_MODULE mu_module;
	QList<MV_MODULE> mvku_modules;
	QList<MV_MODULE> mvmk_modules;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;
};

#endif // LKA05_H

