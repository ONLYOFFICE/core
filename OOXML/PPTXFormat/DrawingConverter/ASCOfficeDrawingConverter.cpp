/*
 * (c) Copyright Ascensio System SIA 2010-2019
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

#include "ASCOfficeDrawingConverter.h"

#include "../../PPTXFormat/Logic/SpTreeElem.h"
#include "../../PPTXFormat/Logic/Geometry.h"
#include "../../PPTXFormat/Logic/SpTreeElem.h"
#include "../../PPTXFormat/Logic/Shape.h"
#include "../../PPTXFormat/Logic/Pic.h"
#include "../../PPTXFormat/Logic/CxnSp.h"
#include "../../PPTXFormat/Logic/SpTree.h"
#include "../../PPTXFormat/Logic/GraphicFrame.h"
#include "../../PPTXFormat/Logic/Colors/SrgbClr.h"
#include "../../PPTXFormat/Logic/Colors/PrstClr.h"
#include "../../PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../../PPTXFormat/Logic/Colors/SysClr.h"
#include "../../PPTXFormat/Logic/Effects/AlphaModFix.h"
#include "../../PPTXFormat/PPTX.h"
#include "../../PPTXFormat/LegacyDiagramText.h"

#include "../../../MsBinaryFile/PptFile/Drawing/Elements.h"
#include "../../../MsBinaryFile/Common/Vml/PPTXShape/Pptx2PptShapeConverter.h"
#include "../../../MsBinaryFile/Common/Vml/toVmlConvert.h"

#include "../../../DesktopEditor/common/Directory.h"

#include "../../DocxFormat/Diagram/DiagramData.h"
#include "../../DocxFormat/Diagram/DiagramDrawing.h"
#include "../../DocxFormat/Media/Image.h"
#include "../../Common/SimpleTypes_Base.h"
#include "../../Common/SimpleTypes_Vml.h"

#include "../../Binary/Presentation/BinWriters.h"

const double g_emu_koef	= 25.4 * 36000 / 72.0;

const std::wstring _start_xml_object(L"<main xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:x=\"urn:schemas-microsoft-com:office:excel\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\">");
const std::wstring _end_xml_object(L"</main>");

static void GetColorWithEffect(const std::wstring& sColor, const int& R, const int& G, const int& B,
                               int& resR, int& resG, int& resB)
{
    resR = R;
    resG = G;
    resB = B;

    double param = 0;
    std::wstring::size_type pos1 = sColor.find('(');
    std::wstring::size_type pos2 = sColor.find(')');
    if (pos1 == std::wstring::npos || pos2 == std::wstring::npos)
        return;
    if (pos2 < (pos1 + 2))
        return;

	std::wstring s = sColor.substr(pos1 + 1, pos2 - pos1 - 1); 
    param = XmlUtils::GetInteger(s);
    bool isEffect = false;

    if (0 == sColor.find(L"darken"))
    {
        resR = (int)(R * param / 255);
        resG = (int)(G * param / 255);
        resB = (int)(B * param / 255);
        isEffect = true;
    }
    else if (0 == sColor.find(L"lighten"))
    {
        resR = 255 - (int)((255 - R) * param / 255);
        resG = 255 - (int)((255 - G) * param / 255);
        resB = 255 - (int)((255 - B) * param / 255);
        isEffect = true;
    }
    else if (0 == sColor.find(L"add"))
    {
        resR = R + (int)param;
        resG = G + (int)param;
        resB = B + (int)param;
        isEffect = true;
    }
    else if (0 == sColor.find(L"subtract"))
    {
        resR = R - (int)param;
        resG = G - (int)param;
        resB = B - (int)param;
        isEffect = true;
    }
    else if (0 == sColor.find(L"reversesubtract"))
    {
        resR = (int)param - R;
        resG = (int)param - G;
        resB = (int)param - B;
        isEffect = true;
    }
    else if (0 == sColor.find(L"blackwhite"))
    {
        resR = (R < (int)param) ? 0 : 255;
        resG = (G < (int)param) ? 0 : 255;
        resB = (B < (int)param) ? 0 : 255;
        isEffect = true;
    }

    if (isEffect)
    {
        resR = (resR < 0) ? 0 : resR;
        resR = (resR > 255) ? 255 : resR;

        resG = (resG < 0) ? 0 : resG;
        resG = (resG > 255) ? 255 : resG;

        resB = (resB < 0) ? 0 : resB;
        resB = (resB > 255) ? 255 : resB;
    }
}

namespace PPTX2EditorAdvanced
{
    OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::wstring& str)//const
    {
        if (str == _T(""))
            return OOXMLShapes::sptNil;
        switch ((CHAR)str[0])
        {
        case 'a':
            if (str == _T("accentBorderCallout1")) return OOXMLShapes::sptCAccentBorderCallout1;
            if (str == _T("accentBorderCallout2")) return OOXMLShapes::sptCAccentBorderCallout2;
            if (str == _T("accentBorderCallout3")) return OOXMLShapes::sptCAccentBorderCallout3;
            if (str == _T("accentCallout1")) return OOXMLShapes::sptCAccentCallout1;
            if (str == _T("accentCallout2")) return OOXMLShapes::sptCAccentCallout2;
            if (str == _T("accentCallout3")) return OOXMLShapes::sptCAccentCallout3;
            if (str == _T("actionButtonBackPrevious")) return OOXMLShapes::sptCActionButtonBackPrevious;
            if (str == _T("actionButtonBeginning")) return OOXMLShapes::sptCActionButtonBeginning;
            if (str == _T("actionButtonBlank")) return OOXMLShapes::sptCActionButtonBlank;
            if (str == _T("actionButtonDocument")) return OOXMLShapes::sptCActionButtonDocument;
            if (str == _T("actionButtonEnd")) return OOXMLShapes::sptCActionButtonEnd;
            if (str == _T("actionButtonForwardNext")) return OOXMLShapes::sptCActionButtonForwardNext;
            if (str == _T("actionButtonHelp")) return OOXMLShapes::sptCActionButtonHelp;
            if (str == _T("actionButtonHome")) return OOXMLShapes::sptCActionButtonHome;
            if (str == _T("actionButtonInformation")) return OOXMLShapes::sptCActionButtonInformation;
            if (str == _T("actionButtonMovie")) return OOXMLShapes::sptCActionButtonMovie;
            if (str == _T("actionButtonReturn")) return OOXMLShapes::sptCActionButtonReturn;
            if (str == _T("actionButtonSound")) return OOXMLShapes::sptCActionButtonSound;
            if (str == _T("arc")) return OOXMLShapes::sptCArc;
            break;
        case 'b':
            if (str == _T("bentArrow")) return OOXMLShapes::sptCBentArrow;
            if (str == _T("bentConnector2")) return OOXMLShapes::sptCBentConnector2;
            if (str == _T("bentConnector3")) return OOXMLShapes::sptCBentConnector3;
            if (str == _T("bentConnector4")) return OOXMLShapes::sptCBentConnector4;
            if (str == _T("bentConnector5")) return OOXMLShapes::sptCBentConnector5;
            if (str == _T("bentUpArrow")) return OOXMLShapes::sptCBentUpArrow;
            if (str == _T("bevel")) return OOXMLShapes::sptCBevel;
            if (str == _T("blockArc")) return OOXMLShapes::sptCBlockArc;
            if (str == _T("borderCallout1")) return OOXMLShapes::sptCBorderCallout1;
            if (str == _T("borderCallout2")) return OOXMLShapes::sptCBorderCallout2;
            if (str == _T("borderCallout3")) return OOXMLShapes::sptCBorderCallout3;
            if (str == _T("bracePair")) return OOXMLShapes::sptCBracePair;
            if (str == _T("bracketPair")) return OOXMLShapes::sptCBracketPair;
            break;
        case 'c':
            if (str == _T("callout1")) return OOXMLShapes::sptCCallout1;
            if (str == _T("callout2")) return OOXMLShapes::sptCCallout2;
            if (str == _T("callout3")) return OOXMLShapes::sptCCallout3;
            if (str == _T("can")) return OOXMLShapes::sptCCan;
            if (str == _T("chartPlus")) return OOXMLShapes::sptCChartPlus;
            if (str == _T("chartStar")) return OOXMLShapes::sptCChartStar;
            if (str == _T("chartX")) return OOXMLShapes::sptCChartX;
            if (str == _T("chevron")) return OOXMLShapes::sptCChevron;
            if (str == _T("chord")) return OOXMLShapes::sptCChord;
            if (str == _T("circularArrow")) return OOXMLShapes::sptCCircularArrow;
            if (str == _T("cloud")) return OOXMLShapes::sptCCloud;
            if (str == _T("cloudCallout")) return OOXMLShapes::sptCCloudCallout;
            if (str == _T("corner")) return OOXMLShapes::sptCCorner;
            if (str == _T("cornerTabs")) return OOXMLShapes::sptCCornerTabs;
            if (str == _T("cube")) return OOXMLShapes::sptCCube;
            if (str == _T("curvedConnector2")) return OOXMLShapes::sptCCurvedConnector2;
            if (str == _T("curvedConnector3")) return OOXMLShapes::sptCCurvedConnector3;
            if (str == _T("curvedConnector4")) return OOXMLShapes::sptCCurvedConnector4;
            if (str == _T("curvedConnector5")) return OOXMLShapes::sptCCurvedConnector5;
            if (str == _T("curvedDownArrow")) return OOXMLShapes::sptCCurvedDownArrow;
            if (str == _T("curvedLeftArrow")) return OOXMLShapes::sptCCurvedLeftArrow;
            if (str == _T("curvedRightArrow")) return OOXMLShapes::sptCCurvedRightArrow;
            if (str == _T("curvedUpArrow")) return OOXMLShapes::sptCCurvedUpArrow;
            break;
        case 'd':
            if (str == _T("decagon")) return OOXMLShapes::sptCDecagon;
            if (str == _T("diagStripe")) return OOXMLShapes::sptCDiagStripe;
            if (str == _T("diamond")) return OOXMLShapes::sptCDiamond;
            if (str == _T("dodecagon")) return OOXMLShapes::sptCDodecagon;
            if (str == _T("donut")) return OOXMLShapes::sptCDonut;
            if (str == _T("doubleWave")) return OOXMLShapes::sptCDoubleWave;
            if (str == _T("downArrow")) return OOXMLShapes::sptCDownArrow;
            if (str == _T("downArrowCallout")) return OOXMLShapes::sptCDownArrowCallout;
            break;
        case 'e':
            if (str == _T("ellipse")) return OOXMLShapes::sptCEllipse;
            if (str == _T("ellipseRibbon")) return OOXMLShapes::sptCEllipseRibbon;
            if (str == _T("ellipseRibbon2")) return OOXMLShapes::sptCEllipseRibbon2;
            break;
        case 'f':
            if (str == _T("flowChartAlternateProcess")) return OOXMLShapes::sptCFlowChartAlternateProcess;
            if (str == _T("flowChartCollate")) return OOXMLShapes::sptCFlowChartCollate;
            if (str == _T("flowChartConnector")) return OOXMLShapes::sptCFlowChartConnector;
            if (str == _T("flowChartDecision")) return OOXMLShapes::sptCFlowChartDecision;
            if (str == _T("flowChartDelay")) return OOXMLShapes::sptCFlowChartDelay;
            if (str == _T("flowChartDisplay")) return OOXMLShapes::sptCFlowChartDisplay;
            if (str == _T("flowChartDocument")) return OOXMLShapes::sptCFlowChartDocument;
            if (str == _T("flowChartExtract")) return OOXMLShapes::sptCFlowChartExtract;
            if (str == _T("flowChartInputOutput")) return OOXMLShapes::sptCFlowChartInputOutput;
            if (str == _T("flowChartInternalStorage")) return OOXMLShapes::sptCFlowChartInternalStorage;
            if (str == _T("flowChartMagneticDisk")) return OOXMLShapes::sptCFlowChartMagneticDisk;
            if (str == _T("flowChartMagneticDrum")) return OOXMLShapes::sptCFlowChartMagneticDrum;
            if (str == _T("flowChartMagneticTape")) return OOXMLShapes::sptCFlowChartMagneticTape;
            if (str == _T("flowChartManualInput")) return OOXMLShapes::sptCFlowChartManualInput;
            if (str == _T("flowChartManualOperation")) return OOXMLShapes::sptCFlowChartManualOperation;
            if (str == _T("flowChartMerge")) return OOXMLShapes::sptCFlowChartMerge;
            if (str == _T("flowChartMultidocument")) return OOXMLShapes::sptCFlowChartMultidocument;
            if (str == _T("flowChartOfflineStorage")) return OOXMLShapes::sptCFlowChartOfflineStorage;
            if (str == _T("flowChartOffpageConnector")) return OOXMLShapes::sptCFlowChartOffpageConnector;
            if (str == _T("flowChartOnlineStorage")) return OOXMLShapes::sptCFlowChartOnlineStorage;
            if (str == _T("flowChartOr")) return OOXMLShapes::sptCFlowChartOr;
            if (str == _T("flowChartPredefinedProcess")) return OOXMLShapes::sptCFlowChartPredefinedProcess;
            if (str == _T("flowChartPreparation")) return OOXMLShapes::sptCFlowChartPreparation;
            if (str == _T("flowChartProcess")) return OOXMLShapes::sptCFlowChartProcess;
            if (str == _T("flowChartPunchedCard")) return OOXMLShapes::sptCFlowChartPunchedCard;
            if (str == _T("flowChartPunchedTape")) return OOXMLShapes::sptCFlowChartPunchedTape;
            if (str == _T("flowChartSort")) return OOXMLShapes::sptCFlowChartSort;
            if (str == _T("flowChartSummingJunction")) return OOXMLShapes::sptCFlowChartSummingJunction;
            if (str == _T("flowChartTerminator")) return OOXMLShapes::sptCFlowChartTerminator;
            if (str == _T("foldedCorner")) return OOXMLShapes::sptCFoldedCorner;
            if (str == _T("frame")) return OOXMLShapes::sptCFrame;
            if (str == _T("funnel")) return OOXMLShapes::sptCFunnel;
            break;
        case 'g':
            if (str == _T("gear6")) return OOXMLShapes::sptCGear6;
            if (str == _T("gear9")) return OOXMLShapes::sptCGear9;
            break;
        case 'h':
            if (str == _T("halfFrame")) return OOXMLShapes::sptCHalfFrame;
            if (str == _T("heart")) return OOXMLShapes::sptCHeart;
            if (str == _T("heptagon")) return OOXMLShapes::sptCHeptagon;
            if (str == _T("hexagon")) return OOXMLShapes::sptCHexagon;
            if (str == _T("homePlate")) return OOXMLShapes::sptCHomePlate;
            if (str == _T("horizontalScroll")) return OOXMLShapes::sptCHorizontalScroll;
            break;
        case 'i':
            if (str == _T("irregularSeal1")) return OOXMLShapes::sptCIrregularSeal1;
            if (str == _T("irregularSeal2")) return OOXMLShapes::sptCIrregularSeal2;
            break;
        case 'l':
            if (str == _T("leftArrow")) return OOXMLShapes::sptCLeftArrow;
            if (str == _T("leftArrowCallout")) return OOXMLShapes::sptCLeftArrowCallout;
            if (str == _T("leftBrace")) return OOXMLShapes::sptCLeftBrace;
            if (str == _T("leftBracket")) return OOXMLShapes::sptCLeftBracket;
            if (str == _T("leftCircularArrow")) return OOXMLShapes::sptCLeftCircularArrow;
            if (str == _T("leftRightArrow")) return OOXMLShapes::sptCLeftRightArrow;
            if (str == _T("leftRightArrowCallout")) return OOXMLShapes::sptCLeftRightArrowCallout;
            if (str == _T("leftRightCircularArrow")) return OOXMLShapes::sptCLeftRightCircularArrow;
            if (str == _T("leftRightRibbon")) return OOXMLShapes::sptCLeftRightRibbon;
            if (str == _T("leftRightUpArrow")) return OOXMLShapes::sptCLeftRightUpArrow;
            if (str == _T("leftUpArrow")) return OOXMLShapes::sptCLeftUpArrow;
            if (str == _T("lightningBolt")) return OOXMLShapes::sptCLightningBolt;
            if (str == _T("line")) return OOXMLShapes::sptCLine;
            if (str == _T("lineInv")) return OOXMLShapes::sptCLineInv;
            break;
        case 'm':
            if (str == _T("mathDivide")) return OOXMLShapes::sptCMathDivide;
            if (str == _T("mathEqual")) return OOXMLShapes::sptCMathEqual;
            if (str == _T("mathMinus")) return OOXMLShapes::sptCMathMinus;
            if (str == _T("mathMultiply")) return OOXMLShapes::sptCMathMultiply;
            if (str == _T("mathNotEqual")) return OOXMLShapes::sptCMathNotEqual;
            if (str == _T("mathPlus")) return OOXMLShapes::sptCMathPlus;
            if (str == _T("moon")) return OOXMLShapes::sptCMoon;
            break;
        case 'n':
            if (str == _T("nonIsoscelesTrapezoid")) return OOXMLShapes::sptCNonIsoscelesTrapezoid;
            if (str == _T("noSmoking")) return OOXMLShapes::sptCNoSmoking;
            if (str == _T("notchedRightArrow")) return OOXMLShapes::sptCNotchedRightArrow;
            break;
        case 'o':
            if (str == _T("octagon")) return OOXMLShapes::sptCOctagon;
            break;
        case 'p':
            if (str == _T("parallelogram")) return OOXMLShapes::sptCParallelogram;
            if (str == _T("pentagon")) return OOXMLShapes::sptCPentagon;
            if (str == _T("pie")) return OOXMLShapes::sptCPie;
            if (str == _T("pieWedge")) return OOXMLShapes::sptCPieWedge;
            if (str == _T("plaque")) return OOXMLShapes::sptCPlaque;
            if (str == _T("plaqueTabs")) return OOXMLShapes::sptCPlaqueTabs;
            if (str == _T("plus")) return OOXMLShapes::sptCPlus;
            break;
        case 'q':
            if (str == _T("quadArrow")) return OOXMLShapes::sptCQuadArrow;
            if (str == _T("quadArrowCallout")) return OOXMLShapes::sptCQuadArrowCallout;
            break;
        case 'r':
            if (str == _T("rect")) return OOXMLShapes::sptCRect;
            if (str == _T("ribbon")) return OOXMLShapes::sptCRibbon;
            if (str == _T("ribbon2")) return OOXMLShapes::sptCRibbon2;
            if (str == _T("rightArrow")) return OOXMLShapes::sptCRightArrow;
            if (str == _T("rightArrowCallout")) return OOXMLShapes::sptCRightArrowCallout;
            if (str == _T("rightBrace")) return OOXMLShapes::sptCRightBrace;
            if (str == _T("rightBracket")) return OOXMLShapes::sptCRightBracket;
            if (str == _T("round1Rect")) return OOXMLShapes::sptCRound1Rect;
            if (str == _T("round2DiagRect")) return OOXMLShapes::sptCRound2DiagRect;
            if (str == _T("round2SameRect")) return OOXMLShapes::sptCRound2SameRect;
            if (str == _T("roundRect")) return OOXMLShapes::sptCRoundRect;
            if (str == _T("rtTriangle")) return OOXMLShapes::sptCRtTriangle;
            break;
        case 's':
            if (str == _T("smileyFace")) return OOXMLShapes::sptCSmileyFace;
            if (str == _T("snip1Rect")) return OOXMLShapes::sptCSnip1Rect;
            if (str == _T("snip2DiagRect")) return OOXMLShapes::sptCSnip2DiagRect;
            if (str == _T("snip2SameRect")) return OOXMLShapes::sptCSnip2SameRect;
            if (str == _T("snipRoundRect")) return OOXMLShapes::sptCSnipRoundRect;
            if (str == _T("squareTabs")) return OOXMLShapes::sptCSquareTabs;
            if (str == _T("star10")) return OOXMLShapes::sptCStar10;
            if (str == _T("star12")) return OOXMLShapes::sptCStar12;
            if (str == _T("star16")) return OOXMLShapes::sptCStar16;
            if (str == _T("star24")) return OOXMLShapes::sptCStar24;
            if (str == _T("star32")) return OOXMLShapes::sptCStar32;
            if (str == _T("star4")) return OOXMLShapes::sptCStar4;
            if (str == _T("star5")) return OOXMLShapes::sptCStar5;
            if (str == _T("star6")) return OOXMLShapes::sptCStar6;
            if (str == _T("star7")) return OOXMLShapes::sptCStar7;
            if (str == _T("star8")) return OOXMLShapes::sptCStar8;
            if (str == _T("straightConnector1")) return OOXMLShapes::sptCStraightConnector1;
            if (str == _T("stripedRightArrow")) return OOXMLShapes::sptCStripedRightArrow;
            if (str == _T("sun")) return OOXMLShapes::sptCSun;
            if (str == _T("swooshArrow")) return OOXMLShapes::sptCSwooshArrow;
            break;
        case 't':
            if (str == _T("teardrop")) return OOXMLShapes::sptCTeardrop;
            if (str == _T("trapezoid")) return OOXMLShapes::sptCTrapezoid;
            if (str == _T("triangle")) return OOXMLShapes::sptCTriangle;
            break;
        case 'u':
            if (str == _T("upArrow")) return OOXMLShapes::sptCUpArrow;
            if (str == _T("upArrowCallout")) return OOXMLShapes::sptCUpArrowCallout;
            if (str == _T("upDownArrow")) return OOXMLShapes::sptCUpDownArrow;
            if (str == _T("upDownArrowCallout")) return OOXMLShapes::sptCUpDownArrowCallout;
            if (str == _T("uturnArrow")) return OOXMLShapes::sptCUturnArrow;
            break;
        case 'v':
            if (str == _T("verticalScroll")) return OOXMLShapes::sptCVerticalScroll;
            break;
        case 'w':
            if (str == _T("wave")) return OOXMLShapes::sptCWave;
            if (str == _T("wedgeEllipseCallout")) return OOXMLShapes::sptCWedgeEllipseCallout;
            if (str == _T("wedgeRectCallout")) return OOXMLShapes::sptCWedgeRectCallout;
            if (str == _T("wedgeRoundRectCallout")) return OOXMLShapes::sptCWedgeRoundRectCallout;
            break;
        }
        return OOXMLShapes::sptNil;
    }
}


namespace NS_DWC_Common
{
	void CorrentCropString(std::wstring& s)
	{
		size_t nLen = s.length();
		if (nLen > 0 && (s[nLen - 1] == ((wchar_t)'f')))
		{
			s.erase(nLen - 1);
			int nVal = XmlUtils::GetInteger(s);
			double dKoef = 100000.0 / 65536;
			nVal = (int)(dKoef * nVal + 0.5);
			s = std::to_wstring(nVal);
		}
	}

	BYTE getOpacityFromString(const std::wstring opacityStr)
	{
		BYTE alpha;
		if (opacityStr.find(L"f") != -1)
			alpha = (BYTE)(XmlUtils::GetDouble(opacityStr) / 65536 * 256);
		else
		{
			if (0 == opacityStr.find(L"."))
			{
				std::wstring str = L"0" + opacityStr;
				alpha = (BYTE)(XmlUtils::GetDouble(str) * 256);
			}
			else
				alpha = (BYTE)(XmlUtils::GetDouble(opacityStr) * 256);
		}
		return alpha;
	}

	long getRealFromString(const std::wstring& str)
	{
		long val = 0;

		if (str.find(L"f") != -1)
			val = XmlUtils::GetInteger(str);
		else
			val = (long)(XmlUtils::GetDouble(str) * 65536);

		return val;
	}

	int getRotateAngle(const std::wstring& str, const nullable_bool& flipX, const nullable_bool& flipY)
	{
		bool bIsInvertAngle = false;

		int nCheckInvert = 0;

		if (flipX.is_init() && flipX.get() == true)
			nCheckInvert += 1;
		if (flipY.is_init() && flipY.get() == true)
			nCheckInvert += 1;

		int nRot = XmlUtils::GetInteger(str);
		if (str.rfind(wchar_t('f')) != -1)
		{
			double dVal = (double)nRot;
			dVal /= 65536;

			if (nCheckInvert == 1)
			{
				dVal = -dVal;
			}

			if (dVal > 360)
			{
				int nPart = (int)(dVal / 360);
				dVal = dVal - nPart * 360;
			}
			else if (dVal < 0)
			{
				int nPart = (int)(dVal / 360);
				nPart = 1 - nPart;
				dVal = dVal + nPart * 360;
			}

			nRot = (int)(dVal * 60000);
		}
		else
		{
			if (nCheckInvert == 1)
			{
				nRot = -nRot;
			}

			if (nRot > 360)
			{
				int nPart = (int)(nRot / 360);
				nRot = nRot - nPart * 360;
			}
			else if (nRot < 0)
			{
				int nPart = (int)(nRot / 360);
				nPart = 1 - nPart;
				nRot = nRot + nPart * 360;
			}

			nRot *= 60000;
		}

		return nRot;
	}
	const static int shemeDefaultColor[] =
	{
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,
		0x00000000,	0x00FFFFFF,	0x00FF0000,	0x0000FF00,	0x000000FF,	0x00FFFF00,	0x00FF00FF,	0x0000FFFF,
		0x00800000,	0x00008000,	0x00000080,	0x00808000,	0x00800080,	0x00008080,	0x00C0C0C0,	0x00808080,
		0x009999FF,	0x00993366,	0x00FFFFCC,	0x00CCFFFF,	0x00660066,	0x00FF8080,	0x000066CC,	0x00CCCCFF,
		0x00000080,	0x00FF00FF,	0x00FFFF00,	0x0000FFFF,	0x00800080,	0x00800000,	0x00008080,	0x000000FF,
		0x0000CCFF,	0x00CCFFFF,	0x00CCFFCC,	0x00FFFF99,	0x0099CCFF,	0x00FF99CC,	0x00CC99FF,	0x00FFCC99,
		0x003366FF,	0x0033CCCC,	0x0099CC00,	0x00FFCC00,	0x00FF9900,	0x00FF6600,	0x00666699,	0x00969696,
		0x00003366,	0x00339966,	0x00003300,	0x00333300,	0x00993300,	0x00993366,	0x00333399,	0x00333333
	};
	//const static int controlPanelColors1[] = 
	//{
	//	0x00FFFFFF,	0x00CCCCCC,	0x00FFFFFF,	0x006363CE,
	//	0x00DDDDDD,	0x00DDDDDD,	0x00888888,	0x00000000,	
	//	0x00000000,	0x00808080,	0x00B5D5FF,	0x00000000,	
	//	0x00FFFFFF,	0x00FFFFFF,	0x007F7F7F,	0x00FBFCC5,	
	//	0x00000000,	0x00F7F7F7,	0x00000000,	0x00FFFFFF,	
	//	0x00666666,	0x00C0C0C0,	0x00DDDDDD,	0x00C0C0C0,	
	//	0x00888888,	0x00FFFFFF,	0x00CCCCCC,	0x00000000
	//};
	//const static int controlPanelColors2[] = 
	//{
	//	0x00000000,	0x00FFFFFF,	0x00000000,	0x00FFFFFF,
	//	0x00000000,	0x00000000,	0x00000000,	0x00FFFFFF,	
	//	0x00FFFFFF,	0x00000000,	0x00FFFFFF,	0x00FFFFFF,	
	//	0x00000000,	0x00000000,	0x00000000,	0x00000000,	
	//	0x00FFFFFF,	0x00FFFFFF,	0x00FFFFFF,	0x00000000,	
	//	0x00FFFFFF,	0x00000000,	0x00000000,	0x00000000,	
	//	0x00000000,	0x00000000,	0x00FFFFFF,	0x00FFFFFF
	//};
	bool getColorFromString(const std::wstring& colorStr, ODRAW::CColor & color)
	{
		bool bSet = false;
		if (colorStr.find(L"#") != std::wstring::npos)
		{
			if (colorStr.length() == 4)
			{
				int lColor = XmlUtils::GetColorBGR(colorStr.substr(1, 3));
				BYTE lB = ((lColor >> 16) & 0x0F);
				BYTE lG = ((lColor >> 20) & 0x0F);
				BYTE lR = ((lColor >> 8) & 0x0F);

				color.R = ((lR << 4) + lR);
				color.G = ((lG << 4) + lG);
				color.B = ((lB << 4) + lB);
				color.A = 0;
			}
			else
			{
				int lColor = XmlUtils::GetColorBGR(colorStr.substr(1, 6));
				color.R = (BYTE)(lColor >> 0);
				color.G = (BYTE)(lColor >> 8);
				color.B = (BYTE)(lColor >> 16);
				color.A = 0;
			}
			bSet = true;
		}
		else
		{
			int RGB = 0;
			std::wstring str;

			size_t pos = colorStr.find(L"["); //"buttonFace [67]"
			if (pos != std::wstring::npos)
			{
				size_t pos1 = colorStr.find(L"]", pos + 1);
				if (pos1 != std::wstring::npos)
				{
					str = colorStr.substr(pos + 1, pos1 - pos - 1);

					if (NSStringUtils::IsNumber(str))
					{
						int index = XmlUtils::GetInteger(str);
						if (index < 64)
						{
							RGB = shemeDefaultColor[index];
							bSet = true;
						}
					}
				}
				//else if (index < 93)
				//{
				//	RGB = background ? controlPanelColors2[index - 65] : controlPanelColors1[index - 65];
				//	bSet = true;
				//}
			}

			if (!bSet)
			{
				pos = colorStr.find(' ');
				if (pos == std::wstring::npos)
					str = colorStr;
				else
					str = colorStr.substr(0, pos);

				switch (str[0])
				{
				case 'a':
					if (str == L"aliceBlue") { RGB = 0xF0F8FF; bSet = true; break; } // (Alice Blue Preset Color)		Specifies a color with RGB value (240,248,255)
					else if (str == L"antiqueWhite") { RGB = 0xFAEBD7; bSet = true; break; } // (Antique White Preset Color)	Specifies a color with RGB value (250,235,215)
					else if (str == L"aqua") { RGB = 0x00FFFF; bSet = true; break; } // (Aqua Preset Color)				Specifies a color with RGB value (0,255,255)
					else if (str == L"aquamarine") { RGB = 0x7FFFD4; bSet = true; break; } // (Aquamarine Preset Color)		Specifies a color with RGB value (127,255,212)
					else if (str == L"azure") { RGB = 0xF0FFFF; bSet = true; break; } // (Azure Preset Color)			Specifies a color with RGB value (240,255,255)
					break;
				case 'b':
					if (str == L"beige") { RGB = 0xF5F5DC; bSet = true; break; } // (Beige Preset Color)			Specifies a color with RGB value (245,245,220)
					else if (str == L"bisque") { RGB = 0xFFE4C4; bSet = true; break; } // (Bisque Preset Color)			Specifies a color with RGB value (255,228,196)
					else if (str == L"black") { RGB = 0x000000; bSet = true; break; } // (Black Preset Color)			Specifies a color with RGB value (0,0,0)
					else if (str == L"blanchedAlmond") { RGB = 0xFFEBCD; bSet = true; break; } // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205)
					else if (str == L"blue") { RGB = 0x0000FF; bSet = true; break; } // (Blue Preset Color)				Specifies a color with RGB value (0,0,255)
					else if (str == L"blueViolet") { RGB = 0x8A2BE2; bSet = true; break; } // (Blue Violet Preset Color)		Specifies a color with RGB value (138,43,226)
					else if (str == L"brown") { RGB = 0xA52A2A; bSet = true; break; } // (Brown Preset Color)			Specifies a color with RGB value (165,42,42)
					else if (str == L"burlyWood") { RGB = 0xDEB887; bSet = true; break; } // (Burly Wood Preset Color)		Specifies a color with RGB value (222,184,135)
					else if (str == L"buttonFace") { RGB = 0xF0F0F0; bSet = true; break; }
					else if (str == L"buttonText") { RGB = 0x000000; bSet = true; break; }
					break;
				case 'c':
					if (str == L"cadetBlue") { RGB = 0x5F9EA0; bSet = true; break; } // (Cadet Blue Preset Color)		Specifies a color with RGB value (95,158,160)
					else if (str == L"chartreuse") { RGB = 0x7FFF00; bSet = true; break; } // (Chartreuse Preset Color)		Specifies a color with RGB value (127,255,0)
					else if (str == L"chocolate") { RGB = 0xD2691E; bSet = true; break; } // (Chocolate Preset Color)		Specifies a color with RGB value (210,105,30)
					else if (str == L"coral") { RGB = 0xFF7F50; bSet = true; break; } // (Coral Preset Color)			Specifies a color with RGB value (255,127,80)
					else if (str == L"cornflowerBlue") { RGB = 0x6495ED; bSet = true; break; } // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237)
					else if (str == L"cornsilk") { RGB = 0xFFF8DC; bSet = true; break; } // (Cornsilk Preset Color)			Specifies a color with RGB value (255,248,220)
					else if (str == L"crimson") { RGB = 0xDC143C; bSet = true; break; } // (Crimson Preset Color)			Specifies a color with RGB value (220,20,60)
					else if (str == L"cyan") { RGB = 0x00FFFF; bSet = true; break; } // (Cyan Preset Color)				Specifies a color with RGB value (0,255,255)
					break;
				case 'd':
					if (str == L"darkBlue") { RGB = 0x00008B; bSet = true; break; } // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
					else if (str == L"darkCyan") { RGB = 0x008B8B; bSet = true; break; } // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
					else if (str == L"darkGoldenrod") { RGB = 0xB8860B; bSet = true; break; } // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
					else if (str == L"darkGray") { RGB = 0xA9A9A9; bSet = true; break; } // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					else if (str == L"darkGreen") { RGB = 0x006400; bSet = true; break; } // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
					else if (str == L"darkGrey") { RGB = 0xA9A9A9; bSet = true; break; } // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					else if (str == L"darkKhaki") { RGB = 0xBDB76B; bSet = true; break; } // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
					else if (str == L"darkMagenta") { RGB = 0x8B008B; bSet = true; break; } // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
					else if (str == L"darkOliveGreen") { RGB = 0x556B2F; bSet = true; break; } // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
					else if (str == L"darkOrange") { RGB = 0xFF8C00; bSet = true; break; } // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
					else if (str == L"darkOrchid") { RGB = 0x9932CC; bSet = true; break; } // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
					else if (str == L"darkRed") { RGB = 0x8B0000; bSet = true; break; } // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
					else if (str == L"darkSalmon") { RGB = 0xE9967A; bSet = true; break; } // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
					else if (str == L"darkSeaGreen") { RGB = 0x8FBC8F; bSet = true; break; } // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,143)
					else if (str == L"darkSlateBlue") { RGB = 0x483D8B; bSet = true; break; } // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
					else if (str == L"darkSlateGray") { RGB = 0x2F4F4F; bSet = true; break; } // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					else if (str == L"darkSlateGrey") { RGB = 0x2F4F4F; bSet = true; break; } // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					else if (str == L"darkTurquoise") { RGB = 0x00CED1; bSet = true; break; } // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
					else if (str == L"darkViolet") { RGB = 0x9400D3; bSet = true; break; } // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
					else if (str == L"deepPink") { RGB = 0xFF1493; bSet = true; break; } // (Deep Pink Preset Color)		Specifies a color with RGB value (255,20,147)
					else if (str == L"deepSkyBlue") { RGB = 0x00BFFF; bSet = true; break; } // (Deep Sky Blue Preset Color)	Specifies a color with RGB value (0,191,255)
					else if (str == L"dimGray") { RGB = 0x696969; bSet = true; break; } // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
					else if (str == L"dimGrey") { RGB = 0x696969; bSet = true; break; } // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
					else if (str == L"dkBlue") { RGB = 0x00008B; bSet = true; break; } // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
					else if (str == L"dkCyan") { RGB = 0x008B8B; bSet = true; break; } // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
					else if (str == L"dkGoldenrod") { RGB = 0xB8860B; bSet = true; break; } // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
					else if (str == L"dkGray") { RGB = 0xA9A9A9; bSet = true; break; } // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					else if (str == L"dkGreen") { RGB = 0x006400; bSet = true; break; } // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
					else if (str == L"dkGrey") { RGB = 0xA9A9A9; bSet = true; break; } // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					else if (str == L"dkKhaki") { RGB = 0xBDB76B; bSet = true; break; } // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
					else if (str == L"dkMagenta") { RGB = 0x8B008B; bSet = true; break; } // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
					else if (str == L"dkOliveGreen") { RGB = 0x556B2F; bSet = true; break; } // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
					else if (str == L"dkOrange") { RGB = 0xFF8C00; bSet = true; break; } // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
					else if (str == L"dkOrchid") { RGB = 0x9932CC; bSet = true; break; } // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
					else if (str == L"dkRed") { RGB = 0x8B0000; bSet = true; break; } // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
					else if (str == L"dkSalmon") { RGB = 0xE9967A; bSet = true; break; } // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
					else if (str == L"dkSeaGreen") { RGB = 0x8FBC8B; bSet = true; break; } // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,139)
					else if (str == L"dkSlateBlue") { RGB = 0x483D8B; bSet = true; break; } // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
					else if (str == L"dkSlateGray") { RGB = 0x2F4F4F; bSet = true; break; } // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					else if (str == L"dkSlateGrey") { RGB = 0x2F4F4F; bSet = true; break; } // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					else if (str == L"dkTurquoise") { RGB = 0x00CED1; bSet = true; break; } // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
					else if (str == L"dkViolet") { RGB = 0x9400D3; bSet = true; break; } // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
					else if (str == L"dodgerBlue") { RGB = 0x1E90FF; bSet = true; break; } // (Dodger Blue Preset Color)		Specifies a color with RGB value (30,144,255)
					break;
				case 'f':
					if (str == L"firebrick") { RGB = 0xB22222; bSet = true; break; } // (Firebrick Preset Color)		Specifies a color with RGB value (178,34,34)
					else if (str == L"floralWhite") { RGB = 0xFFFAF0; bSet = true; break; } // (Floral White Preset Color)		Specifies a color with RGB value (255,250,240)
					else if (str == L"forestGreen") { RGB = 0x228B22; bSet = true; break; } // (Forest Green Preset Color)		Specifies a color with RGB value (34,139,34)
					else if (str == L"fuchsia") { RGB = 0xFF00FF; bSet = true; break; } // (Fuchsia Preset Color)			Specifies a color with RGB value (255,0,255)
					break;
				case 'g':
					if (str == L"gainsboro") { RGB = 0xDCDCDC; bSet = true; break; } // (Gainsboro Preset Color)		Specifies a color with RGB value (220,220,220)
					else if (str == L"ghostWhite") { RGB = 0xF8F8FF; bSet = true; break; } // (Ghost White Preset Color)		Specifies a color with RGB value (248,248,255)
					else if (str == L"gold") { RGB = 0xFFD700; bSet = true; break; } // (Gold Preset Color)				Specifies a color with RGB value (255,215,0)
					else if (str == L"goldenrod") { RGB = 0xDAA520; bSet = true; break; } // (Goldenrod Preset Color)		Specifies a color with RGB value (218,165,32)
					else if (str == L"gray") { RGB = 0x808080; bSet = true; break; } // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
					else if (str == L"green") { RGB = 0x008000; bSet = true; break; } // (Green Preset Color)				Specifies a color with RGB value (0,128,0)
					else if (str == L"greenYellow") { RGB = 0xADFF2F; bSet = true; break; } // (Green Yellow Preset Color)		Specifies a color with RGB value (173,255,47)
					else if (str == L"grey") { RGB = 0x808080; bSet = true; break; } // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
					break;
				case 'h':
					if (str == L"honeydew") { RGB = 0xF0FFF0; bSet = true; break; } // (Honeydew Preset Color)			Specifies a color with RGB value (240,255,240)
					else if (str == L"hotPink") { RGB = 0xFF69B4; bSet = true; break; } // (Hot Pink Preset Color)			Specifies a color with RGB value (255,105,180)
					break;
				case 'i':
					if (str == L"indianRed") { RGB = 0xCD5C5C; bSet = true; break; } // (Indian Red Preset Color)		Specifies a color with RGB value (205,92,92)
					else if (str == L"indigo") { RGB = 0x4B0082; bSet = true; break; } // (Indigo Preset Color)			Specifies a color with RGB value (75,0,130)
					else if (str == L"ivory") { RGB = 0xFFFFF0; bSet = true; break; } // (Ivory Preset Color)			Specifies a color with RGB value (255,255,240)
					else if (str == L"infoText") { RGB = 0x000000; bSet = true; break; }
					break;
				case 'k':
					if (str == L"khaki") { RGB = 0xF0E68C; bSet = true; break; } // (Khaki Preset Color)			Specifies a color with RGB value (240,230,140)
					break;
				case 'l':
					if (str == L"lavender") { RGB = 0xE6E6FA; bSet = true; break; } // (Lavender Preset Color)			Specifies a color with RGB value (230,230,250)
					else if (str == L"lavenderBlush") { RGB = 0xFFF0F5; bSet = true; break; } // (Lavender Blush Preset Color)	Specifies a color with RGB value (255,240,245)
					else if (str == L"lawnGreen") { RGB = 0x7CFC00; bSet = true; break; } // (Lawn Green Preset Color)		Specifies a color with RGB value (124,252,0)
					else if (str == L"lemonChiffon") { RGB = 0xFFFACD; bSet = true; break; } // (Lemon Chiffon Preset Color)	Specifies a color with RGB value (255,250,205)
					else if (str == L"lightBlue") { RGB = 0xADD8E6; bSet = true; break; } // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
					else if (str == L"lightCoral") { RGB = 0xF08080; bSet = true; break; } // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
					else if (str == L"lightCyan") { RGB = 0xE0FFFF; bSet = true; break; } // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
					else if (str == L"lightGoldenrodYellow") { RGB = 0xFAFAD2; bSet = true; break; } // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,210)
					else if (str == L"lightGray") { RGB = 0xD3D3D3; bSet = true; break; } // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					else if (str == L"lightGreen") { RGB = 0x90EE90; bSet = true; break; } // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
					else if (str == L"lightGrey") { RGB = 0xD3D3D3; bSet = true; break; } // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					else if (str == L"lightPink") { RGB = 0xFFB6C1; bSet = true; break; } // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
					else if (str == L"lightSalmon") { RGB = 0xFFA07A; bSet = true; break; } // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
					else if (str == L"lightSeaGreen") { RGB = 0x20B2AA; bSet = true; break; } // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
					else if (str == L"lightSkyBlue") { RGB = 0x87CEFA; bSet = true; break; } // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
					else if (str == L"lightSlateGray") { RGB = 0x778899; bSet = true; break; } // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					else if (str == L"lightSlateGrey") { RGB = 0x778899; bSet = true; break; } // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					else if (str == L"lightSteelBlue") { RGB = 0xB0C4DE; bSet = true; break; } // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
					else if (str == L"lightYellow") { RGB = 0xFFFFE0; bSet = true; break; } // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
					else if (str == L"lime") { RGB = 0x00FF00; bSet = true; break; } // (Lime Preset Color)				Specifies a color with RGB value (0,255,0)
					else if (str == L"limeGreen") { RGB = 0x32CD32; bSet = true; break; } // (Lime Green Preset Color)		Specifies a color with RGB value (50,205,50)
					else if (str == L"linen") { RGB = 0xFAF0E6; bSet = true; break; } // (Linen Preset Color)			Specifies a color with RGB value (250,240,230)
					else if (str == L"ltBlue") { RGB = 0xADD8E6; bSet = true; break; } // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
					else if (str == L"ltCoral") { RGB = 0xF08080; bSet = true; break; } // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
					else if (str == L"ltCyan") { RGB = 0xE0FFFF; bSet = true; break; } // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
					else if (str == L"ltGoldenrodYellow") { RGB = 0xFAFA78; bSet = true; break; } // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,120)
					else if (str == L"ltGray") { RGB = 0xD3D3D3; bSet = true; break; } // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					else if (str == L"ltGreen") { RGB = 0x90EE90; bSet = true; break; } // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
					else if (str == L"ltGrey") { RGB = 0xD3D3D3; bSet = true; break; } // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					else if (str == L"ltPink") { RGB = 0xFFB6C1; bSet = true; break; } // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
					else if (str == L"ltSalmon") { RGB = 0xFFA07A; bSet = true; break; } // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
					else if (str == L"ltSeaGreen") { RGB = 0x20B2AA; bSet = true; break; } // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
					else if (str == L"ltSkyBlue") { RGB = 0x87CEFA; bSet = true; break; } // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
					else if (str == L"ltSlateGray") { RGB = 0x778899; bSet = true; break; } // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					else if (str == L"ltSlateGrey") { RGB = 0x778899; bSet = true; break; } // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					else if (str == L"ltSteelBlue") { RGB = 0xB0C4DE; bSet = true; break; } // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
					else if (str == L"ltYellow") { RGB = 0xFFFFE0; bSet = true; break; } // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
					break;
				case 'm':
					if (str == L"magenta") { RGB = 0xFF00FF; bSet = true; break; } // (Magenta Preset Color)			Specifies a color with RGB value (255,0,255)
					else if (str == L"maroon") { RGB = 0x800000; bSet = true; break; } // (Maroon Preset Color)			Specifies a color with RGB value (128,0,0)
					else if (str == L"medAquamarine") { RGB = 0x66CDAA; bSet = true; break; } // (Medium Aquamarine Preset Color)Specifies a color with RGB value (102,205,170)
					else if (str == L"medBlue") { RGB = 0x0000CD; bSet = true; break; } // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
					else if (str == L"mediumAquamarine") { RGB = 0x66CDAA; bSet = true; break; } // (Medium Aquamarine Color)		Specifies a color with RGB value (102,205,170)
					else if (str == L"mediumBlue") { RGB = 0x0000CD; bSet = true; break; } // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
					else if (str == L"mediumOrchid") { RGB = 0xBA55D3; bSet = true; break; } // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
					else if (str == L"mediumPurple") { RGB = 0x9370DB; bSet = true; break; } // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
					else if (str == L"mediumSeaGreen") { RGB = 0x3CB371; bSet = true; break; } // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
					else if (str == L"mediumSlateBlue") { RGB = 0x7B68EE; bSet = true; break; } // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
					else if (str == L"mediumSpringGreen") { RGB = 0x00FA9A; bSet = true; break; } // (Medium Spring Color)			Specifies a color with RGB value (0,250,154)
					else if (str == L"mediumTurquoise") { RGB = 0x48D1CC; bSet = true; break; } // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
					else if (str == L"mediumVioletRed") { RGB = 0xC71585; bSet = true; break; } // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
					else if (str == L"medOrchid") { RGB = 0xBA55D3; bSet = true; break; } // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
					else if (str == L"medPurple") { RGB = 0x9370DB; bSet = true; break; } // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
					else if (str == L"medSeaGreen") { RGB = 0x3CB371; bSet = true; break; } // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
					else if (str == L"medSlateBlue") { RGB = 0x7B68EE; bSet = true; break; } // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
					else if (str == L"medSpringGreen") { RGB = 0x00FA9A; bSet = true; break; } // (Medium Spring Preset Color)	Specifies a color with RGB value (0,250,154)
					else if (str == L"medTurquoise") { RGB = 0x48D1CC; bSet = true; break; } // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
					else if (str == L"medVioletRed") { RGB = 0xC71585; bSet = true; break; } // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
					else if (str == L"midnightBlue") { RGB = 0x191970; bSet = true; break; } // (Midnight Blue Preset Color)	Specifies a color with RGB value (25,25,112)
					else if (str == L"mintCream") { RGB = 0xF5FFFA; bSet = true; break; } // (Mint Cream Preset Color)		Specifies a color with RGB value (245,255,250)
					else if (str == L"mistyRose") { RGB = 0xFFE4FF; bSet = true; break; } // (Misty Rose Preset Color)		Specifies a color with RGB value (255,228,225)
					else if (str == L"moccasin") { RGB = 0xFFE4B5; bSet = true; break; } // (Moccasin Preset Color)			Specifies a color with RGB value (255,228,181)
					else if (str == L"menuText") { RGB = 0x000000; bSet = true; break; }
					break;
				case 'n':
					if (str == L"navajoWhite") { RGB = 0xFFDEAD; bSet = true; break; } // (Navajo White Preset Color)		Specifies a color with RGB value (255,222,173)
					else if (str == L"navy") { RGB = 0x000080; bSet = true; break; } // (Navy Preset Color)				Specifies a color with RGB value (0,0,128)
					break;
				case 'o':
					if (str == L"oldLace") { RGB = 0xFDF5E6; bSet = true; break; } // (Old Lace Preset Color)			Specifies a color with RGB value (253,245,230)
					else if (str == L"olive") { RGB = 0x808000; bSet = true; break; } // (Olive Preset Color)			Specifies a color with RGB value (128,128,0)
					else if (str == L"oliveDrab") { RGB = 0x6B8E23; bSet = true; break; } // (Olive Drab Preset Color)		Specifies a color with RGB value (107,142,35)
					else if (str == L"orange") { RGB = 0xFFA500; bSet = true; break; } // (Orange Preset Color)			Specifies a color with RGB value (255,165,0)
					else if (str == L"orangeRed") { RGB = 0xFF4500; bSet = true; break; } // (Orange Red Preset Color)		Specifies a color with RGB value (255,69,0)
					else if (str == L"orchid") { RGB = 0xDA70D6; bSet = true; break; } // (Orchid Preset Color)			Specifies a color with RGB value (218,112,214)
					break;
				case 'p':
					if (str == L"paleGoldenrod") { RGB = 0xEEE8AA; bSet = true; break; } // (Pale Goldenrod Preset Color)	Specifies a color with RGB value (238,232,170)
					else if (str == L"paleGreen") { RGB = 0x98FB98; bSet = true; break; } // (Pale Green Preset Color)		Specifies a color with RGB value (152,251,152)
					else if (str == L"paleTurquoise") { RGB = 0xAFEEEE; bSet = true; break; } // (Pale Turquoise Preset Color)	Specifies a color with RGB value (175,238,238)
					else if (str == L"paleVioletRed") { RGB = 0xDB7093; bSet = true; break; } // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147)
					else if (str == L"papayaWhip") { RGB = 0xFFEFD5; bSet = true; break; } // (Papaya Whip Preset Color)		Specifies a color with RGB value (255,239,213)
					else if (str == L"peachPuff") { RGB = 0xFFDAB9; bSet = true; break; } // (Peach Puff Preset Color)		Specifies a color with RGB value (255,218,185)
					else if (str == L"peru") { RGB = 0xCD853F; bSet = true; break; } // (Peru Preset Color)				Specifies a color with RGB value (205,133,63)
					else if (str == L"pink") { RGB = 0xFFC0CB; bSet = true; break; } // (Pink Preset Color)				Specifies a color with RGB value (255,192,203)
					else if (str == L"plum") { RGB = 0xD3A0D3; bSet = true; break; } // (Plum Preset Color)				Specifies a color with RGB value (221,160,221)
					else if (str == L"powderBlue") { RGB = 0xB0E0E6; bSet = true; break; } // (Powder Blue Preset Color)		Specifies a color with RGB value (176,224,230)
					else if (str == L"purple") { RGB = 0x800080; bSet = true; break; } // (Purple Preset Color)			Specifies a color with RGB value (128,0,128)
					break;
				case 'r':
					if (str == L"red") { RGB = 0xFF0000; bSet = true; break; } // (Red Preset Color)				Specifies a color with RGB value (255,0,0)
					else if (str == L"rosyBrown") { RGB = 0xBC8F8F; bSet = true; break; } // (Rosy Brown Preset Color)		Specifies a color with RGB value (188,143,143)
					else if (str == L"royalBlue") { RGB = 0x4169E1; bSet = true; break; } // (Royal Blue Preset Color)		Specifies a color with RGB value (65,105,225)
					break;
				case 's':
					if (str == L"saddleBrown") { RGB = 0x8B4513; bSet = true; break; } // (Saddle Brown Preset Color)		Specifies a color with RGB value (139,69,19)
					else if (str == L"salmon") { RGB = 0xFA8072; bSet = true; break; } // (Salmon Preset Color)			Specifies a color with RGB value (250,128,114)
					else if (str == L"sandyBrown") { RGB = 0xF4A460; bSet = true; break; } // (Sandy Brown Preset Color)		Specifies a color with RGB value (244,164,96)
					else if (str == L"seaGreen") { RGB = 0x2E8B57; bSet = true; break; } // (Sea Green Preset Color)		Specifies a color with RGB value (46,139,87)
					else if (str == L"seaShell") { RGB = 0xFFF5EE; bSet = true; break; } // (Sea Shell Preset Color)		Specifies a color with RGB value (255,245,238)
					else if (str == L"sienna") { RGB = 0xA0522D; bSet = true; break; } // (Sienna Preset Color)			Specifies a color with RGB value (160,82,45)
					else if (str == L"silver") { RGB = 0xC0C0C0; bSet = true; break; } // (Silver Preset Color)			Specifies a color with RGB value (192,192,192)
					else if (str == L"skyBlue") { RGB = 0x87CEEB; bSet = true; break; } // (Sky Blue Preset Color)			Specifies a color with RGB value (135,206,235)
					else if (str == L"slateBlue") { RGB = 0x6A5AEB; bSet = true; break; } // (Slate Blue Preset Color)		Specifies a color with RGB value (106,90,205)
					else if (str == L"slateGray") { RGB = 0x708090; bSet = true; break; } // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
					else if (str == L"slateGrey") { RGB = 0x708090; bSet = true; break; } // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
					else if (str == L"snow") { RGB = 0xFFFAFA; bSet = true; break; } // (Snow Preset Color)				Specifies a color with RGB value (255,250,250)
					else if (str == L"springGreen") { RGB = 0x00FF7F; bSet = true; break; } // (Spring Green Preset Color)		Specifies a color with RGB value (0,255,127)
					else if (str == L"steelBlue") { RGB = 0x4682B4; bSet = true; break; } // (Steel Blue Preset Color)		Specifies a color with RGB value (70,130,180)
					break;
				case 't':
					if (str == L"tan") { RGB = 0xD2B48C; bSet = true; break; } // (Tan Preset Color)				Specifies a color with RGB value (210,180,140)
					else if (str == L"teal") { RGB = 0x008080; bSet = true; break; } // (Teal Preset Color)				Specifies a color with RGB value (0,128,128)
					else if (str == L"thistle") { RGB = 0xD8BFD8; bSet = true; break; } // (Thistle Preset Color)			Specifies a color with RGB value (216,191,216)
					else if (str == L"tomato") { RGB = 0xFF7347; bSet = true; break; } // (Tomato Preset Color)			Specifies a color with RGB value (255,99,71)
					else if (str == L"turquoise") { RGB = 0x40E0D0; bSet = true; break; } // (Turquoise Preset Color)		Specifies a color with RGB value (64,224,208)
					break;
				case 'v':
					if (str == L"violet") { RGB = 0xEE82EE; bSet = true; break; } // (Violet Preset Color)			Specifies a color with RGB value (238,130,238)
					break;
				case 'w':
					if (str == L"wheat") { RGB = 0xF5DEB3; bSet = true; break; } // (Wheat Preset Color)			Specifies a color with RGB value (245,222,179)
					else if (str == L"white") { RGB = 0xFFFFFF; bSet = true; break; } // (White Preset Color)			Specifies a color with RGB value (255,255,255)
					else if (str == L"whiteSmoke") { RGB = 0xF5F5F5; bSet = true; break; } // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245)
					else if (str == L"windowsText") { RGB = 0x000000; bSet = true; break; } // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245)
					break;
				case 'y':
					if (str == L"yellow") { RGB = 0xFFFF00; bSet = true; break; } // (Yellow Preset Color)			Specifies a color with RGB value (255,255,0)
					else if (str == L"yellowGreen") { RGB = 0x9ACD32; bSet = true; break; } // (Yellow Green Preset Color)		Specifies a color with RGB value (154,205,50)
					break;
				}
			}
			if (bSet)
			{
				color.R = (BYTE)(RGB >> 16);
				color.G = (BYTE)(RGB >> 8);
				color.B = (BYTE)(RGB);
				color.A = 0;
			}
		}
		return bSet;
	}
}
namespace PPTX
{
	CStringTrimmer::CStringTrimmer()
	{
        m_Separator = (wchar_t)' ';
	}
	CStringTrimmer::~CStringTrimmer()
	{
	}
	void CStringTrimmer::LoadFromString(std::wstring& strParams)
	{
		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
        wchar_t* pData	= (wchar_t*)strParams.c_str();
		int nCount		= (int)		strParams.length();

		int nPosition	= 0;
        wchar_t* pDataMem = pData;

		int nCurPosition = 0;
		while (nPosition <= nCount)
		{
			if (nPosition == nCount || (pData[nPosition] == m_Separator))
			{
				int nLen = nPosition - nCurPosition;
				if (nLen == 0)
				{
                    m_arParams.push_back(L"");
				}
				else
				{
					m_arParams.push_back(strParams.substr(nCurPosition, nLen));
				}
				nCurPosition = nPosition + 1;
			}
			++nPosition;
		}
	}
	double CStringTrimmer::GetParameter(int nIndex, double dDefault)
	{
		if (nIndex < 0 || nIndex >= (int)m_arParams.size())
			return dDefault;

		if (m_arParams[nIndex].length() == 0)
			return dDefault;

		SimpleTypes::CPoint parserPoint;
		return parserPoint.FromString(m_arParams[nIndex]);
	}

	CCSS::CCSS()
	{
		Clear();
	}
	CCSS::~CCSS()
	{
	}
	void CCSS::Clear()
	{
        m_strClassName = L"";
		m_mapSettings.clear();
	}
	void CCSS::LoadFromString(std::wstring& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
        wchar_t* pData	= (wchar_t*)strParams.c_str();
		int nCount		= (int)		strParams.length();

		int nPosition	= 0;
        wchar_t* pDataMem = pData;

        while ((nPosition < nCount) && ((wchar_t(' ') == *pDataMem) || (wchar_t('\n') == *pDataMem) || (wchar_t('.') == *pDataMem)))
		{
			++nPosition;
			++pDataMem;
		}

		int nPosNameStart = nPosition;
        while ((nPosition < nCount) && (wchar_t(' ') != *pDataMem) && (wchar_t('{') != *pDataMem))
		{
			++nPosition;
			++pDataMem;
		}
		m_strClassName = strParams.substr(nPosNameStart, nPosition - nPosNameStart);

		while (true)
		{
			++nPosition;
			++pDataMem;

			// пропускаем пробелы
            while ((nPosition < nCount) && ((wchar_t(' ') == *pDataMem) || (wchar_t('{') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
            while ((nPosition < nCount) && (wchar_t(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			std::wstring strName = strParams.substr(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
            while ((nPosition < nCount) && (wchar_t(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
            while ((nPosition < nCount) && (wchar_t(';') != *pDataMem) && (wchar_t('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			std::wstring strValue = strParams.substr(nPosOld, nPosition - nPosOld);

			if (m_mapSettings.find(strName) == m_mapSettings.end())
			{
				m_mapSettings.insert(std::make_pair(strName, strValue));
			}
			else
			{
				m_mapSettings[strName] += L"," + strValue;
			}
		}
	}

	void CCSS::LoadFromString2(std::wstring& strParams)
	{
		Clear();

		// здесь не будем плодить тормозов - напишем без всяких Mid, Find, чтобы был только один проход
        wchar_t* pData	= (wchar_t*)strParams.c_str();
        int nCount      = (int)		strParams.length();

		int nPosition	= 0;
        wchar_t* pDataMem = pData;

        m_strClassName = L"";

		while (true)
		{
			// пропускаем пробелы
            while ((nPosition < nCount) && ((wchar_t(' ') == *pDataMem) || (wchar_t('{') == *pDataMem) || (wchar_t(';') == *pDataMem)))
			{
				++nPosition;
				++pDataMem;
			}

			int nPosOld = nPosition;

			// ищем имя настройки
            while ((nPosition < nCount) && (wchar_t(':') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			if (nPosOld == nPosition)
			{
				// нету ни одной настройки
				break;
			}
			std::wstring strName = strParams.substr(nPosOld, nPosition - nPosOld);

			// убираем ':'
			++nPosition;
			++pDataMem;

			// пропускаем пробелы настройки
            while ((nPosition < nCount) && (wchar_t(' ') == *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			nPosOld = nPosition;

			// пропускаем пробелы настройки
            while ((nPosition < nCount) && (wchar_t(';') != *pDataMem) && (wchar_t('}') != *pDataMem))
			{
				++nPosition;
				++pDataMem;
			}

			std::wstring strValue = strParams.substr(nPosOld, nPosition - nPosOld);

			if (pData[nPosOld] == WCHAR('.'))
                strValue = (L"0" + strValue);

			if (m_mapSettings.find(strName) == m_mapSettings.end())
			{
				m_mapSettings.insert(std::make_pair(strName, strValue));
			}
			else
			{
				m_mapSettings[strName] += L"," + strValue;
			}
		}
	}

	CStylesCSS::CStylesCSS() : m_arStyles()
	{
	}
	CStylesCSS::~CStylesCSS()
	{
	}
	void CStylesCSS::Clear()
	{
		m_arStyles.clear();
	}
	void CStylesCSS::LoadStyles(std::wstring& strParams)
	{
		Clear();

        wchar_t* pData	= (wchar_t*)strParams.c_str();
        int nCount      = (int)		strParams.length();

		int nPosition	 = 0;
		int nPositionOld = 0;
        wchar_t* pDataMem = pData;

		while (nPosition < nCount)
		{
            if (*pDataMem == wchar_t('}'))
			{
				std::wstring strTemp = strParams.substr(nPositionOld, nPosition - nPositionOld + 1);
				
				CCSS elm;
				m_arStyles.push_back(elm);
				m_arStyles.back().LoadFromString(strTemp);

				nPositionOld = nPosition + 1;
			}

			++nPosition;
			++pDataMem;
		}
	}
}

CSpTreeElemProps::CSpTreeElemProps()
{
	X = 0;
	Y = 0;
	Width = 0;
	Height = 0;

	IsTop = true;
}
CElementProps::CElementProps() : m_Properties()
{
}
CElementProps::~CElementProps()
{
	FinalRelease();
}
void CElementProps::FinalRelease()
{
	m_Properties.clear();
}
HRESULT CElementProps::GetProperty(LONG lId, ASC_VARIANT* pProp)
{
	if (NULL == pProp)
		return S_FALSE;

    std::map<LONG, ASC_VARIANT>::iterator pFind = m_Properties.find(lId);
	if (m_Properties.end() == pFind)
		return S_FALSE;

	bool bIsSupportProp = CopyProperty(*pProp, pFind->second);

	if (!bIsSupportProp)
	{
		return S_FALSE;
	}

	return S_OK;
}
HRESULT CElementProps::SetProperty(LONG lId, ASC_VARIANT prop)
{
    ASC_VARIANT var;
	bool bIsSupportProp = CopyProperty(var, prop);

    if (!bIsSupportProp)
		return S_FALSE;

    m_Properties.insert(std::pair<LONG, ASC_VARIANT>(lId, var));
	return S_OK;
}
bool CElementProps::CopyProperty(ASC_VARIANT& oDst, const ASC_VARIANT& oSrc)
{
	oDst.vt = oSrc.vt;
	switch (oDst.vt)
	{
    case ASC_VT_I4:
		{
			oDst.lVal = oSrc.lVal;
			break;
		}
    case ASC_VT_R8:
		{
			oDst.dblVal = oSrc.dblVal;
			break;
		}
    case ASC_VT_STR:
		{

            oDst.strVal = oSrc.strVal;
			break;
		}
	default:
		return false;			
	}
	return true;
}

namespace NSBinPptxRW
{

CDrawingConverter::CElement::CElement()
{
	m_pElement	= NULL;
	m_pProps	= NULL;
}
CDrawingConverter::CElement::~CElement()
{
	RELEASEOBJECT(m_pElement);
	RELEASEOBJECT(m_pProps);
}

CDrawingConverter::CElement& CDrawingConverter::CElement::operator=(const CDrawingConverter::CElement& oSrc)
{
	m_pElement	= oSrc.m_pElement;
	m_pProps	= oSrc.m_pProps;
	return *this;
}
CDrawingConverter::CElement::CElement(const CElement& oSrc)
{
	*this = oSrc;
}
CDrawingConverter::CDrawingConverter()
{
    m_nCurrentIndexObject   = 0;
    m_lNextId               = 1;
    m_lCurrentObjectTop     = 0;
    m_pOOXToVMLRenderer     = NULL;
    m_bIsUseConvertion2007  = true;
    m_pBinaryWriter         = new NSBinPptxRW::CBinaryFileWriter();
    m_pReader               = new NSBinPptxRW::CBinaryFileReader();
    m_pImageManager         = new NSBinPptxRW::CImageManager2();
    m_pXmlWriter            = new NSBinPptxRW::CXmlWriter();
   
	m_pTheme                = new NSCommon::smart_ptr<PPTX::Theme>();
	m_pClrMap               = new NSCommon::smart_ptr<PPTX::Logic::ClrMap>;
}
CDrawingConverter::~CDrawingConverter()
{
	Clear();
	RELEASEOBJECT(m_pOOXToVMLRenderer);
	RELEASEOBJECT(m_pBinaryWriter);
	RELEASEOBJECT(m_pReader);
	RELEASEOBJECT(m_pImageManager);
	RELEASEOBJECT(m_pXmlWriter);
	RELEASEOBJECT(m_pTheme);
	RELEASEOBJECT(m_pClrMap);
}
void CDrawingConverter::SetMainDocument(BinDocxRW::CDocxSerializer* pDocument)
{
	m_pBinaryWriter->ClearNoAttack();
	m_pBinaryWriter->m_pCommon->m_pMediaManager->Clear();
	
	m_pBinaryWriter->SetMainDocument(pDocument);
	m_pReader->SetMainDocument(pDocument);
	m_lNextId = 1;

	m_pImageManager->m_nDocumentType = m_pReader->m_nDocumentType = XMLWRITER_DOC_TYPE_DOCX;
}

void CDrawingConverter::SetSrcPath(const std::wstring& sPath, int nDocType)
{
    m_pReader->m_pRels->m_pManager  = m_pImageManager;
    m_pReader->m_strFolder          = sPath;

    m_pImageManager->m_nDocumentType = nDocType;
}
void CDrawingConverter::SetDstPath(const std::wstring& sPath)
{
    m_pImageManager->SetDstFolder(sPath);
}
void CDrawingConverter::SetTempPath(const std::wstring& sPath)
{
    m_pBinaryWriter->m_pCommon->m_pMediaManager->m_strTempMedia = sPath;
}
std::wstring CDrawingConverter::GetTempPath()
{
	return ((m_pBinaryWriter) && (m_pBinaryWriter->m_pCommon) && (m_pBinaryWriter->m_pCommon->m_pMediaManager))  ? m_pBinaryWriter->m_pCommon->m_pMediaManager->m_strTempMedia : L"";
}
void CDrawingConverter::SetEmbedDstPath(const std::wstring& sPath)
{
    m_pImageManager->SetDstEmbed(sPath);

    NSDirectory::CreateDirectory(sPath);
}
void CDrawingConverter::SetMediaDstPath(const std::wstring& sPath)
{   
	m_pBinaryWriter->m_pCommon->m_pMediaManager->m_strDstMedia = sPath;
}
void CDrawingConverter::ClearShapeTypes()
{
	m_mapShapeTypes.clear();
}

void CDrawingConverter::AddShapeType(XmlUtils::CXmlNode& oNodeST)
{
    std::wstring strId = oNodeST.GetAttribute(L"id");

	if (strId.empty())
	{
		strId = oNodeST.GetAttribute(L"type");
		if (strId[0] == (wchar_t)('#'))
		{
			strId = strId.substr(1);
		}
	}

	if (strId.empty()) return;
	//if (m_mapShapeTypes.find(strId) == m_mapShapeTypes.end())//?? с затиранием ???
	{
		CPPTShape* pShape = new CPPTShape();
		pShape->m_bIsShapeType = true;
		
		pShape->LoadFromXMLShapeType(oNodeST);

		CShapePtr pS = CShapePtr(new CShape(NSBaseShape::unknown, 0));
		pS->setBaseShape(CBaseShapePtr(pShape));
		
		LoadCoordSize(oNodeST, pS);

		m_mapShapeTypes.insert(std::make_pair(strId, pS));			
	}
}
HRESULT CDrawingConverter::AddShapeType(const std::wstring& bsXml)
{
    std::wstring strXml = L"<main ";

    strXml += L"\
				 xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
				 xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
				 xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
				 xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
				 xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
				 xmlns:v=\"urn:schemas-microsoft-com:vml\" \
				 xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
				 xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
				 xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
				 xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
				 xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
				 xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
				 xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
				 xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
				 xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
				 xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
				 xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
				 xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
				 xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
                 xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"";

    strXml += L">";

	strXml += bsXml;

    strXml += L"</main>";

	XmlUtils::CXmlNode oNode;
	oNode.FromXmlString(strXml);

	if (oNode.IsValid())
	{
        XmlUtils::CXmlNode oNodeST = oNode.ReadNodeNoNS(L"shapetype");

		AddShapeType(oNodeST);
    }

	return S_OK;
}

PPTX::Logic::SpTreeElem CDrawingConverter::ObjectFromXml(const std::wstring& sXml, 	std::wstring** ppMainProps)
{
	PPTX::Logic::SpTreeElem oElem;
    std::wstring sBegin(L"<main xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:p=\"urn:schemas-microsoft-com:office:powerpoint\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:x=\"urn:schemas-microsoft-com:office:excel\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\">");
    
    std::wstring sEnd(L"</main>");
    std::wstring strXml = sBegin + sXml + sEnd;

	XmlUtils::CXmlNode oMainNode;
	if (!oMainNode.FromXmlString(strXml))
		return oElem;

	XmlUtils::CXmlNodes oNodes;
    if (!oMainNode.GetNodes(L"*", oNodes))
		return oElem;

	for (LONG i = 0; i < oNodes.GetCount(); ++i)
	{
		XmlUtils::CXmlNode oParseNode;
		oNodes.GetAt(i, oParseNode);

		std::wstring strFullName = oParseNode.GetName();
        std::wstring strNS       = XmlUtils::GetNamespace(strFullName);
        std::wstring strName     = XmlUtils::GetNameNoNS(strFullName);

        while (true)
		{
            if (strName == L"drawing")
			{
				ConvertDrawing(&oElem, oParseNode, ppMainProps, true);
				break;
			}
            else if (strName == L"background")
            {
				ConvertShape(&oElem, oParseNode, ppMainProps, false);
                break;
             }
            else if (strName == L"pict" || strName == L"object")
			{
				//сначала shape type
				XmlUtils::CXmlNode oNodeST;
				if (oParseNode.GetNode(L"v:shapetype", oNodeST))
				{
					AddShapeType(oNodeST);
				}
				//binary data
				XmlUtils::CXmlNode oNodeBinData;
				oParseNode.GetNode(L"w:binData", oNodeBinData);

				XmlUtils::CXmlNodes oChilds;
                if (oParseNode.GetNodes(L"*", oChilds))
				{
					LONG lChildsCount = oChilds.GetCount();
                    bool bIsFound = false;
					PPTX::Logic::SpTreeElem* pElem = NULL;
					PPTX::Logic::COLEObject* pOle = NULL;
					for (LONG k = 0; k < lChildsCount; k++)
					{
						XmlUtils::CXmlNode oNodeP;
						oChilds.GetAt(k, oNodeP);

						std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
                        if (L"shape"     == strNameP ||
                            L"rect"      == strNameP ||
                            L"oval"      == strNameP ||
                            L"line"      == strNameP ||
                            L"background"== strNameP ||
                            L"roundrect" == strNameP ||
                            L"polyline"  == strNameP)
						{

							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;

								if (oNodeBinData.IsValid())
								{
									pElem->fromXML(oNodeBinData);
									oNodeBinData.Clear();
								}		
								ConvertShape(pElem, oNodeP, ppMainProps, true);

#ifdef AVS_OFFICE_DRAWING_DUMP_XML_TEST
								NSBinPptxRW::CXmlWriter oXmlW(m_pReader->m_nDocumentType);
								pElem->toXmlWriter(&oXmlW);
								std::wstring strXmlTemp = oXmlW.GetXmlString();
#endif
							}
						}
                        else if (L"OLEObject" == strNameP || L"objectEmbed" == strNameP)
						{
							pOle = new PPTX::Logic::COLEObject();
							pOle->fromXML(oNodeP);
						}
                        else if (L"group" == strNameP)
						{
							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								ConvertGroup(pElem, oNodeP, ppMainProps, true);

#ifdef AVS_OFFICE_DRAWING_DUMP_XML_TEST
								NSBinPptxRW::CXmlWriter oXmlW(m_pReader->m_nDocumentType);
								pElem->toXmlWriter(&oXmlW);
								std::wstring strXmlTemp = oXmlW.GetXmlString();
#endif
							}
						}
						else if (L"drawing" == strNameP)
						{
							ConvertDrawing(pElem, oNodeP, ppMainProps, true);
						}
						else
						{
							continue;
						}

						if (bIsFound)
							break;
					}
					if(NULL != pElem)
					{
						if(NULL != pOle && pOle->m_sProgId.IsInit() && (pOle->m_oId.IsInit() || pOle->m_OleObjectFile.IsInit()))
						{
							PPTX::Logic::Shape* pShape = dynamic_cast<PPTX::Logic::Shape*>(pElem->GetElem().operator ->());
							if(NULL != pShape && pShape->spPr.Fill.Fill.IsInit())
							{
								bool bImageOle = false;

								if (pShape->spPr.Fill.m_type == PPTX::Logic::UniFill::blipFill) bImageOle = true;
								
								PPTX::Logic::BlipFill oBlipFillNew;

								if (!bImageOle)
									oBlipFillNew.blip = new PPTX::Logic::Blip();

								const PPTX::Logic::BlipFill& oBlipFill = bImageOle ? pShape->spPr.Fill.Fill.as<PPTX::Logic::BlipFill>() : oBlipFillNew;
								
								if(oBlipFill.blip.IsInit())
								{
									if (pOle->m_OleObjectFile.IsInit())
									{
										oBlipFill.blip->oleFilepathBin = pOle->m_OleObjectFile->filename().GetPath();
									}
									else if (pOle->m_oId.IsInit())
									{
										oBlipFill.blip->oleRid = pOle->m_oId.get().ToString();
									}
                                    if(strName == L"object")
									{
                                        int nDxaOrig = oParseNode.ReadAttributeInt(L"w:dxaOrig");
                                        int nDyaOrig = oParseNode.ReadAttributeInt(L"w:dyaOrig");
										if (nDxaOrig > 0 && nDyaOrig > 0)
										{
											pOle->m_oDxaOrig = nDxaOrig;
											pOle->m_oDyaOrig = nDyaOrig;
										}
									}

									PPTX::Logic::Pic *newElem = new PPTX::Logic::Pic();

									newElem->blipFill	= oBlipFill;
									newElem->spPr		= pShape->spPr;
									newElem->style		= pShape->style;
									newElem->oleObject.reset(pOle);
									pOle = NULL;								
									
									pElem->InitElem(newElem);
								}
							}
						}
						m_pBinaryWriter->WriteRecord1(1, *pElem);
					}
					RELEASEOBJECT(pElem)
					RELEASEOBJECT(pOle)
				}

				break;
			}
			else if (strName == L"oleObj")
			{
				nullable<PPTX::Logic::Pic> pic = oParseNode.ReadNode(_T("p:pic"));
				if (pic.is_init())
				{
					pic->fromXMLOle(oParseNode); // todooo сделать норальный объект 

					m_pBinaryWriter->WriteRecord2(1, pic);
				}
				break;
			}
			else if (strName == L"AlternateContent")
			{
				XmlUtils::CXmlNode oNodeDr;
                if (oParseNode.GetNode(L"w:drawing", oNodeDr))
				{
                    strName = L"drawing";
					oParseNode = oNodeDr;
					continue;
				}

                if (oParseNode.GetNode(L"mc:Choice", oNodeDr))
				{
					oParseNode = oNodeDr;
					continue;
				}

                if (oParseNode.GetNode(L"w:pict", oNodeDr))
				{
                    strName = L"pict";
					oParseNode = oNodeDr;
					continue;
				}

                if (oParseNode.GetNode(L"w:object", oNodeDr))
				{
                    strName = L"object";
					oParseNode = oNodeDr;
					continue;
				}

                if (oParseNode.GetNode(L"xdr:sp", oNodeDr))
				{
                    strName = L"sp";
					oParseNode = oNodeDr;
					continue;
				}

                if (oParseNode.GetNode(L"mc:Fallback", oNodeDr))
				{
					oParseNode = oNodeDr;
					continue;
				}				

				break;
			}
			else
			{
				oElem = oParseNode;
				break;
			}
		}
	}
	return oElem;
}

std::wstring CDrawingConverter::ObjectToDrawingML(const std::wstring& sXml, int nDocType)
{
	std::wstring *pMainProps	= new std::wstring();

	PPTX::Logic::SpTreeElem oElem = ObjectFromXml(sXml, &pMainProps);
	
	if (oElem.is_init() == false) return L"";

	NSBinPptxRW::CXmlWriter oXmlWriter(nDocType);
	oXmlWriter.m_bIsUseOffice2007 = false;

	oXmlWriter.m_bIsTop = true;
	
	oXmlWriter.WriteString(L"<w:drawing>");

	bool bIsInline = false;
	
	std::wstring strMainProps = *pMainProps;
	std::wstring strMainPropsTail;
    
	int nIndexF = (int)strMainProps.find(L"</wp:inline>");
	if (-1 != nIndexF)
	{
		bIsInline = true;
		strMainProps = strMainProps.substr(0, nIndexF);
	}
	else
	{
        nIndexF = (int)strMainProps.find(L"</wp:anchor>");
		strMainProps = strMainProps.substr(0, nIndexF);
	}

	if (-1 == nIndexF)
	{
		oElem.toXmlWriter(&oXmlWriter);
		return oXmlWriter.GetXmlString();
	}

    int nIndexTail = (int)strMainProps.find(L"<wp14:sizeRel");
	if(-1 != nIndexTail)
	{
        strMainPropsTail    = strMainProps.substr( nIndexTail );
        strMainProps        = strMainProps.substr(0, nIndexTail);
	}

	oXmlWriter.WriteString(strMainProps);

	if (oElem.is<PPTX::Logic::SpTree>())
	{
        oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\">");
	}
	else if (oElem.is<PPTX::Logic::Pic>())
	{
        oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
	}
	else
	{
        oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
	}
	oElem.toXmlWriter(&oXmlWriter);
    oXmlWriter.WriteString(L"</a:graphicData></a:graphic>");

	oXmlWriter.WriteString(strMainPropsTail);
    oXmlWriter.WriteString(bIsInline ? L"</wp:inline>" : L"</wp:anchor>");

    oXmlWriter.WriteString(L"</w:drawing>");

	delete pMainProps;

	return oXmlWriter.GetXmlString();
}

std::wstring CDrawingConverter::ObjectToVML	(const std::wstring& sXml)
{
	std::wstring *pMainProps	= new std::wstring();
	
	PPTX::Logic::SpTreeElem oElem = ObjectFromXml(sXml, &pMainProps);
	
	if (oElem.is_init() == false) return L"";
	
	NSBinPptxRW::CXmlWriter oXmlWriter(m_pReader->m_nDocumentType);
	oXmlWriter.m_bIsUseOffice2007 = true;

	oXmlWriter.m_bIsTop = true;
	
	if (NULL == m_pOOXToVMLRenderer)
		m_pOOXToVMLRenderer = new COOXToVMLGeometry();
	oXmlWriter.m_pOOXToVMLRenderer = m_pOOXToVMLRenderer;

	oXmlWriter.WriteString(L"<w:pict>");

	if (oElem.is<PPTX::Logic::SpTree>())
	{
		ConvertGroupVML(oElem, *pMainProps, oXmlWriter);
	}
	else if (oElem.is<PPTX::Logic::Shape>())
	{
		ConvertShapeVML(oElem, *pMainProps, oXmlWriter);
	}

    oXmlWriter.WriteString(L"</w:pict>");

	delete pMainProps;

	return oXmlWriter.GetXmlString();
}

HRESULT CDrawingConverter::AddObject(const std::wstring& bsXml, std::wstring** pMainProps)
{
    std::wstring strXml = _start_xml_object + bsXml + _end_xml_object;
    
    m_pBinaryWriter->m_pCommon->CheckFontPicker();
    
    ++m_lCurrentObjectTop;
    bool bResult = ParceObject(strXml, pMainProps);
    --m_lCurrentObjectTop;

	return bResult ? S_OK : S_FALSE;
}
void CDrawingConverter::ConvertVml(const std::wstring& sXml, std::vector<nullable<PPTX::Logic::SpTreeElem>> &elements)
{
	std::wstring strXml = _start_xml_object + sXml + _end_xml_object;

	XmlUtils::CXmlNode oMainNode;
	if (!oMainNode.FromXmlString(strXml))
		return;

	XmlUtils::CXmlNodes oNodes;
	if (!oMainNode.GetNodes(L"*", oNodes))
		return;

	LONG lCount = oNodes.GetCount();
	std::wstring* pMainProps = NULL;
	std::wstring** ppMainProps = &pMainProps;

	for (LONG i = 0; i < lCount; ++i)
	{
		XmlUtils::CXmlNode oParseNode;
		oNodes.GetAt(i, oParseNode);

		std::wstring strFullName = oParseNode.GetName();
		std::wstring strNS = XmlUtils::GetNamespace(strFullName);
		std::wstring strName = XmlUtils::GetNameNoNS(strFullName);

		while (true)
		{
			if (strName == L"background")
			{
				nullable<PPTX::Logic::SpTreeElem> elem;
				elem.Init();

				ConvertShape(elem.GetPointer(), oParseNode, ppMainProps, false);
				elements.push_back(elem);
				break;
			}
			else if (strName == L"pict" || strName == L"object")
			{
				//сначала shape type
				XmlUtils::CXmlNode oNodeST;
				if (oParseNode.GetNode(L"v:shapetype", oNodeST))
				{
					AddShapeType(oNodeST);
				}
				//binary data
				XmlUtils::CXmlNode oNodeBinData;
				oParseNode.GetNode(L"w:binData", oNodeBinData);

				XmlUtils::CXmlNodes oChilds;
				if (oParseNode.GetNodes(L"*", oChilds))
				{
					LONG lChildsCount = oChilds.GetCount();
					bool bIsFound = false;
					PPTX::Logic::COLEObject* pOle = NULL;
					
					nullable<PPTX::Logic::SpTreeElem> pElem;

					for (LONG k = 0; k < lChildsCount; k++)
					{
						XmlUtils::CXmlNode oNodeP;
						oChilds.GetAt(k, oNodeP);

						std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
						if (L"shape" == strNameP ||
							L"rect" == strNameP ||
							L"oval" == strNameP ||
							L"line" == strNameP ||
							L"background" == strNameP ||
							L"roundrect" == strNameP ||
							L"image" == strNameP ||
							L"polyline" == strNameP)
						{
							if (false == pElem.IsInit())
							{
								pElem = new PPTX::Logic::SpTreeElem;
								if (oNodeBinData.IsValid())
								{
									pElem->fromXML(oNodeBinData);
								}
								ConvertShape(pElem.GetPointer(), oNodeP, ppMainProps, true);
							}
						}
						else if (L"OLEObject" == strNameP || L"objectEmbed" == strNameP)
						{
							pOle = new PPTX::Logic::COLEObject();
							pOle->fromXML(oNodeP);
						}
						else if (L"group" == strNameP)
						{
							if (false == pElem.IsInit())
							{
								pElem = new PPTX::Logic::SpTreeElem;
								ConvertGroup(pElem.GetPointer(), oNodeP, ppMainProps, true);
							}
						}
						else if (L"drawing" == strNameP)
						{
							if (false == pElem.IsInit())
							{
								pElem = new PPTX::Logic::SpTreeElem;
								ConvertDrawing(pElem.GetPointer(), oNodeP, ppMainProps, true);
							}
						}
						else
						{
							continue;
						}

						if (bIsFound)
							break;
					}
					if (pElem.IsInit() && NULL != pOle && pOle->m_sProgId.IsInit() && (pOle->m_oId.IsInit() || pOle->m_OleObjectFile.IsInit()))
					{
						PPTX::Logic::Pic* pPicture = dynamic_cast<PPTX::Logic::Pic*>(pElem->GetElem().operator ->());
						PPTX::Logic::Shape* pShape = dynamic_cast<PPTX::Logic::Shape*>(pElem->GetElem().operator ->());

						if ((!pPicture && pShape) && (pShape->spPr.Fill.is<PPTX::Logic::BlipFill>()))
						{//convert pptx shape to pptx picture
							pPicture = new PPTX::Logic::Pic();

							pPicture->nvPicPr.nvPr = pShape->nvSpPr.nvPr;
							pPicture->nvPicPr.cNvPr = pShape->nvSpPr.cNvPr;
							pPicture->spPr = pShape->spPr;
							pPicture->blipFill = pShape->spPr.Fill.as<PPTX::Logic::BlipFill>();
							pPicture->spPr.Fill = PPTX::Logic::UniFill();

							pShape = NULL;
							pElem->InitElem(pPicture);
						}
						if ((pPicture) && (pPicture->blipFill.blip.IsInit()))
						{
							if (pOle->m_OleObjectFile.IsInit())
							{
								pPicture->blipFill.blip->oleFilepathBin = pOle->m_OleObjectFile->filename().GetPath();
							}
							else if (pOle->m_oId.IsInit())
							{
								pPicture->blipFill.blip->oleRid = pOle->m_oId.get().ToString();
							}
							if (strName == L"object")
							{
								int nDxaOrig = oParseNode.ReadAttributeInt(L"w:dxaOrig");
								int nDyaOrig = oParseNode.ReadAttributeInt(L"w:dyaOrig");
								if (nDxaOrig > 0 && nDyaOrig > 0)
								{
									pOle->m_oDxaOrig = nDxaOrig;
									pOle->m_oDyaOrig = nDyaOrig;
								}
							}
							pPicture->oleObject.reset(pOle);
							pOle = NULL;
						}
					}
					if (pElem.IsInit())
						elements.push_back(pElem);
					RELEASEOBJECT(pOle)
				}
				break;
			}
			else
			{
				nullable<PPTX::Logic::SpTreeElem> elem(oParseNode);
				elements.push_back(elem);
				break;
			}
		}
	}
}
bool CDrawingConverter::ParceObject(const std::wstring& strXml, std::wstring** pMainProps)
{
	XmlUtils::CXmlNode oMainNode;
	if (!oMainNode.FromXmlString(strXml))
		return NULL;

	XmlUtils::CXmlNodes oNodes;
    if (!oMainNode.GetNodes(L"*", oNodes))
		return NULL;

	m_pBinaryWriter->StartRecord(0);
	m_pBinaryWriter->ClearCurShapePositionAndSizes();

	LONG lCount = oNodes.GetCount();
	for (LONG i = 0; i < lCount; ++i)
	{
		XmlUtils::CXmlNode oParseNode;
		oNodes.GetAt(i, oParseNode);

		std::wstring strFullName = oParseNode.GetName();
        std::wstring strNS       = XmlUtils::GetNamespace(strFullName);
        std::wstring strName     = XmlUtils::GetNameNoNS(strFullName);

        while (true)
		{
			if (strName == L"background")
            {
				PPTX::Logic::SpTreeElem oElem;
				ConvertShape(&oElem, oParseNode, pMainProps, false);
				m_pBinaryWriter->WriteRecord1(1, oElem);
                break;
			}
            else if (strName == L"pict" || strName == L"object")
			{
				//сначала shape type
				XmlUtils::CXmlNode oNodeST;
				if (oParseNode.GetNode(L"v:shapetype", oNodeST))
				{
					AddShapeType(oNodeST);
				}
				//binary data
				XmlUtils::CXmlNode oNodeBinData;
				oParseNode.GetNode(L"w:binData", oNodeBinData);

				XmlUtils::CXmlNodes oChilds;
                if (oParseNode.GetNodes(L"*", oChilds))
				{
					LONG lChildsCount = oChilds.GetCount();
                    bool bIsFound = false;
					PPTX::Logic::SpTreeElem* pElem = NULL;
					PPTX::Logic::COLEObject* pOle = NULL;
					for (LONG k = 0; k < lChildsCount; k++)
					{
						XmlUtils::CXmlNode oNodeP;
						oChilds.GetAt(k, oNodeP);

						std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
                        if (L"shape"     == strNameP ||
                            L"rect"      == strNameP ||
                            L"oval"      == strNameP ||
                            L"line"      == strNameP ||
                            L"background"== strNameP ||
                            L"roundrect" == strNameP ||
                            L"image"	 == strNameP ||
                            L"polyline"  == strNameP)
						{
							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								if (oNodeBinData.IsValid())
								{
									pElem->fromXML(oNodeBinData);
								}
								ConvertShape(pElem, oNodeP, pMainProps, true);
							}
						}
                        else if (L"OLEObject" == strNameP || L"objectEmbed" == strNameP)
						{
							pOle = new PPTX::Logic::COLEObject();
							pOle->fromXML(oNodeP);
						}
                        else if (L"group" == strNameP)
						{
							if(NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								ConvertGroup(pElem, oNodeP, pMainProps, true);
							}
						}
						else if (L"drawing" == strNameP)
						{
							if (NULL == pElem)
							{
								pElem = new PPTX::Logic::SpTreeElem;
								ConvertDrawing(pElem, oNodeP, pMainProps, true);
							}
						}
						else
						{
							continue;
						}

						if (bIsFound)
							break;
					}
					if(pElem && NULL != pOle && pOle->m_sProgId.IsInit() && (pOle->m_oId.IsInit() || pOle->m_OleObjectFile.IsInit()))
					{
						PPTX::Logic::Pic* pPicture = dynamic_cast<PPTX::Logic::Pic*>(pElem->GetElem().operator ->());
						PPTX::Logic::Shape* pShape = dynamic_cast<PPTX::Logic::Shape*>(pElem->GetElem().operator ->());
						
						if ((!pPicture && pShape) && (pShape->spPr.Fill.is<PPTX::Logic::BlipFill>()))
						{//convert pptx shape to pptx picture
							pPicture = new PPTX::Logic::Pic();

							pPicture->nvPicPr.nvPr	= pShape->nvSpPr.nvPr;
							pPicture->nvPicPr.cNvPr	= pShape->nvSpPr.cNvPr;
							pPicture->spPr			= pShape->spPr;
							pPicture->blipFill		= pShape->spPr.Fill.as<PPTX::Logic::BlipFill>();
							pPicture->spPr.Fill		= PPTX::Logic::UniFill();

							pShape = NULL; 
							pElem->InitElem(pPicture);							
						}
						if ((pPicture) && (pPicture->blipFill.blip.IsInit()))
						{
							if (pOle->m_OleObjectFile.IsInit())
							{
								pPicture->blipFill.blip->oleFilepathBin = pOle->m_OleObjectFile->filename().GetPath();
							}
							else if (pOle->m_oId.IsInit())
							{
								pPicture->blipFill.blip->oleRid = pOle->m_oId.get().ToString();
							}
                            if(strName == L"object")
							{
                                int nDxaOrig = oParseNode.ReadAttributeInt(L"w:dxaOrig");
                                int nDyaOrig = oParseNode.ReadAttributeInt(L"w:dyaOrig");
								if (nDxaOrig > 0 && nDyaOrig > 0)
								{
									pOle->m_oDxaOrig = nDxaOrig;
									pOle->m_oDyaOrig = nDyaOrig;
								}
							}
							pPicture->oleObject.reset(pOle);
							pOle = NULL;								
						}
					}
					if (pElem)
						m_pBinaryWriter->WriteRecord1(1, *pElem);

					RELEASEOBJECT(pElem)
					RELEASEOBJECT(pOle)
				}
				break;
			}
			else
			{
				PPTX::Logic::SpTreeElem oElem;
				oElem = oParseNode;

				if (oElem.is_init())
				{
					m_pBinaryWriter->WriteRecord1(1, oElem);
				}
				break;
			}
		}
	}

	m_pBinaryWriter->EndRecord();
	return true;
}

void CDrawingConverter::ConvertDiagram(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop)
{
	if (!result) return;

	nullable<OOX::RId>		id_data;
	nullable<OOX::RId>		id_drawing;

	smart_ptr<OOX::File>	oFileData;
	smart_ptr<OOX::File>	oFileDrawing;

	OOX::CDiagramData*		pDiagramData	= NULL;
	OOX::CDiagramDrawing*	pDiagramDrawing	= NULL;

    XmlMacroReadAttributeBase(oNode, L"r:dm", id_data);
	
	if (id_data.IsInit())
	{
		oFileData = m_pBinaryWriter->GetRels()->Find(*id_data);
		
		if (oFileData.is_init())
		{
			pDiagramData = dynamic_cast<OOX::CDiagramData*>(oFileData.GetPointer());										
			if ((pDiagramData) && (pDiagramData->m_oDataModel.IsInit()))
			{
				for (size_t i = 0; (pDiagramData->m_oDataModel->m_oExtLst.IsInit()) && i < pDiagramData->m_oDataModel->m_oExtLst->m_arrExt.size(); i++)
				{
					if (pDiagramData->m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt.IsInit())
					{
						id_drawing = pDiagramData->m_oDataModel->m_oExtLst->m_arrExt[i]->m_oDataModelExt->m_oRelId;
						break;
					}
				}
			}
		}
		if (id_drawing.is_init())
		{
			oFileDrawing = m_pBinaryWriter->GetRels()->Find(*id_drawing);
			pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());
		}
		if (!pDiagramDrawing && pDiagramData)
		{
		   OOX::CPath pathDiagramData = pDiagramData->m_strFilename;

			int a1 = (int)pathDiagramData.GetFilename().find(L".");
			std::wstring strId = pathDiagramData.GetFilename().substr(4, pathDiagramData.GetFilename().length() - 8);
			
			OOX::CPath pathDiagramDrawing = pathDiagramData.GetDirectory() + FILE_SEPARATOR_STR + L"drawing" + strId + L".xml";	

			if (NSFile::CFileBinary::Exists(pathDiagramDrawing.GetPath()))
			{
				oFileDrawing = smart_ptr<OOX::File>(dynamic_cast<OOX::File*>(new OOX::CDiagramDrawing(NULL, pathDiagramDrawing)));
				if (oFileDrawing.IsInit())
					pDiagramDrawing = dynamic_cast<OOX::CDiagramDrawing*>(oFileDrawing.GetPointer());
			}
		}
	}

	if ((pDiagramDrawing) && (pDiagramDrawing->m_oShapeTree.IsInit()))
	{
		result->InitElem(new PPTX::Logic::SpTree(*pDiagramDrawing->m_oShapeTree));
		//to correct write blipFill rId to binary
		SetRels(pDiagramDrawing);
	}
	else
	{//BG-FSC1.docx
		//parse pDiagramData !!
	}
	if (result->is<PPTX::Logic::SpTree>())
	{
		PPTX::Logic::SpTree& _pElem = result->as<PPTX::Logic::SpTree>();
		if (!_pElem.grpSpPr.xfrm.is_init())
		{
			_pElem.grpSpPr.xfrm = new PPTX::Logic::Xfrm();

			_pElem.grpSpPr.xfrm->offX = m_pBinaryWriter->m_lXCurShape;
			_pElem.grpSpPr.xfrm->offY = m_pBinaryWriter->m_lYCurShape;
			_pElem.grpSpPr.xfrm->extX = m_pBinaryWriter->m_lCxCurShape;
			_pElem.grpSpPr.xfrm->extY = m_pBinaryWriter->m_lCyCurShape;
			_pElem.grpSpPr.xfrm->chOffX = (int)0;
			_pElem.grpSpPr.xfrm->chOffY = (int)0;
			_pElem.grpSpPr.xfrm->chExtX = m_pBinaryWriter->m_lCxCurShape;
			_pElem.grpSpPr.xfrm->chExtY = m_pBinaryWriter->m_lCyCurShape;
		}
		else
		{
			if (!_pElem.grpSpPr.xfrm->offX.is_init())	_pElem.grpSpPr.xfrm->offX = m_pBinaryWriter->m_lXCurShape;
			if (!_pElem.grpSpPr.xfrm->offY.is_init())	_pElem.grpSpPr.xfrm->offY = m_pBinaryWriter->m_lYCurShape;
			if (!_pElem.grpSpPr.xfrm->extX.is_init())	_pElem.grpSpPr.xfrm->extX = m_pBinaryWriter->m_lCxCurShape;
			if (!_pElem.grpSpPr.xfrm->extY.is_init())	_pElem.grpSpPr.xfrm->extY = m_pBinaryWriter->m_lCyCurShape;
			
			if (!_pElem.grpSpPr.xfrm->chOffX.is_init())	_pElem.grpSpPr.xfrm->chOffX = (int)0;
			if (!_pElem.grpSpPr.xfrm->chOffY.is_init())	_pElem.grpSpPr.xfrm->chOffY = (int)0;
			if (!_pElem.grpSpPr.xfrm->chExtX.is_init())	_pElem.grpSpPr.xfrm->chExtX = m_pBinaryWriter->m_lCxCurShape;
			if (!_pElem.grpSpPr.xfrm->chExtY.is_init())	_pElem.grpSpPr.xfrm->chExtY = m_pBinaryWriter->m_lCyCurShape;
		}
					
	}								
}
void CDrawingConverter::ConvertDrawing(PPTX::Logic::SpTreeElem *elem, XmlUtils::CXmlNode& oNodeShape, std::wstring**& pMainProps, bool bIsTop)
{
	if (!elem) return;

	XmlUtils::CXmlNode oNodeAnchorInline = oNodeShape.ReadNodeNoNS(L"anchor");
	if (!oNodeAnchorInline.IsValid())
	{
		oNodeAnchorInline = oNodeShape.ReadNodeNoNS(L"inline");
	}

	if (oNodeAnchorInline.IsValid())
	{
		XmlUtils::CXmlNode oNodeExt;

		m_pBinaryWriter->m_lXCurShape = 0;
		m_pBinaryWriter->m_lYCurShape = 0;

		if (oNodeAnchorInline.GetNode(L"wp:extent", oNodeExt))
		{
			m_pBinaryWriter->m_lCxCurShape = oNodeExt.ReadAttributeInt(L"cx");
			m_pBinaryWriter->m_lCyCurShape = oNodeExt.ReadAttributeInt(L"cy");
		}
		XmlUtils::CXmlNode oNodeDocPr;
		if (oNodeAnchorInline.GetNode(L"wp:docPr", oNodeDocPr))
		{//vml shapes without id .. reset all id 
			oNodeDocPr.SetAttributeInt(L"id", m_lNextId++);

		}
		SendMainProps(oNodeAnchorInline.GetXml(), pMainProps);

		XmlUtils::CXmlNode oNodeGraphic = oNodeAnchorInline.ReadNodeNoNS(L"graphic");
		XmlUtils::CXmlNode oNodeGraphicData = oNodeGraphic.ReadNodeNoNS(L"graphicData");

		if (oNodeGraphicData.IsValid())
		{
			XmlUtils::CXmlNodes oChilds;
			oNodeGraphicData.GetNodes(L"*", oChilds);

			if (1 == oChilds.GetCount())
			{
				XmlUtils::CXmlNode oNodeContent;
				oChilds.GetAt(0, oNodeContent);

/*				if (L"dgm:relIds" == oNodeContent.GetName() && m_pBinaryWriter->m_pCurrentContainer->is_init())
				{
					ConvertDiagram(elem, oNodeContent, pMainProps, true);
				}
				else */if (L"wpc:wpc" == oNodeContent.GetName())
				{
					PPTX::Logic::SpTree* pTree = new PPTX::Logic::SpTree();

					pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
					pTree->grpSpPr.xfrm->offX = m_pBinaryWriter->m_lXCurShape;
					pTree->grpSpPr.xfrm->offY = m_pBinaryWriter->m_lYCurShape;
					pTree->grpSpPr.xfrm->extX = m_pBinaryWriter->m_lCxCurShape;
					pTree->grpSpPr.xfrm->extY = m_pBinaryWriter->m_lCyCurShape;

					pTree->fromXML(oNodeContent);
					elem->InitElem(pTree);
				}
				else
				{
					elem->fromXML(oNodeContent);
				}
			}
		}
	}
}
void CDrawingConverter::ConvertShape(PPTX::Logic::SpTreeElem *elem, XmlUtils::CXmlNode& oNodeShape, std::wstring**& pMainProps,bool bIsTop)
{
	if (!elem) return;

	std::wstring strNameNode = oNodeShape.GetName();
	
	bool bIsNeedCoordSizes	= true;
	bool bTextBox			= false;
	bool bPicture			= false;
	bool bStroked			= true;
	bool bHidden			= false;
	bool bShapeTypeSet		= true;

    std::wstring strStyleAdvenced = L"";

	PPT::CShapeElement oShapeElem;
	CPPTShape* pPPTShape = NULL;
	bool bSetShape = false;

    if (L"v:background" == strNameNode)
    {
        pPPTShape = new CPPTShape();
        pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
        pPPTShape->ReCalculate();
    }
    else if (L"v:rect" == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
		pPPTShape->ReCalculate();
	}
    else if (L"v:roundrect" == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)2);

		std::wstring strArc = oNodeShape.GetAttribute(L"arcsize");
		if (false == strArc.empty())
		{
			SimpleTypes::CUniversalMeasureOrPercent value;
			value.FromString(strArc);
			if (value.IsPercent())
			{
			}
			else
			{
				pPPTShape->SetAdjustment(0, value.GetValue() / 10.);
			}
		}
		pPPTShape->ReCalculate();
	}
    else if (L"v:oval" == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)3);
		pPPTShape->ReCalculate();
	}
    else if (L"v:image" == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)75);
		pPPTShape->ReCalculate();
	}
	else if (L"v:line" == strNameNode)
	{
		pPPTShape = new CPPTShape();
		pPPTShape->SetShapeType((PPTShapes::ShapeType)20);
		pPPTShape->ReCalculate();

        std::wstring strCoord1 = oNodeShape.GetAttributeOrValue(L"from");
        std::wstring strCoord2 = oNodeShape.GetAttributeOrValue(L"to");

        if (!strCoord1.empty() && !strCoord2.empty())
		{
			std::vector<std::wstring> oArray1;
            boost::algorithm::split(oArray1, strCoord1, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			std::vector<std::wstring> oArray2;
            boost::algorithm::split(oArray2, strCoord2, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (oArray1.size() >= 2 && oArray2.size() >= 2)
			{
				SimpleTypes::CPoint parserPoint;
				double x1 = parserPoint.FromString(oArray1[0]);
				double y1 = parserPoint.FromString(oArray1[1]);
				double x2 = parserPoint.FromString(oArray2[0]);
				double y2 = parserPoint.FromString(oArray2[1]);

				if (x1 > x2)
				{
					double tmp = x1;
					x1 = x2;
					x2 = tmp;
				}
				if (y1 > y2)
				{
					double tmp = y1;
					y1 = y2;
					y2 = tmp;
				}

                strStyleAdvenced = L";left:"    + XmlUtils::ToString(x1,      L"%.2lf")
                                 + L";top:"     + XmlUtils::ToString(y1,      L"%.2lf")
                                 + L";width:"   + XmlUtils::ToString(x2-x1,   L"%.2lf")
                                 + L";height:"  + XmlUtils::ToString(y2-y1,   L"%.2lf")
                                 + L";";
			}
		}
	}
    else if (L"v:polyline" == strNameNode)
	{
        std::wstring strPoints = oNodeShape.GetAttributeOrValue(L"points");
        if (L"" != strPoints)
		{
			std::vector<std::wstring> oArray;
            boost::algorithm::split(oArray, strPoints, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			int nSize = (int)oArray.size();
			if ((nSize % 2 == 0) && nSize > 3)
			{
				int* _POINTS = new int[nSize];
				double dKoef = 25.4 * 36000 / 72.0;

				for (int k = 0; k < nSize; ++k)
				{
                    if (L"" == oArray[k])
					{
						_POINTS[k] = 0;
					}
					else
					{
						SimpleTypes::CPoint parserPoint;
						_POINTS[k] = (int)(dKoef * parserPoint.FromString(oArray[k]));
					}
				}

				// теперь определяем bounds
				int _x = INT_MAX;
				int _y = INT_MAX;
				int _r = INT_MIN;
				int _b = INT_MIN;

				for (int k = 0; k < nSize; k += 2)
				{
					int tmpx = _POINTS[k];
					int tmpy = _POINTS[k + 1];
					if (tmpx < _x)
						_x = tmpx;
					if (tmpx > _r)
						_r = tmpx;
					if (tmpy < _y)
						_y = tmpy;
					if (tmpy > _b)
						_b = tmpy;
				}

				int nOffsetX = _POINTS[0] - _x;
				int nOffsetY = _POINTS[1] - _y;

                strStyleAdvenced += L";margin-left:" + std::to_wstring(_x) + L";margin-top:" + std::to_wstring(_y)
                        + L";width:" + std::to_wstring(_r - _x) + L";height:" + std::to_wstring(_b - _y) + L";polyline_correct:true;";

                double dKoefX = 21600.0 / (std::max)((_r - _x), 1);
                double dKoefY = 21600.0 / (std::max)((_b - _y), 1);
                std::wstring strPath;
				for (int k = 0; k < nSize; k += 2)
				{
                    std::wstring _s;
                    if (k == 0) _s = L"m";
                    else        _s = L"l";

                    strPath += _s + std::to_wstring((int)(dKoefX * (_POINTS[k] - _x))) + L"," + std::to_wstring((int)(dKoefY * (_POINTS[k + 1] - _y)));
                }

                strPath += L"e";

				RELEASEARRAYOBJECTS(_POINTS);

				pPPTShape = new CPPTShape();
				pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
				// иначе сохранится рект
				pPPTShape->m_eType = PPTShapes::sptCustom;

				pPPTShape->LoadPathList(strPath);
				pPPTShape->ReCalculate();
				bIsNeedCoordSizes = false;
			}
		}		
	}
    else if (L"v:shape" == strNameNode)
	{
		bShapeTypeSet = false;
		std::wstring strType = oNodeShape.GetAttribute(L"type");
        if (strType.length() > 2 && strType[0] == (wchar_t)('#'))
		{
			strType = strType.substr(1);

			std::map<std::wstring, CShapePtr>::iterator pFind = m_mapShapeTypes.find(strType);
			if (m_mapShapeTypes.end() != pFind)
			{
                CBaseShapePtr base_shape_type = pFind->second->getBaseShape();
				CPPTShape* ppt_shape_type = dynamic_cast<CPPTShape*>(base_shape_type.get());
				
				pPPTShape = new CPPTShape();
				base_shape_type->SetToDublicate(pPPTShape);

				pPPTShape->m_eType = ppt_shape_type->m_eType;
				bShapeTypeSet = true;
			}
			else if (std::wstring::npos != strType.find(L"t75"))
			{
				pPPTShape = new CPPTShape();
				pPPTShape->SetShapeType((PPTShapes::ShapeType)75);
				pPPTShape->m_eType = PPTShapes::sptCFrame;
			}
		}
		
		if (NULL == pPPTShape)
		{
            std::wstring strType = oNodeShape.GetAttribute(L"o:connectortype");
			if (strType.length() > 0)
			{
				int type = 0;
                if      (strType == L"elbow")		type = PPTShapes::sptCBentConnector2;
                else if (strType == L"straight")	type = PPTShapes::sptCStraightConnector1;
                else if (strType == L"curved")      type = PPTShapes::sptCCurvedConnector2;

				if (type > 0)
				{
					pPPTShape = new CPPTShape();
					pPPTShape->SetShapeType((PPTShapes::ShapeType)type);
					pPPTShape->m_bIsStroked = true;
					pPPTShape->ReCalculate();
				}
			}
			else
			{
				pPPTShape = new CPPTShape();
				pPPTShape->SetShapeType((PPTShapes::ShapeType)1);
				// иначе сохранится рект
				pPPTShape->m_eType = PPTShapes::sptCustom;
			}			
		}
		oShapeElem.m_pShape->setBaseShape(CBaseShapePtr(pPPTShape));
		bSetShape = true;
		
		if (bIsNeedCoordSizes)
		{
			LoadCoordPos(oNodeShape, oShapeElem.m_pShape); //for path calculate
		}
		pPPTShape->LoadFromXMLShapeType(oNodeShape);
	}

	if (pPPTShape != NULL)
	{	
		if (!bSetShape)
			oShapeElem.m_pShape->setBaseShape(CBaseShapePtr(pPPTShape));
		if (bIsNeedCoordSizes)
		{
			LoadCoordSize(oNodeShape, oShapeElem.m_pShape);
		}
		else
		{
			oShapeElem.m_pShape->m_dWidthLogic  = 21600;
			oShapeElem.m_pShape->m_dHeightLogic = 21600;

			oShapeElem.m_pShape->getBaseShape()->m_oPath.SetCoordsize(21600, 21600);			
		}

		if (pPPTShape->m_oSignatureLine.IsInit())
			pPPTShape->m_eType = PPTShapes::ShapeType::sptCFrame;

		if (pPPTShape->m_eType == PPTShapes::sptCTextBox)
		{
			bTextBox = true;
		}
		else if (pPPTShape->m_eType == PPTShapes::sptCFrame)
		{
			bPicture = true;
		}
		else if (pPPTShape->m_eType == PPTShapes::sptCNotchedCircularArrow)
		{
			bHidden = true;
		}
        std::wstring strXmlPPTX;

		PPTX::Logic::Shape*	pShape		= NULL; //todoooo + CxnSp
		PPTX::Logic::Pic*	pPicture	= NULL;
		
		PPTX::Logic::SpPr*	pSpPr		= NULL;
		PPTX::Logic::CNvPr* pCNvPr		= NULL;
		
		if (bPicture && false == pPPTShape->m_oSignatureLine.IsInit())
		{
			pPicture = new PPTX::Logic::Pic();
			elem->InitElem(pPicture);

			pSpPr	= &pPicture->spPr;
			pCNvPr	= &pPicture->nvPicPr.cNvPr;
			
			strXmlPPTX = oShapeElem.ConvertPPTShapeToPPTX(true);
		}
		else
		{
			pShape = new PPTX::Logic::Shape();
			elem->InitElem(pShape);
			
			pSpPr	= &pShape->spPr;
			pCNvPr	= &pShape->nvSpPr.cNvPr;

			if (!pShape->oTextBoxBodyPr.IsInit())
				pShape->oTextBoxBodyPr	= new PPTX::Logic::BodyPr();
			
			if (pPPTShape->IsWordArt())
			{
				ConvertWordArtShape(elem, oNodeShape, pPPTShape);
				strXmlPPTX = L"<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>";
			}
			else
			{
				strXmlPPTX = oShapeElem.ConvertPPTShapeToPPTX(true);

				// в старых шейпах текст крутится отдельно
				pShape->oTextBoxBodyPr->upright = true;
			}

			XmlUtils::CXmlNode oNodeTextBox;
			
			std::wstring sTextboxStyle;

			bool			res_text = oNodeShape.GetNode(L"v:textbox", oNodeTextBox);
			if (!res_text)	res_text = oNodeShape.GetNode(L"w:textbox", oNodeTextBox); // libre 4.0 эту хрень делает
			
			if (res_text)
			{
				if (!bShapeTypeSet) pPPTShape->m_bIsStroked = false; // default for textbox easy set

				if (pPPTShape->m_eType == PPTShapes::sptCNotchedCircularArrow)
				{
					pPPTShape->m_eType = PPTShapes::sptCTextBox;
					bTextBox = true;
				}
				XmlUtils::CXmlNode oNodeContent;
				if (oNodeTextBox.GetNode(L"w:txbxContent", oNodeContent))
				{
					pShape->oTextBoxShape = oNodeContent;
				}
				else if (oNodeTextBox.GetNode(L"div", oNodeContent))
				{//The <div> tag was used to create an area in which the rotation would take place. 
					ConvertTextVML(oNodeTextBox, pShape);
				}

				std::wstring	sTextInset		= oNodeTextBox.GetAttribute(L"inset");
				std::wstring	sTextInsetMode	= oNodeTextBox.GetAttribute(L"o:insetmode");
								sTextboxStyle	= oNodeTextBox.GetAttribute(L"style");

				if (L"" != sTextInset && ((L"" == sTextInsetMode) || (L"custom" == sTextInsetMode)))
				{
					PPTX::CStringTrimmer oTrimmer;
					oTrimmer.m_Separator = (wchar_t)',';
					oTrimmer.LoadFromString(sTextInset);

					double dTextMarginLeft		= oTrimmer.GetParameter(0, 0.1);
					double dTextMarginTop		= oTrimmer.GetParameter(1, 0.05);
					double dTextMarginRight		= oTrimmer.GetParameter(2, 0.1);
					double dTextMarginBottom	= oTrimmer.GetParameter(3, 0.05);

					pShape->oTextBoxBodyPr->lIns = (int)(12700 * dTextMarginLeft	+ 0.5);
					pShape->oTextBoxBodyPr->tIns = (int)(12700 * dTextMarginTop		+ 0.5);
					pShape->oTextBoxBodyPr->rIns = (int)(12700 * dTextMarginRight	+ 0.5);
					pShape->oTextBoxBodyPr->bIns = (int)(12700 * dTextMarginBottom	+ 0.5);
				}

				if (!sTextboxStyle.empty())
				{//todooo прописать все остальное 
					SimpleTypes::Vml::CCssStyle oCssStyle;
					oCssStyle.FromString(sTextboxStyle);

					if (false == oCssStyle.m_arrProperties.empty())
					{
						std::map<SimpleTypes::Vml::ECssPropertyType, size_t>::iterator pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptLayoutFlow);

						if (pFind != oCssStyle.m_mapProperties.end())
						{
							if (oCssStyle.m_arrProperties[pFind->second]->get_Value().eLayoutFlow == SimpleTypes::Vml::csslayoutflowVertical)
							{
								pShape->oTextBoxBodyPr->vert = new PPTX::Limit::TextVerticalType();
								pShape->oTextBoxBodyPr->vert->set(L"vert");
							}
						}
						pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptMsoLayoutFlowAlt);
						if (pFind != oCssStyle.m_mapProperties.end())
						{
							if (oCssStyle.m_arrProperties[pFind->second]->get_Value().eLayoutFlowAlt == SimpleTypes::Vml::csslayoutflowaltBottomToTop)
							{
								if (pShape->oTextBoxBodyPr->vert.IsInit() == false)
									pShape->oTextBoxBodyPr->vert = new PPTX::Limit::TextVerticalType();
								pShape->oTextBoxBodyPr->vert->set(L"vert270");
						   }
						}
						if (pShape->oTextBoxBodyPr->vert.IsInit())
						{
							if (pShape->txBody.IsInit() == false)
								pShape->txBody = new PPTX::Logic::TxBody();
							
							if (!pShape->txBody->bodyPr.IsInit())
								pShape->txBody->bodyPr = new PPTX::Logic::BodyPr();

							pShape->txBody->bodyPr->vert = pShape->oTextBoxBodyPr->vert;		
						}

						pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptMsoRotate);
						if (pFind != oCssStyle.m_mapProperties.end())
						{
							try
							{
								double val = 0;
								switch (oCssStyle.m_arrProperties[pFind->second]->get_Value().eRotate)
								{
								case SimpleTypes::Vml::cssmsorotate90:	val = 90; break;
								case SimpleTypes::Vml::cssmsorotate180:	val = 180; break;
								case SimpleTypes::Vml::cssmsorotate270:	val = 270; break;
								}
								pShape->oTextBoxBodyPr->rot = val * 60000;  //для docx, xlsx
								if (pShape->txBody.IsInit() == false)       //для pptx
									pShape->txBody = new PPTX::Logic::TxBody();

								if (!pShape->txBody->bodyPr.IsInit())
									pShape->txBody->bodyPr = new PPTX::Logic::BodyPr();

								pShape->txBody->bodyPr->rot = pShape->oTextBoxBodyPr->rot;
							}catch(...){}
						}
						pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptMsoFitShapeToText);
						if (pFind != oCssStyle.m_mapProperties.end())
						{
							if (oCssStyle.m_arrProperties[pFind->second]->get_Value().bValue)
							{
								if (pShape->txBody.IsInit() == false)                       //для pptx /// todooo схлопнуть
									pShape->txBody = new PPTX::Logic::TxBody();

								if (!pShape->txBody->bodyPr.IsInit())
									pShape->txBody->bodyPr = new PPTX::Logic::BodyPr();

								pShape->txBody->bodyPr->Fit.type = PPTX::Logic::TextFit::FitSpAuto;
								pShape->oTextBoxBodyPr->Fit.type = PPTX::Logic::TextFit::FitSpAuto;
							}
						}
						pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptMsoFitTextToShape);
						if (pFind != oCssStyle.m_mapProperties.end())
						{
							if (oCssStyle.m_arrProperties[pFind->second]->get_Value().bValue)
							{
								if (pShape->txBody.IsInit() == false)                       //для pptx
									pShape->txBody = new PPTX::Logic::TxBody();

								if (!pShape->txBody->bodyPr.IsInit())
									pShape->txBody->bodyPr = new PPTX::Logic::BodyPr();

								pShape->txBody->bodyPr->Fit.type = PPTX::Logic::TextFit::FitNo;//?? todoooo
								pShape->oTextBoxBodyPr->Fit.type = PPTX::Logic::TextFit::FitNo;//?? todoooo

							}
						}
						pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptMsoTextScale);
						if (pFind != oCssStyle.m_mapProperties.end())
						{
							try
							{
								if (oCssStyle.m_arrProperties[pFind->second]->get_Value().oValue.eType == SimpleTypes::Vml::cssunitstypeUnits)
								{
									pShape->oTextBoxBodyPr->Fit.fontScale = (int)(100 * oCssStyle.m_arrProperties[pFind->second]->get_Value().oValue.dValue);  //для docx, xlsx
									pShape->oTextBoxBodyPr->Fit.type = PPTX::Logic::TextFit::FitNormAuto;

									if (pShape->txBody.IsInit() == false)    //для pptx
										pShape->txBody = new PPTX::Logic::TxBody();

									if (!pShape->txBody->bodyPr.IsInit())
										pShape->txBody->bodyPr = new PPTX::Logic::BodyPr();

									pShape->txBody->bodyPr->Fit.fontScale = pShape->oTextBoxBodyPr->Fit.fontScale;
									pShape->txBody->bodyPr->Fit.type = pShape->oTextBoxBodyPr->Fit.type;
								}
							}catch(...){}
						}
					}
				}
			}

			XmlUtils::CXmlNode oNodeTextData;
			if (oNodeShape.GetNode(L"p:textdata", oNodeTextData))
			{
				std::wstring sId = oNodeTextData.GetAttribute(L"id");

				if (sId.length() > 0 && m_pBinaryWriter->GetRels().IsInit())
				{
					OOX::RId rId(sId);
					smart_ptr<PPTX::LegacyDiagramText> pExt = m_pBinaryWriter->GetRels()->Get<PPTX::LegacyDiagramText>(rId);

					if (pExt.IsInit())
					{
						if (pShape->txBody.IsInit() == false)
							pShape->txBody = new PPTX::Logic::TxBody();
						if (pShape->txBody.IsInit())
						{
							PPTX::Logic::Paragraph p;
							PPTX::Logic::Run  *r = new PPTX::Logic::Run();
							r->SetText(pExt->m_Text);

							if (pExt->m_arrRPr.size() > 0)
							{
								r->rPr = pExt->m_arrRPr[0];
							}
							
							PPTX::Logic::RunElem elm;
							p.RunElems.push_back(elm);

							if (pExt->m_arrPPr.size() > 0)
							{
								p.pPr = pExt->m_arrPPr[0];
							}
							p.RunElems.back().InitRun(r);

							pShape->txBody->Paragrs.push_back(p);
						}
					}
				}
			}
			if (bTextBox)
			{
				pShape->nvSpPr.cNvSpPr.txBox = true;
			}
		}	
