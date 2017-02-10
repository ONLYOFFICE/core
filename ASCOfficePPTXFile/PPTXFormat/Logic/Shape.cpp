/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


#include "Shape.h"
#include "../SlideLayout.h"
#include "../SlideMaster.h"
#include "../Slide.h"
#include "SpTree.h"

#include "../../../ASCOfficeOdfFileW/source/Oox2OdfConverter/VmlShapeTypes2Oox.h"


namespace PPTX
{
	SimpleTypes::Vml::SptType static Spt2ShapeType(SimpleTypes::EShapeType type) 
	{
		switch(type)
		{
			case	SimpleTypes::shapetypeRect	:			return	SimpleTypes::Vml::sptRectangle	;
			case	SimpleTypes::shapetypeRoundRect	:		return	SimpleTypes::Vml::sptRoundRectangle	;
			case	SimpleTypes::shapetypeEllipse	:		return	SimpleTypes::Vml::sptEllipse	;
			case	SimpleTypes::shapetypeDiamond	:		return	SimpleTypes::Vml::sptDiamond	;
			case	SimpleTypes::shapetypeTriangle	:		return	SimpleTypes::Vml::sptIsocelesTriangle	;
			case	SimpleTypes::shapetypeRtTriangle	:	return	SimpleTypes::Vml::sptRightTriangle	;
			case	SimpleTypes::shapetypeParallelogram	:	return	SimpleTypes::Vml::sptParallelogram	;
			case	SimpleTypes::shapetypeTrapezoid	:		return	SimpleTypes::Vml::sptTrapezoid	;
			case	SimpleTypes::shapetypeHexagon	:		return	SimpleTypes::Vml::sptHexagon	;
			case	SimpleTypes::shapetypeOctagon	:		return	SimpleTypes::Vml::sptOctagon	;
			case	SimpleTypes::shapetypePlus	:			return	SimpleTypes::Vml::sptPlus	;
			case	SimpleTypes::shapetypeStar5	:			return	SimpleTypes::Vml::sptStar	;
			case	SimpleTypes::shapetypeRightArrow:		return	SimpleTypes::Vml::sptArrow	;
			case	SimpleTypes::shapetypeThickArrow:		return	SimpleTypes::Vml::sptThickArrow	;
			case	SimpleTypes::shapetypeHomePlate	:		return	SimpleTypes::Vml::sptHomePlate	;
			case	SimpleTypes::shapetypeCube	:			return	SimpleTypes::Vml::sptCube	;
			case	SimpleTypes::shapetypeBallon	:		return	SimpleTypes::Vml::sptBalloon	;
//			case	SimpleTypes::shapetypeIrregularSeal1:	return	SimpleTypes::Vml::sptSeal	;
			case	SimpleTypes::shapetypeArc	:			return	SimpleTypes::Vml::sptArc	;
			case	SimpleTypes::shapetypeLine	:			return	SimpleTypes::Vml::sptLine	;
			case	SimpleTypes::shapetypePlaque	:		return	SimpleTypes::Vml::sptPlaque	;
			case	SimpleTypes::shapetypeCan	:			return	SimpleTypes::Vml::sptCan	;
			case	SimpleTypes::shapetypeDonut	:			return	SimpleTypes::Vml::sptDonut	;
//			case	SimpleTypes::shapetypeTextSimple	:	return	SimpleTypes::Vml::sptTextSimple	;
//			case	SimpleTypes::shapetypeTextOctagon	:	return	SimpleTypes::Vml::sptTextOctagon	;
//			case	SimpleTypes::shapetypeTextHexagon	:	return	SimpleTypes::Vml::sptTextHexagon	;
//			case	SimpleTypes::shapetypeTextCurve	:		return	SimpleTypes::Vml::sptTextCurve	;
//			case	SimpleTypes::shapetypeTextWave	:		return	SimpleTypes::Vml::sptTextWave	;
//			case	SimpleTypes::shapetypeTextRing	:		return	SimpleTypes::Vml::sptTextRing	;
//			case	SimpleTypes::shapetypeTextOnCurve	:	return	SimpleTypes::Vml::sptTextOnCurve	;
//			case	SimpleTypes::shapetypeTextOnRing	:	return	SimpleTypes::Vml::sptTextOnRing	;
			case	SimpleTypes::shapetypeStraightConnector1:	return	SimpleTypes::Vml::sptStraightConnector1	;
			case	SimpleTypes::shapetypeBentConnector2	:	return	SimpleTypes::Vml::sptBentConnector2	;
			case	SimpleTypes::shapetypeBentConnector3	:	return	SimpleTypes::Vml::sptBentConnector3	;
			case	SimpleTypes::shapetypeBentConnector4	:	return	SimpleTypes::Vml::sptBentConnector4	;
			case	SimpleTypes::shapetypeBentConnector5	:	return	SimpleTypes::Vml::sptBentConnector5	;
			case	SimpleTypes::shapetypeCurvedConnector2	:	return	SimpleTypes::Vml::sptCurvedConnector2	;
			case	SimpleTypes::shapetypeCurvedConnector3	:	return	SimpleTypes::Vml::sptCurvedConnector3	;
			case	SimpleTypes::shapetypeCurvedConnector4	:	return	SimpleTypes::Vml::sptCurvedConnector4	;
			case	SimpleTypes::shapetypeCurvedConnector5	:	return	SimpleTypes::Vml::sptCurvedConnector5	;
			case	SimpleTypes::shapetypeCallout1	:			return	SimpleTypes::Vml::sptCallout1	;
			case	SimpleTypes::shapetypeCallout2	:			return	SimpleTypes::Vml::sptCallout2	;
			case	SimpleTypes::shapetypeCallout3	:			return	SimpleTypes::Vml::sptCallout3	;
			case	SimpleTypes::shapetypeAccentCallout1	:	return	SimpleTypes::Vml::sptAccentCallout1	;
			case	SimpleTypes::shapetypeAccentCallout2	:	return	SimpleTypes::Vml::sptAccentCallout2	;
			case	SimpleTypes::shapetypeAccentCallout3	:	return	SimpleTypes::Vml::sptAccentCallout3	;
			case	SimpleTypes::shapetypeAccentBorderCallout1	:	return	SimpleTypes::Vml::sptAccentBorderCallout1	;
			case	SimpleTypes::shapetypeAccentBorderCallout2	:	return	SimpleTypes::Vml::sptAccentBorderCallout2	;
			case	SimpleTypes::shapetypeAccentBorderCallout3	:	return	SimpleTypes::Vml::sptAccentBorderCallout3	;
			case	SimpleTypes::shapetypeRibbon	:				return	SimpleTypes::Vml::sptRibbon	;
			case	SimpleTypes::shapetypeRibbon2	:				return	SimpleTypes::Vml::sptRibbon2	;
			case	SimpleTypes::shapetypeChevron	:				return	SimpleTypes::Vml::sptChevron	;
			case	SimpleTypes::shapetypePentagon	:				return	SimpleTypes::Vml::sptPentagon	;
			case	SimpleTypes::shapetypeNoSmoking	:				return	SimpleTypes::Vml::sptNoSmoking	;
			case	SimpleTypes::shapetypeStar8	:					return	SimpleTypes::Vml::sptSeal8	;
			case	SimpleTypes::shapetypeStar16	:				return	SimpleTypes::Vml::sptSeal16	;
			case	SimpleTypes::shapetypeStar32	:				return	SimpleTypes::Vml::sptSeal32	;
			case	SimpleTypes::shapetypeWedgeRectCallout	:		return	SimpleTypes::Vml::sptWedgeRectCallout	;
			case	SimpleTypes::shapetypeWedgeRoundRectCallout	:	return	SimpleTypes::Vml::sptWedgeRRectCallout	;
			case	SimpleTypes::shapetypeWedgeEllipseCallout	:	return	SimpleTypes::Vml::sptWedgeEllipseCallout	;
			case	SimpleTypes::shapetypeWave	:					return	SimpleTypes::Vml::sptWave	;
			case	SimpleTypes::shapetypeFoldedCorner	:			return	SimpleTypes::Vml::sptFoldedCorner	;
			case	SimpleTypes::shapetypeLeftArrow	:				return	SimpleTypes::Vml::sptLeftArrow	;
			case	SimpleTypes::shapetypeDownArrow	:				return	SimpleTypes::Vml::sptDownArrow	;
			case	SimpleTypes::shapetypeUpArrow	:				return	SimpleTypes::Vml::sptUpArrow	;
			case	SimpleTypes::shapetypeLeftRightArrow	:		return	SimpleTypes::Vml::sptLeftRightArrow	;
			case	SimpleTypes::shapetypeUpDownArrow	:			return	SimpleTypes::Vml::sptUpDownArrow	;
			case	SimpleTypes::shapetypeIrregularSeal1	:		return	SimpleTypes::Vml::sptIrregularSeal1	;
			case	SimpleTypes::shapetypeIrregularSeal2	:		return	SimpleTypes::Vml::sptIrregularSeal2	;
			case	SimpleTypes::shapetypeLightningBolt	:			return	SimpleTypes::Vml::sptLightningBolt	;
			case	SimpleTypes::shapetypeHeart	:					return	SimpleTypes::Vml::sptHeart	;
			case	SimpleTypes::shapetypeQuadArrow	:				return	SimpleTypes::Vml::sptQuadArrow	;
			case	SimpleTypes::shapetypeLeftArrowCallout	:		return	SimpleTypes::Vml::sptLeftArrowCallout	;
			case	SimpleTypes::shapetypeRightArrowCallout	:		return	SimpleTypes::Vml::sptRightArrowCallout	;
			case	SimpleTypes::shapetypeUpArrowCallout	:		return	SimpleTypes::Vml::sptUpArrowCallout	;
			case	SimpleTypes::shapetypeDownArrowCallout	:		return	SimpleTypes::Vml::sptDownArrowCallout	;
			case	SimpleTypes::shapetypeLeftRightArrowCallout	:	return	SimpleTypes::Vml::sptLeftRightArrowCallout	;
			case	SimpleTypes::shapetypeUpDownArrowCallout	:	return	SimpleTypes::Vml::sptUpDownArrowCallout	;
			case	SimpleTypes::shapetypeQuadArrowCallout	:		return	SimpleTypes::Vml::sptQuadArrowCallout	;
			case	SimpleTypes::shapetypeBevel	:					return	SimpleTypes::Vml::sptBevel	;
			case	SimpleTypes::shapetypeLeftBracket	:			return	SimpleTypes::Vml::sptLeftBracket	;
			case	SimpleTypes::shapetypeRightBracket	:			return	SimpleTypes::Vml::sptRightBracket	;
			case	SimpleTypes::shapetypeLeftBrace	:				return	SimpleTypes::Vml::sptLeftBrace	;
			case	SimpleTypes::shapetypeRightBrace	:			return	SimpleTypes::Vml::sptRightBrace	;
			case	SimpleTypes::shapetypeLeftUpArrow	:			return	SimpleTypes::Vml::sptLeftUpArrow	;
			case	SimpleTypes::shapetypeBentUpArrow	:			return	SimpleTypes::Vml::sptBentUpArrow	;
			case	SimpleTypes::shapetypeBentArrow	:				return	SimpleTypes::Vml::sptBentArrow	;
			case	SimpleTypes::shapetypeStar24	:				return	SimpleTypes::Vml::sptSeal24	;
			case	SimpleTypes::shapetypeStripedRightArrow	:		return	SimpleTypes::Vml::sptStripedRightArrow	;
			case	SimpleTypes::shapetypeNotchedRightArrow	:		return	SimpleTypes::Vml::sptNotchedRightArrow	;
			case	SimpleTypes::shapetypeBlockArc	:				return	SimpleTypes::Vml::sptBlockArc	;
			case	SimpleTypes::shapetypeSmileyFace	:			return	SimpleTypes::Vml::sptSmileyFace	;
			case	SimpleTypes::shapetypeVerticalScroll	:		return	SimpleTypes::Vml::sptVerticalScroll	;
			case	SimpleTypes::shapetypeHorizontalScroll	:		return	SimpleTypes::Vml::sptHorizontalScroll	;
			case	SimpleTypes::shapetypeCircularArrow	:			return	SimpleTypes::Vml::sptCircularArrow	;
//			case	SimpleTypes::shapetypeNotchedCircularArrow	:		return	SimpleTypes::Vml::sptNotchedCircularArrow	;
			case	SimpleTypes::shapetypeUturnArrow	:				return	SimpleTypes::Vml::sptUturnArrow	;
			case	SimpleTypes::shapetypeCurvedRightArrow	:			return	SimpleTypes::Vml::sptCurvedRightArrow	;
			case	SimpleTypes::shapetypeCurvedLeftArrow	:			return	SimpleTypes::Vml::sptCurvedLeftArrow	;
			case	SimpleTypes::shapetypeCurvedUpArrow	:				return	SimpleTypes::Vml::sptCurvedUpArrow	;
			case	SimpleTypes::shapetypeCurvedDownArrow	:			return	SimpleTypes::Vml::sptCurvedDownArrow	;
			case	SimpleTypes::shapetypeCloudCallout	:				return	SimpleTypes::Vml::sptCloudCallout	;
			case	SimpleTypes::shapetypeEllipseRibbon	:				return	SimpleTypes::Vml::sptEllipseRibbon	;
			case	SimpleTypes::shapetypeEllipseRibbon2	:			return	SimpleTypes::Vml::sptEllipseRibbon2	;
			case	SimpleTypes::shapetypeFlowChartProcess	:			return	SimpleTypes::Vml::sptFlowChartProcess	;
			case	SimpleTypes::shapetypeFlowChartDecision	:			return	SimpleTypes::Vml::sptFlowChartDecision	;
			case	SimpleTypes::shapetypeFlowChartInputOutput	:		return	SimpleTypes::Vml::sptFlowChartInputOutput	;
			case	SimpleTypes::shapetypeFlowChartInternalStorage	:	return	SimpleTypes::Vml::sptFlowChartInternalStorage	;
			case	SimpleTypes::shapetypeFlowChartDocument	:			return	SimpleTypes::Vml::sptFlowChartDocument	;
			case	SimpleTypes::shapetypeFlowChartMultidocument	:	return	SimpleTypes::Vml::sptFlowChartMultidocument	;
			case	SimpleTypes::shapetypeFlowChartTerminator	:		return	SimpleTypes::Vml::sptFlowChartTerminator	;
			case	SimpleTypes::shapetypeFlowChartPreparation	:		return	SimpleTypes::Vml::sptFlowChartPreparation	;
			case	SimpleTypes::shapetypeFlowChartManualInput	:		return	SimpleTypes::Vml::sptFlowChartManualInput	;
			case	SimpleTypes::shapetypeFlowChartManualOperation	:	return	SimpleTypes::Vml::sptFlowChartManualOperation	;
			case	SimpleTypes::shapetypeFlowChartConnector	:		return	SimpleTypes::Vml::sptFlowChartConnector	;
			case	SimpleTypes::shapetypeFlowChartPunchedCard	:		return	SimpleTypes::Vml::sptFlowChartPunchedCard	;
			case	SimpleTypes::shapetypeFlowChartPunchedTape	:		return	SimpleTypes::Vml::sptFlowChartPunchedTape	;
			case	SimpleTypes::shapetypeFlowChartSummingJunction	:	return	SimpleTypes::Vml::sptFlowChartSummingJunction	;
			case	SimpleTypes::shapetypeFlowChartOr	:			return	SimpleTypes::Vml::sptFlowChartOr	;
			case	SimpleTypes::shapetypeFlowChartCollate	:		return	SimpleTypes::Vml::sptFlowChartCollate	;
			case	SimpleTypes::shapetypeFlowChartSort	:			return	SimpleTypes::Vml::sptFlowChartSort	;
			case	SimpleTypes::shapetypeFlowChartExtract	:		return	SimpleTypes::Vml::sptFlowChartExtract	;
			case	SimpleTypes::shapetypeFlowChartMerge	:		return	SimpleTypes::Vml::sptFlowChartMerge	;
			//case	SimpleTypes::shapetypeChartOfflineStorage	:	return	SimpleTypes::Vml::sptFlowChartOfflineStorage	;
			case	SimpleTypes::shapetypeFlowChartOnlineStorage:	return	SimpleTypes::Vml::sptFlowChartOnlineStorage	;
			case	SimpleTypes::shapetypeFlowChartMagneticTape	:	return	SimpleTypes::Vml::sptFlowChartMagneticTape	;
			case	SimpleTypes::shapetypeFlowChartMagneticDisk	:	return	SimpleTypes::Vml::sptFlowChartMagneticDisk	;
			case	SimpleTypes::shapetypeFlowChartMagneticDrum	:	return	SimpleTypes::Vml::sptFlowChartMagneticDrum	;
			case	SimpleTypes::shapetypeFlowChartDisplay	:		return	SimpleTypes::Vml::sptFlowChartDisplay	;
			case	SimpleTypes::shapetypeFlowChartDelay	:		return	SimpleTypes::Vml::sptFlowChartDelay	;
//			case	SimpleTypes::shapetypeTextWave3	:				return	SimpleTypes::Vml::sptTextWave3	;
			case	SimpleTypes::shapetypeTextInflateBottom	:		return	SimpleTypes::Vml::sptTextInflateBottom	;
			case	SimpleTypes::shapetypeTextDeflateBottom	:		return	SimpleTypes::Vml::sptTextDeflateBottom	;
			case	SimpleTypes::shapetypeTextInflateTop	:		return	SimpleTypes::Vml::sptTextInflateTop	;
			case	SimpleTypes::shapetypeTextDeflateTop	:		return	SimpleTypes::Vml::sptTextDeflateTop	;
			case	SimpleTypes::shapetypeTextDeflateInflate:		return	SimpleTypes::Vml::sptTextDeflateInflate	;
			case	SimpleTypes::shapetypeTextFadeRight	:			return	SimpleTypes::Vml::sptTextFadeRight	;
			case	SimpleTypes::shapetypeTextFadeLeft	:			return	SimpleTypes::Vml::sptTextFadeLeft	;
			case	SimpleTypes::shapetypeTextFadeUp	:			return	SimpleTypes::Vml::sptTextFadeUp	;
			case	SimpleTypes::shapetypeTextFadeDown	:			return	SimpleTypes::Vml::sptTextFadeDown	;
			case	SimpleTypes::shapetypeBorderCallout1	:		return	SimpleTypes::Vml::sptBorderCallout1	;
			case	SimpleTypes::shapetypeBorderCallout2	:		return	SimpleTypes::Vml::sptBorderCallout2	;
			case	SimpleTypes::shapetypeBorderCallout3	:		return	SimpleTypes::Vml::sptBorderCallout3	;
			case	SimpleTypes::shapetypeLeftRightUpArrow	:		return	SimpleTypes::Vml::sptLeftRightUpArrow	;
			case	SimpleTypes::shapetypeSun	:					return	SimpleTypes::Vml::sptSun	;
			case	SimpleTypes::shapetypeMoon	:					return	SimpleTypes::Vml::sptMoon	;
			case	SimpleTypes::shapetypeBracketPair	:			return	SimpleTypes::Vml::sptBracketPair	;
			case	SimpleTypes::shapetypeBracePair	:				return	SimpleTypes::Vml::sptBracePair	;
			case	SimpleTypes::shapetypeStar4	:					return	SimpleTypes::Vml::sptSeal4	;
			case	SimpleTypes::shapetypeDoubleWave	:			return	SimpleTypes::Vml::sptDoubleWave	;
			case	SimpleTypes::shapetypeActionButtonBlank	:		return	SimpleTypes::Vml::sptActionButtonBlank	;
			case	SimpleTypes::shapetypeActionButtonHome	:		return	SimpleTypes::Vml::sptActionButtonHome	;
			case	SimpleTypes::shapetypeActionButtonHelp	:		return	SimpleTypes::Vml::sptActionButtonHelp	;
			case	SimpleTypes::shapetypeActionButtonInformation	:	return	SimpleTypes::Vml::sptActionButtonInformation	;
			case	SimpleTypes::shapetypeActionButtonForwardNext	:	return	SimpleTypes::Vml::sptActionButtonForwardNext	;
			case	SimpleTypes::shapetypeActionButtonBackPrevious	:	return	SimpleTypes::Vml::sptActionButtonBackPrevious	;
			case	SimpleTypes::shapetypeActionButtonEnd	:			return	SimpleTypes::Vml::sptActionButtonEnd	;
			case	SimpleTypes::shapetypeActionButtonBeginning	:		return	SimpleTypes::Vml::sptActionButtonBeginning	;
			case	SimpleTypes::shapetypeActionButtonReturn	:		return	SimpleTypes::Vml::sptActionButtonReturn	;
			case	SimpleTypes::shapetypeActionButtonDocument	:		return	SimpleTypes::Vml::sptActionButtonDocument	;
			case	SimpleTypes::shapetypeActionButtonSound	:			return	SimpleTypes::Vml::sptActionButtonSound	;
			case	SimpleTypes::shapetypeActionButtonMovie	:			return	SimpleTypes::Vml::sptActionButtonMovie	;
		default:
			return SimpleTypes::Vml::sptNotPrimitive;
		}
	}

