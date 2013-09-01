@ECHO OFF
ECHO Post-Build-Script
ECHO Copying Assets to debug build
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\game\MiniMiner\Debug\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\game\MiniMiner\Debug"
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\test\RenderManagerTest\Debug\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\test\RenderManagerTest\Debug"
ECHO Copying Assets to release build
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\game\MiniMiner\Release\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\game\MiniMiner\Release"
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\test\RenderManagerTest\Release\assets"
XCOPY /I /E /Y "%CD%\bin\*" "%CD%\test\RenderManagerTest\Release"
ECHO Copy assets to projects
XCOPY /I /E /Y "%CD%\assets\*" "%CD%\projects\RenderManagerTest\assets"
ECHO Done