#pragma once

#include "../../../../Common/DocxFormat/Source/Common/ComplexTypes.h"

class OOXShadingReader
{
private:
	ComplexTypes::Word::CShading * m_ooxShading;
public: 
	OOXShadingReader(ComplexTypes::Word::CShading * ooxShading)
	{
		m_ooxShading = ooxShading;
	}
	bool Parse( ReaderParameter oParam, RtfShading& oOutputShading )
	{	
		if (m_ooxShading == NULL) return false;
		
		if (m_ooxShading->m_oColor.IsInit())
		{
			RtfColor oColor(m_ooxShading->m_oColor->Get_R(), m_ooxShading->m_oColor->Get_G(), m_ooxShading->m_oColor->Get_B());
			oOutputShading.m_nForeColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
		if (m_ooxShading->m_oFill.IsInit())
		{
			RtfColor oColor(m_ooxShading->m_oFill->Get_R(), m_ooxShading->m_oFill->Get_G(), m_ooxShading->m_oFill->Get_B());
			oOutputShading.m_nBackColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
		}
		if (m_ooxShading->m_oVal.IsInit())
		{
            switch(m_ooxShading->m_oVal->GetValue())
			{
			case SimpleTypes::shdClear                 : oOutputShading.m_nValue = 0;	break;
			case SimpleTypes::shdDiagCross             : oOutputShading.m_eType = RtfShading::st_chbgdkdcross;	break;
			case SimpleTypes::shdDiagStripe            : oOutputShading.m_eType = RtfShading::st_chbgdkbdiag;	break;
			case SimpleTypes::shdHorzCross             : oOutputShading.m_eType = RtfShading::st_chbgdkcross;	break;
			case SimpleTypes::shdHorzStripe            : oOutputShading.m_eType = RtfShading::st_chbgdkhoriz;	break;
			case SimpleTypes::shdNil                   : oOutputShading.m_nValue = 0;	break;
			case SimpleTypes::shdPct10                 : oOutputShading.m_nValue = 10;	break;;
			case SimpleTypes::shdPct12                 : oOutputShading.m_nValue = 12;	break;
			case SimpleTypes::shdPct15                 : oOutputShading.m_nValue = 15;	break;
			case SimpleTypes::shdPct20                 : oOutputShading.m_nValue = 20;	break;
			case SimpleTypes::shdPct25                 : oOutputShading.m_nValue = 25;	break;
			case SimpleTypes::shdPct30                 : oOutputShading.m_nValue = 30;	break;
			case SimpleTypes::shdPct35                 : oOutputShading.m_nValue = 35;	break;
			case SimpleTypes::shdPct37                 : oOutputShading.m_nValue = 37;	break;
			case SimpleTypes::shdPct40                 : oOutputShading.m_nValue = 40;	break;
			case SimpleTypes::shdPct45                 : oOutputShading.m_nValue = 45;	break;
			case SimpleTypes::shdPct5                  : oOutputShading.m_nValue = 5;	break;
			case SimpleTypes::shdPct55                 : oOutputShading.m_nValue = 55;	break;
			case SimpleTypes::shdPct60                 : oOutputShading.m_nValue = 60;	break;
			case SimpleTypes::shdPct62                 : oOutputShading.m_nValue = 62;	break;
			case SimpleTypes::shdPct65                 : oOutputShading.m_nValue = 65;	break;
			case SimpleTypes::shdPct70                 : oOutputShading.m_nValue = 70;	break;
			case SimpleTypes::shdPct75                 : oOutputShading.m_nValue = 75;	break;
			case SimpleTypes::shdPct80                 : oOutputShading.m_nValue = 80;	break;
			case SimpleTypes::shdPct85                 : oOutputShading.m_nValue = 85;	break;
			case SimpleTypes::shdPct87                 : oOutputShading.m_nValue = 87;	break;
			case SimpleTypes::shdPct90                 : oOutputShading.m_nValue = 90;	break;
			case SimpleTypes::shdPct95                 : oOutputShading.m_nValue = 95;	break;
			case SimpleTypes::shdReverseDiagStripe     : oOutputShading.m_eType = RtfShading::st_chbgdkfdiag;	break;
			case SimpleTypes::shdSolid                 : oOutputShading.m_nValue = 100;	break;
			case SimpleTypes::shdThinDiagCross         : oOutputShading.m_eType = RtfShading::st_chbgdcross;	break;
			case SimpleTypes::shdThinDiagStripe        : oOutputShading.m_eType = RtfShading::st_chbgbdiag;	break;
			case SimpleTypes::shdThinHorzCross         : oOutputShading.m_eType = RtfShading::st_chbgcross;	break;
			case SimpleTypes::shdThinHorzStripe        : oOutputShading.m_eType = RtfShading::st_chbghoriz;	break;
			case SimpleTypes::shdThinReverseDiagStripe : oOutputShading.m_eType = RtfShading::st_chbgfdiag;	break;
			case SimpleTypes::shdThinVertStripe        : oOutputShading.m_eType = RtfShading::st_chbgvert;	break;
			case SimpleTypes::shdVertStripe            : oOutputShading.m_eType = RtfShading::st_chbgdkvert;	break;
			}
		}
		return true;
	}
};
