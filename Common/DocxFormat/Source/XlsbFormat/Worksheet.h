/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef WORKSHEET_H
#define WORKSHEET_H

#include "BaseRecord.h"
namespace XLSB {

    class RowRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 12)
            {
                DWORD row     = GetDword(p + 0);
                DWORD style   = GetDword(p + 4);
                WORD height   = GetWord(p + 8);
                WORD grbits   = GetWord(p + 10);

                bool outline = (grbits & 0x0700) != 0; // 8 levels of outline
                bool resized = (grbits & 0x2000) != 0;
                bool hidden  = (grbits & 0x1000) != 0;

                CHAR s[256];
                sprintf(s, "\tinfo : row=%d, height=%.2f, style=%d, outline=%s, resize=%s, hidden=%s\r\n",
                    row,
                    float(height) / 20,
                    style,
                    outline ? "true":"false",
                    resized ? "true":"false",
                    hidden ?  "true":"false");
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\tinfo : invalid row record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "row";
        }
    };

    class BlankRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 8)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, v:blank\r\n",
                    col,
                    style);
                std::cout<<(s);

            }
            else
            {
                std::cout<<("\t\tinfo : invalid blank record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "blank";
        }
    };

    class NumRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 12)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                double f     = GetFloat(p + 8);

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, v:float=%.6g\r\n",
                    col,
                    style,
                    f);
                std::cout<<(s);

            }
            else
            {
                std::cout<<("\t\tinfo : invalid num record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "num";
        }
    };

    class BoolErrRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 9)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                BYTE b       = GetByte(p + 8);

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, v:boolerr=%d\r\n",
                    col,
                    style,
                    b);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid boolerr record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "boolErr";
        }
    };

    class BoolRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 9)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                BYTE b       = GetByte(p + 8);

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, v:bool=%s\r\n",
                    col,
                    style,
                    b?"true":"false");
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid bool record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "bool";
        }
    };

    class FloatRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 16)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                double f     = GetDouble(p + 8);

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, v:float=%.10g\r\n",
                    col,
                    style,
                    f);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid float record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "float";
        }
    };

    class StringRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 12)
            {
                DWORD col                 = GetDword(p + 0);
                DWORD style               = GetDword(p + 4);
                DWORD sharedstring_index  = GetDword(p + 8);

                WCHAR s[256];
                std::wstring sharedString;
            }
                /*if (sharedstring_index < w._vecSharedStrings.size())
                {

                    sharedString = w._vecSharedStrings[sharedstring_index];

                   swprintf(s, sizeof(s), L"\t\tinfo : col=%d, style=%d, v:stringindex=%d v:string=%s\r\n",
                        col,
                        style,
                        sharedstring_index,
                        sharedString.empty() ? L"" : sharedString.c_str());
                }
                else {

                   swprintf(s, sizeof(s), L"\t\terror : col=%d, style=%d, v:stringindex=%d unknown\r\n",
                        col,
                        style,
                        sharedstring_index);
                }

                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid string record length\r\n");
            }*/

        }

        std::string GetTag()
        {
            return "string";
        }
    };

    class FormulaStringRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 22)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                DWORD lenStr = GetDword(p + 8);

                std::wstring str = GetString(p + 12, lenStr);

                WORD grbits = GetWord(p + 12 + 2 * lenStr);
                bool bCalcOnOpen = (grbits & 0x0010) != 0;

                DWORD lenFormula = GetDword(p + 12 + 2 * lenStr + 2/*grbits*/);

                // formula ptg tokens follow between
                //  p + 12 + 2 * lenStr + 2/*grbits*/ + 4
                //   and
                //  p + 12 + 2 * lenStr + 2/*grbits*/ + 4 + lenFormula

                WCHAR s[256];
               swprintf(s, sizeof(s), L"\t\tinfo : col=%d, style=%d, formula v:calcOnOpen=%s v:string=%s v:lenStr=%d, v:lenFormula=%d\r\n",
                    col,
                    style,
                    bCalcOnOpen ? L"true" : L"false",
                    str.c_str(),
                    lenStr,
                    lenFormula);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid string formula record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "formulaString";
        }
    };

    class FormulaFloatRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 24)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                double d     = GetDouble(p + 8);

                WORD grbits = GetWord(p + 16);
                bool bCalcOnOpen = (grbits & 0x0010) != 0;

                DWORD lenFormula = GetDword(p + 16 + 2/*grbits*/);

                // formula ptg tokens follow between
                //  p + 16 + 2/*grbits*/ + 4
                //   and
                //  p + 16 + 2/*grbits*/ + 4 + lenFormula

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, formula v:calcOnOpen=%s v:float=%.10g v:lenFormula=%d\r\n",
                    col,
                    style,
                    bCalcOnOpen ? L"true" : L"false",
                    d,
                    lenFormula);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid float formula record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "formulaFloat";
        }
    };

    class FormulaBoolRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 19)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                BYTE b       = GetByte(p + 8);

                WORD grbits = GetWord(p + 9);
                bool bCalcOnOpen = (grbits & 0x0010) != 0;

                DWORD lenFormula = GetDword(p + 9 + 2/*grbits*/);

                // formula ptg tokens follow between
                //  p + 9 + 2/*grbits*/ + 4
                //   and
                //  p + 9 + 2/*grbits*/ + 4 + lenFormula

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, formula v:calcOnOpen=%s v:bool=%s v:lenFormula=%d\r\n",
                    col,
                    style,
                    bCalcOnOpen ? "true" : "false",
                    b ? "true":"false",
                    lenFormula);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid bool formula record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "formulaBool";
        }
    };

    class FormulaBoolErrRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 19)
            {
                DWORD col    = GetDword(p + 0);
                DWORD style  = GetDword(p + 4);
                BYTE b       = GetByte(p + 8);

                WORD grbits = GetWord(p + 9);
                bool bCalcOnOpen = (grbits & 0x0010) != 0;

                DWORD lenFormula = GetDword(p + 9 + 2/*grbits*/);

                // formula ptg tokens follow between
                //  p + 9 + 2/*grbits*/ + 4
                //   and
                //  p + 9 + 2/*grbits*/ + 4 + lenFormula

                CHAR s[256];
                sprintf(s, "\t\tinfo : col=%d, style=%d, formula v:calcOnOpen=%s v:boolerr=%d v:lenFormula=%d\r\n",
                    col,
                    style,
                    bCalcOnOpen ? "true" : "false",
                    b,
                    lenFormula);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\t\tinfo : invalid boolerr formula record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "formulaBoolErr";
        }
    };

    class ColRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 18)
            {
                DWORD colmin = GetDword(p + 0);
                DWORD colmax = GetDword(p + 4);
                DWORD width  = GetDword(p + 8);
                DWORD style  = GetDword(p + 12);

                WORD grbits  = GetWord(p + 16);
                bool outline = (grbits & 0x0700) != 0; // 8 levels of outline
                bool resized = (grbits & 0x0002) != 0;
                bool hidden  = (grbits & 0x0001) != 0;

                CHAR s[256];
                sprintf(s, "\tinfo : colmin=%d, colmax=%d, width=%.2f, style=%d, outline=%s, resize=%s, hidden=%s\r\n",
                    colmin,
                    colmax,
                    float(width) / 256,
                    style,
                    outline ? "true":"false",
                    resized ? "true":"false",
                    hidden ?  "true":"false");
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\tinfo : invalid col record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "col";
        }
    };

    class WorksheetRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "worksheet";
        }
    };

    class WorksheetEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/worksheet";
        }
    };

    class SheetViewsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sheetViews";
        }
    };

    class SheetViewsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sheetViews";
        }
    };

    class SheetViewRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sheetView";
        }
    };

    class SheetViewEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sheetView";
        }
    };

    class SheetDataRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sheetData";
        }
    };

    class SheetDataEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sheetData";
        }
    };

    class SheetPRRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            std::cout<<("\tinfo :   <tabColor rgb=.../>\r\n\tinfo :   <outlinePr showOutlineSymbols=.../>\r\n\tinfo :   <pageSetUpPr .../>\r\n\tinfo : </sheetPr>\r\n");
        }

        std::string GetTag()
        {
            return "sheetPr";
        }
    };

    class DimensionRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 16)
            {
                std::cout<<("\tinfo : invalid dimension record length\r\n");
                return;
            }

            DWORD r1 = GetDword(p + 0);
            DWORD r2 = GetDword(p + 4);
            DWORD c1 = GetDword(p + 8);
            DWORD c2 = GetDword(p + 12);

            CHAR s[256];
            sprintf(s, "\tinfo : r1=%d, c1=%d, r2=%d, c2=%d\r\n", r1, c1, r2, c2);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "dimension";
        }
    };

    class SelectionRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "selection";
        }
    };

    class ColsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "cols";
        }
    };

    class ColsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/cols";
        }
    };

    class PageMarginsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "pageMargins";
        }
    };

    class PrintOptionsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "printOptions";
        }
    };

    class PageSetupRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "pageSetup";
        }
    };

    class HeaderFooterRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "headerFooter";
        }
    };

    class SheetFormatPRRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sheetFormatPr";
        }
    };

    class DrawingRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 4)
            {
                std::cout<<("\tinfo : invalid drawing record length\r\n");
                return;
            }

            DWORD lenStr = GetDword(p + 0);
            std::wstring sRId = GetString(p + 4, lenStr);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : r:id=%s r:id:lenStr=%d\r\n",
                sRId.c_str(),
                lenStr);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "drawing";
        }
    };

    class LegacyDrawingRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 4)
            {
                std::cout<<("\tinfo : invalid legacy drawing record length\r\n");
                return;
            }

            DWORD lenStr = GetDword(p + 0);
            std::wstring sRId = GetString(p + 4, lenStr);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : r:id=%s r:id:lenStr=%d\r\n",
                sRId.c_str(),
                lenStr);
            std::cout<<(s);

        }

        std::string GetTag()
        {
            return "legacyDrawing";
        }
    };

    class OLEObjectsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "oleObjects";
        }
    };

    class OLEObjectRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 18)
            {
                std::cout<<("\tinfo : invalid oleobject record length\r\n");
                return;
            }

            DWORD shapeid = GetDword(p + 8);
            DWORD lenProgId = GetDword(p + 14);
            std::wstring sProgId = GetString(p + 14 + 4, lenProgId);
            DWORD lenRId = GetDword(p + 14 + 4 + 2 * lenProgId);
            std::wstring sRId = GetString(p + 14 + 4 + 2 * lenProgId + 4, lenRId);

            WCHAR s[386];
           swprintf(s, sizeof(s), L"\tinfo : shapeId=%d progId=%s progId:lenStr=%d r:id=%s r:id:lenStr=%d\r\n",
                shapeid,
                sProgId.c_str(),
                lenProgId,
                sRId.c_str(),
                lenRId);
            std::cout<<(s);

        }

        std::string GetTag()
        {
            return "oleObject";
        }
    };

    class OLEObjectsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/oleObjects";
        }
    };


    class TablePartsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "tableParts";
        }
    };

    class TablePartRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 4)
            {
                std::cout<<("\tinfo : invalid tablePart record length\r\n");
                return;
            }

            DWORD lenStr = GetDword(p + 0);
            std::wstring sRId = GetString(p + 4, lenStr);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : r:id=%s r:id:lenStr=%d\r\n",
                sRId.c_str(),
                lenStr);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "tablePart";
        }
    };

    class TablePartsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/tableParts";
        }
    };

    class ConditionalFormattingRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "conditionalFormatting";
        }
    };

    class ConditionalFormattingEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/conditionalFormatting";
        }
    };

    class CFRuleRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "cfRule";
        }
    };

    class CFRuleEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/cfRule";
        }
    };

    class IconSetRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "iconSet";
        }
    };

    class IconSetEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/iconSet";
        }
    };

    class DataBarRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "dataBar";
        }
    };

    class DataBarEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/dataBar";
        }
    };

    class ColorScaleRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "colorScale";
        }
    };

    class ColorScaleEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/colorScale";
        }
    };

    class CFVORecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "cfvo";
        }
    };

    class HyperlinkRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 20)
            {
                std::cout<<("\tinfo : invalid hyperlink record length\r\n");
                return;
            }

            DWORD r1 = GetDword(p + 0);
            DWORD r2 = GetDword(p + 4);
            DWORD c1 = GetDword(p + 8);
            DWORD c2 = GetDword(p + 12);

            DWORD lenRId = GetDword(p + 16);
            std::wstring sRId = GetString(p + 20, lenRId);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : r1=%d, c1=%d, r2=%d, c2=%d, rId=%s lenRId=%d\r\n",
                r1,
                c1,
                r2,
                c2,
                sRId.c_str(),
                lenRId);
            std::cout<<(s);

        }

        std::string GetTag()
        {
            return "hyperlink";
        }
    };

    class ColorRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "color";
        }
    };
}
#endif
