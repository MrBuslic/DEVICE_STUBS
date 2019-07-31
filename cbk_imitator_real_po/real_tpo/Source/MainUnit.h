
#ifndef MainUnitH
#define MainUnitH
//================================================================================

#define OPO_V031                        // флаг сборки проекта под версию ОПО v031.x.x
// #define OPO_V033                        // флаг сборки проекта под версию ОПО v033.x.x

#define SPO_ERTI                        // флаг сборки проекта под комплект аппаратуры ЕРТИ
// #define SPO_KOI                         // флаг сборки проекта под комплект аппаратуры КОИ

// #define SPO_BKOI                     // флаг сборки проекта для поддержки проверок РМ БТО

#define BALOI_V11                       // флаг сборки проекта для поддержки функций БАЛОИ
//================================================================================

#if !defined (OPO_V031) && !defined (OPO_V033)
  #error Не определена версия ОПО ЦБК!
#endif
//================================================================================

// выдача сообщений программы по ПКО
#ifdef __ALLOW_PRINT__
#define MSGS(fmt, args...) printf(fmt, ##args);
#else
#define MSGS(fmt, ...)
#endif
//================================================================================

/*
// использование страниц БОЗУ для МКО (0..6):
MkoPackA.nPage  = 0;  
MkoPackB.nPage  = 1;  
MkoPackC.nPage  = 2;  
MkoPackX.nPage  = 3;  
Irq10Pack.nPage = 4;
Baloi_MkoPack.nPage = 5;
*/
//================================================================================

/*
Командное слово МКО:
биты 4-0   = число слов для передачи, 0..31 (или код команды), если 0 = 32 слова
биты 9-5   = подадрес ОУ, 0..31 (если = 0 или 31, младшие биты = код команды)
бит 10     = направление передачи (0 = запись в абонент, 1 = чтение с абонента)
биты 11-15 = адрес ОУ, 0..31 (31 = широковещательный) 

Ответное слово МКО:
бит 0      = неисправность ОУ
бит 1      = принято управление интерфейсом
бит 2      = неисправность абонента
бит 3      = абонент занят
бит 4      = принята групповая команда
биты 7-5   = резерв (обычно = 0)
бит 8      = запрос на обслуживание
бит 9      = передача ОС
бит 10     = ошибка в сообщении
биты 15-11 = адрес ОУ (должен совпадать с адресом из командного слова)
*/
//================================================================================

// макрос формирования командного слова МКО
#define MKO_CMD(Addr, SubAddr, Dir, Size) \
  ((WORD) ( ((((UINT) Addr) & 0x1F) << SH_adrOU) | \
            ((((UINT) Dir) & 0x01) << SH_TR) | \
            ((((UINT) SubAddr) & 0x1F) << SH_SA) | \
            (((UINT) Size) & 0x1F) ) )

// макросы выделения полей из командного слова МКО
#define MKO_CMD_SIZE(Cmd) 		((((UINT) Cmd) & 0x1F) ? (((UINT) Cmd) & 0x1F) : (32))	// число слов для передачи
#define MKO_CMD_SUBADDR(Cmd)    	((((UINT) Cmd) >> SH_SA) & 0x1F)      			// подадрес ОУ
#define MKO_CMD_DIR(Cmd)        	((((UINT) Cmd) >> SH_TR) & 0x01)     			// направление передачи
#define MKO_CMD_ADDR(Cmd)       	((((UINT) Cmd) >> SH_adrOU) & 0x1F)     		// адрес ОУ
//================================================================================

// макросы выделения полей из ответного слова МКО
#define MKO_OS_OU_ERROR(Reply)  	(((UINT) Reply) & 1)           		// неисправность ОУ
#define MKO_OS_CTRL(Reply)      	((((UINT) Reply) >> 1) & 1)		// принято управление интерфейсом
#define MKO_OS_AB_ERROR(Reply)  	((((UINT) Reply) >> 2) & 1)		// неисправность абонента
#define MKO_OS_BUSY(Reply)      	((((UINT) Reply) >> 3) & 1)		// абонент занят
#define MKO_OS_GROUP(Reply)     	((((UINT) Reply) >> 4) & 1)		// принята групповая команда
#define MKO_OS_REQUEST(Reply)   	((((UINT) Reply) >> 8) & 1)		// запрос на обслуживание
#define MKO_OS_SEND(Reply)      	((((UINT) Reply) >> 9) & 1)		// передача ОС
#define MKO_OS_MSG_ERROR(Reply) 	((((UINT) Reply) >> 10) & 1)		// ошибка в сообщении
#define MKO_OS_ADDR(Reply)      	((((UINT) Reply) >> 11) & 0x1F)		// адрес ОУ
//================================================================================

