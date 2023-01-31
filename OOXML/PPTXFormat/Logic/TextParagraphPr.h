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
#ifndef PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_
#define PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_

#include "./../WrapperWritingElement.h"
#include "./../Limit/TextAlign.h"
#include "./../Limit/FontAlign.h"
#include "TextSpacing.h"
#include "RunProperties.h"
#include "Bullets/BulletColor.h"
#include "Bullets/BulletTypeface.h"
#include "Bullets/Bullet.h"
#include "Bullets/BulletSize.h"
#include "Tab.h"

namespace PPTX
{
	namespace Logic
	{
		class TextParagraphPr : public WrapperWritingElement
		{
		public:
			WritingElement_AdditionConstructors(TextParagraphPr)

			TextParagraphPr();
			TextParagraphPr& operator=(const TextParagraphPr& oSrc);

			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual OOX::EElementType getType() const;
			virtual void fromXML(XmlUtils::CXmlNode& node);

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);
			virtual void toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const;

			void Merge(nullable<TextParagraphPr>& props);

			virtual void toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const;
			virtual void fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader);

		public:
			//Childs
			nullable<TextSpacing>	lnSpc;
			nullable<TextSpacing>	spcAft;
			nullable<TextSpacing>	spcBef;

			BulletColor				buColor;
			BulletSize				buSize;
			BulletTypeface			buTypeface;
			Bullet					ParagraphBullet;

			std::vector<Tab>		tabLst;// (Tab List)
			nullable<RunProperties> defRPr;

			// Attribs
			nullable_limit<Limit::TextAlign>	algn;			// (Alignment)
			nullable_int						defTabSz;		// (Default Tab Size)
			nullable_bool eaLnBrk;								// (East Asian Line Break)
			nullable_limit<Limit::FontAlign>	fontAlgn;		// (Font Alignment)
			nullable_bool						hangingPunct;	// (Hanging Punctuation)
			nullable_int						indent;			// (Indent)
			nullable_bool						latinLnBrk;		// (Latin Line Break)
			nullable_int						lvl;			// (Level)
			nullable_int						marL;			// (Left Margin)
			nullable_int						marR;			// (Right Margin)
			nullable_bool						rtl;			// (Right To Left)

		public:
			mutable std::wstring m_name;

		protected:
			virtual void FillParentPointersForChilds();
			void Normalize();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TEXTPARAGRAPHPR_INCLUDE_H_
