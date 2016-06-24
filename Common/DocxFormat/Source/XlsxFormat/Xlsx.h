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
#pragma once
#ifndef OOX_XLSX_INCLUDE_H_
#define OOX_XLSX_INCLUDE_H_

#include "../Base/SmartPtr.h"
#include "../DocxFormat/IFileContainer.h"

#include "../SystemUtility/FileSystem/Directory.h"
#include "../DocxFormat/Theme/Theme.h"
#include "../DocxFormat/App.h"
#include "../DocxFormat/Core.h"

#include "Workbook/Workbook.h"
#include "SharedStrings/SharedStrings.h"
#include "Styles/Styles.h"
#include "Worksheets/Worksheet.h"
#include "CalcChain/CalcChain.h"
#include "ExternalLinks/ExternalLinks.h"
#include "ExternalLinks/ExternalLinkPath.h"
#include <map>

namespace OOX
{
	namespace Spreadsheet
	{
		class CXlsx : public OOX::Spreadsheet::IFileContainer
		{
		public:

			CXlsx()
			{
				init();
			}
			CXlsx(const CPath& oFilePath)
			{
				init();

				Read( oFilePath );
			}
			~CXlsx()
			{
                            if(bDeleteWorkbook)
                                RELEASEOBJECT(m_pWorkbook);
                            if(bDeleteSharedStrings)
                                RELEASEOBJECT(m_pSharedStrings);
                            if(bDeleteStyles)
                                RELEASEOBJECT(m_pStyles);
                            if(bDeleteTheme)
                                RELEASEOBJECT(m_pTheme);
                            if(bDeleteCalcChain)
                                RELEASEOBJECT(m_pCalcChain);
                            if(bDeleteWorksheets)
                            {
                                for (std::map<CString, CWorksheet*>::const_iterator it = m_aWorksheets.begin(); it != m_aWorksheets.end(); ++it)
                                {
                                    if (NULL != it->second)
                                        delete it->second;
                                }
                            }
			}
		public:

                        bool Read(const CPath& oFilePath)
			{
                            // Ищем "/_rels/.rels" и читаем все файлы по рельсам
                            OOX::CRels oRels( oFilePath / FILE_SEPARATOR_STR );
                            IFileContainer::Read( oRels, oFilePath, oFilePath );

                            // Выполняем дополнительные действия для более удобной работы с файлом

                            // Ищем основной документ
                            smart_ptr<OOX::File> pFile = Find(OOX::Spreadsheet::FileTypes::Workbook);
                            if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::Workbook == pFile->type())
                                m_pWorkbook = (OOX::Spreadsheet::CWorkbook*)pFile.operator->();
                            else
                                m_pWorkbook = NULL;

                            if ( m_pWorkbook )
                            {
                                OOX::Spreadsheet::IFileContainer* pDocumentContainer = (OOX::Spreadsheet::IFileContainer*)m_pWorkbook;

                                //SharedStrings
                                pFile = pDocumentContainer->Find( OOX::Spreadsheet::FileTypes::SharedStrings );
                                if ( pFile.IsInit() && OOX::Spreadsheet::FileTypes::SharedStrings == pFile->type() )
                                    m_pSharedStrings = (OOX::Spreadsheet::CSharedStrings*)pFile.operator->();
                                else
                                    m_pSharedStrings = NULL;

                                //Styles
                                pFile = pDocumentContainer->Find( OOX::Spreadsheet::FileTypes::Styles );
                                if ( pFile.IsInit() && OOX::Spreadsheet::FileTypes::Styles == pFile->type() )
                                    m_pStyles = (OOX::Spreadsheet::CStyles*)pFile.operator->();
                                else
                                    m_pStyles = NULL;

                                //OOX::CRels rels(oFilePath / m_pDocument->DefaultDirectory() / m_pDocument->DefaultFileName());
                                //IFileContainer::Read(rels, oFilePath);

                                //Theme
                                pFile = pDocumentContainer->Find(OOX::FileTypes::Theme);
                                if (pFile.IsInit() && OOX::FileTypes::Theme == pFile->type())
                                    m_pTheme = (OOX::CTheme*)pFile.operator->();
                                else
                                    m_pTheme = NULL;

                                //CalcChain
                                pFile = pDocumentContainer->Find(OOX::Spreadsheet::FileTypes::CalcChain);
                                if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::CalcChain == pFile->type())
                                    m_pCalcChain = (OOX::Spreadsheet::CCalcChain*)pFile.operator->();
                                else
                                    m_pCalcChain = NULL;


