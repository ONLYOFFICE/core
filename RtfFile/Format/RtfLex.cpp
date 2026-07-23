/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "RtfLex.h"

StringStream::StringStream()
{
	m_aBuffer = NULL;
	Clear();
}
StringStream::~StringStream()
{
	Clear();
}
void StringStream::Clear()
{
	RELEASEARRAYOBJECTS( m_aBuffer );

	m_nSizeAbs = 0;
	m_nPosAbs = -1;
}
bool StringStream::SetSource( std::wstring sPath  )
{
	Clear();

	NSFile::CFileBinary srcFile;
	if (false == srcFile.OpenFile(sPath.c_str())) return false;

	__int64 totalFileSize = srcFile.GetFileSize();
	if (totalFileSize < 5)
	{
		srcFile.CloseFile();
		return false;
	}

	m_nSizeAbs = (long)totalFileSize;
	m_aBuffer = new unsigned char[m_nSizeAbs];
	DWORD dwBytesRead = 0;

	srcFile.ReadFile(m_aBuffer, (DWORD)m_nSizeAbs);

	dwBytesRead = (DWORD)srcFile.GetPosition();
	srcFile.CloseFile();
	return true;
}
void StringStream::getBytes( int nCount, BYTE** pbData )
{
	if( m_nPosAbs + nCount < m_nSizeAbs )
	{
		(*pbData) = new BYTE[nCount];
		memcpy( (*pbData), (m_aBuffer + m_nPosAbs + 1), nCount);
		m_nPosAbs += nCount;
	}
}
int StringStream::getc()
{
	int nResult = EOF;
	if( m_nPosAbs + 1 < m_nSizeAbs )
	{
		m_nPosAbs++;
		nResult = m_aBuffer[ m_nPosAbs ];
	}
	return nResult;
}
void StringStream::ungetc()
{
	//in the project ungetc is used only after getc
	//so there are no problems with reaching 0
	//if( m_nPosAbs + 2 < m_nSizeAbs )
	{
		m_nPosAbs--;	//take any txt rename to rtf - infinite loop
	}
}
void StringStream::putString( std::string sText )
{
	size_t nExtBufSize = sText.length();
	//copy buffer to temp buffer
	unsigned char* aTempBuf = new unsigned char[ m_nSizeAbs ];
	memcpy( aTempBuf, m_aBuffer, m_nSizeAbs );
	//create new buffer with larger size
	RELEASEARRAYOBJECTS( m_aBuffer );
	m_aBuffer = new unsigned char[ m_nSizeAbs + nExtBufSize ];
	//copy everything to new buffer
	unsigned long nDelimiter = (unsigned long)m_nPosAbs + 1;
	memcpy( m_aBuffer, aTempBuf, nDelimiter );
	memcpy( m_aBuffer + nDelimiter , sText.c_str(), nExtBufSize );

	memcpy( m_aBuffer + nDelimiter + nExtBufSize , aTempBuf + nDelimiter , m_nSizeAbs - nDelimiter );
	RELEASEARRAYOBJECTS( aTempBuf );

	m_nSizeAbs += nExtBufSize;
}
LONG64 StringStream::getCurPosition()
{
	return m_nPosAbs;
}
LONG64 StringStream::getSize()
{
	return m_nSizeAbs;
}

