#ifndef SDLPP_SURFACE_H__
#define SDLPP_SURFACE_H__

#include <string>
#include <mutex>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sdlpp/geometry.h"

namespace SDL
{
class Renderer;

struct Color;

class Surface
{
	public:
		Surface(Size size);
		Surface(SDL_Surface* s) noexcept;
		Surface(std::filesystem::path const& fname);

		Surface(Surface const&) = delete;
		Surface& operator=(Surface const&) = delete;

		Surface(Surface&& other) noexcept;
		Surface& operator=(Surface&& other) noexcept;

		~Surface() noexcept;

		Size getSize() const noexcept;
		SDL_Texture* getTexture(Renderer const&) const;

		void putPixel(Point, Color) noexcept;
		void fillRect(Rect, Color);
		void blit(Surface const& other, Point p, Alignment align=Alignment::TopLeft);
	
	private:
		void release() noexcept;
		void invalidateTexture() noexcept;

		SDL_Surface* surface = nullptr;
		mutable SDL_Texture* texture = nullptr;

		mutable std::mutex mutex;
};
}

#endif // SDLPP_SURFACE_H__
