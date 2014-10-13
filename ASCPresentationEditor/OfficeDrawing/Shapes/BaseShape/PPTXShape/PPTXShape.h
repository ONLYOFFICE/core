#pragma once
#include "./../BaseShape.h"

using namespace NSPresentationEditor;

namespace OOXMLShapes
{
	enum ShapeType : unsigned short
	{
		sptMin = 0,
		sptNotPrimitive = sptMin,
		sptCAccentBorderCallout1,
		sptCAccentBorderCallout2,
		sptCAccentBorderCallout3,
		sptCAccentCallout1,
		sptCAccentCallout2,
		sptCAccentCallout3,
		sptCActionButtonBackPrevious,
		sptCActionButtonBeginning,
		sptCActionButtonBlank,
		sptCActionButtonDocument,
		sptCActionButtonEnd,
		sptCActionButtonForwardNext,
		sptCActionButtonHelp,
		sptCActionButtonHome,
		sptCActionButtonInformation,
		sptCActionButtonMovie,
		sptCActionButtonReturn,
		sptCActionButtonSound,
		sptCArc,
		sptCBentArrow,
		sptCBentConnector2,
		sptCBentConnector3,
		sptCBentConnector4,
		sptCBentConnector5,
		sptCBentUpArrow,
		sptCBevel,
		sptCBlockArc,
		sptCBorderCallout1,
		sptCBorderCallout2,
		sptCBorderCallout3,
		sptCBracePair,
		sptCBracketPair,
		sptCCallout1,
		sptCCallout2,
		sptCCallout3,
		sptCCan,
		sptCChartPlus,
		sptCChartStar,
		sptCChartX,
		sptCChevron,
		sptCChord,
		sptCCircularArrow,
		sptCCloud,
		sptCCloudCallout,
		sptCCorner,
		sptCCornerTabs,
		sptCCube,
		sptCCurvedConnector2,
		sptCCurvedConnector3,
		sptCCurvedConnector4,
		sptCCurvedConnector5,
		sptCCurvedDownArrow,
		sptCCurvedLeftArrow,
		sptCCurvedRightArrow,
		sptCCurvedUpArrow,
		sptCDecagon,
		sptCDiagStripe,
		sptCDiamond,
		sptCDodecagon,
		sptCDonut,
		sptCDoubleWave,
		sptCDownArrow,
		sptCDownArrowCallout,
		sptCEllipse,
		sptCEllipseRibbon,
		sptCEllipseRibbon2,
		sptCFlowChartAlternateProcess,
		sptCFlowChartCollate,
		sptCFlowChartConnector,
		sptCFlowChartDecision,
		sptCFlowChartDelay,
		sptCFlowChartDisplay,
		sptCFlowChartDocument,
		sptCFlowChartExtract,
		sptCFlowChartInputOutput,
		sptCFlowChartInternalStorage,
		sptCFlowChartMagneticDisk,
		sptCFlowChartMagneticDrum,
		sptCFlowChartMagneticTape,
		sptCFlowChartManualInput,
		sptCFlowChartManualOperation,
		sptCFlowChartMerge,
		sptCFlowChartMultidocument,
		sptCFlowChartOfflineStorage,
		sptCFlowChartOffpageConnector,
		sptCFlowChartOnlineStorage,
		sptCFlowChartOr,
		sptCFlowChartPredefinedProcess,
		sptCFlowChartPreparation,
		sptCFlowChartProcess,
		sptCFlowChartPunchedCard,
		sptCFlowChartPunchedTape,
		sptCFlowChartSort,
		sptCFlowChartSummingJunction,
		sptCFlowChartTerminator,
		sptCFoldedCorner,
		sptCFrame,
		sptCFunnel,
		sptCGear6,
		sptCGear9,
		sptCHalfFrame,
		sptCHeart,
		sptCHeptagon,
		sptCHexagon,
		sptCHomePlate,
		sptCHorizontalScroll,
		sptCIrregularSeal1,
		sptCIrregularSeal2,
		sptCLeftArrow,
		sptCLeftArrowCallout,
		sptCLeftBrace,
		sptCLeftBracket,
		sptCLeftCircularArrow,
		sptCLeftRightArrow,
		sptCLeftRightArrowCallout,
		sptCLeftRightCircularArrow,
		sptCLeftRightRibbon,
		sptCLeftRightUpArrow,
		sptCLeftUpArrow,
		sptCLightningBolt,
		sptCLine,
		sptCLineInv,
		sptCMathDivide,
		sptCMathEqual,
		sptCMathMinus,
		sptCMathMultiply,
		sptCMathNotEqual,
		sptCMathPlus,
		sptCMoon,
		sptCNonIsoscelesTrapezoid,
		sptCNoSmoking,
		sptCNotchedRightArrow,
		sptCOctagon,
		sptCParallelogram,
		sptCPentagon,
		sptCPie,
		sptCPieWedge,
		sptCPlaque,
		sptCPlaqueTabs,
		sptCPlus,
		sptCQuadArrow,
		sptCQuadArrowCallout,
		sptCRect,
		sptCRibbon,
		sptCRibbon2,
		sptCRightArrow,
		sptCRightArrowCallout,
		sptCRightBrace,
		sptCRightBracket,
		sptCRound1Rect,
		sptCRound2DiagRect,
		sptCRound2SameRect,
		sptCRoundRect,
		sptCRtTriangle,
		sptCSmileyFace,
		sptCSnip1Rect,
		sptCSnip2DiagRect,
		sptCSnip2SameRect,
		sptCSnipRoundRect,
		sptCSquareTabs,
		sptCStar10,
		sptCStar12,
		sptCStar16,
		sptCStar24,
		sptCStar32,
		sptCStar4,
		sptCStar5,
		sptCStar6,
		sptCStar7,
		sptCStar8,
		sptCStraightConnector1,
		sptCStripedRightArrow,
		sptCSun,
		sptCSwooshArrow,
		sptCTeardrop,
		sptCTextArchDown,
		sptCTextArchDownPour,
		sptCTextArchUp,
		sptCTextArchUpPour,
		sptCTextButton,
		sptCTextButtonPour,
		sptCTextCanDown,
		sptCTextCanUp,
		sptCTextCascadeDown,
		sptCTextCascadeUp,
		sptCTextChevron,
		sptCTextChevronInverted,
		sptCTextCircle,
		sptCTextCirclePour,
		sptCTextCurveDown,
		sptCTextCurveUp,
		sptCTextDeflate,
		sptCTextDeflateBottom,
		sptCTextDeflateInflate,
		sptCTextDeflateInflateDeflate,
		sptCTextDeflateTop,
		sptCTextDoubleWave1,
		sptCTextFadeDown,
		sptCTextFadeLeft,
		sptCTextFadeRight,
		sptCTextFadeUp,
		sptCTextInflate,
		sptCTextInflateBottom,
		sptCTextInflateTop,
		sptCTextPlain,
		sptCTextRingInside,
		sptCTextRingOutside,
		sptCTextSlantDown,
		sptCTextSlantUp,
		sptCTextStop,
		sptCTextTriangle,
		sptCTextTriangleInverted,
		sptCTextWave1,
		sptCTextWave2,
		sptCTextWave4,
		sptCTrapezoid,
		sptCTriangle,
		sptCUpArrow,
		sptCUpArrowCallout,
		sptCUpDownArrow,
		sptCUpDownArrowCallout,
		sptCUturnArrow,
		sptCVerticalScroll,
		sptCWave,
		sptCWedgeEllipseCallout,
		sptCWedgeRectCallout,
		sptCWedgeRoundRectCallout,

