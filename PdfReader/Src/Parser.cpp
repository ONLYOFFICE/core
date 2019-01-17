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
#include <stddef.h>
#include "Object.h"
#include "Array.h"
#include "Dict.h"
#include "Decrypt.h"
#include "Parser.h"
#include "XRef.h"

namespace PdfReader
{
	Parser::Parser(XRef *pXref, Lexer *pLexer, bool bAllowStreams)
	{
		m_pXref  = pXref;
		m_pLexer = pLexer;
		m_nInlineImage = 0;
		m_bAllowStreams = bAllowStreams;
		m_pLexer->GetObject(&m_oBuffer1);
		m_pLexer->GetObject(&m_oBuffer2);
	}

	Parser::~Parser()
	{
		m_oBuffer1.Free();
		m_oBuffer2.Free();
		delete m_pLexer;
	}

	Object *Parser::GetObject(Object *pObject, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen)
	{
		Stream *pStream = NULL;
		Object oTemp;
		int nNum = 0;

		// Обновляем буффер после Inline image data
		if (m_nInlineImage == 2)
		{
			m_oBuffer1.Free();
			m_oBuffer2.Free();
			m_pLexer->GetObject(&m_oBuffer1);
			m_pLexer->GetObject(&m_oBuffer2);
			m_nInlineImage = 0;
		}

		// Массив
		if (m_oBuffer1.IsCommand("["))
		{
			Shift();
			pObject->InitArray(m_pXref);
			while (!m_oBuffer1.IsCommand("]") && !m_oBuffer1.IsEOF())
				pObject->ArrayAdd(GetObject(&oTemp, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen));
			if (m_oBuffer1.IsEOF())
			{
				// TO DO: Error "End of file inside array"
			}
			Shift();
		}
		else if (m_oBuffer1.IsCommand("<<")) 	// Dictionary или Stream
		{
			Shift();
			pObject->InitDict(m_pXref);
			while (!m_oBuffer1.IsCommand(">>") && !m_oBuffer1.IsEOF())
			{
				if (!m_oBuffer1.IsName())
				{
					// TO DO: Error "Dictionary key must be a name object"
					Shift();
				}
				else
				{
					char *sKey = CopyString(m_oBuffer1.GetName());
					Shift();
					if (m_oBuffer1.IsEOF() || m_oBuffer1.IsError())
					{
						MemUtilsFree(sKey);
						break;
					}
					pObject->DictAdd(sKey, GetObject(&oTemp, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen));
				}
			}
			if (m_oBuffer1.IsEOF())
			{
				// TO DO: Error "End of file inside dictionary"
			}
			// Объекты Stream Objects не допустимы внутри потока или объекта типа Stream Objects
			if (m_bAllowStreams && m_oBuffer2.IsCommand("stream"))
			{
				if ((pStream = CreateStream(pObject, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen)))
				{
					pObject->InitStream(pStream);
				}
				else
				{
					pObject->Free();
					pObject->InitError();
				}
			}
			else
			{
				Shift();
			}
		}
		else if (m_oBuffer1.IsInt()) 	// Либо ссылка, либо число
		{
			nNum = m_oBuffer1.GetInt();
			Shift();
			if (m_oBuffer1.IsInt() && m_oBuffer2.IsCommand("R"))
			{
				pObject->InitRef(nNum, m_oBuffer1.GetInt());
				Shift();
				Shift();
			}
			else
			{
				pObject->InitInt(nNum);
			}
		}
		else if (m_oBuffer1.IsString() && sDecryptKey) // строка
		{
			StringExt *seTemp = m_oBuffer1.GetString();
			StringExt *seRes = new StringExt();
			oTemp.InitNull();
			DecryptStream *pDecrypt = new DecryptStream(new MemoryStream(seTemp->GetBuffer(), 0, seTemp->GetLength(), &oTemp), sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen);
			pDecrypt->Reset();
			int nChar = 0;
			while ((nChar = pDecrypt->GetChar()) != EOF)
			{
				seRes->Append((char)nChar);
			}
			delete pDecrypt;
			pObject->InitString(seRes);
			Shift();
		}
		else // простой объект
		{
			m_oBuffer1.Copy(pObject);
			Shift();
		}

		return pObject;
	}

	Stream *Parser::CreateStream(Object *pDict, unsigned char *sDecryptKey, CryptAlgorithm eEncryptAlgorithm, int nKeyLength, int nObjectNum, int nObjectGen)
	{
		Object oTemp;
		unsigned int nLength, unEndPos;

		m_pLexer->SkipToNextLine();
		unsigned int unPos = m_pLexer->GetPos();

		// Считываем длину
		pDict->DictLookup("Length", &oTemp);
		if (oTemp.IsInt())
		{
			nLength = (unsigned int)oTemp.GetInt();
			oTemp.Free();
		}
		else
		{
			oTemp.Free();
			unsigned int unEndPos = unPos;

			while (!m_oBuffer2.IsCommand("endstream") && !m_oBuffer2.IsEOF())
			{
				unEndPos = m_pLexer->GetPos();
				Shift();
			}

			nLength = unEndPos - unPos;
			m_pLexer->SetPos(unPos);

			// Оставим на всякий случай заглушку
			if (nLength <= 0)
				nLength = 5000;
		}

		// Меняем длину, если файл поврежден
		if (m_pXref && m_pXref->GetStreamEnd(unPos, &unEndPos))
		{
			nLength = unEndPos - unPos;
		}

		// В сильно повержденных файлах PDF, выставляем конец потока сразу
		// после его начала
		if (!m_pLexer->GetStream())
		{
			return NULL;
		}

		BaseStream *pBaseStream = m_pLexer->GetStream()->GetBaseStream();

		// Пропускаем данный потока
		m_pLexer->SetPos(unPos + nLength);

		// Проверяем 'endstream'
		Shift();  // '>>'
		Shift();  // 'endstream'
		if (m_oBuffer1.IsCommand("endstream"))
		{
			Shift();
		}
		else
		{
			// TO DO : Error "Missing 'endstream'"

			// Заплатка для поврежденных файлов: 
			nLength += 5000;
		}

		Stream *pStream = pBaseStream->MakeSubStream(unPos, true, nLength, pDict);

		// Decryption
		if (sDecryptKey)
		{
			pStream = new DecryptStream(pStream, sDecryptKey, eEncryptAlgorithm, nKeyLength, nObjectNum, nObjectGen);
		}

		// Filters
		pStream = pStream->AddFilters(pDict);

		return pStream;
	}

	void Parser::Shift()
	{
		if (m_nInlineImage > 0)
		{
			if (m_nInlineImage < 2)
			{
				++m_nInlineImage;
			}
			else
			{
				// В поврежденном потоке, если 'ID' появляется в середине потока,
				// нужно сбросить параметры
				m_nInlineImage = 0;
			}
		}
		else if (m_oBuffer2.IsCommand("ID"))
		{
			m_pLexer->SkipChar(); // пропускаем символ после команды 'ID'
			m_nInlineImage = 1;
		}
		m_oBuffer1.Free();
		m_oBuffer1 = m_oBuffer2;
		if (m_nInlineImage > 0) // не буфферизируем данные Inline Image
			m_oBuffer2.InitNull();
		else
			m_pLexer->GetObject(&m_oBuffer2);
	}
}