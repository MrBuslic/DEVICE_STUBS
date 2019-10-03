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
#include "../buses_imitator/power_bus_rpc.h"
#include "../buses_imitator//interrupt_bus_rpc.h"
#include "../MBK-02_imitator/MBK02_rpc.h"

class R732_widg : public QWidget
{
    Q_OBJECT
public:
	R732_widg();
	~R732_widg();
private:
	QPushButton *MU1;
	QPushButton *MU2;
	QPushButton *main_MPVN;
	QPushButton *reserve_MPVN;
	QPushButton* main_MVKU;
	QPushButton* reserve_MVKU;
	QPushButton* vchm0;
	QPushButton* vchm1;
	QPushButton* vchm2;
	QPushButton* vchm3;
	QList<QPushButton*> vchm_btns_lst;
	QPushButton* proc_vchm0;
	QPushButton* proc_vchm1;
	QPushButton* proc_vchm2;
	QPushButton* proc_vchm3;
	QList<QPushButton*> proc_vchm_btns_lst;

	R732_MU_MODULE mu_module;
	QList<R732_MV_MODULE> mpvn_modules;
	QList<R732_MV_MODULE> mvku_modules;
	R732_VCHM_MODULE vchm_module;

	RPC_omnibus_SLOT_Thread omni_slot_thr;
	RPC_omnibus_SIGNAL_Thread omni_signal_thr;

	RPC_MBK02_SLOT_Thread mbk02_slot_thr;
	RPC_MBK02_SIGNAL_Thread mbk02_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	RPC_interrupt_bus_SLOT_Thread interrupt_slot_thr;
	RPC_interrupt_bus_SIGNAL_Thread interrupt_signal_thr;

	int MKO;
	int adr;
	int num_ku;

	int bus;
	double power;
	double volt;
	bool power_on;
	bool ready_to_work_hard;
	QString name;
	int kpi_counter;

	unsigned short mko_counter;

	void paint_buttons();
	void new_data_mv();
	void set_new_tm();
	void set_new_kpi();
	QVariantList get_mko_counter_word();
	QVariantList get_pups_words_list();
	QVariantList get_vchm_word();
	QTimer vchm_on_timer;
	QTimer mu_on_timer;
	QList<int> vchm_chanels_init;
	bool vchm_is_init;

	void imit_on();
	void imit_off();
	void set_power_back();
	void restart_vchm_proc(int chanel);
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void set_new_mbk02_tm(int sadr, int word);
	void get_power(double volt);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);
	void new_kpi(QVariantList kpi);
private slots:
	void set_vchm_on();
	void set_mu_on();
signals:
	void new_ku(int ku_n, int length, double u, int line);
};

#endif // R732_H

