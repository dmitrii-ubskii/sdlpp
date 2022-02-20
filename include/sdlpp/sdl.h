#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "sdlpp/error.h"

namespace SDL
{
struct Init
{
	Init()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			throw Error{SDL_GetError()};
		}
		SDL_StopTextInput();  // text input is on by default, apparently
		if (IMG_Init(IMG_INIT_PNG) < 0)
		{
			throw Error{IMG_GetError()};
		}
		if (TTF_Init() < 0)
		{
			throw Error{TTF_GetError()};
		}
		if (Mix_Init(MIX_INIT_MP3) < 0)
		{
			throw Error{Mix_GetError()};
		}
	}

	~Init()
	{
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
};
}
