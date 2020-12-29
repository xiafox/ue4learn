@echo off

cd %curpath%
del /F /S /Q	logs\\*.log
rd /S /Q	logs\tftp

echo.
pause