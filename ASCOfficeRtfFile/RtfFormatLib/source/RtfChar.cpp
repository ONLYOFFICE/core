#include "RtfChar.h"
#include "RtfDocument.h"

CString RtfChar::renderRtfText( CString& sText, void* poDocument, RtfCharProperty* oCharProperty )
		{
			RtfDocument* oDocument = static_cast<RtfDocument*>(poDocument);
			CString sResult;

			int nCodePage = -1;
			//применяем параметры codepage от текущего шрифта todo associated fonts.
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
					case RtfDocumentProperty::cp_mac: nCodePage = CP_MACCP;break;
					case RtfDocumentProperty::cp_pc: nCodePage = 437;break;
					case RtfDocumentProperty::cp_pca: nCodePage = 850;break;
				}
			}
			//если ничего нет ставим ANSI
			if( -1 == nCodePage )
				nCodePage = CP_ACP;

			//делаем Ansi строку
			int nBufSize = ::WideCharToMultiByte(nCodePage, 0, sText, -1, NULL, 0, NULL, NULL);
			CStringA sTempResultAnsi;
			LPSTR pcBuffer = sTempResultAnsi.GetBuffer(nBufSize);
			::WideCharToMultiByte(nCodePage, 0, sText, -1, pcBuffer, nBufSize, NULL, NULL);
			sTempResultAnsi.ReleaseBuffer();
			//делаем обратное преобразование чтобы понять какие символы свонвертировались неправильно
			nBufSize = ::MultiByteToWideChar(nCodePage, 0, sTempResultAnsi, -1, NULL, 0);
			CStringW sTempResultUni;
			LPWSTR pwBuffer = sTempResultUni.GetBuffer(nBufSize);
			::MultiByteToWideChar(nCodePage, 0, sTempResultAnsi, -1, pwBuffer, nBufSize);
			sTempResultUni.ReleaseBuffer();

			for( int i = 0; i < sTempResultUni.GetLength() && i < sText.GetLength() ; i++ )
			{
				bool bWriteUnicode = true;
				if( sTempResultUni[i] == sText[i] )
				{
					CString sUniChar; sUniChar.AppendChar( sText[i] );
					
					//делаем Ansi строку
					int nBufSize = ::WideCharToMultiByte(nCodePage, 0, sUniChar, -1, NULL, 0, NULL, NULL);
					CStringA sTempAnsiChars;
					LPSTR pcBuffer = sTempAnsiChars.GetBuffer(nBufSize);
					::WideCharToMultiByte(nCodePage, 0, sUniChar, -1, pcBuffer, nBufSize, NULL, NULL);
					sTempAnsiChars.ReleaseBuffer();
					for( int k = 0; k < sTempAnsiChars.GetLength(); k++ )
					{
						unsigned char nCharCode = sTempAnsiChars[k];
						bWriteUnicode = false;
						//if (nCharCode == '\n') {
						//	sResult.Append(_T("\\line"));
						//} else if (nCharCode == '\r') {
						//	// ignore '\r'
						//} else if (nCharCode == '\t') {
						//	sResult.Append(_T("\\tab"));
						//} else if (nCharCode <= 0xff) {
							if (nCharCode == 0x5c || nCharCode == 0x7b || nCharCode == 0x7d) {
								sResult.AppendFormat( _T("\\'%x"), nCharCode );
							} else if (0x00 <= nCharCode && nCharCode - 1 < 0x10) {
								sResult.AppendFormat(_T("\\'0%x"), nCharCode - 1 );
							} else if (0x10 <= nCharCode - 1 && nCharCode  < 0x20) {
								sResult.AppendFormat(_T("\\'%x"), nCharCode - 1 );
							} else if ( 0x20 <= nCharCode && nCharCode < 0x80 ) {
								sResult.AppendChar( nCharCode );
							} else { // 0x80 <= nUnicode <= 0xff
								sResult.AppendFormat( _T("\\'%x"), nCharCode );
							}
					}
				}
				if( true == bWriteUnicode )
				{
					int nUnicode = (int)sText[i];
					if (0 < nUnicode && nUnicode <= 0x8000) {
						sResult.AppendFormat(_T("\\u%d*"),nUnicode);
					} else if (0x8000 < nUnicode && nUnicode <= 0xffff) {
						sResult.AppendFormat(_T("\\u%d*"),nUnicode - 0x10000);
					} else {
						sResult.Append(_T("\\u9633*"));
					}
				}
			
			}
			return sResult;
		}
