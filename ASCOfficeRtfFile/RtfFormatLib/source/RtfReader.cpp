#include "RtfReader.h"
#include "Writer/OOXWriter.h"
#include "DestinationCommand.h"


RtfReader::RtfReader(RtfDocument& oDocument, std::wstring sFilename ):m_oDocument(oDocument),m_sFilename(sFilename)
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

        //применяем параметры codepage от текущего шрифта todo associated fonts.
        RtfFont oFont;
        if( true == oDocument.m_oFontTable.GetFont( oReader.m_oState->m_oCharProp.m_nFont, oFont ) && !m_bUseGlobalCodepage)
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
                    }
                    else
                        nCodepage = CP_ACP;
                    break;
                }
            case RtfDocumentProperty::cp_mac:   nCodepage = CP_MACCP;   break; //?? todooo
            case RtfDocumentProperty::cp_pc:    nCodepage = 437;        break; //ms dos latin us
            case RtfDocumentProperty::cp_pca:   nCodepage = 850;        break; //ms dos latin eu
            }
        }
        //если ничего нет ставим ANSI
        if( -1 == nCodepage )
            nCodepage = CP_ACP;

        sResult = RtfUtility::convert_string(sCharString.begin(), sCharString.end(), nCodepage);
    }
    return sResult;
}
