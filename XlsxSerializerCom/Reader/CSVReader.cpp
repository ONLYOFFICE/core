#include "CSVReader.h"

#include <map>
#include "../../DesktopEditor/common/File.h"

namespace CSVReader
{
	void AddCell(CString &sText, INT nStartCell, std::stack<INT> &oDeleteChars, OOX::Spreadsheet::CRow &oRow, INT nRow, INT nCol, BOOL bIsWrap)
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
		LONG lValue = wcstol(sText, &pEndPtr, 10);
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

		pCell->m_oRef.Init();
		pCell->m_oRef = OOX::Spreadsheet::CWorksheet::combineRef(nRow, nCol);
		oRow.m_arrItems.push_back(pCell);
	}
    void ReadFromCsvToXlsx(CString &sFileName, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter)
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

		CString sSheetRId = _T("rId1");
		OOX::Spreadsheet::CWorksheet* pWorksheet = new OOX::Spreadsheet::CWorksheet();
		pWorksheet->m_oSheetData.Init();
		OOX::Spreadsheet::CSheet *pSheet = new OOX::Spreadsheet::CSheet();
		pSheet->m_oName.Init();
		pSheet->m_oName->Append(_T("Sheet1"));
		pSheet->m_oSheetId.Init();
		pSheet->m_oSheetId->SetValue(1);
		pSheet->m_oRid.Init();
		pSheet->m_oRid->SetValue(sSheetRId);

		OOX::Spreadsheet::CWorkbook *pWorkbook = oXlsx.GetWorkbook();
		pWorkbook->m_oSheets.Init();
		pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);

		NSFile::CFileBinary oFile;
		if(oFile.OpenFile(string2std_string(sFileName)))
		{
			DWORD nFileSize = 0;
			BYTE* pFileData = new BYTE[oFile.GetFileSize()];
			oFile.ReadFile(pFileData, oFile.GetFileSize(), nFileSize);
			oFile.CloseFile();
//todo
#ifdef _WIN32
			INT nSize = MultiByteToWideChar(nCodePage, 0, (LPCSTR)pFileData, nFileSize, NULL, 0);
			WCHAR *pTemp = new WCHAR [nSize];
			memset(pTemp, 0, sizeof(WCHAR) * nSize);
			MultiByteToWideChar (nCodePage, 0, (LPCSTR)pFileData, nFileSize, pTemp, nSize);
#else
			std::wstring sFileDataW = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pFileData, nFileSize);
			INT nSize = sFileDataW.length();
			const WCHAR *pTemp = sFileDataW.c_str();
#endif

            const WCHAR wcNewLineN = _T('\n');
            const WCHAR wcNewLineR = _T('\r');
            const WCHAR wcQuote = _T('"');
            const WCHAR wcTab = _T('\t');

			BOOL bIsWrap = FALSE;
			WCHAR wcCurrent;
			INT nStartCell = 0;
			std::stack<INT> oDeleteChars;
			BOOL bInQuote = FALSE;
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
					bIsWrap = FALSE;

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
						bIsWrap = TRUE;
						continue;
					}
					// New line
					if (nStartCell != nIndex)
					{
						CString sCellText(pTemp + nStartCell, nIndex - nStartCell);
						AddCell(sCellText, nStartCell, oDeleteChars, *pRow, nIndexRow, nIndexCol++, bIsWrap);
						bIsWrap = FALSE;
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
					if (FALSE == bInQuote && nStartCell == nIndex && nIndex + 1 != nSize)
					{
						// Начало новой ячейки (только если мы сразу после разделителя и не в конце файла)
						bInQuote = !bInQuote;
						nStartCell = nIndex + 1;
					}
					else if (TRUE == bInQuote)
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
#ifdef _WIN32
			RELEASEARRAYOBJECTS(pTemp);
#endif
		}

		std::map<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = oXlsx.GetWorksheets();
		arrWorksheets [sSheetRId] = pWorksheet;
	}
}
