#pragma once

#include "..\MediaDataStruct.h"

/* cpu_flags definitions (make sure to sync this with cpuid.asm for ia32) */

#define CPU_FORCE    (1<<31) /* force passed cpu flags */
#define CPU_ASM      (1<< 7) /* native assembly */
#define CPU_MMX      (1<< 0) /*       mmx : pentiumMMX,k6 */
#define CPU_MMXEXT   (1<< 1) /*   mmx-ext : pentium2, athlon */
#define CPU_SSE      (1<< 2) /*       sse : pentium3, athlonXP */
#define CPU_SSE2     (1<< 3) /*      sse2 : pentium4, athlon64 */
#define CPU_3DNOW    (1<< 4) /*     3dnow : k6-2 */
#define CPU_3DNOWEXT (1<< 5) /* 3dnow-ext : athlon */
#define CPU_TSC      (1<< 6) /*       tsc : Pentium */
