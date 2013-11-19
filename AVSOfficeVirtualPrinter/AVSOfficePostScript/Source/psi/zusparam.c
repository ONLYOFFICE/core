/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zusparam.c 9043 2008-08-28 22:48:19Z giles $ */
/* User and system parameter operators */
#include "memory_.h"
#include "string_.h"
#include "ghost.h"
#include "oper.h"
#include "gscdefs.h"
#include "gsstruct.h"		/* for gxht.h */
#include "gsfont.h"		/* for user params */
#include "gxht.h"		/* for user params */
#include "gsutil.h"
#include "estack.h"
#include "ialloc.h"		/* for imemory for status */
#include "icontext.h"		/* for set_user_params prototype */
#include "idict.h"
#include "idparam.h"
#include "iparam.h"
#include "dstack.h"
#include "iname.h"
#include "itoken.h"
#include "iutil2.h"
#include "ivmem2.h"
#include "store.h"
#include "gsnamecl.h"

/* The (global) font directory */
extern gs_font_dir *ifont_dir;	/* in zfont.c */

/* Define an individual user or system parameter. */
/* Eventually this will be made public. */
#define param_def_common\
    const char *pname

typedef struct param_def_s {
    param_def_common;
} param_def_t;

typedef struct long_param_def_s {
    param_def_common;
    long min_value, max_value;
    long (*current)(i_ctx_t *);
    int (*set)(i_ctx_t *, long);
} long_param_def_t;

#if arch_sizeof_long > arch_sizeof_int
#  define MAX_UINT_PARAM max_uint
#  define MIN_INT_PARAM min_int
#else
#  define MAX_UINT_PARAM max_long
#  define MIN_INT_PARAM min_long
#endif

typedef struct bool_param_def_s {
    param_def_common;
    bool (*current)(i_ctx_t *);
    int (*set)(i_ctx_t *, bool);
} bool_param_def_t;

typedef struct string_param_def_s {
    param_def_common;
    void (*current)(i_ctx_t *, gs_param_string *);
    int (*set)(i_ctx_t *, gs_param_string *);
} string_param_def_t;

/* Define a parameter set (user or system). */
typedef struct param_set_s {
    const long_param_def_t *long_defs;
    uint long_count;
    const bool_param_def_t *bool_defs;
    uint bool_count;
    const string_param_def_t *string_defs;
    uint string_count;
} param_set;

/* Forward references */
static int setparams(i_ctx_t *, gs_param_list *, const param_set *);
static int currentparams(i_ctx_t *, const param_set *);
static int currentparam1(i_ctx_t *, const param_set *);

/* ------ Passwords ------ */

/* <string|int> .checkpassword <0|1|2> */
static int
zcheckpassword(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    ref params[2];
    array_param_list list;
    gs_param_list *const plist = (gs_param_list *)&list;
    int result = 0;
    int code = name_ref(imemory, (const byte *)"Password", 8, &params[0], 0);
    password pass;

    if (code < 0)
	return code;
    params[1] = *op;
    array_param_list_read(&list, params, 2, NULL, false, iimemory);
    if (dict_read_password(&pass, systemdict, "StartJobPassword") >= 0 &&
	param_check_password(plist, &pass) == 0
	)
	result = 1;
    if (dict_read_password(&pass, systemdict, "SystemParamsPassword") >= 0 &&
	param_check_password(plist, &pass) == 0
	)
	result = 2;
    iparam_list_release(&list);
    make_int(op, result);
    return 0;
}

/* ------ System parameters ------ */

/* Integer values */
static long
current_BuildTime(i_ctx_t *i_ctx_p)
{
    return gs_buildtime;
}
static long
current_MaxFontCache(i_ctx_t *i_ctx_p)
{
    return gs_currentcachesize(ifont_dir);
}
static int
set_MaxFontCache(i_ctx_t *i_ctx_p, long val)
{
    return gs_setcachesize(ifont_dir,
			   (uint)(val < 0 ? 0 : val > max_uint ? max_uint :
				   val));
}
static long
current_CurFontCache(i_ctx_t *i_ctx_p)
{
    uint cstat[7];

    gs_cachestatus(ifont_dir, cstat);
    return cstat[0];
}
static long
current_MaxGlobalVM(i_ctx_t *i_ctx_p)
{
    gs_memory_gc_status_t stat;

    gs_memory_gc_status(iimemory_global, &stat);
    return stat.max_vm;
}
static int
set_MaxGlobalVM(i_ctx_t *i_ctx_p, long val)
{
    gs_memory_gc_status_t stat;

    gs_memory_gc_status(iimemory_global, &stat);
    stat.max_vm = max(val, 0);
    gs_memory_set_gc_status(iimemory_global, &stat);
    return 0;
}
static long
current_Revision(i_ctx_t *i_ctx_p)
{
    return gs_revision;
}
static const long_param_def_t system_long_params[] =
{
    {"BuildTime", min_long, max_long, current_BuildTime, NULL},
{"MaxFontCache", 0, MAX_UINT_PARAM, current_MaxFontCache, set_MaxFontCache},
    {"CurFontCache", 0, MAX_UINT_PARAM, current_CurFontCache, NULL},
    {"Revision", min_long, max_long, current_Revision, NULL},
    /* Extensions */
    {"MaxGlobalVM", 0, max_long, current_MaxGlobalVM, set_MaxGlobalVM}
};

