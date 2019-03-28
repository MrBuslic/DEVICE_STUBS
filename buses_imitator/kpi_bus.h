#ifndef KPI_BUS_H
#define KPI_BUS_H

#include <QWidget>

class KPIWidget : public QWidget
{
	Q_OBJECT

public:
	KPIWidget(QWidget *parent = 0);

public slots:
	void make_KPI(QVariantList KPI_list);

signals:
	void new_KPI(QVariantList KPI_list);
};

#endif // KPI_BUS_H

