// Pen.h: interface for the Pen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(Aggplus_Mem)
#define Aggplus_Mem

#include "ap_AggPlusTypes.h"

namespace Aggplus
{

	void *ap_malloc_Aligned(size_t size, BYTE alignment);
	void *ap_malloc16(int size);
	void  ap_free(void *ptr);
}
#endif // !defined(Aggplus_Mem)
