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

#include "App.h"

namespace PPTX
{
	App::App(OOX::Document* pMain) : WrapperFile(pMain)
	{
	}
	App::App(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
	{
		read(filename, map);
	}
	App::~App()
	{
	}
	void App::read(const OOX::CPath& filename, FileMap& map)
	{
		HeadingPairs.clear();
		TitlesOfParts.clear();

		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlMacroReadNodeValueBase(oNode, L"Template", Template);
		XmlMacroReadNodeValueBase(oNode, L"TotalTime", TotalTime);
		XmlMacroReadNodeValueBase(oNode, L"Pages", Pages);
		XmlMacroReadNodeValueBase(oNode, L"Words", Words);
		XmlMacroReadNodeValueBase(oNode, L"Characters", Characters);
		XmlMacroReadNodeValueBase(oNode, L"Application", Application);
		XmlMacroReadNodeValueBase(oNode, L"DocSecurity", DocSecurity);
		XmlMacroReadNodeValueBase(oNode, L"Lines", Lines);
		XmlMacroReadNodeValueBase(oNode, L"PresentationFormat", PresentationFormat);
		XmlMacroReadNodeValueBase(oNode, L"Paragraphs", Paragraphs);
		XmlMacroReadNodeValueBase(oNode, L"Slides", Slides);
		XmlMacroReadNodeValueBase(oNode, L"Notes", Notes);
		XmlMacroReadNodeValueBase(oNode, L"HiddenSlides", HiddenSlides);
		XmlMacroReadNodeValueBase(oNode, L"MMClips", MMClips);
		XmlMacroReadNodeValueBase(oNode, L"ScaleCrop", ScaleCrop);

		XmlUtils::CXmlNode oHP = oNode.ReadNode(_T("HeadingPairs"));
		XmlUtils::CXmlNode oNodeVector1;
		if (oHP.GetNode(_T("vt:vector"), oNodeVector1))
		{
			XmlMacroReadAttributeBase(oNodeVector1, _T("size"), m_Headings);
			XmlMacroLoadArray(oNodeVector1, _T("vt:variant"), HeadingPairs, Logic::HeadingVariant);
		}

		XmlUtils::CXmlNode oTP = oNode.ReadNode(_T("TitlesOfParts"));
		XmlUtils::CXmlNode oNodeVector2;
		if (oTP.GetNode(_T("vt:vector"), oNodeVector2))
		{
			XmlMacroReadAttributeBase(oNodeVector2, _T("size"), m_VectorSize);
			XmlMacroLoadArray(oNodeVector2, _T("vt:variant"), TitlesOfParts, Logic::PartTitle);
		}

		XmlMacroReadNodeValueBase(oNode, L"Manager", Manager);
		XmlMacroReadNodeValueBase(oNode, L"Company", Company);
		XmlMacroReadNodeValueBase(oNode, L"LinksUpToDate", LinksUpToDate);
		XmlMacroReadNodeValueBase(oNode, L"CharactersWithSpaces", CharactersWithSpaces);
		XmlMacroReadNodeValueBase(oNode, L"SharedDoc", SharedDoc);
		XmlMacroReadNodeValueBase(oNode, L"HyperlinkBase", HyperlinkBase);
		XmlMacroReadNodeValueBase(oNode, L"HyperlinksChanged", HyperlinksChanged);
		XmlMacroReadNodeValueBase(oNode, L"AppVersion", AppVersion);
		//DigSig (Digital Signature)
		//HLinks

		Normalize();
	}
	void App::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
	}
	const OOX::FileType App::type() const
	{
		return OOX::FileTypes::App;
	}
	const OOX::CPath App::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath App::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void App::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->StartRecord(NSBinPptxRW::NSMainTables::App);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteString2(0, Template);
		pWriter->WriteString2(1, Application);
		pWriter->WriteString2(2, PresentationFormat);
		pWriter->WriteString2(3, Company);
		pWriter->WriteString2(4, AppVersion);

		pWriter->WriteInt2(5, TotalTime);
		pWriter->WriteInt2(6, Words);
		pWriter->WriteInt2(7, Paragraphs);
		pWriter->WriteInt2(8, Slides);
		pWriter->WriteInt2(9, Notes);
		pWriter->WriteInt2(10, HiddenSlides);
		pWriter->WriteInt2(11, MMClips);

		pWriter->WriteBool2(12, ScaleCrop);
		pWriter->WriteBool2(13, LinksUpToDate);
		pWriter->WriteBool2(14, SharedDoc);
		pWriter->WriteBool2(15, HyperlinksChanged);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

		//start new record because new attributes is incompatible with previous versions
		pWriter->StartRecord(0);
		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

