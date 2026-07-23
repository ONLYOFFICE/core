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
#include "./../Limit/TextCaps.h"
#include "./../Limit/TextStrike.h"
#include "./../Limit/TextUnderline.h"

#include "Ln.h"
#include "UniFill.h"
#include "TextFont.h"
#include "EffectProperties.h"
#include "Hyperlink.h"
#include "ExtP.h"

namespace OOX
{
	namespace Logic
	{
		class CRunProperty;
	}
}
namespace PPTX
{
	namespace Logic
	{
		class Rtl : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(Rtl)
			PPTX_LOGIC_BASE2(Rtl)

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

		protected:
			virtual void FillParentPointersForChilds();
		};

		class UFillTx : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(UFillTx)

			UFillTx(std::wstring name = L"a:uFillTx");

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void Merge(nullable<UFillTx>& uFillTx) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			UniFill				Fill;
			std::wstring		m_name;

		protected:
			virtual void FillParentPointersForChilds();
		};

		class Highlight : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(Highlight)

			Highlight();

			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlNode& node);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);

			void Merge(nullable<Highlight>& highlight) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			UniColor Color;

		protected:
			virtual void FillParentPointersForChilds();
		};

		class RunProperties : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionMethods(RunProperties)

			RunProperties();
			virtual OOX::EElementType getType () const;

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual void fromXML(XmlUtils::CXmlNode& node);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;
			void Merge(nullable<RunProperties>& props) const;

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

			//uLn (Underline Stroke)  §21.1.2.3.14
			//uLnTx (Underline Follows Text)  §21.1.2.3.15
			nullable<Ln>						ln;
			UniFill								Fill;
			EffectProperties					EffectList;

			std::vector<Ext>					extLst;

			nullable<Highlight>					highlight;
			nullable<UFillTx>					uFill;
			nullable<UFillTx>					uFillTx;
			nullable<TextFont>					latin;
			nullable<TextFont>					ea;
			nullable<TextFont>					cs;
			nullable<TextFont>					sym;
			nullable<Hyperlink>					hlinkClick;
			nullable<Hyperlink>					hlinkMouseOver;
			nullable<Rtl>						rtl;

			nullable_string						altLang;
			nullable_bool						b;
			nullable_int						baseline;
			nullable_string						bmk;
			nullable_limit<Limit::TextCaps>		cap;
			nullable_bool						dirty;
			nullable_bool						err;
			nullable_bool						i;
			nullable_int						kern;
			nullable_bool						kumimoji;
			nullable_string						lang;
			nullable_bool						noProof;
			nullable_bool						normalizeH;
			nullable_bool						smtClean;
			nullable_int						smtId;
			nullable_int						spc;
			nullable_limit<Limit::TextStrike>	strike;
			nullable_int						sz;
			nullable_limit<Limit::TextUnderline> u;

			std::wstring						m_name;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();

		public:
			PPTX::Logic::UniColor GetColor() const;
		};
	} // namespace Logic
} // namespace PPTX
