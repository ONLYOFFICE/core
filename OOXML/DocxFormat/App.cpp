/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "App.h"
#include "Docx.h"
#include "../XlsxFormat/Xlsx.h"
#include "../Common/SimpleTypes_Word.h"
#include "../../DesktopEditor/common/SystemUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

namespace OOX
{
	CApp::CApp(OOX::Document* pMain) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pApp = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pApp = this;
	}
	CApp::CApp(OOX::Document* pMain, const CPath& oPath) : OOX::File(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if (docx) docx->m_pApp = this;
		OOX::Spreadsheet::CXlsx* xlsx = dynamic_cast<OOX::Spreadsheet::CXlsx*>(File::m_pMainDocument);
		if (xlsx) xlsx->m_pApp = this;

		read( oPath );
	}
	CApp::~CApp()
	{
	}
	void CApp::read(const CPath& oPath)
	{
		XmlUtils::CXmlNode oProperties;
		oProperties.FromXmlFile( oPath.GetPath(), true );

		if ( L"Properties" == XmlUtils::GetNameNoNS(oProperties.GetName()))
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (oProperties.GetNodes(_T("*"), oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oItem = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oItem.GetName());
					
					if (L"Application" == strName)
						m_sApplication = oItem.GetText();

					else if (L"AppVersion" == strName)
						m_sAppVersion = oItem.GetText();

					else if (L"Characters" == strName)
						m_nCharacters = oItem.GetText();

					else if (L"CharactersWithSpaces" == strName)
						m_nCharactersWithSpaces = oItem.GetText();

					else if (L"Company" == strName)
						m_sCompany = oItem.GetText();

					else if (L"DocSecurity" == strName)
						m_nDocSecurity = oItem.GetText();

					else if (L"HiddenSlides" == strName)
						m_nHiddenSlides = oItem.GetText();

					else if (L"HyperlinkBase" == strName)
						m_sHyperlinkBase = oItem.GetText();

					else if (L"HyperlinksChanged" == strName)
						m_bHyperlinksChanged = oItem.GetText();

					else if (L"Lines" == strName)
						m_nLines = oItem.GetText();

					else if (L"LinksUpToDate" == strName)
						m_bLinksUpToDate = oItem.GetText();

					else if (L"Manager" == strName)
						m_sManager = oItem.GetText();

					else if (L"MMClips" == strName)
						m_nMMClips = oItem.GetText();

					else if (L"Notes" == strName)
						m_nNotes = oItem.GetText();

					else if (L"Pages" == strName)
						m_nPages = oItem.GetText();

					else if (L"Paragraphs" == strName)
						m_nParagraphs = oItem.GetText();

					else if (L"ScaleCrop" == strName)
						m_bScaleCrop = oItem.GetText();

					else if (L"SharedDoc" == strName)
						m_bSharedDoc = oItem.GetText();

					else if (L"Slides" == strName)
						m_nSlides = oItem.GetText();

					else if (L"Template" == strName)
						m_sTemplate = oItem.GetText();

					else if (L"TotalTime" == strName)
						m_nTotalTime = oItem.GetText();

					else if (L"Words" == strName)
						m_nWords = oItem.GetText();

					else if (L"HeadingPairs" == strName)
					{
						XmlUtils::CXmlNode oNodeVector1;
						if (oItem.GetNode(L"vt:vector", oNodeVector1))
						{
							XmlMacroReadAttributeBase(oNodeVector1, L"size", m_Headings);
							XmlMacroLoadArray(oNodeVector1, L"vt:variant", HeadingPairs, PPTX::Logic::HeadingVariant);
						}
					}
					else if (L"TitlesOfParts" == strName)
					{
						XmlUtils::CXmlNode oNodeVector2;
						if (oItem.GetNode(L"vt:vector", oNodeVector2))
						{
							XmlMacroReadAttributeBase(oNodeVector2, L"size", m_VectorSize);
							XmlMacroLoadArray(oNodeVector2, L"vt:variant", TitlesOfParts, PPTX::Logic::PartTitle);
						}

					}
				}
			}
		}
	}
	void CApp::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";

		NSBinPptxRW::CXmlWriter oWriter;
		toXmlWriter(&oWriter);

		sXml += oWriter.GetXmlString();

		NSFile::CFileBinary::SaveToFile( oPath.GetPath(), sXml );
		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
	}
	const OOX::FileType CApp::type() const
	{
		return FileTypes::App;
	}
	const CPath CApp::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CApp::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void CApp::SetDefaults()
	{
		SetRequiredDefaults();

		m_nDocSecurity = 0;
		m_bScaleCrop = false;
		m_bLinksUpToDate = false;
		m_bSharedDoc = false;
		m_bHyperlinksChanged = false;
	}
	void CApp::SetRequiredDefaults()
	{
		m_sAppVersion.reset(NULL);
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
		std::string s = VALUE2STR(INTVER);
		sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif
		m_sApplication = sApplication;
	}

	void CApp::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::App);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteString2(0, m_sTemplate);
		pWriter->WriteString2(1, m_sApplication);
		pWriter->WriteString2(2, m_sPresentationForm);
		pWriter->WriteString2(3, m_sCompany);
		pWriter->WriteString2(4, m_sAppVersion);

		pWriter->WriteInt2(5, m_nTotalTime);
		pWriter->WriteInt2(6, m_nWords);
		pWriter->WriteInt2(7, m_nParagraphs);
		pWriter->WriteInt2(8, m_nSlides);
		pWriter->WriteInt2(9, m_nNotes);
		pWriter->WriteInt2(10, m_nHiddenSlides);
		pWriter->WriteInt2(11, m_nMMClips);

		pWriter->WriteBool2(12, m_bScaleCrop);
		pWriter->WriteBool2(13, m_bLinksUpToDate);
		pWriter->WriteBool2(14, m_bSharedDoc);
		pWriter->WriteBool2(15, m_bHyperlinksChanged);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		//start new record because new attributes is incompatible with previous versions
		pWriter->StartRecord(0);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteInt2(16, m_nCharacters);
		pWriter->WriteInt2(17, m_nCharactersWithSpaces);
		pWriter->WriteInt2(18, m_nDocSecurity);
		pWriter->WriteString2(19, m_sHyperlinkBase);
		pWriter->WriteInt2(20, m_nLines);
		pWriter->WriteString2(21, m_sManager);
		pWriter->WriteInt2(22, m_nPages);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		pWriter->EndRecord();

		pWriter->EndRecord();
	}
	void CApp::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		pReader->Skip(1); // type
		LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

		pReader->Skip(1); // start attributes

		while (true)
		{
			BYTE _at = pReader->GetUChar_TypeNode();
			if (_at == NSBinPptxRW::g_nodeAttributeEnd)
				break;

			switch (_at)
			{
			case 0: m_sTemplate = pReader->GetString2(); break;
			case 1: m_sApplication = pReader->GetString2(); break;
			case 2: m_sPresentationForm = pReader->GetString2(); break;
			case 3: m_sCompany = pReader->GetString2(); break;
			case 4: m_sAppVersion = pReader->GetString2(); break;
			case 5: m_nTotalTime = pReader->GetULong(); break;
			case 6: m_nWords = pReader->GetULong(); break;
			case 7: m_nParagraphs = pReader->GetULong(); break;
			case 8: m_nSlides = pReader->GetULong(); break;
			case 9: m_nNotes = pReader->GetULong(); break;
			case 10: m_nHiddenSlides = pReader->GetULong(); break;
			case 11: m_nMMClips = pReader->GetULong(); break;
			case 12: m_bScaleCrop = pReader->GetBool(); break;
			case 13: m_bLinksUpToDate = pReader->GetBool(); break;
			case 14: m_bSharedDoc = pReader->GetBool(); break;
			case 15: m_bHyperlinksChanged = pReader->GetBool(); break;
			default: break;
			}
		}
		while (pReader->GetPos() < _end_rec)
		{
			BYTE _at = pReader->GetUChar();
			switch (_at)
			{
			case 0:
			{
				LONG _end_rec2 = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					switch (_at)
					{
					case 16: m_nCharacters = pReader->GetULong(); break;
					case 17: m_nCharactersWithSpaces = pReader->GetULong(); break;
					case 18: m_nDocSecurity = pReader->GetULong(); break;
					case 19: m_sHyperlinkBase = pReader->GetString2(); break;
					case 20: m_nLines = pReader->GetULong(); break;
					case 21: m_sManager = pReader->GetString2(); break;
					case 22: m_nPages = pReader->GetULong(); break;
					default: break;
					}
				}

				pReader->Seek(_end_rec2);
			}
			break;
			default:
			{
				pReader->SkipRecord();
				break;
			}
			}
		}

		pReader->Seek(_end_rec);
	}
	void CApp::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(L"Properties");

		pWriter->StartAttributes();

		pWriter->WriteAttribute(L"xmlns", PPTX::g_Namespaces.xmlns.m_strLink);
		pWriter->WriteAttribute(L"xmlns:vt", PPTX::g_Namespaces.vt.m_strLink);

		pWriter->EndAttributes();

		pWriter->WriteNodeValue2(L"Template", m_sTemplate);
		pWriter->WriteNodeValue(L"TotalTime", m_nTotalTime);
		pWriter->WriteNodeValue(L"Pages", m_nPages);
		pWriter->WriteNodeValue(L"Words", m_nWords);
		pWriter->WriteNodeValue(L"Characters", m_nCharacters);
		pWriter->WriteNodeValue(L"CharactersWithSpaces", m_nCharactersWithSpaces);
		pWriter->WriteNodeValue2(L"Application", m_sApplication);
		pWriter->WriteNodeValue(L"DocSecurity", m_nDocSecurity);
		pWriter->WriteNodeValue2(L"PresentationFormat", m_sPresentationForm);
		pWriter->WriteNodeValue(L"Lines", m_nLines);
		pWriter->WriteNodeValue(L"Paragraphs", m_nParagraphs);
		pWriter->WriteNodeValue(L"Slides", m_nSlides);
		pWriter->WriteNodeValue(L"Notes", m_nNotes);
		pWriter->WriteNodeValue(L"HiddenSlides", m_nHiddenSlides);
		pWriter->WriteNodeValue(L"MMClips", m_nMMClips);
		pWriter->WriteNodeValue(L"ScaleCrop", m_bScaleCrop);

		pWriter->StartNode(L"HeadingPairs");
		pWriter->EndAttributes();

		pWriter->StartNode(L"vt:vector");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"size", (int)HeadingPairs.size());
		pWriter->WriteAttribute(L"baseType", (std::wstring)(L"variant"));
		pWriter->EndAttributes();

		pWriter->WriteArray2(HeadingPairs);

		pWriter->EndNode(L"vt:vector");
		pWriter->EndNode(L"HeadingPairs");

		pWriter->StartNode(L"TitlesOfParts");
		pWriter->EndAttributes();

		pWriter->StartNode(L"vt:vector");
		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"size", (int)TitlesOfParts.size());
		pWriter->WriteAttribute(L"baseType", (std::wstring)(L"lpstr"));
		pWriter->EndAttributes();

		pWriter->WriteArray2(TitlesOfParts);

		pWriter->EndNode(L"vt:vector");
		pWriter->EndNode(L"TitlesOfParts");

		pWriter->WriteNodeValue2(L"Manager", m_sManager);
		pWriter->WriteNodeValue2(L"Company", m_sCompany);
		pWriter->WriteNodeValue(L"LinksUpToDate", m_bLinksUpToDate);
		pWriter->WriteNodeValue(L"SharedDoc", m_bSharedDoc);
		pWriter->WriteNodeValue2(L"HyperlinkBase", m_sHyperlinkBase);
		pWriter->WriteNodeValue(L"HyperlinksChanged", m_bHyperlinksChanged);
		pWriter->WriteNodeValue2(L"AppVersion", m_sAppVersion);

		pWriter->EndNode(L"Properties");
	}
} // namespace OOX
