#include <unmbase.h>
#include <windows.h>
#include <qcoreapplication.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif
/*
// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{

switch (fdwReason)      // Дерево разбора уведомлений
{
  case DLL_PROCESS_ATTACH: // Подключение DLL
    MessageBox(NULL,"Подключение Заглушки UNMBASE для Носителя Мезонинов","Использование заглушек!", MB_ICONINFORMATION);

    if (lpvReserved)  // Определение способа загрузки
      MessageBox(NULL,"DLL загружена с неявной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    else
      MessageBox(NULL,"DLL загружена с явной компоновкой","Использование заглушек!", MB_ICONINFORMATION);
    return 1; // успешная инициализация

  case DLL_PROCESS_DETACH: // Отключение DLL
    // Здесь – освобождаем память, закрываем
    // файлы и т.д.
    break;

  case DLL_THREAD_ATTACH: // Уведомление о новом потоке 
    // Здесь – если надо переходим на
    // многопоточный режим работы с
    // использованием средств синхронизации
    // таких как критическая секция, мутанты,
    // семафоры и т.д.
    break;

  case DLL_THREAD_DETACH:
      //Уведомление о завершении потока
    // Здесь – если надо освобождаем все ресурсы, 
    // вязанные с завершившимся потоком. Какой именно
    // поток завершился можно узнать просмотром списка
    // потоков средствами TOOLHELP32
    MessageBox(NULL,"Использование заглушек!","Завершение потока", MB_ICONINFORMATION);
    break;

  }
return TRUE;    // Код возврата игнорируется
}
*/

struct mezanin_struct
{
	ViInt16 Present;
	ViInt16 Type;
};

QList<mezanin_struct> mezanin_list;
int mezanin_list_poiner = 0;

void mezanin_list_add(ViInt16 Present, ViInt16 Type)
{
	mezanin_struct mez;
	mez.Present = Present;
	mez.Type = Type;
	mezanin_list.push_back(mez);
}


_UNMBASE_API ViStatus _VI_FUNC unmbase_init (ViRsrc rsrcName,
							ViBoolean id_query,
							ViBoolean reset,
							ViSession *vi)
{
	QString commapp = QCoreApplication::applicationName();
	if (commapp == "comapp1")
	{
		// Сначала инициализируется НМ, потом НМ-АРМ затем МНУ
		// НМ
		/*
		mezanin_list_add(1, 0x021);	// OSC5
		mezanin_list_add(1, 0x021);
		mezanin_list_add(1, 0x021);
		mezanin_list_add(1, 0x021);
		mezanin_list_add(1, 0x1A);	// MC
		mezanin_list_add(1, 0x1A);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		*/

		mezanin_list_add(1, 0x021);	// OSC5
		mezanin_list_add(0, 0);
		mezanin_list_add(1, 0x1A);	// MC
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);

		mezanin_list_add(1, 0x9);	// MDS32
		mezanin_list_add(1, 0x9);	// MDS32
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);

		mezanin_list_add(1, 0x5);	// MT8K4L
		mezanin_list_add(1, 0x5);	// MT8K4L
		mezanin_list_add(1, 0x5);	// MT8K4L
		mezanin_list_add(1, 0x5);	// MT8K4L
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);

		// НМУ
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
	}

	if (commapp == "comapp2")
	{
		// Сначала инициализируется НМ, потом НМ-АРМ затем МНУ
		// НМ
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(1, 0x17);	// MN8I
		mezanin_list_add(1, 0x1F);	// MN3I
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);

		mezanin_list_add(1, 0x021);	// OSC5
		mezanin_list_add(0, 0);
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(1, 0x0A);	// MFSK24
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		mezanin_list_add(0, 0);
		
	}

	return 0; 
}
/*--------------------------------------------------------------------------*/
/* Init mezzanine session                                                   */
/*  On M-module number Num (if present){ return 0; } open session. Session number is     */
/* index in global array p_mvi + 1, where store session structure pointer   */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_init (ViSession vi,
								ViUInt16 Num,
								ViSession *mvi){ return 0; }
