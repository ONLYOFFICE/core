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
#ifndef PPTX_LOGIC_SET_INCLUDE_H_
#define PPTX_LOGIC_SET_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "AnimVariant.h"

namespace PPTX
{
	namespace Logic
	{
		class Set : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(Set)

			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
				cBhvr	= node.ReadNode(_T("p:cBhvr"));
				to		= node.ReadNode(_T("p:to"));

				FillParentPointersForChilds();
			}

			virtual std::wstring toXML() const
			{
				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);
				oValue.WriteNullable(to);

				return XmlUtils::CreateNode(_T("p:set"), oValue);
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_p_set;
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(toXML());
			}
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
			{
				LONG end = pReader->GetPos() + pReader->GetRecordSize() + 4;

				pReader->Skip(1); // attribute start
				while (true)
				{
					BYTE _at = pReader->GetUChar_TypeNode();
					if (_at == NSBinPptxRW::g_nodeAttributeEnd)
						break;
				}
				while (pReader->GetPos() < end)
				{
					BYTE _rec = pReader->GetUChar();

					switch (_rec)
					{
						case 0:
						{
							cBhvr.fromPPTY(pReader);
						}break;
						case 1:
						{
							to.Init(); to->node_name = L"to";
							to->fromPPTY(pReader);
						}break;
						default:
						{
							pReader->SkipRecord();
						}break;
					}
				}
				pReader->Seek(end);
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, cBhvr);
				pWriter->WriteRecord2(1, to);
			}

			CBhvr					cBhvr;
			nullable<AnimVariant>	to;
		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				if(to.IsInit())
					to->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_SET_INCLUDE_H