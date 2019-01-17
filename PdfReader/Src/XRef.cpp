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
#include "MemoryUtils.h"
#include "Object.h"
#include "Stream.h"
#include "Lexer.h"
#include "Parser.h"
#include "Dict.h"
#include "ErrorConstants.h"
#include "XRef.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
#define XrefSearchSize 1024	// Читаем столько байт, начиная с конца файла, чтобы найти 'startxref'

	//---------------------------------------------------------------------------------------------------------------------------
	// Permission bits
	//---------------------------------------------------------------------------------------------------------------------------

#define PermissionPrint       ( 1 << 2 )
#define PermissionChange      ( 1 << 3 )
#define PermissionCopy        ( 1 << 4 )
#define PermissionNotes       ( 1 << 5 )
#define DefaultPermissionFlags 0xfffc

	//---------------------------------------------------------------------------------------------------------------------------
	// ObjectStream
	//---------------------------------------------------------------------------------------------------------------------------

	class ObjectStream
	{
	public:

		ObjectStream(XRef *pXref, int nObjectStreamNum);

		~ObjectStream();

		int GetObjectStreamNum()
		{
			return m_nObjectStreamNum;
		}

		Object *GetObject(int nObjectIndex, int nObjectNum, Object *pObject);

	private:

		int     m_nObjectStreamNum; // Порядковый номер объекта(Stream Objects)
		int     m_nObjectsCount;    // Количество объектов в потоке
		Object *m_arrObjects;       // Список объектов в потоке
		int    *m_arrObjectsNums;   // Порядковые номера объектов в потоке

	};

	ObjectStream::ObjectStream(XRef *pXref, int nObjectStreamNum)
	{
		Object oTempObject1, oTempObject2;

		m_nObjectStreamNum = nObjectStreamNum;
		m_nObjectsCount    = 0;
		m_arrObjects       = NULL;
		m_arrObjectsNums   = NULL;

		Object oObjectStream;
		if (!pXref->Fetch(m_nObjectStreamNum, 0, &oObjectStream)->IsStream())
		{
			oObjectStream.Free();
			return;
		}

		if (!oObjectStream.StreamGetDict()->Search("N", &oTempObject1)->IsInt())
		{
			oTempObject1.Free();
			oObjectStream.Free();
			return;
		}
		m_nObjectsCount = oTempObject1.GetInt();
		oTempObject1.Free();

		if (m_nObjectsCount <= 0)
		{
			oObjectStream.Free();
			return;
		}

		if (!oObjectStream.StreamGetDict()->Search("First", &oTempObject1)->IsInt())
		{
			oTempObject1.Free();
			oObjectStream.Free();
			return;
		}

		int nFirst = oTempObject1.GetInt();
		oTempObject1.Free();

		if (nFirst < 0)
		{
			oObjectStream.Free();
			return;
		}

		m_arrObjects     = new Object[m_nObjectsCount];
		m_arrObjectsNums = (int *)MemUtilsMallocArray(m_nObjectsCount, sizeof(int));
		int *arrnOffsets = (int *)MemUtilsMallocArray(m_nObjectsCount, sizeof(int));

		oObjectStream.StreamReset();
		oTempObject1.InitNull();

		Stream *pStream = new EmbedStream(oObjectStream.GetStream(), &oTempObject1, true, nFirst);
		Parser *pParser = new Parser(pXref, new Lexer(pXref, pStream), false);
		for (int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex)
		{
			pParser->GetObject(&oTempObject1);
			pParser->GetObject(&oTempObject2);

			if (!oTempObject1.IsInt() || !oTempObject2.IsInt())
			{
				oTempObject1.Free();
				oTempObject2.Free();
				delete pParser;
				MemUtilsFree(arrnOffsets);
				oObjectStream.Free();
				return;
			}
			m_arrObjectsNums[nIndex] = oTempObject1.GetInt();
			arrnOffsets[nIndex] = oTempObject2.GetInt();
			oTempObject1.Free();
			oTempObject2.Free();
			if (m_arrObjectsNums[nIndex] < 0 || arrnOffsets[nIndex] < 0 || (nIndex > 0 && arrnOffsets[nIndex] < arrnOffsets[nIndex - 1]))
			{
				delete pParser;
				MemUtilsFree(arrnOffsets);
				oObjectStream.Free();
				return;
			}
		}
		while (pStream->GetChar() != EOF);
		delete pParser;

		// Сдвигаемся к началу первого объекта
		for (int nIndex = nFirst; nIndex < arrnOffsets[0]; ++nIndex)
		{
			oObjectStream.GetStream()->GetChar();
		}

		for (int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex)
		{
			oTempObject1.InitNull();
			if (nIndex == m_nObjectsCount - 1)
			{
				pStream = new EmbedStream(oObjectStream.GetStream(), &oTempObject1, false, 0);
			}
			else
			{
				pStream = new EmbedStream(oObjectStream.GetStream(), &oTempObject1, true, arrnOffsets[nIndex + 1] - arrnOffsets[nIndex]);
			}
			pParser = new Parser(pXref, new Lexer(pXref, pStream), false);
			pParser->GetObject(&m_arrObjects[nIndex]);
			while (pStream->GetChar() != EOF);
			delete pParser;
		}

		MemUtilsFree(arrnOffsets);

		oObjectStream.Free();
		return;
	}

	ObjectStream::~ObjectStream()
	{
		if (m_arrObjects)
		{
			for (int nIndex = 0; nIndex < m_nObjectsCount; ++nIndex)
			{
				m_arrObjects[nIndex].Free();
			}
			delete[] m_arrObjects;
		}
		MemUtilsFree(m_arrObjectsNums);
	}

	Object *ObjectStream::GetObject(int nObjectIndex, int nObjectNum, Object *pObject)
	{
		if (nObjectIndex < 0 || nObjectIndex >= m_nObjectsCount || nObjectNum != m_arrObjectsNums[nObjectIndex])
		{
			return pObject->InitNull();
		}
		return m_arrObjects[nObjectIndex].Copy(pObject);
	}

	//------------------------------------------------------------------------
	// XRef
	//------------------------------------------------------------------------

	XRef::XRef(BaseStream *pStream)
	{
		m_oCS.InitializeCriticalSection();

		m_bValidXref = true;
		m_eErrorCode = errorNone;
		m_nEntrySize = 0;
		m_arrEntries = NULL;
		m_punStreamEnds = NULL;
		m_nStreamEndsCount = 0;
		m_pObjectStream = NULL;

		m_bEncrypted = false;
		m_nPermissionFlags = DefaultPermissionFlags;
		m_bOwnerPassword = false;

		// Если нет ссылки на данный объект, а ссылка нам нужна как идентефикатор, то выбираем некотый уникальный номер
		// (поскольку корректное версионное число состоит из 5 цифр, поэтому любое 6-циферное число будет безопасным решением)
		m_unRefGenCounter = 100000;

		// Читаем Trailer
		m_pStream = pStream;
		m_nStart  = m_pStream->GetStartPos();
		unsigned int nPos = GetStartXref();

		// Если возникла проблема при поиске 'startxref', пытаемся восстановить 
		// таблицу xref

		if (0 == nPos)
		{
			if (!(m_bValidXref = ConstructXRef()))
			{
				m_eErrorCode = errorDamaged;
				return;
			}
		}
		else // читаем таблицу Xref
		{
			while (ReadXRef(&nPos));

			// Если возникла проблема при поиске 'startxref', пытаемся восстановить 
			// таблицу xref
			if (!m_bValidXref)
			{
				if (!(m_bValidXref = ConstructXRef()))
				{
					m_eErrorCode = errorDamaged;
					return;
				}
			}
		}

		Object oTempObject;

		m_oTrailerDict.DictLookupAndCopy("Root", &oTempObject);
		if (oTempObject.IsRef())
		{
			m_nRootNum = oTempObject.GetRefNum();
			m_nRootGen = oTempObject.GetRefGen();
			oTempObject.Free();
		}
		else
		{
			oTempObject.Free();
			if (!(m_bValidXref = ConstructXRef()))
			{
				m_eErrorCode = errorDamaged;
				return;
			}
		}

		// Теперь устанавливаем в словарь Trailer ссылку на таблиу xref, чтобы мы могли
		// по данной таблице находить косвенные объекты
		m_oTrailerDict.GetDict()->SetXRef(this);
	}

	XRef::~XRef()
	{
		MemUtilsFree(m_arrEntries);
		m_oTrailerDict.Free();
		if (m_punStreamEnds)
		{
			MemUtilsFree(m_punStreamEnds);
		}
		if (m_pObjectStream)
		{
			delete m_pObjectStream;
		}

		m_oCS.DeleteCriticalSection();
	}

	unsigned int XRef::GetStartXref()
	{
		char sBuffer[XrefSearchSize + 1];
		char *pCurChar;
		int nChar = 0, nPos = 0, nCur = 0;

		// считываем последние xrefSearchSize байт
		m_pStream->SetPos(XrefSearchSize, -1);
		for (nPos = 0; nPos < XrefSearchSize; ++nPos)
		{
			if ((nChar = m_pStream->GetChar()) == EOF)
			{
				break;
			}
			sBuffer[nPos] = nChar;
		}
		sBuffer[nPos] = '\0';

		// ищем startxref
		for (nCur = nPos - 9; nCur >= 0; --nCur)
		{
			if (!strncmp(&sBuffer[nCur], "startxref", 9))
			{
				break;
			}
		}
		if (nCur < 0)
			return 0;

		for (pCurChar = &sBuffer[nCur + 9]; isspace(*pCurChar); ++pCurChar);

		m_unLastXRefOffset = StrintToUInt(pCurChar);

		return m_unLastXRefOffset;
	}

	// Считываем одну секцию таблицы Xref. Также считываем соответствующий словарь Trailer
	// и возвращаем указатель на предыдущий.
	bool XRef::ReadXRef(unsigned int *punPos)
	{
		Object oObject;
		bool bResult = true;

		oObject.InitNull();
		Parser *pParser = new Parser(NULL, new Lexer(NULL, m_pStream->MakeSubStream(m_nStart + *punPos, false, 0, &oObject)), true);
		pParser->GetObject(&oObject);

		// Парсим таблицу xref в старой форме
		if (oObject.IsCommand("xref"))
		{
			oObject.Free();
			bResult = ReadXRefTable(pParser, punPos);
		}
		// Парсим таблицу xref, когда она записана в виде потока
		else if (oObject.IsInt())
		{
			oObject.Free();
			if (!pParser->GetObject(&oObject)->IsInt())
			{
				oObject.Free();
				delete pParser;
				m_bValidXref = false;
				return false;
			}
			oObject.Free();
			if (!pParser->GetObject(&oObject)->IsCommand("obj"))
			{
				oObject.Free();
				delete pParser;
				m_bValidXref = false;
				return false;
			}
			oObject.Free();
			if (!pParser->GetObject(&oObject)->IsStream())
			{
				oObject.Free();
				delete pParser;
				m_bValidXref = false;
				return false;
			}
			bResult = ReadXRefStream(oObject.GetStream(), punPos);
			oObject.Free();

		}
		else
		{
			oObject.Free();
			delete pParser;
			m_bValidXref = false;
			return false;
		}

		delete pParser;
		return bResult;
	}

	bool XRef::ReadXRefTable(Parser *pParser, unsigned int *punPos)
	{
		Object oTempObject;

		while (1)
		{
			// сначала в секции идут номер первого объекта и количество объектов
			pParser->GetObject(&oTempObject);
			if (oTempObject.IsCommand("trailer"))
			{
				oTempObject.Free();
				break;
			}
			if (!oTempObject.IsInt())
			{
				oTempObject.Free();
				m_bValidXref = false;
				return false;
			}
			int nFirst = oTempObject.GetInt();
			oTempObject.Free();
			if (!pParser->GetObject(&oTempObject)->IsInt())
			{
				oTempObject.Free();
				m_bValidXref = false;
				return false;
			}
			int nCount = oTempObject.GetInt();
			oTempObject.Free();

			if (nFirst < 0 || nCount < 0 || nFirst + nCount < 0)
			{
				oTempObject.Free();
				m_bValidXref = false;
				return false;
			}
			if (nFirst + nCount > m_nEntrySize)
			{
				int nNewSize = 0;
				for (nNewSize = m_nEntrySize ? 2 * m_nEntrySize : 1024; nFirst + nCount > nNewSize && nNewSize > 0; nNewSize <<= 1);
				if (nNewSize < 0)
				{
					oTempObject.Free();
					m_bValidXref = false;
					return false;
				}
				m_arrEntries = (XRefEntry *)MemUtilsReallocArray(m_arrEntries, nNewSize, sizeof(XRefEntry));
				for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex)
				{
					m_arrEntries[nIndex].unOffset = 0xffffffff;
					m_arrEntries[nIndex].eType    = xrefEntryFree;
				}
				m_nEntrySize = nNewSize;
			}
			for (int nIndex = nFirst; nIndex < nFirst + nCount; ++nIndex)
			{
				// формат: nnnnnnnnnn ggggg n eol
				// nnnnnnnnnn - байтовый сдвиг
				// ggggg      - Generation number(номер версии объекта)
				// n          - Ключ, определяющий используется ли объект
				// eol        - Конец строки(два символа)
				if (!pParser->GetObject(&oTempObject)->IsInt())
				{
					oTempObject.Free();
					m_bValidXref = false;
					return false;
				}

				XRefEntry oEntry;
				oEntry.unOffset = (unsigned int)oTempObject.GetInt();
				oTempObject.Free();
				if (!pParser->GetObject(&oTempObject)->IsInt())
				{
					oTempObject.Free();
					m_bValidXref = false;
					return false;
				}
				oEntry.nGen = oTempObject.GetInt();
				oTempObject.Free();
				pParser->GetObject(&oTempObject);
				if (oTempObject.IsCommand("n"))
				{
					oEntry.eType = xrefEntryUncompressed;
				}
				else if (oTempObject.IsCommand("f"))
				{
					oEntry.eType = xrefEntryFree;
				}
				else
				{
					oTempObject.Free();
					m_bValidXref = false;
					return false;
				}
				oTempObject.Free();
				if (m_arrEntries[nIndex].unOffset == 0xffffffff)
				{
					m_arrEntries[nIndex] = oEntry;
					// в PDF файлах, созданных с помощью Intellectual Property
					// Network, имеется баг: таблица Xref начинается с объекта 
					// под номером 1, вместо 0.
					if (nIndex == 1 && nFirst == 1 && m_arrEntries[1].unOffset == 0 && m_arrEntries[1].nGen == 65535 && m_arrEntries[1].eType == xrefEntryFree)
					{
						nIndex = nFirst = 0;
						m_arrEntries[0] = m_arrEntries[1];
						m_arrEntries[1].unOffset = 0xffffffff;
					}
				}
			}
		}

		// Читаем словарь Trailer
		if (!pParser->GetObject(&oTempObject)->IsDict())
		{
			oTempObject.Free();
			m_bValidXref = false;
			return false;
		}

		// Считываем указатель 'Prev'
		Object oPrev;
		bool bPrev = false;
		oTempObject.GetDict()->SearchAndCopy("Prev", &oPrev);
		if (oPrev.IsInt())
		{
			*punPos = (unsigned int)oPrev.GetInt();
			bPrev = true;
		}
		else if (oPrev.IsRef())
		{
			// Некоторые программы пишут "/Prev NNN 0 R" вместо "/Prev NNN"
			*punPos = (unsigned int)oPrev.GetRefNum();
			bPrev = true;
		}
		else
		{
			bPrev = false;
		}
		oPrev.Free();

		// Сохраняем первый словарь Trailer
		if (m_oTrailerDict.IsNone())
		{
			oTempObject.Copy(&m_oTrailerDict);
		}

		// Проверяем есть ли элемент 'XRefStm'
		if (oTempObject.GetDict()->Search("XRefStm", &oPrev)->IsInt())
		{
			unsigned int unPos2 = (unsigned int)oPrev.GetInt();
			ReadXRef(&unPos2);
			if (!m_bValidXref)
			{
				oPrev.Free();
				oTempObject.Free();
				m_bValidXref = false;
				return false;
			}
		}
		oPrev.Free();

		oTempObject.Free();
		return bPrev;
	}

	bool XRef::ReadXRefStream(Stream *pXrefStream, unsigned int *punPos)
	{
		int arrW[3];

		Dict *pDict = pXrefStream->GetDict();
		Object oTemp;

		if (!pDict->SearchAndCopy("Size", &oTemp)->IsInt())
		{
			oTemp.Free();
			m_bValidXref = false;
			return false;
		}
		int nNewSize = oTemp.GetInt();
		oTemp.Free();

		if (nNewSize < 0)
		{
			oTemp.Free();
			m_bValidXref = false;
			return false;
		}
		if (nNewSize > m_nEntrySize)
		{
			m_arrEntries = (XRefEntry *)MemUtilsReallocArray(m_arrEntries, nNewSize, sizeof(XRefEntry));
			for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex)
			{
				m_arrEntries[nIndex].unOffset = 0xffffffff;
				m_arrEntries[nIndex].eType = xrefEntryFree;
			}
			m_nEntrySize = nNewSize;
		}

		if (!pDict->SearchAndCopy("W", &oTemp)->IsArray() || oTemp.ArrayGetLength() < 3)
		{
			oTemp.Free();
			m_bValidXref = false;
			return false;
		}
		for (int nIndex = 0; nIndex < 3; ++nIndex)
		{
			Object oWItem;
			if (!oTemp.ArrayGet(nIndex, &oWItem)->IsInt())
			{
				oWItem.Free();
				oTemp.Free();
				m_bValidXref = false;
				return false;
			}
			arrW[nIndex] = oWItem.GetInt();
			oWItem.Free();
			if (arrW[nIndex] < 0 || arrW[nIndex] > 4 && arrW[nIndex] != 8)
			{
				oTemp.Free();
				m_bValidXref = false;
				return false;
			}
		}
		oTemp.Free();

		pXrefStream->Reset();
		Object oIndex;
		pDict->SearchAndCopy("Index", &oIndex);

		if (oIndex.IsArray())
		{
			for (int nIndex = 0; nIndex + 1 < oIndex.ArrayGetLength(); nIndex += 2)
			{
				if (!oIndex.ArrayGet(nIndex, &oTemp)->IsInt())
				{
					oIndex.Free();
					oTemp.Free();
					m_bValidXref = false;
					return false;
				}
				int nFirst = oTemp.GetInt();
				oTemp.Free();
				if (!oIndex.ArrayGet(nIndex + 1, &oTemp)->IsInt())
				{
					oIndex.Free();
					oTemp.Free();
					m_bValidXref = false;
					return false;
				}
				int nCount = oTemp.GetInt();
				oTemp.Free();
				if (nFirst < 0 || nCount < 0 || !ReadXRefStreamSection(pXrefStream, arrW, nFirst, nCount))
				{
					oIndex.Free();
					m_bValidXref = false;
					return false;
				}
			}
		}
		else
		{
			if (!ReadXRefStreamSection(pXrefStream, arrW, 0, nNewSize))
			{
				oIndex.Free();
				m_bValidXref = false;
				return false;
			}
		}
		oIndex.Free();

		pDict->SearchAndCopy("Prev", &oTemp);
		bool bPrev = false;
		if (oTemp.IsInt())
		{
			*punPos = (unsigned int)oTemp.GetInt();
			bPrev = true;
		}
		else
		{
			bPrev = false;
		}
		oTemp.Free();
		if (m_oTrailerDict.IsNone())
		{
			m_oTrailerDict.InitDict(pDict);
		}

		return bPrev;
	}

	bool XRef::ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount)
	{
		if (nFirst + nCount < 0)
		{
			return false;
		}
		if (nFirst + nCount > m_nEntrySize)
		{
			int nNewSize = 0;
			for (nNewSize = m_nEntrySize ? 2 * m_nEntrySize : 1024; nFirst + nCount > nNewSize && nNewSize > 0; nNewSize <<= 1);
			if (nNewSize < 0)
			{
				return false;
			}
			m_arrEntries = (XRefEntry *)MemUtilsReallocArray(m_arrEntries, nNewSize, sizeof(XRefEntry));
			for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex)
			{
				m_arrEntries[nIndex].unOffset = 0xffffffff;
				m_arrEntries[nIndex].eType = xrefEntryFree;
			}
			m_nEntrySize = nNewSize;
		}
		for (int nIndex = nFirst; nIndex < nFirst + nCount; ++nIndex)
		{
			int nType = -1;
			int nChar = 0, nGen = 0, nJ = 0;
			unsigned int unOffset = 0;
			if (arrW[0] == 0)
			{
				nType = 1;
			}
			else
			{
				for (nType = 0, nJ = 0; nJ < arrW[0]; ++nJ)
				{
					if ((nChar = pXrefStream->GetChar()) == EOF)
					{
						return false;
					}
					nType = (nType << 8) + nChar;
				}
			}
			for (unOffset = 0, nJ = 0; nJ < arrW[1]; ++nJ)
			{
				if ((nChar = pXrefStream->GetChar()) == EOF)
				{
					return false;
				}
				unOffset = (unOffset << 8) + nChar;
			}
			for (nGen = 0, nJ = 0; nJ < arrW[2]; ++nJ)
			{
				if ((nChar = pXrefStream->GetChar()) == EOF)
				{
					return false;
				}
				nGen = (nGen << 8) + nChar;
			}
			if (m_arrEntries[nIndex].unOffset == 0xffffffff)
			{
				switch (nType)
				{
					case 0:
					m_arrEntries[nIndex].unOffset = unOffset;
					m_arrEntries[nIndex].nGen = nGen;
					m_arrEntries[nIndex].eType = xrefEntryFree;
					break;
					case 1:
					m_arrEntries[nIndex].unOffset = unOffset;
					m_arrEntries[nIndex].nGen = nGen;
					m_arrEntries[nIndex].eType = xrefEntryUncompressed;
					break;
					case 2:
					m_arrEntries[nIndex].unOffset = unOffset;
					m_arrEntries[nIndex].nGen = nGen;
					m_arrEntries[nIndex].eType = xrefEntryCompressed;
					break;
					default:
					return false;
				}
			}
		}

		return true;
	}

	// Пробуем построить таблицу Xref для поврежденного файла.
	bool XRef::ConstructXRef()
	{
		bool bGetRoot = false;

		MemUtilsFree(m_arrEntries);
		m_nEntrySize = 0;
		m_arrEntries = NULL;

		// TO DO : Error "PDF file is damaged - attempting to reconstruct xref table..."
		m_nStreamEndsCount = 0;

		int nStreamEndsSize = 0;

		m_pStream->Reset();

		while (1)
		{
			unsigned int nPos = m_pStream->GetPos();
			char sBuffer[256];
			if (!m_pStream->GetLine(sBuffer, 256))
			{
				break;
			}
			unsigned char *pCur = (unsigned char*)sBuffer;

			// Пропускаем пробелы
			while (*pCur && Lexer::IsSpace(*pCur & 0xff)) ++pCur;

			// Cчитываем словарь Trailer
			if (!strncmp((char*)pCur, "trailer", 7))
			{
				Object oTemp;
				oTemp.InitNull();
				Parser *pParser = new Parser(NULL, new Lexer(NULL, m_pStream->MakeSubStream(nPos + 7, false, 0, &oTemp)), false);
				Object oNewTrailerDict;
				pParser->GetObject(&oNewTrailerDict);
				if (oNewTrailerDict.IsDict())
				{
					oNewTrailerDict.DictLookupAndCopy("Root", &oTemp);
					if (oTemp.IsRef())
					{
						m_nRootNum = oTemp.GetRefNum();
						m_nRootGen = oTemp.GetRefGen();
						if (!m_oTrailerDict.IsNone())
						{
							m_oTrailerDict.Free();
						}
						oNewTrailerDict.Copy(&m_oTrailerDict);
						bGetRoot = true;
					}
					oTemp.Free();
				}
				oNewTrailerDict.Free();
				delete pParser;
			}
			else if (isdigit(*pCur)) // Ищем объект
			{
				int nNum = atoi((char*)pCur);
				if (nNum > 0)
				{
					do {
						++pCur;
					} while (*pCur && isdigit(*pCur));
					if (isspace(*pCur))
					{
						do {
							++pCur;
						} while (*pCur && isspace(*pCur));
						if (isdigit(*pCur))
						{
							int nGen = atoi((char*)pCur);
							do {
								++pCur;
							} while (*pCur && isdigit(*pCur));
							if (isspace(*pCur))
							{
								do {
									++pCur;
								} while (*pCur && isspace(*pCur));
								if (!strncmp((char*)pCur, "obj", 3))
								{
									if (nNum >= m_nEntrySize)
									{
										int nNewSize = (nNum + 1 + 255) & ~255;
										if (nNewSize < 0)
										{
											// TO DO: Error "Bad object number"
											return false;
										}
										m_arrEntries = (XRefEntry *)MemUtilsReallocArray(m_arrEntries, nNewSize, sizeof(XRefEntry));
										for (int nIndex = m_nEntrySize; nIndex < nNewSize; ++nIndex)
										{
											m_arrEntries[nIndex].unOffset = 0xffffffff;
											m_arrEntries[nIndex].eType = xrefEntryFree;
										}
										m_nEntrySize = nNewSize;
									}
									if (m_arrEntries[nNum].eType == xrefEntryFree || nGen >= m_arrEntries[nNum].nGen)
									{
										m_arrEntries[nNum].unOffset = nPos - m_nStart;
										m_arrEntries[nNum].nGen = nGen;
										m_arrEntries[nNum].eType = xrefEntryUncompressed;
									}
								}
							}
						}
					}
				}

			}
			else if (!strncmp((char*)pCur, "endstream", 9))
			{
				if (m_nStreamEndsCount == nStreamEndsSize)
				{
					nStreamEndsSize += 64;
					m_punStreamEnds = (unsigned int *)MemUtilsReallocArray(m_punStreamEnds, nStreamEndsSize, sizeof(int));
				}
				m_punStreamEnds[m_nStreamEndsCount++] = nPos;
			}
		}

		if (bGetRoot)
			return true;

		// TO DO: Error "Couldn't find trailer dictionary"
		return false;
	}

	void XRef::SetEncryption(int nPermissionFlags, bool bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm)
	{
		m_bEncrypted = true;
		
		m_nPermissionFlags = nPermissionFlags;
		m_bOwnerPassword = bOwnerPassword;
		m_nKeyLength = nKeyLength;

		for (int nIndex = 0; nIndex < m_nKeyLength; ++nIndex)
		{
			m_arrDecryptKey[nIndex] = sDecryptKey[nIndex];
		}
		m_nEncryptVersion   = nEncryptVersion;
		m_eEncryptAlgorithm = eEncryptAlgorithm;
	}

	bool XRef::CheckPrint(bool bIgnoreOwnerPassword)
	{
		return (!bIgnoreOwnerPassword && m_bOwnerPassword) || (m_nPermissionFlags & PermissionPrint);
	}

	bool XRef::CheckChange(bool bIgnoreOwnerPassword)
	{
		return (!bIgnoreOwnerPassword && m_bOwnerPassword) || (m_nPermissionFlags & PermissionChange);
	}

	bool XRef::CheckCopy(bool bIgnoreOwnerPassword)
	{
		return (!bIgnoreOwnerPassword && m_bOwnerPassword) || (m_nPermissionFlags & PermissionCopy);
	}

	bool XRef::CheckAddNotes(bool bIgnoreOwnerPassword)
	{
		return (!bIgnoreOwnerPassword && m_bOwnerPassword) || (m_nPermissionFlags & PermissionNotes);
	}

	Object *XRef::Fetch(int nNum, int nGen, Object *pObject)
	{
		CTemporaryCS oCS(&m_oCS);

		if (nNum < 0 || nNum >= m_nEntrySize)
		{
			return pObject->InitNull();
		}
		bool bObjNum = false, bObjGen = false, bObjContent = false;
		int count_obj_parser = 3;
		Object oObjNum;//, oObjGen, oObjContent;
		
		Parser *pParser = NULL;
		XRefEntry *pEntry = &m_arrEntries[nNum];
		switch (pEntry->eType)
		{
			case xrefEntryUncompressed:
			if (pEntry->nGen != nGen)
			{
				return pObject->InitNull();
			}
			oObjNum.InitNull();
			pParser = new Parser(this, new Lexer(this, m_pStream->MakeSubStream(m_nStart + pEntry->unOffset, false, 0, &oObjNum)), true);

			for (int i = 0; i < count_obj_parser; i++)
			{
				Object obj;
				pParser->GetObject(&obj);

				if (obj.IsCommand("ndobj")) //reformed.pdf
				{
					count_obj_parser++;
				}
				else
				{
					//todooo порядок запросов ???
					if (!bObjContent && obj.IsCommand("obj")) bObjContent = true;
					if (obj.IsInt())
					{
						if (!bObjNum && obj.GetInt() == nNum)		bObjNum = true;
						else if (!bObjGen && obj.GetInt() == nGen)	bObjGen = true;
					}
				}

				obj.Free();
			}
			if (!bObjContent || !bObjGen || !bObjNum)
			{
				delete pParser;
				return pObject->InitNull();
			}		
			//pParser->GetObject(&oObjNum);
			//pParser->GetObject(&oObjGen);
			//pParser->GetObject(&oObjContent);
			//if (!oObjNum.IsInt() || oObjNum.GetInt() != nNum || !oObjGen.IsInt() || oObjGen.GetInt() != nGen || !oObjContent.IsCommand("obj"))
			//{
			//	oObjNum.Free();
			//	oObjGen.Free();
			//	oObjContent.Free();
			//	delete pParser;
			//	return pObject->InitNull();
			//}
			pParser->GetObject(pObject, m_bEncrypted ? m_arrDecryptKey : (unsigned char *)NULL, m_eEncryptAlgorithm, m_nKeyLength, nNum, nGen);
			//oObjNum.Free();
			//oObjGen.Free();
			//oObjContent.Free();
			delete pParser;
			break;

			case xrefEntryCompressed:
			if (nGen != 0)
			{
				return pObject->InitNull();
			}
			if (!m_pObjectStream || m_pObjectStream->GetObjectStreamNum() != (int)pEntry->unOffset)
			{
				if (m_pObjectStream)
				{
					delete m_pObjectStream;
				}
				m_pObjectStream = new ObjectStream(this, pEntry->unOffset);
			}
			m_pObjectStream->GetObject(pEntry->nGen, nNum, pObject);
			break;

			default:
			return pObject->InitNull();
		}

		return pObject;
	}

	Object *XRef::GetDocInfo(Object *pObject)
	{
		CTemporaryCS oCS(&m_oCS);

		return m_oTrailerDict.DictLookup("Info", pObject);
	}
	Object *XRef::GetDocInfoCopy(Object *pObject)
	{
		CTemporaryCS oCS(&m_oCS);

		return m_oTrailerDict.DictLookupAndCopy("Info", pObject);
	}

	bool XRef::GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd)
	{
		if (m_nStreamEndsCount == 0 || nStreamStart > m_punStreamEnds[m_nStreamEndsCount - 1])
			return false;

		int nStart = -1;
		int nEnd = m_nStreamEndsCount - 1;
		// m_punStreamEnds[ nStart ] < streamStart <= m_punStreamEnds[ nEnd ]
		while (nEnd - nStart > 1)
		{
			int nMid = (nStart + nEnd) / 2;
			if (nStreamStart <= m_punStreamEnds[nMid])
			{
				nEnd = nMid;
			}
			else
			{
				nStart = nMid;
			}
		}
		*pnStreamEnd = m_punStreamEnds[nEnd];
		return true;
	}

	unsigned int XRef::StrintToUInt(char *sString)
	{
		char *pCur;
		int nIndex = 0;

		unsigned int unRes = 0;
		for (pCur = sString, nIndex = 0; *pCur && isdigit(*pCur) && nIndex < 10; ++pCur, ++nIndex)
		{
			unRes = 10 * unRes + (*pCur - '0');
		}
		return unRes;
	}
}
