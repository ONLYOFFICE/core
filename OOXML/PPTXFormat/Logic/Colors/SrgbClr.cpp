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

#include "SrgbClr.h"

namespace PPTX
{
	namespace Logic
	{		
		OOX::EElementType SrgbClr::getType() const
		{
			return OOX::et_a_srgbClr;
		}
		void SrgbClr::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			std::wstring val;

			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if ( oReader, _T("val"), val)
			WritingElement_ReadAttributes_End_No_NS( oReader )

			if (6 == val.length())
			{
				red		= HexString2Int(val.substr(0, 2));
				green	= HexString2Int(val.substr(2, 2));
				blue	= HexString2Int(val.substr(4, 2));
			}

		}
		void SrgbClr::fromXML(XmlUtils::CXmlLiteReader& oReader)
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
		void SrgbClr::fromXML(XmlUtils::CXmlNode& node)
		{
			std::wstring val = node.GetAttribute(_T("val"));

			if (6 == val.length())
			{
				red		= HexString2Int(val.substr(0, 2));
				green	= HexString2Int(val.substr(2, 2));
				blue	= HexString2Int(val.substr(4, 2));
			}

			Modifiers.clear();
			XmlMacroLoadArray(node, _T("*"), Modifiers, ColorModifier);
		}
		void SrgbClr::fromXMLScRgb(XmlUtils::CXmlNode& node)
		{
			int cred	= node.GetAttributeInt(std::wstring(L"r"), 0);
			int cgreen	= node.GetAttributeInt(std::wstring(L"g"), 0);
			int cblue	= node.GetAttributeInt(std::wstring(L"b"), 0);

			red		= (unsigned char)(255 * scRGB_to_sRGB(cred / 100000.0));
			green	= (unsigned char)(255 * scRGB_to_sRGB(cgreen / 100000.0));
			blue	= (unsigned char)(255 * scRGB_to_sRGB(cblue / 100000.0));

			Modifiers.clear();
			XmlMacroLoadArray(node, _T("*"), Modifiers, ColorModifier);
		}
		std::wstring SrgbClr::toXML() const
		{
			std::wstringstream sstream;
			sstream << boost::wformat( L"%02X%02X%02X" ) % red % green % blue;

			XmlUtils::CAttribute oAttr;
			oAttr.Write(_T("val"), sstream.str());

			XmlUtils::CNodeValue oValue;
			oValue.WriteArray(Modifiers);

			return XmlUtils::CreateNode(_T("a:srgbClr"), oAttr, oValue);
		}
		void SrgbClr::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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

			pWriter->StartNode(sNodeNamespace + _T("srgbClr"));

			std::wstringstream sstream;
			sstream << boost::wformat( L"%02X%02X%02X" ) % red % green % blue;

			pWriter->StartAttributes();
			pWriter->WriteAttribute(sAttrNamespace + _T("val"), sstream.str());
			pWriter->EndAttributes();

			size_t nCount = Modifiers.size();
			for (size_t i = 0; i < nCount; ++i)
				Modifiers[i].toXmlWriter(pWriter);

			pWriter->EndNode(sNodeNamespace + _T("srgbClr"));
		}
		void SrgbClr::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(COLOR_TYPE_SRGB);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBYTE(0); pWriter->WriteBYTE(red);
			pWriter->WriteBYTE(1); pWriter->WriteBYTE(green);
			pWriter->WriteBYTE(2); pWriter->WriteBYTE(blue);
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
		double SrgbClr::scRGB_to_sRGB(double value)
		{
			if( value < 0)
				return 0;
			if(value <= 0.0031308)
				return value * 12.92;
			if(value < 1)
				return 1.055 * (pow(value , (1 / 2.4))) - 0.055;
			return 1;
		}
		void SrgbClr::FillParentPointersForChilds(){}
	} // namespace Logic
} // namespace PPTX