//-------------------------------------------------------------------------------------------------------------------
		XmlUtils::CXmlNode oNodeG;
		oNodeG.FromXmlString(strXmlPPTX);
		pSpPr->Geometry = oNodeG;

        std::wstring strStyle = oNodeShape.GetAttribute(L"style") + strStyleAdvenced;

		PPTX::CCSS oCSSParser;
		oCSSParser.LoadFromString2(strStyle);

		if (bHidden && false == bIsTop)
		{
			pCNvPr->hidden = true;
		}
		
		CSpTreeElemProps oProps;
		oProps.IsTop = bIsTop;
		std::wstring strMainPos = GetDrawingMainProps(oNodeShape, oCSSParser, oProps);
//-------------------------------------------------------------------------------------------------------------------
		if (pShape && oCSSParser.m_mapSettings.size() > 0)
		{
            std::map<std::wstring, std::wstring>::iterator pFind = oCSSParser.m_mapSettings.find(L"v-text-anchor");
			
			if (pFind != oCSSParser.m_mapSettings.end())
			{
                if (pFind->second == L"middle")					pShape->oTextBoxBodyPr->anchor = L"ctr";
                if (pFind->second == L"bottom")					pShape->oTextBoxBodyPr->anchor = L"b";
                if (pFind->second == L"top-center")				pShape->oTextBoxBodyPr->anchor = L"t";
                if (pFind->second == L"middle-center")			pShape->oTextBoxBodyPr->anchor = L"ctr";
                if (pFind->second == L"bottom-center")			pShape->oTextBoxBodyPr->anchor = L"b";
                if (pFind->second == L"top-baseline")			pShape->oTextBoxBodyPr->anchor = L"t";
                if (pFind->second == L"bottom-baseline")		pShape->oTextBoxBodyPr->anchor = L"b";
                if (pFind->second == L"top-center-baseline")	pShape->oTextBoxBodyPr->anchor = L"t";
                if (pFind->second == L"bottom-center-baseline")	pShape->oTextBoxBodyPr->anchor = L"b";
			}
				
            pFind = oCSSParser.m_mapSettings.find(L"mso-wrap-style");
			if (pFind != oCSSParser.m_mapSettings.end() )
			{
                if (pFind->second == L"none")
                    pShape->oTextBoxBodyPr->wrap = L"none";
				else
                    pShape->oTextBoxBodyPr->wrap = L"wrap";
			}
			else if (!pPPTShape->IsWordArt())
                pShape->oTextBoxBodyPr->wrap = L"wrap";
		}
