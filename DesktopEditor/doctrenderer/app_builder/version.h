#ifndef VERSION_H
#define VERSION_H

#define VER_STRINGIFY(d)            #d
#define TO_STR(v)                   VER_STRINGIFY(v)

#define VER_FILEVERSION             WIN_INTVER
#define VER_FILEVERSION_STR         TO_STR(WIN_INTVER)"\0"

#define VER_PRODUCTVERSION          VER_FILEVERSION
#define VER_PRODUCTVERSION_STR      "1.0\0"

#define VER_COMPANYNAME_STR         "Ascensio System SIA\0"
#define VER_LEGALCOPYRIGHT_STR      "Ascensio System SIA " TO_STR(COPYRIGHT_YEAR) "\0"
#define VER_COMPANYDOMAIN_STR       "www.onlyoffice.com\0"
#define ABOUT_COPYRIGHT_STR         VER_LEGALCOPYRIGHT_STR

#define VER_FILEDESCRIPTION_STR     "ONLYOFFICE Document Builder\0"
#define VER_INTERNALNAME_STR        "Document Builder\0"
#define VER_LEGALTRADEMARKS1_STR    "All Rights Reserved\0"
#define VER_LEGALTRADEMARKS2_STR    VER_LEGALTRADEMARKS1_STR
#define VER_ORIGINALFILENAME_STR    "docbuilder.exe\0"
#define VER_PRODUCTNAME_STR         "ONLYOFFICE Document Builder\0"

#endif

