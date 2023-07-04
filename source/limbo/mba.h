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

#ifdef DEBUG
	#define LIMBO_OBFUSCATED_FUNC(ret, name, ...) \
		ret name(__VA_ARGS__)
#else
	// I don't really like this macro, but it's the only one I've found so far.
	// We rename the original function to something different and create a dispatcher function
	//	with the correct name.
	#define LIMBO_OBFUSCATED_FUNC(ret, name, ...) \
		LIMBO_NOINLINE ret LIMBO_CAT(name, __LINE__)(__VA_ARGS__);									\
		_LIMBO_OBFUSCATED_BODY(name)																\
		LIMBO_NOINLINE ret LIMBO_CAT(name, __LINE__)(__VA_ARGS__)

	#define _LIMBO_OBFUSCATED_BODY(name)															\
		template <class... Args>																	\
		LIMBO_NOINLINE decltype(auto) name(Args&&... args) {										\
			/* NOTE: This will fail and emit the original target address if the address isn't valid. */	\
			constexpr std::uintptr_t key = limbo::rng::random_int<__COUNTER__>(-0x1000, 0x1000);	\
			/* NOTE: Addition is the only operation that didn't break this from my testing */		\
			const std::uintptr_t shifted_addr = reinterpret_cast<std::uintptr_t>(&LIMBO_CAT(name, __LINE__)) + key; \
																									\
			const auto loaded_addr = limbo::helpers::load_from_reg(shifted_addr);					\
																									\
			/* opaque predicate to hinder analysis TODO: better predicates */						\
			const auto ret_addr = reinterpret_cast<std::uintptr_t>(_ReturnAddress());				\
																									\
			const std::uintptr_t opaque_zero = limbo::emulation::XorR<5, LIMBO_RAND>(ret_addr, ret_addr); \
			const std::uintptr_t opaque_key = limbo::emulation::AddR<5, LIMBO_RAND>(key, opaque_zero); \
																									\
			/* calculate the correct target address */												\
			const std::uintptr_t fixed_addr = limbo::emulation::SubR<5, LIMBO_RAND>(loaded_addr, opaque_key); \
																									\
			const auto func = (decltype(LIMBO_CAT(name, __LINE__))*)(fixed_addr);					\
			return func(std::forward<Args>(args)...);												\
	};
#endif