		sptMax,
		sptNil = 0x0FFF,
		sptCustom = 0x1000
	};
}

class CPPTXShape : public CBaseShape
{
public:
	OOXMLShapes::ShapeType m_eType;

	NSGuidesOOXML::CFormulaManager FManager;
public:
	CPPTXShape() : CBaseShape(), FManager(m_arAdjustments, Guides)
	{
		m_eType = OOXMLShapes::sptMin;
	}

	~CPPTXShape()
	{
	}

	//virtual bool LoadFromXML(const CString& xml)
	//{
	//	//CString tempXML = _T("<ooxml-shape>") + xml + _T("</ooxml-shape>");
	//	XmlUtils::CXmlNode root;
	//	if(root.FromXmlString(xml))
	//	{
	//		XmlUtils::CXmlNode avLst;
	//		bool res = true;
	//		if(root.GetNode(_T("avLst"), avLst))
	//			res &= LoadAdjustValuesList(avLst.GetXml());

	//		XmlUtils::CXmlNode gdLst;
	//		if(root.GetNode(_T("gdLst"), gdLst))
	//			res &= LoadGuidesList(gdLst.GetXml());

	//		XmlUtils::CXmlNode ahLst;
	//		if(root.GetNode(_T("ahLst"), ahLst))
	//			res &= LoadAdjustHandlesList(ahLst.GetXml());