#define KPA_ADDR 			1               // адрес ОУ КПА на шине МКО

#define KPA_SUBADDR_CMD         	1               // подадрес КПА для считывания команд
#define KPA_SUBADDR_MAIN        	4               // подадрес КПА для выдачи данных из задачи MainTask()
#define KPA_SUBADDR_KPI         	6               // подадрес КПА для выдачи данных по КПИ
#define KPA_SUBADDR_SHBK		8		// подадрес КПА для считывания данных по СЧБК
#define KPA_SUBADDR_REPLY_IMIT		9		// подадрес КПА для считывания данных по имитируемым квитанциям
#define KPA_SUBADDR_MBK4DATA		10		// подадрес КПА для считывания данных для МБК04
#define KPA_SUBADDR_IRQ         	16              // подадрес КПА для выдачи данных из задачи IrqTask()
#define KPA_SUBADDR_DEBUG       	25		// подадрес КПА для выдачи отладочных сообщений

#define KPA_CMD_LEN             	8               // длина командного пакета КПА, слов

#define KPA_IRQ_MASK            	0x0002          // маска для прерываний от КПА
//================================================================================

#define MKO_RX 				1  		// бит направления на МКО - прием из ОУ
#define MKO_TX 				0  		// бит направления на МКО - передача в ОУ
//================================================================================

#define MKO_DETECT_DELAY 		500		// пауза, мс, при поиске КПА на шине
#define MKO_DETECT_TRY          	5               // число попыток связи с КПА
//================================================================================

#define NO_THREAD               	0		// текущий контекст 
#define MAIN_THREAD			1
#define IRQ_THREAD			2
#define ERR_THREAD			3
//================================================================================

// статусы режимов
#define STATE_IDLE			0		// в ожидании/еще не был включен
#define STATE_WORK			1		// в работе
#define STATE_STOP			2		// в ожидании/остановлен
#define STATE_ABORT			3		// прерван по ошибке

// режимы передачи флага данных в МБК04
#define MBK04_FLAG_NONE 		0		// не передавать флаг
#define MBK04_FLAG_BEFORE_DATA          1		// передавать флаг перед пакетом данных
#define MBK04_FLAG_AFTER_DATA           2		// передавать флаг после пакета данных
//================================================================================

// маски типов сообщений, передаваемых при обработке прерывания 2
#define IRQ2_MSG_DONE			0x01		// разрешение на передачу сообщений о успешном завершении
#define IRQ2_MSG_PROCESS		0x02		// разрешение на передачу промежуточных сообщений 
#define IRQ2_MSG_ERROR			0x04		// разрешение на передачу сообщений об ошибках
//================================================================================

#define ASN_IRQ_TIME_WIN		10		// временное окно для измерения точности прерываний от АСН по метке 1s, +/- мс
//================================================================================

#define MBK07_CMD_TIME 			4000     	// пауза на вкл/откл блоков МБК07
#define MBK07_CMD_CTRL_TIME 		1000     	// пауза на вкл/откл блоков МБК07
//================================================================================

