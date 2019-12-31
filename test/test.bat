@echo off
"../bin/mcc" -V
echo. 
echo. 
echo. 
echo TEST RESULTS:
if %errorlevel%==0 (echo OK) else (echo FAIL)
echo %errorlevel%
pause
exit
