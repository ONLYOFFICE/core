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
#include "Basic.h"

#include "Utils.h"
#include "RtfProperty.h"

class RtfAbsPosTab : public IDocumentElement
{
public: 
	enum Leader
	{	l_none,
		l_ptablnone, // ptablnone 	Absolute position tab with a blank leading (default).
		l_ptabldot, // ptabldot 	Absolute position tab with a leading that uses period symbols (.....).
		l_ptablminus, // ptablminus 	Absolute position tab with a leading that uses minus symbols (-----).
		l_ptabluscore, // ptabluscore 	Absolute position tab with a leading that uses underscore symbols (_____).
		l_ptablmdot, // ptablmdot 	Absolute position tab with a leading that uses middle dot symbols (•••••).
	} ;
	enum Alignment
	{	a_none,
		a_left,
		a_center,
		a_right,
	};
 	enum Relative	
	{	r_none,
		r_margin,
		r_indent,
	} ;

	Leader		m_eLeader;
	Alignment	m_eAlignment;
	Relative	m_eRelative;

	RtfAbsPosTab();

	bool IsValid();
	std::wstring RenderToRtf(RenderParameter oRenderParameter);
	std::wstring RenderToOOX(RenderParameter oRenderParameter);
};

class RtfCharSpecial : public IDocumentElement
{
public: 
	enum _RtfSpecChar
	{
		rsc_none,
		rsc_chdate,		// chdate	Current date (as in headers).
		rsc_chdpl,		// chdpl	Current date in long format (for example, Wednesday, February 20, 2008).
		rsc_chdpa,		// chdpa	Current date in abbreviated format (for example, Wed, Feb 20, 2008).
		rsc_chtime,		// chtime	Current time (as in headers).
		rsc_chpgn,		// chpgn	Current page number (as in headers).
		rsc_sectnum,	// sectnum	Current section number (as in headers).
		rsc_chftn,		// chftn	Automatic footnote reference (footnotes follow in a group).
		rsc_chftnEnd,	//Endnote
		rsc_chatn,		// chatn	Annotation reference (annotation text follows in a group).
		rsc_chftnsep,	// chftnsep	Anchoring character for footnote separator.
		rsc_chftnsepc,	// chftnsepc	Anchoring character for footnote continuation.
		rsc_page,		// page	Required page break.
		rsc_column,		// column	Required column break.
		rsc_line,		// line	Required line break (no paragraph break).
		rsc_softpage,	// softpage	Nonrequired page break. Emitted as it appears in galley view.
		rsc_softcol,	// softcol	Nonrequired column break. Emitted as it appears in galley view.
		rsc_softline,	// softline	Nonrequired line break. Emitted as it appears in galley view.
		rsc_tab,		// tab	Tab character. 
		rsc_emspace,	// emspace	Non-breaking space equal to width of character “m” in current font. Some old RTF writers use the construct '{' \emspace '  }' (with two spaces before the closing brace) to trick readers unaware of \emspace into parsing a regular space. A reader should interpret this as an \emspace and a regular space.
		rsc_enspace,	// enspace	Nonbreaking space equal to width of character “n” in current font. Some old RTF writers use the construct '{' \enspace '  }' (with two spaces before the closing brace) to trick readers unaware of \enspace into parsing a regular space. A reader should interpret this as an \enspace and a regular space.
		rsc_qmspace,	// qmspace	One-quarter em space.
		rsc_Formula,	// |	Formula character. (Used by Word 5.1 for the Macintosh as the beginning delimiter for a string of formula typesetting commands.)
		rsc_OptHyphen,	// -	Optional hyphen.
		rsc_NonBrHyphen,// _	Non-breaking hyphen.
		rsc_NonBrSpace,	// ~	Non-breaking space.
		rsc_SubEntry,	// :	Specifies a subentry in an index entry.
		rsc_zwbo,		// zwbo	Zero-width break opportunity. Used to insert break opportunity between two characters.
		rsc_zwnbo,		// zwnbo	Zero-width non-break opportunity. Used to remove break opportunity between two characters.
		rsc_zwj,		// zwj	Zero-width joiner. This is used for ligating (joining) characters.
		rsc_zwnj,		// zwnj	Zero-width nonjoiner. This is used for unligating a character. 
	};   

	int					m_nTextWrapBreak;	// lbrN Text wrapping break of type
	int					m_nSoftHeight;		// softlheightN	Nonrequired line height. This is emitted as a prefix to each line.
	_RtfSpecChar		m_eType;
	RtfCharProperty		m_oProperty;
	
	RtfCharSpecial();

    std::wstring RenderToRtf(RenderParameter oRenderParameter);
    std::wstring _RenderToOOX(RenderParameter oRenderParameter);
    std::wstring RenderToOOX(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfCharSpecial> RtfCharSpecialPtr;

class RtfChar : public IDocumentElement
{            
protected: 
    std::wstring m_sChars;

public: 
	RtfChar();

	RtfCharProperty m_oProperty;
	bool m_bRtfEncode;

	int GetType();

	void AddText(std::wstring text);
	void setText(std::wstring text);
	std::wstring GetText();

    std::wstring RenderToOOX(RenderParameter oRenderParameter);
    
    static std::wstring renderRtfText( std::wstring& sText, void* poDocument, RtfCharProperty* oCharProperty = NULL, bool bMarker = false);
    static std::wstring renderRtfText( std::wstring& sText, void* poDocument, int nCodePage, bool bMarker = false );

    std::wstring RenderToRtf(RenderParameter oRenderParameter);

private: 
    std::wstring renderTextToXML( std::wstring sParam, bool bDelete = false );
};

class RtfCharNative : public RtfChar
{            
public: 
	std::wstring RenderToRtf(RenderParameter oRenderParameter);
};
typedef boost::shared_ptr<RtfCharNative> RtfCharNativePtr;
typedef boost::shared_ptr<RtfChar> RtfCharPtr;
