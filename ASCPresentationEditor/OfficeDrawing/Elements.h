#pragma once
#include "Theme.h"
#include "Shapes/Shape.h"

#ifdef _SVG_CONVERT_TO_IMAGE_
	#include "SVGTransformer.h"
#endif

#ifdef _PRESENTATION_WRITER_
	#include "SVGConverter.h"
#endif

#ifndef DISABLE_FILE_DOWNLOADER
	#include "../../Common/FileDownloader.h"
#endif


#ifdef ENABLE_PPT_TO_PPTX_CONVERT
	#include "Shapes/BaseShape/PPTShape/ppt2pptxshapeconverter.h"
#endif

#if defined(PPTX_DEF)
namespace PPTX2EditorAdvanced
{
	AVSINLINE OOXMLShapes::ShapeType GetShapeTypeFromStr(const CString& str)//const
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
#endif

namespace NSPresentationEditor
{
	/**************************************************************
				  здесь все типы элементов
	**************************************************************/

	class CImageElement : public IElement
	{
	public:
		std::wstring	m_strImageFileName;

		BYTE			m_nAlpha;

		bool			m_bCropEnabled;

		long			m_lcropFromRight;
		long			m_lcropFromLeft;
		long			m_lcropFromTop;
		long			m_lcropFromBottom;

		bool			m_bStretch;
		bool			m_bTile;

		bool			m_bOLE;
		bool			m_bImagePresent;

		std::wstring	m_sImageName;

	public:
		CImageElement() : IElement()
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

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual CString SaveToXML()
		{
			if (!m_oAnimations.m_arAnimations.empty())
				return ToAnimationXml();
			
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			CString strEffect = _T("");
            strEffect.Format(_T("<ImagePaint-DrawImageFromFile left='%d' top='%d' right='%d' bottom='%d' angle='%lf' flags='%d' filepath='%ls' metric='0' backcolor='-1' scaletype='-1' scalecolor='255' widthmetric='%d' heightmetric='%d'>\
					<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0' /></ImagePaint-DrawImageFromFile>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom,
					m_dRotate, lFlags, m_strImageFileName, 
					m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer, 
					m_dStartTime, m_dEndTime);

			return strEffect;
		}

		CString ToAnimationXml()
		{
			LONG lFlags = 0;
			if (m_bFlipH)
				lFlags |= 0x0001;
			if (m_bFlipV)
				lFlags |= 0x0002;

			CString strEffect = _T("");
            strEffect.Format(_T("<ImagePaint-DrawImageFromFile left='%d' top='%d' right='%d' bottom='%d' angle='%lf' flags='%d' filepath='%ls' metric='0' backcolor='-1' scaletype='-1' scalecolor='255' widthmetric='%d' heightmetric='%d'>\
					<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0' /></ImagePaint-DrawImageFromFile>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom,
					m_dRotate, lFlags, m_strImageFileName, 
					m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer, 
					m_dStartTime, m_dEndTime);

			CString TimeLine;
			TimeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),
				m_dStartTime, m_dEndTime );

			CString Source	= m_oAnimations.ToXml(m_dStartTime, m_dEndTime);
			Source	+=	TimeLine;
			Source	+=	_T("</ImagePaint-DrawImageFromFileAnimate>");

			strEffect.Replace ( _T("</ImagePaint-DrawImageFromFile>"), Source );
			strEffect.Replace ( _T("<ImagePaint-DrawImageFromFile"), _T("<ImagePaint-DrawImageFromFileAnimate") );

			return strEffect;
		}

		virtual IElement* CreateDublicate()
		{
			CImageElement* pImageElement = new CImageElement();
			
			SetProperiesToDublicate((IElement*)pImageElement);

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

			return (IElement*)pImageElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
		}
		
#ifdef ENABLE_PPT_TO_PPTX_CONVERT

		AVSINLINE CString ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{
			NSGuidesVML::CFormParam pParamCoef;
			pParamCoef.m_eType = ptValue;
			pParamCoef.m_lParam = 65536;
			pParamCoef.m_lCoef = 65536;

			return ConvertPPTtoPPTX(/*pPPTShape,*/ pParamCoef, bIsNamespace);			
		}


