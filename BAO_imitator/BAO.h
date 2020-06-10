#ifndef BAO_H
#define BAO_H

#include <QDialog>
#include <QMap>
#include <QXmlStreamReader>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QMainWindow>

#include "../OMNIBUSBOX/omnibus_rpc.h"
#include "../buses_imitator/mku_bus_rpc.h"
#include "../buses_imitator/power_bus_rpc.h"

union MKOCommandWord
{
	MKOCommandWord(quint16 raw_word)
	{
		this->command_word = raw_word;
	}

	quint16 command_word;

	struct {
		quint16 words_count : 5,
			subaddress : 5,
			transaction_direction : 1,
			address : 5;
	};
};

union WFC_Word			//ШПК
{
	quint16 com_word[7];
	struct
	{
		quint16					// word 0
			s_main_blk_on : 1,    // Включен основной БЛК ШПК
			s_res_blk_on : 1,   // Включен резервный БЛК ШПК
			s_main_uf : 1,	//Подключено основное устройство функционирования ШПК
			s_res_uf : 1,		//Подключено резервное устройство функционирования ШПК
			s_main_brv : 1,	//Подключен основной БРВ ШПК
			s_res_brv : 1,	//Подключен резервный БРВ ШПК
			s_main_term_on : 1,	//Включен основной термостабилизатор УФ ШПК
			s_res_term_on : 1,	//Включен резервный термостабилизатор УФ ШПК
			: 2,			//Резерв
			s_bp : 1,	//Исправен блок питания гетерного насоса ШПК
			s_tt : 1,	//Включен режим тренировки трубки ШПК
			s_ts_uf_on : 1, //Включен ТС УФ ШПК
			: 1,
			s_pneo : 1,	//Исправен преобразователь напряжения электронной оптики ШПК
			s_on : 1;	//Включен ШПК
		quint16					// word 1
			s_volt_trans_blk_work : 1,	//Исправен преобразователь напряжения БЛК ШПК
			s_preampl_work : 1,	//Исправен предварительный усилитель ШПК
			s_analog_proc_work : 1,	//Исправно устройство аналоговой обработки ШПК
			s_adc_work : 1,	//Исправен аналогово-цифровой преобразователь ШПК
			s_deconv_work : 1,	//Исправен блок разверток ШПК
			s_rastr_cont_work : 1,	//Исправен блок управления растром ШПК
			s_mod_trig_volt_formed : 1,	//Отпирающее напряжение модулятора сформировано в БРВ ШПК
			: 1,
			s_preampl_volt_conv_work : 1,	//Исправен преобразователь напряжения предварительного усилителя ШПК
			s_rastr_cont_volt_conv_work : 1,	//Исправен преобразователь напряжения блока управления растром ШПК
			s_out_stage_volt_conv_work : 1,	//Исправен преобразователь напряжения блока выходных каскадов ШПК
			s_foc_cur_stab_volt_work : 1,	//Исправен преобразователь напряжения стабилизатора тока фокусировки ШПК
			s_glow_volt_conv_work : 1,	//Исправен преобразователь напряжения накала ШПК
			s_tube_power_sup_work : 1,	//Исправен блок питания трубки ШПК
			: 2;
		quint16					// word 2
			s_setup_mode_on : 1,	//Включен режим настройки (+20) ШПК
			: 1,
			s_fos_m_ok : 1,	//ФОС ШПК подключена к основным блокам УФ и БРВ
			s_fos_r_ok : 1,	//ФОС ШПК подключена к резервным блокам УФ и БРВ
			: 11,
			s_res_enddev : 1;	//Проведен сброс оконечного устройства ШПК
		quint16					// word 3
			: 13,
			s_temp_uf_ok : 1,	//Температура УФ ШПК в норме
			: 2;
		quint16					// word 4
			s_current : 8,	//Ток сетки ШПК
			: 8;
		quint16					// word 5
			s_volt_mod_rast_prep : 8,	//Напряжение модулятора в растре подготовки ШПК
			: 8;
		quint16					// word 6
			s_volt_mod_rast_read : 8,	//Напряжение модулятора в растре считывания  ШПК
			: 8;
	};
};
union NFC_Word	//УПК
{
	quint16 com_word[7];
	struct
	{
		quint16					// word 0
			u_main_blk_on : 1,    // Включен основной БЛК УПК
			u_res_blk_on : 1,   // Включен резервный БЛК УПК
			u_main_uf : 1,	//Подключено основное устройство функционирования УПК
			u_res_uf : 1,		//Подключено резервное устройство функционирования УПК
			u_main_brv : 1,	//Подключен основной БРВ УПК
			u_res_brv : 1,	//Подключен резервный БРВ УПК
			u_main_term_on : 1,	//Включен основной термостабилизатор УФ УПК
			u_res_term_on : 1,	//Включен резервный термостабилизатор УФ УПК
			: 2,			//Резерв
			u_bp : 1,	//Исправен блок питания гетерного насоса УПК
			u_tt : 1,	//Включен режим тренировки трубки УПК
			u_ts_uf_on : 1, //Включен ТС УФ УПК
			u_frag_mode_on : 1,	//Включен режим фрагментарного растра УПК
			u_pneo : 1,	//Исправен преобразователь напряжения электронной оптики УПК
			u_on : 1;	//Включен УПК
		quint16					// word 1
			u_volt_trans_blk_work : 1,	//Исправен преобразователь напряжения БЛК УПК
			u_preampl_work : 1,	//Исправен предварительный усилитель УПК
			u_analog_proc_work : 1,	//Исправно устройство аналоговой обработки УПК
			u_adc_work : 1,	//Исправен аналогово-цифровой преобразователь УПК
			u_deconv_work : 1,	//Исправен блок разверток УПК
			u_rastr_cont_work : 1,	//Исправен блок управления растром УПК
			u_mod_trig_volt_formed : 1,	//Отпирающее напряжение модулятора сформировано в БРВ УПК
			: 1,
			u_preampl_volt_conv_work : 1,	//Исправен преобразователь напряжения предварительного усилителя УПК
			u_rastr_cont_volt_conv_work : 1,	//Исправен преобразователь напряжения блока управления растром УПК
			u_out_stage_volt_conv_work : 1,	//Исправен преобразователь напряжения блока выходных каскадов УПК
			u_foc_cur_stab_volt_work : 1,	//Исправен преобразователь напряжения стабилизатора тока фокусировки УПК
			u_glow_volt_conv_work : 1,	//Исправен преобразователь напряжения накала УПК
			u_tube_volt_conv_work : 1,	//Исправен преобразователь напряжения трубки УПК
			: 2;
		quint16					// word 2
			u_setup_mode_on : 1,	//Включен режим настройки (+20) УПК
			: 1,
			u_fos_m_ok : 1,	//ФОС УПК подключена к основным блокам УФ и БРВ
			u_fos_r_ok : 1,	//ФОС УПК подключена к резервным блокам УФ и БРВ
			: 11,
			u_res_enddev : 1;	//Проведен сброс оконечного устройства УПК
		quint16					// word 3
			: 13,
			u_temp_uf_ok : 1,	//Температура УФ УПК в норме
			: 2;
		quint16					// word 4
			u_current : 8,	//Ток сетки УПК
			: 8;
		quint16					// word 5
			u_volt_mod_rast_prep : 8,	//Напряжение модулятора в растре подготовки УПК
			: 8;
		quint16					// word 6
			u_volt_mod_rast_read : 8,	//Напряжение модулятора в растре считывания  УПК
			: 8;
	};
};

