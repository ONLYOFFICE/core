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
#ifndef PPTX_CORE_FILE_INCLUDE_H_
#define PPTX_CORE_FILE_INCLUDE_H_

#include "WrapperFile.h"
#include "../../Common/DocxFormat/Source/DocxFormat/WritingElement.h"

#include "Limit/ContentStatus.h"
#include "FileTypes.h"

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

			XmlMacroReadNodeValueBase(oNode, _T("cp:category"), category);
			XmlMacroReadNodeValueBase(oNode, _T("cp:contentStatus"), contentStatus);
			XmlMacroReadNodeValueBase(oNode, _T("dcterms:created"), created);
			//created = PPTX::DateTime::Parse(document.Root.element("created").text().ToString());
			XmlMacroReadNodeValueBase(oNode, _T("dc:creator"), creator);
			XmlMacroReadNodeValueBase(oNode, _T("dc:description"), description);
			XmlMacroReadNodeValueBase(oNode, _T("dc:identifier"), identifier);
			//todo keywords is complex by spec there is no real example
			XmlMacroReadNodeValueBase(oNode, _T("cp:keywords"), keywords);
			XmlMacroReadNodeValueBase(oNode, _T("dc:language"), language);
			XmlMacroReadNodeValueBase(oNode, _T("cp:lastModifiedBy"), lastModifiedBy);
			XmlMacroReadNodeValueBase(oNode, _T("cp:lastPrinted"), lastPrinted);
			//lastPrinted = PPTX::DateTime::Parse(document.Root.element("lastPrinted").text().ToString());
			XmlMacroReadNodeValueBase(oNode, _T("dcterms:modified"), modified);
			//modified = PPTX::DateTime::Parse(document.Root.element("modified").text().ToString());
			XmlMacroReadNodeValueBase(oNode, _T("cp:revision"), revision);
			XmlMacroReadNodeValueBase(oNode, _T("dc:subject"), subject);
			XmlMacroReadNodeValueBase(oNode, _T("dc:title"), title);
			XmlMacroReadNodeValueBase(oNode, _T("cp:version"), version);
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
			pWriter->StartRecord(NSBinPptxRW::NSMainTables::Core);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteString2(0, title);
			pWriter->WriteString2(1, creator);
			pWriter->WriteString2(2, lastModifiedBy);
			pWriter->WriteString2(3, revision);
			pWriter->WriteString2(4, created);
			pWriter->WriteString2(5, modified);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			//start new record because new attributes is incompatible with previous versions
			pWriter->StartRecord(0);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			pWriter->WriteString2(6, category);
			pWriter->WriteString2(7, contentStatus);
			pWriter->WriteString2(8, description);
			pWriter->WriteString2(9, identifier);
			pWriter->WriteString2(10, keywords);
			pWriter->WriteString2(11, language);
			pWriter->WriteString2(12, lastPrinted);
			pWriter->WriteString2(13, subject);
			pWriter->WriteString2(14, version);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();

			pWriter->EndRecord();
		}
		virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(1); // type
			LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
				case 0: title = pReader->GetString2(); break;
				case 1: creator = pReader->GetString2(); break;
				case 2: lastModifiedBy = pReader->GetString2(); break;
				case 3: revision = pReader->GetString2(); break;
				case 4: created = pReader->GetString2(); break;
				case 5: modified = pReader->GetString2(); break;
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
						LONG _end_rec2 = pReader->GetPos() + pReader->GetLong() + 4;

						pReader->Skip(1); // start attributes

						while (true)
						{
							BYTE _at = pReader->GetUChar_TypeNode();
							if (_at == NSBinPptxRW::g_nodeAttributeEnd)
								break;

							switch (_at)
							{
							case 6: category = pReader->GetString2(); break;
							case 7: contentStatus = pReader->GetString2(); break;
							case 8: description = pReader->GetString2(); break;
							case 9: identifier = pReader->GetString2(); break;
							case 10: keywords = pReader->GetString2(); break;
							case 11: language = pReader->GetString2(); break;
							case 12: lastPrinted = pReader->GetString2(); break;
							case 13: subject = pReader->GetString2(); break;
							case 14: version = pReader->GetString2(); break;
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
		virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("cp:coreProperties"));

			pWriter->StartAttributes();

			pWriter->WriteAttribute(_T("xmlns:cp"), PPTX::g_Namespaces.cp.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dc"), PPTX::g_Namespaces.dc.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dcterms"), PPTX::g_Namespaces.dcterms.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:dcmitype"), PPTX::g_Namespaces.dcmitype.m_strLink);
			pWriter->WriteAttribute(_T("xmlns:xsi"), PPTX::g_Namespaces.xsi.m_strLink);

			pWriter->EndAttributes();

			pWriter->WriteNodeValue2(_T("dc:title"), title);
			pWriter->WriteNodeValue2(_T("dc:subject"), subject);
			pWriter->WriteNodeValue2(_T("dc:creator"), creator);
			pWriter->WriteNodeValue2(_T("cp:keywords"), keywords);
			pWriter->WriteNodeValue2(_T("dc:description"), description);
			pWriter->WriteNodeValue2(_T("dc:identifier"), identifier);
			pWriter->WriteNodeValue2(_T("dc:language"), language);
			pWriter->WriteNodeValue2(_T("cp:lastModifiedBy"), lastModifiedBy);
			pWriter->WriteNodeValue2(_T("cp:revision"), revision);

			if ((lastPrinted.IsInit()) && (false == lastPrinted->empty()))
			{
				pWriter->WriteNodeValue2(_T("cp:lastPrinted"), lastPrinted);
			}
			if ((created.IsInit()) && (false == created->empty()))
			{
				pWriter->WriteNodeBegin(_T("dcterms:created xsi:type=\"dcterms:W3CDTF\""));
				pWriter->WriteStringXML(*created);
				pWriter->WriteNodeEnd(_T("dcterms:created"));
			}
			if ((modified.IsInit()) && (false == modified->empty()))
			{
				pWriter->WriteNodeBegin(_T("dcterms:modified xsi:type=\"dcterms:W3CDTF\""));
				pWriter->WriteStringXML(*modified);
				pWriter->WriteNodeEnd(_T("dcterms:modified"));
			}
			pWriter->WriteNodeValue2(_T("cp:category"), category);
			pWriter->WriteNodeValue2(_T("cp:contentStatus"), contentStatus);
			pWriter->WriteNodeValue2(_T("cp:version"), version);

			pWriter->EndNode(_T("cp:coreProperties"));
		}

	public:
		nullable_string		category;
		nullable_string		contentStatus;
		//nullable_property<std::string, Limit::ContentStatus> contentStatus;
		nullable_string		created;
		//nullable_property<PPTX::DateTime> created;
		nullable_string		creator;
		nullable_string		description;
		nullable_string		identifier;
		nullable_string		keywords;
		nullable_string		language;
		nullable_string		lastModifiedBy;
		nullable_string		lastPrinted;
		nullable_string		modified;
		//nullable_property<PPTX::DateTime> modified;
		nullable_string		revision;
		nullable_string		subject;
		nullable_string		title;
		nullable_string		version;
	};
} // namespace PPTX

#endif // PPTX_CORE_FILE_INCLUDE_H_