                                std::map<CString, smart_ptr<OOX::File>> aWorksheetsFiles;
                                pDocumentContainer->FindAllByType(OOX::Spreadsheet::FileTypes::Worksheet, aWorksheetsFiles);
                                pDocumentContainer->FindAllByType(OOX::Spreadsheet::FileTypes::Chartsheets, aWorksheetsFiles);

                                for (std::map<CString, smart_ptr<OOX::File>>::const_iterator it = aWorksheetsFiles.begin(); it != aWorksheetsFiles.end(); ++it)
                                {
                                    m_aWorksheets [it->first] = (OOX::Spreadsheet::CWorksheet*) it->second.operator->();
                                }
                            }

                            return true;
			}
                        bool Write(const CPath& oDirPath, CString& sAdditionalContentTypes)
			{
                            if(NULL == m_pWorkbook || 0 == m_aWorksheets.size ())
                                return false;
                            PrepareToWrite();

                            OOX::CContentTypes oContentTypes;

                            //docProps
                            //CApp
                            OOX::CApp* pApp = new OOX::CApp();
                            pApp->SetApplication(_T("OnlyOffice"));
                            pApp->SetAppVersion(_T("3.0000"));
                            pApp->SetDocSecurity(0);
                            pApp->SetScaleCrop(false);
                            pApp->SetLinksUpToDate(false);
                            pApp->SetSharedDoc(false);
                            pApp->SetHyperlinksChanged(false);

                            smart_ptr<OOX::File> pAppFile(pApp);
                            const OOX::RId oAppRId =  Add(pAppFile);
                            //CCore
                            OOX::CCore* pCore = new OOX::CCore();
                            pCore->SetCreator(_T(""));
                            pCore->SetLastModifiedBy(_T(""));
                            smart_ptr<OOX::File> pCoreFile(pCore);
                            const OOX::RId oCoreRId = Add(pCoreFile);

                            //xl
                            CPath oXlPath = oDirPath / m_pWorkbook->DefaultDirectory();
                            WriteWorkbook(oXlPath);

                            IFileContainer::Write(oDirPath / FILE_SEPARATOR_STR , OOX::CPath(_T("")), oContentTypes);
                            if(!sAdditionalContentTypes.IsEmpty())
                            {
                                CString sAdditionalContentTypesWrapped;
                                sAdditionalContentTypesWrapped.Append(_T("<Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"));
                                sAdditionalContentTypesWrapped.Append(sAdditionalContentTypes);
                                sAdditionalContentTypesWrapped.Append(_T("</Types>"));
                                OOX::CContentTypes oTempContentTypes;

                                oTempContentTypes.ReadFromString(sAdditionalContentTypesWrapped);

                                for (std::map<CString, ContentTypes::COverride>::const_iterator it = oTempContentTypes.m_arrOverride.begin(); it != oTempContentTypes.m_arrOverride.end(); ++it)
                                {
                                    const ContentTypes::COverride& oOverride = it->second;
                                    const OOX::CPath& oPath = oOverride.filename();
                                    oContentTypes.Registration(oOverride.type(), oPath.GetDirectory(), oPath.GetFilename());
                                }
                            }
                            oContentTypes.Write(oDirPath);
                            return true;
                        }
                        bool WriteWorkbook(const CPath& oDirPath)
			{
                            //Theme
                            OOX::CTheme* pTheme = new OOX::CTheme();
                            pTheme->DoNotWriteContent(true);
                            smart_ptr<OOX::File> pThemeFile(pTheme);
                            m_pWorkbook->Add(pThemeFile);
                            //SharedStrings
                            if(NULL != m_pSharedStrings && m_pSharedStrings->m_arrItems.size() > 0)
                            {
                                smart_ptr<OOX::File> pSharedStringsFile(m_pSharedStrings);
                                bDeleteSharedStrings = false;
                                m_pWorkbook->Add(pSharedStringsFile);
                            }
                            //Styles
                            if(NULL != m_pStyles)
                            {
                                smart_ptr<OOX::File> pStylesFile(m_pStyles);
                                bDeleteStyles = false;
                                m_pWorkbook->Add(pStylesFile);
                            }

                            //Workbook
                            smart_ptr<OOX::File> pWorkbookFile(m_pWorkbook);
                            bDeleteWorkbook = false;
                            Add(pWorkbookFile);
                            return true;
                        }
			void PrepareToWrite()
			{
                            if(NULL != m_pWorkbook)
                                m_pWorkbook->PrepareToWrite();
                            if(NULL != m_pStyles)
                                m_pStyles->PrepareToWrite();

                            for (std::map<CString, CWorksheet*>::const_iterator it = m_aWorksheets.begin(); it != m_aWorksheets.end(); ++it)
                            {
                                if (NULL != it->second)
                                    it->second->PrepareToWrite();
                            }
			}
		public:
			CWorkbook  *GetWorkbook () const
			{
				return m_pWorkbook;
			}
			CWorkbook  *CreateWorkbook ()
			{
				if(bDeleteWorkbook)
					RELEASEOBJECT(m_pWorkbook);
				m_pWorkbook = new CWorkbook();
				bDeleteWorkbook = true;
				return m_pWorkbook;
			}
			CSharedStrings  *GetSharedStrings () const
			{
				return m_pSharedStrings;
			}
			CSharedStrings  *CreateSharedStrings ()
			{
				if(bDeleteSharedStrings)
					RELEASEOBJECT(m_pSharedStrings);
				m_pSharedStrings = new CSharedStrings();
				bDeleteSharedStrings = true;
				return m_pSharedStrings;
			}
			CStyles  *GetStyles () const
			{
				return m_pStyles;
			}
			CStyles  *CreateStyles ()
			{
				if(bDeleteStyles)
					RELEASEOBJECT(m_pStyles);
				m_pStyles = new CStyles();
				bDeleteStyles = true;
				return m_pStyles;
			}
			CTheme  *GetTheme () const
			{
				return m_pTheme;
			}
			CCalcChain  *GetCalcChain () const
			{
				return m_pCalcChain;
			}
			std::map<CString, CWorksheet*>  &GetWorksheets ()
			{
				return m_aWorksheets;
			}
			void PrepareWorkbook()
			{
				IFileContainer::m_mapEnumeratedGlobal.clear();
				if(NULL == m_pWorkbook)
				{
					m_pWorkbook = new OOX::Spreadsheet::CWorkbook();
					m_pWorkbook->m_oWorkbookPr.Init();
					m_pWorkbook->m_oWorkbookPr->m_oDefaultThemeVersion.Init();
					m_pWorkbook->m_oWorkbookPr->m_oDefaultThemeVersion->SetValue(124226);
					m_pWorkbook->m_oBookViews.Init();
					OOX::Spreadsheet::CWorkbookView* pWorkbookView = new OOX::Spreadsheet::CWorkbookView();
					pWorkbookView->m_oXWindow.Init();
					pWorkbookView->m_oXWindow->SetValue(480);
					pWorkbookView->m_oYWindow.Init();
					pWorkbookView->m_oYWindow->SetValue(120);
					pWorkbookView->m_oWindowWidth.Init();
					pWorkbookView->m_oWindowWidth->SetValue(27795);
					pWorkbookView->m_oWindowHeight.Init();
					pWorkbookView->m_oWindowHeight->SetValue(12585);
					m_pWorkbook->m_oBookViews->m_arrItems.push_back(pWorkbookView);
				}
				//добавляем sheet, если нет ни одного
				if (m_aWorksheets.empty())
				{
					OOX::Spreadsheet::CWorksheet* pWorksheet = new OOX::Spreadsheet::CWorksheet();
					pWorksheet->m_oDimension.Init();
					pWorksheet->m_oDimension->m_oRef.Init();
					pWorksheet->m_oDimension->m_oRef->Append(_T("A1"));
					pWorksheet->m_oSheetViews.Init();
					OOX::Spreadsheet::CSheetView* pSheetView = new OOX::Spreadsheet::CSheetView();
					pSheetView->m_oTabSelected.Init();
					pSheetView->m_oTabSelected->FromBool(true);
					pSheetView->m_oWorkbookViewId.Init();
					pSheetView->m_oWorkbookViewId->SetValue(0);
					pWorksheet->m_oSheetViews->m_arrItems.push_back(pSheetView);
					pWorksheet->m_oSheetFormatPr.Init();
					pWorksheet->m_oSheetFormatPr->m_oDefaultRowHeight.Init();
					pWorksheet->m_oSheetFormatPr->m_oDefaultRowHeight->SetValue(15);
					pWorksheet->m_oPageMargins.Init();
					pWorksheet->m_oPageMargins->m_oLeft.Init();
					pWorksheet->m_oPageMargins->m_oLeft->FromInches(0.7);
					pWorksheet->m_oPageMargins->m_oTop.Init();
					pWorksheet->m_oPageMargins->m_oTop->FromInches(0.7);
					pWorksheet->m_oPageMargins->m_oRight.Init();
					pWorksheet->m_oPageMargins->m_oRight->FromInches(0.7);
					pWorksheet->m_oPageMargins->m_oBottom.Init();
					pWorksheet->m_oPageMargins->m_oBottom->FromInches(0.7);
					pWorksheet->m_oPageMargins->m_oHeader.Init();
					pWorksheet->m_oPageMargins->m_oHeader->FromInches(0.3);
					pWorksheet->m_oPageMargins->m_oFooter.Init();
					pWorksheet->m_oPageMargins->m_oFooter->FromInches(0.3);
					smart_ptr<OOX::File> pWorksheetFile(pWorksheet);
					OOX::RId oRId = this->Add(pWorksheetFile);
					m_aWorksheets [oRId.ToString()] = pWorksheet;
					m_pWorkbook->m_oSheets.Init();
					OOX::Spreadsheet::CSheet* pSheet = new OOX::Spreadsheet::CSheet();
					pSheet->m_oName.Init();
					pSheet->m_oName->Append(_T("Sheet1"));
					pSheet->m_oSheetId.Init();
					pSheet->m_oSheetId->SetValue(1);
					pSheet->m_oRid.Init();
					pSheet->m_oRid->SetValue(oRId.ToString());
					m_pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);
				}
				//делаем так чтобы всегда были нулевые стили и первый font всегда имел шрифт и размер
				if(NULL != m_pStyles )
				{
					//Fonts
					if(false == m_pStyles->m_oFonts.IsInit())
						m_pStyles->m_oFonts.Init();
					if(m_pStyles->m_oFonts->m_arrItems.size() == 0)
						m_pStyles->m_oFonts->AddFont(new OOX::Spreadsheet::CFont());
					OOX::Spreadsheet::CFont* pFont = m_pStyles->m_oFonts->m_arrItems[0];
					if(false == pFont->m_oRFont.IsInit())
					{
						pFont->m_oRFont.Init();
						pFont->m_oRFont->m_sVal = _T("Arial");
					}
					if(false == pFont->m_oSz.IsInit() || false == pFont->m_oSz->m_oVal.IsInit())
					{
						pFont->m_oSz.Init();
						pFont->m_oSz->m_oVal.Init();
						pFont->m_oSz->m_oVal->SetValue(11.0);
					}
					//Fills
					if(false == m_pStyles->m_oFills.IsInit())
						m_pStyles->m_oFills.Init();
					if(m_pStyles->m_oFills->m_arrItems.size() == 0)
						m_pStyles->m_oFills->m_arrItems.push_back(new OOX::Spreadsheet::CFill());
						OOX::Spreadsheet::CFill* pFill = m_pStyles->m_oFills->m_arrItems[0];
						if(false == pFill->m_oGradientFill.IsInit())
						{
							if(false == pFill->m_oPatternFill.IsInit())
								pFill->m_oPatternFill.Init();
							if(false == pFill->m_oPatternFill->m_oPatternType.IsInit())
								pFill->m_oPatternFill->m_oPatternType.Init();
							pFill->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);
						}
					if(false == m_pStyles->m_oBorders.IsInit())
						m_pStyles->m_oBorders.Init();
					if(m_pStyles->m_oBorders->m_arrItems.size() == 0)
						m_pStyles->m_oBorders->m_arrItems.push_back(new OOX::Spreadsheet::CBorder());

