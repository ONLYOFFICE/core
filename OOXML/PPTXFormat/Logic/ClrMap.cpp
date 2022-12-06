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

#include "ClrMap.h"

namespace PPTX
{
	namespace Logic
	{
		ClrMap::ClrMap(std::wstring name)
		{
			m_name = name;
			SetMap();
		}
		OOX::EElementType ClrMap::getType() const
		{
			return OOX::et_a_clrMap;
		}
		ClrMap& ClrMap::operator=(const ClrMap& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			m_name = oSrc.m_name;

			m_arColorMap = oSrc.m_arColorMap;

			return *this;
		}
		void ClrMap::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();
			SetMap();

			m_arColorMap.clear();

			ReadAttributes(oReader);
		}
		void ClrMap::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			Limit::ColorSchemeIndex lColorIndex_accent1;
			Limit::ColorSchemeIndex lColorIndex_accent2;
			Limit::ColorSchemeIndex lColorIndex_accent3;
			Limit::ColorSchemeIndex lColorIndex_accent4;
			Limit::ColorSchemeIndex lColorIndex_accent5;
			Limit::ColorSchemeIndex lColorIndex_accent6;
			Limit::ColorSchemeIndex lColorIndex_bg1;
			Limit::ColorSchemeIndex lColorIndex_bg2;
			Limit::ColorSchemeIndex lColorIndex_tx1;
			Limit::ColorSchemeIndex lColorIndex_tx2;
			Limit::ColorSchemeIndex lColorIndex_folHlink;
			Limit::ColorSchemeIndex lColorIndex_hlink;

			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"accent1", lColorIndex_accent1)
				WritingElement_ReadAttributes_Read_else_if( oReader, L"accent2", lColorIndex_accent2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"accent3", lColorIndex_accent3 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"accent4", lColorIndex_accent4 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"accent5", lColorIndex_accent5 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"accent6", lColorIndex_accent6 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"bg1", lColorIndex_bg1 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"bg2", lColorIndex_bg2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"tx1", lColorIndex_tx1 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"tx2", lColorIndex_tx2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"folHlink", lColorIndex_folHlink )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"hlink", lColorIndex_hlink )

				WritingElement_ReadAttributes_Read_else_if( oReader, L"t1", lColorIndex_tx1 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"t2", lColorIndex_tx2 )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"followedHyperlink", lColorIndex_folHlink )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"hyperlink", lColorIndex_hlink )
			WritingElement_ReadAttributes_End_No_NS( oReader )

			m_arColorMap.push_back(std::make_pair(L"bg1", lColorIndex_bg1));
			m_arColorMap.push_back(std::make_pair(L"tx1", lColorIndex_tx1));
			m_arColorMap.push_back(std::make_pair(L"bg2", lColorIndex_bg2));
			m_arColorMap.push_back(std::make_pair(L"tx2", lColorIndex_tx2));
			m_arColorMap.push_back(std::make_pair(L"accent1", lColorIndex_accent1));
			m_arColorMap.push_back(std::make_pair(L"accent2", lColorIndex_accent2));
			m_arColorMap.push_back(std::make_pair(L"accent3", lColorIndex_accent3));
			m_arColorMap.push_back(std::make_pair(L"accent4", lColorIndex_accent4));
			m_arColorMap.push_back(std::make_pair(L"accent5", lColorIndex_accent5));
			m_arColorMap.push_back(std::make_pair(L"accent6", lColorIndex_accent6));
			m_arColorMap.push_back(std::make_pair(L"folHlink", lColorIndex_folHlink));
			m_arColorMap.push_back(std::make_pair(L"hlink", lColorIndex_hlink));
		}
		void ClrMap::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();
			SetMap();

			m_arColorMap.clear();

			Limit::ColorSchemeIndex lColorIndex;

			if (m_name == L"w:clrSchemeMapping")
			{
				lColorIndex._set(node.GetAttribute(L"w:bg1"));				m_arColorMap.push_back(std::make_pair(L"bg1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:t1"));				m_arColorMap.push_back(std::make_pair(L"tx1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:bg2"));				m_arColorMap.push_back(std::make_pair(L"bg2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:t2"));				m_arColorMap.push_back(std::make_pair(L"tx2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent1"));			m_arColorMap.push_back(std::make_pair(L"accent1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent2"));			m_arColorMap.push_back(std::make_pair(L"accent2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent3"));			m_arColorMap.push_back(std::make_pair(L"accent3", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent4"));			m_arColorMap.push_back(std::make_pair(L"accent4", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent5"));			m_arColorMap.push_back(std::make_pair(L"accent5", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:accent6"));			m_arColorMap.push_back(std::make_pair(L"accent6", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:hyperlink"));		m_arColorMap.push_back(std::make_pair(L"hlink", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"w:followedHyperlink"));m_arColorMap.push_back(std::make_pair(L"folHlink", lColorIndex));
			}
			else
			{
				lColorIndex._set(node.GetAttribute(L"bg1"));		m_arColorMap.push_back(std::make_pair(L"bg1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"tx1"));		m_arColorMap.push_back(std::make_pair(L"tx1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"bg2"));		m_arColorMap.push_back(std::make_pair(L"bg2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"tx2"));		m_arColorMap.push_back(std::make_pair(L"tx2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent1"));	m_arColorMap.push_back(std::make_pair(L"accent1", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent2"));	m_arColorMap.push_back(std::make_pair(L"accent2", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent3"));	m_arColorMap.push_back(std::make_pair(L"accent3", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent4"));	m_arColorMap.push_back(std::make_pair(L"accent4", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent5"));	m_arColorMap.push_back(std::make_pair(L"accent5", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"accent6"));	m_arColorMap.push_back(std::make_pair(L"accent6", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"hlink"));		m_arColorMap.push_back(std::make_pair(L"hlink", lColorIndex));
				lColorIndex._set(node.GetAttribute(L"folHlink"));	m_arColorMap.push_back(std::make_pair(L"folHlink", lColorIndex));
			}
		}
		std::wstring ClrMap::toXML() const
		{
			XmlUtils::CAttribute oAttr;

			if (m_name == L"w:clrSchemeMapping")
			{
				for (size_t i = 0; i < m_arColorMap.size(); ++i)
				{
					std::wstring att_name, att_val;

							if (m_arColorMap[i].first == L"tx1")		att_name = L"w:t1";
					else	if (m_arColorMap[i].first == L"tx2")		att_name = L"w:t2";
					else	if (m_arColorMap[i].first == L"hlink")		att_name = L"w:hyperlink";
					else	if (m_arColorMap[i].first == L"folHlink")	att_name = L"w:followedHyperlink";
					else att_name = L"w:" + m_arColorMap[i].first;

							if (m_arColorMap[i].second.get() == L"lt1")		att_val = L"light1";
					else	if (m_arColorMap[i].second.get() == L"lt2")		att_val = L"light2";
					else	if (m_arColorMap[i].second.get() == L"dk1")		att_val = L"dark1";
					else	if (m_arColorMap[i].second.get() == L"dk2")		att_val = L"dark2";
					else	if (m_arColorMap[i].second.get() == L"hlink")	att_val = L"hyperlink";
					else	if (m_arColorMap[i].second.get() == L"folHlink")att_val = L"followedHyperlink";
					else att_val = m_arColorMap[i].second.get();

					oAttr.Write(att_name, att_val);
				}
			}
			else
			{
				for (size_t i = 0; i < m_arColorMap.size(); ++i)
				{
					oAttr.Write(m_arColorMap[i].first, m_arColorMap[i].second.get());
				}
			}

			return XmlUtils::CreateNode(m_name, oAttr);
		}
		void ClrMap::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);

			pWriter->StartAttributes();
			for (size_t i = 0; i < m_arColorMap.size(); ++i)
			{
				pWriter->WriteAttribute(m_arColorMap[i].first, m_arColorMap[i].second.get());
			}
			pWriter->EndAttributes();

			pWriter->EndNode(m_name);
		}
		std::wstring ClrMap::GetColorSchemeIndex(const std::wstring& str)const
		{
			std::map<std::wstring, int>::const_iterator pPair = m_mapColorMap.find(str);
			if (m_mapColorMap.end() != pPair)
			{
				return m_arColorMap[pPair->second].second.get();
			}
			return L"";
		}
		void ClrMap::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

			for (size_t i = 0; i < m_arColorMap.size(); ++i)
			{
				pWriter->WriteLimit1(SchemeClr_GetBYTECode(m_arColorMap[i].first), m_arColorMap[i].second);
			}

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
		}
		void ClrMap::Insert(const std::wstring& prop, const Limit::ColorSchemeIndex& val)
		{
			std::map<std::wstring, int>::iterator pPair = m_mapColorMap.find(prop);

			if (m_mapColorMap.end() != pPair)
			{
				if (pPair->second >= (int)m_arColorMap.size())
					m_arColorMap.resize(12);
				m_arColorMap[pPair->second] = std::make_pair(prop, val);
			}
		}
		bool ClrMap::Find(const std::wstring& prop, Limit::ColorSchemeIndex& val) const
		{
			std::map<std::wstring, int>::const_iterator pPair = m_mapColorMap.find(prop);
			if (m_mapColorMap.end() != pPair)
			{
				if (pPair->second < m_arColorMap.size())
				{
					val = m_arColorMap[pPair->second].second;
					return true;
				}
			}
			return false;
		}
		void ClrMap::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _e = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start sttribute

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				BYTE ind = pReader->GetUChar();
				Limit::ColorSchemeIndex _index;
				_index.SetBYTECode(ind);

				Insert(SchemeClr_GetStringCode(_at), _index);
			}

			pReader->Seek(_e);
		}
		void ClrMap::FillParentPointersForChilds(){};
		void ClrMap::SetMap()
		{
			if (false == m_mapColorMap.empty()) return;

			m_mapColorMap.insert(std::make_pair(L"bg1", 0));
			m_mapColorMap.insert(std::make_pair(L"tx1", 1));
			m_mapColorMap.insert(std::make_pair(L"bg2", 2));
			m_mapColorMap.insert(std::make_pair(L"tx2", 3));
			m_mapColorMap.insert(std::make_pair(L"accent1", 4));
			m_mapColorMap.insert(std::make_pair(L"accent2", 5));
			m_mapColorMap.insert(std::make_pair(L"accent3", 6));
			m_mapColorMap.insert(std::make_pair(L"accent4", 7));
			m_mapColorMap.insert(std::make_pair(L"accent5", 8));
			m_mapColorMap.insert(std::make_pair(L"accent6", 9));
			m_mapColorMap.insert(std::make_pair(L"hlink", 10));
			m_mapColorMap.insert(std::make_pair(L"folHlink", 11));
		}
	} // namespace Logic
} // namespace PPTX