union OS_Word	//ОС
{
	quint16 com_word[13];
	struct
	{
		quint16					// word 0
			szk_m_op : 1,	//Открыты СЗК (основной датчик)
			szk_m_cl : 1,	//Закрыты СЗК (основной датчик)
			szk_r_op : 1,	//Открыты СЗК (резервный датчик)
			szk_r_cl : 1,	//Закрыты СЗК (резервный датчик)
			arr_rem_sen1 : 1,	//1-ый датчик снятия с арретира
			arr_rem_sen2 : 1,	//2-ый датчик снятия с арретира
			: 1,
			u_m_foc_mot_on : 1,	//Подключен основной электропривод фокусировки УПК
			s_m_foc_mot_on : 1,	//Подключен основной электропривод фокусировки ШПК
			arr_inst_sen1 : 1,	//1-ый датчик установки на арретир
			arr_inst_sen2 : 1,	//2-ый датчик установки на арретир
			s_usd_m_mot_on : 1,	//Подключен основной электропривод УСД ШПК
			s_m_ii_fk_on : 1,	//Подключен основной ИИ ФК ШПК
			u_m_ii_fk_on : 1,	//Подключен основной ИИ ФК УПК
			: 2;
		quint16					// word 1
			m_set_drive_sen : 1,	//Подключен основной комплект датчиков приводов
			: 5,
			s_lightfilt : 1,	//Подключен 1 светофильтр ШПК
			u_lightfilt : 1,	//Подключен 1 светофильтр УПК
			: 8;
		quint16					// word 2
			s_inst_diaphr1 : 1,	//Установлена 1 диафрагма ШПК
			s_inst_diaphr2 : 1,	//Установлена 2 диафрагма ШПК
			s_inst_diaphr3 : 1,	//Установлена 3 диафрагма ШПК
			s_step_mov : 1,	//Шаговое перемещение фокусирующего элемента механизма фокусировки ШПК произведено
			u_step_mov : 1,	//Шаговое перемещение фокусирующего элемента механизма фокусировки УПК произведено
			: 11;
		quint16					// word 3
			s_mech_foc_minus : 1,	//Механизмы фокусировки ШПК находятся в крайнем  положении (-)
			s_mech_foc_plus : 1,	//Механизмы фокусировки ШПК находятся в крайнем положении (+)
			s_foc_sen_step : 1,	//Датчик фокусировки ШПК шаг
			: 1,
			s_first_sen_turr : 1,	//1-ый датчик турели ШПК
			s_second_sen_turr : 1,	//2-ой датчик турели ШПК
			s_first_sen_diaphr : 1,	//1-ый датчик диафрагмы ШПК
			s_second_sen_diaphr : 1,	//2-ой датчик диафрагмы ШПК
			u_mech_foc_minus : 1,	//Механизмы фокусировки УПК находятся в крайнем положении(-)
			u_mech_foc_plus : 1,	//Механизмы фокусировки УПК находятся в крайнем положении (+)
			u_foc_sen_step : 1,	//Датчик фокусировки УПК шаг
			: 1,
			u_first_sen_turr : 1,	//1-ый датчик турели УПК
			u_second_sen_turr : 1,	//2-ой датчик турели УПК
			: 2;
		quint16					// word 4
			cur_retarg_ang_y : 16;	//Текущее положение угла перенацеливания  по оси Y
		quint16					// word 5
			cur_retarg_ang_z : 16;	//Текущее положение угла перенацеливания  по  оси Z
		quint16					// word 6
			u_pos_foc_el : 8,	//Положение фокусирующего элемента УПК
			s_pos_foc_el : 8;	//Положение фокусирующего элемента ШПК
		quint16					// word 7
			os_main_blk_on : 1,    // Включен основной БЛК ОС
			os_res_blk_on : 1,   // Включен резервный БЛК ОС
			os_gymbal_mech_rem_arr : 1,	//Механизмы карданова подвеса сняты с арретира
			os_szk_m_mot_on : 1,	//Подключен основной электропривод СЗК
			s_m_mot_turr_on : 1,	//Подключен основной электропривод турели ШПК
			u_m_mot_turr_on : 1,	//Подключен основной электропривод турели УПК
			szk_open : 1,	//Открыты СЗК
			szk_close : 1,	//Закрыты СЗК
			s_inst_lightfilt : 1,	//Установлен светофильтр ШПК
			s_inst_mirr : 1,	//Установлено наклонное зеркало ШПК
			u_inst_lightfilt : 1,	//Установлен светофильтр УПК
			u_inst_mirr : 1,	//Установлено наклонное зеркало УПК
			s_ii_fk_on : 1,	//Включен ИИ ФК ШПК
			u_ii_fk_on : 1,	//Включен ИИ ФК УПК
			: 1,
			os_res_enddev : 1;	//Сброс оконечного устройства БУ ОС
		quint16					// word 8
			: 1,
			sys_retarg_on : 1,	//Включена система перенацеливания
			spn_y_work : 1,	//Исправна СПН по оси Y
			spn_z_work : 1,	//Исправна СПН по оси Z
			mode_retarg_y : 1,	//Режим перенацеливания по  оси Y
			mode_retarg_z : 1,	//Режим перенацеливания по оси Z
			: 6,
			m_sys_retarg_y_on : 1,	//Подключена основная система перенацеливания  по оси Y
			m_sys_retarg_z_on : 1,	//Подключена основная система перенацеливания  по оси Z
			m_sen_ang_y : 1,	//Подключен основной датчик угла  по оси Y
			m_sen_ang_z : 1;	//Подключен основной датчик угла  по оси Z
		quint16					// word 9
			: 8,
			r_5v_y_work : 1,	//5В резервное по оси Y исправно
			m_5v_y_work : 1,	//5В основное по оси Y исправно
			cur_lim_y : 1,	//Ограничение тока по оси Y
			r_ch_y_on : 1,	//Включен резервный канал по оси Y
			m_ch_y_on : 1,	//Включен основной канал по оси Y
			r_sen_y_on : 1,	//Включен резервный датчик по оси Y
			m_sen_y_on : 1,	//Включен основной датчик по оси Y
			sen_ang_y_work : 1;	//Датчик угла исправен  по   оси Y
		quint16					// word 10
			tem_red_y : 8,	//Температура редуктора по оси Y	
			tem_eng_y : 8;	//Температура двигателя  по оси Y
		quint16					// word 11
			: 8,
			r_5v_z_work : 1,	//5В резервное по оси Z исправно
			m_5v_z_work : 1,	//5В основное по оси Z исправно
			cur_lim_z : 1,	//Ограничение тока по оси Z
			r_ch_z_on : 1,	//Включен резервный канал по оси Z
			m_ch_z_on : 1,	//Включен основной канал по оси Z
			r_sen_z_on : 1,	//Включен резервный датчик по оси Z
			m_sen_z_on : 1,	//Включен основной датчик по оси Z
			sen_ang_z_work : 1;	//Датчик угла исправен  по   оси Z
		quint16					// word 12
			tem_red_z : 8,	//Температура редуктора по оси Z
			tem_eng_z : 8;	//Температура двигателя  по оси Z
	};
};
union ThMS_Word1	//СОТР (thermal management system) подадрес 1
{
	quint16 com_word[30];
	struct
	{
		quint16					// word 0
			err_exch_sgo_mc : 1,	//Ошибка обмена с СГО (осн.контроллер)
			err_exch_sgo_rc : 1,	//Ошибка обмена с СГО (рез.контроллер)
			tem_cool_bel320_mc : 1,	//Температура охладителя ниже 47°С (320К) (осн. контр.)
			tem_cool_bel320_rc : 1,	//Температура охладителя ниже 47°С (320К) (рез. контр.)
			tem_com_bel320_mc : 1,	//Температура компрессора ниже 47°С  (320К) (осн. контр.)
			tem_com_bel320_rc : 1,	//Температура компрессора ниже 47°С  (320К) (рез. контр.)
			tem_cold83_mc_ok : 1,	//Температура стыка хладопроводов основновного уровня в норме-(83±2)К (осн.контр.)
			tem_cold83_rc_ok : 1,	//Температура стыка хладопроводов резервного уровня в норме-(83±2) К(рез.контр.)
			: 6,
			depress_cool_unit_mc : 1,	//Разгерметизирован блок охлаждения (основной контр. СГО)
			depress_cool_unit_rc : 1;	//Разгерметизирован блок охлаждения (резервный контр. СГО)
		quint16					// word 1
			temp_sen_TS1 : 11,	//Температурный датчик ДТ1
			: 5;
		quint16					// word 2
			temp_sen_TS2 : 11,	//Температурный датчик ДТ2
			: 5;
		quint16					// word 3
			temp_sen_TS3 : 11,	//Температурный датчик ДТ3
			: 5;
		quint16					// word 4
			temp_sen_TS4 : 11,	//Температурный датчик ДТ4
			: 5;
		quint16					// word 5
			temp_sen_TS5 : 11,	//Температурный датчик ДТ5
			: 5;
		quint16					// word 6
			temp_sen_TS6 : 11,	//Температурный датчик ДТ6
			: 5;
		quint16					// word 7
			temp_sen_TS7 : 11,	//Температурный датчик ДТ7
			: 5;
		quint16					// word 8
			temp_sen_TS8 : 11,	//Температурный датчик ДТ8
			: 5;
		quint16					// word 9
			temp_sen_TS9 : 11,	//Температурный датчик ДТ9
			: 5;
		quint16					// word 10
			temp_sen_TS11 : 11,	//Температурный датчик ДТ11
			: 5;
		quint16					// word 11
			temp_sen_TS12 : 11,	//Температурный датчик ДТ12
			: 5;
		quint16					// word 12
			temp_sen_TSCB : 11,	//Температурный датчик ДТ-БУ
			: 5;
		quint16					// word 13
			temp_sen_RT1 : 11,	//Температурный датчик RТ1
			: 5;
		quint16					// word 14
			temp_sen_RT2 : 11,	//Температурный датчик RТ2
			: 5;
		quint16					// word 15
			temp_sen_RT3 : 11,	//Температурный датчик RТ3
			: 5;
		quint16					// word 16
			press_sen_PS1 : 11,	//Датчик давления ДД1
			: 5;
		quint16					// word 17
			temp_sen_RT4 : 11,	//Температурный датчик RТ4
			: 5;
		quint16					// word 18
			press_sen_PS2 : 11,	//Датчик давления ДД2
			: 5;
		quint16					// word 19
			prog_self_test_sgo_mc : 1,	//Программный самоконтроль СГО (осн. контроллер)
			prog_self_test_sgo_rc : 1,	//Программный самоконтроль СГО (рез. контроллер)
			m_mcs_on : 1,	//Включен основная МКС
			r_mcs_on : 1,	//Включен резервная  МКС
			: 1,	//	ADD!!!!!!!!!!!!!!
			: 1,	//	ADD!!!!!!!!!!!!!!
			tem_cold81_mc_ok : 1,	//Температура стыка хлодопроводов основного уровня в норме ( минус 192±2°С) (81±2)°К (осн. контроллер)
			tem_cold81_rc_ok : 1,	//Температура стыка хлодопроводов основного уровня в норме ( минус 192±2°С) (81±2)°К (осн. контроллер) (РЕЗЕРВНЫЙ, СКОРЕЕ ВСЕГО. ПРОВЕРИТЬ!!!!!!!!!!!!!!)
			: 1,	//	ADD!!!!!!!!!!!!!!
			res_enddev_bks : 1,	//Сброс оконечного устройства БКиС
			internal_interface_work : 1,	//Исправен внутренний интерфейс
			r_sku_sdc_on : 1,	//Включена резервная СКУ СГО
			m_sku_sdc_on : 1,	//Включена основная СКУ СГО
			m_internal_interface_work : 1,	//Работает основной внутренний интерфейс
			r_blk_sotr_on : 1,	//Включен резервный БЛК СОТР
			m_blk_sotr_on : 1;	//Включен основной БЛК СОТР	(2704)
		quint16					// word 20
			heater_on_1 : 1,	//Включен нагреватель 1 (приборная панель АII)
			heater_on_2 : 1,	//Включен нагреватель 2 (приборная панель АII)
			heater_on_3 : 1,	//Включен нагреватель 3 (экран ОС)
			heater_on_4 : 1,	//Включен нагреватель 4 (экран ОС)
			heater_on_5 : 1,	//Включен нагреватель 5 (приборная панель АII)
			heater_on_6 : 1,	//Включен нагреватель 6 (приборная панель АII)
			heater_on_7 : 1,	//Включен нагреватель 7 (экран ОС)
			heater_on_8 : 1,	//Включен нагреватель 8 (экран ОС)
			heater_on_9 : 1,	//Включен нагреватель 9 (приборная панель АII)
			heater_on_10 : 1,	//Включен нагреватель 10 (приборная панель АII)
			heater_on_11 : 1,	//Включен нагреватель 11 (приборная панель АII)
			heater_on_12 : 1,	//Включен нагреватель 12 (приборная панель АII)
			: 4;
		quint16					// word 21
			heater_on_13 : 1,	//Включен нагреватель 13 (экран ОС)
			heater_on_14 : 1,	//Включен нагреватель 14 (экран ОС)
			heater_on_15 : 1,	//Включен нагреватель 15 (экран ОС)
			heater_on_16 : 1,	//Включен нагреватель 16 (Бленда внутренняя)
			heater_on_17 : 1,	//Включен нагреватель 17 (экран ОС)
			: 11;
		quint16					// word 22
			heater_on_18 : 1,	//Включен нагреватель 18 (приборная панель АIV)
			heater_on_19 : 1,	//Включен нагреватель 19 (приборная панель АIV)
			heater_on_20 : 1,	//Включен нагреватель 20 (теплопроводы радиатора ГТА)
			heater_on_21 : 1,	//Включен нагреватель 21 (теплопроводы радиатора ГТА)
			heater_on_22 : 1,	//Включен нагреватель 22 (приборная панель АIV)
			heater_on_23 : 1,	//Включен нагреватель 23 (приборная панель АIV)
			heater_on_24 : 1,	//Включен нагреватель 24 (ОС УПК)
			heater_on_25 : 1,	//Включен нагреватель 25 (ОС УПК)
			heater_on_26 : 1,	//Включен нагреватель 26 (приборная панель АIV)
			heater_on_27 : 1,	//Включен нагреватель 27 (приборная панель АIV)
			heater_on_28 : 1,	//Включен нагреватель 28 (приборная панель АIV)
			heater_on_29 : 1,	//Включен нагреватель 29 (приборная панель АIV)
			: 4;
		quint16					// word 23
			heater_on_30 : 1,	//Включен нагреватель 30 (теплопровод радиатора ОС)
			heater_on_31 : 1,	//Включен нагреватель 31 (теплопровод радиатора ОС)
			heater_on_32 : 1,	//Включен нагреватель 32 (БВ)
			heater_rel_on_33 : 1,	//Включен нагреватель релейный 33
			heater_rel_on_34 : 1,	//Включен нагреватель релейный 34
			: 11;
		quint16					// word 24
			: 16;
		quint16					// word 25
			: 16;
		quint16					// word 26
			: 16;
		quint16					// word 27
			: 16;
		quint16					// word 28
			temp_sen_an_32 : 12,	//Температурный датчик 32 аналоговый (ОС УПК)
			: 4;
		quint16					// word 29
			temp_sen_an_31 : 12,	//Температурный датчик 31 аналоговый (ОС УПК)
			: 4;
	};
};

