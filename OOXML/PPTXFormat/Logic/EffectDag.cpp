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

				pWriter->StartRecord(0);				
				size_t len = Effects.size();
				pWriter->WriteSize_t1(0, len);
				
				for (size_t i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(1, Effects[i]); // id неважен
				}
				pWriter->EndRecord();

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
					case 0:
					{
						ULONG count_effects = pReader->GetULong();
						for (ULONG _eff = 0; _eff < count_effects; ++_eff)
						{
							pReader->Skip(1); // type 

							Effects.push_back(UniEffect());
							Effects.back().fromPPTY(pReader);

							if (false == Effects.back().is_init())
							{
								Effects.pop_back();
							}
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
