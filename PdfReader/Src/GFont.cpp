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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "MemoryUtils.h"
#include "Object.h"
#include "Dict.h"
#include "GlobalParams.h"
#include "CMap.h"
#include "CharCodeToUnicode.h"
#include "EncodingTables.h"
#include "BuiltinFontTables.h"
#include "FontFileType1.h"
#include "FontFileType1C.h"
#include "FontFileTrueType.h"
#include "GFont.h"
#include "File.h"
#include "Stream.h"
#include "XRef.h"

namespace PdfReader
{
	struct StandardFontMapEntry
	{
		char *sAlternativeName;
		char *sProperName;
	};

	// PDF поддерживает 14 стандартных фонтов(Type1) без указаниях ширин и FontDescriptor, 
	// поэтому мы состовляем карту отображающую стандартные фонты в 14 основных фонтов PDF.
	// Эта таблица составлена на основе "implementation note 44 in the PDF 1.4 spec", с
	// небольшими добавками.
	static StandardFontMapEntry c_sStandardFontMap[] =
	{
		{ "Arial", "Helvetica" },
		{ "Arial,Bold", "Helvetica-Bold" },
		{ "Arial,BoldItalic", "Helvetica-BoldOblique" },
		{ "Arial,Italic", "Helvetica-Oblique" },
		{ "Arial-Bold", "Helvetica-Bold" },
		{ "Arial-BoldItalic", "Helvetica-BoldOblique" },
		{ "Arial-BoldItalicMT", "Helvetica-BoldOblique" },
		{ "Arial-BoldMT", "Helvetica-Bold" },
		{ "Arial-Italic", "Helvetica-Oblique" },
		{ "Arial-ItalicMT", "Helvetica-Oblique" },
		{ "ArialMT", "Helvetica" },
		{ "Courier,Bold", "Courier-Bold" },
		{ "Courier,BoldItalic", "Courier-BoldOblique" },
		{ "Courier,Italic", "Courier-Oblique" },
		{ "CourierNew", "Courier" },
		{ "CourierNew,Bold", "Courier-Bold" },
		{ "CourierNew,BoldItalic", "Courier-BoldOblique" },
		{ "CourierNew,Italic", "Courier-Oblique" },
		{ "CourierNew-Bold", "Courier-Bold" },
		{ "CourierNew-BoldItalic", "Courier-BoldOblique" },
		{ "CourierNew-Italic", "Courier-Oblique" },
		{ "CourierNewPS-BoldItalicMT", "Courier-BoldOblique" },
		{ "CourierNewPS-BoldMT", "Courier-Bold" },
		{ "CourierNewPS-ItalicMT", "Courier-Oblique" },
		{ "CourierNewPSMT", "Courier" },
		{ "Helvetica,Bold", "Helvetica-Bold" },
		{ "Helvetica,BoldItalic", "Helvetica-BoldOblique" },
		{ "Helvetica,Italic", "Helvetica-Oblique" },
		{ "Helvetica-BoldItalic", "Helvetica-BoldOblique" },
		{ "Helvetica-Italic", "Helvetica-Oblique" },
		{ "Symbol,Bold", "Symbol" },
		{ "Symbol,BoldItalic", "Symbol" },
		{ "Symbol,Italic", "Symbol" },
		{ "TimesNewRoman", "Times-Roman" },
		{ "TimesNewRoman,Bold", "Times-Bold" },
		{ "TimesNewRoman,BoldItalic", "Times-BoldItalic" },
		{ "TimesNewRoman,Italic", "Times-Italic" },
		{ "TimesNewRoman-Bold", "Times-Bold" },
		{ "TimesNewRoman-BoldItalic", "Times-BoldItalic" },
		{ "TimesNewRoman-Italic", "Times-Italic" },
		{ "TimesNewRomanPS", "Times-Roman" },
		{ "TimesNewRomanPS-Bold", "Times-Bold" },
		{ "TimesNewRomanPS-BoldItalic", "Times-BoldItalic" },
		{ "TimesNewRomanPS-BoldItalicMT", "Times-BoldItalic" },
		{ "TimesNewRomanPS-BoldMT", "Times-Bold" },
		{ "TimesNewRomanPS-Italic", "Times-Italic" },
		{ "TimesNewRomanPS-ItalicMT", "Times-Italic" },
		{ "TimesNewRomanPSMT", "Times-Roman" },
		{ "TimesNewRomanPSMT,Bold", "Times-Bold" },
		{ "TimesNewRomanPSMT,BoldItalic", "Times-BoldItalic" },
		{ "TimesNewRomanPSMT,Italic", "Times-Italic" }
	};

	//------------------------------------------------------------------------
	// GrFont
	//------------------------------------------------------------------------

	GrFont *GrFont::MakeFont(XRef *pXref, char *sTag, Ref oID, Dict *pFontDict, GlobalParams *pGlobalParams)
	{
		// Считываем название шрифта
		StringExt *seName = NULL;

		Object oTemp;
		pFontDict->Search("BaseFont", &oTemp);
		if (oTemp.IsName())
		{
			seName = new StringExt(oTemp.GetName());
		}
		oTemp.Free();

		// Тип шрифта
		GrFont *pFont = NULL;
		pFontDict->Search("Subtype", &oTemp);
		if (oTemp.IsName("Type1") || oTemp.IsName("MMType1"))
		{
			pFont = new Gr8BitFont(pXref, sTag, oID, seName, fontType1, pFontDict, pGlobalParams);
		}
		else if (oTemp.IsName("Type1C"))
		{
			pFont = new Gr8BitFont(pXref, sTag, oID, seName, fontType1C, pFontDict, pGlobalParams);
		}
		else if (oTemp.IsName("Type3"))
		{
			pFont = new Gr8BitFont(pXref, sTag, oID, seName, fontType3, pFontDict, pGlobalParams);
		}
		else if (oTemp.IsName("TrueType"))
		{
			pFont = new Gr8BitFont(pXref, sTag, oID, seName, fontTrueType, pFontDict, pGlobalParams);
		}
		else if (oTemp.IsName("Type0"))
		{
			pFont = new GrCIDFont(pXref, sTag, oID, seName, pFontDict, pGlobalParams);
		}
		else
		{
            if (!seName)
                seName = new StringExt("");
			// TO DO: Error "Unknown font type"
			pFont = new Gr8BitFont(pXref, sTag, oID, seName, fontUnknownType, pFontDict, pGlobalParams);
		}
		oTemp.Free();

		return pFont;
	}

	GrFont::GrFont(char *sTag, Ref oID, StringExt *seName, GlobalParams *pGlobalParams)
	{
		m_pGlobalParams = pGlobalParams;

		m_bValid = false;
		m_seTag  = new StringExt(sTag);
		m_oID    = oID;
		m_seName = seName;
		m_seOriginalName = seName;
		m_seEmbeddedFontName = NULL;
		m_wsExternalFontFilePath = L"";
	}

	GrFont::~GrFont()
	{
		if (m_seTag)
		{
			delete m_seTag;
		}
		if (m_seOriginalName && m_seOriginalName != m_seName)
		{
			delete m_seOriginalName;
		}
		if (m_seName)
		{
			delete m_seName;
		}
		if (m_seEmbeddedFontName)
		{
			delete m_seEmbeddedFontName;
		}
	}

