#pragma once
//#include "stdafx.h"
#include "Formula.h"
#include <string>
#include "./../BaseShape.h"
#include "./../../../../../Common/DocxFormat/Source/XML/XmlUtils.h"

namespace OdpShapes
{
	enum ShapeType : unsigned short
	{
		sptMin = 0,
		sptNotPrimitive = sptMin,
		sptCRect = 1,
		sptCLine = 2,
/*
		sptCRoundRect = 2,
		sptCEllipse = 3,
		sptCDiamond = 4,
		sptCIsocelesTriangle = 5,
		sptCRtTriangle = 6,
		sptCParallelogram = 7,
		sptCTrapezoid = 8,
		sptCHexagon = 9,
		sptCOctagon = 10,
		sptCPlus = 11,
		sptCStar5 = 12,
		sptCRightArrow = 13,
		sptCThickArrow = 14,
		sptCHomePlate = 15,
		sptCCube = 16,
		sptCBalloon = 17,
		sptCSeal = 18,
		sptCArc = 19,
		sptCLine = 20,
		sptCPlaque = 21,
		sptCCan = 22,
		sptCDonut = 23,
		sptCTextSimple = 24,
		sptCTextOctagon = 25,
		sptCTextHexagon = 26,
		sptCTextCurve = 27,
		sptCTextWave = 28,
		sptCTextRing = 29,
		sptCTextOnCurve = 30,
		sptCTextOnRing = 31,
		sptCStraightConnector1 = 32,
		sptCBentConnector2 = 33,
		sptCBentConnector3 = 34,
		sptCBentConnector4 = 35,
		sptCBentConnector5 = 36,
		sptCCurvedConnector2 = 37,
		sptCCurvedConnector3 = 38,
		sptCCurvedConnector4 = 39,
		sptCCurvedConnector5 = 40,
		sptCCallout1 = 41,
		sptCCallout2 = 42,
		sptCCallout3 = 43,
		sptCAccentCallout1 = 44,
		sptCAccentCallout2 = 45,
		sptCAccentCallout3 = 46,
		sptCBorderCallout1 = 47,
		sptCBorderCallout2 = 48,
		sptCBorderCallout3 = 49,
		sptCAccentBorderCallout1 = 50,
		sptCAccentBorderCallout2 = 51,
		sptCAccentBorderCallout3 = 52,
		sptCRibbon = 53,
		sptCRibbon2 = 54,
		sptCChevron = 55,
		sptCPentagon = 56,
		sptCNoSmoking = 57,
		sptCStar8 = 58,
		sptCStar16 = 59,
		sptCStar32 = 60,
		sptCWedgeRectCallout = 61,
		sptCWedgeRoundRectCallout = 62,
		sptCWedgeEllipseCallout = 63,
		sptCWave = 64,
		sptCFoldedCorner = 65,
		sptCLeftArrow = 66,
		sptCDownArrow = 67,
		sptCUpArrow = 68,
		sptCLeftRightArrow = 69,
		sptCUpDownArrow = 70,
		sptCIrregularSeal1 = 71,
		sptCIrregularSeal2 = 72,
		sptCLightningBolt = 73,
		sptCHeart = 74,
		sptCFrame = 75,
		sptCQuadArrow = 76,
		sptCLeftArrowCallout = 77,
		sptCRightArrowCallout = 78,
		sptCUpArrowCallout = 79,
		sptCDownArrowCallout = 80,
		sptCLeftRightArrowCallout = 81,
		sptCUpDownArrowCallout = 82,
		sptCQuadArrowCallout = 83,
		sptCBevel = 84,
		sptCLeftBracket = 85,
		sptCRightBracket = 86,
		sptCLeftBrace = 87,
		sptCRightBrace = 88,
		sptCLeftUpArrow = 89,
		sptCBentUpArrow = 90,
		sptCBentArrow = 91,
		sptCStar24 = 92,
		sptCStripedRightArrow = 93,
		sptCNotchedRightArrow = 94,
		sptCBlockArc = 95,
		sptCSmileyFace = 96,
		sptCVerticalScroll = 97,
		sptCHorizontalScroll = 98,
		sptCCircularArrow = 99,
		sptCNotchedCircularArrow = 100,
		sptCUturnArrow = 101,
		sptCCurvedRightArrow = 102,
		sptCCurvedLeftArrow = 103,
		sptCCurvedUpArrow = 104,
		sptCCurvedDownArrow = 105,
		sptCCloudCallout = 106,
		sptCEllipseRibbon = 107,
		sptCEllipseRibbon2 = 108,
		sptCFlowChartProcess = 109,
		sptCFlowChartDecision = 110,
		sptCFlowChartInputOutput = 111,
		sptCFlowChartPredefinedProcess = 112,
		sptCFlowChartInternalStorage = 113,
		sptCFlowChartDocument = 114,
		sptCFlowChartMultidocument = 115,
		sptCFlowChartTerminator = 116,
		sptCFlowChartPreparation = 117,
		sptCFlowChartManualInput = 118,
		sptCFlowChartManualOperation = 119,
		sptCFlowChartConnector = 120,
		sptCFlowChartPunchedCard = 121,
		sptCFlowChartPunchedTape = 122,
		sptCFlowChartSummingJunction = 123,
		sptCFlowChartOr = 124,
		sptCFlowChartCollate = 125,
		sptCFlowChartSort = 126,
		sptCFlowChartExtract = 127,
		sptCFlowChartMerge = 128,
		sptCFlowChartOfflineStorage = 129,
		sptCFlowChartOnlineStorage = 130,
		sptCFlowChartMagneticTape = 131,
		sptCFlowChartMagneticDisk = 132,
		sptCFlowChartMagneticDrum = 133,
		sptCFlowChartDisplay = 134,
		sptCFlowChartDelay = 135,
		sptCTextPlain = 136,
		sptCTextStop = 137,
		sptCTextTriangle = 138,
		sptCTextTriangleInverted = 139,
		sptCTextChevron = 140,
		sptCTextChevronInverted = 141,
		sptCTextRingInside = 142,
		sptCTextRingOutside = 143,
		sptCTextArchUp = 144,
		sptCTextArchDown = 145,
		sptCTextCircle = 146,
		sptCTextButton = 147,
		sptCTextArchUpPour = 148,
		sptCTextArchDownPour = 149,
		sptCTextCirclePour = 150,
		sptCTextButtonPour = 151,
		sptCTextCurveUp = 152,
		sptCTextCurveDown = 153,
		sptCTextCascadeUp = 154,
		sptCTextCascadeDown = 155,
		sptCTextWave1 = 156,
		sptCTextWave2 = 157,
		sptCTextWave3 = 158,
		sptCTextWave4 = 159,
		sptCTextInflate = 160,
		sptCTextDeflate = 161,
		sptCTextInflateBottom = 162,
		sptCTextDeflateBottom = 163,
		sptCTextInflateTop = 164,
		sptCTextDeflateTop = 165,
		sptCTextDeflateInflate = 166,
		sptCTextDeflateInflateDeflate = 167,
		sptCTextFadeRight = 168,
		sptCTextFadeLeft = 169,
		sptCTextFadeUp = 170,
		sptCTextFadeDown = 171,
		sptCTextSlantUp = 172,
		sptCTextSlantDown = 173,
		sptCTextCanUp = 174,
		sptCTextCanDown = 175,
		sptCFlowChartAlternateProcess = 176,
		sptCFlowChartOffpageConnector = 177,
		sptCCallout90 = 178,
		sptCAccentCallout90 = 179,
		sptCBorderCallout90 = 180,
		sptCAccentBorderCallout90 = 181,
		sptCLeftRightUpArrow = 182,
		sptCSun = 183,
		sptCMoon = 184,
		sptCBracketPair = 185,
		sptCBracePair = 186,
		sptCStar4 = 187,
		sptCDoubleWave = 188,
		sptCActionButtonBlank = 189,
		sptCActionButtonHome = 190,
		sptCActionButtonHelp = 191,
		sptCActionButtonInformation = 192,
		sptCActionButtonForwardNext = 193,
		sptCActionButtonBackPrevious = 194,
		sptCActionButtonEnd = 195,
		sptCActionButtonBeginning = 196,
		sptCActionButtonReturn = 197,
		sptCActionButtonDocument = 198,
		sptCActionButtonSound = 199,
		sptCActionButtonMovie = 200,
		sptCHostControl = 201,
		sptCTextBox = 202,
*/
		sptMax,
		sptNil = 0x0FFF,
		sptCustom = 0x1000
	};
}

