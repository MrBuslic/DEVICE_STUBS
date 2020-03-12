#ifndef MKU_BUS_H
#define MKU_BUS_H

#include <QWidget>
#include <QMap>
#include "rpc_loger.h"

enum
{
	UM = 0,
	LBV = 1,
	PRM_Ant_1 = 2,
	PRM_Ant_2 = 3,
	SGTS_Ant_2 = 4,
	SGTS_Ant_1 = 5,
	BOCH = 6,
	FOS = 7,
	AOS = 8,
	UPch = 9,
	ALPS = 10,
	MFS = 11,
	LPch = 12,
	BUFAR = 13
};

class MKUWidget : public QWidget
{
	Q_OBJECT

public:
	MKUWidget(QWidget *parent = 0);

public slots:
	void make_ku(int ku_n, int length, double u, int line);
	void make_ku_732(int ku_n, int length, double u, int line);
	void make_ku_cbk(int ku_n, int length, double u, int line);
	void make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	int ku_map_channels_setup(int ku_n, int line);
	int mshm_map_channels_setup(int mshm, int line_m);
	int pshm_map_channels_setup(int pshm, int line_p);
	void make_mt_at_state(int dev_name, int state);

	void get_tm(QString tm_name, QVariant& tm_val);
	void set_tm(QString tm_name, QVariant tm_val);
signals:
	void new_ku(int ku_n, int length, double u, int line);
	void new_ku_732(int ku_n, int length, double u, int line);
	void new_ku_cbk(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_mt_at_state(int dev_name, int state);
private:
	LogWidget* log_widget;

	QString get_line(int line);

	QMap<int, int> ku_map_channels;
	QMap<int, int> mshm_map_channels;
	QMap<int, int> pshm_map_channels;

	QMap<QString, QVariant> tm_map;
};

#endif // MKU_BUS_H

