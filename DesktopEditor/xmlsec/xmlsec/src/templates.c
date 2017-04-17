/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Creating signature and encryption templates.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include "globals.h"

#include <stdlib.h>
#include <string.h>

#include <libxml/tree.h>

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/transforms.h>
#include <xmlsec/strings.h>
#include <xmlsec/base64.h>
#include <xmlsec/templates.h>
#include <xmlsec/errors.h>


static xmlNodePtr       xmlSecTmplAddReference          (xmlNodePtr parentNode,
                                                         xmlSecTransformId digestMethodId,
                                                         const xmlChar *id,
                                                         const xmlChar *uri,
                                                         const xmlChar *type);
static int              xmlSecTmplPrepareEncData        (xmlNodePtr parentNode,
                                                         xmlSecTransformId encMethodId);
static int              xmlSecTmplNodeWriteNsList       (xmlNodePtr parentNode,
                                                         const xmlChar** namespaces);
/**************************************************************************
 *
 * <dsig:Signature/> node
 *
 **************************************************************************/
/**
 * xmlSecTmplSignatureCreate:
 * @doc:                the pointer to signature document or NULL; in the
 *                      second case, application must later call @xmlSetTreeDoc
 *                      to ensure that all the children nodes have correct
 *                      pointer to XML document.
 * @c14nMethodId:       the signature canonicalization method.
 * @signMethodId:       the signature  method.
 * @id:                 the node id (may be NULL).
 *
 * Creates new <dsig:Signature/> node with the mandatory <dsig:SignedInfo/>,
 * <dsig:CanonicalizationMethod/>, <dsig:SignatureMethod/> and
 * <dsig:SignatureValue/> children and sub-children.
 * The application is responsible for inserting the returned node
 * in the XML document.
 *
 * Returns: the pointer to newly created <dsig:Signature/> node or NULL if an
 * error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureCreate(xmlDocPtr doc, xmlSecTransformId c14nMethodId,
                      xmlSecTransformId signMethodId, const xmlChar *id) {
    return xmlSecTmplSignatureCreateNsPref(doc, c14nMethodId, signMethodId, id, NULL);
}

/**
 * xmlSecTmplSignatureCreateNsPref:
 * @doc:                the pointer to signature document or NULL; in the
 *                      second case, application must later call @xmlSetTreeDoc
 *                      to ensure that all the children nodes have correct
 *                      pointer to XML document.
 * @c14nMethodId:       the signature canonicalization method.
 * @signMethodId:       the signature  method.
 * @id:                 the node id (may be NULL).
 * @nsPrefix:   the namespace prefix for the signature element (e.g. "dsig"), or NULL
 *
 * Creates new <dsig:Signature/> node with the mandatory
 * <dsig:SignedInfo/>, <dsig:CanonicalizationMethod/>,
 * <dsig:SignatureMethod/> and <dsig:SignatureValue/> children and
 * sub-children. This method differs from xmlSecTmplSignatureCreate in
 * that it will define the http://www.w3.org/2000/09/xmldsig#
 * namespace with the given prefix that will be used for all of the
 * appropriate child nodes.  The application is responsible for
 * inserting the returned node in the XML document.
 *
 * Returns: the pointer to newly created <dsig:Signature/> node or NULL if an
 * error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureCreateNsPref(xmlDocPtr doc, xmlSecTransformId c14nMethodId,
                                xmlSecTransformId signMethodId, const xmlChar *id,
                                const xmlChar* nsPrefix) {
    xmlNodePtr signNode;
    xmlNodePtr signedInfoNode;
    xmlNodePtr cur;
    xmlNsPtr ns;

    xmlSecAssert2(c14nMethodId != NULL, NULL);
    xmlSecAssert2(c14nMethodId->href != NULL, NULL);
    xmlSecAssert2(signMethodId != NULL, NULL);
    xmlSecAssert2(signMethodId->href != NULL, NULL);

    /* create Signature node itself */
    signNode = xmlNewDocNode(doc, NULL, xmlSecNodeSignature, NULL);
    if(signNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlNewDocNode",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeSignature));
        return(NULL);
    }

    ns = xmlNewNs(signNode, xmlSecDSigNs, nsPrefix);
    if(ns == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlNewNs",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "ns=%s",
                    xmlSecErrorsSafeString(xmlSecDSigNs));
        xmlFreeNode(signNode);
        return(NULL);
    }
    xmlSetNs(signNode, ns);

    if(id != NULL) {
        xmlSetProp(signNode, BAD_CAST "Id", id);
    }

    /* add SignedInfo node */
    signedInfoNode = xmlSecAddChild(signNode, xmlSecNodeSignedInfo, xmlSecDSigNs);
    if(signedInfoNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeSignedInfo));
        xmlFreeNode(signNode);
        return(NULL);
    }

    /* add SignatureValue node */
    cur = xmlSecAddChild(signNode, xmlSecNodeSignatureValue, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeSignatureValue));
        xmlFreeNode(signNode);
        return(NULL);
    }

    /* add CanonicaizationMethod node to SignedInfo */
    cur = xmlSecAddChild(signedInfoNode, xmlSecNodeCanonicalizationMethod, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeCanonicalizationMethod));
        xmlFreeNode(signNode);
        return(NULL);
    }
    if(xmlSetProp(cur, xmlSecAttrAlgorithm, c14nMethodId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(c14nMethodId->href));
        xmlFreeNode(signNode);
        return(NULL);
    }

    /* add SignatureMethod node to SignedInfo */
    cur = xmlSecAddChild(signedInfoNode, xmlSecNodeSignatureMethod, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeSignatureMethod));
        xmlFreeNode(signNode);
        return(NULL);
    }
    if(xmlSetProp(cur, xmlSecAttrAlgorithm, signMethodId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(signMethodId->href));
        xmlFreeNode(signNode);
        return(NULL);
    }

    return(signNode);
}