/* Boolean values */
static bool
current_ByteOrder(i_ctx_t *i_ctx_p)
{
    return !arch_is_big_endian;
}
static const bool_param_def_t system_bool_params[] =
{
    {"ByteOrder", current_ByteOrder, NULL}
};

/* String values */
static void
current_RealFormat(i_ctx_t *i_ctx_p, gs_param_string * pval)
{
#if ARCH_FLOATS_ARE_IEEE
    static const char *const rfs = "IEEE";
#else
    static const char *const rfs = "not IEEE";
#endif

    pval->data = (const byte *)rfs;
    pval->size = strlen(rfs);
    pval->persistent = true;
}
static const string_param_def_t system_string_params[] =
{
    {"RealFormat", current_RealFormat, NULL}
};

/* The system parameter set */
static const param_set system_param_set =
{
    system_long_params, countof(system_long_params),
    system_bool_params, countof(system_bool_params),
    system_string_params, countof(system_string_params)
};

/* <dict> .setsystemparams - */
static int
zsetsystemparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;
    dict_param_list list;
    gs_param_list *const plist = (gs_param_list *)&list;
    password pass;

    check_type(*op, t_dictionary);
    code = dict_param_list_read(&list, op, NULL, false, iimemory);
    if (code < 0)
	return code;
    code = dict_read_password(&pass, systemdict, "SystemParamsPassword");
    if (code < 0)
	return code;
    code = param_check_password(plist, &pass);
    if (code != 0) {
	if (code > 0)
	    code = gs_note_error(e_invalidaccess);
	goto out;
    }
    code = param_read_password(plist, "StartJobPassword", &pass);
    switch (code) {
	default:		/* invalid */
	    goto out;
	case 1:		/* missing */
	    break;
	case 0:
	    code = dict_write_password(&pass, systemdict,
				       "StartJobPassword",
				       ! i_ctx_p->LockFilePermissions);
	    if (code < 0)
		goto out;
    }
    code = param_read_password(plist, "SystemParamsPassword", &pass);
    switch (code) {
	default:		/* invalid */
	    goto out;
	case 1:		/* missing */
	    break;
	case 0:
	    code = dict_write_password(&pass, systemdict,
				       "SystemParamsPassword",
				       ! i_ctx_p->LockFilePermissions);
	    if (code < 0)
		goto out;
    }
#if ENABLE_CUSTOM_COLOR_CALLBACK
    /* The custom color callback pointer */
    code = custom_color_callback_put_params(i_ctx_p->pgs, plist);
    if (code < 0)
	goto out;
#endif

    code = setparams(i_ctx_p, plist, &system_param_set);
  out:
    iparam_list_release(&list);
    if (code < 0)
	return code;
    pop(1);
    return 0;
}

/* - .currentsystemparams <name1> <value1> ... */
static int
zcurrentsystemparams(i_ctx_t *i_ctx_p)
{
    return currentparams(i_ctx_p, &system_param_set);
}

/* <name> .getsystemparam <value> */
static int
zgetsystemparam(i_ctx_t *i_ctx_p)
{
    return currentparam1(i_ctx_p, &system_param_set);
}

/* ------ User parameters ------ */

