/** 
 * XML Security standards test: XMLDSig
 * 
 * See Copyright for the status of this software.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
#define snprintf _snprintf
#endif

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifndef XMLSEC_NO_XSLT
#include <libxslt/xslt.h>
#include <libxslt/extensions.h> 
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxslt/security.h>
#include <libexslt/exslt.h>
#endif /* XMLSEC_NO_XSLT */

#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/keys.h>
#include <xmlsec/keyinfo.h>
#include <xmlsec/keysmngr.h>
#include <xmlsec/transforms.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/xmlenc.h>
#include <xmlsec/parser.h>
#include <xmlsec/templates.h>
#include <xmlsec/errors.h>

#include "crypto.h"
#include "cmdline.h"

static const char copyright[] =
    "Written by Aleksey Sanin <aleksey@aleksey.com>.\n\n"
    "Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved..\n"
    "This is free software: see the source for copying information.\n";

static const char bugs[] = 
    "Report bugs to http://www.aleksey.com/xmlsec/bugs.html\n";

static const char helpCommands1[] =     
    "Usage: xmlsec <command> [<options>] [<files>]\n"
    "\n"
    "xmlsec is a command line tool for signing, verifying, encrypting and\n"
    "decrypting XML documents. The allowed <command> values are:\n"
    "  --help      "    "\tdisplay this help information and exit\n"
    "  --help-all  "    "\tdisplay help information for all commands/options and exit\n"
    "  --help-<cmd>"    "\tdisplay help information for command <cmd> and exit\n"
    "  --version   "    "\tprint version information and exit\n"
    "  --keys      "    "\tkeys XML file manipulation\n";

static const char helpCommands2[] =     
#ifndef XMLSEC_NO_XMLDSIG
    "  --sign      "    "\tsign data and output XML document\n"
    "  --verify    "    "\tverify signed document\n"
#ifndef XMLSEC_NO_TMPL_TEST
    "  --sign-tmpl "    "\tcreate and sign dynamicaly generated signature template\n"
#endif /* XMLSEC_NO_TMPL_TEST */
#endif /* XMLSEC_NO_XMLDSIG */
#ifndef XMLSEC_NO_XMLENC
    "  --encrypt   "    "\tencrypt data and output XML document\n"
    "  --decrypt   "    "\tdecrypt data from XML document\n"
#endif /* XMLSEC_NO_XMLENC */
    ;

static const char helpVersion[] = 
    "Usage: xmlsec version\n"
    "Prints version information and exits\n";

static const char helpKeys[] =     
    "Usage: xmlsec keys [<options>] <file>\n"
    "Creates a new XML keys file <file>\n";
    
static const char helpSign[] =     
    "Usage: xmlsec sign [<options>] <file>\n"
    "Calculates XML Digital Signature using template file <file>\n";
    
static const char helpVerify[] =     
    "Usage: xmlsec verify [<options>] <file>\n"
    "Verifies XML Digital Signature in the <file>\n";

static const char helpSignTmpl[] =     
    "Usage: xmlsec sign-tmpl [<options>]\n"
    "Creates a simple dynamic template and calculates XML Digital Signature\n"
    "(for testing only).\n";

static const char helpEncrypt[] =     
    "Usage: xmlsec encrypt [<options>] <file>\n"
    "Encrypts data and creates XML Encryption using template file <file>\n";

static const char helpEncryptTmpl[] =     
    "Usage: xmlsec encrypt [<options>]\n"
    "Creates a simple dynamic template and calculates XML Encryption\n";

static const char helpDecrypt[] =     
    "Usage: xmlsec decrypt [<options>] <file>\n"
    "Decrypts XML Encryption data in the <file>\n";

static const char helpListKeyData[] =     
    "Usage: xmlsec list-key-data\n"
    "Prints the list of known key data klasses\n";

static const char helpCheckKeyData[] =     
    "Usage: xmlsec check-key-data <key-data-name> [<key-data-name> ... ]\n"
    "Checks the given key-data against the list of known key-data klasses\n";

static const char helpListTransforms[] =     
    "Usage: xmlsec list-transforms\n"
    "Prints the list of known transform klasses\n";

static const char helpCheckTransforms[] =     
    "Usage: xmlsec check-transforms <transform-name> [<transform-name> ... ]\n"
    "Checks the given transforms against the list of known transform klasses\n";

#define xmlSecAppCmdLineTopicGeneral            0x0001
#define xmlSecAppCmdLineTopicDSigCommon         0x0002
#define xmlSecAppCmdLineTopicDSigSign           0x0004
#define xmlSecAppCmdLineTopicDSigVerify         0x0008
#define xmlSecAppCmdLineTopicEncCommon          0x0010
#define xmlSecAppCmdLineTopicEncEncrypt         0x0020
#define xmlSecAppCmdLineTopicEncDecrypt         0x0040
/* #define UNUSED         0x0080 */
#define xmlSecAppCmdLineTopicKeysMngr           0x1000
#define xmlSecAppCmdLineTopicX509Certs          0x2000
#define xmlSecAppCmdLineTopicVersion            0x4000
#define xmlSecAppCmdLineTopicCryptoConfig       0x8000
#define xmlSecAppCmdLineTopicAll                0xFFFF

/****************************************************************
 *
 * General configuration params
 *
 ***************************************************************/
