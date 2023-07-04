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

#define _LIMBO_CAT(a, b) a ## b
#define LIMBO_CAT(a, b) _LIMBO_CAT(a, b)

#define LIMBO_RAND limbo::rng::random_int<__COUNTER__>()
#define LIMBO_RAND_FASTSEED(seed) seed * 0x101 /* yolo */

#ifdef DEBUG
	#define LIMBO_FORCEINLINE
	#define LIMBO_NOINLINE
#else
	#if defined(__clang__) || defined(__GNUC__)
		#define LIMBO_FORCEINLINE __attribute__((always_inline)) inline
		#define LIMBO_NOINLINE __attribute__((noinline))
	#else
		#error "Unsupported compiler"
	#endif
#endif
