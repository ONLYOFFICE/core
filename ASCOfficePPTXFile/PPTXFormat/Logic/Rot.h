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
#ifndef PPTX_LOGIC_ROT_INCLUDE_H_
#define PPTX_LOGIC_ROT_INCLUDE_H_

#include "./../WrapperWritingElement.h"

namespace PPTX
{
	namespace Logic
	{

		class Rot : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Rot)
			PPTX_LOGIC_BASE2(Rot)

			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_rot;
			}	
			void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if		( oReader, L"lat", lat)
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"lon", lon)
					WritingElement_ReadAttributes_Read_else_if	( oReader, L"rev", rev)
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				lat = node.GetAttribute(L"lat");
				lon = node.GetAttribute(L"lon");
				rev = node.GetAttribute(L"rev");
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.Write(L"lat", lat);
				oAttr.Write(L"lon", lon);
				oAttr.Write(L"rev", rev);

				return XmlUtils::CreateNode(L"a:rot", oAttr);
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:rot");

				pWriter->StartAttributes();
				pWriter->WriteAttribute(L"lat", lat);
				pWriter->WriteAttribute(L"lon", lon);
				pWriter->WriteAttribute(L"rev", rev);
				pWriter->EndAttributes();

				pWriter->EndNode(L"a:rot");
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, lat);
				pWriter->WriteInt2(1, lon);
				pWriter->WriteInt2(2, rev);
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

					if (0 == _at)		lat	= pReader->GetLong();
					else if (1 == _at)	lon	= pReader->GetLong();
					else if (2 == _at)	rev = pReader->GetLong();
					else
						break;
				}
				pReader->Seek(_end_rec);
			}
			nullable_int lat;
			nullable_int lon;
			nullable_int rev;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ROT_INCLUDE_H_