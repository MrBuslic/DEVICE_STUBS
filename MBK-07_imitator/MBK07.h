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
//Стабильность
enum BK_stability
{
	High = 1,//Высокая
	KG_1 = 2,//Низкая, аключен КГ1
	KG_2 = 3//Низкая, включен КГ2
};
//Режим
enum Mode
{
	PI15,
	PI8FM_1,//ПИ8ФМ15
	PI8FM_0,//ПИ8ФМ1,5
	WTF8,
	IMFM,
	IM,
	IM1
};
//Литера
int lit;
//Подрежимы ПИ-8
enum SubmodePI8
{
	PSP_1,
	PSP_2,
	PSP_3,
	PSP_4	
};
//Обшие подрежимы
enum Submode
{
	Off,
	On
};
//Антена
enum Antenna
{
	OHA,
	MHA0Y,
	MHA1Y
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
class Sub_tmp
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
};
class MBK07_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	MBK07_widg();
	//~MBK07_widg();
	//void set_new_tm();

private:
	QWidget* widg;
	/// -- Главное окно;
	QMainWindow* main_widg;

	QList<QPushButton*> FSMU_blocks;
	QList<QPushButton*> FSVU_canals;

	QPlainTextEdit* Logs;

	QGroupBox *FSMU_gb;
	QGroupBox *FSVU_gb;
	int MKO;
	int adr;
	QVariantList words;
	QGridLayout *SubGrid_glayout;
	QVBoxLayout *FSMUFSVU_vblayout;
	QHBoxLayout *FSVU_hblayout;
	QHBoxLayout *FSMU_hblayout;
	QHBoxLayout *All_vblayout;
	bool flag;
	//// window choose settings
	QPushButton * okBut;
	QDialog *dlg;
	//void MU1_set(QVariantList words);
	//QGroupBox* CheckButtonsBox;
	//QCheckBox* add_set(QString name, QString data, bool is_main = true);
	void choose_dialog();
	//QCheckBox cb;
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
//	MU_MODULE mu_module;
//	QList<MV_MODULE> mvku_modules;
//	QList<MV_MODULE> mvmk_modules;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;
};

#endif // MBK07_H