////////////////////////////////////////////////////////////////////////////////////
		if (bIsTop)
		{			
			SendMainProps(strMainPos, pMainProps);

			if (m_oxfrm_override.IsInit())
			{
				pSpPr->xfrm = m_oxfrm_override;
			}
			else	
			{
				pSpPr->xfrm = new PPTX::Logic::Xfrm();

				if (*pMainProps)
				{//oProps.X выставлять не надо, они учтены в pMainProps

					pSpPr->xfrm->offX = 0;
					pSpPr->xfrm->offY = 0;
				}
				else
				{
					pSpPr->xfrm->offX = oProps.X;
					pSpPr->xfrm->offY = oProps.Y;
				}
				pSpPr->xfrm->extX = oProps.Width;
				pSpPr->xfrm->extY = oProps.Height;
			}

			std::map<std::wstring, std::wstring>::iterator pFind = oCSSParser.m_mapSettings.find(L"flip");
			if (oCSSParser.m_mapSettings.end() != pFind)
			{
                if (pFind->second == L"x")
					pSpPr->xfrm->flipH = true;
                else if (pFind->second == L"y")
					pSpPr->xfrm->flipV = true;
				else if ((pFind->second == L"xy") || (pFind->second == L"yx") || (pFind->second == L"x y") || (pFind->second == L"y x")
					 || (pFind->second == L"y,x") || (pFind->second == L"x,y"))
				{
					pSpPr->xfrm->flipH = true;
					pSpPr->xfrm->flipV = true;
				}
			}

            pFind = oCSSParser.m_mapSettings.find(L"rotation");
			if (oCSSParser.m_mapSettings.end() != pFind)
			{
				pSpPr->xfrm->rot = NS_DWC_Common::getRotateAngle(pFind->second, pSpPr->xfrm->flipH, pSpPr->xfrm->flipV);
			}
		}
		else
		{
			m_pBinaryWriter->m_lXCurShape = 0;
			m_pBinaryWriter->m_lYCurShape = 0;

			m_pBinaryWriter->m_lCxCurShape = 0;
			m_pBinaryWriter->m_lCyCurShape = 0;

			pSpPr->xfrm = new PPTX::Logic::Xfrm();
			pSpPr->xfrm->offX = oProps.X;
			pSpPr->xfrm->offY = oProps.Y;
			pSpPr->xfrm->extX = oProps.Width;
			pSpPr->xfrm->extY = oProps.Height;

			std::map<std::wstring, std::wstring>::iterator pFind = oCSSParser.m_mapSettings.find(L"flip");
			if (oCSSParser.m_mapSettings.end() != pFind)
			{
                if (pFind->second == L"x")
					pSpPr->xfrm->flipH = true;
                else if (pFind->second == L"y")
					pSpPr->xfrm->flipV = true;
				else if ((pFind->second == L"xy") || (pFind->second == L"yx") || (pFind->second == L"x y") || (pFind->second == L"y x")
					 || (pFind->second == L"y,x") || (pFind->second == L"x,y"))
				{
					pSpPr->xfrm->flipH = true;
					pSpPr->xfrm->flipV = true;
				}
			}

            pFind = oCSSParser.m_mapSettings.find(L"rotation");
			if (oCSSParser.m_mapSettings.end() != pFind)
			{
				pSpPr->xfrm->rot = NS_DWC_Common::getRotateAngle(pFind->second, pSpPr->xfrm->flipH, pSpPr->xfrm->flipV);
			}
		}

		if (!bIsTop)
		{
			pCNvPr->id = m_lNextId;
			m_lNextId++;
		}
		else
		{
			pCNvPr->id = -1;
		}
		
		if (pShape)
		{
			pShape->signatureLine = pPPTShape->m_oSignatureLine;
		}

		if (bStroked)
		{
			CheckPenShape(elem, oNodeShape, pPPTShape);		
		}
		
		CheckBrushShape(elem, oNodeShape, pPPTShape);

		CheckBorderShape(elem, oNodeShape, pPPTShape);
	}
}
void CDrawingConverter::ConvertWordArtShape(PPTX::Logic::SpTreeElem* elem, XmlUtils::CXmlNode& oNodeShape, CPPTShape* pPPTShape)
{
	if (!elem) return;
	if (!pPPTShape) return;

	PPTX::Logic::Shape* pShape = dynamic_cast<PPTX::Logic::Shape*>(elem->GetElem().operator ->());
	if (!pShape) return;

	enum EFilltype
	{
		etBlipFill = 0,
		etGradFill = 1,
		etNoFill = 2,
		etPattFill = 3,
		etSolidFill = 4
	};

	PPTShapes::ShapeType eShapeType = pPPTShape->m_eType;
	SimpleTypes::ETextShapeType eTextShapeType;
	switch (eShapeType)
	{
	case PPTShapes::ShapeType::sptCTextPlain:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextPlain; break;
	case PPTShapes::ShapeType::sptCTextArchUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchUp; break;
	case PPTShapes::ShapeType::sptCTextArchDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchDown; break;
	case PPTShapes::ShapeType::sptCTextButton:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextButton; break;
	case PPTShapes::ShapeType::sptCTextCurveUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCurveUp; break;
	case PPTShapes::ShapeType::sptCTextCurveDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCurveDown; break;
	case PPTShapes::ShapeType::sptCTextCanUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCanUp; break;
	case PPTShapes::ShapeType::sptCTextCanDown:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCanDown; break;
	case PPTShapes::ShapeType::sptCTextWave1:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave1; break;
	case PPTShapes::ShapeType::sptCTextWave2:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave2; break;
	case PPTShapes::ShapeType::sptCTextWave3:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDoubleWave1; break;
	case PPTShapes::ShapeType::sptCTextWave4:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextWave4; break;
	case PPTShapes::ShapeType::sptCTextInflate:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflate; break;
	case PPTShapes::ShapeType::sptCTextDeflate:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflate; break;
	case PPTShapes::ShapeType::sptCTextInflateBottom:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflateBottom; break;
	case PPTShapes::ShapeType::sptCTextDeflateBottom:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateBottom; break;
	case PPTShapes::ShapeType::sptCTextInflateTop:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextInflateTop; break;
	case PPTShapes::ShapeType::sptCTextDeflateTop:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateTop; break;
	case PPTShapes::ShapeType::sptCTextDeflateInflate:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateInflate; break;
	case PPTShapes::ShapeType::sptCTextDeflateInflateDeflate:	eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextDeflateInflateDeflate; break;
	case PPTShapes::ShapeType::sptCTextFadeRight:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeRight; break;
	case PPTShapes::ShapeType::sptCTextFadeLeft:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeLeft; break;
	case PPTShapes::ShapeType::sptCTextFadeUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeUp; break;
	case PPTShapes::ShapeType::sptCTextFadeDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextFadeDown; break;
	case PPTShapes::ShapeType::sptCTextSlantUp:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextSlantUp; break;
	case PPTShapes::ShapeType::sptCTextSlantDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextSlantDown; break;
	case PPTShapes::ShapeType::sptCTextCascadeUp:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCascadeUp; break;
	case PPTShapes::ShapeType::sptCTextCascadeDown:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCascadeDown; break;
	case PPTShapes::ShapeType::sptCTextButtonPour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextButtonPour; break;
	case PPTShapes::ShapeType::sptCTextStop:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextStop; break;
	case PPTShapes::ShapeType::sptCTextTriangle:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextTriangle; break;
	case PPTShapes::ShapeType::sptCTextTriangleInverted:		eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextTriangleInverted; break;
	case PPTShapes::ShapeType::sptCTextChevron:					eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextChevron; break;
	case PPTShapes::ShapeType::sptCTextChevronInverted:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextChevronInverted; break;
	case PPTShapes::ShapeType::sptCTextRingInside:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextRingInside; break;
	case PPTShapes::ShapeType::sptCTextRingOutside:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextRingOutside; break;
	case PPTShapes::ShapeType::sptCTextCirclePour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextCirclePour; break;
	case PPTShapes::ShapeType::sptCTextArchUpPour:				eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchUpPour; break;
	case PPTShapes::ShapeType::sptCTextArchDownPour:			eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextArchDownPour; break;
	default:													eTextShapeType = SimpleTypes::ETextShapeType::textshapetypeTextNoShape; break;
	}
	SimpleTypes::CTextShapeType oTextShapeType;
	oTextShapeType.SetValue(eTextShapeType);

	std::wstring strPrstTxWarp = L"<a:prstTxWarp prst=\"" + oTextShapeType.ToString() + L"\"><a:avLst/></a:prstTxWarp>";

	XmlUtils::CXmlNode oPrstTxWarpNode;
	oPrstTxWarpNode.FromXmlString(strPrstTxWarp);

	pShape->oTextBoxBodyPr->prstTxWarp = oPrstTxWarpNode;

	XmlUtils::CXmlNodes oChilds;
	if (oNodeShape.GetNodes(L"*", oChilds))
	{
		LONG lChildsCount = oChilds.GetCount();

		std::vector<std::wstring>	wordArtString;
		EFilltype					eFillType = etNoFill;;
		std::wstring				sTxbxContent = L"<w:txbxContent>";
		std::wstring				sFont = L"Arial Black";
		int							nFontSize = 36;
		bool						bBold = false;
		bool						bItalic = false;

		BYTE						lAlpha;
		bool						bOpacity = false;
		bool						bOpacity2 = false;
		double						nFocus = 0;
		int							nAngle = 90;
		bool						bColors = false;

		PPTX::Logic::ColorModifier				oMod;
		PPTX::Logic::ColorModifier				oMod2;
		std::vector<PPTX::Logic::UniColor*>		arColors;
		std::vector<PPTX::Logic::UniColor*>		arColorsNew;
		std::vector<int>						arPos;
		std::vector<int>						arPosNew;
		std::map<PPTX::Logic::UniColor*, int>	arGradMap;

		int R = 255, G = 255, B = 255;

		nullable_string sFillColor;
		XmlMacroReadAttributeBase(oNodeShape, L"fillcolor", sFillColor);
		if (sFillColor.is_init())
		{
			eFillType = etSolidFill;

			ODRAW::CColor color;
			if (NS_DWC_Common::getColorFromString(*sFillColor, color))
			{
				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";

				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				pSolid->Color.Color->SetRGB(color.R, color.G, color.B);
				arColors.push_back(&pSolid->Color);
				arPos.push_back(0);

				R = color.R;
				G = color.G;
				B = color.B;
			}
		}

		if (eFillType == etNoFill)
		{
			// default color for vml = white
			nullable_string sFilled;
			XmlMacroReadAttributeBase(oNodeShape, L"filled", sFilled);
			if (!sFilled.is_init() || (*sFilled != L"false") && *sFilled != L"f")
			{
				eFillType = etSolidFill;

				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				pSolid->Color.Color->SetRGB(R, G, B);
				arColors.push_back(&pSolid->Color);
				arPos.push_back(0);
			}
		}

		for (LONG k = 0; k < lChildsCount; k++)
		{
			XmlUtils::CXmlNode oNodeP;
			oChilds.GetAt(k, oNodeP);

			std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeP.GetName());
			if (L"textpath" == strNameP)
			{
				std::wstring tmpString = oNodeP.GetText();	//для обхода &#xA пишется дубль в контент

				if (tmpString.empty())
				{
					tmpString = oNodeP.GetAttribute(L"string");
					CorrectXmlString(tmpString);
					wordArtString.push_back(tmpString);
				}
				else
				{
					CorrectXmlString(tmpString);

					int pos1 = 0, pos2 = 0;

					while (pos1 < (int)tmpString.length() && pos2 < (int)tmpString.length())
					{
						pos2 = (int)tmpString.find(L"\n", pos1);
						if (pos2 > 0)
						{
							wordArtString.push_back(tmpString.substr(pos1, pos2 - pos1));
							pos1 = pos2 + 1;
						}
						else break;
					}
					wordArtString.push_back(tmpString.substr(pos1, tmpString.length() - pos1));
				}

				SimpleTypes::Vml::CCssStyle oCssStyle;
				if (pPPTShape->m_textPath.sStyle)
				{
					oCssStyle.FromString(*pPPTShape->m_textPath.sStyle);
				}

				std::map<SimpleTypes::Vml::ECssPropertyType, size_t>::iterator pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptFontFamily);
				if (pFind != oCssStyle.m_mapProperties.end())
				{
					sFont = oCssStyle.m_arrProperties[pFind->second]->get_Value().wsValue;

					XmlUtils::replace_all(sFont, L"\"", L"");
				}
				pFind = pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptFontSize);
				if (pFind != oCssStyle.m_mapProperties.end())
				{
					nFontSize = oCssStyle.m_arrProperties[pFind->second]->get_Value().oValue.dValue;
				}
				pFind = pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptFontStyle);
				if (pFind != oCssStyle.m_mapProperties.end())
				{
					bItalic = (oCssStyle.m_arrProperties[pFind->second]->get_Value().eFontStyle == SimpleTypes::Vml::cssfontstyleItalic);
				}
				pFind = pFind = oCssStyle.m_mapProperties.find(SimpleTypes::Vml::cssptFontWeight);
				if (pFind != oCssStyle.m_mapProperties.end())
				{
					bBold = (oCssStyle.m_arrProperties[pFind->second]->get_Value().eFontWeight >= SimpleTypes::Vml::cssfontweight400);
				}
			}
			else if (L"shadow" == strNameP)
			{
				OOX::Vml::CShadow shadow; shadow.fromXML(oNodeP);
				if (shadow.m_oOn.GetBool())
				{
				}
			}
			else if (L"fill" == strNameP)
			{
				nullable_string sOpacity;
				nullable_string sOpacity2;
				nullable_string sColor2;
				nullable_string sColor;
				nullable_string sType;
				nullable_string sFocus;
				nullable_string sFocusSize;
				nullable_string sFocusPosition;
				nullable_string sAngle;
				nullable_string sColors;

				XmlMacroReadAttributeBase(oNodeP, L"opacity", sOpacity);
				XmlMacroReadAttributeBase(oNodeP, L"opacity2", sOpacity2);
				XmlMacroReadAttributeBase(oNodeP, L"color", sColor);
				XmlMacroReadAttributeBase(oNodeP, L"color2", sColor2);
				XmlMacroReadAttributeBase(oNodeP, L"type", sType);
				XmlMacroReadAttributeBase(oNodeP, L"focus", sFocus);
				XmlMacroReadAttributeBase(oNodeP, L"focussize", sFocusSize);
				XmlMacroReadAttributeBase(oNodeP, L"focusposition", sFocusPosition);
				XmlMacroReadAttributeBase(oNodeP, L"angle", sAngle);
				XmlMacroReadAttributeBase(oNodeP, L"colors", sColors);

				if (sType.is_init())
				{
					if (*sType == L"gradient")          eFillType = etGradFill;
					else	if (*sType == L"gradientradial")	eFillType = etGradFill;
					else	if (*sType == L"pattern")           eFillType = etPattFill;
					else	if (*sType == L"tile")              eFillType = etBlipFill;
					else	if (*sType == L"frame")             eFillType = etBlipFill;
				}
				else
				{
					if (sFocus.is_init() || sColors.is_init() || sAngle.is_init() || sFocusSize.is_init() || sFocusPosition.is_init())
						eFillType = etGradFill;
				}

				if (sFocus.is_init())
				{
                    nFocus = XmlUtils::GetInteger(*sFocus) / 100.0;
				}

				if (sOpacity.is_init())
				{
					bOpacity = true;
					lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);
					oMod.name = L"alpha";
					oMod.val = (int)((1 - lAlpha / 255.0) * 100000.0);

					if (arColors.at(0)->is_init())
						arColors.at(0)->Color->Modifiers.push_back(oMod);
				}
				if (sColor.is_init())
				{
					ODRAW::CColor color;
					if (sColor->find(L"fill") != -1)
					{
						std::wstring sColorEffect = *sColor;
						if (sColorEffect.length() > 5)
							sColorEffect = sColorEffect.substr(5);

						int resR, resG, resB;
						GetColorWithEffect(sColorEffect, R, G, B, resR, resG, resB);

						color.R = resR;
						color.G = resG;
						color.B = resB;

						PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
						oColor->Color = new PPTX::Logic::SrgbClr();
						oColor->Color->SetRGB(color.R, color.G, color.B);

						if (bOpacity)
							oColor->Color->Modifiers.push_back(oMod);

						arColors[0] = oColor;
					}
					else
					{
						if (NS_DWC_Common::getColorFromString(*sColor, color))
						{
							PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
							oColor->Color = new PPTX::Logic::SrgbClr();
							oColor->Color->SetRGB(color.R, color.G, color.B);

							if (bOpacity)
								oColor->Color->Modifiers.push_back(oMod);

							arColors[0] = oColor;
						}
					}
				}
				if (sOpacity2.is_init())
				{
					bOpacity2 = true;
					lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity2);
					oMod.name = L"alpha";
					oMod2.val = (int)((1 - lAlpha / 255.) * 100000.);

					if (arColors.at(1)->is_init())
						arColors.at(1)->Color->Modifiers.push_back(oMod2);
				}
				if (sColor2.is_init())
				{
					ODRAW::CColor color;
					if (sColor2->find(L"fill") != -1)
					{
						std::wstring sColorEffect = *sColor2;
						if (sColorEffect.length() > 5)
							sColorEffect = sColorEffect.substr(5);

						int resR, resG, resB;
						GetColorWithEffect(sColorEffect, R, G, B, resR, resG, resB);

						color.R = resR;
						color.G = resG;
						color.B = resB;
					}
					else
					{
						if (NS_DWC_Common::getColorFromString(*sColor2, color))
						{
						}
					}

					PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
					oColor->Color = new PPTX::Logic::SrgbClr();
					oColor->Color->SetRGB(color.R, color.G, color.B);

					if (bOpacity2)
						oColor->Color->Modifiers.push_back(oMod2);

					if (arColors.size() > 0)
						arColors.push_back(oColor);
					else
					{
						//дублирование 
						PPTX::Logic::UniColor *oColor1 = new PPTX::Logic::UniColor();
						oColor1->Color = new PPTX::Logic::SrgbClr();
						oColor1->Color->SetRGB(color.R, color.G, color.B);

						arColors.push_back(oColor1);
						arPos.push_back(0);
						arColors.push_back(oColor);
					}
					arPos.push_back(100000);
				}
				if (sAngle.is_init())
				{
                    nAngle = XmlUtils::GetInteger(*sAngle);
					nAngle = (-1) * nAngle + 90;
				}
				if (sColors.is_init())
				{
					bColors = true;
					std::wstring strColors = sColors.get();

					arColors.clear();
					arPos.clear();

					std::vector<std::wstring> arSplit;
					boost::algorithm::split(arSplit, strColors, boost::algorithm::is_any_of(L";"), boost::algorithm::token_compress_on);

					for (size_t i = 0; i < arSplit.size(); i++)
					{
						int p = (int)arSplit[i].find(L" ");

						std::wstring strPos = arSplit[i].substr(0, p);
						std::wstring strColor = arSplit[i].substr(p + 1);

						double pos = XmlUtils::GetDouble(strPos);

						ODRAW::CColor color;
						if (NS_DWC_Common::getColorFromString(strColor, color))
						{
							PPTX::Logic::UniColor *oColor = new PPTX::Logic::UniColor();
							oColor->Color = new PPTX::Logic::SrgbClr();
							oColor->Color->SetRGB(color.R, color.G, color.B);

							if (pos <= 1)
								pos = 100000 * pos;
							else
								pos = pos / 65536 * 100000;
							arColors.push_back(oColor);
							arPos.push_back((int)pos);

							arGradMap.insert(std::pair<PPTX::Logic::UniColor*, int>(oColor, (int)pos));
						}
					}
				}

			}
			/*else if (L"dashstyle") == strNameP)
			{
			nullable_string sStrokeDashStyle;
			oNodeP.ReadAttributeBase(L"dashstyle", sStrokeDashStyle);
			if (sStrokeDashStyle.is_init())
			{
			if (*sStrokeDashStyle == L"solid"))
			sDashStyle = L"solid");
			else if (*sStrokeDashStyle == L"shortdash"))
			sDashStyle = L"sysDash");
			else if (*sStrokeDashStyle == L"shortdot"))
			sDashStyle = L"sysDot");
			else if (*sStrokeDashStyle == L"shortdashdot"))
			sDashStyle = L"sysDashDot");
			else if (*sStrokeDashStyle == L"shortdashdotdot"))
			sDashStyle = L"sysDashDotDot");
			else if (*sStrokeDashStyle == L"dot"))
			sDashStyle = L"dot");
			else if (*sStrokeDashStyle == L"dash"))
			sDashStyle = L"dash");
			else if (*sStrokeDashStyle == L"longdash"))
			sDashStyle = L"lgDash");
			else if (*sStrokeDashStyle == L"dashdot"))
			sDashStyle = L"dashDot");
			else if (*sStrokeDashStyle == L"longdashdot"))
			sDashStyle = L"lgDashDot");
			else if (*sStrokeDashStyle == L"longdashdotdot"))
			sDashStyle = L"lgDashDotDot");
			else
			sDashStyle = L"solid");
			}
			}*/
		}
		std::wstring strRPr, strPPr;

		strPPr = L"<w:jc w:val=\"center\"/>";

		if (bBold) strRPr += L"<w:b/><w:bCs/>";
		if (bItalic) strRPr += L"<w:i/><w:iCs/>";
		strRPr += L"<w:rFonts w:ascii=\"" + sFont + L"\" w:hAnsi=\"" + sFont + L"\"/>";
		strRPr += L"<w:sz w:val=\"" + std::to_wstring(nFontSize * 2) + L"\"/><w:szCs w:val=\"" + std::to_wstring(nFontSize * 2) + L"\"/>";

		nullable_string sStrokeColor;
		nullable_string sStrokeWeight;
		nullable_string sStroked;

		XmlMacroReadAttributeBase(oNodeShape, L"strokecolor", sStrokeColor);
		XmlMacroReadAttributeBase(oNodeShape, L"strokeweight", sStrokeWeight);
		XmlMacroReadAttributeBase(oNodeShape, L"stroked", sStroked);

		XmlUtils::CXmlNode oNodeStroke = oNodeShape.ReadNode(L"v:stroke");
		if (oNodeStroke.IsValid())
		{
			nullable_string sStrokeOn;
			XmlMacroReadAttributeBase(oNodeStroke, L"on", sStrokeOn);
			if (sStrokeOn.is_init())
			{
				sStroked.reset();
				sStroked = sStrokeOn;
			}
			nullable_string sStrokeColor1;
			XmlMacroReadAttributeBase(oNodeStroke, L"strokecolor", sStrokeColor1);
			if (sStrokeColor1.is_init())
			{
				sStrokeColor1.reset();
				sStrokeColor = sStrokeColor1;
			}
		}
		//textFill
		strRPr += L"<w14:textFill>";

		if (eFillType == etSolidFill)
		{
			strRPr += L"<w14:solidFill>";
			strRPr += arColors.at(0)->toXML();
			strRPr += L"</w14:solidFill>";
		}
		else if (eFillType == etGradFill)
		{
			strRPr += L"<w14:gradFill><w14:gsLst>";
			int		nSize = (int)arColors.size();
			bool	bRevert = false;
			int		nColorsLen = (int)arColors.size();

			int nDiff = nSize - 1;
			if (nFocus != 1 && nFocus != 0)
				nSize = nSize + nDiff;

			double nShift = 100000 / nSize;
			double dNewZero = 100000 * nFocus;

			//(0 < nFocus &&  nFocus < 1)
			if (((nAngle == 90) && (-1 < nFocus &&  nFocus < 0)) || ((nAngle != 90) && (0 < nFocus &&  nFocus < 1)))
			{
				if (nAngle == 90)
					dNewZero *= -1;

				arColorsNew.push_back(arColors.at(nColorsLen - 1));
				arPosNew.push_back(0);

				for (int i = nColorsLen - 2; i > 0; i--)
				{
					arColorsNew.push_back(arColors.at(i));

					double dPosNew = dNewZero * arPos.at(i) / 100000;
					arPosNew.push_back((int)dPosNew);
				}

				for (int i = 0; i < nColorsLen; i++)
				{
					arColorsNew.push_back(arColors.at(i));

					double dPosNew = dNewZero * arPos.at(i) / 100000 + dNewZero;
					arPosNew.push_back((int)dPosNew);
				}
			}
			//else if (-1 < nFocus &&  nFocus < 0)
			else if (((nAngle != 90) && (-1 < nFocus &&  nFocus < 0)) || ((nAngle == 90) && (0 < nFocus &&  nFocus < 1)))
			{
				if (nAngle != 90)
					dNewZero *= -1;

				for (int i = 0; i < nColorsLen - 1; i++)
				{
					arColorsNew.push_back(arColors.at(i));

					double dPosNew = dNewZero * arPos.at(i) / 100000;
					arPosNew.push_back((int)dPosNew);
				}

				arColorsNew.push_back(arColors.at(nColorsLen - 1));
				arPosNew.push_back((int)dNewZero);

				for (int i = nColorsLen - 2; i >= 0; i--)
				{
					arColorsNew.push_back(arColors.at(i));

					double n1 = 1 - (double)arPos.at(i) / 100000;
					double dPosNew = dNewZero * n1 + dNewZero;
					arPosNew.push_back((int)dPosNew);
				}
			}
			//nFocus == 0
			else if ((nAngle != 90 && nFocus == 0) || (nAngle == 90 && nFocus == 1))
			{
				for (int i = 0; i < nColorsLen; i++)
				{
					arColorsNew.push_back(arColors.at(i));
					arPosNew.push_back(arPos.at(i));
				}
			}
			//nFocus == 1
			else if ((nAngle != 90 && nFocus == 1) || (nAngle == 90 && nFocus == 0))
			{
				for (int i = nColorsLen - 1; i >= 0; i--)
				{
					arColorsNew.push_back(arColors.at(i));
					arPosNew.push_back(arPos.at(nColorsLen - i - 1));
				}
			}

			for (size_t i = 0; i < arColorsNew.size(); i++)
			{
				int pos = arPosNew.at(i);
				std::wstring color = arColorsNew.at(i)->toXML();
				std::wstring strPos = std::to_wstring(pos);

				strRPr += L"<w14:gs w14:pos = \"" + strPos + L"\">";
				strRPr += color;
				strRPr += L"</w14:gs>";
			}

			std::wstring strAngle = std::to_wstring(nAngle * 60000);
			strRPr += L"</w14:gsLst><w14:lin w14:ang=\"" + strAngle + L"\" w14:scaled=\"0\"/></w14:gradFill>";
		}
		else if (eFillType == etNoFill)
		{
			strRPr += L"<w14:noFill/>";
		}
		else
		{
			//не существует в природе
		}

		strRPr += L"</w14:textFill>";

		//textOutline
		double m_dValue = 1;
		if (sStrokeWeight.is_init())
		{
			std::wstring strW(*sStrokeWeight);
			int p = (int)strW.find(L"pt");
			if (p >= 0)
				strW.erase(p);

			m_dValue = XmlUtils::GetDouble(strW);
		}

		std::wstring strStrokeW = std::to_wstring((int)Pt_To_Emu(m_dValue));
		strRPr += L"<w14:textOutline w14:w=\"" + strStrokeW + L"\">";

		smart_ptr<PPTX::Logic::SolidFill> pSolid = new PPTX::Logic::SolidFill();
		pSolid->m_namespace = L"a";
		pSolid->Color.Color = new PPTX::Logic::SrgbClr();
		ODRAW::CColor color;

		bool bStroked = true;
		if (sStroked.is_init())
		{
			if (*sStroked == L"false" || *sStroked == L"f")
			{
				strRPr += L"<w14:noFill/>";
				bStroked = false;
			}
		}
		if (sStrokeColor.is_init())
		{
			ODRAW::CColor color;
			if (NS_DWC_Common::getColorFromString(*sStrokeColor, color))
			{
				pSolid->Color.Color->SetRGB(color.R, color.G, color.B);
			}
		}
		else
			pSolid->Color.Color->SetRGB(0x00, 0x00, 0x00);

		if (bStroked)
		{
			strRPr += pSolid->toXML();
			pPPTShape->m_bIsStroked = false;
		}

		strRPr += L"</w14:textOutline>";

		for (size_t i = 0; i < wordArtString.size(); i++)
		{
			std::wstring sParaRun = L"<w:r><w:rPr>" + strRPr + L"</w:rPr>" + L"<w:t>" + wordArtString[i] + L"</w:t></w:r>";

			sTxbxContent += L"<w:p><w:pPr>" + strPPr + L"<w:rPr>" + strRPr + L"</w:rPr></w:pPr>" + sParaRun + L"</w:p>";
		}
		sTxbxContent += L"</w:txbxContent>";
		pShape->strTextBoxShape = sTxbxContent;

		if (sStroked.is_init() == false && wordArtString.empty() == false)
		{
			bStroked = false;
		}
	}

	// у старого wordArt никаких отступов
	pShape->oTextBoxBodyPr->lIns = 0;
	pShape->oTextBoxBodyPr->tIns = 0;
	pShape->oTextBoxBodyPr->rIns = 0;
	pShape->oTextBoxBodyPr->bIns = 0;

	if (pPPTShape->m_textPath.bFitShape || pPPTShape->m_textPath.bFitPath)
		pShape->oTextBoxBodyPr->Fit.type = PPTX::Logic::TextFit::FitNormAuto;

	pShape->oTextBoxBodyPr->wrap = new PPTX::Limit::TextWrap(1);
	pShape->oTextBoxBodyPr->fromWordArt = true;

	if (!pPPTShape->m_textPath.bTrim)
	{
		// нужно для данного размера шейпа выставить отступы сверху и снизу
		// top: Ascent - CapHeight
		// bottom: Descent
	}
}
void CDrawingConverter::ConvertGroup(PPTX::Logic::SpTreeElem *result, XmlUtils::CXmlNode& oNode, std::wstring**& pMainProps, bool bIsTop)
{
	if (!result) return;

    PPTX::Logic::SpTree* pTree = new PPTX::Logic::SpTree(L"wp");

    if (bIsTop) pTree->m_lGroupIndex = 0;
    else        pTree->m_lGroupIndex = 1;

	XmlUtils::CXmlNodes oNodes;
	XmlUtils::CXmlNode oNodeBinData;
	
	//сначала shape type
    if (oNode.GetNodes(L"*", oNodes))
	{
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oNodeT;
			oNodes.GetAt(i, oNodeT);

			std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeT.GetName());

			if (L"shapetype" == strNameP)
			{
				AddShapeType(oNodeT);
			}
		}
	}
    if (oNode.GetNodes(L"*", oNodes))
	{
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oNodeT;
			oNodes.GetAt(i, oNodeT);

			std::wstring strNameP = XmlUtils::GetNameNoNS(oNodeT.GetName());

			if (L"binData" == strNameP)
			{
				oNodeBinData = oNode;
			}
			else if (L"shape"	== strNameP ||
                L"rect"         == strNameP ||
                L"oval"         == strNameP ||
                L"line"         == strNameP ||
                L"polyline"     == strNameP ||
                L"background"   == strNameP ||
                L"roundrect"    == strNameP)
			{
				PPTX::Logic::SpTreeElem _el; 
				if (oNodeBinData.IsValid())
				{
					_el.fromXML(oNodeBinData);
					oNodeBinData.Clear();
				}
				ConvertShape(&_el, oNodeT, pMainProps, false);
				
				if (_el.is_init())
					pTree->SpTreeElems.push_back(_el);
			}
            else if (L"group" == strNameP)
			{
				PPTX::Logic::SpTreeElem _el;
				ConvertGroup(&_el, oNodeT, pMainProps, false);

				if (_el.is_init())
					pTree->SpTreeElems.push_back(_el);
			}
			else
				continue;
		}
	}

    std::wstring strStyle = oNode.GetAttribute(L"style");

	PPTX::CCSS oCSSParser;
	oCSSParser.LoadFromString2(strStyle);

	CSpTreeElemProps oProps;
	oProps.IsTop = bIsTop;
	std::wstring strMainPos = GetDrawingMainProps(oNode, oCSSParser, oProps);

	LONG lCoordOriginX = 0;
	LONG lCoordOriginY = 0;
	LONG lCoordSizeW = oProps.Width;
	LONG lCoordSizeH = oProps.Height;
	
    std::wstring strCoordSize = oNode.GetAttributeOrValue(L"coordsize");
    if (strCoordSize != L"")
	{
		std::vector<std::wstring> oArray;
        boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

		if (oArray.size() >= 2)
		{
			lCoordSizeW = XmlUtils::GetInteger(oArray[0]);
			lCoordSizeH = XmlUtils::GetInteger(oArray[1]);
		}
	}

    std::wstring strCoordOrigin = oNode.GetAttributeOrValue(L"coordorigin");
    if (strCoordOrigin != L"")
	{
		std::vector<std::wstring> oArray;
        boost::algorithm::split(oArray, strCoordOrigin, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

		if (oArray.size() >= 2)
		{
			lCoordOriginX = XmlUtils::GetInteger(oArray[0]);
			lCoordOriginY = XmlUtils::GetInteger(oArray[1]);
		}
	}

	if (bIsTop)
	{
		SendMainProps(strMainPos, pMainProps);

		if (m_oxfrm_override.IsInit())
		{
			pTree->grpSpPr.xfrm = m_oxfrm_override;
			
			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;		
		}
		else
		{
			pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
			pTree->grpSpPr.xfrm->offX = 0;
			pTree->grpSpPr.xfrm->offY = 0;
			pTree->grpSpPr.xfrm->extX = oProps.Width;
			pTree->grpSpPr.xfrm->extY = oProps.Height;

			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;
		}

		std::map<std::wstring, std::wstring>::iterator pFind = oCSSParser.m_mapSettings.find(L"flip");
		if (oCSSParser.m_mapSettings.end() != pFind)
		{
            if (pFind->second == L"x")
				pTree->grpSpPr.xfrm->flipH = true;
            else if (pFind->second == L"y")
				pTree->grpSpPr.xfrm->flipV = true;
            else if ((pFind->second == L"xy") || (pFind->second == L"yx") || (pFind->second == L"x y") || (pFind->second == L"y x")
				 || (pFind->second == L"y,x") || (pFind->second == L"x,y"))
			{
				pTree->grpSpPr.xfrm->flipH = true;
				pTree->grpSpPr.xfrm->flipV = true;
			}
		}

        pFind = oCSSParser.m_mapSettings.find(L"rotation");
		if (oCSSParser.m_mapSettings.end() != pFind)
		{
			pTree->grpSpPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pFind->second, pTree->grpSpPr.xfrm->flipH, pTree->grpSpPr.xfrm->flipV);
		}
	}
	else
	{
		if (m_oxfrm_override.IsInit())
		{
			pTree->grpSpPr.xfrm = m_oxfrm_override;
		}
		else
		{
			pTree->grpSpPr.xfrm = new PPTX::Logic::Xfrm();
			pTree->grpSpPr.xfrm->offX = oProps.X;
			pTree->grpSpPr.xfrm->offY = oProps.Y;
			pTree->grpSpPr.xfrm->extX = oProps.Width;
			pTree->grpSpPr.xfrm->extY = oProps.Height;

			pTree->grpSpPr.xfrm->chOffX = lCoordOriginX;
			pTree->grpSpPr.xfrm->chOffY = lCoordOriginY;
			pTree->grpSpPr.xfrm->chExtX = lCoordSizeW;
			pTree->grpSpPr.xfrm->chExtY = lCoordSizeH;
		}

		std::map<std::wstring, std::wstring>::iterator pFind = oCSSParser.m_mapSettings.find(L"flip");
		if (oCSSParser.m_mapSettings.end() != pFind)
		{
            if (pFind->second == L"x")
				pTree->grpSpPr.xfrm->flipH = true;
            else if (pFind->second == L"y")
				pTree->grpSpPr.xfrm->flipV = true;
            else if ((pFind->second == L"xy") || (pFind->second == L"yx") || (pFind->second == L"x y") || (pFind->second == L"y x")
				 || (pFind->second == L"y,x") || (pFind->second == L"x,y"))
			{
				pTree->grpSpPr.xfrm->flipH = true;
				pTree->grpSpPr.xfrm->flipV = true;
			}
		}

        pFind = oCSSParser.m_mapSettings.find(L"rotation");
		if (oCSSParser.m_mapSettings.end() != pFind)
		{
			pTree->grpSpPr.xfrm->rot = NS_DWC_Common::getRotateAngle(pFind->second, pTree->grpSpPr.xfrm->flipH, pTree->grpSpPr.xfrm->flipV);			
		}
	}

	if (!bIsTop)
	{
		pTree->nvGrpSpPr.cNvPr.id = m_lNextId;
		m_lNextId++;
	}
	else
	{
		pTree->nvGrpSpPr.cNvPr.id = -1;
	}
	
	result->InitElem(pTree);
}
void CDrawingConverter::LoadCoordPos(XmlUtils::CXmlNode& oNode, CShapePtr pShape)
{
	if (!pShape) return;

	pShape->m_dXLogic	= 0;
	pShape->m_dYLogic	= 0;

	XmlUtils::CXmlNode oNodeTemplate;
    if (oNode.GetNode(L"coordorigin", oNodeTemplate))
	{
        std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(L"val");
        if (!strCoordSize.empty())
		{
			std::vector<std::wstring> oArray;
            boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (oArray.size() >= 2)
			{
                pShape->m_dXLogic = XmlUtils::GetInteger(oArray[0]);
                pShape->m_dYLogic = XmlUtils::GetInteger(oArray[1]);
			}
		}
	}
	else
	{
        std::wstring strCoordSize = oNode.GetAttributeOrValue(L"coordorigin");
        if (!strCoordSize.empty())
		{
			std::vector<std::wstring> oArray;
            boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (oArray.size() >= 2)
			{
                pShape->m_dXLogic = XmlUtils::GetInteger(oArray[0]);
                pShape->m_dYLogic = XmlUtils::GetInteger(oArray[1]);
			}
		}
	}

	pShape->getBaseShape()->m_oPath.SetCoordpos((LONG)pShape->m_dXLogic, (LONG)pShape->m_dYLogic);
}


