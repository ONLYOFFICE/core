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

#include "Comments.h"
#include "FileTypes.h"

namespace PPTX
{
	namespace Logic
	{
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void DrawingElementMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ac:deMkLst");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:ac", L"http://schemas.microsoft.com/office/drawing/2013/main/command");
			pWriter->EndAttributes();

			AnyMonikerList::toXmlWriter(pWriter);

			pWriter->EndNode(L"ac:deMkLst");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void SlideMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"pc:sldMkLst");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/drawing/2013/main/command");
			pWriter->EndAttributes();

			AnyMonikerList::toXmlWriter(pWriter);

			pWriter->EndNode(L"pc:sldMkLst");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void TextCharRangeMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ac:txMkLst");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:ac", L"http://schemas.microsoft.com/office/drawing/2013/main/command");
			pWriter->EndAttributes();

			AnyMonikerList::toXmlWriter(pWriter);

			pWriter->EndNode(L"ac:txMkLst");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		AnyMonikerList::~AnyMonikerList()
		{
			for (auto item : m_arrItems)
			{
				if (item)
				{
					delete item;
					item = NULL;
				}
			}
		}
		void AnyMonikerList::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);

			for (size_t i = 0; i < oNodes.size(); ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());
				WrapperWritingElement *pItem = NULL;

				if (strName == L"docMk")
				{
				}
				if (pItem)
				{
					m_arrItems.push_back(pItem);
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring AnyMonikerList::toXML() const
		{
			return L"";
		}
		void AnyMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			for (auto item : m_arrItems)
			{
				if (item)
				{
					item->toXmlWriter(pWriter);
				}
			}
		}
		void AnyMonikerList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto item : m_arrItems)
			{
				if (item)
				{
					item->toPPTY(pWriter);
				}
			}
		}
		void AnyMonikerList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				WrapperWritingElement* pItem = NULL;
				switch (_at)
				{
				case 0:
				default:
				{
					pReader->SkipRecord();
					break;
				}
				if (pItem)
				{
					pItem->fromPPTY(pReader);
					m_arrItems.push_back(pItem);
				}
				}
			}

			pReader->Seek(_end_rec);
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void CommentReplyList::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"reply")
				{
					m_arReply.emplace_back();
					m_arReply.back().fromXML(oNode);
				}
			}

			FillParentPointersForChilds();
		}
		std::wstring CommentReplyList::toXML() const
		{
			return L"";
		}
		void CommentReplyList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p188:replyLst"));

			pWriter->StartAttributes();
			pWriter->EndAttributes();

			if (m_arReply.size() == 0)
				pWriter->WriteString(L"<p188:replyLst/>");
			else
				pWriter->WriteArray(L"p188:replyLst", m_arReply);

			pWriter->EndNode(_T("p188:replyLst"));
		}
		void CommentReplyList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecordArray(0, 1, m_arReply);
		}
		void CommentReplyList::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					m_arReply.emplace_back();
					m_arReply.back().fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}

			pReader->Seek(_end_rec);
		}
		void CommentReplyList::FillParentPointersForChilds()
		{
			for (size_t i = 0; i < m_arReply.size(); ++i)
				m_arReply[i].SetParentPointer(this);
		}
		void CommentReply::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"authorId", authorId);
			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"created", created);
			XmlMacroReadAttributeBase(node, L"status", status);

			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);

			for (size_t i = 0; i < oNodes.size(); ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"extLst")
				{
				}
				else if (strName == L"txBody")
				{
					txBody = oNode;
				}
			}
		}
		std::wstring CommentReply::toXML() const
		{
			return _T("");
		}
		void CommentReply::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p188:reply"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("authorId"), authorId);
			pWriter->WriteAttribute2(_T("status"), status);
			pWriter->WriteAttribute(_T("id"), id);
			pWriter->EndAttributes();

			if (txBody.IsInit())
			{
				std::wstring textFrom = txBody->GetText();
				pWriter->WriteString(_T("<p:text>"));
				pWriter->WriteStringXML(textFrom);
				pWriter->WriteString(_T("</p:text>"));
			}

			pWriter->EndNode(_T("p188:reply"));
		}
		void CommentReply::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, authorId);
			pWriter->WriteString2(1, id);
			pWriter->WriteString2(2, created);
			pWriter->WriteString2(3, status);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, txBody);
		}
		void CommentReply::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					authorId = pReader->GetLong();
					break;
				case 1:
					id = pReader->GetString2();
					break;
				case 2:
					created = pReader->GetString2();
					break;
				case 3:
					status = pReader->GetString2();
					break;
				default:
					break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					txBody.Init();
					txBody->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();
					break;
				}
				}
			}

			pReader->Seek(_end_rec);
		}
		void CommentReply::FillParentPointersForChilds()
		{
		}

