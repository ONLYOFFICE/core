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

#include "TablePropertyExceptions.h"
#include "ParagraphPropertyExceptions.h"
#include "CharacterPropertyExceptions.h"

namespace DocFileFormat
{
	typedef enum _StyleKind
	{
		paragraph_style = 1,
		character_style,
		table_style,
		numbering_style
	} StyleKind;

	static const wchar_t StyleKindMap[5][10] = 
	{
        L"0",
        L"paragraph",
        L"character",
        L"table",
        L"numbering"
	};

	typedef enum _StyleIdentifier
	{
		Normal = 0,
		Heading1,
		Heading2,
		Heading3,
		Heading4,
		Heading5,
		Heading6,
		Heading7,
		Heading8,
		Heading9,
		Index1,
		Index2,
		Index3,
		Index4,
		Index5,
		Index6,
		Index7,
		Index8,
		Index9,
		TOC1,
		TOC2,
		TOC3,
		TOC4,
		TOC5,
		TOC6,
		TOC7,
		TOC8,
		TOC9,
		NormalIndent, 
		FootnoteText,
		AnnotationText,
		Header,
		Footer,
		IndexHeading,
		Caption,
		ToCaption,
		EnvelopeAddress,
		EnvelopeReturn,
		FootnoteReference,
		AnnotationReference,
		LineNumber,
		PageNumber,
		EndnoteReference,
		EndnoteText,
		TableOfAuthoring,
		Macro,
		TOAHeading,
		List,
		ListBullet,
		ListNumber,
		List2,
		List3,
		List4,
		List5,
		ListBullet2,
		ListBullet3,
		ListBullet4,
		ListBullet5,
		ListNumber2,
		ListNumber3,
		ListNumber4,
		ListNumber5,
		Title,
		Closing,
		Signature,
		NormalCharacter,
		BodyText,
		BodyTextIndent,
		ListContinue,
		ListContinue2,
		ListContinue3,
		ListContinue4,
		ListContinue5,
		MessageHeader,
		Subtitle,
		Salutation,
		Date,
		BodyText1I,
		BodyText1I2,
		NoteHeading,
		BodyText2,
		BodyText3,
		BodyTextIndent2,
		BodyTextIndent3,
		BlockQuote,
		Hyperlink,
		FollowedHyperlink,
		Strong,
		Emphasis,
		NavPane,
		PlainText,
		AutoSignature,
		FormTop,
		FormBottom,
		HtmlNormal,
		HtmlAcronym,
		HtmlAddress,
		HtmlCite,
		HtmlCode,
		HtmlDfn,
		HtmlKbd,
		HtmlPre,
		htmlSamp,
		HtmlTt,
		HtmlVar,
		TableNormal,
		AnnotationSubject,
		NormalList,
		OutlineList1,
		OutlineList2,
		OutlineList3,
		TableSimple,
		TableSimple2,
		TableSimple3,
		TableClassic1,
		TableClassic2,
		TableClassic3,
		TableClassic4,
		TableColorful1,
		TableColorful2,
		TableColorful3,
		TableColumns1,
		TableColumns2,
		TableColumns3,
		TableColumns4,
		TableColumns5,
		TableGrid1,
		TableGrid2,
		TableGrid3,
		TableGrid4,
		TableGrid5,
		TableGrid6,
		TableGrid7,
		TableGrid8,
		TableList1,
		TableList2,
		TableList3,
		TableList4,
		TableList5,
		TableList6,
		TableList7,
		TableList8,
		Table3DFx1,
		Table3DFx2,
		Table3DFx3,
		TableContemporary,
		TableElegant,
		TableProfessional,
		TableSubtle1,
		tableSubtle2,
		TableWeb1,
		TableWeb2,
		TableWeb3,
		Acetate,
		TableGrid,
		TableTheme,
		Max,
		User = 4094,
		Null = 4095
	} StyleIdentifier;

