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
#ifndef PPTX_LOGIC_PATTFILL_INCLUDE_H_
#define PPTX_LOGIC_PATTFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/PattFillVal.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class PattFill : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(PattFill)
			
			PattFill(std::wstring ns = L"a")
			{
				m_namespace = ns;
			}
			PattFill& operator=(const PattFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				prst = oSrc.prst;

				fgClr = oSrc.fgClr;
				bgClr = oSrc.bgClr;

				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_namespace = XmlUtils::GetNamespace(oReader.GetName());

				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring sName = oReader.GetName();
					if (sName == L"a:fgClr")
						fgClr.fromXMLParent(oReader);
					if (sName == L"a:bgClr")
						bgClr.fromXMLParent(oReader);
				}
				FillParentPointersForChilds();	
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_pattFill;
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("prst"), prst )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());

				XmlMacroReadAttributeBase(node, L"prst", prst);
				
				XmlUtils::CXmlNode oNode;
				if (node.GetNode(_T("a:fgClr"), oNode))
					fgClr.GetColorFrom(oNode);
				if (node.GetNode(_T("a:bgClr"), oNode))
					bgClr.GetColorFrom(oNode);

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("prst"), prst);

				XmlUtils::CNodeValue oValue;
				oValue.Write(_T("a:fgClr"), fgClr);
				oValue.Write(_T("a:bgClr"), bgClr);

				std::wstring strName = (_T("") == m_namespace) ? _T("pattFill") : (m_namespace + _T(":pattFill"));
				return XmlUtils::CreateNode(strName, oAttr, oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				std::wstring strName = (_T("") == m_namespace) ? _T("pattFill") : (m_namespace + _T(":pattFill"));
				pWriter->StartNode(strName);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("prst"), prst);
				pWriter->EndAttributes();

				pWriter->StartNode(_T("a:fgClr"));
				pWriter->EndAttributes();
				fgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:fgClr"));

				pWriter->StartNode(_T("a:bgClr"));
				pWriter->EndAttributes();
				bgClr.toXmlWriter(pWriter);
				pWriter->EndNode(_T("a:bgClr"));

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_PATT);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, fgClr);
				pWriter->WriteRecord1(1, bgClr);
				
				pWriter->EndRecord();
			}

		public:
			nullable_limit<Limit::PattFillVal> prst;

			UniColor fgClr;
			UniColor bgClr;

			std::wstring m_namespace;
		protected:
			virtual void FillParentPointersForChilds()
			{
				fgClr.SetParentPointer(this);
				bgClr.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_PATTFILL_INCLUDE_H_
