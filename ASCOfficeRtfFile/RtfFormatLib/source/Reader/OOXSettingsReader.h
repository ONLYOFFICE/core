#pragma once
#include "OOXFontReader.h"
#include "OOXMathPropReader.h"
#include "../Utils.h"

class OOXSettingsReader
{
private: 
	OOX::CSettings* m_ooxSettings;

public: 
	OOXSettingsReader(OOX::CSettings* ooxSettings)
	{
		m_ooxSettings = ooxSettings;
	}
	bool Parse( ReaderParameter oParam )
	{
		//oParam.oRtf->m_oColorSchemeMapping.SetDefaultOOX();
		oParam.oRtf->m_oMathProp.SetDefaultOOX();

		if (m_ooxSettings == NULL) return false;
		
		if (m_ooxSettings->m_oMathPr.IsInit())
		{
			OOXMathPropReader oMathProp(m_ooxSettings->m_oMathPr.GetPointer());
			oMathProp.Parse( oParam );
		}

		if (m_ooxSettings->m_oDoNotHyphenateCaps.IsInit())
		{//todooo проверить чтоб 1 и 0 было
			oParam.oRtf->m_oProperty.m_bHypCaps = !m_ooxSettings->m_oDoNotHyphenateCaps->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oAutoHyphenation.IsInit() )
		{
			oParam.oRtf->m_oProperty.m_bAutoHyp = m_ooxSettings->m_oAutoHyphenation->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oConsecutiveHyphenLimit.IsInit() && m_ooxSettings->m_oConsecutiveHyphenLimit->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nMaxHypen = m_ooxSettings->m_oConsecutiveHyphenLimit->m_oVal->GetValue();
		}
		if (m_ooxSettings->m_oHyphenationZone.IsInit() && m_ooxSettings->m_oHyphenationZone->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nHyphenationRight = m_ooxSettings->m_oHyphenationZone->m_oVal->ToTwips(); //todooo проверить размерность
		}
		if (m_ooxSettings->m_oDefaultTabStop.IsInit() && m_ooxSettings->m_oDefaultTabStop->m_oVal.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nTabWidth = m_ooxSettings->m_oDefaultTabStop->m_oVal->ToTwips();//todooo
		}
		if (m_ooxSettings->m_oBordersDoNotSurroundHeader.IsInit())
		{//todooo проверить чтоб 1 и 0 было
			oParam.oRtf->m_oProperty.m_bDorderSurroundHeader = !m_ooxSettings->m_oBordersDoNotSurroundHeader->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oBordersDoNotSurroundFooter.IsInit())
		{//todooo проверить чтоб 1 и 0 было
			oParam.oRtf->m_oProperty.m_bDorderSurroundFotter = !m_ooxSettings->m_oBordersDoNotSurroundFooter->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oAlignBordersAndEdges.IsInit())
		{
			oParam.oRtf->m_oProperty.m_bAlignBordersAndEdges = m_ooxSettings->m_oAlignBordersAndEdges->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oGutterAtTop.IsInit())
		{
			oParam.oRtf->m_oProperty.m_bGutterAtTop = m_ooxSettings->m_oGutterAtTop->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oMirrorMargins.IsInit())
		{
			oParam.oRtf->m_oProperty.m_bSwitchMargins = m_ooxSettings->m_oMirrorMargins->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oEvenAndOddHeaders.IsInit())
		{
			oParam.oRtf->m_oProperty.m_bFacingPage = m_ooxSettings->m_oEvenAndOddHeaders->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oDisplayBackgroundShape.IsInit())
		{
			oParam.oRtf->m_oProperty.m_nDisplayBackground = m_ooxSettings->m_oDisplayBackgroundShape->m_oVal.ToBool();
		}
		if (m_ooxSettings->m_oCompat.IsInit())
		{
			// todooo - реализовать в DocxFormat

			//if( _T("w:doNotUseHTMLParagraphAutoSpacing") == sNodeName )
			//	oParam.oRtf->m_oProperty.m_bHtmlAutoSpace = 0;
			//else if( _T("w:forgetLastTabAlignment") == sNodeName )
			//	oParam.oRtf->m_oProperty.m_bUseTabAlignment = 0;
		}
		if (m_ooxSettings->m_oFootnotePr.IsInit())
		{
			for (long i = 0; i < m_ooxSettings->m_oFootnotePr->m_arrFootnote.size(); i++)
			{
				if( m_ooxSettings->m_oFootnotePr->m_arrFootnote[i]->m_oId.IsInit())
				{
					oParam.oRtf->m_oProperty.m_aSpecialFootnotes.push_back( m_ooxSettings->m_oFootnotePr->m_arrFootnote[i]->m_oId->GetValue() );
				}
			}
			if( m_ooxSettings->m_oFootnotePr->m_oNumFmt.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumFmt->m_oVal.IsInit())
			{
				oParam.oRtf->m_oProperty.m_nFootnoteNumberingFormat = RtfListLevelProperty::GetFormat(m_ooxSettings->m_oFootnotePr->m_oNumFmt->m_oVal->ToString());
			}
			if( m_ooxSettings->m_oFootnotePr->m_oNumRestart.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumRestart->m_oVal.IsInit())
			{
				switch(m_ooxSettings->m_oFootnotePr->m_oNumRestart->m_oVal->GetValue())
				{
				case SimpleTypes::restartnumberEachPage		: 
					oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachPage; break; 
				case SimpleTypes::restartnumberEachSect		: 
					oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_EachSection; break;
				case SimpleTypes::restartnumberContinious	: 
				default										:
					oParam.oRtf->m_oProperty.m_eFootnoteRestart = RtfDocumentProperty::fr_Continuous; break;
				}
			}
			if( m_ooxSettings->m_oFootnotePr->m_oNumStart.IsInit() && m_ooxSettings->m_oFootnotePr->m_oNumStart->m_oVal.IsInit())
			{
				oParam.oRtf->m_oProperty.m_nFootnoteStart = m_ooxSettings->m_oFootnotePr->m_oNumStart->m_oVal->GetValue();
			}
			if( m_ooxSettings->m_oFootnotePr->m_oPos.IsInit() && m_ooxSettings->m_oFootnotePr->m_oPos->m_oVal.IsInit())
			{
				switch(m_ooxSettings->m_oFootnotePr->m_oPos->m_oVal->GetValue())
				{
				case SimpleTypes::ftnposBeneathText : 
					oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BeneathText; break;
				case SimpleTypes::ftnposDocEnd      : 
					oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndDocument; break;
				case SimpleTypes::ftnposPageBottom  : 
					oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_BottomPage; break;
				case SimpleTypes::ftnposSectEnd     :
				default                : 
					oParam.oRtf->m_oProperty.m_eFootnotePlacement = RtfDocumentProperty::fp_EndSection; break;
				}
			}
		}		
	
		if (m_ooxSettings->m_oEndnotePr.IsInit())
		{
			for (long i = 0; i < m_ooxSettings->m_oEndnotePr->m_arrEndnote.size(); i++)
			{
				if( m_ooxSettings->m_oEndnotePr->m_arrEndnote[i]->m_oId.IsInit())
				{
					oParam.oRtf->m_oProperty.m_aSpecialEndnotes.push_back( m_ooxSettings->m_oEndnotePr->m_arrEndnote[i]->m_oId->GetValue() );
				}
			}
			if( m_ooxSettings->m_oEndnotePr->m_oNumFmt.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumFmt->m_oVal.IsInit())
			{
				oParam.oRtf->m_oProperty.m_nEndnoteNumberingFormat = RtfListLevelProperty::GetFormat(m_ooxSettings->m_oEndnotePr->m_oNumFmt->m_oVal->ToString());
			}
			if( m_ooxSettings->m_oEndnotePr->m_oNumRestart.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumRestart->m_oVal.IsInit())
			{
				switch(m_ooxSettings->m_oEndnotePr->m_oNumRestart->m_oVal->GetValue())
				{
				case SimpleTypes::restartnumberEachSect		: 
					oParam.oRtf->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_EachSection; break;
				case SimpleTypes::restartnumberContinious	: 
				default										:
					oParam.oRtf->m_oProperty.m_eEndnoteRestart = RtfDocumentProperty::er_Continuous; break;
				}
			}
			if( m_ooxSettings->m_oEndnotePr->m_oNumStart.IsInit() && m_ooxSettings->m_oEndnotePr->m_oNumStart->m_oVal.IsInit())
			{
				oParam.oRtf->m_oProperty.m_nEndnoteStart = m_ooxSettings->m_oEndnotePr->m_oNumStart->m_oVal->GetValue();
			}
			if( m_ooxSettings->m_oEndnotePr->m_oPos.IsInit() && m_ooxSettings->m_oEndnotePr->m_oPos->m_oVal.IsInit())
			{
				switch(m_ooxSettings->m_oEndnotePr->m_oPos->m_oVal->GetValue())
				{
				case SimpleTypes::ftnposBeneathText : 
					oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BeneathText; break;
				case SimpleTypes::ftnposDocEnd      : 
					oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndDocument; break;
				case SimpleTypes::ftnposPageBottom  : 
					oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_BottomPage; break;
				case SimpleTypes::ftnposSectEnd     :
				default                : 
					oParam.oRtf->m_oProperty.m_eEndnotePlacement = RtfDocumentProperty::ep_EndSection; break;
				}
			}
		}
		return true;
	}
};
