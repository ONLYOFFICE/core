#ifndef _XMLSIGNER_CERTIFICATE_H_
#define _XMLSIGNER_CERTIFICATE_H_

#ifdef WIN32
#include "XmlSigner_mscrypto.h"
#define CCertificate CCertificate_mscrypto
#endif

#if defined(_LINUX) && !defined(_MAC)

#endif

#ifdef _MAC

#endif

#endif // _XMLSIGNER_CERTIFICATE_H_