/*--------------------------------------------------------------------------*/
/* Close mezzanine session                                                  */
/*  Free session structure (->m_vi_idx=0){ return 0; } and session number (p_mvi[m_vi_idx] = 0){ return 0; }  */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_close (ViSession mvi){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_get_attribute (ViSession mvi,
								ViAttr Attribute_Name,
								void *Attribute_Value){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_set_attribute (ViSession mvi,
								ViAttr Attribute_Name,
								ViAttrState Attribute_Value){ return 0; }
/*--------------------------------------------------------------------------*/
/* Input D32 word from mezzanine memory                                     */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_in16 (ViSession mvi,
								ViUInt32 Addr,
								ViUInt16 *Data){ return 0; }
/*--------------------------------------------------------------------------*/
/* Output D32 word to mezzanine memory                                      */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_out16 (ViSession mvi,
								ViUInt32 Addr,
								ViUInt16 Data){ return 0; }
/*--------------------------------------------------------------------------*/
/* Input block of D16 words from mezzanine memory                           */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_movein16 (ViSession mvi,
								ViUInt32 Addr,
								ViUInt16 *Buff,
								ViInt32 Count,
								ViBoolean Increment){ return 0; }
/*--------------------------------------------------------------------------*/
/* Output block of D16 words to mezzanine memory                            */
/*--------------------------------------------------------------------------*/
//OK1
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_moveout16 (ViSession mvi,
								ViUInt32 Addr,
								ViUInt16 *Buff,
								ViInt32 Count,
								ViBoolean Increment){ return 0; }
/*--------------------------------------------------------------------------*/
/* Set sample width of mezzanine                                            */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_sample_width (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Width){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query sample width of mezzanine                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_sample_width_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Width){ return 0; }
/*--------------------------------------------------------------------------*/
/* Set delay of first mezzanine sample                                      */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_first_delay (ViSession mvi,
								ViUInt16 io,
								ViUInt32 FirstDelay){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query delay of first mezzanine sample                                    */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_first_delay_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *FirstDelay){ return 0; }
/*--------------------------------------------------------------------------*/
/* Allocate memory for mezzanine                                            */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_alloc (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Size,
								ViUInt32 *Addr){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine allocated memory                                         */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_alloc_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Size,
								ViUInt32 *Addr){ return 0; }
/*--------------------------------------------------------------------------*/
/* Switch mezzanine mode:single, block or cycle                             */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_mode (ViSession mvi,
								ViUInt16 io,
								ViUInt16 Mode){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine mode:single, block or cycle                             */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_mode_q (ViSession mvi,
								ViUInt16 io,
								ViUInt16 *Mode){ return 0; }
/*--------------------------------------------------------------------------*/
/* Switch mezzanine START signal mode: pulse or level                       */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_trig_length (ViSession mvi,
								ViUInt16 Mode){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine START signal mode: pulse or level                        */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_trig_length_q (ViSession mvi,
								ViUInt16 *Mode){ return 0; }
/*--------------------------------------------------------------------------*/
/* Setup mezzanine trigger source                                           */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_config_trigger (ViSession mvi,
										ViUInt16 io,
										ViInt16 StartSource){ return 0; }
/*--------------------------------------------------------------------------*/
/* Return mezzanine trigger source                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_config_trigger_q (ViSession mvi,
										ViUInt16 io,
										ViInt16 *StartSource){ return 0; }
/*--------------------------------------------------------------------------*/
/* Set mezzanine sampling frequency                                         */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_clock (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Divisor){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine sampling frequency                                       */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_clock_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Divisor){ return 0; }
/*--------------------------------------------------------------------------*/
/* Set mezzanine block size                                                 */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_block_size (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine block size                                               */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_block_size_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Set mezzanine loop size                                                */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_loop_size (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine loop size                                               */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_loop_size_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Size){ return 0; }
								
/*--------------------------------------------------------------------------*/
/* Set mezzanine loop segment config                                        */
/* (do not switch anything)                                                 */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_segment (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Segment,
								ViUInt32 Offset,
								ViUInt32 Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine loop segment config                                      */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_segment_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Segment,
								ViUInt32 *Offset,
								ViUInt32 *Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Switch mezzanine loop segment                                            */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_segment_switch (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Segment,
								ViBoolean mode){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query current mezzanine loop segment                                     */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_segment_cur_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Segment){ return 0; }
								
/*--------------------------------------------------------------------------*/
/* Set mezzanine packet size                                                */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_packet_size (ViSession mvi,
								ViUInt16 io,
								ViUInt32 Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query mezzanine packet size                                              */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_packet_size_q (ViSession mvi,
								ViUInt16 io,
								ViUInt32 *Size){ return 0; }
/*--------------------------------------------------------------------------*/
/* Install user IRQ handler                                                 */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_handler (ViSession mvi,
								ViAddr Handler){ return 0; }
/*--------------------------------------------------------------------------*/
/* Enable/Disable Events                                                    */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_config_event (ViSession mvi,
								ViBoolean enable){ return 0; }
/*--------------------------------------------------------------------------*/
/* Query event enabled                                                      */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_config_event_q (ViSession mvi,
								ViBoolean *enable,
								ViBoolean *m_enable){ return 0; }
/*--------------------------------------------------------------------------*/
/* Read Last Sample                                                         */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_read_last (ViSession mvi,
									ViUInt32 Buff[],
									ViUInt32 *firstTime,
									ViUInt32 *thisTime){ return 0; }
/*--------------------------------------------------------------------------*/
/* Read Data Array                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_read_block (ViSession mvi,
									ViUInt32 Idx,
									ViUInt32 NSamples,
									ViUInt32 Buff[]){ return 0; }
/*--------------------------------------------------------------------------*/
/* Read next ready samples when cycle mode run                              */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_read_packet (ViSession mvi,
										ViUInt32 NSamples,
										ViUInt32 Buff[],
										ViUInt32 *ReadNSamples){ return 0; }
/*--------------------------------------------------------------------------*/
/* Read last samples when cycle mode run                                    */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_read_last_packet (ViSession mvi,
										ViUInt32 NSamples,
										ViUInt32 Buff[],
										ViUInt32 *ReadNSamples){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_write_block (ViSession mvi,
										ViUInt32 Idx,
										ViUInt32 NSamples,
										ViUInt32 Buff[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_write_packet (ViSession mvi,
										ViUInt32 NSamples,
										ViUInt32 Buff[],
										ViUInt32 *WriteNSamples){ return 0; }
/*--------------------------------------------------------------------------*/
/* Read Data Array from Segment                                             */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_read_segment (ViSession mvi,
										ViUInt16 io,
										ViUInt32 Segment,
										ViUInt32 Idx,
										ViUInt32 NSamples,
										ViUInt32 Buff[]){ return 0; }
/*--------------------------------------------------------------------------*/
/* Write Data Array to Segment                                              */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_write_segment (ViSession mvi,
										ViUInt16 io,
										ViUInt32 Segment,
										ViUInt32 Idx,
										ViUInt32 NSamples,
										ViUInt32 Buff[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_start (ViSession mvi,
								ViUInt16 io){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_stop (ViSession mvi,
								ViUInt16 io){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_async_stop (ViSession mvi,
								ViUInt16 io){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_state (ViSession mvi,
									ViUInt16 io,
									ViInt16 *State){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_reset (ViSession mvi,
									ViUInt16 io){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_type_q (ViSession vi,                                                  //Запрос типа
									ViInt16 N,
									ViInt16 *Present,
									ViInt16 *Type)
{
/*	if ((N == 1) || (N == 2))//MDS32
	{
		*Present = 1;
		*Type = 0x5;
	}
	else if ((N == 3) || (N == 4))//MFSK24
	{
		*Present = 1;
		*Type = 0x0A;
	}
	else
	{
		*Present = 0;
		*Type = 0;
	}
	
*/
	
	*Present = mezanin_list.at(mezanin_list_poiner).Present;
	*Type = mezanin_list.at(mezanin_list_poiner).Type;
	mezanin_list_poiner++;
	return 0; 
}
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_mem_q (ViSession vi,
								ViUInt32 *MemSize){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_time_q (ViSession vi,
								ViUInt32 *time){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_config_trigger (ViSession vi,
										ViInt16 StartSource,
										ViInt16 Master,
										ViInt16 TTLTRGline){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_config_trigger_q (ViSession vi,
										ViInt16 *StartSource,
										ViInt16 *Master,
										ViInt16 *TTLTRGline){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_config_ticks (ViSession vi,
										ViInt16 time){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_config_ticks_q (ViSession vi,
										ViInt16 *time){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_start (ViSession vi){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_stop (ViSession vi){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_state (ViSession vi,
								ViInt16 *State){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_time_sync (ViSession vi, ViBoolean dosync){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_m_howmuch_data_q (ViSession mvi,
									ViUInt16 io,
									ViUInt32 *howmuch){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_reset (ViSession vi){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_self_test (ViSession vi,
									ViInt16 *TestResult,
									ViChar _VI_FAR TestMessage[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_error_query (ViSession vi,
										ViInt32 *Error,
										ViChar _VI_FAR ErrorMessage[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_error_message (ViSession vi,
										ViStatus Status,
										ViChar _VI_FAR ErrorMessage[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_revision_query (ViSession vi,
										ViChar _VI_FAR HardRevision[],
										ViChar _VI_FAR SoftRevision[]){ return 0; }
/*--------------------------------------------------------------------------*/
/*                                                                          */
/*--------------------------------------------------------------------------*/
_UNMBASE_API ViStatus _VI_FUNC unmbase_close (ViSession vi){ return 0; }

/*--------------------------------------------------------------------------*/
/* Utility functions for internal debugging                                 */
/*--------------------------------------------------------------------------*/
ViStatus _VI_FUNCC unmbase_data_read(ViSession vi,
						  ViUInt32 A,   //global RAM address
						  ViUInt32 N,   //number of bytes
						  ViPUInt8 Buff){ return 0; }
ViStatus _VI_FUNCC unmbase_data_write(ViSession vi, 
						ViUInt32 A, //global RAM address
						ViUInt32 N, //number of bytes
						ViPUInt8 Buff){ return 0; }
//for old compatibility
ViStatus _VI_FUNCC unmbase_blk_read(ViSession vi,
						  ViUInt32 A,   //global RAM address
						  ViUInt32 N,   //number of bytes
						  ViPUInt8 Buff,
						  ViInt32 mez){ return 0; }	//для отладки - кто блокирует...
ViStatus _VI_FUNCC unmbase_blk_write(ViSession vi, 
						ViUInt32 A, //global RAM address
						ViUInt32 N, //number of bytes
						ViPUInt8 Buff,
						ViInt32 mez){ return 0; }	//для отладки - кто блокирует...

#if defined(__cplusplus) || defined(__cplusplus__)
		}       //extern "C" {
#endif