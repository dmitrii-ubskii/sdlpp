#ifndef SDLPP_GEOMETRY_H__
#define SDLPP_GEOMETRY_H__

#include <algorithm>
#include <optional>

#include <SDL2/SDL.h>

namespace SDL
{
enum class VerticalAlignment
{
	Top, Middle, Bottom,
};

enum class HorizontalAlignment
{
	Left, Center, Right,
};

struct Alignment
{
	VerticalAlignment v;
	HorizontalAlignment h;

	static Alignment const TopLeft;
	static Alignment const TopCenter;
	static Alignment const TopRight;

	static Alignment const MiddleLeft;
	static Alignment const MiddleCenter;
	static Alignment const MiddleRight;

	static Alignment const BottomLeft;
	static Alignment const BottomCenter;
	static Alignment const BottomRight;
};

constexpr Alignment Alignment::TopLeft = {VerticalAlignment::Top, HorizontalAlignment::Left};
constexpr Alignment Alignment::TopCenter = {VerticalAlignment::Top, HorizontalAlignment::Center};
constexpr Alignment Alignment::TopRight = {VerticalAlignment::Top, HorizontalAlignment::Right};
constexpr Alignment Alignment::MiddleLeft = {VerticalAlignment::Middle, HorizontalAlignment::Left};
constexpr Alignment Alignment::MiddleCenter = {VerticalAlignment::Middle, HorizontalAlignment::Center};
constexpr Alignment Alignment::MiddleRight = {VerticalAlignment::Middle, HorizontalAlignment::Right};
constexpr Alignment Alignment::BottomLeft = {VerticalAlignment::Bottom, HorizontalAlignment::Left};
constexpr Alignment Alignment::BottomCenter = {VerticalAlignment::Bottom, HorizontalAlignment::Center};
constexpr Alignment Alignment::BottomRight = {VerticalAlignment::Bottom, HorizontalAlignment::Right};

struct Point;
struct Rect;

struct Vec2D
{
	int x;
	int y;

	explicit constexpr operator Point() const noexcept;
};

struct Point
{
	int x;
	int y;

	constexpr operator SDL_Point() const noexcept
	{
		return {x, y};
	}

	explicit constexpr operator Vec2D() const noexcept
	{
		return {x, y};
	}

	constexpr bool operator==(Point const& other) const noexcept = default;

	constexpr bool in(Rect r) const noexcept;
};

constexpr Vec2D::operator Point() const noexcept
{
	return {x, y};
}

constexpr Vec2D operator-(Point lhs, Point rhs) noexcept
{
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}

constexpr Point operator-(Point lhs, Vec2D rhs) noexcept
{
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}

constexpr Point operator+(Point lhs, Vec2D rhs) noexcept
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr Vec2D operator+(Vec2D lhs, Vec2D rhs) noexcept
{
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

constexpr Point operator+(Vec2D lhs, Point rhs) noexcept
{
	return rhs + lhs;
}

struct Size
{
	int w;
	int h;

	constexpr bool operator==(Size const& other) const noexcept = default;
};

struct OptionalSize
{
	std::optional<int> w;
	std::optional<int> h;

	constexpr OptionalSize(std::nullopt_t = std::nullopt) noexcept
		: w{std::nullopt}
		, h{std::nullopt}
	{}

	constexpr OptionalSize(Size sz) noexcept
		: w{sz.w}
		, h{sz.h}
	{}

	constexpr OptionalSize(std::optional<int> w_, std::optional<int> h_) noexcept
		: w{w_}
		, h{h_}
	{}

	constexpr bool isFixed() const noexcept
	{
		return w.has_value() && h.has_value();
	}

	constexpr operator Size() const
	{
		if (not this->isFixed())
		{
			throw std::runtime_error(
				"ERROR: Attempting to convert unfixed OptionalSize to Size "
				"(both width and height must have value)"
			);
		}
		return {*w, *h};
	}
};

constexpr Size min(Size s1, Size s2)
{
	return {std::min(s1.w, s2.w), std::min(s1.h, s2.h)};
}

constexpr Size max(Size s1, Size s2)
{
	return {std::max(s1.w, s2.w), std::max(s1.h, s2.h)};
}

constexpr Size operator+(Size s, Vec2D v)
{
	return {s.w + v.x, s.h + v.y};
}

constexpr Size operator-(Size s, Vec2D v)
{
	return {s.w - v.x, s.h - v.y};
}

constexpr OptionalSize min(OptionalSize s1, OptionalSize s2)
{
	auto minopt = [](auto a, auto b) -> std::optional<int>
	{
		if (not a.has_value())
		{
			return b;
		}
		if (not b.has_value())
		{
			return a;
		}
		return std::min(*a, *b);
	};
	return {minopt(s1.w, s2.w), minopt(s1.h, s2.h)};
}

constexpr OptionalSize max(OptionalSize s1, OptionalSize s2)
{
	auto maxopt = [](auto a, auto b) -> std::optional<int>
	{
		if (not a.has_value() or not b.has_value())
		{
			return std::nullopt;
		}
		return std::max(*a, *b);
	};
	return {maxopt(s1.w, s2.w), maxopt(s1.h, s2.h)};
}

struct Rect
{
	Point p;
	Size s;

	constexpr Rect(Point p_, Size s_) noexcept: p{p_}, s{s_} {}
	constexpr Rect(SDL_Rect r) noexcept: p{r.x, r.y}, s{r.w, r.h} {}

	constexpr bool operator==(Rect const& other) const noexcept = default;

	constexpr Rect(Point p_, Size s_, Alignment align) noexcept
		: p{p_}, s{s_}
	{
		switch (align.v)
		{
			case VerticalAlignment::Top:
				break;
			case VerticalAlignment::Middle:
				p.y -= s.h / 2;
				break;
			case VerticalAlignment::Bottom:
				p.y -= s.h;
				break;
		}

		switch (align.h)
		{
			case HorizontalAlignment::Left:
				break;
			case HorizontalAlignment::Center:
				p.x -= s.w / 2;
				break;
			case HorizontalAlignment::Right:
				p.x -= s.w;
				break;
		}
	}

	constexpr Point alignedPoint(Alignment align) const noexcept
	{
		auto t = p;

		switch (align.v)
		{
			case VerticalAlignment::Top:
				break;
			case VerticalAlignment::Middle:
				t.y += s.h / 2;
				break;
			case VerticalAlignment::Bottom:
				t.y += s.h;
				break;
		}

		switch (align.h)
		{
			case HorizontalAlignment::Left:
				break;
			case HorizontalAlignment::Center:
				t.x += s.w / 2;
				break;
			case HorizontalAlignment::Right:
				t.x += s.w;
				break;
		}

		return t;
	}

	constexpr operator SDL_Rect() const noexcept
	{
		return {p.x, p.y, s.w, s.h};
	}
};

constexpr bool Point::in(Rect r) const noexcept
{
	auto dx = x - r.p.x;
	auto dy = y - r.p.y;
	return (dx >= 0) && (dx < r.s.w) && (dy >= 0) && (dy < r.s.h);
}
}

#endif // SDLPP_GEOMETRY_H__
