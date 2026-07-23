/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "TgtEl.h"

namespace PPTX
{
	namespace Logic
	{
		void TgtEl::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNode oNode;
			if (node.GetNode(_T("p:inkTgt"), oNode))
				inkTgt = oNode.ReadAttributeBase(L"spid");
			else if(node.GetNode(_T("p:sndTgt"), oNode))
			{
				XmlMacroReadAttributeBase(oNode, L"r:embed", embed);
				XmlMacroReadAttributeBase(oNode, L"name", name);
				XmlMacroReadAttributeBase(oNode, L"builtIn", builtIn);
			}
			else if(node.GetNode(_T("p:spTgt"), oNode))
				spTgt = oNode;

			FillParentPointersForChilds();
		}
		std::wstring TgtEl::toXML() const
		{
			if (inkTgt.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("spid"), inkTgt);

				return XmlUtils::CreateNode(_T("p:tgtEl"), XmlUtils::CreateNode(_T("p:inkTgt"), oAttr));
			}
			if (embed.IsInit())
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("r:embed"), embed->ToString());
				oAttr.Write(_T("name"), name);
				oAttr.Write(_T("builtIn"), builtIn);

				return XmlUtils::CreateNode(_T("p:tgtEl"), XmlUtils::CreateNode(_T("p:sndTgt"), oAttr));
			}
			if (spTgt.IsInit())
			{
				return XmlUtils::CreateNode(_T("p:tgtEl"), spTgt->toXML());
			}
			return _T("<p:tgtEl><p:sldTgt/></p:tgtEl>");
		}
		void TgtEl::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!pWriter) return;
			pWriter->WriteString(toXML());
		}
		void TgtEl::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			if (!pWriter) return;

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString2(0, inkTgt);

				pWriter->WriteString2(1, name);
				pWriter->WriteBool2(2, builtIn);

				if (embed.IsInit())
				{
					OOX::IFileContainer* pRels = dynamic_cast<OOX::IFileContainer*>(const_cast<PPTX::WrapperFile*>(parentFile));

					if (!pRels && (pWriter))
					{
						pRels = pWriter->GetRelsPtr();
					}

					smart_ptr<OOX::Media> pMedia;

					if (pRels != NULL)
						pMedia = pRels->Get<OOX::Media>(embed.get());

					if (pMedia.IsInit())
					{
						NSShapeImageGen::CMediaInfo oId = pWriter->m_pCommon->m_pMediaManager->WriteMedia(pMedia->filename().GetPath());
						std::wstring s = oId.GetPath2();

						pWriter->WriteString1(3, s);
					}
				}
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, spTgt);
		}
		void TgtEl::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // attribute start

			std::wstring file_name_embed;
			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				else if (0 == _at)	inkTgt = pReader->GetString2();
				else if (1 == _at)	name = pReader->GetString2();
				else if (2 == _at)	builtIn = pReader->GetBool();
				else if (3 == _at)	file_name_embed = pReader->GetString2();

			}
			while (pReader->GetPos() < end)
			{
				BYTE _rec = pReader->GetUChar();

				switch (_rec)
				{
					case 0:
					{
						spTgt.Init();
						spTgt->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();
					}break;
				}
			}
			pReader->Seek(end);

			if (false == file_name_embed.empty())
			{
				std::wstring strPath = pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + file_name_embed;

				NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = pReader->m_pRels->WriteMedia(strPath, 1);

				if (oRelsGeneratorInfo.nImageRId > 0)
				{
					embed = new OOX::RId(oRelsGeneratorInfo.nImageRId);
				}
			}
		}
		void TgtEl::FillParentPointersForChilds()
		{
			if(spTgt.IsInit())
				spTgt->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