/* Integer values */
static long
current_JobTimeout(i_ctx_t *i_ctx_p)
{
    return 0;
}
static int
set_JobTimeout(i_ctx_t *i_ctx_p, long val)
{
    return 0;
}
static long
current_MaxFontItem(i_ctx_t *i_ctx_p)
{
    return gs_currentcacheupper(ifont_dir);
}
static int
set_MaxFontItem(i_ctx_t *i_ctx_p, long val)
{
    return gs_setcacheupper(ifont_dir, val);
}
static long
current_MinFontCompress(i_ctx_t *i_ctx_p)
{
    return gs_currentcachelower(ifont_dir);
}
static int
set_MinFontCompress(i_ctx_t *i_ctx_p, long val)
{
    return gs_setcachelower(ifont_dir, val);
}
static long
current_MaxOpStack(i_ctx_t *i_ctx_p)
{
    return ref_stack_max_count(&o_stack);
}
static int
set_MaxOpStack(i_ctx_t *i_ctx_p, long val)
{
    return ref_stack_set_max_count(&o_stack, val);
}
static long
current_MaxDictStack(i_ctx_t *i_ctx_p)
{
    return ref_stack_max_count(&d_stack);
}
static int
set_MaxDictStack(i_ctx_t *i_ctx_p, long val)
{
    return ref_stack_set_max_count(&d_stack, val);
}
static long
current_MaxExecStack(i_ctx_t *i_ctx_p)
{
    return ref_stack_max_count(&e_stack);
}
static int
set_MaxExecStack(i_ctx_t *i_ctx_p, long val)
{
    return ref_stack_set_max_count(&e_stack, val);
}
static long
current_MaxLocalVM(i_ctx_t *i_ctx_p)
{
    gs_memory_gc_status_t stat;

    gs_memory_gc_status(iimemory_local, &stat);
    return stat.max_vm;
}
static int
set_MaxLocalVM(i_ctx_t *i_ctx_p, long val)
{
    gs_memory_gc_status_t stat;

    gs_memory_gc_status(iimemory_local, &stat);
    stat.max_vm = max(val, 0);
    gs_memory_set_gc_status(iimemory_local, &stat);
    return 0;
}
static long
current_VMReclaim(i_ctx_t *i_ctx_p)
{
    gs_memory_gc_status_t gstat, lstat;

    gs_memory_gc_status(iimemory_global, &gstat);
    gs_memory_gc_status(iimemory_local, &lstat);
    return (!gstat.enabled ? -2 : !lstat.enabled ? -1 : 0);
}
static long
current_VMThreshold(i_ctx_t *i_ctx_p)
{
    gs_memory_gc_status_t stat;

    gs_memory_gc_status(iimemory_local, &stat);
    return stat.vm_threshold;
}
static long
current_WaitTimeout(i_ctx_t *i_ctx_p)
{
    return 0;
}
static int
set_WaitTimeout(i_ctx_t *i_ctx_p, long val)
{
    return 0;
}
static long
current_MinScreenLevels(i_ctx_t *i_ctx_p)
{
    return gs_currentminscreenlevels();
}
static int
set_MinScreenLevels(i_ctx_t *i_ctx_p, long val)
{
    gs_setminscreenlevels((uint) val);
    return 0;
}
static long
current_AlignToPixels(i_ctx_t *i_ctx_p)
{
    return gs_currentaligntopixels(ifont_dir);
}
static int
set_AlignToPixels(i_ctx_t *i_ctx_p, long val)
{
    gs_setaligntopixels(ifont_dir, (uint)val);
    return 0;
}
static long
current_GridFitTT(i_ctx_t *i_ctx_p)
{
    return gs_currentgridfittt(ifont_dir);
}
static int
set_GridFitTT(i_ctx_t *i_ctx_p, long val)
{
    gs_setgridfittt(ifont_dir, (uint)val);
    return 0;
}
static const long_param_def_t user_long_params[] =
{
    {"JobTimeout", 0, MAX_UINT_PARAM,
     current_JobTimeout, set_JobTimeout},
    {"MaxFontItem", MIN_INT_PARAM, MAX_UINT_PARAM,
     current_MaxFontItem, set_MaxFontItem},
    {"MinFontCompress", MIN_INT_PARAM, MAX_UINT_PARAM,
     current_MinFontCompress, set_MinFontCompress},
    {"MaxOpStack", 0, MAX_UINT_PARAM,
     current_MaxOpStack, set_MaxOpStack},
    {"MaxDictStack", 0, MAX_UINT_PARAM,
     current_MaxDictStack, set_MaxDictStack},
    {"MaxExecStack", 0, MAX_UINT_PARAM,
     current_MaxExecStack, set_MaxExecStack},
    {"MaxLocalVM", 0, max_long,
     current_MaxLocalVM, set_MaxLocalVM},
    {"VMReclaim", -2, 0,
     current_VMReclaim, set_vm_reclaim},
    {"VMThreshold", -1, max_long,
     current_VMThreshold, set_vm_threshold},
    {"WaitTimeout", 0, MAX_UINT_PARAM,
     current_WaitTimeout, set_WaitTimeout},
    /* Extensions */
    {"MinScreenLevels", 0, MAX_UINT_PARAM,
     current_MinScreenLevels, set_MinScreenLevels},
    {"AlignToPixels", 0, 1,
     current_AlignToPixels, set_AlignToPixels},
    {"GridFitTT", 0, 3, 
     current_GridFitTT, set_GridFitTT}
};

