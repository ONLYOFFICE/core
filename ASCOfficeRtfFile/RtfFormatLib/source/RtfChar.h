#pragma once 
#include "Basic.h"

#include "Utils.h"
#include "RtfProperty.h"

class RtfAbsPosTab : public IDocumentElement
{
public: typedef enum {l_none,
						l_ptablnone, //\ptablnone 	Absolute position tab with a blank leading (default).
						l_ptabldot, //\ptabldot 	Absolute position tab with a leading that uses period symbols (.....).
						l_ptablminus, //\ptablminus 	Absolute position tab with a leading that uses minus symbols (-----).
						l_ptabluscore, //\ptabluscore 	Absolute position tab with a leading that uses underscore symbols (_____).
						l_ptablmdot, //\ptablmdot 	Absolute position tab with a leading that uses middle dot symbols (•••••).
						} Leader;
public: Leader m_eLeader;
public: typedef enum { a_none,
						a_left,
						a_center,
						a_right,
						} Alignment;
public: Alignment m_eAlignment;
public: typedef enum {r_none,
						r_margin,
						r_indent,
						} Relative;
public: Relative m_eRelative;

public: RtfAbsPosTab()
		{
		}
public: bool IsValid()
		{
			return a_none != m_eAlignment && r_none != m_eRelative;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			switch( m_eLeader )
			{
				case l_ptablnone: sResult.Append(_T("\\ptablnone"));break;
				case l_ptabldot: sResult.Append(_T("\\ptabldot"));break;
				case l_ptablminus: sResult.Append(_T("\\ptablminus"));break;
				case l_ptabluscore: sResult.Append(_T("\\ptabluscore"));break;
				case l_ptablmdot: sResult.Append(_T("\\ptablmdo"));break;
			}
			switch( m_eRelative )
			{
				case r_margin: sResult.Append(_T("\\pmartabq"));break;
				case r_indent: sResult.Append(_T("\\pindtabq"));break;
			}
			switch( m_eAlignment )
			{
				case a_left: sResult.Append(_T("l"));break;
				case a_center: sResult.Append(_T("c"));break;
				case a_right: sResult.Append(_T("r"));break;
			}
			if( false == sResult.IsEmpty() )
				sResult = _T("{") + sResult + _T("}");
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			switch( m_eLeader )
			{
				case l_ptablnone: sResult.Append(_T(" w:leader=\"none\""));break;
				case l_ptabldot: sResult.Append(_T(" w:leader=\"dot\""));break;
				case l_ptablminus: sResult.Append(_T(" w:leader=\"hyphen\""));break;
				case l_ptabluscore: sResult.Append(_T(" w:leader=\"underscore\""));break;
				case l_ptablmdot: sResult.Append(_T(" w:leader=\"middleDot\""));break;
			}
			switch( m_eRelative )
			{
				case r_margin: sResult.Append(_T(" w:relativeTo=\"margin\""));break;
				case r_indent: sResult.Append(_T(" w:relativeTo=\"indent\""));break;
			}
			switch( m_eAlignment )
			{
				case a_left: sResult.Append(_T(" w:alignment=\"left\""));break;
				case a_center: sResult.Append(_T(" w:alignment=\"center\""));break;
				case a_right: sResult.Append(_T(" w:alignment=\"right\""));break;
			}
			if( false == sResult.IsEmpty() )
				sResult = _T("<w:ptab") + sResult + _T("/>");
			return sResult;
		}
};
class RtfCharSpecial : public IDocumentElement
{
public: typedef enum {
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

public: int m_nTextWrapBreak;//\lbrN Text wrapping break of type
public: int m_nSoftHeight;//\softlheightN	Nonrequired line height. This is emitted as a prefix to each line.
public: RtfSpecChar m_eType;
public: RtfCharProperty m_oProperty;
public: RtfCharSpecial()
		{
			m_eType = rsc_none;
			m_nTextWrapBreak = PROP_DEF;
			m_nSoftHeight = PROP_DEF;
		}
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString sResult;
			sResult.Append( _T("{") );
			sResult.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
			switch( m_eType )
			{
				case rsc_chdate: sResult.Append( _T("\\chdate") );break;
				case rsc_chdpl: sResult.Append( _T("\\chdpl") );break;
				case rsc_chdpa: sResult.Append( _T("\\chdpa") );break;
				case rsc_chtime: sResult.Append( _T("\\chtime") );break;
				case rsc_chpgn: sResult.Append( _T("\\chpgn") );break;
				case rsc_sectnum: sResult.Append( _T("\\sectnum") );break;
				case rsc_chftn: sResult.Append( _T("\\chftn") );break;
				case rsc_chftnEnd: sResult.Append( _T("\\chftn") );break;
				case rsc_chatn: sResult.Append( _T("\\chatn") );break;
				case rsc_chftnsep: sResult.Append( _T("\\chftnsep") );break;
				case rsc_chftnsepc: sResult.Append( _T("\\chftnsepc") );break;
				case rsc_page: sResult.Append( _T("\\page") );break;
				case rsc_column: sResult.Append( _T("\\column") );break;
				case rsc_line: sResult.Append( _T("\\line") );break;
				case rsc_softpage: sResult.Append( _T("\\softpage") );break;
				case rsc_softcol: sResult.Append( _T("\\softcol") );break;
				case rsc_softline: sResult.Append( _T("\\softline") );break;
				case rsc_tab: sResult.Append( _T("\\tab") );break;
				case rsc_Formula: sResult.Append( _T("\\|") );break;
				case rsc_OptHyphen: sResult.Append( _T("\\-") );break;
				case rsc_NonBrHyphen: sResult.Append( _T("\\_") );break;
				case rsc_NonBrSpace: sResult.Append( _T("\\~") );break;
				case rsc_zwbo: sResult.Append( _T("\\zwbo") );break;
				case rsc_zwnbo: sResult.Append( _T("\\zwnbo") );break;
				case rsc_zwj: sResult.Append( _T("\\zwj") );break;
				case rsc_zwnj: sResult.Append( _T("\\zwnj") );break;
			}
			if( PROP_DEF != m_nTextWrapBreak )
				sResult.Append( _T("\\par") );
			//switch ( m_nTextWrapBreak ) //не воспринимается word
			//{
			//	case 0: sResult.Append( _T("\\lbr0") );break;
			//	case 1: sResult.Append( _T("\\lbr1") );break;
			//	case 2: sResult.Append( _T("\\lbr2") );break;
			//	case 3: sResult.Append( _T("\\lbr3") );break;
			//}
			if( PROP_DEF != m_nSoftHeight )
			{
				sResult.AppendFormat( _T("\\softlheight%d"), m_nSoftHeight );
			}
			sResult.Append( _T("}") );
			return sResult;
		}
public: CString _RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			switch( m_eType )
			{
			case rsc_chdate: sResult.Append( _T("") );break;
				case rsc_chdpl: sResult.Append( _T("") );break;
				case rsc_chdpa: sResult.Append( _T("") );break;
				case rsc_chtime: sResult.Append( _T("") );break;
				case rsc_chpgn: sResult.Append( _T("<w:pgNum />") );break;
				case rsc_sectnum: sResult.Append( _T("") );break;
				case rsc_chftn: sResult.Append( _T("<w:footnoteRef/>") );break;
				case rsc_chftnEnd: sResult.Append( _T("<w:endnoteRef/>") );break;
				case rsc_chatn: sResult.Append( _T("<w:annotationRef />") );break;
				case rsc_chftnsep: sResult.Append( _T("<w:separator />") );break;
				case rsc_chftnsepc: sResult.Append( _T("<w:continuationSeparator/>") );break;
				case rsc_page: sResult.Append( _T("<w:br w:type=\"page\"/>") );break;
				case rsc_column: sResult.Append( _T("<w:br w:type=\"column\"/>") );break;
				case rsc_line: sResult.Append( _T("<w:br w:type=\"textWrapping\" w:clear=\"none\"/>") );break;
				case rsc_softpage: sResult.Append( _T("") );break;
				case rsc_softcol: sResult.Append( _T("") );break;
				case rsc_softline: sResult.Append( _T("") );break;
				case rsc_tab: sResult.Append( _T("<w:tab/>") );break;
				case rsc_emspace: sResult.Append( _T("") );break;
				case rsc_qmspace: sResult.Append( _T("") );break;
				case rsc_Formula: sResult.Append( _T("") );break;
				case rsc_OptHyphen: sResult.Append( _T("<w:t xml:space=\"preserve\">-</w:t>") );break;//<w:softHyphen/>
				case rsc_NonBrHyphen: sResult.Append( _T("<w:t xml:space=\"preserve\">-</w:t>") );break;//<w:nonBreakHyphen/>
				case rsc_NonBrSpace: sResult.Append( _T("<w:t xml:space=\"preserve\"> </w:t>") );break;
				case rsc_zwbo: sResult.Append( _T("") );break;
				case rsc_zwnbo: sResult.Append( _T("") );break;
				case rsc_zwj: sResult.Append( _T("") );break;
				case rsc_zwnj: sResult.Append( _T("") );break;
			}
			switch ( m_nTextWrapBreak )
			{
				case 0: sResult.Append( _T("<w:br w:type=\"textWrapping\" w:clear=\"none\"/>") );break;
				case 1: sResult.Append( _T("<w:br w:type=\"textWrapping\" w:clear=\"left\"/>") );break;
				case 2: sResult.Append( _T("<w:br w:type=\"textWrapping\" w:clear=\"right\"/>") );break;
				case 3: sResult.Append( _T("<w:br w:type=\"textWrapping\" w:clear=\"all\"/>") );break;
			}
			return sResult;
		}