	static const wchar_t StyleIdentifierMap[159][20] =
	{
        L"Normal",
        L"Heading1",
        L"Heading2",
        L"Heading3",
        L"Heading4",
        L"Heading5",
        L"Heading6",
        L"Heading7",
        L"Heading8",
        L"Heading9",
        L"Index1",
        L"Index2",
        L"Index3",
        L"Index4",
        L"Index5",
        L"Index6",
        L"Index7",
        L"Index8",
        L"Index9",
        L"TOC1",
        L"TOC2",
        L"TOC3",
        L"TOC4",
        L"TOC5",
        L"TOC6",
        L"TOC7",
        L"TOC8",
        L"TOC9",
        L"NormalIndent",
        L"FootnoteText",
        L"AnnotationText",
        L"Header",
        L"Footer",
        L"IndexHeading",
        L"Caption",
        L"ToCaption",
        L"EnvelopeAddress",
        L"EnvelopeReturn",
        L"FootnoteReference",
        L"AnnotationReference",
        L"LineNumber",
        L"PageNumber",
        L"EndnoteReference",
        L"EndnoteText",
        L"TableOfAuthoring",
        L"Macro",
        L"TOAHeading",
        L"List",
        L"ListBullet",
        L"ListNumber",
        L"List2",
        L"List3",
        L"List4",
        L"List5",
        L"ListBullet2",
        L"ListBullet3",
        L"ListBullet4",
        L"ListBullet5",
        L"ListNumber2",
        L"ListNumber3",
        L"ListNumber4",
        L"ListNumber5",
        L"Title",
        L"Closing",
        L"Signature",
        L"NormalCharacter",
        L"BodyText",
        L"BodyTextIndent",
        L"ListContinue",
        L"ListContinue2",
        L"ListContinue3",
        L"ListContinue4",
        L"ListContinue5",
        L"MessageHeader",
        L"Subtitle",
        L"Salutation",
        L"Date",
        L"BodyText1I",
        L"BodyText1I2",
        L"NoteHeading",
        L"BodyText2",
        L"BodyText3",
        L"BodyTextIndent2",
        L"BodyTextIndent3",
        L"BlockQuote",
        L"Hyperlink",
        L"FollowedHyperlink",
        L"Strong",
        L"Emphasis",
        L"NavPane",
        L"PlainText",
        L"AutoSignature",
        L"FormTop",
        L"FormBottom",
        L"HtmlNormal",
        L"HtmlAcronym",
        L"HtmlAddress",
        L"HtmlCite",
        L"HtmlCode",
        L"HtmlDfn",
        L"HtmlKbd",
        L"HtmlPre",
        L"htmlSamp",
        L"HtmlTt",
        L"HtmlVar",
        L"TableNormal",
        L"AnnotationSubject",
        L"NormalList",
        L"OutlineList1",
        L"OutlineList2",
        L"OutlineList3",
        L"TableSimple",
        L"TableSimple2",
        L"TableSimple3",
        L"TableClassic1",
        L"TableClassic2",
        L"TableClassic3",
        L"TableClassic4",
        L"TableColorful1",
        L"TableColorful2",
        L"TableColorful3",
        L"TableColumns1",
        L"TableColumns2",
        L"TableColumns3",
        L"TableColumns4",
        L"TableColumns5",
        L"TableGrid1",
        L"TableGrid2",
        L"TableGrid3",
        L"TableGrid4",
        L"TableGrid5",
        L"TableGrid6",
        L"TableGrid7",
        L"TableGrid8",
        L"TableList1",
        L"TableList2",
        L"TableList3",
        L"TableList4",
        L"TableList5",
        L"TableList6",
        L"TableList7",
        L"TableList8",
        L"Table3DFx1",
        L"Table3DFx2",
        L"Table3DFx3",
        L"TableContemporary",
        L"TableElegant",
        L"TableProfessional",
        L"TableSubtle1",
        L"tableSubtle2",
        L"TableWeb1",
        L"TableWeb2",
        L"TableWeb3",
        L"Acetate",
        L"TableGrid",
        L"TableTheme",
        L"Max",
        L"User",
        L"Null"
	}; 

	class StyleSheetDescription
	{
		friend class StyleSheetMapping;
		friend class CharacterPropertiesMapping;

	public:
		/// The name of the style
		std::wstring xstzName;
		/// Invariant style identifier 
		StyleIdentifier sti;
		/// spare field for any temporary use, always reset back to zero! 
		bool fScratch;
		/// PHEs of all text with this style are wrong
		bool fInvalHeight;
		/// UPEs have been generated 
		bool fHasUpe;
		/// std has been mass-copied; if unused at save time, 
		/// style should be deleted 
		bool fMassCopy;
		/// style kind 
		StyleKind stk;
		/// base style 
		unsigned int istdBase;
		/// number of UPXs (and UPEs) 
		unsigned short cupx;
		/// next style
		unsigned int istdNext;
		/// offset to end of upx's, start of upe's 
		unsigned short bchUpe;
		/// auto redefine style when appropriate 
		bool fAutoRedef;
		/// hidden from UI? 
		bool fHidden;
		/// style already has valid sprmCRgLidX_80 in it 
		bool f97LidsSet;
		/// if f97LidsSet, says whether we copied the lid from sprmCRgLidX 
		/// into sprmCRgLidX_80 or whether we gotrid of sprmCRgLidX_80
		bool fCopyLang;
		/// HTML Threading compose style 
		bool fPersonalCompose;
		/// HTML Threading reply style 
		bool fPersonalReply;
		/// HTML Threading - another user's personal style 
		bool fPersonal;
		/// Do not export this style to HTML/CSS 
		bool fNoHtmlExport;
		/// Do not show this style in long style lists 
		bool fSemiHidden;
		/// Locked style? 
		bool fLocked;
		/// Style is used by a word feature, e.g. footnote
		bool fInternalUse;
		/// Is this style linked to another?
		unsigned int istdLink;
		/// Style has RevMarking history 
		bool fHasOriginalStyle;
		/// marks during merge which doc's style changed 
		unsigned int rsid;
		/// used temporarily during html export 
		unsigned int iftcHtml;
		/// A StyleSheetDescription can have a PAPX.
		/// If the style doesn't modify paragraph properties, papx is null.
		ParagraphPropertyExceptions* papx;
		/// A StyleSheetDescription can have a CHPX.
		/// If the style doesn't modify character properties, chpx is null.
		CharacterPropertyExceptions* chpx;
		/// A StyleSheetDescription can have a TAPX.
		/// If the style doesn't modify table properties, tapx is null.
		TablePropertyExceptions* tapx;
		bool fQFormat;
		bool fUnhideWhenUsed;
		unsigned int uiPriority;

		/// Creates an empty STD object
		StyleSheetDescription();
		virtual ~StyleSheetDescription();
		/// Parses the bytes to retrieve a StyleSheetDescription
		StyleSheetDescription( unsigned char* bytes, int size, int cbStdBase, POLE::Stream* dataStream, int nWordVersion);
	};
}
