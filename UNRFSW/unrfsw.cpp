#include <unrfsw.h>	//need for VI_ERROR_PARAMETERn       

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif


//Инициализация
ViStatus _VI_FUNC unrfsw_init (ViRsrc rsrcName, ViBoolean id_query,ViBoolean reset, ViPSession vi){ return 0; }

//Служебные функции
ViStatus _VI_FUNC unrfsw_reset (ViSession vi){ return 0; }
ViStatus _VI_FUNC unrfsw_error_message (ViSession vi, ViStatus error, ViChar message[]){ return 0; }
ViStatus _VI_FUNC unrfsw_error_query (ViSession vi, ViPInt32 error, ViChar _VI_FAR error_message[]){ return 0; }
ViStatus _VI_FUNC unrfsw_self_test (ViSession vi,ViPInt16 test_result,ViChar _VI_FAR test_message[]){ return 0; }
ViStatus _VI_FUNC unrfsw_revision_query (ViSession vi, ViChar _VI_FAR driver_revision[], ViChar _VI_FAR instrument_revision[]){ return 0; }

//Прикладные функции
//Переключение. канала
ViStatus _VI_FUNC unrfsw_switch (ViSession vi, ViInt32 chan, ViInt32 output){ return 0; }
//Запрос состояния канала
ViStatus _VI_FUNC unrfsw_switch_state (ViSession vi, ViInt32 chan, ViInt32 *output){ return 0; }

//Закрытие
ViStatus _VI_FUNC unrfsw_close (ViSession vi){ return 0; }

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif
