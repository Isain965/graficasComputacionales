#pragma once

namespace cgmath
{
	constexpr float pi = 3.14159226535f;

	inline constexpr float radians(const float& degs)
	{
		return degs * 0.017453292f;
	}
}
