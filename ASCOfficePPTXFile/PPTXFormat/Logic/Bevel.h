/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
			PPTX_LOGIC_BASE2(Bevel)

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
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_name	= XmlUtils::GetNameNoNS(node.GetName());
				
				node.ReadAttributeBase(L"w", w);
				node.ReadAttributeBase(L"h", h);
				node.ReadAttributeBase(L"prst", prst);
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("w"), w);
				oAttr.Write(_T("h"), h);
				oAttr.WriteLimitNullable(_T("prst"), prst);
				
				return XmlUtils::CreateNode(_T("a:") + m_name, oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, w);
				pWriter->WriteInt2(1, h);
				pWriter->WriteLimit2(2, prst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:
			nullable_int						w;
			nullable_int						h;
			nullable_limit<Limit::BevelType>	prst;
		//private:
		public:
			std::wstring								m_name;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BEVEL_INCLUDE_H_