		CString ConvertPPTtoPPTX(/*CPPTShape* pPPTShape, */const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (bIsNamespace)
			{
				return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
			return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
						
			CString strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			strXmlPPTX += _T("</a:custGeom>");
			return strXmlPPTX;
		}

#endif
		AVSINLINE std::wstring DownloadImage(const std::wstring& strFile)
		{
#ifndef DISABLE_FILE_DOWNLOADER
			CFileDownloader oDownloader(strFile, true);
			oDownloader.Start( 1 );
			while ( oDownloader.IsRunned() )
			{
				::Sleep( 10 );
			}

			if ( oDownloader.IsFileDownloaded() )
			{
				m_strImageFileName = oDownloader.GetFilePath();
			}
#endif
            return m_strImageFileName;
		}
	};

	class CShapeElement : public IElement
	{
	public:
		NSBaseShape::ClassType m_ClassType;

		int			m_lShapeType;
		CShape		m_oShape;
		bool		m_bShapePreset; // or rect (
		
		CShapeElement(NSBaseShape::ClassType ClassType, int eType) : IElement(), m_lShapeType(eType), m_oShape(ClassType, eType)
		{
			m_ClassType				= ClassType;			
			m_etType				= etShape;

			m_oShape.m_rcBounds		= m_rcBounds;

			m_oShape.m_dStartTime	= m_dStartTime;
			m_oShape.m_dStartTime	= m_dEndTime;

			m_bShapePreset			= false;
		}

		CShapeElement() : m_oShape(NSBaseShape::unknown, 0x1000)
		{
			m_lShapeType	= 0x1000;
			m_etType		= etShape;
			m_bShapePreset	= false;
		}

		CShapeElement(const CString& str) : IElement(), m_oShape(NSBaseShape::unknown, 0x1000)
		{
			m_lShapeType	= 0x1000;
			m_bShapePreset	= false;

			m_oShape.LoadFromXML(str);
			m_ClassType = m_oShape.m_pShape->GetClassType();
		}
		virtual void NormalizeCoordsByMetric()
		{
			IElement::NormalizeCoordsByMetric();

			double dScaleX				= (double)m_oMetric.m_lUnitsHor / m_oMetric.m_lMillimetresHor;
			double dScaleY				= (double)m_oMetric.m_lUnitsVer	/ m_oMetric.m_lMillimetresVer;

			m_oShape.m_oText.m_oBounds.left		*= dScaleX;
			m_oShape.m_oText.m_oBounds.right	*= dScaleX;
			m_oShape.m_oText.m_oBounds.top		*= dScaleY;
			m_oShape.m_oText.m_oBounds.bottom	*= dScaleY;
		}
		virtual ~CShapeElement()
		{
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual CString SaveToXML()
		{
			if (!m_oAnimations.m_arAnimations.empty())
				return ToAnimationXml();
			
			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_rcBounds);

			oInfo.m_dRotate = m_dRotate;
			oInfo.m_bFlipH	= m_bFlipH;
			oInfo.m_bFlipV	= m_bFlipV;

			return m_oShape.ToXml(oInfo, m_oMetric, m_dStartTime, m_dEndTime, m_pTheme, m_pLayout);
		}