union ThMS_Word2	//СОТР (thermal management system) подадрес 10
{
	quint16 com_word[18];
	struct
	{
		quint16					// word 0
			temp_sen_TS1 : 10,	//Температурный датчик ДТ1
			: 6;
		quint16					// word 1
			temp_sen_TS2 : 10,	//Температурный датчик ДТ2
			: 6;
		quint16					// word 2
			temp_sen_TS3 : 10,	//Температурный датчик ДТ3
			: 6;
		quint16					// word 3
			temp_sen_TS4 : 10,	//Температурный датчик ДТ4
			: 6;
		quint16					// word 4
			temp_sen_TS5 : 10,	//Температурный датчик ДТ5
			: 6;
		quint16					// word 5
			temp_sen_TS6 : 10,	//Температурный датчик ДТ6
			: 6;
		quint16					// word 6
			temp_sen_TS7 : 10,	//Температурный датчик ДТ7
			: 6;
		quint16					// word 7
			temp_sen_TS8 : 10,	//Температурный датчик ДТ8
			: 6;
		quint16					// word 8
			temp_sen_TS9 : 10,	//Температурный датчик ДТ9
			: 6;
		quint16					// word 9
			temp_sen_TS11 : 10,	//Температурный датчик ДТ11
			: 6;
		quint16					// word 10
			temp_sen_TS12 : 10,	//Температурный датчик ДТ12
			: 6;
		quint16					// word 11
			temp_sen_TSCB : 10,	//Температурный датчик ДТ-БУ
			: 6;
		quint16					// word 12
			temp_sen_RT1 : 10,	//Температурный датчик RТ1
			: 6;
		quint16					// word 13
			temp_sen_RT2 : 10,	//Температурный датчик RТ2
			: 6;
		quint16					// word 14
			temp_sen_RT3 : 10,	//Температурный датчик RТ3
			: 6;
		quint16					// word 15
			press_sen_PS1 : 10,	//Датчик давления ДД1
			: 6;
		quint16					// word 16
			temp_sen_RT4 : 10,	//Температурный датчик RТ4
			: 6;
		quint16					// word 17
			press_sen_PS2 : 10,	//Датчик давления ДД2
			: 6;
	};
};

