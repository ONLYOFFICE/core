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

#include "Run.h"

namespace PPTX
{
	namespace Logic
	{
		Run& Run::operator=(const Run& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			rPr = oSrc.rPr;
			text = oSrc.text;

			return *this;
		}
		OOX::EElementType Run::getType () const
		{
			return OOX::et_a_r;
		}
		void Run::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"a:rPr" == sName )
				{
					rPr =  oReader ;
				}
				else if ( L"a:t" == sName )
				{
					if (!text.IsInit())
						text = oReader.GetText2();
				}

			}
		}
		void Run::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int count = oNodes.GetCount();
				for (int i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					std::wstring strName = XmlUtils::GetNameNoNS(oNode.GetName());

					if (_T("rPr") == strName)
					{
						rPr = oNode;
					}
					else if (_T("t") == strName)
					{
						if (!text.IsInit())
							text = oNode.GetTextExt();
					}
				}
			}

			FillParentPointersForChilds();
		}
		void Run::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:r"));
			pWriter->EndAttributes();

			if (rPr.is_init())
				rPr->m_name = _T("a:rPr");
			pWriter->Write(rPr);

			if (text.is_init())
			{
				pWriter->WriteString(_T("<a:t>"));
				pWriter->WriteStringXML(*text);
				pWriter->WriteString(_T("</a:t>"));
			}

			pWriter->EndNode(_T("a:r"));
		}
		void Run::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->StartRecord(PARRUN_TYPE_RUN);

			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteString2(0, text);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, rPr);

			pWriter->EndRecord();


			if (pWriter->m_pCommon->m_pNativePicker->m_bIsEmbeddedFonts)
				pWriter->m_pCommon->m_pNativePicker->m_oEmbeddedFonts.CheckString(text);
		}
		std::wstring Run::GetText() const
		{
			return text.get_value_or(_T(""));
		}
		void Run::SetText(const std::wstring& srcText)
		{
			text = srcText;
		}
		void Run::FillParentPointersForChilds()
		{
			if(rPr.IsInit())
				rPr->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
