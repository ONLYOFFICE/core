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
#ifndef PPTX_LOGIC_BUSZPCT_INCLUDE_H_
#define PPTX_LOGIC_BUSZPCT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{
		class BuSzPct : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(BuSzPct)
			PPTX_LOGIC_BASE2(BuSzPct)

			BuSzPct& operator=(const BuSzPct& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				val = oSrc.val;
				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_buSzPct;
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_ReadSingle ( oReader, _T("val"), val)
				WritingElement_ReadAttributes_End( oReader )
				
				Normalize();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				val = node.ReadAttributeInt(_T("val"));
				Normalize();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write( _T("val"), val);
				return XmlUtils::CreateNode(_T("a:buSzPct"), oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:buSzPct"));
				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("val"), val);
				pWriter->EndAttributes();
				pWriter->EndNode(_T("a:buSzPct"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_SIZE_PCT);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt1(0, val.get_value_or(0));
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->EndRecord();
			}
		public:
			nullable_int val;
		protected:
			virtual void FillParentPointersForChilds(){};

			AVSINLINE void Normalize()
			{
				if (val.IsInit())
				{
					int tmp = *val;
					normalize_value(tmp, 25000, 400000);
					val = tmp;
				}
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_BUSZPCT_INCLUDE_H