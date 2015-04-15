#pragma once

#include "OOXReader.h"
#include "OOXBorderReader.h"
#include "OOXShadingReader.h"
#include "OOXtblpPrReader.h"
#include "OOXReaderBasic.h"
#include "OOXtblLookReader.h"

#include "../RtfDocument.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/Logic/TableProperty.h"

class OOXtblPrReader
{
private:
	OOX::Logic::CTableProperty* m_ooxTableProps;
public: 
	bool m_bDefStyle;
	OOXtblPrReader(OOX::Logic::CTableProperty* ooxTableProps)
	{
		m_bDefStyle = true;
		m_ooxTableProps = ooxTableProps;
	}
	bool Parse( ReaderParameter oParam , RtfTableProperty& oOutputProperty)
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
			CString sStyle = m_ooxTableProps->m_oTblStyle->m_sVal.get2();
			RtfStylePtr oStyle;
			if( true == oParam.oRtf->m_oStyleTable.GetStyle(sStyle, oStyle) )
			{
				RtfStylePtr oResultStyle = oParam.oRtf->m_oStyleTable.GetStyleResulting( oStyle );
				RtfTableStylePtr oTableStyle = boost::shared_static_cast<RtfTableStyle, RtfStyle>( oResultStyle );
				oOutputProperty = oTableStyle->m_oTableProp;
				oOutputProperty.m_nStyle = oTableStyle->m_nID;
			}
		}
		if(m_ooxTableProps->m_oTblInd.IsInit() )
		{
			SimpleTypes::ETblWidth eType = m_ooxTableProps->m_oTblInd->m_oType.IsInit() ? m_ooxTableProps->m_oTblInd->m_oType->GetValue() : SimpleTypes::tblwidthNil;
			double dValue = m_ooxTableProps->m_oTblInd->m_oW.IsInit() ? m_ooxTableProps->m_oTblInd->m_oW->GetValue() : 0;
		
			switch(eType)
			{
			case SimpleTypes::tblwidthDxa://сделаем не по документации, а как все остальные юниты !!!
				{
					oOutputProperty.nTableIndentUnits	= 3;
					oOutputProperty.nTableIndent		= dValue;
					oOutputProperty.m_nLeft				= oOutputProperty.nTableIndent;
				}break;
			case SimpleTypes::tblwidthPct:
				{
				//	oOutputProperty.nTableIndentUnits = 3;
				//	oOutputProperty.nTableIndent = Strings::ToInteger( sValue );
				}break;
			}
		}
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
			MetricUnits eMetric = mu_none;
			int nValue = PROP_DEF;
			//if( mu_Twips == eMetric && PROP_DEF != nValue )
			//todooo сделать вариант с процентми
			if( m_ooxTableProps->m_oTblCellMar->m_oBottom.IsInit() && m_ooxTableProps->m_oTblCellMar->m_oBottom->m_oW.IsInit())
			{
				oOutputProperty.m_nDefCellMarBottomUnits = 3;
				oOutputProperty.m_nDefCellMarBottom = m_ooxTableProps->m_oTblCellMar->m_oBottom->m_oW->GetValue();
			}
			if(m_ooxTableProps->m_oTblCellMar->m_oStart.IsInit() && m_ooxTableProps->m_oTblCellMar->m_oStart->m_oW.IsInit())
			{
				oOutputProperty.m_nDefCellMarLeftUnits = 3;
				oOutputProperty.m_nDefCellMarLeft = m_ooxTableProps->m_oTblCellMar->m_oStart->m_oW->GetValue();
			}
			if(m_ooxTableProps->m_oTblCellMar->m_oEnd.IsInit() && m_ooxTableProps->m_oTblCellMar->m_oEnd->m_oW.IsInit() )
			{
				oOutputProperty.m_nDefCellMarRightUnits = 3;
				oOutputProperty.m_nDefCellMarRight = m_ooxTableProps->m_oTblCellMar->m_oEnd->m_oW->GetValue();
			}
			if(m_ooxTableProps->m_oTblCellMar->m_oTop.IsInit() && m_ooxTableProps->m_oTblCellMar->m_oTop->m_oW.IsInit())
			{
				oOutputProperty.m_nDefCellMarTopUnits = 3;
				oOutputProperty.m_nDefCellMarTop = m_ooxTableProps->m_oTblCellMar->m_oTop->m_oW->GetValue();
			}
		}
		if( m_ooxTableProps->m_oTblCellSpacing.IsInit() && m_ooxTableProps->m_oTblCellSpacing->m_oW.IsInit())
		{
			//todooo тут тоже не было изначально варианта с процентами
			//mu_Twips == eMetric && PROP_DEF != nValue )
			{
				double nValue = m_ooxTableProps->m_oTblCellSpacing->m_oW->GetValue();

				oOutputProperty.m_nDefCellSpTopUnits	= 3;
				oOutputProperty.m_nDefCellSpBottomUnits = 3;
				oOutputProperty.m_nDefCellSpLeftUnits	= 3;
				oOutputProperty.m_nDefCellSpRightUnits	= 3;

				oOutputProperty.m_nDefCellSpTop		= nValue;
				oOutputProperty.m_nDefCellSpBottom	= nValue;
				oOutputProperty.m_nDefCellSpLeft	= nValue;
				oOutputProperty.m_nDefCellSpRight	= nValue;
			}
		}
		if( m_ooxTableProps->m_oTblLook.IsInit())
		{
			OOXtblLookReader oLookReader(m_ooxTableProps->m_oTblLook.GetPointer());
			CtblLook oLook;
			oLookReader.Parse( oParam, oLook );
			if( true == oLook.bFirstRow )
				oOutputProperty.m_bAutoFirstRow = 1;
			if( true == oLook.bLastRow )
				oOutputProperty.m_bAutoLastRow = 1;
			if( true == oLook.bFirstCol )
				oOutputProperty.m_bAutoFirstCol = 1;
			if( true == oLook.bLastCol )
				oOutputProperty.m_bAutoLastCol = 1;
			if( true == oLook.bNoHBand )
				oOutputProperty.m_bAutoNoRowBand = 1;
			if( true == oLook.bNoVBand )
				oOutputProperty.m_bAutoNoColBand = 1;
		}
		if (m_ooxTableProps->m_oTblStyleRowBandSize.IsInit() && m_ooxTableProps->m_oTblStyleRowBandSize->m_oVal.IsInit())
			oOutputProperty.m_nRowBandSize = m_ooxTableProps->m_oTblStyleRowBandSize->m_oVal->GetValue(); 

		if (m_ooxTableProps->m_oTblStyleColBandSize.IsInit() && m_ooxTableProps->m_oTblStyleColBandSize->m_oVal.IsInit())
			oOutputProperty.m_nColBandSize = m_ooxTableProps->m_oTblStyleColBandSize->m_oVal->GetValue(); 
		
		if( m_ooxTableProps->m_oTblW.IsInit() && m_ooxTableProps->m_oTblW->m_oW.IsInit())
		{
			oOutputProperty.m_nWidth = m_ooxTableProps->m_oTblW->m_oW->GetValue();
			
			if( m_ooxTableProps->m_oTblW->m_oType.IsInit())
			{
				switch(m_ooxTableProps->m_oTblW->m_oType->GetValue())
				{
					case SimpleTypes::tblwidthDxa:	oOutputProperty.m_eMUWidth = mu_Twips;	break;
					case SimpleTypes::tblwidthPct:	oOutputProperty.m_eMUWidth = mu_Percent; break;
				}
			}
		}
		return true;
	}
};
