:: make sure that C:\Qt\5.13.0\mingw73_32\bin is on path

windeployqt --dir .\deploy ..\build-laigter-Desktop_Qt_5_13_0_MinGW_32_bit-Release\release\laigter.exe

copy /Y ..\build-laigter-Desktop_Qt_5_13_0_MinGW_32_bit-Release\release\laigter.exe .\deploy\laigter.exe

pause