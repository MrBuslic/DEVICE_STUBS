#ifndef KPI_BUS_H
#define KPI_BUS_H
#include "rpc_loger.h"
#include <QWidget>

class KPIWidget : public QWidget
{
	Q_OBJECT

public:
	KPIWidget(QWidget *parent = 0);

public slots:
	void make_KPI(QVariantList KPI_list);
private:
	LogWidget* log_widget;
signals:
	void new_KPI(QVariantList KPI_list);
};

#endif // KPI_BUS_H

