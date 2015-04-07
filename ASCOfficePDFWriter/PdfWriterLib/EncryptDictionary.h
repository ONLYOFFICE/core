#ifndef _ENCRYPT_DICTIONARY_H
#define _ENCRYPT_DICTIONARY_H

#pragma once 

#include "MetaData.h"

//-------- EncryptDict - ќсновные функции ------------------------------------------------
void          EncryptDictOnFree     (Dict pObj);
EncryptDict   EncryptDictNew        (MMgr oMMgr, Xref pXref);
void          EncryptDictCreateID   (EncryptDict pDict, Dict pInfo, Xref pXref);
unsigned long EncryptDictSetPassword(EncryptDict pDict, const char *sOwnerPassword, const char *sUserPassword);
BOOL          EncryptDictValidate   (EncryptDict pDict);
unsigned long EncryptDictPrepare    (EncryptDict pDict, Dict pInfo, Xref pXref);
EncryptRecPtr EncryptDictGetAttr    (EncryptDict pDict);
#endif /* _ENCRYPT_DICTIONARY_H */