class COdpShape : public NSPresentationEditor::CBaseShape
{
public:
	OdpShapes::ShapeType m_eType;

	NSGuidesOdp::CFormulaManager FManager;
public:
	COdpShape() : CBaseShape(), FManager(m_arAdjustments, Guides)
	{
		m_eType = OdpShapes::sptMin;
	}

	~COdpShape()
	{
	}

//	virtual bool LoadFromXML(const CString& xml)
//	{
//		XmlUtils::CXmlNode root;
//		if(root.FromXmlString(xml))
//		{
//			bool res = true;
//			res &= LoadAdjustValuesList(root.GetAttributeOrValue(_T("draw:modifiers")));
//
//			XmlUtils::CXmlNodes gdLst;
//			if(root.GetNodes(_T("draw:equation"), gdLst))
//			{
//				for(long i = 0; i < gdLst.GetCount(); i++)
//				{
//					XmlUtils::CXmlNode gd;
//					gdLst.GetAt(i, gd);
//					FManager.AddGuide(_T("?") + gd.GetAttribute(_T("draw:name")), gd.GetAttribute(_T("draw:formula")));
//				}
//
//			}
////				res &= LoadGuidesList(gdLst.GetXml());
///*
//			XmlUtils::CXmlNode ahLst;
//			if(root.GetNode(_T("ahLst"), ahLst))
//				res &= LoadAdjustHandlesList(ahLst.GetXml());
//
//			XmlUtils::CXmlNode cxnLst;
//			if(root.GetNode(_T("cxnLst"), cxnLst))
//				res &= LoadConnectorsList(cxnLst.GetXml());
//*/
//			m_strRect = root.GetAttributeOrValue(_T("draw:text-areas"));
//			res &= LoadTextRect(m_strRect);
//
//			m_strPath = root.GetAttributeOrValue(_T("draw:enhanced-path"));
//			res &= LoadPathList(m_strPath);
//
//			CString viewBox = root.GetAttributeOrValue(_T("svg:viewBox"));
//
//			if(viewBox != _T(""))
//			{
//				CSimpleArray<CString> borders;
////				CString left, top, right, bottom;
//				NSStringUtils::ParseString(_T(" "), viewBox, &borders);
//
////				left = borders[0];
////				top = borders[1];
////				right = borders[2];
////				bottom = borders[3];
//
//				FManager.AddGuide(_T("left"), borders[0]);
//				FManager.AddGuide(_T("top"), borders[1]);
//				FManager.AddGuide(_T("right"), borders[2]);
//				FManager.AddGuide(_T("bottom"), borders[3]);
//
//				FManager.Clear();
//				for(int i = 0; i < m_oPath.m_arParts.GetSize(); i++)
//				{
//					m_oPath.m_arParts[i].height = (long)FManager.GetValue(_T("height"));
//					m_oPath.m_arParts[i].width = (long)FManager.GetValue(_T("width"));
//				}
//
//	//			return true;
//			}
//
//
//			return res;
//		}
//		return false;
//	}