void CDrawingConverter::LoadCoordSize(XmlUtils::CXmlNode& oNode, CShapePtr pShape)
{
	if (!pShape) return;
	
	pShape->m_dWidthLogic	= ShapeSizeVML;
	pShape->m_dHeightLogic	= ShapeSizeVML;

	XmlUtils::CXmlNode oNodeTemplate;
    if (oNode.GetNode(L"coordsize", oNodeTemplate))
	{
        std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(L"val");
        if (!strCoordSize.empty())
		{
			std::vector<std::wstring> oArray;
            boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (oArray.size() >= 2)
			{
                pShape->m_dWidthLogic  = (std::max)(XmlUtils::GetInteger(oArray[0]), 1);
                pShape->m_dHeightLogic = (std::max)(XmlUtils::GetInteger(oArray[1]), 1);
			}
		}
	}
	else
	{
        std::wstring strCoordSize = oNode.GetAttributeOrValue(L"coordsize");
        if (!strCoordSize.empty())
		{
			std::vector<std::wstring> oArray;
            boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			if (oArray.size() >= 2)
			{
                pShape->m_dWidthLogic  = (std::max)(XmlUtils::GetInteger(oArray[0]), 1);
                pShape->m_dHeightLogic = (std::max)(XmlUtils::GetInteger(oArray[1]), 1);
			}
		}
	}

	pShape->getBaseShape()->m_oPath.SetCoordsize((LONG)pShape->m_dWidthLogic, (LONG)pShape->m_dHeightLogic);
}

