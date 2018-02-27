/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#ifndef PPTX_CORE_FILE_INCLUDE_H_
#define PPTX_CORE_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "../../Common/DocxFormat/Source/DocxFormat/WritingElement.h"

#include "Limit/ContentStatus.h"
#include "FileTypes.h"

using namespace NSBinPptxRW;

namespace PPTX
{
	class Core : public WrapperFile
	{
	public:
		Core(OOX::Document* pMain) : WrapperFile(pMain)
		{
		}
		Core(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
		{
			read(filename, map);
		}
		virtual ~Core()
		{
		}

	public:
		virtual void read(const OOX::CPath& filename, FileMap& map)
		{
			XmlUtils::CXmlNode oNode;
			oNode.FromXmlFile(filename.m_strFilename);

			oNode.ReadNodeValueBase(_T("dc:title"), title);
			oNode.ReadNodeValueBase(_T("dc:creator"), creator);
			oNode.ReadNodeValueBase(_T("cp:lastModifiedBy"), lastModifiedBy);
			oNode.ReadNodeValueBase(_T("cp:revision"), revision);
			oNode.ReadNodeValueBase(_T("dcterms:modified"), modified);
			oNode.ReadNodeValueBase(_T("dcterms:created"), created);

	//		created = PPTX::DateTime::Parse(document.Root.element("created").text().ToString());
	//			modified = PPTX::DateTime::Parse(document.Root.element("modified").text().ToString());
	/*
			category = document.Root.element("category").text();
			contentStatus = document.Root.element("contentStatus").text();
			description = document.Root.element("description").text();
	//identifier - ???
	//			keywords = document.Root.element("keywords").text();
			language = document.Root.element("language").text();
	//			lastPrinted = PPTX::DateTime::Parse(document.Root.element("lastPrinted").text().ToString());
			subject = document.Root.element("subject").text();
			version = document.Root.element("version").text();
	*/
		}
		virtual void write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
		{
			WrapperFile::write(filename, directory, content);
		}

	public:
		virtual const OOX::FileType type() const
		{
			return OOX::FileTypes::Core;
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
			pWriter->StartRecord(NSMainTables::Core);

			pWriter->WriteBYTE(g_nodeAttributeStart);

			pWriter->WriteString2(0, title);
			pWriter->WriteString2(1, creator);
			pWriter->WriteString2(2, lastModifiedBy);
			pWriter->WriteString2(3, revision);
			pWriter->WriteString2(4, created);
			pWriter->WriteString2(5, modified);

			pWriter->WriteBYTE(g_nodeAttributeEnd);

			pWriter->EndRecord();
		}
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("cp:coreProperties"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns:cp"), PPTX::g_Namespaces.cp.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dc"), PPTX::g_Namespaces.dc.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dcterms"), PPTX::g_Namespaces.dcterms.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:xsi"), PPTX::g_Namespaces.xsi.m_strLink);

			pWriter->EndAttributes();

			pWriter->WriteNodeValue(_T("dc:title"), title);
			pWriter->WriteNodeValue(_T("dc:creator"), creator);
			pWriter->WriteNodeValue(_T("cp:lastModifiedBy"), lastModifiedBy);
			pWriter->WriteNodeValue(_T("cp:revision"), revision);

			pWriter->WriteNodeValue(_T("dcterms:created xsi:type=\"dcterms:W3CDTF\""), created);
			pWriter->WriteNodeValue(_T("dcterms:modified xsi:type=\"dcterms:W3CDTF\""), modified);
			
			pWriter->EndNode(_T("cp:coreProperties"));
		}

	public:
		nullable_string		title;
		nullable_string		creator;
		nullable_string		lastModifiedBy;
		nullable_string		revision;
//		nullable_property<PPTX::DateTime> created;
		nullable_string		created;
//		nullable_property<PPTX::DateTime> modified;
		nullable_string		modified;
/*
		nullable_property<std::string> category;
		nullable_property<std::string, Limit::ContentStatus> contentStatus;
		nullable_property<std::string> description;
//identifier - ???
//			nullable_property<std::string> keywords;
		nullable_property<std::string> language;
		nullable_property<PPTX::DateTime> lastPrinted;
		nullable_property<std::string> subject;
		nullable_property<std::string> version;
*/
	};
} // namespace PPTX

#endif // PPTX_CORE_FILE_INCLUDE_H_