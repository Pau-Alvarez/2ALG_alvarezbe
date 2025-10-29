@ECHO off
cls
.\Premake5\Premake5.exe --file=premake_config.lua vs2022
FOR /D %%i IN (.\..\build\*.*) DO copy edk_memory_configuration.cfg .\%%i\vs2022\

pause
