#include <rsnrpz.h>

#if defined(__cplusplus) || defined(__cplusplus__)
extern "C" {
#endif


/*****************************************************************************/
/*= GLOBAL USER-CALLABLE FUNCTION DECLARATIONS (Exportable Functions) =======*/
/*****************************************************************************/
ViStatus _VI_FUNC rsnrpz_chan_getCacheRange( ViSession instrumentHandle,
                                             ViInt32 channel, 
                                             const char *pszCommand, 
                                             ViReal64 *pdCurrent, 
                                             ViReal64 *pdMin, 
                                             ViReal64 *pdMax ){return 0;}
ViStatus _VI_FUNC rsnrpz_setTimeout(     ViUInt32 ulNewTimo ){return 0;}
ViStatus _VI_FUNC rsnrpz_getTimeout(     ViUInt32 *pulNewTimo ){return 0;}
ViStatus _VI_FUNC rsnrpz_init(           ViRsrc resourceName,
                                         ViBoolean IDQuery,
                                         ViBoolean resetDevice,
                                         ViSession *instrumentHandle ){return 0;}
ViStatus _VI_FUNC rsnrpz_long_distance_setup( 
                                         ViBoolean IDQuery,
                                         ViBoolean resetDevice,
                                         ViString  resourceName,
                                         ViSession *instrumentHandle ){return 0;}
ViStatus _VI_FUNC rsnrpz_AddSensor(      ViSession instrumentHandle,
                                         ViInt32 channel,
                                         ViRsrc resourceName,
                                         ViBoolean IDQuery,
                                         ViBoolean resetDevice ){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_abort(    ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_getCount( ViSession instrumentHandle,
                                         ViInt32* count ){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_initiate( ViSession instrumentHandle ){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_zero(     ViSession instrumentHandle ){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_zeroAdvanced(ViSession instrumentHandle,
                                         ViInt32 channel,
                                         ViInt32 zeroing ){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_isZeroingComplete (ViSession instrumentHandle,
                                                  ViBoolean* zeroingCompleted){return 0;}
ViStatus _VI_FUNC rsnrpz_chans_isMeasurementComplete (ViSession instrumentHandle,
                                                      ViBoolean* measurementCompleted){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_mode (ViSession instrumentHandle, ViInt32 channel,
                                    ViInt32 measurementMode){return 0;}
ViStatus _VI_FUNC rsnrpz_timing_configureExclude (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViReal64 excludeStart,
                                                  ViReal64 excludeStop){return 0;}
ViStatus _VI_FUNC rsnrpz_timing_setTimingExcludeStart (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64 excludeStart){return 0;}
ViStatus _VI_FUNC rsnrpz_timing_getTimingExcludeStart (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64* excludeStart){return 0;}
ViStatus _VI_FUNC rsnrpz_timing_setTimingExcludeStop (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64 excludeStop){return 0;}
ViStatus _VI_FUNC rsnrpz_timing_getTimingExcludeStop (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64* excludeStop){return 0;}
ViStatus _VI_FUNC rsnrpz_bandwidth_setBw (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32 bandwidth){return 0;}
ViStatus _VI_FUNC rsnrpz_bandwidth_getBw (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32* bandwidth){return 0;}
ViStatus _VI_FUNC rsnrpz_bandwidth_getBwList (ViSession instrumentHandle,
                                              ViInt32 channel, ViInt32 bufferSize,
                                              ViChar _VI_FAR bandwidthList[]){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_configureAvgAuto (ViSession instrumentHandle,
                                               ViInt32 channel, ViInt32 resolution){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_configureAvgNSRatio (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViReal64 maximumNoiseRatio,
                                                  ViReal64 upperTimeLimit){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_configureAvgManual (ViSession instrumentHandle,
                                                 ViInt32 channel, ViInt32 count){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setAutoEnabled (ViSession instrumentHandle,
                                             ViInt32 channel,
                                             ViBoolean autoEnabled){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getAutoEnabled (ViSession instrumentHandle,
                                             ViInt32 channel,
                                             ViBoolean* autoEnabled){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setAutoMaxMeasuringTime (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64 upperTimeLimit){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getAutoMaxMeasuringTime (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64* upperTimeLimit){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setAutoNoiseSignalRatio (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64 maximumNoiseRatio){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getAutoNoiseSignalRatio (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64* maximumNoiseRatio){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setAutoResolution (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViInt32 resolution){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getAutoResolution (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViInt32* resolution){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setAutoType (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32 method){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getAutoType (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32* method){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setCount (ViSession instrumentHandle, ViInt32 channel,
                                       ViInt32 count){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getCount (ViSession instrumentHandle, ViInt32 channel,
                                       ViInt32* count){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setEnabled (ViSession instrumentHandle,
                                         ViInt32 channel, ViBoolean averaging){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getEnabled (ViSession instrumentHandle,
                                         ViInt32 channel, ViBoolean* averaging){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setSlot (ViSession instrumentHandle, ViInt32 channel,
                                      ViInt32 timeslot){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getSlot (ViSession instrumentHandle, ViInt32 channel,
                                      ViInt32* timeslot){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_setTerminalControl (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32 terminalControl){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_getTerminalControl (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32* terminalControl){return 0;}
ViStatus _VI_FUNC rsnrpz_avg_reset (ViSession instrumentHandle, ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_range_setAutoEnabled (ViSession instrumentHandle,
                                               ViInt32 channel,
                                               ViBoolean autoRange){return 0;}
ViStatus _VI_FUNC rsnrpz_range_getAutoEnabled (ViSession instrumentHandle,
                                               ViInt32 channel,
                                               ViBoolean* autoRange){return 0;}
ViStatus _VI_FUNC rsnrpz_range_setCrossoverLevel (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViReal64 crossoverLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_range_getCrossoverLevel (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViReal64* crossoverLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_range_setRange (ViSession instrumentHandle,
                                         ViInt32 channel, ViInt32 range){return 0;}
ViStatus _VI_FUNC rsnrpz_range_getRange (ViSession instrumentHandle,
                                         ViInt32 channel, ViInt32* range){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_configureCorrections (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViBoolean offsetState,
                                                    ViReal64 offset,
                                                    ViBoolean reserved1,
                                                    ViString reserved2,
                                                    ViBoolean sParameterEnable){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setCorrectionFrequency (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getCorrectionFrequency (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64* frequency){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setCorrectionFrequencyStep (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViReal64 frequencyStep){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getCorrectionFrequencyStep (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViReal64 *frequencyStep){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setCorrectionFrequencySpacing (ViSession instrumentHandle,
                                                             ViInt32 channel,
                                                             ViInt32 frequencySpacing){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getCorrectionFrequencySpacing (ViSession instrumentHandle,
                                                             ViInt32 channel,
                                                             ViInt32 *frequencySpacing){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setOffset (ViSession instrumentHandle,
                                         ViInt32 channel, ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getOffset (ViSession instrumentHandle,
                                         ViInt32 channel, ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setOffsetEnabled (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViBoolean offsetState){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getOffsetEnabled (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViBoolean* offsetState){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setSParamDeviceEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean sParameterEnable){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getSParamDeviceEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean* sParameterCorrection){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setSParamDevice (ViSession instrumentHandle,
                                      ViInt32 channel, ViInt32 sParameter){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getSParamDevice (ViSession instrumentHandle,
                                      ViInt32 channel, ViInt32 *sParameter){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getSParamDevList( ViSession vi,
                                                ViInt32   channel,
                                                ViInt32   iSpdListSize,
                                                ViChar    spdList[] ){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_configureSourceGammaCorr (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean sourceGammaCorrection,
                                                        ViReal64 magnitude,
                                                        ViReal64 phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setSourceGammaMagnitude (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64 magnitude){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getSourceGammaMagnitude (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64* magnitude){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setSourceGammaPhase (ViSession instrumentHandle,
                                                   ViInt32 channel, ViReal64 phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getSourceGammaPhase (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViReal64* phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setSourceGammaCorrEnabled (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViBoolean sourceGammaCorrection){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getSourceGammaCorrEnabled (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViBoolean* sourceGammaCorrection){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_configureReflectGammaCorr (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViReal64 magnitude,
                                                         ViReal64 phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setReflectionGammaMagn (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64 magnitude){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getReflectionGammaMagn (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViReal64* magnitude){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setReflectionGammaPhase (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64 phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getReflectionGammaPhase (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViReal64* phase){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setReflectionGammaUncertainty (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViReal64 uncertainty){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getReflectionGammaUncertainty (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViReal64 *uncertainty){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_configureDutyCycle (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViBoolean dutyCycleState,
                                                  ViReal64 dutyCycle){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setDutyCycle (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64 dutyCycle){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getDutyCycle (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64* dutyCycle){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_setDutyCycleEnabled (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViBoolean dutyCycleState){return 0;}
ViStatus _VI_FUNC rsnrpz_corr_getDutyCycleEnabled (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViBoolean* dutyCycleState){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setContAvAperture (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64 contAvAperture){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvAperture (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64* contAvAperture){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setContAvSmoothingEnabled (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViBoolean contAvSmoothing){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvSmoothingEnabled (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViBoolean* contAvSmoothing){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setContAvBufferedEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean contAvBufferedMode){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvBufferedEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean* contAvBufferedMode){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setContAvBufferSize (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViInt32 bufferSize){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvBufferSize (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViInt32* bufferSize){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvBufferCount (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViInt32 *bufferCount){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getContAvBufferInfo (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViString infoType,
                                                   ViInt32 arraySize,
                                                   ViChar info[]){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setBurstDropoutTolerance (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64 dropoutTolerance){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getBurstDropoutTolerance (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64* dropoutTolerance){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setBurstChopperEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean burstAvChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getBurstChopperEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean* burstAvChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_configureTimeGate (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViInt32 selectGate,
                                                     ViReal64 offset, ViReal64 time,
                                                     ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setOffsetTime (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32 selectGate,
                                                 ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getOffsetTime (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32 selectGate,
                                                 ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setTime (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 selectGate,
                                           ViReal64 time){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getTime (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 selectGate,
                                           ViReal64* time){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setFrequency (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64 frequency){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getFrequency (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64* frequency){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setMidOffset (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getMidOffset (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setMidLength (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64 length){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getMidLength (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 selectGate,
                                                ViReal64* length){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_setChopperEnabled (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViBoolean timegateChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_timegate_getChopperEnabled (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViBoolean* timegateChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_confTimegate (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64 offset,
                                            ViReal64 time, ViReal64 midambleOffset,
                                            ViReal64 midambleLength){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_confScale (ViSession instrumentHandle,
                                         ViInt32 channel, ViReal64 referenceLevel,
                                         ViReal64 range, ViInt32 points){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setOffsetTime (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getOffsetTime (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setTime (ViSession instrumentHandle, ViInt32 channel,
                                       ViReal64 time){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getTime (ViSession instrumentHandle, ViInt32 channel,
                                       ViReal64* time){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setMidOffset (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getMidOffset (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setMidLength (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64 length){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getMidLength (ViSession instrumentHandle,
                                            ViInt32 channel, ViReal64* length){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setScaleRefLevel (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViReal64 referenceLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getScaleRefLevel (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViReal64* referenceLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setScaleRange (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64 range){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getScaleRange (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64* range){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_setScalePoints (ViSession instrumentHandle,
                                              ViInt32 channel, ViInt32 points){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getScalePoints (ViSession instrumentHandle,
                                              ViInt32 channel, ViInt32* points){return 0;}
ViStatus _VI_FUNC rsnrpz_stat_getScaleWidth (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64* width){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_configureTimeSlot (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViInt32 timeSlotCount,
                                                  ViReal64 width){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_setTimeSlotCount (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32 timeSlotCount){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_getTimeSlotCount (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViInt32* timeSlotCount){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_setTimeSlotWidth (ViSession instrumentHandle,
                                                 ViInt32 channel, ViReal64 width){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_getTimeSlotWidth (ViSession instrumentHandle,
                                                 ViInt32 channel, ViReal64* width){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_setTimeSlotMidOffset (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViReal64 offset){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_getTimeSlotMidOffset (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViReal64* offset){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_setTimeSlotMidLength (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViReal64 length){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_getTimeSlotMidLength (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViReal64* length){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_setTimeSlotChopperEnabled (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViBoolean timeSlotChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_tslot_getTimeSlotChopperEnabled (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViBoolean* timeSlotChopper){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_configureScope (ViSession instrumentHandle,
                                               ViInt32 channel, ViInt32 scopePoints,
                                               ViReal64 scopeTime,
                                               ViReal64 offsetTime,
                                               ViBoolean realtime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_fastZero (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAverageEnabled (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViBoolean scopeAveraging){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAverageEnabled (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViBoolean* scopeAveraging){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAverageCount (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 count){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAverageCount (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32* count){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAverageTerminalControl (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViInt32 terminalControl){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAverageTerminalControl (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViInt32* terminalControl){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setOffsetTime (ViSession instrumentHandle,
                                              ViInt32 channel, ViReal64 offsetTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getOffsetTime (ViSession instrumentHandle,
                                              ViInt32 channel,
                                              ViReal64* offsetTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setPoints (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32 scopePoints){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getPoints (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32* scopePoints){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setRealtimeEnabled (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViBoolean realtime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getRealtimeEnabled (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViBoolean* realtime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setTime (ViSession instrumentHandle, ViInt32 channel,
                                        ViReal64 scopeTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getTime (ViSession instrumentHandle, ViInt32 channel,
                                        ViReal64* scopeTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAutoEnabled (ViSession instrumentHandle,
                                               ViInt32 channel,
                                               ViBoolean autoEnabled){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAutoEnabled (ViSession instrumentHandle,
                                               ViInt32 channel,
                                               ViBoolean* autoEnabled){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAutoMaxMeasuringTime (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64 upperTimeLimit){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAutoMaxMeasuringTime (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64* upperTimeLimit){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAutoNoiseSignalRatio (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64 maximumNoiseRatio){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAutoNoiseSignalRatio (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64* maximumNoiseRatio){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAutoResolution (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViInt32 resolution){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAutoResolution (ViSession instrumentHandle,
                                                  ViInt32 channel,
                                                  ViInt32* resolution){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setAutoType (ViSession instrumentHandle,
                                            ViInt32 channel, ViInt32 method){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getAutoType (ViSession instrumentHandle,
                                            ViInt32 channel, ViInt32* method){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_setEquivalentSampling (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean scopeEquivalentSampling){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_getEquivalentSampling (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean *scopeEquivalentSampling){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setMeasEnabled (ViSession instrumentHandle,
                                           ViInt32 channel,
                                           ViBoolean traceMeasurements){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getMeasEnabled (ViSession instrumentHandle,
                                           ViInt32 channel,
                                           ViBoolean *traceMeasurements){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setMeasAlgorithm (ViSession instrumentHandle,
                                             ViInt32 channel, ViInt32 algorithm){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getMeasAlgorithm (ViSession instrumentHandle,
                                             ViInt32 channel, ViInt32 *algorithm){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setLevelThresholds (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViReal64 durationRef,
                                                   ViReal64 transitionLowRef,
                                                   ViReal64 transitionHighRef){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getLevelThresholds (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViReal64 *durationRef,
                                                   ViReal64 *transitionLowRef,
                                                   ViReal64 *transitionHighRef){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setTime (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64 measTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getTime (ViSession instrumentHandle,
                                             ViInt32 channel, ViReal64 *measTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setOffsetTime (ViSession instrumentHandle,
                                              ViInt32 channel,
                                              ViReal64 offsetTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getOffsetTime (ViSession instrumentHandle,
                                              ViInt32 channel,
                                              ViReal64 *offsetTime){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getPulseTimes (ViSession instrumentHandle,
                                              ViInt32 channel,
                                              ViReal64 *dutyCycle,
                                              ViReal64 *pulseDuration,
                                              ViReal64 *pulsePeriod){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getPulseTransition (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViInt32 slope,
                                                   ViReal64 *duration,
                                                   ViReal64 *occurence,
                                                   ViReal64 *overshoot){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getPulsePower (ViSession instrumentHandle,
                                              ViInt32 channel, ViReal64 *average,
                                              ViReal64 *minPeak,
                                              ViReal64 *maxPeak){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getPulseLevels (ViSession instrumentHandle,
                                               ViInt32 channel,
                                               ViReal64 *topLevel,
                                               ViReal64 *baseLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getPulseReferenceLevels (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViReal64 *lowRefLevel,
                                                        ViReal64 *highRefLevel,
                                                        ViReal64 *durationRefLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_setEquivalentSampling (ViSession instrumentHandle,
                                                           ViInt32 channel,
                                                           ViBoolean scopeMeasEquivSampling){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getEquivalentSampling (ViSession instrumentHandle,
                                                           ViInt32 channel,
                                                           ViBoolean *scopeMeasEquivSampling){return 0;}
ViStatus _VI_FUNC rsnrpz_scope_meas_getSamplePeriod (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViReal64 *samplePeriod){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_configureInternal (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViReal64 triggerLevel,
                                                    ViInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_configureExternal (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViReal64 triggerDelay){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_immediate (ViSession instrumentHandle,
                                            ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setAutoDelayEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean autoDelay){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getAutoDelayEnabled (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViBoolean* autoDelay){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setAutoTriggerEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean autoTrigger){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getAutoTriggerEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean* autoTrigger){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setCount (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 triggerCount){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getCount (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32* triggerCount){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setDelay (ViSession instrumentHandle,
                                           ViInt32 channel, ViReal64 triggerDelay){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getDelay (ViSession instrumentHandle,
                                           ViInt32 channel, ViReal64* triggerDelay){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setHoldoff (ViSession instrumentHandle,
                                             ViInt32 channel,
                                             ViReal64 triggerHoldoff){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getHoldoff (ViSession instrumentHandle,
                                             ViInt32 channel,
                                             ViReal64* triggerHoldoff){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setHysteresis (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViReal64 triggerHysteresis){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getHysteresis (ViSession instrumentHandle,
                                                ViInt32 channel,
                                                ViReal64* triggerHysteresis){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setLevel (ViSession instrumentHandle,
                                           ViInt32 channel, ViReal64 triggerLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getLevel (ViSession instrumentHandle,
                                           ViInt32 channel, ViReal64* triggerLevel){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setSlope (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32 triggerSlope){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getSlope (ViSession instrumentHandle,
                                           ViInt32 channel, ViInt32* triggerSlope){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setSource (ViSession instrumentHandle,
                                            ViInt32 channel, ViInt32 triggerSource){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getSource (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViInt32* triggerSource){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setDropoutTime (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64 dropoutTime){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getDropoutTime (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64* dropoutTime){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setMasterState (ViSession instrumentHandle,
                                        ViInt32 channel, ViBoolean state){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getMasterState (ViSession instrumentHandle,
                                        ViInt32 channel, ViBoolean *state){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_setSyncState (ViSession instrumentHandle,
                                      ViInt32 channel, ViBoolean state){return 0;}
ViStatus _VI_FUNC rsnrpz_trigger_getSyncState (ViSession instrumentHandle,
                                      ViInt32 channel, ViBoolean *state){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_info (ViSession instrumentHandle, ViInt32 channel,
                                    ViString infoType, ViInt32 arraySize,
                                    ViChar _VI_FAR info[]){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_infoHeader (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32 parameterNumber,
                                          ViInt32 arraySize,
                                          ViChar _VI_FAR header[]){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_infosCount (ViSession instrumentHandle,
                                          ViInt32 channel, ViInt32* count){return 0;}
ViStatus _VI_FUNC rsnrpz_system_setStatusUpdateTime (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViReal64 statusUpdateTime){return 0;}
ViStatus _VI_FUNC rsnrpz_system_getStatusUpdateTime (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViReal64 *statusUpdateTime){return 0;}
ViStatus _VI_FUNC rsnrpz_system_setResultUpdateTime (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViReal64 resultUpdateTime){return 0;}
ViStatus _VI_FUNC rsnrpz_system_getResultUpdateTime (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViReal64 *resultUpdateTime){return 0;}
ViStatus _VI_FUNC rsnrpz_calib_test (ViSession instrumentHandle, ViInt32 channel,
                                     ViReal64 *calibTest){return 0;}
ViStatus _VI_FUNC rsnrpz_calib_getTestDeviation (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64 *testDeviation){return 0;}
ViStatus _VI_FUNC rsnrpz_calib_getTestReference (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64 *testReference){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_abort (ViSession instrumentHandle, ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_initiate (ViSession instrumentHandle,
                                        ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setInitContinuousEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean continuousInitiate){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getInitContinuousEnabled (ViSession instrumentHandle,
                                                        ViInt32 channel,
                                                        ViBoolean* continuousInitiate){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_reset (ViSession instrumentHandle, ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setSamplingFrequency (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViInt32 samplingFrequency){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getSamplingFrequency (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViInt32* samplingFrequency){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_zero (ViSession instrumentHandle, ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_isZeroComplete (ViSession instrumentHandle,
                                              ViInt32 channel,
                                              ViBoolean* zeroingComplete){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_isMeasurementComplete (ViSession instrumentHandle,
                                                     ViInt32 channel,
                                                     ViBoolean* measurementComplete){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_selfTest (ViSession instrumentHandle, ViInt32 channel,
                                        ViChar _VI_FAR result[]){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_setAuxiliary (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViInt32 auxiliaryValue){return 0;}
ViStatus _VI_FUNC rsnrpz_chan_getAuxiliary (ViSession instrumentHandle,
                                            ViInt32 channel,
                                            ViInt32* auxiliaryValue){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_readMeasurement (ViSession instrumentHandle,
                                                ViInt32 channel, ViInt32 timeout_ms,
                                                ViReal64* measurement){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_fetchMeasurement (ViSession instrumentHandle,
                                                 ViInt32 channel,
                                                 ViReal64* measurement){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_readBufferMeasurement (ViSession instrumentHandle,
                                                      ViInt32 channel,
                                                      ViInt32 maximumTime_ms,
                                                      ViInt32 bufferSize,
                                                      ViReal64 _VI_FAR measurementArray[],
                                                      ViInt32* readCount){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_fetchBufferMeasurement (ViSession instrumentHandle,
                                                       ViInt32 channel,
                                                       ViInt32 arraySize,
                                                       ViReal64 _VI_FAR measurementArray[],
                                                       ViInt32* readCount){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_sendSoftwareTrigger (ViSession instrumentHandle,
                                                    ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_readMeasurementAux (ViSession instrumentHandle,
                                                   ViInt32 channel,
                                                   ViInt32 timeout_ms,
                                                   ViReal64* measurement,
                                                   ViReal64* aux1, ViReal64* aux2){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_fetchMeasurementAux (ViSession instrumentHandle,
                                                    ViInt32 channel,
                                                    ViInt32 timeout_ms,
                                                    ViReal64* measurement,
                                                    ViReal64* aux1, ViReal64* aux2){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_readBufferMeasurementAux (ViSession instrumentHandle,
                                                         ViInt32 channel,
                                                         ViInt32 maximumTime_ms,
                                                         ViInt32 bufferSize,
                                                         ViReal64 _VI_FAR measurementArray[],
                                                         ViReal64 _VI_FAR aux1Array[],
                                                         ViReal64 _VI_FAR aux2Array[],
                                                         ViInt32* readCount){return 0;}
ViStatus _VI_FUNC rsnrpz_meass_fetchBufferMeasurementAux (ViSession instrumentHandle,
                                                          ViInt32 channel,
                                                          ViInt32 maximumTime_ms,
                                                          ViInt32 bufferSize,
                                                          ViReal64 _VI_FAR measurementArray[],
                                                          ViReal64 _VI_FAR aux1Array[],
                                                          ViReal64 _VI_FAR aux2Array[],
                                                          ViInt32* readCount){return 0;}
ViStatus _VI_FUNC rsnrpz_status_preset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rsnrpz_status_checkCondition (ViSession instrumentHandle,
                                                ViInt32 statusClass, ViUInt32 mask,
                                                ViBoolean* state){return 0;}
ViStatus _VI_FUNC rsnrpz_status_catchEvent (ViSession instrumentHandle,
                                            ViInt32 statusClass, ViUInt32 mask,
                                            ViInt32 direction){return 0;}
ViStatus _VI_FUNC rsnrpz_status_checkEvent (ViSession instrumentHandle,
                                            ViInt32 statusClass, ViUInt32 mask,
                                            ViUInt32 resetMask, ViBoolean* events){return 0;}
ViStatus _VI_FUNC rsnrpz_status_enableEventNotification (ViSession instrumentHandle,
                                                         ViInt32 statusClass,
                                                         ViUInt32 mask){return 0;}
ViStatus _VI_FUNC rsnrpz_status_disableEventNotification (ViSession instrumentHandle,
                                                          ViInt32 statusClass,
                                                          ViUInt32 mask){return 0;}
ViStatus _VI_FUNC rsnrpz_status_registerWindowMessage (ViSession instrumentHandle,
                                                       ViUInt32 *windowHandle,
                                                       ViUInt32 messageID){return 0;}
ViStatus _VI_FUNC rsnrpz_errorCheckState (ViSession instrumentHandle,
                                          ViBoolean stateChecking){return 0;}
ViStatus _VI_FUNC rsnrpz_reset (ViSession instrumentHandle){return 0;}
ViStatus _VI_FUNC rsnrpz_self_test (ViSession instrumentHandle,
                                    ViInt16* selfTestResult,
                                    ViChar _VI_FAR selfTestMessage[]){return 0;}
ViStatus _VI_FUNC rsnrpz_error_query (ViSession instrumentHandle,
                                      ViInt32* errorCode,
                                      ViChar _VI_FAR errorMessage[]){return 0;}
ViStatus _VI_FUNC rsnrpz_error_message (ViSession instrumentHandle,
                                        ViStatus statusCode,
                                        ViChar _VI_FAR message[]){return 0;}
ViStatus _VI_FUNC rsnrpz_revision_query (ViSession instrumentHandle,
                                         ViChar _VI_FAR instrumentDriverRevision[],
                                         ViChar _VI_FAR firmwareRevision[]){return 0;}
ViStatus _VI_FUNC rsnrpz_CloseSensor (ViSession instrumentHandle, ViInt32 channel){return 0;}
ViStatus _VI_FUNC rsnrpz_close (ViSession instrumentHandle){return 0;}

ViStatus _VI_FUNC rsnrpz_GetSensorCount( ViSession iDummyHandle, ViInt32 *piCount ){return 0;}
ViStatus _VI_FUNC rsnrpz_GetSensorInfo( 
                      ViSession      iDummyHandle,
                      ViInt32        iChannel,
                      ViChar _VI_FAR *pszSensorName,
                      ViChar _VI_FAR *pszSensorType,
                      ViChar _VI_FAR *pszSensorSerial ){return 0;}

ViStatus _VI_FUNC rsnrpz_status_driverOpenState( ViBoolean* driverState ){return 0;}

ViStatus _VI_FUNC rsnrpz_service_getDetectorTemperature( ViSession vi, ViInt32 channel, ViReal64* pTemperature ){return 0;}

ViStatus _VI_FUNC rsnrpz_fw_version_check( ViSession      vi,
                                           ViInt32        iBufSize,
                                           ViChar _VI_FAR firmwareCurrent[],
                                           ViChar _VI_FAR firmwareRequiredMinimum[],
                                           ViBoolean     *pbFirmwareOkay ){return 0;}

#if defined(_OUTSIDEVEE_)
ViStatus _VI_FUNC rsnrpz_GetSessionControlHandle(       // internal use only
                       ViSession      iHandle,
                       ViInt32        iChannel,
                       ViSession    **pCtrlSession ){return 0;}

ViStatus _VI_FUNC rsnrpz_status_setDeviceChangedCallback( void (*pFn)(NRP_SESSION), NRP_USERARG lUserArgument ){return 0;}
#endif

#if defined(__cplusplus) || defined(__cplusplus__)
}
#endif


