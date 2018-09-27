/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#ifndef PPTX_APP_FILE_INCLUDE_H_
#define PPTX_APP_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "FileTypes.h"

#include "Logic/PartTitle.h"
#include "Logic/HeadingVariant.h"

namespace PPTX
{
	class App : public WrapperFile
	{
	public:
		App(OOX::Document* pMain) : WrapperFile(pMain)
		{
		}
		App(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
		{
			read(filename, map);
		}
		virtual ~App()
		{
		}

		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			HeadingPairs.clear();
			TitlesOfParts.clear();

			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

            XmlMacroReadNodeValueBase(oNode, L"Template", Template);
            XmlMacroReadNodeValueBase(oNode, L"TotalTime", TotalTime);
            XmlMacroReadNodeValueBase(oNode, L"Words", Words);
            XmlMacroReadNodeValueBase(oNode, L"Application", Application);
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

            XmlMacroReadAttributeBase(oNode, L"Company", Company);
            XmlMacroReadAttributeBase(oNode, L"LinksUpToDate", LinksUpToDate);
            XmlMacroReadAttributeBase(oNode, L"SharedDoc", SharedDoc);
            XmlMacroReadAttributeBase(oNode, L"HyperlinksChanged", HyperlinksChanged);
            XmlMacroReadAttributeBase(oNode, L"AppVersion", AppVersion);
			
			//Characters = document.Root.element("Characters").text();
			//CharactersWithSpaces = document.Root.element("CharactersWithSpaces").text();
			//DigSig (Digital Signature)
			//DocSecurity = document.Root.element("DocSecurity").text();
			//HLinks
			//HyperlinkBase = document.Root.element("HyperlinkBase").text();
			//Lines = document.Root.element("Lines").text();
			//Manager = document.Root.element("Manager").text();
			//Pages = document.Root.element("Pages").text();

			Normalize();
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
		}
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::App;
		}
		virtual const OOX::CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const OOX::CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}

		virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
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

			pWriter->EndRecord();
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("Properties"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns"), PPTX::g_Namespaces.xmlns.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:vt"), PPTX::g_Namespaces.vt.m_strLink);

			pWriter->EndAttributes();

			pWriter->WriteNodeValue(_T("Template"), Template);
			pWriter->WriteNodeValue(_T("TotalTime"), TotalTime);
			pWriter->WriteNodeValue(_T("Words"), Words);
			pWriter->WriteNodeValue(_T("Application"), Application);
			pWriter->WriteNodeValue(_T("PresentationFormat"), PresentationFormat);
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

			pWriter->WriteNodeValue(_T("Company"), Company);
			pWriter->WriteNodeValue(_T("LinksUpToDate"), LinksUpToDate);
			pWriter->WriteNodeValue(_T("SharedDoc"), SharedDoc);
			pWriter->WriteNodeValue(_T("HyperlinksChanged"), HyperlinksChanged);
			pWriter->WriteNodeValue(_T("AppVersion"), AppVersion);

			pWriter->EndNode(_T("Properties"));
		}


		nullable_string						Template;				// (Name of Document Template)
		nullable_int						TotalTime;				// (Total Edit Time Metadata Element)
		nullable_int						Words;					// (Word Count)
		nullable_string						Application;			// (Application Name)
		nullable_string						PresentationFormat;		// (Intended Format of Presentation)
		nullable_int						Paragraphs;				// (Total Number of Paragraphs)
		nullable_int						Slides;					// (Slides Metadata Element)
		nullable_int						Notes;					// (Number of Slides Containing Notes)
		nullable_int						HiddenSlides;			// (Number of Hidden Slides)
		nullable_int						MMClips;				// (Total Number of Multimedia Clips)
		nullable_bool						ScaleCrop;				// (Thumbnail Display Mode)
		std::vector<Logic::HeadingVariant>	HeadingPairs;			// (Heading Pairs)
		std::vector<Logic::PartTitle>		TitlesOfParts;			// (Part Titles)
		nullable_string						Company;				// (Name of Company)
		nullable_bool						LinksUpToDate;			// (Links Up-to-Date)
		nullable_bool						SharedDoc;				// (Shared Document)
		nullable_bool						HyperlinksChanged;		// (Hyperlinks Changed)
		nullable_string						AppVersion;				// (Application Version)

		//nullable_property<int, setter::only_positive<int> > Characters;			// (Total Number of Characters)
		//nullable_property<int, setter::only_positive<int> > CharactersWithSpaces;	// (Number of Characters (With Spaces))
		//DigSig (Digital Signature)
		//nullable_property<int, setter::only_positive<int> > DocSecurity;			// (Document Security)
		//std::list<std::string> - ??? HLinks;										// (Hyperlink List)
		//nullable_property<std::string> HyperlinkBase;								// (Relative Hyperlink Base)
		//nullable_property<int, setter::only_positive<int> > Lines;				// (Number of Lines)
		//nullable_property<std::string> Manager;									// (Name of Manager)
		//nullable_property<int, setter::only_positive<int> > Pages;				// (Total Number of Pages)
	private:
		nullable_int						m_VectorSize;
		nullable_int						m_Headings;

		AVSINLINE void Normalize()
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
		}		
	};
} // namespace PPTX

#endif // PPTX_APP_FILE_INCLUDE_H_
