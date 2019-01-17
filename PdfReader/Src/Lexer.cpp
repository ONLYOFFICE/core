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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include "Lexer.h"

namespace PdfReader
{
	// '1' - означает пробел. '1' или '2' означает, что данным символом заканчивается
	// имя или команда.

	static char c_sSpecialChars[256] =
	{
		//  0x 1x 2x 3x 4x 5x 6x 7x 8x 9x ax bx cx dx ex fx

		1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,   // 0x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 1x
		1, 0, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 0, 0, 2,   // 2x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0,   // 3x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 4x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,   // 5x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 6x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 0, 0,   // 7x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 8x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 9x
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // ax
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // bx
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // cx
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // dx
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // ex
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    // fx
	};

	//------------------------------------------------------------------------
	// Lexer
	//------------------------------------------------------------------------

	Lexer::Lexer(XRef *pXref, Stream *pStream)
	{
		Object oTemp;

		m_oCurStream.InitStream(pStream);
		m_pStreams = new Array(pXref);
		m_pStreams->Add(m_oCurStream.Copy(&oTemp));
		m_nCurStreamIndex = 0;
		m_bFreeArray = true;
		m_oCurStream.StreamReset();
	}

	Lexer::Lexer(XRef *pXref, Object *pObject)
	{
		// Предполагаем, что в pObject либо поток, либо массив потоков
		Object oTemp;

		if (pObject->IsStream())
		{
			m_pStreams = new Array(pXref);
			m_bFreeArray = true;
			m_pStreams->Add(pObject->Copy(&oTemp));
		}
		else
		{
			m_pStreams = pObject->GetArray();
			m_bFreeArray = false;
		}

		m_nCurStreamIndex = 0;

		if (m_pStreams->GetCount() > 0)
		{
			m_pStreams->Get(m_nCurStreamIndex, &m_oCurStream);
			m_oCurStream.StreamReset();
		}
	}

	Lexer::~Lexer()
	{
		if (!m_oCurStream.IsNone())
		{
			m_oCurStream.StreamClose();
			m_oCurStream.Free();
		}
		if (m_bFreeArray)
		{
			delete m_pStreams;
		}
	}

	int Lexer::GetChar()
	{
		int nChar = EOF;
		while (!m_oCurStream.IsNone() && (nChar = m_oCurStream.StreamGetChar()) == EOF)
		{
			m_oCurStream.StreamClose();
			m_oCurStream.Free();
			++m_nCurStreamIndex;
			if (m_nCurStreamIndex < m_pStreams->GetCount())
			{
				m_pStreams->Get(m_nCurStreamIndex, &m_oCurStream);
				m_oCurStream.StreamReset();
			}
		}
		return nChar;
	}

	int Lexer::LookChar()
	{
		if (m_oCurStream.IsNone())
		{
			return EOF;
		}
		return m_oCurStream.StreamLookChar();
	}

