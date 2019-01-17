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
#pragma once

//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>

#include "RtfToken.h"
#include "Utils.h"
#include "Basic.h"

class StringStream
{
private: 
    LONG64 m_nSizeAbs;//размер файла
    LONG64 m_nPosAbs;//позиция в файле

    //std::string m_sBuffer;
	unsigned char* m_aBuffer;
public:
	StringStream()
	{
	   m_aBuffer = NULL;
	   Clear();
	}
	~StringStream()
	{
	   Clear();
	}
	void Clear()
	{
		RELEASEARRAYOBJECTS( m_aBuffer );

		m_nSizeAbs = 0;
		m_nPosAbs = -1;			
	} 
	void SetSource( std::wstring sPath  )
	{
		Clear();

		CFile srcFile;
		
		if (srcFile.OpenFile(sPath.c_str()) != S_OK) return;

		__int64 totalFileSize = srcFile.GetFileSize();

		m_nSizeAbs = (long)totalFileSize;
		m_aBuffer = new unsigned char[m_nSizeAbs];
		DWORD dwBytesRead = 0;

		srcFile.ReadFile(m_aBuffer, (DWORD)m_nSizeAbs);

		dwBytesRead = (DWORD)srcFile.GetPosition();
		srcFile.CloseFile();
	}
    void getBytes( int nCount, BYTE** pbData )
	{
		if( m_nPosAbs + nCount < m_nSizeAbs )
		{
            (*pbData) = new BYTE[nCount];
			memcpy( (*pbData), (m_aBuffer + m_nPosAbs + 1), nCount);
			m_nPosAbs += nCount;
		}
	}
	int getc()
	{
		int nResult = EOF;
		if( m_nPosAbs + 1 < m_nSizeAbs )
		{
			m_nPosAbs++;
			nResult = m_aBuffer[ m_nPosAbs ];
		}
		return nResult;
	}
	void ungetc()
	{
		//в проекте используется ungetcб только после getc
		//поэтому проблем с выходом в 0 нет
        //if( m_nPosAbs + 2 < m_nSizeAbs )
        {
            m_nPosAbs--;	//взять любой txt переименовать в rtf - зацикливание
        }
	}
    void putString( std::string sText )
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
    LONG64 getCurPosition()
	{
		return m_nPosAbs;
	}
    LONG64 getSize()
	{
		return m_nSizeAbs;
	}
};

class RtfLex
{
private: 
	StringStream	m_oStream;
	RtfToken		m_oCurToken;
public: 
	NFileWriter::CBufferedFileWriter* m_oFileWriter;
	char* m_caReadBuffer;
	int m_nReadBufSize;
 
	RtfLex()
	{
		m_oFileWriter = NULL;
		m_nReadBufSize = 1024 * 1024 * 5; // 5мб
        m_caReadBuffer = new char[m_nReadBufSize];
	}
	~RtfLex()
	{
        if (m_caReadBuffer) delete []m_caReadBuffer;
        m_caReadBuffer = NULL;
		RELEASEOBJECT( m_oFileWriter );
	}
	double GetProgress()
	{
		return 1.0 * m_oStream.getCurPosition() / m_oStream.getSize();
	}
	void SetSource( std::wstring sPath )
	{
		m_oStream.SetSource( sPath);

		if (m_oStream.getSize() > m_nReadBufSize)
		{
			m_nReadBufSize = (int)m_oStream.getSize() ;
			if (m_caReadBuffer) delete []m_caReadBuffer;
			m_caReadBuffer = new char[m_nReadBufSize];
		}
	}
	void CloseSource()
	{
		m_oStream.Clear();
	}
	RtfToken NextCurToken()
	{
		return m_oCurToken;
	}
    void ReadBytes( int nCount, BYTE** pbData )
	{
		m_oStream.getBytes(nCount, pbData);
	}
	RtfToken NextToken()
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
    void putString( std::string sText )
	{
		m_oStream.putString( sText );
	}
private: 
	void parseKeyword(RtfToken& token)
	{
        std::string palabraClave;

//        palabraClave.GetBuffer( 20 );
//		palabraClave.ReleaseBuffer();

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

		//Se asigna la palabra clave leida
		token.Type = RtfToken::Keyword;
		token.Key = palabraClave;

		//Se comprueba si la palabra clave tiene parбmetro
		if (RtfUtility::IsDigit(c) || c == '-')
		{
			token.HasParameter = true;

			//Se comprubea si el parбmetro es negativo
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
                parametroInt = _wtoi(parametroStr.c_str());
            }catch(...)
            {
                try
                {
                    parametroInt = (int)_wtoi64(parametroStr.c_str());
                }catch(...)
                {
                }
            }

			if (negativo)
				parametroInt = -parametroInt;

			//Se asigna el parбmetro de la palabra clave
			token.Parameter = parametroInt;
		}

		if (c == ' ')
		{
			m_oStream.getc();
		}
	}
	void parseText(int car, RtfToken& token)
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
		m_oStream.ungetc();
		if( nTempBufPos > 0 )
		{
			m_caReadBuffer[nTempBufPos++] = '\0';
            token.Key += m_caReadBuffer ;
		}
	}
	bool GetNextChar( int& nChar )
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
	void parseTextFile(int car, RtfToken& token)
	{
		if( NULL != m_oFileWriter )
		{
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
						bool bContinue = true;
						if( true == GetNextChar( nFirst ) )
							if( true == GetNextChar( nSecond ) )
							{
								byteByte = 10 * Strings::ToDigit( nFirst ) + Strings::ToDigit( nSecond  );
								m_oFileWriter->Write( &byteByte, 1 );
								bContinue = true;
							}
						if( false == bContinue)
							break;
					}
				}
			}
			catch(...)
			{
			}
		}
    }
};
