@ECHO off

SET LIBS_DIRS=/LIBPATH:..\deps\glfw3\bin\ /LIBPATH:..\deps\glew\
SET LIBS=kernel32.lib GDI32.lib shell32.lib user32.lib opengl32.lib glfw3_mt.lib glew32s.lib
SET INCLUDE_DIRS=/I ..\deps\glew\include /I ..\deps\glfw3\include
SET SRC_FILES=..\src\ogl.cpp ..\src\main.cpp

pushd .

REM mkdir build
cd build

cl %SRC_FILES% %INCLUDE_DIRS% /Zi /link %LIBS% %LIBS_DIRS% 

if NOT ERRORLEVEL 1 call ogl.exe

popd
exit