// возможные уведомления/ответы СПО на команды КПА
#define REPLY_SPO_START			0x0001		// Начальный пуск
#define REPLY_CBK_POST_RESULT		0x0002		// Диагностика ЦБК
#define REPLY_PDO_PASS			0x0003		// Выполнение ПДО
#define REPLY_OK_CHANGE			0x0004		// Изменение ОК
#define REPLY_CBK_CONFIG		0x0005		// Конфигурация ЦБК
#define REPLY_SHBK_WR_ERROR		0x8006		// Ошибка записи СЧБК
#define REPLY_732_SHBK_OUT		0x0007		// СЧБК выдано
#define REPLY_732_SHBK_WR_ERROR		0x8008		// Ошибка выдачи СЧБК
#define REPLY_IRQ_ERROR			0x8009		// Ошибка кода прерывания
#define REPLY_IRQ_STATE			0x000A		// Код прерывания
#define REPLY_732_PUPS_WR_ERROR		0x800B		// Ошибка записи 14Р732
#define REPLY_WR_ERROR_MBK04		0x800C		// Ошибка записи квитанций
#define REPLY_732_VM_STATE_ERROR	0x800D		// Ошибка состояния ВЧМ 
#define REPLY_732_ALL_VM_OFF		0x800E		// ВЧМ 14Р732 выключены
#define REPLY_732_VM_STATE		0x000F		// Конфигурация ВЧМ 14Р732
#define REPLY_732_PUPS_STATE_ERROR	0x8010		// Ошибка ПУПС 14Р732
#define REPLY_732_PUPS_OK		0x0011		// Достоверный ПУПС
#define REPLY_SHBK_WR_OK		0x0012		// СЧБК записано
#define REPLY_732_KPI2_OK		0x0013		// Достоверная КПИ2
#define REPLY_732_KPI2_SYNC_ERROR	0x8014		// Ошибка мажорирования КПИ2
#define REPLY_BUF1_WR_ERROR_MBK04	0x8015		// Ошибка записи в буфер 1 МБК04
#define REPLY_BUF2_WR_ERROR_MBK04	0x8016		// Ошибка записи в буфер 2 МБК04
#define REPLY_CMD_UNKNOWN_ERROR		0x8017		// Несуществующая директива
#define REPLY_CMD_EXECUTION		0x0018		// Выполнение директивы
#define REPLY_CMD_DONE			0x0019		// Директива выполнена
#define REPLY_CMD_ERROR    		0x8019		// Директива не выполнена, ошибка
#define REPLY_IRQ_DONE			0x0019		// Обработка прерывания завершена
#define REPLY_OU_WR_ERROR		0x8020		// Ошибка записи в абонент
#define REPLY_OU_RD_ERROR		0x8021		// Ошибка чтения абонента
#define REPLY_RAM_WR_ERROR		0x8022		// Ошибка при выполнении записи в ОЗУ ЦБК
#define REPLY_RAM_DATA			0x0023		// Выдача запрошенной информации ОЗУ ЦБК
#define REPLY_MBK02_OFF_ERROR		0x8024		// Нет выключения МБК02
#define REPLY_MBK04_OFF_ERROR		0x8025		// Нет выключения МБК04
#define REPLY_CONFIG_IS_SET		0x0026		// Установлена заданная директивой конфигурация заданного устройства
#define REPLY_TEST_IS_DONE		0x0027		// Передача результатов тестирования, проведенного по директиве
#define REPLY_VM_ERROR			0x8028		// Наличие ошибок функций ОПО при выполнении директив включения/выключения ВМ
#define REPLY_CBK_MKO_ERROR		0x8029		// Результат выполнения директив замены ведущего ВМ на МКО не соответствует ожидаемому
#define REPLY_OU_XCHG_ERROR		0x802А		// При обмене с абонентом отсутствует ответное слово
#define REPLY_AFC_OFF_ERROR		0x802В		// БУП-Е проводит обмен по МКО1 после выполнения его выключения
#define REPLY_RES_CHANGE_MBK02		0x002С		// Проведено включение/выключение резервируемых единиц МБК02
#define REPLY_RES_CHANGE_732		0x002D		// Проведено включение/выключение резервируемых единиц 14Р732
#define REPLY_RES_CHANGE_ASN		0x002E		// Проведено включение/выключение резервируемых единиц АСН
#define REPLY_RES_CHANGE_BEH		0x002F		// Проведено включение/выключение резервируемых единиц БЭЧ
#define REPLY_RES_CHANGE_733		0x0030		// Проведено включение/выключение резервируемых единиц 14H733
#define REPLY_RES_CHANGE_MBK07M		0x0031		// Проведено включение/выключение резервируемых единиц МБК07М
#define REPLY_RES_CHANGE_BUPE		0x0032		// Проведено включение/выключение резервируемых единиц БУП-Е
#define REPLY_REQEST_FINIQ		0x0033		// Наличие запроса на обслуживание от формирователя импульса начала кадра БЭЧ-Е
#define REPLY_REQEST_FHV		0x0034		// Наличие запроса на обслуживание от формирователя шкалы времени БЭЧ-Е
#define REPLY_DEVICE_READ_ERROR		0x8035		// Ошибка при чтении абонента/устройства
#define REPLY_DEVICE_WRITE_ERROR	0x8036		// Ошибка при записи в абонент/устройство

