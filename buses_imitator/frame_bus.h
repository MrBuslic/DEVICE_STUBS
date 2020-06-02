#ifndef FRAME_BUS_H
#define FRAME_BUS_H

#include <QWidget>
#include <QVariant>
#include "rpc_loger.h"

class FrameBusWidget : public QWidget
{
	Q_OBJECT

public:
	FrameBusWidget(QWidget *parent = 0);

public slots:
	void make_new_frame_04(QString mode, QVariant frame_data);
	void make_new_frame_733(QString mode, QVariant frame_data);
	void make_new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data);
	void make_new_frame_rm07(QString mode, QVariant frame_data);
	void make_new_frame_n737(QString mode, QVariant frame_data);
protected:
	void closeEvent(QCloseEvent *event);
private:
	LogWidget* log_widget;
signals:
	void new_frame_04(QString mode, QVariant frame_data);
	void new_frame_733(QString mode, QVariant frame_data);
	void new_frame_07(QString mode, int psp, int lit, int fm, QString ant, QVariant frame_data);
	void new_frame_rm07(QString mode, QVariant frame_data);
	void new_frame_n737(QString mode, QVariant frame_data);
};

#endif // MBK02_H