/**
 * xmlSecTmplSignatureEnsureKeyInfo:
 * @signNode:           the  pointer to <dsig:Signature/> node.
 * @id:                 the node id (may be NULL).
 *
 * Adds (if necessary) <dsig:KeyInfo/> node to the <dsig:Signature/>
 * node @signNode.
 *
 * Returns: the pointer to newly created <dsig:KeyInfo/> node or NULL if an
 * error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureEnsureKeyInfo(xmlNodePtr signNode, const xmlChar *id) {
    xmlNodePtr res;

    xmlSecAssert2(signNode != NULL, NULL);

    res = xmlSecFindChild(signNode, xmlSecNodeKeyInfo, xmlSecDSigNs);
    if(res == NULL) {
        xmlNodePtr signValueNode;

        signValueNode = xmlSecFindChild(signNode, xmlSecNodeSignatureValue, xmlSecDSigNs);
        if(signValueNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        xmlSecErrorsSafeString(xmlSecNodeSignatureValue),
                        XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }

        res = xmlSecAddNextSibling(signValueNode, xmlSecNodeKeyInfo, xmlSecDSigNs);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddNextSibling",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeKeyInfo));
            return(NULL);
        }
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    return(res);
}

/**
 * xmlSecTmplSignatureAddReference:
 * @signNode:           the pointer to <dsig:Signature/> node.
 * @digestMethodId:     the reference digest method.
 * @id:                 the node id (may be NULL).
 * @uri:                the reference node uri (may be NULL).
 * @type:               the reference node type (may be NULL).
 *
 * Adds <dsig:Reference/> node with given URI (@uri), Id (@id) and
 * Type (@type) attributes and the required children <dsig:DigestMethod/> and
 * <dsig:DigestValue/> to the <dsig:SignedInfo/> child of @signNode.
 *
 * Returns: the pointer to newly created <dsig:Reference/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureAddReference(xmlNodePtr signNode, xmlSecTransformId digestMethodId,
                    const xmlChar *id, const xmlChar *uri, const xmlChar *type) {
    xmlNodePtr signedInfoNode;

    xmlSecAssert2(signNode != NULL, NULL);
    xmlSecAssert2(digestMethodId != NULL, NULL);
    xmlSecAssert2(digestMethodId->href != NULL, NULL);

    signedInfoNode = xmlSecFindChild(signNode, xmlSecNodeSignedInfo, xmlSecDSigNs);
    if(signedInfoNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeSignedInfo),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    return(xmlSecTmplAddReference(signedInfoNode, digestMethodId, id, uri, type));
}

static xmlNodePtr
xmlSecTmplAddReference(xmlNodePtr parentNode, xmlSecTransformId digestMethodId,
                    const xmlChar *id, const xmlChar *uri, const xmlChar *type) {
    xmlNodePtr res;
    xmlNodePtr cur;

    xmlSecAssert2(parentNode != NULL, NULL);
    xmlSecAssert2(digestMethodId != NULL, NULL);
    xmlSecAssert2(digestMethodId->href != NULL, NULL);

    /* add Reference node */
    res = xmlSecAddChild(parentNode, xmlSecNodeReference, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeReference));
        return(NULL);
    }

    /* set Reference node attributes */
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    if(type != NULL) {
        xmlSetProp(res, xmlSecAttrType, type);
    }
    if(uri != NULL) {
        xmlSetProp(res, xmlSecAttrURI, uri);
    }

    /* add DigestMethod node and set algorithm */
    cur = xmlSecAddChild(res, xmlSecNodeDigestMethod, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDigestMethod));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }
    if(xmlSetProp(cur, xmlSecAttrAlgorithm, digestMethodId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(digestMethodId->href));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }

    /* add DigestValue node */
    cur = xmlSecAddChild(res, xmlSecNodeDigestValue, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDigestValue));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }

    return(res);
}

/**
 * xmlSecTmplSignatureAddObject:
 * @signNode:           the pointer to <dsig:Signature/> node.
 * @id:                 the node id (may be NULL).
 * @mimeType:           the object mime type (may be NULL).
 * @encoding:           the object encoding (may be NULL).
 *
 * Adds <dsig:Object/> node to the <dsig:Signature/> node @signNode.
 *
 * Returns: the pointer to newly created <dsig:Object/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureAddObject(xmlNodePtr signNode, const xmlChar *id,
                         const xmlChar *mimeType, const xmlChar *encoding) {
    xmlNodePtr res;

    xmlSecAssert2(signNode != NULL, NULL);

    res = xmlSecAddChild(signNode, xmlSecNodeObject, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeObject));
        return(NULL);
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    if(mimeType != NULL) {
        xmlSetProp(res, xmlSecAttrMimeType, mimeType);
    }
    if(encoding != NULL) {
        xmlSetProp(res, xmlSecAttrEncoding, encoding);
    }
    return(res);
}

/**
 * xmlSecTmplSignatureGetSignMethodNode:
 * @signNode:           the pointer to <dsig:Signature /> node.
 *
 * Gets pointer to <dsig:SignatureMethod/> child of <dsig:KeyInfo/> node.
 *
 * Returns: pointer to <dsig:SignatureMethod /> node or NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureGetSignMethodNode(xmlNodePtr signNode) {
    xmlNodePtr signedInfoNode;

    xmlSecAssert2(signNode != NULL, NULL);

    signedInfoNode = xmlSecFindChild(signNode, xmlSecNodeSignedInfo, xmlSecDSigNs);
    if(signedInfoNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeSignedInfo),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }
    return(xmlSecFindChild(signedInfoNode, xmlSecNodeSignatureMethod, xmlSecDSigNs));
}

/**
 * xmlSecTmplSignatureGetC14NMethodNode:
 * @signNode:           the pointer to <dsig:Signature /> node.
 *
 * Gets pointer to <dsig:CanonicalizationMethod/> child of <dsig:KeyInfo/> node.
 *
 * Returns: pointer to <dsig:CanonicalizationMethod /> node or NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplSignatureGetC14NMethodNode(xmlNodePtr signNode) {
    xmlNodePtr signedInfoNode;

    xmlSecAssert2(signNode != NULL, NULL);

    signedInfoNode = xmlSecFindChild(signNode, xmlSecNodeSignedInfo, xmlSecDSigNs);
    if(signedInfoNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeSignedInfo),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }
    return(xmlSecFindChild(signedInfoNode, xmlSecNodeCanonicalizationMethod, xmlSecDSigNs));
}

/**
 * xmlSecTmplReferenceAddTransform:
 * @referenceNode:              the pointer to <dsig:Reference/> node.
 * @transformId:                the transform method id.
 *
 * Adds <dsig:Transform/> node to the <dsig:Reference/> node @referenceNode.
 *
 * Returns: the pointer to newly created <dsig:Transform/> node or NULL if an
 * error occurs.
 */
xmlNodePtr
xmlSecTmplReferenceAddTransform(xmlNodePtr referenceNode, xmlSecTransformId transformId) {
    xmlNodePtr transformsNode;
    xmlNodePtr res;

    xmlSecAssert2(referenceNode != NULL, NULL);
    xmlSecAssert2(transformId != NULL, NULL);
    xmlSecAssert2(transformId->href != NULL, NULL);

    /* do we need to create Transforms node first */
    transformsNode = xmlSecFindChild(referenceNode, xmlSecNodeTransforms, xmlSecDSigNs);
    if(transformsNode == NULL) {
        xmlNodePtr tmp;

        tmp = xmlSecGetNextElementNode(referenceNode->children);
        if(tmp == NULL) {
            transformsNode = xmlSecAddChild(referenceNode, xmlSecNodeTransforms, xmlSecDSigNs);
        } else {
            transformsNode = xmlSecAddPrevSibling(tmp, xmlSecNodeTransforms, xmlSecDSigNs);
        }
        if(transformsNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild or xmlSecAddPrevSibling",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeTransforms));
            return(NULL);
        }
    }

    res = xmlSecAddChild(transformsNode, xmlSecNodeTransform, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeTransform));
        return(NULL);
    }

    if(xmlSetProp(res, xmlSecAttrAlgorithm, transformId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(transformId->href));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }

    return(res);
}

