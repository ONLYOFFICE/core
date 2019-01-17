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
#ifndef PPTX_LOGIC_LIN_INCLUDE_H_
#define PPTX_LOGIC_LIN_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Lin : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Lin)
			PPTX_LOGIC_BASE2(Lin)

			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_lin;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS	( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, _T("ang"), ang)
					WritingElement_ReadAttributes_Read_else_if	( oReader, _T("scaled"), scaled)
				WritingElement_ReadAttributes_End_No_NS( oReader )
				
				Normalize();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"ang", ang);
				XmlMacroReadAttributeBase(node, L"scaled", scaled);

				Normalize();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(_T("ang"), ang);
				oAttr.Write(_T("scaled"), scaled);

				return XmlUtils::CreateNode(_T("a:lin"), oAttr);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, ang);
				pWriter->WriteBool2(1, scaled);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;

					if (0 == _at)
						ang = pReader->GetLong();
					else if (1 == _at)
						scaled = pReader->GetBool();
				}

				pReader->Seek(_end_rec);
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
				pWriter->StartNode(sNodeNamespace + _T("lin"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("ang"), ang);
				pWriter->WriteAttribute(sAttrNamespace + _T("scaled"), scaled);
				pWriter->EndAttributes();

				pWriter->EndNode(sNodeNamespace + _T("lin"));
			}
			
		public:
			nullable_int		ang;
			nullable_bool		scaled;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				ang.normalize(0, 21600000);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_LIN_INCLUDE_H_
