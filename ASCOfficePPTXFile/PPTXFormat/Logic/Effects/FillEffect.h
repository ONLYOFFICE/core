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
#ifndef PPTX_LOGIC_FILLEFFECT_INCLUDE_H_
#define PPTX_LOGIC_FILLEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../UniFill.h"

namespace PPTX
{
	namespace Logic
	{
		class FillEffect : public WrapperWritingElement
		{
		public:
			
			WritingElement_AdditionConstructors(FillEffect)
			PPTX_LOGIC_BASE2(FillEffect)
			
			FillEffect& operator=(const FillEffect& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				Fill = oSrc.Fill;
				return *this;
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_fill;
			}	
			void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring strName = oReader.GetName();

					Fill.fromXML(oReader);
				}
				FillParentPointersForChilds();
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				Fill.GetFillFrom(node);
				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				return _T("<a:fill>") + Fill.toXML() + _T("</a:fill>");
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(L"a:fill");
				pWriter->EndAttributes();
				Fill.toXmlWriter(pWriter);
				pWriter->EndNode(L"a:fill");
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->StartRecord(EFFECT_TYPE_FILL);

				pWriter->WriteRecord1(0, Fill);

				pWriter->EndRecord();
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				pReader->Skip(4); // len
				BYTE _type = pReader->GetUChar();
				LONG _end_rec = pReader->GetPos() + pReader->GetLong() + 4;

				pReader->Skip(1); // start attributes

				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;
				}

				while (pReader->GetPos() < _end_rec)
				{
					BYTE _at = pReader->GetUChar();
					switch (_at)
					{
						case 0:
						{
							Fill.fromPPTY(pReader);
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}
		public:
			UniFill Fill;
		protected:
			virtual void FillParentPointersForChilds()
			{
				Fill.SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_FILLEFFECT_INCLUDE_H_