	virtual bool LoadFromXML(const CString& xml)
	{
		XmlUtils::CXmlNode oNodePict;
		if (oNodePict.FromXmlString(xml))
		{
			return LoadFromXML(oNodePict);
		}
		return false;
	}


	virtual bool LoadFromXML(XmlUtils::CXmlNode& root)
	{
		bool res = true;
		res &= LoadAdjustValuesList(root.GetAttributeOrValue(_T("draw:modifiers")));

		XmlUtils::CXmlNodes gdLst;
		if(root.GetNodes(_T("draw:equation"), gdLst))
		{
			int nCountG = gdLst.GetCount();
			for(long i = 0; i < nCountG; i++)
			{
				XmlUtils::CXmlNode gd;
				gdLst.GetAt(i, gd);

				CString str1 = gd.GetAttributeNoSpace(_T("draw:name"));
				CString str2 = gd.GetAttributeNoSpace(_T("draw:formula"));

				FManager.AddGuide(_T("?") + str1, str2);
			}

		}

		NSPresentationEditor::CHandle_ oHandle;
		XmlUtils::CXmlNodes hndLst;
		if(root.GetNodes(_T("draw:handle"), hndLst))
		{
			int nCountH = hndLst.GetCount();
			for(long i = 0; i < nCountH; i++)
			{
				XmlUtils::CXmlNode hnd;
				hndLst.GetAt(i, hnd);

				oHandle.position = hnd.GetAttributeNoSpace(_T("draw:handle-position"));
				oHandle.polar = hnd.GetAttributeNoSpace(_T("draw:handle-polar"));
				oHandle.radiusrange = hnd.GetAttributeNoSpace(_T("draw:handle-radius-range-minimum")) + _T(" ") + hnd.GetAttributeNoSpace(_T("draw:handle-radius-range-maximum"));
				oHandle.switchHandle = hnd.GetAttributeNoSpace(_T("draw:handle-switched"));
				oHandle.xrange = hnd.GetAttributeNoSpace(_T("draw:handle-range-x-minimum")) + _T(" ") + hnd.GetAttributeNoSpace(_T("draw:handle-range-x-maximum"));
				oHandle.yrange = hnd.GetAttributeNoSpace(_T("draw:handle-range-y-minimum")) + _T(" ") + hnd.GetAttributeNoSpace(_T("draw:handle-range-y-maximum"));

				m_arHandles.push_back(oHandle);
			}

		}
//				res &= LoadGuidesList(gdLst.GetXml());
/*
		XmlUtils::CXmlNode ahLst;
		if(root.GetNode(_T("ahLst"), ahLst))
			res &= LoadAdjustHandlesList(ahLst.GetXml());

		XmlUtils::CXmlNode cxnLst;
		if(root.GetNode(_T("cxnLst"), cxnLst))
			res &= LoadConnectorsList(cxnLst.GetXml());
*/
		m_strRect = root.GetAttributeOrValue(_T("draw:text-areas"));
		res &= LoadTextRect(m_strRect);

		m_strPath = root.GetAttributeOrValue(_T("draw:enhanced-path"));
		res &= LoadPathList(m_strPath);

		CString viewBox = root.GetAttributeOrValue(_T("svg:viewBox"));

		if(viewBox != _T(""))
		{
			CSimpleArray<CString> borders;
//				CString left, top, right, bottom;
			NSStringUtils::ParseString(_T(" "), viewBox, &borders);

//				left = borders[0];
//				top = borders[1];
//				right = borders[2];
//				bottom = borders[3];

			FManager.AddGuide(_T("left"), borders[0]);
			FManager.AddGuide(_T("top"), borders[1]);
			FManager.AddGuide(_T("right"), borders[2]);
			FManager.AddGuide(_T("bottom"), borders[3]);

			FManager.Clear();
			for(int i = 0; i < m_oPath.m_arParts.GetSize(); i++)
			{
				m_oPath.m_arParts[i].height = (long)FManager.GetValue(_T("height"));
				m_oPath.m_arParts[i].width = (long)FManager.GetValue(_T("width"));
			}

//			return true;
		}


		return res;
	}

