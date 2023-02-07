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
#include "FontTTWriter.h"
#include "../../DesktopEditor/common/File.h"

#define ttcfTag 0x74746366
#define cmapTag 0x636d6170
#define glyfTag 0x676c7966
#define headTag 0x68656164
#define hheaTag 0x68686561
#define hmtxTag 0x686d7478
#define locaTag 0x6c6f6361
#define nameTag 0x6e616d65
#define  os2Tag 0x4f532f32
#define postTag 0x706f7374

#define  cvtTag 0x63767420
#define fpgmTag 0x6670676d
#define maxpTag 0x6d617870
#define prepTag 0x70726570

namespace PdfWriter
{
	struct TrueTypeTable
	{
		unsigned int unTag;
		unsigned int unChecksum;
		int          nOffset;
		int          nOrigOffset;
		int          nLen;
	};
	struct TrueTypeCmap
	{
		int nPlatform;
		int nEncoding;
		int nOffset;
		int nLen;
		int nFormat;
	};
	struct TrueTypeLoca
	{
		int nIndex;
		int nOrigOffset;
		int nNewOffset;
		int nLen;
	};
	static int CompareTrueTypeLocaOffset(const void *pL1, const void *pL2)
	{
		TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
		TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

		if (pLoca1->nOrigOffset == pLoca2->nOrigOffset)
			return pLoca1->nIndex - pLoca2->nIndex;

		return pLoca1->nOrigOffset - pLoca2->nOrigOffset;
	}
	static int CompareTrueTypeLocaIndex(const void *pL1, const void *pL2)
	{
		TrueTypeLoca *pLoca1 = (TrueTypeLoca *)pL1;
		TrueTypeLoca *pLoca2 = (TrueTypeLoca *)pL2;

		return pLoca1->nIndex - pLoca2->nIndex;
	}
	static int CompareTrueTypeTableTag(const void *pTab1, const void *pTab2)
	{
		TrueTypeTable *pTable1 = (TrueTypeTable *)pTab1;
		TrueTypeTable *pTable2 = (TrueTypeTable *)pTab2;

		return (int)pTable1->unTag - (int)pTable2->unTag;
	}
	//----------------------------------------------------------------------------------------
	// CFontFileBase
	//----------------------------------------------------------------------------------------
	CFontFileBase::CFontFileBase(char *sFile, int nLen, bool bFreeFileData)
	{
		m_sFileData = m_sFile = (unsigned char *)sFile;
		m_nLen = nLen;
		m_bFreeFileData = bFreeFileData;
	}
	CFontFileBase::~CFontFileBase()
	{
		if (m_bFreeFileData)
			free(m_sFileData);
	}
	char*        CFontFileBase::ReadFile(const std::wstring & wsFileName, int *pnFileLen)
	{
		NSFile::CFileBinary oFile;

		if (oFile.OpenFile(wsFileName) == false)
			return NULL;

		int nLen = oFile.GetFileSize();
		DWORD nLenRead = 0;

		char *sBuffer = (char *)malloc(nLen);
		if (NULL != sBuffer)
		{
			if ((int)oFile.ReadFile((BYTE*)sBuffer, nLen, nLenRead) == false)
			{
				if (sBuffer)
					free(sBuffer);

				oFile.CloseFile();
				return NULL;
			}

			oFile.CloseFile();
			*pnFileLen = nLen;
		}
		else
		{
			oFile.CloseFile();
		}
		return sBuffer;
	}
	int          CFontFileBase::GetS8(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		if (nRes & 0x80)
			nRes |= ~0xff;
		return nRes;
	}
	int          CFontFileBase::GetU8(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;
		if (nPos < 0 || nPos >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		return m_sFile[nPos];
	}
	int          CFontFileBase::GetS16BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 1 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		if (nRes & 0x8000)
			nRes |= ~0xffff;
		return nRes;
	}
	int          CFontFileBase::GetU16BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 1 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		return nRes;
	}
	int          CFontFileBase::GetS32BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 3 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		if (nRes & 0x80000000)
			nRes |= ~0xffffffff;

		return nRes;
	}
	unsigned int CFontFileBase::GetU32BE(int nPos, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + 3 >= m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		unsigned int nRes = m_sFile[nPos];
		nRes = (nRes << 8) + m_sFile[nPos + 1];
		nRes = (nRes << 8) + m_sFile[nPos + 2];
		nRes = (nRes << 8) + m_sFile[nPos + 3];
		return nRes;
	}
	unsigned int CFontFileBase::GetUVarBE(int nPos, int nSize, bool *pbSuccess)
	{
		*pbSuccess = true;

		if (nPos < 0 || nPos + nSize > m_nLen)
		{
			*pbSuccess = false;
			return 0;
		}
		unsigned int nRes = 0;
		for (int nIndex = 0; nIndex < nSize; ++nIndex)
			nRes = (nRes << 8) + m_sFile[nPos + nIndex];

		return nRes;
	}
	bool         CFontFileBase::CheckRegion(int nPos, int nSize)
	{
		return (nPos >= 0 && nPos + nSize >= nPos && nPos + nSize <= m_nLen);
	}
	//----------------------------------------------------------------------------------------
	// CFontFileTrueType
	//----------------------------------------------------------------------------------------
	CFontFileTrueType::CFontFileTrueType(char *sBuffer, int nLen, bool bFreeFileData, unsigned int unFontIndex) :CFontFileBase(sBuffer, nLen, bFreeFileData)
	{
		m_pTables      = NULL;
		m_nTablesCount = 0;
		m_pCMaps       = NULL;
		m_nCMapsCount  = 0;
		m_bSuccess     = false;
		m_unFontIndex  = unFontIndex;

		m_nAscent      = 1000;
		m_nDescent     = -500;
		m_nCapHeight   = 800;
		m_nWeight      = 400;

		Parse();
	}
	CFontFileTrueType* CFontFileTrueType::LoadFromBuffer(char *sBuffer, int nLen, unsigned int unIndex)
	{
		CFontFileTrueType *pTTF = new CFontFileTrueType(sBuffer, nLen, false, unIndex);
		if (!pTTF->m_bSuccess)
		{
			delete pTTF;
			return NULL;
		}
		return pTTF;
	}
	CFontFileTrueType* CFontFileTrueType::LoadFromFile(const std::wstring& wsFileName, unsigned int unIndex)
	{
		char *sBuffer;
		int nLen = 0;

		if (!(sBuffer = CFontFileBase::ReadFile(wsFileName, &nLen)))
			return NULL;

		CFontFileTrueType *pTTF = new CFontFileTrueType(sBuffer, nLen, true, unIndex);
		if (!pTTF->m_bSuccess)
		{
			delete pTTF;
			return NULL;
		}
		return pTTF;
	}
	CFontFileTrueType::~CFontFileTrueType()
	{
		if (m_pTables)
			free(m_pTables);

		if (m_pCMaps)
			free(m_pCMaps);
	}
	void         CFontFileTrueType::WriteTTF(CStream* pOutputStream, char* sName, unsigned short* pCodeToGID, unsigned int unCodesCount, unsigned char* pUseGlyfs, long lGlyfsCount)
	{
		static char arrCMapTab[36] =
		{
			0, 0,                   // table version number
			0, 1,                   // number of encoding tables
			0, 1,                   // platform ID
			0, 0,                   // encoding ID
			0, 0, 0, 12,            // offset of subtable
			0, 4,                   // subtable format
			0, 24,                  // subtable length
			0, 0,                   // subtable version
			0, 2,                   // segment count * 2
			0, 2,                   // 2 * 2 ^ floor(log2(segCount))
			0, 0,                   // floor(log2(segCount))
			0, 0,                   // 2*segCount - 2*2^floor(log2(segCount))
			(char)0xff, (char)0xff, // endCount[0]
			0, 0,			        // reserved
			0, 0,                   // startCount[0]
			0, 0,                   // idDelta[0]
			0, 0                    // pad to a mulitple of four bytes
		};

		static char arrNameTab[8] =
		{
			0, 0,			// format
			0, 0,			// number of name records
			0, 6,			// offset to start of string storage
			0, 0			// pad to multiple of four bytes
		};
		static char arrPostTab[32] =
		{
			0, 1, 0, 0,  // format
			0, 0, 0, 0,  // italic angle
			0, 0,        // underline position
			0, 0,        // underline thickness
			0, 0, 0, 0,  // fixed pitch
			0, 0, 0, 0,  // min Type 42 memory
			0, 0, 0, 0,  // max Type 42 memory
			0, 0, 0, 0,  // min Type 1 memory
			0, 0, 0, 0   // max Type 1 memory
		};
		static char arrOS2Tab[86] =
		{
			0, 1,			// version
			0, 1,			// xAvgCharWidth
			0, 0,			// usWeightClass
			0, 0,			// usWidthClass
			0, 0,			// fsType
			0, 0,			// ySubscriptXSize
			0, 0,			// ySubscriptYSize
			0, 0,			// ySubscriptXOffset
			0, 0,			// ySubscriptYOffset
			0, 0,			// ySuperscriptXSize
			0, 0,			// ySuperscriptYSize
			0, 0,			// ySuperscriptXOffset
			0, 0,			// ySuperscriptYOffset
			0, 0,			// yStrikeoutSize
			0, 0,			// yStrikeoutPosition
			0, 0,			// sFamilyClass
			0, 0, 0, 0, 0,  // panose
			0, 0, 0, 0, 0,
			0, 0, 0, 0,     // ulUnicodeRange1
			0, 0, 0, 0,	    // ulUnicodeRange2
			0, 0, 0, 0,     // ulUnicodeRange3
			0, 0, 0, 0,     // ulUnicodeRange4
			0, 0, 0, 0,     // achVendID
			0, 0,           // fsSelection
			0, 0,           // usFirstCharIndex
			0, 0,           // usLastCharIndex
			0, 0,           // sTypoAscender
			0, 0,           // sTypoDescender
			0, 0,           // sTypoLineGap
			0, 0,           // usWinAscent
			0, 0,           // usWinDescent
			0, 0, 0, 0,     // ulCodePageRange1
			0, 0, 0, 0      // ulCodePageRange2
		};
		bool badCmapLen, abbrevHMTX;

		int nZeroLengthTables;
		int nHMetrics, nAdvWidth, nLeftSideBearing;
		TrueTypeTable *pNewTables;
		char *arrNewNameTable, *arrNewCmapTable, *arrNewHHEATable, *arrNewHMTXTable;
		int nNewTables, nCmapIndex, nCmapLen, nGlyphLen, nNewNameLen, nNewCmapLen, nNext;
		int nNewHHEALen, nNewHMTXLen;
		unsigned int nLocaChecksum, nGlyphChecksum, nFileChecksum;
		char *arrTableDir;
		char arrLocaBuf[4], arrChecksumBuf[4];
		unsigned int t;
		int nPos = 0, i, j, k, n;

		// Записываем OpenType шрифт не меняя его
		if (m_bOpenTypeCFF)
		{
			WriteOTF(pOutputStream, sName, pCodeToGID);
			return;
		}

		// Проверяем недостающие таблицы
		bool bMissingCmap = (nCmapIndex = SeekTable("cmap")) < 0;
		bool bMissingName = SeekTable("name") < 0;
		bool bMissingPost = SeekTable("post") < 0;
		bool bMissingOS2  = SeekTable("OS/2") < 0;

		TrueTypeLoca *pLocaTable = (TrueTypeLoca *)malloc((m_nGlyphs + 1) * sizeof(TrueTypeLoca));
		bool bUnsortedLoca = false;
		i = SeekTable("loca");
		nPos = m_pTables[i].nOffset;
		bool bSuccess = true;

		for (i = 0; i <= m_nGlyphs; ++i)
		{
			if (m_nLocaFormat)
			{
				pLocaTable[i].nOrigOffset = (int)GetU32BE(nPos + i * 4, &bSuccess);
			}
			else
			{
				pLocaTable[i].nOrigOffset = 2 * GetU16BE(nPos + i * 2, &bSuccess);
			}
			if (i > 0 && pLocaTable[i].nOrigOffset < pLocaTable[i - 1].nOrigOffset)
			{
				bUnsortedLoca = true;
			}
			// Описание глифа должны быть как минимум 12 байт (nContours,
			// xMin, yMin, xMax, yMax, instructionLength - каждый по 2 байта);
			if (i > 0 && pLocaTable[i].nOrigOffset - pLocaTable[i - 1].nOrigOffset > 0 && pLocaTable[i].nOrigOffset - pLocaTable[i - 1].nOrigOffset < 12)
			{
				pLocaTable[i - 1].nOrigOffset = pLocaTable[i].nOrigOffset;
				bUnsortedLoca = true;
			}
			pLocaTable[i].nIndex = i;
		}

		// Проверяем наличие нулевых таблиц
		nZeroLengthTables = 0;
		for (i = 0; i < m_nTablesCount; ++i)
		{
			if (m_pTables[i].nLen == 0)
				++nZeroLengthTables;
		}

		// Проверяем длину таблицы Cmap
		badCmapLen = false;
		nCmapLen = 0;
		if (!bMissingCmap)
		{
			nCmapLen = m_pCMaps[0].nOffset + m_pCMaps[0].nLen;
			for (i = 1; i < m_nCMapsCount; ++i)
			{
				if (m_pCMaps[i].nOffset + m_pCMaps[i].nLen > nCmapLen)
				{
					nCmapLen = m_pCMaps[i].nOffset + m_pCMaps[i].nLen;
				}
			}
			nCmapLen -= m_pTables[nCmapIndex].nOffset;
			if (nCmapLen > m_pTables[nCmapIndex].nLen)
			{
				badCmapLen = true;
			}
		}

		// Проверяем, является ли таблица 'hmtx' сокращенной.
		i = SeekTable("hhea");
		nHMetrics = GetU16BE(m_pTables[i].nOffset + 34, &bSuccess);
		abbrevHMTX = nHMetrics < m_nGlyphs;

		// Если все впорядке, и нам не надо переписывать таблицы 'cmap' и 'name', тогда пишем файл TTF как он есть
		if (!bMissingCmap && !bMissingName && !bMissingPost && !bMissingOS2 && !bUnsortedLoca && !badCmapLen && !abbrevHMTX && nZeroLengthTables == 0 && !sName && !pCodeToGID)
		{
			pOutputStream->Write((BYTE *)m_sFile, m_nLen);
			free(pLocaTable);
			return;
		}

		// Сортируем таблицу 'loca': некоторые шрифты содержат неупорядоченную
		// таблицу 'loca'; а некоторые шрифты с нормальной таблицей 'loca' 
		// содержат пустые элементы в середине таблицы, cmpTrueTypeLocaOffset
		// использует сдвиги как основной ключ для сортировки, а номера глифов
		// как второй ключ (чтобы элементы в таблице, которые имели одинаковую позицию 
		// шли в том же порядке, как и в исходном шрифте)
		nGlyphLen = 0;
		if (bUnsortedLoca || pUseGlyfs)
		{
			qsort(pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaOffset);
			for (i = 0; i < m_nGlyphs; ++i)
			{
				pLocaTable[i].nLen = pLocaTable[i + 1].nOrigOffset - pLocaTable[i].nOrigOffset;
			}
			pLocaTable[m_nGlyphs].nLen = 0;
			qsort(pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaIndex);
			nPos = 0;

			for (i = 0; i <= m_nGlyphs; ++i)
			{
				// TO DO: Протестировать тут запись только тех глифов, которые нам нужны

				if (pUseGlyfs && lGlyfsCount == m_nGlyphs)
				{
					pLocaTable[i].nNewOffset = nPos;
					int nCurGlyfLen = pLocaTable[i].nLen;
					pLocaTable[i].nLen = 0;

					if (1 == pUseGlyfs[i])
					{
						pLocaTable[i].nLen = nCurGlyfLen;

						nPos += pLocaTable[i].nLen;
						if (nPos & 3)
						{
							nPos += 4 - (nPos & 3);
						}
					}
				}
				else
				{
					pLocaTable[i].nNewOffset = nPos;
					nPos += pLocaTable[i].nLen;
					if (nPos & 3)
					{
						nPos += 4 - (nPos & 3);
					}
				}
			}
			nGlyphLen = nPos;
		}

		// Вычисляем чексуммы таблиц 'loca' и 'glyf'
		nLocaChecksum = nGlyphChecksum = 0;
		if (bUnsortedLoca || pUseGlyfs)
		{
			if (m_nLocaFormat)
			{
				for (j = 0; j <= m_nGlyphs; ++j)
				{
					nLocaChecksum += pLocaTable[j].nNewOffset;
				}
			}
			else
			{
				for (j = 0; j <= m_nGlyphs; j += 2)
				{
					nLocaChecksum += pLocaTable[j].nNewOffset << 16;
					if (j + 1 <= m_nGlyphs)
					{
						nLocaChecksum += pLocaTable[j + 1].nNewOffset;
					}
				}
			}
			nPos = m_pTables[SeekTable("glyf")].nOffset;
			for (j = 0; j < m_nGlyphs; ++j)
			{
				n = pLocaTable[j].nLen;
				if (n > 0)
				{
					k = pLocaTable[j].nOrigOffset;
					if (CheckRegion(nPos + k, n))
					{
						nGlyphChecksum += ComputeTableChecksum(m_sFile + nPos + k, n);
					}
				}
			}
		}

		// Строим новую таблицу 'name'
		if (sName)
		{
			n = strlen(sName);
			nNewNameLen = (6 + 4 * 12 + 2 * (3 * n + 7) + 3) & ~3;
			arrNewNameTable = (char *)malloc(nNewNameLen);
			memset(arrNewNameTable, 0, nNewNameLen);
			arrNewNameTable[0] = 0;		// format selector
			arrNewNameTable[1] = 0;
			arrNewNameTable[2] = 0;		// number of name records
			arrNewNameTable[3] = 4;
			arrNewNameTable[4] = 0;		// offset to start of string storage
			arrNewNameTable[5] = 6 + 4 * 12;
			nNext = 0;
			for (i = 0; i < 4; ++i)
			{
				arrNewNameTable[6 + i * 12 + 0] = 0;	// platform ID = Microsoft
				arrNewNameTable[6 + i * 12 + 1] = 3;
				arrNewNameTable[6 + i * 12 + 2] = 0;	// encoding ID = Unicode
				arrNewNameTable[6 + i * 12 + 3] = 1;
				arrNewNameTable[6 + i * 12 + 4] = 0x04;	// language ID = American English
				arrNewNameTable[6 + i * 12 + 5] = 0x09;
				arrNewNameTable[6 + i * 12 + 6] = 0;	// name ID
				arrNewNameTable[6 + i * 12 + 7] = i + 1;
				arrNewNameTable[6 + i * 12 + 8] = i + 1 == 2 ? 0 : ((2 * n) >> 8); // string length
				arrNewNameTable[6 + i * 12 + 9] = i + 1 == 2 ? 14 : ((2 * n) & 0xff);
				arrNewNameTable[6 + i * 12 + 10] = nNext >> 8;		    // string offset
				arrNewNameTable[6 + i * 12 + 11] = nNext & 0xff;
				if (i + 1 == 2)
				{
					memcpy(arrNewNameTable + 6 + 4 * 12 + nNext, "\0R\0e\0g\0u\0l\0a\0r", 14);
					nNext += 14;
				}
				else
				{
					for (j = 0; j < n; ++j)
					{
						arrNewNameTable[6 + 4 * 12 + nNext + 2 * j] = 0;
						arrNewNameTable[6 + 4 * 12 + nNext + 2 * j + 1] = sName[j];
					}
					nNext += 2 * n;
				}
			}
		}
		else
		{
			nNewNameLen = 0;
			arrNewNameTable = NULL;
		}

		// Строим новую таблицу 'cmap'
		if (pCodeToGID)
		{
			unsigned short ushSubTableLen = 10 + unCodesCount * 2;
			nNewCmapLen = 12 + ushSubTableLen;
			arrNewCmapTable = (char *)malloc(nNewCmapLen);
			arrNewCmapTable[0] = 0;           // table version number = 0
			arrNewCmapTable[1] = 0;           //
			arrNewCmapTable[2] = 0;           // number of encoding tables = 1
			arrNewCmapTable[3] = 1;           //                          
			arrNewCmapTable[4] = 0;           // platform ID = 1 (MacOS) // Эти два поля обязательно должны
			arrNewCmapTable[5] = 1;           //                         // иметь таки значения, иначе, Adobe
			arrNewCmapTable[6] = 0;           // encoding ID = 0         // Acrobat может открыть данный шрифт.
			arrNewCmapTable[7] = 0;           //                         // 
			arrNewCmapTable[8] = 0;           // offset of subtable
			arrNewCmapTable[9] = 0;           //
			arrNewCmapTable[10] = 0;          //
			arrNewCmapTable[11] = 12;         //
			arrNewCmapTable[12] = 0;          // subtable format = 6
			arrNewCmapTable[13] = 6;          //
			arrNewCmapTable[14] = (ushSubTableLen >> 8) & 0xFF; // subtable length
			arrNewCmapTable[15] = ushSubTableLen & 0xFF;        //
			arrNewCmapTable[16] = 0;          // subtable version = 0
			arrNewCmapTable[17] = 0;          //
			arrNewCmapTable[18] = 0;          // firstCode
			arrNewCmapTable[19] = 0;          //
			arrNewCmapTable[20] = 0x01;       // entryCount
			arrNewCmapTable[21] = 0x00;       //

			for (i = 0; i < unCodesCount; ++i)
			{
				arrNewCmapTable[22 + 2 * i]     = pCodeToGID[i] >> 8;
				arrNewCmapTable[22 + 2 * i + 1] = pCodeToGID[i] & 0xff;
			}
		}
		else
		{
			nNewCmapLen = 0;
			arrNewCmapTable = NULL;
		}

		// Генерируем новую таблицу 'hmtx' и обновляем таблицу 'hhea'
		if (abbrevHMTX)
		{
			i = SeekTable("hhea");
			nPos = m_pTables[i].nOffset;
			nNewHHEALen = 36;
			arrNewHHEATable = (char *)malloc(nNewHHEALen);
			for (i = 0; i < nNewHHEALen; ++i)
			{
				arrNewHHEATable[i] = GetU8(nPos++, &bSuccess);
			}
			arrNewHHEATable[34] = m_nGlyphs >> 8;
			arrNewHHEATable[35] = m_nGlyphs & 0xff;
			i = SeekTable("hmtx");
			nPos = m_pTables[i].nOffset;
			nNewHMTXLen = 4 * m_nGlyphs;
			arrNewHMTXTable = (char *)malloc(nNewHMTXLen);
			nAdvWidth = 0;
			for (i = 0; i < nHMetrics; ++i)
			{
				nAdvWidth = GetU16BE(nPos, &bSuccess);
				nLeftSideBearing = GetU16BE(nPos + 2, &bSuccess);
				nPos += 4;
				arrNewHMTXTable[4 * i] = nAdvWidth >> 8;
				arrNewHMTXTable[4 * i + 1] = nAdvWidth & 0xff;
				arrNewHMTXTable[4 * i + 2] = nLeftSideBearing >> 8;
				arrNewHMTXTable[4 * i + 3] = nLeftSideBearing & 0xff;
			}
			for (; i < m_nGlyphs; ++i)
			{
				nLeftSideBearing = GetU16BE(nPos, &bSuccess);
				nPos += 2;
				arrNewHMTXTable[4 * i] = nAdvWidth >> 8;
				arrNewHMTXTable[4 * i + 1] = nAdvWidth & 0xff;
				arrNewHMTXTable[4 * i + 2] = nLeftSideBearing >> 8;
				arrNewHMTXTable[4 * i + 3] = nLeftSideBearing & 0xff;
			}
		}
		else
		{
			arrNewHHEATable = arrNewHMTXTable = NULL;
			nNewHHEALen = nNewHMTXLen = 0;
		}

		// Создаем список таблиц:
		// - сохраняем исходные ненулевые таблицы
		// - переписываем длину таблицы 'cmap', если необходимо
		// - добавляем недостающие таблицы
		// - сортируем таблицы по тэгам
		// - вычисляем новые позиции таблиц, с учетом 4-байтового выравнивания
		// - пересчитываем чексуммы таблиц
		nNewTables = m_nTablesCount - nZeroLengthTables + (bMissingCmap ? 1 : 0) + (bMissingName ? 1 : 0) + (bMissingPost ? 1 : 0) + (bMissingOS2 ? 1 : 0);
		pNewTables = (TrueTypeTable *)malloc(nNewTables * sizeof(TrueTypeTable));
		j = 0;
		for (i = 0; i < m_nTablesCount; ++i)
		{
			if (m_pTables[i].nLen > 0)
			{
				pNewTables[j] = m_pTables[i];
				pNewTables[j].nOrigOffset = m_pTables[i].nOffset;
				if (CheckRegion(m_pTables[i].nOffset, pNewTables[i].nLen))
				{
					pNewTables[j].unChecksum = ComputeTableChecksum(m_sFile + m_pTables[i].nOffset, m_pTables[i].nLen);
					if (m_pTables[i].unTag == headTag)
					{
						// don't include the file checksum
						pNewTables[j].unChecksum -= GetU32BE(m_pTables[i].nOffset + 8, &bSuccess);
					}
				}
				if (pNewTables[j].unTag == cmapTag && pCodeToGID)
				{
					pNewTables[j].nLen = nNewCmapLen;
					pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewCmapTable, nNewCmapLen);
				}
				else if (pNewTables[j].unTag == cmapTag && badCmapLen)
				{
					pNewTables[j].nLen = nCmapLen;
				}
				else if (pNewTables[j].unTag == locaTag && (bUnsortedLoca || pCodeToGID))
				{
					pNewTables[j].nLen = (m_nGlyphs + 1) * (m_nLocaFormat ? 4 : 2);
					pNewTables[j].unChecksum = nLocaChecksum;
				}
				else if (pNewTables[j].unTag == glyfTag && (bUnsortedLoca || pCodeToGID))
				{
					pNewTables[j].nLen = nGlyphLen;
					pNewTables[j].unChecksum = nGlyphChecksum;
				}
				else if (pNewTables[j].unTag == nameTag && sName)
				{
					pNewTables[j].nLen = nNewNameLen;
					pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewNameTable, nNewNameLen);
				}
				else if (pNewTables[j].unTag == hheaTag && abbrevHMTX)
				{
					pNewTables[j].nLen = nNewHHEALen;
					pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewHHEATable, nNewHHEALen);
				}
				else if (pNewTables[j].unTag == hmtxTag && abbrevHMTX)
				{
					pNewTables[j].nLen = nNewHMTXLen;
					pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewHMTXTable, nNewHMTXLen);
				}
				++j;
			}
		}
		if (bMissingCmap)
		{
			pNewTables[j].unTag = cmapTag;
			if (pCodeToGID)
			{
				pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewCmapTable, nNewCmapLen);
				pNewTables[j].nLen = nNewCmapLen;
			}
			else
			{
				pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrCMapTab, sizeof(arrCMapTab));
				pNewTables[j].nLen = sizeof(arrCMapTab);
			}
			++j;
		}
		if (bMissingName)
		{
			pNewTables[j].unTag = nameTag;
			if (sName)
			{
				pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewNameTable, nNewNameLen);
				pNewTables[j].nLen = nNewNameLen;
			}
			else
			{
				pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNameTab, sizeof(arrNameTab));
				pNewTables[j].nLen = sizeof(arrNameTab);
			}
			++j;
		}
		if (bMissingPost)
		{
			pNewTables[j].unTag = postTag;
			pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrPostTab, sizeof(arrPostTab));
			pNewTables[j].nLen = sizeof(arrPostTab);
			++j;
		}
		if (bMissingOS2)
		{
			pNewTables[j].unTag = os2Tag;
			pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrOS2Tab, sizeof(arrOS2Tab));
			pNewTables[j].nLen = sizeof(arrOS2Tab);
			++j;
		}
		qsort(pNewTables, nNewTables, sizeof(TrueTypeTable), CompareTrueTypeTableTag);

		unsigned char *pUseTable = new unsigned char[nNewTables];
		if (!pUseTable)
		{
			free(arrNewHMTXTable);
			free(arrNewHHEATable);
			free(arrNewCmapTable);
			free(arrNewNameTable);
			free(pNewTables);
			free(pLocaTable);
			return;
		}
		::memset(pUseTable, 0, nNewTables * sizeof(unsigned char));
		int nNewReqTables = 0;
		for (int nIndex = 0; nIndex < nNewTables; nIndex++)
		{
			if (pNewTables[nIndex].unTag == cmapTag || pNewTables[nIndex].unTag == glyfTag ||
				pNewTables[nIndex].unTag == headTag || pNewTables[nIndex].unTag == hheaTag ||
				pNewTables[nIndex].unTag == hmtxTag || pNewTables[nIndex].unTag == locaTag ||
				pNewTables[nIndex].unTag == nameTag || pNewTables[nIndex].unTag == os2Tag ||
				pNewTables[nIndex].unTag == postTag || pNewTables[nIndex].unTag == cvtTag ||
				pNewTables[nIndex].unTag == fpgmTag || pNewTables[nIndex].unTag == maxpTag ||
				pNewTables[nIndex].unTag == prepTag)
			{
				pUseTable[nIndex] = 1;
				nNewReqTables++;
			}
		}

		nPos = 12 + nNewReqTables * 16;
		for (i = 0; i < nNewTables; ++i)
		{
			if (1 == pUseTable[i])
			{
				pNewTables[i].nOffset = nPos;
				nPos += pNewTables[i].nLen;
				if (nPos & 3)
				{
					nPos += 4 - (nPos & 3);
				}
			}
		}

		// Записываем информацию о таблицах в файле
		arrTableDir = (char *)malloc(12 + nNewReqTables * 16);
		arrTableDir[0] = 0x00; // sfnt version
		arrTableDir[1] = 0x01; //
		arrTableDir[2] = 0x00; //
		arrTableDir[3] = 0x00; //
		arrTableDir[4] = (char)((nNewReqTables >> 8) & 0xff); // numTables
		arrTableDir[5] = (char)(nNewReqTables & 0xff);
		for (i = -1, t = (unsigned int)nNewReqTables; t; ++i, t >>= 1);
		t = 1 << (4 + i);
		arrTableDir[6] = (char)((t >> 8) & 0xff);		// searchRange
		arrTableDir[7] = (char)(t & 0xff);
		arrTableDir[8] = (char)((i >> 8) & 0xff);		// entrySelector
		arrTableDir[9] = (char)(i & 0xff);
		t = nNewReqTables * 16 - t;
		arrTableDir[10] = (char)((t >> 8) & 0xff);		// rangeShift
		arrTableDir[11] = (char)(t & 0xff);
		nPos = 12;
		for (i = 0; i < nNewTables; ++i)
		{
			if (1 == pUseTable[i])
			{
				arrTableDir[nPos] = (char)(pNewTables[i].unTag >> 24);
				arrTableDir[nPos + 1] = (char)(pNewTables[i].unTag >> 16);
				arrTableDir[nPos + 2] = (char)(pNewTables[i].unTag >> 8);
				arrTableDir[nPos + 3] = (char)pNewTables[i].unTag;
				arrTableDir[nPos + 4] = (char)(pNewTables[i].unChecksum >> 24);
				arrTableDir[nPos + 5] = (char)(pNewTables[i].unChecksum >> 16);
				arrTableDir[nPos + 6] = (char)(pNewTables[i].unChecksum >> 8);
				arrTableDir[nPos + 7] = (char)pNewTables[i].unChecksum;
				arrTableDir[nPos + 8] = (char)(pNewTables[i].nOffset >> 24);
				arrTableDir[nPos + 9] = (char)(pNewTables[i].nOffset >> 16);
				arrTableDir[nPos + 10] = (char)(pNewTables[i].nOffset >> 8);
				arrTableDir[nPos + 11] = (char)pNewTables[i].nOffset;
				arrTableDir[nPos + 12] = (char)(pNewTables[i].nLen >> 24);
				arrTableDir[nPos + 13] = (char)(pNewTables[i].nLen >> 16);
				arrTableDir[nPos + 14] = (char)(pNewTables[i].nLen >> 8);
				arrTableDir[nPos + 15] = (char)pNewTables[i].nLen;
				nPos += 16;
			}
		}
		pOutputStream->Write((BYTE*)arrTableDir, 12 + nNewReqTables * 16);

		// Вычисляем чексумму файла
		nFileChecksum = ComputeTableChecksum((unsigned char *)arrTableDir, 12 + nNewReqTables * 16);
		for (i = 0; i < nNewTables; ++i)
		{
			if (1 == pUseTable[i])
			{
				nFileChecksum += pNewTables[i].unChecksum;
			}
		}
		nFileChecksum = 0xb1b0afba - nFileChecksum;

		// Записываем сами таблицы
		for (i = 0; i < nNewTables; ++i)
		{
			if (1 == pUseTable[i])
			{
				if (pNewTables[i].unTag == headTag)
				{
					if (CheckRegion(pNewTables[i].nOrigOffset, pNewTables[i].nLen))
					{
						pOutputStream->Write((BYTE*)m_sFile + pNewTables[i].nOrigOffset, 8);
						arrChecksumBuf[0] = nFileChecksum >> 24;
						arrChecksumBuf[1] = nFileChecksum >> 16;
						arrChecksumBuf[2] = nFileChecksum >> 8;
						arrChecksumBuf[3] = nFileChecksum;
						pOutputStream->Write((BYTE*)arrChecksumBuf, 4);
						pOutputStream->Write((BYTE*)((char *)m_sFile + pNewTables[i].nOrigOffset + 12), pNewTables[i].nLen - 12);
					}
					else
					{
						for (j = 0; j < pNewTables[i].nLen; ++j)
						{
							pOutputStream->Write((BYTE*)"\0", 1);
						}
					}
				}
				else if (pNewTables[i].unTag == cmapTag && pCodeToGID)
					pOutputStream->Write((BYTE*)arrNewCmapTable, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == cmapTag && bMissingCmap)
					pOutputStream->Write((BYTE*)arrCMapTab, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == nameTag && sName)
					pOutputStream->Write((BYTE*)arrNewNameTable, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == nameTag && bMissingName)
					pOutputStream->Write((BYTE*)arrNameTab, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == postTag && bMissingPost)
					pOutputStream->Write((BYTE*)arrPostTab, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == os2Tag && bMissingOS2)
					pOutputStream->Write((BYTE*)arrOS2Tab, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == hheaTag && abbrevHMTX)
					pOutputStream->Write((BYTE*)arrNewHHEATable, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == hmtxTag && abbrevHMTX)
					pOutputStream->Write((BYTE*)arrNewHMTXTable, pNewTables[i].nLen);
				else if (pNewTables[i].unTag == locaTag && (bUnsortedLoca || pCodeToGID))
				{
					for (j = 0; j <= m_nGlyphs; ++j)
					{
						if (m_nLocaFormat)
						{
							arrLocaBuf[0] = (char)(pLocaTable[j].nNewOffset >> 24);
							arrLocaBuf[1] = (char)(pLocaTable[j].nNewOffset >> 16);
							arrLocaBuf[2] = (char)(pLocaTable[j].nNewOffset >> 8);
							arrLocaBuf[3] = (char)pLocaTable[j].nNewOffset;
							pOutputStream->Write((BYTE*)arrLocaBuf, 4);
						}
						else
						{
							arrLocaBuf[0] = (char)(pLocaTable[j].nNewOffset >> 9);
							arrLocaBuf[1] = (char)(pLocaTable[j].nNewOffset >> 1);
							pOutputStream->Write((BYTE*)arrLocaBuf, 2);
						}
					}
				}
				else if (pNewTables[i].unTag == glyfTag && (bUnsortedLoca || pCodeToGID))
				{
					nPos = m_pTables[SeekTable("glyf")].nOffset;

					for (j = 0; j < m_nGlyphs; ++j)
					{
						n = pLocaTable[j].nLen;
						if (n > 0)
						{
							k = pLocaTable[j].nOrigOffset;
							if (CheckRegion(nPos + k, n))
							{
								pOutputStream->Write((BYTE*)((char *)m_sFile + nPos + k), n);
							}
							else
							{
								for (k = 0; k < n; ++k)
								{
									pOutputStream->Write((BYTE*)"\0", 1);
								}
							}
							if ((k = pLocaTable[j].nLen & 3))
							{
								pOutputStream->Write((BYTE*)"\0\0\0\0", 4 - k);
							}
						}
					}
				}
				else
				{
					if (CheckRegion(pNewTables[i].nOrigOffset, pNewTables[i].nLen))
					{
						pOutputStream->Write((BYTE*)((char *)m_sFile + pNewTables[i].nOrigOffset), pNewTables[i].nLen);
					}
					else
					{
						for (j = 0; j < pNewTables[i].nLen; ++j)
						{
							pOutputStream->Write((BYTE*)"\0", 1);
						}
					}
				}
				if (pNewTables[i].nLen & 3)
				{
					pOutputStream->Write((BYTE*)"\0\0\0", 4 - (pNewTables[i].nLen & 3));
				}
			}
		}

		delete[]pUseTable;
		free(arrNewHMTXTable);
		free(arrNewHHEATable);
		free(arrNewCmapTable);
		free(arrNewNameTable);
		free(arrTableDir);
		free(pNewTables);
		free(pLocaTable);
	}
	void         CFontFileTrueType::WriteOTF(CStream* pOutputStream, char* sName, unsigned short* pCodeToGID)
	{
		if (!m_bOpenTypeCFF || SeekTable("CFF ") < 0)
			return;

		// Open Type Font записываем так как он есть, не изменяя его
		pOutputStream->Write((BYTE*)m_sFile, m_nLen);

		return;
	}
	int          CFontFileTrueType::GetAscent()
	{
		return m_nAscent;
	}
	int          CFontFileTrueType::GetDescent()
	{
		return m_nDescent;
	}
	int          CFontFileTrueType::GetCapHeight()
	{
		return m_nCapHeight;
	}
	int*         CFontFileTrueType::GetBBox()
	{
		return m_arrBBox;
	}
	int          CFontFileTrueType::GetWeight()
	{
		return m_nWeight;
	}
	unsigned int CFontFileTrueType::ComputeTableChecksum(unsigned char *sData, int nLength)
	{
		unsigned int nWord = 0;

		unsigned int nChecksum = 0;
		for (int nIndex = 0; nIndex + 3 < nLength; nIndex += 4)
		{
			nWord = ((sData[nIndex] & 0xff) << 24) + ((sData[nIndex + 1] & 0xff) << 16) + ((sData[nIndex + 2] & 0xff) << 8) + (sData[nIndex + 3] & 0xff);
			nChecksum += nWord;
		}
		if (nLength & 3)
		{
			nWord = 0;
			int nTemp = nLength & ~3;
			switch (nLength & 3)
			{
				case 3:
				nWord |= (sData[nTemp + 2] & 0xff) << 8;
				case 2:
				nWord |= (sData[nTemp + 1] & 0xff) << 16;
				case 1:
				nWord |= (sData[nTemp] & 0xff) << 24;
				break;
			}
			nChecksum += nWord;
		}
		return nChecksum;
	}
	void         CFontFileTrueType::Parse()
	{
		int nPos = 0, nIndex = 0, nJ;

		m_bSuccess = true;

		// Проверяем является ли данный файл (TTC)
		unsigned int unTopTag = GetU32BE(0, &m_bSuccess);
		if (!m_bSuccess)
			return;

		if (unTopTag == ttcfTag)
		{
			unsigned int unVersion  = GetU32BE(4, &m_bSuccess);
			unsigned int unNumFonts = GetU32BE(8, &m_bSuccess);
			unsigned int unFontIndex = m_unFontIndex >= unNumFonts ? 0 : m_unFontIndex;
			nPos = GetU32BE(12 + 4 * unFontIndex, &m_bSuccess);
			if (!m_bSuccess)
				return;
		}
		else
			nPos = 0;

		// Проверяем sfnt версию
		int nSfntVersion = GetU32BE(nPos, &m_bSuccess);
		if (!m_bSuccess)
			return;

		// Проверяем на формат данных. CCF или нет?
		m_bOpenTypeCFF = (nSfntVersion == 0x4f54544f); // 'OTTO'

		m_nTablesCount = GetU16BE(nPos + 4, &m_bSuccess);
		if (!m_bSuccess)
			return;

		m_pTables = (TrueTypeTable *)malloc(m_nTablesCount * sizeof(TrueTypeTable));
		nPos += 12;

		for (nIndex = 0; nIndex < m_nTablesCount; ++nIndex)
		{
			m_pTables[nIndex].unTag = GetU32BE(nPos, &m_bSuccess);
			m_pTables[nIndex].unChecksum = GetU32BE(nPos + 4, &m_bSuccess);
			m_pTables[nIndex].nOffset = (int)GetU32BE(nPos + 8, &m_bSuccess);
			m_pTables[nIndex].nLen = (int)GetU32BE(nPos + 12, &m_bSuccess);
			if (m_pTables[nIndex].nOffset + m_pTables[nIndex].nLen < m_pTables[nIndex].nOffset || m_pTables[nIndex].nOffset + m_pTables[nIndex].nLen > m_nLen)
			{
				m_bSuccess = false;
			}
			nPos += 16;
		}
		if (!m_bSuccess)
			return;

		// ищем таблицы необходимые как и для TrueType так и для Type 42 
		if (SeekTable("head") < 0 || SeekTable("hhea") < 0 || SeekTable("maxp") < 0 || SeekTable("hmtx") < 0 || (!m_bOpenTypeCFF && SeekTable("loca") < 0) || (!m_bOpenTypeCFF && SeekTable("glyf") < 0) || (m_bOpenTypeCFF && SeekTable("CFF ") < 0))
		{
			m_bSuccess = false;
			return;
		}

		// читаем таблицы CMaps
		if ((nIndex = SeekTable("cmap")) >= 0)
		{
			nPos = m_pTables[nIndex].nOffset + 2;
			m_nCMapsCount = GetU16BE(nPos, &m_bSuccess);
			nPos += 2;
			if (!m_bSuccess)
				return;

			m_pCMaps = (TrueTypeCmap *)malloc(m_nCMapsCount * sizeof(TrueTypeCmap));

			for (nJ = 0; nJ < m_nCMapsCount; ++nJ)
			{
				m_pCMaps[nJ].nPlatform = GetU16BE(nPos, &m_bSuccess);
				m_pCMaps[nJ].nEncoding = GetU16BE(nPos + 2, &m_bSuccess);
				unsigned int nTemp = GetU32BE(nPos + 4, &m_bSuccess);
				m_pCMaps[nJ].nOffset = m_pTables[nIndex].nOffset + GetU32BE(nPos + 4, &m_bSuccess);
				nPos += 8;
				m_pCMaps[nJ].nFormat = GetU16BE(m_pCMaps[nJ].nOffset, &m_bSuccess);
				m_pCMaps[nJ].nLen = GetU16BE(m_pCMaps[nJ].nOffset + 2, &m_bSuccess);
			}
			if (!m_bSuccess)
				return;
		}
		else
			m_nCMapsCount = 0;

		nIndex = SeekTable("maxp");
		m_nGlyphs = GetU16BE(m_pTables[nIndex].nOffset + 4, &m_bSuccess);
		if (!m_bSuccess)
			return;

		nIndex = SeekTable("head");
		m_arrBBox[0] = GetS16BE(m_pTables[nIndex].nOffset + 36, &m_bSuccess);
		m_arrBBox[1] = GetS16BE(m_pTables[nIndex].nOffset + 38, &m_bSuccess);
		m_arrBBox[2] = GetS16BE(m_pTables[nIndex].nOffset + 40, &m_bSuccess);
		m_arrBBox[3] = GetS16BE(m_pTables[nIndex].nOffset + 42, &m_bSuccess);
		m_nLocaFormat = GetS16BE(m_pTables[nIndex].nOffset + 50, &m_bSuccess);
		if (!m_bSuccess)
			return;

		// Проверяем корректность таблицы loca
		if (!m_bOpenTypeCFF)
		{
			nIndex = SeekTable("loca");
			if (m_pTables[nIndex].nLen < 0)
			{
				m_bSuccess = false;
				return;
			}
			if (m_pTables[nIndex].nLen < (m_nGlyphs + 1) * (m_nLocaFormat ? 4 : 2))
			{
				m_nGlyphs = m_pTables[nIndex].nLen / (m_nLocaFormat ? 4 : 2) - 1;
			}
			for (nJ = 0; nJ <= m_nGlyphs; ++nJ)
			{
				if (m_nLocaFormat)
					nPos = (int)GetU32BE(m_pTables[nIndex].nOffset + nJ * 4, &m_bSuccess);
				else
					nPos = GetU16BE(m_pTables[nIndex].nOffset + nJ * 2, &m_bSuccess);

				if (nPos < 0 || nPos > m_nLen)
					m_bSuccess = false;
			}
			if (!m_bSuccess)
				return;
		}

		ReadOS2();
	}
	int          CFontFileTrueType::SeekTable(const char *sTag)
	{
		unsigned int nTagIndex = ((sTag[0] & 0xff) << 24) | ((sTag[1] & 0xff) << 16) | ((sTag[2] & 0xff) << 8) | (sTag[3] & 0xff);
		for (int nIndex = 0; nIndex < m_nTablesCount; ++nIndex)
		{
			if (m_pTables[nIndex].unTag == nTagIndex)
			{
				return nIndex;
			}
		}
		return -1;
	}
	void         CFontFileTrueType::ReadOS2()
	{
		int nIndex = SeekTable("OS/2");
		if (-1 != nIndex && m_pTables[nIndex].nLen > 0)
		{
			unsigned int unOffset = m_pTables[nIndex].nOffset;
			int nTableVersion = GetS16BE(unOffset, &m_bSuccess);
			m_nWeight	 = GetS16BE(unOffset + 4, &m_bSuccess); 
			m_nAscent	 = GetS16BE(unOffset + 68, &m_bSuccess);
			m_nDescent	 = GetS16BE(unOffset + 70, &m_bSuccess);

			//https://learn.microsoft.com/en-us/typography/opentype/spec/os2#scapheight
			if (nTableVersion >= 2)
				m_nCapHeight = GetS16BE(unOffset + 88, &m_bSuccess);
		}
	}
}
