@echo off
"../bin/mcc" -V
echo. 
echo. 
echo. 
echo %errorlevel%
if %errorlevel%==0 (echo TEST OK) else (echo TEST FAIL)
pause
exit