					//Xfs
					if(false == m_pStyles->m_oCellXfs.IsInit())
						m_pStyles->m_oCellXfs.Init();
					if(m_pStyles->m_oCellXfs->m_arrItems.size() == 0)
						m_pStyles->m_oCellXfs->m_arrItems.push_back(new OOX::Spreadsheet::CXfs());

					OOX::Spreadsheet::CXfs* pXfs = m_pStyles->m_oCellXfs->m_arrItems[0];
					if(false == pXfs->m_oBorderId.IsInit())
					{
						pXfs->m_oBorderId.Init();
						pXfs->m_oBorderId->SetValue(0);
					}
					if(false == pXfs->m_oFillId.IsInit())
					{
						pXfs->m_oFillId.Init();
						pXfs->m_oFillId->SetValue(0);
					}
					if(false == pXfs->m_oFontId.IsInit())
					{
						pXfs->m_oFontId.Init();
						pXfs->m_oFontId->SetValue(0);
					}
					if(false == pXfs->m_oNumFmtId.IsInit())
					{
						pXfs->m_oNumFmtId.Init();
						pXfs->m_oNumFmtId->SetValue(0);
					}
				}
				//переносим теги <is> и ячейки с типом str в sharedString
				for (std::map<CString, CWorksheet*>::const_iterator it = m_aWorksheets.begin(); it != m_aWorksheets.end(); ++it)
				{
					PrepareWorksheet(it->second);
				}
				//todo парсим даты в формате iso 8601
			}
		private:
			void PrepareWorksheet(CWorksheet* pWorksheet)
			{
				if(pWorksheet->m_oSheetData.IsInit())
				{
					std::vector<OOX::Spreadsheet::CRow*>& aRows = pWorksheet->m_oSheetData->m_arrItems;
					for(unsigned int i = 0, length = aRows.size(); i < length; ++i)
					{
						OOX::Spreadsheet::CRow* pRow = aRows[i];
						std::vector<OOX::Spreadsheet::CCell*> & aCells = pRow->m_arrItems;
						for(unsigned int j = 0, length2 = aCells.size(); j < length2; ++j)
						{
							OOX::Spreadsheet::CCell* pCell = aCells[j];
							if(pCell->m_oType.IsInit())
							{
								if(SimpleTypes::Spreadsheet::celltypeInlineStr == pCell->m_oType->GetValue())
								{
									CSharedStrings* pSharedStrings = GetSharedStrings();
									if(NULL == pSharedStrings)
										pSharedStrings = CreateSharedStrings();
									OOX::Spreadsheet::CSi* pSi = pCell->m_oRichText.GetPointerEmptyNullable();
									if(NULL != pSi)
									{
										int nIndex = pSharedStrings->AddSi(pSi);
										//меняем значение ячейки
										pCell->m_oValue.Init();
										pCell->m_oValue->m_sText.Format(_T("%d"), nIndex);
										//меняем тип ячейки
										pCell->m_oType.Init();
										pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
									}
								}
								else if(SimpleTypes::Spreadsheet::celltypeStr == pCell->m_oType->GetValue() || SimpleTypes::Spreadsheet::celltypeError == pCell->m_oType->GetValue())
								{
									CSharedStrings* pSharedStrings = GetSharedStrings();
									if(NULL == pSharedStrings)
										pSharedStrings = CreateSharedStrings();
									CString sValue;
									if(pCell->m_oValue.IsInit())
										sValue = pCell->m_oValue->ToString();
									//добавляем в SharedStrings
									CSi* pSi = new CSi();
									CText* pText =  new CText();
									pText->m_sText = sValue;
									pSi->m_arrItems.push_back(pText);
									int nIndex = pSharedStrings->AddSi(pSi);
									//меняем значение ячейки
									pCell->m_oValue.Init();
									pCell->m_oValue->m_sText.Format(_T("%d"), nIndex);
									//меняем тип ячейки
									if(SimpleTypes::Spreadsheet::celltypeStr == pCell->m_oType->GetValue())
									{
										pCell->m_oType.Init();
										pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeSharedString);
									}
								}
								else if(SimpleTypes::Spreadsheet::celltypeBool == pCell->m_oType->GetValue())
								{
									//обычно пишется 1/0, но встречается, что пишут true/false
									if(pCell->m_oValue.IsInit())
									{
										SimpleTypes::COnOff<> oOnOff;
										oOnOff.FromString(pCell->m_oValue->ToString());
										pCell->m_oValue.Init();
										if(oOnOff.ToBool())
											pCell->m_oValue->m_sText = _T("1");
										else
											pCell->m_oValue->m_sText = _T("0");
									}
								}
							}
							
						}
					}
				}
			}
			void init()
			{
				m_pWorkbook = NULL;
				m_pSharedStrings = NULL;
				m_pStyles = NULL;
				m_pTheme = NULL;
				m_pCalcChain = NULL;

				bDeleteWorkbook = false;
				bDeleteSharedStrings = false;
				bDeleteStyles = false;
				bDeleteTheme = false;
				bDeleteCalcChain = false;
				bDeleteWorksheets = false;
			}
		private:
			CWorkbook  *m_pWorkbook;
			bool bDeleteWorkbook;
			CSharedStrings  *m_pSharedStrings;
			bool bDeleteSharedStrings;
			CStyles  *m_pStyles;
			bool bDeleteStyles;
			CTheme  *m_pTheme;
			bool bDeleteTheme;
			CCalcChain  *m_pCalcChain;
			bool bDeleteCalcChain;
			std::map<CString, CWorksheet*> m_aWorksheets;
			bool bDeleteWorksheets;
		};

	} //Spreadsheet
} // OOX

#endif // OOX_XLSX_INCLUDE_H_