#define REPLY_VM_ON_ERROR		0x8035		// Ошибка включения ВМ
#define REPLY_VM_OFF_ERROR		0x8036		// Ошибка выключения ВМ

#define REPLY_OK_READ_ERROR             0x8037          // Сбой при считывании ОК с устройства
#define REPLY_OK_SEND_ERROR             0x8038          // Сбой при отправке данных ОК в КПА
#define REPLY_DEVICE_OFF_CTRL_ERROR	0x8039		// Устройство не установлено на контроль
#define REPLY_TIME_ORDER_ERROR		0x803A		// Запрос сделан с нарушением порядка во времени
#define REPLY_732_INFO_ERROR		0x803B		// "Ошибка ИНФ 14Р732", ошибка считывания конфигурации 732

#define REPLY_733_INFO_ERROR		0x803C		// "Ошибка ИНФ 14Р733", ошибка считывания конфигурации 733
#define REPLY_733_PUPS_CLR_ERROR	0x803D		// Ошибка сброса ПУПС 14Р733
#define REPLY_733_WRONG_CONFIG		0x803E		// Неправильная конфигурация 14Р733
#define REPLY_733_VM_CONFIG		0x003F		// Передача конфигурации ВЧМ 14Р733
#define REPLY_733_WRONG_VM_STATE	0x8040		// Ошибка в конфигурации ВЧМ 14Р733
#define REPLY_733_WRONG_PUPS_STATE	0x8041		// Ошибка состояния ПУПС 14Р733
#define REPLY_733_GOOD_PUPS		0x0042		// Достоверный ПУПС 14Р733
#define REPLY_733_UPI_READ_ERROR	0x8043		// Ошибка считывания состояния УПИ 733
#define REPLY_733_UPI_INFO   		0x0044		// Передаются данные состояния УПИ 733

#define REPLY_IRQ_MESSAGE		0x0045		// сообщение о приходе прерывания
#define REPLY_ASN_IRQ_MESSAGE 		0x0046		// информация о прерываниях от АСН

#define REPLY_BUPE_OFF_ERROR   		0x8047		// Ошибка выключения БУП-Е
#define REPLY_BUPE_ON_ERROR   		0x8048		// Ошибка включения БУП-Е
#define REPLY_BUPE_BUS_DISABLE_ERROR    0x8049		// Ошибка отключения шин БУП-Е
#define REPLY_ASN_RECONFIG_ERROR	0x804A		// Ошибка при переключении комплекта АСН - АСН выставил бит ошибки
#define REPLY_733_UPI_ON_ERROR   	0x804B		// Ошибка при включении блока УПИ 
#define REPLY_733_UPI_OFF_ERROR   	0x804C		// Ошибка при отключении блока УПИ 
#define REPLY_733_UPI_STATE_ERROR   	0x804D		// Конфигурация блока УПИ не соответствует ожидаемой

#define REPLY_MBK07_FSMU_OFF_ERROR   	0x804E		// Ошибка при выключении блока МБК07 ФСМУ
#define REPLY_MBK07_FSVU_OFF_ERROR      0x804F		// Ошибка при выключении блока МБК07 ФСВУ
#define REPLY_MBK07_FSMU_ON_ERROR   	0x8050		// Ошибка при включении блока МБК07 ФСМУ
#define REPLY_MBK07_FSVU_ON_ERROR       0x8051		// Ошибка при включении блока МБК07 ФСВУ
#define REPLY_MBK07_LO_STAB_ON_ERROR	0x8052		// Ошибка при включении режима низкой стабильности
#define REPLY_MBK07_HI_STAB_ON_ERROR	0x8053		// Ошибка при включении режима высокой стабильности
#define REPLY_MBK07_KG_OFF_ERROR	0x8054		// Ошибка при проверке отключенного состояния генераторов
#define REPLY_MBK07_KG1_ON_ERROR	0x8055		// Ошибка при включении КГ1
#define REPLY_MBK07_KG2_ON_ERROR	0x8056		// Ошибка при включении КГ2
#define REPLY_MBK07_FSMU_STATE_ERROR    0x8057		// Ошибка состояния ФСМУ (текущее состояние не соответствует ожидаемому)
#define REPLY_MBK07_FSVU_STATE_ERROR    0x8058		// Ошибка состояния ФСВУ (текущее состояние не соответствует ожидаемому)
#define REPLY_MBK07_ANT_STATE_ERROR     0x8059		// Ошибка состояния антенн (текущее состояние не соответствует ожидаемому)
#define REPLY_MBK07_STATE_ERROR         0x805A		// Ошибка состояния блоков МБК07 (недопустимая конфигурация)