/**
 * xmlSecTmplObjectAddSignProperties:
 * @objectNode:         the  pointer to <dsig:Object/> node.
 * @id:                 the node id (may be NULL).
 * @target:             the Target  (may be NULL).
 *
 * Adds <dsig:SignatureProperties/> node to the <dsig:Object/> node @objectNode.
 *
 * Returns: the pointer to newly created <dsig:SignatureProperties/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplObjectAddSignProperties(xmlNodePtr objectNode, const xmlChar *id, const xmlChar *target) {
    xmlNodePtr res;

    xmlSecAssert2(objectNode != NULL, NULL);

    res = xmlSecAddChild(objectNode, xmlSecNodeSignatureProperties, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeSignatureProperties));
        return(NULL);
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    if(target != NULL) {
        xmlSetProp(res, xmlSecAttrTarget, target);
    }
    return(res);
}

/**
 * xmlSecTmplObjectAddManifest:
 * @objectNode:         the  pointer to <dsig:Object/> node.
 * @id:                 the node id (may be NULL).
 *
 * Adds <dsig:Manifest/> node to the <dsig:Object/> node @objectNode.
 *
 * Returns: the pointer to newly created <dsig:Manifest/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplObjectAddManifest(xmlNodePtr objectNode,  const xmlChar *id) {
    xmlNodePtr res;

    xmlSecAssert2(objectNode != NULL, NULL);

    res = xmlSecAddChild(objectNode, xmlSecNodeManifest, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeManifest));
        return(NULL);
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    return(res);
}

/**
 * xmlSecTmplManifestAddReference:
 * @manifestNode:       the pointer to <dsig:Manifest/> node.
 * @digestMethodId:     the reference digest method.
 * @id:                 the node id (may be NULL).
 * @uri:                the reference node uri (may be NULL).
 * @type:               the reference node type (may be NULL).
 *
 * Adds <dsig:Reference/> node with specified URI (@uri), Id (@id) and
 * Type (@type) attributes and the required children <dsig:DigestMethod/> and
 * <dsig:DigestValue/> to the <dsig:Manifest/> node @manifestNode.
 *
 * Returns: the pointer to newly created <dsig:Reference/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplManifestAddReference(xmlNodePtr manifestNode, xmlSecTransformId digestMethodId,
                              const xmlChar *id, const xmlChar *uri, const xmlChar *type) {
    return(xmlSecTmplAddReference(manifestNode, digestMethodId, id, uri, type));
}

/**************************************************************************
 *
 * <enc:EncryptedData/> node
 *
 **************************************************************************/
/**
 * xmlSecTmplEncDataCreate:
 * @doc:                the pointer to signature document or NULL; in the later
 *                      case, application must later call @xmlSetTreeDoc to ensure
 *                      that all the children nodes have correct pointer to XML document.
 * @encMethodId:        the encryption method (may be NULL).
 * @id:                 the Id attribute (optional).
 * @type:               the Type attribute (optional)
 * @mimeType:           the MimeType attribute (optional)
 * @encoding:           the Encoding attribute (optional)
 *
 * Creates new <enc:EncryptedData /> node for encryption template.
 *
 * Returns: the pointer newly created  <enc:EncryptedData/> node or NULL
 * if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataCreate(xmlDocPtr doc, xmlSecTransformId encMethodId,
                            const xmlChar *id, const xmlChar *type,
                            const xmlChar *mimeType, const xmlChar *encoding) {
    xmlNodePtr encNode;
    xmlNsPtr ns;

    encNode = xmlNewDocNode(doc, NULL, xmlSecNodeEncryptedData, NULL);
    if(encNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlNewDocNode",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeEncryptedData));
        return(NULL);
    }

    ns = xmlNewNs(encNode, xmlSecEncNs, NULL);
    if(ns == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlNewNs",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "ns=%s",
                    xmlSecErrorsSafeString(xmlSecEncNs));
        return(NULL);
    }
    xmlSetNs(encNode, ns);

    if(id != NULL) {
        xmlSetProp(encNode, xmlSecAttrId, id);
    }
    if(type != NULL) {
        xmlSetProp(encNode, xmlSecAttrType, type);
    }
    if(mimeType != NULL) {
        xmlSetProp(encNode, xmlSecAttrMimeType, mimeType);
    }
    if(encoding != NULL) {
        xmlSetProp(encNode, xmlSecAttrEncoding, encoding);
    }

    if(xmlSecTmplPrepareEncData(encNode, encMethodId) < 0) {
        xmlFreeNode(encNode);
        return(NULL);
    }
    return(encNode);
}

static int
xmlSecTmplPrepareEncData(xmlNodePtr parentNode, xmlSecTransformId encMethodId) {
    xmlNodePtr cur;

    xmlSecAssert2(parentNode != NULL, -1);
    xmlSecAssert2((encMethodId == NULL) || (encMethodId->href != NULL), -1);

    /* add EncryptionMethod node if requested */
    if(encMethodId != NULL) {
        cur = xmlSecAddChild(parentNode, xmlSecNodeEncryptionMethod, xmlSecEncNs);
        if(cur == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeEncryptionMethod));
            return(-1);
        }
        if(xmlSetProp(cur, xmlSecAttrAlgorithm, encMethodId->href) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSetProp",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "name=%s,value=%s",
                        xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                        xmlSecErrorsSafeString(encMethodId->href));
            return(-1);
        }
    }

    /* and CipherData node */
    cur = xmlSecAddChild(parentNode, xmlSecNodeCipherData, xmlSecEncNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeCipherData));
        return(-1);
    }

    return(0);
}


