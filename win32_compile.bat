
:: Enter the full path to the Qt libraries you wish to compile against. 
:: You must specifically point to the "bin" folder in which qmake.exe resides, e.g. C:\qt-4.7.4\bin
:: The Qt directory by itself (C:\qt-4.7.4\) will not work.
@set qt=C:\qt-4.7.4\bin

:: Enter the full path to MinGW toolkit. It usually installs to C:\MinGW\bin
@set mingw=C:\MinGW\bin


:: DO *NOT* EDIT BELOW THIS LINE!!!!!!!
:: ###################################################################################

@TITLE ROBOJOURNAL BUILD HELPER SCRIPT FOR WINDOWS 2000/XP/VISTA/7
@ECHO.
@ECHO Version 1.1 -- 8/16/2012
@ECHO.
@ECHO #############################################################
@ECHO.
@ECHO This script assumes the following values are true:
@ECHO.
@ECHO.
@ECHO ^* QT ^>^= 4.7.4 is installed at:
@ECHO   [%qt%]
@ECHO.
@ECHO ^* 32-bit MinGW (compiler) toolkit is installed at:
@ECHO   [%mingw%]
@ECHO.
@ECHO These values MUST correspond to the install locations of Qt and MinGW
@ECHO on your system. Change them if necessary BEFORE running this script!
@echo Open win32_compile.bat in a text editor if you need to make changes. 
@ECHO.
@echo BUILD PATH: %cd%\release
@echo.
@ECHO #############################################################
@ECHO.

@pause

@path %qt%
@qmake robojournal.pro
@path %mingw%
mingw32-make
mingw32-make clean
@ECHO.
@ECHO #############################################################
@ECHO Build process complete!
@ECHO.
@ECHO robojournal.exe will work by itself if the Qt you compiled against 
@ECHO was static-built. 
@ECHO.
@ECHO If your Qt is ^*NOT^* static, locate the following files on your hard drive (they
@ECHO MUST be from the same Qt and MinGW libraries you compiled against) and copy 
@ECHO them to  %cd%\release.  
@ECHO.
@ECHO ^* libgcc_s_dw2-1.dll
@ECHO ^* libmysql.dll
@ECHO ^* libstdc^+^+-6.dll
@ECHO ^* QtCore4.dll
@ECHO ^* QtGui4.dll
@ECHO ^* QtSql4.dll
@ECHO.
@ECHO Thank you for choosing RoboJournal. 
@ECHO.
@pause