/**
 * XML Security Library (http://www.aleksey.com/xmlsec).
 *
 * Export macro declarations for Win32 platform.
 *
 * This is free software; see Copyright file in the source
 * distribution for preciese wording.
 *
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_EXPORTS_H__
#define __XMLSEC_EXPORTS_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Now, the export orgy begins. The following we must do for the
   Windows platform with MSVC compiler. */

#if !defined XMLSEC_EXPORT
#  if defined(_WIN32)
     /* if we compile libxmlsec itself: */
#    if defined(IN_XMLSEC)
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_EXPORT __declspec(dllexport)
#      else
#        define XMLSEC_EXPORT extern
#      endif
     /* if a client program includes this file: */
#    else
#if 1
       /* gcc fail by initialisation of global variable with error
          (as example in .../openssl/ciphers.c):
            "initializer element is not constant"
          To avoid this we shouldn't use __declspec(dllimport).
          This will enable auto-import feature. */
#      define XMLSEC_EXPORT
#else
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_EXPORT __declspec(dllimport)
#      else
#        define XMLSEC_EXPORT
#      endif
#endif
#    endif
   /* This holds on all other platforms/compilers, which are easier to
      handle in regard to this. */
#  else
#    define XMLSEC_EXPORT
#  endif
#endif

#if !defined XMLSEC_CRYPTO_EXPORT
#  if defined(_WIN32)
     /* if we compile libxmlsec itself: */
#    if defined(IN_XMLSEC_CRYPTO)
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_CRYPTO_EXPORT __declspec(dllexport)
#      else
#        define XMLSEC_CRYPTO_EXPORT extern
#      endif
     /* if a client program includes this file: */
#    else
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_CRYPTO_EXPORT __declspec(dllimport)
#      else
#        define XMLSEC_CRYPTO_EXPORT
#      endif
#    endif
   /* This holds on all other platforms/compilers, which are easier to
      handle in regard to this. */
#  else
#    define XMLSEC_CRYPTO_EXPORT
#  endif
#endif

#if !defined XMLSEC_EXPORT_VAR
#  if defined(_WIN32)
     /* if we compile libxmlsec itself: */
#    if defined(IN_XMLSEC)
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_EXPORT_VAR __declspec(dllexport) extern
#      else
#        define XMLSEC_EXPORT_VAR extern
#      endif
     /* if we compile libxmlsec-crypto itself: */
#    elif defined(IN_XMLSEC_CRYPTO)
#        define XMLSEC_EXPORT_VAR extern
     /* if a client program includes this file: */
#    else
#      if !defined(XMLSEC_STATIC)
#        define XMLSEC_EXPORT_VAR __declspec(dllimport) extern
#      else
#        define XMLSEC_EXPORT_VAR extern
#      endif
#    endif
   /* This holds on all other platforms/compilers, which are easier to
      handle in regard to this. */
#  else
#    define XMLSEC_EXPORT_VAR extern
#  endif
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_EXPORTS_H__ */


