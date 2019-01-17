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
#ifndef PPTX_LOGIC_DUOTONE_INCLUDE_H_
#define PPTX_LOGIC_DUOTONE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniColor.h"

namespace PPTX
{
	namespace Logic
	{
		class Duotone : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Duotone)
			PPTX_LOGIC_BASE2(Duotone)

			Duotone& operator=(const Duotone& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Colors = oSrc.Colors;
				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_duotone;
			}	
			void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring strName = oReader.GetName();

					UniColor col;
					Colors.push_back(col);
					Colors.back().fromXML(oReader);
				}
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Colors.clear();
                XmlMacroLoadArray(node, L"*", Colors, UniColor);
				FillParentPointersForChilds();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.WriteArray(Colors);

				return XmlUtils::CreateNode(L"a:duotone", oValue);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:duotone");
				pWriter->EndAttributes();

				pWriter->WriteArray2(Colors);

				pWriter->EndNode(L"a:duotone");
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->Skip(4); // len
				BYTE _type = pReader->GetUChar();
				LONG _e = pReader->GetPos() + pReader->GetLong() + 4;

				ULONG count = pReader->GetULong();
				for (ULONG i = 0; i < count; ++i)
				{
					pReader->Skip(1); // type 

					Colors.push_back(UniColor());
					Colors.back().fromPPTY(pReader);

					if (false == Colors.back().is_init())
					{
						Colors.pop_back();
					}
				}
				pReader->Seek(_e);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_DUOTONE);

				ULONG len = (ULONG)Colors.size();
				pWriter->WriteULONG(len);
				
				for (ULONG i = 0; i < len; ++i)
				{
					pWriter->WriteRecord1(0, Colors[i]);
				}

				pWriter->EndRecord();
			}

		public:
			std::vector<UniColor> Colors;
		protected:
			virtual void FillParentPointersForChilds()
			{
				size_t count = Colors.size();
				for (size_t i = 0; i < count; ++i)
					Colors[i].SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_DUOTONE_INCLUDE_H_