/* Boolean values */
static bool
current_AccurateScreens(i_ctx_t *i_ctx_p)
{
    return gs_currentaccuratescreens();
}
static int
set_AccurateScreens(i_ctx_t *i_ctx_p, bool val)
{
    gs_setaccuratescreens(val);
    return 0;
}
/* Boolean values */
static bool
current_UseWTS(i_ctx_t *i_ctx_p)
{
    return gs_currentusewts();
}
static int
set_UseWTS(i_ctx_t *i_ctx_p, bool val)
{
    gs_setusewts(val);
    return 0;
}
static bool
current_LockFilePermissions(i_ctx_t *i_ctx_p)
{
    return i_ctx_p->LockFilePermissions;
}
static int
set_LockFilePermissions(i_ctx_t *i_ctx_p, bool val)
{
    /* allow locking even if already locked */
    if (i_ctx_p->LockFilePermissions && !val)
	return_error(e_invalidaccess);
    i_ctx_p->LockFilePermissions = val;
    return 0;
}
static bool
current_RenderTTNotdef(i_ctx_t *i_ctx_p)
{
    return i_ctx_p->RenderTTNotdef;
}
static int
set_RenderTTNotdef(i_ctx_t *i_ctx_p, bool val)
{
    i_ctx_p->RenderTTNotdef = val;
    return 0;
}
static const bool_param_def_t user_bool_params[] =
{
    {"AccurateScreens", current_AccurateScreens, set_AccurateScreens},
    {"UseWTS", current_UseWTS, set_UseWTS},
    {"LockFilePermissions", current_LockFilePermissions, set_LockFilePermissions},
    {"RenderTTNotdef", current_RenderTTNotdef, set_RenderTTNotdef}
};

/* The user parameter set */
static const param_set user_param_set =
{
    user_long_params, countof(user_long_params),
    user_bool_params, countof(user_bool_params),
    0, 0 
};

/* <dict> .setuserparams - */
/* We break this out for use when switching contexts. */
int
set_user_params(i_ctx_t *i_ctx_p, const ref *paramdict)
{
    dict_param_list list;
    int code;

    check_type(*paramdict, t_dictionary);
    code = dict_param_list_read(&list, paramdict, NULL, false, iimemory);
    if (code < 0)
	return code;
    code = setparams(i_ctx_p, (gs_param_list *)&list, &user_param_set);
    iparam_list_release(&list);
    return code;
}
static int
zsetuserparams(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code = set_user_params(i_ctx_p, op);

    if (code >= 0) {
	/* Update cached scanner options. */
	i_ctx_p->scanner_options =
	    ztoken_scanner_options(op, i_ctx_p->scanner_options);
	pop(1);
    }
    return code;
}

/* - .currentuserparams <name1> <value1> ... */
static int
zcurrentuserparams(i_ctx_t *i_ctx_p)
{
    return currentparams(i_ctx_p, &user_param_set);
}

/* <name> .getuserparam <value> */
static int
zgetuserparam(i_ctx_t *i_ctx_p)
{
    return currentparam1(i_ctx_p, &user_param_set);
}

/* ------ Initialization procedure ------ */

const op_def zusparam_op_defs[] =
{
	/* User and system parameters are accessible even in Level 1 */
	/* (if this is a Level 2 system). */
    {"0.currentsystemparams", zcurrentsystemparams},
    {"0.currentuserparams", zcurrentuserparams},
    {"1.getsystemparam", zgetsystemparam},
    {"1.getuserparam", zgetuserparam},
    {"1.setsystemparams", zsetsystemparams},
    {"1.setuserparams", zsetuserparams},
	/* The rest of the operators are defined only in Level 2. */
    op_def_begin_level2(),
    {"1.checkpassword", zcheckpassword},
    op_def_end(0)
};

/* ------ Internal procedures ------ */

