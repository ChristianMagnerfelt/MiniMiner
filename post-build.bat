@ECHO OFF
ECHO Post-Build-Script
ECHO Copying Assets to debug build
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\game\MiniMiner\Debug\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\game\MiniMiner\Debug"
ECHO Copying Assets to release build
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\game\MiniMiner\Release\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\game\MiniMiner\Release"
ECHO Done