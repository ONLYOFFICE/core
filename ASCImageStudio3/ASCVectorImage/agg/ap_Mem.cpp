//----------------------------------------------------------------------------
// Mem func.  - Version 0.1.1 - 18-11-2004
// Marlon Mircevski
//----------------------------------------------------------------------------
#include "stdafx.h"
#include "ap_Mem.h"

namespace Aggplus
{

	//Malloc func
	void *ap_malloc_Aligned(size_t size, BYTE alignment)
	{
		BYTE *mem_ptr;
	  
		if(!alignment)
		{
				// We have not to satisfy any alignment
				if((mem_ptr = (BYTE *) malloc(size + 1)) != NULL)
			{
					// Store (mem_ptr - "real allocated memory") in *(mem_ptr-1)
					*mem_ptr = 0;
					// Return the mem_ptr pointer
					return (void *) mem_ptr++;
			}
		}
		else
		{
				BYTE *tmp;
				// Allocate the required size memory + alignment so we
				// can realign the data if necessary

				if((tmp = (BYTE *) malloc(size + alignment)) != NULL)
			{
					// Align the tmp pointer
					mem_ptr = (BYTE *)((DWORD)(tmp + alignment - 1)&(~(DWORD)(alignment - 1)));

					// Special case where malloc have already satisfied the alignment
					// We must add alignment to mem_ptr because we must store
					// (mem_ptr - tmp) in *(mem_ptr-1)
					// If we do not add alignment to mem_ptr then *(mem_ptr-1) points
					// to a forbidden memory space

			if(mem_ptr == tmp) mem_ptr += alignment;

					// (mem_ptr - tmp) is stored in *(mem_ptr-1) so we are able to retrieve
					// the real malloc block allocated and free it in x_free

			*(mem_ptr - 1) = (BYTE)(mem_ptr - tmp);

					// Return the aligned pointer
					return (void *) mem_ptr;
			}
		}
		return NULL;
	}

	// memory alloc
	void *ap_malloc16(int size)
	{
		void *ptr = ap_malloc_Aligned(size, 16);
		if(!ptr) 
			return NULL;
		memset(ptr, 0, size);
		return ptr;
	}

	// NOTE: ptr = NULL is explicetly allowed
	void ap_free(void *ptr)
	{
		if(ptr) 
			free((BYTE*)ptr - *((BYTE*)ptr - 1));
	}

}
