#ifndef R732_H
#define R732_H

#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include "R732Modules.h"
#include "instruments.h"

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../MBK-02_imitator/MBK02_rpc.h"

class R732_widg : public QWidget
{
    Q_OBJECT
public:
	R732_widg();
	~R732_widg() {}
private:
	QPushButton *MU1;
	QPushButton *MU2;
	QPushButton *main_MPVN;
	QPushButton *reserve_MPVN;
	QPushButton* main_MVKU;
	QPushButton* reserve_MVKU;

	QGroupBox *MVKU_gb;
	QGroupBox *MPVN_gb;

	QGridLayout *MU_glayout;

	MU_MODULE mu_module;
	QList<MV_MODULE> mpvn_modules;
	QList<MV_MODULE> mvku_modules;

	RPC_omnibus_SLOT_Thread omni_slot_thr;
	RPC_omnibus_SIGNAL_Thread omni_signal_thr;

	RPC_MBK02_SLOT_Thread mbk02_slot_thr;
	RPC_MBK02_SIGNAL_Thread mbk02_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SLOT_Thread mku_signal_thr;

	int MKO;
	int adr;
	bool flag;
	int num_ku;

	void paint_buttons();
	void new_data_mv();
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void set_new_mbk02_tm(int sadr, int word);
signals:
	void new_ku(int ku_n, int length, double u, int line);
};

#endif // R732_H

