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
#include "OOXReader.h"
#include "OOXReaderBasic.h"
#include "OOXcnfStyleReader.h"

#include "../RtfDocument.h"

class OOXtrPrReader
{
private:
	OOX::Logic::CTableRowProperties*	m_ooxTableRowProps;
public: 
	OOXtrPrReader(OOX::Logic::CTableRowProperties*	ooxTableRowProps)
	{
		m_ooxTableRowProps = ooxTableRowProps;
	}
	
	bool Parse( ReaderParameter oParam , RtfRowProperty& oOutputProperty, CcnfStyle& oConditionalTableStyle)
	{
		if (m_ooxTableRowProps == NULL) return false;

		//ищем cnfStyle  и применяем внешний стиль
		if( NULL != oParam.poTableStyle )
		{
			if( m_ooxTableRowProps->m_oCnfStyle.IsInit() )
			{
				CcnfStyle ocnfStyle;
				OOXcnfStyleReader ocnfStyleReader(m_ooxTableRowProps->m_oCnfStyle.GetPointer());
				ocnfStyleReader.Parse( oParam, ocnfStyle );

				oConditionalTableStyle.Merge( ocnfStyle );
			}
		}
		RtfTableStylePtr oResultStyle = oConditionalTableStyle.ApplyTableStyle( oParam.poTableStyle );
		if( NULL != oResultStyle )
			oOutputProperty.Merge( oResultStyle->m_oRowProp );

		if (m_ooxTableRowProps->m_oTblHeader.IsInit() )
			oOutputProperty.m_bIsHeader = m_ooxTableRowProps->m_oTblHeader->m_oVal.ToBool() ? 1 : 0;
		
//todooo непонятнка
//		if (m_ooxTableRowProps->m_oCantSplit.IsInit() )
//             oOutputProperty.m_bIsHeader= m_ooxTableRowProps->m_oCantSplit->m_oVal.ToBool() ? 1 : 0;
		
		if (m_ooxTableRowProps->m_oJc.IsInit() && m_ooxTableRowProps->m_oJc->m_oVal.IsInit())
		{
			switch(m_ooxTableRowProps->m_oJc->m_oVal->GetValue())
			{
				case SimpleTypes::jctableCenter : oOutputProperty.m_eJust = RtfRowProperty::rj_trqc; break;
				case SimpleTypes::jctableEnd    : oOutputProperty.m_eJust = RtfRowProperty::rj_trqr; break;
				case SimpleTypes::jctableStart  : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
				case SimpleTypes::jctableLeft   : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
				case SimpleTypes::jctableRight  : oOutputProperty.m_eJust = RtfRowProperty::rj_trqr; break;
			}
		}

		if( m_ooxTableRowProps->m_oTblHeight.IsInit() && m_ooxTableRowProps->m_oTblHeight->m_oVal.IsInit())
		{
			oOutputProperty.m_nHeight = m_ooxTableRowProps->m_oTblHeight->m_oVal->ToTwips();
			
			if (m_ooxTableRowProps->m_oTblHeight->m_oHRule.IsInit() && m_ooxTableRowProps->m_oTblHeight->m_oHRule->GetValue() == SimpleTypes::heightruleExact)
				oOutputProperty.m_nHeight = -oOutputProperty.m_nHeight;
		}
		if( m_ooxTableRowProps->m_oWBefore.IsInit() && m_ooxTableRowProps->m_oWBefore->m_oW.IsInit())
		{
			oOutputProperty.m_nWidthStartInvCell = m_ooxTableRowProps->m_oWBefore->m_oW->GetValue();
			oOutputProperty.m_eMUStartInvCell = MetricUnits::mu_Twips;
		}
		if( m_ooxTableRowProps->m_oWAfter.IsInit() && m_ooxTableRowProps->m_oWAfter->m_oW.IsInit())
		{
			oOutputProperty.m_nWidthEndInvCell  = m_ooxTableRowProps->m_oWAfter->m_oW->GetValue();
			oOutputProperty.m_eMUEndInvCell = MetricUnits::mu_Twips;
		}

		if( m_ooxTableRowProps->m_oGridBefore.IsInit() && m_ooxTableRowProps->m_oGridBefore->m_oVal.IsInit())
			oOutputProperty.m_nGridBefore = m_ooxTableRowProps->m_oGridBefore->m_oVal->GetValue();

		if( m_ooxTableRowProps->m_oGridAfter.IsInit() && m_ooxTableRowProps->m_oGridAfter->m_oVal.IsInit())
			oOutputProperty.m_nGridAfter = m_ooxTableRowProps->m_oGridAfter->m_oVal->GetValue();
		
		return true;
	}

};
