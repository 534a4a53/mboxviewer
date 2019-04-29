@echo off

REM Few hints first :)
REM %~1         - expands %1 removing any surrounding quotes (")
REM %~f1        - expands %1 to a fully qualified path name
REM %~d1        - expands %1 to a drive letter only
REM %~p1        - expands %1 to a path only
REM %~n1        - expands %1 to a file name only
REM %~x1        - expands %1 to a file extension only
REM %~s1        - expanded path contains short names only
REM %~a1        - expands %1 to file attributes
REM %~t1        - expands %1 to date/time of file
REM %~z1        - expands %1 to size of file
REM The modifiers can be combined to get compound results:
REM %~dp1       - expands %1 to a drive letter and path only
REM %~nx1       - expands %1 to a file name and extension only

REM This script is working example script to render subset of HTML files listed in the text file to PDF using wkhtmltopdf tool.


setlocal enabledelayedexpansion

REM Update path if needed
REM Download wkhtmltopdf from https://wkhtmltopdf.org/downloads.html
REM Usage link on how to control header and footer https://wkhtmltopdf.org/usage/wkhtmltopdf.txt
set ProgName=wkhtmltopdf.exe
set ProgDirectoryPath=C:\Program Files\wkhtmltopdf\bin
set CmdPath=%ProgDirectoryPath%\%ProgName%

if NOT exist "%CmdPath%" (

echo.
echo Invalid path to %CmdPath% executable file.
echo Please install and/or update the wkhtmltopdf path in the batch file and re-run the script again.
echo.

pause
exit
)

set HTMLFileName=
set HTMLFileNameBase=
set PDF_GROUP_DIR=

set CURRENT_DIR=%cd%
set PDF_GROUP_DIR=%CURRENT_DIR%
if NOT exist !PDF_GROUP_DIR! mkdir !PDF_GROUP_DIR!

REM
set HTMLListFile=%CURRENT_DIR%\GroupHTMLFileList.txt

for /F "usebackq tokens=*" %%A in ("%HTMLListFile%") do (

Set HTMLFileName=%%A
Set HTMLFileNameBase=%%~nA

REM Create PDF_GROUP_DIR based on file name path
REM set PDF_GROUP_DIR=%%~pA\PDF_GROUP
REM if NOT exist !PDF_GROUP_DIR! mkdir !PDF_GROUP_DIR!

REM echo HTMLFileName=!HTMLFileName!
REM echo HTMLFileNameBase=!HTMLFileNameBase!
REM echo PDF_GROUP_DIR=!PDF_GROUP_DIR!

echo "%CmdPath%" --log-level none --no-background --footer-right "Page [page] of [toPage]" "!HTMLFileName!" "!PDF_GROUP_DIR!\!HTMLFileNameBase!.pdf" 
call "%CmdPath%" --log-level none --no-background --footer-right "Page [page] of [toPage]" "!HTMLFileName!" "!PDF_GROUP_DIR!\!HTMLFileNameBase!.pdf" 

echo.

)

pause