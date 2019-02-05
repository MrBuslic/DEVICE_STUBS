#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QString>
#include <QPushButton>
#include <QLayout>
#include <QFileDialog>
#include <QFile>
#include <QThread>
#include <QProgressBar>
#include <QLabel>
#include <QFrame>
#include <QPixmap>
#include <QMainWindow>
#include <QVariant>
#include "mbk04Modules.h"


class MainWidget :
	public QWidget
{
	Q_OBJECT
public:
	MainWidget();
	~MainWidget();
	
private:
	QWidget* widg;
	
	QMainWindow* main_widg;
	
	QPushButton* o_rez_btn;
	
	QPushButton* r_rez_btn;
	
	QPushButton* ik15_btn;
	
	QPushButton* ik8_btn;
	
	QPushButton* vtf_btn;
	
	static const int MKO = 1;
	static const int adr = 4;
	int flag;
	QMap<CURRENT_DEV, MV_DEV> devices;
	CURRENT_DEV current_dev;
	REZH_FRAME current_rezh;
	void state_changed();
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku(int ku_n, int length, double u);
signals:
	void new_tm(int nw);

};

#endif

