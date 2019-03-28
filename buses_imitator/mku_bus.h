#ifndef MKU_BUS_H
#define MKU_BUS_H

#include <QWidget>

class MKUWidget : public QWidget
{
	Q_OBJECT

public:
	MKUWidget(QWidget *parent = 0);


public slots:
	void make_ku(int ku_n, int length, double u, int line);
	void make_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);

signals:
	void new_ku(int ku_n, int length, double u, int line);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
};

#endif // MKU_BUS_H