	virtual bool LoadAdjustValuesList(const CString& xml)
	{
		if(xml != _T(""))
		{
			CSimpleArray<CString> adjusts;
			NSStringUtils::ParseString(_T(" "), xml, &adjusts);
			m_arAdjustments.RemoveAll();
			CString buffer;
			for(int i = 0; i < adjusts.GetSize(); i++)
			{
				buffer.Format(_T("%i"), i);
				FManager.AddAdjustment(CString(_T("$")) + buffer, XmlUtils::GetInteger(adjusts[i]));
			}
			return true;
		}
		return false;
	}

	virtual bool LoadGuidesList(const CString& xml)
	{
		return true;
	}

	virtual bool LoadAdjustHandlesList(const CString& xml)
	{
		return true;
	}

	virtual bool LoadConnectorsList(const CString& xml)
	{
		return true;
	}

	virtual bool LoadTextRect(const CString& xml)
	{
		if(xml != _T(""))
		{
			CSimpleArray<CString> borders;
			NSStringUtils::ParseString(_T(" "), xml, &borders);

			RECT TextRect;
			TextRect.left = (long)FManager.GetValue(borders[0]);
			TextRect.top = (long)FManager.GetValue(borders[1]);
			TextRect.right = (long)FManager.GetValue(borders[2]);
			TextRect.bottom = (long)FManager.GetValue(borders[3]);
			if(m_arTextRects.GetSize() > 0)
				m_arTextRects[0] = TextRect;
			else m_arTextRects.Add(TextRect);

//			return true;
		}
		return true;
	}

