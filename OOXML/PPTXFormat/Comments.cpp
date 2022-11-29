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

#include "Comments.h"
#include "FileTypes.h"

namespace PPTX
{
	namespace Logic
	{
			void Comment::fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"authorId", authorId);
				XmlMacroReadAttributeBase(node, L"dt", dt);
				XmlMacroReadAttributeBase(node, L"idx", idx);

				XmlUtils::CXmlNode oNodePos = node.ReadNode(_T("p:pos"));
				if (oNodePos.IsValid())
				{
					XmlMacroReadAttributeBase(oNodePos, L"x", pos_x);
					XmlMacroReadAttributeBase(oNodePos, L"y", pos_y);
				}

				XmlUtils::CXmlNode oNodeText = node.ReadNode(_T("p:text"));
				if (oNodeText.IsValid())
					text = oNodeText.GetTextExt();

				XmlUtils::CXmlNode oNodeExtLst = node.ReadNode(_T("p:extLst"));

				bool bIsFound1 = false;
				bool bIsFound2 = false;
				if (oNodeExtLst.IsValid())
				{
					XmlUtils::CXmlNodes oNodesExt;
					if (oNodeExtLst.GetNodes(_T("p:ext"), oNodesExt))
					{
						int nCountExts = oNodesExt.GetCount();
						for (int nIndex = 0; nIndex < nCountExts; ++nIndex)
						{
							XmlUtils::CXmlNode oNodeExt;
							oNodesExt.GetAt(nIndex, oNodeExt);

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

					XmlUtils::CXmlNode oNodeExt = oNodeExtLst.ReadNode(_T("p:ext"));
					if (oNodeExt.IsValid())
					{
						XmlUtils::CXmlNode oNodeTI = oNodeExt.ReadNode(_T("p15:threadingInfo"));
						if (oNodeTI.IsValid())
						{
							XmlUtils::CXmlNode oNodeParent = oNodeTI.ReadNode(_T("p15:parentCm"));

							XmlMacroReadAttributeBase(oNodeParent, L"authorId", parentAuthorId);
							XmlMacroReadAttributeBase(oNodeParent, L"idx", parentCommentId);
						}
					}
				}
				//check comment guid
				if(!(additional_data.IsInit() && 0 == additional_data->compare(0, 13, L"teamlab_data:") && std::wstring::npos != additional_data->find(L"4;38;")))
				{
					if(!additional_data.IsInit())
					{
						additional_data = L"teamlab_data:";
					}
					if(':' != additional_data->back() && ';' != additional_data->back())
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

				bool bIsExtLst = false;
				if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init() || additional_data.is_init())
					bIsExtLst = true;

				if (bIsExtLst)
					pWriter->WriteString(_T("<p:extLst>"));

				if ((parentAuthorId.is_init() && parentCommentId.is_init()) || timeZoneBias.is_init())
				{
					pWriter->WriteString(_T("<p:ext uri=\"{C676402C-5697-4E1C-873F-D02D1690AC5C}\">\
<p15:threadingInfo xmlns:p15=\"http://schemas.microsoft.com/office/powerpoint/2012/main\""));
					if(timeZoneBias.is_init())
					{
						pWriter->WriteString(_T(" timeZoneBias=\""));
						pWriter->WriteINT(timeZoneBias.get());
						pWriter->WriteString(_T("\""));
					}
					pWriter->WriteString(_T(">"));
					if(parentAuthorId.is_init() && parentCommentId.is_init())
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
				pWriter->WriteString2(1, dt);
				pWriter->WriteInt2(2, idx);

				pWriter->WriteInt2(3, pos_x);
				pWriter->WriteInt2(4, pos_y);

				pWriter->WriteString2(5, text);

				pWriter->WriteInt2(6, parentAuthorId);
				pWriter->WriteInt2(7, parentCommentId);

				pWriter->WriteString2(8, additional_data);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->StartRecord(0);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(9, timeZoneBias);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
				pWriter->EndRecord();
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
			void Comment::FillParentPointersForChilds()
			{
			}
	}

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

		XmlUtils::CXmlNodes oNodes;
		oNode.GetNodes(_T("p:cm"), oNodes);
		int nCount = oNodes.GetCount();
		for (int i = 0; i < nCount; ++i)
		{
			XmlUtils::CXmlNode oCm;
			oNodes.GetAt(i, oCm);

			PPTX::Logic::Comment elm;
			m_arComments.push_back(elm);
			m_arComments.back().fromXML(oCm);
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
