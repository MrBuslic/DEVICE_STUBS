#ifndef CBK_IMIT_H
#define CBK_IMIT_H
#include "omnibus_rpc.h"

#include "real_tpo\MY_BOS\thread_module.h"
#include <QtWidgets>
#include <bitset>

#include "../buses_imitator/power_bus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"

enum PowerState{ON, OFF, CRASH};
enum POState{SPOBU, TPO, VACANT, UNDEFINED};

#define SPO_START_TIME 30000

struct VM_State
{
	int VMPowerState[4];
	int VMPOState[4];
	long long StartTime[4];
	bool running[4];
};

struct Work_State
{
	int VM;
	int PO;
};

class CBK_MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CBK_MainWindow();
protected:
	QSettings *m_settings;
private:
	VM_State VMS;
	Work_State w_state;
	QList<QLabel*> VM_Labels;
	QList<QComboBox*> VM_Combos;
	QList<QPushButton*> ON_btns;
	QList<QPushButton*> OFF_btns;
	QList<QPushButton*> CRASH_btns;
	QPushButton *Get_Time;
	QTextEdit *edit;
	QStringList str_combo;
	QGroupBox *createVMGroup(int n_vm);

	TPOThread tpo_thread;
	InterHandlerThread interrupt_thread;
	//TimeThread b_time;

	std::bitset<5> pitanie;

	void VM_init();
	void WorkState_init();
	void set_str_combo();
	void closeEvent(QCloseEvent *event);

	QMap<QTimer*, int> on_timers;
	QMap<int, QTimer*> on_timers_;

	void opo_loaded(int n_vm);

private slots:
	void on_timeout();

	void set_VM_ON(int n_vm);
	void VM_ON_clicked();
	void set_VM_OFF(int n_vm);
	void VM_OFF_clicked();
	void set_VM_CRASH(int n_vm);
	void VM_CRASH_clicked();
	void change_PO_VM(int index);
	
	//void slot_vm_is_on(int n_vm);
	//void slot_vm_is_off(int n_vm);
	void run_PO(int PO);
	void shutdown_PO(int PO);

	void read_settings();
	void write_settings();


	void show_time();

	void get_power(double _volt);
public slots:
	void set_tm_state();
	void new_ku(int ku_n, int length, double u, int line);

signals:
	void vm_is_on(int n_vm);
	void vm_is_off(int n_vm);
	void vm_change_po(int n_vm);

private:
	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;
};
#endif