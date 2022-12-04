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

#include "Hyperlink.h"

#include "../Slide.h"
#include "../SlideMaster.h"
#include "../SlideLayout.h"
#include "../Theme.h"
#include "../../DocxFormat/External/HyperLink.h"

namespace PPTX
{
	namespace Logic
	{
		Hyperlink::Hyperlink(const std::wstring& name)
		{
			m_name = name;
		}
		OOX::EElementType Hyperlink::getType () const
		{
			return OOX::et_a_hyperlink;
		}
		void Hyperlink::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = XmlUtils::GetNameNoNS(oReader.GetName());

			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if (sName == L"snd")
				{
					snd	= oReader;
					break;
				}

			}
		}
		void Hyperlink::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start	( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"r:id"), id )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"), id )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"invalidUrl"), invalidUrl )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"action"), action )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"tgtFrame"), tgtFrame )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"tooltip"), tooltip )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"history"), history)
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"highlightClick"), highlightClick )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"endSnd"), endSnd )
			WritingElement_ReadAttributes_End	( oReader )
		}
		void Hyperlink::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = XmlUtils::GetNameNoNS(node.GetName());

			std::wstring sSndNodeName = (L"snd");
			snd	= node.ReadNodeNoNS(sSndNodeName);

			XmlMacroReadAttributeBase(node, L"r:id", id);
			XmlMacroReadAttributeBase(node, L"invalidUrl", invalidUrl);
			XmlMacroReadAttributeBase(node, L"action", action);
			XmlMacroReadAttributeBase(node, L"tgtFrame", tgtFrame);
			XmlMacroReadAttributeBase(node, L"tooltip", tooltip);
			XmlMacroReadAttributeBase(node, L"history", history);
			XmlMacroReadAttributeBase(node, L"highlightClick", highlightClick);
			XmlMacroReadAttributeBase(node, L"endSnd", endSnd);

			if (false == id.IsInit())
			{
				XmlMacroReadAttributeBase( node, L"relationships:id", id );
			}
		}
		void Hyperlink::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:" + m_name);

			pWriter->StartAttributes();
			pWriter->WriteAttribute	(L"r:id",		id);
			pWriter->WriteAttribute2(L"invalidUrl", invalidUrl);
			pWriter->WriteAttribute2(L"action",		action);
			pWriter->WriteAttribute2(L"tgtFrame",	tgtFrame);

			if (tooltip.IsInit())
				pWriter->WriteAttribute(L"tooltip", XmlUtils::EncodeXmlString(*tooltip));

			pWriter->WriteAttribute(L"history",			history);
			pWriter->WriteAttribute(L"highlightClick",	highlightClick);
			pWriter->WriteAttribute(L"endSnd",			endSnd);
			pWriter->EndAttributes();

			pWriter->Write(snd);

			pWriter->EndNode(L"a:" + m_name);
		}
		void Hyperlink::FillParentPointersForChilds()
		{
			if(snd.IsInit())
				snd->SetParentPointer(this);
		}
		std::wstring Hyperlink::GetPathFromId(OOX::IFileContainer* pRels, const std::wstring & rId)const
		{
			if (rId.empty()) return L"";

			OOX::RId rid(rId);

			std::wstring sLink = L"";
			if (pRels != NULL)
			{
				smart_ptr<OOX::HyperLink> p = pRels->Get<OOX::HyperLink>(rid);
				if (p.is_init())
					sLink = p->Uri().m_strFilename;
			}
            if(sLink.empty())
			{
				 if(parentFileIs<FileContainer>())
					 sLink = parentFileAs<FileContainer>().GetLinkFromRId(rid);
			}

            XmlUtils::replace_all(sLink, L"\\",     L"/");
            XmlUtils::replace_all(sLink, L"//",     L"/");
            XmlUtils::replace_all(sLink, L"http:/", L"http://");
            XmlUtils::replace_all(sLink, L"https:/",L"https://");
            XmlUtils::replace_all(sLink, L"ftp:/",  L"ftp://");
            XmlUtils::replace_all(sLink, L"file:/", L"file://");

			return sLink;
		}		
		void Hyperlink::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			OOX::IFileContainer* pRels = pWriter->GetRels().GetPointer();

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			if (id.is_init())
			{
				std::wstring hyperlinkPath = GetPathFromId(pRels, id.IsInit() ? *id : L"");
				pWriter->WriteString1(0, hyperlinkPath);
			}

			pWriter->WriteString2(1, invalidUrl);
			pWriter->WriteString2(2, action);
			pWriter->WriteString2(3, tgtFrame);
			pWriter->WriteString2(4, tooltip);
			pWriter->WriteBool2(5, history);
			pWriter->WriteBool2(6, highlightClick);
			pWriter->WriteBool2(7, endSnd);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, snd);
		}
		void Hyperlink::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			bool bIsPresentUrl = false;
			std::wstring strUrl;

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
					{
						strUrl = pReader->GetString2(true);
						bIsPresentUrl = true;
						break;
					}
					case 1:
					{
						invalidUrl = pReader->GetString2(true);
						break;
					}
					case 2:
					{
						action = pReader->GetString2();
						break;
					}
					case 3:
					{
						tgtFrame = pReader->GetString2();
						break;
					}
					case 4:
					{
						tooltip = pReader->GetString2();
						break;
					}
					case 5:
					{
						history = pReader->GetBool();
						break;
					}
					case 6:
					{
						highlightClick = pReader->GetBool();
						break;
					}
					case 7:
					{
						endSnd = pReader->GetBool();
						break;
					}
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
						snd = new PPTX::Logic::WavAudioFile(L"snd");
						snd->fromPPTY(pReader);
						break;
					}
					default:
					{
						break;
					}
				}
			}
			pReader->Seek(_end_rec);

			if (bIsPresentUrl)
			{
				if (strUrl == _T(""))
					id = _T("");
				else
				{
					unsigned int lId = pReader->m_pRels->WriteHyperlink(strUrl, action.is_init());

					id = L"rId" + std::to_wstring(lId);
				}
			}

		}
	} // namespace Logic
} // namespace PPTX

