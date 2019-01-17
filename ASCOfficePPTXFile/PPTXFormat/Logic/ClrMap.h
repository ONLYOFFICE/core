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
#pragma once
#ifndef PPTX_LOGIC_CLRMAP_INCLUDE_H_
#define PPTX_LOGIC_CLRMAP_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/ColorSchemeIndex.h"

namespace PPTX
{
	namespace Logic
	{
		class ClrMap : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(ClrMap)
			
			ClrMap(std::wstring name = L"p:clrMap")
			{
				m_name = name;
			}

			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_clrMap;
			}	
			ClrMap& operator=(const ClrMap& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				m_name = oSrc.m_name;
				
				ColorMap.clear();
				for (std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = oSrc.ColorMap.begin(); pPair != oSrc.ColorMap.end(); ++pPair)
				{
					ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(pPair->first, pPair->second));
				}
				return *this;
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name = oReader.GetName();

				ColorMap.clear();

				ReadAttributes(oReader);
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
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
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("accent1"), lColorIndex_accent1)
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("accent2"), lColorIndex_accent2 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("accent3"), lColorIndex_accent3 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("accent4"), lColorIndex_accent4 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("accent5"), lColorIndex_accent5 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("accent6"), lColorIndex_accent6 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("bg1"), lColorIndex_bg1 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("bg2"), lColorIndex_bg2 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("tx1"), lColorIndex_tx1 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("tx2"), lColorIndex_tx2 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("folHlink"), lColorIndex_folHlink )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("hlink"), lColorIndex_hlink )
					
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("t1"), lColorIndex_tx1 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("t2"), lColorIndex_tx2 )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("followedHyperlink"), lColorIndex_folHlink )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("hyperlink"), lColorIndex_hlink )
				WritingElement_ReadAttributes_End_No_NS( oReader )
				
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent1"), lColorIndex_accent1));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent2"), lColorIndex_accent2));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent3"), lColorIndex_accent3));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent4"), lColorIndex_accent4));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent5"), lColorIndex_accent5));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent6"), lColorIndex_accent6));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg1"), lColorIndex_bg1));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg2"), lColorIndex_bg2));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx1"), lColorIndex_tx1));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx2"), lColorIndex_tx2));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("folHlink"), lColorIndex_folHlink));
				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("hlink"), lColorIndex_hlink));
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name = node.GetName();

				ColorMap.clear();

				Limit::ColorSchemeIndex lColorIndex;

				if (m_name == L"w:clrSchemeMapping")
				{
					lColorIndex._set(node.GetAttribute(_T("w:accent1")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:accent2")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent2"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:accent3")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent3"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:accent4")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent4"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:accent5")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent5"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:accent6")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent6"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:bg1")));				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:bg2")));				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg2"), lColorIndex));
					
					lColorIndex._set(node.GetAttribute(_T("w:t1")));				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:t2")));				ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx2"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:followedHyperlink")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("folHlink"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("w:hyperlink")));			ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("hlink"), lColorIndex));
				}
				else
				{
					lColorIndex._set(node.GetAttribute(_T("accent1")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("accent2")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent2"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("accent3")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent3"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("accent4")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent4"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("accent5")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent5"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("accent6")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("accent6"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("bg1")));		ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("bg2")));		ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("bg2"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("tx1")));		ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx1"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("tx2")));		ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("tx2"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("folHlink")));ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("folHlink"), lColorIndex));
					lColorIndex._set(node.GetAttribute(_T("hlink")));	ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(_T("hlink"), lColorIndex));
				}
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				
				if (m_name == L"w:clrSchemeMapping")
				{
					for (std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = ColorMap.begin(); pPair != ColorMap.end(); ++pPair)
					{
						std::wstring att_name = L"w:" + pPair->first;
								if (pPair->first == L"tx1")			att_name = L"w:t1";
						else	if (pPair->first == L"tx2")			att_name = L"w:t2";
						else	if (pPair->first == L"folHlink")	att_name = L"w:followedHyperlink";
						else	if (pPair->first == L"hlink")		att_name = L"w:hyperlink";

						oAttr.Write(att_name, pPair->second.get());
					}
				}
				else
				{
					for (std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = ColorMap.begin(); pPair != ColorMap.end(); ++pPair)
					{
						oAttr.Write(pPair->first, pPair->second.get());
					}
				}

				return XmlUtils::CreateNode(m_name, oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(m_name);

				pWriter->StartAttributes();
				for (std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = ColorMap.begin(); pPair != ColorMap.end(); ++pPair)
				{
					pWriter->WriteAttribute(pPair->first, pPair->second.get());
				}
				pWriter->EndAttributes();

				pWriter->EndNode(m_name);
			}

			virtual std::wstring GetColorSchemeIndex(const std::wstring& str)const
			{
				std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = ColorMap.find(str);
				if (ColorMap.end() != pPair)
					return pPair->second.get();
				return str;
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);

				for (std::map<std::wstring, Limit::ColorSchemeIndex>::const_iterator pPair = ColorMap.begin(); pPair != ColorMap.end(); ++pPair)
				{
					pWriter->WriteLimit1(SchemeClr_GetBYTECode(pPair->first), pPair->second);
				}

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start sttribute

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					BYTE ind = pReader->GetUChar();
					Limit::ColorSchemeIndex _index;
					_index.SetStringCode(ind);

					ColorMap.insert(std::pair<std::wstring,Limit::ColorSchemeIndex>(SchemeClr_GetStringCode(_at), _index));
				}

				pReader->Seek(_e);				
			}

			std::map<std::wstring, Limit::ColorSchemeIndex> ColorMap;

			std::wstring m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_CLRMAP_INCLUDE_H_
