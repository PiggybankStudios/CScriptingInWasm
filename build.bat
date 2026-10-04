@echo off

setlocal enabledelayedexpansion

REM We allow pig_build being cloned into this repo's folder, but if we don't find that then we will look for and possibly download it into a folder in the directory above this one
set PIG_BUILD_RELATIVE_PATH=..\pig_build
if exist "pig_build" (
	set PIG_BUILD_RELATIVE_PATH=pig_build
)
for %%I in ("%PIG_BUILD_RELATIVE_PATH%") do set "PIG_BUILD_ABSOLUTE_PATH=%%~fI"

if not exist "%PIG_BUILD_RELATIVE_PATH%" (
	git --version > NUL 2> NUL
	if NOT "!ERRORLEVEL!"=="0" (
		echo "Git is not installed. Please manually download PigBuild into %PIG_BUILD_ABSOLUTE_PATH%"
		exit
	)
	git clone https://github.com/PiggybankStudios/PigBuild %PIG_BUILD_RELATIVE_PATH%
)

call %PIG_BUILD_RELATIVE_PATH%\shell\build.bat %*