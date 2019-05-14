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
#pragma once
#include "Theme.h"
#include "Shapes/Shape.h"

#ifndef DISABLE_FILE_DOWNLOADER
    #include "../../../Common/FileDownloader/FileDownloader.h"
#endif

#include "Shapes/BaseShape/PPTShape/Ppt2PptxShapeConverter.h"

namespace PPTX2EditorAdvanced
{
    AVSINLINE OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::wstring& str)//const
	{
		if(str == _T(""))
			return OOXMLShapes::sptNil;
		switch((CHAR)str[0])
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


namespace PPT_FORMAT
{
	class CImageElement : public CElement
	{
	public:
		std::wstring	m_strImageFileName;

		BYTE			m_nAlpha;

		bool			m_bCropEnabled;

		_INT32			m_lcropFromRight;
		_INT32			m_lcropFromLeft;
		_INT32			m_lcropFromTop;
		_INT32			m_lcropFromBottom;

		bool			m_bStretch;
		bool			m_bTile;

		bool			m_bOLE;
		bool			m_bImagePresent;

		std::wstring	m_sImageName;

		CImageElement() : CElement()
		{
			m_etType = etPicture;
			
			m_nAlpha				= 0xFF;

			m_bCropEnabled			= false;
			
			m_lcropFromRight		= 0;
			m_lcropFromLeft			= 0;
			m_lcropFromTop			= 0;
			m_lcropFromBottom		= 0;

			m_bStretch				= true;
			m_bTile					= false;
			m_bOLE					= false;
			m_bImagePresent			= false;

		}
		virtual ~CImageElement()
		{
		}
		virtual CElementPtr CreateDublicate()
		{
			CImageElement* pImageElement = new CImageElement();

			CElementPtr	pElement = CElementPtr(	pImageElement );
			
			SetProperiesToDublicate(pElement);

			pImageElement->m_strImageFileName		= m_strImageFileName;
			pImageElement->m_nAlpha					= m_nAlpha;

			pImageElement->m_bCropEnabled			= m_bCropEnabled;

			pImageElement->m_lcropFromRight			= m_lcropFromRight;
			pImageElement->m_lcropFromLeft			= m_lcropFromLeft;
			pImageElement->m_lcropFromTop			= m_lcropFromTop;
			pImageElement->m_lcropFromBottom		= m_lcropFromBottom;

			pImageElement->m_bStretch				= m_bStretch;
			pImageElement->m_bTile					= m_bTile;
			pImageElement->m_bImagePresent			= m_bImagePresent;
			pImageElement->m_bOLE					= m_bOLE;

			return pElement;
		}
        AVSINLINE std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{
			NSGuidesVML::CFormParam pParamCoef;
			pParamCoef.m_eType = ptValue;
			pParamCoef.m_lParam = 65536;
			pParamCoef.m_lCoef = 65536;

			return ConvertPPTtoPPTX(/*pPPTShape,*/ pParamCoef, bIsNamespace);			
		}


        std::wstring ConvertPPTtoPPTX(/*CPPTShape* pPPTShape, */const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (bIsNamespace)
			{
				return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
			return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
						
            std::wstring strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}

		AVSINLINE std::wstring DownloadImage(const std::wstring& strFile)
		{
#ifndef DISABLE_FILE_DOWNLOADER
			CFileDownloader oDownloader(strFile, true);
			if ( oDownloader.DownloadSync() )
			{
				m_strImageFileName = oDownloader.GetFilePath();
			}
#endif
            return m_strImageFileName;
		}
	};

	class CShapeElement : public CElement
	{
	public:
		NSBaseShape::ClassType m_ClassType;

		int			m_lShapeType;
		CShapePtr	m_pShape;
		bool		m_bShapePreset; // or rect (
		
		CShapeElement(NSBaseShape::ClassType ClassType, int eType) : CElement()
		{
			m_lShapeType			= eType;
			m_ClassType				= ClassType;			
			m_etType				= etShape;

			m_pShape = CShapePtr( new CShape(ClassType, eType));

			m_pShape->m_rcBounds	= m_bChildAnchorEnabled ? m_rcChildAnchor : m_rcAnchor;

			m_pShape->m_dStartTime	= m_dStartTime;
			m_pShape->m_dStartTime	= m_dEndTime;

			m_bShapePreset			= false;

		}

		CShapeElement() : CElement()
		{
			m_lShapeType	= 0x1000;
			m_etType		= etShape;
			m_bShapePreset	= false;
			
			m_pShape = CShapePtr( new CShape(NSBaseShape::unknown, 0x1000));
		}

        CShapeElement(const std::wstring& str) : CElement()
		{
			m_lShapeType	= 0x1000;
			m_bShapePreset	= false;

			m_pShape = CShapePtr( new CShape(NSBaseShape::unknown, 0x1000));
			m_pShape->LoadFromXML(str);
			
			m_ClassType = m_pShape->getBaseShape()->GetClassType();
		}
		//virtual void NormalizeCoordsByMetric()
		//{
		//	CElement::NormalizeCoordsByMetric();

		//	double dScaleX				= (double)m_oMetric.m_lUnitsHor / m_oMetric.m_lMillimetresHor;
		//	double dScaleY				= (double)m_oMetric.m_lUnitsVer	/ m_oMetric.m_lMillimetresVer;

		//	m_pShape->m_oText.m_oBounds.left		= (int)(dScaleX * m_pShape->m_oText.m_oBounds.left);
		//	m_pShape->m_oText.m_oBounds.right	= (int)(dScaleX * m_pShape->m_oText.m_oBounds.right);
		//	m_pShape->m_oText.m_oBounds.top		= (int)(dScaleY * m_pShape->m_oText.m_oBounds.top);
		//	m_pShape->m_oText.m_oBounds.bottom	= (int)(dScaleY * m_pShape->m_oText.m_oBounds.bottom);
		//}
		virtual ~CShapeElement()
		{
		}

		virtual CElementPtr CreateDublicate()
		{
			CShapeElement* pShapeElement = new CShapeElement(m_ClassType, m_lShapeType);
			
			CElementPtr	pElement = CElementPtr(	pShapeElement );
			
			SetProperiesToDublicate(pElement);

			pShapeElement->m_lShapeType		= m_lShapeType;
			pShapeElement->m_bShapePreset	= m_bShapePreset;

			m_pShape->SetToDublicate(pShapeElement->m_pShape.get());
			
			return pElement;
		}
		bool SetUpTextPlaceholder(std::wstring newText);
		
		virtual void SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
		{
			m_pShape->m_oText.m_lPlaceholderType = m_lPlaceholderType;
			m_pShape->m_oText.m_lPlaceholderID	= m_lPlaceholderID;

			m_pShape->getBaseShape()->ReCalculate();

			SetupTextProperties(pSlide, pTheme, pLayout);

			CalculateColor(m_oPen.Color		, pSlide, pTheme, pLayout);
			CalculateColor(m_oBrush.Color1	, pSlide, pTheme, pLayout);
			CalculateColor(m_oBrush.Color2	, pSlide, pTheme, pLayout);
		}

		virtual void SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

		void CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

        AVSINLINE std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{
			CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_pShape->getBaseShape().get());
			if (NULL == pPPTShape)
			{
				// такого быть не может
				return _T("");
			}			

			NSGuidesVML::CFormParam pParamCoef;
			pParamCoef.m_eType	= ptValue;
			pParamCoef.m_lParam = 65536;
			pParamCoef.m_lCoef	= 65536;

			return ConvertPPTtoPPTX(pPPTShape, pParamCoef, bIsNamespace);			
		}

#ifndef OPTIMIZE_COMPILE_CONVERT_PPT_TO_PPTX

        std::wstring ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
						
            std::wstring strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			CFormulaConverter pFormulaConverter;

			//coeff
				pFormulaConverter.ConvertCoef ( pCoef );

			//guids----------------------------------------
			int nGuidCount = pPPTShape->m_oManager.m_arFormulas.size();
			if (0 != nGuidCount)
			{
					pFormulaConverter.ConvertFormula ( pPPTShape->m_oManager.m_arFormulas);					
			}				

			//path------------------------------------------
			int nPathCount = pPPTShape->m_strPath.GetLength();
			if (0 != nPathCount && pPPTShape->m_eType != 1)
			{
				pFormulaConverter.ConvertPath (pPPTShape->m_strPath, pPPTShape->m_oPath);

				//string rect
				int nRectCount = (int)pPPTShape->m_arStringTextRects.GetCount();
				if (0 != nRectCount)
				{
					pFormulaConverter.ConvertTextRect ( pPPTShape->m_arStringTextRects[0] );					
				}

				int nHandlesCount = pPPTShape->m_arHandles.size();
				int nAdjCount = pPPTShape->m_arAdjustments.size();

				//handles
				if (0 != nHandlesCount || 0 != nAdjCount)
				{
					pFormulaConverter.ConvertHandle (pPPTShape->m_arHandles, pPPTShape->m_arAdjustments, pPPTShape->m_eType);
				}
				pFormulaConverter.strGuidsRes = pFormulaConverter.strCoef + pFormulaConverter.strGuidsRes;

				//adj----------------------------
				if (pFormulaConverter.strAdjRes.GetLength() == 0)
					strXmlPPTX += _T("<a:avLst/>");
				else
					strXmlPPTX += _T("<a:avLst>") + pFormulaConverter.strAdjRes + _T("</a:avLst>");

				//guids--------------------------
				if (pFormulaConverter.strGuidsRes.GetLength() == 0)
					strXmlPPTX += _T("<a:gdLst/>");
				else
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.strGuidsRes + _T("</a:gdLst>");

				//handles---------------------------
				if (pFormulaConverter.strHandleRes.GetLength() == 0)
					strXmlPPTX += _T("<a:ahLst/>");
				else
					strXmlPPTX += _T("<a:ahLst>") + pFormulaConverter.strHandleRes + _T("</a:ahLst>");
					
				//connectors-------------------------
				strXmlPPTX += _T("<a:cxnLst/>");
				
				//textRect---------------------------
				if (pFormulaConverter.strTextRect.GetLength() != 0)
					strXmlPPTX += pFormulaConverter.strTextRect;

				//path------------------------------
				strXmlPPTX += _T("<a:pathLst>");
				strXmlPPTX += pFormulaConverter.strPathRes;
				strXmlPPTX += _T("</a:pathLst>");					
			}

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}
#else
        std::wstring ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect || pPPTShape->m_eType == PPTShapes::sptCFrame || pPPTShape->m_eType == PPTShapes::sptCTextBox)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
			else if (pPPTShape->m_eType == PPTShapes::sptCLine)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"line\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"line\"><a:avLst/></a:prstGeom>");
			}
			else if (pPPTShape->m_eType == PPTShapes::sptCEllipse)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"ellipse\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"ellipse\"><a:avLst/></a:prstGeom>");
			}
            std::wstring strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			CFormulaConverter pFormulaConverter;

			//coeff
			pFormulaConverter.ConvertCoef(pCoef);

			//guids----------------------------------------
			int nGuidCount = (int)pPPTShape->m_oManager.m_arFormulas.size();
			if (0 != nGuidCount)
			{
				pFormulaConverter.ConvertFormula(pPPTShape->m_oManager.m_arFormulas);
			}				

			//path------------------------------------------
            int nPathCount = (int)pPPTShape->m_strPath.length();
			if (0 != nPathCount && pPPTShape->m_eType != 1)
			{
				pFormulaConverter.ConvertPath(pPPTShape->m_strPath, pPPTShape->m_oPath);

				//string rect
				int nRectCount = (int)pPPTShape->m_arStringTextRects.size();
				if (0 != nRectCount)
				{
					pFormulaConverter.ConvertTextRect(pPPTShape->m_arStringTextRects[0]);
				}

				int nHandlesCount	= (int)pPPTShape->m_arHandles.size();
				int nAdjCount		= (int)pPPTShape->m_arAdjustments.size();

				//handles
				if (0 != nHandlesCount || 0 != nAdjCount)
				{
					pFormulaConverter.ConvertHandle(pPPTShape->m_arHandles, pPPTShape->m_arAdjustments, pPPTShape->m_eType);
				}

				//adj----------------------------
				if (pFormulaConverter.m_oAdjRes.GetSize() == 0)
					strXmlPPTX += _T("<a:avLst/>");
				else
					strXmlPPTX += _T("<a:avLst>") + pFormulaConverter.m_oAdjRes.GetXmlString() + _T("</a:avLst>");

				//guids--------------------------
				if (pFormulaConverter.m_oGuidsRes.GetSize() == 0)
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + _T("</a:gdLst>");
				else
					strXmlPPTX += _T("<a:gdLst>") + pFormulaConverter.m_oCoef.GetXmlString() + pFormulaConverter.m_oGuidsRes.GetXmlString() + _T("</a:gdLst>");

				//handles---------------------------
				if (pFormulaConverter.m_oHandleRes.GetSize() == 0)
					strXmlPPTX += _T("<a:ahLst/>");
				else
					strXmlPPTX += _T("<a:ahLst>") + pFormulaConverter.m_oHandleRes.GetXmlString() + _T("</a:ahLst>");
					
				//connectors-------------------------
				strXmlPPTX += _T("<a:cxnLst/>");
				
				//textRect---------------------------
				if (pFormulaConverter.m_oTextRect.GetSize() != 0)
					strXmlPPTX += pFormulaConverter.m_oTextRect.GetXmlString();

				//path------------------------------
				strXmlPPTX += _T("<a:pathLst>");
				strXmlPPTX += pFormulaConverter.m_oPathRes.GetXmlString();
				strXmlPPTX += _T("</a:pathLst>");					
			}
			else
			{
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}
#endif

		
	};

	class CAudioElement : public CImageElement
	{
	public:
		std::wstring	m_strAudioFileName;
		
		BYTE			m_nAmplify;
		bool			m_bWithVideo;
		double			m_dAudioDuration;

		double			m_dClipStartTime;
		double			m_dClipEndTime;
        bool			m_bLoop;

		CAudioElement() : CImageElement()
		{
			m_etType			= etAudio;
			
			m_nAmplify			= 100;
			m_bWithVideo		= false;

			m_dAudioDuration	= 0.0;
			m_bLoop				= false;

			m_dClipStartTime	= 0.0;
			m_dClipEndTime		= -1.0;
		}

		virtual ~CAudioElement()
		{
		}

		virtual CElementPtr CreateDublicate()
		{
			CAudioElement* pAudioElement = new CAudioElement();
			CElementPtr	pElement = CElementPtr(	pAudioElement );
			
			SetProperiesToDublicate(pElement);

			pAudioElement->m_strAudioFileName	= m_strAudioFileName;
			pAudioElement->m_nAmplify			= m_nAmplify;
			pAudioElement->m_bWithVideo			= m_bWithVideo;

			pAudioElement->m_dAudioDuration		= m_dAudioDuration;
			pAudioElement->m_bLoop				= m_bLoop;

			pAudioElement->m_dClipStartTime		= m_dClipStartTime;
			pAudioElement->m_dClipEndTime		= m_dClipEndTime;

			return pElement;
		}
	};
	class CVideoElement : public CImageElement
	{
	public:
		std::wstring	m_strVideoFileName;
		double			m_dVideoDuration;
		BYTE			m_nAlpha;

		double			m_dClipStartTime;
		double			m_dClipEndTime;

		bool			m_bLoop;

		CVideoElement() : CImageElement()
		{
			m_etType			=	etVideo;
			
			m_dVideoDuration	=	0.0;
			m_nAlpha			=	0xFF;
					
			m_dClipStartTime	=	-1.0;
			m_dClipEndTime		=	-1.0;
			m_bLoop				=	false;
		}

		virtual ~CVideoElement()
		{
		}
		
		virtual CElementPtr CreateDublicate()
		{
			CVideoElement* pVideoElement = new CVideoElement();
			
			CElementPtr	pElement = CElementPtr(	pVideoElement );
			
			SetProperiesToDublicate(pElement);

			pVideoElement->m_strVideoFileName	=	m_strVideoFileName;
			pVideoElement->m_nAlpha				=	m_nAlpha;

			pVideoElement->m_dVideoDuration		=	m_dVideoDuration;

			pVideoElement->m_dClipStartTime		=	m_dClipStartTime;
			pVideoElement->m_dClipEndTime		=	m_dClipEndTime;
			pVideoElement->m_bLoop				=	m_bLoop;

			return pElement;
		}
	};

	class CGroupElement : public CElement
	{
	public:
		CGroupElement() : CElement()
		{
			m_etType = etGroup;
		}
		virtual ~CGroupElement()
		{
		}
		virtual CElementPtr CreateDublicate()
		{
			CGroupElement* pGroupElement = new CGroupElement();

			CElementPtr	pElement = CElementPtr(	pGroupElement );
			
			SetProperiesToDublicate(pElement);

			return pElement;
		}
        AVSINLINE std::wstring ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{

			return L"";		
		}
	};


}

