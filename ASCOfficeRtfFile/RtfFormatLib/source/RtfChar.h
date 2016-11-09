/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		l_ptablnone, //\ptablnone 	Absolute position tab with a blank leading (default).
		l_ptabldot, //\ptabldot 	Absolute position tab with a leading that uses period symbols (.....).
		l_ptablminus, //\ptablminus 	Absolute position tab with a leading that uses minus symbols (-----).
		l_ptabluscore, //\ptabluscore 	Absolute position tab with a leading that uses underscore symbols (_____).
		l_ptablmdot, //\ptablmdot 	Absolute position tab with a leading that uses middle dot symbols (•••••).
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

	RtfAbsPosTab()
	{
	}
	bool IsValid()
	{
		return a_none != m_eAlignment && r_none != m_eRelative;
	}
	
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		switch( m_eLeader )
		{
			case l_ptablnone:	sResult += L"\\ptablnone";	break;
			case l_ptabldot:	sResult += L"\\ptabldot";	break;
			case l_ptablminus:	sResult += L"\\ptablminus";	break;
			case l_ptabluscore: sResult += L"\\ptabluscore";	break;
			case l_ptablmdot:	sResult += L"\\ptablmdo";	break;
		}
		switch( m_eRelative )
		{
			case r_margin: sResult += L"\\pmartabq";	break;
			case r_indent: sResult += L"\\pindtabq";	break;
		}
		switch( m_eAlignment )
		{
			case a_left:	sResult += L"l";	break;
			case a_center:	sResult += L"c";	break;
			case a_right:	sResult += L"r";	break;
		}
		if( false == sResult.IsEmpty() )
			sResult = L"{" + sResult + L"}";
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		switch( m_eLeader )
		{
			case l_ptablnone:	sResult += L" w:leader=\"none\"";		break;
			case l_ptabldot:	sResult += L" w:leader=\"dot\"";			break;
			case l_ptablminus:	sResult += L" w:leader=\"hyphen\"";		break;
			case l_ptabluscore: sResult += L" w:leader=\"underscore\"";	break;
			case l_ptablmdot:	sResult += L" w:leader=\"middleDot\"";	break;
		}
		switch( m_eRelative )
		{
			case r_margin: sResult += L" w:relativeTo=\"margin\"";	break;
			case r_indent: sResult += L" w:relativeTo=\"indent\"";	break;
		}
		switch( m_eAlignment )
		{
			case a_left:	sResult += L" w:alignment=\"left\"";		break;
			case a_center:	sResult += L" w:alignment=\"center\"";	break;
			case a_right:	sResult += L" w:alignment=\"right\"";	break;
		}
		if( !sResult.IsEmpty() )
			sResult = L"<w:ptab" + sResult + L"/>";
		return sResult;
	}
};
class RtfCharSpecial : public IDocumentElement
{
public: 
	typedef enum {
			rsc_none,
			rsc_chdate,//\chdate	Current date (as in headers).
			rsc_chdpl,//\chdpl	Current date in long format (for example, Wednesday, February 20, 2008).
			rsc_chdpa,//\chdpa	Current date in abbreviated format (for example, Wed, Feb 20, 2008).
			rsc_chtime,//\chtime	Current time (as in headers).
			rsc_chpgn,//\chpgn	Current page number (as in headers).
			rsc_sectnum,//\sectnum	Current section number (as in headers).
			rsc_chftn,//\chftn	Automatic footnote reference (footnotes follow in a group).
			rsc_chftnEnd,//Endnote
			rsc_chatn,//\chatn	Annotation reference (annotation text follows in a group).
			rsc_chftnsep,//\chftnsep	Anchoring character for footnote separator.
			rsc_chftnsepc,//\chftnsepc	Anchoring character for footnote continuation.
			rsc_page,//\page	Required page break.
			rsc_column,//\column	Required column break.
			rsc_line,//\line	Required line break (no paragraph break).
			rsc_softpage,//\softpage	Nonrequired page break. Emitted as it appears in galley view.
			rsc_softcol,//\softcol	Nonrequired column break. Emitted as it appears in galley view.
			rsc_softline,//\softline	Nonrequired line break. Emitted as it appears in galley view.
			rsc_tab,//\tab	Tab character. 
			rsc_emspace,//\emspace	Non-breaking space equal to width of character “m” in current font. Some old RTF writers use the construct '{' \emspace '  }' (with two spaces before the closing brace) to trick readers unaware of \emspace into parsing a regular space. A reader should interpret this as an \emspace and a regular space.
			rsc_enspace,//\enspace	Nonbreaking space equal to width of character “n” in current font. Some old RTF writers use the construct '{' \enspace '  }' (with two spaces before the closing brace) to trick readers unaware of \enspace into parsing a regular space. A reader should interpret this as an \enspace and a regular space.
			rsc_qmspace,//\qmspace	One-quarter em space.
			rsc_Formula,//\|	Formula character. (Used by Word 5.1 for the Macintosh as the beginning delimiter for a string of formula typesetting commands.)
			rsc_OptHyphen,//\-	Optional hyphen.
			rsc_NonBrHyphen,//\_	Non-breaking hyphen.
			rsc_NonBrSpace,//\~	Non-breaking space.
			rsc_SubEntry,//\:	Specifies a subentry in an index entry.
			rsc_zwbo,//\zwbo	Zero-width break opportunity. Used to insert break opportunity between two characters.
			rsc_zwnbo,//\zwnbo	Zero-width non-break opportunity. Used to remove break opportunity between two characters.
			rsc_zwj,//\zwj	Zero-width joiner. This is used for ligating (joining) characters.
			rsc_zwnj,//\zwnj	Zero-width nonjoiner. This is used for unligating a character. 
	} RtfSpecChar;   

