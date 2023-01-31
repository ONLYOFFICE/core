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
	//в проекте используется ungetcб только после getc
	//поэтому проблем с выходом в 0 нет
	//if( m_nPosAbs + 2 < m_nSizeAbs )
	{
		m_nPosAbs--;	//взять любой txt переименовать в rtf - зацикливание
	}
}
void StringStream::putString( std::string sText )
{
	size_t nExtBufSize = sText.length();
	//копируем буфер в темповый буфер
	unsigned char* aTempBuf = new unsigned char[ m_nSizeAbs ];
	memcpy( aTempBuf, m_aBuffer, m_nSizeAbs );
	//создаем новый буфер большего размера
	RELEASEARRAYOBJECTS( m_aBuffer );
	m_aBuffer = new unsigned char[ m_nSizeAbs + nExtBufSize ];
	//копируем все в новый буфер
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
	m_nReadBufSize = 1024 * 1024 * 5; // 5мб
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

	while ( c >= 0 && c <= 0x1f )
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
			else if( c == '\n' )
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
				if( nCharCode >= 0 && nCharCode <=30 )//искуственно сидвигаем на 1 чтобы не потерять \'00 ( символов от 0 до 0x20 служебные)
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
	int nTempBufPos = 0; //1 мб

	int c = car;
	//while ((isalnum(c) || c == '"'|| c == ':'|| c == '/' || c == '.') &&c != '\\' && c != '}' && c != '{' && c != Eof) // иправиЃEЃEрвьD усЃEвиЃE
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