/* Set the values of a parameter set from a parameter list. */
/* We don't attempt to back out if anything fails. */
static int
setparams(i_ctx_t *i_ctx_p, gs_param_list * plist, const param_set * pset)
{
    int i, code;

    for (i = 0; i < pset->long_count; i++) {
	const long_param_def_t *pdef = &pset->long_defs[i];
	long val;

	if (pdef->set == NULL)
	    continue;
	code = param_read_long(plist, pdef->pname, &val);
	switch (code) {
	    default:		/* invalid */
		return code;
	    case 1:		/* missing */
		break;
	    case 0:
		if (val < pdef->min_value || val > pdef->max_value)
		    return_error(e_rangecheck);
		code = (*pdef->set)(i_ctx_p, val);
		if (code < 0)
		    return code;
	}
    }
    for (i = 0; i < pset->bool_count; i++) {
	const bool_param_def_t *pdef = &pset->bool_defs[i];
	bool val;

	if (pdef->set == NULL)
	    continue;
	code = param_read_bool(plist, pdef->pname, &val);
	if (code == 0)
	    code = (*pdef->set)(i_ctx_p, val);
	if (code < 0)
	    return code;
    }
/****** WE SHOULD DO STRINGS AND STRING ARRAYS, BUT WE DON'T YET ******/
    return 0;
}

/* Get the current values of a parameter set to the stack. */
static bool
pname_matches(const char *pname, const ref * psref)
{
    return
	(psref == 0 ||
	 !bytes_compare((const byte *)pname, strlen(pname),
			psref->value.const_bytes, r_size(psref)));
}
static int
current_param_list(i_ctx_t *i_ctx_p, const param_set * pset,
		   const ref * psref /*t_string */ )
{
    stack_param_list list;
    gs_param_list *const plist = (gs_param_list *)&list;
    int i, code = 0;

    stack_param_list_write(&list, &o_stack, NULL, iimemory);
    for (i = 0; i < pset->long_count; i++) {
	const char *pname = pset->long_defs[i].pname;

	if (pname_matches(pname, psref)) {
	    long val = (*pset->long_defs[i].current)(i_ctx_p);

	    code = param_write_long(plist, pname, &val);
	    if (code < 0)
		return code;
	}
    }
    for (i = 0; i < pset->bool_count; i++) {
	const char *pname = pset->bool_defs[i].pname;

	if (pname_matches(pname, psref)) {
	    bool val = (*pset->bool_defs[i].current)(i_ctx_p);

	    code = param_write_bool(plist, pname, &val);
	    if (code < 0)
		return code;
	}
    }
    for (i = 0; i < pset->string_count; i++) {
	const char *pname = pset->string_defs[i].pname;

	if (pname_matches(pname, psref)) {
	    gs_param_string val;

	    (*pset->string_defs[i].current)(i_ctx_p, &val);
	    code = param_write_string(plist, pname, &val);
	    if (code < 0)
		return code;
	}
    }
    if (psref) {
	/*
	 * Scanner options can be read, but only individually by .getuserparam.
	 * This avoids putting them into userparams, and being affected by save/restore.
	 */
	const char *pname;
	bool val;
	int code;

	switch (ztoken_get_scanner_option(psref, i_ctx_p->scanner_options, &pname)) {
	    case 0:
		code = param_write_null(plist, pname);
		break;
	    case 1:
		val = true;
		code = param_write_bool(plist, pname, &val);
		break;
	    default:
		code = 0;
		break;
	}
	if (code < 0)
	    return code;
    }
#if ENABLE_CUSTOM_COLOR_CALLBACK
    if (pset == &system_param_set) {
        /* The custom_color callback pointer */
	if (pname_matches(CustomColorCallbackParamName, psref))
	    code = custom_color_callback_get_params(i_ctx_p->pgs, plist);
    }
#endif
    return code;
}

/* Get the current values of a parameter set to the stack. */
static int
currentparams(i_ctx_t *i_ctx_p, const param_set * pset)
{
    return current_param_list(i_ctx_p, pset, NULL);
}

/* Get the value of a single parameter to the stack, or signal an error. */
static int
currentparam1(i_ctx_t *i_ctx_p, const param_set * pset)
{
    os_ptr op = osp;
    ref sref;
    int code;

    check_type(*op, t_name);
    check_ostack(2);
    name_string_ref(imemory, (const ref *)op, &sref);
    code = current_param_list(i_ctx_p, pset, &sref);
    if (code < 0)
	return code;
    if (osp == op)
	return_error(e_undefined);
    /* We know osp == op + 2. */
    ref_assign(op, op + 2);
    pop(2);
    return code;
}
