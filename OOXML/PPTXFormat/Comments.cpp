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
#include "Folder.h"
#include "Presentation.h"

namespace PPTX
{
	namespace Logic
	{
		void TextCharRangeContext::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"len", len);
			XmlMacroReadAttributeBase(node, L"hash", hash);
		}
		void TextCharRangeContext::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, hash);
			pWriter->WriteUInt2(1, len);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void TextCharRangeContext::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					hash = pReader->GetLong();
					break;
				case 1:
					len = pReader->GetLong();
					break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void TextCharRangeContext::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ac:context");
				pWriter->StartAttributes();
					pWriter->WriteAttribute2(L"len", len);
					pWriter->WriteAttribute2(L"hash", hash);
				pWriter->EndAttributes();
			pWriter->EndNode(L"ac:context");
		}
//--------------------------------------------------------------------------------------------------------------
		void TextCharRangeMoniker::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlMacroReadAttributeBase(node, L"cp", cp);
			XmlMacroReadAttributeBase(node, L"len", len);

			std::vector<XmlUtils::CXmlNode> oNodes;
			node.GetNodes(L"*", oNodes);
			size_t nCount = oNodes.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				XmlUtils::CXmlNode& oNode = oNodes[i];
				std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

				if (strName == L"context")
				{
					context = oNode;
				}
			}
		}
		void TextCharRangeMoniker::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(25);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteUInt2(0, cp);
			pWriter->WriteUInt2(1, len);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			
			pWriter->WriteRecord2(0, context);
			
			pWriter->EndRecord();
		}
		void TextCharRangeMoniker::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					cp = pReader->GetLong();
					break;
				case 1:
					len = pReader->GetLong();
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
					context.Init();
					context->fromPPTY(pReader);
				}break;
				default:
				{
					pReader->SkipRecord();					
				}break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void TextCharRangeMoniker::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ac:txMk");
			pWriter->StartAttributes();
			pWriter->WriteAttribute2(L"cp", cp);
			pWriter->WriteAttribute2(L"len", len);
			pWriter->EndAttributes();
			
			pWriter->Write(context);
			
			pWriter->EndNode(L"ac:txMk");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void DrawingMoniker::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"id", id);
			XmlMacroReadAttributeBase(node, L"creationId", creationId);
		}
		void DrawingMoniker::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(base_type);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					pWriter->WriteUInt2(0, id);
					pWriter->WriteString2(1, creationId);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();
		}
		void DrawingMoniker::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					creationId = pReader->GetString2();
					break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void DrawingMoniker::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (node_name.empty()) return;

			pWriter->StartNode(node_name);
				pWriter->StartAttributes();
					pWriter->WriteAttribute2(L"id", id);
					pWriter->WriteAttribute2(L"creationId", creationId);
				pWriter->EndAttributes();
			pWriter->EndNode(node_name);
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void SlideBaseMoniker::fromXML(XmlUtils::CXmlNode& node)
		{
			node_name = node.GetName();

			XmlMacroReadAttributeBase(node, L"cId", cId);
			XmlMacroReadAttributeBase(node, L"sldId", sldId);
		}
		void SlideBaseMoniker::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(base_type);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					pWriter->WriteUInt2(0, cId);
					pWriter->WriteUInt2(1, sldId);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();
		}
		void SlideBaseMoniker::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
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
					cId = pReader->GetLong();
					break;
				case 1:
					sldId = pReader->GetLong();
					break;
				default:
					break;
				}
			}
			pReader->Seek(_end_rec);
		}
		void SlideBaseMoniker::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (node_name.empty()) return;

			pWriter->StartNode(node_name);
			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/powerpoint/2013/main/command");
				pWriter->WriteAttribute2(L"cId", cId);
				pWriter->WriteAttribute2(L"sldId", sldId);
			pWriter->EndAttributes();
			pWriter->EndNode(node_name);
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void DocumentMoniker::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		void DocumentMoniker::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(20);
			pWriter->EndRecord();
		}
		void DocumentMoniker::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;
			pReader->Seek(_end_rec);
		}
		void DocumentMoniker::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"pc:docMk");
			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/powerpoint/2013/main/command");
			pWriter->EndAttributes();
			pWriter->EndNode(L"pc:docMk");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void DrawingElementMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"ac:deMkLst");
			pWriter->StartAttributes();
				pWriter->WriteAttribute(L"xmlns:ac", L"http://schemas.microsoft.com/office/drawing/2013/main/command");
				pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/powerpoint/2013/main/command");
			pWriter->EndAttributes();
				AnyMonikerList::toXmlWriter(pWriter);
			pWriter->EndNode(L"ac:deMkLst");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
		void SlideMonikerList::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"pc:sldMkLst");
				pWriter->StartAttributes();
					pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/powerpoint/2013/main/command");
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
				pWriter->WriteAttribute(L"xmlns:pc", L"http://schemas.microsoft.com/office/drawing/2013/main/command");
			pWriter->EndAttributes();

			AnyMonikerList::toXmlWriter(pWriter);

			pWriter->EndNode(L"ac:txMkLst");
		}
