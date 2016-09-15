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
#include "RtfChar.h"
#include "RtfDocument.h"

CString RtfChar::renderRtfText( CString& sText, void* poDocument, RtfCharProperty* oCharProperty  )
{
    RtfDocument* oDocument = static_cast<RtfDocument*>(poDocument);
    CString sResult;

    int nCodePage = -1;
    //применяем параметры codepage от текущего шрифта todo associated fonts.
    //todooo разобраться со шрифтами и их подбором
    RtfFont oFont;
    if( NULL != oCharProperty && true == oDocument->m_oFontTable.GetFont( oCharProperty->m_nFont, oFont ) )
    {
        if( PROP_DEF != oFont.m_nCharset )
            nCodePage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
        else if( PROP_DEF != oFont.m_nCodePage )
            nCodePage = oFont.m_nCodePage;
    }

    //от настроек документа
    if( -1 == nCodePage && RtfDocumentProperty::cp_none != oDocument->m_oProperty.m_eCodePage )
    {
        switch ( oDocument->m_oProperty.m_eCodePage )
        {
        case RtfDocumentProperty::cp_ansi:
        {
            if( PROP_DEF != oDocument->m_oProperty.m_nAnsiCodePage )
                nCodePage = oDocument->m_oProperty.m_nAnsiCodePage;
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

    std::wstring    unicodeStr (sText.GetBuffer(), sText.GetLength());
    std::string     ansiStr ;

	if (unicodeStr.empty())
	{
		return sResult;
	}

	ansiStr = RtfUtility::convert_string(unicodeStr.begin(), unicodeStr.end(), nCodePage);

    CString sTextBack  = RtfUtility::convert_string(ansiStr.begin(), ansiStr.end(), nCodePage);
    //обратное преобразование чтобы понять какие символы свонвертировались неправильно

	while (sTextBack.GetLength() < sText.GetLength())
		sTextBack += L"-";

    for( int i = 0; i < sText.GetLength() ; i++ )
    {
        bool bWriteUnicode = true;

        if(sTextBack[i] == sText[i] )
        {
            CString sUniChar; sUniChar.AppendChar( unicodeStr[i] );

            //делаем Ansi строку sUniChar
            // -> sTempAnsiChars
            std::string sTempAnsiChars = RtfUtility::convert_string(unicodeStr.begin()+i, unicodeStr.begin() + i + 1, nCodePage);

            for( int k = 0; k < sTempAnsiChars.length(); k++ )
            {
                unsigned char nCharCode = sTempAnsiChars[k];
                bWriteUnicode = false;

                if (nCharCode == 0x5c || nCharCode == 0x7b || nCharCode == 0x7d)
                {
                    sResult.AppendFormat( _T("\\'%x"), nCharCode );
                } else if (0x00 <= nCharCode && nCharCode - 1 < 0x10)
                {
                    sResult.AppendFormat(_T("\\'0%x"), nCharCode - 1 );
                } else if (0x10 <= nCharCode - 1 && nCharCode  < 0x20)
                {
                    sResult.AppendFormat(_T("\\'%x"), nCharCode - 1 );
                } else if ( 0x20 <= nCharCode && nCharCode < 0x80 )
                {
                    sResult.AppendChar( nCharCode );
                } else { // 0x80 <= nUnicode <= 0xff
                    sResult.AppendFormat( _T("\\'%x"), nCharCode );
                }
            }
        }
        if( true == bWriteUnicode )
        {
            int nUnicode = (int)unicodeStr[i];

            if (0 < nUnicode && nUnicode <= 0x8000)
            {
                sResult.AppendFormat(_T("\\u%d*"),nUnicode);
            } else if (0x8000 < nUnicode && nUnicode <= 0xffff) {
                sResult.AppendFormat(_T("\\u%d*"), nUnicode - 0x10000); //??? font alt name china ALL FONTS NEW.docx (Mekanik LET)
            } else {
                sResult += _T("\\u9633*");
            }
        }

    }
    return sResult;
}
