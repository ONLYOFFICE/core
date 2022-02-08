#ifndef RECORDTYPES_H
#define RECORDTYPES_H

#include <unordered_map>
#include "../XlsxFormat/WritingElement.h"

namespace XLSB
{

// Workbook records
#define BIFF12_DEFINEDNAME            0x27
#define BIFF12_FILEVERSION            0x0180
#define BIFF12_WORKBOOK               0x0183
#define BIFF12_WORKBOOK_END           0x0184
#define BIFF12_BOOKVIEWS              0x0187
#define BIFF12_BOOKVIEWS_END          0x0188
#define BIFF12_SHEETS                 0x018F
#define BIFF12_SHEETS_END             0x0190
#define BIFF12_WORKBOOKPR             0x0199
#define BIFF12_SHEET                  0x019C
#define BIFF12_CALCPR                 0x019D
#define BIFF12_WORKBOOKVIEW           0x019E
#define BIFF12_EXTERNALREFERENCES     0x02E1
#define BIFF12_EXTERNALREFERENCES_END 0x02E2
#define BIFF12_EXTERNALREFERENCE      0x02E3
#define BIFF12_WEBPUBLISHING          0x04A9

// Worksheet records
#define BIFF12_ROW             0x00 // row info
#define BIFF12_BLANK           0x01 // empty cell
#define BIFF12_NUM             0x02 // single-precision float
#define BIFF12_BOOLERR         0x03 // error identifier
#define BIFF12_BOOL            0x04 // boolean value
#define BIFF12_FLOAT           0x05 // double-precision float
#define BIFF12_STRING          0x07 // string (shared string index)
#define BIFF12_FORMULA_STRING  0x08 // formula returning a string (inline string)
#define BIFF12_FORMULA_FLOAT   0x09 // formula returning a double-precision float
#define BIFF12_FORMULA_BOOL    0x0A // formula returning a boolean
#define BIFF12_FORMULA_BOOLERR 0x0B // formula returning an error identifier
#define BIFF12_COL             0x3C // column info
#define BIFF12_WORKSHEET       0x0181
#define BIFF12_WORKSHEET_END   0x0182
#define BIFF12_SHEETVIEWS      0x0185
#define BIFF12_SHEETVIEWS_END  0x0186
#define BIFF12_SHEETVIEW       0x0189
#define BIFF12_SHEETVIEW_END   0x018A
#define BIFF12_SHEETDATA       0x0191
#define BIFF12_SHEETDATA_END   0x0192
#define BIFF12_SHEETPR         0x0193
#define BIFF12_DIMENSION       0x0194
#define BIFF12_SELECTION       0x0198
#define BIFF12_COLS            0x0386
#define BIFF12_COLS_END        0x0387
#define BIFF12_CONDITIONALFORMATTING 0x03CD
#define BIFF12_CONDITIONALFORMATTING_END 0x03CE
#define BIFF12_CFRULE          0x03CF
#define BIFF12_CFRULE_END      0x03D0
#define BIFF12_ICONSET         0x03D1
#define BIFF12_ICONSET_END     0x03D2
#define BIFF12_DATABAR         0x03D3
#define BIFF12_DATABAR_END     0x03D4
#define BIFF12_COLORSCALE      0x03D5
#define BIFF12_COLORSCALE_END  0x03D6
#define BIFF12_CFVO            0x03D7
#define BIFF12_PAGEMARGINS     0x03DC
#define BIFF12_PRINTOPTIONS    0x03DD
#define BIFF12_PAGESETUP       0x03DE
#define BIFF12_HEADERFOOTER    0x03DF
#define BIFF12_SHEETFORMATPR   0x03E5
#define BIFF12_HYPERLINK       0x03EE
#define BIFF12_DRAWING         0x04A6
#define BIFF12_LEGACYDRAWING   0x04A7
#define BIFF12_COLOR           0x04B4
#define BIFF12_OLEOBJECTS      0x04FE
#define BIFF12_OLEOBJECT       0x04FF
#define BIFF12_OLEOBJECTS_END  0x0580
#define BIFF12_TABLEPARTS      0x0594
#define BIFF12_TABLEPART       0x0595
#define BIFF12_TABLEPARTS_END  0x0596

//SharedStrings records
#define BIFF12_SI              0x13
#define BIFF12_SST             0x019F
#define BIFF12_SST_END         0x01A0

//Styles records
#define BIFF12_FONT            0x2B
#define BIFF12_FILL            0x2D
#define BIFF12_BORDER          0x2E
#define BIFF12_XF              0x2F
#define BIFF12_CELLSTYLE       0x30
#define BIFF12_STYLESHEET      0x0296
#define BIFF12_STYLESHEET_END  0x0297
#define BIFF12_COLORS          0x03D9
#define BIFF12_COLORS_END      0x03DA
#define BIFF12_DXFS            0x03F9
#define BIFF12_DXFS_END        0x03FA
#define BIFF12_TABLESTYLES     0x03FC
#define BIFF12_TABLESTYLES_END 0x03FD
#define BIFF12_FILLS           0x04DB
#define BIFF12_FILLS_END       0x04DC
#define BIFF12_FONTS           0x04E3
#define BIFF12_FONTS_END       0x04E4
#define BIFF12_BORDERS         0x04E5
#define BIFF12_BORDERS_END     0x04E6
#define BIFF12_CELLXFS         0x04E9
#define BIFF12_CELLXFS_END     0x04EA
#define BIFF12_CELLSTYLES      0x04EB
#define BIFF12_CELLSTYLES_END  0x04EC
#define BIFF12_CELLSTYLEXFS    0x04F2
#define BIFF12_CELLSTYLEXFS_END 0x04F3

//Comment records
#define BIFF12_COMMENTS        0x04F4
#define BIFF12_COMMENTS_END    0x04F5
#define BIFF12_AUTHORS         0x04F6
#define BIFF12_AUTHORS_END     0x04F7
#define BIFF12_AUTHOR          0x04F8
#define BIFF12_COMMENTLIST     0x04F9
#define BIFF12_COMMENTLIST_END 0x04FA
#define BIFF12_COMMENT         0x04FB
#define BIFF12_COMMENT_END     0x04FC
#define BIFF12_TEXT            0x04FD

//Table records
#define BIFF12_AUTOFILTER      0x01A1
#define BIFF12_AUTOFILTER_END  0x01A2
#define BIFF12_FILTERCOLUMN    0x01A3
#define BIFF12_FILTERCOLUMN_END 0x01A4
#define BIFF12_FILTERS         0x01A5
#define BIFF12_FILTERS_END     0x01A6
#define BIFF12_FILTER          0x01A7
#define BIFF12_TABLE           0x02D7
#define BIFF12_TABLE_END       0x02D8
#define BIFF12_TABLECOLUMNS    0x02D9
#define BIFF12_TABLECOLUMNS_END 0x02DA
#define BIFF12_TABLECOLUMN     0x02DB
#define BIFF12_TABLECOLUMN_END 0x02DC
#define BIFF12_TABLESTYLEINFO  0x0481
#define BIFF12_SORTSTATE       0x0492
#define BIFF12_SORTCONDITION   0x0494
#define BIFF12_SORTSTATE_END   0x0495

//QueryTable records
#define BIFF12_QUERYTABLE            0x03BF
#define BIFF12_QUERYTABLE_END        0x03C0
#define BIFF12_QUERYTABLEREFRESH     0x03C1
#define BIFF12_QUERYTABLEREFRESH_END 0x03C2
#define BIFF12_QUERYTABLEFIELDS      0x03C7
#define BIFF12_QUERYTABLEFIELDS_END  0x03C8
#define BIFF12_QUERYTABLEFIELD       0x03C9
#define BIFF12_QUERYTABLEFIELD_END   0x03CA

//Connection records
#define BIFF12_CONNECTIONS        0x03AD
#define BIFF12_CONNECTIONS_END    0x03AE
#define BIFF12_CONNECTION         0x01C9
#define BIFF12_CONNECTION_END     0x01CA
#define BIFF12_DBPR               0x01CB
#define BIFF12_DBPR_END           0x01CC

static std::unordered_map<DWORD, CF_RECORD_TYPE> biff12TypeRecord =
{
    {BIFF12_SI, rt_SST_ITEM},
    {BIFF12_SST, rt_BEGIN_SST},
    {BIFF12_SST_END, rt_END_SST}
};

}
#endif // RECORDTYPES_H