	namespace Logic
	{
		Shape::Shape()
		{
		}

		Shape::~Shape()
		{
		}

		Shape::Shape(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
		}

		const Shape& Shape::operator =(XmlUtils::CXmlNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Shape::fromXML(XmlUtils::CXmlNode& node)
		{
			node.ReadAttributeBase(L"useBgFill", attrUseBgFill);

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("nvSpPr") == strName)
						nvSpPr = oNode;
					else if (_T("spPr") == strName)
						spPr = oNode;
					else if (_T("style") == strName)
						style = oNode;
					else if (_T("txBody") == strName)
						txBody = oNode;
					else if (_T("txXfrm")  == strName)
						txXfrm = oNode;
					else if (_T("txbx") == strName || _T("textbox") == strName)
					{
						XmlUtils::CXmlNode _node = oNode.ReadNode(_T("w:txbxContent"));
						if (_node.IsValid())
							TextBoxShape = _node.GetXml();
					}
					else if (_T("cNvPr") == strName)
					{
						nvSpPr.cNvPr = oNode;
					}
					else if (_T("cNvSpPr") == strName)
					{
						nvSpPr.cNvSpPr = oNode;
					}
					else if (_T("txSp") == strName)
					{
						txBody = oNode.ReadNodeNoNS(_T("txBody"));
					}
					else if (_T("bodyPr") == strName)
					{
						TextBoxBodyPr = oNode;
					}
				}
			}

