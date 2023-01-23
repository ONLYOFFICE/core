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
#include "CSVReader.h"

#include "../../../XlsbFormat/Xlsb.h"
#include "../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../DesktopEditor/common/File.h"

#include <map>
#include <locale>

#include "../../../../DesktopEditor/common/File.h"
#include "../../../Base/unicode_util.h"
#include "../../../../Common/OfficeFileErrorDescription.h"
#include "../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../UnicodeConverter/UnicodeConverter_Encodings.h"
#include "../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/Styles/Styles.h"

class CSVReader::Impl
{
public:
	Impl() {}
	_UINT32 Read(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, _UINT32 nCodePage, const std::wstring& wcDelimiter);
private:
	void AddCell(std::wstring &sText, INT nStartCell, std::stack<INT> &oDeleteChars, OOX::Spreadsheet::CRow &oRow, INT nRow, INT nCol, bool bIsWrap);

	std::map<std::wstring, unsigned int> mapDataNumber;
	OOX::Spreadsheet::CStyles *m_pStyles = NULL;
//---------------------------------------------------------------------------------------------------------
	const std::wstring ansi_2_unicode(const unsigned char* data, DWORD data_size)
	{
		std::wstring result;

		std::locale loc("");
		std::ctype<wchar_t> const &facet = std::use_facet<std::ctype<wchar_t> >(loc);

		result.resize(data_size);
	    
		facet.widen((char*)data, (char*)data + data_size, &result[0]);
		return result;
	}
	void utf8_2_unicode(const unsigned char* data, DWORD data_size, std::wstring &wStr)
	{
		wStr.resize(data_size + 1);
		unsigned int nLength = data_size;
		
		ConversionResult eUnicodeConversionResult = conversionOK;
		if (sizeof(wchar_t) == 2)//utf8 -> utf16
		{
			memset ((void *)wStr.data(), 0, sizeof (UTF16) * (nLength + 1));

			UTF8 *pStrUtf8 = (UTF8 *) data;

			const UTF8 *pStrUtf8_Conv = pStrUtf8;
			UTF16 *pStrUtf16_Conv = (UTF16 *)wStr.data();

			eUnicodeConversionResult = ConvertUTF8toUTF16 (&pStrUtf8_Conv,	 &pStrUtf8[nLength]
					, &pStrUtf16_Conv, &((UTF16 *)wStr.data())[nLength]
					, strictConversion);
		}
		else //utf8 -> utf32
		{
			//UTF32 *pStrUtf32 = new UTF32 [nLength + 1];
			//memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength + 1));
			memset((void *)wStr.data(), 0, sizeof(UTF16) * (nLength + 1));

			UTF8 *pStrUtf8 = (UTF8 *) data;

			const UTF8 *pStrUtf8_Conv = pStrUtf8;
			UTF32 *pStrUtf32_Conv = (UTF32 *)wStr.data();

			eUnicodeConversionResult = ConvertUTF8toUTF32 (&pStrUtf8_Conv, &pStrUtf8[nLength]
					, &pStrUtf32_Conv, &((UTF32 *)wStr.data())[nLength]
					, strictConversion);
		}
		if (conversionOK != eUnicodeConversionResult)
		{
			wStr.clear();	
			std::string inp((char*)data, data_size);
			wStr = std::wstring(inp.begin(), inp.end());
		}
	}
	const std::wstring utf16_2_unicode(const unsigned char* data, DWORD data_size)
	{
		if (sizeof(wchar_t) == 2)//utf16 -> utf16
		{
			return std::wstring((wchar_t*)data, data_size / 2);
		}
		else //utf16 -> utf32
		{
			unsigned int nLength = data_size / 2;

			UTF32 *pStrUtf32 = new UTF32 [nLength + 1];
			memset ((void *) pStrUtf32, 0, sizeof (UTF32) * (nLength + 1));

			UTF16 *pStrUtf16 = (UTF16 *) data;

			const UTF16 *pStrUtf16_Conv = pStrUtf16;
			UTF32 *pStrUtf32_Conv = pStrUtf32;

			ConversionResult eUnicodeConversionResult = ConvertUTF16toUTF32 (&pStrUtf16_Conv, &pStrUtf16[nLength]
					, &pStrUtf32_Conv, &pStrUtf32 [nLength]
					, strictConversion);

			if (conversionOK != eUnicodeConversionResult)
			{
				delete [] pStrUtf32;
				return std::wstring();
			}
			std::wstring utf32Str ((wchar_t *) pStrUtf32);

			delete [] pStrUtf32;
			return utf32Str;
		}
	}
	const std::wstring utf32_2_unicode(const unsigned char* data, DWORD data_size)
	{
		if (sizeof(wchar_t) == 4)//utf32 -> utf32
		{
			return std::wstring((wchar_t*)data, data_size / 4);
		}
		else //utf32 -> utf16
		{
			unsigned int nLength = data_size / 4;

			UTF16 *pStrUtf16 = new UTF16 [nLength + 1];
			memset ((void *) pStrUtf16, 0, sizeof (UTF16) * (nLength + 1));

			UTF32 *pStrUtf32 = (UTF32 *) data;

			const UTF32 *pStrUtf32_Conv = pStrUtf32;
			UTF16 *pStrUtf16_Conv = pStrUtf16;

			ConversionResult eUnicodeConversionResult = ConvertUTF32toUTF16 (&pStrUtf32_Conv, &pStrUtf32[nLength]
					, &pStrUtf16_Conv, &pStrUtf16 [nLength]
					, strictConversion);

			if (conversionOK != eUnicodeConversionResult)
			{
				delete [] pStrUtf16;
				return std::wstring();
			}
			std::wstring utf16Str ((wchar_t *) pStrUtf16);

			delete [] pStrUtf16;
			return utf16Str;
		}
	}
};
//-----------------------------------------------------------------------------------------------	
void CSVReader::Impl::AddCell(std::wstring &sText, INT nStartCell, std::stack<INT> &oDeleteChars, OOX::Spreadsheet::CRow &oRow, INT nRow, INT nCol, bool bIsWrap)
{
	while (!oDeleteChars.empty())
	{
		INT nIndex = oDeleteChars.top() - nStartCell;
		sText.erase(nIndex, 1);
		oDeleteChars.pop();
	}
	size_t length = sText.length();
	
// Пустую не пишем
	if ((0 == length) || (sText[0] == L'\0'))
		return;

	OOX::Spreadsheet::CCell *pCell = new OOX::Spreadsheet::CCell();
	pCell->m_oType.Init();

	pCell->m_oCacheValue = sText; // как есть 

	wchar_t *pEndPtr;
	double dValue = wcstod(sText.c_str(), &pEndPtr);

	if (std::isnan(dValue) || std::isinf(dValue))
		pEndPtr = (wchar_t *)sText.c_str();
	
	if ((0 == *pEndPtr) || (pEndPtr != sText.c_str() && (sText.c_str() + length  - pEndPtr) < 3))
	{
		std::wstring data_format;
		std::wstring postfix;

		if (0 != *pEndPtr)
		{
			size_t sz = length - (pEndPtr - sText.c_str());

			while (sz > 0)
			{
				if (pEndPtr[sz - 1] != L' ')
					break;
				sz--;
			}

			if (sz > 0)
			{
				postfix = std::wstring(pEndPtr, sz);
			}
		}

		size_t pos = sText.find(L".");
		if (pos != std::wstring::npos)
		{
			size_t fraction = sText.length() - pos - ((0 != *pEndPtr) ? 2 : 1);
			for (size_t i = 0; i < fraction && fraction != std::wstring::npos; ++i)
				data_format += L"0";
		}
		if (false == data_format.empty()) data_format = L"." + data_format;

		pCell->m_oValue.Init();
		
		if (0 != *pEndPtr)
		{
			if (false == postfix.empty())
			{
				if (postfix[0] == L'%')
				{
					pCell->m_oValue->m_sText = std::to_wstring(dValue / 100.);
				}
				else
				{
					pCell->m_oValue->m_sText = sText.substr(0, length - 1);

					for (size_t i = 0; i < postfix.size(); ++i)
					{
						data_format += std::wstring(L"\\") + postfix[i];
					}
				}
			}	
		}
		else
			pCell->m_oValue->m_sText = sText;

		if (false == data_format.empty())
		{
			data_format = L"0" + data_format;

			std::map<std::wstring, unsigned int>::iterator pFind = mapDataNumber.find(data_format);
			if (pFind != mapDataNumber.end())
			{
				pCell->m_oStyle = pFind->second;
			}
			else
			{
				if (!m_pStyles->m_oNumFmts.IsInit()) m_pStyles->m_oNumFmts.Init();

				m_pStyles->m_oNumFmts->m_arrItems.push_back(new OOX::Spreadsheet::CNumFmt());
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oFormatCode = data_format;
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId.Init();
				m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->SetValue(164 + m_pStyles->m_oNumFmts->m_arrItems.size());

				// Normal + data format
				OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();

				pXfs->m_oBorderId.Init();	pXfs->m_oBorderId->SetValue(0);
				pXfs->m_oFillId.Init();		pXfs->m_oFillId->SetValue(0);
				pXfs->m_oFontId.Init();		pXfs->m_oFontId->SetValue(0);
				pXfs->m_oNumFmtId.Init();	pXfs->m_oNumFmtId->SetValue(m_pStyles->m_oNumFmts->m_arrItems.back()->m_oNumFmtId->GetValue());

				m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

				pCell->m_oStyle = (unsigned int)(m_pStyles->m_oCellXfs->m_arrItems.size() - 1);
				mapDataNumber.insert(std::make_pair(data_format, *pCell->m_oStyle));
			}
		}
	}
	else
	{
		pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
		pCell->m_oRichText.Init();
		OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
		pText->m_sText = sText;
		pCell->m_oRichText->m_arrItems.push_back(pText);
	}

	if (bIsWrap)
	{
		pCell->m_oStyle = 1;
	}

	pCell->setRowCol(nRow, nCol);
	oRow.m_arrItems.push_back(pCell);
}
_UINT32 CSVReader::Impl::Read(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, _UINT32 nCodePage, const std::wstring& sDelimiter)
{
	NSFile::CFileBinary oFile;
	if (false == oFile.OpenFile(sFileName)) return AVS_FILEUTILS_ERROR_CONVERT;
	//-----------------------------------------------------------------------------------
	// Создадим Workbook
	oXlsx.CreateWorkbook();
	// Создадим стили
	oXlsx.CreateStyles();

	// Добавим стили для wrap-а
	oXlsx.m_pStyles->m_oCellXfs.Init();
	oXlsx.m_pStyles->m_oCellXfs->m_oCount.Init();
	oXlsx.m_pStyles->m_oCellXfs->m_oCount->SetValue(2);

	m_pStyles = oXlsx.m_pStyles;

// Normall default
	OOX::Spreadsheet::CXfs* pXfs = NULL;
	pXfs = new OOX::Spreadsheet::CXfs();
	pXfs->m_oBorderId.Init();		pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();			pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();			pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();		pXfs->m_oNumFmtId->SetValue(0);

	oXlsx.m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

// Wrap style
	pXfs = new OOX::Spreadsheet::CXfs();
	pXfs->m_oBorderId.Init();		pXfs->m_oBorderId->SetValue(0);
	pXfs->m_oFillId.Init();			pXfs->m_oFillId->SetValue(0);
	pXfs->m_oFontId.Init();			pXfs->m_oFontId->SetValue(0);
	pXfs->m_oNumFmtId.Init();		pXfs->m_oNumFmtId->SetValue(0);

	pXfs->m_oApplyAlignment.Init();	pXfs->m_oApplyAlignment->SetValue(SimpleTypes::onoffTrue);
	pXfs->m_oAligment.Init();		pXfs->m_oAligment->m_oWrapText.Init();
	pXfs->m_oAligment->m_oWrapText->SetValue(SimpleTypes::onoffTrue);

	oXlsx.m_pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

	smart_ptr<OOX::Spreadsheet::CWorksheet> pWorksheet(new OOX::Spreadsheet::CWorksheet(NULL));
	pWorksheet->m_oSheetData.Init();

	//-----------------------------------------------------------------------------------
	DWORD nFileSize = 0;
	BYTE* pFileData = new BYTE[oFile.GetFileSize() + 64];

	oFile.ReadFile(pFileData, oFile.GetFileSize(), nFileSize);
	oFile.CloseFile();
	//skip bom
	DWORD nInputBufferSize = nFileSize;
	BYTE* pInputBuffer = pFileData;
	if (nInputBufferSize >= 3 && 0xef == pInputBuffer[0] && 0xbb == pInputBuffer[1] && 0xbf == pInputBuffer[2])
	{
		nInputBufferSize -= 3;
		pInputBuffer += 3;
	}
	else if (nInputBufferSize >= 2 && ((0xfe == pInputBuffer[0] && 0xff == pInputBuffer[1]) || (0xff == pInputBuffer[0] && 0xfe == pInputBuffer[1])))
	{
		nInputBufferSize -= 2;
		pInputBuffer += 2;
	}

	std::wstring sFileDataW;

	if (nCodePage == 1000)
	{
		sFileDataW = ansi_2_unicode(pInputBuffer, nInputBufferSize);
	}
	else if (nCodePage == 46)//utf-8
	{
		utf8_2_unicode(pInputBuffer, nInputBufferSize, sFileDataW);
	}
	else if (nCodePage == 48)//utf-16
	{
		sFileDataW = utf16_2_unicode(pInputBuffer, nInputBufferSize);
	}
	else if (nCodePage == 50) // utf-32
	{
		sFileDataW = utf32_2_unicode(pInputBuffer, nInputBufferSize);
	}
	else
	{
		const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodePage];

		NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
		sFileDataW = oUnicodeConverter.toUnicode((const char*)pInputBuffer, nInputBufferSize, oEncodindId.Name);
	}
	//------------------------------------------------------------------------------------------------------------------------------     
	delete[]pFileData;

	size_t nSize = sFileDataW.length();

	if (nSize < 1 && nInputBufferSize > 0)
	{//для синхронности вывода превью и нормального результата
		const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodePage];

		NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
		sFileDataW = oUnicodeConverter.toUnicode((const char*)pInputBuffer, nInputBufferSize, oEncodindId.Name);

		nSize = sFileDataW.length();
		//return AVS_FILEUTILS_ERROR_CONVERT_ICU;
	}

	WCHAR wcDelimiterLeading = L'\0';
	WCHAR wcDelimiterTrailing = L'\0';
	int nDelimiterSize = 0;

	if (sDelimiter.length() > 0)
	{
		wcDelimiterLeading = sDelimiter[0];
		nDelimiterSize = 1;
		if (2 == sizeof(wchar_t) && 0xD800 <= wcDelimiterLeading && wcDelimiterLeading <= 0xDBFF && sDelimiter.length() > 1)
		{
			wcDelimiterTrailing = sDelimiter[1];
			nDelimiterSize = 2;
		}
	}

	const WCHAR wcNewLineN = _T('\n');
	const WCHAR wcNewLineR = _T('\r');
	const WCHAR wcQuote = _T('"');
	const WCHAR wcTab = _T('\t');

	bool bIsWrap = false;
	WCHAR wcCurrent;
	INT nStartCell = 0;
	std::stack<INT> oDeleteChars;

	bool bMsLimit = false;
	bool bInQuote = false;
	INT nIndexRow = 0;
	INT nIndexCol = 0;
	OOX::Spreadsheet::CRow *pRow = new OOX::Spreadsheet::CRow();
	pRow->m_oR.Init();
	pRow->m_oR->SetValue(nIndexRow + 1);

	const WCHAR *pTemp = sFileDataW.c_str();
	for (size_t nIndex = 0; nIndex < nSize; ++nIndex)
	{
		wcCurrent = pTemp[nIndex];
		if (wcDelimiterLeading == wcCurrent && (L'\0' == wcDelimiterTrailing || (nIndex + 1 < nSize && wcDelimiterTrailing == pTemp[nIndex + 1])))
		{
			if (bInQuote)
				continue;
			// New Cell
			std::wstring sCellText(pTemp + nStartCell, nIndex - nStartCell);

			AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
			oDeleteChars = std::stack<INT>();
			bIsWrap = false;

			if (nIndex + nDelimiterSize == nSize)
			{
				pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
				pRow = NULL;
			}

			if (nIndex + nDelimiterSize > 500000)
			{
				nStartCell = 0;
				sFileDataW.erase(0, nIndex + nDelimiterSize);
				nSize -= (nIndex + nDelimiterSize); nIndex = 0; 
				pTemp = sFileDataW.c_str();
			}
			else
				nStartCell = nIndex + nDelimiterSize;
		}
		else if (wcNewLineN == wcCurrent || wcNewLineR == wcCurrent)
		{
			if (bInQuote)
			{
				// Добавим Wrap
				bIsWrap = true;
				continue;
			}
			// New line
			if (nStartCell != nIndex)
			{
				std::wstring sCellText(pTemp + nStartCell, nIndex - nStartCell);
				AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
				bIsWrap = false;
			}

			if (wcNewLineR == wcCurrent && nIndex + 1 != nSize && wcNewLineN == pTemp[nIndex + 1])
			{
				// На комбинацию \r\n должен быть только 1 перенос
				++nIndex;
			}

			if (nIndex + 1 > 500000)
			{
				nStartCell = 0;
				sFileDataW.erase(0, nIndex + 1);
				nSize -= (nIndex + 1); nIndex = 0; 
				pTemp = sFileDataW.c_str();
			}
			else
				nStartCell = nIndex + 1;

			pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);

			pRow = new OOX::Spreadsheet::CRow();
			pRow->m_oR.Init();
			pRow->m_oR->SetValue(++nIndexRow + 1);
			nIndexCol = 0;

			if (pWorksheet->m_oSheetData->m_arrItems.size() > 1048576)
			{
				bMsLimit = true;
				break; // ограниечние мс
			}
		}
		else if (wcQuote == wcCurrent)
		{
			// Quote
			if (false == bInQuote && nStartCell == nIndex && nIndex + 1 != nSize)
			{
				// Начало новой ячейки (только если мы сразу после разделителя и не в конце файла)
				bInQuote = !bInQuote;
				nStartCell = nIndex + 1;
			}
			else if (bInQuote)
			{
				// Нужно удалить кавычку ограничитель
				oDeleteChars.push(nIndex);

				// Если следующий символ кавычка, то мы не закончили ограничитель строки (1997,Ford,E350,"Super, ""luxurious"" truck")
				if (nIndex + 1 != nSize && wcQuote == pTemp[nIndex + 1])
					++nIndex;
				else
					bInQuote = !bInQuote;
			}
		}
		else if (wcTab == wcCurrent)
		{
			// delete tab if not delimiter
			oDeleteChars.push(nIndex);
		}
	}

	if (nStartCell != nSize && !bMsLimit)
	{
		// New line
		std::wstring sCellText(pTemp + nStartCell, nSize - nStartCell);
		AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
		pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
	}
	else
	{
		RELEASEOBJECT(pRow);
	}
	oXlsx.m_arWorksheets.push_back(pWorksheet.GetPointer());

	smart_ptr<OOX::File> oWorksheetFile = pWorksheet.smart_dynamic_cast<OOX::File>();
	const OOX::RId oRid = oXlsx.m_pWorkbook->Add(oWorksheetFile);

	oXlsx.m_mapWorksheets.insert(std::make_pair(oRid.ToString(), pWorksheet.GetPointer())); // for bin

	OOX::Spreadsheet::CSheet *pSheet = new OOX::Spreadsheet::CSheet();

	pSheet->m_oName = L"Sheet1";
	pSheet->m_oSheetId.Init();
	pSheet->m_oSheetId->SetValue(1);
	pSheet->m_oRid.Init();
	pSheet->m_oRid->SetValue(oRid.ToString());

	oXlsx.m_pWorkbook->m_oSheets.Init();
	oXlsx.m_pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);

	return bMsLimit ? AVS_FILEUTILS_ERROR_CONVERT_ROWLIMITS : 0;
}
//----------------------------------------------------------------------------------
CSVReader::CSVReader() : impl_(new CSVReader::Impl())
{
}
CSVReader::~CSVReader()
{
}
_UINT32 CSVReader::Read(const std::wstring &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, _UINT32 nCodePage, const std::wstring& sDelimiter)
{
	return impl_->Read(sFileName, oXlsx, nCodePage, sDelimiter);
}
