#ifndef R733_H
#define R733_H

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include "R733Modules.h"
#include "instruments.h"

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
//#include "../MBK-02_imitator/MBK02_rpc.h"
#include "../mbk04_imitator/mbk04_rpc.h"

class R733_widg : public QWidget
{
    Q_OBJECT
public:
	R733_widg();
	~R733_widg() {}
private:
	QPushButton *MU1;
	QPushButton *MU2;
	QPushButton *main_MPVN;
	QPushButton *reserve_MPVN;
	QPushButton* main_MVKU;
	QPushButton* reserve_MVKU;

	QCheckBox* auto_scroll_box;

	QList<QPushButton*>  VCH_list;
	QList<QPushButton*>  UPI_list;
	QGroupBox *MVKU_gb;
	QGroupBox *MPVN_gb;
	QGroupBox *VCH_gb;
	QGroupBox *UPI_gb;

	QGridLayout *MU_glayout;


	//MAJORITAR majoritar;
	REGIME regime_upi;

	MU_MODULE mu_module;
	UPI_MODULE upi_module;
	QList<MV_MODULE> mpvn_modules;
	QList<MV_MODULE> mvku_modules;
	//QList<UPI_MODULE> UPI_modules;

	RPC_omnibus_SLOT_Thread omni_slot_thr;
	RPC_omnibus_SIGNAL_Thread omni_signal_thr;

//	RPC_MBK02_SLOT_Thread mbk02_slot_thr;
//	RPC_MBK02_SIGNAL_Thread mbk02_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SLOT_Thread mku_signal_thr;

	RPC_mbk04_SLOT_Thread mbk04_slot_thr;
	RPC_mbk04_SIGNAL_Thread mbk04_signal_thr;

	int MKO;
	int adr;
	bool flag;
	int num_ku;

	int rrr;
	int rpk;
	int rkm;
	int rpk_1;
	int rpk_2;
	QTextDocument* _doc;
	QTextEdit* edit_info;
	QStringList log_buffer;
	QMutex log_mutex;
	QTextCursor* _cursor;
	QScrollBar* _scroll_bar;
	bool auto_scroll;

	void paint_buttons();
	void new_data_mv();
	void msg_to_log(const QString& _msg);

	QMap<int, QString> mode_names;
	QMap<int, QString> major_names;
	QMap<int, QString> channel_names;

	QList<int> channels_upi;
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void auto_scroll_clicked(int _state);
signals:
	void new_ku(int ku_n, int length, double u, int line);
};

#endif // R733_H

