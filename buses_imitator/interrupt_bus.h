#ifndef INTERRUPT_BUS_H
#define INTERRUPT_BUS_H

#include <QWidget>
#include "rpc_loger.h"

class InterruptWidget : public QWidget
{
	Q_OBJECT

public:
	InterruptWidget(QWidget *parent = 0);

public slots:
	void make_interrupt(int _n, short _chan, double _u, double _t);
private:
	LogWidget* log_widget;
signals:
	void new_interrupt(int _n, short _chan, double _u, double _t);
};

#endif // INTERUPT_BUS_H

