#include "CSVWriter.h"

namespace CSVWriter
{
	void WriteFile(NSFile::CFileBinary *pFile, WCHAR **pWriteBuffer, INT &nCurrentIndex, CString &sWriteString, UINT &nCodePage, BOOL bIsEnd)
	{
		if (NULL == pFile || NULL == pWriteBuffer)
			return;
		INT nCountChars = sWriteString.GetLength();
		if (0 == nCountChars && !bIsEnd)
			return;

        const INT c_nSize = 1048576; // 1024 * 1024
        const INT nSizeWchar = sizeof(WCHAR);
		if (NULL == *pWriteBuffer)
		{
			*pWriteBuffer = new WCHAR[c_nSize];
			memset(*pWriteBuffer, 0, nSizeWchar * c_nSize);
			nCurrentIndex = 0;
		}

		if (nCountChars + nCurrentIndex > c_nSize || bIsEnd)
		{
			// Буффер заполнился, пишем
			if (nCodePage == CP_UTF16)
			{
				pFile->WriteFile((BYTE*)*pWriteBuffer, sizeof (WCHAR) * nCurrentIndex);
			}
			else
			{
//todo
#if defined(_WIN32) || defined (_WIN64)
				INT nSize = WideCharToMultiByte(nCodePage, 0, *pWriteBuffer, nCurrentIndex, NULL, NULL, NULL, NULL);
				CHAR *pString = new CHAR [nSize];
				memset(pString, 0, sizeof (CHAR) * nSize);
				WideCharToMultiByte (nCodePage, 0, *pWriteBuffer, -1, pString, nSize, NULL, NULL);
				pFile->WriteFile((BYTE*)pString, sizeof (CHAR) * nSize);
				RELEASEARRAYOBJECTS(pString);
#else
				BYTE* pUtf8 = NULL;
				long nUtf8Size;
				NSFile::CUtf8Converter::GetUtf8StringFromUnicode(*pWriteBuffer, nCurrentIndex, pUtf8, nUtf8Size);
				pFile->WriteFile(pUtf8, nUtf8Size);
				RELEASEARRAYOBJECTS(pUtf8);
#endif
			}	
			
			memset(*pWriteBuffer, 0, nSizeWchar * c_nSize);
			nCurrentIndex = 0;
		}
		
		if (!bIsEnd)
		{
			memcpy(*pWriteBuffer + nCurrentIndex, sWriteString.GetBuffer(), nCountChars * nSizeWchar);
			nCurrentIndex += nCountChars;
		}
	}
    void WriteFromXlsxToCsv(CString &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter, BOOL bJSON)
	{
		NSFile::CFileBinary oFile;
		oFile.CreateFileW(string2std_string(sFileDst));

		// Нужно записать шапку
		if (CP_UTF8 == nCodePage)
		{
			BYTE arUTF8[3] = {0xEF, 0xBB, 0xBF};
			oFile.WriteFile(arUTF8, 3);
		}
		else if (CP_UTF16 == nCodePage)
		{
			BYTE arUTF16[2] = {0xFF, 0xFE};
			oFile.WriteFile(arUTF16, 2);
		}
		else if (CP_unicodeFFFE == nCodePage)
		{
			BYTE arBigEndian[2] = {0xFE, 0xFF};
			oFile.WriteFile(arBigEndian, 2);
		}

		CString sNewLineN = _T("\n");
		LONG lActiveSheet = 0;
		INT nCurrentIndex = 0;
		WCHAR *pWriteBuffer = NULL;

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

			// Get active sheet rId (для конвертации в CSV нужно использовать name, т.к. это наш бинарник из js-скриптов и еще нет rId
			// А для json-а нужно пользовать rId, т.к. при открытии они используются
			if (pWorkbook->m_oSheets.IsInit() && 0 <= pWorkbook->m_oSheets->m_arrItems.size())
			{
				OOX::Spreadsheet::CSheet *pSheet;
				if (lActiveSheet <= pWorkbook->m_oSheets->m_arrItems.size())
					pSheet = pWorkbook->m_oSheets->m_arrItems[lActiveSheet];
				else
					pSheet = pWorkbook->m_oSheets->m_arrItems[0];

				sSheetRId = bJSON ? pSheet->m_oRid->GetValue() : pSheet->m_oName.get2();
			}

			std::map<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = oXlsx.GetWorksheets();
			std::map<CString, OOX::Spreadsheet::CWorksheet*>::const_iterator pPair = arrWorksheets.find(sSheetRId);
			if (pPair != arrWorksheets.end())
			{
				OOX::Spreadsheet::CWorksheet *pWorksheet = pPair->second;
				if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
				{
					OOX::Spreadsheet::CSharedStrings *pSharedStrings = oXlsx.GetSharedStrings();
					CString sDelimiter = _T(""); sDelimiter += wcDelimiter;
					CString sEscape = _T("\"\n");
					sEscape += wcDelimiter;
					CString sEndJson = CString(_T("]"));
					CString sQuote = _T("\"");
					CString sDoubleQuote = _T("\"\"");
                    CString sBkt = CString(_T("["));
                    CString sBktComma = _T(",[");
                    
					if (bJSON)
                        CSVWriter::WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sBkt, nCodePage);

					INT nRowCurrent = 1;
					for (INT i = 0; i < pWorksheet->m_oSheetData->m_arrItems.size(); ++i)
					{
						OOX::Spreadsheet::CRow *pRow = static_cast<OOX::Spreadsheet::CRow *>(pWorksheet->m_oSheetData->m_arrItems[i]);
						INT nRow = pRow->m_oR.IsInit() ? pRow->m_oR->GetValue() : 0 == i ? nRowCurrent : nRowCurrent + 1;

						if (bJSON)
                            CSVWriter::WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, (0 == i ? sBkt: sBktComma), nCodePage);
						else
						{
							while (nRow > nRowCurrent)
							{
								// Write new line
								++nRowCurrent;
								WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage);
							}
						}

						INT nColCurrent = 1;
						BOOL bIsWriteCell = FALSE; // Нужно только для записи JSON-а
						for (INT j = 0; j < pRow->m_arrItems.size(); ++j)
						{
							INT nRowTmp = 0;
							INT nCol = 0;
							if (!OOX::Spreadsheet::CWorksheet::parseRef(pRow->m_arrItems[j]->m_oRef.get2(), nRowTmp, nCol))
								nCol = 0 == j ? nColCurrent : nColCurrent + 1;

							while (nCol > nColCurrent)
							{
								if (bJSON && FALSE == bIsWriteCell)
								{
									// Запишем пустые строки (для JSON-а)
									WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sDoubleQuote, nCodePage);
								}
								// Write delimiter
								++nColCurrent;
								WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sDelimiter, nCodePage);
								bIsWriteCell = FALSE;
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
							if (bJSON || -1 != sCellValue.FindOneOf(sEscape))
							{
								sCellValue.Replace(sQuote, sDoubleQuote);
								sCellValue = sQuote + sCellValue + sQuote;
							}
							// Write cell value
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sCellValue, nCodePage);
							bIsWriteCell = TRUE;
						}

						if (bJSON)
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage);
					}

					if (bJSON)
					{
						WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage);
						WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage, TRUE);
					}
				}
			}
		}

		// Теперь мы пишем как MS Excel (новую строку записываем в файл)
		if (!bJSON)
		{
			WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage);
			WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage, TRUE);
		}
		RELEASEARRAYOBJECTS(pWriteBuffer);
		oFile.CloseFile();
	}
}
