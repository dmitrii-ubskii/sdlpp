#ifndef SDLPP_ERROR_H_
#define SDLPP_ERROR_H_

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

#endif // SDLPP_ERROR_H_