public: CString RenderToOOX(RenderParameter oRenderParameter)
		{
			CString sResult;
			if(RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType)
			{
				sResult.Append(_T("<w:r>"));
				sResult.Append(_T("<w:rPr>"));
				sResult.Append( m_oProperty.RenderToOOX(oRenderParameter) );
				sResult.Append(_T("</w:rPr>"));
				sResult.Append( _RenderToOOX(oRenderParameter) );
				sResult.Append(_T("</w:r>"));
			}
			else if(RENDER_TO_OOX_PARAM_TEXT == oRenderParameter.nType || 
					RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType ||
					RENDER_TO_OOX_PARAM_PLAIN == oRenderParameter.nType)
				sResult.Append( _RenderToOOX(oRenderParameter) );
			return sResult;
		}
};
typedef boost::shared_ptr<RtfCharSpecial> RtfCharSpecialPtr;

class RtfChar : public IDocumentElement
{            
public: RtfChar()
		{
			m_bRtfEncode = true;
		}
protected: CString m_sChars;
public: RtfCharProperty m_oProperty;
public: bool m_bRtfEncode;

public: int GetType()
		{
			return TYPE_RTF_CHAR;
		}
public: void AddText(CString text)
		{
			m_sChars.Append( text );
		}
public: void setText(CString text)
		{
			m_sChars = text;
		}
public: CString GetText()
		{
			return m_sChars;
		}
private: CString renderTextToXML( CString sParam )
		{
			CString sResult;
			if( _T("Text") == sParam )
            {
                sResult.AppendFormat( _T("<w:t xml:space= \"preserve\">%ls</w:t>"), Utils::PrepareToXML( m_sChars ).GetBuffer() );
            }
			else if( _T("Math") == sParam )
            {
                sResult.AppendFormat( _T("<m:t xml:space= \"preserve\">%ls</m:t>"), Utils::PrepareToXML( m_sChars ).GetBuffer());
            }
			return sResult;
		}
public:
    CString RenderToOOX(RenderParameter oRenderParameter)
    {
        CString sResult;
        if(RENDER_TO_OOX_PARAM_RUN == oRenderParameter.nType)
        {
            sResult.Append(_T("<w:r>"));
            sResult.Append(_T("<w:rPr>"));
            sResult.Append( m_oProperty.RenderToOOX(oRenderParameter) );
            sResult.Append(_T("</w:rPr>"));
            sResult.Append( renderTextToXML(_T("Text")) );
            sResult.Append(_T("</w:r>"));
        }
        else if(RENDER_TO_OOX_PARAM_TEXT == oRenderParameter.nType)
            sResult = renderTextToXML( _T("Text") );
        else if( RENDER_TO_OOX_PARAM_MATH == oRenderParameter.nType)
            sResult = renderTextToXML( _T("Math") );
        else if( RENDER_TO_OOX_PARAM_PLAIN == oRenderParameter.nType)
            sResult = m_sChars;
        return sResult;
    }

