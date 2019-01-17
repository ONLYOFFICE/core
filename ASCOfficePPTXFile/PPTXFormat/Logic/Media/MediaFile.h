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
#ifndef PPTX_LOGIC_MEDIAFILE_INCLUDE_H_
#define PPTX_LOGIC_MEDIAFILE_INCLUDE_H_

#include "../../WrapperWritingElement.h"
#include "../../../../Common/DocxFormat/Source/DocxFormat/RId.h"

namespace PPTX
{
	namespace Logic
	{
		class MediaFile : public WrapperWritingElement
		{
		public:			
			WritingElement_AdditionConstructors(MediaFile)

			MediaFile(std::wstring name_)
			{
				name = name_;
			}

			MediaFile& operator=(const MediaFile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				name = oSrc.name;
				link = oSrc.link;
				contentType = oSrc.contentType;

				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				name = XmlUtils::GetNameNoNS(oReader.GetName());
				
				ReadAttributes(oReader);
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				name		= XmlUtils::GetNameNoNS(node.GetName());
				link		= node.GetAttribute(_T("r:link"));
				XmlMacroReadAttributeBase(node, L"contentType", contentType);
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"r:link",	link)
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"contentType",	contentType)
				WritingElement_ReadAttributes_End( oReader )
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(L"r:link", link.ToString());
				oAttr.Write(L"contentType", contentType);

				return XmlUtils::CreateNode(L"a:" + name, oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:" + name);
					pWriter->StartAttributes();
						pWriter->WriteAttribute (L"r:link", link.ToString());
						pWriter->WriteAttribute (L"contentType", contentType);
					pWriter->EndAttributes();
				pWriter->EndNode(L"a:" + name);
			}

			std::wstring		name;
			OOX::RId			link;
			nullable_string		contentType;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_MEDIAFILE_INCLUDE_H_
