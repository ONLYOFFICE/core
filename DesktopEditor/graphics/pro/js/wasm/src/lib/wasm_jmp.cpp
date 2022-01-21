#include "./wasm_jmp.h"

void ft_longjmp_ex(void* env, int val)
{
	throw val;
}
