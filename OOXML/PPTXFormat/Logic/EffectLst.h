/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include "./../WrapperWritingElement.h"
#include "Effects/Blur.h"
#include "Effects/Glow.h"
#include "Effects/OuterShdw.h"
#include "Effects/PrstShdw.h"
#include "Effects/InnerShdw.h"
#include "Effects/Reflection.h"
#include "Effects/SoftEdge.h"
#include "Effects/FillOverlay.h"

namespace PPTX
{
	namespace Logic
	{
		class EffectLst : public WrapperWritingElement
		{
		public:			
			WritingElement_AdditionMethods(EffectLst)
			PPTX_LOGIC_BASE2(EffectLst)

            EffectLst& operator=(const EffectLst& oSrc);

			virtual OOX::EElementType getType () const
			{
				return OOX::et_a_effectLst;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void fromXML(XmlUtils::CXmlNode& node);

            virtual std::wstring toXML() const;
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			void Merge(EffectLst& effectLst) const;

		public:
			nullable<Blur>			blur;
			nullable<FillOverlay>	fillOverlay;
			nullable<Glow>			glow;
			nullable<InnerShdw>		innerShdw;
			nullable<OuterShdw>		outerShdw;
			nullable<PrstShdw>		prstShdw;
			nullable<Reflection>	reflection;
			nullable<SoftEdge>		softEdge;
		protected:
            virtual void FillParentPointersForChilds();

		};
	} // namespace Logic
} // namespace PPTX
