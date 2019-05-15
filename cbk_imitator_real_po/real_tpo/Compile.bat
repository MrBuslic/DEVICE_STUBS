@echo on
set PATH=%SubMicPath%\TSRP\CDS\GnuMIPS;%path%
set GCC_EXEC_PREFIX=%SubMicPath:\=/%/TSRP/CDS/GNUMIPS/gcc-lib/

make -C D:/Roman/TPO/Source/ clean PRJ=VersionProject CMD=clean
make -C D:/Roman/TPO/Source/ clean PRJ=MainProject CMD=clean
make -C D:/Roman/TPO/Source/ clean PRJ=VecProject CMD=clean
make -C D:/Roman/TPO/Source/ clean PRJ=BcuProject CMD=clean

make PRJ=VersionProject -C D:/Roman/TPO/Source/ VersionProject.e
make PRJ=MainProject -C D:/Roman/TPO/Source/ MainProject.e
make PRJ=VecProject -C D:/Roman/TPO/Source/ VecProject.e
make PRJ=BcuProject -C D:/Roman/TPO/Source/ BcuProject.e
pause

