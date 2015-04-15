#include "RtfReader.h"
#include "Writer/OOXWriter.h"
#include "DestinationCommand.h"

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