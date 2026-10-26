@echo off
setlocal
if "%~2"=="" (
  echo Usage: run-gather-tests.cmd ^<java-21-home^> ^<build-directory^>
  exit /b 2
)
set "JAVA=%~1\bin\java.exe"
set "BUILD=%~f2"
set "ROOT=%~dp0..\..\.."
set "OUTPUT=%BUILD%\gather-tests"
if not exist "%OUTPUT%" mkdir "%OUTPUT%"
set "CP=%ROOT%\.build\java-dependencies\*;%BUILD%\bin\PurpleWave.jar;%BUILD%\bin\lib\*"
"%JAVA%" -cp "%CP%" scala.tools.nsc.Main -usejavacp -d "%OUTPUT%" "%~dp0GatherEmptyThreatsTest.scala"
if errorlevel 1 exit /b %ERRORLEVEL%
"%JAVA%" -cp "%OUTPUT%;%CP%" Micro.GatherEmptyThreatsTest
exit /b %ERRORLEVEL%
