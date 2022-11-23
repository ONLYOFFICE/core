#ifndef _XML_OPENSSL_CONFIG_H_
#define _XML_OPENSSL_CONFIG_H_

#include <string>
#include <vector>
#include "../../../common/base_export.h"

#ifdef COMMON_OPENSSL_BUILDING_INTERNAL
#define OPENSSL_DECL
#else
#ifdef COMMON_OPENSSL_BUILDING
#define OPENSSL_DECL Q_DECL_EXPORT
#else
#define OPENSSL_DECL Q_DECL_IMPORT
#endif
#endif

#endif // _XML_OPENSSL_CONFIG_H_
