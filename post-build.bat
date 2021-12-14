@echo off

REM Run from root directory!
if not exist "%cd%\bin\bin\x64\Debug\assets\shaders\" mkdir "%cd%\bin\bin\x64\Debug\assets\shaders"


echo "Current Directory " %cd%

echo "Compiling shaders..."
echo "Kohi/assets/shaders/Builtin.MaterialShader.vert.glsl -> bin/bin/x64/Debug/assets/shaders/Builtin.MaterialShader.vert.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=vert Kohi/assets/shaders/Builtin.MaterialShader.vert.glsl -o bin/bin/x64/Debug/assets/shaders/Builtin.MaterialShader.vert.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Kohi/assets/shaders/Builtin.MaterialShader.frag.glsl -> bin/bin/x64/Debug/assets/shaders/Builtin.MaterialShader.frag.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=frag Kohi/assets/shaders/Builtin.MaterialShader.frag.glsl -o bin/bin/x64/Debug/assets/shaders/Builtin.MaterialShader.frag.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Copying assets..."
echo xcopy "Kohi\assets\" "bin\bin\x64\Debug\assets" /h /i /c /k /e /r /y
xcopy "Kohi\assets\" "bin\bin\x64\Debug\assets" /h /i /c /k /e /r /y

echo xcopy "Kohi\assets\" "Testbed\assets" /h /i /c /k /e /r /y
xcopy "Kohi\assets\" "Testbed\assets" /h /i /c /k /e /r /y


echo "Done."
pause