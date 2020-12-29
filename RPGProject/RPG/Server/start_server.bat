@echo off

for /f "tokens=4" %%a in ('route print^|findstr 0.0.0.0.*0.0.0.0') do (
 set ip=%%a
	goto out
)
:out

echo ip = %ip%

for /f "tokens=1-4 delims=." %%a in ("%ip%") do set strip=%%~a%%~b%%~c%%~d

echo strip = %strip%
set curpath=%~dp0

set KBE_ROOT=%curpath%
set KBE_RES_PATH=%KBE_ROOT%kbe/res/;%KBE_ROOT%/res/
set KBE_BIN_PATH=%KBE_ROOT%kbe/bin/Hybrid/

set uid=%strip%

cd %curpath%
tasklist|findstr "machine.exe"||goto next2
call "kill_server.bat"
sleep 3
call "kill_log.bat"
:next2

echo KBE_ROOT = %KBE_ROOT%
echo KBE_RES_PATH = %KBE_RES_PATH%
echo KBE_BIN_PATH = %KBE_BIN_PATH%

start /B %KBE_BIN_PATH%/machine.exe    --cid=%strip%1000 --gus=1
start /B  %KBE_BIN_PATH%/logger.exe     --cid=%strip%2000 --gus=2
start /B  %KBE_BIN_PATH%/interfaces.exe --cid=%strip%3000 --gus=3
start /B  %KBE_BIN_PATH%/dbmgr.exe      --cid=%strip%4000 --gus=4
start /B  %KBE_BIN_PATH%/baseappmgr.exe --cid=%strip%5000 --gus=5
start /B  %KBE_BIN_PATH%/cellappmgr.exe --cid=%strip%6000 --gus=6
start /B  %KBE_BIN_PATH%/baseapp.exe    --cid=%strip%7001 --gus=7
start /B  %KBE_BIN_PATH%/cellapp.exe    --cid=%strip%8002 --gus=10
start /B  %KBE_BIN_PATH%/loginapp.exe   --cid=%strip%9000 --gus=11