/**
 * xmlSecTmplEncDataEnsureKeyInfo:
 * @encNode:            the pointer to <enc:EncryptedData/> node.
 * @id:                 the Id attrbibute (optional).
 *
 * Adds <dsig:KeyInfo/> to the  <enc:EncryptedData/> node @encNode.
 *
 * Returns: the pointer to newly created <dsig:KeyInfo/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataEnsureKeyInfo(xmlNodePtr encNode, const xmlChar* id) {
    xmlNodePtr res;

    xmlSecAssert2(encNode != NULL, NULL);

    res = xmlSecFindChild(encNode, xmlSecNodeKeyInfo, xmlSecDSigNs);
    if(res == NULL) {
        xmlNodePtr cipherDataNode;

        cipherDataNode = xmlSecFindChild(encNode, xmlSecNodeCipherData, xmlSecEncNs);
        if(cipherDataNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        xmlSecErrorsSafeString(xmlSecNodeCipherData),
                        XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                        XMLSEC_ERRORS_NO_MESSAGE);
            return(NULL);
        }

        res = xmlSecAddPrevSibling(cipherDataNode, xmlSecNodeKeyInfo, xmlSecDSigNs);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddPrevSibling",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeKeyInfo));
            return(NULL);
        }
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    return(res);
}

/**
 * xmlSecTmplEncDataEnsureEncProperties:
 * @encNode:            the pointer to <enc:EncryptedData/> node.
 * @id:                 the Id attribute (optional).
 *
 * Adds <enc:EncryptionProperties/> node to the <enc:EncryptedData/>
 * node @encNode.
 *
 * Returns: the pointer to newly created <enc:EncryptionProperties/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataEnsureEncProperties(xmlNodePtr encNode, const xmlChar *id) {
    xmlNodePtr res;

    xmlSecAssert2(encNode != NULL, NULL);

    res = xmlSecFindChild(encNode, xmlSecNodeEncryptionProperties, xmlSecEncNs);
    if(res == NULL) {
        res = xmlSecAddChild(encNode, xmlSecNodeEncryptionProperties, xmlSecEncNs);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeEncryptionProperties));
            return(NULL);
        }
    }

    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }

    return(res);
}

/**
 * xmlSecTmplEncDataAddEncProperty:
 * @encNode:            the pointer to <enc:EncryptedData/> node.
 * @id:                 the Id attribute (optional).
 * @target:             the Target attribute (optional).
 *
 * Adds <enc:EncryptionProperty/> node (and the parent
 * <enc:EncryptionProperties/> node if required) to the
 * <enc:EncryptedData/> node @encNode.
 *
 * Returns: the pointer to newly created <enc:EncryptionProperty/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataAddEncProperty(xmlNodePtr encNode, const xmlChar *id, const xmlChar *target) {
    xmlNodePtr encProps;
    xmlNodePtr res;

    xmlSecAssert2(encNode != NULL, NULL);

    encProps = xmlSecTmplEncDataEnsureEncProperties(encNode, NULL);
    if(encProps == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecTmplEncDataEnsureEncProperties",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecAddChild(encProps, xmlSecNodeEncryptionProperty, xmlSecEncNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeEncryptionProperty));
        return(NULL);
    }
    if(id != NULL) {
        xmlSetProp(res, xmlSecAttrId, id);
    }
    if(target != NULL) {
        xmlSetProp(res, xmlSecAttrTarget, target);
    }

    return(res);
}

/**
 * xmlSecTmplEncDataEnsureCipherValue:
 * @encNode:            the pointer to <enc:EncryptedData/> node.
 *
 * Adds <enc:CipherValue/> to the <enc:EncryptedData/> node @encNode.
 *
 * Returns: the pointer to newly created <enc:CipherValue/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataEnsureCipherValue(xmlNodePtr encNode) {
    xmlNodePtr cipherDataNode;
    xmlNodePtr res, tmp;

    xmlSecAssert2(encNode != NULL, NULL);

    cipherDataNode = xmlSecFindChild(encNode, xmlSecNodeCipherData, xmlSecEncNs);
    if(cipherDataNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeCipherData),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    /* check that we don;t have CipherReference node */
    tmp = xmlSecFindChild(cipherDataNode, xmlSecNodeCipherReference, xmlSecEncNs);
    if(tmp != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeCipherReference),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecFindChild(cipherDataNode, xmlSecNodeCipherValue, xmlSecEncNs);
    if(res == NULL) {
        res = xmlSecAddChild(cipherDataNode, xmlSecNodeCipherValue, xmlSecEncNs);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeCipherValue));
            return(NULL);
        }
    }

    return(res);
}

/**
 * xmlSecTmplEncDataEnsureCipherReference:
 * @encNode:            the pointer to <enc:EncryptedData/> node.
 * @uri:                the URI attribute (may be NULL).
 *
 * Adds <enc:CipherReference/> node with specified URI attribute @uri
 * to the <enc:EncryptedData/> node @encNode.
 *
 * Returns: the pointer to newly created <enc:CipherReference/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataEnsureCipherReference(xmlNodePtr encNode, const xmlChar *uri) {
    xmlNodePtr cipherDataNode;
    xmlNodePtr res, tmp;

    xmlSecAssert2(encNode != NULL, NULL);

    cipherDataNode = xmlSecFindChild(encNode, xmlSecNodeCipherData, xmlSecEncNs);
    if(cipherDataNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeCipherData),
                    XMLSEC_ERRORS_R_NODE_NOT_FOUND,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    /* check that we don;t have CipherValue node */
    tmp = xmlSecFindChild(cipherDataNode, xmlSecNodeCipherValue, xmlSecEncNs);
    if(tmp != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeCipherValue),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    res = xmlSecFindChild(cipherDataNode, xmlSecNodeCipherReference, xmlSecEncNs);
    if(res == NULL) {
        res = xmlSecAddChild(cipherDataNode, xmlSecNodeCipherReference, xmlSecEncNs);
        if(res == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeCipherReference));
            return(NULL);
        }
    }

    if(uri != NULL) {
        xmlSetProp(res, xmlSecAttrURI, uri);
    }

    return(res);
}

/**
 * xmlSecTmplEncDataGetEncMethodNode:
 * @encNode:            the pointer to <enc:EcnryptedData /> node.
 *
 * Gets pointer to <enc:EncrytpionMethod/> node.
 *
 * Returns: pointer to <enc:EncryptionMethod /> node or NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplEncDataGetEncMethodNode(xmlNodePtr encNode) {
    xmlSecAssert2(encNode != NULL, NULL);

    return(xmlSecFindChild(encNode, xmlSecNodeEncryptionMethod, xmlSecEncNs));
}

/**
 * xmlSecTmplCipherReferenceAddTransform:
 * @cipherReferenceNode:        the pointer to <enc:CipherReference/> node.
 * @transformId:                the transform id.
 *
 * Adds <dsig:Transform/> node (and the parent <dsig:Transforms/> node)
 * with specified transform methods @transform to the <enc:CipherReference/>
 * child node of the <enc:EncryptedData/> node @encNode.
 *
 * Returns: the pointer to newly created <dsig:Transform/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplCipherReferenceAddTransform(xmlNodePtr cipherReferenceNode,
                                  xmlSecTransformId transformId) {
    xmlNodePtr transformsNode;
    xmlNodePtr res;

    xmlSecAssert2(cipherReferenceNode != NULL, NULL);
    xmlSecAssert2(transformId != NULL, NULL);
    xmlSecAssert2(transformId->href != NULL, NULL);

    transformsNode = xmlSecFindChild(cipherReferenceNode, xmlSecNodeTransforms, xmlSecEncNs);
    if(transformsNode == NULL) {
        transformsNode = xmlSecAddChild(cipherReferenceNode, xmlSecNodeTransforms, xmlSecEncNs);
        if(transformsNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeTransforms));
            return(NULL);
        }
    }

    res = xmlSecAddChild(transformsNode,  xmlSecNodeTransform, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeTransform));
        return(NULL);
    }

    if(xmlSetProp(res, xmlSecAttrAlgorithm, transformId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(transformId->href));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }

    return(res);
}


/***********************************************************************
 *
 * <enc:EncryptedKey> node
 *
 **********************************************************************/

