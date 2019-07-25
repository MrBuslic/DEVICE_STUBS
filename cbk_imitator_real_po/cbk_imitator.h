#ifndef CBK_IMIT_H
#define CBK_IMIT_H
#include "omnibus_rpc.h"
#include "VIP_imit.h"
#include "real_tpo\MY_BOS\thread_module.h"
#include <QtWidgets>

enum PowerState{ON, OFF, CRASH};
enum POState{SPOBU, TPO, VACANT, UNDEFINED};

struct VM_State
{
	int VMPowerState[4];
	int VMPOState[4];
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
	QLabel *VM1_Label, *VM2_Label, *VM3_Label, *VM4_Label, *ipMFSK_Label, *ipMDS_Label, *portMFSK_Label, *portMDS_Label;
	QComboBox *VM1_Combo, *VM2_Combo, *VM3_Combo, *VM4_Combo;
	QPushButton *VM1_ON, *VM2_ON, *VM3_ON, *VM4_ON, *VM1_OFF, *VM2_OFF, *VM3_OFF, *VM4_OFF, *VM1_CRASH, *VM2_CRASH, *VM3_CRASH, *VM4_CRASH, *MFSK_ON, *MFSK_RE, *MDS_ON, *MDS_RE, *Get_Time;
	QLineEdit *ipMFSK_Edit, *portMFSK_Edit, *ipMDS_Edit, *portMDS_Edit;
	QTextEdit *edit;
	QStringList str_combo;
	QGroupBox *createVM1Group();
	QGroupBox *createVM2Group();
	QGroupBox *createVM3Group();
	QGroupBox *createVM4Group();
	QGroupBox *createMDS32Group();
	QGroupBox *createMFSK24Group();
	mds32_exchange* mds32_imit;
	mfsk24_exchange* mfsk24_imit;
	TPOThread tpo_thread;
	InterHandlerThread interrupt_thread;
	//TimeThread b_time;

	bool pitanie[5];

	void VM_init();
	void WorkState_init();
	void set_str_combo();
	void closeEvent(QCloseEvent *event);

private slots:
	void set_VM1_ON();
	void set_VM2_ON();
	void set_VM3_ON();
	void set_VM4_ON();
	
	void set_VM1_OFF();
	void set_VM2_OFF();
	void set_VM3_OFF();
	void set_VM4_OFF();
	
	void set_VM1_CRASH();
	void set_VM2_CRASH();
	void set_VM3_CRASH();
	void set_VM4_CRASH();

	void change_PO_VM1(int index);
	void change_PO_VM2(int index);
	void change_PO_VM3(int index);
	void change_PO_VM4(int index);
	
	void slot_vm_is_on(int n_vm);
	void slot_vm_is_off(int n_vm);
	void run_PO(int PO);
	void shutdown_PO(int PO);

	void read_settings();
	void write_settings();

	void connectMDS();
	void connectMFSK();
	void reconnectMDS();
	void reconnectMFSK();
	void mds32_send_sample(int channel, uint& buf, int& flag);
	void slot_mfsk24_impulse_change(QVariantList);
	
	void show_time();

signals:
	void vm_is_on(int n_vm);
	void vm_is_off(int n_vm);
	void vm_change_po(int n_vm);
};
#endif