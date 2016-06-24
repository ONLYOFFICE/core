/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#pragma once
#include "../RtfDocument.h"

#include "OOXBorderReader.h"
#include "OOXcnfStyleReader.h"

class OOXtcPrReader
{
	OOX::Logic::CTableCellProperties*		m_ooxTableCellProps;
public: 
	OOXtcPrReader(OOX::Logic::CTableCellProperties*		ooxTableCellProps)
	{
		m_ooxTableCellProps = ooxTableCellProps;
	}
	bool Parse( ReaderParameter oParam ,RtfCellProperty& oOutputProperty,  CcnfStyle& oConditionalTableStyle, int nCurCell, int nCellCount, int nCurRow, int nRowCount  )
	{
		if (m_ooxTableCellProps == NULL) return false;

		//ищем cnfStyle  и применяем внешний стиль
		if( NULL != oParam.poTableStyle )
		{
			if( m_ooxTableCellProps->m_oCnfStyle.IsInit() )
			{
				CcnfStyle ocnfStyle;
				OOXcnfStyleReader ocnfStyleReader(m_ooxTableCellProps->m_oCnfStyle.GetPointer());
				ocnfStyleReader.Parse( oParam, ocnfStyle );

				oConditionalTableStyle.Merge( ocnfStyle );
			}
		}
		RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
		if( NULL != oResultStyle )
			oOutputProperty.Merge( oResultStyle->m_oCellProp );
		oConditionalTableStyle.ApplyTableStyleToCellBorder( oParam.poTableStyle, oOutputProperty, nCurCell, nCellCount, nCurRow, nRowCount );

		if( m_ooxTableCellProps->m_oHMerge.IsInit() && m_ooxTableCellProps->m_oHMerge->m_oVal.IsInit())
		{
			switch(m_ooxTableCellProps->m_oHMerge->m_oVal->GetValue())
			{
			case SimpleTypes::mergeContinue : oOutputProperty.m_bMerge = 1;			break;
			case SimpleTypes::mergeRestart  : oOutputProperty.m_bMergeFirst = 1;	break;
			}
		}
		if( m_ooxTableCellProps->m_oVMerge.IsInit() && m_ooxTableCellProps->m_oVMerge->m_oVal.IsInit() )
		{
			if(m_ooxTableCellProps->m_oVMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart)
				oOutputProperty.m_bMergeFirstVertical = 1;
			else 
				oOutputProperty.m_bMergeVertical = 1;
		}
		if( m_ooxTableCellProps->m_oTcFitText.IsInit())
			oOutputProperty.m_bFitText = m_ooxTableCellProps->m_oTcFitText->m_oVal.ToBool() ? 1 : 0;
	
		if( m_ooxTableCellProps->m_oNoWrap.IsInit())
			oOutputProperty.m_bNoWrap= m_ooxTableCellProps->m_oNoWrap->m_oVal.ToBool() ? 1 : 0;

		if( m_ooxTableCellProps->m_oTcMar.IsInit() )
		{
			//todooo сделать реализацию с процентами
			if( m_ooxTableCellProps->m_oTcMar->m_oBottom.IsInit() && m_ooxTableCellProps->m_oTcMar->m_oBottom->m_oW.IsInit())
			{
				oOutputProperty.m_nPaddingBottom = m_ooxTableCellProps->m_oTcMar->m_oBottom->m_oW->GetValue();
				oOutputProperty.m_nIsPaddingBottom = 3;
			}
			if( m_ooxTableCellProps->m_oTcMar->m_oEnd.IsInit() && m_ooxTableCellProps->m_oTcMar->m_oEnd->m_oW.IsInit())
			{
				oOutputProperty.m_nPaddingRight = m_ooxTableCellProps->m_oTcMar->m_oEnd->m_oW->GetValue();
				oOutputProperty.m_nIsPaddingRight = 3;
			}
			if( m_ooxTableCellProps->m_oTcMar->m_oStart.IsInit() && m_ooxTableCellProps->m_oTcMar->m_oStart->m_oW.IsInit())
			{
				oOutputProperty.m_nPaddingLeft = m_ooxTableCellProps->m_oTcMar->m_oStart->m_oW->GetValue();
				oOutputProperty.m_nIsPaddingLeft = 3;
			}
			if( m_ooxTableCellProps->m_oTcMar->m_oTop.IsInit() && m_ooxTableCellProps->m_oTcMar->m_oTop->m_oW.IsInit())
			{
				oOutputProperty.m_nPaddingTop = m_ooxTableCellProps->m_oTcMar->m_oTop->m_oW->GetValue();
				oOutputProperty.m_nIsPaddingTop = 3;
			}
		}

		if( m_ooxTableCellProps->m_oTcW.IsInit() && m_ooxTableCellProps->m_oTcW->m_oW.IsInit()) 
		{
			oOutputProperty.m_nWidth		= m_ooxTableCellProps->m_oTcW->m_oW->GetValue();
			oOutputProperty.m_eWidthUnits	= m_ooxTableCellProps->m_oTcW->m_oW->IsPercent() ? mu_Percent : mu_Twips;
		}
		if( m_ooxTableCellProps->m_oHideMark.IsInit())
			oOutputProperty.m_bHideMark = m_ooxTableCellProps->m_oHideMark->m_oVal.ToBool() ? 1 : 0;
		
		if( m_ooxTableCellProps->m_oTcBorders.IsInit() )
		{
			if( m_ooxTableCellProps->m_oTcBorders->m_oTL2BR.IsInit())
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oTL2BR.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderDiagonalLR  );
			}
			if(m_ooxTableCellProps->m_oTcBorders->m_oTR2BL.IsInit() )
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oTR2BL.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderDiagonalRL  );
			}
			if(m_ooxTableCellProps->m_oTcBorders->m_oStart.IsInit() )
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oStart.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderLeft  );
			}
			if(m_ooxTableCellProps->m_oTcBorders->m_oTop.IsInit() )
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oTop.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderTop  );
			}
			if(m_ooxTableCellProps->m_oTcBorders->m_oEnd.IsInit() )
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oEnd.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderRight  );
			}
			if(m_ooxTableCellProps->m_oTcBorders->m_oBottom.IsInit() )
			{
				OOXBorderReader oBorderReader(m_ooxTableCellProps->m_oTcBorders->m_oBottom.GetPointer());
				oBorderReader.Parse( oParam,oOutputProperty.m_oBorderBottom  );
			}
		}
		if( m_ooxTableCellProps->m_oShd.IsInit())
		{
			OOXShadingReader oShadingReader(m_ooxTableCellProps->m_oShd.GetPointer());
			oShadingReader.Parse( oParam,oOutputProperty.m_oShading  );
		}
		if( m_ooxTableCellProps->m_oVAlign.IsInit() && m_ooxTableCellProps->m_oVAlign->m_oVal.IsInit())
		{
			switch(m_ooxTableCellProps->m_oVAlign->m_oVal->GetValue())
			{
				case SimpleTypes::verticaljcBoth   : break;
				case SimpleTypes::verticaljcBottom : oOutputProperty.m_eAlign = RtfCellProperty::ca_Bottom; break;
				case SimpleTypes::verticaljcCenter : oOutputProperty.m_eAlign = RtfCellProperty::ca_Center; break;
				case SimpleTypes::verticaljcTop    : oOutputProperty.m_eAlign = RtfCellProperty::ca_Top;	break;
			}
		}
		if( m_ooxTableCellProps->m_oTextDirection.IsInit() && m_ooxTableCellProps->m_oTextDirection->m_oVal.IsInit())
		{
			switch(m_ooxTableCellProps->m_oTextDirection->m_oVal->GetValue())
			{
				case SimpleTypes::textdirectionLr  : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_lrtb;
				case SimpleTypes::textdirectionLrV : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_lrtbv;
				case SimpleTypes::textdirectionRl  : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_tbrl;
				case SimpleTypes::textdirectionRlV : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_tbrlv;
				case SimpleTypes::textdirectionTb  : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_btlr; //??
				case SimpleTypes::textdirectionTbV : oOutputProperty.m_oCellFlow = RtfCellProperty::cf_btlr; //??
			}
		}
		if (m_ooxTableCellProps->m_oGridSpan.IsInit() && m_ooxTableCellProps->m_oGridSpan->m_oVal.IsInit())
			oOutputProperty.m_nSpan = m_ooxTableCellProps->m_oGridSpan->m_oVal->GetValue();

		return true;
	}
};