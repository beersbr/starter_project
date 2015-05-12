#include <SDL2/sdl.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <SDL2/sdl_opengl.h>

#include <iostream>
#include <cstdlib>

#ifdef _WINDOWS_
#include <windows.h>
#endif 

#include "glutil.cpp"

#define KILOBYTE(n) ((n)*1024)
#define MEGABYTE(n) ((n)*1024)
#define GIGABYTE(n) ((n)*1024)
#define TERABYTE(n) ((n)*1024)

int main(int argc, char *argv[])
{

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		#if DEBUG_BUILD
		std::cout << "Could not initialize SDL 2.0" << std::endl;
		#endif

		exit(1);
	}

	IMG_Init(IMG_INIT_PNG);

	return 0;
}