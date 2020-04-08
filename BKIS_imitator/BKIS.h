#ifndef BKIS_H
#define BKIS_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>
#include <QLineEdit>
#include <QTimer>
#include <QTime>
#include <qplaintextedit.h>
#include <qsettings.h>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

union BKIS_modes
{
	quint16 _word;
	struct
	{
		quint16 kontr : 2, //BKIS_modes
			cgo : 2, //BKIS_modes
			gpfm2 : 2, //BKIS_modes
			gpfm1 : 2, //BKIS_modes
			sig_96 : 1, //0 - сигнал от УПИ, 1- сигнал от 15Э1827
			dev_on : 1, //1 - включать устр-ва,1 - игнорировать первые биты
			res : 4,
			kontr_mode : 1; // 0 - режим контроля, 1 - рабочий режим

	};
};

union BKIS_shos
{
	quint16 _words[32];
	struct
	{
		quint16 pprch_chans;
		quint16 pprch_key : 1, //1 - основной ключ, 0 - резервный
			dpkr_mode : 1, //1 - штатная работа, 0 - контроль
			res1 : 2,
			pprch_afs2_mode : 1, //1 - многочастотный, 0 - одночастотный
			pprch_afs1_mode : 1, //1 - многочастотный, 0 - одночастотный
			res2 : 10;
		quint16 sts_afs1_freq_code : 9,
			sgs_afs1_freq_code : 7;
		quint16 sts_afs2_freq_code : 9,
			sgs_afs2_freq_code : 7;

		quint16 gs_chan_2 : 7,
			gs_chan_202 : 1,
			gs_chan_1 : 7,
			gs_chan_201 : 1;


		quint16 gs_chan_4 : 7,
			gs_chan_204 : 1,
			gs_chan_3 : 7,
			gs_chan_203 : 1;

		quint16 gs_chan_6 : 7,
			gs_chan_206 : 1,
			gs_chan_5 : 7,
			gs_chan_205 : 1;

		quint16 gs_chan_8 : 7,
			gs_chan_208 : 1,
			gs_chan_7 : 7,
			gs_chan_207 : 1;

		quint16 gs_chan_10 : 7,
			gs_chan_210 : 1,
			gs_chan_9 : 7,
			gs_chan_209 : 1;

		quint16 gs_chan_12 : 7,
			gs_chan_212 : 1,
			gs_chan_11 : 7,
			gs_chan_211 : 1;

		quint16 gs_chan_14 : 7,
			gs_chan_214 : 1,
			gs_chan_13 : 7,
			gs_chan_213 : 1;

		quint16 gs_chan_16 : 7,
			gs_chan_216 : 1,
			gs_chan_15 : 7,
			gs_chan_215 : 1;

		quint16 gs_chan_18 : 7,
			gs_chan_218 : 1,
			gs_chan_17 : 7,
			gs_chan_217 : 1;

		quint16 gs_chan_20 : 7,
			gs_chan_220 : 1,
			gs_chan_19 : 7,
			gs_chan_219 : 1;

		quint16 gs_chan_22 : 7,
			gs_chan_222 : 1,
			gs_chan_21 : 7,
			gs_chan_221 : 1;

		quint16 gs_chan_24 : 7,
			gs_chan_224 : 1,
			gs_chan_23 : 7,
			gs_chan_223 : 1;

		quint16 gs_chan_26 : 7,
			gs_chan_226 : 1,
			gs_chan_25 : 7,
			gs_chan_225 : 1;

		quint16 gs_chan_gr2 : 7,
			gs_chan_2gr2 : 1,
			gs_chan_gr1 : 7,
			gs_chan_2gr1 : 1;

		quint16 gs_chan_gr4 : 7,
			gs_chan_2gr4 : 1,
			gs_chan_gr3 : 7,
			gs_chan_2gr3 : 1;

		quint16 gs_chan_gr_res : 8,
			gs_chan_gr5 : 7,
			gs_chan_2gr5 : 1;

		quint16 komut_12 : 12,
			pr_12 : 1,//0 - АФС-1, 1 - АФС-2
			res_12 : 3;

		quint16 komut_01 : 12,
			pr_01 : 1,//0 - АФС-1, 1 - АФС-2
			res_01 : 3;

		quint16 res3 : 4,
			trans_mode : 1, //0 - 1 только СС, 1 - ГС + СС
			shps_freq : 1, //0 - 19.2 МГц, 1 - 4.8 МГц
			shps_key : 1, //0 - резервный ключ, 1 - основной ключ
			shps_struct : 1, //0 - непериодическая (ДСЧ ВКЛ), 1 - периодическая (ДСЧ ОТКЛ)
			ss_modulation : 3,  //000 – УП СС
								//001 – ШП СС
								//010 – техн.режим 1 («лог.1» пост.уровня)
								//011 – техн.режим 2 («лог.0» пост.уровня)
								//100 – техн.режим 3 («меандр» 2, 4 Мбит / с)
								//101 – техн.режим 4 («лог.1» импульсная передача)
								//110 – техн.режим 5 («лог.0» импульсная передача)
								//111 – техн.режим 6 («меандр» 19, 2 Мбит / с)
			
			gs_modulation : 3,  //000 – УП СС
								//001 – ШП СС
								//010 – техн.режим 1 («лог.1» пост.уровня)
								//011 – техн.режим 2 («лог.0» пост.уровня)
								//100 – техн.режим 3 («меандр» 2, 4 Мбит / с)
								//101 – техн.режим 4 («лог.1» импульсная передача)
								//110 – техн.режим 5 («лог.0» импульсная передача)
								//111 – техн.режим 6 («меандр» 19, 2 Мбит / с)

			gs_speed : 2;   //00 – 60 кбит/с
							//01 – резерв
							//10 – 300 кбит / с
							//11 – 600 кбит / с
		quint16 res_words[9];

	};
};

class BKIS_widg : public QWidget
{
    Q_OBJECT
	
public:
	BKIS_widg(QWidget *parent = 0);
	~BKIS_widg();
	
private:
	//BKIS_KP current_KP = BKIS_OFF;

	const int MKO = 0;
	const int adr = 22;

	void update_graphics();//обновление графики
	void set_new_tm();//составление ОК-ов

	void imit_off();//включение имитатора
	void imit_on();//выключение имитатора
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku_mk(int ku, int length_ku, double u_ku, int line_ku);
	void set_blk_state(bool state);
	void set_interface_state(bool state);
private:
	QString name = "АОС";//Имя устройства
	int bus  = 2;//номер шины для шины питания(power_bus)
	int volt;//Принятое напряжение

	bool blk_state;
	bool interface_state;
	
	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // BKIS_H