static xmlSecAppCmdLineParam helpParam = { 
    xmlSecAppCmdLineTopicGeneral,
    "--help",
    "-h",
    "--help"
    "\n\tprint help information about the command",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam cryptoParam = { 
    xmlSecAppCmdLineTopicCryptoConfig,
    "--crypto",
    NULL,
    "--crypto <name>"
    "\n\tthe name of the crypto engine to use from the following"
    "\n\tlist: openssl, mscrypto, nss, gnutls, gcrypt (if no crypto engine is"
    "\n\tspecified then the default one is used)",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam cryptoConfigParam = { 
    xmlSecAppCmdLineTopicCryptoConfig,
    "--crypto-config",
    NULL,
    "--crypto-config <path>"
    "\n\tpath to crypto engine configuration",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};


static xmlSecAppCmdLineParam repeatParam = { 
    xmlSecAppCmdLineTopicCryptoConfig,
    "--repeat",
    "-r",
    "--repeat <number>"
    "\n\trepeat the operation <number> times",
    xmlSecAppCmdLineParamTypeNumber,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};


static xmlSecAppCmdLineParam disableErrorMsgsParam = { 
    xmlSecAppCmdLineTopicGeneral,
    "--disable-error-msgs",
    NULL,
    "--disable-error-msgs"
    "\n\tdo not print xmlsec error messages",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam printCryptoErrorMsgsParam = { 
    xmlSecAppCmdLineTopicGeneral,
    "--print-crypto-error-msgs",
    NULL,
    "--print-crypto-error-msgs"
    "\n\tprint errors stack at the end",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

/****************************************************************
 *
 * Keys Manager params
 *
 ***************************************************************/
static xmlSecAppCmdLineParam genKeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--gen-key",
    "-g",
    "--gen-key[:<name>] <keyKlass>-<keySize>"
    "\n\tgenerate new <keyKlass> key of <keySize> bits size,"
    "\n\tset the key name to <name> and add the result to keys"
    "\n\tmanager (for example, \"--gen:mykey rsa-1024\" generates"
    "\n\ta new 1024 bits RSA key and sets it's name to \"mykey\")",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam keysFileParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--keys-file",
    "-k",
    "--keys-file <file>"
    "\n\tload keys from XML file",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam privkeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--privkey-pem",
    "--privkey",
    "--privkey-pem[:<name>] <file>[,<cafile>[,<cafile>[...]]]"
    "\n\tload private key from PEM file and certificates"
    "\n\tthat verify this key",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam privkeyDerParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--privkey-der",
    NULL,
    "--privkey-der[:<name>] <file>[,<cafile>[,<cafile>[...]]]"
    "\n\tload private key from DER file and certificates"
    "\n\tthat verify this key",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pkcs8PemParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pkcs8-pem",
    "--privkey-p8-pem",
    "--pkcs8-pem[:<name>] <file>[,<cafile>[,<cafile>[...]]]"
    "\n\tload private key from PKCS8 PEM file and PEM certificates"
    "\n\tthat verify this key",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pkcs8DerParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pkcs8-der",
    "--privkey-p8-der",
    "--pkcs8-der[:<name>] <file>[,<cafile>[,<cafile>[...]]]"
    "\n\tload private key from PKCS8 DER file and DER certificates"
    "\n\tthat verify this key",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pubkeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pubkey-pem",
    "--pubkey",
    "--pubkey-pem[:<name>] <file>"
    "\n\tload public key from PEM file",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pubkeyDerParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pubkey-der",
    NULL,
    "--pubkey-der[:<name>] <file>"
    "\n\tload public key from DER file",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};


#ifndef XMLSEC_NO_AES    
static xmlSecAppCmdLineParam aeskeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--aeskey",
    NULL,
    "--aeskey[:<name>] <file>"
    "\n\tload AES key from binary file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};
#endif /* XMLSEC_NO_AES */

#ifndef XMLSEC_NO_DES   
static xmlSecAppCmdLineParam deskeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--deskey",
    NULL,
    "--deskey[:<name>] <file>"
    "\n\tload DES key from binary file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};
#endif /* XMLSEC_NO_DES */

#ifndef XMLSEC_NO_HMAC    
static xmlSecAppCmdLineParam hmackeyParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--hmackey",
    NULL,
    "--hmackey[:<name>] <file>"
    "\n\tload HMAC key from binary file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};
#endif /* XMLSEC_NO_HMAC */

static xmlSecAppCmdLineParam pwdParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pwd",
    NULL,
    "--pwd <password>"
    "\n\tthe password to use for reading keys and certs",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam enabledKeyDataParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--enabled-key-data",
    NULL,
    "--enabled-key-data <list>"
    "\n\tcomma separated list of enabled key data (list of "
    "\n\tregistered key data klasses is available with \"--list-key-data\""
    "\n\tcommand); by default, all registered key data are enabled",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam enabledRetrievalMethodUrisParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--enabled-retrieval-method-uris",
    NULL,
    "--enabled-retrieval-uris <list>"
    "\n\tcomma separated list of of the following values:"
    "\n\t\"empty\", \"same-doc\", \"local\",\"remote\" to restrict possible URI"
    "\n\tattribute values for the <dsig:RetrievalMethod> element.",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

/****************************************************************
 *
 * Common params
 *
 ***************************************************************/
static xmlSecAppCmdLineParam sessionKeyParam = { 
    xmlSecAppCmdLineTopicDSigSign | xmlSecAppCmdLineTopicEncEncrypt,
    "--session-key",
    NULL,
    "--session-key <keyKlass>-<keySize>"
    "\n\tgenerate new session <keyKlass> key of <keySize> bits size"
    "\n\t(for example, \"--session des-192\" generates a new 192 bits"
    "\n\tDES key for DES3 encryption)",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam outputParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--output",
    "-o",
    "--output <filename>"
    "\n\twrite result document to file <filename>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam nodeIdParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--node-id",
    NULL,
    "--node-id <id>"
    "\n\tset the operation start point to the node with given <id>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam nodeNameParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--node-name",
    NULL,   
    "--node-name [<namespace-uri>:]<name>"
    "\n\tset the operation start point to the first node"
    "\n\twith given <name> and <namespace> URI",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    
    
static xmlSecAppCmdLineParam nodeXPathParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--node-xpath",
    NULL,   
    "--node-xpath <expr>"
    "\n\tset the operation start point to the first node"
    "\n\tselected by the specified XPath expression",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam dtdFileParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--dtd-file",
    NULL,   
    "--dtd-file <file>"
    "\n\tload the specified file as the DTD",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam printDebugParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--print-debug",
    NULL,   
    "--print-debug"
    "\n\tprint debug information to stdout",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam printXmlDebugParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--print-xml-debug",
    NULL,   
    "--print-xml-debug"
    "\n\tprint debug information to stdout in xml format",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};    

static xmlSecAppCmdLineParam idAttrParam = { 
    xmlSecAppCmdLineTopicDSigCommon | 
    xmlSecAppCmdLineTopicEncCommon,
    "--id-attr",
    NULL,   
    "--id-attr[:<attr-name>] [<node-namespace-uri>:]<node-name>"
    "\n\tadds attributes <attr-name> (default value \"id\") from all nodes"
    "\n\twith<node-name> and namespace <node-namespace-uri> to the list of"
    "\n\tknown ID attributes; this is a hack and if you can use DTD or schema"
    "\n\tto declare ID attributes instead (see \"--dtd-file\" option),"
    "\n\tI don't know what else might be broken in your application when"
    "\n\tyou use this hack",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};    


/****************************************************************
 *
 * Common dsig params
 *
 ***************************************************************/
#ifndef XMLSEC_NO_XMLDSIG
static xmlSecAppCmdLineParam ignoreManifestsParam = { 
    xmlSecAppCmdLineTopicDSigCommon,
    "--ignore-manifests",
    NULL,
    "--ignore-manifests"
    "\n\tdo not process <dsig:Manifest> elements",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam storeReferencesParam = { 
    xmlSecAppCmdLineTopicDSigCommon,
    "--store-references",
    NULL,
    "--store-references"
    "\n\tstore and print the result of <dsig:Reference/> element processing"
    "\n\tjust before calculating digest",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam storeSignaturesParam = { 
    xmlSecAppCmdLineTopicDSigCommon,
    "--store-signatures",
    NULL,
    "--store-signatures"
    "\n\tstore and print the result of <dsig:Signature> processing"
    "\n\tjust before calculating signature",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam enabledRefUrisParam = { 
    xmlSecAppCmdLineTopicDSigCommon,
    "--enabled-reference-uris",
    NULL,
    "--enabled-reference-uris <list>"
    "\n\tcomma separated list of of the following values:"
    "\n\t\"empty\", \"same-doc\", \"local\",\"remote\" to restrict possible URI"
    "\n\tattribute values for the <dsig:Reference> element",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam enableVisa3DHackParam = { 
    xmlSecAppCmdLineTopicDSigCommon,
    "--enable-visa3d-hack",
    NULL,
    "--enable-visa3d-hack"
    "\n\tenables Visa3D protocol specific hack for URI attributes processing"
    "\n\twhen we are trying not to use XPath/XPointer engine; this is a hack"
    "\n\tand I don't know what else might be broken in your application when"
    "\n\tyou use it (also check \"--id-attr\" option because you might need it)",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

#endif /* XMLSEC_NO_XMLDSIG */

/****************************************************************
 *
 * Enc params
 *
 ***************************************************************/
#ifndef XMLSEC_NO_XMLENC
static xmlSecAppCmdLineParam enabledCipherRefUrisParam = { 
    xmlSecAppCmdLineTopicEncCommon,
    "--enabled-cipher-reference-uris",
    NULL,
    "--enabled-cipher-reference-uris <list>"
    "\n\tcomma separated list of of the following values:"
    "\n\t\"empty\", \"same-doc\", \"local\",\"remote\" to restrict possible URI"
    "\n\tattribute values for the <enc:CipherReference> element",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam binaryDataParam = { 
    xmlSecAppCmdLineTopicEncEncrypt,
    "--binary-data",
    "--binary",
    "--binary-data <file>"
    "\n\tbinary <file> to encrypt",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam xmlDataParam = { 
    xmlSecAppCmdLineTopicEncEncrypt,
    "--xml-data",
    NULL,
    "--xml-data <file>"
    "\n\tXML <file> to encrypt",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};
#endif /* XMLSEC_NO_XMLENC */


/****************************************************************
 *
 * X509 params
 *
 ***************************************************************/
#ifndef XMLSEC_NO_X509    
static xmlSecAppCmdLineParam pkcs12Param = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pkcs12",
    NULL,
    "--pkcs12[:<name>] <file>"
    "\n\tload load private key from pkcs12 file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pubkeyCertParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pubkey-cert-pem",
    "--pubkey-cert",
    "--pubkey-cert-pem[:<name>] <file>"
    "\n\tload public key from PEM cert file",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam pubkeyCertDerParam = { 
    xmlSecAppCmdLineTopicKeysMngr,
    "--pubkey-cert-der",
    NULL,
    "--pubkey-cert-der[:<name>] <file>"
    "\n\tload public key from DER cert file",
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamFlagParamNameValue | xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam trustedParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--trusted-pem",
    "--trusted",
    "--trusted-pem <file>"
    "\n\tload trusted (root) certificate from PEM file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam untrustedParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--untrusted-pem",
    "--untrusted",
    "--untrusted-pem <file>"
    "\n\tload untrusted certificate from PEM file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam trustedDerParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--trusted-der",
    NULL,
    "--trusted-der <file>"
    "\n\tload trusted (root) certificate from DER file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam untrustedDerParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--untrusted-der",
    NULL,
    "--untrusted-der <file>"
    "\n\tload untrusted certificate from DER file <file>",
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamFlagMultipleValues,
    NULL
};

static xmlSecAppCmdLineParam verificationTimeParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--verification-time",
    NULL,
    "--verification-time <time>"
    "\n\tthe local time in \"YYYY-MM-DD HH:MM:SS\" format"
    "\n\tused certificates verification",
    xmlSecAppCmdLineParamTypeTime,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam depthParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--depth",
    NULL,    
    "--depth <number>"
    "\n\tmaximum certificates chain depth",
    xmlSecAppCmdLineParamTypeNumber,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};

static xmlSecAppCmdLineParam X509SkipStrictChecksParam = { 
    xmlSecAppCmdLineTopicX509Certs,
    "--X509-skip-strict-checks",
    NULL,    
    "--X509-skip-strict-checks"
    "\n\tskip strict checking of X509 data",
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamFlagNone,
    NULL
};
#endif /* XMLSEC_NO_X509 */    

static xmlSecAppCmdLineParamPtr parameters[] = {
    /* common dsig params */
#ifndef XMLSEC_NO_XMLDSIG
    &ignoreManifestsParam,
    &storeReferencesParam,
    &storeSignaturesParam,
    &enabledRefUrisParam,
    &enableVisa3DHackParam,
#endif /* XMLSEC_NO_XMLDSIG */

    /* enc params */
#ifndef XMLSEC_NO_XMLENC
    &binaryDataParam,
    &xmlDataParam,
    &enabledCipherRefUrisParam,
#endif /* XMLSEC_NO_XMLENC */
             
    /* common dsig and enc parameters */
    &sessionKeyParam,    
    &outputParam,
    &printDebugParam,
    &printXmlDebugParam,    
    &dtdFileParam,
    &nodeIdParam,
    &nodeNameParam,
    &nodeXPathParam,
    &idAttrParam,
    
    /* Keys Manager params */
    &enabledKeyDataParam,
    &enabledRetrievalMethodUrisParam,
    &genKeyParam,
    &keysFileParam,
    &privkeyParam,
    &privkeyDerParam,
    &pkcs8PemParam,
    &pkcs8DerParam,
    &pubkeyParam,
    &pubkeyDerParam,
#ifndef XMLSEC_NO_AES    
    &aeskeyParam,
#endif  /* XMLSEC_NO_AES */    
#ifndef XMLSEC_NO_DES
    &deskeyParam,
#endif  /* XMLSEC_NO_DES */    
#ifndef XMLSEC_NO_HMAC    
    &hmackeyParam,
#endif  /* XMLSEC_NO_HMAC */    
    &pwdParam,
#ifndef XMLSEC_NO_X509
    &pkcs12Param,
    &pubkeyCertParam,
    &pubkeyCertDerParam,
    &trustedParam,
    &untrustedParam,
    &trustedDerParam,
    &untrustedDerParam,
    &verificationTimeParam,
    &depthParam,    
    &X509SkipStrictChecksParam,    
#endif /* XMLSEC_NO_X509 */    
    
    /* General configuration params */
    &cryptoParam,
    &cryptoConfigParam,
    &repeatParam,
    &disableErrorMsgsParam,
    &printCryptoErrorMsgsParam,
    &helpParam,
        
    /* MUST be the last one */
    NULL
};

typedef enum {
    xmlSecAppCommandUnknown = 0,
    xmlSecAppCommandHelp,
    xmlSecAppCommandListKeyData,
    xmlSecAppCommandCheckKeyData,
    xmlSecAppCommandListTransforms,    
    xmlSecAppCommandCheckTransforms,    
    xmlSecAppCommandVersion,
    xmlSecAppCommandKeys,
    xmlSecAppCommandSign,
    xmlSecAppCommandVerify,
    xmlSecAppCommandSignTmpl,
    xmlSecAppCommandEncrypt,
    xmlSecAppCommandDecrypt,
    xmlSecAppCommandEncryptTmpl
} xmlSecAppCommand;

typedef struct _xmlSecAppXmlData                                xmlSecAppXmlData,
                                                                *xmlSecAppXmlDataPtr;
struct _xmlSecAppXmlData {
    xmlDocPtr   doc;
    xmlDtdPtr   dtd;
    xmlNodePtr  startNode;
};

static xmlSecAppXmlDataPtr      xmlSecAppXmlDataCreate          (const char* filename,
                                                                 const xmlChar* defStartNodeName,
                                                                 const xmlChar* defStartNodeNs);
static void                     xmlSecAppXmlDataDestroy         (xmlSecAppXmlDataPtr data);


static xmlSecAppCommand         xmlSecAppParseCommand           (const char* cmd, 
                                                                 xmlSecAppCmdLineParamTopic* topics,
                                                                 xmlSecAppCommand* subCommand);
static void                     xmlSecAppPrintHelp              (xmlSecAppCommand command, 
                                                                 xmlSecAppCmdLineParamTopic topics);
#define                         xmlSecAppPrintUsage()           xmlSecAppPrintHelp(xmlSecAppCommandUnknown, 0)
static int                      xmlSecAppInit                   (void);
static void                     xmlSecAppShutdown               (void);
static int                      xmlSecAppLoadKeys               (void);
static int                      xmlSecAppPrepareKeyInfoReadCtx  (xmlSecKeyInfoCtxPtr ctx);

#ifndef XMLSEC_NO_XMLDSIG
static int                      xmlSecAppSignFile               (const char* filename);
static int                      xmlSecAppVerifyFile             (const char* filename);
#ifndef XMLSEC_NO_TMPL_TEST
static int                      xmlSecAppSignTmpl               (void);
#endif /* XMLSEC_NO_TMPL_TEST */
static int                      xmlSecAppPrepareDSigCtx         (xmlSecDSigCtxPtr dsigCtx);
static void                     xmlSecAppPrintDSigCtx           (xmlSecDSigCtxPtr dsigCtx);
#endif /* XMLSEC_NO_XMLDSIG */

#ifndef XMLSEC_NO_XMLENC
static int                      xmlSecAppEncryptFile            (const char* filename);
static int                      xmlSecAppDecryptFile            (const char* filename);
#ifndef XMLSEC_NO_TMPL_TEST
static int                      xmlSecAppEncryptTmpl            (void);
#endif /* XMLSEC_NO_TMPL_TEST */
static int                      xmlSecAppPrepareEncCtx          (xmlSecEncCtxPtr encCtx);
static void                     xmlSecAppPrintEncCtx            (xmlSecEncCtxPtr encCtx);
#endif /* XMLSEC_NO_XMLENC */

static void                     xmlSecAppListKeyData            (void);
static int                      xmlSecAppCheckKeyData       (const char * name);
static void                     xmlSecAppListTransforms         (void);
static int                      xmlSecAppCheckTransform     (const char * name);

static xmlSecTransformUriType   xmlSecAppGetUriType             (const char* string);
static FILE*                    xmlSecAppOpenFile               (const char* filename);
static void                     xmlSecAppCloseFile              (FILE* file);
static int                      xmlSecAppWriteResult            (xmlDocPtr doc,
                                                                 xmlSecBufferPtr buffer);
static int                      xmlSecAppAddIDAttr              (xmlNodePtr cur,
                                                                 const xmlChar* attr,
                                                                 const xmlChar* node,
                                                                 const xmlChar* nsHref);                                                                 

xmlSecKeysMngrPtr gKeysMngr = NULL;
int repeats = 1;
int print_debug = 0;
clock_t total_time = 0;
const char* xmlsec_crypto = NULL;
const char* tmp = NULL;

int main(int argc, const char **argv) {
    xmlSecAppCmdLineParamTopic cmdLineTopics;
    xmlSecAppCommand command, subCommand;
    int pos, i;
    int res = 1;

    /* read the command (first argument) */
    if(argc < 2) {
        xmlSecAppPrintUsage();
        goto fail;
    }
    command = xmlSecAppParseCommand(argv[1], &cmdLineTopics, &subCommand);
    if(command == xmlSecAppCommandUnknown) {
        fprintf(stderr, "Error: unknown command \"%s\"\n", argv[1]);
        xmlSecAppPrintUsage();
        goto fail;
    }
    
    /* do as much as we can w/o initialization */
    if(command == xmlSecAppCommandHelp) {
        xmlSecAppPrintHelp(subCommand, cmdLineTopics);
        goto success;
    } else if(command == xmlSecAppCommandVersion) {
        fprintf(stdout, "%s %s (%s)\n", PACKAGE, XMLSEC_VERSION, xmlSecGetDefaultCrypto());
        goto success;
    }
    
    /* parse command line */
    pos = xmlSecAppCmdLineParamsListParse(parameters, cmdLineTopics, argv, argc, 2);
    if(pos < 0) {
        fprintf(stderr, "Error: invalid parameters\n");
        xmlSecAppPrintUsage();
        goto fail;
    }
    
    /* is it a help request? */    
    if(xmlSecAppCmdLineParamIsSet(&helpParam)) {
        xmlSecAppPrintHelp(command, cmdLineTopics);
        goto success;
    }
    
    /* we need to have some files at the end */
    switch(command) {
        case xmlSecAppCommandKeys:
        case xmlSecAppCommandSign:
        case xmlSecAppCommandVerify:
        case xmlSecAppCommandEncrypt:
        case xmlSecAppCommandDecrypt:
            if(pos >= argc) {
                fprintf(stderr, "Error: <file> parameter is required for this command\n");
                xmlSecAppPrintUsage();
                goto fail;
            }
            break;
        default:
            break;
    }

    /* now init the xmlsec and all other libs */
    /* ignore "--crypto" if we don't have dynamic loading */
    tmp = xmlSecAppCmdLineParamGetString(&cryptoParam);
#if !defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
    if((tmp != NULL) && (strcmp(tmp, "default") != 0)) {
        xmlsec_crypto = tmp;
    }
#else /* !defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */
    if((tmp != NULL) && (xmlStrcmp(BAD_CAST tmp, xmlSecGetDefaultCrypto()) != 0)) {
        fprintf(stderr, "Error: dynaimc crypto libraries loading is disabled and the only available crypto library is '%s'\n", xmlSecGetDefaultCrypto());
        xmlSecAppPrintUsage();
        goto fail;
    }
#endif /* !defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

    if(xmlSecAppInit() < 0) {
        fprintf(stderr, "Error: initialization failed\n");
        xmlSecAppPrintUsage();
        goto fail;
    }    
    
    /* load keys */
    if(xmlSecAppLoadKeys() < 0) {
        fprintf(stderr, "Error: keys manager creation failed\n");
        xmlSecAppPrintUsage();
        goto fail;
    }
    
    /* get the "repeats" number */
    if(xmlSecAppCmdLineParamIsSet(&repeatParam) && 
       (xmlSecAppCmdLineParamGetInt(&repeatParam, 1) > 0)) {
       
        repeats = xmlSecAppCmdLineParamGetInt(&repeatParam, 1);
    }

    /* execute requested number of times */
    for(; repeats > 0; --repeats) {
        switch(command) {
        case xmlSecAppCommandListKeyData:
            xmlSecAppListKeyData();
            break;
        case xmlSecAppCommandCheckKeyData:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppCheckKeyData(argv[i]) < 0) {
                    fprintf(stderr, "Error: key data \"%s\" not found\n", argv[i]);
                    goto fail;
                } else {
                    fprintf(stdout, "Key data \"%s\" found\n", argv[i]);
                }
            }
            break;
        case xmlSecAppCommandListTransforms:
            xmlSecAppListTransforms();
            break;          
        case xmlSecAppCommandCheckTransforms:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppCheckTransform(argv[i]) < 0) {
                    fprintf(stderr, "Error: transform \"%s\" not found\n", argv[i]);
                    goto fail;
                } else {
                    fprintf(stdout, "Transforms \"%s\" found\n", argv[i]);
                }
            }
            break;          
        case xmlSecAppCommandKeys:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppCryptoSimpleKeysMngrSave(gKeysMngr, argv[i], xmlSecKeyDataTypeAny) < 0) {
                    fprintf(stderr, "Error: failed to save keys to file \"%s\"\n", argv[i]);
                    goto fail;
                }
            }
            break;
#ifndef XMLSEC_NO_XMLDSIG
        case xmlSecAppCommandSign:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppSignFile(argv[i]) < 0) {
                    fprintf(stderr, "Error: failed to sign file \"%s\"\n", argv[i]);
                    goto fail;
                }
            }
            break;
        case xmlSecAppCommandVerify:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppVerifyFile(argv[i]) < 0) {
                    fprintf(stderr, "Error: failed to verify file \"%s\"\n", argv[i]);
                    goto fail;
                }
            }
            break;
#ifndef XMLSEC_NO_TMPL_TEST
        case xmlSecAppCommandSignTmpl:
            if(xmlSecAppSignTmpl() < 0) {
                fprintf(stderr, "Error: failed to create and sign template\n");
                goto fail;
            }
            break;
#endif /* XMLSEC_NO_TMPL_TEST */
#endif /* XMLSEC_NO_XMLDSIG */

#ifndef XMLSEC_NO_XMLENC
        case xmlSecAppCommandEncrypt:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppEncryptFile(argv[i]) < 0) {
                    fprintf(stderr, "Error: failed to encrypt file with template \"%s\"\n", argv[i]);
                    goto fail;
                }
            }
            break;
        case xmlSecAppCommandDecrypt:
            for(i = pos; i < argc; ++i) {
                if(xmlSecAppDecryptFile(argv[i]) < 0) {
                    fprintf(stderr, "Error: failed to decrypt file \"%s\"\n", argv[i]);
                    goto fail;
                }
            }
            break;
