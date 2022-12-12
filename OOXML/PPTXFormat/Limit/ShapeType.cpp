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

#include "ShapeType.h"

namespace PPTX
{
	namespace Limit
	{		
		ShapeType::ShapeType()
		{
			m_strValue = L"rect";
		}
		void ShapeType::set(const std::wstring& strValue)
		{
			if ((L"accentBorderCallout1" == strValue) ||
				(L"accentBorderCallout2" == strValue) ||
				(L"accentBorderCallout3" == strValue) ||
				(L"accentCallout1" == strValue) ||
				(L"accentCallout2" == strValue) ||
				(L"accentCallout3" == strValue) ||
				(L"actionButtonBackPrevious" == strValue) ||
				(L"actionButtonBeginning" == strValue) ||
				(L"actionButtonBlank" == strValue) ||
				(L"actionButtonDocument" == strValue) ||
				(L"actionButtonEnd" == strValue) ||
				(L"actionButtonForwardNext" == strValue) ||
				(L"actionButtonHelp" == strValue) ||
				(L"actionButtonHome" == strValue) ||
				(L"actionButtonInformation" == strValue) ||
				(L"actionButtonMovie" == strValue) ||
				(L"actionButtonReturn" == strValue) ||
				(L"actionButtonSound" == strValue) ||
				(L"arc" == strValue) ||
				(L"bentArrow" == strValue) ||
				(L"bentConnector2" == strValue) ||
				(L"bentConnector3" == strValue) ||
				(L"bentConnector4" == strValue) ||
				(L"bentConnector5" == strValue) ||
				(L"bentUpArrow" == strValue) ||
				(L"bevel" == strValue) ||
				(L"blockArc" == strValue) ||
				(L"borderCallout1" == strValue) ||
				(L"borderCallout2" == strValue) ||
				(L"borderCallout3" == strValue) ||
				(L"bracePair" == strValue) ||
				(L"bracketPair" == strValue) ||
				(L"callout1" == strValue) ||
				(L"callout2" == strValue) ||
				(L"callout3" == strValue) ||
				(L"can" == strValue) ||
				(L"chartPlus" == strValue) ||
				(L"chartStar" == strValue) ||
				(L"chartX" == strValue) ||
				(L"chevron" == strValue) ||
				(L"chord" == strValue) ||
				(L"circularArrow" == strValue) ||
				(L"cloud" == strValue) ||
				(L"cloudCallout" == strValue) ||
				(L"corner" == strValue) ||
				(L"cornerTabs" == strValue) ||
				(L"cube" == strValue) ||
				(L"curvedConnector2" == strValue) ||
				(L"curvedConnector3" == strValue) ||
				(L"curvedConnector4" == strValue) ||
				(L"curvedConnector5" == strValue) ||
				(L"curvedDownArrow" == strValue) ||
				(L"curvedLeftArrow" == strValue) ||
				(L"curvedRightArrow" == strValue) ||
				(L"curvedUpArrow" == strValue) ||
				(L"decagon" == strValue) ||
				(L"diagStripe" == strValue) ||
				(L"diamond" == strValue) ||
				(L"dodecagon" == strValue) ||
				(L"donut" == strValue) ||
				(L"doubleWave" == strValue) ||
				(L"downArrow" == strValue) ||
				(L"downArrowCallout" == strValue) ||
				(L"ellipse" == strValue) ||
				(L"ellipseRibbon" == strValue) ||
				(L"ellipseRibbon2" == strValue) ||
				(L"flowChartAlternateProcess" == strValue) ||
				(L"flowChartCollate" == strValue) ||
				(L"flowChartConnector" == strValue) ||
				(L"flowChartDecision" == strValue) ||
				(L"flowChartDelay" == strValue) ||
				(L"flowChartDisplay" == strValue) ||
				(L"flowChartDocument" == strValue) ||
				(L"flowChartExtract" == strValue) ||
				(L"flowChartInputOutput" == strValue) ||
				(L"flowChartInternalStorage" == strValue) ||
				(L"flowChartMagneticDisk" == strValue) ||
				(L"flowChartMagneticDrum" == strValue) ||
				(L"flowChartMagneticTape" == strValue) ||
				(L"flowChartManualInput" == strValue) ||
				(L"flowChartManualOperation" == strValue) ||
				(L"flowChartMerge" == strValue) ||
				(L"flowChartMultidocument" == strValue) ||
				(L"flowChartOfflineStorage" == strValue) ||
				(L"flowChartOffpageConnector" == strValue) ||
				(L"flowChartOnlineStorage" == strValue) ||
				(L"flowChartOr" == strValue) ||
				(L"flowChartPredefinedProcess" == strValue) ||
				(L"flowChartPreparation" == strValue) ||
				(L"flowChartProcess" == strValue) ||
				(L"flowChartPunchedCard" == strValue) ||
				(L"flowChartPunchedTape" == strValue) ||
				(L"flowChartSort" == strValue) ||
				(L"flowChartSummingJunction" == strValue) ||
				(L"flowChartTerminator" == strValue) ||
				(L"foldedCorner" == strValue) ||
				(L"frame" == strValue) ||
				(L"funnel" == strValue) ||
				(L"gear6" == strValue) ||
				(L"gear9" == strValue) ||
				(L"halfFrame" == strValue) ||
				(L"heart" == strValue) ||
				(L"heptagon" == strValue) ||
				(L"hexagon" == strValue) ||
				(L"homePlate" == strValue) ||
				(L"horizontalScroll" == strValue) ||
				(L"irregularSeal1" == strValue) ||
				(L"irregularSeal2" == strValue) ||
				(L"leftArrow" == strValue) ||
				(L"leftArrowCallout" == strValue) ||
				(L"leftBrace" == strValue) ||
				(L"leftBracket" == strValue) ||
				(L"leftCircularArrow" == strValue) ||
				(L"leftRightArrow" == strValue) ||
				(L"leftRightArrowCallout" == strValue) ||
				(L"leftRightCircularArrow" == strValue) ||
				(L"leftRightRibbon" == strValue) ||
				(L"leftRightUpArrow" == strValue) ||
				(L"leftUpArrow" == strValue) ||
				(L"lightningBolt" == strValue) ||
				(L"line" == strValue) ||
				(L"lineInv" == strValue) ||
				(L"mathDivide" == strValue) ||
				(L"mathEqual" == strValue) ||
				(L"mathMinus" == strValue) ||
				(L"mathMultiply" == strValue) ||
				(L"mathNotEqual" == strValue) ||
				(L"mathPlus" == strValue) ||
				(L"moon" == strValue) ||
				(L"nonIsoscelesTrapezoid" == strValue) ||
				(L"noSmoking" == strValue) ||
				(L"notchedRightArrow" == strValue) ||
				(L"octagon" == strValue) ||
				(L"parallelogram" == strValue) ||
				(L"pentagon" == strValue) ||
				(L"pie" == strValue) ||
				(L"pieWedge" == strValue) ||
				(L"plaque" == strValue) ||
				(L"plaqueTabs" == strValue) ||
				(L"plus" == strValue) ||
				(L"quadArrow" == strValue) ||
				(L"quadArrowCallout" == strValue) ||
				(L"rect" == strValue) ||
				(L"ribbon" == strValue) ||
				(L"ribbon2" == strValue) ||
				(L"rightArrow" == strValue) ||
				(L"rightArrowCallout" == strValue) ||
				(L"rightBrace" == strValue) ||
				(L"rightBracket" == strValue) ||
				(L"round1Rect" == strValue) ||
				(L"round2DiagRect" == strValue) ||
				(L"round2SameRect" == strValue) ||
				(L"roundRect" == strValue) ||
				(L"rtTriangle" == strValue) ||
				(L"smileyFace" == strValue) ||
				(L"snip1Rect" == strValue) ||
				(L"snip2DiagRect" == strValue) ||
				(L"snip2SameRect" == strValue) ||
				(L"snipRoundRect" == strValue) ||
				(L"squareTabs" == strValue) ||
				(L"star10" == strValue) ||
				(L"star12" == strValue) ||
				(L"star16" == strValue) ||
				(L"star24" == strValue) ||
				(L"star32" == strValue) ||
				(L"star4" == strValue) ||
				(L"star5" == strValue) ||
				(L"star6" == strValue) ||
				(L"star7" == strValue) ||
				(L"star8" == strValue) ||
				(L"straightConnector1" == strValue) ||
				(L"stripedRightArrow" == strValue) ||
				(L"sun" == strValue) ||
				(L"swooshArrow" == strValue) ||
				(L"teardrop" == strValue) ||
				(L"trapezoid" == strValue) ||
				(L"triangle" == strValue) ||
				(L"upArrow" == strValue) ||
				(L"upArrowCallout" == strValue) ||
				(L"upDownArrow" == strValue) ||
				(L"upDownArrowCallout" == strValue) ||
				(L"uturnArrow" == strValue) ||
				(L"verticalScroll" == strValue) ||
				(L"wave" == strValue) ||
				(L"wedgeEllipseCallout" == strValue) ||
				(L"wedgeRectCallout" == strValue) ||
				(L"wedgeRoundRectCallout" == strValue))
			{
				m_strValue = strValue;
			}
		}
		BYTE ShapeType::GetBYTECode() const
		{
			//not using
			return 0;
		}
		void ShapeType::SetBYTECode(const BYTE& src)
		{
			//not using
		}
	} // namespace Limit
} // namespace PPTX
