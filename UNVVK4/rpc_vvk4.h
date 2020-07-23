#ifndef RPC_VVK4_H
#define RPC_VVK4_H
#ifdef WIN32
#include "winsock2.h"
#endif
#include <QObject>
#include <QString>
#include <QWidget>
#include <QThread>
#include <QLineEdit>
#include <QTimer>
#include <QMutex>
#include <QFormLayout>
#include <memory>
#include <qlayout.h>

class RpcVvk4Widget : public QWidget
{
	Q_OBJECT
public:
	RpcVvk4Widget();
	enum CHAN
	{
		EI = 0,
		SUM
	};
public slots:
	int unvvk4_commut_ListOutput(int _line, QString _masOn, QString _masOff);
	void get_commut_chanels_list(QVariantList& ei_list, QVariantList& sum_list);
	int unvvk4_config_MeasureLine(int line, int state);
	void get_measure_lines(QVariantList& mes_list);
private:
	QLineEdit* ei_chanels_lbl;
	QLineEdit* sum_chanels_lbl;
	QLineEdit* meas_chanels_lbl;
	QMutex log_mutex;
	QList<int> ei_chanels_list;
	QList<int> sum_chanels_list;
	QList<int> measure_line_list;

	QMutex comut_mutex;
	QMutex meas_mutex;

	QList<int> string_to_int_list(QString chanels);
	QString int_list_to_string(QList<int> chanels_list);
	void update_view();
};

#endif //RPC_VVK4_H