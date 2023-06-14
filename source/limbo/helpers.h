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

#include <array>

namespace limbo
{
	//
	// This namespace implements helper/utility functions that make our life easier
	//
	namespace helpers
	{
		//
		// Forces the compiler to generate code to load a value from a register instead of putting it into .data
		// Modified from xorstr (https://github.com/JustasMasiulis/xorstr/blob/master/include/xorstr.hpp#L88)
		//
		template<typename T>
		LIMBO_FORCEINLINE constexpr T load_from_reg(T value) noexcept
		{
#if defined(__clang__) || defined(__GNUC__)
			// Handles Clang(-CL) and GCC. This generates cleaner asm so we prefer this for Clang-CL
			asm(
				"" 
				: "=r"(value)
				: "0"(value) 
				:
			);

			return value;
#elif _MSC_VER
			// Handles MSVC
			volatile T reg = value;
			return reg;
#else
			#error "Unsupported compiler"
#endif
		}

		//
		// Forces the compiler to generate code to load an immediate value into a register
		//
		template<typename T>
		LIMBO_FORCEINLINE constexpr T load_from_reg_imm(T value) noexcept
		{
#if defined(__clang__) || defined(__GNUC__)
			//
			// Generated code isn't perfect (see below), but it's the best I found so far.
			//
			// lea rax, loc_1400019B7
			// mov rax, rax
			//
			std::uintptr_t loaded_value;

			asm(
				"mov %1, %0"
				: "=r" (loaded_value)
				: "r" (value)
			);

			return loaded_value;
#elif defined(_MSC_VER)
			return value;
#else
			#error "Unsupported compiler"
#endif
		}
	}
}
