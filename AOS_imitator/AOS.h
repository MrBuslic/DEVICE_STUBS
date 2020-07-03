#ifndef AOS_H
#define AOS_H

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

enum DEVICE_STATES
{
	AOS_KP_OFF = 0, 
	AOS_KP_1 = 4,
	AOS_KP_2 = 2,
	AOS_KP_3 = 1
};

union AOS_modes
{
	quint16 _word;
	struct
	{
		quint16 kontr : 2,  // AOS_modes // устройство контроля 00 - выключение для всех 3-х комплектов
			                                               //   01 - включение первого устройства
			                                               //   10 - включение второго устройства
			                                               //   11 - включение третьего устройства

			cgo : 2,        // AOS_modes // устройство ЦГО      00 - выключение для всех 3-х комплектов
			                                               //   01 - включение первого устройства
			                                               //   10 - включение второго устройства
			                                               //   11 - включение третьего устройства
			gpfm2 : 2,  // AOS_modes    //   00 - выключение для всех 3-х комплектов
			                            //   01 - включение первого устройства
			                            //   10 - включение второго устройства
			                            //   11 - включение третьего устройств 
			gpfm1 : 2,  // AOS_modes //   00 - выключение для всех 3-х комплектов
			                         //   01 - включение первого устройства
			                         //   10 - включение второго устройства
			                         //   11 - включение третьего устройств 
			sig_96 : 1,     // 0 - сигнал от УПИ, 1- сигнал от 15Э1827
			dev_on : 1,     // 1 - включать устр-ва,1 - игнорировать первые биты
			res : 4,
			kontr_mode : 1; // 0 - режим контроля, 1 - рабочий режим

	};
};

union AOS_shos
{
	quint16 _words[32];
	struct
	{
		quint16 pprch_chans;
		quint16 pprch_key : 1, // 1 - основной ключ, 0 - резервный
			dpkr_mode : 1,     // 1 - штатная работа, 0 - контроль // режим работы
			res1 : 2,
			pprch_afs2_mode : 1, //1 - многочастотный, 0 - одночастотный 
			pprch_afs1_mode : 1, //1 - многочастотный, 0 - одночастотный
			res2 : 10;            
		quint16 sts_afs1_freq_code : 9,  // поле "Код частоты синтезаторов точной сетки АФС1"
			sgs_afs1_freq_code : 7;      // поле "Код частоты синтезаторов грубой сетки АФС1"
		quint16 sts_afs2_freq_code : 9,    // поле "Код частоты синтезаторов точной сетки АФС2"
			sgs_afs2_freq_code : 7;        // поле "Код частоты синтезаторов грубой сетки АФС2"

		quint16 gs_chan_2 : 7,   // поле "адрес канала ГС" 
			gs_chan_202 : 1,     // указатель "Канал приёма" | 0- 102 ; 1- 202  |
			gs_chan_1 : 7,       // поле "адрес канала ГС"
			gs_chan_201 : 1;     // указатель "Канал приёма" | 0- 101 ; 1- 201  |


		quint16 gs_chan_4 : 7,    // поле "адрес канала ГС"
			gs_chan_204 : 1,      // указатель "Канал приёма" | 0- 104 ; 1- 204  |
			gs_chan_3 : 7,        // поле "адрес канала ГС"
			gs_chan_203 : 1;      // указатель "Канал приёма" | 0- 103 ; 1- 203  |

		quint16 gs_chan_6 : 7,  // поле "адрес канала ГС"
			gs_chan_206 : 1,    // указатель "Канал приёма" | 0- 106 ; 1- 206  |
			gs_chan_5 : 7,      // поле "адрес канала ГС"
			gs_chan_205 : 1;    // указатель "Канал приёма" | 0- 105 ; 1- 205  |

		quint16 gs_chan_8 : 7,  // поле "адрес канала ГС"
			gs_chan_208 : 1,    // указатель "Канал приёма" | 0- 108 ; 1- 208  |
			gs_chan_7 : 7,      // поле "адрес канала ГС"
			gs_chan_207 : 1;    // указатель "Канал приёма" | 0- 107 ; 1- 207  |

		quint16 gs_chan_10 : 7,   // поле "адрес канала ГС"
			gs_chan_210 : 1,      // указатель "Канал приёма" | 0- 110 ; 1- 210  |
			gs_chan_9 : 7,        // поле "адрес канала ГС"
			gs_chan_209 : 1;      // указатель "Канал приёма" | 0- 109 ; 1- 209  |

