@echo off

set CommonLinkerFlags= user32.lib gdi32.lib winmm.lib kernel32.lib opengl32.lib
 
IF NOT EXIST build mkdir build
pushd build
cl -FC -Zi ../win32_Sandpile.cpp  user32.lib Gdi32.lib opengl32.lib 
popd

