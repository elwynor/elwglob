@echo off
rem Build 26.10.11.5 09:24PM
rem RUN_TESTS.CMD - build and run the Globals unit tests.
rem Needs Visual Studio 2022 (or Build Tools 2022) and Python 3.  Run from
rem anywhere.  Exit code 0 = all tests passed.
setlocal
set HERE=%~dp0
set ROOT=%HERE%..
set SRC=%ROOT%\SRC\ELWGLOB
set OUT=%ROOT%\BUILD\tests
set VCVARS=%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars32.bat
if not exist "%VCVARS%" set VCVARS=%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat

python "%HERE%TEST_SIZES.PY" || exit /b 1
python "%HERE%TEST_HELPMAP.PY" || exit /b 1

call "%VCVARS%" >nul || exit /b 2
if exist "%OUT%" rmdir /s /q "%OUT%"
mkdir "%OUT%\src" "%OUT%\run"
rem GLOSET.C goes next to the stand-in GLOBROUS.H so that one is found first.
copy /y "%SRC%\GLOSET.C" "%OUT%\src" >nul
copy /y "%SRC%\GLOFILES.C" "%OUT%\src" >nul
copy /y "%SRC%\GLOFILES.H" "%OUT%\src" >nul
copy /y "%SRC%\GLOCLOCK.C" "%OUT%\src" >nul
copy /y "%SRC%\GLOCLOCK.H" "%OUT%\src" >nul
copy /y "%SRC%\GLOSET.H" "%OUT%\src" >nul
copy /y "%SRC%\GLOSETS.H" "%OUT%\src" >nul
copy /y "%SRC%\TVARS.H" "%OUT%\src" >nul
copy /y "%SRC%\ELWGLOB.H" "%OUT%\src" >nul
copy /y "%HERE%STUB\GLOBROUS.H" "%OUT%\src" >nul
copy /y "%HERE%TEST_GLOSET.C" "%OUT%\src" >nul
rem /GF puts string constants in read-only memory, as the module's /O2 build
rem does; the stand-in Btrieve then refuses them as lookup keys like the engine.
cl /nologo /W3 /J /GF /I"%OUT%\src" /Fo"%OUT%\\" /Fe"%OUT%\TEST_GLOSET.EXE" ^
   "%OUT%\src\TEST_GLOSET.C" "%OUT%\src\GLOSET.C" "%OUT%\src\GLOFILES.C" "%OUT%\src\GLOCLOCK.C" >"%OUT%\build.txt" || (type "%OUT%\build.txt" & exit /b 2)
"%OUT%\TEST_GLOSET.EXE" "%HERE%DATA\ELWGLOB.343" "%OUT%\run"
exit /b %ERRORLEVEL%