	int					m_nTextWrapBreak;//\lbrN Text wrapping break of type
	int					m_nSoftHeight;//\softlheightN	Nonrequired line height. This is emitted as a prefix to each line.
	RtfSpecChar			m_eType;
	RtfCharProperty		m_oProperty;
	
	RtfCharSpecial()
	{
		m_eType				= rsc_none;
		m_nTextWrapBreak	= PROP_DEF;
		m_nSoftHeight		= PROP_DEF;
	}
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString sResult;
		sResult += L"{";
		sResult += m_oProperty.RenderToRtf( oRenderParameter );
		switch( m_eType )
		{
			case rsc_chdate:		sResult += L"\\chdate";		break;
			case rsc_chdpl:			sResult += L"\\chdpl";		break;
			case rsc_chdpa:			sResult += L"\\chdpa";		break;
			case rsc_chtime:		sResult += L"\\chtime";		break;
			case rsc_chpgn:			sResult += L"\\chpgn";		break;
			case rsc_sectnum:		sResult += L"\\sectnum";		break;
			case rsc_chftn:			sResult += L"\\chftn";		break;
			case rsc_chftnEnd:		sResult += L"\\chftn";		break;
			case rsc_chatn:			sResult += L"\\chatn";		break;
			case rsc_chftnsep:		sResult += L"\\chftnsep";	break;
			case rsc_chftnsepc:		sResult += L"\\chftnsepc";	break;
			case rsc_page:			sResult += L"\\page";		break;
			case rsc_column:		sResult += L"\\column";		break;
			case rsc_line:			sResult += L"\\line";		break;
			case rsc_softpage:		sResult += L"\\softpage";	break;
			case rsc_softcol:		sResult += L"\\softcol";		break;
			case rsc_softline:		sResult += L"\\softline";	break;
			case rsc_tab:			sResult += L"\\tab";			break;
			case rsc_Formula:		sResult += L"\\|";			break;
			case rsc_OptHyphen:		sResult += L"\\-";			break;
			case rsc_NonBrHyphen:	sResult += L"\\_";			break;
			case rsc_NonBrSpace:	sResult += L"\\~";			break;
			case rsc_zwbo:			sResult += L"\\zwbo";		break;
			case rsc_zwnbo:			sResult += L"\\zwnbo";		break;
			case rsc_zwj:			sResult += L"\\zwj";			break;
			case rsc_zwnj:			sResult += L"\\zwnj";		break;
		}
		if( PROP_DEF != m_nTextWrapBreak )
			sResult += L"\\par";
		//switch ( m_nTextWrapBreak ) //не воспринимается word
		//{
		//	case 0: sResult += L"\\lbr0";break;
		//	case 1: sResult += L"\\lbr1";break;
		//	case 2: sResult += L"\\lbr2";break;
		//	case 3: sResult += L"\\lbr3";break;
		//}
		if( PROP_DEF != m_nSoftHeight )
		{
			sResult.AppendFormat( L"\\softlheight%d", m_nSoftHeight );
		}
		sResult += L"}";
		return sResult;
	}
	CString _RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		switch( m_eType )
		{
			case rsc_chdate:		sResult += L"";										break;
			case rsc_chdpl:			sResult += L"";										break;
			case rsc_chdpa:			sResult += L"";										break;
			case rsc_chtime:		sResult += L"";										break;
			case rsc_chpgn:			sResult += L"<w:pgNum />";							break;
			case rsc_sectnum:		sResult += L"";										break;
			case rsc_chftn:			sResult += L"<w:footnoteRef/>";						break;
			case rsc_chftnEnd:		sResult += L"<w:endnoteRef/>";						break;
			case rsc_chatn:			sResult += L"<w:annotationRef />";					break;
			case rsc_chftnsep:		sResult += L"<w:separator />";						break;
			case rsc_chftnsepc:		sResult += L"<w:continuationSeparator/>";			break;
			case rsc_page:			sResult += L"<w:br w:type=\"page\"/>";				break;
			case rsc_column:		sResult += L"<w:br w:type=\"column\"/>";			break;
			case rsc_line:			sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"none\"/>";break;
			case rsc_softpage:		sResult += L"";										break;
			case rsc_softcol:		sResult += L"";										break;
			case rsc_softline:		sResult += L"";										break;
			case rsc_tab:			sResult += L"<w:tab/>";								break;
			case rsc_emspace:		sResult += L"";										break;
			case rsc_qmspace:		sResult += L"";										break;
			case rsc_Formula:		sResult += L"";										break;
			case rsc_zwbo:			sResult += L"";										break;
			case rsc_zwnbo:			sResult += L"";										break;
			case rsc_zwj:			sResult += L"";										break;
			case rsc_zwnj:			sResult += L"";										break;
			case rsc_OptHyphen:		sResult += L"<w:t xml:space=\"preserve\">-</w:t>";	break;//<w:softHyphen/>
			case rsc_NonBrHyphen:	sResult += L"<w:t xml:space=\"preserve\">-</w:t>";	break;//<w:nonBreakHyphen/>
			case rsc_NonBrSpace:	sResult += L"<w:t xml:space=\"preserve\"> </w:t>";	break;
		}
		switch ( m_nTextWrapBreak )
		{
			case 0: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"none\"/>";		break;
			case 1: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"left\"/>";		break;
			case 2: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"right\"/>";	break;
			case 3: sResult += L"<w:br w:type=\"textWrapping\" w:clear=\"all\"/>";		break;
		}
		return sResult;
	}
	CString RenderToOOX(RenderParameter oRenderParameter)
	{
		CString sResult;
		if(RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType)
		{
			sResult += L"<w:r>";
				sResult += L"<w:rPr>";
					sResult += m_oProperty.RenderToOOX(oRenderParameter);
				sResult += L"</w:rPr>";
				sResult += _RenderToOOX(oRenderParameter);
			sResult += L"</w:r>";
		}
		else if(RENDER_TO_OOX_PARAM_TEXT	== oRenderParameter.nType || 
				RENDER_TO_OOX_PARAM_MATH	== oRenderParameter.nType ||
				RENDER_TO_OOX_PARAM_PLAIN	== oRenderParameter.nType)
			sResult += _RenderToOOX(oRenderParameter);
		return sResult;
	}
};
typedef boost::shared_ptr<RtfCharSpecial> RtfCharSpecialPtr;

