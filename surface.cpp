#include "sdlpp/surface.h"

#include <cmath>
#include <array>

#include "sdlpp/error.h"
#include "sdlpp/video.h"
#include "sdlpp/pixel.h"

namespace SDL
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    constexpr Uint32 rmask = 0xff000000;
    constexpr Uint32 gmask = 0x00ff0000;
    constexpr Uint32 bmask = 0x0000ff00;
    constexpr Uint32 amask = 0x000000ff;
#else
    constexpr Uint32 rmask = 0x000000ff;
    constexpr Uint32 gmask = 0x0000ff00;
    constexpr Uint32 bmask = 0x00ff0000;
    constexpr Uint32 amask = 0xff000000;
#endif

SDL_Surface* createSurface(SDL::Size size)
{
	auto s = SDL_CreateRGBSurface(0, size.w, size.h, 32, rmask, gmask, bmask, amask);
	if (s == nullptr)
	{
		throw Error{SDL_GetError()};
	}
	return s;
}

Surface::Surface(Size size)
	: surface{createSurface(size)}
{}

Surface::Surface(SDL_Surface* s) noexcept
	: surface{s}
{}

Surface::Surface(std::filesystem::path const& fname)
	: surface{IMG_Load(fname.c_str())}
{
	if (surface == nullptr)
	{
		throw Error{IMG_GetError()};
	}
}

Surface::Surface(Surface&& other) noexcept
	: surface{other.surface}
	, texture{other.texture}
{
	other.surface = nullptr;
	other.texture = nullptr;
}

Surface& Surface::operator=(Surface&& other) noexcept
{
	release();

	surface = other.surface;
	texture = other.texture;

	other.surface = nullptr;
	other.texture = nullptr;

	return *this;
}

Surface::~Surface() noexcept
{
	release();
}

void Surface::release() noexcept
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

Size Surface::getSize() const noexcept
{
	SDL_Rect r;
	SDL_GetClipRect(surface, &r);
	return Rect{r}.s;
}

SDL_Texture* Surface::getTexture(Renderer const& renderer) const
{
	std::unique_lock hold{mutex};
	if (texture == nullptr)
	{
		texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
		if (texture == nullptr)
		{
			throw Error{SDL_GetError()};
		}
	}
	return texture;
}

void Surface::invalidateTexture() noexcept
{
	SDL_DestroyTexture(texture);
	texture = nullptr;
}

void Surface::putPixel(Point p, Color c) noexcept
{
	auto offset = 4 * (p.y * surface->w + p.x);
	auto pixels = static_cast<uint8_t*>(surface->pixels);
	pixels[offset+0] = c.b;
	pixels[offset+1] = c.g;
	pixels[offset+2] = c.r;
	pixels[offset+3] = c.a;

	invalidateTexture();
}

void Surface::fillRect(Rect r, Color c)
{
	SDL_Rect dst = r;
	if (SDL_FillRect(surface, &dst, SDL_MapRGBA(surface->format, c.r, c.g, c.b, c.a)) < 0)
	{
		throw Error{SDL_GetError()};
	}
	invalidateTexture();
}

void Surface::blit(Surface const& other, Point p, Alignment align)
{
	SDL_Rect dst = Rect{p, other.getSize(), align};
	if (SDL_BlitSurface(other.surface, nullptr, surface, &dst) < 0)
	{
		throw Error{SDL_GetError()};
	}
}
}