RtfLex::RtfLex()
{
	m_oFileWriter = NULL;
	m_nReadBufSize = 1024 * 1024 * 5; // 5MB
	m_caReadBuffer = new char[m_nReadBufSize];
}
RtfLex::~RtfLex()
{
	if (m_caReadBuffer) delete []m_caReadBuffer;
	m_caReadBuffer = NULL;
	RELEASEOBJECT( m_oFileWriter );
}
double RtfLex::GetProgress()
{
	return 1.0 * m_oStream.getCurPosition() / m_oStream.getSize();
}
bool RtfLex::SetSource( std::wstring sPath )
{
	if (false == m_oStream.SetSource(sPath)) return false;

	if (m_oStream.getSize() > m_nReadBufSize)
	{
		m_nReadBufSize = (int)m_oStream.getSize() ;
		if (m_caReadBuffer) delete []m_caReadBuffer;
		m_caReadBuffer = new char[m_nReadBufSize];
	}
	return true;
}
void RtfLex::CloseSource()
{
	m_oStream.Clear();
}
RtfToken RtfLex::NextCurToken()
{
	return m_oCurToken;
}
void RtfLex::ReadBytes( int nCount, BYTE** pbData )
{
	m_oStream.getBytes(nCount, pbData);
}
RtfToken RtfLex::NextToken()
{
	int c;

	m_oCurToken = RtfToken() ;

	c = m_oStream.getc( );

    while ((c >= 0 && c <= 8) || (c >= 10 && c <= 0x1f))
        c = m_oStream.getc( );

	if (c != EOF)
	{
		switch (c)
		{
		case '{':
			m_oCurToken.Type = RtfToken::GroupStart;
			break;
		case '}':
			m_oCurToken.Type = RtfToken::GroupEnd;
			break;
		case '\\':
			parseKeyword(m_oCurToken);
			break;
		default:
			m_oCurToken.Type = RtfToken::Text;
			if( NULL == m_oFileWriter )
				parseText(c, m_oCurToken);
			else
				parseTextFile(c, m_oCurToken);
			break;
		}
	}
	else
	{
		m_oStream.Clear();
		m_oCurToken.Type = RtfToken::Eof;
	}

	return m_oCurToken;
}
void RtfLex::putString( std::string sText )
{
	m_oStream.putString( sText );
}
void RtfLex::parseKeyword(RtfToken& token)
{
	std::string palabraClave;

	std::wstring parametroStr ;
	int parametroInt = 0;

	int c = m_oStream.getc();
	m_oStream.ungetc();
	bool negativo = false;

	if ( !RtfUtility::IsAlpha( c ) )
	{
		c = m_oStream.getc();

		if(c == '\\' || c == '{' || c == '}')
		{
			token.Type = RtfToken::Text;
			token.Key = (char)c;
		}
		else if( c > 0 && c <= 31 )
		{
			if( c == '\t' )
			{
				token.Type = RtfToken::Keyword;
				token.Key = std::string("tab");
			}
            else if( c == '\n'|| c == '\r' )
			{
				token.Type = RtfToken::Keyword;
				token.Key = std::string("par");
			}
			else
			{
				token.Type = RtfToken::Text;
				token.Key = std::string("");
			}
		}
		else
		{
			token.Type = RtfToken::Control;

			token.Key = std::to_string( c);

			if (c == '\'')
			{
				token.HasParameter = true;
				int nCharCode = RtfUtility::ToByte( m_oStream.getc() ) << 4;
				nCharCode |= RtfUtility::ToByte( m_oStream.getc() );
				if( nCharCode >= 0 && nCharCode <=30 )//artificially shift by 1 to not lose \'00 (characters from 0 to 0x20 are service characters)
					nCharCode++;
				token.Parameter = nCharCode;
			}
			else if( c == '|' || c == '~' || c == '-' || c == '_' || c == ':' )
			{
				token.Type = RtfToken::Keyword;
				token.Key.erase();
				token.Key += (char)c ;
			}
		}
		return;
	}
	c = m_oStream.getc();
	m_oStream.ungetc();

	while (RtfUtility::IsAlpha(c))
	{
		m_oStream.getc();
		palabraClave += (char)c;

		c = m_oStream.getc();
		m_oStream.ungetc();
	}

	token.Type = RtfToken::Keyword;
	token.Key = palabraClave;

	if (RtfUtility::IsDigit(c) || c == '-')
	{
		token.HasParameter = true;

		if (c == '-')
		{
			negativo = true;

			m_oStream.getc();
		}

		c = m_oStream.getc();
		m_oStream.ungetc();
		while (RtfUtility::IsDigit(c))
		{
			m_oStream.getc();
			parametroStr += c;

			c = m_oStream.getc();
			m_oStream.ungetc();
		}
		try
		{
			parametroInt = XmlUtils::GetInteger(parametroStr);
		}
		catch (...)
		{
			try
			{
				parametroInt = (int)XmlUtils::GetInteger64(parametroStr);
			}
			catch (...)
			{
			}
		}

		if (negativo)
			parametroInt = -parametroInt;

		//if (c == ' ' || c == '\\' || c == '}' || c == '{' || c == '\"' || c == ';')
		{
			token.Parameter = parametroInt;
		}
		//else
		//{
		//	token.HasParameter = false;
		//	//token.Parameter = 0;
		//}
	}

	if (c == ' ')
	{
		m_oStream.getc();
	}
}
void RtfLex::parseText(int car, RtfToken& token)
{
	int nTempBufPos = 0; //1 MB

	int c = car;
	//while ((isalnum(c) || c == '"'|| c == ':'|| c == '/' || c == '.') &&c != '\\' && c != '}' && c != '{' && c != Eof) 
	//while (c != '\\' && c != '}' && c != '{' && c != Eof)
	//while (c != ';' &&c ! = '\\' && c != '}' && c != '{' && c != EOF)
	while (c != '\\' && c != '}' && c != '{' && c != EOF)
	{
		if( nTempBufPos >= m_nReadBufSize )
		{
			m_caReadBuffer[nTempBufPos++] = '\0';
			token.Key += m_caReadBuffer ;
			nTempBufPos = 0;
		}
		m_caReadBuffer[nTempBufPos++] = (char)c;

		c = m_oStream.getc();
		//Se ignoran los retornos de carro, tabuladores y caracteres nulos
		while (c == '\r' || c == '\n')
			c = m_oStream.getc();
	}
	if (c != EOF)
	{
		m_oStream.ungetc();
	}
	if( nTempBufPos > 0 )
	{
		m_caReadBuffer[nTempBufPos++] = '\0';
		token.Key += m_caReadBuffer ;
	}
}
bool RtfLex::GetNextChar( int& nChar )
{
	int c = m_oStream.getc();
	m_oStream.ungetc();
	//Se ignoran los retornos de carro, tabuladores y caracteres nulos
	while (c == '\r' || c == '\n')
	{
		m_oStream.getc();
		c = m_oStream.getc();
		m_oStream.ungetc();
	}
	if( c != '\\' && c != '}' && c != '{' && c != EOF )
	{
		m_oStream.getc();
		nChar = c;
		return true;
	}
	else
		return false;
}
void RtfLex::parseTextFile(int car, RtfToken& token)
{
	if (NULL == m_oFileWriter) return;

	try
	{
		int nFirst = car;
		int nSecond = 0;
		if( true == GetNextChar( nSecond ) )
		{
			BYTE byteByte = 10 * RtfUtility::ToByte( nFirst ) + RtfUtility::ToByte( nSecond  );
			m_oFileWriter->Write( &byteByte, 1 );
			while( true )
			{
				bool bContinue = false;
				if (true == GetNextChar(nFirst))
				{
					if (true == GetNextChar(nSecond))
					{
						byteByte = 10 * Strings::ToDigit(nFirst) + Strings::ToDigit(nSecond);
						m_oFileWriter->Write(&byteByte, 1);
						bContinue = true;
					}
				}
				if ( false == bContinue)
					break;
			}
		}
	}
	catch(...)
	{
	}
}
