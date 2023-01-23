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

#include "CommentAuthors.h"

namespace PPTX
{
	namespace Logic
	{
		void CommentAuthor::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"lastIdx", last_idx);
			XmlMacroReadAttributeBase(node, L"clrIdx", clr_idx);
			XmlMacroReadAttributeBase(node, L"name", name);
			XmlMacroReadAttributeBase(node, L"initials", initials);
		}
		std::wstring CommentAuthor::toXML() const
		{
			return _T("");
		}
		void CommentAuthor::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:cmAuthor"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("id"), id);
			pWriter->WriteAttribute2(_T("name"), name);
			pWriter->WriteAttribute(_T("initials"), initials);
			pWriter->WriteAttribute(_T("lastIdx"), last_idx);
			pWriter->WriteAttribute(_T("clrIdx"), clr_idx);
			pWriter->EndAttributes();

			pWriter->EndNode(_T("p:cmAuthor"));
		}
		void CommentAuthor::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, id);
			pWriter->WriteInt2(1, last_idx);
			pWriter->WriteInt2(2, clr_idx);
			pWriter->WriteString2(3, name);
			pWriter->WriteString2(4, initials);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void CommentAuthor::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
				case 0:
					id = pReader->GetLong();
					break;
				case 1:
					last_idx = pReader->GetLong();
					break;
				case 2:
					clr_idx = pReader->GetLong();
					break;
				case 3:
					name = pReader->GetString2();
					break;
				case 4:
					initials = pReader->GetString2();
					break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void CommentAuthor::FillParentPointersForChilds()
		{
		}
	}

	Authors::Authors(OOX::Document* pMain) : WrapperFile(pMain)
	{
	}
	Authors::Authors(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
	{
		read(filename, map);
	}
	Authors::~Authors()
	{
	}
	void Authors::read(const OOX::CPath& filename, FileMap& map)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		XmlUtils::CXmlNodes oNodes;
		oNode.GetNodes(_T("p:cmAuthor"), oNodes);
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oCm;
			oNodes.GetAt(i, oCm);

			PPTX::Logic::CommentAuthor comm;

			m_arAuthors.push_back(comm);
			m_arAuthors.back().fromXML(oCm);
		}
	}
	void Authors::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
	}
	const OOX::FileType Authors::type() const
	{
		return OOX::Presentation::FileTypes::CommentAuthors;
	}
	const OOX::CPath Authors::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath Authors::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void Authors::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecordArray(0, 0, m_arAuthors);
	}
	void Authors::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("p:cmAuthorLst"));

		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
		pWriter->EndAttributes();

		pWriter->WriteArray2(m_arAuthors);

		pWriter->EndNode(_T("p:cmAuthorLst"));
	}
	void Authors::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
	{
		LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

		while (pReader->GetPos() < end)
		{
			BYTE _rec = pReader->GetUChar();

			switch (_rec)
			{
				case 0:
				{
					pReader->Skip(4); // len
					ULONG lCount = pReader->GetULong();

					for (ULONG i = 0; i < lCount; ++i)
					{
						pReader->Skip(1);

						PPTX::Logic::CommentAuthor comm;

						m_arAuthors.push_back(comm);
						m_arAuthors.back().fromPPTY(pReader);
					}

					break;
				}
				default:
				{
					pReader->SkipRecord();
					break;
				}
			}
		}

		pReader->Seek(end);
	}

} // namespace PPTX
