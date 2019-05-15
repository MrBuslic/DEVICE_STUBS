@echo off

set __MACHINE__=CBK
set vmList=1
set vmMap=0x1

rem chdir %SubMicPath%\Target\OPO\KDZ\%__MACHINE__%\Debug\NOK_2\
start %SubMicPath%\Host\TSRP\Dbg\%__MACHINE__%\modcbkm.exe
rem start %SubMicPath%\NOK_2\Dbg\CBK\nok_cbk.exe