union ThMS_Word3	//СОТР (thermal management system) подадрес 18
{
	quint16 com_word[31];
	struct
	{
		quint16					// word 0
			temp_sen_an_1 : 12,	//Температурный датчик 1 аналоговый (Тп ГКМ)
			: 4;
		quint16					// word 1
			temp_sen_an_2 : 12,	//Температурный датчик 2 аналоговый (Тп ГКМ)
			: 4;
		quint16					// word 2
			temp_sen_an_3 : 12,	//Температурный датчик 3 аналоговый (Тп ГКМ)
			: 4;
		quint16					// word 3
			temp_sen_an_4 : 12,	//Температурный датчик 4 аналоговый (Тп ГКМ)
			: 4;
		quint16					// word 4
			temp_sen_an_5 : 12,	//Температурный датчик 5 аналоговый (Тп ГКМ)
			: 4;
		quint16					// word 5
			temp_sen_an_39 : 12,	//Температурный датчик 39 аналоговый (приборная панель АIV)
			: 4;
		quint16					// word 6
			temp_sen_an_7 : 12,	//Температурный датчик 7 аналоговый (БАОВ)
			: 4;
		quint16					// word 7
			temp_sen_an_8 : 12,	//Температурный датчик 8 аналоговый (БТ УПК)
			: 4;
		quint16					// word 8
			temp_sen_an_40 : 12,	//Температурный датчик 40 аналоговый (приборная панель АIV)
			: 4;
		quint16					// word 9
			temp_sen_an_10 : 12,	//Температурный датчик 10 аналоговый (радиатор ГТА)
			: 4;
		quint16					// word 10
			temp_sen_an_12 : 12,	//Температурный датчик 12 аналоговый (радиатор ОС)
			: 4;
		quint16					// word 11
			temp_sen_an_43 : 12,	//Температурный датчик 43 аналоговый (ФОП)
			: 4;
		quint16					// word 12
			: 16;
		quint16					// word 13
			temp_sen_an_45 : 12,	//Температурный датчик 45 аналоговый (радиатор ОС)
			: 4;
		quint16					// word 14
			temp_sen_an_15 : 12,	//Температурный датчик 15 аналоговый (экран)
			: 4;
		quint16					// word 15
			temp_sen_an_48 : 12,	//Температурный датчик 48 аналоговый (БВ)
			: 4;
		quint16					// word 16
			temp_sen_an_21 : 12,	//Температурный датчик  аналоговый 21 (БН)
			: 4;
		quint16					// word 17
			temp_sen_an_50 : 12,	//Температурный датчик 50 аналоговый (БВ)
			: 4;
		quint16					// word 18
			temp_sen_an_53 : 12,	//Температурный датчик 53 аналоговый (БВ)
			: 4;
		quint16					// word 19
			temp_sen_an_52 : 12,	//Температурный датчик 52 аналоговый (БВ)
			: 4;
		quint16					// word 20
			temp_sen_an_51 : 12,	//Температурный датчик 51 аналоговый (БВ)
			: 4;
		quint16					// word 21
			temp_sen_an_22 : 12,	//Температурный датчик  аналоговый 22 БТ ШПК (ФОС)
			: 4;
		quint16					// word 22
			temp_sen_an_25 : 12,	//Температурный датчик 25 аналоговый (ОС ШПК)
			: 4;
		quint16					// word 23
			temp_sen_an_26 : 12,	//Температурный датчик 26 аналоговый (ОС ШПК)
			: 4;
		quint16					// word 24
			temp_sen_an_24 : 12,	//Температурный датчик 24 аналоговый (ОС ШПК)
			: 4;
		quint16					// word 25
			temp_sen_an_57 : 12,	//Температурный датчик 57 аналоговый (приборная панель АII)
			: 4;		
		quint16					// word 26
			temp_sen_an_27 : 12,	//Температурный датчик 27 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 27
			temp_sen_an_61 : 12,	//Температурный датчик 61 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 28
			temp_sen_an_64 : 12,	//Температурный датчик 64 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 29
			temp_sen_an_28 : 12,	//Температурный датчик 28 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 30
			temp_sen_an_63 : 12,	//Температурный датчик 63 аналоговый (ОСвкп УПК)
			: 4;
	};
};

