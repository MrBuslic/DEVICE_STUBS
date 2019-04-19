#ifndef FRAME_BUS_H
#define FRAME_BUS_H

#include <QWidget>
#include <QVariant>

class FrameBusWidget : public QWidget
{
	Q_OBJECT

public:
	FrameBusWidget(QWidget *parent = 0);

public slots:
	void make_new_frame(QString mode, QVariant frame_data);

signals:
	void new_frame(QString mode, QVariant frame_data);
};

#endif // MBK02_H

