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
#ifndef PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_
#define PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "CViewPr.h"
#include "Sld.h"

namespace PPTX
{
	namespace nsViewProps
	{
		class OutlineViewPr : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(OutlineViewPr)

			OutlineViewPr& operator=(const OutlineViewPr& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				CViewPr = oSrc.CViewPr;
				SldLst = oSrc.SldLst;

				return *this;
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				CViewPr = node.ReadNode(_T("p:cViewPr"));
				SldLst.clear();

				node.ReadNode(_T("p:sldLst")).LoadArray(_T("p:sld"), SldLst);
			}
			virtual CString toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(CViewPr);

				if (0 < SldLst.size())
					oValue.WriteArray(_T("p:sldLst"), SldLst);

				return XmlUtils::CreateNode(_T("p:outlineViewPr"), oValue);
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteRecord1(0, CViewPr);

				pWriter->StartRecord(1);
				ULONG len = (ULONG)SldLst.size();
				pWriter->WriteULONG(len);
				for (ULONG i = 0; i < len; i++)
				{
					pWriter->WriteRecord1(2, SldLst[i]);
				}
				pWriter->EndRecord();
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("p:outlineViewPr"));
				pWriter->EndAttributes();

				CViewPr.toXmlWriter(pWriter);
				pWriter->WriteArray(_T("p:sldLst"), SldLst);
				
				pWriter->EndNode(_T("p:outlineViewPr"));
			}

		public:
			nsViewProps::CViewPr			CViewPr;
			std::vector<nsViewProps::Sld>		SldLst;
		protected:
			virtual void FillParentPointersForChilds()
			{
				CViewPr.SetParentPointer(this);

				size_t count = SldLst.size();
				for (size_t i = 0; i < count; ++i)
					SldLst[i].SetParentPointer(this);
			}
		};
	} // namespace nsViewProps
} // namespace PPTX

#endif // PPTX_VIEWPROPS_OUTLINE_VIEW_PROPERTIES_INCLUDE_H_