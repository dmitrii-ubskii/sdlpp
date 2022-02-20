#pragma once

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

	static constexpr Color RGBA(unsigned int rgba) noexcept
	{
		uint8_t r = static_cast<uint8_t>((rgba & 0xFF000000) >> 24);
		uint8_t g = static_cast<uint8_t>((rgba & 0x00FF0000) >> 16);
		uint8_t b = static_cast<uint8_t>((rgba & 0x0000FF00) >> 8);
		uint8_t a = static_cast<uint8_t>((rgba & 0x000000FF));
		return {r, g, b, a};
	}

	constexpr operator SDL_Color() const noexcept
	{
		return {r, g, b, a};
	}

	static Color const Black;
	static Color const Blue;
	static Color const Green;
	static Color const Cyan;
	static Color const Red;
	static Color const Magenta;
	static Color const Yellow;
	static Color const White;
};

constexpr Color const Color::Black = Color::RGB(0x000000);
constexpr Color const Color::Blue = Color::RGB(0x0000FF);
constexpr Color const Color::Green = Color::RGB(0x00FF00);
constexpr Color const Color::Cyan = Color::RGB(0x00FFFF);
constexpr Color const Color::Red = Color::RGB(0xFF0000);
constexpr Color const Color::Magenta = Color::RGB(0xFF00FF);
constexpr Color const Color::Yellow = Color::RGB(0xFFFF00);
constexpr Color const Color::White = Color::RGB(0xFFFFFF);
}
