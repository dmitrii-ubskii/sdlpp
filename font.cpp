#include "sdlpp/font.h"

#include "sdlpp/error.h"
#include "sdlpp/pixel.h"
#include "sdlpp/surface.h"

using namespace std::literals;

namespace SDL
{
Font::Font(std::string file_, int ptsize)
	: file{file_}
{
	addSize(ptsize);
}

Font::~Font() noexcept
{
	release();
}

Font::Font(Font&& other) noexcept
{
	std::unique_lock<std::mutex> otherPin{other.mutex};
	file = std::move(other.file);
	fonts = std::move(other.fonts);
}

Font& Font::operator=(Font&& other) noexcept
{
	if (this != &other)
	{
		std::unique_lock<std::mutex> pin{mutex};
		std::unique_lock<std::mutex> otherPin{other.mutex};

		release();
	
		file = std::move(other.file);
		fonts = std::move(other.fonts);
	}

	return *this;
}

void Font::addSize(int ptsize) const
{
	std::unique_lock hold {mutex};
	if (fonts.find(ptsize) == fonts.end())
	{
		fonts.insert({ptsize, TTF_OpenFont(file.c_str(), ptsize)});
		if (fonts[ptsize] == nullptr)
		{
			throw Error{TTF_GetError()};
		}
	}
}

Surface Font::render(std::string text, int ptsize, Color color) const
{
	addSize(ptsize);
	auto s = TTF_RenderUTF8_Blended(fonts[ptsize], text.c_str(), color);
	if (s == nullptr)
	{
		if (TTF_GetError() == "Text has zero width"s)  // vexing exception workaround
		{
			return Surface(Size{1, 1});
		}
		throw Error{TTF_GetError()};
	}
	return s;
}

Surface Font::renderWrapped(std::string text, int ptsize, unsigned int width, Color color) const
{
	addSize(ptsize);
	auto s = TTF_RenderUTF8_Blended_Wrapped(fonts[ptsize], text.c_str(), color, width);
	if (s == nullptr)
	{
		if (TTF_GetError() == "Text has zero width"s)  // vexing exception workaround
		{
			return Surface(Size{1, 1});
		}
		throw Error{TTF_GetError()};
	}
	return s;
}

void Font::release() noexcept
{
	for (auto&& [_, font]: fonts)
	{
		TTF_CloseFont(font);
	}
}
}
