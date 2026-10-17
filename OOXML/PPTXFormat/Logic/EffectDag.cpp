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
#include "EffectDag.h"

namespace PPTX
{
	namespace Logic
	{
		void EffectDag::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
            Effects.clear();
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
                std::wstring sName = oReader.GetName();
				
				UniEffect uni;
				Effects.push_back(uni);
				Effects.back().fromXML(oReader);
			}
		}
		void EffectDag::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name	= node.GetName();
            XmlMacroReadAttributeBase(node, L"name", name);
            XmlMacroReadAttributeBase(node, L"type", type);
            Effects.clear();
            XmlMacroLoadArray(node, _T("*"), Effects, UniEffect);

			FillParentPointersForChilds();
		}

		std::wstring EffectDag::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("name"), name);
			oAttr.WriteLimitNullable(_T("type"), type);

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Effects);

			return XmlUtils::CreateNode(m_name.empty() ? L"a:effectDag" : m_name, oAttr, oValue);
		}
		void EffectDag::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			//effectLst, effectDag, cont

			pWriter->StartNode(m_name.empty() ? L"a:effectDag" : m_name);
				pWriter->StartAttributes();
					pWriter->WriteAttribute(L"name", name);
					pWriter->WriteAttribute(L"type", type);
				pWriter->EndAttributes();

				for (size_t i = 0; i < Effects.size(); i++)
				{
					Effects[i].toXmlWriter(pWriter);
				}
			pWriter->EndNode(m_name.empty() ? L"a:effectDag" : m_name);
		}
		void EffectDag::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(EFFECT_TYPE_DAG);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, name);
			pWriter->WriteLimit2(1, type);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			
				for (size_t i = 0; i < Effects.size(); ++i)
				{
					pWriter->WriteRecord1(1, Effects[i]); // id doesn't matter
				}

			pWriter->EndRecord();
		}
		void EffectDag::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			pReader->Skip(4); // len
			BYTE _type = pReader->GetUChar(); 
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1);

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				switch (_at)
				{
					case 0:
						name = pReader->GetString2(); break;
					case 1:
						type = pReader->GetUChar(); break; 
				}
			}
			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 1:
					{
						Effects.emplace_back();
						Effects.back().fromPPTY(pReader);
						if (false == Effects.back().is_init())
						{
							Effects.pop_back();
						}
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}

		void EffectDag::FillParentPointersForChilds()
		{
            size_t count = Effects.size();
			for(size_t i = 0; i < count; ++i)
				Effects[i].SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
