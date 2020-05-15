#ifndef SDLPP_FONT_H__
#define SDLPP_FONT_H__

#include <string>
#include <unordered_map>
#include <mutex>

#include <SDL2/SDL_ttf.h>

namespace SDL
{
struct Color;

class Surface;

class Font
{
	public:
		Font(std::string file, int ptsize=16);

		Font(Font const&) = delete;
		Font& operator=(Font const&) = delete;

		Font(Font&& other) noexcept;
		Font& operator=(Font&& other) noexcept;

		~Font() noexcept;

		Surface render(std::string text, int ptsize, Color color) const;
		Surface renderWrapped(std::string text, int ptsize, unsigned int width, Color color) const;

	private:
		std::string file;  // need to reopen for a different ptsize
		// TODO FIXME figure out how to use SDL_RWops to read from disk into memory instead

		mutable std::unordered_map<int, TTF_Font*> fonts;
		void addSize(int ptsize) const;

		void release() noexcept;

		mutable std::mutex mutex;
};
}

#endif // SDLPP_FONT_H__