//----------------------------------------------------------------------------------------------------------------------------------------------------
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
					AssignPtrXmlContentNoMain(pItem, DocumentMoniker, oNode)
				else if(strName == L"sldMasterMk")
					AssignPtrXmlContentNoMain(pItem, SlideMasterMoniker, oNode)
				else if (strName == L"sldLayoutMk")
					AssignPtrXmlContentNoMain(pItem, SlideLayoutMoniker, oNode)
				else if (strName == L"sldMk")
					AssignPtrXmlContentNoMain(pItem, SlideMoniker, oNode)
				else if (strName == L"cxnSpMk")
					AssignPtrXmlContentNoMain(pItem, ConnectorMoniker, oNode)
				else if (strName == L"spMk")
					AssignPtrXmlContentNoMain(pItem, ShapeMoniker, oNode)
				else if (strName == L"picMk")
					AssignPtrXmlContentNoMain(pItem, PictureMoniker, oNode)
				else if (strName == L"graphicFrameMk")
					AssignPtrXmlContentNoMain(pItem, GraphicFrameMoniker, oNode)
				else if (strName == L"grpSpMk")
					AssignPtrXmlContentNoMain(pItem, GroupShapeMoniker, oNode)
				else if (strName == L"inkMk")
					AssignPtrXmlContentNoMain(pItem, InkMoniker, oNode)
				else if (strName == L"txMk")
					AssignPtrXmlContentNoMain(pItem, TextCharRangeMoniker, oNode)

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
				if (item.IsInit())
				{
					item->toXmlWriter(pWriter);
				}
			}
		}
		void AnyMonikerList::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (auto item : m_arrItems)
			{
				if (item.IsInit())
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
					case 1: pItem = new GraphicFrameMoniker(); break;
					case 2: pItem = new GroupShapeMoniker(); break;
					case 3: pItem = new PictureMoniker(); break;
					case 4: pItem = new ShapeMoniker(); break;
					case 5: pItem = new ConnectorMoniker(); break;
					case 6: pItem = new InkMoniker(); break;
					case 10: pItem = new SlideMoniker(); break;
					case 11: pItem = new SlideLayoutMoniker(); break;
					case 12: pItem = new SlideMasterMoniker(); break;
					case 20: pItem = new DocumentMoniker(); break;
					case 25: pItem = new TextCharRangeMoniker(); break;
					default:
						break;
				}
				if (pItem)
				{
					pItem->fromPPTY(pReader);
					m_arrItems.push_back(pItem);
				}
				else
				{
					pReader->SkipRecord();
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
			if (m_arReply.size() == 0)
				pWriter->WriteString(L"<p188:replyLst/>");
			else
				pWriter->WriteArray(L"p188:replyLst", m_arReply);
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
					pReader->Skip(4); // len
					ULONG lCount = pReader->GetULong();

					for (ULONG i = 0; i < lCount; ++i)
					{
						pReader->Skip(1);

						m_arReply.emplace_back();
						m_arReply.back().fromPPTY(pReader);
					}

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
			return L"";
		}
		void CommentReply::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p188:reply");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"id", id);
			pWriter->WriteAttribute(L"authorId", authorId);
			pWriter->WriteAttribute(L"created", created);
			pWriter->WriteAttribute2(L"status", status);
			pWriter->EndAttributes();

			if (txBody.IsInit())
			{
				txBody->m_name = L"p188:txBody";
				txBody->toXmlWriter(pWriter);
			}

			pWriter->EndNode(L"p188:reply");
		}
		void CommentReply::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, authorId);
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
					authorId = pReader->GetString2();
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
			if (!bModern)
			{
				authorIdx = XmlUtils::GetInteger(*authorId);
			}
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

			for (size_t i = 0; i < oNodes.size(); ++i)
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
								XmlUtils::CXmlNode oNodeTI = oNodeExt.ReadNode(L"p15:threadingInfo");
								if (oNodeTI.IsValid())
								{
									XmlMacroReadAttributeBase(oNodeTI, L"timeZoneBias", timeZoneBias);

									XmlUtils::CXmlNode oNodeParent = oNodeTI.ReadNode(L"p15:parentCm");

									XmlMacroReadAttributeBase(oNodeParent, L"authorId", parentAuthorId);
									XmlMacroReadAttributeBase(oNodeParent, L"idx", parentCommentId);

									bIsFound1 = true;
								}
							}
							if (!bIsFound2)
							{
								XmlUtils::CXmlNode oNodeAU = oNodeExt.ReadNode(L"p15:presenceInfo");
								if (oNodeAU.IsValid())
								{
									std::wstring strData = oNodeAU.GetAttribute(L"userId");

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
					anyMkLst = new TextCharRangeMonikerList();
					anyMkLst->fromXML(oNode);
				}
				else if (strName == L"sldMkLst")
				{
					anyMkLst = new SlideMonikerList();
					anyMkLst->fromXML(oNode);
				}
				else if (strName == L"deMkLst")
				{
					anyMkLst = new DrawingElementMonikerList();
					anyMkLst->fromXML(oNode);
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
			return L"";
		}
		void Comment::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring extAdditional = additional_data.IsInit() ? (L"<p:ext uri=\"{19B8F6BF-5375-455C-9EA6-DF929625EA0E}\">\
<p15:presenceInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\" userId=\"" +  XmlUtils::EncodeXmlStringExtend(*additional_data) + L"\" providerId=\"AD\"/></p:ext>") : L"";

			if (bModern)
			{
				pWriter->StartNode(L"p188:cm");
				pWriter->StartAttributes();
				
				pWriter->WriteAttribute(L"id", id);
				if (authorId.IsInit())
				{
					pWriter->WriteAttribute(L"authorId", *authorId);
				}
				else
				{
					pWriter->WriteAttribute(L"authorId", authorIdx);
				}
				if (created.IsInit())
					pWriter->WriteAttribute(L"created", created);
				else
					pWriter->WriteAttribute(L"created", dt);
				
					pWriter->WriteAttribute(L"status", status);
					pWriter->WriteAttribute(L"title", title);
					pWriter->WriteAttribute(L"startDate", startDate);
					pWriter->WriteAttribute(L"dueDate", dueDate);
					pWriter->WriteAttribute(L"assignedTo", assignedTo);
					pWriter->WriteAttribute2(L"complete", complete);
				pWriter->EndAttributes();

				if (anyMkLst.IsInit())
				{
					anyMkLst->toXmlWriter(pWriter);
				}
				if (pos_x.is_init() && pos_y.is_init())
				{
					std::wstring sPos = L"<p188:pos x=\"" + std::to_wstring(*pos_x) + L"\" y=\"" + std::to_wstring(*pos_y) + L"\"/>";
					pWriter->WriteString(sPos);
				}
				pWriter->Write(replyLst);
				if (txBody.IsInit())
				{
					txBody->m_name = L"p188:txBody";
					txBody->toXmlWriter(pWriter);
				}
				else if (text.is_init())
				{
					pWriter->WriteString(L"<p188:txBody><a:bodyPr/><a:lstStyle/><a:p><a:r><a:rPr/><a:t>");
					pWriter->WriteStringXML(*text);
					pWriter->WriteString(L"</a:t></a:r></a:p></p188:txBody>");
				}
				if (additional_data.is_init())
				{
					pWriter->WriteString(L"<p188:extLst>");
						pWriter->WriteString(extAdditional);
					pWriter->WriteString(L"</p188:extLst>");
				
					//ext - p223
				}
				pWriter->EndNode(L"p188:cm");
			}
			else
			{
				pWriter->StartNode(L"p:cm");
				pWriter->StartAttributes();
				if (authorIdx.IsInit())
				{
					pWriter->WriteAttribute(L"authorId", authorIdx);
				}
				pWriter->WriteAttribute2(L"dt", dt);
				
				if (idx.IsInit())
				{
					pWriter->WriteAttribute(L"idx", idx);
				}
				else if (id.IsInit())
				{
					int int_id = XmlUtils::GetHex(id->c_str());
					pWriter->WriteAttribute(L"idx", abs(int_id));
				}
				pWriter->EndAttributes();
				
				if (pos_x.is_init() && pos_y.is_init())
				{
					std::wstring sPos = L"<p:pos x=\"" + std::to_wstring(*pos_x) + L"\" y=\"" + std::to_wstring(*pos_y) + L"\"/>";
					pWriter->WriteString(sPos);
				}
				else
				{
					std::wstring sPos = L"<p:pos x=\"0\" y=\"0\"/>";
					pWriter->WriteString(sPos);
				}
				if (text.is_init())
				{
					pWriter->WriteString(L"<p:text>");
					pWriter->WriteStringXML(*text);
					pWriter->WriteString(L"</p:text>");
				}
				else if (txBody.IsInit())
				{
					std::wstring textFrom = txBody->GetText();
					pWriter->WriteString(L"<p:text>");
					pWriter->WriteStringXML(textFrom);
					pWriter->WriteString(L"</p:text>");
				}
				bool bIsExtLst = false;
				if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init() || additional_data.is_init())
					bIsExtLst = true;

				if (bIsExtLst)
				{
					pWriter->WriteString(L"<p:extLst>");

					if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init())
					{
						pWriter->WriteString(L"<p:ext uri=\"{C676402C-5697-4E1C-873F-D02D1690AC5C}\">\
<p15:threadingInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\"");
						if (timeZoneBias.is_init())
						{
							pWriter->WriteString(L" timeZoneBias=\"");
							pWriter->WriteINT(timeZoneBias.get());
							pWriter->WriteString(L"\"");
						}
						pWriter->WriteString(L">");
						if (parentAuthorId.is_init() && parentCommentId.is_init())
						{
							std::wstring sPos = L"<p15:parentCm authorId=\"" + std::to_wstring(*parentAuthorId) + L"\" idx=\"" + std::to_wstring(*parentCommentId) + L"\"/>";
							pWriter->WriteString(sPos);
						}

						pWriter->WriteString(L"</p15:threadingInfo></p:ext>");
					}
					pWriter->WriteString(extAdditional);

					pWriter->WriteString(L"</p:extLst>");
				}

				pWriter->EndNode(L"p:cm");
			}
		}
		void Comment::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, authorIdx);

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

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		
			pWriter->StartRecord(100);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
					pWriter->WriteString2(0, id);
					pWriter->WriteString2(1, created);
					pWriter->WriteString2(2, authorId);
					pWriter->WriteString2(3, status);
					pWriter->WriteString2(4, startDate);
					pWriter->WriteString2(5, dueDate);
					pWriter->WriteString2(6, assignedTo);
					pWriter->WriteString2(7, title);
					pWriter->WriteUInt2(8, complete);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();

			pWriter->StartRecord(0);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(9, timeZoneBias);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			pWriter->EndRecord();

			pWriter->WriteRecord2(1, txBody);
			pWriter->WriteRecord2(2, replyLst);

			if (anyMkLst.IsInit())
			{
				pWriter->StartRecord(anyMkLst->pptyType());
				anyMkLst->toPPTY(pWriter);
				pWriter->EndRecord();
			}
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
					authorIdx = pReader->GetLong();
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
				default:
					break;
				}
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 100:
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
						case 0:
							id = pReader->GetString2();
							break;
						case 1:
							created = pReader->GetString2();
							break;
						case 2:
							authorId = pReader->GetString2();
							break;
						case 3:
							status = pReader->GetString2();
							break;
						case 4:
							startDate = pReader->GetString2();
							break;
						case 5:
							dueDate = pReader->GetString2();
							break;
						case 6:
							assignedTo = pReader->GetString2();
							break;
						case 7:
							title = pReader->GetString2();
							break;
						case 8:
							complete = pReader->GetLong();
							break;
						default:
							break;
						}
					}
					pReader->Seek(_end_rec2);
				}break;
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
					bModern = true;
				}break;
				case 2:
				{
					replyLst.Init();
					replyLst->fromPPTY(pReader);
					bModern = true;
				}break;
				case 3:
				{
					anyMkLst = new TextCharRangeMonikerList();
					anyMkLst->fromPPTY(pReader);
					bModern = true;
				}break;
				case 4:
				{
					anyMkLst = new SlideMonikerList();
					anyMkLst->fromPPTY(pReader);
					bModern = true;
				}break;
				case 5:
				{
					anyMkLst = new DrawingElementMonikerList();
					anyMkLst->fromPPTY(pReader);
					bModern = true;
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

		for (size_t i = 0; i < oNodes.size(); ++i)
		{
			XmlUtils::CXmlNode& oCm = oNodes[i];

			std::wstring strName = XmlUtils::GetNameNoNS(oCm.GetName());

			if (strName == L"cm")
			{
				m_arComments.emplace_back();

				if (XmlUtils::GetNamespace(oCm.GetName()) == L"p188")
					m_arComments.back().bModern = true;

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
		if (bModern) 	return OOX::Presentation::FileTypes::ModernComments;
		else 	 	 	return OOX::Presentation::FileTypes::SlideComments;
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
		pWriter->StartNode(std::wstring(bModern ? L"p188" : L"p") + L":cmLst");

		pWriter->StartAttributes();
			pWriter->WriteAttribute(L"xmlns:a", PPTX::g_Namespaces.a.m_strLink);
			pWriter->WriteAttribute(L"xmlns:r", PPTX::g_Namespaces.r.m_strLink);
			pWriter->WriteAttribute(L"xmlns:p", PPTX::g_Namespaces.p.m_strLink);
			pWriter->WriteAttribute(L"xmlns:m", PPTX::g_Namespaces.m.m_strLink);
			pWriter->WriteAttribute(L"xmlns:w", PPTX::g_Namespaces.w.m_strLink);
			if (bModern)
				pWriter->WriteAttribute(L"xmlns:p188", L"http://schemas.microsoft.com/office/powerpoint/2018/8/main");
			pWriter->EndAttributes();

			for (auto comm : m_arComments)
			{
				comm.bModern = bModern;
				comm.toXmlWriter(pWriter);
			}

		pWriter->EndNode(std::wstring(bModern ? L"p188" : L"p") + L":cmLst");
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

					m_arComments.emplace_back();
					m_arComments.back().fromPPTY(pReader);
					
					if (m_arComments.back().bModern)
						bModern = true;
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
		
		PPTX::Document* pptxDocument = dynamic_cast<PPTX::Document*>(this->m_pMainDocument);
		if (!bModern && (pptxDocument) && (pptxDocument->main))
		{
			if (false == pptxDocument->main->commentAuthors.IsInit())
			{
				pptxDocument->main->commentAuthors = new PPTX::Authors(pptxDocument);
			}
			PPTX::Authors *authors = pptxDocument->main->commentAuthors.GetPointer();
			int ind = 1;
			for (auto &comm : m_arComments)
			{
				if (true == comm.idx.IsInit() && true == comm.authorIdx.IsInit())
					break;

				std::map<std::wstring, int>::iterator pFind = authors->mapAuthors.find(*comm.authorId);

				if (pFind != authors->mapAuthors.end())
				{
					comm.authorIdx = pFind->second;
				}
				else
				{
					comm.authorIdx = authors->mapAuthors.size() + 1;
					authors->mapAuthors.insert(std::make_pair(*comm.authorId, *comm.authorIdx));
				}
				//todooo idx new generate ??? (now id->idx)
			}
		}
	}
} // namespace PPTX
