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
#ifndef PPTX_LOGIC_BEVEL_INCLUDE_H_
#define PPTX_LOGIC_BEVEL_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/BevelType.h"

namespace PPTX
{
	namespace Logic
	{

		class Bevel : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Bevel)
			
			Bevel(const std::wstring name = L"a:bevel")
			{
				m_name = name;
			}	
			virtual ~Bevel() {}		
			Bevel(const Bevel& oSrc) { *this = oSrc; }

			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_bevel;
			}	
			void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				m_name	= XmlUtils::GetNameNoNS(oReader.GetName());

				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("w"), w)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("h"), h)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("prst"), prst)
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= XmlUtils::GetNameNoNS(node.GetName());
				
				XmlMacroReadAttributeBase(node, L"w", w);
				XmlMacroReadAttributeBase(node, L"h", h);
				XmlMacroReadAttributeBase(node, L"prst", prst);
			}

			virtual std::wstring toXML() const
			{
				std::wstring namespace_ = XmlUtils::GetNamespace(m_name);
				if (namespace_ == L"w14")
				{
					namespace_ + L"w14:";
				}
				else namespace_.clear();

				XmlUtils::CAttribute oAttr;
				oAttr.Write(namespace_ + L"w", w);
				oAttr.Write(namespace_ + L"h", h);
				oAttr.WriteLimitNullable(namespace_ + L"prst", prst);
				
				return XmlUtils::CreateNode(m_name, oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
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


				pWriter->StartNode(sNodeNamespace + m_name);

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + L"w", w);
				pWriter->WriteAttribute(sAttrNamespace + L"h", h);
				if (prst.IsInit())
				{
					pWriter->WriteAttribute(sAttrNamespace + L"prst", prst->get());
				}
				pWriter->EndAttributes();
				
				pWriter->EndNode(sNodeNamespace + m_name);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, w);
				pWriter->WriteInt2(1, h);
				pWriter->WriteLimit2(2, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)		w = pReader->GetLong();
					else if (1 == _at)	h = pReader->GetLong();
					else if (2 == _at)	prst = pReader->GetUChar();
					else
						break;
				}

				pReader->Seek(_end_rec);
			}

			nullable_int						w;
			nullable_int						h;
			nullable_limit<Limit::BevelType>	prst;

			std::wstring						m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BEVEL_INCLUDE_H_
