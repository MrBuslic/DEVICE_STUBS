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

class BusesWidget : public QWidget
{
	Q_OBJECT

public:
	//	explicit LKA05_widg(QWidget *parent = 0);
	BusesWidget(QWidget *parent = 0);
	//~MBK02_widg();



public slots:
	void make_interrupt(int _n, short _chan, double _u, double _t);
	void make_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void make_ku(int ku_n, int length, double u, int line);
	void make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void make_KPI(QVariantList KPI_list);

signals:
	void new_interrupt(int _n, short _chan, double _u, double _t);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_KPI(QVariantList KPI_list);
};

#endif // MBK02_H

