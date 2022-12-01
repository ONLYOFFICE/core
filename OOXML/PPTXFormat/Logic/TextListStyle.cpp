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

#include "TextListStyle.h"

namespace PPTX
{
	namespace Logic
	{
		TextListStyle::TextListStyle(std::wstring name)
		{
			m_name = name;
		}
		void TextListStyle::fromXML(XmlUtils::CXmlNode& node)
		{
			m_name = node.GetName();

			/*defPPr*/ levels[9] = node.ReadNode(_T("a:defPPr"));
			/*lvl1pPr*/levels[0] = node.ReadNode(_T("a:lvl1pPr"));
			/*lvl2pPr*/levels[1] = node.ReadNode(_T("a:lvl2pPr"));
			/*lvl3pPr*/levels[2] = node.ReadNode(_T("a:lvl3pPr"));
			/*lvl4pPr*/levels[3] = node.ReadNode(_T("a:lvl4pPr"));
			/*lvl5pPr*/levels[4] = node.ReadNode(_T("a:lvl5pPr"));
			/*lvl6pPr*/levels[5] = node.ReadNode(_T("a:lvl6pPr"));
			/*lvl7pPr*/levels[6] = node.ReadNode(_T("a:lvl7pPr"));
			/*lvl8pPr*/levels[7] = node.ReadNode(_T("a:lvl8pPr"));
			/*lvl9pPr*/levels[8] = node.ReadNode(_T("a:lvl9pPr"));

			FillParentPointersForChilds();
		}
		OOX::EElementType TextListStyle::getType () const
		{
			return OOX::et_Unknown;//a_bodyStyle;
		}
		void TextListStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			m_name = oReader.GetName();

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

				if (_T("defPPr") == strName)
					levels[9] = oReader;
				else if (_T("lvl1pPr") == strName)
					levels[0] = oReader;
				else if (_T("lvl2pPr") == strName)
					levels[1] = oReader;
				else if (_T("lvl3pPr") == strName)
					levels[2] = oReader;
				else if (_T("lvl4pPr") == strName)
					levels[3] = oReader;
				else if (_T("lvl5pPr") == strName)
					levels[4] = oReader;
				else if (_T("lvl6pPr") == strName)
					levels[5] = oReader;
				else if (_T("lvl7pPr") == strName)
					levels[6] = oReader;
				else if (_T("lvl8pPr") == strName)
					levels[7] = oReader;
				else if (_T("lvl9pPr") == strName)
					levels[8] = oReader;
			}
		}
		std::wstring TextListStyle::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(/*defPPr*/  levels[9]);
			oValue.WriteNullable(/*lvl1pPr*/ levels[0]);
			oValue.WriteNullable(/*lvl2pPr*/ levels[1]);
			oValue.WriteNullable(/*lvl3pPr*/ levels[2]);
			oValue.WriteNullable(/*lvl4pPr*/ levels[3]);
			oValue.WriteNullable(/*lvl5pPr*/ levels[4]);
			oValue.WriteNullable(/*lvl6pPr*/ levels[5]);
			oValue.WriteNullable(/*lvl7pPr*/ levels[6]);
			oValue.WriteNullable(/*lvl8pPr*/ levels[7]);
			oValue.WriteNullable(/*lvl9pPr*/ levels[8]);

			return XmlUtils::CreateNode(m_name, oValue);
		}
		void TextListStyle::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, levels[0]);
			pWriter->WriteRecord2(1, levels[1]);
			pWriter->WriteRecord2(2, levels[2]);
			pWriter->WriteRecord2(3, levels[3]);
			pWriter->WriteRecord2(4, levels[4]);
			pWriter->WriteRecord2(5, levels[5]);
			pWriter->WriteRecord2(6, levels[6]);
			pWriter->WriteRecord2(7, levels[7]);
			pWriter->WriteRecord2(8, levels[8]);
			pWriter->WriteRecord2(9, levels[9]);
		}
		void TextListStyle::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(m_name);
			pWriter->EndAttributes();

			pWriter->Write(/*defPPr*/  levels[9]);
			pWriter->Write(/*lvl1pPr*/ levels[0]);
			pWriter->Write(/*lvl2pPr*/ levels[1]);
			pWriter->Write(/*lvl3pPr*/ levels[2]);
			pWriter->Write(/*lvl4pPr*/ levels[3]);
			pWriter->Write(/*lvl5pPr*/ levels[4]);
			pWriter->Write(/*lvl6pPr*/ levels[5]);
			pWriter->Write(/*lvl7pPr*/ levels[6]);
			pWriter->Write(/*lvl8pPr*/ levels[7]);
			pWriter->Write(/*lvl9pPr*/ levels[8]);

			pWriter->EndNode(m_name);
		}
		void TextListStyle::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			std::wstring arr_names[10] = {_T("a:lvl1pPr"), _T("a:lvl2pPr"), _T("a:lvl3pPr"), _T("a:lvl4pPr"), _T("a:lvl5pPr"),
				_T("a:lvl6pPr"), _T("a:lvl7pPr"), _T("a:lvl8pPr"), _T("a:lvl9pPr"), _T("a:defPPr")};

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();

				if (_at < 0 || _at > 9)
					break;

				levels[_at] = new TextParagraphPr();
				levels[_at]->m_name = arr_names[_at];
				levels[_at]->fromPPTY(pReader);
			}

			pReader->Seek(_end_rec);
		}
		bool TextListStyle::IsListStyleEmpty()
		{
			for (int i = 0; i < 10; i++)
			{
				if (levels[i].IsInit())
				{
					if (levels[i]->ParagraphBullet.is_init() == false) continue;
					if (levels[i]->ParagraphBullet.is<PPTX::Logic::BuNone>())continue;

					return false;
				}
			}
			return true;
		}
		void TextListStyle::Merge(nullable<TextListStyle>& lstStyle)const
		{
			if(!lstStyle.is_init())
				lstStyle = TextListStyle();
			for(int i = 0; i < 10; i++)
				if(levels[i].is_init())
					levels[i]->Merge(lstStyle->levels[i]);
		}
		void TextListStyle::FillParentPointersForChilds()
		{
			for(int i = 0; i < 10; i++)
			{
				if(levels[i].is_init())
					levels[i]->SetParentPointer(this);
			}
		}
	} // namespace Logic
} // namespace PPTX