union ThMS_Word4	//СОТР (thermal management system) подадрес 19
{
	quint16 com_word[31];
	struct
	{
		quint16					// word 0
			temp_sen_an_33 : 12,	//Температурный датчик 33 аналоговый (Тп ГКП)
			: 4;
		quint16					// word 1
			temp_sen_an_34 : 12,	//Температурный датчик 34 аналоговый (Тп ГКП)
			: 4;
		quint16					// word 2
			temp_sen_an_35 : 12,	//Температурный датчик 35 аналоговый (Тп ГКП)
			: 4;
		quint16					// word 3
			: 16;
		quint16					// word 4
			temp_sen_an_37 : 12,	//Температурный датчик 37 аналоговый (приборная панель АIV)
			: 4;
		quint16					// word 5
			temp_sen_an_6 : 12,	//Температурный датчик 6 аналоговый (БАОВ)
			: 4;
		quint16					// word 6
			temp_sen_an_38 : 12,	//Температурный датчик 38 аналоговый (приборная панель АIV)
			: 4;
		quint16					// word 7
			temp_sen_an_41 : 12,	//Температурный датчик 41 аналоговый (приборная панель АIV)
			: 4;
		quint16					// word 8
			temp_sen_an_9 : 12,	//Температурный датчик 9 аналоговый (БТ УПК)
			: 4;
		quint16					// word 9
			temp_sen_an_42 : 12,	//Температурный датчик 42 аналоговый (ФОП)
			: 4;
		quint16					// word 10
			temp_sen_an_13 : 12,	//Температурный датчик 13 аналоговый (экран)
			: 4;
		quint16					// word 11
			temp_sen_an_14 : 12,	//Температурный датчик 14 аналоговый (экран)
			: 4;
		quint16					// word 12
			temp_sen_an_44 : 12,	//Температурный датчик 44 аналоговый (ФОП)
			: 4;		
		quint16					// word 13
			temp_sen_an_11 : 12,	//Температурный датчик 11 аналоговый (радиатор ОС)
			: 4;
		quint16					// word 14
			: 16;
		quint16					// word 15
			temp_sen_an_20 : 12,	//Температурный датчик  аналоговый 20 (БН)
			: 4;
		quint16					// word 16
			temp_sen_an_17 : 12,	//Температурный датчик  аналоговый 17 (БН)
			: 4;
		quint16					// word 17
			temp_sen_an_16 : 12,	//Температурный датчик  аналоговый 16 (НПШ)
			: 4;
		quint16					// word 18
			temp_sen_an_19 : 12,	//Температурный датчик  аналоговый 19 (БН)
			: 4;
		quint16					// word 19
			temp_sen_an_18 : 12,	//Температурный датчик  аналоговый 18 (БН)
			: 4;
		quint16					// word 20
			temp_sen_an_49 : 12,	//Температурный датчик 49 аналоговый (БВ)
			: 4;
		quint16					// word 21
			temp_sen_an_54 : 12,	//Температурный датчик 54 аналоговый (ОС ШПК)
			: 4;
		quint16					// word 22
			temp_sen_an_55 : 12,	//Температурный датчик 55 аналоговый (приборная панель АII)
			: 4;
		quint16					// word 23
			temp_sen_an_56 : 12,	//Температурный датчик 56 аналоговый (приборная панель АII)
			: 4;		
		quint16					// word 24
			temp_sen_an_23 : 12,	//Температурный датчик аналоговый 23 БТ ШПК (ФОС)
			: 4;
		quint16					// word 25
			temp_sen_an_58 : 12,	//Температурный датчик 58 аналоговый (приборная панель АII)
			: 4;
		quint16					// word 26
			temp_sen_an_59 : 12,	//Температурный датчик 59 аналоговый (приборная панель АII)
			: 4;
		quint16					// word 27
			temp_sen_an_60 : 12,	//Температурный датчик 60 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 28
			temp_sen_an_62 : 12,	//Температурный датчик 62 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 29
			temp_sen_an_30 : 12,	//Температурный датчик 30 аналоговый (ОСвкп УПК)
			: 4;
		quint16					// word 30
			temp_sen_an_29 : 12,	//Температурный датчик 29 аналоговый (ОСвкп УПК)
			: 4;		
	};
};

