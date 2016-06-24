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
#include "CSVWriter.h"
#include "../../UnicodeConverter/UnicodeConverter.h"
#include "../../UnicodeConverter/UnicodeConverter_Encodings.h"
#include "../../DesktopEditor/common/StringBuilder.h"

namespace CSVWriter
{
	void escapeJson(const CString sInput, NSStringUtils::CStringBuilder& oBuilder)
	{
		//http://stackoverflow.com/questions/7724448/simple-json-string-escape-for-c
		for (int i = 0; i < sInput.GetLength(); ++i)
		{
			WCHAR c = sInput[i];
			switch (c)
			{
			case '"': oBuilder.WriteString(_T("\\\"")); break;
			case '\\': oBuilder.WriteString(_T("\\\\")); break;
			case '\b': oBuilder.WriteString(_T("\\b")); break;
			case '\f': oBuilder.WriteString(_T("\\f")); break;
			case '\n': oBuilder.WriteString(_T("\\n")); break;
			case '\r': oBuilder.WriteString(_T("\\r")); break;
			case '\t': oBuilder.WriteString(_T("\\t")); break;
			default:
				if ('\x00' <= c && c <= '\x1f')
				{
					oBuilder.WriteString(_T("\\u00"));
					oBuilder.WriteHexByte((unsigned char)c);
				}
				else
				{
					oBuilder.AddCharSafe(c);
				}
			}
		}
	}
    void WriteFile(NSFile::CFileBinary *pFile, WCHAR **pWriteBuffer, INT &nCurrentIndex, CString &sWriteString, UINT &nCodePage, bool bIsEnd)
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
                const NSUnicodeConverter::EncodindId& oEncodindId = NSUnicodeConverter::Encodings[nCodePage];
                NSUnicodeConverter::CUnicodeConverter oUnicodeConverter;
                std::string sFileDataA = oUnicodeConverter.fromUnicode(*pWriteBuffer, nCurrentIndex, oEncodindId.Name);
                pFile->WriteFile((BYTE*)sFileDataA.c_str(), sizeof (CHAR) * sFileDataA.length());
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
    void WriteFromXlsxToCsv(const CString &sFileDst, OOX::Spreadsheet::CXlsx &oXlsx, UINT nCodePage, const WCHAR wcDelimiter, bool bJSON)
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
                        bool bIsWriteCell = false; // Нужно только для записи JSON-а
						for (INT j = 0; j < pRow->m_arrItems.size(); ++j)
						{
							INT nRowTmp = 0;
							INT nCol = 0;
							if (!OOX::Spreadsheet::CWorksheet::parseRef(pRow->m_arrItems[j]->m_oRef.get2(), nRowTmp, nCol))
								nCol = 0 == j ? nColCurrent : nColCurrent + 1;

							while (nCol > nColCurrent)
							{
                                if (bJSON && false == bIsWriteCell)
								{
									// Запишем пустые строки (для JSON-а)
									WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sDoubleQuote, nCodePage);
								}
								// Write delimiter
								++nColCurrent;
								WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sDelimiter, nCodePage);
                                bIsWriteCell = false;
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
							if(bJSON)
							{
								NSStringUtils::CStringBuilder oBuilder;
								oBuilder.WriteString(_T("\""));
								escapeJson(sCellValue, oBuilder);
								oBuilder.WriteString(_T("\""));
								sCellValue = CString(oBuilder.GetBuffer(), oBuilder.GetCurSize());
							}
							else if (-1 != sCellValue.FindOneOf(sEscape))
							{
								sCellValue.Replace(sQuote, sDoubleQuote);
								sCellValue = sQuote + sCellValue + sQuote;
							}
							// Write cell value
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sCellValue, nCodePage);
                            bIsWriteCell = true;
						}

						if (bJSON)
							WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage);
					}

					if (bJSON)
					{
						WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage);
                        WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sEndJson, nCodePage, true);
					}
				}
			}
		}

		// Теперь мы пишем как MS Excel (новую строку записываем в файл)
		if (!bJSON)
		{
			WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage);
            WriteFile(&oFile, &pWriteBuffer, nCurrentIndex, sNewLineN, nCodePage, true);
		}
		RELEASEARRAYOBJECTS(pWriteBuffer);
		oFile.CloseFile();
	}
}