	//		XmlUtils::CXmlNode cxnLst;
	//		if(root.GetNode(_T("cxnLst"), cxnLst))
	//			res &= LoadConnectorsList(cxnLst.GetXml());

	//		XmlUtils::CXmlNode textRect;
	//		if(root.GetNode(_T("rect"), textRect))
	//		{
	//			m_strRect = textRect.GetXml();
	//			res &= LoadTextRect(m_strRect);
	//		}

	//		XmlUtils::CXmlNode pathLst;
	//		if(root.GetNode(_T("pathLst"), pathLst))
	//		{
	//			m_strPath = pathLst.GetXml();
	//			res &= LoadPathList(m_strPath);
	//		}

	//		return res;
	//	}
	//	return false;
	//}


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
		//CString tempXML = _T("<ooxml-shape>") + xml + _T("</ooxml-shape>");
		
		XmlUtils::CXmlNode avLst;
		bool res = true;
		if(root.GetNode(_T("avLst"), avLst))
			res &= LoadAdjustValuesList(avLst.GetXml());

		XmlUtils::CXmlNode gdLst;
		if(root.GetNode(_T("gdLst"), gdLst))
			res &= LoadGuidesList(gdLst.GetXml());

		XmlUtils::CXmlNode ahLst;
		if(root.GetNode(_T("ahLst"), ahLst))
			res &= LoadAdjustHandlesList(ahLst.GetXml());

		XmlUtils::CXmlNode cxnLst;
		if(root.GetNode(_T("cxnLst"), cxnLst))
			res &= LoadConnectorsList(cxnLst.GetXml());

		XmlUtils::CXmlNode textRect;
		if(root.GetNode(_T("rect"), textRect))
		{
			m_strRect = textRect.GetXml();
			res &= LoadTextRect(m_strRect);
		}

		XmlUtils::CXmlNode pathLst;
		if(root.GetNode(_T("pathLst"), pathLst))
		{
			m_strPath = pathLst.GetXml();
			res &= LoadPathList(m_strPath);
		}

