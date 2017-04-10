/**
 *
 * XMLSec library
 *
 * DES Algorithm support
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2010-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef XMLSEC_NO_DES
#include "globals.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <gcrypt.h>


#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/transforms.h>
#include <xmlsec/errors.h>

#include <xmlsec/gnutls/crypto.h>

/**************************************************************************
 *
 * We use xmlsec-gcrypt for all the basic crypto ops
 *
 *****************************************************************************/
#include <xmlsec/gcrypt/crypto.h>


/**
 * xmlSecGnuTLSTransformKWDes3GetKlass:
 *
 * The Triple DES key wrapper transform klass.
 *
 * Returns: Triple DES key wrapper transform klass.
 */
xmlSecTransformId
xmlSecGnuTLSTransformKWDes3GetKlass(void) {
    return(xmlSecGCryptTransformKWDes3GetKlass());
}

#endif /* XMLSEC_NO_DES */

