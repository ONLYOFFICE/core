/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#ifndef XMLSEC_NO_X509

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libxml/tree.h>
#include <libxml/parser.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/buffer.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/x509.h>
#include <xmlsec/errors.h>

/**
 * xmlSecX509DataGetNodeContent:
 * @node:               the pointer to <dsig:X509Data/> node.
 * @keyInfoCtx:         the pointer to <dsig:KeyInfo/> node processing context.
 *
 * Reads the contents of <dsig:X509Data/> node and returns it as
 * a bits mask.
 *
 * Returns: the bit mask representing the <dsig:X509Data/> node content
 * or a negative value if an error occurs.
 */
int
xmlSecX509DataGetNodeContent (xmlNodePtr node, xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlNodePtr cur;
    int content = 0;

    xmlSecAssert2(node != NULL, 0);
    xmlSecAssert2(keyInfoCtx != NULL, -1);

    /* determine the current node content */
    cur = xmlSecGetNextElementNode(node->children);
    while(cur != NULL) {
        if(xmlSecCheckNodeName(cur, xmlSecNodeX509Certificate, xmlSecDSigNs)) {
            if(xmlSecIsEmptyNode(cur) == 1) {
                content |= XMLSEC_X509DATA_CERTIFICATE_NODE;
            }
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SubjectName, xmlSecDSigNs)) {
            if(xmlSecIsEmptyNode(cur) == 1) {
                content |= XMLSEC_X509DATA_SUBJECTNAME_NODE;
            }
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509IssuerSerial, xmlSecDSigNs)) {
            if(xmlSecIsEmptyNode(cur) == 1) {
                content |= XMLSEC_X509DATA_ISSUERSERIAL_NODE;
            }
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509SKI, xmlSecDSigNs)) {
            if(xmlSecIsEmptyNode(cur) == 1) {
                content |= XMLSEC_X509DATA_SKI_NODE;
            }
        } else if(xmlSecCheckNodeName(cur, xmlSecNodeX509CRL, xmlSecDSigNs)) {
            if(xmlSecIsEmptyNode(cur) == 1) {
                content |= XMLSEC_X509DATA_CRL_NODE;
            }
        } else {
            /* todo: fail on unknown child node? */
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }

    return (content);
}

#endif /* XMLSEC_NO_X509 */