	Object *Lexer::GetObject(Object *pObject)
	{
		char *pCurPointer;
		int nChar, nTempChar;
		bool bNegative = false, bDone = false;
		int nBracketCount = 0;
		int nInt = 0;
		double dFloat = 0, dScale = 0;
		StringExt *seString;
		int nCount = 0, nHexCharLen = 0;

		// Пропускаем комментарии и пробелы
		bool bComment = false;
		while (1)
		{
			if ((nChar = GetChar()) == EOF)
			{
				return pObject->InitEOF();
			}
			if (bComment)
			{
				if (nChar == '\r' || nChar == '\n')
					bComment = false;
			}
			else if (nChar == '%')
			{
				bComment = true;
			}
			else if (c_sSpecialChars[nChar] != 1)
			{
				break;
			}
		}

		// начинаем чтение объекта
		switch (nChar)
		{

			// Число
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case '-': case '.':
			bNegative = false;
			nInt = 0;
			if (nChar == '-')
			{
				bNegative = true;
			}
			else if (nChar == '.')
			{
				goto doReal;
			}
			else
			{
				nInt = nChar - '0';
			}
			while (1)
			{
				nChar = LookChar();
				if (isdigit(nChar))
				{
					GetChar();
					nInt = nInt * 10 + (nChar - '0');
				}
				else if (nChar == '.')
				{
					GetChar();
					goto doReal;
				}
				else
				{
					break;
				}
			}
			if (bNegative)
				nInt = -nInt;
			pObject->InitInt(nInt);
			break;
		doReal:
			dFloat = nInt;
			dScale = 0.1;
			while (1)
			{
				nChar = LookChar();
				if (nChar == '-')
				{
					// игнорируем знаки минус, появляющиеся по середине числа
					// (Adobe игнорирует такие ситуации)

					// TO DO: Error "Badly formatted number"
					GetChar();
					continue;
				}
				if (!isdigit(nChar))
				{
					break;
				}
				GetChar();
				dFloat = dFloat + dScale * (nChar - '0');
				dScale *= 0.1;
			}
			if (bNegative)
				dFloat = -dFloat;
			pObject->InitReal(dFloat);
			break;

			// Строка
			case '(':
			pCurPointer = m_sTempBuffer;
			nCount = 0;
			nBracketCount = 1; // счетчик собок
			bDone = false;
			seString = NULL;
			do {
				nTempChar = EOF;
				switch (nChar = GetChar())
				{

					case EOF:
#if 0
					// This breaks some PDF files, e.g., ones from Photoshop.
					case '\r':
					case '\n':
#endif
					// TO DO: Error "Unterminated string"
					bDone = true;
					break;

					case '(':
					++nBracketCount;
					nTempChar = nChar;
					break;

					case ')':
					if (--nBracketCount == 0)
					{
						bDone = true;
					}
					else
					{
						nTempChar = nChar;
					}
					break;
					case '\\':
					switch (nChar = GetChar())
					{
						case 'n': nTempChar = '\n'; break;
						case 'r': nTempChar = '\r'; break;
						case 't': nTempChar = '\t'; break;
						case 'b': nTempChar = '\b'; break;
						case 'f': nTempChar = '\f'; break;
						case '\\':
						case '(':
						case ')': nTempChar = nChar;    break;
						case '0': case '1': case '2': case '3':
						case '4': case '5': case '6': case '7':
						nTempChar = nChar - '0';
						nChar = LookChar();
						if (nChar >= '0' && nChar <= '7')
						{
							GetChar();
							nTempChar = (nTempChar << 3) + (nChar - '0');
							nChar = LookChar();
							if (nChar >= '0' && nChar <= '7')
							{
								GetChar();
								nTempChar = (nTempChar << 3) + (nChar - '0');
							}
						}
						break;
						case '\r':
						nChar = LookChar();
						if (nChar == '\n')
						{
							GetChar();
						}
						break;
						case '\n':
						break;
						case EOF:
						// TO DO: Error "Unterminated string"
						bDone = true;
						break;
						default:
						nTempChar = nChar;
						break;
					}
					break;

					default:
					nTempChar = nChar;
					break;
				}

				if (nTempChar != EOF)
				{
					if (nCount == TokenBufferSize)
					{
						if (!seString)
							seString = new StringExt(m_sTempBuffer, TokenBufferSize);
						else
							seString->Append(m_sTempBuffer, TokenBufferSize);
						pCurPointer = m_sTempBuffer;
						nCount = 0;
					}
					*pCurPointer++ = (char)nTempChar;
					++nCount;
				}
			} while (!bDone);

			if (!seString)
				seString = new StringExt(m_sTempBuffer, nCount);
			else
				seString->Append(m_sTempBuffer, nCount);
			pObject->InitString(seString);
			break;
			// Имя
			case '/':
			pCurPointer = m_sTempBuffer;
			nCount = 0;
			while ((nChar = LookChar()) != EOF && !c_sSpecialChars[nChar])
			{
				GetChar();
				if (nChar == '#')
				{
					nTempChar = LookChar();
					if (nTempChar >= '0' && nTempChar <= '9')
					{
						nChar = nTempChar - '0';
					}
					else if (nTempChar >= 'A' && nTempChar <= 'F')
					{
						nChar = nTempChar - 'A' + 10;
					}
					else if (nTempChar >= 'a' && nTempChar <= 'f')
					{
						nChar = nTempChar - 'a' + 10;
					}
					else
					{
						goto notEscChar;
					}
					GetChar();
					nChar <<= 4;
					nTempChar = GetChar();
					if (nTempChar >= '0' && nTempChar <= '9')
					{
						nChar += nTempChar - '0';
					}
					else if (nTempChar >= 'A' && nTempChar <= 'F')
					{
						nChar += nTempChar - 'A' + 10;
					}
					else if (nTempChar >= 'a' && nTempChar <= 'f')
					{
						nChar += nTempChar - 'a' + 10;
					}
					else
					{
						// TO DO: Error "Illegal digit in hex char in name"
					}
				}
			notEscChar:
				if (++nCount == TokenBufferSize)
				{
					// TO DO: Error "Name token too long"
					break;
				}
				*pCurPointer++ = nChar;
			}
			*pCurPointer = '\0';
			pObject->InitName(m_sTempBuffer);
			break;

			// Массив
			case '[':
			case ']':
			m_sTempBuffer[0] = nChar;
			m_sTempBuffer[1] = '\0';
			pObject->InitCommand(m_sTempBuffer);
			break;

			// Hex или Dictionary
			case '<':
			nChar = LookChar();

			// Dictionary
			if (nChar == '<')
			{
				GetChar();
				m_sTempBuffer[0] = m_sTempBuffer[1] = '<';
				m_sTempBuffer[2] = '\0';
				pObject->InitCommand(m_sTempBuffer);
			}
			else // Hex string
			{
				pCurPointer = m_sTempBuffer;
				nHexCharLen = nCount = 0;
				nTempChar = 0;
				seString = NULL;
				while (1)
				{
					nChar = GetChar();
					if (nChar == '>')
					{
						break;
					}
					else if (nChar == EOF)
					{
						// TO DO: Error "Unterminated hex string"
						break;
					}
					else if (c_sSpecialChars[nChar] != 1)
					{
						nTempChar = nTempChar << 4;
						if (nChar >= '0' && nChar <= '9')
							nTempChar += nChar - '0';
						else if (nChar >= 'A' && nChar <= 'F')
							nTempChar += nChar - 'A' + 10;
						else if (nChar >= 'a' && nChar <= 'f')
							nTempChar += nChar - 'a' + 10;
						else
						{
							// TO DO: Error "Illegal character <nChar> in hex string"
						}
						if (++nHexCharLen == 2)
						{
							if (nCount == TokenBufferSize)
							{
								if (!seString)
									seString = new StringExt(m_sTempBuffer, TokenBufferSize);
								else
									seString->Append(m_sTempBuffer, TokenBufferSize);
								pCurPointer = m_sTempBuffer;
								nCount = 0;
							}
							*pCurPointer++ = (char)nTempChar;
							++nCount;
							nTempChar = 0;
							nHexCharLen = 0;
						}
					}
				}
				if (!seString)
					seString = new StringExt(m_sTempBuffer, nCount);
				else
					seString->Append(m_sTempBuffer, nCount);
				if (nHexCharLen == 1)
					seString->Append((char)(nTempChar << 4));
				pObject->InitString(seString);
			}
			break;

			// Dictionary
			case '>':
			nChar = LookChar();
			if (nChar == '>')
			{
				GetChar();
				m_sTempBuffer[0] = m_sTempBuffer[1] = '>';
				m_sTempBuffer[2] = '\0';
				pObject->InitCommand(m_sTempBuffer);
			}
			else
			{
				// TO DO: Error "Illegal character '>'"
				pObject->InitError();
			}
			break;

			// Error
			case ')':
			case '{':
			case '}':
			// TO DO: Error "Illegal character <nChar>"
			pObject->InitError();
			break;

			// Command (это просто какое нибудь зарезервированное слово, например 'obj' или 'stream')
			default:
			pCurPointer = m_sTempBuffer;
			*pCurPointer++ = nChar;
			nCount = 1;
			while ((nChar = LookChar()) != EOF && !c_sSpecialChars[nChar])
			{
				GetChar();
				if (++nCount == TokenBufferSize)
				{
					// TO DO: Error "Command token too long"
					break;
				}
				*pCurPointer++ = nChar;
			}
			*pCurPointer = '\0';
			if (m_sTempBuffer[0] == 't' && !strcmp(m_sTempBuffer, "true"))
			{
				pObject->InitBool(true);
			}
			else if (m_sTempBuffer[0] == 'f' && !strcmp(m_sTempBuffer, "false"))
			{
				pObject->InitBool(false);
			}
			else if (m_sTempBuffer[0] == 'n' && !strcmp(m_sTempBuffer, "null"))
			{
				pObject->InitNull();
			}
			else
			{
				pObject->InitCommand(m_sTempBuffer);
			}
			break;
		}

		return pObject;
	}

	void Lexer::SkipToNextLine()
	{
		while (1)
		{
			int nChar = GetChar();
			if (nChar == EOF || nChar == '\n')
			{
				return;
			}
			if (nChar == '\r')
			{
				if ((nChar = LookChar()) == '\n')
				{
					GetChar();
				}
				return;
			}
		}
	}

	bool Lexer::IsSpace(int nChar)
	{
		return nChar >= 0 && nChar <= 0xff && c_sSpecialChars[nChar] == 1;
	}
}