/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <map>
#include "../../DesktopEditor/common/File.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
#include "../../UnicodeConverter/UnicodeConverter_Encodings.h"

namespace CSVReader
{
    void AddCell(CString &sText, INT nStartCell, std::stack<INT> &oDeleteChars, OOX::Spreadsheet::CRow &oRow, INT nRow, INT nCol, bool bIsWrap)
	{
		while(!oDeleteChars.empty())
		{
			INT nIndex = oDeleteChars.top() - nStartCell;
			sText.Delete(nIndex);
			oDeleteChars.pop();
		}
		// Пустую не пишем
		if (0 == sText.GetLength())
			return;

		OOX::Spreadsheet::CCell *pCell = new OOX::Spreadsheet::CCell();
		pCell->m_oType.Init();

		WCHAR *pEndPtr;
		double dValue = wcstod(sText, &pEndPtr);
		if (NULL != *pEndPtr)
		{
			// Не число
			pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeInlineStr);
			pCell->m_oRichText.Init();
			OOX::Spreadsheet::CText *pText = new OOX::Spreadsheet::CText();
			pText->m_sText = sText;
			pCell->m_oRichText->m_arrItems.push_back(pText);
		}
		else
		{
			// Число
			pCell->m_oValue.Init();
			pCell->m_oValue->m_sText = sText;
		}

		if (bIsWrap)
		{
			// WrapStyle
			pCell->m_oStyle.Init();
			pCell->m_oStyle->SetValue(1);
		}

		pCell->setRowCol(nRow, nCol);
		oRow.m_arrItems.push_back(pCell);
	}
    void ReadFromCsvToXlsx(const CString &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter)
	{
		// Создадим Workbook
		oXlsx.CreateWorkbook();
		// Создадим стили
		oXlsx.CreateStyles();

		// Добавим стили для wrap-а
		OOX::Spreadsheet::CStyles *pStyles = oXlsx.GetStyles();
		pStyles->m_oCellXfs.Init();
		pStyles->m_oCellXfs->m_oCount.Init();
		pStyles->m_oCellXfs->m_oCount->SetValue(2);

		// Normall default
		OOX::Spreadsheet::CXfs* pXfs = NULL;
		pXfs = new OOX::Spreadsheet::CXfs();
		pXfs->m_oBorderId.Init();
		pXfs->m_oBorderId->SetValue(0);
		pXfs->m_oFillId.Init();
		pXfs->m_oFillId->SetValue(0);
		pXfs->m_oFontId.Init();
		pXfs->m_oFontId->SetValue(0);
		pXfs->m_oNumFmtId.Init();
		pXfs->m_oNumFmtId->SetValue(0);
		pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

		// Wrap style
		pXfs = new OOX::Spreadsheet::CXfs();
		pXfs->m_oBorderId.Init();
		pXfs->m_oBorderId->SetValue(0);
		pXfs->m_oFillId.Init();
		pXfs->m_oFillId->SetValue(0);
		pXfs->m_oFontId.Init();
		pXfs->m_oFontId->SetValue(0);
		pXfs->m_oNumFmtId.Init();
		pXfs->m_oNumFmtId->SetValue(0);

		pXfs->m_oApplyAlignment.Init();
		pXfs->m_oApplyAlignment->SetValue(SimpleTypes::onoffTrue);
		pXfs->m_oAligment.Init();
		pXfs->m_oAligment->m_oWrapText.Init();
		pXfs->m_oAligment->m_oWrapText->SetValue(SimpleTypes::onoffTrue);
		pStyles->m_oCellXfs->m_arrItems.push_back(pXfs);

		std::wstring sSheetRId = L"rId1";
		OOX::Spreadsheet::CWorksheet* pWorksheet = new OOX::Spreadsheet::CWorksheet();
		pWorksheet->m_oSheetData.Init();
		
		OOX::Spreadsheet::CSheet *pSheet = new OOX::Spreadsheet::CSheet();
		
		pSheet->m_oName.Init();
		pSheet->m_oName->append(L"Sheet1");
		pSheet->m_oSheetId.Init();
		pSheet->m_oSheetId->SetValue(1);
		pSheet->m_oRid.Init();
		pSheet->m_oRid->SetValue(sSheetRId);

		OOX::Spreadsheet::CWorkbook *pWorkbook = oXlsx.GetWorkbook();
		pWorkbook->m_oSheets.Init();
		pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);

		NSFile::CFileBinary oFile;
		if(oFile.OpenFile(sFileName))
		{
			DWORD nFileSize = 0;
			BYTE* pFileData = new BYTE[oFile.GetFileSize()];
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

            const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodePage];
            NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
            std::wstring sFileDataW = oUnicodeConverter.toUnicode((const char*)pInputBuffer, nInputBufferSize, oEncodindId.Name);
            INT nSize = sFileDataW.length();
            const WCHAR *pTemp =sFileDataW.c_str();

            const WCHAR wcNewLineN = _T('\n');
            const WCHAR wcNewLineR = _T('\r');
            const WCHAR wcQuote = _T('"');
            const WCHAR wcTab = _T('\t');

            bool bIsWrap = false;
			WCHAR wcCurrent;
			INT nStartCell = 0;
			std::stack<INT> oDeleteChars;

            bool bInQuote = false;
			INT nIndexRow = 0;
			INT nIndexCol = 0;
			OOX::Spreadsheet::CRow *pRow = new OOX::Spreadsheet::CRow();
			pRow->m_oR.Init();
			pRow->m_oR->SetValue(nIndexRow + 1);
			for (INT nIndex = 0; nIndex < nSize; ++nIndex)
			{
				wcCurrent = pTemp[nIndex];
				if (wcDelimiter == wcCurrent)
				{
					if (bInQuote)
						continue;
					// New Cell
					CString sCellText(pTemp + nStartCell, nIndex - nStartCell);
					AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
                    bIsWrap = false;

					nStartCell = nIndex + 1;
					if (nStartCell == nSize)
					{
						pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
						pRow = NULL;
					}
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
						CString sCellText(pTemp + nStartCell, nIndex - nStartCell);
						AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
                        bIsWrap = false;
					}

					if (wcNewLineR == wcCurrent && nIndex + 1 != nSize && wcNewLineN == pTemp[nIndex + 1])
					{
						// На комбинацию \r\n должен быть только 1 перенос
						++nIndex;
					}
					
					nStartCell = nIndex + 1;

					pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
					pRow = new OOX::Spreadsheet::CRow();
					pRow->m_oR.Init();
					pRow->m_oR->SetValue(++nIndexRow + 1);
					nIndexCol = 0;
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
                    else if ( bInQuote )
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

			if (nStartCell != nSize)
			{
				// New line
				CString sCellText(pTemp + nStartCell, nSize - nStartCell);
				AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
				pWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
			}
			else
			{
				RELEASEOBJECT(pRow);
			}
		}

		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = oXlsx.GetWorksheets();
		arrWorksheets [sSheetRId] = pWorksheet;
	}
}
