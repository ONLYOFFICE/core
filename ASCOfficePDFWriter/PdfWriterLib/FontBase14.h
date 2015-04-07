#ifndef _FONT_BASE_14_H
#define _FONT_BASE_14_H

#include "Utils.h"
#include "FontTypes.h"
#include "FontType1.h"

//----- ќсновные 14 фонтов --------------------------------------------------------------------------

#define  FONT_COURIER                 "Courier"
#define  FONT_COURIER_BOLD            "Courier-Bold"
#define  FONT_COURIER_OBLIQUE         "Courier-Oblique"
#define  FONT_COURIER_BOLD_OBLIQUE    "Courier-BoldOblique"
#define  FONT_HELVETICA               "Helvetica"
#define  FONT_HELVETICA_BOLD          "Helvetica-Bold"
#define  FONT_HELVETICA_OBLIQUE       "Helvetica-Oblique"
#define  FONT_HELVETICA_BOLD_OBLIQUE  "Helvetica-BoldOblique"
#define  FONT_TIMES_ROMAN             "Times-Roman"
#define  FONT_TIMES_BOLD              "Times-Bold"
#define  FONT_TIMES_ITALIC            "Times-Italic"
#define  FONT_TIMES_BOLD_ITALIC       "Times-BoldItalic"
#define  FONT_SYMBOL                  "Symbol"
#define  FONT_ZAPF_DINGBATS           "ZapfDingbats"
typedef struct TBase14FontDefData 
{
	const char      *sFontName;//[LIMIT_MAX_NAME_LEN + 1];
    const CharData  *pWidthsTable;
    BOOL             bIsFontSpecific;
    short            nAscent;
    short            nDescent;
    unsigned short   nXHeight;
    unsigned short   nCapHeight;
    Box              oBox;
} Base14FontDefData;


//---------------------------------------------------------------------------------------------------

const   Base14FontDefData* Base14FontDefFindBuiltinData(const char *sFontName);
FontDef                    Base14FontDefNew            (MMgr oMMgr, const char *sFontName);
                   
#endif /* _FONT_BASE_14_H */
