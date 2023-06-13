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

namespace limbo
{
	//
	// This namespace implements "emulation" of arithmetic expressions
	// NOTE: Using 'auto' in the function prototypes below requires C++20
	//			but decreased compile time in my testing (also looks cleaner).
	//			To enable C++17 support, replace the 'auto' with 'template <typename T>'
	//
	namespace emulation
	{
		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto AddR(auto rhs, auto lhs);

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto SubR(auto rhs, auto lhs);

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto OrR(auto rhs, auto lhs);

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto XorR(auto rhs, auto lhs);

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto AndR(auto rhs, auto lhs);

		// //

		LIMBO_FORCEINLINE auto Add(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp + lhs;
		}

		LIMBO_FORCEINLINE auto Sub(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp - lhs;
		}

		LIMBO_FORCEINLINE auto Mul(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp * lhs;
		}

		LIMBO_FORCEINLINE auto Div(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp / lhs;
		}

		LIMBO_FORCEINLINE auto And(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp & lhs;
		}

		LIMBO_FORCEINLINE auto Or(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp | lhs;
		}

		LIMBO_FORCEINLINE auto Not(auto rhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return ~tmp;
		}

		LIMBO_FORCEINLINE auto Minus(auto rhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return -tmp;
		}

		LIMBO_FORCEINLINE auto Xor(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp ^ lhs;
		}

		LIMBO_FORCEINLINE auto Shl(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp << lhs;
		}

		LIMBO_FORCEINLINE auto Shr(auto rhs, auto lhs)
		{
			auto tmp = limbo::helpers::load_from_reg(rhs);
			return tmp >> lhs;
		}

		// //
		// TODO: Add more/better rewrite rules

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Add2(auto rhs, auto lhs)
		{
			// (rhs & lhs) + (rhs | lhs)
			return AddR<N, R>(AndR<N, R>(rhs, lhs), OrR<N, R>(rhs, lhs));
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Add3(auto rhs, auto lhs)
		{
			// (rhs ^ lhs) + 2 * (rhs & lhs)
			return AddR<N, R>(XorR<N, R>(rhs, lhs), Mul(2, AndR<N, R>(rhs, lhs)));
		}


		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Sub2(auto rhs, auto lhs)
		{
			// (rhs ^ -lhs) + 2 * (rhs & -lhs)
			return AddR<N, R>(XorR<N, R>(rhs, Minus(lhs)), Mul(2, AndR<N, R>(rhs, Minus(lhs))));
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Sub3(auto rhs, auto lhs)
		{
			// (rhs + 1) + ~lhs
			return AddR<N, R>(AddR<N, R>(rhs, 1), Not(lhs));
		}


		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto And2(auto rhs, auto lhs)
		{
			// (~rhs | lhs) - ~rhs
			return SubR<N, R>(OrR<N, R>(Not(rhs), lhs), Not(rhs));
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto And3(auto rhs, auto lhs)
		{
			// ((~rhs | lhs) + rhs) + 1
			return AddR<N, R>(AddR<N, R>(OrR<N, R>(Not(rhs), lhs), rhs), 1);
		}


		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Or2(auto rhs, auto lhs)
		{
			// (rhs & ~lhs) + lhs
			return AddR<N, R>(AndR<N, R>(rhs, Not(lhs)), lhs);
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Or3(auto rhs, auto lhs)
		{
			// ((rhs + lhs) + 1) + ((-rhs - 1) | (-lhs - 1))
			return AddR<N, R>(AddR<N, R>(AddR<N, R>(rhs, lhs), 1), OrR<N, R>(SubR<N, R>(Minus(rhs), 1), SubR<N, R>(Minus(lhs), 1)));
		}


		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Xor2(auto rhs, auto lhs)
		{
			// (~rhs & lhs) | (rhs & ~lhs)
			return OrR<N, R>(AndR<N, R>(Not(rhs), lhs), AndR<N, R>(rhs, Not(lhs)));
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto Xor3(auto rhs, auto lhs)
		{
			// (rhs | lhs) - (rhs & rhs)
			return SubR<N, R>(OrR<N, R>(rhs, lhs), AndR<N, R>(rhs, lhs));
		}

		// //

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto AddR(auto rhs, auto lhs)
		{
#ifdef DEBUG
			// Don't emulate on debug builds
			return Add(rhs, lhs);
#else
			// Check how many iterations left
			if constexpr (N > 0)
			{
				constexpr auto i = R % 2;

				if constexpr (i == 0)
				{
					return Add2<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
				else
				{
					return Add3<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
			}
			else
			{
				return Add(rhs, lhs);
			}
#endif
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto SubR(auto rhs, auto lhs)
		{
#ifdef DEBUG
			// Don't emulate on debug builds
			return Sub(rhs, lhs);
#else
			if constexpr (N > 0)
			{
				constexpr auto i = R % 2;

				if constexpr (i == 0)
				{
					return Sub2<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
				else
				{
					return Sub3<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
			}
			else
			{
				return Sub(rhs, lhs);
			}
#endif
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto AndR(auto rhs, auto lhs)
		{
#ifdef DEBUG
			// Don't emulate on debug builds
			return And(rhs, lhs);
#else
			if constexpr (N > 0)
			{
				constexpr auto i = R % 2;

				if constexpr (i == 0)
				{
					return And2<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
				else
				{
					return And3<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
			}
			else
			{
				return And(rhs, lhs);
			}
#endif
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto OrR(auto rhs, auto lhs)
		{
#ifdef DEBUG
			// Don't emulate on debug builds
			return Or(rhs, lhs);
#else
			if constexpr (N > 0)
			{
				constexpr auto i = R % 3;

				if constexpr (i == 0)
				{
					return Or2<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
				else
				{
					return Or3<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
			}
			else
			{
				return Or(rhs, lhs);
			}
#endif
		}

		template <std::uint8_t N, std::uint64_t R>
		LIMBO_FORCEINLINE auto XorR(auto rhs, auto lhs)
		{
#ifdef DEBUG
			// Don't emulate on debug builds
			return Xor(rhs, lhs);
#else
			if constexpr (N > 0)
			{
				constexpr auto i = R % 5;

				if constexpr (i == 0)
				{
					return Xor2<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
				else
				{
					return Xor3<N - 1, LIMBO_RAND_FASTSEED(R)>(rhs, lhs);
				}
			}
			else
			{
				return Xor(rhs, lhs);
			}
#endif
		}
	}
}