#define REPLY_MDI_DATA                  0x005B		// Выдан массив МДИ при запуске ЦБК
#define REPLY_BEH_IRQ_MESSAGE 		0x005C		// информация о прерываниях от БЭЧ
#define REPLY_MBK04_SHZK_WR_ERROR       0x805D          // Ошибка выдачи флага СЧЗК
#define REPLY_MBK04_KPI_WR_ERROR        0x805E          // Ошибка выдачи флага КПИ

#define REPLY_733_MU_WRONG_CONFIG	0x8060		// Неправильная конфигурация МУ 733
#define REPLY_733_MVKU_WRONG_CONFIG	0x8061		// Неправильная конфигурация МВКУ 733
#define REPLY_733_MPVN_WRONG_CONFIG	0x8062		// Неправильная конфигурация МПВН 733
#define REPLY_733_IRQ_TIMEOUT 		0x8063		// Таймаут ожидания прерывания от 733
#define REPLY_733_IRQ_CLR_ERROR		0x8064		// Сбой при сбросе прерывания от 733
#define REPLY_733_UPI_RESET_ERROR	0x8065		// Сбой при сбросе УПИ в исходное состояние

#define REPLY_732_EXT_INFO    	        0x0066		// дополнительные данные ПУПС 732 

#define REPLY_WAITED_BALOI_MSG		0x0070          // получено ожидаемое сообщение от БАЛОИ
#define REPLY_ASN_DELTA_TIME		0x0078          // передача дельты времени АСН/периодическая задача
//================================================================================

#define REPLY_CMD_DATA_ERROR		0x8400          // Ошибка в параметрах команды
#define REPLY_CMD_CRC_ERROR		0x8401          // Ошибка в CRC команды
#define REPLY_DATA_CRC_ERROR		0x8402          // Ошибка в CRC данных
#define REPLY_MKO_INT_ERROR 		0x8403          // Ошибка при связи по МКО, внутренние ошибки ОПО
#define REPLY_MKO_NO_OS_ERROR		0x8404		// Ошибка при связи по МКО, нет ответного слова от устройства
#define REPLY_MKO_LINK_ERROR		0x8405		// Ошибка при связи по МКО, нет связи/нарушена связь с устройством
#define REPLY_NO_DATA_ERROR		0x8406          // Ошибка при связи по МКО, нет слов данных
#define REPLY_DEVICE_BUSY_ERROR		0x8407          // Ошибка при связи по МКО, устройство занято
#define REPLY_INTERNAL_DATA_ERROR       0x8408          // Ошибка во внутренних данных (таблицы, ets)
#define REPLY_MISS_OK_NUMBER_ERROR	0x8409		// Данный номер ОК не найден в таблицах СПО
#define REPLY_DEVICE_CRC_ERROR          0x840A          // Неверная контрольная сумма в данных устройства 
#define REPLY_DEVICE_STATE1_ERROR       0x840B          // Состояние устройства не соответствует ожидаемому
#define REPLY_DEVICE_STATE2_ERROR       0x840C          // Состояние устройства не соответствует ожидаемому
#define REPLY_DEVICE_STATE3_ERROR       0x840D          // Состояние устройства не соответствует ожидаемому
#define REPLY_MKO_OU_ERROR		0x840E          // Устройство выставило биты ошибок в ОС

