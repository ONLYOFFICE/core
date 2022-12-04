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

#include "PrstClr.h"

namespace PPTX
{
	namespace Logic
	{
		OOX::EElementType PrstClr::getType() const
		{
			return OOX::et_a_prstClr;
		}
		void PrstClr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void PrstClr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), val)
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void PrstClr::fromXML(XmlUtils::CXmlNode& node)
		{
			val = node.GetAttribute(_T("val"));
			Modifiers.clear();
			XmlMacroLoadArray(node, _T("*"), Modifiers, ColorModifier);
		}
		std::wstring PrstClr::toXML() const
		{
			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("val"), val.get());

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Modifiers);

			return XmlUtils::CreateNode(_T("a:prstClr"), oAttr, oValue);
		}
		void PrstClr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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
			pWriter->StartNode(sNodeNamespace + _T("prstClr"));

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("val"), val.get());
			pWriter->EndAttributes();

			size_t nCount = Modifiers.size();
			for (size_t i = 0; i < nCount; ++i)
				Modifiers[i].toXmlWriter(pWriter);

			pWriter->EndNode(sNodeNamespace + _T("prstClr"));
		}
		DWORD PrstClr::GetRGBA(DWORD RGBA) const
		{
			PrstClr* pColor = const_cast<PrstClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetRGBA(RGBA);
		}
		DWORD PrstClr::GetARGB(DWORD ARGB) const
		{
			PrstClr* pColor = const_cast<PrstClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetARGB(ARGB);
		}
		DWORD PrstClr::GetBGRA(DWORD BGRA) const
		{
			PrstClr* pColor = const_cast<PrstClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetBGRA(BGRA);
		}
		DWORD PrstClr::GetABGR(DWORD ABGR) const
		{
			PrstClr* pColor = const_cast<PrstClr*>(this);
			pColor->FillRGBFromVal();
			return ColorBase::GetABGR(ABGR);
		}
		void PrstClr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(COLOR_TYPE_PRST);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString1(0, val.get());
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
		void PrstClr::FillRGBFromVal()
		{
			DWORD RGB = 0;
			std::wstring str = val.get();
			if(str != _T(""))
			{
				switch(str[0])
				{
				case 'a':
					if(str == _T("aliceBlue"))		{RGB = 0xF0F8FF; break;} // (Alice Blue Preset Color)		Specifies a color with RGB value (240,248,255)
					if(str == _T("antiqueWhite"))	{RGB = 0xFAEBD7; break;} // (Antique White Preset Color)	Specifies a color with RGB value (250,235,215)
					if(str == _T("aqua"))			{RGB = 0x00FFFF; break;} // (Aqua Preset Color)				Specifies a color with RGB value (0,255,255)
					if(str == _T("aquamarine"))		{RGB = 0x7FFFD4; break;} // (Aquamarine Preset Color)		Specifies a color with RGB value (127,255,212)
					if(str == _T("azure"))			{RGB = 0xF0FFFF; break;} // (Azure Preset Color)			Specifies a color with RGB value (240,255,255)
					break;
				case 'b':
					if(str == _T("beige"))			{RGB = 0xF5F5DC; break;} // (Beige Preset Color)			Specifies a color with RGB value (245,245,220)
					if(str == _T("bisque"))			{RGB = 0xFFE4C4; break;} // (Bisque Preset Color)			Specifies a color with RGB value (255,228,196)
					if(str == _T("black"))			{RGB = 0x000000; break;} // (Black Preset Color)			Specifies a color with RGB value (0,0,0)
					if(str == _T("blanchedAlmond"))	{RGB = 0xFFEBCD; break;} // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205)
					if(str == _T("blue"))			{RGB = 0x0000FF; break;} // (Blue Preset Color)				Specifies a color with RGB value (0,0,255)
					if(str == _T("blueViolet"))		{RGB = 0x8A2BE2; break;} // (Blue Violet Preset Color)		Specifies a color with RGB value (138,43,226)
					if(str == _T("brown"))			{RGB = 0xA52A2A; break;} // (Brown Preset Color)			Specifies a color with RGB value (165,42,42)
					if(str == _T("burlyWood"))		{RGB = 0xDEB887; break;} // (Burly Wood Preset Color)		Specifies a color with RGB value (222,184,135)
					break;
				case 'c':
					if(str == _T("cadetBlue"))		{RGB = 0x5F9EA0; break;} // (Cadet Blue Preset Color)		Specifies a color with RGB value (95,158,160)
					if(str == _T("chartreuse"))		{RGB = 0x7FFF00; break;} // (Chartreuse Preset Color)		Specifies a color with RGB value (127,255,0)
					if(str == _T("chocolate"))		{RGB = 0xD2691E; break;} // (Chocolate Preset Color)		Specifies a color with RGB value (210,105,30)
					if(str == _T("coral"))			{RGB = 0xFF7F50; break;} // (Coral Preset Color)			Specifies a color with RGB value (255,127,80)
					if(str == _T("cornflowerBlue"))	{RGB = 0x6495ED; break;} // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237)
					if(str == _T("cornsilk"))		{RGB = 0xFFF8DC; break;} // (Cornsilk Preset Color)			Specifies a color with RGB value (255,248,220)
					if(str == _T("crimson"))		{RGB = 0xDC143C; break;} // (Crimson Preset Color)			Specifies a color with RGB value (220,20,60)
					if(str == _T("cyan"))			{RGB = 0x00FFFF; break;} // (Cyan Preset Color)				Specifies a color with RGB value (0,255,255)
					break;
				case 'd':
					if(str == _T("darkBlue"))		{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
					if(str == _T("darkCyan"))		{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
					if(str == _T("darkGoldenrod"))	{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
					if(str == _T("darkGray"))		{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					if(str == _T("darkGreen"))		{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
					if(str == _T("darkGrey"))		{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					if(str == _T("darkKhaki"))		{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
					if(str == _T("darkMagenta"))	{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
					if(str == _T("darkOliveGreen"))	{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
					if(str == _T("darkOrange"))		{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
					if(str == _T("darkOrchid"))		{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
					if(str == _T("darkRed"))		{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
					if(str == _T("darkSalmon"))		{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
					if(str == _T("darkSeaGreen"))	{RGB = 0x8FBC8F; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,143)
					if(str == _T("darkSlateBlue"))	{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
					if(str == _T("darkSlateGray"))	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					if(str == _T("darkSlateGrey"))	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					if(str == _T("darkTurquoise"))	{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
					if(str == _T("darkViolet"))		{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
					if(str == _T("deepPink"))		{RGB = 0xFF1493; break;} // (Deep Pink Preset Color)		Specifies a color with RGB value (255,20,147)
					if(str == _T("deepSkyBlue"))	{RGB = 0x00BFFF; break;} // (Deep Sky Blue Preset Color)	Specifies a color with RGB value (0,191,255)
					if(str == _T("dimGray"))		{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
					if(str == _T("dimGrey"))		{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105)
					if(str == _T("dkBlue"))			{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139)
					if(str == _T("dkCyan"))			{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139)
					if(str == _T("dkGoldenrod"))	{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11)
					if(str == _T("dkGray"))			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					if(str == _T("dkGreen"))		{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0)
					if(str == _T("dkGrey"))			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169)
					if(str == _T("dkKhaki"))		{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107)
					if(str == _T("dkMagenta"))		{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139)
					if(str == _T("dkOliveGreen"))	{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47)
					if(str == _T("dkOrange"))		{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0)
					if(str == _T("dkOrchid"))		{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204)
					if(str == _T("dkRed"))			{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0)
					if(str == _T("dkSalmon"))		{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122)
					if(str == _T("dkSeaGreen"))		{RGB = 0x8FBC8B; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,139)
					if(str == _T("dkSlateBlue"))	{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139)
					if(str == _T("dkSlateGray"))	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					if(str == _T("dkSlateGrey"))	{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79)
					if(str == _T("dkTurquoise"))	{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209)
					if(str == _T("dkViolet"))		{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211)
					if(str == _T("dodgerBlue"))		{RGB = 0x1E90FF; break;} // (Dodger Blue Preset Color)		Specifies a color with RGB value (30,144,255)
					break;
				case 'f':
					if(str == _T("firebrick"))		{RGB = 0xB22222; break;} // (Firebrick Preset Color)		Specifies a color with RGB value (178,34,34)
					if(str == _T("floralWhite"))	{RGB = 0xFFFAF0; break;} // (Floral White Preset Color)		Specifies a color with RGB value (255,250,240)
					if(str == _T("forestGreen"))	{RGB = 0x228B22; break;} // (Forest Green Preset Color)		Specifies a color with RGB value (34,139,34)
					if(str == _T("fuchsia"))		{RGB = 0xFF00FF; break;} // (Fuchsia Preset Color)			Specifies a color with RGB value (255,0,255)
					break;
				case 'g':
					if(str == _T("gainsboro"))		{RGB = 0xDCDCDC; break;} // (Gainsboro Preset Color)		Specifies a color with RGB value (220,220,220)
					if(str == _T("ghostWhite"))		{RGB = 0xF8F8FF; break;} // (Ghost White Preset Color)		Specifies a color with RGB value (248,248,255)
					if(str == _T("gold"))			{RGB = 0xFFD700; break;} // (Gold Preset Color)				Specifies a color with RGB value (255,215,0)
					if(str == _T("goldenrod"))		{RGB = 0xDAA520; break;} // (Goldenrod Preset Color)		Specifies a color with RGB value (218,165,32)
					if(str == _T("gray"))			{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
					if(str == _T("green"))			{RGB = 0x8000; break;} // (Green Preset Color)				Specifies a color with RGB value (0,128,0)
					if(str == _T("greenYellow"))	{RGB = 0xADFF2F; break;} // (Green Yellow Preset Color)		Specifies a color with RGB value (173,255,47)
					if(str == _T("grey"))			{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128)
					break;
				case 'h':
					if(str == _T("honeydew"))		{RGB = 0xF0FFF0; break;} // (Honeydew Preset Color)			Specifies a color with RGB value (240,255,240)
					if(str == _T("hotPink"))		{RGB = 0xFF69B4; break;} // (Hot Pink Preset Color)			Specifies a color with RGB value (255,105,180)
					break;
				case 'i':
					if(str == _T("indianRed"))		{RGB = 0xCD5C5C; break;} // (Indian Red Preset Color)		Specifies a color with RGB value (205,92,92)
					if(str == _T("indigo"))			{RGB = 0x4B0082; break;} // (Indigo Preset Color)			Specifies a color with RGB value (75,0,130)
					if(str == _T("ivory"))			{RGB = 0xFFFFF0; break;} // (Ivory Preset Color)			Specifies a color with RGB value (255,255,240)
					break;
				case 'k':
					if(str == _T("khaki"))			{RGB = 0xF0E68C; break;} // (Khaki Preset Color)			Specifies a color with RGB value (240,230,140)
					break;
				case 'l':
					if(str == _T("lavender"))		{RGB = 0xE6E6FA; break;} // (Lavender Preset Color)			Specifies a color with RGB value (230,230,250)
					if(str == _T("lavenderBlush"))	{RGB = 0xFFF0F5; break;} // (Lavender Blush Preset Color)	Specifies a color with RGB value (255,240,245)
					if(str == _T("lawnGreen"))		{RGB = 0x7CFC00; break;} // (Lawn Green Preset Color)		Specifies a color with RGB value (124,252,0)
					if(str == _T("lemonChiffon"))	{RGB = 0xFFFACD; break;} // (Lemon Chiffon Preset Color)	Specifies a color with RGB value (255,250,205)
					if(str == _T("lightBlue"))		{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
					if(str == _T("lightCoral"))		{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
					if(str == _T("lightCyan"))		{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
					if(str==_T("lightGoldenrodYellow")){RGB = 0xFAFAD2;break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,210)
					if(str == _T("lightGray"))		{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					if(str == _T("lightGreen"))		{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
					if(str == _T("lightGrey"))		{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					if(str == _T("lightPink"))		{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
					if(str == _T("lightSalmon"))	{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
					if(str == _T("lightSeaGreen"))	{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
					if(str == _T("lightSkyBlue"))	{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
					if(str == _T("lightSlateGray"))	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					if(str == _T("lightSlateGrey"))	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					if(str == _T("lightSteelBlue"))	{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
					if(str == _T("lightYellow"))	{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
					if(str == _T("lime"))			{RGB = 0x00FF00; break;} // (Lime Preset Color)				Specifies a color with RGB value (0,255,0)
					if(str == _T("limeGreen"))		{RGB = 0x32CD32; break;} // (Lime Green Preset Color)		Specifies a color with RGB value (50,205,50)
					if(str == _T("linen"))			{RGB = 0xFAF0E6; break;} // (Linen Preset Color)			Specifies a color with RGB value (250,240,230)
					if(str == _T("ltBlue"))			{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230)
					if(str == _T("ltCoral"))		{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128)
					if(str == _T("ltCyan"))			{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255)
					if(str == _T("ltGoldenrodYellow")){RGB = 0xFAFA78; break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,120)
					if(str == _T("ltGray"))			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					if(str == _T("ltGreen"))		{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144)
					if(str == _T("ltGrey"))			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211)
					if(str == _T("ltPink"))			{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193)
					if(str == _T("ltSalmon"))		{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122)
					if(str == _T("ltSeaGreen"))		{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170)
					if(str == _T("ltSkyBlue"))		{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250)
					if(str == _T("ltSlateGray"))	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					if(str == _T("ltSlateGrey"))	{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153)
					if(str == _T("ltSteelBlue"))	{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222)
					if(str == _T("ltYellow"))		{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224)
					break;
				case 'm':
					if(str == _T("magenta"))		{RGB = 0xFF00FF; break;} // (Magenta Preset Color)			Specifies a color with RGB value (255,0,255)
					if(str == _T("maroon"))			{RGB = 0x800000; break;} // (Maroon Preset Color)			Specifies a color with RGB value (128,0,0)
					if(str == _T("medAquamarine"))	{RGB = 0x66CDAA; break;} // (Medium Aquamarine Preset Color)Specifies a color with RGB value (102,205,170)
					if(str == _T("medBlue"))		{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
					if(str == _T("mediumAquamarine")) {RGB = 0x66CDAA; break;} // (Medium Aquamarine Color)		Specifies a color with RGB value (102,205,170)
					if(str == _T("mediumBlue"))		{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205)
					if(str == _T("mediumOrchid"))	{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
					if(str == _T("mediumPurple"))	{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
					if(str == _T("mediumSeaGreen"))	{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
					if(str == _T("mediumSlateBlue")){RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
					if(str == _T("mediumSpringGreen")){RGB = 0x00FA9A; break;} // (Medium Spring Color)			Specifies a color with RGB value (0,250,154)
					if(str == _T("mediumTurquoise")){RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
					if(str == _T("mediumVioletRed")){RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
					if(str == _T("medOrchid"))		{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211)
					if(str == _T("medPurple"))		{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219)
					if(str == _T("medSeaGreen"))	{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113)
					if(str == _T("medSlateBlue"))	{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238)
					if(str == _T("medSpringGreen"))	{RGB = 0x00FA9A; break;} // (Medium Spring Preset Color)	Specifies a color with RGB value (0,250,154)
					if(str == _T("medTurquoise"))	{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204)
					if(str == _T("medVioletRed"))	{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133)
					if(str == _T("midnightBlue"))	{RGB = 0x191970; break;} // (Midnight Blue Preset Color)	Specifies a color with RGB value (25,25,112)
					if(str == _T("mintCream"))		{RGB = 0xF5FFFA; break;} // (Mint Cream Preset Color)		Specifies a color with RGB value (245,255,250)
					if(str == _T("mistyRose"))		{RGB = 0xFFE4FF; break;} // (Misty Rose Preset Color)		Specifies a color with RGB value (255,228,225)
					if(str == _T("moccasin"))		{RGB = 0xFFE4B5; break;} // (Moccasin Preset Color)			Specifies a color with RGB value (255,228,181)
					break;
				case 'n':
					if(str == _T("navajoWhite"))	{RGB = 0xFFDEAD; break;} // (Navajo White Preset Color)		Specifies a color with RGB value (255,222,173)
					if(str == _T("navy"))			{RGB = 0x000080; break;} // (Navy Preset Color)				Specifies a color with RGB value (0,0,128)
					break;
				case 'o':
					if(str == _T("oldLace"))		{RGB = 0xFDF5E6; break;} // (Old Lace Preset Color)			Specifies a color with RGB value (253,245,230)
					if(str == _T("olive"))			{RGB = 0x808000; break;} // (Olive Preset Color)			Specifies a color with RGB value (128,128,0)
					if(str == _T("oliveDrab"))		{RGB = 0x6B8E23; break;} // (Olive Drab Preset Color)		Specifies a color with RGB value (107,142,35)
					if(str == _T("orange"))			{RGB = 0xFFA500; break;} // (Orange Preset Color)			Specifies a color with RGB value (255,165,0)
					if(str == _T("orangeRed"))		{RGB = 0xFF4500; break;} // (Orange Red Preset Color)		Specifies a color with RGB value (255,69,0)
					if(str == _T("orchid"))			{RGB = 0xDA70D6; break;} // (Orchid Preset Color)			Specifies a color with RGB value (218,112,214)
					break;
				case 'p':
					if(str == _T("paleGoldenrod"))	{RGB = 0xEEE8AA; break;} // (Pale Goldenrod Preset Color)	Specifies a color with RGB value (238,232,170)
					if(str == _T("paleGreen"))		{RGB = 0x98FB98; break;} // (Pale Green Preset Color)		Specifies a color with RGB value (152,251,152)
					if(str == _T("paleTurquoise"))	{RGB = 0xAFEEEE; break;} // (Pale Turquoise Preset Color)	Specifies a color with RGB value (175,238,238)
					if(str == _T("paleVioletRed"))	{RGB = 0xDB7093; break;} // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147)
					if(str == _T("papayaWhip"))		{RGB = 0xFFEFD5; break;} // (Papaya Whip Preset Color)		Specifies a color with RGB value (255,239,213)
					if(str == _T("peachPuff"))		{RGB = 0xFFDAB9; break;} // (Peach Puff Preset Color)		Specifies a color with RGB value (255,218,185)
					if(str == _T("peru"))			{RGB = 0xCD853F; break;} // (Peru Preset Color)				Specifies a color with RGB value (205,133,63)
					if(str == _T("pink"))			{RGB = 0xFFC0CB; break;} // (Pink Preset Color)				Specifies a color with RGB value (255,192,203)
					if(str == _T("plum"))			{RGB = 0xD3A0D3; break;} // (Plum Preset Color)				Specifies a color with RGB value (221,160,221)
					if(str == _T("powderBlue"))		{RGB = 0xB0E0E6; break;} // (Powder Blue Preset Color)		Specifies a color with RGB value (176,224,230)
					if(str == _T("purple"))			{RGB = 0x800080; break;} // (Purple Preset Color)			Specifies a color with RGB value (128,0,128)
					break;
				case 'r':
					if(str == _T("red"))			{RGB = 0xFF0000; break;} // (Red Preset Color)				Specifies a color with RGB value (255,0,0)
					if(str == _T("rosyBrown"))		{RGB = 0xBC8F8F; break;} // (Rosy Brown Preset Color)		Specifies a color with RGB value (188,143,143)
					if(str == _T("royalBlue"))		{RGB = 0x4169E1; break;} // (Royal Blue Preset Color)		Specifies a color with RGB value (65,105,225)
					break;
				case 's':
					if(str == _T("saddleBrown"))	{RGB = 0x8B4513; break;} // (Saddle Brown Preset Color)		Specifies a color with RGB value (139,69,19)
					if(str == _T("salmon"))			{RGB = 0xFA8072; break;} // (Salmon Preset Color)			Specifies a color with RGB value (250,128,114)
					if(str == _T("sandyBrown"))		{RGB = 0xF4A460; break;} // (Sandy Brown Preset Color)		Specifies a color with RGB value (244,164,96)
					if(str == _T("seaGreen"))		{RGB = 0x2E8B57; break;} // (Sea Green Preset Color)		Specifies a color with RGB value (46,139,87)
					if(str == _T("seaShell"))		{RGB = 0xFFF5EE; break;} // (Sea Shell Preset Color)		Specifies a color with RGB value (255,245,238)
					if(str == _T("sienna"))			{RGB = 0xA0522D; break;} // (Sienna Preset Color)			Specifies a color with RGB value (160,82,45)
					if(str == _T("silver"))			{RGB = 0xC0C0C0; break;} // (Silver Preset Color)			Specifies a color with RGB value (192,192,192)
					if(str == _T("skyBlue"))		{RGB = 0x87CEEB; break;} // (Sky Blue Preset Color)			Specifies a color with RGB value (135,206,235)
					if(str == _T("slateBlue"))		{RGB = 0x6A5AEB; break;} // (Slate Blue Preset Color)		Specifies a color with RGB value (106,90,205)
					if(str == _T("slateGray"))		{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
					if(str == _T("slateGrey"))		{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144)
					if(str == _T("snow"))			{RGB = 0xFFFAFA; break;} // (Snow Preset Color)				Specifies a color with RGB value (255,250,250)
					if(str == _T("springGreen"))	{RGB = 0x00FF7F; break;} // (Spring Green Preset Color)		Specifies a color with RGB value (0,255,127)
					if(str == _T("steelBlue"))		{RGB = 0x4682B4; break;} // (Steel Blue Preset Color)		Specifies a color with RGB value (70,130,180)
					break;
				case 't':
					if(str == _T("tan"))			{RGB = 0xD2B48C; break;} // (Tan Preset Color)				Specifies a color with RGB value (210,180,140)
					if(str == _T("teal"))			{RGB = 0x008080; break;} // (Teal Preset Color)				Specifies a color with RGB value (0,128,128)
					if(str == _T("thistle"))		{RGB = 0xD8BFD8; break;} // (Thistle Preset Color)			Specifies a color with RGB value (216,191,216)
					if(str == _T("tomato"))			{RGB = 0xFF7347; break;} // (Tomato Preset Color)			Specifies a color with RGB value (255,99,71)
					if(str == _T("turquoise"))		{RGB = 0x40E0D0; break;} // (Turquoise Preset Color)		Specifies a color with RGB value (64,224,208)
					break;
				case 'v':
					if(str == _T("violet"))			{RGB = 0xEE82EE; break;} // (Violet Preset Color)			Specifies a color with RGB value (238,130,238)
					break;
				case 'w':
					if(str == _T("wheat"))			{RGB = 0xF5DEB3; break;} // (Wheat Preset Color)			Specifies a color with RGB value (245,222,179)
					if(str == _T("white"))			{RGB = 0xFFFFFF; break;} // (White Preset Color)			Specifies a color with RGB value (255,255,255)
					if(str == _T("whiteSmoke"))		{RGB = 0xF5F5F5; break;} // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245)
					break;
				case 'y':
					if(str == _T("yellow"))			{RGB = 0xFFFF00; break;} // (Yellow Preset Color)			Specifies a color with RGB value (255,255,0)
					if(str == _T("yellowGreen"))	{RGB = 0x9ACD32; break;} // (Yellow Green Preset Color)		Specifies a color with RGB value (154,205,50)
					break;
				}
			}

			blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);
		}
		void PrstClr::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX
