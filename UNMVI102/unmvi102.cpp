extern "C"
{
#include <unmvi102.h>
}
#include <windows.h>
/*
// Объявляем функцию DllMain
BOOL APIENTRY DllMain(HINSTANCE hinstDLL,
      DWORD fdwReason, LPVOID lpvReserved)
{

switch (fdwReason)      // Дерево разбора уведомлений
{
  case DLL_PROCESS_ATTACH: // Подключение DLL
    MessageBox(NULL,"Подключение Заглушки UNMDS32 для Мезонина МДС-32","Использование заглушек!", MB_ICONINFORMATION);

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
ViStatus _VI_FUNC unmvi102_init (ViRsrc rsrcName, ViBoolean IDquery, ViBoolean doReset, ViSession *mezvi){return 0;}

ViStatus _VI_FUNC unmvi102_connect (ViSession mezvi, ViSession vi, ViUInt16 m_num, ViBoolean IDquery, ViBoolean doReset){return 0;}

ViStatus _VI_FUNC unmvi102_reset (ViSession mvi) {return 0;}

ViStatus _VI_FUNC unmvi102_switching_b (ViSession mvi, ViInt16 inVal, ViInt32 *outputAT28, ViInt32 *outputAT29, ViInt32 *outputAT30, ViInt32 *outputAT31) {return 0;}

ViStatus _VI_FUNC unmvi102_switching_r (ViSession mvi, ViInt16 outVal, ViInt32 *inputAT84, ViInt32 *inputAT85, ViInt32 *inputAT89){return 0;}

ViStatus _VI_FUNC unmvi102_attenuator_b (ViSession mvi, ViReal64 atten) {return 0;} 

ViStatus _VI_FUNC unmvi102_attenuator_r (ViSession mvi, ViReal64 atten) {return 0;} 

ViStatus _VI_FUNC unmvi102_set_input_freq (ViSession mvi, ViReal64 freq){return 0;}

ViStatus _VI_FUNC unmvi102_attenuator_q (ViSession mvi, ViInt16 sys, ViReal64 *atten){return 0;}

ViStatus _VI_FUNC unmvi102_self_test (ViSession mvi, ViInt16 *result, ViChar testMessage[]) {return 0;}

ViStatus _VI_FUNC unmvi102_error_query (ViSession mvi, ViPInt32 error, ViChar _VI_FAR errorMessage[]){return 0;}

ViStatus _VI_FUNC unmvi102_error_message (ViSession mvi, ViStatus status, ViChar errorMessage[]){return 0;}

ViStatus _VI_FUNC unmvi102_revision_query (ViSession mvi, ViChar driverVer[], ViChar instrumentVer[]){return 0;}

ViStatus _VI_FUNC unmvi102_close (ViSession mvi){return 0;}

ViStatus _VI_FUNC unmvi102_change_limit_b (ViSession mvi, ViReal64 Bt,
										   ViReal64 Bb){return 0;}

ViStatus _VI_FUNC unmvi102_change_limit_r (ViSession mvi, ViReal64 Rt,
										   ViReal64 Rb){return 0;}

ViStatus _VI_FUNC unmvi102_input_power (ViSession mvi, ViInt16 sys, ViReal64* buf) {return 0;}

ViStatus _VI_FUNC unmvi102_input_VCH (ViSession mvi, ViInt16 state){return 0;}

ViStatus _VI_FUNC unmvi102_input_VCH_q (ViSession mvi, ViInt16 *state) {return 0;}

ViStatus _VI_FUNC unmvi102_signal_setting_VCH (ViSession mvi, ViInt32 freq, ViInt32 atten, ViInt32 outsig) {return 0;}

ViStatus _VI_FUNC unmvi102_syn_outSignal(ViSession mvi,ViInt32 outsig){return 0;}  

ViStatus _VI_FUNC unmvi102_output_power (ViSession mvi, ViReal64* buf) {return 0;}

ViStatus _VI_FUNC unmvi102_temperature_survey (ViSession mvi, ViInt16 numDm, ViReal64* buf){return 0;} 

ViStatus _VI_FUNC unmvi102_event (ViSession mvi, ViInt16 *Bsignal, ViInt16 *Rsignal, ViInt16 *Lsignal) {return 0;}

ViStatus _VI_FUNC unmvi102_ok_mode (ViSession mvi, ViInt16 testmode , ViInt16*result, ViChar errorMes[]) {return 0;}


ViStatus _VI_FUNC unmvi102_readFlash(ViSession mvi){return 0;}
ViStatus _VI_FUNC unmvi102_writeFlash(ViSession mvi){return 0;}

ViStatus _VI_FUNC unmvi102_change_limit(ViSession mvi, ViReal64 Bt, ViReal64 Bb, ViReal64 Rt, ViReal64 Rb) {return 0;}


ViStatus _VI_FUNC unmvi102_check_Coeff(ViSession mvi){return 0;}
ViStatus _VI_FUNC unmvi102_coef_return(ViSession mvi, ViReal64 pwl[3]){return 0;}
ViStatus _VI_FUNC unmvi102_coef_save(ViSession mvi, ViReal64 *pwl){return 0;}
ViStatus _VI_FUNC unmvi102_cable_check(ViSession mvi, ViInt16* result, ViChar errorMes[]){return 0;}
ViStatus _VI_FUNC unmvi102_atten_coef_save(ViSession mvi, ViInt32 range, ViReal64 attenB, ViReal64 attenR){return 0;}
ViStatus _VI_FUNC unmvi102_atten_coef_return(ViSession mvi, ViInt32 range, ViReal64 *attenB, ViReal64 *attenR){return 0;}
ViStatus _VI_FUNC unmvi102_calibr_DM(ViSession mvi,ViInt16 numDM, ViInt16 range, ViReal64 P1, ViReal64 P2, ViInt32 KOD1, ViInt32 KOD2){return 0;}

ViStatus _VI_FUNC unmvi102_save_correct_coef_input_freq(ViSession mvi, ViReal64 coef, ViInt32 numCoef, ViBoolean R){return 0;}
ViStatus _VI_FUNC unmvi102_reset_correct_coef_input_freq(ViSession mvi, ViBoolean R){return 0;}

ViStatus _VI_FUNC unmvi102_once_attenuator_b (ViSession mvi, ViReal64 atten, ViInt16 numbA){return 0;}
ViStatus _VI_FUNC unmvi102_once_attenuator_r (ViSession mvi, ViReal64 atten, ViInt16 numbA){return 0;}


ViStatus _VI_FUNC unmvi102_resetCoeff(ViSession mvi){return 0;}


ViStatus _VI_FUNC unmvi102_attenuator_b_calibr (ViSession mvi, ViReal64 atten){return 0;}
ViStatus _VI_FUNC unmvi102_attenuator_r_calibr(ViSession mvi, ViReal64 atten){return 0;}