//----------------------------------------------------------------------------------------------------------------------------------------------------
		void Comment::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"authorId", authorId);
			XmlMacroReadAttributeBase(node, L"dt", dt);
			XmlMacroReadAttributeBase(node, L"idx", idx);

			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"created", created);
			XmlMacroReadAttributeBase(node, L"status", status);
			XmlMacroReadAttributeBase(node, L"title", title);
			XmlMacroReadAttributeBase(node, L"startDate", startDate);
			XmlMacroReadAttributeBase(node, L"dueDate", dueDate);
			XmlMacroReadAttributeBase(node, L"assignedTo", assignedTo);
			XmlMacroReadAttributeBase(node, L"complete", complete);

			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"pos")
				{
					XmlMacroReadAttributeBase(oNode, L"x", pos_x);
					XmlMacroReadAttributeBase(oNode, L"y", pos_y);
				}
				else if (strName == L"text")
				{
					text = oNode.GetTextExt();
				}
				else if (strName == L"extLst")
				{
					bool bIsFound1 = false;
					bool bIsFound2 = false;

					std::vector<XmlUtils::CXmlNode> oNodesExt;
					if (oNode.GetNodes(L"p:ext", oNodesExt))
					{
						size_t nCountExts = oNodesExt.size();
						for (size_t nIndex = 0; nIndex < nCountExts; ++nIndex)
						{
							XmlUtils::CXmlNode& oNodeExt = oNodesExt[nIndex];

							// parent
							if (!bIsFound1)
							{
								XmlUtils::CXmlNode oNodeTI = oNodeExt.ReadNode(_T("p15:threadingInfo"));
								if (oNodeTI.IsValid())
								{
									XmlMacroReadAttributeBase(oNodeTI, L"timeZoneBias", timeZoneBias);

									XmlUtils::CXmlNode oNodeParent = oNodeTI.ReadNode(_T("p15:parentCm"));

									XmlMacroReadAttributeBase(oNodeParent, L"authorId", parentAuthorId);
									XmlMacroReadAttributeBase(oNodeParent, L"idx", parentCommentId);

									bIsFound1 = true;
								}
							}
							if (!bIsFound2)
							{
								XmlUtils::CXmlNode oNodeAU = oNodeExt.ReadNode(_T("p15:presenceInfo"));
								if (oNodeAU.IsValid())
								{
									std::wstring strData = oNodeAU.GetAttribute(_T("userId"));

									if (!strData.empty())
										additional_data = XmlUtils::EncodeXmlString(strData);

									bIsFound2 = true;
								}
							}
						}
					}
				}
				else if (strName == L"txMkLst")
				{
					txMkLst = oNode;
				}
				else if (strName == L"sldMkLst")
				{
					sldMkLst = oNode;
				}
				else if (strName == L"deMkLst")
				{
					deMkLst = oNode;
				}
				else if (strName == L"txBody")
				{
					txBody = oNode;
				}
				else if (strName == L"replyLst")
				{
					replyLst = oNode;
				}
			}

			//check comment guid
			if (!(additional_data.IsInit() && 0 == additional_data->compare(0, 13, L"teamlab_data:") && std::wstring::npos != additional_data->find(L"4;38;")))
			{
				if (!additional_data.IsInit())
				{
					additional_data = L"teamlab_data:";
				}
				if (':' != additional_data->back() && ';' != additional_data->back())
				{
					additional_data->append(L";");
				}
				additional_data->append(L"4;38;{" + XmlUtils::GenerateGuid() + L"}");
			}
		}
		std::wstring Comment::toXML() const
		{
			return _T("");
		}
		void Comment::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("p:cm"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(_T("authorId"), authorId);
			pWriter->WriteAttribute2(_T("dt"), dt);
			pWriter->WriteAttribute(_T("idx"), idx);
			pWriter->EndAttributes();

			if (pos_x.is_init() && pos_y.is_init())
			{
				std::wstring sPos = L"<p:pos x=\"" + std::to_wstring(*pos_x) + L"\" y=\"" + std::to_wstring(*pos_y) + L"\"/>";
				pWriter->WriteString(sPos);
			}
			if (text.is_init())
			{
				pWriter->WriteString(_T("<p:text>"));
				pWriter->WriteStringXML(*text);
				pWriter->WriteString(_T("</p:text>"));
			}
			else if (txBody.IsInit())
			{
				std::wstring textFrom = txBody->GetText();
				pWriter->WriteString(_T("<p:text>"));
				pWriter->WriteStringXML(textFrom);
				pWriter->WriteString(_T("</p:text>"));
			}

			bool bIsExtLst = false;
			if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init() || additional_data.is_init())
				bIsExtLst = true;

			if (bIsExtLst)
				pWriter->WriteString(_T("<p:extLst>"));

			if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init())
			{
				pWriter->WriteString(_T("<p:ext uri=\"{C676402C-5697-4E1C-873F-D02D1690AC5C}\">\
<p15:threadingInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\""));
				if (timeZoneBias.is_init())
				{
					pWriter->WriteString(_T(" timeZoneBias=\""));
					pWriter->WriteINT(timeZoneBias.get());
					pWriter->WriteString(_T("\""));
				}
				pWriter->WriteString(_T(">"));
				if (parentAuthorId.is_init() && parentCommentId.is_init())
				{
					std::wstring sPos = L"<p15:parentCm authorId=\"" + std::to_wstring(*parentAuthorId) + L"\" idx=\"" + std::to_wstring(*parentCommentId) + L"\"/>";
					pWriter->WriteString(sPos);
				}

				pWriter->WriteString(_T("</p15:threadingInfo></p:ext>"));
			}

			if (additional_data.is_init())
			{
				pWriter->WriteString(_T("<p:ext uri=\"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}\">\
<p15:presenceInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\" userId=\""));

				std::wstring strData = XmlUtils::EncodeXmlStringExtend(additional_data.get());

				pWriter->WriteString(strData);

				pWriter->WriteString(_T("\" providerId=\"AD\"/></p:ext>"));
			}

			if (bIsExtLst)
				pWriter->WriteString(_T("</p:extLst>"));

			pWriter->EndNode(_T("p:cm"));
		}
		void Comment::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteInt2(0, authorId);

			if (dt.IsInit() && !dt->empty())
			{
				pWriter->WriteString2(1, dt);
			}
			else if (created.IsInit() && !created->empty())
			{
				std::wstring date = *created;
				size_t pos_point = date.rfind(L".");
				size_t pos_sep = date.rfind(L":");

				if (pos_point != std::wstring::npos && pos_point > pos_sep)
				{
					date = date.substr(0, pos_point);
				}
				if (!created->empty() && date.back() != L'Z') date += L"Z";

				pWriter->WriteString1(1, date);
			}
			pWriter->WriteInt2(2, idx);

			pWriter->WriteInt2(3, pos_x);
			pWriter->WriteInt2(4, pos_y);

			if (text.IsInit())
			{
				pWriter->WriteString2(5, text);
			}
			else if (txBody.IsInit())
			{
				std::wstring modern_text = txBody->GetText(true);
				pWriter->WriteString1(5, modern_text);
			}
			pWriter->WriteInt2(6, parentAuthorId);
			pWriter->WriteInt2(7, parentCommentId);

			pWriter->WriteString2(8, additional_data);

			//pWriter->WriteString2(9, id);
			//pWriter->WriteString2(10, created);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->StartRecord(0);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(9, timeZoneBias);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();

			pWriter->WriteRecord2(1, txBody);
			pWriter->WriteRecord2(2, replyLst);
			pWriter->WriteRecord2(3, txMkLst);
			pWriter->WriteRecord2(4, sldMkLst);
			pWriter->WriteRecord2(5, deMkLst);
		}
		void Comment::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
						authorId = pReader->GetLong();
						break;
					case 1:
						dt = pReader->GetString2();
						break;
					case 2:
						idx = pReader->GetLong();
						break;
					case 3:
						pos_x = pReader->GetLong();
						break;
					case 4:
						pos_y = pReader->GetLong();
						break;
					case 5:
						text = pReader->GetString2();
						break;
					case 6:
						parentAuthorId = pReader->GetLong();
						break;
					case 7:
						parentCommentId = pReader->GetLong();
						break;
					case 8:
						additional_data = pReader->GetString2();
						break;
					case 9:
						id = pReader->GetString2();
						break;
					case 10:
						created = pReader->GetString2();
						break;
					default:
						break;
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
							case 9: timeZoneBias = pReader->GetLong(); break;
							default: break;
							}
						}

						pReader->Seek(_end_rec2);
					}break;
					case 1:
					{
						txBody.Init();
						txBody->fromPPTY(pReader);
					}break;
					case 2:
					{
						replyLst.Init();
						replyLst->fromPPTY(pReader);
					}break;
					case 3:
					{
						txMkLst.Init();
						txMkLst->fromPPTY(pReader);
					}break;
					case 4:
					{
						sldMkLst.Init();
						sldMkLst->fromPPTY(pReader);
					}break;
					case 5:
					{
						deMkLst.Init();
						deMkLst->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
						break;
					}
				}
			}

			pReader->Seek(_end_rec);
		}
		void Comment::FillParentPointersForChilds()
		{
		}
	}