	void GrFont::ReadFontDescriptor(XRef *pXref, Dict *pFontDict)
	{
		m_nFlags = fontSerif;

		m_oEmbFontFileRef.nNum = -1;
		m_oEmbFontFileRef.nGen = -1;
		m_dMissingWidth = 0;

		Object oFontDescriptor;
		if (pFontDict->Search("FontDescriptor", &oFontDescriptor)->IsDict())
		{
			// Flags
			Object oDictItem;
			if (oFontDescriptor.DictLookup("Flags", &oDictItem)->IsInt())
			{
				m_nFlags = oDictItem.GetInt();
			}
			oDictItem.Free();

			// FontName
			oFontDescriptor.DictLookup("FontName", &oDictItem);
			if (oDictItem.IsName())
			{
				m_seEmbeddedFontName = new StringExt(oDictItem.GetName());
			}
			oDictItem.Free();

			// Ищем внедренный FontFile
			if (oFontDescriptor.DictLookupAndCopy("FontFile", &oDictItem)->IsRef())
			{
				m_oEmbFontFileRef = oDictItem.GetRef();
				if (m_eType != fontType1)
				{
					// TO DO: Error "Mismatch between font type and embedded font file"
					m_eType = fontType1;
				}
			}
			oDictItem.Free();

			if (m_oEmbFontFileRef.nNum == -1 && oFontDescriptor.DictLookupAndCopy("FontFile2", &oDictItem)->IsRef())
			{
				m_oEmbFontFileRef = oDictItem.GetRef();
				if (m_eType != fontTrueType && m_eType != fontCIDType2)
				{
					// TO DO: Error "Mismatch between font type and embedded font file"
					m_eType = (m_eType == fontCIDType0 ? fontCIDType2 : fontTrueType);
				}
			}
			oDictItem.Free();

			if (m_oEmbFontFileRef.nNum == -1 && oFontDescriptor.DictLookupAndCopy("FontFile3", &oDictItem)->IsRef())
			{
				Object oStream;
				if (oDictItem.Fetch(pXref, &oStream)->IsStream())
				{
					Object oTemp;
					oStream.StreamGetDict()->Search("Subtype", &oTemp);
					if (oTemp.IsName("Type1"))
					{
						m_oEmbFontFileRef = oDictItem.GetRef();
						if (m_eType != fontType1)
						{
							// TO DO: Error "Mismatch between font type and embedded font file"
							m_eType = fontType1;
						}
					}
					else if (oTemp.IsName("Type1C"))
					{
						m_oEmbFontFileRef = oDictItem.GetRef();
						if (m_eType != fontType1 && m_eType != fontType1C)
						{
							// TO DO: Error "Mismatch between font type and embedded font file"
						}
						m_eType = fontType1C;
					}
					else if (oTemp.IsName("TrueType"))
					{
						m_oEmbFontFileRef = oDictItem.GetRef();
						if (m_eType != fontTrueType)
						{
							// TO DO: Error "Mismatch between font type and embedded font file"
							m_eType = fontTrueType;
						}
					}
					else if (oTemp.IsName("CIDFontType0C"))
					{
						m_oEmbFontFileRef = oDictItem.GetRef();
						if (m_eType != fontCIDType0)
						{
							// TO DO: Error "Mismatch between font type and embedded font file"
						}
						m_eType = fontCIDType0C;
					}
					else if (oTemp.IsName("OpenType"))
					{
						m_oEmbFontFileRef = oDictItem.GetRef();
						if (m_eType == fontTrueType)
						{
							m_eType = fontTrueTypeOT;
						}
						else if (m_eType == fontType1)
						{
							m_eType = fontType1COT;
						}
						else if (m_eType == fontCIDType0)
						{
							m_eType = fontCIDType0COT;
						}
						else if (m_eType == fontCIDType2)
						{
							m_eType = fontCIDType2OT;
						}
						else
						{
							// TO DO: Error "Mismatch between font type and embedded font file"
						}
					}
					else
					{
						// TO DO: Error "Unknown embedded font type"
					}
					oTemp.Free();
				}
				oStream.Free();
			}
			oDictItem.Free();

			// MissingWidth
			oFontDescriptor.DictLookup("MissingWidth", &oDictItem);
			if (oDictItem.IsNum())
			{
				m_dMissingWidth = oDictItem.GetNum();
			}
			oDictItem.Free();

			double dTemp = 0;
			// Ascent
			oFontDescriptor.DictLookup("Ascent", &oDictItem);
			if (oDictItem.IsNum())
			{
				dTemp = 0.001 * oDictItem.GetNum();
				//  Некоторые неправильнае шрифты устанавливают Ascent и Descent равными 0
				if (dTemp != 0)
				{
					m_dAscent = dTemp;
				}
			}
			oDictItem.Free();

			// Descent
			oFontDescriptor.DictLookup("Descent", &oDictItem);
			if (oDictItem.IsNum())
			{
				dTemp = 0.001 * oDictItem.GetNum();
				//  Некоторые неправильнае шрифты устанавливают Ascent и Descent равными 0
				if (dTemp != 0)
				{
					m_dDescent = dTemp;
				}
				// Descent должно быть отрицательным значением
				if (m_dDescent > 0)
				{
					m_dDescent = -m_dDescent;
				}
			}
			oDictItem.Free();

			// FontBBox
			if (oFontDescriptor.DictLookup("FontBBox", &oDictItem)->IsArray())
			{
				for (int nIndex = 0; nIndex < 4 && nIndex < oDictItem.ArrayGetLength(); ++nIndex)
				{
					Object oTemp;
					if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
					{
						m_arrFontBBox[nIndex] = 0.001 * oTemp.GetNum();
					}
					oTemp.Free();
				}
			}
			oDictItem.Free();

		}
		oFontDescriptor.Free();
	}

	CharCodeToUnicode *GrFont::ReadToUnicodeCMap(Dict *pFontDict, int nBitsCount, CharCodeToUnicode *pCharToUnicode)
	{
		Object oTemp;
		if (!pFontDict->Search("ToUnicode", &oTemp)->IsStream())
		{
			oTemp.Free();
			return NULL;
		}
		StringExt *seBuffer = new StringExt();
		oTemp.StreamReset();
		int nChar = 0;
		while ((nChar = oTemp.StreamGetChar()) != EOF)
		{
			seBuffer->Append(nChar);
		}
		oTemp.StreamClose();
		oTemp.Free();
		if (pCharToUnicode)
		{
			pCharToUnicode->MergeCMap(seBuffer, nBitsCount, m_pGlobalParams);
		}
		else
		{
			pCharToUnicode = CharCodeToUnicode::ParseCMap(seBuffer, nBitsCount, m_pGlobalParams);
		}
		delete seBuffer;
		return pCharToUnicode;
	}

	void GrFont::FindExternalFontFile(bool bBuiltin)
	{
		static wchar_t *c_wsType1Ext[] ={ L".pfa", L".pfb", L".ps", L"", NULL };
		static wchar_t *c_wsTTFExts[]  ={ L".ttf", NULL };

		if (m_pGlobalParams && m_seName)
		{
			if (bBuiltin)
			{
				if (!GetEmbeddedFontFileRef(&m_oEmbFontFileRef))
				{
					m_wsExternalFontFilePath = m_pGlobalParams->GetBuiltinFontPath(m_seName);
					m_eType = fontType1;
				}
			}
			else
			{
				if (m_eType == fontType1)
				{
					m_wsExternalFontFilePath = m_pGlobalParams->FindFontFile(m_seName, c_wsType1Ext);
				}
				else if (m_eType == fontTrueType)
				{
					m_wsExternalFontFilePath = m_pGlobalParams->FindFontFile(m_seName, c_wsTTFExts);
				}
			}
		}
	}

	char *GrFont::ReadExternalFontFile(int *pnLen)
	{
		FILE* pFile = NSFile::CFileBinary::OpenFileNative(m_wsExternalFontFilePath, L"rb");
		if (!pFile)
		{
			// TO DO: Error "External font file vanished"
			return NULL;
		}

		fseek(pFile, 0, SEEK_END);
		*pnLen = (int)ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		char *sBuffer = (char *)MemUtilsMalloc(*pnLen);
		if ((int)fread(sBuffer, 1, *pnLen, pFile) != *pnLen)
		{
			// TO DO: Error "Error reading external font file"
		}
		fclose(pFile);
		return sBuffer;
	}

	char *GrFont::ReadEmbeddedFontFile(XRef *pXref, int *pnLen)
	{
		Object oFontFile;
		oFontFile.InitRef(m_oEmbFontFileRef.nNum, m_oEmbFontFileRef.nGen);
		Object oStream;
		oFontFile.Fetch(pXref, &oStream);
		if (!oStream.IsStream())
		{
			// TO DO: Error "Embedded font file is not a stream"
			oStream.Free();
			oFontFile.Free();
			m_oEmbFontFileRef.nNum = -1;
			return NULL;
		}
		Stream *pStream = oStream.GetStream();
		char *sBuffer = NULL;
		int nSize = 0;
		int nIndex = 0;
		pStream->Reset();
		int nChar = 0;
		while ((nChar = pStream->GetChar()) != EOF)
		{
			if (nIndex == nSize)
			{
				nSize += 4096;
				sBuffer = (char *)MemUtilsRealloc(sBuffer, nSize);
			}
			sBuffer[nIndex++] = nChar;
		}
		*pnLen = nIndex;
		pStream->Close();

		oStream.Free();
		oFontFile.Free();

		return sBuffer;
	}

	//------------------------------------------------------------------------
	// Gr8BitFont
	//------------------------------------------------------------------------

