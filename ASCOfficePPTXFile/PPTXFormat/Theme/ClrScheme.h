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
#ifndef PPTX_THEME_CLRSCHEME_INCLUDE_H_
#define PPTX_THEME_CLRSCHEME_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Logic/UniColor.h"

namespace PPTX
{
	namespace nsTheme
	{
		class ClrScheme : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(ClrScheme)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name = node.GetAttribute(_T("name"));

				Logic::UniColor lColor;
				Scheme.clear();

                XmlUtils::CXmlNode node1;

                node1 = node.ReadNode(_T("a:dk1"));
                lColor.GetColorFrom( node1);        Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("dk1"), lColor));
                node1 = node.ReadNode(_T("a:lt1"));
                lColor.GetColorFrom( node1 );       Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("lt1"), lColor));
                node1 = node.ReadNode(_T("a:dk2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("dk2"), lColor));
                node1 = node.ReadNode(_T("a:lt2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("lt2"), lColor));
                node1 = node.ReadNode(_T("a:accent1"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent1"), lColor));
                node1 = node.ReadNode(_T("a:accent2"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent2"), lColor));
                node1 = node.ReadNode(_T("a:accent3"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent3"), lColor));
                node1 = node.ReadNode(_T("a:accent4"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent4"), lColor));
                node1 = node.ReadNode(_T("a:accent5"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent5"), lColor));
                node1 = node.ReadNode(_T("a:accent6"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("accent6"), lColor));
                node1 = node.ReadNode(_T("a:hlink"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("hlink"), lColor));
                node1 = node.ReadNode(_T("a:folHlink"));
                lColor.GetColorFrom(node1);         Scheme.insert(std::pair<std::wstring,Logic::UniColor>(_T("folHlink"), lColor));

				FillWithDefaults();
				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CNodeValue oValue;

				for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					oValue.Write2(_T("a:") + pPair->first, pPair->second.toXML());
				}

				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("name"), name);

				return XmlUtils::CreateNode(_T("a:clrScheme"), oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:clrScheme"));

				pWriter->StartAttributes();
                pWriter->WriteAttribute2(_T("name"), name);
				pWriter->EndAttributes();

				std::wstring arr[12] = {_T("dk1"), _T("lt1"), _T("dk2"), _T("lt2"), _T("accent1"), _T("accent2"), _T("accent3"), _T("accent4"), 
					_T("accent5"), _T("accent6"), _T("hlink"), _T("folHlink")};

				for (LONG i = 0; i < 12; ++i)
				{
					std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(arr[i]);
				
					if (pPair != Scheme.end())
					{
						pWriter->StartNode(_T("a:") + arr[i]);
						pWriter->EndAttributes();
						pPair->second.toXmlWriter(pWriter);
						pWriter->EndNode(_T("a:") + arr[i]);
					}
				}

				/*
				POSITION pos = Scheme.GetStartPosition();
				while (NULL != pos)
				{
					const std::map<std::wstring, Logic::UniColor>::iterator pPair = Scheme.GetNext(pos);
					pWriter->StartNode(_T("a:") + pPair->m_key);
					pWriter->EndAttributes();
					pPair->second.toXmlWriter(pWriter);
					pWriter->EndNode(_T("a:") + pPair->m_key);					
				}
				*/

				pWriter->EndNode(_T("a:clrScheme"));
			}

			virtual DWORD GetRGBAFromScheme(const std::wstring& str)const
			{
				std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetRGBA();
			}
			virtual DWORD GetARGBFromScheme(const std::wstring& str)const
			{
				std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
				{//default color map extension
					if (str == L"tx1")		pPair = Scheme.find(L"dk1");
					else if (str == L"tx2") pPair = Scheme.find(L"dk2");
					else if (str == L"bg1") pPair = Scheme.find(L"lt1");
					else if (str == L"bg2") pPair = Scheme.find(L"lt1");
					
					if (Scheme.end() == pPair) return 0;
					else return pPair->second.GetARGB();
				}
				else
					return pPair->second.GetARGB();
			}
			virtual DWORD GetBGRAFromScheme(const std::wstring& str)const
			{
				std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetBGRA();
			}
			virtual DWORD GetABGRFromScheme(const std::wstring& str)const
			{
				std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.find(str);
				if (Scheme.end() == pPair)
					return 0;
				return pPair->second.GetABGR();
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteString1(0, name);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					pWriter->WriteRecord1(SchemeClr_GetBYTECode(pPair->first), pPair->second);
				}
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attribute

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						name = pReader->GetString2();
				}

				while (pReader->GetPos() < _e)
				{
					BYTE _rec = pReader->GetUChar();
					
					Logic::UniColor color;
					color.fromPPTY(pReader);

					Scheme.insert(std::pair<std::wstring,Logic::UniColor>(SchemeClr_GetStringCode(_rec), color));
				}

				pReader->Seek(_e);
			}

			std::wstring							name;
			std::map<std::wstring, Logic::UniColor> Scheme;

			ClrScheme& operator=(const ClrScheme& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name = oSrc.name;
				
				Scheme.clear();
				for (std::map<std::wstring, Logic::UniColor>::const_iterator pPair = oSrc.Scheme.begin(); pPair != oSrc.Scheme.end(); ++pPair)
				{
					Scheme.insert(std::pair<std::wstring,Logic::UniColor>(pPair->first, pPair->second));
				}

				return *this;
			}
			void FillWithDefaults()
			{
				Logic::UniColor lColor;
				std::map<std::wstring, Logic::UniColor>::iterator it;
				it = Scheme.find(L"dk1");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0,0,0);
					Scheme[L"dk1"] = lColor;
				}
				it = Scheme.find(L"lt1");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(255,255,255);
					Scheme[L"lt1"] = lColor;
				}
				it = Scheme.find(L"dk2");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x1F,0x49, 0x7D);
					Scheme[L"dk2"] = lColor;
				}
				it = Scheme.find(L"lt2");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0xEE,0xEC,0xE1);
					Scheme[L"lt2"] = lColor;
				}
				it = Scheme.find(L"accent1");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x4F, 0x81, 0xBD);
					Scheme[L"accent1"] = lColor;
				}
				it = Scheme.find(L"accent2");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0xC0,0x50,0x4D);
					Scheme[L"accent2"] = lColor;
				}
				it = Scheme.find(L"accent3");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x9B,0xBB,0x59);
					Scheme[L"accent3"] = lColor;
				}
				it = Scheme.find(L"accent4");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x80,0x64,0xA2);
					Scheme[L"accent4"] = lColor;
				}
				it = Scheme.find(L"accent5");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x4B,0xAC,0xC6);
					Scheme[L"accent5"] = lColor;
				}
				it = Scheme.find(L"accent6");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0xF7,0x96,0x46);
					Scheme[L"accent6"] = lColor;
				}
				it = Scheme.find(L"hlink");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x00,0x00,0xFF);
					Scheme[L"hlink"] = lColor;
				}
				it = Scheme.find(L"folHlink");
				if (Scheme.end() == it || !it->second.is_init())
				{
					lColor.SetRGBColor(0x80, 0x00, 0x80);
					Scheme[L"folHlink"] = lColor;
				}
				
				FillParentPointersForChilds();
			}
		protected:
			virtual void FillParentPointersForChilds()
			{
				for (std::map<std::wstring, Logic::UniColor>::iterator pPair = Scheme.begin(); pPair != Scheme.end(); ++pPair)
				{
					pPair->second.SetParentPointer(this);
				}
			}
			//Logic::UniColor dk1; //Dark 1 
			//Logic::UniColor lt1; //Light 1 
			//Logic::UniColor dk2; //Dark 2 
			//Logic::UniColor lt2; //Light 2 
			//Logic::UniColor accent1; //Accent 1 
			//Logic::UniColor accent2; //Accent 2 
			//Logic::UniColor accent3; //Accent 3 
			//Logic::UniColor accent4; //Accent 4 
			//Logic::UniColor accent5; //Accent 5 
			//Logic::UniColor accent6; //Accent 6 
			//Logic::UniColor hlink; //Hyperlink 
			//Logic::UniColor folHlink; //Followed Hyperlink 
		};
	} // namespace nsTheme
} // namespace PPTX

#endif // PPTX_THEME_CLRSCHEME_INCLUDE_H
