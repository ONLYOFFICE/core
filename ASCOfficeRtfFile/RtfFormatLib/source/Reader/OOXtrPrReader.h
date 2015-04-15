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
			oOutputProperty.m_bIsHeader = m_ooxTableRowProps->m_oTblHeader->m_oVal.ToBool();
		
		if (m_ooxTableRowProps->m_oCantSplit.IsInit() )
			 oOutputProperty.m_bIsHeader= m_ooxTableRowProps->m_oTblHeader->m_oVal.ToBool();
		
		if (m_ooxTableRowProps->m_oJc.IsInit() && m_ooxTableRowProps->m_oJc->m_oVal.IsInit())
		{
			switch(m_ooxTableRowProps->m_oJc->m_oVal->GetValue())
			{
				case SimpleTypes::jctableCenter : oOutputProperty.m_eJust = RtfRowProperty::rj_trqc; break;
				case SimpleTypes::jctableEnd    : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
				case SimpleTypes::jctableStart  : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
				case SimpleTypes::jctableLeft   : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
				case SimpleTypes::jctableRight  : oOutputProperty.m_eJust = RtfRowProperty::rj_trql; break;
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