	Gr8BitFont::Gr8BitFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, GrFontType eType, Dict *pFontDict, GlobalParams *pGlobalParams) :
		GrFont(sTag, oID, seName, pGlobalParams)
	{
		m_eType = eType;
		m_pCharToUnicode = NULL;

		if (m_seName)
		{
			StringExt *seName2 = m_seName->Copy();
			int nIndex = 0;
			while (nIndex < seName2->GetLength())
			{
				if (seName2->GetAt(nIndex) == ' ')
				{
					seName2->Delete(nIndex);
				}
				else
				{
					++nIndex;
				}
			}
			int nFirst = 0;
			int nLast = sizeof(c_sStandardFontMap) / sizeof(StandardFontMapEntry);

			// Поиск методом деления отрезка пополам
			while (nLast - nFirst > 1)
			{
				int nMiddle = (nFirst + nLast) / 2;
				if (seName2->Compare(c_sStandardFontMap[nMiddle].sAlternativeName) >= 0)
				{
					nFirst = nMiddle;
				}
				else
				{
					nLast = nMiddle;
				}
			}
			if (!seName2->Compare(c_sStandardFontMap[nFirst].sAlternativeName))
			{
				m_seName = new StringExt(c_sStandardFontMap[nFirst].sProperName);
			}
			delete seName2;
		}

		// Смотрим является ли наш фонт одним из 14 стандартных
		BuiltinFont *pBuiltinFont = NULL;
		if (m_seName)
		{
			for (int nIndex = 0; nIndex < BuiltinFontsCount; ++nIndex)
			{
				if (!m_seName->Compare(c_arrBuiltinFonts[nIndex].sName))
				{
					pBuiltinFont = &c_arrBuiltinFonts[nIndex];
					break;
				}
			}
		}

		// Выставляем стандартные значения для Ascent/Descent/BBox
		if (pBuiltinFont)
		{
			m_dAscent  = 0.001 * pBuiltinFont->nAscent;
			m_dDescent = 0.001 * pBuiltinFont->nDescent;
			m_arrFontBBox[0] = 0.001 * pBuiltinFont->arrBBox[0];
			m_arrFontBBox[1] = 0.001 * pBuiltinFont->arrBBox[1];
			m_arrFontBBox[2] = 0.001 * pBuiltinFont->arrBBox[2];
			m_arrFontBBox[3] = 0.001 * pBuiltinFont->arrBBox[3];
		}
		else
		{
			m_dAscent  = 0.95;
			m_dDescent = -0.35;
			m_arrFontBBox[0] = m_arrFontBBox[1] = m_arrFontBBox[2] = m_arrFontBBox[3] = 0;
		}

		// Считываем данные из FontDescriptor
		ReadFontDescriptor(pXref, pFontDict);

		// Для стандартных фонтов выставляем стандартные значения Ascent/Descent/BBox
		if (pBuiltinFont && m_oEmbFontFileRef.nNum < 0)
		{
			m_dAscent  = 0.001 * pBuiltinFont->nAscent;
			m_dDescent = 0.001 * pBuiltinFont->nDescent;
			m_arrFontBBox[0] = 0.001 * pBuiltinFont->arrBBox[0];
			m_arrFontBBox[1] = 0.001 * pBuiltinFont->arrBBox[1];
			m_arrFontBBox[2] = 0.001 * pBuiltinFont->arrBBox[2];
			m_arrFontBBox[3] = 0.001 * pBuiltinFont->arrBBox[3];
		}

		// Ищем внешний FontFile
		FindExternalFontFile(pBuiltinFont ? true : false);

		// FontMatrix
		m_arrFontMatrix[0] = m_arrFontMatrix[3] = 1;
		m_arrFontMatrix[1] = m_arrFontMatrix[2] = m_arrFontMatrix[4] = m_arrFontMatrix[5] = 0;

		Object oDictItem;
		if (pFontDict->Search("FontMatrix", &oDictItem)->IsArray())
		{
			for (int nIndex = 0; nIndex < 6 && nIndex < oDictItem.ArrayGetLength(); ++nIndex)
			{
				Object oTemp;
				if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
				{
					m_arrFontMatrix[nIndex] = oTemp.GetNum();
				}
				oTemp.Free();
			}
		}
		oDictItem.Free();

		// Считываем данные, характерные для шрифта Type 3
		if (m_eType == fontType3)
		{
			if (pFontDict->Search("FontBBox", &oDictItem)->IsArray())
			{
				for (int nIndex = 0; nIndex < 4 && nIndex < oDictItem.ArrayGetLength(); ++nIndex)
				{
					Object oTemp;
					if (oDictItem.ArrayGet(nIndex, &oTemp)->IsNum())
					{
						m_arrFontBBox[nIndex] = oTemp.GetNum();
					}
					oTemp.Free();
				}
			}
			oDictItem.Free();

			if (!pFontDict->Search("CharProcs", &m_oCharProcs)->IsDict())
			{
				// TO DO: Error "Missing or invalid CharProcs dictionary in Type 3 font"
				m_oCharProcs.Free();
			}

			if (!pFontDict->Search("Resources", &m_oResources)->IsDict())
			{
				m_oResources.Free();
			}
		}

		//----- Строим Encoding -----

		// Encodings начинается с BaseEncoding, которую мы можем найти в
		// (в порядке приоритета):
		//   1. pFontDict.Encoding or pFontDict.Encoding.BaseEncoding
		//        - MacRoman / MacExpert / WinAnsi / Standard
		//   2. Во внедренном или внешнем FontFile
		//   3. Стандартные значения(если до этого не нашли):
		//        - Builtin   --> Builtin encoding
		//        - TrueType  --> WinAnsiEncoding
		//        - Остальные --> StandardEncoding

		// Ищем BaseEncoding в pFontDict
		m_bHasEncoding = false;
		m_bUsesMacRomanEncoding = false;
		char **ppBaseEncoding = NULL;
		bool bBaseEncodingFromFontFile = false;

		pFontDict->Search("Encoding", &oDictItem);
		if (oDictItem.IsDict())
		{
			Object oTemp;
			oDictItem.DictLookup("BaseEncoding", &oTemp);
			if (oTemp.IsName("MacRomanEncoding"))
			{
				m_bHasEncoding = true;
				m_bUsesMacRomanEncoding = true;
				ppBaseEncoding = c_arrMacRomanEncoding;
			}
			else if (oTemp.IsName("MacExpertEncoding"))
			{
				m_bHasEncoding = true;
				ppBaseEncoding = c_arrMacExpertEncoding;
			}
			else if (oTemp.IsName("WinAnsiEncoding"))
			{
				m_bHasEncoding = true;
				ppBaseEncoding = c_arrWinAnsiEncoding;
			}
			oTemp.Free();
		}
		else if (oDictItem.IsName("MacRomanEncoding"))
		{
			m_bHasEncoding = true;
			m_bUsesMacRomanEncoding = true;
			ppBaseEncoding = c_arrMacRomanEncoding;
		}
		else if (oDictItem.IsName("MacExpertEncoding"))
		{
			m_bHasEncoding = true;
			ppBaseEncoding = c_arrMacExpertEncoding;
		}
		else if (oDictItem.IsName("WinAnsiEncoding"))
		{
			m_bHasEncoding = true;
			ppBaseEncoding = c_arrWinAnsiEncoding;
		}

		// Ищем BaseEncoding в FontFile(только для Type1)
		CFontFileType1  *pFFT1  = NULL;
		CFontFileType1C *pFFT1C = NULL;
		char *sBuffer = NULL;
		int nLen = 0;

		if (m_eType == fontType1 && (L"" != m_wsExternalFontFilePath || m_oEmbFontFileRef.nNum >= 0))
		{
			if (L"" != m_wsExternalFontFilePath)
			{
				pFFT1 = CFontFileType1::LoadFromFile((wchar_t*)m_wsExternalFontFilePath.c_str());
			}
			else
			{
				sBuffer = ReadEmbeddedFontFile(pXref, &nLen);
				pFFT1 = CFontFileType1::LoadFromBuffer(sBuffer, nLen);
			}
			if (pFFT1)
			{
				if (pFFT1->GetName())
				{
					if (m_seEmbeddedFontName)
					{
						delete m_seEmbeddedFontName;
					}
					m_seEmbeddedFontName = new StringExt(pFFT1->GetName());
				}
				if (!ppBaseEncoding)
				{
					ppBaseEncoding = pFFT1->GetEncoding();
					bBaseEncodingFromFontFile = true;
				}
			}
		}
		else if (m_eType == fontType1C && (L"" != m_wsExternalFontFilePath || m_oEmbFontFileRef.nNum >= 0))
		{
			if (L"" != m_wsExternalFontFilePath)
			{
				pFFT1C = CFontFileType1C::LoadFromFile((wchar_t*)m_wsExternalFontFilePath.c_str());
			}
			else
			{
				sBuffer = ReadEmbeddedFontFile(pXref, &nLen);
				pFFT1C = CFontFileType1C::LoadFromBuffer(sBuffer, nLen);
			}
			if (pFFT1C)
			{
				if (pFFT1C->GetName())
				{
					if (m_seEmbeddedFontName)
					{
						delete m_seEmbeddedFontName;
					}
					m_seEmbeddedFontName = new StringExt(pFFT1C->GetName());
				}
				if (!ppBaseEncoding)
				{
					ppBaseEncoding = pFFT1C->GetEncoding();
					bBaseEncodingFromFontFile = true;
				}
			}
		}
		if (sBuffer)
		{
			MemUtilsFree(sBuffer);
		}

		// Стандартные значения BaseEncoding
		if (!ppBaseEncoding)
		{
			if (pBuiltinFont && m_oEmbFontFileRef.nNum < 0)
			{
				ppBaseEncoding = pBuiltinFont->ppDefaultBaseEncoding;
				m_bHasEncoding = true;
			}
			else if (m_eType == fontTrueType)
			{
				ppBaseEncoding = c_arrWinAnsiEncoding;
			}
			else
			{
				ppBaseEncoding = c_arrStandardEncoding;
			}
		}

		// copy the base encoding
		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			m_ppEncoding[nIndex] = ppBaseEncoding[nIndex];
			if ((m_arrEncFree[nIndex] = bBaseEncodingFromFontFile) && m_ppEncoding[nIndex])
			{
				m_ppEncoding[nIndex] = CopyString(ppBaseEncoding[nIndex]);
			}
		}

		// Некоторые Type 1C Font Files имею пустые кодировки, что может навредить конвертации T1C->T1,
		// поэтому мы заполняем все пустые места из StandardEncoding
		if (m_eType == fontType1C && (L"" != m_wsExternalFontFilePath || m_oEmbFontFileRef.nNum >= 0) && bBaseEncodingFromFontFile)
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				if (!m_ppEncoding[nIndex] && c_arrStandardEncoding[nIndex])
				{
					m_ppEncoding[nIndex] = c_arrStandardEncoding[nIndex];
					m_arrEncFree[nIndex] = false;
				}
			}
		}

		// Differences
		if (oDictItem.IsDict())
		{
			Object oDifferences;
			oDictItem.DictLookup("Differences", &oDifferences);
			if (oDifferences.IsArray())
			{
				m_bHasEncoding = true;
				int nCode = 0;
				for (int nIndex = 0; nIndex < oDifferences.ArrayGetLength(); ++nIndex)
				{
					Object oTemp;
					oDifferences.ArrayGet(nIndex, &oTemp);
					if (oTemp.IsInt())
					{
						nCode = oTemp.GetInt();
					}
					else if (oTemp.IsName())
					{
						if (nCode >= 0 && nCode < 256)
						{
							if (m_arrEncFree[nCode])
							{
								MemUtilsFree(m_ppEncoding[nCode]);
							}
							m_ppEncoding[nCode] = CopyString(oTemp.GetName());
							m_arrEncFree[nCode] = true;
						}
						++nCode;
					}
					else
					{
						// TO DO: Error "Wrong type in font encoding resource differences"
					}
					oTemp.Free();
				}
			}
			oDifferences.Free();
		}
		oDictItem.Free();

		if (pFFT1)
		{
			delete pFFT1;
		}
		if (pFFT1C)
		{
			delete pFFT1C;
		}

		//----- build the mapping to Unicode -----

		// Шаг 1: Используем соответствие Name-to-Unicode
		bool bMissing = false, bHex = false;
		char *sCharName;
		Unicode arrToUnicode[256];
		for (int nCode = 0; nCode < 256; ++nCode)
		{
			if ((sCharName = m_ppEncoding[nCode]))
			{
				if (m_pGlobalParams && !(arrToUnicode[nCode] = m_pGlobalParams->MapNameToUnicode(sCharName)) && strcmp(sCharName, ".notdef"))
				{
					// Если данного символа не было в таблице Name-to-Unicode table, проверяем 
					// как выглядит Name. ( либо 'Axx', либо 'xx', где  'A' - произвольная буква
					// и 'xx' - два шестнадцатиричных числа
					if ((strlen(sCharName) == 3 && isalpha(sCharName[0]) &&
						isxdigit(sCharName[1]) && isxdigit(sCharName[2]) &&
						((sCharName[1] >= 'a' && sCharName[1] <= 'f') ||
						(sCharName[1] >= 'A' && sCharName[1] <= 'F') ||
						(sCharName[2] >= 'a' && sCharName[2] <= 'f') ||
						(sCharName[2] >= 'A' && sCharName[2] <= 'F'))
						) ||
						(strlen(sCharName) == 2 &&
						isxdigit(sCharName[0]) && isxdigit(sCharName[1]) &&
						((sCharName[0] >= 'a' && sCharName[0] <= 'f') ||
						(sCharName[0] >= 'A' && sCharName[0] <= 'F') ||
						(sCharName[1] >= 'a' && sCharName[1] <= 'f') ||
						(sCharName[1] >= 'A' && sCharName[1] <= 'F'))
						))
					{
						bHex = true;
					}
					bMissing = true;
				}
			}
			else
			{
				arrToUnicode[nCode] = 0;
			}
		}

		// Шаг 2: Заполняем пропущенные символы, ищем имена одного из видов
		// 'Axx', 'xx', 'Ann', 'ABnn' или 'nn', где 'A'  и 'B' - любые буквы,
		// 'xx' -  два шестнадцатиричных числа, и 'nn' - 2-4 десятичных числа
		if (bMissing && m_pGlobalParams && m_pGlobalParams->GetMapNumericCharNames())
		{
			for (int nCode = 0; nCode < 256; ++nCode)
			{
				if ((sCharName = m_ppEncoding[nCode]) && !arrToUnicode[nCode] && strcmp(sCharName, ".notdef"))
				{
					int nCount = strlen(sCharName);
					int nCode2 = -1;
					if (bHex && nCount == 3 && isalpha(sCharName[0]) && isxdigit(sCharName[1]) && isxdigit(sCharName[2]))
					{
						sscanf(sCharName + 1, "%x", &nCode2);
					}
					else if (bHex && nCount == 2 && isxdigit(sCharName[0]) && isxdigit(sCharName[1]))
					{
						sscanf(sCharName, "%x", &nCode2);
					}
					else if (!bHex && nCount >= 2 && nCount <= 4 && isdigit(sCharName[0]) && isdigit(sCharName[1]))
					{
						nCode2 = atoi(sCharName);
					}
					else if (nCount >= 3 && nCount <= 5 && isdigit(sCharName[1]) && isdigit(sCharName[2]))
					{
						nCode2 = atoi(sCharName + 1);
					}
					else if (nCount >= 4 && nCount <= 6 && isdigit(sCharName[2]) && isdigit(sCharName[3]))
					{
						nCode2 = atoi(sCharName + 2);
					}
					if (nCode2 >= 0 && nCode2 <= 0xff)
					{
						arrToUnicode[nCode] = (Unicode)nCode2;
					}
				}
			}
		}
		else if (bMissing && m_pGlobalParams && m_pGlobalParams->GetMapUnknownCharNames()) 	// Если установлен флаг 'mapUnknownCharNames'
		{
			for (int nCode = 0; nCode < 256; ++nCode)
			{
				if (!arrToUnicode[nCode])
				{
					arrToUnicode[nCode] = nCode;
				}
			}
		}

		m_pCharToUnicode = CharCodeToUnicode::Make8BitToUnicode(arrToUnicode);

		// ToUnicode CMap
		ReadToUnicodeCMap(pFontDict, 8, m_pCharToUnicode);

		// Ищем Unicode-to-Unicode
		CharCodeToUnicode *pUnicodeToUnicode, *pCharToUnicode;
		if (m_seName && m_pGlobalParams && (pUnicodeToUnicode = m_pGlobalParams->GetUnicodeToUnicode(m_seName)))
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				arrToUnicode[nIndex] = 0;
			}
			pCharToUnicode = CharCodeToUnicode::Make8BitToUnicode(arrToUnicode);
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				Unicode arrUnicodeBuf[8];
				int nCount = m_pCharToUnicode->MapToUnicode((CharCode)nIndex, arrUnicodeBuf, 8);
				if (nCount >= 1)
				{
					nCount = pUnicodeToUnicode->MapToUnicode((CharCode)arrUnicodeBuf[0], arrUnicodeBuf, 8);
					if (nCount >= 1)
					{
						pCharToUnicode->SetMapping((CharCode)nIndex, arrUnicodeBuf, nCount);
					}
				}
			}
			pUnicodeToUnicode->Release();
			if (m_pCharToUnicode)
				delete m_pCharToUnicode;
			m_pCharToUnicode = pCharToUnicode;
		}

		// Widths

		for (int nCode = 0; nCode < 256; ++nCode)
		{
			m_arrWidths[nCode] = m_dMissingWidth * 0.001;
		}

		// Используем шириные из pFontDict, если они там заданы
		pFontDict->Search("FirstChar", &oDictItem);
		int nFirstChar = oDictItem.IsInt() ? oDictItem.GetInt() : 0;
		oDictItem.Free();

		if (nFirstChar < 0 || nFirstChar > 255)
		{
			nFirstChar = 0;
		}
		pFontDict->Search("LastChar", &oDictItem);
		int nLastChar = oDictItem.IsInt() ? oDictItem.GetInt() : 255;
		oDictItem.Free();

		if (nLastChar < 0 || nLastChar > 255)
		{
			nLastChar = 255;
		}
		double dMult = (m_eType == fontType3) ? m_arrFontMatrix[0] : 0.001;

		pFontDict->Search("Widths", &oDictItem);
		if (oDictItem.IsArray())
		{
			m_nFlags |= fontFixedWidth;
			if (oDictItem.ArrayGetLength() < nLastChar - nFirstChar + 1)
			{
				nLastChar = nFirstChar + oDictItem.ArrayGetLength() - 1;
			}
			for (int nCode = nFirstChar; nCode <= nLastChar; ++nCode)
			{
				Object oTemp;
				oDictItem.ArrayGet(nCode - nFirstChar, &oTemp);
				if (oTemp.IsNum())
				{
					m_arrWidths[nCode] = oTemp.GetNum() * dMult;
					if (m_arrWidths[nCode] != m_arrWidths[nFirstChar])
					{
						m_nFlags &= ~fontFixedWidth;
					}
				}
				oTemp.Free();
			}
		}
		else if (pBuiltinFont)  // Используем ширины из Built-in шрифта
		{
			// Некорректные PDF-файлы кодируют символ с номером 32 как .notdef(хотя это пробел)
			unsigned short unWidth = 0;
			if (BuiltinFontGetWidth(pBuiltinFont, "space", &unWidth))
			{
				m_arrWidths[32] = 0.001 * unWidth;
			}
			for (int nCode = 0; nCode < 256; ++nCode)
			{
				if (m_ppEncoding[nCode] && BuiltinFontGetWidth(pBuiltinFont, m_ppEncoding[nCode], &unWidth))
				{
					m_arrWidths[nCode] = 0.001 * unWidth;
				}
			}
		}
		else // Ширины не заданы - заполняем стандартными значениями
		{
			int nIndex = 0;
			if (IsFixedWidth())
			{
				nIndex = 0;
			}
			else if (IsSerif())
			{
				nIndex = 8;
			}
			else
			{
				nIndex = 4;
			}
			if (IsBold())
			{
				nIndex += 2;
			}
			if (IsItalic())
			{
				nIndex += 1;
			}
			pBuiltinFont = c_arrBuiltinFontSubset[nIndex];

			// Некорректные PDF-файлы кодируют символ с номером 32 как .notdef(хотя это пробел)
			unsigned short unWidth = 0;
			if (BuiltinFontGetWidth(pBuiltinFont, "space", &unWidth))
			{
				m_arrWidths[32] = 0.001 * unWidth;
			}
			for (int nCode = 0; nCode < 256; ++nCode)
			{
				if (m_ppEncoding[nCode] && BuiltinFontGetWidth(pBuiltinFont, m_ppEncoding[nCode], &unWidth))
				{
					m_arrWidths[nCode] = 0.001 * unWidth;
				}
			}
		}
		oDictItem.Free();

		m_bValid = true;
	}

	Gr8BitFont::~Gr8BitFont()
	{
		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			if (m_arrEncFree[nIndex] && m_ppEncoding[nIndex])
			{
				MemUtilsFree(m_ppEncoding[nIndex]);
			}
		}
		m_pCharToUnicode->Release();
		if (m_oCharProcs.IsDict())
		{
			m_oCharProcs.Free();
		}
		if (m_oResources.IsDict())
		{
			m_oResources.Free();
		}
	}

	int Gr8BitFont::GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy)
	{
		CharCode nCharCode;

		*punCode = nCharCode = (CharCode)(*sText & 0xff);
		*uLen = m_pCharToUnicode->MapToUnicode(nCharCode, punUnicode, uSize);
		*pdDx = m_arrWidths[nCharCode];
		*pdDy = *pdVx = *pdVy = 0;
		return 1;
	}

	CharCodeToUnicode *Gr8BitFont::GetToUnicode()
	{
		m_pCharToUnicode->AddRef();
		return m_pCharToUnicode;
	}

	unsigned short *Gr8BitFont::GetCodeToGIDMap(CFontFileTrueType *pTTF)
	{
		unsigned short *pMap = (unsigned short *)MemUtilsMallocArray(256, sizeof(unsigned short));
		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			pMap[nIndex] = 0;
		}

		// 1. Если в PDF задана кодировка:
		//    1a. Если в PDF шрифте определна кодировка MacRomanEncoding и
		//        TrueType-шрифт имеет Macintosh Roman CMap, используем его, 
		//        и получем обатно по именам коды символов.
		//    1b. Если TrueType-шрифт имеет Microsoft Unicode CMap или 
		//        non-Microsoft Unicode CMap, используем его, и используем 
		//        юникодные номера, а не коды символов.
		//    1c. Если PDF-шрифт - Symbolic и TrueType-шрифт имеет 
		//        Microsoft Symbol CMap, используем его, и используем сами
		//        коды символов (возможно со сдвигом 0xf000).
		//    1d. Если TrueType-шрифт имеет Macintosh Roman CMap, используем 
		//        его как и в случае 1a.
		// 2. Если PDF-шрифт не имеет кодировки или PDF-шрифт - Symbolic:
		//    2a. Если TrueType-шрифт имеет Macintosh Roman CMap, используем 
		//        его, и используем сами коды символов (возможно со сдвигом
		//        0xf000).
		//    2b. Если TrueType-шрифт имеет Microsoft Symbol CMap, используем
		//        его, и используем сами коды символов (возможно со сдвигом
		//        0xf000).
		// 3. Если не один из этих случаев не применим, тогда используем первый
		//    CMap. (но такого не должно происходить)


		// TO DO: Возможна ситуация, когда одинаковых кодировок несколько, и в они содерржат
		//        разные символы. В таких случаях надо бы делать общую кодировку, а не считывать
		//        одну конкретную.

		int nUnicodeCmap = -1, nMacRomanCmap = -1, nMSSymbolCmap = -1;
		int nCmapPlatform = 0, nCmapEncoding = 0;
		for (int nIndex = 0; nIndex < pTTF->GetCmapsCount(); ++nIndex)
		{
			nCmapPlatform = pTTF->GetCmapPlatform(nIndex);
			nCmapEncoding = pTTF->GetCmapEncoding(nIndex);

			if ((nCmapPlatform == 3 && nCmapEncoding == 1) || nCmapPlatform == 0)
			{
				nUnicodeCmap = nIndex;
			}
			else if (nCmapPlatform == 1 && nCmapEncoding == 0)
			{
				nMacRomanCmap = nIndex;
			}
			else if (nCmapPlatform == 3 && nCmapEncoding == 0)
			{
				nMSSymbolCmap = nIndex;
			}
		}
		int nCmap = 0;
		bool bUseMacRoman = false;
		bool bUseUnicode = false;
		if (m_bHasEncoding)
		{
			if (m_bUsesMacRomanEncoding && nMacRomanCmap >= 0)
			{
				nCmap = nMacRomanCmap;
				bUseMacRoman = true;
			}
			else if (nUnicodeCmap >= 0)
			{
				nCmap = nUnicodeCmap;
				bUseUnicode = true;
			}
			else if ((m_nFlags & fontSymbolic) && nMSSymbolCmap >= 0)
			{
				nCmap = nMSSymbolCmap;
			}
			else if ((m_nFlags & fontSymbolic) && nMacRomanCmap >= 0)
			{
				nCmap = nMacRomanCmap;
			}
			else if (nMacRomanCmap >= 0)
			{
				nCmap = nMacRomanCmap;
				bUseMacRoman = true;
			}
		}
		else
		{
			if (nMSSymbolCmap >= 0)
			{
				nCmap = nMSSymbolCmap;
			}
			else if (nMacRomanCmap >= 0)
			{
				nCmap = nMacRomanCmap;
			}
		}

		char *sCharName;
		int nCode = 0;

		if (bUseMacRoman)
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				if ((sCharName = m_ppEncoding[nIndex]))
				{
					if (m_pGlobalParams && (nCode = m_pGlobalParams->GetMacRomanCharCode(sCharName)))
					{
						pMap[nIndex] = pTTF->MapCodeToGID(nCmap, nCode);
					}
				}
			}
		}
		else if (bUseUnicode)
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				int nLen = 0;
				Unicode nUnicode = 0;
				if (((sCharName = m_ppEncoding[nIndex]) && (m_pGlobalParams) && (nUnicode = m_pGlobalParams->MapNameToUnicode(sCharName))) || (nLen = m_pCharToUnicode->MapToUnicode((CharCode)nIndex, &nUnicode, 1)))
				{
					pMap[nIndex] = pTTF->MapCodeToGID(nCmap, nUnicode);
				}
			}
		}
		else
		{
			for (int nIndex = 0; nIndex < 256; ++nIndex)
			{
				if (!(pMap[nIndex] = pTTF->MapCodeToGID(nCmap, nIndex)))
				{
					pMap[nIndex] = pTTF->MapCodeToGID(nCmap, 0xf000 + nIndex);
				}
			}
		}

		for (int nIndex = 0; nIndex < 256; ++nIndex)
		{
			if (!pMap[nIndex] && (sCharName = m_ppEncoding[nIndex]))
			{
				pMap[nIndex] = (unsigned short)(int)pTTF->MapNameToGID(sCharName);
			}
		}

		return pMap;
	}

	Dict *Gr8BitFont::GetCharProcs()
	{
		return m_oCharProcs.IsDict() ? m_oCharProcs.GetDict() : (Dict *)NULL;
	}

	Object *Gr8BitFont::GetCharProc(int nCode, Object *pProc)
	{
		if (m_ppEncoding[nCode] && m_oCharProcs.IsDict())
		{
			m_oCharProcs.DictLookup(m_ppEncoding[nCode], pProc);
		}
		else
		{
			pProc->InitNull();
		}
		return pProc;
	}

	Dict *Gr8BitFont::GetResources()
	{
		return m_oResources.IsDict() ? m_oResources.GetDict() : (Dict *)NULL;
	}

	//------------------------------------------------------------------------
	// GrCIDFont
	//------------------------------------------------------------------------

	static int CompareWidthExceptions(const void *pWidth1, const void *pWidth2)
	{
		return ((GrFontCIDWidthException *)pWidth1)->nFirst - ((GrFontCIDWidthException *)pWidth2)->nFirst;
	}

	static int CompareWidthExceptionsV(const void *pWidth1, const void *pWidth2)
	{
		return ((GrFontCIDWidthExceptionV *)pWidth1)->nFirst - ((GrFontCIDWidthExceptionV *)pWidth2)->nFirst;
	}

	GrCIDFont::GrCIDFont(XRef *pXref, char *sTag, Ref oID, StringExt *seName, Dict *pFontDict, GlobalParams *pGlobalParams) :
		GrFont(sTag, oID, seName, pGlobalParams)
	{
		m_dAscent = 0.95;
		m_dDescent = -0.35;
		m_arrFontBBox[0] = m_arrFontBBox[1] = m_arrFontBBox[2] = m_arrFontBBox[3] = 0;
		m_pCMap = NULL;
		m_pCharToUnicode = NULL;
		m_oWidths.dDefaultWidth = 1.0;
		m_oWidths.dDefaultHeight = -1.0;
		m_oWidths.dDefaultV = 0.880;
		m_oWidths.pExceptions = NULL;
		m_oWidths.nExceptionsCount = 0;
		m_oWidths.pExceptionsV = NULL;
		m_oWidths.nExceptionsVCount = 0;
		m_pCidToGID = NULL;
		m_nCidToGIDLen = 0;

		// Descendant font
		Object oDictItem;
		if (!pFontDict->Search("DescendantFonts", &oDictItem)->IsArray())
		{
			// TO DO: Error "Missing DescendantFonts entry in Type 0 font"
			oDictItem.Free();
			return;
		}
		Object oDescendantObject;
		if (!oDictItem.ArrayGet(0, &oDescendantObject)->IsDict())
		{
			// TO DO: Error "Bad descendant font in Type 0 font"
			oDescendantObject.Free();
			oDictItem.Free();
			return;
		}
		oDictItem.Free();

		Dict *pDescendantDict = oDescendantObject.GetDict();

		// Subtype

		if (!pDescendantDict->Search("Subtype", &oDictItem))
		{
			// TO DO: Error "Missing Subtype entry in Type 0 descendant font"
			oDescendantObject.Free();
			oDictItem.Free();
			return;
		}
		if (oDictItem.IsName("CIDFontType0"))
		{
			m_eType = fontCIDType0;
		}
		else if (oDictItem.IsName("CIDFontType2"))
		{
			m_eType = fontCIDType2;
		}
		else
		{
			// TO DO: Error "Unknown Type 0 descendant font type"
			oDictItem.Free();
			oDescendantObject.Free();
			return;
		}
		oDictItem.Free();

		ReadFontDescriptor(pXref, pDescendantDict);

		// Ищем внешний FontFile
		FindExternalFontFile(false);

		// Кодировка

		// Char collection
		if (!pDescendantDict->Search("CIDSystemInfo", &oDictItem)->IsDict())
		{
			// TO DO: Error "Missing CIDSystemInfo dictionary in Type 0 descendant font"
			oDescendantObject.Free();
			oDictItem.Free();
			return;
		}
		Object oRegistry;
		oDictItem.DictLookup("Registry", &oRegistry);
		Object oOrdering;
		oDictItem.DictLookup("Ordering", &oOrdering);

		if (!oRegistry.IsString() || !oOrdering.IsString())
		{
			// TO DO: Error "Invalid CIDSystemInfo dictionary in Type 0 descendant font"
			oRegistry.Free();
			oOrdering.Free();
			oDescendantObject.Free();
			oDictItem.Free();
			return;
		}

		StringExt *seCollection = oRegistry.GetString()->Copy()->Append('-')->Append(oOrdering.GetString());
		oOrdering.Free();
		oRegistry.Free();
		oDictItem.Free();

		// ToUnicode CMap
		if (!(m_pCharToUnicode = ReadToUnicodeCMap(pFontDict, 16, NULL)))
		{

			// "Adobe-Identity" и "Adobe-UCS" collections не имеют файлов СidToUnicode
			if (seCollection->Compare("Adobe-Identity") && seCollection->Compare("Adobe-UCS"))
			{
				// Ищем файлы cidToUnicode, данные извне
				if (m_pGlobalParams && !(m_pCharToUnicode = m_pGlobalParams->GetCIDToUnicode(seCollection)))
				{
					// TO DO: Error "Unknown character collection" 
				}
			}
		}

		// Ищем Unicode-to-Unicode
		CharCodeToUnicode *pUnicodeToUnicode = NULL;
		if (m_seName && m_pGlobalParams && (pUnicodeToUnicode = m_pGlobalParams->GetUnicodeToUnicode(m_seName)))
		{
			if (m_pCharToUnicode)
			{
				for (CharCode nCharCode = 0; nCharCode < m_pCharToUnicode->GetLength(); ++nCharCode)
				{
					Unicode arrUnicodeBuffer[8];
					int nCount = m_pCharToUnicode->MapToUnicode(nCharCode, arrUnicodeBuffer, 8);
					if (nCount >= 1)
					{
						nCount = pUnicodeToUnicode->MapToUnicode((CharCode)arrUnicodeBuffer[0], arrUnicodeBuffer, 8);
						if (nCount >= 1)
						{
							m_pCharToUnicode->SetMapping(nCharCode, arrUnicodeBuffer, nCount);
						}
					}
				}
				pUnicodeToUnicode->Release();
			}
			else
			{
				m_pCharToUnicode = pUnicodeToUnicode;
			}
		}

		// Кодировка (т.е. CMap)
		pFontDict->Search("Encoding", &oDictItem);

		if (oDictItem.IsName())
		{
			StringExt  *seCMapName = new StringExt(oDictItem.GetName());

			if (m_pGlobalParams && !(m_pCMap = m_pGlobalParams->GetCMap(seCollection, seCMapName)))
			{
				// TO DO: Error "Unknown CMap for character collection"
				if (seCollection)
					delete seCollection;
				if (seCMapName)
					delete seCMapName;

				oDictItem.Free();
				oDescendantObject.Free();
				return;
			}
			if (seCMapName)
				delete seCMapName;
		}
		else if (oDictItem.IsStream())
		{
			Stream *pStream = oDictItem.GetStream();
			Dict *pEncDict = pStream->GetDict();

			Object oEncItem;

			pEncDict->Search("CMapName", &oEncItem);
			if (!oEncItem.IsName())
			{
				if (seCollection)
					delete seCollection;
				oEncItem.Free();
				oDictItem.Free();
				oDescendantObject.Free();
				return;
			}

			StringExt *seCMapName = new StringExt(oEncItem.GetName());
			oEncItem.Free();

			std::wstring wsTempFile;
			FILE *pFile = NULL;
			if (!OpenTempFile(&wsTempFile, &pFile, L"wb", L".cmap", (wchar_t*)m_pGlobalParams->GetTempFolder().c_str()))
			{
				if (seCollection)
					delete seCollection;

				if (seCMapName)
					delete seCMapName;

				oDictItem.Free();
				oDescendantObject.Free();
				return;
			}
			pStream->Reset();
			int nChar = 0;
			while (EOF != (nChar = pStream->GetChar()))
			{
				::fputc(nChar, pFile);
			}

			fclose(pFile);

			if (!(m_pCMap = pGlobalParams->GetCMap(seCollection, seCMapName, (wchar_t*)wsTempFile.c_str())))
			{
				NSFile::CFileBinary::Remove(wsTempFile.c_str());

				if (seCollection)
					delete seCollection;

				if (seCMapName)
					delete seCMapName;

				oDictItem.Free();
				oDescendantObject.Free();
				return;
			}

			NSFile::CFileBinary::Remove(wsTempFile.c_str());

			if (seCMapName)
				delete seCMapName;

		}
		else
		{
			// TO DO: Error "Missing or invalid Encoding entry in Type 0 font"
			if (seCollection)
				delete seCollection;
			oDictItem.Free();
			oDescendantObject.Free();
			return;
		}
		oDictItem.Free();

		if (seCollection)
			delete seCollection;

		// CIDToGIDMap (для внедренных шрифтов-TrueType)
		if (m_eType == fontCIDType2)
		{
			pDescendantDict->Search("CIDToGIDMap", &oDictItem);
			if (oDictItem.IsStream())
			{
				int nChar1 = 0, nChar2 = 0;
				m_nCidToGIDLen = 0;
				int nLen = 64;
				m_pCidToGID = (unsigned short *)MemUtilsMallocArray(nLen, sizeof(unsigned short));
				oDictItem.StreamReset();
				while ((nChar1 = oDictItem.StreamGetChar()) != EOF && (nChar2 = oDictItem.StreamGetChar()) != EOF)
				{
					if (m_nCidToGIDLen == nLen)
					{
						nLen *= 2;
						m_pCidToGID = (unsigned short *)MemUtilsReallocArray(m_pCidToGID, nLen, sizeof(unsigned short));
					}
					m_pCidToGID[m_nCidToGIDLen++] = (unsigned short)((nChar1 << 8) + nChar2);
				}
			}
			else if (!oDictItem.IsName("Identity") && !oDictItem.IsNull())
			{
				// TO DO: Error "Invalid CIDToGIDMap entry in CID font"
			}
			oDictItem.Free();
		}

		// Метрики

		// Стандартное значение ширин
		if (pDescendantDict->Search("DW", &oDictItem)->IsInt())
		{
			m_oWidths.dDefaultWidth = oDictItem.GetInt() * 0.001;
		}
		oDictItem.Free();

		// Исключения
		if (pDescendantDict->Search("W", &oDictItem)->IsArray())
		{
			int nExceptionsSize = 0;
			int nCounter = 0;
			while (nCounter + 1 < oDictItem.ArrayGetLength())
			{
				Object oFirst, oSecond;
				oDictItem.ArrayGet(nCounter, &oFirst);
				oDictItem.ArrayGet(nCounter + 1, &oSecond);
				if (oFirst.IsInt() && oSecond.IsInt() && nCounter + 2 < oDictItem.ArrayGetLength())
				{
					// формат: C_first C_last W 
					Object oWidth;
					if (oDictItem.ArrayGet(nCounter + 2, &oWidth)->IsNum())
					{
						if (m_oWidths.nExceptionsCount == nExceptionsSize)
						{
							nExceptionsSize += 16;
							m_oWidths.pExceptions = (GrFontCIDWidthException *)MemUtilsReallocArray(m_oWidths.pExceptions, nExceptionsSize, sizeof(GrFontCIDWidthException));
						}
						m_oWidths.pExceptions[m_oWidths.nExceptionsCount].nFirst = oFirst.GetInt();
						m_oWidths.pExceptions[m_oWidths.nExceptionsCount].nLast  = oSecond.GetInt();
						m_oWidths.pExceptions[m_oWidths.nExceptionsCount].dWidth = oWidth.GetNum() * 0.001;
						++m_oWidths.nExceptionsCount;
					}
					else
					{
						// TO DO: Error "Bad widths array in Type 0 font"
					}
					oWidth.Free();
					nCounter += 3;
				}
				else if (oFirst.IsInt() && oSecond.IsArray())
				{
					// Формат: c [ w1 w2 … wn ]
					if (m_oWidths.nExceptionsCount + oSecond.ArrayGetLength() > nExceptionsSize)
					{
						nExceptionsSize = (m_oWidths.nExceptionsCount + oSecond.ArrayGetLength() + 15) & ~15;
						m_oWidths.pExceptions = (GrFontCIDWidthException *)MemUtilsReallocArray(m_oWidths.pExceptions, nExceptionsSize, sizeof(GrFontCIDWidthException));
					}
					int nCurFirst = oFirst.GetInt();
					for (int nArrayIndex = 0; nArrayIndex < oSecond.ArrayGetLength(); ++nArrayIndex)
					{
						Object oTemp;
						if (oSecond.ArrayGet(nArrayIndex, &oTemp)->IsNum())
						{
							m_oWidths.pExceptions[m_oWidths.nExceptionsCount].nFirst = nCurFirst;
							m_oWidths.pExceptions[m_oWidths.nExceptionsCount].nLast  = nCurFirst;
							m_oWidths.pExceptions[m_oWidths.nExceptionsCount].dWidth = oTemp.GetNum() * 0.001;
							++nCurFirst;
							++m_oWidths.nExceptionsCount;
						}
						else
						{
							// TO DO: Error "Bad widths array in Type 0 font"
						}
						oTemp.Free();
					}
					nCounter += 2;
				}
				else
				{
					// TO DO: Error "Bad widths array in Type 0 font");
					++nCounter;
				}
				oSecond.Free();
				oFirst.Free();
			}
			qsort(m_oWidths.pExceptions, m_oWidths.nExceptionsCount, sizeof(GrFontCIDWidthException), &CompareWidthExceptions);
		}
		oDictItem.Free();

		// Стандартные метрики по вертикали
		if (pDescendantDict->Search("DW2", &oDictItem)->IsArray() && oDictItem.ArrayGetLength() == 2)
		{
			Object oTemp;
			if (oDictItem.ArrayGet(0, &oTemp)->IsNum())
			{
				m_oWidths.dDefaultV = oTemp.GetNum() * 0.001;
			}
			oTemp.Free();

			if (oDictItem.ArrayGet(1, &oTemp)->IsNum())
			{
				m_oWidths.dDefaultHeight = oTemp.GetNum() * 0.001;
			}
			oTemp.Free();
		}
		oDictItem.Free();

		// Исключения
		if (pDescendantDict->Search("W2", &oDictItem)->IsArray())
		{
			int nExceptionsSize = 0;
			int nCounter = 0;
			while (nCounter + 1 < oDictItem.ArrayGetLength())
			{
				Object oFirst, oSecond;
				oDictItem.ArrayGet(nCounter, &oFirst);
				oDictItem.ArrayGet(nCounter + 1, &oSecond);
				if (oFirst.IsInt() && oSecond.IsInt() && nCounter + 4 < oDictItem.ArrayGetLength())
				{
					// Формат: С_first С_last W11y V1x V1y
					Object oHeight, oVx, oVy;
					if (oDictItem.ArrayGet(nCounter + 2, &oHeight)->IsNum() && oDictItem.ArrayGet(nCounter + 3, &oVx)->IsNum() && oDictItem.ArrayGet(nCounter + 4, &oVy)->IsNum())
					{
						if (m_oWidths.nExceptionsVCount == nExceptionsSize)
						{
							nExceptionsSize += 16;
							m_oWidths.pExceptionsV = (GrFontCIDWidthExceptionV *)MemUtilsReallocArray(m_oWidths.pExceptionsV, nExceptionsSize, sizeof(GrFontCIDWidthExceptionV));
						}
						m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].nFirst  = oFirst.GetInt();
						m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].nLast   = oSecond.GetInt();
						m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dHeight = oHeight.GetNum() * 0.001;
						m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dVx     = oVx.GetNum() * 0.001;
						m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dVy     = oVy.GetNum() * 0.001;
						++m_oWidths.nExceptionsVCount;
					}
					else
					{
						// TO DO: Error "Bad widths (W2) array in Type 0 font"
					}
					oVy.Free();
					oVx.Free();
					oHeight.Free();
					nCounter += 5;
				}
				else if (oFirst.IsInt() && oSecond.IsArray())
				{
					if (m_oWidths.nExceptionsVCount + oSecond.ArrayGetLength() / 3 > nExceptionsSize)
					{
						nExceptionsSize = (m_oWidths.nExceptionsVCount + oSecond.ArrayGetLength() / 3 + 15) & ~15;
						m_oWidths.pExceptionsV = (GrFontCIDWidthExceptionV *)MemUtilsReallocArray(m_oWidths.pExceptionsV, nExceptionsSize, sizeof(GrFontCIDWidthExceptionV));
					}
					int nCurFirst = oFirst.GetInt();
					for (int nArrayIndex = 0; nArrayIndex < oSecond.ArrayGetLength(); nArrayIndex += 3)
					{
						Object oHeight, oVx, oVy;
						if (oSecond.ArrayGet(nArrayIndex, &oHeight)->IsNum() && oSecond.ArrayGet(nArrayIndex + 1, &oVx)->IsNum() && oSecond.ArrayGet(nArrayIndex + 2, &oVy)->IsNum())
						{
							// TO DO: Здесь было исправлено!!! неправильные индексы передавались -> вместо nExceptionsVCount передавались nExceptionsCount
							m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].nFirst  = nCurFirst;
							m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].nLast   = nCurFirst;
							m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dHeight = oHeight.GetNum() * 0.001;
							m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dVx     = oVx.GetNum() * 0.001;
							m_oWidths.pExceptionsV[m_oWidths.nExceptionsVCount].dVy     = oVy.GetNum() * 0.001;
							++nCurFirst;
							++m_oWidths.nExceptionsVCount;
						}
						else
						{
							// TO DO: Error "Bad widths (W2) array in Type 0 font"
						}
						oVy.Free();
						oVx.Free();
						oHeight.Free();
					}
					nCounter += 2;
				}
				else
				{
					// TO DO: Error "Bad widths (W2) array in Type 0 font"
					++nCounter;
				}
				oSecond.Free();
				oFirst.Free();
			}
			qsort(m_oWidths.pExceptionsV, m_oWidths.nExceptionsVCount, sizeof(GrFontCIDWidthExceptionV), &CompareWidthExceptionsV);
		}
		oDictItem.Free();

		oDescendantObject.Free();
		m_bValid = true;
		return;
	}

	GrCIDFont::~GrCIDFont()
	{
		if (m_pCMap)
		{
			m_pCMap->Release();
		}
		if (m_pCharToUnicode)
		{
			m_pCharToUnicode->Release();
		}

		MemUtilsFree(m_oWidths.pExceptions);
		MemUtilsFree(m_oWidths.pExceptionsV);

		if (m_pCidToGID)
		{
			MemUtilsFree(m_pCidToGID);
		}
	}

	int GrCIDFont::GetNextChar(char *sText, int nLen, CharCode *punCode, Unicode *punUnicode, int uSize, int *uLen, double *pdDx, double *pdDy, double *pdVx, double *pdVy)
	{
		CID nCID = 0;
		int nCount = 0;

		if (!m_pCMap)
		{
			*punCode = 0;
			*uLen = 0;
			*pdDx = *pdDy = 0;
			return 1;
		}

		*punCode = (CharCode)(nCID = m_pCMap->GetCID(sText, nLen, &nCount));
		// Временно

		*uLen = nCount;
		if (1 == nCount)
			*punUnicode = sText[0];
		else if (2 == nCount)
		{
			*punUnicode = 0;
			*punUnicode |= sText[0] & 0xFF;
			*punUnicode <<= 8;
			*punUnicode |= sText[1] & 0xFF;
		}
		else
		{
			*uLen = 0;
			*punUnicode = 0;
		}

		//if ( m_pCharToUnicode ) 
		//{
		//	*uLen = m_pCharToUnicode->MapToUnicode( nCID, punUnicode, uSize);
		//} 
		//else 
		//{
		//	*uLen = 0;
		//}

		double dWidth = 0, dHeight = 0, dVx = 0, dVy = 0;
		// Horizontal
		if (m_pCMap->GetWMode() == 0)
		{
			dWidth = m_oWidths.dDefaultWidth;
			dHeight = dVx = dVy = 0;
			if (m_oWidths.nExceptionsCount > 0 && nCID >= m_oWidths.pExceptions[0].nFirst)
			{
				int nFirst = 0;
				int nLast = m_oWidths.nExceptionsCount;

				while (nLast - nFirst > 1)
				{
					int nMiddle = (nFirst + nLast) / 2;
					if (m_oWidths.pExceptions[nMiddle].nFirst <= nCID)
					{
						nFirst = nMiddle;
					}
					else
					{
						nLast = nMiddle;
					}
				}
				if (nCID <= m_oWidths.pExceptions[nFirst].nLast)
				{
					dWidth = m_oWidths.pExceptions[nFirst].dWidth;
				}
			}
		}
		else // Vertical
		{
			dWidth = 0;
			dHeight = m_oWidths.dDefaultHeight;
			dVx = m_oWidths.dDefaultWidth / 2;
			dVy = m_oWidths.dDefaultV;
			if (m_oWidths.nExceptionsVCount > 0 && nCID >= m_oWidths.pExceptionsV[0].nFirst)
			{
				int nFirst = 0;
				int nLast = m_oWidths.nExceptionsVCount;

				while (nLast - nFirst > 1)
				{
					int nMiddle = (nFirst + nLast) / 2;
					if (m_oWidths.pExceptionsV[nMiddle].nLast <= nCID)
					{
						nFirst = nMiddle;
					}
					else
					{
						nLast = nMiddle;
					}
				}
				if (nCID <= m_oWidths.pExceptionsV[nFirst].nLast)
				{
					dHeight = m_oWidths.pExceptionsV[nFirst].dHeight;
					dVx = m_oWidths.pExceptionsV[nFirst].dVx;
					dVy = m_oWidths.pExceptionsV[nFirst].dVy;
				}
			}
		}

		*pdDx = dWidth;
		*pdDy = dHeight;
		*pdVx = dVx;
		*pdVy = dVy;

		return nCount;
	}

	int GrCIDFont::GetWMode()
	{
		return m_pCMap ? m_pCMap->GetWMode() : 0;
	}

	CharCodeToUnicode *GrCIDFont::GetToUnicode()
	{
		if (m_pCharToUnicode)
		{
			m_pCharToUnicode->AddRef();
		}
		return m_pCharToUnicode;
	}

	StringExt *GrCIDFont::GetCollection()
	{
		return m_pCMap ? m_pCMap->GetCollection() : (StringExt *)NULL;
	}

	//------------------------------------------------------------------------
	// GrFontDict
	//------------------------------------------------------------------------

	GrFontDict::GrFontDict(XRef *pXref, Ref *pFontDictRef, Dict *pFontDict, GlobalParams *pGlobalParams)
	{
		m_pGlobalParams = pGlobalParams;

		Ref oRef;

		m_nFontsCount = pFontDict->GetEntryCount();
		m_ppFonts = (GrFont **)MemUtilsMallocArray(m_nFontsCount, sizeof(GrFont *));
		for (int nIndex = 0; nIndex < m_nFontsCount; ++nIndex)
		{
			Object oFontRef, oFont;
			pFontDict->GetValueCopy(nIndex, &oFontRef);
			oFontRef.Fetch(pXref, &oFont);
			if (oFont.IsDict())
			{
				if (oFontRef.IsRef())
				{
					oRef = oFontRef.GetRef();
				}
				else
				{
					oRef.nNum = nIndex;
					if (pXref)
					{
						oRef.nGen = pXref->GenerateUniqueRefGen();
					}
					else
					{
						oRef.nGen = 999999;
					}
				}
				m_ppFonts[nIndex] = GrFont::MakeFont(pXref, pFontDict->GetKey(nIndex), oRef, oFont.GetDict(), pGlobalParams);
				if (m_ppFonts[nIndex] && !m_ppFonts[nIndex]->CheckValidate())
				{
					if (m_ppFonts[nIndex])
						delete m_ppFonts[nIndex];
					m_ppFonts[nIndex] = NULL;
				}
			}
			else
			{
				// TO DO: Error "font resource is not a dictionary"
				m_ppFonts[nIndex] = NULL;
			}
			oFontRef.Free();
			oFont.Free();
		}
	}

	GrFontDict::~GrFontDict()
	{
		for (int nIndex = 0; nIndex < m_nFontsCount; ++nIndex)
		{
			if (m_ppFonts[nIndex])
			{
				delete m_ppFonts[nIndex];
			}
		}
		MemUtilsFree(m_ppFonts);
	}

	GrFont *GrFontDict::Search(char *sTag)
	{
		for (int nIndex = 0; nIndex < m_nFontsCount; ++nIndex)
		{
			if (m_ppFonts[nIndex] && m_ppFonts[nIndex]->CheckTag(sTag))
			{
				return m_ppFonts[nIndex];
			}
		}
		return NULL;
	}
}
