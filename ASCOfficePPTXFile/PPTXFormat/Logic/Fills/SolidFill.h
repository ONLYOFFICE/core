/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
#ifndef PPTX_LOGIC_SOLIDFILL_INCLUDE_H_
#define PPTX_LOGIC_SOLIDFILL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{

		class SolidFill : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(SolidFill)

			SolidFill& operator=(const SolidFill& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Color		= oSrc.Color;
				m_namespace	= oSrc.m_namespace;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				m_namespace = XmlUtils::GetNamespace(node.GetName());
				Color.GetColorFrom(node);
				FillParentPointersForChilds();
			}
			virtual CString toXML() const
			{
				CString strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));

				XmlUtils::CNodeValue oValue;
				oValue.Write(Color);

				return XmlUtils::CreateNode(strName, oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				CString strName;
				if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
					strName = _T("w14:solidFill");
				else
					strName = (_T("") == m_namespace) ? _T("solidFill") : (m_namespace + _T(":solidFill"));

				pWriter->StartNode(strName);
				pWriter->EndAttributes();
				
				if (Color.is_init())
					Color.toXmlWriter(pWriter);

				pWriter->EndNode(strName);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(FILL_TYPE_SOLID);

				pWriter->WriteRecord1(0, Color);

				pWriter->EndRecord();
			}

			void Merge(SolidFill& fill)const
			{
				if(Color.is_init())
					fill.Color = Color;
			}
		public:
			UniColor Color;

			CString m_namespace;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Color.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SOLIDFILL_INCLUDE_H_