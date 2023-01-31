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

#include "Stretch.h"

namespace PPTX
{
	namespace Logic
	{
		Stretch& Stretch::operator=(const Stretch& oSrc)
		{
			parentFile		= oSrc.parentFile;
			parentElement	= oSrc.parentElement;

			fillRect = oSrc.fillRect;
			return *this;
		}
		OOX::EElementType Stretch::getType() const
		{
			return OOX::et_a_stretch;
		}
		void Stretch::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				if (_T("fillRect") == XmlUtils::GetNameNoNS(oReader.GetName()))
					fillRect = oReader;
			}
		}
		void Stretch::fromXML(XmlUtils::CXmlNode& node)
		{
			XmlUtils::CXmlNodes oNodes;
			if (node.GetNodes(_T("*"), oNodes))
			{
				int count = oNodes.GetCount();
				for (int i = 0; i < count; ++i)
				{
					XmlUtils::CXmlNode oNode;
					oNodes.GetAt(i, oNode);

					if (_T("fillRect") == XmlUtils::GetNameNoNS(oNode.GetName()))
						fillRect = oNode;
				}
			}
			FillParentPointersForChilds();
		}
		std::wstring Stretch::toXML() const
		{
			XmlUtils::CNodeValue oValue;
			oValue.WriteNullable(fillRect);

			return XmlUtils::CreateNode(_T("a:stretch"), oValue);
		}
		void Stretch::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			pWriter->StartNode(_T("a:stretch"));
			pWriter->EndAttributes();

			pWriter->Write(fillRect);

			pWriter->EndNode(_T("a:stretch"));
		}
		void Stretch::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteRecord2(0, fillRect);
		}
		void Stretch::FillParentPointersForChilds()
		{
			if(fillRect.IsInit())
				fillRect->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
