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
#include "EffectStyle.h"

namespace PPTX
{
	namespace Logic
	{
		EffectStyle& EffectStyle::operator=(const EffectStyle& oSrc)
		{
			parentFile = oSrc.parentFile;
			parentElement = oSrc.parentElement;

			Effects = oSrc.Effects;
			scene3d = oSrc.scene3d;
			sp3d = oSrc.sp3d;
			return *this;
		}
		void EffectStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			std::vector<XmlUtils::CXmlNode> oNodes;
			if (node.GetNodes(L"*", oNodes))
			{
				size_t nCount = oNodes.size();
				for (size_t i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode& oNode = oNodes[i];

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					 if (L"scene3d" == strName)
						scene3d = oNode;
					else if (L"sp3d" == strName)
						sp3d = oNode;
					else if (L"effectDag" == strName ||
							L"effectLst" == strName)
					{
						Effects.fromXML(oNode);
					}
				}
			}
			FillParentPointersForChilds();
		}
		void EffectStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(L"a:effectStyle");
			pWriter->EndAttributes();

			Effects.toXmlWriter(pWriter);
			pWriter->Write(scene3d);
			pWriter->Write(sp3d);

			pWriter->EndNode(L"a:effectStyle");
		}
		void EffectStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();
				if (strName == L"a:scene3d")
					scene3d = oReader;
				else if (strName == L"a:sp3d")
					sp3d = oReader;
				else
					Effects.fromXML(oReader);
			}
			FillParentPointersForChilds();
		}
		void EffectStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord1(0, Effects);
			pWriter->WriteRecord2(1, scene3d);
			pWriter->WriteRecord2(2, sp3d);
		}
		void EffectStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
				case 0:
				{
					Effects.fromPPTY(pReader);					
				}break;
				case 1:
				{
					scene3d = new Logic::Scene3d();
					scene3d->fromPPTY(pReader);					
				}break;
				case 2:
				{
					sp3d = new Logic::Sp3d();
					sp3d->fromPPTY(pReader);					
				}break;
				default:
					break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void EffectStyle::FillParentPointersForChilds()
		{
			Effects.SetParentPointer(this);
			if(scene3d.IsInit())
				scene3d->SetParentPointer(this);
			if(sp3d.IsInit())
				sp3d->SetParentPointer(this);
		}

	} // namespace Logic
} // namespace PPTX
