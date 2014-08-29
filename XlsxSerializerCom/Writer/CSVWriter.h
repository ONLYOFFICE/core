#pragma once
#define CP_UTF16		1200
#define CP_unicodeFFFE	1201

namespace CSVWriter
{
	void WriteFile(CFile *pFile, WCHAR **pWriteBuffer, INT &nCurrentIndex, CString &sWriteString, UINT &nCodePage, BOOL bIsEnd = FALSE)
	{
		if (NULL == pFile || NULL == pWriteBuffer)
			return;
		INT nCountChars = sWriteString.GetLength();
		if (0 == nCountChars && !bIsEnd)
			return;

		CONST INT c_nSize = 1048576; // 1024 * 1024
		CONST INT nSizeWchar = sizeof(WCHAR);
		if (NULL == *pWriteBuffer)
		{
			*pWriteBuffer = new WCHAR[c_nSize];
			::ZeroMemory(*pWriteBuffer, nSizeWchar * c_nSize);
			nCurrentIndex = 0;
		}

		if (nCountChars + nCurrentIndex > c_nSize || bIsEnd)
		{
			// Буффер заполнился, пишем
			if (nCodePage == CP_UTF16)
			{
				pFile->WriteFile(*pWriteBuffer, sizeof (WCHAR) * nCurrentIndex);
			}
			else
			{
				INT nSize = WideCharToMultiByte(nCodePage, 0, *pWriteBuffer, nCurrentIndex, NULL, NULL, NULL, NULL);
				CHAR *pString = new CHAR [nSize];
				::ZeroMemory (pString, sizeof (CHAR) * nSize);
				WideCharToMultiByte (CP_UTF8, 0, *pWriteBuffer, -1, pString, nSize, NULL, NULL);
				pFile->WriteFile(pString, sizeof (CHAR) * nSize);
				RELEASEARRAYOBJECTS(pString);
			}	
			
			::ZeroMemory(*pWriteBuffer, nSizeWchar * c_nSize);
			nCurrentIndex = 0;
		}
		
		if (!bIsEnd)
		{
			::CopyMemory(*pWriteBuffer + nCurrentIndex, sWriteString.GetBuffer(), nCountChars * nSizeWchar);
			nCurrentIndex += nCountChars;
		}
	}
	void WriteFromXlsxToCsv(CString &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, CONST WCHAR wcDelimiter)
	{
		CFile oFile;
		oFile.CreateFileW(sFileDst);

		// Нужно записать шапку
		if (CP_UTF8 == nCodePage)
		{
			UCHAR arUTF8[3] = {0xEF, 0xBB, 0xBF};
			oFile.WriteFile(arUTF8, 3);
		}
		else if (CP_UTF16 == nCodePage)
		{
			UCHAR arUTF16[2] = {0xFF, 0xFE};
			oFile.WriteFile(arUTF16, 2);
		}
		else if (CP_unicodeFFFE == nCodePage)
		{
			UCHAR arBigEndian[2] = {0xFE, 0xFF};
			oFile.WriteFile(arBigEndian, 2);
		}

		LONG lActiveSheet = 0;
		CString sSheetRId = _T("Sheet1"); // Читаем не по rId, а по имени листа
		OOX::Spreadsheet::CWorkbook *pWorkbook = oXlsx.GetWorkbook();
		if (NULL != pWorkbook)
		{
			// Get active sheet
			if (pWorkbook->m_oBookViews.IsInit() && 0 < pWorkbook->m_oBookViews->m_arrItems.size())
			{
				if (pWorkbook->m_oBookViews->m_arrItems[0]->m_oActiveTab.IsInit())
				{
					lActiveSheet = pWorkbook->m_oBookViews->m_arrItems[0]->m_oActiveTab->GetValue();
					if (0 > lActiveSheet)
						lActiveSheet = 0;
				}
			}

			// Get active sheet rId
			if (pWorkbook->m_oSheets.IsInit() && 0 <= pWorkbook->m_oSheets->m_arrItems.size())
			{
				if (lActiveSheet <= pWorkbook->m_oSheets->m_arrItems.size())
					sSheetRId = pWorkbook->m_oSheets->m_arrItems[lActiveSheet]->m_oName.get2();
				else
					sSheetRId = pWorkbook->m_oSheets->m_arrItems[0]->m_oName.get2();
			}

			CAtlMap<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = oXlsx.GetWorksheets();
			CAtlMap<CString, OOX::Spreadsheet::CWorksheet*>::CPair* pPair = arrWorksheets.Lookup(sSheetRId);
			if (NULL != pPair)
			{
				OOX::Spreadsheet::CWorksheet *pWorksheet = pPair->m_value;
				if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
				{
					OOX::Spreadsheet::CSharedStrings *pSharedStrings = oXlsx.GetSharedStrings();
					CString sNewLineN = _T("\n");
					CString sDelimiter = _T(""); sDelimiter += wcDelimiter;
					CONST WCHAR wcQuote = _T('"');
					CString sEscape = _T("\"\n");
					sEscape += wcDelimiter;

					INT nCurrentIndex = 0;
					WCHAR *pWriteBuffer = NULL;

					INT nRowCurrent = 1;
					for (INT i = 0; i < pWorksheet->m_oSheetData->m_arrItems.size(); ++i)
					{
						OOX::Spreadsheet::CRow *pRow = static_cast<OOX::Spreadsheet::CRow *>(pWorksheet->m_oSheetData->m_arrItems[i]);
						INT nRow = pRow->m_oR.IsInit() ? pRow->m_oR->GetValue() : 0 == i ? nRowCurrent : nRowCurrent + 1;

						while (nRow > nRowCurrent)
						{
							// Write new line
							++nRowCurrent;
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage);
						}

						INT nColCurrent = 1;
						for (INT j = 0; j < pRow->m_arrItems.size(); ++j)
						{
							INT nRowTmp = 0;
							INT nCol = 0;
							if (!OOX::Spreadsheet::CWorksheet::parseRef(pRow->m_arrItems[j]->m_oRef.get2(), nRowTmp, nCol))
								nCol = 0 == j ? nColCurrent : nColCurrent + 1;

							while (nCol > nColCurrent)
							{
								// Write delimiter
								++nColCurrent;
								WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sDelimiter, nCodePage);
							}

							OOX::Spreadsheet::CCell *pCell = static_cast<OOX::Spreadsheet::CCell *>(pRow->m_arrItems[j]);

							// Get cell value
							CString sCellValue = _T("");
							if (pCell->m_oValue.IsInit())
							{
								if (pCell->m_oType.IsInit() && SimpleTypes::Spreadsheet::celltypeNumber != pCell->m_oType->GetValue())
								{
									int nValue = _wtoi(pCell->m_oValue->ToString());
									if (0 <= nValue && nValue < pSharedStrings->m_arrItems.size())
									{
										OOX::Spreadsheet::CSi *pSi = static_cast<OOX::Spreadsheet::CSi *>(pSharedStrings->m_arrItems[nValue]);
										if (NULL != pSi && pSi->m_arrItems.size() > 0)
											if(NULL != pSi && pSi->m_arrItems.size() > 0)
											{
												OOX::Spreadsheet::WritingElement* pWe = pSi->m_arrItems[0];
												if(OOX::Spreadsheet::et_t == pWe->getType())
												{
													OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(pWe);
													sCellValue = pText->m_sText;
												}
											}
									}
								}
								else 
								{
									sCellValue = pCell->m_oValue->ToString();
								}
							}

							// Escape cell value
							if (-1 != sCellValue.FindOneOf(sEscape))
							{
								sCellValue.Replace(_T("\""), _T("\"\""));
								sCellValue = wcQuote + sCellValue + wcQuote;
							}
							// Write cell value
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sCellValue, nCodePage);
						}
					}

					WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage, TRUE);
					RELEASEARRAYOBJECTS(pWriteBuffer);
				}
			}
		}

		oFile.CloseFile();
	}
}