		return res;
	}


	virtual bool LoadAdjustValuesList(const CString& xml)
	{
		XmlUtils::CXmlNode avLst;
		if(avLst.FromXmlString(xml))
		{
			XmlUtils::CXmlNodes list;
			if(avLst.GetNodes(_T("gd"), list))
			{
				for(long i = 0; i < list.GetCount(); i++)
				{
					XmlUtils::CXmlNode gd;
					list.GetAt(i, gd);
					FManager.AddAdjustment(gd.GetAttribute(_T("name")), gd.GetAttribute(_T("fmla")));
				}
			}
			return true;
		}
		return false;
	}

	virtual bool LoadGuidesList(const CString& xml)
	{
		XmlUtils::CXmlNode gdLst;
		if(gdLst.FromXmlString(xml))
		{
			XmlUtils::CXmlNodes list;
			if(gdLst.GetNodes(_T("gd"), list))
			{
				for(long i = 0; i < list.GetCount(); i++)
				{
					XmlUtils::CXmlNode gd;
					list.GetAt(i, gd);
					FManager.AddGuide(gd.GetAttribute(_T("name")), gd.GetAttribute(_T("fmla")));
				}
			}
			return true;
		}
		return false;
	}

	virtual bool LoadAdjustHandlesList(const CString& xml)
	{
		/*XmlUtils::CXmlNode ahLst;
		if(ahLst.FromXmlString(xml))
		{
			XmlUtils::CXmlNodes oNodes;
			if (ahLst.GetNodes(_T("ahXY"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeH;
					oNodes.GetAt(i, oNodeH);

					CHandle_ oH;
					oH.switchHandle = oNodeH.GetAttribute(_T("switch"));
					oH.xrange = oNodeH.GetAttribute(_T("minX")) + _T(" ") + oNodeH.GetAttribute(_T("maxX"));
					oH.yrange = oNodeH.GetAttribute(_T("minY")) + _T(" ") + oNodeH.GetAttribute(_T("maxY"));

					XmlUtils::CXmlNodes oPosNodes;
					if (oNodeH.GetNodes(_T("pos"), oPosNodes))
					{
						int nCountPos = oPosNodes.GetCount();
						XmlUtils::CXmlNode oPosNode;
						oPosNodes.GetAt(0, oPosNode);

						CString strX = oPosNode.GetAttribute(_T("x"));
						CString strY = oPosNode.GetAttribute(_T("y"));

						if (strX == _T("l"))
							oH.position += _T("topLeft");
						else if (strX == _T("r"))
							oH.position += _T("bottomRight");
						else 
							oH.position += oNodeH.GetAttribute(_T("gdRefX"));

						if (strY == _T("t"))
							oH.position += _T("topLeft");
						else if (strY == _T("b"))
							oH.position += _T("bottomRight");
						else 
							oH.position += _T(" ") + oNodeH.GetAttribute(_T("gdRefY"));
					}
					//TODO разобратьс€ с пол€рными хендлами
					//пол€:
					//polar = 10800, 10800
					//position = gdrefR, gdrefAng (adj)
					//radiusrange = minR, maxR

					m_arHandles.push_back(oH);
				}
			}
			return true;
		}
		return false;*/
		return true;
	}

	virtual bool LoadConnectorsList(const CString& xml)
	{
		return true;
	}

	virtual bool LoadTextRect(const CString& xml)
	{
		XmlUtils::CXmlNode rect;
		if(rect.FromXmlString(xml))
		{
			RECT TextRect;
			TextRect.top = (long)FManager.GetValue(rect.GetAttribute(_T("t")));
			TextRect.left = (long)FManager.GetValue(rect.GetAttribute(_T("l")));
			TextRect.right = (long)FManager.GetValue(rect.GetAttribute(_T("r")));
			TextRect.bottom = (long)FManager.GetValue(rect.GetAttribute(_T("b")));
			if(m_arTextRects.size() > 0)
				m_arTextRects[0] = TextRect;
			else m_arTextRects.push_back(TextRect);
			return true;
		}
		return false;
	}

	virtual bool LoadPathList(const CString& xml)
	{
		XmlUtils::CXmlNode pathLst;
		if(pathLst.FromXmlString(xml))
		{
			XmlUtils::CXmlNodes list;
			if(pathLst.GetNodes(_T("path"), list))
			{
				m_oPath.FromXML(list, FManager);
			}
			return true;
		}
		return false;
	}

	virtual bool SetAdjustment(long index, long value)
	{
		FManager.Clear();
		return (TRUE == m_arAdjustments.SetAtIndex(index, value));
	}

	virtual CString ToXML(CGeomShapeInfo& GeomInfo, CMetricInfo& MetricInfo, double StartTime, double EndTime, CBrush& Brush, CPen& Pen)
	{
		FManager.SetWidthHeight(GeomInfo.m_dWidth, GeomInfo.m_dHeight);
		ReCalculate();

		return m_oPath.ToXml(GeomInfo, StartTime, EndTime, Pen, Brush, MetricInfo, NSBaseShape::pptx);
	}
	virtual void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& pGeomInfo, double dStartTime, double dEndTime, CPen& pPen, CBrush& pFore, CMetricInfo& pInfo)
	{
		FManager.SetWidthHeight(pGeomInfo.m_dWidth, pGeomInfo.m_dHeight);
		ReCalculate();

		m_oPath.ToRenderer(pRenderer, pGeomInfo, dStartTime, dEndTime, pPen, pFore, pInfo, GetClassType());
	}

	virtual void ReCalculate()
	{
		FManager.ReCalculateGuides();
		LoadTextRect(m_strRect);
		LoadPathList(m_strPath);
	}

	static CPPTXShape* CreateByType(OOXMLShapes::ShapeType type);
	virtual const ClassType GetClassType()const
	{
		return NSBaseShape::pptx;
	}

	virtual bool SetProperties(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSBaseShape::pptx)
			return false;

		FManager = ((CPPTXShape*)Shape)->FManager;
		return CBaseShape::SetProperties(Shape);
	}

	virtual bool SetToDublicate(CBaseShape* Shape)
	{
		if(Shape == NULL)
			return false;

		if(Shape->GetClassType() != NSBaseShape::pptx)
			return false;

		((CPPTXShape*)Shape)->FManager = FManager;
		return CBaseShape::SetToDublicate(Shape);
	}

	bool SetShapeType(OOXMLShapes::ShapeType type)
	{
		CPPTXShape* l_pShape = CreateByType(type);
		if(l_pShape != NULL)
		{
			m_eType = type;

			SetProperties(l_pShape);
			delete l_pShape;
			return true;
		}

		m_eType = OOXMLShapes::sptCustom;
		return false;
	}

	virtual void SetWidthHeightLogic(const double& dWidth, const double& dHeight)
	{
		FManager.SetWidthHeight(dWidth, dHeight);
	}
	virtual void GetWidthHeightLogic(double& dWidth, double& dHeight)
	{
		dWidth = FManager.GetWidth();
		dHeight = FManager.GetHeight();
	}
};