#ifndef SDLPP_PIXEL_H__
#define SDLPP_PIXEL_H__

#include <SDL2/SDL.h>

namespace SDL
{
struct Color
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t a = 0xFF;

	static constexpr Color RGB(int rgb) noexcept
	{
		uint8_t r = static_cast<uint8_t>((rgb & 0xFF0000) >> 16);
		uint8_t g = static_cast<uint8_t>((rgb & 0x00FF00) >> 8);
		uint8_t b = static_cast<uint8_t>((rgb & 0x0000FF));
		return {r, g, b};
	}

	constexpr operator SDL_Color() const noexcept
	{
		return {r, g, b, a};
	}
};

}

#endif // SDLPP_PIXEL_H__