std::wstring CDrawingConverter::GetDrawingMainProps(XmlUtils::CXmlNode& oNode, PPTX::CCSS& oCssStyles, CSpTreeElemProps& oProps)
{
	std::map<std::wstring, std::wstring>::iterator pFind;

	bool bIsInline = false;
	bool bIsMargin = false;
	bool bZIndex = false;

	if (oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.find(L"z-index"))
		bZIndex = true;

	if (oProps.IsTop == true)
	{
		if ((oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(L"left"))			&&
			(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(L"margin-left"))	&&
			(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(L"top"))			&&
			(oCssStyles.m_mapSettings.end() == oCssStyles.m_mapSettings.find(L"margin-top")))
		{
			bIsInline = true;
		}
		if (((oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.find(L"margin-left"))	&&
			(oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.find(L"margin-top")))
		||
			((oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.find(L"left"))	&&
			(oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.find(L"top"))))
		{
			bIsMargin = true;
		}
		pFind = oCssStyles.m_mapSettings.find(L"mso-position-horizontal-relative");
		if (oCssStyles.m_mapSettings.end() != pFind && ((pFind->second == L"text" && !bIsMargin) || pFind->second == L"char"))
		{
			pFind = oCssStyles.m_mapSettings.find(L"mso-position-vertical-relative");
			if (oCssStyles.m_mapSettings.end() != pFind && ((pFind->second == L"text" && !bIsMargin) || pFind->second == L"line"))
			{	
				if (!bZIndex || !bIsMargin) //Liturgie Homberg 2017 mit Abendmahlsteil.docx
					bIsInline = true;
			}
		}	

		if (!bIsInline)
		{
			pFind = oCssStyles.m_mapSettings.find(L"position");
			if (oCssStyles.m_mapSettings.end() != pFind && pFind->second == L"static")
			{
				bIsInline = true;
			}
		}
	}

	SimpleTypes::CPoint parserPoint;
	double dKoef = 25.4 * 36000 / 72.0;
	double dKoefSize = oProps.IsTop ? dKoef : 1;

	LONG left	= 0;
	LONG top	= 0;
	LONG width	= 0;
	LONG height = 0;

    pFind = oCssStyles.m_mapSettings.find(L"polyline_correct");
	bool bIsPolyCorrect = (oCssStyles.m_mapSettings.end() != pFind) ? true : false;
	if (bIsPolyCorrect)
		dKoefSize = 1;

	if (!bIsInline)
	{
        pFind = oCssStyles.m_mapSettings.find(L"margin-left");
		if (oCssStyles.m_mapSettings.end() == pFind)
            pFind = oCssStyles.m_mapSettings.find(L"left");

		if (oCssStyles.m_mapSettings.end() != pFind)
		{
			std::vector<std::wstring> oArray1;
            boost::algorithm::split(oArray1, pFind->second, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

			for (size_t i = 0; i < oArray1.size(); i++)
			{
				left += (LONG)(dKoefSize * parserPoint.FromString(oArray1[i]) + 0.5);
			}
		}

        pFind = oCssStyles.m_mapSettings.find(L"margin-top");
		
		if (oCssStyles.m_mapSettings.end() == pFind)
            pFind = oCssStyles.m_mapSettings.find(L"top");

		if (oCssStyles.m_mapSettings.end() != pFind)
		{
			std::vector<std::wstring> oArray1;
			boost::algorithm::split(oArray1, pFind->second, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
			for (size_t i = 0; i < oArray1.size(); i++)
		{
				top += (LONG)(dKoefSize * parserPoint.FromString(oArray1[i]) + 0.5);
			}
		}
	}

    pFind = oCssStyles.m_mapSettings.find(L"width");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		width = (LONG)(dKoefSize * parserPoint.FromString(pFind->second) + 0.5);
	}
	else
	{
        pFind = oCssStyles.m_mapSettings.find(L"margin-right");
		if (oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.end())
			width = (LONG)(dKoefSize * parserPoint.FromString(pFind->second) + 0.5) - left;
	}

    pFind = oCssStyles.m_mapSettings.find(L"height");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		height = (LONG)(dKoefSize * parserPoint.FromString(pFind->second) + 0.5);
	}
	else
	{
        pFind = oCssStyles.m_mapSettings.find(L"margin-bottom");
		if (oCssStyles.m_mapSettings.end() != oCssStyles.m_mapSettings.end())
			height = (LONG)(dKoefSize * parserPoint.FromString(pFind->second) + 0.5) - top;
	}

	unsigned long margL = (unsigned long)(9 * dKoef + 0.5);
	unsigned long margT = 0;
	unsigned long margR = (unsigned long)(9 * dKoef + 0.5);
	unsigned long margB = 0;

    pFind = oCssStyles.m_mapSettings.find(L"mso-wrap-distance-left");
	if (oCssStyles.m_mapSettings.end() != pFind)
		margL = (unsigned long)(dKoef * parserPoint.FromString(pFind->second) + 0.5);

    pFind = oCssStyles.m_mapSettings.find(L"mso-wrap-distance-top");
	if (oCssStyles.m_mapSettings.end() != pFind)
		margT = (unsigned long)(dKoef * parserPoint.FromString(pFind->second) + 0.5);

    pFind = oCssStyles.m_mapSettings.find(L"mso-wrap-distance-right");
	if (oCssStyles.m_mapSettings.end() != pFind)
		margR = (unsigned long)(dKoef * parserPoint.FromString(pFind->second) + 0.5);

    pFind = oCssStyles.m_mapSettings.find(L"mso-wrap-distance-bottom");
	if (oCssStyles.m_mapSettings.end() != pFind)
		margB = (unsigned long)(dKoef * parserPoint.FromString(pFind->second) + 0.5);

	nullable_double rel_width;
	nullable_double rel_height;
	nullable_double rel_top;
	nullable_double rel_left;

    pFind = oCssStyles.m_mapSettings.find(L"mso-width-percent");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		rel_width = parserPoint.FromString(pFind->second) / 1000.;
	}
    pFind = oCssStyles.m_mapSettings.find(L"mso-height-percent");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		rel_height = parserPoint.FromString(pFind->second) / 1000.;
	}	
    pFind = oCssStyles.m_mapSettings.find(L"mso-top-percent");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		rel_top = parserPoint.FromString(pFind->second) / 1000.;
	}	
    pFind = oCssStyles.m_mapSettings.find(L"mso-left-percent");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		rel_left = parserPoint.FromString(pFind->second) / 1000.;
	}	

	oProps.X		= left;
	oProps.Y		= top;
	oProps.Width	= width;
	oProps.Height	= height;

	m_pBinaryWriter->m_lXCurShape = left;
	m_pBinaryWriter->m_lYCurShape = top;

	m_pBinaryWriter->m_lCxCurShape = width;
	m_pBinaryWriter->m_lCyCurShape = height;

 	bool bExtendedSize = false;
	XmlUtils::CXmlNode oNodeShadow = oNode.ReadNode(L"v:shadow");
    std::wstring strShadow;
	if (oNodeShadow.IsValid())
	{
		OOX::Vml::CShadow shadow; shadow.fromXML(oNodeShadow);
		if (shadow.m_oOn.GetBool())
		{
			bExtendedSize = true;
		}
	}
	std::wstring originalId = oNode.ReadAttribute(L"id");

	if (bIsInline)
	{
		NSBinPptxRW::CXmlWriter oWriter;
        oWriter.StartNode(L"wp:inline");

		oWriter.StartAttributes();
        oWriter.WriteAttribute(L"xmlns:wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing");
        oWriter.WriteAttribute(L"distT", margT);
        oWriter.WriteAttribute(L"distB", margB);
        oWriter.WriteAttribute(L"distL", margL);
        oWriter.WriteAttribute(L"distR", margR);
		oWriter.EndAttributes();

		oWriter.StartNode(L"wp:extent");
		oWriter.StartAttributes();
        oWriter.WriteAttribute(L"cx", width );
        oWriter.WriteAttribute(L"cy", height );
		oWriter.EndAttributes();
        oWriter.EndNode(L"wp:extent");

        oWriter.StartNode(L"wp:effectExtent");
		oWriter.StartAttributes();
		oWriter.WriteAttribute(L"l", bExtendedSize ? 10795 : 0);
        oWriter.WriteAttribute(L"t", bExtendedSize ? 5080 : 0);
        oWriter.WriteAttribute(L"r", bExtendedSize ? 28575 : 0);
        oWriter.WriteAttribute(L"b", bExtendedSize ? 26670 : 0);
		oWriter.EndAttributes();
        oWriter.EndNode(L"wp:effectExtent");

		if (rel_width.is_init())
		{
			oWriter.StartNode(L"wp14:sizeRelH");
			oWriter.StartAttributes();
				oWriter.WriteAttribute(L"relativeFrom", L"page");
			oWriter.EndAttributes();
			oWriter.StartNode(L"wp14:pctWidth");
			oWriter.EndAttributes();

			oWriter.WriteString(std::to_wstring((INT)(*rel_width * 100000)));

			oWriter.EndNode(L"wp14:pctWidth");
			oWriter.EndNode(L"wp14:sizeRelH");
		}
		if (rel_height.is_init())
		{
			oWriter.StartNode(L"wp14:sizeRelV");
			oWriter.StartAttributes();
				oWriter.WriteAttribute(L"relativeFrom", L"page");
			oWriter.EndAttributes();
			oWriter.StartNode(L"wp14:pctHeight");
			oWriter.EndAttributes();

			oWriter.WriteString(std::to_wstring((INT)(*rel_height * 100000)));

			oWriter.EndNode(L"wp14:pctHeight");
			oWriter.EndNode(L"wp14:sizeRelV");
		}

		std::wstring strId = L"<wp:docPr id=\"" + std::to_wstring(m_lNextId) + L"\" name=\"" + originalId + L"\"/>";
		m_lNextId++;

		oWriter.WriteString(strId);

        oWriter.EndNode(L"wp:inline");

		return oWriter.GetXmlString();
	}
