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
#include "Controls.h"
#include "../DrawingConverter/ASCOfficeDrawingConverter.h"
#include "../../DocxFormat/Media/ActiveX.h"

#include "../Slide.h"

namespace PPTX
{
	namespace Logic
	{
		void Controls::fromXML(XmlUtils::CXmlNode& node)
		{
			arrControls.clear();

			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (strName == L"control")
					{
						Control elem(oNode);
						arrControls.push_back(elem);
					}
					else if (L"AlternateContent" == strName)
					{
						XmlUtils::CXmlNode oNodeChoice, oNodeFallback;
						XmlUtils::CXmlNode oNodeFallbackControl;

						bool resFallback, resChoice = oNode.GetNode(L"mc:Choice", oNodeChoice);
						if (oNode.GetNode(L"mc:Fallback", oNodeFallback))
						{
							resFallback = oNodeFallback.GetNode(L"p:control", oNodeFallbackControl);
						}

						std::wstring sRequires;
						if (resChoice)
						{
							oNodeChoice.GetAttributeIfExist(L"Requires", sRequires);
						}
						if (L"v" != sRequires || !resFallback)
						{
							XmlUtils::CXmlNode oNodeChoiceControl;
							if (oNodeChoice.GetNode(L"p:control", oNodeChoiceControl))
							{
								Control elem(oNodeChoiceControl);
								arrControls.push_back(elem);
								continue;
							}
						}
						Control elem(oNodeFallbackControl);
						arrControls.push_back(elem);
					}
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring Controls::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			oValue.WriteArray(arrControls);

			return XmlUtils::CreateNode(L"p:controls", oValue);
		}

		void Controls::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:controls");
			pWriter->EndAttributes();

			for (size_t i = 0; i < arrControls.size(); ++i)
				arrControls[i].toXmlWriter(pWriter);

			pWriter->EndNode(L"p:controls");
		}
		void Controls::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			for (size_t i = 0; i < arrControls.size(); i++)
			{
				pWriter->WriteRecord1(0, arrControls[i]);
			}
		}
		void Controls::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						arrControls.push_back(Control(m_pMainDocument));
						arrControls.back().fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
		}
