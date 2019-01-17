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
#ifndef PPTX_LOGIC_TILE_INCLUDE_H_
#define PPTX_LOGIC_TILE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "./../../Limit/RectAlign.h"
#include "./../../Limit/Flip.h"

namespace PPTX
{
	namespace Logic
	{

		class Tile : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Tile)
			PPTX_LOGIC_BASE2(Tile)

			Tile& operator=(const Tile& oSrc)
			{
				parentFile		= oSrc.parentFile;
				parentElement	= oSrc.parentElement;

				algn	= oSrc.algn;
				flip	= oSrc.flip;
				sx		= oSrc.sx;
				sy		= oSrc.sy;
				tx		= oSrc.tx;
				ty		= oSrc.ty;

				return *this;
			}

			virtual OOX::EElementType getType() const
			{
				return OOX::et_a_tile;
			}			
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );
			}
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start_No_NS( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("algn"), algn )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("flip"), flip )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("sx"), sx )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("sy"), sy )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("tx"), tx )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("ty"), ty )
				WritingElement_ReadAttributes_End_No_NS( oReader )
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				XmlMacroReadAttributeBase(node, L"algn", algn);
				XmlMacroReadAttributeBase(node, L"flip", flip);
				XmlMacroReadAttributeBase(node, L"sx", sx);
				XmlMacroReadAttributeBase(node, L"sy", sy);
				XmlMacroReadAttributeBase(node, L"tx", tx);
				XmlMacroReadAttributeBase(node, L"ty", ty);
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
				oAttr.WriteLimitNullable(_T("algn"), algn);
				oAttr.WriteLimitNullable(_T("flip"), flip);
				oAttr.Write(_T("sx"), sx);
				oAttr.Write(_T("sy"), sy);
				oAttr.Write(_T("tx"), tx);
				oAttr.Write(_T("ty"), ty);

				return XmlUtils::CreateNode(_T("a:tile"), oAttr);
			}

			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->StartNode(_T("a:tile"));

				pWriter->StartAttributes();
				pWriter->WriteAttribute(_T("algn"), algn);
				pWriter->WriteAttribute(_T("flip"), flip);
				pWriter->WriteAttribute(_T("sx"), sx);
				pWriter->WriteAttribute(_T("sy"), sy);
				pWriter->WriteAttribute(_T("tx"), tx);
				pWriter->WriteAttribute(_T("ty"), ty);
				pWriter->EndAttributes();

				pWriter->EndNode(_T("a:tile"));
			}

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteInt2(0, sx);
				pWriter->WriteInt2(1, sy);
				pWriter->WriteInt2(2, tx);
				pWriter->WriteInt2(3, ty);
				pWriter->WriteLimit2(4, algn);
				pWriter->WriteLimit2(5, flip);
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

					switch (_at)
					{
						case 0:
						{
							sx = pReader->GetLong();
							break;
						}
						case 1:
						{
							sy = pReader->GetLong();
							break;
						}
						case 2:
						{
							tx = pReader->GetLong();
							break;
						}
						case 3:
						{
							ty = pReader->GetLong();
							break;
						}
						case 4:
						{
							algn = new Limit::RectAlign();
							algn->SetBYTECode(pReader->GetUChar());
							break;
						}
						case 5:
						{
							flip = new Limit::Flip();
							flip->SetBYTECode(pReader->GetUChar());
							break;
						}
						default:
							break;
					}
				}

				pReader->Seek(_end_rec);
			}

		public:
			nullable_limit<Limit::RectAlign>	algn;
			nullable_limit<Limit::Flip>			flip;
			nullable_int						sx;
			nullable_int						sy;
			nullable_int						tx;
			nullable_int						ty;
		protected:
			virtual void FillParentPointersForChilds(){};
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TILE_INCLUDE_H_
