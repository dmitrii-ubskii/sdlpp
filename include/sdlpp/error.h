#pragma once

#include <stdexcept>

#include <SDL2/SDL.h>

namespace SDL
{
class Error: public std::runtime_error
{
	public:
		Error(std::string const& message) noexcept: std::runtime_error{message} {}
};
}
