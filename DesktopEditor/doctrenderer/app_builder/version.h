#ifndef VERSION_H
#define VERSION_H

#define VER_FILEVERSION             1,0,0,2
#define VER_FILEVERSION_STR         "1.0.0.2\0"

#define VER_PRODUCTVERSION          VER_FILEVERSION
#define VER_PRODUCTVERSION_STR      "1.0\0"

#ifdef ONLY_RU
#define VER_COMPANYNAME_STR         "Novie kommunikacionnie tehnologii CJSC\0"
#define VER_LEGALCOPYRIGHT_STR      "Novie kommunikacionnie tehnologii CJSC, 2016\0"
#define VER_COMPANYDOMAIN_STR       "www.onlyoffice.ru\0"
#define ABOUT_COPYRIGHT_STR         "1999-2016 ЗАО 'НКТ'\0"
#else
#define VER_COMPANYNAME_STR         "Ascensio System SIA\0"
#define VER_LEGALCOPYRIGHT_STR      "Ascensio System SIA 2016\0"
#define VER_COMPANYDOMAIN_STR       "www.onlyoffice.com\0"
#define ABOUT_COPYRIGHT_STR         VER_LEGALCOPYRIGHT_STR
#endif

#define VER_FILEDESCRIPTION_STR     "ONLYOFFICE Document Builder\0"
#define VER_INTERNALNAME_STR        "Document Builder\0"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved\0"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "docbuilder.exe\0"
#define VER_PRODUCTNAME_STR         "ONLYOFFICE Document Builder\0"

#endif

