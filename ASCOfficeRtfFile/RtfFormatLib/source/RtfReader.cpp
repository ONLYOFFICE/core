#include "RtfReader.h"
#include "Writer/OOXWriter.h"
#include "DestinationCommand.h"

#if !defined (_WIN32) && !defined(_WIN64)
    #include <boost/locale.hpp>
    #include <locale>
    typedef std::codecvt<wchar_t, char, mbstate_t> codecvt_type;
#endif

RtfReader::RtfReader(RtfDocument& oDocument, CString sFilename ):m_oDocument(oDocument),m_sFilename(sFilename)
{
	m_oState = ReaderStatePtr(new ReaderState());
	m_nFootnote = PROP_DEF;
	m_nDefFont = PROP_DEF;
	m_convertationManager = NULL;
}
bool RtfReader::Load()
{
	m_oLex.SetSource( m_sFilename );
	RtfNormalReader oNormalReader( m_oDocument, (*this) );
	oNormalReader.Parse( m_oDocument, (*this) );
	m_oLex.CloseSource();
    return true;
}
void RtfReader::PushState()
{
	ReaderStatePtr psaveNew = ReaderStatePtr(new ReaderState());
	psaveNew -> m_nUD = m_oState->m_nUD;
	psaveNew -> m_oCharProp = m_oState->m_oCharProp;
	psaveNew -> m_oParagraphProp = m_oState->m_oParagraphProp;
	psaveNew -> m_oRowProperty = m_oState->m_oRowProperty;
	psaveNew -> m_oCellProperty = m_oState->m_oCellProperty;
	psaveNew -> m_oCurOldList = m_oState->m_oCurOldList;
	//psaveNew -> m_oSectionProp = m_oState->m_oSectionProp;
	psaveNew -> psave = m_oState;
	m_oState = psaveNew;

	if( PROP_DEF == m_oState->m_oCharProp.m_nFont )
		m_oState->m_oCharProp.m_nFont = m_nDefFont;
}
void RtfReader::PopState()
{
	if( 0 != m_oState->psave )
		m_oState = m_oState->psave;
	//delete psaveOld;
}

CString RtfAbstractReader::ExecuteTextInternalCodePage( std::string& sCharString, RtfDocument& oDocument, RtfReader& oReader)
{
    CString sResult;

    if( false == sCharString.empty())
    {
        int         nCodepage = -1;
        std::string sCodepage;

        //применяем параметры codepage от текущего шрифта todo associated fonts.
        RtfFont oFont;
        if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) )
        {
            if( PROP_DEF != oFont.m_nCharset )
            {
                nCodepage = RtfUtility::CharsetToCodepage( oFont.m_nCharset );
            }
            else if( PROP_DEF != oFont.m_nCodePage )
            {
                nCodepage = oFont.m_nCodePage;
            }
        }
        //от настроек документа
        if( -1 == nCodepage && RtfDocumentProperty::cp_none != oDocument.m_oProperty.m_eCodePage )
        {
            switch ( oDocument.m_oProperty.m_eCodePage )
            {
            case RtfDocumentProperty::cp_ansi:
                {
                    if( PROP_DEF != oDocument.m_oProperty.m_nAnsiCodePage )
                    {
                        nCodepage = oDocument.m_oProperty.m_nAnsiCodePage;
                        sCodepage = oDocument.m_oProperty.m_sAnsiCodePage;
                    }
                    else
                        nCodepage = CP_ACP;
                    break;
                }
            case RtfDocumentProperty::cp_mac:   nCodepage = CP_MACCP;   sCodepage = "macintosh";    break; //?? todooo
            case RtfDocumentProperty::cp_pc:    nCodepage = 437;        sCodepage = "IBM437";       break; //ms dos latin us
            case RtfDocumentProperty::cp_pca:   nCodepage = 850;        sCodepage = "ibm850";       break; //ms dos latin eu
            }
        }
        //если ничего нет ставим ANSI
        if( -1 == nCodepage )
            nCodepage = CP_ACP;

#if defined (_WIN32) || defined (_WIN64)
        int nLengthW ;
        nLengthW = MultiByteToWideChar(nCodepage, 0, sCharString, -1, NULL, NULL);
        MultiByteToWideChar(nCodepage, 0, sCharString, -1, sResult.GetBuffer( nLengthW ), nLengthW);
        sResult.ReleaseBuffer();
#else
        if (nCodepage > 0)
        {
            std::mbstate_t state;

            boost::locale::generator gen;
            std::locale loc(gen(sCodepage.c_str()));

            const codecvt_type& cdcvt = std::use_facet<codecvt_type>(loc);

            wchar_t * wchars = new wchar_t [sCharString.size()+ 1];

            const char *in_next = 0;
            wchar_t *out_next = 0;

            std::codecvt_base::result r;
            r = cdcvt.in (state, sCharString.c_str(), sCharString.c_str() + sCharString.length(), in_next,
                          wchars, wchars + sCharString.size() + 1, out_next);
            *out_next = '\0';

            sResult = CString(wchars);

            delete [] wchars;
        }
        else
        {
            //ansi
            std::wstring s(sCharString.begin(), sCharString.end());

            sResult = std_string2string(s);
        }

#endif
    }
    return sResult;
}
