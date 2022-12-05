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

#include "SysClr.h"
#include <boost/format.hpp>

namespace PPTX
{
	namespace Logic
	{
		DWORD SysClr::GetRGBA(DWORD RGBA) const
		{
			SysClr* pColor = const_cast<SysClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetRGBA(RGBA);
		}
		DWORD SysClr::GetARGB(DWORD ARGB) const
		{
			SysClr* pColor = const_cast<SysClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetARGB(ARGB);
		}
		DWORD SysClr::GetBGRA(DWORD BGRA) const
		{
			SysClr* pColor = const_cast<SysClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetBGRA(BGRA);
		}
		DWORD SysClr::GetABGR(DWORD ABGR) const
		{
			SysClr* pColor = const_cast<SysClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetABGR(ABGR);
		}
		void SysClr::FillRGBFromVal()
		{
			DWORD RGB = 0;
			std::wstring str = val.get();
			if (str != L"")
			{
				switch((CHAR)str[0])
				{
				case '3':
					if(str == _T("3dDkShadow")) { RGB = ::GetSysColor(COLOR_3DDKSHADOW); break;}
					if(str == _T("3dLight")) { RGB = ::GetSysColor(COLOR_3DLIGHT); break;}
					break;
				case 'a':
					if(str == _T("activeBorder")) { RGB = ::GetSysColor(COLOR_ACTIVEBORDER); break;}
					if(str == _T("activeCaption")) { RGB = ::GetSysColor(COLOR_ACTIVECAPTION); break;}
					if(str == _T("appWorkspace")) { RGB = ::GetSysColor(COLOR_APPWORKSPACE); break;}
					break;
				case 'b':
					if(str == _T("background")) { RGB = ::GetSysColor(COLOR_BACKGROUND); break;}
					if(str == _T("btnFace")) { RGB = ::GetSysColor(COLOR_BTNFACE); break;}
					if(str == _T("btnHighlight")) { RGB = ::GetSysColor(COLOR_BTNHIGHLIGHT); break;}
					if(str == _T("btnShadow")) { RGB = ::GetSysColor(COLOR_BTNSHADOW); break;}
					if(str == _T("btnText")) { RGB = ::GetSysColor(COLOR_BTNTEXT); break;}
					break;
				case 'c':
					if(str == _T("captionText")) { RGB = ::GetSysColor(COLOR_CAPTIONTEXT); break;}
					break;
				case 'g':
					if(str == _T("gradientActiveCaption")) { RGB = ::GetSysColor(COLOR_GRADIENTACTIVECAPTION); break;}
					if(str == _T("gradientInactiveCaption")) { RGB = ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION); break;}
					if(str == _T("grayText")) { RGB = ::GetSysColor(COLOR_GRAYTEXT); break;}
					break;
				case 'h':
					if(str == _T("highlight")) { RGB = ::GetSysColor(COLOR_HIGHLIGHT); break;}
					if(str == _T("highlightText")) { RGB = ::GetSysColor(COLOR_HIGHLIGHTTEXT); break;}
					if(str == _T("hotLight")) { RGB = ::GetSysColor(COLOR_HOTLIGHT); break;}
					break;
				case 'i':
					if(str == _T("inactiveBorder")) { RGB = ::GetSysColor(COLOR_INACTIVEBORDER); break;}
					if(str == _T("inactiveCaption")) { RGB = ::GetSysColor(COLOR_INACTIVECAPTION); break;}
					if(str == _T("inactiveCaptionText")) { RGB = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT); break;}
					if(str == _T("infoBk")) { RGB = ::GetSysColor(COLOR_INFOBK); break;}
					if(str == _T("infoText")) { RGB = ::GetSysColor(COLOR_INFOTEXT); break;}
					break;
				case 'm':
					if(str == _T("menu")) { RGB = ::GetSysColor(COLOR_MENU); break;}
					if(str == _T("menuBar")) { RGB = ::GetSysColor(COLOR_MENUBAR); break;}
					if(str == _T("menuHighlight")) { RGB = ::GetSysColor(COLOR_MENUHILIGHT); break;}
					if(str == _T("menuText")) { RGB = ::GetSysColor(COLOR_MENUTEXT); break;}
					break;
				case 's':
					if(str == _T("scrollBar")) { RGB = ::GetSysColor(COLOR_SCROLLBAR); break;}
					break;
				case 'w':
					if(str == _T("window")) { RGB = ::GetSysColor(COLOR_WINDOW); break;}
					if(str == _T("windowFrame")) { RGB = ::GetSysColor(COLOR_WINDOWFRAME); break;}
					if(str == _T("windowText")) { RGB = ::GetSysColor(COLOR_WINDOWTEXT); break;}
					break;
				}
			}
			blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);
		}
		void SysClr::fromXML(XmlUtils::CXmlNode& node)
		{
			val = node.GetAttribute(_T("val"));
			Modifiers.clear();
			XmlMacroLoadArray(node, _T("*"), Modifiers, ColorModifier);
		}
		OOX::EElementType SysClr::getType() const
		{
			return OOX::et_a_sysClr;
		}
		void SysClr::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				ColorModifier m;
				Modifiers.push_back(m);
				Modifiers.back().fromXML(oReader);
			}
		}
		void SysClr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), val)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		std::wstring SysClr::toXML() const
		{
			std::wstringstream sstream;
			sstream << boost::wformat( L"%02x%02x%02x" ) % red % green % blue;

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("val"), val.get());
			oAttr.Write(_T("lastClr"), sstream.str());

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Modifiers);

			return XmlUtils::CreateNode(_T("a:sysClr"), oAttr, oValue);
		}
		void SysClr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = _T("w14:");
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = _T("a:");

			pWriter->StartNode(sNodeNamespace + _T("sysClr"));

			std::wstringstream sstream;
			sstream << boost::wformat( L"%02X%02X%02X" ) % red % green % blue;

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("val"), val.get());
			pWriter->WriteAttribute(sAttrNamespace + _T("lastClr"), sstream.str());
			pWriter->EndAttributes();

			size_t nCount = Modifiers.size();
			for (size_t i = 0; i < nCount; ++i)
				Modifiers[i].toXmlWriter(pWriter);

			pWriter->EndNode(sNodeNamespace + _T("sysClr"));
		}
		void SysClr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(COLOR_TYPE_SYS);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, val.get());

			SysClr* pColor = const_cast<SysClr*>(this);
			pColor->FillRGBFromVal();

			pWriter->WriteBYTE(1);
			pWriter->WriteBYTE(red);
			pWriter->WriteBYTE(2);
			pWriter->WriteBYTE(green);
			pWriter->WriteBYTE(3);
			pWriter->WriteBYTE(blue);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			ULONG len = (ULONG)Modifiers.size();
			if (len != 0)
			{
				pWriter->StartRecord(0);
				pWriter->WriteULONG(len);

				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(1, Modifiers[i]);
				}

				pWriter->EndRecord();
			}

			pWriter->EndRecord();
		}
		void SysClr::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX

