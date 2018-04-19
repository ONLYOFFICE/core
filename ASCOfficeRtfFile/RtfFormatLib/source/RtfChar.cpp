﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "RtfChar.h"
#include "RtfDocument.h"

#include "Writer/OOXWriter.h"

std::wstring RtfChar::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);
    
    std::wstring sResult;
    if (RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType)
    {
		bool bInsert = false;
		bool bDelete = false;

		if (m_oProperty.m_nRevised != PROP_DEF)
		{
			bInsert = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauth);
            std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oProperty.m_nRevised = PROP_DEF;
		}
		if (m_oProperty.m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauthDel);
            std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oProperty.m_nDeleted = PROP_DEF;
		}
        sResult += L"<w:r>";
			sResult += L"<w:rPr>";
				sResult += m_oProperty.RenderToOOX(oRenderParameter);
			sResult += L"</w:rPr>";
			sResult += renderTextToXML(L"Text", bDelete );
        sResult += L"</w:r>";
		
		if (bDelete)sResult += L"</w:del>";
		if (bInsert)sResult += L"</w:ins>";
	}
    else if(RENDER_TO_OOX_PARAM_TEXT == oRenderParameter.nType)
	{
        sResult = renderTextToXML( L"Text" );
	}
    else if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
	{
		sResult += L"<m:r>";
			bool bInsert = false;
			bool bDelete = false;

			if (m_oProperty.m_nRevised != PROP_DEF)
			{
				bInsert = true;
				
                std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauth);
                std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttm).c_str());
				
				sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
				m_oProperty.m_nRevised = PROP_DEF;
			}
			if (m_oProperty.m_nDeleted != PROP_DEF)
			{
				bDelete = true;
				
                std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauthDel);
                std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttmDel).c_str());
				
				sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
				m_oProperty.m_nDeleted = PROP_DEF;
			}
			sResult += m_oProperty.RenderToOOX(oRenderParameter);//w:rPr внутри
			sResult += renderTextToXML( L"Math" );
			
			if (bDelete)sResult += L"</w:del>";
			if (bInsert)sResult += L"</w:ins>";
		sResult += L"</m:r>";	
	}
    else if( RENDER_TO_OOX_PARAM_PLAIN == oRenderParameter.nType)
        sResult = m_sChars;
    return sResult;
}

std::wstring RtfChar::renderTextToXML( std::wstring sParam, bool bDelete )
{
    std::wstring sResult;
	if( L"Text" == sParam )
    {
		if (bDelete == false)
		{
			sResult += L"<w:t xml:space= \"preserve\">";
                sResult += XmlUtils::EncodeXmlString( m_sChars, true );
			sResult += L"</w:t>";
		}
		else
		{
			sResult += L"<w:delText>";
                sResult += XmlUtils::EncodeXmlString( m_sChars, true );
			sResult += L"</w:delText>";
		}
    }
    else if( L"Math" == sParam && !m_sChars.empty())
    {
		sResult += L"<m:t>";
            sResult += XmlUtils::EncodeXmlString( m_sChars, true );
		sResult += L"</m:t>";
    }
	return sResult;
}

std::wstring RtfChar::renderRtfText( std::wstring& sText, void* poDocument, RtfCharProperty* oCharProperty  )
{
    RtfDocument* pDocument = static_cast<RtfDocument*>(poDocument);
	
	int iFont = 0;
	if (oCharProperty) iFont = oCharProperty->m_nFont;

    int nCodePage = -1;
    //применяем параметры codepage от текущего шрифта todo associated fonts.
    //todooo разобраться со шрифтами и их подбором
    RtfFont oFont;
    if( NULL != oCharProperty && true == pDocument->m_oFontTable.GetFont( oCharProperty->m_nFont, oFont ) )
    {
        if( PROP_DEF != oFont.m_nCharset )
            nCodePage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
        else if( PROP_DEF != oFont.m_nCodePage )
            nCodePage = oFont.m_nCodePage;
    }

	return renderRtfText(sText, pDocument, nCodePage);

}

std::wstring RtfChar::renderRtfText( std::wstring& sText, void* poDocument, int nCodePage  )
{
	RtfDocument* pDocument = static_cast<RtfDocument*>(poDocument);
    std::wstring sResult;

    //от настроек документа
    if( -1 == nCodePage && RtfDocumentProperty::cp_none != pDocument->m_oProperty.m_eCodePage )
    {
        switch ( pDocument->m_oProperty.m_eCodePage )
        {
        case RtfDocumentProperty::cp_ansi:
        {
            if( PROP_DEF != pDocument->m_oProperty.m_nAnsiCodePage )
                nCodePage = pDocument->m_oProperty.m_nAnsiCodePage;
            else
                nCodePage = CP_ACP;
            break;
        }
        case RtfDocumentProperty::cp_mac: nCodePage = CP_MACCP; break;
        case RtfDocumentProperty::cp_pc:  nCodePage = 437;      break;
        case RtfDocumentProperty::cp_pca: nCodePage = 850;      break;
        }
    }
    //если ничего нет ставим ANSI
    if( -1 == nCodePage )
        nCodePage = CP_ACP;

    std::wstring    unicodeStr (sText);
    std::string     ansiStr ;

	if (unicodeStr.empty())
	{
		return sResult;
	}

	ansiStr = RtfUtility::convert_string(unicodeStr.begin(), unicodeStr.end(), nCodePage);

    // std::wstring sTextBack  = RtfUtility::convert_string(ansiStr.begin(), ansiStr.end(), nCodePage);
    std::wstring sTextBack  = RtfUtility::convert_string_icu(ansiStr.begin(), ansiStr.end(), nCodePage);

	if (!ansiStr.empty() && sTextBack.empty())
	{
		//code page not support in icu !!!
        sTextBack = RtfUtility::convert_string(ansiStr.begin(), ansiStr.end(), nCodePage);
	}

    //обратное преобразование чтобы понять какие символы свонвертировались неправильно
    while (sTextBack.length() < sText.length())
		sTextBack += L"-";

    for( size_t i = 0; i < sText.length() ; i++ )
    {
        bool bWriteUnicode = true;

        if(sTextBack[i] == sText[i] )
        {
            std::wstring sUniChar; sUniChar += unicodeStr[i];

            //делаем Ansi строку sUniChar
            // -> sTempAnsiChars
            std::string sTempAnsiChars = RtfUtility::convert_string(unicodeStr.begin()+i, unicodeStr.begin() + i + 1, nCodePage);

            for( size_t k = 0; k < sTempAnsiChars.length(); k++ )
            {
                unsigned char nCharCode = sTempAnsiChars[k];
                bWriteUnicode = false;

                if (nCharCode == 0x5c || nCharCode == 0x7b || nCharCode == 0x7d)
                {
                    sResult += L"\\'" + XmlUtils::IntToString( nCharCode, L"%x");
                } else if (0x00 <= nCharCode && nCharCode - 1 < 0x10)
                {
                    sResult += L"\\'0" + XmlUtils::IntToString( nCharCode - 1, L"%x" );
                } else if (0x10 <= nCharCode - 1 && nCharCode  < 0x20)
                {
                    sResult += L"\\'" + XmlUtils::IntToString(nCharCode - 1, L"%x" );
                } else if ( 0x20 <= nCharCode && nCharCode < 0x80 )
                {
                    sResult += nCharCode;
                } else { // 0x80 <= nUnicode <= 0xff
                    sResult += L"\\'" + XmlUtils::IntToString(nCharCode, L"%x" );
                }
            }
        }
        if( true == bWriteUnicode )
        {
            int nUnicode = (int)unicodeStr[i];

            if (0 < nUnicode && nUnicode <= 0x8000)
            {
                sResult += L"\\u" + std::to_wstring(nUnicode) + L"*";
            } else if (0x8000 < nUnicode && nUnicode <= 0xffff) {
                sResult += L"\\u" + std::to_wstring(nUnicode - 0x10000) + L"*"; //??? font alt name china ALL FONTS NEW.docx (Mekanik LET)
            } else {
                sResult += L"\\u9633*";
            }
        }

    }
    return sResult;
}
std::wstring RtfChar::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring result;

    if( RENDER_TO_RTF_PARAM_CHAR ==  oRenderParameter.nType )
    {
        if( true == m_bRtfEncode )
            result += renderRtfText( m_sChars, oRenderParameter.poDocument, &m_oProperty );
        else
            result += m_sChars;
    }
    else
    {
        std::wstring sText;
        if( true == m_bRtfEncode )
            sText = renderRtfText( m_sChars, oRenderParameter.poDocument, &m_oProperty );
        else
            sText = m_sChars;

        std::wstring sTextProp =  m_oProperty.RenderToRtf( oRenderParameter ) ;
		
        if( !sText.empty() || !sTextProp.empty())
        {
            if (oRenderParameter.nType != RENDER_TO_RTF_PARAM_NESTED)
				result += L"{";
				
				result += sTextProp;
				result += L" ";
				result += sText;

			if (oRenderParameter.nType != RENDER_TO_RTF_PARAM_NESTED)
				result += L"}";
        }
    }
    return result;
}

std::wstring RtfCharSpecial::_RenderToOOX(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	switch( m_eType )
	{
		case rsc_chdate:		sResult += L"";										break;
		case rsc_chdpl:			sResult += L"";										break;
		case rsc_chdpa:			sResult += L"";										break;
		case rsc_chtime:		sResult += L"";										break;
		case rsc_chpgn:			sResult += L"<w:pgNum />";							break;
		case rsc_sectnum:		sResult += L"";										break;
		case rsc_chftn:			sResult += L"<w:footnoteRef/>";						break;
		case rsc_chftnEnd:		sResult += L"<w:endnoteRef/>";						break;
		case rsc_chatn:			sResult += L"<w:annotationRef />";					break;
		case rsc_chftnsep:		sResult += L"<w:separator />";						break;
		case rsc_chftnsepc:		sResult += L"<w:continuationSeparator/>";			break;
		case rsc_page:			sResult += L"<w:br w:type=\"page\"/>";				break;
		case rsc_column:		sResult += L"<w:br w:type=\"column\"/>";			break;
		case rsc_line:			sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"none\"/>";break;
		case rsc_softpage:		sResult += L"";										break;
		case rsc_softcol:		sResult += L"";										break;
		case rsc_softline:		sResult += L"";										break;
		case rsc_tab:			sResult += L"<w:tab/>";								break;
		case rsc_emspace:		sResult += L"";										break;
		case rsc_qmspace:		sResult += L"";										break;
		case rsc_Formula:		sResult += L"";										break;
		case rsc_zwbo:			sResult += L"";										break;
		case rsc_zwnbo:			sResult += L"";										break;
		case rsc_zwj:			sResult += L"";										break;
		case rsc_zwnj:			sResult += L"";										break;
		case rsc_OptHyphen:		sResult += L"<w:t xml:space=\"preserve\">-</w:t>";	break;//<w:softHyphen/>
		case rsc_NonBrHyphen:	sResult += L"<w:t xml:space=\"preserve\">-</w:t>";	break;//<w:nonBreakHyphen/>
		case rsc_NonBrSpace:	sResult += L"<w:t xml:space=\"preserve\"> </w:t>";	break;
	}
	switch ( m_nTextWrapBreak )
	{
		case 0: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"none\"/>";		break;
		case 1: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"left\"/>";		break;
		case 2: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"right\"/>";	break;
		case 3: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"all\"/>";		break;
	}
	return sResult;
}
std::wstring RtfCharSpecial::RenderToOOX(RenderParameter oRenderParameter)
{
	RtfDocument*	poRtfDocument	= static_cast<RtfDocument*>	(oRenderParameter.poDocument);
	OOXWriter*		poOOXWriter		= static_cast<OOXWriter*>	(oRenderParameter.poWriter);

    std::wstring sResult;
	if(RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType)
	{
		bool bInsert = false;
		bool bDelete = false;

		if (m_oProperty.m_nRevised != PROP_DEF)
		{
			bInsert = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauth);
            std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttm).c_str());
			
			sResult += L"<w:ins w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oProperty.m_nRevised = PROP_DEF;
		}
		if (m_oProperty.m_nDeleted != PROP_DEF)
		{
			bDelete = true;
			
            std::wstring sAuthor = poRtfDocument->m_oRevisionTable.GetAuthor(m_oProperty.m_nRevauthDel);
            std::wstring sDate(RtfUtility::convertDateTime(m_oProperty.m_nRevdttmDel).c_str());
			
			sResult += L"<w:del w:date=\"" + sDate +  L"\" w:author=\"" + sAuthor + L"\" w:id=\"" + std::to_wstring(poOOXWriter->m_nCurTrackChangesId++).c_str() + L"\">";
			m_oProperty.m_nDeleted = PROP_DEF;
		}
		sResult += L"<w:r>";
			sResult += L"<w:rPr>";
				sResult += m_oProperty.RenderToOOX(oRenderParameter);
			sResult += L"</w:rPr>";
			sResult += _RenderToOOX(oRenderParameter);
		sResult += L"</w:r>";
		
		if (bDelete)sResult += L"</w:del>";
		if (bInsert)sResult += L"</w:ins>";
	}
	else if(RENDER_TO_OOX_PARAM_TEXT	== oRenderParameter.nType || 
			RENDER_TO_OOX_PARAM_MATH	== oRenderParameter.nType ||
			RENDER_TO_OOX_PARAM_PLAIN	== oRenderParameter.nType)
	{
		sResult += _RenderToOOX(oRenderParameter);
	}
	return sResult;
}
	
std::wstring RtfCharSpecial::RenderToRtf(RenderParameter oRenderParameter)
{
    std::wstring sResult;
	sResult += L"{";
	sResult += m_oProperty.RenderToRtf( oRenderParameter );
	switch( m_eType )
	{
		case rsc_chdate:		sResult += L"\\chdate";		break;
		case rsc_chdpl:			sResult += L"\\chdpl";		break;
		case rsc_chdpa:			sResult += L"\\chdpa";		break;
		case rsc_chtime:		sResult += L"\\chtime";		break;
		case rsc_chpgn:			sResult += L"\\chpgn";		break;
		case rsc_sectnum:		sResult += L"\\sectnum";		break;
		case rsc_chftn:			sResult += L"\\chftn";		break;
		case rsc_chftnEnd:		sResult += L"\\chftn";		break;
		case rsc_chatn:			sResult += L"\\chatn";		break;
		case rsc_chftnsep:		sResult += L"\\chftnsep";	break;
		case rsc_chftnsepc:		sResult += L"\\chftnsepc";	break;
		case rsc_page:			sResult += L"\\page";		break;
		case rsc_column:		sResult += L"\\column";		break;
		case rsc_line:			sResult += L"\\line";		break;
		case rsc_softpage:		sResult += L"\\softpage";	break;
		case rsc_softcol:		sResult += L"\\softcol";		break;
		case rsc_softline:		sResult += L"\\softline";	break;
		case rsc_tab:			sResult += L"\\tab";			break;
		case rsc_Formula:		sResult += L"\\|";			break;
		case rsc_OptHyphen:		sResult += L"\\-";			break;
		case rsc_NonBrHyphen:	sResult += L"\\_";			break;
		case rsc_NonBrSpace:	sResult += L"\\~";			break;
		case rsc_zwbo:			sResult += L"\\zwbo";		break;
		case rsc_zwnbo:			sResult += L"\\zwnbo";		break;
		case rsc_zwj:			sResult += L"\\zwj";			break;
		case rsc_zwnj:			sResult += L"\\zwnj";		break;
	}
	if( PROP_DEF != m_nTextWrapBreak )
		sResult += L"\\par";
	//switch ( m_nTextWrapBreak ) //не воспринимается word
	//{
	//	case 0: sResult += L"\\lbr0";break;
	//	case 1: sResult += L"\\lbr1";break;
	//	case 2: sResult += L"\\lbr2";break;
	//	case 3: sResult += L"\\lbr3";break;
	//}
	if( PROP_DEF != m_nSoftHeight )
	{
        sResult += L"\\softlheight" + std::to_wstring( m_nSoftHeight );
	}
	sResult += L"}";
	return sResult;
	}
