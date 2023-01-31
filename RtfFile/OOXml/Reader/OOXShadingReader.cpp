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

#include "OOXShadingReader.h"

OOXShadingReader::OOXShadingReader(ComplexTypes::Word::CShading * ooxShading)
{
	m_ooxShading = ooxShading;
}
bool OOXShadingReader::Parse( ReaderParameter oParam, RtfShading& oOutputShading )
{
	if (m_ooxShading == NULL) return false;

	if ((m_ooxShading->m_oColor.IsInit()) && (m_ooxShading->m_oColor->GetValue() == SimpleTypes::hexcolorRGB))
	{
		RtfColor oColor(m_ooxShading->m_oColor->Get_R(), m_ooxShading->m_oColor->Get_G(), m_ooxShading->m_oColor->Get_B());
		oOutputShading.m_nForeColor = oParam.oRtf->m_oColorTable.AddItem( oColor );
	}
	if ((m_ooxShading->m_oFill.IsInit()) && (m_ooxShading->m_oFill->GetValue() == SimpleTypes::hexcolorRGB))
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