			FillParentPointersForChilds();
		}

		std::wstring Shape::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("useBgFill"), attrUseBgFill);

			XmlUtils::CNodeValue oValue;
			oValue.Write(nvSpPr);
			oValue.Write(spPr);
			oValue.WriteNullable(style);
			oValue.WriteNullable(txBody);

			return XmlUtils::CreateNode(_T("p:sp"), oAttr, oValue);
		}		

		void Shape::FillParentPointersForChilds()
		{
			nvSpPr.SetParentPointer(this);
			spPr.SetParentPointer(this);
			if(style.IsInit())
				style->SetParentPointer(this);
			if(txBody.IsInit())
				txBody->SetParentPointer(this);
			levelUp = NULL;
		}

		void Shape::GetRect(Aggplus::RECT& pRect)const
		{
			pRect.bottom = 0;
			pRect.left = 0;
			pRect.right = 0;
			pRect.top = 0;

			if(spPr.xfrm.is_init())
			{
				pRect.left		= spPr.xfrm->offX.get_value_or(0);
				pRect.top		= spPr.xfrm->offY.get_value_or(0);
				pRect.right		= pRect.left + spPr.xfrm->extX.get_value_or(0);
				pRect.bottom	= pRect.top + spPr.xfrm->extY.get_value_or(0);
			}
			//else
			//{
			//	if(bMergeWithLevelUp)
			//	{
			//		FillLevelUp();
			//		if(levelUp != NULL)
			//			levelUp->GetRect(pRect);
			//	}
			//}
			if(parentIs<Logic::SpTree>())
				parentAs<Logic::SpTree>().NormalizeRect(pRect);
		}

		void Shape::FillLevelUp()const
		{
			if((levelUp == NULL) && (nvSpPr.nvPr.ph.IsInit()))
			{
				if((nvSpPr.nvPr.ph->type.IsInit()) || (nvSpPr.nvPr.ph->idx.IsInit()))
				{
					if(parentFileIs<Slide>())
						parentFileAs<Slide>().Layout->GetLevelUp(*this);
					else if(parentFileIs<SlideLayout>())
						parentFileAs<SlideLayout>().Master->GetLevelUp(*this);
				}
			}
		}

		void Shape::FillShapeProperties(ShapeProperties& props)
		{
			//props.SetParentFilePointer(*parentFile);
			if(parentFileIs<SlideMaster>())
			{
				std::wstring type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
				//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
				//	type = "text-box";
				parentFileAs<SlideMaster>().FillShapeProperties(props, type);
				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<SlideLayout>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					std::wstring type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<SlideLayout>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			else if(parentFileIs<Slide>())
			{
				//FillLevelUp();
				//if(levelUp != NULL)
				//	levelUp->GetShapeProperties(props);
				//else
				//{
					std::wstring type = nvSpPr.nvPr.ph.is_init()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
					//if( (nvSpPr->cNvSpPr->txBox.get_value_or(false)) && (type == "") )
					//	type = "text-box";
					parentFileAs<Slide>().FillShapeProperties(props, type);
				//}

				props.FillFromTextBody(txBody);
				if(style.IsInit())
					props.FillFontRef(style->fontRef);

				//props.SetParentFilePointer(parentFile);
			}
			props.SetParentFilePointer(parentFile);
		}

		void Shape::FillShapeTextProperties(CShapeTextProperties& props)
		{
			std::wstring type = nvSpPr.nvPr.ph.IsInit()?nvSpPr.nvPr.ph->type.get_value_or(_T("body")):_T("");
			if (parentFileIs<SlideMaster>())
			{
				parentFileAs<SlideMaster>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, NULL);
				if (style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}
			else if (parentFileIs<SlideLayout>())
			{
				parentFileAs<SlideLayout>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, NULL);
				if(style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}
			else if (parentFileIs<Slide>())
			{
				parentFileAs<Slide>().FillShapeTextProperties(props, type);
				props.FillFromTextBody(txBody, body);
				if(style.IsInit())
					props.FillFontRef(style->fontRef, isFontRefInSlide);
			}

			props.SetParentFilePointer(parentFile);
		}

		DWORD Shape::GetFill(UniFill& fill)const
		{
			DWORD BGRA = 0;
			//fill.SetParentFilePointer(*parentFile);

			if(style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetFillStyle(style->fillRef.idx.get_value_or(0), fill);

				if (style->fillRef.Color.is_init())
				{
					if (fill.is<PPTX::Logic::SolidFill>())
					{
						fill.as<PPTX::Logic::SolidFill>().Color = style->fillRef.Color;
					}
				}
				BGRA = style->fillRef.Color.GetBGRA();
			}

			if(spPr.Fill.is_init())
				spPr.Fill.Merge(fill);

			//if((!fill.is_init()) && (bMergeWithLevelUp))
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetFill(fill);
			//}

			return BGRA;
		}

		DWORD Shape::GetLine(Ln& line)const
		{
			DWORD BGRA = 0;
			//line.SetParentFilePointer(*parentFile);

			//if(bMergeWithLevelUp)
			//{
			//	FillLevelUp();
			//	if(levelUp != NULL)
			//		BGRA = levelUp->GetLine(line);
			//}
			if(style.IsInit())
			{
				if(parentFileIs<PPTX::Slide>())
					parentFileAs<PPTX::Slide>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideLayout>())
					parentFileAs<PPTX::SlideLayout>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);
				else if(parentFileIs<PPTX::SlideMaster>())
					parentFileAs<PPTX::SlideMaster>().theme->GetLineStyle(style->lnRef.idx.get_value_or(0), line);

				BGRA = style->lnRef.Color.GetBGRA();
			}

			if(spPr.ln.IsInit())
				spPr.ln->Merge(line);
			return BGRA;
		}

		void Shape::Merge(Shape& shape, bool bIsSlidePlaceholder)const
		{
			shape.nvSpPr = nvSpPr;
			spPr.Merge(shape.spPr);
			if(style.is_init())
			{
				shape.isFontRefInSlide = bIsSlidePlaceholder;
				shape.style = style;
				shape.style->SetParentFilePointer(parentFile);
			}
			if (!bIsSlidePlaceholder)
			{
				if(txBody.IsInit())
					txBody->Merge(shape.txBody);
			}
			else
			{
				if (txBody.is_init())
				{				
					if(!shape.txBody.is_init())
						shape.txBody = new TxBody();
				
					txBody->bodyPr.Merge(shape.txBody->bodyPr);
					if(txBody->lstStyle.is_init())
					{
						for (int i = 0; i < 10; i++)
							if(txBody->lstStyle->levels[i].is_init())
								txBody->lstStyle->levels[i]->Merge(body[i]);
					}
				}
			}
		}

		void Shape::GetShapeFullDescription(Shape& shape, int level)const
		{
			if(level == 0)
				//shape.SetParentFilePointer(*parentFile);
				shape.SetParentPointer(parentElement);

			if(!nvSpPr.nvPr.ph.is_init())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideMaster>())
			{
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::SlideLayout>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape);
				return;
			}
			if(parentFileIs<PPTX::Slide>())
			{
				FillLevelUp();
				if(levelUp != NULL)
					levelUp->GetShapeFullDescription(shape, level + 1);
				Merge(shape, true);
				return;
			}
		}

		void Shape::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap, bool in_group)
		{
			std::wstring strPath, strTextRect;
			bool bOle = false;
			SimpleTypes::Vml::SptType vmlPrst = SimpleTypes::Vml::sptNotPrimitive;

			LONG lW = 43200, lH = 43200;
			int dL = 0, dT = 0, dW = 0, dH = 0;
			
			if (spPr.Geometry.is<PrstGeom>())
			{
				const PPTX::Logic::PrstGeom & lpGeom = spPr.Geometry.as<PPTX::Logic::PrstGeom>();
				
				SimpleTypes::CShapeType<> ooxPrst = SimpleTypes::CShapeType<>(lpGeom.prst.get());
				vmlPrst =  Spt2ShapeType( ooxPrst.GetValue());
			}   

			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->offX.is_init())	dL = *spPr.xfrm->offX;
				if (spPr.xfrm->offY.is_init())	dT = *spPr.xfrm->offY;
				if (spPr.xfrm->extX.is_init())	dW = *spPr.xfrm->extX;
				if (spPr.xfrm->extY.is_init())	dH = *spPr.xfrm->extY;

				lW = spPr.xfrm->extX.get_value_or(43200);
				lH = spPr.xfrm->extY.get_value_or(43200);
			}

			spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);

            std::wstring strId		= L"shape " + std::to_wstring(pWriter->m_lObjectIdVML);
            std::wstring strSpid	= L"_x" + std::to_wstring(0xFFFF & (pWriter->m_lObjectIdVML >> 16)) + L"_s" + std::to_wstring(0xFFFF & pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

            std::wstring strFillAttr;
            std::wstring strStrokeAttr;
            std::wstring strFillNode;
            std::wstring strStrokeNode;;

			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, bOle);
			CalculateLine(spPr, style, oTheme, oClrMap, strStrokeAttr, strStrokeNode, bOle);

			pWriter->StartNode(L"v:shape");

			pWriter->StartAttributes();

            pWriter->WriteAttribute(L"id",		strId);
            pWriter->WriteAttribute(L"o:spid",	strSpid);

			if (vmlPrst != SimpleTypes::Vml::sptNotPrimitive)
				pWriter->WriteAttribute(L"o:spt", (int)vmlPrst);

			NSBinPptxRW::CXmlWriter oStylesWriter;
			if (pWriter->m_strStyleMain.empty())
			{
				oStylesWriter.WriteAttributeCSS(L"position", L"absolute");
				if (in_group)
				{
					oStylesWriter.WriteAttributeCSS_int(L"left",	dL / 100);
					oStylesWriter.WriteAttributeCSS_int(L"top",		dT / 100);
					oStylesWriter.WriteAttributeCSS_int(L"width",	dW / 100);
					oStylesWriter.WriteAttributeCSS_int(L"height",	dH / 100);
				}
				else
				{
					oStylesWriter.WriteAttributeCSS_int_pt(L"left",		dL / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"top",		dT / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"width",	dW / 12700);
					oStylesWriter.WriteAttributeCSS_int_pt(L"height",	dH / 12700);
				}
			}
			if (spPr.xfrm.is_init())
			{
				if (spPr.xfrm->rot.is_init())
				{
					int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
					oStylesWriter.WriteAttributeCSS_int(L"rotation", nRot);
				}
				bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
				bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
				if (bIsFH && bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"xy");
				}
				else if (bIsFH)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"x");
				}
				else if (bIsFV)
				{
					oStylesWriter.WriteAttributeCSS(L"flip", L"y");
				}
			}

			if (txBody.is_init())
			{
				if (txBody->bodyPr.anchor.is_init())
				{
					std::wstring _strAnchor = txBody->bodyPr.anchor->get();
					if (_strAnchor == L"t")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"top");
					else if (_strAnchor == L"b")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"bottom");
					else if (_strAnchor == L"ctr")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"middle");
				}
			}
			else if (TextBoxBodyPr.is_init())
			{
				if (TextBoxBodyPr->anchor.is_init())
				{
					std::wstring _strAnchor = TextBoxBodyPr->anchor->get();
					if (_strAnchor == L"t")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"top");
					else if (_strAnchor == L"b")
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"bottom");
					else if (_strAnchor == _T("ctr"))
						oStylesWriter.WriteAttributeCSS(L"v-text-anchor", L"middle");
				}
			}

			pWriter->WriteAttribute(L"style", pWriter->m_strStyleMain + oStylesWriter.GetXmlString());

			if(!bOle)
			{
				oStylesWriter.ClearNoAttack();
				oStylesWriter.m_oWriter.AddSize(30);
				oStylesWriter.m_oWriter.AddIntNoCheck(dW / 100);
				oStylesWriter.m_oWriter.AddCharNoCheck(WCHAR(','));
				oStylesWriter.m_oWriter.AddIntNoCheck(dH / 100);
				pWriter->WriteAttribute(L"coordsize", oStylesWriter.GetXmlString());

				pWriter->WriteAttribute(L"path", strPath);
			}

			if (!pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}

			pWriter->WriteString(strFillAttr);
			pWriter->WriteString(strStrokeAttr);

			pWriter->EndAttributes();

			pWriter->StartNode(L"v:path");
			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"textboxrect", strTextRect);
			pWriter->EndAttributes();
			pWriter->EndNode(L"v:path");

			pWriter->WriteString(strFillNode);
			pWriter->WriteString(strStrokeNode);
			
			pWriter->WriteString(pWriter->m_strNodes);
			pWriter->m_strNodes.clear();

			if (TextBoxShape.is_init())
			{
				pWriter->StartNode(L"v:textbox");
				pWriter->EndAttributes();
				pWriter->WriteString(*TextBoxShape);
				pWriter->EndNode(L"v:textbox");
			}

			pWriter->EndNode(L"v:shape");

			pWriter->m_strStyleMain.clear();
		}
		void Shape::toXmlWriterVMLBackground(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap)
		{
			std::wstring strFillAttr, strFillNode;
			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, false);

			pWriter->StartNode(L"v:background");

			pWriter->StartAttributes();

			pWriter->WriteString(L" id=\"_x0000_s1025\"");

			if (!pWriter->m_strAttributesMain.empty())
			{
				pWriter->WriteString(pWriter->m_strAttributesMain);
				pWriter->m_strAttributesMain.clear();
			}

			pWriter->WriteString(strFillAttr);

			pWriter->EndAttributes();

			pWriter->WriteString(strFillNode);

			pWriter->EndNode(L"v:background");
		}
	} // namespace Logic
} // namespace PPTX