//------------------------------------------------------------------------------------
	NSBinPptxRW::CXmlWriter oWriter;
    oWriter.StartNode(L"wp:anchor");

	oWriter.StartAttributes();

    oWriter.WriteAttribute(L"xmlns:wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing");
    oWriter.WriteAttribute(L"distT", margT);
    oWriter.WriteAttribute(L"distB", margB);
    oWriter.WriteAttribute(L"distL", margL);
    oWriter.WriteAttribute(L"distR", margR);

    pFind = oCssStyles.m_mapSettings.find(L"z-index");
	nullable_int64 zIndex;
	
	if (oCssStyles.m_mapSettings.end() != pFind)
	{ 
		zIndex = (__int64)parserPoint.FromString(pFind->second);
		
		_INT64 zIndex_ = *zIndex;// >= 0 ? *zIndex : -*zIndex;
		
		if (m_nDrawingMaxZIndex == 0 && ((zIndex_ < 0xF000000 && zIndex_ > 0x80000) || 
										(zIndex_ > -0xF000000 && zIndex_ < -0x80000)))
		{
			zIndex_ = 0xF000000 - 0x80000 + zIndex_;
		}
		else
		{
			zIndex_ = abs(zIndex_);
		}
		
		oWriter.WriteAttribute(L"relativeHeight", std::to_wstring(zIndex_));
	}

    
	XmlUtils::CXmlNode oNodeWrap = oNode.ReadNode(L"w10:wrap");
    
	std::wstring strWrapType;
	if (oNodeWrap.IsValid())
	{
        strWrapType = oNodeWrap.GetAttribute(L"type");

		/*
		nullable_string sAnchorX;
		oNodeWrap.ReadAttributeBase(L"anchorx", sAnchorX);
		nullable_string sAnchorY;
		oNodeWrap.ReadAttributeBase(L"anchory", sAnchorY);

		if (sAnchorX.is_init())
            oCssStyles.m_mapSettings.insert(std::pair<>(L"mso-position-horizontal-relative"), *sAnchorX);
		if (sAnchorY.is_init())
            oCssStyles.m_mapSettings.insert(std::pair<>(L"mso-position-vertical-relative"), *sAnchorY);
		*/
	}

    nullable_bool isAllowInCell;
    nullable_string sAllowInCell;

    XmlMacroReadAttributeBase(oNode, L"o:allowincell", sAllowInCell);
    if (sAllowInCell.is_init())
    {
        if ((L"f" == *sAllowInCell) || (L"false"== *sAllowInCell))
            isAllowInCell = false;
        if ((L"t" == *sAllowInCell) || (L"true"== *sAllowInCell))
            isAllowInCell = true;
    }
	nullable_bool isAllowOverlap;
    nullable_string sAllowOverlap;
	
    XmlMacroReadAttributeBase(oNode, L"o:allowoverlap", sAllowOverlap);
	if (sAllowOverlap.is_init())
    {
        if ((L"f" == *sAllowOverlap) || (L"false"== *sAllowOverlap))
            isAllowOverlap = false;
        if ((L"t" == *sAllowOverlap) || (L"true"== *sAllowOverlap))
            isAllowOverlap = true;
    }
    std::wstring strWrapPoints = oNode.GetAttribute(L"wrapcoords");
    std::wstring strWrapPointsResult;
    if (!strWrapPoints.empty())
	{
		std::vector<std::wstring> arPoints;
        boost::algorithm::split(arPoints, strWrapPoints, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

		int nCountP = (int)arPoints.size();
		if (nCountP > 1 && ((nCountP % 2) == 0))
		{
            strWrapPointsResult = L"<wp:wrapPolygon edited=\"1\">";

			for (int i = 0; i < nCountP; i += 2)
			{
				if (i == 0)
				{
                    strWrapPointsResult += (L"<wp:start x=\"" + arPoints[i] + L"\" y=\"" + arPoints[i + 1] + L"\"/>");
				}
				else
				{
                    strWrapPointsResult += (L"<wp:lineTo x=\"" + arPoints[i] + L"\" y=\"" + arPoints[i + 1] + L"\"/>");
				}
			}

            strWrapPointsResult += L"</wp:wrapPolygon>";
		}
	}

    if (zIndex.is_init())
	{
		if (*zIndex > 0)
		{
            oWriter.WriteAttribute(L"behindDoc", L"0");
		}
		else if (*zIndex < 0)
		{
            oWriter.WriteAttribute(L"behindDoc", L"1");
		}
	}
	if (isAllowOverlap.is_init())
	{
        if (*isAllowOverlap)
            oWriter.WriteAttribute(L"allowOverlap", L"1");
        else
            oWriter.WriteAttribute(L"allowOverlap", L"0");
	}
    if (isAllowInCell.is_init())
    {
        if (*isAllowInCell)
            oWriter.WriteAttribute(L"layoutInCell", L"1");
        else
            oWriter.WriteAttribute(L"layoutInCell", L"0");
    }

	oWriter.EndAttributes();

    oWriter.StartNode(L"wp:positionH");

	oWriter.StartAttributes();

    pFind = oCssStyles.m_mapSettings.find(L"mso-position-horizontal-relative");

	nullable_string sHRelativeFrom;
	nullable_string sVRelativeFrom;

	if (pFind != oCssStyles.m_mapSettings.end())
	{
			 if (L"char" == pFind->second)				sHRelativeFrom = L"character";
        else if (L"page" == pFind->second)				sHRelativeFrom = L"page";
        else if (L"margin" == pFind->second)			sHRelativeFrom = L"margin";
        else if (L"left-margin-area" == pFind->second)	sHRelativeFrom = L"leftMargin";
        else if (L"right-margin-area" == pFind->second)	sHRelativeFrom = L"rightMargin";
        else if (L"inner-margin-area" == pFind->second)	sHRelativeFrom = L"insideMargin";
        else if (L"outer-margin-area" == pFind->second)	sHRelativeFrom = L"outsideMargin";
		else
            sHRelativeFrom = L"column";
	}
	else
	{
        sHRelativeFrom = L"column";
	}
	oWriter.WriteAttribute(L"relativeFrom", *sHRelativeFrom);

	oWriter.EndAttributes();

    std::wstring strPosH = L"absolute";
    pFind = oCssStyles.m_mapSettings.find(L"mso-position-horizontal");
	if (oCssStyles.m_mapSettings.end() != pFind)
		strPosH = pFind->second;

    if (strPosH == L"absolute")
	{
		if (rel_left.is_init())
		{
			oWriter.WriteString(L"<wp14:pctPosHOffset>");
			oWriter.WriteLONG((INT)(*rel_left * 100000));
			oWriter.WriteString(L"</wp14:pctPosHOffset>");
		}
		else
		{
			oWriter.WriteString(L"<wp:posOffset>");
			oWriter.WriteLONG(left);
			oWriter.WriteString(L"</wp:posOffset>");
		}
	}
	else
	{
        oWriter.WriteString(L"<wp:align>");
		oWriter.WriteString(strPosH);
        oWriter.WriteString(L"</wp:align>");
	}

    oWriter.EndNode(L"wp:positionH");

    oWriter.StartNode(L"wp:positionV");

	oWriter.StartAttributes();

    pFind = oCssStyles.m_mapSettings.find(L"mso-position-vertical-relative");
	if (pFind != oCssStyles.m_mapSettings.end())
	{
			 if (L"margin" == pFind->second)				sVRelativeFrom = L"margin";
        else if (L"text" == pFind->second)					sVRelativeFrom = L"paragraph";
        else if (L"page" == pFind->second)					sVRelativeFrom = L"page";
        else if (L"top-margin-area" == pFind->second)		sVRelativeFrom = L"topMargin";
        else if (L"bottom-margin-area" == pFind->second)	sVRelativeFrom = L"bottomMargin";
        else if (L"inner-margin-area" == pFind->second)		sVRelativeFrom = L"insideMargin";
        else if (L"outer-margin-area" == pFind->second)		sVRelativeFrom = L"outsideMargin";
		else
            sVRelativeFrom = L"line";
	}
	else
	{
        sVRelativeFrom = L"paragraph";
	}
	oWriter.WriteAttribute(L"relativeFrom", *sVRelativeFrom);

	oWriter.EndAttributes();

    std::wstring strPosV = L"absolute";
    pFind = oCssStyles.m_mapSettings.find(L"mso-position-vertical");
	if (oCssStyles.m_mapSettings.end() != pFind)
		strPosV = pFind->second;

    if (strPosV == L"absolute")
	{
		if (rel_top.is_init())
		{
			oWriter.WriteString(L"<wp14:pctPosVOffset>");
			oWriter.WriteLONG((INT)(*rel_top * 100000));
			oWriter.WriteString(L"</wp14:pctPosVOffset>");
		}
		else			
		{
			oWriter.WriteString(L"<wp:posOffset>");
			oWriter.WriteLONG(top);
			oWriter.WriteString(L"</wp:posOffset>");
		}
	}
	else
	{
        oWriter.WriteString(L"<wp:align>");
		oWriter.WriteString(strPosV);
        oWriter.WriteString(L"</wp:align>");
	}

    oWriter.EndNode(L"wp:positionV");

    oWriter.StartNode(L"wp:extent");
	oWriter.StartAttributes();
    oWriter.WriteAttribute(L"cx", width );
    oWriter.WriteAttribute(L"cy", height);
	oWriter.EndAttributes();
    oWriter.EndNode(L"wp:extent");
    
	oWriter.StartNode(L"wp:effectExtent");
	oWriter.StartAttributes();
	oWriter.WriteAttribute(L"l", bExtendedSize ? 10795 : 0);
    oWriter.WriteAttribute(L"t", bExtendedSize ? 5080 : 0);
    oWriter.WriteAttribute(L"r", bExtendedSize ? 28575 : 0);
    oWriter.WriteAttribute(L"b", bExtendedSize ? 26670 : 0);
	oWriter.EndAttributes();
    oWriter.EndNode(L"wp:effectExtent");

	if (oNodeWrap.IsValid())
	{		
        if (strWrapType == L"none" || strWrapType == L"")
            oWriter.WriteString(L"<wp:wrapNone/>");
        else if (strWrapType == L"square")
            oWriter.WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
        else if (strWrapType == L"topAndBottom")
            oWriter.WriteString(L"<wp:wrapTopAndBottom/>");
        else if (strWrapType == L"tight")
		{
            if (strWrapPointsResult.empty())
			{
                oWriter.WriteString(L"<wp:wrapTight wrapText=\"bothSides\"/>");
			}
			else
			{
                oWriter.WriteString(L"<wp:wrapTight wrapText=\"bothSides\">");
				oWriter.WriteString(strWrapPointsResult);
                oWriter.WriteString(L"</wp:wrapTight>");
			}			
		}
        else if (strWrapType == L"through")
		{
            if (strWrapPointsResult.empty())
			{
                oWriter.WriteString(L"<wp:wrapThrough wrapText=\"bothSides\"/>");
			}
			else
			{
                oWriter.WriteString(L"<wp:wrapThrough wrapText=\"bothSides\">");
				oWriter.WriteString(strWrapPointsResult);
                oWriter.WriteString(L"</wp:wrapThrough>");
			}						
		}		
		else 
            oWriter.WriteString(L"<wp:wrapSquare wrapText=\"bothSides\"/>");
	}	
	else
	{
        oWriter.WriteString(L"<wp:wrapNone/>");
	}	
	bool bHidden = false;
    pFind = oCssStyles.m_mapSettings.find(L"visibility");
	if (oCssStyles.m_mapSettings.end() != pFind)
	{
		if (L"hidden" == pFind->second)
			bHidden = true;
	}
	if (rel_width.is_init())
	{
		oWriter.StartNode(L"wp14:sizeRelH");
		oWriter.StartAttributes();
			oWriter.WriteAttribute(L"relativeFrom", *sHRelativeFrom);
		oWriter.EndAttributes();
		oWriter.StartNode(L"wp14:pctWidth");
		oWriter.EndAttributes();

		oWriter.WriteString(std::to_wstring((INT)(*rel_width * 100000)));

		oWriter.EndNode(L"wp14:pctWidth");
		oWriter.EndNode(L"wp14:sizeRelH");
	}
	if (rel_height.is_init())
	{
		oWriter.StartNode(L"wp14:sizeRelV");
		oWriter.StartAttributes();
			oWriter.WriteAttribute(L"relativeFrom", *sVRelativeFrom);
		oWriter.EndAttributes();
		oWriter.StartNode(L"wp14:pctHeight");
		oWriter.EndAttributes();

		oWriter.WriteString(std::to_wstring((INT)(*rel_height * 100000)));

		oWriter.EndNode(L"wp14:pctHeight");
		oWriter.EndNode(L"wp14:sizeRelV");
	}

	std::wstring strId = L"<wp:docPr id=\"" + std::to_wstring(m_lNextId) + L"\" name=\"" + originalId + L"\"" + (bHidden ? L" hidden=\"true\"" : L"") + L"/>";
	m_lNextId++;

	oWriter.WriteString(strId);

    oWriter.EndNode(L"wp:anchor");
	
	return oWriter.GetXmlString();
}


std::wstring CDrawingConverter::GetVMLShapeXml(PPTX::Logic::SpTreeElem& oElem)
{
	CPPTXShape* pShapePPTX = NULL;
	if (oElem.is<PPTX::Logic::Shape>())
	{
		const PPTX::Logic::Shape& lpOriginShape = oElem.as<PPTX::Logic::Shape>();

		if (lpOriginShape.spPr.Geometry.is<PPTX::Logic::PrstGeom>())
		{
			const PPTX::Logic::PrstGeom lpGeom = lpOriginShape.spPr.Geometry.as<PPTX::Logic::PrstGeom>();

			OOXMLShapes::ShapeType _lspt = PPTX2EditorAdvanced::GetShapeTypeFromStr(lpGeom.prst.get());
			if(_lspt != OOXMLShapes::sptNil) 
			{
				pShapePPTX = new CPPTXShape();
				pShapePPTX->SetType(NSBaseShape::pptx, _lspt);

				std::wstring strAdjustValues = lpGeom.GetODString();
				pShapePPTX->LoadAdjustValuesList(strAdjustValues);
			}
		}
		else if(lpOriginShape.spPr.Geometry.is<PPTX::Logic::CustGeom>())
		{
			const PPTX::Logic::CustGeom lpGeom = lpOriginShape.spPr.Geometry.as<PPTX::Logic::CustGeom>();
			std::wstring strShape = lpGeom.GetODString();
			pShapePPTX = new CPPTXShape();
			pShapePPTX->LoadFromXML(strShape);
		}
		else
		{
			pShapePPTX = new CPPTXShape();
			pShapePPTX->SetType(NSBaseShape::pptx, (int)OOXMLShapes::sptCRect);
		}								
	}

	if (NULL != pShapePPTX)
	{
		NSGuidesVML::CConverterPPTXPPT oConverterPPTX_2_PPT;
		oConverterPPTX_2_PPT.Convert(pShapePPTX);

		std::wstring sDumpXml = GetVMLShapeXml(oConverterPPTX_2_PPT.GetConvertedShape());

		return sDumpXml;
	}

    return L"";
}

std::wstring CDrawingConverter::GetVMLShapeXml(CPPTShape* pPPTShape)
{
	NSBinPptxRW::CXmlWriter oXmlWriter;
    oXmlWriter.StartNode(L"v:shape");
	oXmlWriter.StartAttributes();

    std::wstring strCoordSize;
	LONG lCoordW = 21600;
	LONG lCoordH = 21600;
	if (0 < pPPTShape->m_oPath.m_arParts.size())
	{
		lCoordW = pPPTShape->m_oPath.m_arParts[0].width;
		lCoordH = pPPTShape->m_oPath.m_arParts[0].height;
	}
    strCoordSize = std::to_wstring(lCoordW) + L"," + std::to_wstring(lCoordH);
    oXmlWriter.WriteAttribute(L"coordsize", strCoordSize);

	int nAdjCount = (int)pPPTShape->m_arAdjustments.size();
	if (nAdjCount > 0)
	{
        oXmlWriter.WriteString(L" adj=\"");

		for (int i = 0; i < nAdjCount; ++i)
		{
			if (0 != i)
			{
                std::wstring s = L"," + std::to_wstring(pPPTShape->m_arAdjustments[i]);
				oXmlWriter.WriteString(s);
			}
			else
			{
                oXmlWriter.WriteString(std::to_wstring(pPPTShape->m_arAdjustments[i]));
			}
		}

        oXmlWriter.WriteString(L"\"");
	}

    oXmlWriter.WriteAttribute(L"path", pPPTShape->m_strPath);
	oXmlWriter.EndAttributes();

	std::vector<CFormula>& arGuides = pPPTShape->m_oManager.m_arFormulas;
	int nGuides = (int)arGuides.size();
	if (nGuides != 0)
	{
        oXmlWriter.StartNode(L"v:formulas");
		oXmlWriter.StartAttributes();
		oXmlWriter.EndAttributes();

		for (int i = 0; i < nGuides; ++i)
		{
			CFormula& oGuide = arGuides[i];
			if ((int)oGuide.m_eFormulaType >= VML_GUIDE_COUNT)
				break;

            oXmlWriter.WriteString(L"<v:f eqn=\"");
			
            oXmlWriter.WriteString(VML_GUIDE_TYPE[(int)oGuide.m_eFormulaType]);
			
			BYTE nParams = VML_GUIDE_PARAM_COUNT[(int)oGuide.m_eFormulaType];
			if (nParams > 0)
			{
                std::wstring str;
				if (oGuide.m_eType1 == ptAdjust)
				{
                    str = L" #";
				}
				else if (oGuide.m_eType1 == ptFormula)
				{
                    str = L" @";
				}
				else
				{
                    str = L" ";
				}
                str += std::to_wstring(oGuide.m_lParam1);
                oXmlWriter.WriteString(str);
			}
			if (nParams > 1)
			{
                std::wstring str;
				if (oGuide.m_eType2 == ptAdjust)
				{
                     str = L" #";
				}
				else if (oGuide.m_eType2 == ptFormula)
				{
                     str = L" @";
				}
				else
				{
                     str = L" ";
				}
                str += std::to_wstring(oGuide.m_lParam2);
                oXmlWriter.WriteString(str);
			}
			if (nParams > 2)
			{
                std::wstring str ;
				if (oGuide.m_eType3 == ptAdjust)
				{
                    str = L" #";
				}
				else if (oGuide.m_eType3 == ptFormula)
				{
                    str = L" @";
				}
				else
				{
                    str = L" ";
				}
                str += std::to_wstring(oGuide.m_lParam3);
				oXmlWriter.WriteString(str);
			}

            oXmlWriter.WriteString(L"\"/>");
		}

        oXmlWriter.EndNode(L"v:formulas");

		size_t nTextRectCount = pPPTShape->m_arStringTextRects.size();
		if (0 < nTextRectCount)
		{
            oXmlWriter.WriteString(L"<v:path textboxrect=\"");

			for (size_t i = 0; i < nTextRectCount; ++i)
			{
				if (0 != i)
                    oXmlWriter.WriteString(L",");

				oXmlWriter.WriteString(pPPTShape->m_arStringTextRects[i]);
			}

            oXmlWriter.WriteString(L"\"/>");
		}

		int nHandles = (int)pPPTShape->m_arHandles.size();
		if (0 < nHandles)
		{
            oXmlWriter.StartNode(L"v:handles");
			oXmlWriter.StartAttributes();
			oXmlWriter.EndAttributes();

			for (int i = 0; i < nHandles; ++i)
			{
                oXmlWriter.StartNode(L"v:h");
				
				CHandle_& oH = pPPTShape->m_arHandles[i];

                if (oH.position != L"")
                    oXmlWriter.WriteAttribute(L"position", oH.position);

                if (oH.xrange != L"")
                    oXmlWriter.WriteAttribute(L"xrange", oH.xrange);

                if (oH.yrange != L"")
                    oXmlWriter.WriteAttribute(L"yrange", oH.yrange);

                if (oH.polar != L"")
                    oXmlWriter.WriteAttribute(L"polar", oH.polar);

                if (oH.radiusrange != L"")
                    oXmlWriter.WriteAttribute(L"radiusrange", oH.radiusrange);

                if (oH.switchHandle != L"")
                    oXmlWriter.WriteAttribute(L"switch", oH.switchHandle);

                oXmlWriter.WriteString(L"/>");
			}

            oXmlWriter.EndNode(L"v:handles");
		}
	}

    oXmlWriter.EndNode(L"v:shape");
	return oXmlWriter.GetXmlString();
}