		quint16 gs_chan_12 : 7, // поле "адрес канала ГС"
			gs_chan_212 : 1,    // указатель "Канал приёма" | 0- 112 ; 1- 212  |
			gs_chan_11 : 7,     // поле "адрес канала ГС"
			gs_chan_211 : 1;    // указатель "Канал приёма" | 0- 111 ; 1- 211  |

		quint16 gs_chan_14 : 7,   // поле "адрес канала ГС"
			gs_chan_214 : 1,      // указатель "Канал приёма" | 0- 114 ; 1- 214  |
			gs_chan_13 : 7,       // поле "адрес канала ГС"
			gs_chan_213 : 1;      // указатель "Канал приёма" | 0- 113 ; 1- 213  |

		quint16 gs_chan_16 : 7,  // поле "адрес канала ГС"
			gs_chan_216 : 1,     // указатель "Канал приёма" | 0- 116 ; 1- 216  |
			gs_chan_15 : 7,      // поле "адрес канала ГС"
			gs_chan_215 : 1;     // указатель "Канал приёма" | 0- 115 ; 1- 215  |

		quint16 gs_chan_18 : 7,  // поле "адрес канала ГС"
			gs_chan_218 : 1,     // указатель "Канал приёма" | 0- 118 ; 1- 218  |
			gs_chan_17 : 7,      // поле "адрес канала ГС"
			gs_chan_217 : 1;     // указатель "Канал приёма" | 0- 117 ; 1- 217  |

		quint16 gs_chan_20 : 7,     // поле "адрес канала ГС"
			gs_chan_220 : 1,        // указатель "Канал приёма" | 0- 120 ; 1- 220  |
			gs_chan_19 : 7,         // поле "адрес канала ГС"
			gs_chan_219 : 1;        // указатель "Канал приёма" | 0- 119 ; 1- 219  |

		quint16 gs_chan_22 : 7,  // поле "адрес канала ГС"
			gs_chan_222 : 1,     // указатель "Канал приёма" | 0- 122 ; 1- 222  |
			gs_chan_21 : 7,      // поле "адрес канала ГС"
			gs_chan_221 : 1;     // указатель "Канал приёма" | 0- 121 ; 1- 221  |

		quint16 gs_chan_24 : 7,    // поле "адрес канала ГС"
			gs_chan_224 : 1,       // указатель "Канал приёма" | 0- 124 ; 1- 224  |
			gs_chan_23 : 7,        // поле "адрес канала ГС"
			gs_chan_223 : 1;       // указатель "Канал приёма" | 0- 123 ; 1- 223  |

		quint16 gs_chan_26 : 7,  // поле "адрес канала ГС"
			gs_chan_226 : 1,     // указатель "Канал приёма" | 0- 126 ; 1- 226  |
			gs_chan_25 : 7,      // поле "адрес канала ГС"
			gs_chan_225 : 1;     // указатель "Канал приёма" | 0- 125 ; 1- 225  |

		quint16 gs_chan_gr2 : 7,   // поле "адрес каналов ГР2 в ГС [131 - 134] и [231 - 234]"
			gs_chan_2gr2 : 1,      // указатель "каналы приёма ГР2" | 0-  [131 - 134] ; 1- [231 - 234] |
			gs_chan_gr1 : 7,       // поле "адрес каналов ГР1 в ГС [127 - 130] и [227-230]"
			gs_chan_2gr1 : 1;      // указатель "каналы приёма ГР1" | 0-  [127 - 130] ; 1- [227 - 230] |

		quint16 gs_chan_gr4 : 7, // поле "адрес каналов ГР4 в ГС [139 - 142] и [239 - 242]"
			gs_chan_2gr4 : 1,    // указатель "каналы приёма ГР3" | 0-  [139 - 142] ; 1- [239 - 242] |
			gs_chan_gr3 : 7,     // поле "адрес каналов ГР3 в ГС [135 - 138] и [235-238]"
			gs_chan_2gr3 : 1;    // указатель "каналы приёма ГР3" | 0-  [135 - 138] ; 1- [235 - 238] |

		quint16 gs_chan_gr_res : 8,
			gs_chan_gr5 : 7,          // поле "адрес каналов ГР5 в ГС [143 - 146] и [243 - 246]"
			gs_chan_2gr5 : 1;         // указатель "каналы приёма ГР5" | 0- [143 - 146] ; 1- [243 - 246]  |

