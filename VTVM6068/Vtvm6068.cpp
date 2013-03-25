extern "C"
{
#include <Vtvm6068.h>
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
    MessageBox(NULL,"Подключение Заглушки Vtvm6068 для блока RS-422","Использование заглушек!", MB_ICONINFORMATION);

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


//
//				INSTRUMENT FUNCTION PROTOTYPES
//				------------------------------

// VXI Plug&Play Required Functions
ViStatus _VI_FUNC vtvm6068_init(ViString instrDesc, ViBoolean idQuery, ViBoolean reset, ViPSession instrHndl){ return 0; }
ViStatus _VI_FUNC vtvm6068_reset(ViSession instrHndl){ return 0; }
ViStatus _VI_FUNC vtvm6068_self_test(ViSession instrHndl, ViPInt16 testResult, ViPString testMessage){ return 0; }
ViStatus _VI_FUNC vtvm6068_error_query(ViSession instrHndl, ViPInt32 errorCode, ViPString errorString){ return 0; }
ViStatus _VI_FUNC vtvm6068_error_message(ViSession instrHndl, ViStatus error, ViPString message){ return 0; }
ViStatus _VI_FUNC vtvm6068_revision_query(ViSession instrHndl, ViPString driverRevision, ViPString instrumentRevision){ return 0; }
ViStatus _VI_FUNC vtvm6068_close(ViSession instrHndl){ return 0; }

//Backwards compatibility functions
ViStatus _VI_FUNC vtvm6068_selfTest(ViSession instrHndl, ViPInt16 testResult, ViPString testMessage){ return 0; }
ViStatus _VI_FUNC vtvm6068_errorQuery(ViSession instrHndl, ViPInt32 errorCode, ViPString errorString){ return 0; }
ViStatus _VI_FUNC vtvm6068_errorMessage(ViSession instrHndl, ViStatus error, ViPString message){ return 0; }
ViStatus _VI_FUNC vtvm6068_revisionQuery(ViSession instrHndl, ViPString driverRevision, ViPString instrumentRevision){ return 0; }

// CONFIGURATION FUNCTIONS
ViStatus _VI_FUNC vtvm6068_Config_UART_Channel(ViSession InstrumentHandle, ViInt16 ChannelNumber, ViInt16 SerialStandard, ViInt16 DataBits, ViInt16 StopBits, ViInt16 Tx_Parity, ViInt16 Rx_Parity){ return 0; }
ViStatus _VI_FUNC vtvm6068_Config_HDLC_Channel(ViSession InstrumentHandle, ViInt16 ChannelNumber, ViInt16 SerialStandard, ViInt16 TxDataCode, ViInt16 RxDataCode, 
												ViInt16 CRC_Type, ViInt32 Rx_HDLC_Address[], ViInt16 NumberOfAddresses, ViInt32 Rx_HDLC_AddressMask){ return 0; }
ViStatus _VI_FUNC vtvm6068_setupBaudRate(ViSession instrHndl, ViInt16 generator, ViReal64 baudRate, ViInt16 divisor){ return 0; }
ViStatus _VI_FUNC vtvm6068_setupTxRxChannel(ViSession instrHndl, ViInt16 channelNo, ViBoolean txOrRx, ViInt16 parity, ViInt16 numBits, ViInt16 numStopBits, ViInt16 interfaceStd, ViInt16 serialProtocol, ViInt16 dataEncodeType){ return 0; }
ViStatus _VI_FUNC vtvm6068_connectDisconnectHWFIFO(ViSession instrHndl, ViBoolean connectDisconnect, ViBoolean txOrRx, ViInt16		channelNo){ return 0; }
ViStatus _VI_FUNC vtvm6068_configClockParams(ViSession instrHndl, ViInt16 channelNo, ViInt16 direction, ViInt16 recvClockSource, ViInt16 recvDivideRatio, ViInt16 tranClockSource, ViInt16 tranDivideRatio){ return 0; }
ViStatus _VI_FUNC vtvm6068_configHDLCParams(ViSession instrHndl, ViInt16 channelNo, ViInt16 crcType, ViInt32 recvHDLCAddress[], ViInt16 numOfAddresses, ViInt32 recvHDLCAddressMask){ return 0; }
ViStatus _VI_FUNC vtvm6068_enableDisableCTSHShake(ViSession instrHndl, ViInt16 channelNo, ViInt16 enabDisabCTS){ return 0; }
ViStatus _VI_FUNC vtvm6068_enabDisabRxDuringTx(ViSession instrHndl, ViInt16 channelNo, ViInt16 enabDisabRecv){ return 0; }
ViStatus _VI_FUNC vtvm6068_configTraceSize(ViSession instrHndl, ViInt16 traceName, ViInt32 queueSize){ return 0; }

// APPLICATION FUNCTION
ViStatus _VI_FUNC vtvm6068_diagnostic(ViSession instrHndl, ViPInt16 result){ return 0; }

// DATA FUNCTIONS
ViStatus _VI_FUNC vtvm6068_loadDataViaWS(ViSession instrHndl, ViInt16 channel, ViInt16 data[], ViInt32 numOfBytes){ return 0; }
ViStatus _VI_FUNC vtvm6068_loadDataViaFIFO(ViSession instrHndl, ViInt16 data[], ViInt32 numOfBytes){ return 0; }
ViStatus _VI_FUNC vtvm6068_readDataViaWS(ViSession instrHndl, ViInt16 channel, ViInt16 data[], ViPInt32 numOfBytes){ return 0; }
ViStatus _VI_FUNC vtvm6068_readDataViaFIFO(ViSession instrHndl, ViInt16 data[], ViInt16 error[], ViPInt32 numOfBytes){ return 0; }

ViStatus _VI_FUNC vtvm6068_directIo(ViSession instrHndl, ViString cmd_string, ViPString response_string){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryBaudRate(ViSession instrHndl, ViInt16 generator, ViPReal64 baudRate, ViPInt16 divisor){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryChannelConfig(ViSession instrHndl, ViInt16 channelNo, ViBoolean txOrRx, ViPInt16 parity, ViPInt16 numBits, ViPInt16 numStopBits, ViPInt16 interfaceStd, ViPInt16 serialProtocol, ViPInt16 dataEncodeType){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryActiveTraceNames(ViSession instrHndl, ViPInt16 txChannel, ViPInt16 rxChannel){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryTraceParams(ViSession instrHndl, ViInt16 traceName, ViPInt32 traceLength, ViPInt32 freeQueueMem, ViPInt32 queueSize){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryClockParams(ViSession instrHndl, ViInt16 channelNo, ViPInt16 direction, ViPInt16 recvClockSource, ViPInt16 recvDivideRatio, ViPInt16 tranClockSource, ViPInt16 tranDivideRatio){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryHDLCParams(ViSession instrHndl, ViInt16 channelNo, ViPInt16 crcType, ViInt32 recvHDLCAddress[], ViPInt32 recvHDLCAddressMask){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryCTSHShakeMode(ViSession instrHndl, ViInt16 channelNo, ViPInt16 enabDisabCTS){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryReceiverState(ViSession instrHndl, ViInt16 channelNo, ViPInt16 enabDisabRecv){ return 0; }
ViStatus _VI_FUNC vtvm6068_scanChassis(ViPInt16 laArray, ViPInt16 slotNumArray, ViPInt16 boardNumArray, ViPInt16 num6068sFound, ViPInt16 flagVXI_GPIB){ return 0; }
ViStatus _VI_FUNC vtvm6068_enableDisableSCPILog(ViInt16 scpiLog, ViString logFileName){ return 0; }
ViStatus _VI_FUNC vtvm6068_readDataFromFile(ViSession instrHndl, ViChar fileName[], ViInt16 dataArray[], ViPInt32 numOfElems){ return 0; }
ViStatus _VI_FUNC vtvm6068_saveDataToFile(ViSession instrHndl, ViChar fileName[], ViInt16 dataArray[], ViInt32 numOfElems){ return 0; }
ViStatus _VI_FUNC vtvm6068_setStatusMask(ViSession instrHndl, ViInt16 serviceReqEnablRreg, ViInt16 eventStatusEnableReg, ViInt16 operStatusEnableReg){ return 0; }
ViStatus _VI_FUNC vtvm6068_setOperComplete(ViSession instrHndl){ return 0; }
ViStatus _VI_FUNC vtvm6068_getOperComplete(ViSession instrHndl, ViPInt16 retOper){ return 0; }
ViStatus _VI_FUNC vtvm6068_clearStatusReg(ViSession instrHndl){ return 0; }
ViStatus _VI_FUNC vtvm6068_queryDeviceStatusReg(ViSession instrHndl, ViInt16 whichStatusReg, ViPInt32 valueStatusReg){ return 0; }