#define REPLY_CMD_STATE_ERROR		0x840F		// неверное состояние программы
#define REPLY_CMD_OVERFLOW_ERROR	0x8410		// переполнение списка
#define REPLY_ALREADY_SET_ERROR		0x8411		// элемент уже в списке
#define REPLY_ITEM_NOT_FOUND		0x8412		// указанный элемент не найден
#define REPLY_OPO_INT_ERROR		0x8413		// ошибка ОПО при вызове системной функции
#define REPLY_PERIODIC_MSG_ERROR	0x8414		// ошибка при выдаче периодического сообщения по МКО
#define REPLY_PROHIB_MSG_ERROR		0x8415		// сообщение в списке блокируемых сообщений МКО
#define REPLY_TIMEOUT_ERROR		0x8416		// время ожидания готовности/завершения истекло
#define REPLY_MKO_OS_MSG_ERROR          0x8417          // Устройство выставило в ОС бит ошибки в сообщении
#define REPLY_MKO_OS_STATE_ERROR        0x8418          // Ошибки в ОС или устройство выставило в ОС биты ошибок

#define REPLY_ERROR_POINT_1		0x8421		// точка сбоя 1
#define REPLY_ERROR_POINT_2		0x8422		// точка сбоя 2
#define REPLY_ERROR_POINT_3		0x8423		// точка сбоя 3
#define REPLY_ERROR_POINT_4		0x8424		// точка сбоя 4

#define REPLY_UNK_ERROR			0x84FF          // Неизвестная ошибка
//================================================================================

#define REPLY_IRQ_00_DONE		0x0800		// Обработка прерывания 0x00 завершена
#define REPLY_IRQ_01_DONE		0x0801		// Обработка прерывания 0x01 завершена
#define REPLY_IRQ_02_DONE		0x0802		// Обработка прерывания 0x02 завершена
#define REPLY_IRQ_03_DONE		0x0803		// Обработка прерывания 0x03 завершена
#define REPLY_IRQ_04_DONE		0x0804		// Обработка прерывания 0x04 завершена
#define REPLY_IRQ_05_DONE		0x0805		// Обработка прерывания 0x05 завершена
#define REPLY_IRQ_06_DONE		0x0806		// Обработка прерывания 0x06 завершена
#define REPLY_IRQ_07_DONE		0x0807		// Обработка прерывания 0x07 завершена
#define REPLY_IRQ_08_DONE		0x0808		// Обработка прерывания 0x08 завершена
#define REPLY_IRQ_09_DONE		0x0809		// Обработка прерывания 0x09 завершена
#define REPLY_IRQ_0A_DONE		0x080A		// Обработка прерывания 0x0A завершена
#define REPLY_IRQ_0B_DONE		0x080B		// Обработка прерывания 0x0B завершена
#define REPLY_IRQ_0C_DONE		0x080C		// Обработка прерывания 0x0C завершена
#define REPLY_IRQ_0D_DONE		0x080D		// Обработка прерывания 0x0D завершена
#define REPLY_IRQ_0E_DONE		0x080E		// Обработка прерывания 0x0E завершена
#define REPLY_IRQ_0F_DONE		0x080F		// Обработка прерывания 0x0F завершена

#define REPLY_IRQ_00_ERROR		0x8800          // Ошибка при обработке прерывания 0x00
#define REPLY_IRQ_01_ERROR		0x8801          // Ошибка при обработке прерывания 0x01
#define REPLY_IRQ_02_ERROR		0x8802          // Ошибка при обработке прерывания 0x02
#define REPLY_IRQ_03_ERROR		0x8803          // Ошибка при обработке прерывания 0x03
#define REPLY_IRQ_04_ERROR		0x8804          // Ошибка при обработке прерывания 0x04
#define REPLY_IRQ_05_ERROR		0x8805          // Ошибка при обработке прерывания 0x05
#define REPLY_IRQ_06_ERROR		0x8806          // Ошибка при обработке прерывания 0x06
#define REPLY_IRQ_07_ERROR		0x8807          // Ошибка при обработке прерывания 0x07
#define REPLY_IRQ_08_ERROR		0x8808          // Ошибка при обработке прерывания 0x08
#define REPLY_IRQ_09_ERROR		0x8809          // Ошибка при обработке прерывания 0x09
#define REPLY_IRQ_0A_ERROR		0x880A          // Ошибка при обработке прерывания 0x0A
#define REPLY_IRQ_0B_ERROR		0x880B          // Ошибка при обработке прерывания 0x0B
#define REPLY_IRQ_0C_ERROR		0x880C          // Ошибка при обработке прерывания 0x0C
#define REPLY_IRQ_0D_ERROR		0x880D          // Ошибка при обработке прерывания 0x0D
#define REPLY_IRQ_0E_ERROR		0x880E          // Ошибка при обработке прерывания 0x0E
#define REPLY_IRQ_0F_ERROR		0x880F          // Ошибка при обработке прерывания 0x0F
//================================================================================