		quint16 komut_12 : 12,  // поле "Данные комутатора сигнала 1,2 Кбит/с"
			pr_12 : 1,          // 0 - АФС-1, 1 - АФС-2 !адрес приёмника!
			res_12 : 3;

		quint16 komut_01 : 12,   // поле "Данные комутатора сигнала 0,1 Кбит/с"
			pr_01 : 1,           // 0 - АФС-1, 1 - АФС-2 !признак "адреса приёмника"!
			res_01 : 3;

		quint16 res3 : 4,
			trans_mode : 1,  // 0 - 1 только СС, 1 - ГС + СС
			shps_freq : 1,   // 0 - 19.2 МГц, 1 - 4.8 МГц
			shps_key : 1,    // 0 - резервный ключ, 1 - основной ключ
			shps_struct : 1, // 0 - непериодическая (ДСЧ ВКЛ), 1 - периодическая (ДСЧ ОТКЛ)
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

union GSU_kommut
{
	quint16 _word;
	struct
	{
		quint16 kommut_data : 12,
			afs_num : 1,
			res : 3;
	};
};

union AOS_state
{
	quint16 _words[7];

	struct
	{    //Сд1
		quint16 counter : 15, // значения циклического 15-ти разрядного двоичного счётчика
			res_counter : 1; // резерв
		//СД2
		quint16 kontr_dev_state : 3, // 3 комплекта устройств контроля	
			cgo_dev_state : 3,       // 3 комплекта устройства ЦГО
			gpfm2_dev_state : 3,     // 3 комплекта ПРМ тракта АФС2
			gpfm1_dev_state : 3,     // 3 комплекта ПРМ тракта АФС1
			res_sd2 : 1,
			restart_watchdog : 1,    // 1 - перезаупск по сторожевому таймеру, 0 - после включения АОС-МЧ потенциальной командой
			aos_test_data_ready : 1,  
			kontr_mode : 1;          // указатель "Режим работы АОС" | 1- штатная работа; 0- самоконтроль |
		// СД3
		quint16 kontr_dev_working : 3,  // 3 комплекта устройства контроля
			cgo_dev_working : 3,        // 3 комплекта устройства ЦГО
			gpfm2_dev_working : 3,      // 3 комплекта ПРМ тракта АФС2
			gpfm1_dev_working : 3,      // 3 комплекта ПРМ тракта АФС1
			vmu_bu_working : 1,      // работоспособность интерфейса ВМУ БУ АОС (стартовые тесты)
			bu_working : 1,          // работоспособность БУ АОС
			bu_test_data_ready : 1,  // готовность данных самоконтроля БУ АОС
			aos_working : 1;         // работоспособность изделия АОС | 1- изделие исправно ; 0- изделие не исправно |
		// СД4
		quint16 nka_buf_state : 1,    // состояние буфера хранения сообщения "Номера КА"
			gsu_buf_state : 1,        // состояние буфера хранения сообщения "Ключ ШПС"
			shos_buf_state : 1,       // состояние буфера хранения сообщения "Ключ ППРЧ"
			shps_buf_state : 1,       // состояние буфера хранения сообщения "Данные коммутаторов ГСУ"
			pprch_buf_state : 1,      // состояние буфера хранения сообщения "Режимы АОС и данные СхОС"
			priem_soobsheniy_key_sh_p_s : 1,   // 1- данные сообщения "Ключ ШПС" приняты достоверно; 0 - данные сообщения "Ключ ШПС" приняты с ошибкой
			priem_soobsheniy_key_p_p_r_ch : 1, // 1- данные сообщения "Ключ ППРЧ" приняты достоверно; 0 - данные сообщения "Ключ ППРЧ" приняты с ошибкой
			rezerv_SD4_1 : 3,                  // Резерв(значение разрядов - 0) 
			signal_IK8_UPI_BKUPI : 1,          // 1 - сигнал ИК8 присутствует на входе АОС; 0 - отсутствие сигнала ИК8 на входе АОС; 
			TMI_Signal_15e1827 : 1,            // 1 - сигнал 15Э1827 присутствует на входе АОС; 0 - отсутствие сигнала 15Э1827 на входе АОС
			TMI_peredacha_signala_IK8_v_magistralnom_tracte : 2, // 00 - cигнал АОС ("меандр")
																 // 01 - сигнал ИК8 от УПИ БКУПИ
																 // 10 - сигнал 9,6 кБод от 15Э1827
																 // 11 - резерв
			rezerv_SD4_2 : 2;// Резерв(значение разрядов - 0) 