//----------------------------------------------------------------------------------------------------------------------------------------------------
	Comments::Comments(OOX::Document* pMain) : WrapperFile(pMain)
	{
	}
	Comments::Comments(OOX::Document* pMain, const OOX::CPath& filename, FileMap& map) : WrapperFile(pMain)
	{
		read(filename, map);
	}
	Comments::~Comments()
	{
	}
	void Comments::read(const OOX::CPath& filename, FileMap& map)
	{
		XmlUtils::CXmlNode oNode;
		oNode.FromXmlFile(filename.m_strFilename);

		if (XmlUtils::GetNamespace(oNode.GetName()) == L"p188")
			bModern = true;

		std::vector<XmlUtils::CXmlNode> oNodes;
		oNode.GetNodes(L"*", oNodes);
		size_t nCount = oNodes.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode& oCm = oNodes[i];

			std::wstring strName = XmlUtils::GetNameNoNS(oCm.GetName());

			if (strName == L"cm")
			{
				PPTX::Logic::Comment elm;

				if (XmlUtils::GetNamespace(oCm.GetName()) == L"p188")
					elm.bModern = true;
				m_arComments.push_back(elm);
				m_arComments.back().fromXML(oCm);
			}
		}
	}
	void Comments::write(const OOX::CPath& filename, const OOX::CPath& directory, OOX::CContentTypes& content)const
	{
		WrapperFile::write(filename, directory, content);
	}
	const OOX::FileType Comments::type() const
	{
		return OOX::Presentation::FileTypes::SlideComments;
	}
	const OOX::CPath Comments::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const OOX::CPath Comments::DefaultFileName() const
	{
		return type().DefaultFileName();
	}
	void Comments::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
	{
		pWriter->WriteRecordArray(0, 0, m_arComments);
	}
	void Comments::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
	{
		pWriter->StartNode(_T("p:cmLst"));

		pWriter->StartAttributes();
		pWriter->WriteAttribute(_T("xmlns:a"), PPTX::g_Namespaces.a.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:r"), PPTX::g_Namespaces.r.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:p"), PPTX::g_Namespaces.p.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:m"), PPTX::g_Namespaces.m.m_strLink);
		pWriter->WriteAttribute(_T("xmlns:w"), PPTX::g_Namespaces.w.m_strLink);
		pWriter->EndAttributes();

		pWriter->WriteArray2(m_arComments);

		pWriter->EndNode(_T("p:cmLst"));
	}
	void Comments::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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

					PPTX::Logic::Comment elm;
					m_arComments.push_back(elm);
					m_arComments.back().fromPPTY(pReader);
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