#define REPLY_START_AFTER_CBK_RECONFIG	0x0100		// запуск после реконфигурации ЦБК
#define REPLY_CBK_RECONFIG_PARAM_ERROR 	0x8101          // ошибка в параметрах реконфигурации
#define REPLY_CBK_RECONFIG_NO_VM 	0x8102          // нет машин для втягивания в рабочую конфигурацию
#define REPLY_CBK_RECONFIG_UNK_ERROR	0x8103          // функция реконфигурации вернула неизвестный код ошибки
//================================================================================

#define REPLY_FLAG_ERROR		0x8000		// установка старшего бита в ответе - признак ошибки
#define REPLY_DEBUG    			0x00FF		// отладочное сообщение в ответ на прерывание
//================================================================================

// выравнивание размера на 16-бит слово (выравнивание вверх)
#define WORD_ALIGN(Size) (((Size) & 1) ? ((Size) + 1) : (Size))
//================================================================================

#include "Bos.h"     	// заголовки функций BOS
#include "HookDrv.h" 	// определения констант перехватчика
#include "General.h" 	// стандартные макросы
#include "Stdio.h"   	// для использования printf
#include "String.h"  	// операции со строками и памятью
#include "MemAdrSPO.h"  // константы с адресами ОПО/СПО
#include "MemDef.h"     // системные типы/константы ОПО
//================================================================================

#include "Integer.h" 	// определения элементарных типов
#include "DataUnit.h"
#include "ControlUnit.h"
#include "CrcUnit.h"
#include "VersionUnit.h"
#include "PeriodicMkoMsgUnit.h"
#include "ProhibitedMkoMsgUnit.h"
#include "PacketMkoMsgUnit.h"
#include "MkoStressTestUnit.h"
#include "BkoiUnit.h"
//================================================================================

void MainTask (int AParam);
void IrqTask (int AChanMask);
void ErrorTask (UINT *AdrBuf);
void TestTask (int AParam);
void AsnPeriodicTask (int AParam);
unsigned OpoToSpoHookProc (unsigned Param);
//================================================================================

void MainTaskDoReconfigCbk();
void MainTaskInit (void);
void MainTaskRun (void);
void MainTaskRunCheck (void);
void MainTaskIdle (void);
void MainTaskCheckCbkConfig (void);
void MainTaskDoShbkImit (void);
void MainTaskDoReplyImit (void);
void MakeShbkBuffer (UINT ShbkMask, BYTE *ShbkBuffer);
void CheckAsnDeltaTime (void);
//================================================================================

void AsnControlProc (sysTime_t *ACurrTime);
void BehControlProc (sysTime_t *ACurrTime);
void SetDefaultConfig (void);
void HandlerTableInit (void);
void DetectMkoLine (void);
void MainTaskGetMkiData (void);
void MainTaskGetMdiData (void);
int DeltaTimeMSec (sysTime_t *BeginTime, sysTime_t *EndTime);
void SleepMSec (int ATime);
void PrintTaskState (int TaskNum);
//================================================================================

