/*
 * XML Security Library
 *
 * globals.h: internal header only used during the compilation
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_GLOBALS_H__
#define __XMLSEC_GLOBALS_H__

/**
 * Use autoconf defines if present.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#define IN_XMLSEC_CRYPTO
#define XMLSEC_PRIVATE


#define XMLSEC_GCRYPT_MAX_DIGEST_SIZE           256
#define XMLSEC_GCRYPT_REPORT_ERROR(err)         \
        "error code=%d; error message='%s'",    \
        (int)err, xmlSecErrorsSafeString(gcry_strerror((err)))

#endif /* ! __XMLSEC_GLOBALS_H__ */