class BAO_widg : public QWidget
{
    Q_OBJECT

public:
//	explicit LKA05_widg(QWidget *parent = 0);
	BAO_widg();
	~BAO_widg();	
private:
	/// -- Main window
	QMainWindow* main_widg;

	QList<QPushButton*> mainSetButton;
	QList<QPushButton*> reserveSetButton;
	QPushButton* startButton;
	
	//QList<QPushButton*> main_MVKU;
	//QList<QPushButton*> reserve_MVKU;
	//QList<QPushButton*> main_MVMK;
	//QList<QPushButton*> reserve_MVMK;

	QGroupBox *WC_gb;
	QGroupBox *NC_gb;
	QGroupBox *OS_gb;
	QGroupBox *SOTR_gb;
	
	RPC_omnibus_SLOT_Thread omnibus_slot_thr;
	RPC_omnibus_SIGNAL_Thread omnibus_signal_thr;

	RPC_mku_bus_SLOT_Thread mku_slot_thr;
	RPC_mku_bus_SIGNAL_Thread mku_signal_thr;

	RPC_power_bus_SLOT_Thread power_slot_thr;
	RPC_power_bus_SIGNAL_Thread power_signal_thr;

	const int MKO = 0;
	QVariantList tmp_list;
	WFC_Word word_for_WFC;
	NFC_Word word_for_NFC;
	OS_Word word_for_OS;
	ThMS_Word1 word_for_ThMS1;
	ThMS_Word2 word_for_ThMS2;
	ThMS_Word3 word_for_ThMS3;
	ThMS_Word4 word_for_ThMS4;