		// СД5
		quint16 rezerv_SD5_1 : 5,            // резерв (значения разрядов - 0) 
			sinhronizatsiy_G_T_CH_C_CGO : 1, // 1 - ГТЧ находится в синхронизме; 0 - отсутствие синхронизации ГТЧ
			signal_OG_v_ustroistve_CGO : 1,  // 1- cигнал ОГ присутствует на входе ЦГО; 0 - отсутствие сигнала ОГ на входе ЦГО
			rezerv_SD5_2 : 1,                // резерв ( значение разряда - 1 ) 
			rezerv_SD5_3 : 2,                // резерв ( значение разряда - 0 ) 
			rabotosposobnost_intef_VMU_BU_AOS : 1,              // работоспособность интерфейса ВМУ БУ АОС  (_VMU_ внутренняя гистраль управления)
			rabotosposobnost_intef_VMU_ustroistva_kontrily : 1, // работоспособность интерфейса ВМУ устройства контроля
			rabotosposobnost_intef_VMU_ustroistva_CGO : 1,      // работоспособность интерфейса ВМУ устройства ЦГО;        | 1 - исправно ; 0 - неисправно | 
			rabotosposobnost_intef_VMU_PRM_trakta_AFS2 : 1,     // работоспособность интерфейса ВМУ ПРМ тракта АФС2 (ГПФМ2)
			rabotosposobnost_intef_VMU_PRM_trakta_AFS1 : 1,     // работоспособность интерфейса ВМУ ПРМ тракта АФС1 (ГПФМ1)
			rezerv_SD5_4 : 1;                                   // резерв(значения разрядов - 1)
		//СД6
		quint16 rezerv_SD6_1 : 4,                             // Резерв(значение разрядов - 0) 
			signala_na_vhode_v_priemnik_tracta_AFS2 : 1,      // 1 - сигнал ПЧ присутствует на входе ГПФМ2; 0 - отсутствие сигнала ПЧ на входе ГПФМ2
			sinhronizatsiy_G_T_CH_priemnika_trakta_AFS2 : 1,  // 1 - ГТЧ находится в синхронизме; 0 - отсутствие синхронизации ГТЧ
			signal_OG_v_priemnike_trakta_AFS2 : 1,            // 1- сигнал ОГ присутствует на входе ГПФМ2; 0 - отсутствие сигнала ОГ на входе ГПФМ2
			rezim_raboti_priemnika_trakta_AFS2 : 1,           // 1 - приём инф. сигнала (инф. = информационного ????); 0 - контроль
			rezerv_SD6_2 : 4,                                 // резерв (значения разрядов - 0)
			signal_P_CH_na_vhode_v_priemnik_trakta_AFS2 : 1,  // 1 - сигнал ПЧ присутствует на входе ГПФМ1; 0 - отсутствует сигнал ПЧ на входе ГПФМ1
			sunhronizatsiy_G_T_CH_priemnika_trakta_AFS1 : 1,  // 1 - ГТЧ находится в синхронизме; 0 - отсутсвие синхронизации ГТЧ
			signal_OG_v_priemnike_trakta_AFS1 : 1,            // 1 - сигнал ОГ присутствует на входе ГПФМ1; 0 - отсутствие сигнала ОГ на входе ГПФМ1
			rezhim_raboti_priemnika_trakta_AFS1 : 1;          // 1 - приём инф. сигнала (инф. = информационного ????); 0 - контроль

