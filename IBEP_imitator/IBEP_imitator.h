#ifndef IBEP_imitator_H
#define IBEP_imitator_H

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollBar>
#include <QList>

#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>


class IBEP_imitator :
	public QMainWindow
{
	Q_OBJECT
public:
	IBEP_imitator();
	~IBEP_imitator();
	
private:
	QWidget* widg;
	
	QPushButton *NK_btn;
	QPushButton *KOM_btn;

	QLabel *voltage_label;
	QLabel *amperage_label;

	QGridLayout *gridLayout;

	void setNK(bool on);
	void setKOM(bool on);

signals:
	void test(QByteArray ba);

public slots:
	void receiveData(QByteArray ba);


};

#endif

