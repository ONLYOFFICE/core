/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

		if ( _T("Properties") == oProperties.GetName() )
		{
			XmlUtils::CXmlNode oItem;

			if ( oProperties.GetNode( _T("Application"), oItem ) )
				m_sApplication = oItem.GetText();

			if ( oProperties.GetNode( _T("AppVersion"), oItem ) )
				m_sAppVersion = oItem.GetText();

			if ( oProperties.GetNode( _T("Characters"), oItem ) )
				m_nCharacters = oItem.GetText();

			if ( oProperties.GetNode( _T("CharactersWithSpaces"), oItem ) )
				m_nCharactersWithSpaces = oItem.GetText();

			if ( oProperties.GetNode( _T("Company"), oItem ) )
				m_sCompany = oItem.GetText();

			if ( oProperties.GetNode( _T("DocSecurity"), oItem ) )
				m_nDocSecurity = oItem.GetText();

			if ( oProperties.GetNode( _T("HiddenSlides"), oItem ) )
				m_nHiddenSlides = oItem.GetText();

			if ( oProperties.GetNode( _T("HyperlinkBase"), oItem ) )
				m_sHyperlinkBase = oItem.GetText();

			if ( oProperties.GetNode( _T("HyperlinksChanged"), oItem ) )
				m_bHyperlinksChanged = oItem.GetText();

			if ( oProperties.GetNode( _T("Lines"), oItem ) )
				m_nLines = oItem.GetText();

			if ( oProperties.GetNode( _T("LinksUpToDate"), oItem ) )
				m_bLinksUpToDate = oItem.GetText();

			if ( oProperties.GetNode( _T("Manager"), oItem ) )
				m_sManager = oItem.GetText();

			if ( oProperties.GetNode( _T("MMClips"), oItem ) )
				m_nMMClips = oItem.GetText();

			if ( oProperties.GetNode( _T("Notes"), oItem ) )
				m_nNotes = oItem.GetText();

			if ( oProperties.GetNode( _T("Pages"), oItem ) )
				m_nPages = oItem.GetText();

			if ( oProperties.GetNode( _T("Paragraphs"), oItem ) )
				m_nParagraphs = oItem.GetText();

			if ( oProperties.GetNode( _T("ScaleCrop"), oItem ) )
				m_bScaleCrop = oItem.GetText();

			if ( oProperties.GetNode( _T("SharedDoc"), oItem ) )
				m_bSharedDoc = oItem.GetText();

			if ( oProperties.GetNode( _T("Slides"), oItem ) )
				m_nSlides = oItem.GetText();

			if ( oProperties.GetNode( _T("Template"), oItem ) )
				m_sTemplate = oItem.GetText();

			if ( oProperties.GetNode( _T("TotalTime"), oItem ) )
				m_nTotalTime = oItem.GetText();

			if ( oProperties.GetNode( _T("Words"), oItem ) )
				m_nWords = oItem.GetText();
			
			XmlUtils::CXmlNode oHP = oProperties.ReadNode(_T("HeadingPairs"));
			XmlUtils::CXmlNode oNodeVector1;
			if (oHP.GetNode(_T("vt:vector"), oNodeVector1))
			{
				XmlMacroReadAttributeBase(oNodeVector1, _T("size"), m_Headings);
				XmlMacroLoadArray(oNodeVector1, _T("vt:variant"), HeadingPairs, PPTX::Logic::HeadingVariant);
			}

			XmlUtils::CXmlNode oTP = oProperties.ReadNode(_T("TitlesOfParts"));
			XmlUtils::CXmlNode oNodeVector2;
			if (oTP.GetNode(_T("vt:vector"), oNodeVector2))
			{
				XmlMacroReadAttributeBase(oNodeVector2, _T("size"), m_VectorSize);
				XmlMacroLoadArray(oNodeVector2, _T("vt:variant"), TitlesOfParts, PPTX::Logic::PartTitle);
			}
		}
	}
	void CApp::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>");

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
		pWriter->StartNode(_T("Properties"));

		pWriter->StartAttributes();

		pWriter->WriteAttribute(_T("xmlns"), PPTX::g_Namespaces.xmlns.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:vt"), PPTX::g_Namespaces.vt.m_strLink);

		pWriter->EndAttributes();

		pWriter->WriteNodeValue2(_T("Template"), m_sTemplate);
		pWriter->WriteNodeValue(_T("TotalTime"), m_nTotalTime);
		pWriter->WriteNodeValue(_T("Pages"), m_nPages);
		pWriter->WriteNodeValue(_T("Words"), m_nWords);
		pWriter->WriteNodeValue(_T("Characters"), m_nCharacters);
		pWriter->WriteNodeValue(_T("CharactersWithSpaces"), m_nCharactersWithSpaces);
		pWriter->WriteNodeValue2(_T("Application"), m_sApplication);
		pWriter->WriteNodeValue(_T("DocSecurity"), m_nDocSecurity);
		pWriter->WriteNodeValue2(_T("PresentationFormat"), m_sPresentationForm);
		pWriter->WriteNodeValue(_T("Lines"), m_nLines);
		pWriter->WriteNodeValue(_T("Paragraphs"), m_nParagraphs);
		pWriter->WriteNodeValue(_T("Slides"), m_nSlides);
		pWriter->WriteNodeValue(_T("Notes"), m_nNotes);
		pWriter->WriteNodeValue(_T("HiddenSlides"), m_nHiddenSlides);
		pWriter->WriteNodeValue(_T("MMClips"), m_nMMClips);
		pWriter->WriteNodeValue(_T("ScaleCrop"), m_bScaleCrop);

		pWriter->StartNode(_T("HeadingPairs"));
		pWriter->EndAttributes();

		pWriter->StartNode(_T("vt:vector"));
		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("size"), (int)HeadingPairs.size());
		pWriter->WriteAttribute(_T("baseType"), (std::wstring)_T("variant"));
		pWriter->EndAttributes();

		pWriter->WriteArray2(HeadingPairs);

		pWriter->EndNode(_T("vt:vector"));
		pWriter->EndNode(_T("HeadingPairs"));

		pWriter->StartNode(_T("TitlesOfParts"));
		pWriter->EndAttributes();

		pWriter->StartNode(_T("vt:vector"));
		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("size"), (int)TitlesOfParts.size());
		pWriter->WriteAttribute(_T("baseType"), (std::wstring)_T("lpstr"));
		pWriter->EndAttributes();

		pWriter->WriteArray2(TitlesOfParts);

		pWriter->EndNode(_T("vt:vector"));
		pWriter->EndNode(_T("TitlesOfParts"));

		pWriter->WriteNodeValue2(_T("Manager"), m_sManager);
		pWriter->WriteNodeValue2(_T("Company"), m_sCompany);
		pWriter->WriteNodeValue(_T("LinksUpToDate"), m_bLinksUpToDate);
		pWriter->WriteNodeValue(_T("SharedDoc"), m_bSharedDoc);
		pWriter->WriteNodeValue2(_T("HyperlinkBase"), m_sHyperlinkBase);
		pWriter->WriteNodeValue(_T("HyperlinksChanged"), m_bHyperlinksChanged);
		pWriter->WriteNodeValue2(_T("AppVersion"), m_sAppVersion);

		pWriter->EndNode(_T("Properties"));
	}
} // namespace OOX
