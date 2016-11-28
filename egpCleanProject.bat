@echo off
REM Clean Project Utility by D. Buckstein
REM Purges garbage files and folders from this project directory
cd "%~dp0\"
rd /s /q "bin"
rd /s /q "lib"
cd project/VS2015
cd egpDemo_freeglut
rd /s /q "build"
cd ../
cd egpLib
rd /s /q "build"
cd ../../../
del /s /q *.db
del /s /q *.sdf
del /s /q *.ncb
del /s /q *.suo
del /s /q *.open
del /s /q desktop.ini
