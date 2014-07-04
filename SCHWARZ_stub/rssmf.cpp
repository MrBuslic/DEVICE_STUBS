#include <rssmf.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif



/*****************************************************************************/
/*= GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions) =======*/
/*****************************************************************************/
ViStatus _VI_FUNC rssmf_init (ViRsrc resourceName, ViBoolean IDQuery,
                              ViBoolean resetDevice, ViPSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_confLFFreqSweep (ViSession instrumentHandle, ViInt32 mode,
                                         ViReal64 startFrequency,
                                         ViReal64 stopFrequency, ViInt32 spacing,
                                         ViReal64 step, ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_LFSweepTrigger (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureLFSweepMarker (ViSession instrumentHandle,
                                                ViUInt32 channel, ViBoolean state,
                                                ViBoolean amplitudeMarker,
                                                ViReal64 frequency,
                                                ViReal64 amplitude,
                                                ViInt32 polarity,
                                                ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepMode (ViSession instrumentHandle, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepSource (ViSession instrumentHandle,
                                          ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepSource (ViSession instrumentHandle,
                                          ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepStartFreq (ViSession instrumentHandle,
                                             ViReal64 startFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepStartFreq (ViSession instrumentHandle,
                                             ViPReal64 startFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepStopFreq (ViSession instrumentHandle,
                                            ViReal64 stopFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepStopFreq (ViSession instrumentHandle,
                                            ViPReal64 stopFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepShape (ViSession instrumentHandle, ViInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepShape (ViSession instrumentHandle,
                                         ViPInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepSpacing (ViSession instrumentHandle,
                                           ViInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepSpacing (ViSession instrumentHandle,
                                           ViPInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepStepLin (ViSession instrumentHandle,
                                           ViReal64 stepLin){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepStepLin (ViSession instrumentHandle,
                                           ViPReal64 stepLin){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepStepLog (ViSession instrumentHandle,
                                           ViReal64 stepLog){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepStepLog (ViSession instrumentHandle,
                                           ViPReal64 stepLog){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepDwellTime (ViSession instrumentHandle,
                                             ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepDwellTime (ViSession instrumentHandle,
                                             ViPReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepManualStep (ViSession instrumentHandle,
                                              ViReal64 manualLFSweep){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepManualStep (ViSession instrumentHandle,
                                              ViPReal64 manualLFSweep){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepPoints (ViSession instrumentHandle,
                                          ViUInt32 LFSweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepPoints (ViSession instrumentHandle,
                                          ViPUInt32 LFSweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepMarkerState (ViSession instrumentHandle,
                                               ViUInt32 channel, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepMarkerState (ViSession instrumentHandle,
                                               ViUInt32 channel, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepMarkerFrequency (ViSession instrumentHandle,
                                                   ViUInt32 channel,
                                                   ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepMarkerFrequency (ViSession instrumentHandle,
                                                   ViUInt32 channel,
                                                   ViPReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepAmplitudeMarkerState (ViSession instrumentHandle,
                                                        ViUInt32 channel,
                                                        ViBoolean amplitudeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepAmplitudeMarkerState (ViSession instrumentHandle,
                                                        ViUInt32 channel,
                                                        ViPBoolean amplitudeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepAmplitudeMarkerAttenuation
             (ViSession instrumentHandle, ViReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepAmplitudeMarkerAttenuation
             (ViSession instrumentHandle, ViPReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepMarkerPolarity (ViSession instrumentHandle,
                                                  ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepMarkerPolarity (ViSession instrumentHandle,
                                                  ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepActiveMarker (ViSession instrumentHandle,
                                                ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetLFSweepActiveMarker (ViSession instrumentHandle,
                                                ViPInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLFSweepAllMarkersOff (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenBandwidth (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenBandwidth (ViSession instrumentHandle,
                                           ViInt32 channel, ViPInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenFrequency (ViSession instrumentHandle,
                                           ViInt32 channel,
                                           ViReal64 LFGenFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenFrequency (ViSession instrumentHandle,
                                           ViInt32 channel,
                                           ViPReal64 LFGenFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenShape (ViSession instrumentHandle, ViInt32 channel,
                                       ViInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenShape (ViSession instrumentHandle, ViInt32 channel,
                                       ViPInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenShapeTrapeze (ViSession instrumentHandle,
                                              ViInt32 channel, ViInt32 trapeze,
                                              ViReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenShapeTrapeze (ViSession instrumentHandle,
                                              ViInt32 channel, ViInt32 trapeze,
                                              ViPReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenShapeTriangle (ViSession instrumentHandle,
                                               ViInt32 channel, ViInt32 triangle,
                                               ViReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenShapeTriangle (ViSession instrumentHandle,
                                               ViInt32 channel, ViInt32 triangle,
                                               ViPReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_SetLFGenShapePulse (ViSession instrumentHandle,
                                            ViInt32 channel, ViInt32 pulse,
                                            ViReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_GetLFGenShapePulse (ViSession instrumentHandle,
                                            ViInt32 channel, ViInt32 pulse,
                                            ViPReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_confNoise (ViSession instrumentHandle, ViReal64 bandwidth,
                                   ViInt32 distribution){return 0;}
ViStatus _VI_FUNC rssmf_SetNOISEBandwidth (ViSession instrumentHandle,
                                           ViReal64 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_GetNOISEBandwidth (ViSession instrumentHandle,
                                           ViPReal64 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetNOISEDistribution (ViSession instrumentHandle,
                                              ViInt32 distribution){return 0;}
ViStatus _VI_FUNC rssmf_GetNOISEDistribution (ViSession instrumentHandle,
                                              ViPInt32 distribution){return 0;}
ViStatus _VI_FUNC rssmf_GetNOISERelativeLevel (ViSession instrumentHandle,
                                               ViPReal64 relativeLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetNOISEAbsoluteLevel (ViSession instrumentHandle,
                                               ViPReal64 absoluteLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetNOISEBandwidthState (ViSession instrumentHandle,
                                                ViBoolean bandwidthState){return 0;}
ViStatus _VI_FUNC rssmf_GetNOISEBandwidthState (ViSession instrumentHandle,
                                                ViPBoolean bandwidthState){return 0;}
ViStatus _VI_FUNC rssmf_confLFOutput (ViSession instrumentHandle, ViInt32 channel,
                                      ViBoolean state, ViInt32 source,
                                      ViReal64 outputVoltage){return 0;}
ViStatus _VI_FUNC rssmf_SetLFOutputMonitoring (ViSession instrumentHandle,
                                               ViInt32 monitoring){return 0;}
ViStatus _VI_FUNC rssmf_SetLFOutputDCOffset (ViSession instrumentHandle,
                                             ViReal64 DCOffset){return 0;}
ViStatus _VI_FUNC rssmf_GetLFOutputMonitoring (ViSession instrumentHandle,
                                               ViPInt32 monitoring){return 0;}
ViStatus _VI_FUNC rssmf_GetLFOutputDCOffset (ViSession instrumentHandle,
                                             ViPReal64 DCOffset){return 0;}
ViStatus _VI_FUNC rssmf_SetLFOutputState (ViSession instrumentHandle,
                                          ViBoolean LFOutputState){return 0;}
ViStatus _VI_FUNC rssmf_GetLFOutputState (ViSession instrumentHandle,
                                          ViPBoolean LFOutputState){return 0;}
ViStatus _VI_FUNC rssmf_SetLFOutputSource (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetLFOutputSource (ViSession instrumentHandle,
                                           ViInt32 channel, ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetLFOutputVoltage (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViReal64 outputVoltage){return 0;}
ViStatus _VI_FUNC rssmf_GetLFOutputVoltage (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViPReal64 LFGenVoltage){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureLFLevelSweepMode (ViSession instrumentHandle,
                                                   ViInt32 mode, ViInt32 source,
                                                   ViReal64 startLevel,
                                                   ViReal64 stopLevel,
                                                   ViReal64 step,
                                                   ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_LFLevelSweepTrigger (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureLFLevelSweepMarker (ViSession instrumentHandle,
                                                     ViUInt32 channel,
                                                     ViBoolean state,
                                                     ViReal64 level,
                                                     ViInt32 polarity,
                                                     ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepMode (ViSession instrumentHandle,
                                             ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepSource (ViSession instrumentHandle,
                                               ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepSource (ViSession instrumentHandle,
                                               ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepStartLevel (ViSession instrumentHandle,
                                                   ViReal64 startLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepStartLevel (ViSession instrumentHandle,
                                                   ViPReal64 startLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepStopLevel (ViSession instrumentHandle,
                                                  ViReal64 stopLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepStopLevel (ViSession instrumentHandle,
                                                  ViPReal64 stopLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepStep (ViSession instrumentHandle,
                                             ViReal64 step){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepStep (ViSession instrumentHandle,
                                             ViPReal64 step){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepCount (ViSession instrumentHandle,
                                              ViUInt32 count){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepCount (ViSession instrumentHandle,
                                              ViPUInt32 count){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepShape (ViSession instrumentHandle,
                                              ViInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepShape (ViSession instrumentHandle,
                                              ViPInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepDwellTime (ViSession instrumentHandle,
                                                  ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepDwellTime (ViSession instrumentHandle,
                                                  ViPReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepManualStep (ViSession instrumentHandle,
                                                   ViReal64 manualLFSweep){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepManualStep (ViSession instrumentHandle,
                                                   ViPReal64 manualLFSweep){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepMarkerState (ViSession instrumentHandle,
                                                    ViUInt32 channel,
                                                    ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepMarkerState (ViSession instrumentHandle,
                                                    ViUInt32 channel,
                                                    ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepMarkerLevel (ViSession instrumentHandle,
                                                    ViUInt32 channel,
                                                    ViReal64 level){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepMarkerLevel (ViSession instrumentHandle,
                                                    ViUInt32 channel,
                                                    ViPReal64 level){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepMarkerPolarity (ViSession instrumentHandle,
                                                       ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepMarkerPolarity (ViSession instrumentHandle,
                                                       ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepActiveMarker (ViSession instrumentHandle,
                                                     ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetLFLevelSweepActiveMarker (ViSession instrumentHandle,
                                                     ViPInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLFLevelSweepAllMarkersOff (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureExternalInput (ViSession instrumentHandle,
                                                ViInt32 input, ViInt32 coupling,
                                                ViInt32 impedance,
                                                ViInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetExternalInputCoupling (ViSession instrumentHandle,
                                                  ViInt32 input, ViInt32 coupling){return 0;}
ViStatus _VI_FUNC rssmf_GetExternalInputCoupling (ViSession instrumentHandle,
                                                  ViInt32 input, ViPInt32 coupling){return 0;}
ViStatus _VI_FUNC rssmf_SetExternalInputImpedance (ViSession instrumentHandle,
                                                   ViInt32 input,
                                                   ViInt32 impedance){return 0;}
ViStatus _VI_FUNC rssmf_GetExternalInputImpedance (ViSession instrumentHandle,
                                                   ViInt32 input,
                                                   ViPInt32 impedance){return 0;}
ViStatus _VI_FUNC rssmf_SetExternalInputBandwidth (ViSession instrumentHandle,
                                                   ViInt32 input,
                                                   ViInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_GetExternalInputBandwidth (ViSession instrumentHandle,
                                                   ViInt32 input,
                                                   ViPInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetAllModulationsState (ViSession instrumentHandle,
                                                ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_confPulm (ViSession instrumentHandle, ViBoolean state,
                                  ViInt32 source, ViInt32 externalPolarity,
                                  ViInt32 videoPolarity){return 0;}
ViStatus _VI_FUNC rssmf_confPulmGen (ViSession instrumentHandle,
                                     ViBoolean pulseOutputState,
                                     ViReal64 pulsePeriod, ViReal64 pulseWidth,
                                     ViReal64 pulseDelay,
                                     ViBoolean doublePulseState,
                                     ViReal64 doublePulseDelay,
                                     ViInt32 outputPolarity){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMState (ViSession instrumentHandle, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMState (ViSession instrumentHandle, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMSource (ViSession instrumentHandle, ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMSource (ViSession instrumentHandle, ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMExternalPolarity (ViSession instrumentHandle,
                                                 ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMExternalPolarity (ViSession instrumentHandle,
                                                 ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMVideoPolarity (ViSession instrumentHandle,
                                              ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMVideoPolarity (ViSession instrumentHandle,
                                              ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMSyncToInternalClock (ViSession instrumentHandle,
                                                    ViBoolean synchronizeToInternalClock){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMSyncToInternalClock (ViSession instrumentHandle,
                                                    ViPBoolean synchronizeToInternalClock){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMPeriod (ViSession instrumentHandle,
                                       ViReal64 pulsePeriod){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMPeriod (ViSession instrumentHandle,
                                       ViPReal64 pulsePeriod){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMWidth (ViSession instrumentHandle,
                                      ViReal64 pulseWidth){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMWidth (ViSession instrumentHandle,
                                      ViPReal64 pulseWidth){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMDelay (ViSession instrumentHandle,
                                      ViReal64 pulseDelay){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMDelay (ViSession instrumentHandle,
                                      ViPReal64 pulseDelay){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMDoubleState (ViSession instrumentHandle,
                                            ViBoolean doublePulseState){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMDoubleState (ViSession instrumentHandle,
                                            ViPBoolean doublePulseState){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMDoubleDelay (ViSession instrumentHandle,
                                            ViReal64 doublePulseDelay){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMDoubleDelay (ViSession instrumentHandle,
                                            ViPReal64 doublePulseDelay){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMDoubleWidth (ViSession instrumentHandle,
                                            ViReal64 doubleWidth){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMDoubleWidth (ViSession instrumentHandle,
                                            ViPReal64 doubleWidth){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMGeneratorOutputState (ViSession instrumentHandle,
                                                     ViBoolean outputState){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMGeneratorOutputState (ViSession instrumentHandle,
                                                     ViPBoolean outputState){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMOutputPolarity (ViSession instrumentHandle,
                                               ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMOutputPolarity (ViSession instrumentHandle,
                                               ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_PULMPulseTrainCatalog (ViSession instrumentHandle,
                                               ViString directoryPath,
                                               ViInt32 arraySize,
                                               ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_PULMDeletePulseTrainFile (ViSession instrumentHandle,
                                                  ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_PULMSelectPulseTrainFile (ViSession instrumentHandle,
                                                  ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMPulseTrainLength (ViSession instrumentHandle,
                                                 ViPInt32 pulseTrainLength){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMMode (ViSession instrumentHandle, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMMode (ViSession instrumentHandle, ViPInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainOfftime (ViSession instrumentHandle,
                                             ViInt32 arraySize,
                                             ViReal64 _VI_FAR offtime[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainOfftime (ViSession instrumentHandle,
                                             ViInt32 arraySize,
                                             ViReal64 _VI_FAR offtime[]){return 0;}
ViStatus _VI_FUNC rssmf_GetTrainOfftimePoints (ViSession instrumentHandle,
                                               ViPInt32 numberOfOfftimePoints){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainOntime (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViReal64 _VI_FAR ontime[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainOntime (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViReal64 _VI_FAR ontime[]){return 0;}
ViStatus _VI_FUNC rssmf_GetTrainOntimePoints (ViSession instrumentHandle,
                                              ViPInt32 numberOfOntimePoints){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainRepetition (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViInt32 _VI_FAR repetition[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainRepetition (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViInt32 _VI_FAR repetition[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportListMode (ViSession instrumentHandle,
                                                          ViInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportListMode (ViSession instrumentHandle,
                                                          ViPInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportListFile (ViSession instrumentHandle,
                                                          ViString sourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportListFile (ViSession instrumentHandle,
                                                          ViChar _VI_FAR sourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_PULMTrainImportExportExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportASCIIFile
             (ViSession instrumentHandle, ViString ASCIISourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportASCIIFile
             (ViSession instrumentHandle, ViChar _VI_FAR ASCIISourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportASCIIExtension
             (ViSession instrumentHandle, ViInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportASCIIExtension
             (ViSession instrumentHandle, ViPInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViPInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTrainImportExportASCIIDecimalSeparator
             (ViSession instrumentHandle, ViInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTrainImportExportASCIIDecimalSeparator
             (ViSession instrumentHandle, ViPInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_PULMTrainImportExportCatalog (ViSession instrumentHandle,
                                                      ViInt32 arraySize,
                                                      ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_confPulmTrigger (ViSession instrumentHandle,
                                         ViInt32 triggerLevel,
                                         ViInt32 externalInputImpedance,
                                         ViInt32 triggerMode,
                                         ViInt32 extTriggerInputSlope,
                                         ViInt32 gateInputPolarity){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTriggerLevel (ViSession instrumentHandle,
                                             ViInt32 triggerLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTriggerLevel (ViSession instrumentHandle,
                                             ViPInt32 triggerLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMExternalInputImpedance (ViSession instrumentHandle,
                                                       ViInt32 externalInputImpedance){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMExternalInputImpedance (ViSession instrumentHandle,
                                                       ViPInt32 externalInputImpedance){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMTriggerMode (ViSession instrumentHandle,
                                            ViInt32 triggerMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMTriggerMode (ViSession instrumentHandle,
                                            ViPInt32 triggerMode){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMExtTriggerInputSlope (ViSession instrumentHandle,
                                                     ViInt32 extTriggerInputSlope){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMExtTriggerInputSlope (ViSession instrumentHandle,
                                                     ViPInt32 extTriggerInputSlope){return 0;}
ViStatus _VI_FUNC rssmf_SetPULMGateInputPolarity (ViSession instrumentHandle,
                                                  ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetPULMGateInputPolarity (ViSession instrumentHandle,
                                                  ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_configAM (ViSession instrumentHandle, ViInt32 channel,
                                  ViBoolean AMState, ViInt32 AMSource,
                                  ViReal64 AMDepth, ViBoolean scanState,
                                  ViReal64 AMSensitivity, ViReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_SetAMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViBoolean AMState){return 0;}
ViStatus _VI_FUNC rssmf_GetAMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViPBoolean AMState){return 0;}
ViStatus _VI_FUNC rssmf_SetAMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViInt32 AMSource){return 0;}
ViStatus _VI_FUNC rssmf_GetAMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViPInt32 AMSource){return 0;}
ViStatus _VI_FUNC rssmf_SetAMDepth (ViSession instrumentHandle, ViInt32 channel,
                                    ViReal64 AMDepth){return 0;}
ViStatus _VI_FUNC rssmf_GetAMDepth (ViSession instrumentHandle, ViInt32 channel,
                                    ViPReal64 AMDepth){return 0;}
ViStatus _VI_FUNC rssmf_SetAMSensitivity (ViSession instrumentHandle,
                                          ViInt32 channel, ViReal64 AMSensitivity){return 0;}
ViStatus _VI_FUNC rssmf_GetAMSensitivity (ViSession instrumentHandle,
                                          ViInt32 channel, ViPReal64 AMSensitivity){return 0;}
ViStatus _VI_FUNC rssmf_SetAMScanState (ViSession instrumentHandle,
                                        ViBoolean scanState){return 0;}
ViStatus _VI_FUNC rssmf_GetAMScanState (ViSession instrumentHandle,
                                        ViPBoolean scanState){return 0;}
ViStatus _VI_FUNC rssmf_SetAMRatio (ViSession instrumentHandle, ViReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_GetAMRatio (ViSession instrumentHandle, ViPReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_confFM (ViSession instrumentHandle, ViInt32 channel,
                                ViBoolean state, ViInt32 FMSource,
                                ViReal64 deviation, ViReal64 ratio, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetFMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetFMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetFMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViInt32 FMSource){return 0;}
ViStatus _VI_FUNC rssmf_GetFMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViPInt32 FMSource){return 0;}
ViStatus _VI_FUNC rssmf_SetFMDeviation (ViSession instrumentHandle, ViInt32 channel,
                                        ViReal64 deviation){return 0;}
ViStatus _VI_FUNC rssmf_GetFMDeviation (ViSession instrumentHandle, ViInt32 channel,
                                        ViPReal64 deviation){return 0;}
ViStatus _VI_FUNC rssmf_SetFMRatio (ViSession instrumentHandle, ViReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_GetFMRatio (ViSession instrumentHandle, ViPReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_SetFMMode (ViSession instrumentHandle, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_GetFMMode (ViSession instrumentHandle, ViPInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_confPM (ViSession instrumentHandle, ViInt32 channel,
                                ViBoolean state, ViInt32 PMSource,
                                ViReal64 deviation, ViReal64 ratio, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetPMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPMState (ViSession instrumentHandle, ViInt32 channel,
                                    ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViInt32 PMSource){return 0;}
ViStatus _VI_FUNC rssmf_GetPMSource (ViSession instrumentHandle, ViInt32 channel,
                                     ViPInt32 PMSource){return 0;}
ViStatus _VI_FUNC rssmf_SetPMDeviation (ViSession instrumentHandle, ViInt32 channel,
                                        ViReal64 deviation){return 0;}
ViStatus _VI_FUNC rssmf_GetPMDeviation (ViSession instrumentHandle, ViInt32 channel,
                                        ViPReal64 deviation){return 0;}
ViStatus _VI_FUNC rssmf_SetPMRatio (ViSession instrumentHandle, ViReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_GetPMRatio (ViSession instrumentHandle, ViPReal64 ratio){return 0;}
ViStatus _VI_FUNC rssmf_SetPMMode (ViSession instrumentHandle, ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_GetPMMode (ViSession instrumentHandle, ViPInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetDMExternalInputsPolarity (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetDMExternalInputsPolarity (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_configASK (ViSession instrumentHandle, ViBoolean ASKState,
                                   ViInt32 ASKSource, ViReal64 ASKDepth){return 0;}
ViStatus _VI_FUNC rssmf_SetASKState (ViSession instrumentHandle,
                                     ViBoolean ASKState){return 0;}
ViStatus _VI_FUNC rssmf_GetASKState (ViSession instrumentHandle,
                                     ViPBoolean ASKState){return 0;}
ViStatus _VI_FUNC rssmf_SetASKSource (ViSession instrumentHandle,
                                      ViInt32 ASKSource){return 0;}
ViStatus _VI_FUNC rssmf_GetASKSource (ViSession instrumentHandle,
                                      ViPInt32 ASKSource){return 0;}
ViStatus _VI_FUNC rssmf_SetASKDepth (ViSession instrumentHandle, ViReal64 ASKDepth){return 0;}
ViStatus _VI_FUNC rssmf_GetASKDepth (ViSession instrumentHandle,
                                     ViPReal64 ASKDepth){return 0;}
ViStatus _VI_FUNC rssmf_configFSK (ViSession instrumentHandle, ViBoolean FSKState,
                                   ViInt32 FSKSource, ViReal64 FSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_SetFSKState (ViSession instrumentHandle,
                                     ViBoolean FSKState){return 0;}
ViStatus _VI_FUNC rssmf_GetFSKState (ViSession instrumentHandle,
                                     ViPBoolean FSKState){return 0;}
ViStatus _VI_FUNC rssmf_SetFSKSource (ViSession instrumentHandle,
                                      ViInt32 FSKSource){return 0;}
ViStatus _VI_FUNC rssmf_GetFSKSource (ViSession instrumentHandle,
                                      ViPInt32 FSKSource){return 0;}
ViStatus _VI_FUNC rssmf_SetFSKDeviation (ViSession instrumentHandle,
                                         ViReal64 FSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_GetFSKDeviation (ViSession instrumentHandle,
                                         ViPReal64 FSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_configPSK (ViSession instrumentHandle, ViBoolean PSKState,
                                   ViInt32 PSKSource, ViReal64 PSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_SetPSKState (ViSession instrumentHandle,
                                     ViBoolean PSKState){return 0;}
ViStatus _VI_FUNC rssmf_GetPSKState (ViSession instrumentHandle,
                                     ViPBoolean PSKState){return 0;}
ViStatus _VI_FUNC rssmf_SetPSKSource (ViSession instrumentHandle,
                                      ViInt32 PSKSource){return 0;}
ViStatus _VI_FUNC rssmf_GetPSKSource (ViSession instrumentHandle,
                                      ViPInt32 PSKSource){return 0;}
ViStatus _VI_FUNC rssmf_SetPSKDeviation (ViSession instrumentHandle,
                                         ViReal64 PSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_GetPSKDeviation (ViSession instrumentHandle,
                                         ViPReal64 PSKDeviation){return 0;}
ViStatus _VI_FUNC rssmf_confCHIRP (ViSession instrumentHandle, ViBoolean CHRIPState,
                                   ViInt32 direction, ViReal64 bandwidth,
                                   ViReal64 pulsePeriod, ViReal64 pulseWidth,
                                   ViInt32 triggerMode, ViInt32 triggerSlope,
                                   ViInt32 impedance){return 0;}
ViStatus _VI_FUNC rssmf_confCHIRPTrigger (ViSession instrumentHandle,
                                          ViInt32 triggerSlope,
                                          ViInt32 gatePolarity, ViInt32 impedance,
                                          ViInt32 triggerMode,
                                          ViInt32 triggerLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPTriggerMode (ViSession instrumentHandle,
                                             ViInt32 triggerMode){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPTriggerMode (ViSession instrumentHandle,
                                             ViPInt32 triggerMode){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPTriggerSlope (ViSession instrumentHandle,
                                              ViInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPTriggerSlope (ViSession instrumentHandle,
                                              ViPInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPTriggerLevel (ViSession instrumentHandle,
                                              ViInt32 triggerLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPTriggerLevel (ViSession instrumentHandle,
                                              ViPInt32 triggerLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPGatePolarity (ViSession instrumentHandle,
                                              ViInt32 gatePolarity){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPGatePolarity (ViSession instrumentHandle,
                                              ViPInt32 gatePolarity){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPExternalImpedance (ViSession instrumentHandle,
                                                   ViInt32 impedance){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPExternalImpedance (ViSession instrumentHandle,
                                                   ViPInt32 impedance){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPDirection (ViSession instrumentHandle,
                                           ViInt32 direction){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPDirection (ViSession instrumentHandle,
                                           ViPInt32 direction){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPState (ViSession instrumentHandle,
                                       ViBoolean CHRIPState){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPState (ViSession instrumentHandle,
                                       ViPBoolean CHRIPState){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPBandwidth (ViSession instrumentHandle,
                                           ViReal64 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPBandwidth (ViSession instrumentHandle,
                                           ViPReal64 bandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPPulsePeriod (ViSession instrumentHandle,
                                             ViReal64 pulsePeriod){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPPulsePeriod (ViSession instrumentHandle,
                                             ViPReal64 pulsePeriod){return 0;}
ViStatus _VI_FUNC rssmf_SetCHIRPPulseWidth (ViSession instrumentHandle,
                                            ViReal64 pulseWidth){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPPulseWidth (ViSession instrumentHandle,
                                            ViPReal64 pulseWidth){return 0;}
ViStatus _VI_FUNC rssmf_GetCHIRPCompressionRatio (ViSession instrumentHandle,
                                                  ViPReal64 compressionRatio){return 0;}
ViStatus _VI_FUNC rssmf_SetOutputState (ViSession instrumentHandle,
                                        ViBoolean RFOutputState){return 0;}
ViStatus _VI_FUNC rssmf_GetOutputState (ViSession instrumentHandle,
                                        ViPBoolean RFOutputState){return 0;}
ViStatus _VI_FUNC rssmf_confRFFreq (ViSession instrumentHandle, ViReal64 frequency,
                                    ViReal64 offset, ViReal64 multiplier,
                                    ViBoolean variationActive,
                                    ViReal64 variationStep){return 0;}
ViStatus _VI_FUNC rssmf_SetRFFrequency (ViSession instrumentHandle,
                                        ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_GetRFFrequency (ViSession instrumentHandle,
                                        ViPReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_SetRFFrequencyOffset (ViSession instrumentHandle,
                                              ViReal64 frequencyOffset){return 0;}
ViStatus _VI_FUNC rssmf_GetRFFrequencyOffset (ViSession instrumentHandle,
                                              ViPReal64 frequencyOffset){return 0;}
ViStatus _VI_FUNC rssmf_SetRFFrequencyMultiplier (ViSession instrumentHandle,
                                                  ViReal64 multiplier){return 0;}
ViStatus _VI_FUNC rssmf_GetRFFrequencyMultiplier (ViSession instrumentHandle,
                                                  ViPReal64 multiplier){return 0;}
ViStatus _VI_FUNC rssmf_SetRFVariationActive (ViSession instrumentHandle,
                                              ViBoolean variationActive){return 0;}
ViStatus _VI_FUNC rssmf_GetRFVariationActive (ViSession instrumentHandle,
                                              ViPBoolean variationActive){return 0;}
ViStatus _VI_FUNC rssmf_SetRFVariationStep (ViSession instrumentHandle,
                                            ViReal64 variationStep){return 0;}
ViStatus _VI_FUNC rssmf_GetRFVariationStep (ViSession instrumentHandle,
                                            ViPReal64 variationStep){return 0;}
ViStatus _VI_FUNC rssmf_SetRFFrequencyonRecall (ViSession instrumentHandle,
                                                ViInt32 RFFrequencyOnRecall){return 0;}
ViStatus _VI_FUNC rssmf_GetRFFrequencyonRecall (ViSession instrumentHandle,
                                                ViPInt32 RFFrequencyOnRecall){return 0;}
ViStatus _VI_FUNC rssmf_SetRFDeltaPhase (ViSession instrumentHandle,
                                         ViReal64 deltaPhase){return 0;}
ViStatus _VI_FUNC rssmf_GetRFDeltaPhase (ViSession instrumentHandle,
                                         ViPReal64 deltaPhase){return 0;}
ViStatus _VI_FUNC rssmf_ResetRFDeltaPhaseDisplay (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_confFrequencySweep (ViSession instrumentHandle,
                                            ViInt32 mode, ViInt32 frequencySetting,
                                            ViReal64 centerFrequency, ViReal64 span,
                                            ViReal64 startFrequency,
                                            ViReal64 stopFrequency, ViInt32 spacing,
                                            ViReal64 step, ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_FrequencySweepExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetInstrumentTriggerInputSlope (ViSession instrumentHandle,
                                                        ViInt32 inputTriggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureFrequencySweepMarker (ViSession instrumentHandle,
                                                       ViUInt32 channel,
                                                       ViBoolean state,
                                                       ViBoolean amplitudeMarker,
                                                       ViReal64 frequency,
                                                       ViReal64 amplitude,
                                                       ViInt32 polarity,
                                                       ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepMode (ViSession instrumentHandle,
                                               ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepStartFreq (ViSession instrumentHandle,
                                                    ViReal64 startFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepStartFreq (ViSession instrumentHandle,
                                                    ViPReal64 startFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepStopFreq (ViSession instrumentHandle,
                                                   ViReal64 stopFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepStopFreq (ViSession instrumentHandle,
                                                   ViPReal64 stopFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepCenterFreq (ViSession instrumentHandle,
                                                     ViReal64 centerFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepCenterFreq (ViSession instrumentHandle,
                                                     ViPReal64 centerFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepSpan (ViSession instrumentHandle,
                                               ViReal64 span){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepSpan (ViSession instrumentHandle,
                                               ViPReal64 span){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepSpacing (ViSession instrumentHandle,
                                                  ViInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepSpacing (ViSession instrumentHandle,
                                                  ViPInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepStepLin (ViSession instrumentHandle,
                                                  ViReal64 stepLin){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepStepLin (ViSession instrumentHandle,
                                                  ViPReal64 stepLin){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepStepLog (ViSession instrumentHandle,
                                                  ViReal64 stepLog){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepStepLog (ViSession instrumentHandle,
                                                  ViPReal64 stepLog){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepTime (ViSession instrumentHandle,
                                               ViReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepTime (ViSession instrumentHandle,
                                               ViPReal64 time){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepRampTime (ViSession instrumentHandle,
                                                   ViReal64 rampTime){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepRampTime (ViSession instrumentHandle,
                                                   ViPReal64 rampTime){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepDwellTime (ViSession instrumentHandle,
                                                    ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepDwellTime (ViSession instrumentHandle,
                                                    ViPReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepManualStep (ViSession instrumentHandle,
                                                     ViReal64 manualStep){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepManualStep (ViSession instrumentHandle,
                                                     ViPReal64 manualStep){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepPoints (ViSession instrumentHandle,
                                                 ViInt32 frequencySweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepPoints (ViSession instrumentHandle,
                                                 ViPInt32 frequencySweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetInstrumentTriggerInputSlope (ViSession instrumentHandle,
                                                        ViPInt32 inputTriggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepShape (ViSession instrumentHandle,
                                                ViInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepShape (ViSession instrumentHandle,
                                                ViPInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepMarkerState (ViSession instrumentHandle,
                                                      ViUInt32 channel,
                                                      ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepMarkerState (ViSession instrumentHandle,
                                                      ViUInt32 channel,
                                                      ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepMarkerFrequency (ViSession instrumentHandle,
                                                          ViUInt32 channel,
                                                          ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepMarkerFrequency (ViSession instrumentHandle,
                                                          ViUInt32 channel,
                                                          ViPReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepAmplitudeMarkerState
             (ViSession instrumentHandle, ViUInt32 channel,
              ViBoolean amplitudeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepAmplitudeMarkerState
             (ViSession instrumentHandle, ViUInt32 channel,
              ViPBoolean amplitudeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepAmplitudeMarkerAttenuation
             (ViSession instrumentHandle, ViReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepAmplitudeMarkerAttenuation
             (ViSession instrumentHandle, ViPReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepMarkerPolarity (ViSession instrumentHandle,
                                                         ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepMarkerPolarity (ViSession instrumentHandle,
                                                         ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepActiveMarker (ViSession instrumentHandle,
                                                       ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetFrequencySweepActiveMarker (ViSession instrumentHandle,
                                                       ViPInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepAllMarkersOff (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetFrequencySweepMarkerTransfer
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_confList (ViSession instrumentHandle, ViString listName,
                                  ViInt32 listMode, ViInt32 arraySize,
                                  ViReal64 _VI_FAR frequencyValues[],
                                  ViReal64 _VI_FAR powerValues[],
                                  ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_LISTCatalog (ViSession instrumentHandle,
                                     ViString directoryPath, ViInt32 arraySize,
                                     ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_LISTDeleteList (ViSession instrumentHandle,
                                        ViString listName){return 0;}
ViStatus _VI_FUNC rssmf_LISTDeleteAll (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_LISTResetList (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_LISTTriggerExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTMode (ViSession instrumentHandle, ViInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTDwellTime (ViSession instrumentHandle,
                                          ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTDwellTime (ViSession instrumentHandle,
                                          ViPReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTSelectList (ViSession instrumentHandle,
                                           ViString listName){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTSelectList (ViSession instrumentHandle,
                                           ViChar _VI_FAR listName[]){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTFrequencyValues (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViReal64 _VI_FAR frequencyValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTFrequencyValues (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViReal64 _VI_FAR frequencyValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTFrequencyPoints (ViSession instrumentHandle,
                                                ViPInt32 numberOfFrequencyPoints){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTPowerValues (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViReal64 _VI_FAR powerValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTPowerValues (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViReal64 _VI_FAR powerValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTPowerPoints (ViSession instrumentHandle,
                                            ViPInt32 numberOfPowerPoints){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTAttenuatorMode (ViSession instrumentHandle,
                                               ViInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTAttenuatorMode (ViSession instrumentHandle,
                                               ViPInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTStepIndex (ViSession instrumentHandle,
                                          ViInt32 stepIndex){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTStepIndex (ViSession instrumentHandle,
                                          ViPInt32 stepIndex){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTStartIndex (ViSession instrumentHandle,
                                           ViInt32 startIndex){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTStartIndex (ViSession instrumentHandle,
                                           ViPInt32 startIndex){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTStopIndex (ViSession instrumentHandle,
                                          ViInt32 stopIndex){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTStopIndex (ViSession instrumentHandle,
                                          ViPInt32 stopIndex){return 0;}
ViStatus _VI_FUNC rssmf_LearnLISTModeData (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportListMode (ViSession instrumentHandle,
                                                     ViInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportListMode (ViSession instrumentHandle,
                                                     ViPInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportListFile (ViSession instrumentHandle,
                                                     ViString sourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportListFile (ViSession instrumentHandle,
                                                     ViChar _VI_FAR sourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_ImportExportExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportASCIIFile (ViSession instrumentHandle,
                                                      ViString ASCIISourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportASCIIFile (ViSession instrumentHandle,
                                                      ViChar _VI_FAR ASCIISourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportASCIIExtension
             (ViSession instrumentHandle, ViInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportASCIIExtension
             (ViSession instrumentHandle, ViPInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViPInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_SetLISTImportExportASCIIColumnDecimalSeparator
             (ViSession instrumentHandle, ViInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_GetLISTImportExportASCIIColumnDecimalSeparator
             (ViSession instrumentHandle, ViPInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_LISTImportExportCatalog (ViSession instrumentHandle,
                                                 ViInt32 arraySize,
                                                 ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_confRosc (ViSession instrumentHandle, ViInt32 ROSCSource,
                                  ViInt32 externalReferenceFrequency,
                                  ViInt32 ROSCSynchronizationBandwidth,
                                  ViInt32 ROSCOutput, ViBoolean EFCState,
                                  ViBoolean adjustmentActive,
                                  ViInt32 adjustmentFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCSource (ViSession instrumentHandle,
                                       ViInt32 ROSCSource){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCSource (ViSession instrumentHandle,
                                       ViPInt32 ROSCSource){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCRFOutput (ViSession instrumentHandle,
                                         ViBoolean RFOutput){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCRFOutput (ViSession instrumentHandle,
                                         ViPBoolean RFOutput){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCExternalReferenceFrequency
             (ViSession instrumentHandle, ViInt32 externalReferenceFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCExternalReferenceFrequency
             (ViSession instrumentHandle, ViPInt32 externalReferenceFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCSynchronizationBandwidth (ViSession instrumentHandle,
                                                         ViInt32 ROSCSynchronizationBandwidth){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCSynchronizationBandwidth (ViSession instrumentHandle,
                                                         ViPInt32 ROSCSynchronizationBandwidth){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCOutput (ViSession instrumentHandle,
                                       ViInt32 ROSCOutput){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCOutput (ViSession instrumentHandle,
                                       ViPInt32 ROSCOutput){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCEFCState (ViSession instrumentHandle,
                                         ViBoolean EFCState){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCEFCState (ViSession instrumentHandle,
                                         ViPBoolean EFCState){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCAdjustmentActive (ViSession instrumentHandle,
                                                 ViBoolean adjustmentActive){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCAdjustmentActive (ViSession instrumentHandle,
                                                 ViPBoolean adjustmentActive){return 0;}
ViStatus _VI_FUNC rssmf_SetROSCAdjustmentFrequency (ViSession instrumentHandle,
                                                    ViInt32 adjustmentFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetROSCAdjustmentFrequency (ViSession instrumentHandle,
                                                    ViPInt32 adjustmentFrequency){return 0;}
ViStatus _VI_FUNC rssmf_confPowerSense (ViSession instrumentHandle, ViInt32 sense,
                                        ViBoolean initPowerMeas, ViInt32 unit,
                                        ViBoolean permanentDisplay, ViInt32 source,
                                        ViReal64 frequency, ViInt32 filterMode,
                                        ViBoolean useSParameters){return 0;}
ViStatus _VI_FUNC rssmf_confPowerResponseMeasurement (ViSession instrumentHandle,
                                                      ViInt32 sense,
                                                      ViInt32 sweepMode,
                                                      ViReal64 start, ViReal64 stop,
                                                      ViInt32 timing, ViInt32 steps,
                                                      ViInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_confPowerResponseMeasurementDiagram
             (ViSession instrumentHandle, ViInt32 sense,
              ViReal64 frequencyYScaleMinimum, ViReal64 frequencyYScaleMaximum,
              ViReal64 powerYScaleMinimum, ViReal64 powerYScaleMaximum,
              ViInt32 senseMode){return 0;}
ViStatus _VI_FUNC rssmf_PowerZero (ViSession instrumentHandle, ViInt32 sense){return 0;}
ViStatus _VI_FUNC rssmf_SetInitiatePowerMeasurement (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViBoolean initPowerMeas){return 0;}
ViStatus _VI_FUNC rssmf_GetInitiatePowerMeasurement (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViPBoolean initPowerMeas){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerUseSParameters (ViSession instrumentHandle,
                                                ViInt32 sense,
                                                ViPBoolean useSParameters){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerFrequency (ViSession instrumentHandle,
                                           ViInt32 sense, ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFrequency (ViSession instrumentHandle,
                                           ViInt32 sense, ViPReal64 frequency){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerOffset (ViSession instrumentHandle, ViInt32 sense,
                                        ViReal64 offset){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerOffset (ViSession instrumentHandle, ViInt32 sense,
                                        ViPReal64 offset){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerOffsetState (ViSession instrumentHandle,
                                             ViInt32 sense, ViBoolean offsetState){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerOffsetState (ViSession instrumentHandle,
                                             ViInt32 sense, ViPBoolean offsetState){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSource (ViSession instrumentHandle, ViInt32 sense,
                                        ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSource (ViSession instrumentHandle, ViInt32 sense,
                                        ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerUnit (ViSession instrumentHandle, ViInt32 sense,
                                      ViInt32 unit){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerUnit (ViSession instrumentHandle, ViInt32 sense,
                                      ViPInt32 unit){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerDisplayPermanentState (ViSession instrumentHandle,
                                                       ViInt32 sense,
                                                       ViBoolean permanentDisplay){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerDisplayPermanentState (ViSession instrumentHandle,
                                                       ViInt32 sense,
                                                       ViPBoolean permanentDisplay){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerDisplayPermanentPriority (ViSession instrumentHandle,
                                                          ViInt32 trace,
                                                          ViInt32 priority){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerDisplayPermanentPriority (ViSession instrumentHandle,
                                                          ViInt32 trace,
                                                          ViPInt32 priority){return 0;}
ViStatus _VI_FUNC rssmf_confPowerFilter (ViSession instrumentHandle, ViInt32 sense,
                                         ViInt32 filterMode, ViReal64 noiseContent,
                                         ViReal64 timeout){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerFilterLength (ViSession instrumentHandle,
                                              ViInt32 sense, ViInt32 filterLength){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFilterLength (ViSession instrumentHandle,
                                              ViInt32 sense, ViPInt32 filterLength){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerFilterMode (ViSession instrumentHandle,
                                            ViInt32 sense, ViInt32 filterMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFilterMode (ViSession instrumentHandle,
                                            ViInt32 sense, ViPInt32 filterMode){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerFilterFixedNoiseContent (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViReal64 noiseContent){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFilterFixedNoiseContent (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViPReal64 noiseContent){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerFilterFixedNoiseTimeout (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViReal64 timeout){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFilterFixedNoiseTimeout (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViPReal64 timeout){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerFilterLengthAuto (ViSession instrumentHandle,
                                                  ViInt32 sense,
                                                  ViPInt32 filterLengthAuto){return 0;}
ViStatus _VI_FUNC rssmf_PowerFilterSearchOnce (ViSession instrumentHandle,
                                               ViInt32 sense){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorGateState (ViSession instrumentHandle,
                                                 ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGateState (ViSession instrumentHandle,
                                                 ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorGateStart (ViSession instrumentHandle,
                                                 ViInt32 gate, ViReal64 start){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGateStart (ViSession instrumentHandle,
                                                 ViInt32 gate, ViPReal64 start){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorGateStop (ViSession instrumentHandle,
                                                ViInt32 gate, ViReal64 stop){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGateStop (ViSession instrumentHandle,
                                                ViInt32 gate, ViPReal64 stop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorGateTrace (ViSession instrumentHandle,
                                                 ViInt32 trace){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGateTrace (ViSession instrumentHandle,
                                                 ViPInt32 trace){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGateAveragePower (ViSession instrumentHandle,
                                                        ViInt32 gate,
                                                        ViPReal64 averagePower){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorGatePeakPower (ViSession instrumentHandle,
                                                     ViInt32 gate,
                                                     ViPReal64 peakPower){return 0;}
ViStatus _VI_FUNC rssmf_PowerSweepInitiate (ViSession instrumentHandle,
                                            ViInt32 sense, ViInt32 timeout){return 0;}
ViStatus _VI_FUNC rssmf_PowerSweepAbort (ViSession instrumentHandle, ViInt32 sense){return 0;}
ViStatus _VI_FUNC rssmf_confPowerSweepFrequencySeparateFrequencyRange
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state,
              ViReal64 separateFrequencyStart, ViReal64 separateFrequencyStop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyStart (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViReal64 frequencyStart){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyStart (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViPReal64 frequencyStart){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyStop (ViSession instrumentHandle,
                                                    ViInt32 sense,
                                                    ViReal64 frequencyStop){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyStop (ViSession instrumentHandle,
                                                    ViInt32 sense,
                                                    ViPReal64 frequencyStop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySlope (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySlope (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViPInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyTiming (ViSession instrumentHandle,
                                                      ViInt32 sense,
                                                      ViInt32 frequencyTiming){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyTiming (ViSession instrumentHandle,
                                                      ViInt32 sense,
                                                      ViPInt32 frequencyTiming){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySpacing (ViSession instrumentHandle,
                                                       ViInt32 sense,
                                                       ViInt32 frequencySpacing){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySpacing (ViSession instrumentHandle,
                                                       ViInt32 sense,
                                                       ViPInt32 frequencySpacing){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySteps (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViInt32 frequencySteps){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySteps (ViSession instrumentHandle,
                                                     ViInt32 sense,
                                                     ViPInt32 frequencySteps){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyYScaleMinimum
             (ViSession instrumentHandle, ViInt32 sense, ViReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyYScaleMinimum
             (ViSession instrumentHandle, ViInt32 sense, ViPReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyYScaleMaximum
             (ViSession instrumentHandle, ViInt32 sense, ViReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyYScaleMaximum
             (ViSession instrumentHandle, ViInt32 sense, ViPReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyExecution (ViSession instrumentHandle,
                                                         ViInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyExecution (ViSession instrumentHandle,
                                                         ViPInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyYAutoScale (ViSession instrumentHandle,
                                                          ViInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyYAutoScale (ViSession instrumentHandle,
                                                          ViPInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_ResetPowerSweepFrequencyYScale (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySeparateFrequencyRange
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySeparateFrequencyRange
             (ViSession instrumentHandle, ViInt32 sense, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySeparateFrequencyStart
             (ViSession instrumentHandle, ViInt32 sense,
              ViReal64 separateFrequencyStart){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySeparateFrequencyStart
             (ViSession instrumentHandle, ViInt32 sense,
              ViPReal64 separateFrequencyStart){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencySeparateFrequencyStop
             (ViSession instrumentHandle, ViInt32 sense,
              ViReal64 separateFrequencyStop){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencySeparateFrequencyStop
             (ViSession instrumentHandle, ViInt32 sense,
              ViPReal64 separateFrequencyStop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyReferenceXValues
             (ViSession instrumentHandle, ViReal64 frequencyXPointA,
              ViReal64 frequencyXPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyReferenceXValues
             (ViSession instrumentHandle, ViPReal64 frequencyXPointA,
              ViPReal64 frequencyXPointB){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepFrequencyReferenceYValues
             (ViSession instrumentHandle, ViReal64 powerYPointA,
              ViReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepFrequencyReferenceYValues
             (ViSession instrumentHandle, ViPReal64 powerYPointA,
              ViPReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GeneratePowerSweepFrequencyReferenceCurve
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_confPowerSweepPowerSeparateFrequencyRange
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state,
              ViReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerStart (ViSession instrumentHandle,
                                                 ViInt32 sense,
                                                 ViReal64 powerStart){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerStart (ViSession instrumentHandle,
                                                 ViInt32 sense,
                                                 ViPReal64 powerStart){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerStop (ViSession instrumentHandle,
                                                ViInt32 sense, ViReal64 powerStop){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerStop (ViSession instrumentHandle,
                                                ViInt32 sense, ViPReal64 powerStop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerSpacing (ViSession instrumentHandle,
                                                   ViInt32 sense,
                                                   ViInt32 powerSpacing){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerSpacing (ViSession instrumentHandle,
                                                   ViInt32 sense,
                                                   ViPInt32 powerSpacing){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerTiming (ViSession instrumentHandle,
                                                  ViInt32 sense,
                                                  ViInt32 powerTiming){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerTiming (ViSession instrumentHandle,
                                                  ViInt32 sense,
                                                  ViPInt32 powerTiming){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerSteps (ViSession instrumentHandle,
                                                 ViInt32 sense, ViInt32 powerSteps){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerSteps (ViSession instrumentHandle,
                                                 ViInt32 sense,
                                                 ViPInt32 powerSteps){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerYScaleMinimum (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerYScaleMinimum (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViPReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerYScaleMaximum (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerYScaleMaximum (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViPReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerExecution (ViSession instrumentHandle,
                                                     ViInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerExecution (ViSession instrumentHandle,
                                                     ViPInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerYAutoScale (ViSession instrumentHandle,
                                                      ViInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerYAutoScale (ViSession instrumentHandle,
                                                      ViPInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_ResetPowerSweepPowerYScale (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerSeparateFrequencyState
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerSeparateFrequencyState
             (ViSession instrumentHandle, ViInt32 sense, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerSeparateFrequency
             (ViSession instrumentHandle, ViInt32 sense,
              ViReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerSeparateFrequency
             (ViSession instrumentHandle, ViInt32 sense,
              ViPReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerReferenceXValues
             (ViSession instrumentHandle, ViReal64 powerXPointA,
              ViReal64 powerXPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerReferenceXValues
             (ViSession instrumentHandle, ViPReal64 powerXPointA,
              ViPReal64 powerXPointB){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepPowerReferenceYValues
             (ViSession instrumentHandle, ViReal64 powerYPointA,
              ViReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerReferenceYValues
             (ViSession instrumentHandle, ViPReal64 powerYPointA,
              ViPReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepPowerReferenceNumberOfPoints
             (ViSession instrumentHandle, ViPReal64 numberOfPoints){return 0;}
ViStatus _VI_FUNC rssmf_GeneratePowerSweepPowerReferenceCurve
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_confPowerSweepTimeSeparateFrequencyRange
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state,
              ViReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimePulseDataAnalysisState
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimePulseDataAnalysisState
             (ViSession instrumentHandle, ViInt32 sense, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimePulseThresholdBase
             (ViSession instrumentHandle, ViInt32 sense, ViInt32 base){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimePulseThresholdBase
             (ViSession instrumentHandle, ViInt32 sense, ViPInt32 base){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimePulseThresholdReferenceLevel
             (ViSession instrumentHandle, ViInt32 sense, ViInt32 mode,
              ViReal64 referenceLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimePulseThresholdReferenceLevel
             (ViSession instrumentHandle, ViInt32 sense, ViInt32 mode,
              ViPReal64 referenceLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerSource (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeTriggerSource (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerDropOutTime
             (ViSession instrumentHandle, ViInt32 sense, ViReal64 dropOutTime){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeTriggerDropOutTime
             (ViSession instrumentHandle, ViInt32 sense, ViPReal64 dropOutTime){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerHysteresis
             (ViSession instrumentHandle, ViInt32 sense, ViReal64 hysteresis){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeTriggerHysteresis
             (ViSession instrumentHandle, ViInt32 sense, ViPReal64 hysteresis){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerThreshold
             (ViSession instrumentHandle, ViInt32 sense, ViReal64 threshold){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeTriggerThreshold
             (ViSession instrumentHandle, ViInt32 sense, ViPReal64 threshold){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerAutoSet (ViSession instrumentHandle,
                                                         ViInt32 sense,
                                                         ViInt32 timeout){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeStart (ViSession instrumentHandle,
                                                ViInt32 sense, ViReal64 timeStart){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeStart (ViSession instrumentHandle,
                                                ViInt32 sense, ViPReal64 timeStart){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeStop (ViSession instrumentHandle,
                                               ViInt32 sense, ViReal64 timeStop){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeStop (ViSession instrumentHandle,
                                               ViInt32 sense, ViPReal64 timeStop){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeTriggerEvents (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViInt32 triggerEvents){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeTriggerEvents (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViPInt32 triggerEvents){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeSpacing (ViSession instrumentHandle,
                                                  ViInt32 sense,
                                                  ViInt32 timeSpacing){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeSpacing (ViSession instrumentHandle,
                                                  ViInt32 sense,
                                                  ViPInt32 timeSpacing){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeAveragingFactor (ViSession instrumentHandle,
                                                          ViInt32 averagingFactor){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeAveragingFactor (ViSession instrumentHandle,
                                                          ViPInt32 averagingFactor){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeSteps (ViSession instrumentHandle,
                                                ViInt32 sense, ViInt32 timeSteps){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeSteps (ViSession instrumentHandle,
                                                ViInt32 sense, ViPInt32 timeSteps){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerTimeTriggerSource (ViSession instrumentHandle,
                                                   ViInt32 sense, ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerTimeTriggerSource (ViSession instrumentHandle,
                                                   ViInt32 sense, ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeYScaleMinimum (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeYScaleMinimum (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViPReal64 yScaleMinimum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeYScaleMaximum (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeYScaleMaximum (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViPReal64 yScaleMaximum){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeExecution (ViSession instrumentHandle,
                                                    ViInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeExecution (ViSession instrumentHandle,
                                                    ViPInt32 execution){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeYAutoScale (ViSession instrumentHandle,
                                                     ViInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeYAutoScale (ViSession instrumentHandle,
                                                     ViPInt32 autoScale){return 0;}
ViStatus _VI_FUNC rssmf_ResetPowerSweepTimeYScale (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeSeparateFrequencyState
             (ViSession instrumentHandle, ViInt32 sense, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeSeparateFrequencyState
             (ViSession instrumentHandle, ViInt32 sense, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeSeparateFrequency
             (ViSession instrumentHandle, ViInt32 sense,
              ViReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeSeparateFrequency
             (ViSession instrumentHandle, ViInt32 sense,
              ViPReal64 separateFrequency){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeReferenceXValues
             (ViSession instrumentHandle, ViReal64 timeXPointA,
              ViReal64 timeXPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeReferenceXValues
             (ViSession instrumentHandle, ViPReal64 timeXPointA,
              ViPReal64 timeXPointB){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTimeReferenceYValues
             (ViSession instrumentHandle, ViReal64 powerYPointA,
              ViReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeReferenceYValues
             (ViSession instrumentHandle, ViPReal64 powerYPointA,
              ViPReal64 powerYPointB){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTimeReferenceNumberOfPoints
             (ViSession instrumentHandle, ViPReal64 numberOfPoints){return 0;}
ViStatus _VI_FUNC rssmf_GeneratePowerSweepTimeReferenceCurve
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTraceState (ViSession instrumentHandle,
                                                 ViInt32 trace, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceState (ViSession instrumentHandle,
                                                 ViInt32 trace, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTraceColor (ViSession instrumentHandle,
                                                 ViInt32 trace, ViInt32 color){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceColor (ViSession instrumentHandle,
                                                 ViInt32 trace, ViPInt32 color){return 0;}
ViStatus _VI_FUNC rssmf_copyPowerSweepTrace (ViSession instrumentHandle,
                                             ViInt32 trace, ViInt32 copyMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceDataPoints (ViSession instrumentHandle,
                                                      ViInt32 trace,
                                                      ViPInt32 dataPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceXValues (ViSession instrumentHandle,
                                                   ViInt32 trace, ViInt32 arraySize,
                                                   ViReal64 _VI_FAR xValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceYValues (ViSession instrumentHandle,
                                                   ViInt32 trace, ViInt32 arraySize,
                                                   ViReal64 _VI_FAR yValues[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepTraceSource (ViSession instrumentHandle,
                                                  ViInt32 trace, ViInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTraceSource (ViSession instrumentHandle,
                                                  ViInt32 trace, ViPInt32 source){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTracePulseDataAnalysisState
             (ViSession instrumentHandle, ViInt32 trace, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTracePulseThresholdBase
             (ViSession instrumentHandle, ViInt32 trace, ViPInt32 base){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepTracePulseThresholdReferenceLevel
             (ViSession instrumentHandle, ViInt32 trace, ViInt32 mode,
              ViPReal64 referenceLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepSenseMode (ViSession instrumentHandle,
                                                ViInt32 sense, ViInt32 senseMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepSenseMode (ViSession instrumentHandle,
                                                ViInt32 sense, ViPInt32 senseMode){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepMode (ViSession instrumentHandle,
                                           ViInt32 sense, ViInt32 sweepMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepMode (ViSession instrumentHandle,
                                           ViInt32 sense, ViPInt32 sweepMode){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepDataPoints (ViSession instrumentHandle,
                                                 ViInt32 sense,
                                                 ViPInt32 dataPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepDataXValues (ViSession instrumentHandle,
                                                  ViInt32 sense, ViInt32 arraySize,
                                                  ViReal64 _VI_FAR dataXValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepDataYValues (ViSession instrumentHandle,
                                                  ViInt32 sense, ViInt32 arraySize,
                                                  ViReal64 _VI_FAR dataYValues[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerDisplayIndication (ViSession instrumentHandle,
                                                   ViInt32 trace, ViInt32 type,
                                                   ViBoolean indication){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerDisplayIndication (ViSession instrumentHandle,
                                                   ViInt32 trace, ViInt32 type,
                                                   ViPBoolean indication){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerDisplayPulseDataAnalysis (ViSession instrumentHandle,
                                                          ViInt32 trace,
                                                          ViInt32 type,
                                                          ViPReal64 pulseDataAnalysis){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorMathematicsState (ViSession instrumentHandle,
                                                        ViInt32 trace,
                                                        ViInt32 sweepMode,
                                                        ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorMathematicsState (ViSession instrumentHandle,
                                                        ViInt32 trace,
                                                        ViInt32 sweepMode,
                                                        ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSensorMathematicsSubtraction
             (ViSession instrumentHandle, ViInt32 resultTrace, ViInt32 sweepMode,
              ViInt32 firstOperand, ViInt32 secondOperand){return 0;}
ViStatus _VI_FUNC rssmf_PowerSensorDevice (ViSession instrumentHandle,
                                           ViInt32 sense, ViPInt32 device){return 0;}
ViStatus _VI_FUNC rssmf_PowerSensorType (ViSession instrumentHandle, ViInt32 sense,
                                         ViChar _VI_FAR type[]){return 0;}
ViStatus _VI_FUNC rssmf_PowerReadPower (ViSession instrumentHandle, ViInt32 read,
                                        ViPReal64 power){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSensorVersion (ViSession instrumentHandle,
                                               ViInt32 sense,
                                               ViChar _VI_FAR version[]){return 0;}
ViStatus _VI_FUNC rssmf_PowerSensorSerialNumber (ViSession instrumentHandle,
                                                 ViInt32 sense,
                                                 ViChar _VI_FAR serialNumber[]){return 0;}
ViStatus _VI_FUNC rssmf_confDiagramDisplay (ViSession instrumentHandle,
                                            ViBoolean grid,
                                            ViInt32 backgroundColor){return 0;}
ViStatus _VI_FUNC rssmf_SetDiagramGrid (ViSession instrumentHandle, ViBoolean grid){return 0;}
ViStatus _VI_FUNC rssmf_GetDiagramGrid (ViSession instrumentHandle,
                                        ViPBoolean grid){return 0;}
ViStatus _VI_FUNC rssmf_SetDiagramBackgroundColor (ViSession instrumentHandle,
                                                   ViInt32 backgroundColor){return 0;}
ViStatus _VI_FUNC rssmf_GetDiagramBackgroundColor (ViSession instrumentHandle,
                                                   ViPInt32 backgroundColor){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyLNGFormat (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViInt32 sweepHardcopyFormat){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyLNGFormat (ViSession instrumentHandle,
                                                        ViInt32 sense,
                                                        ViPInt32 sweepHardcopyFormat){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyFile (ViSession instrumentHandle,
                                                   ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyFile (ViSession instrumentHandle,
                                                   ViChar _VI_FAR fileName[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyDevice (ViSession instrumentHandle,
                                                     ViInt32 hardcopyDevice){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyDevice (ViSession instrumentHandle,
                                                     ViPInt32 hardcopyDevice){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyImageSize (ViSession instrumentHandle,
                                                        ViInt32 imageSize){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyImageSize (ViSession instrumentHandle,
                                                        ViPInt32 imageSize){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyMarkerState (ViSession instrumentHandle,
                                                          ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyMarkerState (ViSession instrumentHandle,
                                                          ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SavePowerSweepHardcopy (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyCSVHeader (ViSession instrumentHandle,
                                                        ViInt32 CSVHeader){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyCSVHeader (ViSession instrumentHandle,
                                                        ViPInt32 CSVHeader){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyCSVOrientation
             (ViSession instrumentHandle, ViInt32 CSVOrientation){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyCSVOrientation
             (ViSession instrumentHandle, ViPInt32 CSVOrientation){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyCSVColumnSeparator
             (ViSession instrumentHandle, ViInt32 CSVColumnSeparator){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyCSVColumnSeparator
             (ViSession instrumentHandle, ViPInt32 CSVColumnSeparator){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyCSVDecimalPoint
             (ViSession instrumentHandle, ViInt32 CSVDecimalPoint){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyCSVDecimalPoint
             (ViSession instrumentHandle, ViPInt32 CSVDecimalPoint){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyCSVData (ViSession instrumentHandle,
                                                      ViString destination){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyAutomaticNamingState
             (ViSession instrumentHandle, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticNamingState
             (ViSession instrumentHandle, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFileName
             (ViSession instrumentHandle, ViChar _VI_FAR fileName[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyAutomaticDirectory
             (ViSession instrumentHandle, ViString directoryName){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticDirectory
             (ViSession instrumentHandle, ViChar _VI_FAR directoryName[]){return 0;}
ViStatus _VI_FUNC rssmf_DeletePowerSweepHardcopyImageFiles
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyAutomaticFileDateState
             (ViSession instrumentHandle, ViInt32 fileNamePart, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFileDateState
             (ViSession instrumentHandle, ViInt32 fileNamePart, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFileDate
             (ViSession instrumentHandle, ViInt32 fileNamePart,
              ViChar _VI_FAR part[]){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyAutomaticFilePrefixState
             (ViSession instrumentHandle, ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFilePrefixState
             (ViSession instrumentHandle, ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerSweepHardcopyAutomaticFilePrefix
             (ViSession instrumentHandle, ViString prefix){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFilePrefix
             (ViSession instrumentHandle, ViChar _VI_FAR prefix[]){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerSweepHardcopyAutomaticFileNumber
             (ViSession instrumentHandle, ViPInt32 number){return 0;}
ViStatus _VI_FUNC rssmf_SetAllRFOutputsState (ViSession instrumentHandle,
                                              ViBoolean RFOutputStates){return 0;}
ViStatus _VI_FUNC rssmf_confRFLevel (ViSession instrumentHandle, ViReal64 amplitude,
                                     ViReal64 offset, ViReal64 limit,
                                     ViInt32 powerResolution,
                                     ViBoolean variationActive,
                                     ViReal64 variationStep,
                                     ViInt32 RFLevelOnRecall){return 0;}
ViStatus _VI_FUNC rssmf_SetRFAmplitude (ViSession instrumentHandle,
                                        ViReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_GetRFAmplitude (ViSession instrumentHandle,
                                        ViPReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_SetRFAmplitudeIgnoreOffset (ViSession instrumentHandle,
                                                    ViReal64 amplitude){return 0;}
ViStatus _VI_FUNC rssmf_SetRFLevelLimit (ViSession instrumentHandle,
                                         ViReal64 limit){return 0;}
ViStatus _VI_FUNC rssmf_GetRFLevelLimit (ViSession instrumentHandle,
                                         ViPReal64 limit){return 0;}
ViStatus _VI_FUNC rssmf_SetRFLevelOffset (ViSession instrumentHandle,
                                          ViReal64 offset){return 0;}
ViStatus _VI_FUNC rssmf_GetRFLevelOffset (ViSession instrumentHandle,
                                          ViPReal64 offset){return 0;}
ViStatus _VI_FUNC rssmf_SetRFPowerResolution (ViSession instrumentHandle,
                                              ViInt32 powerResolution){return 0;}
ViStatus _VI_FUNC rssmf_GetRFPowerResolution (ViSession instrumentHandle,
                                              ViPInt32 powerResolution){return 0;}
ViStatus _VI_FUNC rssmf_SetRFLevelonRecall (ViSession instrumentHandle,
                                            ViInt32 RFLevelOnRecall){return 0;}
ViStatus _VI_FUNC rssmf_GetRFLevelonRecall (ViSession instrumentHandle,
                                            ViPInt32 RFLevelOnRecall){return 0;}
ViStatus _VI_FUNC rssmf_SetRFLevelVariationActive (ViSession instrumentHandle,
                                                   ViBoolean variationActive){return 0;}
ViStatus _VI_FUNC rssmf_GetRFLevelVariationActive (ViSession instrumentHandle,
                                                   ViPBoolean variationActive){return 0;}
ViStatus _VI_FUNC rssmf_SetRFLevelVariationStep (ViSession instrumentHandle,
                                                 ViReal64 variationStep){return 0;}
ViStatus _VI_FUNC rssmf_GetRFLevelVariationStep (ViSession instrumentHandle,
                                                 ViPReal64 variationStep){return 0;}
ViStatus _VI_FUNC rssmf_SetRFPowerOnState (ViSession instrumentHandle,
                                           ViInt32 powerOnState){return 0;}
ViStatus _VI_FUNC rssmf_GetRFPowerOnState (ViSession instrumentHandle,
                                           ViPInt32 powerOnState){return 0;}
ViStatus _VI_FUNC rssmf_SetALCState (ViSession instrumentHandle,
                                     ViBoolean ALCState){return 0;}
ViStatus _VI_FUNC rssmf_GetALCState (ViSession instrumentHandle,
                                     ViPBoolean ALCState){return 0;}
ViStatus _VI_FUNC rssmf_SetALCRFPowerDuringPowerSearch (ViSession instrumentHandle,
                                                        ViInt32 RFPowerDuringSearch){return 0;}
ViStatus _VI_FUNC rssmf_GetALCRFPowerDuringPowerSearch (ViSession instrumentHandle,
                                                        ViPInt32 RFPowerDuringSearch){return 0;}
ViStatus _VI_FUNC rssmf_ALCSearchOnce (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetALCDetector (ViSession instrumentHandle,
                                        ViInt32 detector){return 0;}
ViStatus _VI_FUNC rssmf_GetALCDetector (ViSession instrumentHandle,
                                        ViPInt32 detector){return 0;}
ViStatus _VI_FUNC rssmf_SetALCPowerSensorType (ViSession instrumentHandle,
                                               ViInt32 powerSensorType){return 0;}
ViStatus _VI_FUNC rssmf_GetALCPowerSensorType (ViSession instrumentHandle,
                                               ViPInt32 powerSensorType){return 0;}
ViStatus _VI_FUNC rssmf_SetALCReferenceVoltage (ViSession instrumentHandle,
                                                ViReal64 referenceVoltage){return 0;}
ViStatus _VI_FUNC rssmf_GetALCReferenceVoltage (ViSession instrumentHandle,
                                                ViPReal64 referenceVoltage){return 0;}
ViStatus _VI_FUNC rssmf_SetUserCorrectionDataState (ViSession instrumentHandle,
                                                    ViBoolean correctionState){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataState (ViSession instrumentHandle,
                                                    ViPBoolean correctionState){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionCurrentValue (ViSession instrumentHandle,
                                                       ViPReal64 correctionCurrentValue){return 0;}
ViStatus _VI_FUNC rssmf_TriggerUserCorrectionMeasurement
             (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataCatalog (ViSession instrumentHandle,
                                                      ViString directoryPath,
                                                      ViInt32 arraySize,
                                                      ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_SetUserCorrectionDataTable (ViSession instrumentHandle,
                                                    ViString correctionTable){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataTable (ViSession instrumentHandle,
                                                    ViChar _VI_FAR correctionTable[]){return 0;}
ViStatus _VI_FUNC rssmf_SetUserCorrectionDataFrequencyValues
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR frequencyValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataFrequencyValues
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViReal64 _VI_FAR frequencyValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataNumberofFrequencyValues
             (ViSession instrumentHandle, ViPInt32 numberOfFrequencyValues){return 0;}
ViStatus _VI_FUNC rssmf_SetUserCorrectionDataLevelValues (ViSession instrumentHandle,
                                                          ViInt32 arraySize,
                                                          ViReal64 _VI_FAR levelValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataLevelValues (ViSession instrumentHandle,
                                                          ViInt32 arraySize,
                                                          ViReal64 _VI_FAR levelValues[]){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionDataNumberofLevelValues
             (ViSession instrumentHandle, ViPInt32 numberOfLevelValues){return 0;}
ViStatus _VI_FUNC rssmf_SetUserCorrectionPowerSensorType (ViSession instrumentHandle,
                                                          ViInt32 powerSensorType){return 0;}
ViStatus _VI_FUNC rssmf_GetUserCorrectionPowerSensorType (ViSession instrumentHandle,
                                                          ViPInt32 powerSensorType){return 0;}
ViStatus _VI_FUNC rssmf_UserCorrectionFillListWithSensor (ViSession instrumentHandle,
                                                          ViInt32 sensor,
                                                          ViInt32 timeout){return 0;}
ViStatus _VI_FUNC rssmf_UserCorrectionDataDeleteTable (ViSession instrumentHandle,
                                                       ViString tableName){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportListMode
             (ViSession instrumentHandle, ViInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportListMode
             (ViSession instrumentHandle, ViPInt32 listMode){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportListFile
             (ViSession instrumentHandle, ViString sourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportListFile
             (ViSession instrumentHandle, ViChar _VI_FAR sourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_CORRectionImportExportExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportASCIIFile
             (ViSession instrumentHandle, ViString ASCIISourceFile){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportASCIIFile
             (ViSession instrumentHandle, ViChar _VI_FAR ASCIISourceFile[]){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportASCIIExtension
             (ViSession instrumentHandle, ViInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportASCIIExtension
             (ViSession instrumentHandle, ViPInt32 extension){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportASCIIColumnSeparator
             (ViSession instrumentHandle, ViPInt32 separator){return 0;}
ViStatus _VI_FUNC rssmf_SetCORRectionImportExportASCIIColumnDecimalSeparator
             (ViSession instrumentHandle, ViInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_GetCORRectionImportExportASCIIColumnDecimalSeparator
             (ViSession instrumentHandle, ViPInt32 decimalSeparator){return 0;}
ViStatus _VI_FUNC rssmf_CORRectionImportExportCatalog (ViSession instrumentHandle,
                                                       ViString directoryPath,
                                                       ViInt32 arraySize,
                                                       ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_confRFLevelSweep (ViSession instrumentHandle,
                                          ViInt32 levelSweepMode,
                                          ViReal64 startLevel, ViReal64 stopLevel,
                                          ViReal64 stepLin, ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_LevelSweepExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_ConfigureLevelSweepMarker (ViSession instrumentHandle,
                                                   ViUInt32 channel,
                                                   ViBoolean state, ViReal64 level,
                                                   ViInt32 polarity,
                                                   ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepMode (ViSession instrumentHandle,
                                           ViInt32 mode){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepStartLevel (ViSession instrumentHandle,
                                                 ViReal64 startLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepStartLevel (ViSession instrumentHandle,
                                                 ViPReal64 startLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepStopLevel (ViSession instrumentHandle,
                                                ViReal64 stopLevel){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepStopLevel (ViSession instrumentHandle,
                                                ViPReal64 stopLevel){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepStep (ViSession instrumentHandle,
                                           ViReal64 step){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepStep (ViSession instrumentHandle,
                                           ViPReal64 step){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepDwell (ViSession instrumentHandle,
                                            ViReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepDwell (ViSession instrumentHandle,
                                            ViPReal64 dwellTime){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepManualStep (ViSession instrumentHandle,
                                                 ViReal64 manualStep){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepManualStep (ViSession instrumentHandle,
                                                 ViPReal64 manualStep){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepPoints (ViSession instrumentHandle,
                                             ViInt32 levelSweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepPoints (ViSession instrumentHandle,
                                             ViPInt32 levelSweepPoints){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepAttenuatorMode (ViSession instrumentHandle,
                                                     ViInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepAttenuatorMode (ViSession instrumentHandle,
                                                     ViPInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepShape (ViSession instrumentHandle,
                                            ViInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepShape (ViSession instrumentHandle,
                                            ViPInt32 shape){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepSpacing (ViSession instrumentHandle,
                                              ViPInt32 spacing){return 0;}
ViStatus _VI_FUNC rssmf_ResetLevelSweep (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepMarkerState (ViSession instrumentHandle,
                                                  ViUInt32 channel,
                                                  ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepMarkerState (ViSession instrumentHandle,
                                                  ViUInt32 channel,
                                                  ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepMarkerLevel (ViSession instrumentHandle,
                                                  ViUInt32 channel, ViReal64 level){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepMarkerLevel (ViSession instrumentHandle,
                                                  ViUInt32 channel,
                                                  ViPReal64 level){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepMarkerPolarity (ViSession instrumentHandle,
                                                     ViInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepMarkerPolarity (ViSession instrumentHandle,
                                                     ViPInt32 polarity){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepActiveMarker (ViSession instrumentHandle,
                                                   ViInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_GetLevelSweepActiveMarker (ViSession instrumentHandle,
                                                   ViPInt32 activeMarker){return 0;}
ViStatus _VI_FUNC rssmf_SetLevelSweepAllMarkersOff (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetRFAttenuatorMode (ViSession instrumentHandle,
                                             ViInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_GetRFAttenuatorMode (ViSession instrumentHandle,
                                             ViPInt32 attenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_SetRFAttenuation (ViSession instrumentHandle,
                                          ViReal64 attenuation){return 0;}
ViStatus _VI_FUNC rssmf_GetRFAttenuation (ViSession instrumentHandle,
                                          ViPReal64 attenuation){return 0;}
ViStatus _VI_FUNC rssmf_GetRFAttenuatorLowerRange (ViSession instrumentHandle,
                                                   ViPReal64 attenuatorLowerRange){return 0;}
ViStatus _VI_FUNC rssmf_GetRFAttenuatorUpperRange (ViSession instrumentHandle,
                                                   ViPReal64 attenuatorUpperRange){return 0;}
ViStatus _VI_FUNC rssmf_SetRFOFFAttenuatorMode (ViSession instrumentHandle,
                                                ViInt32 OFFAttenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_GetRFOFFAttenuatorMode (ViSession instrumentHandle,
                                                ViPInt32 OFFAttenuatorMode){return 0;}
ViStatus _VI_FUNC rssmf_CALibrationAll (ViSession instrumentHandle, ViInt32 timeout,
                                        ViPInt32 response){return 0;}
ViStatus _VI_FUNC rssmf_CALibrationFrequency (ViSession instrumentHandle,
                                              ViInt32 timeout, ViPInt32 response){return 0;}
ViStatus _VI_FUNC rssmf_CALibrationModulationGenerators (ViSession instrumentHandle,
                                                         ViInt32 timeout,
                                                         ViPInt32 response){return 0;}
ViStatus _VI_FUNC rssmf_CALibrationLevel (ViSession instrumentHandle,
                                          ViInt32 timeout, ViPInt32 response){return 0;}
ViStatus _VI_FUNC rssmf_SetCALibrationLevelState (ViSession instrumentHandle,
                                                  ViBoolean levelState){return 0;}
ViStatus _VI_FUNC rssmf_GetCALibrationLevelState (ViSession instrumentHandle,
                                                  ViPBoolean levelState){return 0;}
ViStatus _VI_FUNC rssmf_SetCALibrationLevelLoopGainState (ViSession instrumentHandle,
                                                          ViBoolean levelLoopGainState){return 0;}
ViStatus _VI_FUNC rssmf_GetCALibrationLevelLoopGainState (ViSession instrumentHandle,
                                                          ViPBoolean levelLoopGainState){return 0;}
ViStatus _VI_FUNC rssmf_ClearStatus (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_EventStatusEnable (ViSession instrumentHandle,
                                           ViInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_EventStatusQuery (ViSession instrumentHandle,
                                          ViPInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_IdentificationQuery (ViSession instrumentHandle,
                                             ViChar _VI_FAR instrumentIdentification[]){return 0;}
ViStatus _VI_FUNC rssmf_IndividualStatusQuery (ViSession instrumentHandle,
                                               ViPInt32 ISTFlag){return 0;}
ViStatus _VI_FUNC rssmf_OperationComplete (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_OperationCompleteQuery (ViSession instrumentHandle,
                                                ViPInt32 OPCState){return 0;}
ViStatus _VI_FUNC rssmf_OptionIdentificationQuery (ViSession instrumentHandle,
                                                   ViChar _VI_FAR optionIdentification[]){return 0;}
ViStatus _VI_FUNC rssmf_ParallelPollRegisterEnable (ViSession instrumentHandle,
                                                    ViInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_ParallelPollRegisterQuery (ViSession instrumentHandle,
                                                   ViPInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_PowerOnStatusClear (ViSession instrumentHandle,
                                            ViInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_PowerOnStatusClearQuery (ViSession instrumentHandle,
                                                 ViPInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_Recall (ViSession instrumentHandle,
                                ViInt32 instrumentStatus){return 0;}
ViStatus _VI_FUNC rssmf_Save (ViSession instrumentHandle, ViInt32 instrumentStatus){return 0;}
ViStatus _VI_FUNC rssmf_ServiceRequestEnable (ViSession instrumentHandle,
                                              ViInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_ServiceRequestEnableQuery (ViSession instrumentHandle,
                                                   ViPInt32 registerValue){return 0;}
ViStatus _VI_FUNC rssmf_StatusByteQuery (ViSession instrumentHandle,
                                         ViPInt32 statusByte){return 0;}
ViStatus _VI_FUNC rssmf_Trigger (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_TriggerWaitOPC (ViSession instrumentHandle,
                                        ViInt32 timeout){return 0;}
ViStatus _VI_FUNC rssmf_SelfTestQuery (ViSession instrumentHandle,
                                       ViPInt32 errorCode){return 0;}
ViStatus _VI_FUNC rssmf_WaittoContinue (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_PresetSignalPath (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetImageFileName (ViSession instrumentHandle,
                                          ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_GetImageFileName (ViSession instrumentHandle,
                                          ViChar _VI_FAR fileName[]){return 0;}
ViStatus _VI_FUNC rssmf_SetImageFormat (ViSession instrumentHandle,
                                        ViInt32 imageFormat){return 0;}
ViStatus _VI_FUNC rssmf_GetImageFormat (ViSession instrumentHandle,
                                        ViPInt32 imageFormat){return 0;}
ViStatus _VI_FUNC rssmf_GetImageSnapshot (ViSession instrumentHandle,
                                          ViString destination){return 0;}
ViStatus _VI_FUNC rssmf_SetImageSize (ViSession instrumentHandle,
                                      ViInt32 imageSize){return 0;}
ViStatus _VI_FUNC rssmf_GetImageSize (ViSession instrumentHandle,
                                      ViPInt32 imageSize){return 0;}
ViStatus _VI_FUNC rssmf_SetOutputDevice (ViSession instrumentHandle,
                                         ViInt32 outputDevice){return 0;}
ViStatus _VI_FUNC rssmf_GetOutputDevice (ViSession instrumentHandle,
                                         ViPInt32 outputDevice){return 0;}
ViStatus _VI_FUNC rssmf_SaveHardCopy (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetAutomaticNamingState (ViSession instrumentHandle,
                                                 ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticNamingState (ViSession instrumentHandle,
                                                 ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFilePath (ViSession instrumentHandle,
                                              ViInt32 arraySize,
                                              ViChar _VI_FAR filePath[]){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFileName (ViSession instrumentHandle,
                                              ViChar _VI_FAR fileName[]){return 0;}
ViStatus _VI_FUNC rssmf_SetAutomaticDirectory (ViSession instrumentHandle,
                                               ViString directoryName){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticDirectory (ViSession instrumentHandle,
                                               ViChar _VI_FAR directory[]){return 0;}
ViStatus _VI_FUNC rssmf_DeleteImageFiles (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetAutomaticFileDateState (ViSession instrumentHandle,
                                                   ViInt32 fileNamePart,
                                                   ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFileDateState (ViSession instrumentHandle,
                                                   ViInt32 fileNamePart,
                                                   ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFileDate (ViSession instrumentHandle,
                                              ViInt32 fileNamePart,
                                              ViChar _VI_FAR part[]){return 0;}
ViStatus _VI_FUNC rssmf_SetAutomaticFilePrefixState (ViSession instrumentHandle,
                                                     ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFilePrefixState (ViSession instrumentHandle,
                                                     ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetAutomaticFilePrefix (ViSession instrumentHandle,
                                                ViString prefix){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFilePrefix (ViSession instrumentHandle,
                                                ViChar _VI_FAR fileName[]){return 0;}
ViStatus _VI_FUNC rssmf_GetAutomaticFileNumber (ViSession instrumentHandle,
                                                ViPInt32 number){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticModules (ViSession instrumentHandle,
                                           ViString moduleName, ViInt32 arraySize,
                                           ViChar _VI_FAR moduleConfiguration[]){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticModuleNames (ViSession instrumentHandle,
                                               ViInt32 arraySize,
                                               ViChar _VI_FAR moduleNames[]){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticTestPoints (ViSession instrumentHandle,
                                              ViInt32 arraySize,
                                              ViChar _VI_FAR testPoints[]){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticTestPointVoltages (ViSession instrumentHandle,
                                                     ViString testPointName,
                                                     ViPReal64 measuredVoltage){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticOperationTime (ViSession instrumentHandle,
                                                 ViPUInt32 operationTime){return 0;}
ViStatus _VI_FUNC rssmf_DIAGnosticPowerOnEvents (ViSession instrumentHandle,
                                                 ViPUInt32 powerOnEvents){return 0;}
ViStatus _VI_FUNC rssmf_TESTDirect (ViSession instrumentHandle, ViInt32 command,
                                    ViChar _VI_FAR assemblyResponse[]){return 0;}
ViStatus _VI_FUNC rssmf_SetKeyboardLock (ViSession instrumentHandle,
                                         ViBoolean keyboardLock){return 0;}
ViStatus _VI_FUNC rssmf_GetKeyboardLock (ViSession instrumentHandle,
                                         ViPBoolean keyboardLock){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayPowerSaveState (ViSession instrumentHandle,
                                                  ViBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayPowerSaveState (ViSession instrumentHandle,
                                                  ViPBoolean state){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayPowerSaveHoldoff (ViSession instrumentHandle,
                                                    ViInt32 holdoff){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayPowerSaveHoldoff (ViSession instrumentHandle,
                                                    ViPInt32 holdoff){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayBlankMode (ViSession instrumentHandle,
                                             ViInt32 blankMode){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayBlankMode (ViSession instrumentHandle,
                                             ViPInt32 blankMode){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayProportionalScale (ViSession instrumentHandle,
                                                     ViInt32 proportionalScale){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayProportionalScale (ViSession instrumentHandle,
                                                     ViPInt32 proportionalScale){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayUpdate (ViSession instrumentHandle,
                                          ViBoolean displayUpdate){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayUpdate (ViSession instrumentHandle,
                                          ViPBoolean displayUpdate){return 0;}
ViStatus _VI_FUNC rssmf_SetDisplayLock (ViSession instrumentHandle,
                                        ViBoolean displayLock){return 0;}
ViStatus _VI_FUNC rssmf_GetDisplayLock (ViSession instrumentHandle,
                                        ViPBoolean displayLock){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryCatalog (ViSession instrumentHandle,
                                        ViString directoryPath, ViInt32 arraySize,
                                        ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryCatalogLength (ViSession instrumentHandle,
                                              ViString directoryPath,
                                              ViPInt32 catalogLength){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryChangeDirectory (ViSession instrumentHandle,
                                                ViString directoryName){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryCopy (ViSession instrumentHandle,
                                     ViString sourcePath, ViString destinationPath){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryBlockDataWrite (ViSession instrumentHandle,
                                               ViString fileName,
                                               ViInt32 binaryBlockLength,
                                               ViChar _VI_FAR binaryBlockData[]){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryBlockDataRead (ViSession instrumentHandle,
                                              ViString fileName,
                                              ViInt32 binaryBlockLength,
                                              ViChar _VI_FAR binaryBlockData[]){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryDirectoryCatalog (ViSession instrumentHandle,
                                                 ViString directoryPath,
                                                 ViInt32 arraySize,
                                                 ViChar _VI_FAR catalogList[]){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryDirectoryCatalogLength (ViSession instrumentHandle,
                                                       ViString directoryPath,
                                                       ViPInt32 catalogLength){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryDelete (ViSession instrumentHandle,
                                       ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryLoad (ViSession instrumentHandle,
                                     ViInt32 instrumentSetting, ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryMakeDirectory (ViSession instrumentHandle,
                                              ViString directoryName){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryMove (ViSession instrumentHandle,
                                     ViString fileSource, ViString fileDestination){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryRemoveDirectory (ViSession instrumentHandle,
                                                ViString directoryName){return 0;}
ViStatus _VI_FUNC rssmf_MMEMoryStore (ViSession instrumentHandle,
                                      ViInt32 instrumentSetting, ViString fileName){return 0;}
ViStatus _VI_FUNC rssmf_setRegister (ViSession instrumentHandle,
                                     ViInt32 registerGroup, ViInt32 registerType,
                                     ViInt32 value){return 0;}
ViStatus _VI_FUNC rssmf_getRegister (ViSession instrumentHandle,
                                     ViInt32 registerGroup, ViInt32 registerType,
                                     ViPInt32 value){return 0;}
ViStatus _VI_FUNC rssmf_STATusPRESet (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemBeeper (ViSession instrumentHandle,
                                         ViBoolean systemBeeper){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemBeeper (ViSession instrumentHandle,
                                         ViPBoolean systemBeeper){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemErrorQueueAll (ViSession instrumentHandle,
                                             ViInt32 arraySize,
                                             ViChar _VI_FAR errorQueue[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemErrorQueueCodeAll (ViSession instrumentHandle,
                                                 ViInt32 arraySize,
                                                 ViChar _VI_FAR errorQueueCode[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemErrorQueueCodeNext (ViSession instrumentHandle,
                                                  ViPInt32 errorQueueCodeNext){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemErrorQueueCount (ViSession instrumentHandle,
                                               ViPInt32 errorQueueCount){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemUSBResource (ViSession instrumentHandle,
                                           ViInt32 arraySize,
                                           ViChar _VI_FAR USBResource[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemGPIBResource (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViChar _VI_FAR GPIBResource[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemSerialResource (ViSession instrumentHandle,
                                              ViInt32 arraySize,
                                              ViChar _VI_FAR serialResource[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemOptionUserDefined (ViSession instrumentHandle,
                                                 ViString userDefinedIdentification){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemProtect (ViSession instrumentHandle,
                                          ViInt32 protectionLevel,
                                          ViBoolean protection, ViString password){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemVersion (ViSession instrumentHandle,
                                          ViChar _VI_FAR SCPIVersion[]){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemStartupStatus (ViSession instrumentHandle,
                                                ViPInt32 startupComplete){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemFactoryPreset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemErrorList (ViSession instrumentHandle,
                                            ViInt32 arraySize,
                                            ViChar _VI_FAR errorQueue[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkIPAddress (ViSession instrumentHandle,
                                                   ViString IPAddress){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkIPAddress (ViSession instrumentHandle,
                                                   ViInt32 arraySize,
                                                   ViChar _VI_FAR IPAddress[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkIPAddressSubnetMask
             (ViSession instrumentHandle, ViString IPAddressSubnetMask){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkIPAddressSubnetMask
             (ViSession instrumentHandle, ViInt32 arraySize,
              ViChar _VI_FAR IPAddressSubnetMask[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkIPAddressGateway (ViSession instrumentHandle,
                                                          ViString IPAddressGateway){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkIPAddressGateway (ViSession instrumentHandle,
                                                          ViInt32 arraySize,
                                                          ViChar _VI_FAR IPAddressGateway[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkIPAddressMode (ViSession instrumentHandle,
                                                       ViInt32 IPAddressMode){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkIPAddressMode (ViSession instrumentHandle,
                                                       ViPInt32 IPAddressMode){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemNetworkIPAddressExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkCommonWorkgroup (ViSession instrumentHandle,
                                                         ViString commonWorkgroup){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkCommonWorkgroup (ViSession instrumentHandle,
                                                         ViInt32 arraySize,
                                                         ViChar _VI_FAR commonWorkgroup[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkCommonDomain (ViSession instrumentHandle,
                                                      ViString commonDomain){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkCommonDomain (ViSession instrumentHandle,
                                                      ViInt32 arraySize,
                                                      ViChar _VI_FAR commonDomain[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkCommonHostname (ViSession instrumentHandle,
                                                        ViString commonHostname){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkCommonHostname (ViSession instrumentHandle,
                                                        ViInt32 arraySize,
                                                        ViChar _VI_FAR commonHostname[]){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemNetworkCommonExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkPreferedDNS (ViSession instrumentHandle,
                                                     ViString preferedDNS){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkPreferedDNS (ViSession instrumentHandle,
                                                     ViInt32 arraySize,
                                                     ViChar _VI_FAR preferedDNS[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkAlternateDNS (ViSession instrumentHandle,
                                                      ViString alternateDNS){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkAlternateDNS (ViSession instrumentHandle,
                                                      ViInt32 arraySize,
                                                      ViChar _VI_FAR alternateDNS[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkDNSMode (ViSession instrumentHandle,
                                                 ViInt32 DNSMode){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkDNSMode (ViSession instrumentHandle,
                                                 ViPInt32 DNSMode){return 0;}
ViStatus _VI_FUNC rssmf_SYSTemNetworkDNSExecute (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkDomain (ViSession instrumentHandle,
                                                ViString domain){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkDomain (ViSession instrumentHandle,
                                                ViInt32 arraySize,
                                                ViChar _VI_FAR domain[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkGateway (ViSession instrumentHandle,
                                                 ViString gateway){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkGateway (ViSession instrumentHandle,
                                                 ViInt32 arraySize,
                                                 ViChar _VI_FAR gateway[]){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkResource (ViSession instrumentHandle,
                                                  ViInt32 arraySize,
                                                  ViChar _VI_FAR resource[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkSubnetMask (ViSession instrumentHandle,
                                                    ViString subnetMask){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkSubnetMask (ViSession instrumentHandle,
                                                    ViInt32 arraySize,
                                                    ViChar _VI_FAR subnetMask[]){return 0;}
ViStatus _VI_FUNC rssmf_SetSYSTemNetworkWorkgroup (ViSession instrumentHandle,
                                                   ViString workgroup){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkWorkgroup (ViSession instrumentHandle,
                                                   ViInt32 arraySize,
                                                   ViChar _VI_FAR workgroup[]){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetworkMACAddress (ViSession instrumentHandle,
                                                    ViInt32 arraySize,
                                                    ViChar _VI_FAR MACAddress[]){return 0;}
ViStatus _VI_FUNC rssmf_GetSYSTemNetwork (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_TriggerSourceAllSweeps (ViSession instrumentHandle,
                                                ViInt32 triggerSourceAllSweeps){return 0;}
ViStatus _VI_FUNC rssmf_TriggerAllSweeps (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SweepReset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_TriggerRFFrequencySweep (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_TriggerRFLevelSweep (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_SetNoSignalMarkerPolarity (ViSession instrumentHandle,
                                                   ViInt32 markerPolarity){return 0;}
ViStatus _VI_FUNC rssmf_GetNoSignalMarkerPolarity (ViSession instrumentHandle,
                                                   ViPInt32 markerPolarity){return 0;}
ViStatus _VI_FUNC rssmf_SetBlankWidth (ViSession instrumentHandle,
                                       ViInt32 blankWidth){return 0;}
ViStatus _VI_FUNC rssmf_GetBlankWidth (ViSession instrumentHandle,
                                       ViPInt32 blankWidth){return 0;}
ViStatus _VI_FUNC rssmf_SetAngleUnits (ViSession instrumentHandle,
                                       ViInt32 angleUnits){return 0;}
ViStatus _VI_FUNC rssmf_GetAngleUnits (ViSession instrumentHandle,
                                       ViPInt32 angleUnits){return 0;}
ViStatus _VI_FUNC rssmf_SetPowerUnits (ViSession instrumentHandle,
                                       ViInt32 powerUnits){return 0;}
ViStatus _VI_FUNC rssmf_GetPowerUnits (ViSession instrumentHandle,
                                       ViPInt32 powerUnits){return 0;}
ViStatus _VI_FUNC rssmf_SetVelocityUnits (ViSession instrumentHandle,
                                          ViInt32 velocityUnits){return 0;}
ViStatus _VI_FUNC rssmf_GetVelocityUnits (ViSession instrumentHandle,
                                          ViPInt32 velocityUnits){return 0;}
ViStatus _VI_FUNC rssmf_readToFile (ViSession instrumentHandle, ViString source,
                                    ViString destination){return 0;}
ViStatus _VI_FUNC rssmf_writeFromFile (ViSession instrumentHandle, ViString source,
                                       ViString destination){return 0;}
ViStatus _VI_FUNC rssmf_errorCheckState (ViSession instrumentHandle,
                                         ViBoolean stateChecking){return 0;}
ViStatus _VI_FUNC rssmf_optionCheckState (ViSession instrumentHandle,
                                          ViBoolean stateChecking){return 0;}
ViStatus _VI_FUNC rssmf_rangeCheckState (ViSession instrumentHandle,
                                         ViBoolean rangeChecking){return 0;}
ViStatus _VI_FUNC rssmf_writeInstrData (ViSession instrumentHandle,
                                        ViString writeBuffer){return 0;}
ViStatus _VI_FUNC rssmf_readInstrData (ViSession instrumentHandle,
                                       ViInt32 numberBytesToRead,
                                       ViChar _VI_FAR readBuffer[],
                                       ViPInt32 numBytesRead){return 0;}
ViStatus _VI_FUNC rssmf_DevicePreset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_reset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rssmf_self_test (ViSession instrumentHandle,
                                   ViPInt16 selfTestResult,
                                   ViChar _VI_FAR selfTestMessage[]){return 0;}
ViStatus _VI_FUNC rssmf_error_query (ViSession instrumentHandle, ViPInt32 errorCode,
                                     ViChar _VI_FAR errorMessage[]){return 0;}
ViStatus _VI_FUNC rssmf_error_message (ViSession instrumentHandle,
                                       ViStatus statusCode,
                                       ViChar _VI_FAR message[]){return 0;}
ViStatus _VI_FUNC rssmf_revision_query (ViSession instrumentHandle,
                                        ViChar _VI_FAR instrumentDriverRevision[],
                                        ViChar _VI_FAR firmwareRevision[]){return 0;}
ViStatus _VI_FUNC rssmf_delay (ViSession instrumentHandle, ViReal64 seconds){return 0;}
ViStatus _VI_FUNC rssmf_close (ViSession instrumentHandle){return 0;}

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif


