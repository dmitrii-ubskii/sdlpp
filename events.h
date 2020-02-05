#ifndef SDLPP_EVENTS_H__
#define SDLPP_EVENTS_H__

#include <SDL2/SDL.h>

#include "sdlpp/geometry.h"

namespace SDL
{
enum class EventType
{
	MouseButtonUp, MouseButtonDown
};

enum class MouseButton
{
	Left, Middle, Right, X1, X2
};

enum class ButtonState
{
	Pressed, Released
};

struct MouseButtonEvent
{
	std::uint32_t timestamp;
	std::uint32_t windowId;
	std::uint32_t mouseId;

	MouseButton button;
	ButtonState state;
	std::uint8_t clicks;

	Point p;

	MouseButtonEvent(SDL_MouseButtonEvent ev) noexcept
		: timestamp{ev.timestamp}
		, windowId{ev.windowID}
		, mouseId{ev.which}
		, clicks{ev.clicks}
		, p{ev.x, ev.y}
	{
		switch (ev.state)
		{
			case SDL_PRESSED:
				state = ButtonState::Pressed;
				break;

			case SDL_RELEASED:
				state = ButtonState::Released;
				break;
		}

		switch (ev.button)
		{
			case SDL_BUTTON_LEFT:
				button = MouseButton::Left;
				break;

			case SDL_BUTTON_MIDDLE:
				button = MouseButton::Middle;
				break;

			case SDL_BUTTON_RIGHT:
				button = MouseButton::Right;
				break;

			case SDL_BUTTON_X1:
				button = MouseButton::X1;
				break;

			case SDL_BUTTON_X2:
				button = MouseButton::X2;
				break;
		}
	}
};
}

#endif // SDLPP_EVENTS_H__
