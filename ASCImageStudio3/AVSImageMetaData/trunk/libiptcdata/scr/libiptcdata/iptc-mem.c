#include <libiptcdata/iptc-mem.h>
#include <stdlib.h>


struct _IptcMem {
	unsigned int ref_count;
	IptcMemAllocFunc alloc_func;
	IptcMemReallocFunc realloc_func;
	IptcMemFreeFunc free_func;
};

static void *
iptc_mem_alloc_func (IptcLong ds)
{
	return calloc ((size_t) ds, 1);
}

static void *
iptc_mem_realloc_func (void *d, IptcLong ds)
{
	return realloc (d, (size_t) ds);
}

static void
iptc_mem_free_func (void *d)
{
	free (d);
}

IptcMem *
iptc_mem_new (IptcMemAllocFunc alloc_func, IptcMemReallocFunc realloc_func,
	      IptcMemFreeFunc free_func)
{
	IptcMem *mem;

	if (!alloc_func || !realloc_func) return NULL;
	mem = alloc_func ? alloc_func (sizeof (IptcMem)) :
		           realloc_func (NULL, sizeof (IptcMem));
	if (!mem) return NULL;
	mem->ref_count = 1;

	mem->alloc_func   = alloc_func;
	mem->realloc_func = realloc_func;
	mem->free_func    = free_func;

	return mem;
}

void
iptc_mem_ref (IptcMem *mem)
{
	if (!mem) return;
	mem->ref_count++;
}

void
iptc_mem_unref (IptcMem *mem)
{
	if (!mem) return;
	if (!--mem->ref_count)
		iptc_mem_free (mem, mem);
}

void
iptc_mem_free (IptcMem *mem, void *d)
{
	if (!mem) return;
	if (mem->free_func) {
		mem->free_func (d);
		return;
	}
}

void *
iptc_mem_alloc (IptcMem *mem, IptcLong ds)
{
	if (!mem) return NULL;
	if (mem->alloc_func || mem->realloc_func)
		return mem->alloc_func ? mem->alloc_func (ds) :
					 mem->realloc_func (NULL, ds);
	return NULL;
}

void *
iptc_mem_realloc (IptcMem *mem, void *d, IptcLong ds)
{
	return (mem && mem->realloc_func) ? mem->realloc_func (d, ds) : NULL;
}

IptcMem *
iptc_mem_new_default (void)
{
	return iptc_mem_new (iptc_mem_alloc_func, iptc_mem_realloc_func,
			     iptc_mem_free_func);
}