		pWriter->WriteInt2(16, Characters);
		pWriter->WriteInt2(17, CharactersWithSpaces);
		pWriter->WriteInt2(18, DocSecurity);
		pWriter->WriteString2(19, HyperlinkBase);
		pWriter->WriteInt2(20, Lines);
		pWriter->WriteString2(21, Manager);
		pWriter->WriteInt2(22, Pages);

		pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		pWriter->EndRecord();

		pWriter->EndRecord();
	}
	void App::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
			case 0: Template = pReader->GetString2(); break;
			case 1: Application = pReader->GetString2(); break;
			case 2: PresentationFormat = pReader->GetString2(); break;
			case 3: Company = pReader->GetString2(); break;
			case 4: AppVersion = pReader->GetString2(); break;
			case 5: TotalTime = pReader->GetULong(); break;
			case 6: Words = pReader->GetULong(); break;
			case 7: Paragraphs = pReader->GetULong(); break;
			case 8: Slides = pReader->GetULong(); break;
			case 9: Notes = pReader->GetULong(); break;
			case 10: HiddenSlides = pReader->GetULong(); break;
			case 11: MMClips = pReader->GetULong(); break;
			case 12: ScaleCrop = pReader->GetBool(); break;
			case 13: LinksUpToDate = pReader->GetBool(); break;
			case 14: SharedDoc = pReader->GetBool(); break;
			case 15: HyperlinksChanged = pReader->GetBool(); break;
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
						case 16: Characters = pReader->GetULong(); break;
						case 17: CharactersWithSpaces = pReader->GetULong(); break;
						case 18: DocSecurity = pReader->GetULong(); break;
						case 19: HyperlinkBase = pReader->GetString2(); break;
						case 20: Lines = pReader->GetULong(); break;
						case 21: Manager = pReader->GetString2(); break;
						case 22: Pages = pReader->GetULong(); break;
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
	void App::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("Properties"));

		pWriter->StartAttributes();

		pWriter->WriteAttribute(_T("xmlns"), PPTX::g_Namespaces.xmlns.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:vt"), PPTX::g_Namespaces.vt.m_strLink);

		pWriter->EndAttributes();

		pWriter->WriteNodeValue2(_T("Template"), Template);
		pWriter->WriteNodeValue(_T("TotalTime"), TotalTime);
		pWriter->WriteNodeValue(_T("Pages"), Pages);
		pWriter->WriteNodeValue(_T("Words"), Words);
		pWriter->WriteNodeValue(_T("Characters"), Characters);
		pWriter->WriteNodeValue(_T("CharactersWithSpaces"), CharactersWithSpaces);
		pWriter->WriteNodeValue2(_T("Application"), Application);
		pWriter->WriteNodeValue(_T("DocSecurity"), DocSecurity);
		pWriter->WriteNodeValue2(_T("PresentationFormat"), PresentationFormat);
		pWriter->WriteNodeValue(_T("Lines"), Lines);
		pWriter->WriteNodeValue(_T("Paragraphs"), Paragraphs);
		pWriter->WriteNodeValue(_T("Slides"), Slides);
		pWriter->WriteNodeValue(_T("Notes"), Notes);
		pWriter->WriteNodeValue(_T("HiddenSlides"), HiddenSlides);
		pWriter->WriteNodeValue(_T("MMClips"), MMClips);
		pWriter->WriteNodeValue(_T("ScaleCrop"), ScaleCrop);

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

		pWriter->WriteNodeValue2(_T("Manager"), Manager);
		pWriter->WriteNodeValue2(_T("Company"), Company);
		pWriter->WriteNodeValue(_T("LinksUpToDate"), LinksUpToDate);
		pWriter->WriteNodeValue(_T("SharedDoc"), SharedDoc);
		pWriter->WriteNodeValue2(_T("HyperlinkBase"), HyperlinkBase);
		pWriter->WriteNodeValue(_T("HyperlinksChanged"), HyperlinksChanged);
		pWriter->WriteNodeValue2(_T("AppVersion"), AppVersion);

		pWriter->EndNode(_T("Properties"));
	}
	void App::Normalize()
	{
		TotalTime.normalize_positive();
		Words.normalize_positive();
		Paragraphs.normalize_positive();
		Slides.normalize_positive();
		Notes.normalize_positive();
		HiddenSlides.normalize_positive();
		MMClips.normalize_positive();
		m_VectorSize.normalize_positive();
		m_Headings.normalize_positive();
		Characters.normalize_positive();
		CharactersWithSpaces.normalize_positive();
		DocSecurity.normalize_positive();
		Lines.normalize_positive();
		Pages.normalize_positive();
	}
} // namespace PPTX
