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
#ifndef PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_
#define PPTX_LOGIC_ANIMEFFECT_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "CBhvr.h"
#include "AnimVariant.h"
#include "./../../Limit/TLTransition.h"

namespace PPTX
{
	namespace Logic
	{
		class AnimEffect : public WrapperWritingElement
		{
		public:
			PPTX_LOGIC_BASE(AnimEffect)

			virtual void fromXML(XmlUtils::CXmlNode& node)
            {
                XmlMacroReadAttributeBase(node, L"transition", transition);
                XmlMacroReadAttributeBase(node, L"filter", filter);
                XmlMacroReadAttributeBase(node, L"prLst", prLst);
				
				cBhvr		= node.ReadNode(_T("p:cBhvr"));
				progress	= node.ReadNodeNoNS(_T("progress"));

				FillParentPointersForChilds();
			}
			virtual std::wstring toXML() const
			{
				XmlUtils::CAttribute oAttr;
                oAttr.WriteLimitNullable(_T("transition"), transition);
				oAttr.Write(_T("filter"), filter);
				oAttr.Write(_T("prLst"), prLst);

				XmlUtils::CNodeValue oValue;
				oValue.Write(cBhvr);
				oValue.WriteNullable(progress);

				return XmlUtils::CreateNode(_T("p:animEffect"), oAttr, oValue);
			}
			virtual OOX::EElementType getType() const
			{
				return OOX::et_p_animEffect;
			}
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
			{
				pWriter->WriteString(toXML());
			}
			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
			{
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
				pWriter->WriteLimit2(0, transition);
				pWriter->WriteString2(1, filter);
				pWriter->WriteString2(2, prLst);
				pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

				pWriter->WriteRecord1(0, cBhvr);
				pWriter->WriteRecord2(1, progress);
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

					else if (0 == _at)	transition = pReader->GetUChar();
					else if (1 == _at)	filter = pReader->GetString2();
					else if (2 == _at)	prLst = pReader->GetString2();
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
						progress.Init(); progress->node_name = L"progress";
						progress->fromPPTY(pReader);
					}break;
					default:
					{
						pReader->SkipRecord();

					}break;
					}
				}
				pReader->Seek(end);
			}

			CBhvr cBhvr;			
			nullable<AnimVariant>				progress;

            nullable_limit<Limit::TLTransition> transition;
			nullable_string						filter;
			nullable_string						prLst;

		protected:
			virtual void FillParentPointersForChilds()
			{
				cBhvr.SetParentPointer(this);
				if(progress.IsInit())
					progress->SetParentPointer(this);
			}
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_ANIMEFFECT_INCLUDE_H
