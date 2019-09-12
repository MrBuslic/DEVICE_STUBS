#ifndef MKU_BUS_H
#define MKU_BUS_H

#include <QWidget>
#include <QMap>
#include "rpc_loger.h"

class MKUWidget : public QWidget
{
	Q_OBJECT

public:
	MKUWidget(QWidget *parent = 0);

public slots:
	void make_ku(int ku_n, int length, double u, int line);
	void make_ku_732(int ku_n, int length, double u, int line);
	void make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	int ku_map_channels_setup(int ku_n, int line);
	int mshm_map_channels_setup(int mshm, int line_m);
	int pshm_map_channels_setup(int pshm, int line_p);

signals:
	void new_ku(int ku_n, int length, double u, int line);
	void new_ku_732(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
private:
	LogWidget* log_widget;

	QString get_line(int line);
private:
	QMap<int, int> ku_map_channels;
	QMap<int, int> mshm_map_channels;
	QMap<int, int> pshm_map_channels;
};

#endif // MKU_BUS_H