#ifndef XMLSEC_NO_TMPL_TEST
        case xmlSecAppCommandEncryptTmpl:
            if(xmlSecAppEncryptTmpl() < 0) {
                fprintf(stderr, "Error: failed to create and encrypt template\n");
                goto fail;
            }
            break;
#endif /* XMLSEC_NO_TMPL_TEST */
#endif /* XMLSEC_NO_XMLENC */

        default:
            fprintf(stderr, "Error: invalid command %d\n", command);
            xmlSecAppPrintUsage();
            goto fail;
        }
    }

    /* print perf stats results */
    if(xmlSecAppCmdLineParamIsSet(&repeatParam) && 
       (xmlSecAppCmdLineParamGetInt(&repeatParam, 1) > 0)) {
       
        repeats = xmlSecAppCmdLineParamGetInt(&repeatParam, 1);
        fprintf(stderr, "Executed %d tests in %ld msec\n", repeats, (1000 * total_time) / CLOCKS_PER_SEC);    
    }

    goto success;
success:
    res = 0;
fail:
    if(gKeysMngr != NULL) {
        xmlSecKeysMngrDestroy(gKeysMngr);
        gKeysMngr = NULL;
    }
    xmlSecAppShutdown();
    xmlSecAppCmdLineParamsListClean(parameters);
    return(res);
}