/**
 * xmlSecTmplReferenceListAddDataReference:
 * @encNode:                    the pointer to <enc:EncryptedKey/> node.
 * @uri:                        uri to reference (optional)
 *
 * Adds <enc:DataReference/> and the parent <enc:ReferenceList/> node (if needed).
 *
 * Returns: the pointer to newly created <enc:DataReference/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplReferenceListAddDataReference(xmlNodePtr encNode, const xmlChar *uri) {
    xmlNodePtr refListNode, res;

    xmlSecAssert2(encNode != NULL, NULL);

    refListNode = xmlSecFindChild(encNode, xmlSecNodeReferenceList, xmlSecEncNs);
    if(refListNode == NULL) {
        refListNode = xmlSecAddChild(encNode, xmlSecNodeReferenceList, xmlSecEncNs);
        if(refListNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeReferenceList));
            return(NULL);
        }
    }

    res = xmlSecAddChild(refListNode,  xmlSecNodeDataReference, xmlSecEncNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeDataReference));
        return(NULL);
    }

    if(uri != NULL) {
        if(xmlSetProp(res, xmlSecAttrURI, uri) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSetProp",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "name=%s,value=%s",
                        xmlSecErrorsSafeString(xmlSecAttrURI),
                        xmlSecErrorsSafeString(uri));
            xmlUnlinkNode(res);
            xmlFreeNode(res);
            return(NULL);
        }
    }

    return(res);
}

/**
 * xmlSecTmplReferenceListAddKeyReference:
 * @encNode:                    the pointer to <enc:EncryptedKey/> node.
 * @uri:                        uri to reference (optional)
 *
 * Adds <enc:KeyReference/> and the parent <enc:ReferenceList/> node (if needed).
 *
 * Returns: the pointer to newly created <enc:KeyReference/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplReferenceListAddKeyReference(xmlNodePtr encNode, const xmlChar *uri) {
    xmlNodePtr refListNode, res;

    xmlSecAssert2(encNode != NULL, NULL);

    refListNode = xmlSecFindChild(encNode, xmlSecNodeReferenceList, xmlSecEncNs);
    if(refListNode == NULL) {
        refListNode = xmlSecAddChild(encNode, xmlSecNodeReferenceList, xmlSecEncNs);
        if(refListNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeReferenceList));
            return(NULL);
        }
    }

    res = xmlSecAddChild(refListNode,  xmlSecNodeKeyReference, xmlSecEncNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeKeyReference));
        return(NULL);
    }

    if(uri != NULL) {
        if(xmlSetProp(res, xmlSecAttrURI, uri) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSetProp",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "name=%s,value=%s",
                        xmlSecErrorsSafeString(xmlSecAttrURI),
                        xmlSecErrorsSafeString(uri));
            xmlUnlinkNode(res);
            xmlFreeNode(res);
            return(NULL);
        }
    }

    return(res);
}


/**************************************************************************
 *
 * <dsig:KeyInfo/> node
 *
 **************************************************************************/

/**
 * xmlSecTmplKeyInfoAddKeyName:
 * @keyInfoNode:        the pointer to <dsig:KeyInfo/> node.
 * @name:               the key name (optional).
 *
 * Adds <dsig:KeyName/> node to the <dsig:KeyInfo/> node @keyInfoNode.
 *
 * Returns: the pointer to the newly created <dsig:KeyName/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplKeyInfoAddKeyName(xmlNodePtr keyInfoNode, const xmlChar* name) {
    xmlNodePtr res;

    xmlSecAssert2(keyInfoNode != NULL, NULL);

    res = xmlSecAddChild(keyInfoNode, xmlSecNodeKeyName, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeKeyName));
        return(NULL);
    }
    if(name != NULL) {
        xmlSecNodeEncodeAndSetContent(res, name);
    }
    return(res);
}

/**
 * xmlSecTmplKeyInfoAddKeyValue:
 * @keyInfoNode:        the pointer to <dsig:KeyInfo/> node.
 *
 * Adds <dsig:KeyValue/> node to the <dsig:KeyInfo/> node @keyInfoNode.
 *
 * Returns: the pointer to the newly created <dsig:KeyValue/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplKeyInfoAddKeyValue(xmlNodePtr keyInfoNode) {
    xmlNodePtr res;

    xmlSecAssert2(keyInfoNode != NULL, NULL);

    res = xmlSecAddChild(keyInfoNode, xmlSecNodeKeyValue, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeKeyValue));
        return(NULL);
    }

    return(res);
}

/**
 * xmlSecTmplKeyInfoAddX509Data:
 * @keyInfoNode:        the pointer to <dsig:KeyInfo/> node.
 *
 * Adds <dsig:X509Data/> node to the <dsig:KeyInfo/> node @keyInfoNode.
 *
 * Returns: the pointer to the newly created <dsig:X509Data/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplKeyInfoAddX509Data(xmlNodePtr keyInfoNode) {
    xmlNodePtr res;

    xmlSecAssert2(keyInfoNode != NULL, NULL);

    res = xmlSecAddChild(keyInfoNode, xmlSecNodeX509Data, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509Data));
        return(NULL);
    }

    return(res);
}

/**
 * xmlSecTmplKeyInfoAddRetrievalMethod:
 * @keyInfoNode:        the pointer to <dsig:KeyInfo/> node.
 * @uri:                the URI attribute (optional).
 * @type:               the Type attribute(optional).
 *
 * Adds <dsig:RetrievalMethod/> node to the <dsig:KeyInfo/> node @keyInfoNode.
 *
 * Returns: the pointer to the newly created <dsig:RetrievalMethod/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplKeyInfoAddRetrievalMethod(xmlNodePtr keyInfoNode, const xmlChar *uri,
                             const xmlChar *type) {
    xmlNodePtr res;

    xmlSecAssert2(keyInfoNode != NULL, NULL);

    res = xmlSecAddChild(keyInfoNode, xmlSecNodeRetrievalMethod, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRetrievalMethod));
        return(NULL);
    }

    if(uri != NULL) {
        xmlSetProp(res, xmlSecAttrURI, uri);
    }
    if(type != NULL) {
        xmlSetProp(res, xmlSecAttrType, type);
    }
    return(res);
}

/**
 * xmlSecTmplRetrievalMethodAddTransform:
 * @retrMethodNode:     the pointer to <dsig:RetrievalMethod/> node.
 * @transformId:        the transform id.
 *
 * Adds <dsig:Transform/> node (and the parent <dsig:Transforms/> node
 * if required) to the <dsig:RetrievalMethod/> node @retrMethod.
 *
 * Returns: the pointer to the newly created <dsig:Transforms/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplRetrievalMethodAddTransform(xmlNodePtr retrMethodNode, xmlSecTransformId transformId) {
    xmlNodePtr transformsNode;
    xmlNodePtr res;

    xmlSecAssert2(retrMethodNode != NULL, NULL);
    xmlSecAssert2(transformId != NULL, NULL);
    xmlSecAssert2(transformId->href != NULL, NULL);

    transformsNode = xmlSecFindChild(retrMethodNode, xmlSecNodeTransforms, xmlSecDSigNs);
    if(transformsNode == NULL) {
        transformsNode = xmlSecAddChild(retrMethodNode, xmlSecNodeTransforms, xmlSecDSigNs);
        if(transformsNode == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlSecAddChild",
                        XMLSEC_ERRORS_R_XMLSEC_FAILED,
                        "node=%s",
                        xmlSecErrorsSafeString(xmlSecNodeTransforms));
            return(NULL);
        }
    }

    res = xmlSecAddChild(transformsNode,  xmlSecNodeTransform, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeTransform));
        return(NULL);
    }

    if(xmlSetProp(res, xmlSecAttrAlgorithm, transformId->href) == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSetProp",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    "name=%s,value=%s",
                    xmlSecErrorsSafeString(xmlSecAttrAlgorithm),
                    xmlSecErrorsSafeString(transformId->href));
        xmlUnlinkNode(res);
        xmlFreeNode(res);
        return(NULL);
    }

    return(res);
}


/**
 * xmlSecTmplKeyInfoAddEncryptedKey:
 * @keyInfoNode:        the pointer to <dsig:KeyInfo/> node.
 * @encMethodId:        the encryption method (optional).
 * @id:                 the Id attribute (optional).
 * @type:               the Type attribute (optional).
 * @recipient:          the Recipient attribute (optional).
 *
 * Adds <enc:EncryptedKey/> node with given attributes to
 * the <dsig:KeyInfo/> node @keyInfoNode.
 *
 * Returns: the pointer to the newly created <enc:EncryptedKey/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplKeyInfoAddEncryptedKey(xmlNodePtr keyInfoNode, xmlSecTransformId encMethodId,
                         const xmlChar* id, const xmlChar* type, const xmlChar* recipient) {
    xmlNodePtr encKeyNode;

    xmlSecAssert2(keyInfoNode != NULL, NULL);

    /* we allow multiple encrypted key elements */
    encKeyNode = xmlSecAddChild(keyInfoNode, xmlSecNodeEncryptedKey, xmlSecEncNs);
    if(encKeyNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeEncryptedKey));
        return(NULL);
    }

    if(id != NULL) {
        xmlSetProp(encKeyNode, xmlSecAttrId, id);
    }
    if(type != NULL) {
        xmlSetProp(encKeyNode, xmlSecAttrType, type);
    }
    if(recipient != NULL) {
        xmlSetProp(encKeyNode, xmlSecAttrRecipient, recipient);
    }

    if(xmlSecTmplPrepareEncData(encKeyNode, encMethodId) < 0) {
        xmlUnlinkNode(encKeyNode);
        xmlFreeNode(encKeyNode);
        return(NULL);
    }
    return(encKeyNode);
}

