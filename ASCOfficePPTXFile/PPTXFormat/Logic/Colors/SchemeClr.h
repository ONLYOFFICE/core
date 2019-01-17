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
#ifndef PPTX_LOGIC_SCHEMECLR_INCLUDE_H_
#define PPTX_LOGIC_SCHEMECLR_INCLUDE_H_

#include "./../../Limit/SchemeClrVal.h"
#include "./../../Theme/ClrScheme.h"
#include "ColorBase.h"

namespace PPTX
{
	namespace Logic
	{
		class SchemeClr : public ColorBase
		{
		public:
			WritingElement_AdditionConstructors(SchemeClr)
			PPTX_LOGIC_BASE2(SchemeClr)

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual std::wstring toXML() const;

			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_schemeClr;
			}	

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("val"), val)
				WritingElement_ReadAttributes_End_No_NS( oReader )
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
				pWriter->StartNode(sNodeNamespace + _T("schemeClr"));
						
				pWriter->StartAttributes();
				pWriter->WriteAttribute(sAttrNamespace + _T("val"), val.get());
				pWriter->EndAttributes();

				size_t nCount = Modifiers.size();
				for (size_t i = 0; i < nCount; ++i)
					Modifiers[i].toXmlWriter(pWriter);
				
				pWriter->EndNode(sNodeNamespace + _T("schemeClr"));
			}

			virtual DWORD GetRGBA(DWORD RGBA) const;
			virtual DWORD GetARGB(DWORD ARGB) const;
			virtual DWORD GetBGRA(DWORD BGRA) const;
			virtual DWORD GetABGR(DWORD ABGR) const;

			virtual DWORD GetRGBColor(NSCommon::smart_ptr<PPTX::Theme>& _oTheme, NSCommon::smart_ptr<PPTX::Logic::ClrMap>& _oClrMap, DWORD ARGB = 0);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(COLOR_TYPE_SCHEME);

				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit1(0, val);
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

		public:
			Limit::SchemeClrVal val;
		private:
			void FillRGBFromVal(DWORD rgb);

		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SCHEMECLR_INCLUDE_H