#ifndef XMLSEC_NO_XMLDSIG
static int 
xmlSecAppSignFile(const char* filename) {
    xmlSecAppXmlDataPtr data = NULL;
    xmlSecDSigCtx dsigCtx;
    clock_t start_time;
    int res = -1;
    
    if(filename == NULL) {
        return(-1);
    }

    if(xmlSecDSigCtxInitialize(&dsigCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: dsig context initialization failed\n");
        return(-1);
    }

    if(xmlSecAppPrepareDSigCtx(&dsigCtx) < 0) {
        fprintf(stderr, "Error: dsig context preparation failed\n");
        goto done;
    }

    /* parse template and select start node */
    data = xmlSecAppXmlDataCreate(filename, xmlSecNodeSignature, xmlSecDSigNs);
    if(data == NULL) {
        fprintf(stderr, "Error: failed to load template \"%s\"\n", filename);
        goto done;
    }

    
    /* sign */
    start_time = clock();
    if(xmlSecDSigCtxSign(&dsigCtx, data->startNode) < 0) {
        fprintf(stderr,"Error: signature failed \n");
        goto done;
    }
    total_time += clock() - start_time;    

    if(repeats <= 1) { 
        FILE* f;
        
        f = xmlSecAppOpenFile(xmlSecAppCmdLineParamGetString(&outputParam));
        if(f == NULL) {
            fprintf(stderr,"Error: failed to open output file \"%s\"\n",
                    xmlSecAppCmdLineParamGetString(&outputParam));
            goto done;
        }
        xmlDocDump(f, data->doc);
        xmlSecAppCloseFile(f);
    }

    res = 0;
done:
    /* print debug info if requested */
    if(repeats <= 1) {
        xmlSecAppPrintDSigCtx(&dsigCtx);
    }
    xmlSecDSigCtxFinalize(&dsigCtx);
    if(data != NULL) {
        xmlSecAppXmlDataDestroy(data);
    }
    return(res);
}

static int 
xmlSecAppVerifyFile(const char* filename) {
    xmlSecAppXmlDataPtr data = NULL;
    xmlSecDSigCtx dsigCtx;
    clock_t start_time;
    int res = -1;
    
    if(filename == NULL) {
        return(-1);
    }

    if(xmlSecDSigCtxInitialize(&dsigCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: dsig context initialization failed\n");
        return(-1);
    }
    if(xmlSecAppPrepareDSigCtx(&dsigCtx) < 0) {
        fprintf(stderr, "Error: dsig context preparation failed\n");
        goto done;
    }
    
    /* parse template and select start node */
    data = xmlSecAppXmlDataCreate(filename, xmlSecNodeSignature, xmlSecDSigNs);
    if(data == NULL) {
        fprintf(stderr, "Error: failed to load document \"%s\"\n", filename);
        goto done;
    }

    /* sign */
    start_time = clock();
    if(xmlSecDSigCtxVerify(&dsigCtx, data->startNode) < 0) {
        fprintf(stderr,"Error: signature failed \n");
        goto done;
    }
    total_time += clock() - start_time;    

    if((repeats <= 1) && (dsigCtx.status != xmlSecDSigStatusSucceeded)){ 
        /* return an error if signature does not match */
        goto done;
    }

    res = 0;
done:
    /* print debug info if requested */
    if(repeats <= 1) {
        xmlSecDSigReferenceCtxPtr dsigRefCtx;
        xmlSecSize good, i, size;
        FILE* f;
        
        f = xmlSecAppOpenFile(xmlSecAppCmdLineParamGetString(&outputParam));
        if(f == NULL) {
            fprintf(stderr,"Error: failed to open output file \"%s\"\n",
                    xmlSecAppCmdLineParamGetString(&outputParam));
            goto done;
        }
        xmlSecAppCloseFile(f);

        switch(dsigCtx.status) {
            case xmlSecDSigStatusUnknown:
                fprintf(stderr, "ERROR\n");
                break;
            case xmlSecDSigStatusSucceeded:
                fprintf(stderr, "OK\n");
                break;
            case xmlSecDSigStatusInvalid:
                fprintf(stderr, "FAIL\n");
                break;
        }    

        /* print stats about # of good/bad references/manifests */
        size = xmlSecPtrListGetSize(&(dsigCtx.signedInfoReferences));
        for(i = good = 0; i < size; ++i) {
            dsigRefCtx = (xmlSecDSigReferenceCtxPtr)xmlSecPtrListGetItem(&(dsigCtx.signedInfoReferences), i);
            if(dsigRefCtx == NULL) {
                fprintf(stderr,"Error: reference ctx is null\n");
                goto done;
            }
            if(dsigRefCtx->status == xmlSecDSigStatusSucceeded) {
                ++good;
            }
        }
        fprintf(stderr, "SignedInfo References (ok/all): %d/%d\n", good, size);

        size = xmlSecPtrListGetSize(&(dsigCtx.manifestReferences));
        for(i = good = 0; i < size; ++i) {
            dsigRefCtx = (xmlSecDSigReferenceCtxPtr)xmlSecPtrListGetItem(&(dsigCtx.manifestReferences), i);
            if(dsigRefCtx == NULL) {
                fprintf(stderr,"Error: reference ctx is null\n");
                goto done;
            }
            if(dsigRefCtx->status == xmlSecDSigStatusSucceeded) {
                ++good;
            }
        }
        fprintf(stderr, "Manifests References (ok/all): %d/%d\n", good, size);

        xmlSecAppPrintDSigCtx(&dsigCtx);
    }
    xmlSecDSigCtxFinalize(&dsigCtx);
    if(data != NULL) {
        xmlSecAppXmlDataDestroy(data);
    }
    return(res);
}

#ifndef XMLSEC_NO_TMPL_TEST
static int 
xmlSecAppSignTmpl(void) {
    xmlDocPtr doc = NULL;
    xmlNodePtr cur;
    xmlSecDSigCtx dsigCtx;
    clock_t start_time;
    int res = -1;
        
    if(xmlSecDSigCtxInitialize(&dsigCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: dsig context initialization failed\n");
        return(-1);
    }
    if(xmlSecAppPrepareDSigCtx(&dsigCtx) < 0) {
        fprintf(stderr, "Error: dsig context preparation failed\n");
        goto done;
    }
    
    /* prepare template */
    doc = xmlNewDoc(BAD_CAST "1.0");
    if(doc == NULL) {
        fprintf(stderr, "Error: failed to create doc\n");
        goto done;
    }
    
    cur = xmlSecTmplSignatureCreate(doc, xmlSecTransformInclC14NId,
                                    xmlSecTransformHmacSha1Id, NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to create Signature node\n");
        goto done;
    }
    xmlDocSetRootElement(doc, cur);

    /* set hmac signature length */
    cur = xmlSecTmplSignatureGetSignMethodNode(xmlDocGetRootElement(doc));
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to find SignatureMethod node\n");
        goto done;
    }
    if(xmlSecTmplTransformAddHmacOutputLength(cur, 93) < 0) {
        fprintf(stderr, "Error: failed to set hmac length\n");
        goto done;
    }
    
    cur = xmlSecTmplSignatureAddReference(xmlDocGetRootElement(doc), 
                                    xmlSecTransformSha1Id, 
                                    BAD_CAST "ref1", NULL, NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to add Reference node\n");
        goto done;
    }
    
    cur = xmlSecTmplReferenceAddTransform(cur, xmlSecTransformXPath2Id);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to add XPath transform\n");
        goto done;
    }
    
    if(xmlSecTmplTransformAddXPath2(cur, BAD_CAST "intersect", 
                                    BAD_CAST "//*[@Id='object1']", NULL) < 0) {
        fprintf(stderr, "Error: failed to set XPath expression\n");
        goto done;    
    }
    
    cur = xmlSecTmplSignatureAddObject(xmlDocGetRootElement(doc), 
                                    BAD_CAST "object1", NULL, NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to add Object node\n");
        goto done;
    }
    xmlNodeSetContent(cur, BAD_CAST "This is signed data");

    /* add key information */
    cur = xmlSecTmplSignatureEnsureKeyInfo(xmlDocGetRootElement(doc), NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to add KeyInfo node\n");
        goto done;      
    }
    if(xmlSecTmplKeyInfoAddKeyName(cur, NULL) == NULL) {
        fprintf(stderr, "Error: failed to add KeyName node\n");
        goto done;      
    }
    
    /* sign */
    start_time = clock();
    if(xmlSecDSigCtxSign(&dsigCtx, xmlDocGetRootElement(doc)) < 0) {
        fprintf(stderr,"Error: signature failed \n");
        goto done;
    }
    total_time += clock() - start_time;    

    if(repeats <= 1) { 
        FILE* f;
        
        f = xmlSecAppOpenFile(xmlSecAppCmdLineParamGetString(&outputParam));
        if(f == NULL) {
            fprintf(stderr,"Error: failed to open output file \"%s\"\n",
                    xmlSecAppCmdLineParamGetString(&outputParam));
            goto done;
        }
        xmlDocDump(f, doc);
        xmlSecAppCloseFile(f);
    }

    res = 0;
done:
    /* print debug info if requested */
    if(repeats <= 1) {
        xmlSecAppPrintDSigCtx(&dsigCtx);
    }
    xmlSecDSigCtxFinalize(&dsigCtx);
    if(doc != NULL) {
        xmlFreeDoc(doc);
    }
    return(res);
}
#endif /* XMLSEC_NO_TMPL_TEST */

static int
xmlSecAppPrepareDSigCtx(xmlSecDSigCtxPtr dsigCtx) {
    if(dsigCtx == NULL) {
        fprintf(stderr, "Error: dsig context is null\n");
        return(-1);
    }

    /* set key info params */
    if(xmlSecAppPrepareKeyInfoReadCtx(&(dsigCtx->keyInfoReadCtx)) < 0) {
        fprintf(stderr, "Error: failed to prepare key info context\n");
        return(-1);
    }

    if(xmlSecAppCmdLineParamGetString(&sessionKeyParam) != NULL) {
        dsigCtx->signKey = xmlSecAppCryptoKeyGenerate(xmlSecAppCmdLineParamGetString(&sessionKeyParam),
                                NULL, xmlSecKeyDataTypeSession);
        if(dsigCtx->signKey == NULL) {
            fprintf(stderr, "Error: failed to generate a session key \"%s\"\n",
                    xmlSecAppCmdLineParamGetString(&sessionKeyParam));
            return(-1);
        }
    }

    /* set dsig params */
    if(xmlSecAppCmdLineParamIsSet(&ignoreManifestsParam)) {
        dsigCtx->flags |= XMLSEC_DSIG_FLAGS_IGNORE_MANIFESTS; 
    }
    if(xmlSecAppCmdLineParamIsSet(&storeReferencesParam)) {
        dsigCtx->flags |= XMLSEC_DSIG_FLAGS_STORE_SIGNEDINFO_REFERENCES |
                          XMLSEC_DSIG_FLAGS_STORE_MANIFEST_REFERENCES; 
        print_debug = 1;
    }
    if(xmlSecAppCmdLineParamIsSet(&storeSignaturesParam)) {
        dsigCtx->flags |= XMLSEC_DSIG_FLAGS_STORE_SIGNATURE; 
        print_debug = 1;
    }
    if(xmlSecAppCmdLineParamIsSet(&enableVisa3DHackParam)) {
        dsigCtx->flags |= XMLSEC_DSIG_FLAGS_USE_VISA3D_HACK; 
    }
    
    if(xmlSecAppCmdLineParamGetStringList(&enabledRefUrisParam) != NULL) {
        dsigCtx->enabledReferenceUris = xmlSecAppGetUriType(
                    xmlSecAppCmdLineParamGetStringList(&enabledRefUrisParam));
        if(dsigCtx->enabledReferenceUris == xmlSecTransformUriTypeNone) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n",
                    xmlSecAppCmdLineParamGetStringList(&enabledRefUrisParam));
            return(-1);
        }
    }

    return(0);
}

