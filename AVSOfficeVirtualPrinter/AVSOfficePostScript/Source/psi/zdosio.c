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

/* $Id: zdosio.c 9043 2008-08-28 22:48:19Z giles $ */
/* MS-DOS direct I/O operators. */
/* These should NEVER be included in a released configuration! */
#include "dos_.h"
#include "ghost.h"
#include "oper.h"
#include "store.h"

/* <port> .inport <word> */
static int
zinport(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, inport((int)op->value.intval));
    return 0;
}

/* <port> .inportb <byte> */
static int
zinportb(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, inportb((int)op->value.intval));
    return 0;
}

/* <port> <word> .outport - */
static int
zoutport(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_type(op[-1], t_integer);
    outport((int)op[-1].value.intval, (int)op->value.intval);
    pop(1);
    return 0;
}

/* <port> <byte> .outportb - */
static int
zoutportb(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_int_leu(op[-1], 0xff);
    outportb((int)op[-1].value.intval, (byte) op->value.intval);
    pop(1);
    return 0;
}

/* <loc> .peek <byte> */
static int
zpeek(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    make_int(op, *(byte *) (op->value.intval));
    return 0;
}

/* <loc> <byte> .poke - */
static int
zpoke(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_integer);
    check_int_leu(op[-1], 0xff);
    *(byte *) (op[-1].value.intval) = (byte) op->value.intval;
    pop(1);
    return 0;
}

/* ------ Operator initialization ------ */

const op_def zdosio_op_defs[] =
{
    {"1.inport", zinport},
    {"1.inportb", zinportb},
    {"2.outport", zoutport},
    {"2.outportb", zoutportb},
    {"1.peek", zpeek},
    {"2.poke", zpoke},
    op_def_end(0)
};
