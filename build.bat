@echo off
set OUTPUT="starter.exe"
set LIBRARIES= "user32.lib" "shell32.lib" "kernel32.lib" "opengl32.lib" "SDL2.lib" "glew32.lib" "SDL2main.lib" "SDL2_image.lib"
set LINKER=/NODEFAULTLIB:library /LIBPATH:third_party/library %LIBRARIES% /SUBSYSTEM:CONSOLE /OUT:%OUTPUT%
set PREPARSER=/D DEBUG_BUILD /D _WINDOWS_

cl main.cpp /Zi /Od %PREPARSER% /MDd /EHsc /I ./third_party/include -FC /link %LINKER% 