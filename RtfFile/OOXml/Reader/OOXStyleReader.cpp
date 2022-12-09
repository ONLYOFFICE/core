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

#include "OOXtblPrReader.h"
#include "OOXtcPrReader.h"
#include "OOXrPrReader.h"

#include "OOXStyleReader.h"

OOXStyleReader::OOXStyleReader(OOX::CStyle    *ooxStyle)
{
	m_ooxStyle = ooxStyle;
}
bool OOXStyleReader::ParseStyle( ReaderParameter oParam, RtfStylePtr& oOutputStyle)
{
	if (m_ooxStyle == NULL) return false;

	RtfStylePtr oNewStyle;

	bool bDefault = m_ooxStyle->m_oDefault.IsInit() ? m_ooxStyle->m_oDefault->ToBool() : false;

	if (m_ooxStyle->m_oType.IsInit() == false) return false;

	switch(m_ooxStyle->m_oType->GetValue())
	{
	case SimpleTypes::styletypeCharacter : oNewStyle = RtfStylePtr( new RtfCharStyle() );		break;
		//case styletypeNumbering : break;
	case SimpleTypes::styletypeParagraph : oNewStyle = RtfStylePtr( new RtfParagraphStyle() );	break;
	case SimpleTypes::styletypeTable     : oNewStyle = RtfStylePtr( new RtfTableStyle() );		break;
	}

	if (oNewStyle == NULL) return true;

	RtfStyle::_StyleType eStyleType = oNewStyle->m_eType;

	oNewStyle->m_nID = oParam.oRtf->m_oStyleTable.GetCount() + 1;

	oNewStyle->m_sID = m_ooxStyle->m_sStyleId.get_value_or(L"");

	if(m_ooxStyle->m_oName.IsInit() && m_ooxStyle->m_oName->m_sVal.IsInit())
		oNewStyle->m_sName = *m_ooxStyle->m_oName->m_sVal;

	if (m_ooxStyle->m_oHidden.IsInit())
		oNewStyle->m_bHidden = m_ooxStyle->m_oHidden->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oLocked.IsInit())
		oNewStyle->m_bLocked = m_ooxStyle->m_oLocked->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oSemiHidden.IsInit())
		oNewStyle->m_nSemiHidden = m_ooxStyle->m_oSemiHidden->m_oVal.ToBool();

	if (m_ooxStyle->m_oQFormat.IsInit())
		oNewStyle->m_bQFormat = m_ooxStyle->m_oQFormat->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oUnhideWhenUsed.IsInit())
		oNewStyle->m_bUnhiddenWhenUse = m_ooxStyle->m_oUnhideWhenUsed->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oPersonal.IsInit())
		oNewStyle->m_bPersonal = m_ooxStyle->m_oPersonal->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oPersonalReply.IsInit())
		oNewStyle->m_bReply = m_ooxStyle->m_oPersonalReply->m_oVal.ToBool() ? 1 : 0;
	
	if (m_ooxStyle->m_oPersonalCompose.IsInit())
		oNewStyle->m_bCompose = m_ooxStyle->m_oPersonalCompose->m_oVal.ToBool() ? 1 : 0;

	if (m_ooxStyle->m_oUiPriority.IsInit())
		oNewStyle->m_bCompose = *m_ooxStyle->m_oUiPriority->m_oVal;

	if (m_ooxStyle->m_oRunPr.IsInit() && ( RtfStyle::stCharacter == eStyleType || RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ))//todo с rtf стилями
	{
		RtfCharStylePtr oNewCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oNewStyle );

		OOXrPrReader orPrReader(m_ooxStyle->m_oRunPr.GetPointer());
		orPrReader.m_bDefStyle = false;

		orPrReader.Parse( oParam, oNewCharStyle->m_oCharProp );
	}
	if (m_ooxStyle->m_oParPr.IsInit()  && ( RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ) )
	{
		RtfParagraphStylePtr oNewParStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oNewStyle );

		OOXpPrReader opPrReader(m_ooxStyle->m_oParPr.GetPointer());
		opPrReader.m_bDefStyle = false;

		CcnfStyle style;
		opPrReader.Parse( oParam, oNewParStyle->m_oParProp, style );

		//oNewParStyle->m_oParProp.m_nListId	= PROP_DEF; //экспериментально вроде нельзя иметь numbering в параграф стиле
		//oNewParStyle->m_oParProp.m_nListLevel = PROP_DEF;

	}
	if (m_ooxStyle->m_oTblPr.IsInit()  && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewStyle );

		OOXtblPrReader otblPrReader(m_ooxStyle->m_oTblPr.GetPointer());
		otblPrReader.m_bDefStyle = false;

		otblPrReader.Parse( oParam, oNewTableStyle->m_oTableProp );
	}
	if (m_ooxStyle->m_oTrPr.IsInit() && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewStyle );

		OOXtrPrReader otrPrReader(m_ooxStyle->m_oTrPr.GetPointer());

		CcnfStyle style;
		otrPrReader.Parse( oParam, oNewTableStyle->m_oRowProp, style );
	}
	if (m_ooxStyle->m_oTcPr.IsInit() && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewStyle );

		OOXtcPrReader otcPrReader(m_ooxStyle->m_oTcPr.GetPointer(), m_ooxStyle->m_oTblPr.GetPointer());

		CcnfStyle style;
		otcPrReader.Parse( oParam, oNewTableStyle->m_oCellProp, style, -1, -1, -1, -1 );
	}
	if (m_ooxStyle->m_arrTblStylePr.size() > 0  && RtfStyle::stTable == eStyleType )
	{
		for (size_t i = 0; i < m_ooxStyle->m_arrTblStylePr.size(); i++)
		{
			RtfTableStylePtr oTableStyle =boost::static_pointer_cast<RtfTableStyle, RtfStyle>(oNewStyle);

			RtfStylePtr oNewSubStyle ;
			OOXStyleReader oStyleReader(m_ooxStyle);

			oStyleReader.ParseTableStyle(m_ooxStyle->m_arrTblStylePr[i], oParam, oNewSubStyle);

			oNewSubStyle->m_sName = oNewStyle->m_sName;

			if( /*true == oNewSubStyle->IsValid() && */RtfStyle::stTable == oNewSubStyle->m_eType )
			{
				oNewSubStyle->m_nID = oParam.oRtf->m_oStyleTable.GetCount() + 1;

				if (m_ooxStyle->m_arrTblStylePr[i]->m_oType.IsInit())
				{
					switch(m_ooxStyle->m_arrTblStylePr[i]->m_oType->GetValue())
					{
					case SimpleTypes::tblstyleoverridetypeBand1Horz  :
					{
						oTableStyle->m_oBandHorOdd = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oBandHorOdd->m_oParProp.m_bStyleOddRowBand = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeBand1Vert  :
					{
						oTableStyle->m_oBandVerOdd = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oBandVerOdd->m_oParProp.m_bStyleOddColBand = 1; //для конвертации в Rtf
					}break;
					case SimpleTypes::tblstyleoverridetypeBand2Horz  :
					{
						oTableStyle->m_oBandHorEven = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oBandHorEven->m_oParProp.m_bStyleEvenRowBand = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeBand2Vert  :
					{
						oTableStyle->m_oBandVerEven = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oBandVerEven->m_oParProp.m_bStyleEvenColBand = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeFirstCol   :
					{
						oTableStyle->m_oFirstCol = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oFirstCol->m_oParProp.m_bStyleFirstCollumn = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeFirstRow   :
					{
						oTableStyle->m_oFirstRow = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oFirstRow->m_oParProp.m_bStyleFirstRow = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeLastCol    :
					{
						oTableStyle->m_oLastCol = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oLastCol->m_oParProp.m_bStyleLastCollumn = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeLastRow    :
					{
						oTableStyle->m_oLastRow = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oLastRow->m_oParProp.m_bStyleLastRow = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeNeCell     :
					{
						oTableStyle->m_oNECell = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oNECell->m_oParProp.m_bStyleNECell = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeNwCell     :
					{
						oTableStyle->m_oNWCell = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oNWCell->m_oParProp.m_bStyleNWCell = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeSeCell     :
					{
						oTableStyle->m_oSECell = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oSECell->m_oParProp.m_bStyleSECell = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeSwCell     :
					{
						oTableStyle->m_oSWCell = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewSubStyle );
						oTableStyle->m_oSWCell->m_oParProp.m_bStyleSWCell = 1;
					}break;
					case SimpleTypes::tblstyleoverridetypeWholeTable :
					{
					}break;
					}
				}
			}
		}
	}
	if( true == oNewStyle->IsValid() )
	{
		if( true == bDefault )
		{
			if( RtfStyle::stCharacter == eStyleType && NULL == oParam.oReader->m_oDefCharStyle )
				oParam.oReader->m_oDefCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oNewStyle );
			else if( RtfStyle::stParagraph == eStyleType && NULL == oParam.oReader->m_oDefParStyle )
				oParam.oReader->m_oDefParStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oNewStyle );
			else if( RtfStyle::stTable == eStyleType && NULL == oParam.oReader->m_oDefTableStyle )
				oParam.oReader->m_oDefTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oNewStyle );
		}
		oOutputStyle = oNewStyle;
		return true;
	}
	return false;
}
bool OOXStyleReader::ParseTableStyle(OOX::Logic::CTableStyleProperties * oox_tableStyle, ReaderParameter oParam, RtfStylePtr& oOutputStyle)
{
	if (oox_tableStyle == NULL) return false;

	if (oOutputStyle == NULL)
	{
		oOutputStyle		= RtfStylePtr ( new RtfTableStyle() );
		oOutputStyle->m_nID = oParam.oRtf->m_oStyleTable.GetCount() + 1;
	}
	RtfStyle::_StyleType eStyleType = oOutputStyle->m_eType;//todooo проверить

	if (oox_tableStyle->m_oRunPr.IsInit() &&
			( RtfStyle::stCharacter == eStyleType || RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ))//todo с rtf стилями
	{
		RtfCharStylePtr oNewCharStyle = boost::static_pointer_cast<RtfCharStyle, RtfStyle>( oOutputStyle );

		OOXrPrReader orPrReader(oox_tableStyle->m_oRunPr.GetPointer());
		orPrReader.m_bDefStyle = false;

		orPrReader.Parse( oParam, oNewCharStyle->m_oCharProp );
	}
	if (oox_tableStyle->m_oParPr.IsInit()  && ( RtfStyle::stParagraph == eStyleType || RtfStyle::stTable == eStyleType ) )
	{
		RtfParagraphStylePtr oNewParStyle = boost::static_pointer_cast<RtfParagraphStyle, RtfStyle>( oOutputStyle );

		OOXpPrReader opPrReader(oox_tableStyle->m_oParPr.GetPointer());
		opPrReader.m_bDefStyle = false;

		CcnfStyle style;

		opPrReader.Parse( oParam, oNewParStyle->m_oParProp, style );

		oNewParStyle->m_oParProp.m_nListId = PROP_DEF; //экспериментально вроде нельзя иметь numbering в параграф стиле
		oNewParStyle->m_oParProp.m_nListLevel = PROP_DEF;

	}
	if (oox_tableStyle->m_oTblPr.IsInit()  && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oOutputStyle );

		OOXtblPrReader otblPrReader(oox_tableStyle->m_oTblPr.GetPointer());
		otblPrReader.m_bDefStyle = false;

		otblPrReader.Parse( oParam, oNewTableStyle->m_oTableProp );
	}
	if (oox_tableStyle->m_oTrPr.IsInit() && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oOutputStyle );

		OOXtrPrReader otrPrReader(oox_tableStyle->m_oTrPr.GetPointer());

		CcnfStyle style;
		otrPrReader.Parse( oParam, oNewTableStyle->m_oRowProp, style );
	}
	if (oox_tableStyle->m_oTcPr.IsInit() && RtfStyle::stTable == eStyleType )
	{
		RtfTableStylePtr oNewTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oOutputStyle );

		OOXtcPrReader otcPrReader(oox_tableStyle->m_oTcPr.GetPointer(), oox_tableStyle->m_oTblPr.GetPointer());

		CcnfStyle style;
		otcPrReader.Parse( oParam, oNewTableStyle->m_oCellProp, style, -1, -1, -1, -1 );
	}
	return true;
}
bool OOXStyleReader::ParseRelations( ReaderParameter oParam )
{
	if (m_ooxStyle == NULL) return false;
	if (m_ooxStyle->m_sStyleId.IsInit() == false) return false;

	RtfStylePtr oCurStyle ;
	std::wstring sStyleName = *m_ooxStyle->m_sStyleId;

	if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyleName, oCurStyle) )
	{
		if (m_ooxStyle->m_oBasedOn.IsInit())
		{
			RtfStylePtr oBaseStyle;
			if( true == oParam.oRtf->m_oStyleTable.GetStyle(*m_ooxStyle->m_oBasedOn->m_sVal, oBaseStyle) )
				oCurStyle->m_nBasedOn = oBaseStyle->m_nID;
		}
		if (m_ooxStyle->m_oNext.IsInit())
		{
			RtfStylePtr oNextStyle;
			if( true == oParam.oRtf->m_oStyleTable.GetStyle(*m_ooxStyle->m_oNext->m_sVal, oNextStyle) )
				oCurStyle->m_nNext = oNextStyle->m_nID;
		}
		if (m_ooxStyle->m_oLink.IsInit())
		{
			RtfStylePtr oLinkStyle;
			if( true == oParam.oRtf->m_oStyleTable.GetStyle(*m_ooxStyle->m_oLink->m_sVal, oLinkStyle) )
				oCurStyle->m_nLink = oLinkStyle->m_nID;
		}
	}
	return true;
}
