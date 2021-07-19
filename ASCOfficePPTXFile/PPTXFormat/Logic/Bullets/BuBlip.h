﻿/*
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

#include "./../../WrapperWritingElement.h"
#include "./../Fills/Blip.h"

namespace PPTX
{
	namespace Logic
	{
		class BuBlip : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(BuBlip)
			PPTX_LOGIC_BASE2(BuBlip)

			BuBlip& operator=(const BuBlip& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				blip = oSrc.blip;
				return *this;
			}

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
                    std::wstring strName = XmlUtils::GetNameNoNS(oReader.GetName());

					if (strName == L"blip")
					{
						blip  = oReader;
						break;
					}
				}
			}
			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_buBlip;
			}

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				blip = node.ReadNodeNoNS(L"blip");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:buBlip");
				pWriter->EndAttributes();
				blip.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:buBlip");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(BULLET_TYPE_BULLET_BLIP);
				pWriter->WriteRecord1(0, blip);
				pWriter->EndRecord();
			}

		public:
			Blip blip;
		protected:
			virtual void FillParentPointersForChilds()
			{
				blip.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX