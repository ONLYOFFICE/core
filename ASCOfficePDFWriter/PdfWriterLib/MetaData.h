#ifndef _INFO_H
#define _INFO_H

#pragma once 

#include "Objects.h"
#include "Utils.h"

static const char  *c_asInfoItemNames[] = 
{
    "CreationDate",
    "ModDate",
    "Author",
    "Creator",
    "Producer",
    "Title",
    "Subject",
    "Keywords",
    NULL
};

//static const char* InfoTypeToName       (InfoType eType)
//{
//    unsigned int nIndex = (unsigned int)eType;
//
//    return c_asInfoItemNames[ nIndex ];
//}

unsigned long      InfoSetInfoAttr      (Dict pInfo, InfoType eType, const char *sValue, EncoderRecPtr pEncoder);
const char*        InfoGetInfoAttr      (Dict pInfo, InfoType eType);
unsigned long      InfoSetInfoDateAttr  (Dict pInfo, InfoType eType, TDate oDate);
#endif /* _INFO_H */

