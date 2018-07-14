#pragma once
#include <cstdint>

class Color
{
public:
	float r, g, b, a;

	Color(float _r,float _g,float _b, float _a)
	{
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	Color(const Color& other)
	{
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->a = other.a;
	}

	// casting
	operator DWORD () const
	{				
		return (uint8_t)(r * 255.0f + 0.5f) << 24 &
			   (uint8_t)(g * 255.0f + 0.5f) << 16 & 
			   (uint8_t)(b * 255.0f + 0.5f) << 8 & 
			   (uint8_t)(a * 255.0f + 0.5f);
	}

	operator float* ()
	{
		return (float*)this;
	}
};