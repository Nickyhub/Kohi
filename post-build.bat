@echo off


echo "Current Directory " %cd%

echo "Compiling shaders..."
echo "Kohi/assets/shaders/Builtin.MaterialShader.vert.glsl -> assets/shaders/Builtin.MaterialShader.vert.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=vert Kohi/assets/shaders/Builtin.MaterialShader.vert.glsl -o Kohi/assets/shaders/Builtin.MaterialShader.vert.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Kohi/assets/shaders/Builtin.MaterialShader.frag.glsl -> assets/shaders/Builtin.MaterialShader.frag.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=frag Kohi/assets/shaders/Builtin.MaterialShader.frag.glsl -o Kohi/assets/shaders/Builtin.MaterialShader.frag.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Kohi/assets/shaders/Builtin.UIShader.vert.glsl -> assets/shaders/Builtin.UIShader.vert.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=vert Kohi/assets/shaders/Builtin.UIShader.vert.glsl -o Kohi/assets/shaders/Builtin.UIShader.vert.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "Kohi/assets/shaders/Builtin.UIShader.frag.glsl -> assets/shaders/Builtin.UIShader.frag.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=frag Kohi/assets/shaders/Builtin.UIShader.frag.glsl -o Kohi/assets/shaders/Builtin.UIShader.frag.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)


xcopy "Kohi/assets" "bin\bin\x64\Debug\assets" /h /i /c /k /e /r /y
echo "Done."
pause