int MkoXchgAn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgBn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgBnStd (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgBnMini (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgBnExt (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgBnBlock (int MkoLine, int MkoChan, WORD MkoCmd1, WORD MkoCmd2, int MsgCount, int SendCmdMsg);
int MkoXchgXn (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);
int MkoXchgStub (int MkoLine, int MkoChan, WORD MkoCmd, int F4Mode);

int CheckMkoXchg (mkoPackRes_t *PackRes, mkoPackMsg_t *MsgList, mkoMsgRes_t *MsgRes, int MsgCount);
int DECODE_XCHG_ERROR (int Result);
int BitCount (int Value);

void ReplyToKpaNoData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr);
void ReplyToKpaWithData (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize);

void ReplyToKpaWithDataX (int ABaseMkoLine, int ABaseMkoChan, int AReply, int ASubAddr, void *ABuffer, int ADataSize);

void Input (unsigned nMKO, unsigned ADR, unsigned pADR, unsigned nCD, unsigned short *INF);
void Output (unsigned nMKO, unsigned ADR, unsigned pADR, unsigned nCD, unsigned short *INF);     
//================================================================================

void SendStartMsgToKPA (void);
void SendVmStateToKPA (void);
void SendMkiDataToKPA (void);
void SendMdiDataToKPA (void);
void SendNextPassMsgToKPA (void);
void SendCbkReConfigMsgToKPA (void);
void SendCbkConfigToKPA (void);
//================================================================================

void HandleIrqBA (int AChanMask);

void IrqHandler00 (void);
void IrqHandler00TaskDeltaTime (void);
void IrqHandler01 (void);
void IrqHandler02 (void);
void IrqHandler03 (void);
void IrqHandler04 (void);
void IrqHandler05 (void);
void IrqHandler06 (void);
void IrqHandler07 (void);

void IrqHandler08 (void);
void IrqHandler09 (void);
void IrqHandler0A (void);
void IrqHandler0B (void);
void IrqHandler0C (void);
void IrqHandler0D (void);
void IrqHandler0E (void);
void IrqHandler0F (void);

void SendIrqMessageToKPA (int AMask);
//================================================================================

void KpaCommandUnknown (int BaseMkoLine, int BaseMkoChan);

void KpaCommand01 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand02 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand03 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand04 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand05 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand06 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand07 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand08 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand09 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0A_Erti (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0A_Koi (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0B (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0C (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0D (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0E (int BaseMkoLine, int BaseMkoChan);
void KpaCommand0F (int BaseMkoLine, int BaseMkoChan);
void KpaCommand10 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand11 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand13 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand14 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand15 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand16 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand17 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand18 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand19 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand20 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand21 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand22 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand23 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand24 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand25 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand26 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand27 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand28 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand29 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2A (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2B (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2C (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2D (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2E (int BaseMkoLine, int BaseMkoChan);
void KpaCommand2F (int BaseMkoLine, int BaseMkoChan);

void KpaCommand30 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand31 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand32 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand33 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand34 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand35 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand36 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand37 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand38 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand39 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand3A (int BaseMkoLine, int BaseMkoChan);
void KpaCommand40 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand41 (int BaseMkoLine, int BaseMkoChan);

void KpaCommand60 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand61 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand62 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand63 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand64 (int BaseMkoLine, int BaseMkoChan);
void KpaCommand65 (int BaseMkoLine, int BaseMkoChan);

void KpaCommandFB (int BaseMkoLine, int BaseMkoChan);
void KpaCommandFC (int BaseMkoLine, int BaseMkoChan);
void KpaCommandFD (int BaseMkoLine, int BaseMkoChan);
void KpaCommandFE (int BaseMkoLine, int BaseMkoChan);
void KpaCommandFF (int BaseMkoLine, int BaseMkoChan);
//================================================================================

void CbkChanPowerOn (int BaseMkoLine, int BaseMkoChan, int CbkChan);
void CbkChanPowerOff (int BaseMkoLine, int BaseMkoChan, int CbkChan, int Context);
void Get_OK_OKK_Data (int BaseMkoLine, int BaseMkoChan, int AMode);

int Reconfig733 (int BaseMkoLine, int BaseMkoChan);
int Read733Config (int DevMkoLine, int DevMkoChan, void *Config);
int Read733UpiConfig (int DevMkoLine, int DevMkoChan, void *Config);

int Check733Config (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, WORD MUPR_Orig, WORD MVKU_Orig, WORD VCHM_Orig);

int DisableMBK07x (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int DoFsvuOff);
int CheckFsmuPower (int DevMkoLine, int DevMkoChan);
int CheckFsvuPower (int DevMkoLine, int DevMkoChan);
int CheckFsmuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State);
int CheckFsvuOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMko7Chan, int WantState, int State);
int CheckGenOK (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan, int WantState, int State);
void BUPE_Chan1_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan);
void BUPE_Chan2_Enable (int BaseMkoLine, int BaseMkoChan, int DevMkoLine, int DevMkoChan);

void CbkErrCtrlCallBack (void);
void Irq10PackInit (void);

int CheckTimeMarkInInt (int TimeInt);
int CheckVmDeSynch (int VmNum, int PassCount);
//================================================================================

#endif