class RtfChar : public IDocumentElement
{            
protected: CString m_sChars;
public: 
	RtfChar()
	{
		m_bRtfEncode = true;
	}
	RtfCharProperty m_oProperty;
	bool m_bRtfEncode;

	int GetType()
	{
		return TYPE_RTF_CHAR;
	}
	void AddText(CString text)
	{
		m_sChars += text;
	}
	void setText(CString text)
	{
		m_sChars = text;
	}

	CString GetText()
	{
		return m_sChars;
	}
    CString RenderToOOX(RenderParameter oRenderParameter);
    static CString renderRtfText( CString& sText, void* poDocument, RtfCharProperty* oCharProperty = NULL );

    CString RenderToRtf(RenderParameter oRenderParameter);
private: 
	CString renderTextToXML( CString sParam, bool bDelete = false );
};
class RtfCharNative : public RtfChar
{            
public: 
	CString RenderToRtf(RenderParameter oRenderParameter)
	{
		CString result;
		if( RENDER_TO_RTF_PARAM_CHAR ==  oRenderParameter.nType )
		{
			result = m_sChars;
		}
		else
		{
			CString sText = m_sChars;
			if( L"" != sText )
			{
				result += L"{";
				result += m_oProperty.RenderToRtf( oRenderParameter );
				result += L" " + sText;
				result += L"}";
			}
		}
		return result;
	}
};
typedef boost::shared_ptr<RtfCharNative> RtfCharNativePtr;
typedef boost::shared_ptr<RtfChar> RtfCharPtr;
