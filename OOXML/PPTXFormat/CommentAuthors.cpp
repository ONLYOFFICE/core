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

#include "CommentAuthors.h"

namespace PPTX
{
	namespace Logic
	{
		void CommentAuthor::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"id", id);
			if (!bModern && id.IsInit())
			{
				idx = XmlUtils::GetInteger(*id);
			}
			XmlMacroReadAttributeBase(node, L"lastIdx", last_idx);
			XmlMacroReadAttributeBase(node, L"clrIdx", clr_idx);
			XmlMacroReadAttributeBase(node, L"name", name);
			XmlMacroReadAttributeBase(node, L"initials", initials);
			XmlMacroReadAttributeBase(node, L"userId", userId);
			XmlMacroReadAttributeBase(node, L"providerId", providerId);
		}
		std::wstring CommentAuthor::toXML() const
		{
			return L"";
		}
		void CommentAuthor::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(bModern ? L"p188:author" : L"p:cmAuthor");

			pWriter->StartAttributes();
			if (!bModern && idx.IsInit())
			{
				pWriter->WriteAttribute(L"id", *idx);
			}
			else
			{
				pWriter->WriteAttribute(L"id", id.IsInit() ? *id : std::to_wstring(*idx));
			}
			pWriter->WriteAttribute2(L"name", name);
			pWriter->WriteAttribute(L"initials", initials);
			if (last_idx.IsInit())
			{
				pWriter->WriteAttribute(L"lastIdx", last_idx);
			}
			else if(!bModern)
			{
				pWriter->WriteAttribute(L"lastIdx", 2147483647);
			}
			if (clr_idx.IsInit())
			{
				pWriter->WriteAttribute(L"clrIdx", clr_idx);
			}
			else if (!bModern)
			{
				pWriter->WriteAttribute(L"clrIdx", 0);
			}
			pWriter->WriteAttribute2(L"userId", userId);
			pWriter->WriteAttribute2(L"providerId", providerId);
			pWriter->EndAttributes();

			pWriter->EndNode(bModern ? L"p188:author" : L"p:cmAuthor");
		}
		void CommentAuthor::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, idx);
				pWriter->WriteInt2(1, last_idx);
				pWriter->WriteInt2(2, clr_idx);
				pWriter->WriteString2(3, name);
				pWriter->WriteString2(4, initials);
				pWriter->WriteString2(5, id);
				pWriter->WriteString2(6, userId);
				pWriter->WriteString2(7, providerId);
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
					idx = pReader->GetLong();
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
				case 5:
					id = pReader->GetString2();
					break;
				case 6:
					userId = pReader->GetString2();
					break;
				case 7:
					providerId = pReader->GetString2();
					break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
			
			if (id.IsInit())
			{
				bModern = true;
			}
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

		if (XmlUtils::GetNamespace(oNode.GetName()) == L"p188")
			bModern = true;

		std::vector<XmlUtils::CXmlNode> oNodes;
		oNode.GetNodes(L"*", oNodes);

		for (size_t i = 0; i < oNodes.size(); ++i)
		{
			XmlUtils::CXmlNode& oCm = oNodes[i];

			std::wstring strName = XmlUtils::GetNameNoNS(oCm.GetName());

			if (strName == L"author")
			{
				m_arAuthors.emplace_back();

				if (XmlUtils::GetNamespace(oCm.GetName()) == L"p188")
					m_arAuthors.back().bModern = true;
				
				m_arAuthors.back().fromXML(oCm);
			}
		}
	}
	void Authors::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
	}
	const OOX::FileType Authors::type() const
	{
		if (bModern) 	return OOX::Presentation::FileTypes::ModernCommentAuthors;
		else			return OOX::Presentation::FileTypes::CommentAuthors;
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
		pWriter->StartNode(bModern ? L"p188:authorLst" : L"p:cmAuthorLst");

		pWriter->StartAttributes();
		pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
		if (bModern)
			pWriter->WriteAttribute(L"xmlns:p188", L"http://schemas.microsoft.com/office/powerpoint/2018/8/main");
		pWriter->EndAttributes();

		for (auto author : m_arAuthors)
		{
			author.bModern = bModern;
			author.toXmlWriter(pWriter);
		}

		pWriter->EndNode(bModern ? L"p188:authorLst" : L"p:cmAuthorLst");
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

						m_arAuthors.emplace_back();
						m_arAuthors.back().fromPPTY(pReader);

						if (m_arAuthors.back().bModern)
							bModern = true;
					}					
				}break;
				default:
				{
					pReader->SkipRecord();
					break;
				}
			}
		}
		pReader->Seek(end);

		if (!bModern)
		{
			bool bNew = mapAuthors.empty();

			for (auto &auth : m_arAuthors)
			{
				if (true == auth.idx.IsInit())
					break;
				if (false == auth.id.IsInit())
					break;

				if (bNew)
				{
					auth.idx = mapAuthors.size() + 1;
					mapAuthors.insert(std::make_pair(*auth.id, *auth.idx));
				}
				else
				{
					std::map<std::wstring, int>::iterator pFind = mapAuthors.find(*auth.id);

					if (pFind != mapAuthors.end())
					{
						auth.idx = pFind->second;
					}
					else
					{
						auth.idx = mapAuthors.size() + 1;
						mapAuthors.insert(std::make_pair(*auth.id, *auth.idx));
					}
				}
			}
		}
	}

} // namespace PPTX