//------------------------------------------------------------------------------------------------------------------------
		void Control::fromXML(XmlUtils::CXmlNode& node)
		{
			name = node.GetAttribute(L"name");
			spid = node.GetAttribute(L"spid");

			XmlMacroReadAttributeBase(node, L"r:id", rId);
			XmlMacroReadAttributeBase(node, L"imgW", width);
			XmlMacroReadAttributeBase(node, L"imgH", height);
			XmlMacroReadAttributeBase(node, L"showAsIcon", showAsIcon);

			if (false == rId.IsInit())
				XmlMacroReadAttributeBase(node, L"id", rId);
			pic = node.ReadNode(L"p:pic");

			FillParentPointersForChilds();
		}

		std::wstring Control::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(L"name", name);

			if (rId.IsInit())	oAttr.Write(L"r:id", rId->ToString());
			if (width.IsInit())	oAttr.Write(L"imgW", std::to_wstring(*width));
			if (height.IsInit())oAttr.Write(L"imgH", std::to_wstring(*height));

			oAttr.Write(L"spid", spid);

			return XmlUtils::CreateNode(L"p:control", oAttr);
		}
		void Control::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"p:control");

			pWriter->StartAttributes();
			pWriter->WriteAttribute(L"name", name);
			pWriter->WriteAttribute(L"spid", spid);

			if (rId.IsInit())  pWriter->WriteAttribute(L"r:id", rId->ToString());
			if (width.IsInit())pWriter->WriteAttribute(L"imgW", std::to_wstring(*width));
			if (height.IsInit())pWriter->WriteAttribute(L"imgH", std::to_wstring(*height));
			pWriter->WriteAttribute(L"showAsIcon", showAsIcon);
			pWriter->EndAttributes();

			pWriter->Write(pic);

			pWriter->EndNode(L"p:control");
		}
		std::wstring Control::GetVmlXmlBySpid(std::wstring spid, smart_ptr<OOX::IFileContainer> & rels)  const
		{
			std::wstring xml;
			if(parentFileIs<PPTX::Slide>() && parentFileAs<PPTX::Slide>().Vml.IsInit())
			{
				xml		= parentFileAs<PPTX::Slide>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::Slide>().Vml.smart_dynamic_cast<OOX::IFileContainer>();
			}
			else if(parentFileIs<PPTX::SlideLayout>() && parentFileAs<PPTX::SlideLayout>().Vml.IsInit())
			{
				xml= parentFileAs<PPTX::SlideLayout>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideLayout>().Vml.smart_dynamic_cast<OOX::IFileContainer>();
			}
			else if(parentFileIs<PPTX::SlideMaster>() && parentFileAs<PPTX::SlideMaster>().Vml.IsInit())
			{
				xml = parentFileAs<PPTX::SlideMaster>().GetVmlXmlBySpid(spid);
				rels	= parentFileAs<PPTX::SlideMaster>().Vml.smart_dynamic_cast<OOX::IFileContainer>();
			}

			return xml;
		}
		void Control::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter)  const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, name);
				pWriter->WriteUInt2(1, width);
				pWriter->WriteUInt2(2, height);
				pWriter->WriteBool2(3, showAsIcon);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			if (pic.IsInit())
			{
				pWriter->WriteRecord2(0, pic);
			}
			else if (spid.IsInit())
			{
				std::wstring s = *spid;
				if (s.length() < 8) s = L"_x0000_s" + s;

				smart_ptr<OOX::IFileContainer> rels;
				std::wstring xml = GetVmlXmlBySpid(s, rels);

				if (false == xml.empty())
				{
					std::wstring temp = L"<v:object>";
					temp += xml;
					temp += L"</v:object>";

					NSBinPptxRW::CDrawingConverter oDrawingConverter;

					RELEASEOBJECT(oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager);
					oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = pWriter->m_pCommon->m_pMediaManager;

					std::wstring *main_props = NULL;

					oDrawingConverter.SetRels(rels);

					std::vector<nullable<PPTX::Logic::SpTreeElem>> elements;
					oDrawingConverter.ConvertVml(temp, elements);
					oDrawingConverter.m_pBinaryWriter->m_pCommon->m_pMediaManager = NULL;

					smart_ptr<OOX::IFileContainer> rels_old = pWriter->GetRels();
					pWriter->SetRels(rels);
					for (size_t i = 0; i < elements.size(); ++i)
					{
						pWriter->WriteRecord2(0, elements[i]);
					}
					pWriter->SetRels(rels_old);
				}
			}

			//----------------------------------
			smart_ptr<OOX::File> pFileControl;

			if (rId.IsInit() && parentFileIs<PPTX::Slide>())
			{
				pFileControl = parentFileAs<PPTX::Slide>().Find(rId.get());
			}
			smart_ptr<OOX::ActiveX_xml> pActiveX_xml = pFileControl.smart_dynamic_cast<OOX::ActiveX_xml>();

			if (pActiveX_xml.IsInit())
			{
				pWriter->StartRecord(1);
				pActiveX_xml->toPPTY(pWriter);
				pWriter->EndRecord();
			}
		}
		void Control::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				if (0 == _at)		name = pReader->GetString2();
				else if (1 == _at)	width = pReader->GetULong();
				else if (2 == _at)	height = pReader->GetULong();
				else if (3 == _at)	showAsIcon = pReader->GetBool();
				else
					break;
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						pReader->Skip(1); // type (0)
						LONG nElemLength = pReader->GetLong(); // len

						pic.Init();
						pic->fromPPTY(pReader);
					}break;
					case 1:
					{
						smart_ptr<OOX::ActiveX_xml> pActiveX_xml = new OOX::ActiveX_xml(m_pMainDocument);
						pActiveX_xml->fromPPTY(pReader);

						std::wstring sActiveXFileName = L"activeX" + std::to_wstring(pReader->m_nCountActiveX++) + L".xml";
						OOX::CPath sActiveXPath = pReader->m_pRels->m_pManager->GetDstFolder() + FILE_SEPARATOR_STR + L"activeX";
						
						NSDirectory::CreateDirectory(sActiveXPath.GetPath());
						
						OOX::CPath oActiveXRegPath = std::wstring(L"/ppt/activeX/");
						pActiveX_xml->write(sActiveXPath + FILE_SEPARATOR_STR + sActiveXFileName, oActiveXRegPath, *pReader->m_pRels->m_pManager->m_pContentTypes);

						std::wstring sActiveXRelsPath = L"../activeX/" + sActiveXFileName;
						size_t nRId = pReader->m_pRels->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/control", sActiveXRelsPath, L"");
						rId = OOX::RId(nRId);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(_end_rec);
		}
	} // namespace Logic
} // namespace PPTX