/***********************************************************************
 *
 * <dsig:X509Data> node
 *
 **********************************************************************/
/**
 * xmlSecTmplX509DataAddIssuerSerial:
 * @x509DataNode:       the pointer to <dsig:X509Data/> node.
 *
 * Adds <dsig:X509IssuerSerial/> node to the given <dsig:X509Data/> node.
 *
 * Returns: the pointer to the newly created <dsig:X509IssuerSerial/> node or
 * NULL if an error occurs.
 */

xmlNodePtr
xmlSecTmplX509DataAddIssuerSerial(xmlNodePtr x509DataNode) {
    xmlNodePtr cur;

    xmlSecAssert2(x509DataNode != NULL, NULL);

    cur = xmlSecFindChild(x509DataNode, xmlSecNodeX509IssuerSerial, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerSerial),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    cur = xmlSecAddChild(x509DataNode, xmlSecNodeX509IssuerSerial, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerSerial));
        return(NULL);
    }

    return (cur);
}

/**
 * xmlSecTmplX509IssuerSerialAddIssuerName:
 * @x509IssuerSerialNode:       the pointer to <dsig:X509IssuerSerial/> node.
 * @issuerName:         the issuer name (optional).
 *
 * Adds <dsig:X509IssuerName/> node to the <dsig:X509IssuerSerial/> node @x509IssuerSerialNode.
 *
 * Returns: the pointer to the newly created <dsig:X509IssuerName/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplX509IssuerSerialAddIssuerName(xmlNodePtr x509IssuerSerialNode, const xmlChar* issuerName) {
        xmlNodePtr res;

        xmlSecAssert2(x509IssuerSerialNode != NULL, NULL);

  if(xmlSecFindChild(x509IssuerSerialNode, xmlSecNodeX509IssuerName,
                                xmlSecDSigNs) != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerName),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
        }

        res = xmlSecAddChild(x509IssuerSerialNode, xmlSecNodeX509IssuerName, xmlSecDSigNs);
    if(res == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509IssuerName));
        return(NULL);
    }

                if (issuerName != NULL) {
                        xmlSecNodeEncodeAndSetContent(res, issuerName);
                }
                return(res);
}

/**
 * xmlSecTmplX509IssuerSerialAddSerialNumber:
 * @x509IssuerSerialNode:       the pointer to <dsig:X509IssuerSerial/> node.
 * @serial:             the serial number (optional).
 *
 * Adds <dsig:X509SerialNumber/> node to the <dsig:X509IssuerSerial/> node @x509IssuerSerialNode.
 *
 * Returns: the pointer to the newly created <dsig:X509SerialNumber/> node or
 * NULL if an error occurs.
 */
xmlNodePtr
xmlSecTmplX509IssuerSerialAddSerialNumber(xmlNodePtr x509IssuerSerialNode, const xmlChar* serial) {
        xmlNodePtr res;

        xmlSecAssert2(x509IssuerSerialNode != NULL, NULL);

        if(xmlSecFindChild(x509IssuerSerialNode, xmlSecNodeX509SerialNumber,
                                xmlSecDSigNs) != NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                xmlSecErrorsSafeString(xmlSecNodeX509SerialNumber),
                                XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                                XMLSEC_ERRORS_NO_MESSAGE);
                return(NULL);
        }

        res = xmlSecAddChild(x509IssuerSerialNode, xmlSecNodeX509SerialNumber, xmlSecDSigNs);
        if(res == NULL) {
                xmlSecError(XMLSEC_ERRORS_HERE,
                                NULL,
                                "xmlSecAddChild",
                                XMLSEC_ERRORS_R_XMLSEC_FAILED,
                                "node=%s",
                                xmlSecErrorsSafeString(xmlSecNodeX509SerialNumber));
                return(NULL);
        }

        if (serial != NULL) {
                xmlSecNodeEncodeAndSetContent(res, serial);
        }
        return(res);
}

