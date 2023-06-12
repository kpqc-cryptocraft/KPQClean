#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

extern "C"
{
	EXPORT void input(int* a) {
		printf("Security Level이 몇 인지 입력하세요(128, 192, 256)");
		scanf("%d", a);
		while (*a != 128 && *a != 192 && *a != 256)
		{
			printf("128, 192, 256 숫자만 입력해주세요.");
			scanf("%d", a);
		}
		//printf("DLL input : a : %d%d\n", *a);
	}

	EXPORT void printseed(int a) {
		printf("Seed : %d\n", a);
	}

	


	EXPORT void printmsg(wchar_t *a) {
		printf("msg : %s\n", a);
	}
}