#pragma once

namespace MemoryUtils
{
	class CMemoryUtils
	{
	public:
		CMemoryUtils()
		{
			m_bMMXSupported = CheckForMMX();
			m_bSSESupported = CheckForSSE();
		}
		~CMemoryUtils()
		{
		}

		BOOL IsMMXSupported() const
		{
			return m_bMMXSupported;
		}
		BOOL IsSSESupported() const
		{
			return m_bSSESupported;
		}
		void memset(void *pDest, unsigned char value, int nBytes) const
		{
			if (!m_bMMXSupported)
			{
				::memset(pDest, value, nBytes);
				return;
			}
			int nOffset  = (int)((reinterpret_cast<ULONG64>(pDest)) % 8);

			if (nBytes < 32)
			{
				::memset(pDest, value, nBytes);
			}
			else
			{
				if (nOffset != 0)
				{
					int nPreBytes = 8-nOffset;
					memset(pDest, value, nPreBytes);
					pDest = (void*)((BYTE*)pDest + nPreBytes);
					nBytes -= nPreBytes;
				}

				int nLoops = nBytes/64;
				unsigned __int64 value8x = value;
				value8x = value8x | (value8x<<8);
				value8x = value8x | (value8x<<16);
				value8x = value8x | (value8x<<32);

				if (nLoops > 0)
				__asm
				{
					mov ecx, nLoops
					mov edi, pDest

					movq mm0, qword ptr value8x
					movq mm1, qword ptr value8x
					movq mm2, qword ptr value8x
					movq mm3, qword ptr value8x
					movq mm4, qword ptr value8x
					movq mm5, qword ptr value8x
					movq mm6, qword ptr value8x
					movq mm7, qword ptr value8x

					ALIGN 16
					MMX_memset_loop:

						MOVQ       [edi], mm0       
						MOVQ       [edi+8], mm1
						MOVQ       [edi+16], mm2
						MOVQ       [edi+24], mm3
						MOVQ       [edi+32], mm4
						MOVQ       [edi+40], mm5
						MOVQ       [edi+48], mm6
						MOVQ       [edi+56], mm7
		                
						ADD        edi, 64

						dec    ecx
						jnz    MMX_memset_loop

					EMMS
				}

				int nBytesDone = nLoops*64;
				int nBytesLeft = nBytes - nBytesDone;
				if (nBytesLeft > 0)
					::memset((unsigned char *)pDest + nBytesDone, value, nBytesLeft);
			}
		}

		// эту функцию нельзя использовать при перекрытии блоков памяти!
		// если надо копировать блоки памяти с перекрытием надо воспользоваться
		// стандартными функциями memcpy() или memmove()
		void memcpy(void *pDest, void *pSrc, int nBytes) const
		{
			if (!m_bMMXSupported)
			{
				::memcpy(pDest, pSrc, nBytes);
				return;
			}

			int nOffsetSrc  = (int)((reinterpret_cast<ULONG64>(pSrc)) % 8);
			int nOffsetDst = (int)((reinterpret_cast<ULONG64>(pDest)) % 8);

			if (nOffsetSrc != nOffsetDst || nBytes < 32)
			{
				// don't use MMX, since data is not aligned
				::memcpy(pDest, pSrc, nBytes);
			}
			else
			{
				if (nOffsetSrc != 0)
				{
					int nPreBytes = 8 - nOffsetSrc;
					::memcpy(pDest, pSrc, nPreBytes);
					pSrc = (void*)((BYTE*)pSrc + nPreBytes);
					pDest = (void*)((BYTE*)pDest + nPreBytes);
					nBytes -= nPreBytes;
					//assert((((unsigned long)pSrc) % 8)==0);
					//assert((((unsigned long)pDest) % 8)==0);
				}

				int nLoops = nBytes/64;

				if (nLoops > 0)
				__asm
				{
					mov ecx, nLoops
					mov eax, pSrc
					mov edi, pDest

					ALIGN 16
					MMX_memcpy_loop:

						MOVQ       mm0, [eax]   
						MOVQ       mm1, [eax+8] 
						MOVQ       mm2, [eax+16]
						MOVQ       mm3, [eax+24]
						MOVQ       mm4, [eax+32]
						MOVQ       mm5, [eax+40]
						MOVQ       mm6, [eax+48]
						MOVQ       mm7, [eax+56]

						MOVQ       [edi], mm0       
						MOVQ       [edi+8], mm1
						MOVQ       [edi+16], mm2
						MOVQ       [edi+24], mm3
						MOVQ       [edi+32], mm4
						MOVQ       [edi+40], mm5
						MOVQ       [edi+48], mm6
						MOVQ       [edi+56], mm7
		                
						ADD        eax, 64
						ADD        edi, 64

						dec    ecx
						jnz    MMX_memcpy_loop

					EMMS
				}

				int nBytesDone = nLoops*64;
				int nBytesLeft = nBytes - nBytesDone;
				if (nBytesLeft > 0)
				{
					::memcpy((unsigned char *)pDest + nBytesDone, (unsigned char *)pSrc + nBytesDone, nBytesLeft);
				}
			}
		}


	protected:
		BOOL m_bMMXSupported;
		BOOL m_bSSESupported;

		BOOL CheckForMMX()
		{
			DWORD dwMMX = 0;
			DWORD *pdwMMX = &dwMMX;
			__try {
				__asm {
					mov eax, 1
					cpuid
					mov edi, pdwMMX
					mov dword ptr [edi], edx
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				dwMMX = 0;
			}

			if (dwMMX & 0x00800000)  // check bit 23
				return TRUE;

			return FALSE;
		}
		BOOL CheckForSSE()
		{
			/*
			The SSE instruction set was introduced with the Pentium III and features:
				* Additional MMX instructions such as min/max
				* Prefetch and write-through instructions for optimizing data movement 
					from and to the L2/L3 caches and main memory
				* 8 New 128 bit XMM registers (xmm0..xmm7) and corresponding 32 bit floating point 
					(single precision) instructions
			*/

			DWORD dwSSE = 0;
			DWORD *pdwSSE = &dwSSE;
			__try {
				__asm
				{
					mov eax, 1
					cpuid
					mov edi, pdwSSE
					mov dword ptr [edi], edx
				}
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				dwSSE = 0;
			}

			if (dwSSE & 0x02000000)  // check bit 25
				return TRUE;

			return FALSE;
		}

	};
};