/**
 * xmlSecTmplX509DataAddSubjectName:
 * @x509DataNode:       the pointer to <dsig:X509Data/> node.
 *
 * Adds <dsig:X509SubjectName/> node to the given <dsig:X509Data/> node.
 *
 * Returns: the pointer to the newly created <dsig:X509SubjectName/> node or
 * NULL if an error occurs.
 */

xmlNodePtr
xmlSecTmplX509DataAddSubjectName(xmlNodePtr x509DataNode) {
    xmlNodePtr cur;

    xmlSecAssert2(x509DataNode != NULL, NULL);

    cur = xmlSecFindChild(x509DataNode, xmlSecNodeX509SubjectName, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509SubjectName),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    cur = xmlSecAddChild(x509DataNode, xmlSecNodeX509SubjectName, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SubjectName));
        return(NULL);
    }

    return (cur);
}

/**
 * xmlSecTmplX509DataAddSKI:
 * @x509DataNode:       the pointer to <dsig:X509Data/> node.
 *
 * Adds <dsig:X509SKI/> node to the given <dsig:X509Data/> node.
 *
 * Returns: the pointer to the newly created <dsig:X509SKI/> node or
 * NULL if an error occurs.
 */

xmlNodePtr
xmlSecTmplX509DataAddSKI(xmlNodePtr x509DataNode) {
    xmlNodePtr cur;

    xmlSecAssert2(x509DataNode != NULL, NULL);

    cur = xmlSecFindChild(x509DataNode, xmlSecNodeX509SKI, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509SKI),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    cur = xmlSecAddChild(x509DataNode, xmlSecNodeX509SKI, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509SKI));
        return(NULL);
    }

    return (cur);
}


/**
 * xmlSecTmplX509DataAddCertificate:
 * @x509DataNode:       the pointer to <dsig:X509Data/> node.
 *
 * Adds <dsig:X509Certificate/> node to the given <dsig:X509Data/> node.
 *
 * Returns: the pointer to the newly created <dsig:X509Certificate/> node or
 * NULL if an error occurs.
 */

xmlNodePtr
xmlSecTmplX509DataAddCertificate(xmlNodePtr x509DataNode) {
    xmlNodePtr cur;

    xmlSecAssert2(x509DataNode != NULL, NULL);

    cur = xmlSecFindChild(x509DataNode, xmlSecNodeX509Certificate, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509Certificate),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    cur = xmlSecAddChild(x509DataNode, xmlSecNodeX509Certificate, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509Certificate));
        return(NULL);
    }

    return (cur);
}

/**
 * xmlSecTmplX509DataAddCRL:
 * @x509DataNode:       the pointer to <dsig:X509Data/> node.
 *
 * Adds <dsig:X509CRL/> node to the given <dsig:X509Data/> node.
 *
 * Returns: the pointer to the newly created <dsig:X509CRL/> node or
 * NULL if an error occurs.
 */

xmlNodePtr
xmlSecTmplX509DataAddCRL(xmlNodePtr x509DataNode) {
    xmlNodePtr cur;

    xmlSecAssert2(x509DataNode != NULL, NULL);

    cur = xmlSecFindChild(x509DataNode, xmlSecNodeX509CRL, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeX509CRL),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(NULL);
    }

    cur = xmlSecAddChild(x509DataNode, xmlSecNodeX509CRL, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeX509CRL));
        return(NULL);
    }

    return (cur);
}

/*************************************************************************
 *
 * <dsig:Transform/> node
 *
 ************************************************************************/

/**
 * xmlSecTmplTransformAddHmacOutputLength:
 * @transformNode:      the pointer to <dsig:Transform/> node
 * @bitsLen:            the required length in bits
 *
 * Creates <dsig:HMACOutputLength/> child for the HMAC transform
 * node @node.
 *
 * Returns: 0 on success and a negatie value otherwise.
 */
int
xmlSecTmplTransformAddHmacOutputLength(xmlNodePtr transformNode, xmlSecSize bitsLen) {
    xmlNodePtr cur;
    char buf[32];

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(bitsLen > 0, -1);

    cur = xmlSecFindChild(transformNode, xmlSecNodeHMACOutputLength, xmlSecDSigNs);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeHMACOutputLength),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecAddChild(transformNode, xmlSecNodeHMACOutputLength, xmlSecDSigNs);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeHMACOutputLength));
        return(-1);
    }

    sprintf(buf, "%u", bitsLen);
    xmlNodeSetContent(cur, BAD_CAST buf);
    return(0);
}

/**
 * xmlSecTmplTransformAddRsaOaepParam:
 * @transformNode:      the pointer to <dsig:Transform/> node.
 * @buf:                the OAEP param buffer.
 * @size:               the OAEP param buffer size.
 *
 * Creates <enc:OAEPParam/> child node in the @node.
 *
 * Returns: 0 on success or a negative value if an error occurs.
 */
int
xmlSecTmplTransformAddRsaOaepParam(xmlNodePtr transformNode,
                        const xmlSecByte *buf, xmlSecSize size) {
    xmlNodePtr oaepParamNode;
    xmlChar *base64;

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(buf != NULL, -1);
    xmlSecAssert2(size > 0, -1);

    oaepParamNode = xmlSecFindChild(transformNode, xmlSecNodeRsaOAEPparams, xmlSecEncNs);
    if(oaepParamNode != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeRsaOAEPparams),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    oaepParamNode = xmlSecAddChild(transformNode, xmlSecNodeRsaOAEPparams, xmlSecEncNs);
    if(oaepParamNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeRsaOAEPparams));
        return(-1);
    }

    base64 = xmlSecBase64Encode(buf, size, 0);
    if(base64 == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecBase64Encode",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "size=%d", size);
        return(-1);
    }

    xmlNodeSetContent(oaepParamNode, base64);
    xmlFree(base64);
    return(0);
}

/**
 * xmlSecTmplTransformAddXsltStylesheet:
 * @transformNode:      the pointer to <dsig:Transform/> node.
 * @xslt:               the XSLT transform exspression.
 *
 * Writes the XSLT transform expression to the @node.
 *
 * Returns: 0 on success or a negative value otherwise.
 */
int
xmlSecTmplTransformAddXsltStylesheet(xmlNodePtr transformNode, const xmlChar *xslt) {
    xmlDocPtr xsltDoc;
    int ret;

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(xslt != NULL, -1);

    xsltDoc = xmlParseMemory((const char*)xslt, xmlStrlen(xslt));
    if(xsltDoc == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlParseMemory",
                    XMLSEC_ERRORS_R_XML_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    ret = xmlSecReplaceContent(transformNode, xmlDocGetRootElement(xsltDoc));
    if(ret < 0) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecReplaceContent",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    XMLSEC_ERRORS_NO_MESSAGE);
        xmlFreeDoc(xsltDoc);
        return(-1);
    }

    xmlFreeDoc(xsltDoc);
    return(0);
}