    static CString renderRtfText( CString& sText, void* poDocument, RtfCharProperty* oCharProperty = NULL );

    CString RenderToRtf(RenderParameter oRenderParameter)
    {
        CString result;
        if( RENDER_TO_RTF_PARAM_CHAR ==  oRenderParameter.nType )
        {
            if( true == m_bRtfEncode )
                result.Append( renderRtfText( m_sChars, oRenderParameter.poDocument, &m_oProperty ) );
            else
                result.Append( m_sChars );
        }
        else
        {
            CString sText;
            if( true == m_bRtfEncode )
                sText = renderRtfText( m_sChars, oRenderParameter.poDocument, &m_oProperty );
            else
                sText = m_sChars;
            if( _T("") != sText )
            {
                result.Append(_T("{"));
                result.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
                result.Append( _T(" ") + sText );
                result.Append(_T("}"));
            }
        }
        return result;
    }
};
class RtfCharNative : public RtfChar
{            
public: CString RenderToRtf(RenderParameter oRenderParameter)
		{
			CString result;
			if( RENDER_TO_RTF_PARAM_CHAR ==  oRenderParameter.nType )
			{
				result = m_sChars;
			}
			else
			{
				CString sText = m_sChars;
				if( _T("") != sText )
				{
					result.Append(_T("{"));
					result.Append( m_oProperty.RenderToRtf( oRenderParameter ) );
					result.Append( _T(" ") + sText );
					result.Append(_T("}"));
				}
			}
			return result;
		}
};
typedef boost::shared_ptr<RtfCharNative> RtfCharNativePtr;
typedef boost::shared_ptr<RtfChar> RtfCharPtr;