		// СД7
		quint16 rezerv_SD7_1 : 4,                        // Резерв(значение разрядов - 0) 
			TMI_rabotosposobnost_UK : 1,                 // 1 - устройство контроля исправно; 0 - устройство контроля не исправно 
			TMI_sinhronizatsiy_G_T_CH_UK : 1,            // 1 - ГТЧ находится в синхронизме; 0 - отсутсвие синхронизации ГТЧ
			TMI_signal_OG_v_ustroistve_kontroly_UK : 1,  // 1 - сигнал ОГ присутствует на входе УК;  0 - отсутсвие сигнала ОГ на входе УК
			rezerv_SD7_2 : 1,                            // Резерв(значение разрядов - 1)
			rezerv_SD7_3 : 1,                            // Резерв(значение разрядов - 0)
			TMI_rabotosposobnost_ustroistva_CGO : 1,     // 1 - устройство ЦГО исправно; 0 - устройство ЦГО не исправно
			rezerv_SD7_4 : 3,                            //  резерв (значения разрядов - 0) 
			TMI_rabotosposobnost_priemnika_trakta_AFS2 : 1,       // 1 - ГПФМ2 исправен; 0 - ГПФМ2 не исправен  (ГПФМ2)
			TMI_rabotosposobnost_priemnika_trakta_AFS1 : 1,       // 1 - ГПФМ1 исправен; 0 - ГПФМ1 не исправен  (ГПФМ1)
			ukazatel_gotovnost_dannih_samokontroly_AOS_M_CH : 1;  // 1 - данные готовы; 0 - данные не готовы
	};
};



class AOS_widg : public QWidget
{
    Q_OBJECT

public:
	AOS_widg(QWidget *parent = 0);
	~AOS_widg();

private:
	//AOS_KP current_KP = AOS_OFF;
	// ---------- виджеты --------- начало --------
	int name_ustroistva, number_komplekta_AOS; // передаваемые параметры ( name_ustroistva = 8 (AOC); number_komplekta_AOS = 1|2|3 )
	QLabel *nomer_vkluchaemogo_komplekta_Label; // просто надпись "№ включенного комплекта AOC: "
	QLabel *nomer_vkluchaemogo_komplekta_samo_znachenie; // сам номер устройства
	//QLabel *nomer_vkluchaemogo_komplekta_Label; // просто надпись "№ включенного комплекта AOC: "
	//;.QLabel *nomer_vkluchaemogo_komplekta_samo_znachenie; // сам номер устройства
	// ----- объявление кнопок ------- начало --------
	QPushButton *perviy_komplekt_GPFM1;
	QPushButton *vtoroi_komplekt_GPFM1;
	QPushButton *tretiy_komplekt_GPFM1;

	QPushButton *perviy_komplekt_GPFM2;
	QPushButton *vtoroi_komplekt_GPFM2;
	QPushButton *tretiy_komplekt_GPFM2;

	QPushButton *perviy_komplekt_CGO;
	QPushButton *vtorji_komplekt_CGO;
	QPushButton *tretiy_komplekt_CGO;

	QPushButton *perviy_komplekt_kontroly;
	QPushButton *vtoroi_komplekt_kontroly;
	QPushButton *tretiy_komplekt_kontroly;
	// ----- объявление кнопок ------- конец --------

    // ---------- виджеты --------- конец --------
	const int MKO = 1;
	const int adr = 21;
	//QHBoxLayout *a;
//	QGroupBox *Group_Vkl_kompl_AOS;
	
	void risovanie_okna_AOS();  // и остальных виджетов АОС
	void update_graphics(int);  // обновление графики
	void funk_perescheta(); // преобразует входные данные 0,1,2,3 в 0,1,2,4
	void set_new_tm();     // составление ОК-ов
	void TMI_1_AOS();   // составление ТМИ - 1 
	void TMI_2_AOS();  // составление ТМИ - 2
	void imit_off();  // включение имитатора
	void imit_on();  // выключение имитатора
protected:
	void closeEvent(QCloseEvent *event);
public slots:
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_ku_mk(int name_ustroistva, int number_komplekta_AOS);

private:
	QString name = "АОС";  // Имя устройства
	int bus  = 2;          // номер шины для шины питания(power_bus)
	int volt;              // Принятое напряжение

	DEVICE_STATES kontr_dev;
	DEVICE_STATES cgo_dev;
	DEVICE_STATES gpfm2_dev;
	DEVICE_STATES gpfm1_dev;
	
	AOS_shos shos;
	AOS_state aos_state;
	AOS_modes aos_modes;
	QList<GSU_kommut> gsu_kommut;

	//QGroupBox group_spravochnay_inform;
	//QGroupBox group_komplektov;


	QByteArray pprch_main_key_data;
	int pprch_main_key_num;

	QByteArray pprch_res_key_data;
	int pprch_main_res_num;

	QByteArray shps_main_key_data;
	int shps_main_key_ps;

	QByteArray shps_res_key_data;
	int shps_res_key_ps;

	int n_ka;

	RPC_omnibus_SLOT_Thread slot_thr;
	RPC_omnibus_SIGNAL_Thread signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;


};

#endif // AOS_H

