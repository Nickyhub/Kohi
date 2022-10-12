@echo off

echo "Compiling shaders..."

PUSHD bin
bin\x64\Debug\tools.exe buildshaders ^
..\Kohi\assets\shaders\Builtin.MaterialShader.vert.glsl ^
..\Kohi\assets\shaders\Builtin.MaterialShader.frag.glsl ^
..\Kohi\assets\shaders\Builtin.UIShader.vert.glsl ^
..\Kohi\assets\shaders\Builtin.UIShader.frag.glsl ^
..\Kohi\assets\shaders\Builtin.SkyboxShader.vert.glsl ^
..\Kohi\assets\shaders\Builtin.SkyboxShader.frag.glsl ^
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit)

POPD


echo "assets\shaders\Builtin.SkyboxShader.vert.glsl -> assets\shaders\Builtin.SkyboxShader.vert.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=vert Kohi\assets\shaders\Builtin.SkyboxShader.vert.glsl -o Kohi\assets\shaders\Builtin.SkyboxShader.vert.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

echo "assets\shaders\Builtin.SkyboxShader.frag.glsl -> assets\shaders\Builtin.SkyboxShader.frag.spv"
%VULKAN_SDK%\bin\glslc.exe -fshader-stage=frag Kohi\assets\shaders\Builtin.SkyboxShader.frag.glsl -o Kohi\assets\shaders\Builtin.SkyboxShader.frag.spv
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)

xcopy "Kohi\assets" "bin\bin\x64\Debug\assets" /h /i /c /k /e /r /y
IF %ERRORLEVEL% NEQ 0 (echo Error: %ERRORLEVEL% && exit)
echo "Done."


echo "Done."
PAUSE