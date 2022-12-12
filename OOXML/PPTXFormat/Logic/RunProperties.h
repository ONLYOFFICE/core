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
#ifndef PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_
#define PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H_

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

namespace PPTX
{
	namespace Logic
	{
		class Rtl : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(Rtl)
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
			WritingElement_AdditionConstructors(UFillTx)

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
			WritingElement_AdditionConstructors(Highlight)

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
			WritingElement_AdditionConstructors(RunProperties)

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

#endif // PPTX_LOGIC_RUNPROPERTIES_INCLUDE_H
