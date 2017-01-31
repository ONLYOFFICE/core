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
	OOX::Vml::SptType static Spt2ShapeType(SimpleTypes::EShapeType type) 
	{
		switch(type)
		{
			case	SimpleTypes::shapetypeRect	:			return	OOX::Vml::sptRectangle	;
			case	SimpleTypes::shapetypeRoundRect	:		return	OOX::Vml::sptRoundRectangle	;
			case	SimpleTypes::shapetypeEllipse	:		return	OOX::Vml::sptEllipse	;
			case	SimpleTypes::shapetypeDiamond	:		return	OOX::Vml::sptDiamond	;
			case	SimpleTypes::shapetypeTriangle	:		return	OOX::Vml::sptIsocelesTriangle	;
			case	SimpleTypes::shapetypeRtTriangle	:	return	OOX::Vml::sptRightTriangle	;
			case	SimpleTypes::shapetypeParallelogram	:	return	OOX::Vml::sptParallelogram	;
			case	SimpleTypes::shapetypeTrapezoid	:		return	OOX::Vml::sptTrapezoid	;
			case	SimpleTypes::shapetypeHexagon	:		return	OOX::Vml::sptHexagon	;
			case	SimpleTypes::shapetypeOctagon	:		return	OOX::Vml::sptOctagon	;
			case	SimpleTypes::shapetypePlus	:			return	OOX::Vml::sptPlus	;
			case	SimpleTypes::shapetypeStar5	:			return	OOX::Vml::sptStar	;
			case	SimpleTypes::shapetypeRightArrow:		return	OOX::Vml::sptArrow	;
			case	SimpleTypes::shapetypeThickArrow:		return	OOX::Vml::sptThickArrow	;
			case	SimpleTypes::shapetypeHomePlate	:		return	OOX::Vml::sptHomePlate	;
			case	SimpleTypes::shapetypeCube	:			return	OOX::Vml::sptCube	;
			case	SimpleTypes::shapetypeBallon	:		return	OOX::Vml::sptBalloon	;
//			case	SimpleTypes::shapetypeIrregularSeal1:	return	OOX::Vml::sptSeal	;
			case	SimpleTypes::shapetypeArc	:			return	OOX::Vml::sptArc	;
			case	SimpleTypes::shapetypeLine	:			return	OOX::Vml::sptLine	;
			case	SimpleTypes::shapetypePlaque	:		return	OOX::Vml::sptPlaque	;
			case	SimpleTypes::shapetypeCan	:			return	OOX::Vml::sptCan	;
			case	SimpleTypes::shapetypeDonut	:			return	OOX::Vml::sptDonut	;
//			case	SimpleTypes::shapetypeTextSimple	:	return	OOX::Vml::sptTextSimple	;
//			case	SimpleTypes::shapetypeTextOctagon	:	return	OOX::Vml::sptTextOctagon	;
//			case	SimpleTypes::shapetypeTextHexagon	:	return	OOX::Vml::sptTextHexagon	;
//			case	SimpleTypes::shapetypeTextCurve	:		return	OOX::Vml::sptTextCurve	;
//			case	SimpleTypes::shapetypeTextWave	:		return	OOX::Vml::sptTextWave	;
//			case	SimpleTypes::shapetypeTextRing	:		return	OOX::Vml::sptTextRing	;
//			case	SimpleTypes::shapetypeTextOnCurve	:	return	OOX::Vml::sptTextOnCurve	;
//			case	SimpleTypes::shapetypeTextOnRing	:	return	OOX::Vml::sptTextOnRing	;
			case	SimpleTypes::shapetypeStraightConnector1:	return	OOX::Vml::sptStraightConnector1	;
			case	SimpleTypes::shapetypeBentConnector2	:	return	OOX::Vml::sptBentConnector2	;
			case	SimpleTypes::shapetypeBentConnector3	:	return	OOX::Vml::sptBentConnector3	;
			case	SimpleTypes::shapetypeBentConnector4	:	return	OOX::Vml::sptBentConnector4	;
			case	SimpleTypes::shapetypeBentConnector5	:	return	OOX::Vml::sptBentConnector5	;
			case	SimpleTypes::shapetypeCurvedConnector2	:	return	OOX::Vml::sptCurvedConnector2	;
			case	SimpleTypes::shapetypeCurvedConnector3	:	return	OOX::Vml::sptCurvedConnector3	;
			case	SimpleTypes::shapetypeCurvedConnector4	:	return	OOX::Vml::sptCurvedConnector4	;
			case	SimpleTypes::shapetypeCurvedConnector5	:	return	OOX::Vml::sptCurvedConnector5	;
			case	SimpleTypes::shapetypeCallout1	:			return	OOX::Vml::sptCallout1	;
			case	SimpleTypes::shapetypeCallout2	:			return	OOX::Vml::sptCallout2	;
			case	SimpleTypes::shapetypeCallout3	:			return	OOX::Vml::sptCallout3	;
			case	SimpleTypes::shapetypeAccentCallout1	:	return	OOX::Vml::sptAccentCallout1	;
			case	SimpleTypes::shapetypeAccentCallout2	:	return	OOX::Vml::sptAccentCallout2	;
			case	SimpleTypes::shapetypeAccentCallout3	:	return	OOX::Vml::sptAccentCallout3	;
			case	SimpleTypes::shapetypeAccentBorderCallout1	:	return	OOX::Vml::sptAccentBorderCallout1	;
			case	SimpleTypes::shapetypeAccentBorderCallout2	:	return	OOX::Vml::sptAccentBorderCallout2	;
			case	SimpleTypes::shapetypeAccentBorderCallout3	:	return	OOX::Vml::sptAccentBorderCallout3	;
			case	SimpleTypes::shapetypeRibbon	:				return	OOX::Vml::sptRibbon	;
			case	SimpleTypes::shapetypeRibbon2	:				return	OOX::Vml::sptRibbon2	;
			case	SimpleTypes::shapetypeChevron	:				return	OOX::Vml::sptChevron	;
			case	SimpleTypes::shapetypePentagon	:				return	OOX::Vml::sptPentagon	;
			case	SimpleTypes::shapetypeNoSmoking	:				return	OOX::Vml::sptNoSmoking	;
			case	SimpleTypes::shapetypeStar8	:					return	OOX::Vml::sptSeal8	;
			case	SimpleTypes::shapetypeStar16	:				return	OOX::Vml::sptSeal16	;
			case	SimpleTypes::shapetypeStar32	:				return	OOX::Vml::sptSeal32	;
			case	SimpleTypes::shapetypeWedgeRectCallout	:		return	OOX::Vml::sptWedgeRectCallout	;
			case	SimpleTypes::shapetypeWedgeRoundRectCallout	:	return	OOX::Vml::sptWedgeRRectCallout	;
			case	SimpleTypes::shapetypeWedgeEllipseCallout	:	return	OOX::Vml::sptWedgeEllipseCallout	;
			case	SimpleTypes::shapetypeWave	:					return	OOX::Vml::sptWave	;
			case	SimpleTypes::shapetypeFoldedCorner	:			return	OOX::Vml::sptFoldedCorner	;
			case	SimpleTypes::shapetypeLeftArrow	:				return	OOX::Vml::sptLeftArrow	;
			case	SimpleTypes::shapetypeDownArrow	:				return	OOX::Vml::sptDownArrow	;
			case	SimpleTypes::shapetypeUpArrow	:				return	OOX::Vml::sptUpArrow	;
			case	SimpleTypes::shapetypeLeftRightArrow	:		return	OOX::Vml::sptLeftRightArrow	;
			case	SimpleTypes::shapetypeUpDownArrow	:			return	OOX::Vml::sptUpDownArrow	;
			case	SimpleTypes::shapetypeIrregularSeal1	:		return	OOX::Vml::sptIrregularSeal1	;
			case	SimpleTypes::shapetypeIrregularSeal2	:		return	OOX::Vml::sptIrregularSeal2	;
			case	SimpleTypes::shapetypeLightningBolt	:			return	OOX::Vml::sptLightningBolt	;
			case	SimpleTypes::shapetypeHeart	:					return	OOX::Vml::sptHeart	;
			case	SimpleTypes::shapetypeQuadArrow	:				return	OOX::Vml::sptQuadArrow	;
			case	SimpleTypes::shapetypeLeftArrowCallout	:		return	OOX::Vml::sptLeftArrowCallout	;
			case	SimpleTypes::shapetypeRightArrowCallout	:		return	OOX::Vml::sptRightArrowCallout	;
			case	SimpleTypes::shapetypeUpArrowCallout	:		return	OOX::Vml::sptUpArrowCallout	;
			case	SimpleTypes::shapetypeDownArrowCallout	:		return	OOX::Vml::sptDownArrowCallout	;
			case	SimpleTypes::shapetypeLeftRightArrowCallout	:	return	OOX::Vml::sptLeftRightArrowCallout	;
			case	SimpleTypes::shapetypeUpDownArrowCallout	:	return	OOX::Vml::sptUpDownArrowCallout	;
			case	SimpleTypes::shapetypeQuadArrowCallout	:		return	OOX::Vml::sptQuadArrowCallout	;
			case	SimpleTypes::shapetypeBevel	:					return	OOX::Vml::sptBevel	;
			case	SimpleTypes::shapetypeLeftBracket	:			return	OOX::Vml::sptLeftBracket	;
			case	SimpleTypes::shapetypeRightBracket	:			return	OOX::Vml::sptRightBracket	;
			case	SimpleTypes::shapetypeLeftBrace	:				return	OOX::Vml::sptLeftBrace	;
			case	SimpleTypes::shapetypeRightBrace	:			return	OOX::Vml::sptRightBrace	;
			case	SimpleTypes::shapetypeLeftUpArrow	:			return	OOX::Vml::sptLeftUpArrow	;
			case	SimpleTypes::shapetypeBentUpArrow	:			return	OOX::Vml::sptBentUpArrow	;
			case	SimpleTypes::shapetypeBentArrow	:				return	OOX::Vml::sptBentArrow	;
			case	SimpleTypes::shapetypeStar24	:				return	OOX::Vml::sptSeal24	;
			case	SimpleTypes::shapetypeStripedRightArrow	:		return	OOX::Vml::sptStripedRightArrow	;
			case	SimpleTypes::shapetypeNotchedRightArrow	:		return	OOX::Vml::sptNotchedRightArrow	;
			case	SimpleTypes::shapetypeBlockArc	:				return	OOX::Vml::sptBlockArc	;
			case	SimpleTypes::shapetypeSmileyFace	:			return	OOX::Vml::sptSmileyFace	;
			case	SimpleTypes::shapetypeVerticalScroll	:		return	OOX::Vml::sptVerticalScroll	;
			case	SimpleTypes::shapetypeHorizontalScroll	:		return	OOX::Vml::sptHorizontalScroll	;
			case	SimpleTypes::shapetypeCircularArrow	:			return	OOX::Vml::sptCircularArrow	;
//			case	SimpleTypes::shapetypeNotchedCircularArrow	:		return	OOX::Vml::sptNotchedCircularArrow	;
			case	SimpleTypes::shapetypeUturnArrow	:				return	OOX::Vml::sptUturnArrow	;
			case	SimpleTypes::shapetypeCurvedRightArrow	:			return	OOX::Vml::sptCurvedRightArrow	;
			case	SimpleTypes::shapetypeCurvedLeftArrow	:			return	OOX::Vml::sptCurvedLeftArrow	;
			case	SimpleTypes::shapetypeCurvedUpArrow	:				return	OOX::Vml::sptCurvedUpArrow	;
			case	SimpleTypes::shapetypeCurvedDownArrow	:			return	OOX::Vml::sptCurvedDownArrow	;
			case	SimpleTypes::shapetypeCloudCallout	:				return	OOX::Vml::sptCloudCallout	;
			case	SimpleTypes::shapetypeEllipseRibbon	:				return	OOX::Vml::sptEllipseRibbon	;
			case	SimpleTypes::shapetypeEllipseRibbon2	:			return	OOX::Vml::sptEllipseRibbon2	;
			case	SimpleTypes::shapetypeFlowChartProcess	:			return	OOX::Vml::sptFlowChartProcess	;
			case	SimpleTypes::shapetypeFlowChartDecision	:			return	OOX::Vml::sptFlowChartDecision	;
			case	SimpleTypes::shapetypeFlowChartInputOutput	:		return	OOX::Vml::sptFlowChartInputOutput	;
			case	SimpleTypes::shapetypeFlowChartInternalStorage	:	return	OOX::Vml::sptFlowChartInternalStorage	;
			case	SimpleTypes::shapetypeFlowChartDocument	:			return	OOX::Vml::sptFlowChartDocument	;
			case	SimpleTypes::shapetypeFlowChartMultidocument	:	return	OOX::Vml::sptFlowChartMultidocument	;
			case	SimpleTypes::shapetypeFlowChartTerminator	:		return	OOX::Vml::sptFlowChartTerminator	;
			case	SimpleTypes::shapetypeFlowChartPreparation	:		return	OOX::Vml::sptFlowChartPreparation	;
			case	SimpleTypes::shapetypeFlowChartManualInput	:		return	OOX::Vml::sptFlowChartManualInput	;
			case	SimpleTypes::shapetypeFlowChartManualOperation	:	return	OOX::Vml::sptFlowChartManualOperation	;
			case	SimpleTypes::shapetypeFlowChartConnector	:		return	OOX::Vml::sptFlowChartConnector	;
			case	SimpleTypes::shapetypeFlowChartPunchedCard	:		return	OOX::Vml::sptFlowChartPunchedCard	;
			case	SimpleTypes::shapetypeFlowChartPunchedTape	:		return	OOX::Vml::sptFlowChartPunchedTape	;
			case	SimpleTypes::shapetypeFlowChartSummingJunction	:	return	OOX::Vml::sptFlowChartSummingJunction	;
			case	SimpleTypes::shapetypeFlowChartOr	:			return	OOX::Vml::sptFlowChartOr	;
			case	SimpleTypes::shapetypeFlowChartCollate	:		return	OOX::Vml::sptFlowChartCollate	;
			case	SimpleTypes::shapetypeFlowChartSort	:			return	OOX::Vml::sptFlowChartSort	;
			case	SimpleTypes::shapetypeFlowChartExtract	:		return	OOX::Vml::sptFlowChartExtract	;
			case	SimpleTypes::shapetypeFlowChartMerge	:		return	OOX::Vml::sptFlowChartMerge	;
			//case	SimpleTypes::shapetypeChartOfflineStorage	:	return	OOX::Vml::sptFlowChartOfflineStorage	;
			case	SimpleTypes::shapetypeFlowChartOnlineStorage:	return	OOX::Vml::sptFlowChartOnlineStorage	;
			case	SimpleTypes::shapetypeFlowChartMagneticTape	:	return	OOX::Vml::sptFlowChartMagneticTape	;
			case	SimpleTypes::shapetypeFlowChartMagneticDisk	:	return	OOX::Vml::sptFlowChartMagneticDisk	;
			case	SimpleTypes::shapetypeFlowChartMagneticDrum	:	return	OOX::Vml::sptFlowChartMagneticDrum	;
			case	SimpleTypes::shapetypeFlowChartDisplay	:		return	OOX::Vml::sptFlowChartDisplay	;
			case	SimpleTypes::shapetypeFlowChartDelay	:		return	OOX::Vml::sptFlowChartDelay	;
//			case	SimpleTypes::shapetypeTextWave3	:				return	OOX::Vml::sptTextWave3	;
			case	SimpleTypes::shapetypeTextInflateBottom	:		return	OOX::Vml::sptTextInflateBottom	;
			case	SimpleTypes::shapetypeTextDeflateBottom	:		return	OOX::Vml::sptTextDeflateBottom	;
			case	SimpleTypes::shapetypeTextInflateTop	:		return	OOX::Vml::sptTextInflateTop	;
			case	SimpleTypes::shapetypeTextDeflateTop	:		return	OOX::Vml::sptTextDeflateTop	;
			case	SimpleTypes::shapetypeTextDeflateInflate:		return	OOX::Vml::sptTextDeflateInflate	;
			case	SimpleTypes::shapetypeTextFadeRight	:			return	OOX::Vml::sptTextFadeRight	;
			case	SimpleTypes::shapetypeTextFadeLeft	:			return	OOX::Vml::sptTextFadeLeft	;
			case	SimpleTypes::shapetypeTextFadeUp	:			return	OOX::Vml::sptTextFadeUp	;
			case	SimpleTypes::shapetypeTextFadeDown	:			return	OOX::Vml::sptTextFadeDown	;
			case	SimpleTypes::shapetypeBorderCallout1	:		return	OOX::Vml::sptBorderCallout1	;
			case	SimpleTypes::shapetypeBorderCallout2	:		return	OOX::Vml::sptBorderCallout2	;
			case	SimpleTypes::shapetypeBorderCallout3	:		return	OOX::Vml::sptBorderCallout3	;
			case	SimpleTypes::shapetypeLeftRightUpArrow	:		return	OOX::Vml::sptLeftRightUpArrow	;
			case	SimpleTypes::shapetypeSun	:					return	OOX::Vml::sptSun	;
			case	SimpleTypes::shapetypeMoon	:					return	OOX::Vml::sptMoon	;
			case	SimpleTypes::shapetypeBracketPair	:			return	OOX::Vml::sptBracketPair	;
			case	SimpleTypes::shapetypeBracePair	:				return	OOX::Vml::sptBracePair	;
			case	SimpleTypes::shapetypeStar4	:					return	OOX::Vml::sptSeal4	;
			case	SimpleTypes::shapetypeDoubleWave	:			return	OOX::Vml::sptDoubleWave	;
			case	SimpleTypes::shapetypeActionButtonBlank	:		return	OOX::Vml::sptActionButtonBlank	;
			case	SimpleTypes::shapetypeActionButtonHome	:		return	OOX::Vml::sptActionButtonHome	;
			case	SimpleTypes::shapetypeActionButtonHelp	:		return	OOX::Vml::sptActionButtonHelp	;
			case	SimpleTypes::shapetypeActionButtonInformation	:	return	OOX::Vml::sptActionButtonInformation	;
			case	SimpleTypes::shapetypeActionButtonForwardNext	:	return	OOX::Vml::sptActionButtonForwardNext	;
			case	SimpleTypes::shapetypeActionButtonBackPrevious	:	return	OOX::Vml::sptActionButtonBackPrevious	;
			case	SimpleTypes::shapetypeActionButtonEnd	:			return	OOX::Vml::sptActionButtonEnd	;
			case	SimpleTypes::shapetypeActionButtonBeginning	:		return	OOX::Vml::sptActionButtonBeginning	;
			case	SimpleTypes::shapetypeActionButtonReturn	:		return	OOX::Vml::sptActionButtonReturn	;
			case	SimpleTypes::shapetypeActionButtonDocument	:		return	OOX::Vml::sptActionButtonDocument	;
			case	SimpleTypes::shapetypeActionButtonSound	:			return	OOX::Vml::sptActionButtonSound	;
			case	SimpleTypes::shapetypeActionButtonMovie	:			return	OOX::Vml::sptActionButtonMovie	;
		default:
			return OOX::Vml::sptNotPrimitive;
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

		void Shape::toXmlWriterVML(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap)
		{
			std::wstring strPath = _T("");
			std::wstring strTextRect = _T("");

			LONG lW = 43200;
			LONG lH = 43200;
			if (spPr.xfrm.is_init())
			{
				lW = spPr.xfrm->extX.get_value_or(43200);
				lH = spPr.xfrm->extY.get_value_or(43200);
			}
			bool bOle = false;

			spPr.Geometry.ConvertToCustomVML(pWriter->m_pOOXToVMLRenderer, strPath, strTextRect, lW, lH);

            std::wstring strId  = L"shape " + std::to_wstring(pWriter->m_lObjectIdVML);
            std::wstring strSpid = L"_x" + std::to_wstring(0xFFFF & (pWriter->m_lObjectIdVML >> 16)) + L"_s" + std::to_wstring(0xFFFF & pWriter->m_lObjectIdVML);
			pWriter->m_lObjectIdVML++;

            std::wstring strFillAttr;
            std::wstring strStrokeAttr;
            std::wstring strFillNode;
            std::wstring strStrokeNode;;

            CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, bOle);
			CalculateLine(spPr, style, oTheme, oClrMap, strStrokeAttr, strStrokeNode, bOle);

            if (!pWriter->m_strStyleMain.empty())
			{
                pWriter->StartNode(L"v:shape");

				pWriter->StartAttributes();

                pWriter->WriteAttribute(L"id",		strId);
                pWriter->WriteAttribute(L"o:spid",	strSpid);

				if (spPr.Geometry.is<PrstGeom>())
				{
					const PPTX::Logic::PrstGeom & lpGeom = spPr.Geometry.as<PPTX::Logic::PrstGeom>();
					
					SimpleTypes::CShapeType<> ooxPrst = SimpleTypes::CShapeType<>(lpGeom.prst.get());
					OOX::Vml::SptType vmlPrst =  Spt2ShapeType( ooxPrst.GetValue());

					if (vmlPrst != OOX::Vml::sptNotPrimitive)
						pWriter->WriteAttribute(L"o:spt", (int)vmlPrst);
				}

				NSBinPptxRW::CXmlWriter oStylesWriter;
				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->rot.is_init())
					{
						int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
						oStylesWriter.WriteAttributeCSS_int(_T("rotation"), nRot);
					}
					bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
					bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
					if (bIsFH && bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("xy"));
					}
					else if (bIsFH)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("x"));
					}
					else if (bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("y"));
					}
				}

				if (txBody.is_init())
				{
					if (txBody->bodyPr.anchor.is_init())
					{
						std::wstring _strAnchor = txBody->bodyPr.anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}
				else if (TextBoxBodyPr.is_init())
				{
					if (TextBoxBodyPr->anchor.is_init())
					{
						std::wstring _strAnchor = TextBoxBodyPr->anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}

				if (oStylesWriter.GetSize() == 0)
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain);
				}
				else
				{
					pWriter->WriteAttribute(_T("style"), pWriter->m_strStyleMain + oStylesWriter.GetXmlString());
				}
				if(!bOle)
				{
					pWriter->WriteAttribute(_T("coordsize"), (std::wstring)_T("100000,100000"));
					pWriter->WriteAttribute(_T("path"), strPath);
				}

				if (!pWriter->m_strAttributesMain.empty())
				{
					pWriter->WriteString(pWriter->m_strAttributesMain);
					pWriter->m_strAttributesMain.clear();
				}

				pWriter->WriteString(strFillAttr);
				pWriter->WriteString(strStrokeAttr);

				pWriter->EndAttributes();

				pWriter->StartNode(_T("v:path"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("textboxrect"), strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("v:path"));

				pWriter->WriteString(strFillNode);
				pWriter->WriteString(strStrokeNode);
				pWriter->WriteString(pWriter->m_strNodes);
				pWriter->m_strNodes = _T("");

				if (TextBoxShape.is_init())
				{
					pWriter->StartNode(_T("v:textbox"));
					pWriter->EndAttributes();
					pWriter->WriteString(*TextBoxShape);
					pWriter->EndNode(_T("v:textbox"));
				}

				pWriter->EndNode(_T("v:shape"));

				pWriter->m_strStyleMain = _T("");
			}
			else
			{
				int dL = 0;
				int dT = 0;
				int dW = 0;
				int dH = 0;

				NSBinPptxRW::CXmlWriter oStylesWriter;
				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->offX.is_init())
						dL = (*spPr.xfrm->offX)	/ 12700.;
					if (spPr.xfrm->offY.is_init())
						dT = (*spPr.xfrm->offY)	/ 12700.;
					if (spPr.xfrm->extX.is_init())
						dW = (*spPr.xfrm->extX) / 12700.;
					if (spPr.xfrm->extY.is_init())
						dH = (*spPr.xfrm->extY	/ 12700.);
				}
				oStylesWriter.WriteAttributeCSS(L"position", L"absolute");
				oStylesWriter.WriteAttributeCSS_int_pt(L"left",		dL);
				oStylesWriter.WriteAttributeCSS_int_pt(L"top",		dT);
				oStylesWriter.WriteAttributeCSS_int_pt(L"width",	dW);
				oStylesWriter.WriteAttributeCSS_int_pt(L"height",	dH);

				if (spPr.xfrm.is_init())
				{
					if (spPr.xfrm->rot.is_init())
					{
						int nRot = (int)((double)(*(spPr.xfrm->rot)) / 60000.0);
						oStylesWriter.WriteAttributeCSS_int(_T("rotation"), nRot);
					}
					bool bIsFH = spPr.xfrm->flipH.get_value_or(false);
					bool bIsFV = spPr.xfrm->flipV.get_value_or(false);
					if (bIsFH && bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("xy"));
					}
					else if (bIsFH)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("x"));
					}
					else if (bIsFV)
					{
						oStylesWriter.WriteAttributeCSS(_T("flip"), _T("y"));
					}
				}

				if (txBody.is_init())
				{
					if (txBody->bodyPr.anchor.is_init())
					{
						std::wstring _strAnchor = txBody->bodyPr.anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}
				else if (TextBoxBodyPr.is_init())
				{
					if (TextBoxBodyPr->anchor.is_init())
					{
						std::wstring _strAnchor = TextBoxBodyPr->anchor->get();
						if (_strAnchor == _T("t"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("top"));
						else if (_strAnchor == _T("b"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("bottom"));
						else if (_strAnchor == _T("ctr"))
							oStylesWriter.WriteAttributeCSS(_T("v-text-anchor"), _T("middle"));
					}
				}

				pWriter->StartNode(_T("v:shape"));

				pWriter->StartAttributes();

				pWriter->WriteAttribute(_T("id"), strId);
				pWriter->WriteAttribute(_T("o:spid"), strSpid);

				if (spPr.Geometry.is<PrstGeom>())
				{
					const PPTX::Logic::PrstGeom & lpGeom = spPr.Geometry.as<PPTX::Logic::PrstGeom>();
					
					SimpleTypes::CShapeType<> ooxPrst = SimpleTypes::CShapeType<>(lpGeom.prst.get());
					OOX::Vml::SptType vmlPrst =  Spt2ShapeType( ooxPrst.GetValue());

					if (vmlPrst != OOX::Vml::sptNotPrimitive)
						pWriter->WriteAttribute(L"o:spt", (int)vmlPrst);
				}
				pWriter->WriteAttribute(_T("style"), oStylesWriter.GetXmlString());

				if(!bOle)
				{
					pWriter->WriteAttribute(_T("coordsize"), (std::wstring)_T("100000,100000"));
					pWriter->WriteAttribute(_T("path"), strPath);
				}

				if (!pWriter->m_strAttributesMain.empty())
				{
					pWriter->WriteString(pWriter->m_strAttributesMain);
					pWriter->m_strAttributesMain.clear();
				}

				pWriter->WriteString(strFillAttr);
				pWriter->WriteString(strStrokeAttr);

				pWriter->EndAttributes();

				pWriter->StartNode(_T("v:path"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("textboxrect"), strTextRect);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("v:path"));

				pWriter->WriteString(strFillNode);
				pWriter->WriteString(strStrokeNode);

				if (TextBoxShape.is_init())
				{
					pWriter->StartNode(_T("v:textbox"));
					pWriter->EndAttributes();
					pWriter->WriteString(*TextBoxShape);
					pWriter->EndNode(_T("v:textbox"));
				}

				pWriter->EndNode(_T("v:shape"));
			}
		}
		void Shape::toXmlWriterVMLBackground(NSBinPptxRW::CXmlWriter *pWriter, NSCommon::smart_ptr<PPTX::WrapperFile>& oTheme, NSCommon::smart_ptr<PPTX::WrapperWritingElement>& oClrMap)
		{
			std::wstring strFillAttr = _T("");
			std::wstring strFillNode = _T("");
			CalculateFill(spPr, style, oTheme, oClrMap, strFillAttr, strFillNode, false);

			pWriter->StartNode(_T("v:background"));

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

			pWriter->EndNode(_T("v:background"));
		}
	} // namespace Logic
} // namespace PPTX
