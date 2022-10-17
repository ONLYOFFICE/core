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
#pragma once

#include "Workbook.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CWorkbook::CWorkbook(OOX::Document* pMain) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
		{
			m_bMacroEnabled = false;
			m_bSpreadsheets = true;
			m_pPersonList = NULL;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(pMain);
			if ((xlsx) && (!xlsx->m_pWorkbook))
			{
				xlsx->m_pWorkbook = this;
			}
		}
		CWorkbook::CWorkbook(OOX::Document* pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain), WritingElement(pMain)
		{
			m_bMacroEnabled = false;
			m_bSpreadsheets = true;
			m_pPersonList = NULL;

			CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
			if ((xlsx) && (!xlsx->m_pWorkbook))
			{
				xlsx->m_pWorkbook = this;
			}
			read(oRootPath, oPath);
		}
		CWorkbook::~CWorkbook()
		{
		}
		void CWorkbook::readBin(const CPath& oPath)
		{
			CXlsb* xlsb = dynamic_cast<CXlsb*>(File::m_pMainDocument);
			if (xlsb)
			{
				XLSB::WorkBookStreamPtr workBookStream(new XLSB::WorkBookStream);

				xlsb->ReadBin(oPath, workBookStream.get());

				workBookStream->UpdateXti(xlsb->GetGlobalinfo());
				workBookStream->UpdateDefineNames(xlsb->GetGlobalinfo());

				if (workBookStream != nullptr)
				{
					if (workBookStream->m_BOOKVIEWS != nullptr)
						m_oBookViews = static_cast<XLSB::BOOKVIEWS*>(workBookStream->m_BOOKVIEWS.get())->m_arBrtBookView;
					if (workBookStream->m_BrtCalcProp != nullptr)
						m_oCalcPr = workBookStream->m_BrtCalcProp;
					if (!workBookStream->m_arBrtName.empty())
						m_oDefinedNames = workBookStream->m_arBrtName;
					if (workBookStream->m_BUNDLESHS != nullptr)
						m_oSheets = static_cast<XLSB::BUNDLESHS*>(workBookStream->m_BUNDLESHS.get())->m_arBrtBundleSh;
					if (workBookStream->m_BrtWbProp != nullptr)
						m_oWorkbookPr = workBookStream->m_BrtWbProp;
					if (workBookStream->m_PIVOTCACHEIDS != nullptr)
						m_oPivotCaches = workBookStream->m_PIVOTCACHEIDS;

					if (workBookStream->m_BrtBookProtectionIso != nullptr)
						m_oWorkbookProtection = workBookStream->m_BrtBookProtectionIso;
					else if (workBookStream->m_BrtBookProtection != nullptr)
						m_oWorkbookProtection = workBookStream->m_BrtBookProtection;

					if (workBookStream->m_EXTERNALS != nullptr)
						m_oExternalReferences = static_cast<XLSB::EXTERNALS*>(workBookStream->m_EXTERNALS.get())->m_arSUP;
					if (workBookStream->m_BrtFileVersion != nullptr)
						m_oAppName = static_cast<XLSB::FileVersion*>(workBookStream->m_BrtFileVersion.get())->stAppName.value();

					if (workBookStream->m_FRTWORKBOOK != nullptr)
						m_oExtLst = workBookStream->m_FRTWORKBOOK;
				}

				//workBookStream.reset();

			}
		}

		void CWorkbook::read(const CPath& oRootPath, const CPath& oPath)
		{
			m_oReadPath = oPath;

			if (m_oReadPath.GetExtention() == _T(".bin"))
			{
				readBin(m_oReadPath);
			}
			else
			{
				XmlUtils::CXmlLiteReader oReader;

				if (!oReader.FromFile(oPath.GetPath()))
					return;

				if (!oReader.ReadNextNode())
					return;

				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (L"workbook" == sName)
				{
					fromXML(oReader);
				}
			}

			IFileContainer::Read(oRootPath, oPath); //в данном случае порядок считывания важен для xlsb

			CXlsx* xlsx = dynamic_cast<CXlsx*>(File::m_pMainDocument);
			if ((xlsx) && (xlsx->m_pVbaProject))
			{
				m_bMacroEnabled = true;
			}
		}

		void CWorkbook::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<workbook xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\">");

			if (m_oWorkbookPr.IsInit())
				m_oWorkbookPr->toXML(writer);
			if (m_oWorkbookProtection.IsInit())
				m_oWorkbookProtection->toXML(writer);
			if (m_oBookViews.IsInit())
				m_oBookViews->toXML(writer);
			if (m_oSheets.IsInit())
				m_oSheets->toXML(writer);
			if (m_oExternalReferences.IsInit())
				m_oExternalReferences->toXML(writer);
			if (m_oDefinedNames.IsInit())
				m_oDefinedNames->toXML(writer);
			if (m_oCalcPr.IsInit())
				m_oCalcPr->toXML(writer);

			if (m_oPivotCachesXml.IsInit())
				writer.WriteString(m_oPivotCachesXml.get());
			else if (m_oPivotCaches.IsInit())
				m_oPivotCaches->toXML(writer);
			if (m_oOleSize.IsInit())
			{
				writer.WriteString(L"<oleSize ref=\"" + *m_oOleSize + L"\"/>");
			}
			if (m_oExtLst.IsInit())
				writer.WriteString(m_oExtLst->toXMLWithNS(L""));
			writer.WriteString(L"</workbook>");
		}
		void CWorkbook::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (!oReader.IsEmptyNode())
			{
				int nDocumentDepth = oReader.GetDepth();
				while (oReader.ReadNextSiblingNode(nDocumentDepth))
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (L"bookViews" == sName)
						m_oBookViews = oReader;
					else if (L"calcPr" == sName)
						m_oCalcPr = oReader;
					else if (L"definedNames" == sName)
						m_oDefinedNames = oReader;
					else if (L"sheets" == sName)
						m_oSheets = oReader;
					else if (L"workbookPr" == sName)
						m_oWorkbookPr = oReader;
					else if (L"workbookProtection" == sName)
						m_oWorkbookProtection = oReader;
					else if (L"externalReferences" == sName)
						m_oExternalReferences = oReader;
					else if (L"pivotCaches" == sName)
						m_oPivotCaches = oReader;
					else if (L"extLst" == sName)
						m_oExtLst = oReader;
					else if (L"oleSize" == sName)
					{
						WritingElement_ReadAttributes_Start(oReader)
							WritingElement_ReadAttributes_Read_if(oReader, L"ref", m_oOleSize)
						WritingElement_ReadAttributes_End(oReader)
					}
					else if (L"fileVersion" == sName)
					{
						WritingElement_ReadAttributes_Start(oReader)
							WritingElement_ReadAttributes_Read_if(oReader, L"appName", m_oAppName)
						WritingElement_ReadAttributes_End(oReader)
					}					
					else if (L"WindowHeight" == sName)
					{
					}
					else if (L"WindowWidth" == sName)
					{
					}
					else if (L"RefModeR1C1" == sName)
					{
						if (!m_oCalcPr.IsInit()) m_oCalcPr.Init();
						m_oCalcPr->m_oRefMode = SimpleTypes::Spreadsheet::refmodeR1C1;
					}
				}
			}
		}
		void CWorkbook::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			NSStringUtils::CStringBuilder sXml;

			sXml.WriteString(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

			toXML(sXml);

			std::wstring sPath = oPath.GetPath();
			NSFile::CFileBinary::SaveToFile(sPath.c_str(), sXml.GetData());

			oContent.Registration(type().OverrideType(), oDirectory, oPath.GetFilename());
			IFileContainer::Write(oPath, oDirectory, oContent);
		}
		void CWorkbook::PrepareToWrite()
		{
			//WorkbookPr
			if (false == m_oWorkbookPr.IsInit())
				m_oWorkbookPr.Init();
			//todo если этот параметр выставлен, то берется стандартная тема, а не из файла.
			//if(false == m_oWorkbookPr->m_oDefaultThemeVersion.IsInit())
			//{
			//	m_oWorkbookPr->m_oDefaultThemeVersion.Init();
			//	m_oWorkbookPr->m_oDefaultThemeVersion->SetValue(124226);
			//}
			//BookViews
			if (false == m_oBookViews.IsInit())
				m_oBookViews.Init();

			if (m_oBookViews->m_arrItems.empty())
				m_oBookViews->m_arrItems.push_back(new OOX::Spreadsheet::CWorkbookView());

			OOX::Spreadsheet::CWorkbookView* pWorkbookView = m_oBookViews->m_arrItems.front();

			if (false == pWorkbookView->m_oXWindow.IsInit())
			{
				pWorkbookView->m_oXWindow.Init();
				pWorkbookView->m_oXWindow->SetValue(360);
			}
			if (false == pWorkbookView->m_oYWindow.IsInit())
			{
				pWorkbookView->m_oYWindow.Init();
				pWorkbookView->m_oYWindow->SetValue(15);
			}
			if (false == pWorkbookView->m_oWindowWidth.IsInit())
			{
				pWorkbookView->m_oWindowWidth.Init();
				pWorkbookView->m_oWindowWidth->SetValue(20955);
			}
			if (false == pWorkbookView->m_oWindowHeight.IsInit())
			{
				pWorkbookView->m_oWindowHeight.Init();
				pWorkbookView->m_oWindowHeight->SetValue(9720);
			}
		}
		LONG CWorkbook::GetActiveSheetIndex()
		{
			LONG lActiveSheet = 0;
			std::wstring sSheetRId = L"Sheet1"; // Читаем не по rId, а по имени листа
												// Get active sheet
			if (m_oBookViews.IsInit() && !m_oBookViews->m_arrItems.empty())
			{
				if (m_oBookViews->m_arrItems.front()->m_oActiveTab.IsInit())
				{
					lActiveSheet = m_oBookViews->m_arrItems.front()->m_oActiveTab->GetValue();
					if (0 > lActiveSheet)
						lActiveSheet = 0;
				}
			}
			return lActiveSheet;
		}
	} //Spreadsheet
} // namespace OOX