void CDrawingConverter::SendMainProps(const std::wstring& strMainProps, std::wstring**& pMainProps)
{
	if (((m_pBinaryWriter) && (m_pBinaryWriter->m_pMainDocument)) || !m_pBinaryWriter)
	{
		*pMainProps = new std::wstring();
		**pMainProps = strMainProps;
	}
}
void CDrawingConverter::CheckBorderShape(PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape)
{
	if (!oElem) return;

	PPTX::Logic::Shape* pShape		= dynamic_cast<PPTX::Logic::Shape*>	(oElem->GetElem().operator ->());
	PPTX::Logic::Pic*	pPicture	= dynamic_cast<PPTX::Logic::Pic*>	(oElem->GetElem().operator ->());

	PPTX::Logic::SpPr *pSpPr = NULL;

	if (pShape)		pSpPr = &pShape->spPr; 	
	if (pPicture)	pSpPr = &pPicture->spPr;

	if (!pSpPr) return;
	
	if ( (pSpPr->ln.IsInit()) && (pSpPr->ln->Fill.m_type != PPTX::Logic::UniFill::noFill) )
		return; //дублирование обрамлением линией

	nullable_string sColorBorder;
    XmlMacroReadAttributeBase(oNode, L"o:borderleftcolor", sColorBorder);
	
	XmlUtils::CXmlNode oNodeBorder = oNode.ReadNode(L"w10:borderleft");

	if (oNodeBorder.IsValid())
	{
		nullable_string sTypeBorder;
        XmlMacroReadAttributeBase(oNodeBorder, L"type", sTypeBorder);
		
		SimpleTypes::CBorderType borderType;
		if (sTypeBorder.IsInit())
		{
			borderType.FromString(sTypeBorder.get());
		}
		if (borderType.GetValue() != SimpleTypes::bordertypeNone)
		{
			pSpPr->ln.Init();
			
			nullable_int nWidthBorder;
			XmlMacroReadAttributeBase(oNodeBorder, L"width", nWidthBorder);

			if (borderType.GetValue() > 0 && 
				borderType.GetValue() < 6)
			{
				pSpPr->ln->prstDash.Init();
				pSpPr->ln->prstDash->val = new PPTX::Limit::PrstDashVal();
				switch(borderType.GetValue())
				{
					case SimpleTypes::bordertypeDash:			pSpPr->ln->prstDash->val->SetBYTECode(3); break;
					case SimpleTypes::bordertypeDashDotDot:		pSpPr->ln->prstDash->val->SetBYTECode(5); break;
					case SimpleTypes::bordertypeDashDotStroked:	pSpPr->ln->prstDash->val->SetBYTECode(1); break;
					case SimpleTypes::bordertypeDashedSmall:	pSpPr->ln->prstDash->val->SetBYTECode(0); break;
					case SimpleTypes::bordertypeDot:			pSpPr->ln->prstDash->val->SetBYTECode(2); break;
					case SimpleTypes::bordertypeDotDash:		pSpPr->ln->prstDash->val->SetBYTECode(1); break;
				}
			}
			if (nWidthBorder.IsInit())
			{
				pSpPr->ln->w = (int)(*nWidthBorder * g_emu_koef);//pt to emu
			}
			if (sColorBorder.IsInit())
			{
				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				
				if (std::wstring::npos != sColorBorder->find(L"#"))
				{
					pSolid->Color.Color->SetHexString(sColorBorder->substr(1));
				}
				else
				{
					//"red", L"black" , .. to color
				}

				pSpPr->ln->Fill.m_type	= PPTX::Logic::UniFill::solidFill;
				pSpPr->ln->Fill.Fill	= pSolid;

			}
		}
	}
}

void CDrawingConverter::CheckBrushShape(PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape)
{
	if (!oElem) return;

	PPTX::Logic::Shape* pShape		= dynamic_cast<PPTX::Logic::Shape*>	(oElem->GetElem().operator ->());
	PPTX::Logic::Pic*	pPicture	= dynamic_cast<PPTX::Logic::Pic*>	(oElem->GetElem().operator ->());

	PPTX::Logic::SpPr *pSpPr = NULL;

	if (pShape)		pSpPr = &pShape->spPr; 	
	if (pPicture)	pSpPr = &pPicture->spPr;

	if (!pSpPr) return;
	
	int R = 255;
    int G = 255;
    int B = 255;

	nullable_string sFillColor;
    XmlMacroReadAttributeBase(oNode, L"fillcolor", sFillColor);
	
	if (sFillColor.is_init() && !pPPTShape->IsWordArt())
	{
		ODRAW::CColor color;
		if (NS_DWC_Common::getColorFromString(*sFillColor, color))
		{
			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = L"a";
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

			pSpPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
			pSpPr->Fill.Fill = pSolid;
		}
	}
    else if (!pPPTShape->IsWordArt())
    {
        // default fillcolor in vml = white
        PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
        pSolid->m_namespace = L"a";
        pSolid->Color.Color = new PPTX::Logic::SrgbClr();
        pSolid->Color.Color->SetRGB(R, G, B);

		pSpPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
        pSpPr->Fill.Fill = pSolid;
    }

	nullable_string sFilled;
    XmlMacroReadAttributeBase(oNode, L"filled", sFilled);
	if (sFilled.is_init())
	{
        if (*sFilled == L"false" || *sFilled == L"f")
		{
			PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
            pNoFill->m_namespace = L"a";

			pSpPr->Fill.m_type = PPTX::Logic::UniFill::noFill;
			pSpPr->Fill.Fill = pNoFill;
		}
	}
	else if (!pPPTShape->m_bIsFilled)
	{
		PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
        pNoFill->m_namespace = L"a";

		pSpPr->Fill.m_type = PPTX::Logic::UniFill::noFill;
		pSpPr->Fill.Fill = pNoFill;
	}

	nullable_string sOpacity;
    XmlMacroReadAttributeBase(oNode, L"opacity", sOpacity);
	if (sOpacity.is_init())
	{
		BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);

		if (pSpPr->Fill.is<PPTX::Logic::SolidFill>())
		{
			PPTX::Logic::ColorModifier oMod;
            oMod.name = L"alpha";
			int nA = (int)(lAlpha * 100000.0 / 255.0);
			oMod.val = nA;
			pSpPr->Fill.as<PPTX::Logic::SolidFill>().Color.Color->Modifiers.push_back(oMod);
		}
	}

    XmlUtils::CXmlNode oNodeFill = oNode.ReadNode(L"v:fill");
	if (oNodeFill.IsValid() && !pPPTShape->IsWordArt())
	{
		nullable_string sType;
        XmlMacroReadAttributeBase(oNodeFill, L"type", sType);

		sOpacity.reset();
        XmlMacroReadAttributeBase(oNodeFill, L"opacity", sOpacity);

		nullable_string sColor;
        XmlMacroReadAttributeBase(oNodeFill, L"color", sColor);
		if (sColor.is_init())
		{
			ODRAW::CColor color;
			if (NS_DWC_Common::getColorFromString(*sColor, color))
			{
				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

				pSpPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
				pSpPr->Fill.Fill = pSolid;

				if (!sFillColor.is_init())
					sFillColor = sColor;
			}
		}
		if (!sColor.is_init()) sColor = sFillColor;

		nullable_string sRid;
        XmlMacroReadAttributeBase(oNodeFill, L"r:id", sRid);
		if (false == sRid.IsInit())
		{
			XmlMacroReadAttributeBase( oNodeFill, L"relationships:id", sRid );
		}
		if (sRid.is_init())
		{		
			PPTX::Logic::BlipFill* pBlipFill = NULL;
			if (pPicture)
			{
				pBlipFill = &pPicture->blipFill;
			}
			else
			{
				pBlipFill = new PPTX::Logic::BlipFill();
				
				pSpPr->Fill.m_type = PPTX::Logic::UniFill::blipFill;
				pSpPr->Fill.Fill = pBlipFill;
			}
            pBlipFill->m_namespace = L"a";
			pBlipFill->blip = new PPTX::Logic::Blip();
			pBlipFill->blip->embed = new OOX::RId(*sRid);

            if (sType.is_init() && ((*sType == L"tile") || (*sType == L"pattern")))
			{
				pBlipFill->tile = new PPTX::Logic::Tile();				
			}
			else
			{
				pBlipFill->stretch = new PPTX::Logic::Stretch();				
			}
		}		
		nullable_string sRotate;
        XmlMacroReadAttributeBase(oNodeFill, L"rotate", sRotate);

		nullable_string sMethod;
        XmlMacroReadAttributeBase(oNodeFill, L"method", sMethod);
		
		nullable_string sColor2;
        XmlMacroReadAttributeBase(oNodeFill, L"color2", sColor2);
		
		nullable_string sFocus;
        XmlMacroReadAttributeBase(oNodeFill, L"focus", sFocus);
		//
        if (sType.is_init() && (*sType == L"gradient" || *sType == L"gradientradial" || *sType == L"gradientRadial"))
		{
			PPTX::Logic::GradFill* pGradFill = new PPTX::Logic::GradFill();
            pGradFill->m_namespace = L"a";
		
			if (sColor.is_init())
			{
				ODRAW::CColor color;
				if (NS_DWC_Common::getColorFromString(*sColor, color))
				{
					PPTX::Logic::Gs Gs_;
					Gs_.color.Color = new PPTX::Logic::SrgbClr();
					Gs_.color.Color->SetRGB(color.R, color.G, color.B);

					Gs_.pos = 0;
					pGradFill->GsLst.push_back(Gs_);

					R = color.R;
					G = color.G;
					B = color.B;
				}
			}
			if (sColor2.is_init())
			{
				PPTX::Logic::Gs Gs_;
				Gs_.color.Color = new PPTX::Logic::SrgbClr();
                if (sColor2->find(L"fill") != -1)
				{
                    std::wstring sColorEffect = *sColor2;
                    if (sColorEffect.length() > 5)
                        sColorEffect = sColorEffect.substr(5);

                    int resR, resG, resB;
                    GetColorWithEffect(sColorEffect, R, G, B, resR, resG, resB);

                    Gs_.color.Color->SetRGB(resR, resG, resB);
				}
				else
				{
					ODRAW::CColor color;
					if (NS_DWC_Common::getColorFromString(*sColor2, color))
					{
						Gs_.color.Color->SetRGB(color.R, color.G, color.B);
					}
				}

				Gs_.pos = 100 * 1000;
				pGradFill->GsLst.push_back( Gs_ );
			}
			if (pGradFill->GsLst.size() == 1)	//Sindicatum.docx
			{
				PPTX::Logic::Gs Gs_; 
				Gs_.pos = 0;
				Gs_.color.Color = new PPTX::Logic::SrgbClr(); Gs_.color.Color->SetRGB(0xff, 0xff, 0xff);
				
				if (pGradFill->GsLst[0].pos == 0)
					Gs_.pos = 100 * 1000;
				
				pGradFill->GsLst.push_back( Gs_ );
			}
			//todooo method
			if (sRotate.is_init())
			{
				pGradFill->lin = new PPTX::Logic::Lin();
				pGradFill->lin->scaled = 1;

				if (*sRotate == L"l") pGradFill->lin->ang = 0   * 60000;
				if (*sRotate == L"t") pGradFill->lin->ang = 90  * 60000;
				if (*sRotate == L"b") pGradFill->lin->ang = 270 * 60000;
				if (*sRotate == L"r") pGradFill->lin->ang = 180 * 60000;
			}

			pSpPr->Fill.m_type = PPTX::Logic::UniFill::gradFill;
			pSpPr->Fill.Fill = pGradFill;
		}	

		if (sOpacity.is_init())
		{
			BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);

			if (pSpPr->Fill.is<PPTX::Logic::SolidFill>())
			{
				PPTX::Logic::ColorModifier oMod;
                oMod.name = L"alpha";
				int nA = (int)(lAlpha * 100000.0 / 255.0);
				oMod.val = nA;
				pSpPr->Fill.as<PPTX::Logic::SolidFill>().Color.Color->Modifiers.push_back(oMod);
			}
			else if (pSpPr->Fill.is<PPTX::Logic::BlipFill>())
			{
				PPTX::Logic::AlphaModFix* pAlphaMod = new PPTX::Logic::AlphaModFix();
				int nA = (int)(lAlpha * 100000.0 / 255.0);
				pAlphaMod->amt = nA;

				PPTX::Logic::UniEffect oEff;
				oEff.InitPointer(pAlphaMod);

				pSpPr->Fill.as<PPTX::Logic::BlipFill>().blip->Effects.push_back(oEff);
			}
		}
	}
	if (true)
	{
        XmlUtils::CXmlNode oNodeFillID = oNode.ReadNode(L"v:imagedata");

		if (oNodeFillID.IsValid())
		{
			nullable_string sRid;
            XmlMacroReadAttributeBase(oNodeFillID, L"r:id", sRid);
			
			nullable_string sRelid;
            XmlMacroReadAttributeBase(oNodeFillID, L"o:relid", sRelid);

			nullable_string sPictId;
            XmlMacroReadAttributeBase(oNodeFillID, L"r:pict", sPictId);

			if (sRid.is_init() || sRelid.is_init() || sPictId.is_init() || oElem->m_binaryData.IsInit())
			{			
				nullable_string sType;
                XmlMacroReadAttributeBase(oNodeFillID, L"type", sType);

				PPTX::Logic::BlipFill* pBlipFill = NULL;
				
				if (pPicture)
				{
					pBlipFill = &pPicture->blipFill;
				}
				else
				{
					pBlipFill = new PPTX::Logic::BlipFill();
					
					pSpPr->Fill.m_type = PPTX::Logic::UniFill::blipFill;
					pSpPr->Fill.Fill = pBlipFill;
				}
                pBlipFill->m_namespace = L"a";

				pBlipFill->blip = new PPTX::Logic::Blip();
				
				if (oElem->m_binaryData.IsInit())
				{
					pBlipFill->blip->dataFilepathImage = L"data:base64," + *oElem->m_binaryData;
				}
				else
				{					
					std::wstring sId = sRid.IsInit() ? *sRid : (sRelid.IsInit() ? *sRelid : (sPictId.IsInit() ? *sPictId : L""));
					pBlipFill->blip->embed = new OOX::RId(sId);
				}

                if (sType.is_init() && *sType == L"tile")
				{
					pBlipFill->tile = new PPTX::Logic::Tile();				
				}else
				{
					//stretch ??? bug 28238
					pBlipFill->stretch.Init();
					pBlipFill->stretch->fillRect.Init();
				}

                std::wstring strCropT = oNodeFillID.GetAttribute(L"croptop");
                std::wstring strCropL = oNodeFillID.GetAttribute(L"cropleft");
                std::wstring strCropR = oNodeFillID.GetAttribute(L"cropright");
                std::wstring strCropB = oNodeFillID.GetAttribute(L"cropbottom");

				NS_DWC_Common::CorrentCropString(strCropL);
				NS_DWC_Common::CorrentCropString(strCropT);
				NS_DWC_Common::CorrentCropString(strCropR);
				NS_DWC_Common::CorrentCropString(strCropB);

                if (L"" != strCropL || L"" != strCropT || L"" != strCropR || L"" != strCropB)
				{
					pBlipFill->srcRect = new PPTX::Logic::Rect();

                    std::wstring str0 = L"0";
                    if (L"" != strCropL)
						pBlipFill->srcRect->l = strCropL;
					else
						pBlipFill->srcRect->l = str0;

                    if (L"" != strCropT)
						pBlipFill->srcRect->t = strCropT;
					else
						pBlipFill->srcRect->t = str0;

                    if (L"" != strCropR)
						pBlipFill->srcRect->r = strCropR;
					else
						pBlipFill->srcRect->r = str0;

                    if (L"" != strCropB)
						pBlipFill->srcRect->b = strCropB;
					else
						pBlipFill->srcRect->b = str0;
				}
			}
		}
	}
	if (pPicture)
	{
		pSpPr->Fill.m_type = PPTX::Logic::UniFill::notInit;

		if (false == pPicture->blipFill.blip.is_init())
		{//MSF_Lec3-4.docx
			oElem->InitElem(NULL);
		}
	}
	else
	{
		// default params for fill shape
		if (!pSpPr->Fill.Fill.is_init())
		{
			if (pPPTShape->IsWordArt())
			{
				PPTX::Logic::NoFill* pNoFill = new PPTX::Logic::NoFill();
				pNoFill->m_namespace = L"a";

				pSpPr->Fill.m_type = PPTX::Logic::UniFill::noFill;
				pSpPr->Fill.Fill = pNoFill;
			}
			else
			{
				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				pSolid->Color.Color->SetRGB(0xFF, 0xFF, 0xFF);

				pSpPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
				pSpPr->Fill.Fill = pSolid;

				if (sOpacity.is_init())
				{
					BYTE lAlpha = NS_DWC_Common::getOpacityFromString(*sOpacity);
					PPTX::Logic::ColorModifier oMod;
					oMod.name = L"alpha";
					int nA = (int)(lAlpha * 100000.0 / 255.0);
					oMod.val = nA;
					pSolid->Color.Color->Modifiers.push_back(oMod);
				}
			}
		}
	}
}
void CDrawingConverter::CheckPenShape(PPTX::Logic::SpTreeElem* oElem, XmlUtils::CXmlNode& oNode, CPPTShape* pPPTShape)
{
	if (!oElem)return;

	PPTX::Logic::Shape* pShape		= dynamic_cast<PPTX::Logic::Shape*>	(oElem->GetElem().operator ->());
	PPTX::Logic::Pic*	pPicture	= dynamic_cast<PPTX::Logic::Pic*>	(oElem->GetElem().operator ->());

	PPTX::Logic::SpPr *pSpPr = NULL;

	if (pShape)		pSpPr = &pShape->spPr; 	
	if (pPicture)	pSpPr = &pPicture->spPr;

	if (!pSpPr) return;

	if (!pPPTShape->m_bIsStroked)
	{
		if (!pSpPr->ln.is_init())
			pSpPr->ln = new PPTX::Logic::Ln();

		pSpPr->ln->Fill.m_type = PPTX::Logic::UniFill::noFill;
		pSpPr->ln->Fill.Fill = new PPTX::Logic::NoFill();
	}

	if (pPPTShape->IsWordArt())
		return;

	nullable_string sStrokeColor;
    XmlMacroReadAttributeBase(oNode, L"strokecolor", sStrokeColor);
	if (sStrokeColor.is_init())
	{
		ODRAW::CColor color;
		if (NS_DWC_Common::getColorFromString(*sStrokeColor, color))
		{
			pPPTShape->m_bIsStroked = true;

			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
			pSolid->m_namespace = L"a";
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

			pSpPr->ln->Fill.m_type = PPTX::Logic::UniFill::solidFill;
			pSpPr->ln->Fill.Fill = pSolid;
		}
	}

	nullable_string sStrokeWeight;
    XmlMacroReadAttributeBase(oNode, L"strokeweight", sStrokeWeight);
	if (sStrokeWeight.is_init())
	{
		pPPTShape->m_bIsStroked = true;

		if (!pSpPr->ln.is_init())
			pSpPr->ln = new PPTX::Logic::Ln();

        if (sStrokeWeight->length() > 0 && sStrokeWeight->at(0) == wchar_t('.'))
		{
            sStrokeWeight = (L"0" + *sStrokeWeight);
		}

		SimpleTypes::CPoint oPoint;
		int size = (int)(g_emu_koef * oPoint.FromString(*sStrokeWeight));		

		pSpPr->ln->w = size;
		pPPTShape->m_bIsStroked = true;
	}
    XmlUtils::CXmlNode oNodeStroke = oNode.ReadNode(L"v:stroke");
	if (oNodeStroke.IsValid())
	{
		sStrokeColor.reset();
        XmlMacroReadAttributeBase(oNodeStroke, L"strokecolor", sStrokeColor);
		if (sStrokeColor.is_init())
		{
			pPPTShape->m_bIsStroked = true;

			ODRAW::CColor color;
			if (NS_DWC_Common::getColorFromString(*sStrokeColor, color))
			{
				if (!pSpPr->ln.is_init())
					pSpPr->ln = new PPTX::Logic::Ln();

				PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
				pSolid->m_namespace = L"a";
				pSolid->Color.Color = new PPTX::Logic::SrgbClr();
				pSolid->Color.Color->SetRGB(color.R, color.G, color.B);

				pSpPr->ln->Fill.m_type = PPTX::Logic::UniFill::solidFill;
				pSpPr->ln->Fill.Fill = pSolid;
			}
		}

		nullable_string sStrokeDashStyle;
        XmlMacroReadAttributeBase(oNodeStroke, L"dashstyle", sStrokeDashStyle);
		if (sStrokeDashStyle.is_init())
		{
			pPPTShape->m_bIsStroked = true;

			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			pSpPr->ln->prstDash = new PPTX::Logic::PrstDash();

            if		(*sStrokeDashStyle == L"solid")			pSpPr->ln->prstDash->val = L"solid";
            else if (*sStrokeDashStyle == L"shortdash")		pSpPr->ln->prstDash->val = L"sysDash";
            else if (*sStrokeDashStyle == L"shortdot")		pSpPr->ln->prstDash->val = L"sysDot";
            else if (*sStrokeDashStyle == L"shortdashdot")	pSpPr->ln->prstDash->val = L"sysDashDot";
            else if (*sStrokeDashStyle == L"shortdashdotdot")pSpPr->ln->prstDash->val = L"sysDashDotDot";
            else if (*sStrokeDashStyle == L"dot")			pSpPr->ln->prstDash->val = L"dot";
            else if (*sStrokeDashStyle == L"dash")			pSpPr->ln->prstDash->val = L"dash";
            else if (*sStrokeDashStyle == L"longdash")		pSpPr->ln->prstDash->val = L"lgDash";
            else if (*sStrokeDashStyle == L"dashdot")		pSpPr->ln->prstDash->val = L"dashDot";
            else if (*sStrokeDashStyle == L"longdashdot")	pSpPr->ln->prstDash->val = L"lgDashDot";
            else if (*sStrokeDashStyle == L"longdashdotdot")pSpPr->ln->prstDash->val = L"lgDashDotDot";
			else											pSpPr->ln->prstDash->val = L"solid";
		}

		nullable_string sEndArraw;
        XmlMacroReadAttributeBase(oNodeStroke, L"endarrow", sEndArraw);
		if (sEndArraw.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			pSpPr->ln->tailEnd = new PPTX::Logic::LineEnd();

            if		(*sEndArraw == L"none")		pSpPr->ln->tailEnd->type = L"none";
            else if (*sEndArraw == L"block")	pSpPr->ln->tailEnd->type = L"triangle";
            else if (*sEndArraw == L"classic")	pSpPr->ln->tailEnd->type = L"stealth";
            else if (*sEndArraw == L"diamond")	pSpPr->ln->tailEnd->type = L"diamond";
            else if (*sEndArraw == L"oval")		pSpPr->ln->tailEnd->type = L"oval";
            else if (*sEndArraw == L"open")		pSpPr->ln->tailEnd->type = L"arrow";
			else								pSpPr->ln->tailEnd->type = L"none";
		}

		nullable_string sEndArrawLen;
        XmlMacroReadAttributeBase(oNodeStroke, L"endarrowlength", sEndArrawLen);
		if (sEndArrawLen.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			if (!pSpPr->ln->tailEnd.is_init())
				pSpPr->ln->tailEnd = new PPTX::Logic::LineEnd();

            if		(*sEndArrawLen == L"short")		pSpPr->ln->tailEnd->len = L"sm";
            else if (*sEndArrawLen == L"medium")	pSpPr->ln->tailEnd->len = L"med";
            else if (*sEndArrawLen == L"long")		pSpPr->ln->tailEnd->len = L"lg";
			else									pSpPr->ln->tailEnd->len = L"med";
		}

		nullable_string sEndArrawWidth;
        XmlMacroReadAttributeBase(oNodeStroke, L"endarrowwidth", sEndArrawWidth);
		if (sEndArrawWidth.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			if (!pSpPr->ln->tailEnd.is_init())
				pSpPr->ln->tailEnd = new PPTX::Logic::LineEnd();

				if	(*sEndArrawWidth == L"narrow")	pSpPr->ln->tailEnd->w = L"sm";
            else if (*sEndArrawWidth == L"medium")	pSpPr->ln->tailEnd->w = L"med";
            else if (*sEndArrawWidth == L"wide")	pSpPr->ln->tailEnd->w = L"lg";
			else									pSpPr->ln->tailEnd->w = L"med";
		}

		nullable_string sStartArraw;
        XmlMacroReadAttributeBase(oNodeStroke, L"startarrow", sStartArraw);
		if (sStartArraw.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			pSpPr->ln->headEnd = new PPTX::Logic::LineEnd();

            if		(*sStartArraw == L"none")	pSpPr->ln->headEnd->type = L"none";
            else if (*sStartArraw == L"block")	pSpPr->ln->headEnd->type = L"triangle";
            else if (*sStartArraw == L"classic")pSpPr->ln->headEnd->type = L"stealth";
            else if (*sStartArraw == L"diamond")pSpPr->ln->headEnd->type = L"diamond";
            else if (*sStartArraw == L"oval")	pSpPr->ln->headEnd->type = L"oval";
            else if (*sStartArraw == L"open")	pSpPr->ln->headEnd->type = L"arrow";
			else				                pSpPr->ln->headEnd->type = L"none";
		}

		nullable_string sStartArrawLen;
        XmlMacroReadAttributeBase(oNodeStroke, L"startarrowlength", sStartArrawLen);
		if (sStartArrawLen.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			if (!pSpPr->ln->headEnd.is_init())
				pSpPr->ln->headEnd = new PPTX::Logic::LineEnd();

            if		(*sStartArrawLen == L"short")	pSpPr->ln->headEnd->len = L"sm";
            else if (*sStartArrawLen == L"medium")	pSpPr->ln->headEnd->len = L"med";
            else if (*sStartArrawLen == L"long")	pSpPr->ln->headEnd->len = L"lg";
			else									pSpPr->ln->headEnd->len = L"med";
		}

		nullable_string sStartArrawWidth;
        XmlMacroReadAttributeBase(oNodeStroke, L"startarrowwidth", sStartArrawWidth);
		if (sStartArrawWidth.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			if (!pSpPr->ln->headEnd.is_init())
				pSpPr->ln->headEnd = new PPTX::Logic::LineEnd();

            if		(*sStartArrawWidth == L"narrow")	pSpPr->ln->headEnd->w = L"sm";
            else if (*sStartArrawWidth == L"medium")	pSpPr->ln->headEnd->w = L"med";
            else if (*sStartArrawWidth == L"wide")		pSpPr->ln->headEnd->w = L"lg";
			else						                pSpPr->ln->headEnd->w = L"med";
		}

		nullable_string sEndCap;
        XmlMacroReadAttributeBase(oNodeStroke, L"endcap", sEndCap);
		if (sEndCap.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

            if		(*sEndCap == L"flat")	pSpPr->ln->cap = L"flat";
            else if (*sEndCap == L"round")	pSpPr->ln->cap = L"rnd";
            else if (*sEndCap == L"square")	pSpPr->ln->cap = L"sq";
		}

		nullable_string sLineJoin;
        XmlMacroReadAttributeBase(oNodeStroke, L"joinstyle", sLineJoin);
		if (sLineJoin.is_init())
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

            if		(*sLineJoin == L"bevel")	pSpPr->ln->Join.type = PPTX::Logic::JoinBevel;
            else if (*sLineJoin == L"miter")	pSpPr->ln->Join.type = PPTX::Logic::JoinMiter;
            else if (*sLineJoin == L"round")	pSpPr->ln->Join.type = PPTX::Logic::JoinRound;
		}		
	}
	nullable_string sStroked;
	XmlMacroReadAttributeBase(oNode, L"stroked", sStroked);

	if (oNodeStroke.IsValid())
	{
		nullable_string sStrokeOn;
		XmlMacroReadAttributeBase(oNodeStroke, L"on", sStrokeOn);
		if (sStrokeOn.is_init())
		{
			sStroked.reset();
			sStroked = sStrokeOn;
		}
	}
	if (sStroked.is_init())
	{
		if (*sStroked == L"false" || *sStroked == L"f")
		{
			if (!pSpPr->ln.is_init())
				pSpPr->ln = new PPTX::Logic::Ln();

			pSpPr->ln->Fill.m_type = PPTX::Logic::UniFill::noFill;
			pSpPr->ln->Fill.Fill = new PPTX::Logic::NoFill();

			pPPTShape->m_bIsStroked = false;
		}
	}
	// default params
	if (pShape) // not Picture
	{
		if (!pSpPr->ln.is_init())
			pSpPr->ln = new PPTX::Logic::Ln();

		if (!pSpPr->ln->Fill.Fill.is_init())
		{
			PPTX::Logic::SolidFill* pSolid = new PPTX::Logic::SolidFill();
            pSolid->m_namespace = L"a";
			pSolid->Color.Color = new PPTX::Logic::SrgbClr();
			pSolid->Color.Color->SetRGB(0, 0, 0);

			pSpPr->ln->Fill.m_type = PPTX::Logic::UniFill::solidFill;
			pSpPr->ln->Fill.Fill = pSolid;
		}
	}
}

HRESULT CDrawingConverter::LoadClrMap(const std::wstring& bsXml)
{
	smart_ptr<PPTX::Logic::ClrMap> pClrMap = new PPTX::Logic::ClrMap();
	
    std::wstring strXml = L"<main xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\">" + bsXml + L"</main>";
	XmlUtils::CXmlNode oNode;
	oNode.FromXmlString(strXml);

	if (oNode.IsValid())
	{
        XmlUtils::CXmlNode nodeMap = oNode.ReadNode(L"w:clrSchemeMapping");
        pClrMap->fromXML(nodeMap);
	}
	
	*m_pClrMap = pClrMap;
	return S_OK;
}