static void
xmlSecAppPrintDSigCtx(xmlSecDSigCtxPtr dsigCtx) { 
    if(dsigCtx == NULL) {
        return;
    }

    if(xmlSecAppCmdLineParamIsSet(&printDebugParam) || xmlSecAppCmdLineParamIsSet(&printXmlDebugParam)) { 
        print_debug = 0;
    }
    
    /* print debug info if requested */
    if((print_debug != 0) || xmlSecAppCmdLineParamIsSet(&printDebugParam)) {
        xmlSecDSigCtxDebugDump(dsigCtx, stdout);
    }
    
    if(xmlSecAppCmdLineParamIsSet(&printXmlDebugParam)) {          
        xmlSecDSigCtxDebugXmlDump(dsigCtx, stdout);
    }
}

#endif /* XMLSEC_NO_XMLDSIG */

#ifndef XMLSEC_NO_XMLENC
static int 
xmlSecAppEncryptFile(const char* filename) {
    xmlSecAppXmlDataPtr data = NULL;
    xmlSecEncCtx encCtx;
    xmlDocPtr doc = NULL;
    xmlNodePtr startTmplNode;
    clock_t start_time;
    int res = -1;

    if(filename == NULL) {
        return(-1);
    }

    if(xmlSecEncCtxInitialize(&encCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: enc context initialization failed\n");
        return(-1);
    }
    if(xmlSecAppPrepareEncCtx(&encCtx) < 0) {
        fprintf(stderr, "Error: enc context preparation failed\n");
        goto done;
    }

    /* parse doc and find template node */
    doc = xmlSecParseFile(filename);
    if(doc == NULL) {
        fprintf(stderr, "Error: failed to parse xml file \"%s\"\n", 
                filename);
        goto done;
    }
    startTmplNode = xmlSecFindNode(xmlDocGetRootElement(doc), xmlSecNodeEncryptedData, xmlSecEncNs);
    if(startTmplNode == NULL) {
        fprintf(stderr, "Error: failed to find default node with name=\"%s\"\n", 
                xmlSecNodeEncryptedData);
        goto done;
    }

    if(xmlSecAppCmdLineParamGetString(&binaryDataParam) != NULL) {
        /* encrypt */
        start_time = clock();            
        if(xmlSecEncCtxUriEncrypt(&encCtx, startTmplNode, BAD_CAST xmlSecAppCmdLineParamGetString(&binaryDataParam)) < 0) {
            fprintf(stderr, "Error: failed to encrypt file \"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&binaryDataParam));
            goto done;
        }
        total_time += clock() - start_time;    
    } else if(xmlSecAppCmdLineParamGetString(&xmlDataParam) != NULL) {
        /* parse file and select node for encryption */
        data = xmlSecAppXmlDataCreate(xmlSecAppCmdLineParamGetString(&xmlDataParam), NULL, NULL);
        if(data == NULL) {
            fprintf(stderr, "Error: failed to load file \"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&xmlDataParam));
            goto done;
        }

        /* encrypt */
        start_time = clock();            
        if(xmlSecEncCtxXmlEncrypt(&encCtx, startTmplNode, data->startNode) < 0) {
            fprintf(stderr, "Error: failed to encrypt xml file \"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&xmlDataParam));
            goto done;
        }
        total_time += clock() - start_time;    
    } else {
        fprintf(stderr, "Error: encryption data not specified (use \"--xml\" or \"--binary\" options)\n");
        goto done;
    }
    
    /* print out result only once per execution */
    if(repeats <= 1) {
        if(encCtx.resultReplaced) {
            if(xmlSecAppWriteResult((data != NULL) ? data->doc : doc, NULL) < 0) {
                goto done;
            }
        } else {
            if(xmlSecAppWriteResult(NULL, encCtx.result) < 0) {
                goto done;
            }
        }       
    }
    res = 0;    

done:
    /* print debug info if requested */
    if(repeats <= 1) {
        xmlSecAppPrintEncCtx(&encCtx);
    }
    xmlSecEncCtxFinalize(&encCtx);

    if(data != NULL) {
        xmlSecAppXmlDataDestroy(data);
    }
    if(doc != NULL) {
        xmlFreeDoc(doc);
    }
    return(res);
}

static int 
xmlSecAppDecryptFile(const char* filename) {
    xmlSecAppXmlDataPtr data = NULL;
    xmlSecEncCtx encCtx;
    clock_t start_time;
    int res = -1;

    if(filename == NULL) {
        return(-1);
    }

    if(xmlSecEncCtxInitialize(&encCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: enc context initialization failed\n");
        return(-1);
    }
    if(xmlSecAppPrepareEncCtx(&encCtx) < 0) {
        fprintf(stderr, "Error: enc context preparation failed\n");
        goto done;
    }

    /* parse template and select start node */
    data = xmlSecAppXmlDataCreate(filename, xmlSecNodeEncryptedData, xmlSecEncNs);
    if(data == NULL) {
        fprintf(stderr, "Error: failed to load template \"%s\"\n", filename);
        goto done;
    }

    start_time = clock();  
    if(xmlSecEncCtxDecrypt(&encCtx, data->startNode) < 0) {
        fprintf(stderr, "Error: failed to decrypt file\n");
        goto done;
    }
    total_time += clock() - start_time;    
    
    /* print out result only once per execution */
    if(repeats <= 1) {
        if(encCtx.resultReplaced) {
            if(xmlSecAppWriteResult(data->doc, NULL) < 0) {
                goto done;
            }
        } else {
            if(xmlSecAppWriteResult(NULL, encCtx.result) < 0) {
                goto done;
            }
        }       
    }
    res = 0;    

done:
    /* print debug info if requested */
    if(repeats <= 1) { 
        xmlSecAppPrintEncCtx(&encCtx);
    }
    xmlSecEncCtxFinalize(&encCtx);

    if(data != NULL) {
        xmlSecAppXmlDataDestroy(data);
    }
    return(res);
}

#ifndef XMLSEC_NO_TMPL_TEST
static int 
xmlSecAppEncryptTmpl(void) {
    const char* data = "Hello, World!";
    xmlSecEncCtx encCtx;
    xmlDocPtr doc = NULL;
    xmlNodePtr cur;
    clock_t start_time;
    int res = -1;

    if(xmlSecEncCtxInitialize(&encCtx, gKeysMngr) < 0) {
        fprintf(stderr, "Error: enc context initialization failed\n");
        return(-1);
    }
    if(xmlSecAppPrepareEncCtx(&encCtx) < 0) {
        fprintf(stderr, "Error: enc context preparation failed\n");
        goto done;
    }

    /* prepare template */
    doc = xmlNewDoc(BAD_CAST "1.0");
    if(doc == NULL) {
        fprintf(stderr, "Error: failed to create doc\n");
        goto done;
    }

    cur = xmlSecTmplEncDataCreate(doc, xmlSecTransformDes3CbcId, 
                                  NULL, NULL, NULL, NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to encryption template\n");
        goto done;      
    }
    xmlDocSetRootElement(doc, cur);

    if(xmlSecTmplEncDataEnsureCipherValue(xmlDocGetRootElement(doc)) == NULL) {
        fprintf(stderr, "Error: failed to add CipherValue node\n");
        goto done;      
    }

    /* add key information */
    cur = xmlSecTmplEncDataEnsureKeyInfo(xmlDocGetRootElement(doc), NULL);
    if(cur == NULL) {
        fprintf(stderr, "Error: failed to add KeyInfo node\n");
        goto done;      
    }
    if(xmlSecTmplKeyInfoAddKeyName(cur, NULL) == NULL) {
        fprintf(stderr, "Error: failed to add KeyName node\n");
        goto done;      
    }

    /* encrypt */
    start_time = clock();            
    if(xmlSecEncCtxBinaryEncrypt(&encCtx, xmlDocGetRootElement(doc), 
                                (const xmlSecByte*)data, strlen(data)) < 0) {
        fprintf(stderr, "Error: failed to encrypt data\n");
        goto done;      
    }
    total_time += clock() - start_time;    
    
    /* print out result only once per execution */
    if(repeats <= 1) {
        if(encCtx.resultReplaced) {
            if(xmlSecAppWriteResult(doc, NULL) < 0) {
                goto done;
            }
        } else {
            if(xmlSecAppWriteResult(NULL, encCtx.result) < 0) {
                goto done;
            }
        }       
    }
    res = 0;    

done:
    /* print debug info if requested */
    if(repeats <= 1) {
        xmlSecAppPrintEncCtx(&encCtx);
    }
    xmlSecEncCtxFinalize(&encCtx);
    if(doc != NULL) {
        xmlFreeDoc(doc);
    }
    return(res);
}
#endif /* XMLSEC_NO_TMPL_TEST */

static int
xmlSecAppPrepareEncCtx(xmlSecEncCtxPtr encCtx) {    
    if(encCtx == NULL) {
        fprintf(stderr, "Error: enc context is null\n");
        return(-1);
    }

    /* set key info params */
    if(xmlSecAppPrepareKeyInfoReadCtx(&(encCtx->keyInfoReadCtx)) < 0) {
        fprintf(stderr, "Error: failed to prepare key info context\n");
        return(-1);
    }

    if(xmlSecAppCmdLineParamGetString(&sessionKeyParam) != NULL) {
        encCtx->encKey = xmlSecAppCryptoKeyGenerate(xmlSecAppCmdLineParamGetString(&sessionKeyParam),
                                NULL, xmlSecKeyDataTypeSession);
        if(encCtx->encKey == NULL) {
            fprintf(stderr, "Error: failed to generate a session key \"%s\"\n",
                    xmlSecAppCmdLineParamGetString(&sessionKeyParam));
            return(-1);
        }
    }

    if(xmlSecAppCmdLineParamGetStringList(&enabledCipherRefUrisParam) != NULL) {
        encCtx->transformCtx.enabledUris = xmlSecAppGetUriType(
                    xmlSecAppCmdLineParamGetStringList(&enabledCipherRefUrisParam));
        if(encCtx->transformCtx.enabledUris == xmlSecTransformUriTypeNone) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n",
                    xmlSecAppCmdLineParamGetStringList(&enabledCipherRefUrisParam));
            return(-1);
        }
    }
    return(0);
}

static void 
xmlSecAppPrintEncCtx(xmlSecEncCtxPtr encCtx) {
    if(encCtx == NULL) {
        return;
    }
    
    /* print debug info if requested */
    if((print_debug != 0) || xmlSecAppCmdLineParamIsSet(&printDebugParam)) {
        xmlSecEncCtxDebugDump(encCtx, stdout);
    }
    
    if(xmlSecAppCmdLineParamIsSet(&printXmlDebugParam)) {          
        xmlSecEncCtxDebugXmlDump(encCtx, stdout);
    }
}

#endif /* XMLSEC_NO_XMLENC */

static void 
xmlSecAppListKeyData(void) {
    fprintf(stdout, "Registered key data klasses:\n");
    xmlSecKeyDataIdListDebugDump(xmlSecKeyDataIdsGet(), stdout);
}

static int 
xmlSecAppCheckKeyData(const char * name) {
    if(xmlSecKeyDataIdListFindByName(xmlSecKeyDataIdsGet(), BAD_CAST name, xmlSecKeyDataUsageAny) == xmlSecKeyDataIdUnknown) {
        return -1;
    }
    return 0;
}

static void 
xmlSecAppListTransforms(void) {
    fprintf(stdout, "Registered transform klasses:\n");
    xmlSecTransformIdListDebugDump(xmlSecTransformIdsGet(), stdout);
}

static int 
xmlSecAppCheckTransform(const char * name) {
    if(xmlSecTransformIdListFindByName(xmlSecTransformIdsGet(), BAD_CAST name, xmlSecTransformUsageAny) == xmlSecTransformIdUnknown) {
        return -1;
    }
    return 0;
}

static int 
xmlSecAppPrepareKeyInfoReadCtx(xmlSecKeyInfoCtxPtr keyInfoCtx) {
    xmlSecAppCmdLineValuePtr value;
    int ret;
    
    if(keyInfoCtx == NULL) {
        fprintf(stderr, "Error: key info context is null\n");
        return(-1);
    }

#ifndef XMLSEC_NO_X509
    if(xmlSecAppCmdLineParamIsSet(&verificationTimeParam)) {
        keyInfoCtx->certsVerificationTime = xmlSecAppCmdLineParamGetTime(&verificationTimeParam, 0);
    }
    if(xmlSecAppCmdLineParamIsSet(&depthParam)) {
        keyInfoCtx->certsVerificationDepth = xmlSecAppCmdLineParamGetInt(&depthParam, 0);
    }
    if(xmlSecAppCmdLineParamIsSet(&X509SkipStrictChecksParam)) {
        keyInfoCtx->flags |= XMLSEC_KEYINFO_FLAGS_X509DATA_SKIP_STRICT_CHECKS;
    }
#endif /* XMLSEC_NO_X509 */

    /* read enabled key data list */
    for(value = enabledKeyDataParam.value; value != NULL; value = value->next) {
        if(value->strListValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    enabledKeyDataParam.fullName);
            return(-1);
        } else {
            xmlSecKeyDataId dataId;
            const char* p;
            
            for(p = value->strListValue; (p != NULL) && ((*p) != '\0'); p += strlen(p)) {
                dataId = xmlSecKeyDataIdListFindByName(xmlSecKeyDataIdsGet(), BAD_CAST p, xmlSecKeyDataUsageAny);
                if(dataId == xmlSecKeyDataIdUnknown) {
                    fprintf(stderr, "Error: key data \"%s\" is unknown.\n", p);
                    return(-1);
                }
                ret = xmlSecPtrListAdd(&(keyInfoCtx->enabledKeyData), (const xmlSecPtr)dataId);
                if(ret < 0) {
                    fprintf(stderr, "Error: failed to enable key data \"%s\".\n", p);
                    return(-1);
                }
            }
        }
    }

    /* read enabled RetrievalMethod uris */
    if(xmlSecAppCmdLineParamGetStringList(&enabledRetrievalMethodUrisParam) != NULL) {
        keyInfoCtx->retrievalMethodCtx.enabledUris = xmlSecAppGetUriType(
                    xmlSecAppCmdLineParamGetStringList(&enabledRetrievalMethodUrisParam));
        if(keyInfoCtx->retrievalMethodCtx.enabledUris == xmlSecTransformUriTypeNone) {
            fprintf(stderr, "Error: failed to parse \"%s\"\n",
                    xmlSecAppCmdLineParamGetStringList(&enabledRetrievalMethodUrisParam));
            return(-1);
        }
    }

    return(0);
}

static int 
xmlSecAppLoadKeys(void) {
    xmlSecAppCmdLineValuePtr value;
    
    if(gKeysMngr != NULL) {
        fprintf(stderr, "Error: keys manager already initialized.\n");
        return(-1);     
    }    

    /* create and initialize keys manager */
    gKeysMngr = xmlSecKeysMngrCreate();
    if(gKeysMngr == NULL) {
        fprintf(stderr, "Error: failed to create keys manager.\n");
        return(-1);
    }
    if(xmlSecAppCryptoSimpleKeysMngrInit(gKeysMngr) < 0) {
        fprintf(stderr, "Error: failed to initialize keys manager.\n");
        return(-1);
    }    

    /* generate new key file */
    for(value = genKeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", genKeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyGenerate(gKeysMngr, value->strValue, value->paramNameValue) < 0) {
            fprintf(stderr, "Error: failed to generate key \"%s\".\n", value->strValue);
            return(-1);
        }       
    }

    /* read all xml key files */
    for(value = keysFileParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", keysFileParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrLoad(gKeysMngr, value->strValue) < 0) {
            fprintf(stderr, "Error: failed to load xml keys file \"%s\".\n", value->strValue);
            return(-1);
        }       
    }

    /* read all private keys */
    for(value = privkeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    privkeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue, 
                    xmlSecKeyDataFormatPem) < 0) {
            fprintf(stderr, "Error: failed to load private key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    for(value = privkeyDerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    privkeyDerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatDer) < 0) {
            fprintf(stderr, "Error: failed to load private key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    for(value = pkcs8PemParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pkcs8PemParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatPkcs8Pem) < 0) {
            fprintf(stderr, "Error: failed to load private key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    for(value = pkcs8DerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pkcs8DerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatPkcs8Der) < 0) {
            fprintf(stderr, "Error: failed to load private key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    /* read all public keys */
    for(value = pubkeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pubkeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatPem) < 0) {
            fprintf(stderr, "Error: failed to load public key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    for(value = pubkeyDerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pubkeyDerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatDer) < 0) {
            fprintf(stderr, "Error: failed to load public key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

#ifndef XMLSEC_NO_AES    
    /* read all AES keys */
    for(value = aeskeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    aeskeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrBinaryKeyLoad(gKeysMngr, 
                    "aes", value->strValue, value->paramNameValue) < 0) {
            fprintf(stderr, "Error: failed to load aes key from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }
#endif /* XMLSEC_NO_AES */ 

#ifndef XMLSEC_NO_DES    
    /* read all des keys */
    for(value = deskeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    deskeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrBinaryKeyLoad(gKeysMngr, 
                    "des", value->strValue, value->paramNameValue) < 0) {
            fprintf(stderr, "Error: failed to load des key from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }
#endif /* XMLSEC_NO_DES */ 

#ifndef XMLSEC_NO_HMAC    
    /* read all hmac keys */
    for(value = hmackeyParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    hmackeyParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrBinaryKeyLoad(gKeysMngr, 
                    "hmac", value->strValue, value->paramNameValue) < 0) {
            fprintf(stderr, "Error: failed to load hmac key from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }
#endif /* XMLSEC_NO_HMAC */ 

#ifndef XMLSEC_NO_X509
    /* read all pkcs12 files */
    for(value = pkcs12Param.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", pkcs12Param.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrPkcs12KeyLoad(gKeysMngr, 
                    value->strValue,
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue) < 0) {
            fprintf(stderr, "Error: failed to load pkcs12 key from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }

    /* read all trusted certs */
    for(value = trustedParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", trustedParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrCertLoad(gKeysMngr, 
                    value->strValue, xmlSecKeyDataFormatPem,
                    xmlSecKeyDataTypeTrusted) < 0) {
            fprintf(stderr, "Error: failed to load trusted cert from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }
    for(value = trustedDerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", trustedDerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrCertLoad(gKeysMngr, 
                    value->strValue, xmlSecKeyDataFormatDer,
                    xmlSecKeyDataTypeTrusted) < 0) {
            fprintf(stderr, "Error: failed to load trusted cert from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }


    /* read all public keys in certs */
    for(value = pubkeyCertParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pubkeyCertParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatCertPem) < 0) {
            fprintf(stderr, "Error: failed to load public key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }

    for(value = pubkeyCertDerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    pubkeyCertDerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrKeyAndCertsLoad(gKeysMngr, 
                    value->strListValue, 
                    xmlSecAppCmdLineParamGetString(&pwdParam),
                    value->paramNameValue,
                    xmlSecKeyDataFormatCertDer) < 0) {
            fprintf(stderr, "Error: failed to load public key from \"%s\".\n", 
                    value->strListValue);
            return(-1);
        }
    }


    /* read all untrusted certs */
    for(value = untrustedParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", untrustedParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrCertLoad(gKeysMngr, 
                    value->strValue, xmlSecKeyDataFormatPem,
                    xmlSecKeyDataTypeNone) < 0) {
            fprintf(stderr, "Error: failed to load untrusted cert from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }
    for(value = untrustedDerParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", untrustedDerParam.fullName);
            return(-1);
        } else if(xmlSecAppCryptoSimpleKeysMngrCertLoad(gKeysMngr, 
                    value->strValue, xmlSecKeyDataFormatDer,
                    xmlSecKeyDataTypeNone) < 0) {
            fprintf(stderr, "Error: failed to load untrusted cert from \"%s\".\n",
                    value->strValue);
            return(-1);
        }
    }

#endif /* XMLSEC_NO_X509 */    

    return(0);
}

static int intialized = 0;

#ifndef XMLSEC_NO_XSLT
static xsltSecurityPrefsPtr xsltSecPrefs = NULL;
#endif /* XMLSEC_NO_XSLT */

static int
xmlSecAppInit(void) {
    if(intialized != 0) {
        return(0);
    }
    intialized = 1;
    
    /* Init libxml */     
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlTreeIndentString = "\t";
#ifndef XMLSEC_NO_XSLT
    xmlIndentTreeOutput = 1; 
#endif /* XMLSEC_NO_XSLT */
                
    
    /* Init libxslt */
#ifndef XMLSEC_NO_XSLT
    /* disable everything */
    xsltSecPrefs = xsltNewSecurityPrefs(); 
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_FILE,        xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_FILE,       xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_CREATE_DIRECTORY, xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_READ_NETWORK,     xsltSecurityForbid);
    xsltSetSecurityPrefs(xsltSecPrefs,  XSLT_SECPREF_WRITE_NETWORK,    xsltSecurityForbid);
    xsltSetDefaultSecurityPrefs(xsltSecPrefs); 
#endif /* XMLSEC_NO_XSLT */                
    
    /* Init xmlsec */
    if(xmlSecInit() < 0) {
        fprintf(stderr, "Error: xmlsec intialization failed.\n");
        return(-1);
    }
    if(xmlSecCheckVersion() != 1) {
        fprintf(stderr, "Error: loaded xmlsec library version is not compatible.\n");
        return(-1);
    }

#if !defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING)
    if(xmlSecCryptoDLLoadLibrary(BAD_CAST xmlsec_crypto) < 0) {
        fprintf(stderr, "Error: unable to load xmlsec-%s library. Make sure that you have\n"
                        "this it installed, check shared libraries path (LD_LIBRARY_PATH)\n"
                        "envornment variable or use \"--crypto\" option to specify different\n"
                        "crypto engine.\n",
                        ((xmlsec_crypto != NULL) ? BAD_CAST xmlsec_crypto : xmlSecGetDefaultCrypto())
        );
        return(-1);
    }
#endif /* !defined(XMLSEC_NO_CRYPTO_DYNAMIC_LOADING) && defined(XMLSEC_CRYPTO_DYNAMIC_LOADING) */

    /* Init Crypto */
    if(xmlSecAppCryptoInit(xmlSecAppCmdLineParamGetString(&cryptoConfigParam)) < 0) {
        fprintf(stderr, "Error: xmlsec crypto intialization failed.\n");
        return(-1);
    }
    return(0);
}

static void
xmlSecAppShutdown(void) {
    if(intialized == 0) {
        return;
    }

    /* Shutdown Crypto */
    if(xmlSecAppCryptoShutdown() < 0) {
        fprintf(stderr, "Error: xmlsec crypto shutdown failed.\n");
    }
    
    /* Shutdown xmlsec */
    if(xmlSecShutdown() < 0) {
        fprintf(stderr, "Error: xmlsec shutdown failed.\n");
    }
    
    /* Shutdown libxslt/libxml */
#ifndef XMLSEC_NO_XSLT
    xsltFreeSecurityPrefs(xsltSecPrefs);
    xsltCleanupGlobals();            
#endif /* XMLSEC_NO_XSLT */
    xmlCleanupParser();
}

static xmlSecAppXmlDataPtr 
xmlSecAppXmlDataCreate(const char* filename, const xmlChar* defStartNodeName, const xmlChar* defStartNodeNs) {
    xmlSecAppCmdLineValuePtr value;
    xmlSecAppXmlDataPtr data;
    xmlNodePtr cur = NULL;
        
    if(filename == NULL) {
        fprintf(stderr, "Error: xml filename is null\n");
        return(NULL);
    }
    
    /* create object */
    data = (xmlSecAppXmlDataPtr) xmlMalloc(sizeof(xmlSecAppXmlData));
    if(data == NULL) {
        fprintf(stderr, "Error: failed to create xml data\n");
        return(NULL);
    }
    memset(data, 0, sizeof(xmlSecAppXmlData));
    
    /* parse doc */
    data->doc = xmlSecParseFile(filename);
    if(data->doc == NULL) {
        fprintf(stderr, "Error: failed to parse xml file \"%s\"\n", 
                filename);
        xmlSecAppXmlDataDestroy(data);
        return(NULL);    
    }
    
    /* load dtd and set default attrs and ids */
    if(xmlSecAppCmdLineParamGetString(&dtdFileParam) != NULL) {
        xmlValidCtxt ctx;

        data->dtd = xmlParseDTD(NULL, BAD_CAST xmlSecAppCmdLineParamGetString(&dtdFileParam));
        if(data->dtd == NULL) {
            fprintf(stderr, "Error: failed to parse dtd file \"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&dtdFileParam));
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }

        memset(&ctx, 0, sizeof(ctx));    
        /* we don't care is doc actually valid or not */
        xmlValidateDtd(&ctx, data->doc, data->dtd);
    }
    
    /* set ID attributes from command line */
    for(value = idAttrParam.value; value != NULL; value = value->next) {
        if(value->strValue == NULL) {
            fprintf(stderr, "Error: invalid value for option \"%s\".\n", 
                    idAttrParam.fullName);
            xmlSecAppXmlDataDestroy(data);
            return(NULL);
        } else {
            xmlChar* attrName = (value->paramNameValue != NULL) ? BAD_CAST value->paramNameValue : BAD_CAST "id";
            xmlChar* nodeName;
            xmlChar* nsHref;
            xmlChar* buf;
            
            buf = xmlStrdup(BAD_CAST value->strValue);
            if(buf == NULL) {
                fprintf(stderr, "Error: failed to duplicate string \"%s\"\n", value->strValue);
                xmlSecAppXmlDataDestroy(data);
                return(NULL);    
            }
            nodeName = (xmlChar*)strrchr((char*)buf, ':');
            if(nodeName != NULL) {
                (*(nodeName++)) = '\0';
                nsHref = buf;
            } else {
                nodeName = buf;
                nsHref = NULL;
            }

            /* process children first because it does not matter much but does simplify code */
            cur = xmlSecGetNextElementNode(data->doc->children);
            while(cur != NULL) {
                if(xmlSecAppAddIDAttr(cur, attrName, nodeName, nsHref) < 0) {
                    fprintf(stderr, "Error: failed to add ID attribute \"%s\" for node \"%s\"\n", attrName, value->strValue);
                    xmlFree(buf);
                    xmlSecAppXmlDataDestroy(data);
                    return(NULL);    
                }
                cur = xmlSecGetNextElementNode(cur->next);
            }

            xmlFree(buf);
        }
    }


    /* now find the start node */
    if(xmlSecAppCmdLineParamGetString(&nodeIdParam) != NULL) {
        xmlAttrPtr attr;
            
        attr = xmlGetID(data->doc, BAD_CAST xmlSecAppCmdLineParamGetString(&nodeIdParam));
        if(attr == NULL) {
            fprintf(stderr, "Error: failed to find node with id=\"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&nodeIdParam));
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
        cur = attr->parent;
    } else if(xmlSecAppCmdLineParamGetString(&nodeNameParam) != NULL) {
        xmlChar* buf;
        xmlChar* name;
        xmlChar* ns;
        
        buf = xmlStrdup(BAD_CAST xmlSecAppCmdLineParamGetString(&nodeNameParam));
        if(buf == NULL) {
            fprintf(stderr, "Error: failed to duplicate node \"%s\"\n", 
                    xmlSecAppCmdLineParamGetString(&nodeNameParam));
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
        name = (xmlChar*)strrchr((char*)buf, ':');
        if(name != NULL) {
            (*(name++)) = '\0';
            ns = buf;
        } else {
            name = buf;
            ns = NULL;
        }
        
        cur = xmlSecFindNode(xmlDocGetRootElement(data->doc), name, ns);
        if(cur == NULL) {
            fprintf(stderr, "Error: failed to find node with name=\"%s\"\n", 
                    name);
            xmlFree(buf);
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
        xmlFree(buf);
    } else if(xmlSecAppCmdLineParamGetString(&nodeXPathParam) != NULL) {
        xmlXPathContextPtr ctx = NULL;
        xmlXPathObjectPtr obj = NULL;
        xmlNodePtr rootNode;
        xmlNsPtr ns;
        int ret;

        rootNode = xmlDocGetRootElement(data->doc);
        if(rootNode == NULL) {
            fprintf(stderr, "Error: failed to find root node\n"); 
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
        
        ctx = xmlXPathNewContext(data->doc);
        if(ctx == NULL) {
            fprintf(stderr, "Error: failed to create xpath context\n");
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
        
        /* register namespaces from the root node */
        for(ns = rootNode->nsDef; ns != NULL; ns = ns->next) {
            if(ns->prefix != NULL){
                ret = xmlXPathRegisterNs(ctx, ns->prefix, ns->href);
                if(ret != 0) {
                    fprintf(stderr, "Error: failed to register namespace \"%s\"\n", ns->prefix);
                    xmlXPathFreeContext(ctx);
                    xmlSecAppXmlDataDestroy(data);
                    return(NULL);    
                }
            }
        }
        
        obj = xmlXPathEval(BAD_CAST xmlSecAppCmdLineParamGetString(&nodeXPathParam), ctx);
        if(obj == NULL) {
            fprintf(stderr, "Error: failed to evaluate xpath expression\n");
            xmlXPathFreeContext(ctx);
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }

        if((obj->nodesetval == NULL) || (obj->nodesetval->nodeNr != 1)) {
            fprintf(stderr, "Error: xpath expression evaluation does not return a single node as expected\n");
            xmlXPathFreeObject(obj);
            xmlXPathFreeContext(ctx);
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
                
        cur = obj->nodesetval->nodeTab[0];
        xmlXPathFreeContext(ctx);
        xmlXPathFreeObject(obj);
        
    } else {
        cur = xmlDocGetRootElement(data->doc);
        if(cur == NULL) {
            fprintf(stderr, "Error: failed to get root element\n"); 
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
    }
    
    if(defStartNodeName != NULL) {
        data->startNode = xmlSecFindNode(cur, defStartNodeName, defStartNodeNs);
        if(data->startNode == NULL) {
            fprintf(stderr, "Error: failed to find default node with name=\"%s\"\n", 
                    defStartNodeName);
            xmlSecAppXmlDataDestroy(data);
            return(NULL);    
        }
    } else {
        data->startNode = cur;
    }
    
    return(data);
}

static void 
xmlSecAppXmlDataDestroy(xmlSecAppXmlDataPtr data) {
    if(data == NULL) {
        fprintf(stderr, "Error: xml data is null\n");
        return;
    }
    if(data->dtd != NULL) {
        xmlFreeDtd(data->dtd);
    }
    if(data->doc != NULL) {
        xmlFreeDoc(data->doc);
    }
    memset(data, 0, sizeof(xmlSecAppXmlData));
    xmlFree(data);    
}

static xmlSecAppCommand 
xmlSecAppParseCommand(const char* cmd, xmlSecAppCmdLineParamTopic* cmdLineTopics, xmlSecAppCommand* subCommand) {
    if(subCommand != NULL) {
        (*subCommand) = xmlSecAppCommandUnknown;
    }

    if((cmd == NULL) || (cmdLineTopics == NULL)) {
        return(xmlSecAppCommandUnknown);
    } else 

    if((strcmp(cmd, "help") == 0) || (strcmp(cmd, "--help") == 0)) {
        (*cmdLineTopics) = 0;
        return(xmlSecAppCommandHelp);
    } else 

    if((strcmp(cmd, "help-all") == 0) || (strcmp(cmd, "--help-all") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicAll;
        return(xmlSecAppCommandHelp);
    } else 
    
    if((strncmp(cmd, "help-", 5) == 0) || (strncmp(cmd, "--help-", 7) == 0)) {   
        cmd = (cmd[0] == '-') ? cmd + 7 : cmd + 5;
        if(subCommand) {
            (*subCommand) = xmlSecAppParseCommand(cmd, cmdLineTopics, NULL);
        } else {
            (*cmdLineTopics) = 0;
        }
        return(xmlSecAppCommandHelp);
    } else 

    if((strcmp(cmd, "version") == 0) || (strcmp(cmd, "--version") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicVersion;
        return(xmlSecAppCommandVersion);
    } else 

    if((strcmp(cmd, "list-key-data") == 0) || (strcmp(cmd, "--list-key-data") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicCryptoConfig;
        return(xmlSecAppCommandListKeyData);
    } else 

    if((strcmp(cmd, "check-key-data") == 0) || (strcmp(cmd, "--check-key-data") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicCryptoConfig;
        return(xmlSecAppCommandCheckKeyData);
    } else 

    if((strcmp(cmd, "list-transforms") == 0) || (strcmp(cmd, "--list-transforms") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicCryptoConfig;
        return(xmlSecAppCommandListTransforms);
    } else 

    if((strcmp(cmd, "check-transforms") == 0) || (strcmp(cmd, "--check-transforms") == 0)) {
        (*cmdLineTopics) = xmlSecAppCmdLineTopicCryptoConfig;
        return(xmlSecAppCommandCheckTransforms);
    } else 
    
    if((strcmp(cmd, "keys") == 0) || (strcmp(cmd, "--keys") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral | 
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandKeys);
    } else 
    
#ifndef XMLSEC_NO_XMLDSIG
    if((strcmp(cmd, "sign") == 0) || (strcmp(cmd, "--sign") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicDSigCommon |
                        xmlSecAppCmdLineTopicDSigSign |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandSign);
    } else 
    
    if((strcmp(cmd, "verify") == 0) || (strcmp(cmd, "--verify") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicDSigCommon |
                        xmlSecAppCmdLineTopicDSigVerify |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandVerify);
    } else 
#ifndef XMLSEC_NO_TMPL_TEST
    if((strcmp(cmd, "sign-tmpl") == 0) || (strcmp(cmd, "--sign-tmpl") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicDSigCommon |
                        xmlSecAppCmdLineTopicDSigSign |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandSignTmpl);
    } else 
#endif /* XMLSEC_NO_TMPL_TEST */
    
#endif /* XMLSEC_NO_XMLDSIG */

#ifndef XMLSEC_NO_XMLENC
    if((strcmp(cmd, "encrypt") == 0) || (strcmp(cmd, "--encrypt") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicEncCommon |
                        xmlSecAppCmdLineTopicEncEncrypt |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandEncrypt);
    } else 

    if((strcmp(cmd, "decrypt") == 0) || (strcmp(cmd, "--decrypt") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicEncCommon |
                        xmlSecAppCmdLineTopicEncDecrypt |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandDecrypt);
    } else 

#ifndef XMLSEC_NO_TMPL_TEST
    if((strcmp(cmd, "encrypt-tmpl") == 0) || (strcmp(cmd, "--encrypt-tmpl") == 0)) {
        (*cmdLineTopics) = 
                        xmlSecAppCmdLineTopicGeneral |
                        xmlSecAppCmdLineTopicCryptoConfig |
                        xmlSecAppCmdLineTopicEncCommon |
                        xmlSecAppCmdLineTopicEncEncrypt |
                        xmlSecAppCmdLineTopicKeysMngr |
                        xmlSecAppCmdLineTopicX509Certs;
        return(xmlSecAppCommandEncryptTmpl);
    } else 
#endif /* XMLSEC_NO_TMPL_TEST */
#endif /* XMLSEC_NO_XMLENC */

    if(1) {
        (*cmdLineTopics) = 0;
        return(xmlSecAppCommandUnknown);
    }
}
        
static void 
xmlSecAppPrintHelp(xmlSecAppCommand command, xmlSecAppCmdLineParamTopic topics) {
    switch(command) {
    case xmlSecAppCommandUnknown:
    case xmlSecAppCommandHelp:
        fprintf(stdout, "%s%s\n", helpCommands1, helpCommands2);
        break;
    case xmlSecAppCommandVersion:
        fprintf(stdout, "%s\n", helpVersion);
        break;
    case xmlSecAppCommandListKeyData:
        fprintf(stdout, "%s\n", helpListKeyData);
        break;
    case xmlSecAppCommandCheckKeyData:
        fprintf(stdout, "%s\n", helpCheckKeyData);
        break;
    case xmlSecAppCommandListTransforms:
        fprintf(stdout, "%s\n", helpListTransforms);
        break;
    case xmlSecAppCommandCheckTransforms:
        fprintf(stdout, "%s\n", helpCheckTransforms);
        break;
    case xmlSecAppCommandKeys:
        fprintf(stdout, "%s\n", helpKeys);
        break;
    case xmlSecAppCommandSign:
        fprintf(stdout, "%s\n", helpSign);
        break;
    case xmlSecAppCommandVerify:
        fprintf(stdout, "%s\n", helpVerify);
        break;
    case xmlSecAppCommandEncrypt:
        fprintf(stdout, "%s\n", helpEncrypt);
        break;
    case xmlSecAppCommandDecrypt:
        fprintf(stdout, "%s\n", helpDecrypt);
        break;
    case xmlSecAppCommandSignTmpl:
        fprintf(stdout, "%s\n", helpSignTmpl);
        break;
    case xmlSecAppCommandEncryptTmpl:
        fprintf(stdout, "%s\n", helpEncryptTmpl);
        break;
    }
    if(topics != 0) {
        fprintf(stdout, "Options:\n");
        xmlSecAppCmdLineParamsListPrint(parameters, topics, stdout);
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "\n%s\n", bugs);
    fprintf(stdout, "%s\n", copyright);
}

static xmlSecTransformUriType 
xmlSecAppGetUriType(const char* string) {
    xmlSecTransformUriType type = xmlSecTransformUriTypeNone;
    
    while((string != NULL) && (string[0] != '\0')) {
        if(strcmp(string, "empty") == 0) {
            type |= xmlSecTransformUriTypeEmpty;
        } else if(strcmp(string, "same-doc") == 0) {
            type |= xmlSecTransformUriTypeSameDocument;
        } else if(strcmp(string, "local") == 0) {
            type |= xmlSecTransformUriTypeLocal;
        } else if(strcmp(string, "remote") == 0) {
            type |= xmlSecTransformUriTypeRemote;
        } else {
            fprintf(stderr, "Error: invalid uri type: \"%s\"\n", string);
            return(xmlSecTransformUriTypeNone);
        }
        string += strlen(string) + 1;
    }
    return(type);
}

static FILE* 
xmlSecAppOpenFile(const char* filename) {
    FILE* file;
    
    if((filename == NULL) || (strcmp(filename, XMLSEC_STDOUT_FILENAME) == 0)) {
        return(stdout);
    }
    file = fopen(filename, "wb");
    if(file == NULL) {
        fprintf(stderr, "Error: failed to open file \"%s\"\n", filename);
        return(NULL);
    }
    
    return(file);
}

static void 
xmlSecAppCloseFile(FILE* file) {
    if((file == NULL) || (file == stdout) || (file == stderr)) {
        return;
    }
    
    fclose(file);
}

static int 
xmlSecAppWriteResult(xmlDocPtr doc, xmlSecBufferPtr buffer) {
    FILE* f;

    f = xmlSecAppOpenFile(xmlSecAppCmdLineParamGetString(&outputParam));
    if(f == NULL) {
        return(-1);
    }
    if(doc != NULL) {
        xmlDocDump(f, doc);    
    } else if((buffer != NULL) && (xmlSecBufferGetData(buffer) != NULL)) {
        (void)fwrite(xmlSecBufferGetData(buffer), xmlSecBufferGetSize(buffer), 1, f); 
    } else {
        fprintf(stderr, "Error: both result doc and result buffer are null\n"); 
        xmlSecAppCloseFile(f);
        return(-1);
    }    
    xmlSecAppCloseFile(f);
    return(0);
}

static int  
xmlSecAppAddIDAttr(xmlNodePtr node, const xmlChar* attrName, const xmlChar* nodeName, const xmlChar* nsHref) {
    xmlAttrPtr attr, tmpAttr;
    xmlNodePtr cur;
    xmlChar* id;
    
    if((node == NULL) || (attrName == NULL) || (nodeName == NULL)) {
        return(-1);
    }
    
    /* process children first because it does not matter much but does simplify code */
    cur = xmlSecGetNextElementNode(node->children);
    while(cur != NULL) {
        if(xmlSecAppAddIDAttr(cur, attrName, nodeName, nsHref) < 0) {
            return(-1);
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }
    
    /* node name must match */
    if(!xmlStrEqual(node->name, nodeName)) {
        return(0);
    }
        
    /* if nsHref is set then it also should match */    
    if((nsHref != NULL) && (node->ns != NULL) && (!xmlStrEqual(nsHref, node->ns->href))) {
        return(0);
    }
    
    /* the attribute with name equal to attrName should exist */
    for(attr = node->properties; attr != NULL; attr = attr->next) {
        if(xmlStrEqual(attr->name, attrName)) {
            break;
        }
    }
    if(attr == NULL) {
        return(0);
    }
    
    /* and this attr should have a value */
    id = xmlNodeListGetString(node->doc, attr->children, 1);
    if(id == NULL) {
        return(0);
    }
    
    /* check that we don't have same ID already */
    tmpAttr = xmlGetID(node->doc, id);
    if(tmpAttr == NULL) {
        xmlAddID(NULL, node->doc, id, attr);
    } else if(tmpAttr != attr) {
        fprintf(stderr, "Error: duplicate ID attribute \"%s\"\n", id);  
        xmlFree(id);
        return(-1);
    }
    xmlFree(id);
    return(0);
}



