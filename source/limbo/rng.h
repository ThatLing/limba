#pragma once

/*
* Copyright 2023 Ling
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include <cstdint>
#include <limits>

namespace limbo
{
	//
	// This namespace implements compile-time random number generation.
	// We use the compilation timestamp as a seed for xorshift
	// https://en.wikipedia.org/wiki/Xorshift
	//
	namespace rng
	{
		// 512 is the max recursion depth
		constexpr auto max_depth = 448;

		constexpr auto time = __TIME__;
		constexpr auto seed = static_cast<int>(time[7]) + static_cast<int>(time[6]) * 10 + static_cast<int>(time[4]) * 60 + static_cast<int>(time[3]) * 600 + static_cast<int>(time[1]) * 3600 + static_cast<int>(time[0]) * 36000;

		template <int N>
		LIMBO_FORCEINLINE constexpr std::uint64_t xorshift()
		{
			constexpr std::uint64_t s = xorshift<N - 1>();
			constexpr std::uint64_t x1 = s ^ (s >> 12);
			constexpr std::uint64_t x2 = x1 ^ (x1 << 25);
			constexpr std::uint64_t x3 = x2 ^ (x2 >> 27);

			return x3 * 0x2545F4914F6CDD1DULL;
		}

		template <>
		LIMBO_FORCEINLINE constexpr std::uint64_t xorshift<0>()
		{
			constexpr std::uint64_t x1 = seed ^ (seed >> 12);
			constexpr std::uint64_t x2 = x1 ^ (x1 << 25);
			constexpr std::uint64_t x3 = x2 ^ (x2 >> 27);

			return x3 * 0x2545F4914F6CDD1DULL;
		}

		// Returns a random uint64_t in range of [0, max]
		template <std::uint64_t N>
		LIMBO_FORCEINLINE constexpr std::uint64_t random_int(std::uint64_t max = (std::numeric_limits<std::uint64_t>::max)())
		{
			return xorshift<N % max_depth>() % max;
		}

		// Returns a random uint64_t in range of [min, max]
		template <std::uint64_t N>
		LIMBO_FORCEINLINE constexpr std::uint64_t random_int(std::uint64_t min, std::uint64_t max)
		{
			return min + random_int<N>(max - min - 1);
		}
	}
}