		CString ToAnimationXml()
		{
			CGeomShapeInfo oInfo;
			oInfo.SetBounds(m_rcBounds);

			oInfo.m_dRotate = m_dRotate;
			oInfo.m_bFlipH	= m_bFlipH;
			oInfo.m_bFlipV	= m_bFlipV;

			m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
			m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;

			CString timeXML;
			timeXML.Format(_T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "), m_dStartTime, m_dEndTime );
			
			CString baseXML = m_oShape.ToXml(oInfo, m_oMetric, m_dStartTime, m_dEndTime, m_pTheme, m_pLayout);

			if ( -1 != baseXML.Find ( _T("<ImagePaint-DrawTextEx") ) )
			{
				CString sBounds;	// FIX
				sBounds.Format(_T("<bounds left='%d' top='%d' right='%d' bottom='%d'/>"), 
					(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom);

				CString sReplace = m_oAnimations.ToXml(m_dStartTime, m_dEndTime) + timeXML + sBounds + _T("</ImagePaint-DrawTextAnimateEx>");
				
				baseXML.Replace ( _T("</ImagePaint-DrawTextEx>"),	sReplace );
				baseXML.Replace ( _T("<ImagePaint-DrawTextEx"),		_T("<ImagePaint-DrawTextAnimateEx") );
			}

			if ( -1 != baseXML.Find ( _T("<ImagePaint-DrawGraphicPath") ) )
			{
                                CString sAnim = m_oAnimations.ToXml(m_dStartTime, m_dEndTime, true);
				if (sAnim.GetLength())
				{
					CString sReplace = sAnim + timeXML + _T("</ImagePaint-DrawGraphicPathAnimate>");
					
					baseXML.Replace ( _T("</ImagePaint-DrawGraphicPath>"),	sReplace );
					baseXML.Replace ( _T("<ImagePaint-DrawGraphicPath"),	_T("<ImagePaint-DrawGraphicPathAnimate") );
				}
			}
			
			return baseXML;
		}

		virtual IElement* CreateDublicate()
		{
			CShapeElement* pShapeElement = new CShapeElement(m_ClassType, m_lShapeType);
			
			SetProperiesToDublicate((IElement*)pShapeElement);

			pShapeElement->m_lShapeType		= m_lShapeType;
			pShapeElement->m_bShapePreset	= m_bShapePreset;

			m_oShape.SetToDublicate(&pShapeElement->m_oShape);
			return (IElement*)pShapeElement;
		}

		
		virtual void SetupProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
		{
			std::map<CElementProperty::Type, CElementProperty>* pMap = &m_oProperties.m_arProperties;
			
			for (std::map<CElementProperty::Type, CElementProperty>::iterator pPair = pMap->begin(); pPair != pMap->end(); ++pPair)
			{
				CElementProperty oProperty = pPair->second;
				SetupProperty(pSlide, pTheme, pLayout, &oProperty);
			}

			m_oShape.m_oText.m_lPlaceholderType = m_lPlaceholderType;
			m_oShape.m_oText.m_lPlaceholderID	= m_lPlaceholderID;

			m_oShape.m_pShape->ReCalculate();

			SetupTextProperties(pSlide, pTheme, pLayout);

			CalculateColor(m_oPen.Color		, pSlide, pTheme, pLayout);
			CalculateColor(m_oBrush.Color1	, pSlide, pTheme, pLayout);
			CalculateColor(m_oBrush.Color2	, pSlide, pTheme, pLayout);
		}

		virtual void SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

		void CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout);

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
			const LONG EMU_MM = 36000;

			switch (pProperty->m_ID)
			{
			case CElementProperty::epTextMarginLeft:
				{
					m_oShape.m_dTextMarginX		= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginTop:
				{
					m_oShape.m_dTextMarginY			= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginRight:
				{
					m_oShape.m_dTextMarginRight		= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epTextMarginBottom:
				{
					m_oShape.m_dTextMarginBottom	= (double)pProperty->m_dwValue / EMU_MM;
					break;
				}
			case CElementProperty::epText:
				{
					//m_oShape.m_oText.m_sText		= pProperty->m_strAdvanced;
				}
			case CElementProperty::epTextWrap:
				{
					m_oShape.m_oText.m_lWrapMode	= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushType:
				{
					m_oBrush.Type			= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushColor1:
				{
					m_oBrush.Color1.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oBrush.Color1, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epBrushColor2:
				{
					m_oBrush.Color2.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oBrush.Color2, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epBrushAlpha1:
				{
					m_oBrush.Alpha1 = (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushAlpha2:
				{
					m_oBrush.Alpha2 = (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epBrushTxPath:
				{
					m_oBrush.TexturePath = pProperty->m_strAdvanced;
					break;
				}
			case CElementProperty::epBrushTxMode:
				{
					m_oBrush.TextureMode	= (LONG)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFilled:
				{
					if (0 == pProperty->m_dwValue)
					{
						m_oBrush.Alpha1		= 0;
					}
					break;
				}
			case CElementProperty::epPenColor:
				{
					m_oPen.Color.SetSBGR(pProperty->m_dwValue);
					CalculateColor(m_oPen.Color, pSlide, pTheme, pLayout);
					break;
				}
			case CElementProperty::epPenAlpha:
				{
					m_oPen.Alpha		= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epPenWidth:
				{
					m_oPen.Size		= (double)pProperty->m_dwValue / 0xFFFF;
					break;
				}
			case CElementProperty::epPenJoin:
				{
					m_oPen.LineJoin	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineDash:
				{
					m_oPen.DashStyle	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineStartCap:
				{
					m_oPen.LineStartCap	= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epLineEndCap:
				{
					m_oPen.LineEndCap		= (BYTE)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epStroked:
				{
					if (0 == pProperty->m_dwValue)
						m_oPen.Alpha = 0;

					break;
				}
			case CElementProperty::epFontName:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Name = pProperty->m_strAdvanced;
					break;
				}
			case CElementProperty::epFontHorAlign:
				{
					m_oShape.m_oText.m_oAttributes.m_nTextAlignHorizontal = (int)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontVertAlign:
				{
					m_oShape.m_oText.m_oAttributes.m_nTextAlignVertical	  = (int)pProperty->m_dwValue;	
					break;
				}
			case CElementProperty::epFontSize:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Size			= (double)pProperty->m_dwValue / 0xFFFF;
					break;
				}
			case CElementProperty::epFontBold:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Bold			= (bool)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontItalic:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Italic		= (bool)pProperty->m_dwValue;
					break;
				}
			case CElementProperty::epFontStrikeout:
				{
					m_oShape.m_oText.m_oAttributes.m_oFont.Strikeout	= (BYTE)pProperty->m_dwValue;
					break;
				}
			default:
				break;
			}
		}

#ifdef ENABLE_PPT_TO_PPTX_CONVERT

		AVSINLINE CString ConvertPPTShapeToPPTX(bool bIsNamespace = false)
		{
			CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_oShape.m_pShape);
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

		CString ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect)
			{
				if (bIsNamespace)
				{
					return _T("<a:prstGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" prst=\"rect\"><a:avLst/></a:prstGeom>");
				}
				return _T("<a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom>");
			}
						
			CString strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

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
		CString ConvertPPTtoPPTX(CPPTShape* pPPTShape, const NSGuidesVML::CFormParam& pCoef, bool bIsNamespace = false)
		{
			if (pPPTShape->m_eType == PPTShapes::sptCRect)
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
						
			CString strXmlPPTX = bIsNamespace ? _T("<a:custGeom xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\">") : _T("<a:custGeom>");

			CFormulaConverter pFormulaConverter;

			//coeff
			pFormulaConverter.ConvertCoef(pCoef);

			//guids----------------------------------------
			int nGuidCount = pPPTShape->m_oManager.m_arFormulas.size();
			if (0 != nGuidCount)
			{
				pFormulaConverter.ConvertFormula(pPPTShape->m_oManager.m_arFormulas);
			}				

			//path------------------------------------------
			int nPathCount = pPPTShape->m_strPath.GetLength();
			if (0 != nPathCount && pPPTShape->m_eType != 1)
			{
				pFormulaConverter.ConvertPath(pPPTShape->m_strPath, pPPTShape->m_oPath);

				//string rect
				int nRectCount = (int)pPPTShape->m_arStringTextRects.size();
				if (0 != nRectCount)
				{
					pFormulaConverter.ConvertTextRect(pPPTShape->m_arStringTextRects[0]);
				}

				int nHandlesCount = pPPTShape->m_arHandles.size();
				int nAdjCount = pPPTShape->m_arAdjustments.size();

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

#endif
		
	};

	class CAudioElement : public IElement
	{
	public:
		std::wstring	m_strAudioFileName;
		
		BYTE			m_nAmplify;
		bool			m_bWithVideo;
		double			m_dAudioDuration;

		double			m_dClipStartTime;
		double			m_dClipEndTime;
        bool			m_bLoop;

	public:
		CAudioElement() : IElement()
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

		virtual CString SaveToXML()	
		{
			CString element = _T("");
			element.Format(_T("<AudioSource StartTime='%lf' Duration='%lf' Amplify='%lf'>"), m_dStartTime, m_dEndTime-m_dStartTime, (double)m_nAmplify);

			int lIndex = m_strAudioFileName.find(L"file:///");
			if (0 == lIndex)
			{
				m_strAudioFileName = m_strAudioFileName.substr(8);
				//m_strFileName.Replace('/', '\\');
				//m_strFileName.Replace(L"%20", L" ");
			}

			CString strFileName = std_string2string(m_strAudioFileName);
			CorrectXmlString(strFileName);

			CString source;
            source.Format(_T("<Source StartTime='%lf' EndTime='%lf' FilePath='%ls' loop='%d' />"), m_dClipStartTime, m_dClipEndTime, strFileName, m_bLoop);
			element	+=	source;		

			CString animations;
			if(!m_oAnimations.m_arAnimations.empty())								//	для audio только "media call's" - play - pause - stop
			{
				m_oAnimations.m_dSlideWidth		=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
				animations						=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}
			
			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );
			
			element			+=	timeLine;		
			element			+=	_T("</AudioSource>");

			return element;
		}

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}

		virtual IElement* CreateDublicate()
		{
			CAudioElement* pAudioElement = new CAudioElement();
			
			SetProperiesToDublicate((IElement*)pAudioElement);

			pAudioElement->m_strAudioFileName	= m_strAudioFileName;
			pAudioElement->m_nAmplify			= m_nAmplify;
			pAudioElement->m_bWithVideo			= m_bWithVideo;

			pAudioElement->m_dAudioDuration		= m_dAudioDuration;
			pAudioElement->m_bLoop				= m_bLoop;

			pAudioElement->m_dClipStartTime		= m_dClipStartTime;
			pAudioElement->m_dClipEndTime		= m_dClipEndTime;

			return (IElement*)pAudioElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
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

	public:
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

		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
			CString strXml = SaveToXML();
			oWriter.WriteString(strXml);
		}
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
		}
		
		virtual CString SaveToXML()
		{
			return GetVideoStream () + GetAudioStream ();
		}
		
		CString ToAnimationXml()
		{
			return _T("");
		}

		virtual IElement* CreateDublicate()
		{
			CVideoElement* pVideoElement = new CVideoElement();
			
			SetProperiesToDublicate((IElement*)pVideoElement);

			pVideoElement->m_strVideoFileName	=	m_strVideoFileName;
			pVideoElement->m_nAlpha				=	m_nAlpha;

			pVideoElement->m_dVideoDuration		=	m_dVideoDuration;

			pVideoElement->m_dClipStartTime		=	m_dClipStartTime;
			pVideoElement->m_dClipEndTime		=	m_dClipEndTime;
			pVideoElement->m_bLoop				=	m_bLoop;

			return (IElement*)pVideoElement;
		}

		virtual void SetupProperty(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout, CElementProperty* pProperty)
		{
		}

		inline CString GetVideoStream ()
		{
			int lIndex = m_strVideoFileName.find(L"file:///");
			if (0 == lIndex)
			{
				m_strVideoFileName = m_strVideoFileName.substr(8);
				/*m_strFileName.Replace('/', '\\');*/
			}
			
			CString strFileName = std_string2string(m_strVideoFileName);
			CorrectXmlString(strFileName);

			CString element;
			element.Format ( _T("<VideoStream left='%d' top='%d' right='%d' bottom='%d' angle='%f' loop='%d' ")
				_T(" widthmetric='%d' heightmetric='%d' ")
                _T(" file='%ls' begin='%f' end='%f' >"),
				(LONG)m_rcBounds.left, (LONG)m_rcBounds.top, (LONG)m_rcBounds.right, (LONG)m_rcBounds.bottom, m_dRotate, m_bLoop,
				m_oMetric.m_lMillimetresHor, m_oMetric.m_lMillimetresVer,
				strFileName, m_dClipStartTime, m_dClipEndTime );

			CString animations;
			if(!m_oAnimations.m_arAnimations.empty())
			{
                                m_oAnimations.m_dSlideWidth	=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
                                animations			=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}

			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );

			element			+=	timeLine;		
			element			+=	_T("</VideoStream>");

			return element;
		}

		inline CString GetAudioStream ()
		{
			CString element = _T("");
			element.Format(_T("<AudioSource StartTime='%lf' Duration='%lf' Amplify='%lf' loop='%d' >"), m_dStartTime, m_dEndTime - m_dStartTime, 100.0, m_bLoop);

			int lIndex = m_strVideoFileName.find(L"file:///");
			if (0 == lIndex)
			{
				m_strVideoFileName = m_strVideoFileName.substr(8);
				//m_strFileName.Replace('/', '\\');
				//m_strFileName.Replace(L"%20", L" ");
			}

			CString strFileName = std_string2string(m_strVideoFileName);
			CorrectXmlString(strFileName);

			CString source;
            source.Format(_T("<Source StartTime='%lf' EndTime='%lf' FilePath='%ls'/>"), m_dClipStartTime, m_dClipEndTime, strFileName);
			element	+=	source;		

			CString animations;
			if(!m_oAnimations.m_arAnimations.empty())								//	для audio только "media call's" - play - pause - stop
			{
                                m_oAnimations.m_dSlideWidth	=	m_oMetric.m_lMillimetresHor;
				m_oAnimations.m_dSlideHeight	=	m_oMetric.m_lMillimetresVer;
                                animations			=	m_oAnimations.ToXml(m_dStartTime, m_dEndTime);

				element		+=	animations;
			}
			
			CString timeLine;
			timeLine.Format ( _T("<timeline type = \"1\"  begin=\"%f\" end=\"%f\" fadein=\"0\" fadeout=\"0\" completeness=\"1.0\"/> "),	m_dStartTime, m_dEndTime );
			
			element			+=	timeLine;		
			element			+=	_T("</AudioSource>");

			return element;
		}
	};

}
namespace NSStrings
{
	class CTextItem
	{
	protected:
		wchar_t*	m_pData;
		size_t		m_lSize;

		wchar_t*	m_pDataCur;
		size_t		m_lSizeCur;

	public:
		CTextItem()
		{
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= m_lSize;
		}
		CTextItem(const CTextItem& oSrc)
		{
			m_pData = NULL;
			*this = oSrc;
		}
		CTextItem& operator=(const CTextItem& oSrc)
		{
			RELEASEMEM(m_pData);

			m_lSize		= oSrc.m_lSize;
			m_lSizeCur	= oSrc.m_lSizeCur;
			m_pData		= (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, oSrc.m_pData, m_lSizeCur * sizeof(wchar_t));
							
			m_pDataCur = m_pData + m_lSizeCur;

			return *this;
		}

		CTextItem(const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = 0;
			m_pDataCur = m_pData;
		}
		CTextItem(wchar_t* pData, const size_t& nLen)
		{
			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;
		}
		CTextItem(wchar_t* pData, BYTE* pUnicodeChecker = NULL)
		{
			size_t nLen = GetStringLen(pData);

			m_lSize = nLen;
			m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));

			memcpy(m_pData, pData, m_lSize * sizeof(wchar_t));
				
			m_lSizeCur = m_lSize;
			m_pDataCur = m_pData + m_lSize;

			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		virtual ~CTextItem()
		{
			RELEASEMEM(m_pData);
		}

		AVSINLINE void AddSize(const size_t& nSize)
		{
			if (NULL == m_pData)
			{
                m_lSize = (std::max)(nSize, (size_t) 1000);
				m_pData = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
				
				m_lSizeCur = 0;
				m_pDataCur = m_pData;
				return;
			}

			if ((m_lSizeCur + nSize) > m_lSize)
			{
				while ((m_lSizeCur + nSize) > m_lSize)
				{
					m_lSize *= 2;
				}

				wchar_t* pRealloc = (wchar_t*)realloc(m_pData, m_lSize * sizeof(wchar_t));
				if (NULL != pRealloc)
				{
					// реаллок сработал
					m_pData		= pRealloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
				else
				{
					wchar_t* pMalloc = (wchar_t*)malloc(m_lSize * sizeof(wchar_t));
					memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(wchar_t));

					free(m_pData);
					m_pData		= pMalloc;
					m_pDataCur	= m_pData + m_lSizeCur;
				}
			}
		}

	public:
		
		AVSINLINE void operator+=(const CTextItem& oTemp)
		{
			WriteString(oTemp.m_pData, oTemp.m_lSizeCur);
		}
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void operator+=(_bstr_t& oTemp)
		{
			size_t nLen = oTemp.length();
			WriteString(oTemp.GetBSTR(), nLen);
		}
#endif
		AVSINLINE void operator+=(CString& oTemp)
		{
			size_t nLen = (size_t)oTemp.GetLength();

			#ifdef _UNICODE
			WriteString(oTemp.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)oTemp;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE wchar_t operator[](const size_t& nIndex)
		{
			if (nIndex < m_lSizeCur)
				return m_pData[nIndex];

			return 0;
		}

		AVSINLINE void SetText(BSTR& bsText)
		{
			ClearNoAttack();
#if defined(_WIN32) || defined (_WIN64)
			size_t nLen = GetStringLen(bsText);
            WriteString(bsText, nLen);
#else
            size_t nLen = bsText.length();
            WriteString(bsText.c_str(), nLen);
#endif

			for (size_t i = 0; i < nLen; ++i)
			{
				if (WCHAR(8233) == m_pData[i])
					m_pData[i] = WCHAR(' ');
			}
		}
		AVSINLINE void AddSpace()
		{
			AddSize(1);
			*m_pDataCur = wchar_t(' ');

			++m_lSizeCur;
			++m_pDataCur;
		}
		AVSINLINE void CorrectUnicode(const BYTE* pUnicodeChecker)
		{
			if (NULL != pUnicodeChecker)
			{
				wchar_t* pMemory = m_pData;
				while (pMemory < m_pDataCur)
				{
					if (!pUnicodeChecker[*pMemory])
						*pMemory = wchar_t(' ');
					++pMemory;
				}
			}
		}
		AVSINLINE void RemoveLastSpaces()
		{
			wchar_t* pMemory = m_pDataCur - 1;
			while ((pMemory > m_pData) && (wchar_t(' ') == *pMemory))
			{
				--pMemory;
				--m_lSizeCur;
				--m_pDataCur;
			}

		}
		AVSINLINE bool IsSpace()
		{
			if (1 != m_lSizeCur)
				return false;
			return (wchar_t(' ') == *m_pData);
		}
		
	public:
        AVSINLINE void WriteString(const wchar_t* pString, const size_t& nLen)
		{
			AddSize(nLen);
            memcpy(m_pDataCur, pString, nLen * sizeof(wchar_t));
            //memcpy(m_pDataCur, pString, nLen << 1);
			m_pDataCur += nLen;
			m_lSizeCur += nLen;
		}
		AVSINLINE size_t GetCurSize()
		{
			return m_lSizeCur;
		}
		AVSINLINE size_t GetSize()
		{
			return m_lSize;
		}
		AVSINLINE void Clear()
		{
			RELEASEMEM(m_pData);
			
			m_pData = NULL;
			m_lSize = 0;

			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}
		AVSINLINE void ClearNoAttack()
		{
			m_pDataCur	= m_pData;
			m_lSizeCur	= 0;
		}

		AVSINLINE size_t GetStringLen(wchar_t* pData)
		{
			wchar_t* s = pData;
			for (; *s != 0; ++s);
			return (size_t)(s - pData);
		}

		AVSINLINE CString GetCString()
		{
			CString str(m_pData, (int)m_lSizeCur);
			return str;
		}
		AVSINLINE wchar_t* GetBuffer()
		{
			return m_pData;
		}
	};

	class CStringWriter : public CTextItem
	{
	public:
		CStringWriter() : CTextItem()
		{
		}
		virtual ~CStringWriter()
		{
		}

	public:
		
#if defined(_WIN32) || defined (_WIN64)
		AVSINLINE void WriteString(_bstr_t& bsString)
		{
			size_t nLen = bsString.length();
			CTextItem::WriteString(bsString.GetBSTR(), nLen);
		}
#endif
		AVSINLINE void WriteString(CString& sString)
		{
			size_t nLen = (size_t)sString.GetLength();

			#ifdef _UNICODE
			CTextItem::WriteString(sString.GetBuffer(), nLen);
			#else
			CStringW str = (CStringW)sString;
			WriteString(str.GetBuffer(), nLen);
			#endif
		}
		AVSINLINE void WriteString(wchar_t* pString, const size_t& nLen)
		{
			CTextItem::WriteString(pString, nLen);
		}
		AVSINLINE void Write(CStringWriter& oWriter)
		{
			CTextItem::WriteString(oWriter.m_pData, oWriter.m_lSizeCur);
		}
	};
}