HRESULT CDrawingConverter::SaveObject(LONG lStart, LONG lLength, const std::wstring& bsMainProps, std::wstring & sXml)
{
	bool bIsInline = false;
	std::wstring strMainProps = bsMainProps;
	std::wstring strMainPropsTail;
    int nIndexF = (int)strMainProps.find(L"</wp:inline>");
	if (-1 != nIndexF)
	{
		bIsInline = true;
		strMainProps = strMainProps.substr(0, nIndexF);
	}
	else
	{
        nIndexF = (int)strMainProps.find(L"</wp:anchor>");
		strMainProps = strMainProps.substr(0, nIndexF);
	}

	if (-1 == nIndexF)
		return S_FALSE;

    int nIndexTail = (int)strMainProps.find(L"<wp14:sizeRel");
	if(-1 != nIndexTail)
	{
        strMainPropsTail    = strMainProps.substr( nIndexTail );
        strMainProps        = strMainProps.substr(0, nIndexTail);
	}

	/*
    std::wstring strId = L"");
    strId.Format(L"<wp:docPr id=\"%d\" name=\"\"/>"), m_pReader->GenerateNextId());
	strMainProps += strId;
	*/

    //strMainProps += L"<wp:cNvGraphicFramePr/>");

	m_pReader->Seek(lStart);
	
	++m_nCurrentIndexObject;

	BYTE typeRec1 = m_pReader->GetUChar(); // must be 0;
	LONG szRec1 = m_pReader->GetRecordSize();
	LONG _e = m_pReader->GetPos() + szRec1 + 4;

	if (typeRec1 == 0 && szRec1 > 0)
	{
		BYTE typeRec2 = m_pReader->GetUChar(); // must be 1;
		LONG szRec2 = m_pReader->GetLong();

		if (typeRec2 == 1 && szRec2 > 0)
		{
			PPTX::Logic::SpTreeElem oElem;

			if (m_pReader->m_nDocumentType == 0)
				m_pReader->m_nDocumentType = XMLWRITER_DOC_TYPE_DOCX;

			oElem.fromPPTY(m_pReader);
			bool bOle = false;
			if (oElem.is<PPTX::Logic::Pic>())
			{
				PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
				if(oPic.oleObject.IsInit())
				{
					bOle = oPic.oleObject->isValid();
					if (oPic.oleObject->m_oDxaOrig.IsInit() == false)
					{
						oPic.oleObject->m_oDxaOrig = 0;
					}
					if (oPic.oleObject->m_oDyaOrig.IsInit() == false)
					{
						oPic.oleObject->m_oDyaOrig = 0;
					}
				}
			}
			bool bSignatureLine = false;
			if (oElem.is<PPTX::Logic::Shape>())
			{
				PPTX::Logic::Shape& oShape = oElem.as<PPTX::Logic::Shape>();
				if(oShape.signatureLine.IsInit())
				{
					bSignatureLine = true;
				}
			}
			NSBinPptxRW::CXmlWriter oXmlWriter(m_pReader->m_nDocumentType);

			oXmlWriter.m_lObjectIdVML = m_pXmlWriter->m_lObjectIdVML;
			oXmlWriter.m_lObjectIdOle = m_pXmlWriter->m_lObjectIdOle;
			oXmlWriter.m_bIsUseOffice2007 = m_bIsUseConvertion2007;

			oXmlWriter.m_bIsTop = (1 == m_nCurrentIndexObject) ? true : false;

			if (NULL == m_pOOXToVMLRenderer)
				m_pOOXToVMLRenderer = new COOXToVMLGeometry();
			oXmlWriter.m_pOOXToVMLRenderer = m_pOOXToVMLRenderer;

			if(bOle)
			{
				ConvertPicVML(oElem, bsMainProps, oXmlWriter);
			}
			else if(bSignatureLine)
			{
				oXmlWriter.WriteString(L"<w:pict>");
				ConvertShapeVML(oElem, bsMainProps, oXmlWriter, true);
				oXmlWriter.WriteString(L"</w:pict>");
			}
			else
			{
			
				bool bIsNeedConvert2007 = false;

				if (m_bIsUseConvertion2007)
				{
					if (oElem.is<PPTX::Logic::SpTree>()	|| 
						oElem.is<PPTX::Logic::Shape>()	||
						oElem.is<PPTX::Logic::Pic>())
					{
						oXmlWriter.WriteString(L"<mc:AlternateContent><mc:Choice Requires=\"wpg\">");
						bIsNeedConvert2007 = true;
					}
				}

				oXmlWriter.WriteString(L"<w:drawing>");
				oXmlWriter.WriteString(strMainProps);

				bool bAddGraphicData = false;
				if (oElem.is<PPTX::Logic::SpTree>())
				{
					bAddGraphicData = true;
					oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
		<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\">");
				}
				else if (oElem.is<PPTX::Logic::Pic>())
				{
					bAddGraphicData = true;
					oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
		<a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">");
				}
				else if (oElem.is<PPTX::Logic::Shape>())
				{
					bAddGraphicData = true;
					oXmlWriter.WriteString(L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">\
		<a:graphicData uri=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\">");
				}
				oElem.toXmlWriter(&oXmlWriter); 
				
				if (bAddGraphicData)
					oXmlWriter.WriteString(L"</a:graphicData></a:graphic>");

				oXmlWriter.WriteString(strMainPropsTail);
				oXmlWriter.WriteString(bIsInline ? L"</wp:inline>" : L"</wp:anchor>");
				oXmlWriter.WriteString(L"</w:drawing>");

				if (bIsNeedConvert2007)
				{
					oXmlWriter.WriteString(L"</mc:Choice><mc:Fallback>");
					oXmlWriter.WriteString(L"<w:pict>");

					if (oElem.is<PPTX::Logic::SpTree>())
					{
						ConvertGroupVML(oElem, bsMainProps, oXmlWriter);
					}
					else if (oElem.is<PPTX::Logic::Shape>())
					{
						ConvertShapeVML(oElem, bsMainProps, oXmlWriter);
					}
					else if (oElem.is<PPTX::Logic::Pic>())
					{
						ConvertPicVML(oElem, bsMainProps, oXmlWriter);
					}
					oXmlWriter.WriteString(L"</w:pict>");
					oXmlWriter.WriteString(L"</mc:Fallback></mc:AlternateContent>");
				}
			}
			--m_nCurrentIndexObject;

			m_pXmlWriter->m_lObjectIdVML = oXmlWriter.m_lObjectIdVML;
			m_pXmlWriter->m_lObjectIdOle = oXmlWriter.m_lObjectIdOle;

			sXml = oXmlWriter.GetXmlString();
		}
	}

	m_pReader->Seek(_e);
	return S_OK;
}
void CDrawingConverter::SaveObjectExWriterInit(NSBinPptxRW::CXmlWriter& oXmlWriter, int nDocType)
{
	oXmlWriter.m_lObjectIdVML       = m_pXmlWriter->m_lObjectIdVML;
	oXmlWriter.m_lObjectIdOle       = m_pXmlWriter->m_lObjectIdOle;
	oXmlWriter.m_lDocType           = (BYTE)nDocType;
	oXmlWriter.m_bIsUseOffice2007   = false;

	oXmlWriter.m_bIsTop = (1 == m_nCurrentIndexObject) ;

	if (NULL == m_pOOXToVMLRenderer)
		m_pOOXToVMLRenderer = new COOXToVMLGeometry();
	oXmlWriter.m_pOOXToVMLRenderer = m_pOOXToVMLRenderer;
}
void CDrawingConverter::SaveObjectExWriterRelease(NSBinPptxRW::CXmlWriter& oXmlWriter)
{
	m_pXmlWriter->m_lObjectIdVML = oXmlWriter.m_lObjectIdVML;
	m_pXmlWriter->m_lObjectIdOle = oXmlWriter.m_lObjectIdOle;
}
HRESULT CDrawingConverter::SaveObjectEx(LONG lStart, LONG lLength, const std::wstring& bsMainProps, int nDocType, std::wstring & sXml)
{
	m_pImageManager->m_nDocumentType = nDocType;
	
	if (XMLWRITER_DOC_TYPE_DOCX == nDocType || 
		XMLWRITER_DOC_TYPE_DOCX_GLOSSARY == nDocType)	//docx
	{
		m_pReader->m_nDocumentType = nDocType;
		return SaveObject(lStart, lLength, bsMainProps, sXml);
	}
	else
	{
		PPTX::Logic::SpTreeElem oElem;

		m_pReader->Seek(lStart);

		m_pReader->m_nDocumentType = nDocType;	
		++m_nCurrentIndexObject;
		
		BYTE typeRec1   = m_pReader->GetUChar();    // must be 0;
		LONG _e         = m_pReader->GetPos()   + m_pReader->GetLong() + 4;
		
		try
		{
			m_pReader->Skip(5); // type record (must be 1) + 4 byte - len record

			oElem.fromPPTY(m_pReader);			
		}
		catch(...)
		{
			//todooo
		}

		bool bOle = false;
		if (oElem.is<PPTX::Logic::Pic>())
		{
			PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
			if(oPic.oleObject.IsInit())
			{
				bOle = oPic.oleObject->isValid();
			}
		}
		
		m_pReader->m_nDocumentType = XMLWRITER_DOC_TYPE_PPTX;

		NSBinPptxRW::CXmlWriter oXmlWriter;
		SaveObjectExWriterInit(oXmlWriter, nDocType);

		if(bOle)
		{
			ConvertPicVML(oElem, bsMainProps, oXmlWriter);
		}
		else
		{
			oElem.toXmlWriter(&oXmlWriter);
		}

		--m_nCurrentIndexObject;

		SaveObjectExWriterRelease(oXmlWriter);
		sXml = oXmlWriter.GetXmlString();
	
		m_pReader->Seek(_e);
	}
	return S_OK;
}
std::wstring CDrawingConverter::SaveObjectBackground(LONG lStart, LONG lLength)
{
	if (lLength < 1) return L"";

	m_pReader->Seek(lStart);

	++m_nCurrentIndexObject;
	BYTE typeRec1 = m_pReader->GetUChar(); // must be 0;
	LONG _e = m_pReader->GetPos() + m_pReader->GetRecordSize() + 4;
	
	PPTX::Logic::SpTreeElem oElem;
	try
	{
		m_pReader->Skip(5); // type record (must be 1) + 4 byte - len record

		oElem.fromPPTY(m_pReader);
	}
	catch(...)
	{
		//todooo
	}

	NSBinPptxRW::CXmlWriter oXmlWriter;
	SaveObjectExWriterInit(oXmlWriter, m_pReader->m_nDocumentType);

	m_pReader->m_nDocumentType = XMLWRITER_DOC_TYPE_PPTX;

	if (oElem.is<PPTX::Logic::Shape>())
	{
		oXmlWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе

		PPTX::Logic::Shape& oShape = oElem.as<PPTX::Logic::Shape>();
		oShape.toXmlWriterVMLBackground(&oXmlWriter, *m_pTheme, *m_pClrMap);
	}
	--m_nCurrentIndexObject;

	SaveObjectExWriterRelease(oXmlWriter);

	m_pReader->Seek(_e);
	return oXmlWriter.GetXmlString();
}

void CDrawingConverter::ConvertPicVML(PPTX::Logic::SpTreeElem& oElem, const std::wstring& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter)
{
    ConvertMainPropsToVML(bsMainProps, oWriter, oElem);

	oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе
	PPTX::Logic::Pic& oPic = oElem.as<PPTX::Logic::Pic>();
	oPic.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap);
}

void CDrawingConverter::ConvertShapeVML(PPTX::Logic::SpTreeElem& oElem, const std::wstring& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter, bool bSignature)
{
    ConvertMainPropsToVML(bsMainProps, oWriter, oElem);

	if (oElem.is<PPTX::Logic::Shape>())
	{
		oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе
		
		PPTX::Logic::Shape& oShape = oElem.as<PPTX::Logic::Shape>();
		oShape.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap, false, bSignature);
	}
}

void CDrawingConverter::ConvertGroupVML(PPTX::Logic::SpTreeElem& oElem, const std::wstring& bsMainProps, NSBinPptxRW::CXmlWriter& oWriter)
{
    ConvertMainPropsToVML(bsMainProps, oWriter, oElem);

	if (oElem.is<PPTX::Logic::SpTree>())
	{
		oWriter.m_bIsTop = true; // не забыть скинуть в самом шейпе (вместе с остальными параметрами)
		
		PPTX::Logic::SpTree& oGroup = oElem.as<PPTX::Logic::SpTree>();
		oGroup.toXmlWriterVML(&oWriter, *m_pTheme, *m_pClrMap);
	}
}
void CDrawingConverter::ConvertTextVML(XmlUtils::CXmlNode &nodeTextBox, PPTX::Logic::Shape* pShape)
{
	if (pShape->txBody.IsInit() == false)
		pShape->txBody = new PPTX::Logic::TxBody();
	
	if (pShape->txBody.IsInit() == false) return;

    XmlUtils::CXmlNodes nodes = nodeTextBox.GetNodes(L"*");

	for (int i = 0 ; i < nodes.GetCount(); i++)
	{
		XmlUtils::CXmlNode node;
		if (nodes.GetAt(i, node))
		{
			std::wstring name = node.GetName();
            if (name == L"div")
			{
				PPTX::Logic::Paragraph p;

                std::wstring strStyle = node.GetAttribute(L"style");
				PPTX::CCSS oCSSParser;
				oCSSParser.LoadFromString2(strStyle);

				//todooo oCSSParser->pPr

                XmlUtils::CXmlNodes nodesDiv = node.GetNodes(L"*");
				for (int j = 0 ; j < nodesDiv.GetCount(); j++)
				{
					XmlUtils::CXmlNode node1;
					if (nodesDiv.GetAt(j, node1))
					{
						name = node1.GetName();
                        if (name == L"font")
						{
							PPTX::Logic::Run  *run = new PPTX::Logic::Run();
							
							run->SetText(node1.GetText());
							
							std::vector<std::wstring > attNames, attValues;
                            node1.GetAllAttributes(attNames,attValues);

							if (attNames.size() > 0)
							{
								run->rPr = new PPTX::Logic::RunProperties();							
									
								for (size_t r = 0; r < attNames.size(); r++)
								{
                                    if (attNames[r] == L"color" && attValues[r].length() == 7)
									{
                                        XmlUtils::replace_all(attValues[r], L"#", L"");

                                        PPTX::Logic::SolidFill	*fill	= new PPTX::Logic::SolidFill();
										PPTX::Logic::SrgbClr	*color	= new PPTX::Logic::SrgbClr();
										
										color->SetHexString(attValues[r]);
										fill->Color.Color = color;
										
										run->rPr->Fill.Fill = fill;
										run->rPr->Fill.m_type = PPTX::Logic::UniFill::solidFill;
									}
                                    else if (attNames[r] == L"size")
									{
                                        run->rPr->sz = XmlUtils::GetInteger(attValues[r]) * 5;
									}
                                    else if (attNames[r] == L"face")
									{	
										run->rPr->latin = new PPTX::Logic::TextFont();
										run->rPr->latin->typeface = attValues[r];
									}
								}
							}
							PPTX::Logic::RunElem elm;
							p.RunElems.push_back(elm);
							
							p.RunElems.back().InitRun(run);
						}
					}
				}
				pShape->txBody->Paragrs.push_back(p);
			}
		}
	}
}
void CDrawingConverter::ConvertMainPropsToVML(const std::wstring& bsMainProps, NSBinPptxRW::CXmlWriter& outWriter, PPTX::Logic::SpTreeElem& oElem)
{
    if (bsMainProps.empty())
        return;

	XmlUtils::CXmlNode oNode;
    if (!oNode.FromXmlString(bsMainProps))
		return;

	NSBinPptxRW::CXmlWriter oWriter;

	nullable_int margT; XmlMacroReadAttributeBase(oNode, L"distT", margT);
	nullable_int margB; XmlMacroReadAttributeBase(oNode, L"distB", margB);
	nullable_int margL; XmlMacroReadAttributeBase(oNode, L"distL", margL);
	nullable_int margR; XmlMacroReadAttributeBase(oNode, L"distR", margR);

	double dKoef = 72.0 / (36000 * 25.4);

	if (margL.is_init())
		oWriter.WriteAttributeCSS_double1_pt(L"mso-wrap-distance-left", dKoef * (*margL));
	if (margT.is_init())
		oWriter.WriteAttributeCSS_double1_pt(L"mso-wrap-distance-top", dKoef * (*margT));
	if (margR.is_init())
		oWriter.WriteAttributeCSS_double1_pt(L"mso-wrap-distance-right", dKoef * (*margR));
	if (margB.is_init())
		oWriter.WriteAttributeCSS_double1_pt(L"mso-wrap-distance-bottom", dKoef * (*margB));

	outWriter.m_strStyleWrap = oWriter.GetXmlString();
	oWriter.ClearNoAttack();

	if (L"wp:inline" == oNode.GetName())
	{
		XmlUtils::CXmlNode oNodeS;
        if (oNode.GetNode(L"wp:extent", oNodeS))
		{
            int _width = oNodeS.ReadAttributeInt(L"cx");
            int _height = oNodeS.ReadAttributeInt(L"cy");

            oWriter.WriteAttributeCSS_double1_pt(L"width", dKoef * _width);
            oWriter.WriteAttributeCSS_double1_pt(L"height", dKoef * _height);
		}
	}
	else
	{
		oWriter.WriteAttributeCSS(L"position", L"absolute");
		nullable_bool behindDoc;	XmlMacroReadAttributeBase(oNode, L"behindDoc", behindDoc);
		nullable_bool allowOverlap; XmlMacroReadAttributeBase(oNode, L"allowOverlap", allowOverlap);
		nullable_bool layoutInCell; XmlMacroReadAttributeBase(oNode, L"layoutInCell", layoutInCell);

		XmlMacroReadAttributeBase(oNode, L"relativeHeight", outWriter.m_zIndex);
		if (outWriter.m_zIndex.is_init())
		{
			_INT64 z_index = *outWriter.m_zIndex;

			if ((behindDoc.IsInit()) && (*behindDoc == true))
			{
				outWriter.m_zIndex = -z_index;
			}
			oWriter.WriteAttributeCSS(L"z-index", std::to_wstring(*outWriter.m_zIndex));
		}

		if (allowOverlap.is_init())
		{
			oWriter.WriteAttributeCSS(L"o:allowoverlap", *allowOverlap ? L"true" : L"false");
		}

		if (layoutInCell.is_init())
		{
			oWriter.WriteAttributeCSS(L"o:allowincell", *layoutInCell ? L"true" : L"false");
		}

		XmlUtils::CXmlNode oNodeHorP;
		if (oNode.GetNode(L"wp:positionH", oNodeHorP))
		{
			std::wstring strWriteRelFrom = oNodeHorP.GetAttribute(L"relativeFrom", L"column");
			if (strWriteRelFrom == L"character")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"char");
			else if (strWriteRelFrom == L"page")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"page");
			else if (strWriteRelFrom == L"margin")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"margin");
			else if (strWriteRelFrom == L"leftMargin")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"left-margin-area");
			else if (strWriteRelFrom == L"rightMargin")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"right-margin-area");
			else if (strWriteRelFrom == L"insideMargin")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"inner-margin-area");
			else if (strWriteRelFrom == L"outsideMargin")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"outer-margin-area");
			else if (strWriteRelFrom == L"column")
				oWriter.WriteAttributeCSS(L"mso-position-horizontal-relative", L"text");

			XmlUtils::CXmlNode oNodeO;
			if (oNodeHorP.GetNode(L"wp:posOffset", oNodeO))
			{
				int nPos = oNodeHorP.ReadValueInt(L"wp:posOffset");
				outWriter.m_dX = dKoef * nPos;

				oWriter.WriteAttributeCSS_double1_pt(L"margin-left", outWriter.m_dX);
				oWriter.WriteAttributeCSS(L"mso-position-horizontal", L"absolute");
			}
			else
			{
				std::wstring sA = oNodeHorP.ReadValueString(L"wp:align");
				if (L"" != sA)
				{
					oWriter.WriteAttributeCSS(L"mso-position-horizontal", sA);
				}
			}
		}

		XmlUtils::CXmlNode oNodeVerP;
		if (oNode.GetNode(L"wp:positionV", oNodeVerP))
		{
			std::wstring strWriteRelFrom = oNodeVerP.GetAttribute(L"relativeFrom", L"paragraph");
			if (strWriteRelFrom == L"margin")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"margin");
			else if (strWriteRelFrom == L"paragraph")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"text");
			else if (strWriteRelFrom == L"page")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"page");
			else if (strWriteRelFrom == L"topMargin")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"top-margin-area");
			else if (strWriteRelFrom == L"bottomMargin")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"bottom-margin-area");
			else if (strWriteRelFrom == L"insideMargin")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"inner-margin-area");
			else if (strWriteRelFrom == L"outsideMargin")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"outer-margin-area");
			else if (strWriteRelFrom == L"line")
				oWriter.WriteAttributeCSS(L"mso-position-vertical-relative", L"line");

			XmlUtils::CXmlNode oNodeO;
			if (oNodeVerP.GetNode(L"wp:posOffset", oNodeO))
			{
				int nPos = oNodeVerP.ReadValueInt(L"wp:posOffset");
				outWriter.m_dY = dKoef * nPos;

				oWriter.WriteAttributeCSS_double1_pt(L"margin-top", outWriter.m_dY);
				oWriter.WriteAttributeCSS(L"mso-position-vertical", L"absolute");
			}
			else
			{
				std::wstring sA = oNodeVerP.ReadValueString(L"wp:align");
				if (L"" != sA)
				{
					oWriter.WriteAttributeCSS(L"mso-position-vertical", sA);
				}
			}
		}

		XmlUtils::CXmlNode oNodeS;
		if (oNode.GetNode(L"wp:extent", oNodeS))
		{
			int _width = oNodeS.ReadAttributeInt(L"cx");
			int _height = oNodeS.ReadAttributeInt(L"cy");

			outWriter.m_dWidth = dKoef * _width;
			outWriter.m_dHeight = dKoef * _height;

			oWriter.WriteAttributeCSS_double1_pt(L"width", outWriter.m_dWidth);
			oWriter.WriteAttributeCSS_double1_pt(L"height", outWriter.m_dHeight);
		}

		XmlUtils::CXmlNode oNodeWrap = oNode.ReadNode(L"<wp:wrapNone/>");
		XmlUtils::CXmlNode oNodeWrapPoints;
		if (oNodeWrap.IsValid())
		{
			// ok
		}
		else
		{
			oNodeWrap = oNode.ReadNode(L"wp:wrapSquare");
			if (oNodeWrap.IsValid())
			{
				outWriter.m_strNodes += L"<w10:wrap type=\"square\"/>";
			}
			else
			{
				oNodeWrap = oNode.ReadNode(L"wp:wrapTopAndBottom");
				if (oNodeWrap.IsValid())
				{
					outWriter.m_strNodes += L"<w10:wrap type=\"topAndBottom\"/>";
				}
				else
				{
					oNodeWrap = oNode.ReadNode(L"wp:wrapTight");
					if (oNodeWrap.IsValid())
					{
						outWriter.m_strNodes += L"<w10:wrap type=\"tight\"/>";
						oNodeWrap.GetNode(L"wp:wrapPolygon", oNodeWrapPoints);
					}
					else
					{
						oNodeWrap = oNode.ReadNode(L"wp:wrapThrough");
						if (oNodeWrap.IsValid())
						{
							outWriter.m_strNodes += L"<w10:wrap type=\"through\"/>";
							oNodeWrap.GetNode(L"wp:wrapPolygon", oNodeWrapPoints);
						}
					}
				}
			}
		}

		if (oNodeWrapPoints.IsValid())
		{
			double dKoefX = 100000.0 / 21600;
			double dKoefY = dKoefX;
			if (oElem.is<PPTX::Logic::SpTree>())
			{
				PPTX::Logic::SpTree& oSpTr = oElem.as<PPTX::Logic::SpTree>();
				if (oSpTr.grpSpPr.xfrm.is_init())
				{
					int nW = oSpTr.grpSpPr.xfrm->chExtX.get_value_or(21600);
					int nH = oSpTr.grpSpPr.xfrm->chExtY.get_value_or(21600);

					//dKoefX = ((double)(nW)) / 21600.0;
					//dKoefY = ((double)(nH)) / 21600.0;
					dKoefX = 1.0;
					dKoefY = 1.0;
				}
			}

			std::wstring strAttr = L" wrapcoords=\"";
			XmlUtils::CXmlNodes oNodesP;
			if (oNodeWrapPoints.GetNodes(L"*", oNodesP))
			{
				int nCountP = oNodesP.GetCount();
				for (int i = 0; i < nCountP; ++i)
				{
					XmlUtils::CXmlNode oNodeT;
					oNodesP.GetAt(i, oNodeT);

					int nX = oNodeT.ReadAttributeInt(L"x");
					int nY = oNodeT.ReadAttributeInt(L"y");
					nX = (int)(dKoefX * nX + 0.5);
					nY = (int)(dKoefY * nY + 0.5);

					std::wstring strFP = std::to_wstring(nX) + L" " + std::to_wstring(nY);
					strAttr += strFP;

					if (i < (nCountP - 1))
						strAttr += L" ";
				}
			}
			strAttr += L"\"";

			outWriter.m_strAttributesMain += strAttr;
		}
		

	}
	XmlUtils::CXmlNode oNodeDocPr;
	if (oNode.GetNode(L"wp:docPr", oNodeDocPr))
	{
		std::wstring strName = oNodeDocPr.GetAttribute(L"name");
		if (false == strName.empty())
		{
			std::wstring test = XmlUtils::GetLower(strName);
			if (std::wstring::npos != test.find(L"watermark"))
				outWriter.m_strId = strName;
		}
	}
	outWriter.m_strStyleMain = oWriter.GetXmlString();
}

//HRESULT CDrawingConverter::GetTxBodyBinary(const std::wstring& bsXml)
//{
//	XmlUtils::CXmlNode oNode;
//	if (!oNode.FromXmlString((std::wstring)bsXml))
//		return S_FALSE;
//	
//	PPTX::Logic::TxBody oTxBody(oNode);
//
//	//m_pBinaryWriter->ClearNoAttack();
//	//ULONG lOldPos = m_pBinaryWriter->GetPosition();
//	m_pBinaryWriter->m_pCommon->CheckFontPicker();
//	//m_pBinaryWriter->m_pCommon->m_pNativePicker->Init(m_strFontDirectory);
//
//	m_pBinaryWriter->WriteRecord1(0, oTxBody);
//
//	//m_pBinaryWriter->SetPosition(lOldPos);
//
//	//m_pBinaryWriter->ClearNoAttack();
//	return S_OK;
//}

//HRESULT CDrawingConverter::GetTxBodyXml(LONG lStart, std::wstring& sXml)
//{
//	m_pReader->Seek(lStart);
//
//	BYTE type = m_pReader->GetUChar();
//    if (0 != type)
//		return S_FALSE;
//
//	PPTX::Logic::TxBody oTxBody;
//	oTxBody.fromPPTY(m_pReader);
//
//	NSBinPptxRW::CXmlWriter oWriter;
//	oTxBody.toXmlWriterExcel(&oWriter);
//
//    sXml = oWriter.GetXmlString();
//
//	return S_OK;
//}

HRESULT CDrawingConverter::SetFontDir(const std::wstring& bsFontDir)
{
	m_strFontDirectory = bsFontDir;
	return S_OK;
}

HRESULT CDrawingConverter::GetRecordBinary(LONG lRecordType, const std::wstring& sXml)
{
    if (sXml.empty())
		return S_FALSE;

    std::wstring strXml = L"<main ";

    strXml += L"\
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:ve=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:a14=\"http://schemas.microsoft.com/office/drawing/2010/main\" \
xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\" \
xmlns:xdr=\"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing\" \
xmlns:c=\"http://schemas.openxmlformats.org/drawingml/2006/chart\"";

    strXml += L">";
    strXml += sXml;
    strXml += L"</main>";

	XmlUtils::CXmlNode oNodeMain;
	if (!oNodeMain.FromXmlString(strXml))
		return S_FALSE;

	XmlUtils::CXmlNodes oNodes;
    if (!oNodeMain.GetNodes(L"*", oNodes))
		return S_FALSE;

	if (1 != oNodes.GetCount())
		return S_FALSE;

	XmlUtils::CXmlNode oNode;
	oNodes.GetAt(0, oNode);

	PPTX::WrapperWritingElement* pWritingElem = NULL;
	switch (lRecordType)
	{
	case XMLWRITER_RECORD_TYPE_SPPR:
		{
			PPTX::Logic::SpPr* pSpPr = new PPTX::Logic::SpPr();
			*pSpPr = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pSpPr;
			break;
		}
	case XMLWRITER_RECORD_TYPE_CLRMAPOVR:
		{
			PPTX::Logic::ClrMap* pClrMap = new PPTX::Logic::ClrMap();
			*pClrMap = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pClrMap;
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_OUTLINE:
		{
			PPTX::Logic::Ln* pLn = new PPTX::Logic::Ln();
			*pLn = oNode;

			pWritingElem = (PPTX::WrapperWritingElement*)pLn;
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_FILL:
		{
			PPTX::Logic::UniFill* pUniFill = new PPTX::Logic::UniFill();
			pUniFill->GetFillFrom(oNode);

			pWritingElem = (PPTX::WrapperWritingElement*)pUniFill;
			break;
		}
	default:
		break;
	}

	if (NULL == pWritingElem)
		return S_FALSE;

	//m_pBinaryWriter->ClearNoAttack();
	m_pBinaryWriter->m_pCommon->CheckFontPicker();

	//ULONG lOldPos = m_pBinaryWriter->GetPosition();

	m_pBinaryWriter->WriteRecord1(0, *pWritingElem);
	
	
	RELEASEOBJECT(pWritingElem);

	//m_pBinaryWriter->SetPosition(lOldPos);

	return S_OK;	
}

HRESULT CDrawingConverter::GetRecordXml(LONG lStart, LONG lLength, LONG lRecType, int nDocType, std::wstring & sXml)
{
	if (NULL == m_pReader)
		return S_FALSE;

	m_pReader->m_pRels->m_pManager->m_nDocumentType = nDocType;

	m_pReader->Seek(lStart);
	
	BYTE typeRec1 = m_pReader->GetUChar();
	
	PPTX::WrapperWritingElement* pWritingElem = NULL;

	switch (lRecType)
	{
	case XMLWRITER_RECORD_TYPE_SPPR:
		{
			pWritingElem = (PPTX::WrapperWritingElement*)(new PPTX::Logic::SpPr());
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_CLRMAPOVR:
		{
			PPTX::Logic::ClrMap* pClrMap = new  PPTX::Logic::ClrMap();
            pClrMap->m_name = L"a:clrMapOvr";
			pWritingElem = (PPTX::WrapperWritingElement*)(pClrMap);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_OUTLINE:
		{
			PPTX::Logic::Ln* pLn = new  PPTX::Logic::Ln();
			pWritingElem = (PPTX::WrapperWritingElement*)(pLn);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	case XMLWRITER_RECORD_TYPE_TEXT_FILL:
		{
			PPTX::Logic::UniFill* pUniFill = new  PPTX::Logic::UniFill();
			pWritingElem = (PPTX::WrapperWritingElement*)(pUniFill);
			pWritingElem->fromPPTY(m_pReader);
			break;
		}
	default:
		break;
	}

	if (NULL == pWritingElem)
		return S_FALSE;

	NSBinPptxRW::CXmlWriter oXmlWriter;
	oXmlWriter.m_lDocType = (BYTE)nDocType;
    oXmlWriter.m_bIsUseOffice2007 = false;
	oXmlWriter.m_bIsTop = true;

	pWritingElem->toXmlWriter(&oXmlWriter);

    sXml = oXmlWriter.GetXmlString();

	RELEASEOBJECT(pWritingElem);

	return S_OK;
}

void CDrawingConverter::SetDstContentRels()
{
	 m_pReader->SetDstContentRels();
}
void CDrawingConverter::SaveDstContentRels(const std::wstring& bsRelsPath)
{
	m_pReader->SaveDstContentRels(bsRelsPath);
}
void CDrawingConverter::WriteRels (const std::wstring& bsType, const std::wstring& bsTarget, const std::wstring& bsTargetMode, unsigned int* lId)
{
	if (NULL == lId)				return;
	if (NULL == m_pReader)			return;
	if (NULL == m_pReader->m_pRels)	return;

	*lId = m_pReader->m_pRels->WriteRels(bsType, bsTarget, bsTargetMode);
}
void CDrawingConverter::Registration (const std::wstring& sType, const std::wstring& oDirectory, const std::wstring& oFilename)
{
	if (!m_pImageManager) return;
	if (!m_pImageManager->m_pContentTypes) return;
	
	m_pImageManager->m_pContentTypes->Registration(sType, OOX::CPath(oDirectory), OOX::CPath(oFilename));
}

HRESULT CDrawingConverter::SetFontPicker(COfficeFontPicker* pFontPicker)
{
	m_pBinaryWriter->m_pCommon->CreateFontPicker(pFontPicker);
	return S_OK;
}

HRESULT CDrawingConverter::SetAdditionalParam(const std::wstring& ParamName, BYTE *pArray, size_t szCount)
{
    std::wstring name = ParamName;
    if (name == L"xfrm_override" && pArray)
    {
        PPTX::Logic::Xfrm *pXfrm = (PPTX::Logic::Xfrm*)pArray;
		
        m_oxfrm_override = new PPTX::Logic::Xfrm(*pXfrm);
    }
    return S_OK;

}
HRESULT CDrawingConverter::GetAdditionalParam(const std::wstring& ParamName, BYTE **pArray, size_t& szCount)
{
    //std::wstring name = ParamName;
    //if (name == L"SerializeImageManager")
    //{
    //    NSBinPptxRW::CBinaryFileWriter oWriter;

    //    return oWriter.Serialize(m_pBinaryWriter->m_pCommon->m_pImageManager, pArray, szCount) ? S_OK : S_FALSE;
    //}
    //else if (name == L"SerializeImageManager2")
    //{
    //    NSBinPptxRW::CBinaryFileWriter oWriter;

    //    return oWriter.Serialize(m_pImageManager, pArray, szCount) ? S_OK : S_FALSE;
    //}
    return S_OK;
}

OOX::CContentTypes* CDrawingConverter::GetContentTypes()
{
	return m_pImageManager->m_pContentTypes;
}

void CDrawingConverter::Clear()
{
	m_mapShapeTypes.clear();
}
void CDrawingConverter::SetRels(smart_ptr<OOX::IFileContainer> container)
{
	m_pBinaryWriter->SetRels(container);
}
void CDrawingConverter::SetRels(OOX::IFileContainer *container)
{
	m_pBinaryWriter->SetRels(container);
}
smart_ptr<OOX::IFileContainer> CDrawingConverter::GetRels()
{
	return m_pBinaryWriter->GetRels();
}
void CDrawingConverter::SetFontManager(NSFonts::IFontManager* pFontManager)
{
	if(NULL != m_pBinaryWriter && NULL != m_pBinaryWriter->m_pCommon && NULL != m_pBinaryWriter->m_pCommon->m_pMediaManager)
	{
		m_pBinaryWriter->m_pCommon->m_pMediaManager->SetFontManager(pFontManager);
	}
}
}
