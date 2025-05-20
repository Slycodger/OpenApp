@echo off

if not exist linecountLogs/ (
  mkdir lineCountLogs
)

set MMDDYY=%date:~4,10%
set MMDDYY=%MMDDYY:/=-%
set HHMMSSMM=%time: =0%
set HHMMSSMM=%HHMMSSMM::=-%
set HHMMSSMM=%HHMMSSMM:~0,8%
set file=lineCountLogs/%MMDDYY%_%HHMMSSMM%.txt

for /F "tokens=*" %%a in ('powershell.exe -command "(gci -path "./src/" | select-string .).count"') do set srcLines=%%a
for /F "tokens=*" %%a in ('powershell.exe -command "(gci -path "./shaders/" | select-string .).count"') do set shaderLines=%%a
for /F "tokens=*" %%a in ('powershell.exe -command "(Get-ChildItem "./src/").Count"') do set srcFiles=%%a
for /F "tokens=*" %%a in ('powershell.exe -command "(Get-ChildItem "./shaders/").Count"') do set shaderFiles=%%a

echo src files : %srcFiles%
echo src lines : %srcLines%
echo --------------------------------------------------
echo shader files : %shaderFiles%
echo shaders lines :  %shaderLines%

echo src files : %srcFiles%> %file%
echo src lines : %srcLines%>> %file%
echo -------------------------------------------------->> %file%
echo shader files : %shaderFiles%>> %file%
echo shaders lines : %shaderLines%>> %file%
pause
exit
@echo on