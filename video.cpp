#include "sdlpp/video.h"

#include "sdlpp/error.h"
#include "sdlpp/pixel.h"
#include "sdlpp/surface.h"

namespace SDL
{
Window::Window(std::string const& title, Size s)
	: window{SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			s.w, s.h,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
		)}
	, renderer{*this}
{
	if (window == nullptr)
	{
		throw Error{SDL_GetError()};
	}
}

Window::~Window() noexcept
{
	SDL_DestroyWindow(window);
}

Renderer& Window::getRenderer() noexcept
{
	return renderer;
}

SDL_Window* Window::get() const noexcept
{
	return window;
}

Renderer::Renderer(Window& w)
	: renderer{SDL_CreateRenderer(w.get(), -1, SDL_RENDERER_ACCELERATED)} // | SDL_RENDERER_PRESENTVSYNC)}
{
	if (renderer == nullptr)
	{
		throw Error{SDL_GetError()};
	}
	if (SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

Renderer::~Renderer() noexcept
{
	SDL_DestroyRenderer(renderer);
}

void Renderer::clear(Color c)
{
	setColor(c);
	if (SDL_RenderClear(renderer) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

void Renderer::present() noexcept
{
	SDL_RenderPresent(renderer);
}

Rect Renderer::getViewport() const noexcept
{
	SDL_Rect r;
	SDL_RenderGetViewport(renderer, &r);
	return r;
}

void Renderer::copySurface(Surface const& s, Point p, Alignment align)
{
	copySurface(s, {{}, s.getSize()}, p, align);
}

void Renderer::copySurface(Surface const& s, Rect r, Point p, Alignment align)
{
	SDL_Rect src = r;
	SDL_Rect dst = Rect{p, r.s, align};

	if (SDL_RenderCopy(renderer, s.getTexture(*this), &src, &dst) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

void Renderer::drawLine(Point from, Point to, Color c)
{
	setColor(c);
	if (SDL_RenderDrawLine(renderer, from.x, from.y, to.x, to.y) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

void Renderer::drawRect(Rect r, Color c)
{
	SDL_Rect r_ = r;
	setColor(c);
	if (SDL_RenderDrawRect(renderer, &r_) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

void Renderer::fillRect(Rect r, Color c)
{
	SDL_Rect r_ = r;
	setColor(c);
	if (SDL_RenderFillRect(renderer, &r_) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

void Renderer::putPixel(Point p, Color c)
{
	setColor(c);
	if (SDL_RenderDrawPoint(renderer, p.x, p.y) < 0)
	{
		throw Error{SDL_GetError()};
	}
}

SDL_Renderer* Renderer::get() const noexcept
{
	return renderer;
}

void Renderer::setColor(Color c)
{
	if (SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a) < 0)
	{
		throw Error{SDL_GetError()};
	}
}
}