/**
 * xmlSecTmplTransformAddC14NInclNamespaces:
 * @transformNode:      the pointer to <dsig:Transform/> node.
 * @prefixList:         the white space delimited  list of namespace prefixes,
 *                      where "#default" indicates the default namespace
 *                      (optional).
 *
 * Adds "inclusive" namespaces to the ExcC14N transform node @node.
 *
 * Returns: 0 if success or a negative value otherwise.
 */
int
xmlSecTmplTransformAddC14NInclNamespaces(xmlNodePtr transformNode,
                                         const xmlChar *prefixList) {
    xmlNodePtr cur;

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(prefixList != NULL, -1);

    cur = xmlSecFindChild(transformNode, xmlSecNodeInclusiveNamespaces, xmlSecNsExcC14N);
    if(cur != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeInclusiveNamespaces),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    cur = xmlSecAddChild(transformNode, xmlSecNodeInclusiveNamespaces, xmlSecNsExcC14N);
    if(cur == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    xmlSecErrorsSafeString(xmlSecNodeGetName(transformNode)),
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeInclusiveNamespaces));
        return(-1);
    }

    xmlSetProp(cur, xmlSecAttrPrefixList, prefixList);
    return(0);
}

/**
 * xmlSecTmplTransformAddXPath:
 * @transformNode:      the pointer to the <dsig:Transform/> node.
 * @expression:         the XPath expression.
 * @nsList:             the NULL terminated list of namespace prefix/href pairs
 *                      (optional).
 *
 * Writes XPath transform infromation to the <dsig:Transform/> node
 * @node.
 *
 * Returns: 0 for success or a negative value otherwise.
 */
int
xmlSecTmplTransformAddXPath(xmlNodePtr transformNode, const xmlChar *expression,
                         const xmlChar **nsList) {
    xmlNodePtr xpathNode;

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(expression != NULL, -1);

    xpathNode = xmlSecFindChild(transformNode, xmlSecNodeXPath, xmlSecDSigNs);
    if(xpathNode != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeXPath),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    xpathNode = xmlSecAddChild(transformNode, xmlSecNodeXPath, xmlSecDSigNs);
    if(xpathNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeXPath));
        return(-1);
    }

    xmlSecNodeEncodeAndSetContent(xpathNode, expression);
    return((nsList != NULL) ? xmlSecTmplNodeWriteNsList(xpathNode, nsList) : 0);
}

/**
 * xmlSecTmplTransformAddXPath2:
 * @transformNode:      the pointer to the <dsig:Transform/> node.
 * @type:               the XPath2 transform type ("union", "intersect" or "subtract").
 * @expression:         the XPath expression.
 * @nsList:             the NULL terminated list of namespace prefix/href pairs.
 *                      (optional).
 *
 * Writes XPath2 transform infromation to the <dsig:Transform/> node
 * @node.
 *
 * Returns: 0 for success or a negative value otherwise.
 */
int
xmlSecTmplTransformAddXPath2(xmlNodePtr transformNode, const xmlChar* type,
                        const xmlChar *expression, const xmlChar **nsList) {
    xmlNodePtr xpathNode;

    xmlSecAssert2(transformNode != NULL, -1);
    xmlSecAssert2(type != NULL, -1);
    xmlSecAssert2(expression != NULL, -1);

    xpathNode = xmlSecAddChild(transformNode, xmlSecNodeXPath, xmlSecXPath2Ns);
    if(xpathNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeXPath));
        return(-1);
    }
    xmlSetProp(xpathNode, xmlSecAttrFilter, type);

    xmlSecNodeEncodeAndSetContent(xpathNode, expression);
    return((nsList != NULL) ? xmlSecTmplNodeWriteNsList(xpathNode, nsList) : 0);
}

/**
 * xmlSecTmplTransformAddXPointer:
 * @transformNode:      the pointer to the <dsig:Transform/> node.
 * @expression:         the XPath expression.
 * @nsList:             the NULL terminated list of namespace prefix/href pairs.
 *                      (optional).
 *
 * Writes XPoniter transform infromation to the <dsig:Transform/> node
 * @node.
 *
 * Returns: 0 for success or a negative value otherwise.
 */
int
xmlSecTmplTransformAddXPointer(xmlNodePtr transformNode, const xmlChar *expression,
                         const xmlChar **nsList) {
    xmlNodePtr xpointerNode;

    xmlSecAssert2(expression != NULL, -1);
    xmlSecAssert2(transformNode != NULL, -1);

    xpointerNode = xmlSecFindChild(transformNode, xmlSecNodeXPointer, xmlSecXPointerNs);
    if(xpointerNode != NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    xmlSecErrorsSafeString(xmlSecNodeXPointer),
                    XMLSEC_ERRORS_R_NODE_ALREADY_PRESENT,
                    XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }

    xpointerNode = xmlSecAddChild(transformNode, xmlSecNodeXPointer, xmlSecXPointerNs);
    if(xpointerNode == NULL) {
        xmlSecError(XMLSEC_ERRORS_HERE,
                    NULL,
                    "xmlSecAddChild",
                    XMLSEC_ERRORS_R_XMLSEC_FAILED,
                    "node=%s",
                    xmlSecErrorsSafeString(xmlSecNodeXPointer));
        return(-1);
    }


    xmlSecNodeEncodeAndSetContent(xpointerNode, expression);
    return((nsList != NULL) ? xmlSecTmplNodeWriteNsList(xpointerNode, nsList) : 0);
}

static int
xmlSecTmplNodeWriteNsList(xmlNodePtr parentNode, const xmlChar** nsList) {
    xmlNsPtr ns;
    const xmlChar *prefix;
    const xmlChar *href;
    const xmlChar **ptr;

    xmlSecAssert2(parentNode != NULL, -1);
    xmlSecAssert2(nsList != NULL, -1);

    ptr = nsList;
    while((*ptr) != NULL) {
        if(xmlStrEqual(BAD_CAST "#default", (*ptr))) {
            prefix = NULL;
        } else {
            prefix = (*ptr);
        }
        if((++ptr) == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        NULL,
                        XMLSEC_ERRORS_R_INVALID_DATA,
                        "unexpected end of ns list");
            return(-1);
        }
        href = *(ptr++);

        ns = xmlNewNs(parentNode, href, prefix);
        if(ns == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
                        NULL,
                        "xmlNewNs",
                        XMLSEC_ERRORS_R_XML_FAILED,
                        "href=%s;prefix=%s",
                        xmlSecErrorsSafeString(href),
                        xmlSecErrorsSafeString(prefix));
            return(-1);
        }
    }
    return(0);
}