	virtual bool LoadPathList(const CString& xml)
	{
		if(xml != _T(""))
		{
			m_oPath.FromXML(xml, FManager);

			for(int i = 0; i < m_oPath.m_arParts.GetSize(); i++)
			{
				m_oPath.m_arParts[i].height = (long)FManager.GetValue(_T("height"));
				m_oPath.m_arParts[i].width = (long)FManager.GetValue(_T("width"));
			}

			return true;
		}
		return false;
	}

	virtual bool SetAdjustment(long index, long value)
	{
		FManager.Clear();
		//return (TRUE == m_arAdjustments.SetAtIndex(index, value));
		CString buffer;
		buffer.Format(_T("%i"), index);
		FManager.AddAdjustment(_T("$") + buffer, value);
		return true;
	}

	virtual CString ToXML(NSPresentationEditor::CGeomShapeInfo& GeomInfo, NSPresentationEditor::CMetricInfo& MetricInfo, double StartTime, double EndTime, NSPresentationEditor::CBrush& Brush, NSPresentationEditor::CPen& Pen)
	{
//		FManager.SetWidthHeight((long)GeomInfo.m_dWidth, (long)GeomInfo.m_dHeight);
		ReCalculate();

		return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, MetricInfo, NSPresentationEditor::NSBaseShape::odp);
	}

	virtual void ReCalculate()
	{
		FManager.ReCalculateGuides();
		LoadTextRect(m_strRect);
		LoadPathList(m_strPath);
	}

	static COdpShape* CreateByType(OdpShapes::ShapeType type);
	virtual const NSPresentationEditor::NSBaseShape::ClassType GetClassType()const
	{
		return NSPresentationEditor::NSBaseShape::odp;
	}

	virtual bool SetProperties(NSPresentationEditor::CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSPresentationEditor::NSBaseShape::odp)
			return false;

		FManager = ((COdpShape*)Shape)->FManager;
		return NSPresentationEditor::CBaseShape::SetProperties(Shape);
	}

	virtual bool SetToDublicate(NSPresentationEditor::CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSPresentationEditor::NSBaseShape::odp)
			return false;

		((COdpShape*)Shape)->FManager = FManager;
		return NSPresentationEditor::CBaseShape::SetToDublicate(Shape);
	}

	bool SetShapeType(OdpShapes::ShapeType type)
	{
		COdpShape* l_pShape = CreateByType(type);
		if(l_pShape != NULL)
		{
			m_eType = type;

			SetProperties(l_pShape);
			delete l_pShape;
			return true;
		}

		m_eType = OdpShapes::sptCustom;
		return false;
	}
};