	//int adr;
	void clear_WFC_Word(WFC_Word &com_word);
	void clear_NFC_Word(NFC_Word &com_word);
	void clear_OS_Word(OS_Word &com_word);
	void clear_ThMS_Word1(ThMS_Word1 &com_word);
	void clear_ThMS_Word2(ThMS_Word2 &com_word);
	void clear_ThMS_Word3(ThMS_Word3 &com_word);
	void clear_ThMS_Word4(ThMS_Word4 &com_word);

	void update_WFC_Word(WFC_Word com_word);
	void update_NFC_Word(NFC_Word com_word);
	void update_OS_Word(OS_Word com_word);
	void update_ThMS_Word1(ThMS_Word1 com_word);
	void update_ThMS_Word2(ThMS_Word2 com_word);
	void update_ThMS_Word3(ThMS_Word3 com_word);
	void update_ThMS_Word4(ThMS_Word4 com_word);

	void answer_adrs_14(MKOCommandWord cwd, QVariantList words);
	void answer_adrs_15(MKOCommandWord cwd, QVariantList words);
	void answer_adrs_16(MKOCommandWord cwd, QVariantList words);
	void answer_adrs_17(MKOCommandWord cwd, QVariantList words);

	bool power_on = false;
	bool start_on = false;
	bool WFC_on = false;
	bool NFC_on = false;
	bool OS_on = false;
	bool ThMS_on = false;
public slots:
	void get_power(double volt);
	void new_message(QVariant dt, int mko, int line, int cwd, QVariantList words, int os);
	void new_mk(int mshm, int pshm, int length_m, int length_p, double u_m, double u_p, int dt, int line_m, int line_p);

};

#endif // LKA05_H
