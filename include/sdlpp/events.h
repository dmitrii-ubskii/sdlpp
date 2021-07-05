#ifndef SDLPP_EVENTS_H_
#define SDLPP_EVENTS_H_

#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <variant>
#include <queue>

#include <SDL2/SDL.h>

#include "sdlpp/geometry.h"

namespace SDL
{
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
	std::uint32_t windowId;
	std::uint32_t mouseId;

	MouseButton button;
	ButtonState state;
	std::uint8_t clicks;

	Point p;

	MouseButtonEvent(SDL_MouseButtonEvent ev) noexcept
		: windowId{ev.windowID}
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

enum class WindowEventType
{
	WindowShown, WindowHidden,
	WindowExposed,
	WindowSizeChanged, /* this event is followed by WindowResized
	                    * if the size was changed by an external event (i.e. user or WM) */
	WindowMinimized, WindowMaximized, WindowRestored,
	WindowEnter, WindowLeave,
	WindowFocusGained, WindowFocusLost,
};

struct WindowEvent
{
	std::uint32_t windowId;
	WindowEventType type;

	WindowEvent(SDL_WindowEvent ev)
		: windowId{ev.windowID}
	{
		switch (ev.event)
		{
			case SDL_WINDOWEVENT_SHOWN:
				type = WindowEventType::WindowShown;
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				type = WindowEventType::WindowHidden;
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				type = WindowEventType::WindowExposed;
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				type = WindowEventType::WindowSizeChanged;
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				type = WindowEventType::WindowMinimized;
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				type = WindowEventType::WindowMaximized;
				break;
			case SDL_WINDOWEVENT_RESTORED:
				type = WindowEventType::WindowRestored;
				break;
			case SDL_WINDOWEVENT_ENTER:
				type = WindowEventType::WindowEnter;
				break;
			case SDL_WINDOWEVENT_LEAVE:
				type = WindowEventType::WindowLeave;
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				type = WindowEventType::WindowFocusGained;
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				type = WindowEventType::WindowFocusLost;
				break;
		}
	}
};

struct WindowMovedEvent
{
	std::uint32_t windowId;
	std::int32_t x;
	std::int32_t y;

	WindowMovedEvent(SDL_WindowEvent ev)
		: windowId{ev.windowID}
		, x{ev.data1}
		, y{ev.data2}
	{}
};

struct WindowResizedEvent
{
	std::uint32_t windowId;
	std::int32_t w;
	std::int32_t h;

	WindowResizedEvent(SDL_WindowEvent ev)
		: windowId{ev.windowID}
		, w{ev.data1}
		, h{ev.data2}
	{}
};

enum class EventType
{
	Quit,
	/* Android & iOS events */
	AppTerminating, AppLowMemory,
	AppWillEnterBackground, AppDidEnterBackground,
	AppWillEnterForeground, AppDidEnterForeground,
	/* Window events */
	WindowMoved,
	WindowResized,
	WindowEvent,
	/* Keyboard events */
	KeyDown, KeyUp,
	TextEditing, TextInput,
	/* Mouse events */
	MouseMotion, MouseButtonDown, MouseButtonUp, MouseWheel,
	/* Joystick events */
	JoyAxisMotion, JoyBallMotion, JoyHatMotion,
	JoyButtonDown, JoyButtonUp,
	JoyDeviceAdded, JoyDeviceRemoved,
	/* Controller events */
	ControllerAxisMotion, ControllerButtonDown, ControllerButtonUp,
	ControllerDeviceAdded, ControllerDeviceRemoved, ControllerDeviceRemapped,
	/* Touch events */
	FingerDown, FingerUp, FingerMotion,
	/* Gesture events */
	DollarGesture, DollarRecord, MultiGesture,
	/* Misc */
	ClipboardUpdate,
	DropFile,
	UserEvent,
	NotImplemented
};

struct Event
{
	static Event fromSdlEvent(SDL_Event ev)
	{
		auto timestamp = ev.common.timestamp;
		switch (ev.type)
		{
			case SDL_WINDOWEVENT:
					switch (ev.window.event)
					{
						case SDL_WINDOWEVENT_MOVED:
							return {EventType::WindowMoved, timestamp, WindowMovedEvent{ev.window}};
						case SDL_WINDOWEVENT_RESIZED:
							return {EventType::WindowResized, timestamp, WindowResizedEvent{ev.window}};
						default:
							return {EventType::WindowEvent, timestamp, WindowEvent{ev.window}};
					}
			case SDL_MOUSEBUTTONUP:
				return {EventType::MouseButtonUp, timestamp, MouseButtonEvent{ev.button}};
			case SDL_MOUSEBUTTONDOWN:
				return {EventType::MouseButtonDown, timestamp, MouseButtonEvent{ev.button}};
			default:
				return {EventType::NotImplemented, ev.common.timestamp, ev};
		}
	}

	EventType tag;
	std::uint32_t timestamp;

	using EventVariant = std::variant<
		WindowEvent,
		WindowResizedEvent,
		WindowMovedEvent,
		MouseButtonEvent,
		SDL_Event
	>;
	EventVariant event;
};

class EventQueue
{
	public:
		void pumpEvents()
		{
			SDL_Event ev;
			while (SDL_PollEvent(&ev) != 0)
			{
				push(Event::fromSdlEvent(ev));
			}
		}

		Event wait_pop()
		{
			std::unique_lock<std::mutex> pin{m};
			cv.wait(pin, [this]{ return not empty(); });
			return pop_unsafe();
		}

		void push(Event ev)
		{
			std::unique_lock<std::mutex> pin{m};
			queue_.push(ev);
			cv.notify_all();
		}

		bool empty() const
		{
			return queue_.empty();
		}

	private:
		mutable std::mutex m;
		std::condition_variable cv;

		Event pop_unsafe()
		{
			auto ev = queue_.front();
			queue_.pop();
			return ev;
		}

		std::queue<Event> queue_;
};
}

#endif // SDLPP_EVENTS_H_
