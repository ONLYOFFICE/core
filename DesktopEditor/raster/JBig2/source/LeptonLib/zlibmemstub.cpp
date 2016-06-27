
/*
 *  zlibmemstub.c
 *
 *     Stubs for zlibmem.c functions
 */

#include <stdio.h>
#include "allheaders.h"

#ifdef HAVE_CONFIG_H
#include "config_auto.h"
#endif  /* HAVE_CONFIG_H */

/* --------------------------------------------*/
#if  !HAVE_LIBZ   /* defined in environ.h */
/* --------------------------------------------*/

l_uint8 * zlibCompress(l_uint8 *datain, l_int32 nin, l_int32 *pnout)
{
    return (l_uint8 *)ERROR_PTR("function not present", "zlibCompress", NULL);
}

l_uint8 * zlibUncompress(l_uint8 *datain, l_int32 nin, l_int32 *pnout)
{
    return (l_uint8 *)ERROR_PTR("function not present", "zlibUncompress", NULL);
}

/* --------------------------------------------*/
#endif  /* !HAVE_LIBZ */
/* --------------------------------------------*/

