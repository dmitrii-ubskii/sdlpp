#ifndef SDLPP_VIDEO_H__
#define SDLPP_VIDEO_H__

#include <string>

#include <SDL2/SDL.h>

#include "sdlpp/geometry.h"

namespace SDL
{
class Window;

struct Color;

class Surface;

class Renderer
{
	public:
		Renderer(Window&);
		~Renderer() noexcept;

		void clear(Color);
		void present() noexcept;

		Rect getViewport() const noexcept;

		void copySurface(Surface const& s, Point p, Alignment align=Alignment::TopLeft);
		void copySurface(Surface const& s, Rect r, Point p, Alignment align=Alignment::TopLeft);
		void drawLine(Point from, Point to, Color);
		void drawRect(Rect, Color);
		void fillRect(Rect, Color);
		void putPixel(Point, Color);

		SDL_Renderer* get() const noexcept;

	private:
		SDL_Renderer* renderer;

		void setColor(Color);
};

class Window
{
	public:
		Window(std::string const& title, Size const s);
		~Window() noexcept;

		Renderer& getRenderer() noexcept;

		SDL_Window* get() const noexcept;

	private:
		SDL_Window* window;
		Renderer renderer;
};
}

#endif // SDLPP_VIDEO_H__
