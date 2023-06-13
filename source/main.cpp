
#include <cstdio>

#include "limbo.h"


struct TestStruct
{
	int a;
};

LIMBO_OBFUSCATED_FUNC(int, obf_test, TestStruct struct_)
{
	printf("[obf_test] Argument: %d\n", struct_.a);

	return struct_.a + 5;
}


int main(int argc, char* argv[])
{
	printf("[main] Starting...\n");

	TestStruct emu {};
	emu.a = 522;

	const auto ret = obf_test(emu);
	printf("[main] Received: %d\n", ret);

	return 0;
}
