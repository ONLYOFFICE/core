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
#ifndef PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_EXTP_PROPERTIES_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/RId.h"

/*

<p:ext uri="{DAA4B4D4-6D71-4841-9C94-3DE7FCFB9230}">
<p14:media xmlns:p14="http://schemas.microsoft.com/office/powerpoint/2010/main" r:embed="rId2">
<p14:trim st="93333.0816" end="96583.4816"/>
</p14:media>
</p:ext>

*/

namespace PPTX
{
	namespace Logic
	{
		class Ext : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Ext)

		public:
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlUtils::CXmlNode media;
				if (node.GetNode(_T("p14:media"), media))
				{
					link	=	media.GetAttribute(_T("r:embed"));

					std::wstring xmkl = media.GetXml();

					XmlUtils::CXmlNode trim;
					if (media.GetNode(_T("p14:trim"), trim))
					{
						trim.ReadAttributeBase(_T("st"), st);
						trim.ReadAttributeBase(_T("end"), end);
					}
				}
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				XmlUtils::CNodeValue oValue;

				return XmlUtils::CreateNode(_T("p:ext"), oValue);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				// TODO:
			}
			
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				link.toPPTY(0, pWriter);
				pWriter->WriteDouble2(1, st);
				pWriter->WriteDouble2(2, end);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);
			}

		public:

			OOX::RId				link;

			// trim
			nullable_double			st;
			nullable_double			end;

		protected:
			virtual void FillParentPointersForChilds()
			{

			}
		};
	} 
} 

#endif 