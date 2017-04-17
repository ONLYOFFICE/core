/** 
 * XMLSec library
 * 
 * Command line parsing routines
 *
 * See Copyright for the status of this software.
 * 
 * Copyright (C) 2002-2016 Aleksey Sanin <aleksey@aleksey.com>. All Rights Reserved.
 */
#ifndef __XMLSEC_APPS_CMDLINE_H__
#define __XMLSEC_APPS_CMDLINE_H__    

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */ 

#include <time.h>

typedef struct _xmlSecAppCmdLineParam           xmlSecAppCmdLineParam,
                                                *xmlSecAppCmdLineParamPtr;
typedef struct _xmlSecAppCmdLineValue           xmlSecAppCmdLineValue,
                                                *xmlSecAppCmdLineValuePtr;
typedef unsigned int                            xmlSecAppCmdLineParamTopic;

#define xmlSecAppCmdLineParamFlagNone                   0x0000
#define xmlSecAppCmdLineParamFlagParamNameValue         0x0001
#define xmlSecAppCmdLineParamFlagMultipleValues         0x0002

#define XMLSEC_STDOUT_FILENAME                          "-"

typedef enum {
    xmlSecAppCmdLineParamTypeFlag,
    xmlSecAppCmdLineParamTypeString,
    xmlSecAppCmdLineParamTypeStringList,
    xmlSecAppCmdLineParamTypeNumber,
    xmlSecAppCmdLineParamTypeTime
} xmlSecAppCmdLineParamType;

struct _xmlSecAppCmdLineParam {
    xmlSecAppCmdLineParamTopic  topics;
    const char*                 fullName;
    const char*                 shortName;
    const char*                 help;
    xmlSecAppCmdLineParamType   type;
    int                         flags;
    xmlSecAppCmdLineValuePtr    value;
};

int             xmlSecAppCmdLineParamIsSet              (xmlSecAppCmdLineParamPtr param);
const char*     xmlSecAppCmdLineParamGetString          (xmlSecAppCmdLineParamPtr param);
const char*     xmlSecAppCmdLineParamGetStringList      (xmlSecAppCmdLineParamPtr param);
int             xmlSecAppCmdLineParamGetInt             (xmlSecAppCmdLineParamPtr param,
                                                         int def);
time_t          xmlSecAppCmdLineParamGetTime            (xmlSecAppCmdLineParamPtr param,
                                                         time_t def);

int             xmlSecAppCmdLineParamsListParse         (xmlSecAppCmdLineParamPtr* params,
                                                         xmlSecAppCmdLineParamTopic topcis,
                                                         const char** argv,
                                                         int argc,
                                                         int pos);
void            xmlSecAppCmdLineParamsListClean         (xmlSecAppCmdLineParamPtr* params);
void            xmlSecAppCmdLineParamsListPrint         (xmlSecAppCmdLineParamPtr* params,
                                                         xmlSecAppCmdLineParamTopic topic,
                                                         FILE* output);

struct _xmlSecAppCmdLineValue {
    xmlSecAppCmdLineParamPtr    param;
    int                         pos;
    const char*                 paramNameValue;
    const char*                 strValue;
    const char*                 strListValue;
    int                         intValue;
    time_t                      timeValue;
    xmlSecAppCmdLineValuePtr    next;
};


xmlSecAppCmdLineValuePtr xmlSecAppCmdLineValueCreate    (xmlSecAppCmdLineParamPtr param,
                                                         int pos);
void                     xmlSecAppCmdLineValueDestroy   (xmlSecAppCmdLineValuePtr value);                                                                


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __XMLSEC_APPS_CMDLINE_H__ */



