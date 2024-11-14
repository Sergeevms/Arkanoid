#pragma once
#include <random>

namespace Arkanoid
{
	template <bool is_integral, typename T>
	struct Uniform_distribution_selector;

	template <typename T>
	struct Uniform_distribution_selector<true, T> 
	{
		using type = std::uniform_int_distribution<T>;
	};

	template <typename T>
	struct Uniform_distribution_selector<false, T>
	{
		using type = std::uniform_real_distribution<T>;
	};

	template <typename T>
	T random(T lower = T(0), T higher = T(99))
	{
		if (lower >= higher)
		{
			return lower;
		}

		using Uniform_distribution_type = typename Uniform_distribution_selector<std::is_integral<T>::value, T>::type;

		Uniform_distribution_type distribution(lower, higher);
		static std::mt19937 engine;

		return distribution(engine);
	}
}