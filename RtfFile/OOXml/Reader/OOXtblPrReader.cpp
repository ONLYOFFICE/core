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

#include "OOXReader.h"
#include "OOXBorderReader.h"
#include "OOXShadingReader.h"
#include "OOXtblpPrReader.h"
#include "OOXtblLookReader.h"
#include "OOXtcPrReader.h"

#include "OOXtblPrReader.h"

OOXtblPrReader::OOXtblPrReader(OOX::Logic::CTableProperty* ooxTableProps)
{
	m_bDefStyle = true;
	m_ooxTableProps = ooxTableProps;
}
bool OOXtblPrReader::Parse( ReaderParameter oParam , RtfTableProperty& oOutputProperty)
{
	if (m_ooxTableProps == NULL) return false;

	//сначала применяем стили

	//применяем стиль по-умолчанию,если не определен свой
	if( m_ooxTableProps->m_oTblStyle.IsInit() == false && true == m_bDefStyle && NULL != oParam.oReader->m_oDefTableStyle )
	{
		oOutputProperty.Merge( oParam.oReader->m_oDefTableStyle->m_oTableProp );
		oOutputProperty.m_nStyle = oParam.oReader->m_oDefTableStyle->m_nID;
	}

	if(m_ooxTableProps->m_oTblStyle.IsInit() && m_ooxTableProps->m_oTblStyle->m_sVal.IsInit())
	{
		std::wstring sStyle = *m_ooxTableProps->m_oTblStyle->m_sVal;
		RtfStylePtr oStyle;
		if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyle, oStyle) )
		{
			RtfStylePtr oResultStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oStyle );
			RtfTableStylePtr oTableStyle = boost::static_pointer_cast<RtfTableStyle, RtfStyle>( oResultStyle );

			oOutputProperty.Merge( oTableStyle->m_oTableProp );
			oOutputProperty.m_nStyle = oTableStyle->m_nID;
		}
	}
	OOXtcPrReader::Parse(m_ooxTableProps->m_oTblInd.GetPointer(), oOutputProperty.eTableIndentUnit, oOutputProperty.nTableIndent, false);
	oOutputProperty.m_nLeft = oOutputProperty.nTableIndent;

	if( m_ooxTableProps->m_oTblLayout.IsInit() && m_ooxTableProps->m_oTblLayout->m_oType.IsInit())
	{
		if( m_ooxTableProps->m_oTblLayout->m_oType->GetValue() ==  SimpleTypes::tbllayouttypeAutofit)	oOutputProperty.m_nAutoFit = 1;
		if( m_ooxTableProps->m_oTblLayout->m_oType->GetValue() ==  SimpleTypes::tbllayouttypeFixed)		oOutputProperty.m_nAutoFit = 0;
	}
	if(m_ooxTableProps->m_oTblOverlap.IsInit() && m_ooxTableProps->m_oTblOverlap->m_oVal.IsInit())
	{
		if(m_ooxTableProps->m_oTblOverlap->m_oVal->GetValue() ==  SimpleTypes::tbloverlapOverlap)
			oOutputProperty.m_bOverlap = 1;
	}
	if( m_ooxTableProps->m_oTblpPr.IsInit() )
	{
		OOXtblpPrReader otblpPrReader(m_ooxTableProps->m_oTblpPr.GetPointer());
		otblpPrReader.Parse( oParam, oOutputProperty );
	}
	if( (m_ooxTableProps->m_oJc.IsInit()) && (m_ooxTableProps->m_oJc->m_oVal.IsInit()))
	{
		switch(m_ooxTableProps->m_oJc->m_oVal->GetValue())
		{
		case SimpleTypes::jctableCenter		: oOutputProperty.m_eJust = RtfTableProperty::rj_trqc;break;
		case SimpleTypes::jctableEnd		:
		case SimpleTypes::jctableRight		: oOutputProperty.m_eJust = RtfTableProperty::rj_trqr;break;
		case SimpleTypes::jctableStart		:
		case SimpleTypes::jctableLeft		: oOutputProperty.m_eJust = RtfTableProperty::rj_trql;break;
		}
	}
	if( m_ooxTableProps->m_oTblBorders.IsInit())
	{
		if( m_ooxTableProps->m_oTblBorders->m_oTop.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oTop.GetPointer());
			oBorderReader.Parse( oParam,oOutputProperty.m_oBorderTop);
		}
		if( m_ooxTableProps->m_oTblBorders->m_oStart.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oStart.GetPointer());
			oBorderReader.Parse(oParam,oOutputProperty.m_oBorderLeft);
		}
		if( m_ooxTableProps->m_oTblBorders->m_oBottom.IsInit())
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oBottom.GetPointer());
			oBorderReader.Parse(oParam,oOutputProperty.m_oBorderBottom);
		}
		if( m_ooxTableProps->m_oTblBorders->m_oEnd.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oEnd.GetPointer());
			oBorderReader.Parse(oParam,oOutputProperty.m_oBorderRight);
		}
		if( m_ooxTableProps->m_oTblBorders->m_oInsideH.IsInit())
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oInsideH.GetPointer());
			oBorderReader.Parse(oParam,oOutputProperty.m_oBorderHor);
		}
		if( m_ooxTableProps->m_oTblBorders->m_oInsideV.IsInit() )
		{
			OOXBorderReader oBorderReader(m_ooxTableProps->m_oTblBorders->m_oInsideV.GetPointer());
			oBorderReader.Parse(oParam,oOutputProperty.m_oBorderVert);
		}
	}
	if( m_ooxTableProps->m_oShade.IsInit())
	{
		OOXShadingReader oShadingReader(m_ooxTableProps->m_oShade.GetPointer());
		oShadingReader.Parse( oParam, oOutputProperty.m_oShading );
	}
	if( m_ooxTableProps->m_oTblCellMar.IsInit() )
	{
		OOXtcPrReader::Parse(m_ooxTableProps->m_oTblCellMar->m_oBottom.GetPointer(),	oOutputProperty.m_eDefCellMarBottomUnit,	oOutputProperty.m_nDefCellMarBottom,	false);
		OOXtcPrReader::Parse(m_ooxTableProps->m_oTblCellMar->m_oStart.GetPointer(),		oOutputProperty.m_eDefCellMarLeftUnit,		oOutputProperty.m_nDefCellMarLeft,		false);
		OOXtcPrReader::Parse(m_ooxTableProps->m_oTblCellMar->m_oEnd.GetPointer(),		oOutputProperty.m_eDefCellMarRightUnit,		oOutputProperty.m_nDefCellMarRight,		false);
		OOXtcPrReader::Parse(m_ooxTableProps->m_oTblCellMar->m_oTop.GetPointer(),		oOutputProperty.m_eDefCellMarTopUnit,		oOutputProperty.m_nDefCellMarTop,		false);

	}

	if( m_ooxTableProps->m_oTblCellSpacing.IsInit())
	{
		OOXtcPrReader::Parse(m_ooxTableProps->m_oTblCellSpacing.GetPointer(), oOutputProperty.m_eDefCellSpTopUnit, oOutputProperty.m_nDefCellSpTop, false);

		oOutputProperty.m_eDefCellSpBottomUnit	= oOutputProperty.m_eDefCellSpTopUnit;
		oOutputProperty.m_eDefCellSpLeftUnit	= oOutputProperty.m_eDefCellSpTopUnit;
		oOutputProperty.m_eDefCellSpRightUnit	= oOutputProperty.m_eDefCellSpTopUnit;

		oOutputProperty.m_nDefCellSpBottom	= oOutputProperty.m_nDefCellSpTop;
		oOutputProperty.m_nDefCellSpLeft	= oOutputProperty.m_nDefCellSpTop;
		oOutputProperty.m_nDefCellSpRight	= oOutputProperty.m_nDefCellSpTop;
	}
	if( m_ooxTableProps->m_oTblLook.IsInit())
	{
		OOXtblLookReader oLookReader(m_ooxTableProps->m_oTblLook.GetPointer());
		CtblLook oLook;
		oLookReader.Parse( oParam, oLook );

		if ( oLook.bFirstRow )	oOutputProperty.m_bAutoFirstRow = 1;
		if ( oLook.bLastRow )	oOutputProperty.m_bAutoLastRow = 1;
		if ( oLook.bFirstCol )	oOutputProperty.m_bAutoFirstCol = 1;
		if ( oLook.bLastCol )	oOutputProperty.m_bAutoLastCol = 1;
		if ( oLook.bNoHBand )	oOutputProperty.m_bAutoNoRowBand = 1;
		if ( oLook.bNoVBand )	oOutputProperty.m_bAutoNoColBand = 1;
	}
	if (m_ooxTableProps->m_oTblStyleRowBandSize.IsInit() && m_ooxTableProps->m_oTblStyleRowBandSize->m_oVal.IsInit())
		oOutputProperty.m_nRowBandSize = *m_ooxTableProps->m_oTblStyleRowBandSize->m_oVal;

	if (m_ooxTableProps->m_oTblStyleColBandSize.IsInit() && m_ooxTableProps->m_oTblStyleColBandSize->m_oVal.IsInit())
		oOutputProperty.m_nColBandSize = *m_ooxTableProps->m_oTblStyleColBandSize->m_oVal;

	OOXtcPrReader::Parse(m_ooxTableProps->m_oTblW.GetPointer(), oOutputProperty.m_eWidthUnit, oOutputProperty.m_nWidth, true );

	return true;
}
