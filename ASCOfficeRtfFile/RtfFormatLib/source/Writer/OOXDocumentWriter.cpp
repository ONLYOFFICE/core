#include "OOXDocumentWriter.h"
//#include "OOXContentTypesWriter.h"
#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXFootnoteWriter.h"
#include "OOXStylesWriter.h"

//#include "../../../../ASCOfficeDocxFile2/BinReader/ContentTypesWriter.h"

CString OOXDocumentWriter::CreateXmlStart()
{
	//пишем Footnotes
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;



	//пишем document.xml
	CString sResult = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n");
	sResult.Append( _T("<w:document") );
	sResult.Append( _T(" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"") );
	sResult.Append( _T(" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\"") );
	sResult.Append( _T(" xmlns:v=\"urn:schemas-microsoft-com:vml\"") );
	sResult.Append( _T(" xmlns:o=\"urn:schemas-microsoft-com:office:office\"") );
	sResult.Append( _T(" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\"") );
	sResult.Append( _T(" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\"") );
	sResult.Append( _T(" xmlns:w10=\"urn:schemas-microsoft-com:office:word\"") );
	sResult.Append( _T(" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"") );
	sResult.Append( _T(">") );

	sResult.Append( _T("<w:body>") );
	return sResult;
}
CString OOXDocumentWriter::CreateXmlEnd( )
{
	CString sResult ;

	//пишем все кроме document.xml
	RenderParameter oNewParam;
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;

	OOXEndnoteWriter* poEndnoteWriter = static_cast<OOXEndnoteWriter*>( m_oWriter.m_poEndnoteWriter );
	OOXFootnoteWriter* poFootnoteWriter = static_cast<OOXFootnoteWriter*>( m_oWriter.m_poFootnoteWriter );
	if( NULL != m_oDocument.m_oFootnoteCon )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( _T("continuationSeparator"), 1, m_oDocument.m_oFootnoteCon->RenderToOOX( oNewParam ) );
	}
	if( NULL != m_oDocument.m_oFootnoteSep )
	{
		oNewParam.poRels = poFootnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poFootnoteWriter->AddFootnoteBegin( _T("separator"), 0, m_oDocument.m_oFootnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteCon )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( _T("continuationSeparator"), 1, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}
	if( NULL != m_oDocument.m_oEndnoteSep )
	{
		oNewParam.poRels = poEndnoteWriter->m_oRelsWriter.get();
		oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
		poEndnoteWriter->AddEndnoteBegin( _T("separator"), 0, m_oDocument.m_oEndnoteSep->RenderToOOX(oNewParam) );
	}

	//fontTable.xml
	OOXFontTableWriter* poFontTableWriter = static_cast<OOXFontTableWriter*>( m_oWriter.m_poFontTableWriter );
	oNewParam.poRels = poFontTableWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_FONTTABLE;
	poFontTableWriter->AddContent( m_oDocument.m_oFontTable.RenderToOOX( oNewParam ) );

	//settings.xml
	OOXSettingsWriter* poSettingsWriter = static_cast<OOXSettingsWriter*>( m_oWriter.m_poSettingsWriter );
	oNewParam.poRels = poSettingsWriter->m_oRelsWriter.get();
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	poSettingsWriter->AddContent( m_oDocument.m_oProperty.RenderToOOX(oNewParam) );
	//poSettingsWriter->AddContent( m_oDocument.m_oColorSchemeMapping.RenderToOOX(oNewParam) );
	poSettingsWriter->AddContent( m_oDocument.m_oMathProp.RenderToOOX(oNewParam) );

	//numbering.xml
	OOXNumberingWriter* poNumberingWriter = static_cast<OOXNumberingWriter*>( m_oWriter.m_poNumberingWriter );
	oNewParam.poRels	= poNumberingWriter->m_oRelsWriter.get();
	oNewParam.nType		= RENDER_TO_OOX_PARAM_NUMBERING;
	poNumberingWriter->AddNumbering( m_oDocument.m_oListTabel.RenderToOOX(oNewParam) );
	poNumberingWriter->AddNumbering( m_oDocument.m_oListOverrideTabel.RenderToOOX(oNewParam) );

	//style.xml
	OOXStylesWriter* poStylesWriter = static_cast<OOXStylesWriter*>( m_oWriter.m_poStylesWriter );
	oNewParam.poRels	= poNumberingWriter->m_oRelsWriter.get();
	oNewParam.nType		= RENDER_TO_OOX_PARAM_NUMBERING;

	//core.xml
	oNewParam.poRels	= NULL;
	oNewParam.nType		= RENDER_TO_OOX_PARAM_CORE;
	oNewParam.poWriter	= m_oWriter.m_poDocPropsCore;
	m_oDocument.m_oInformation.RenderToOOX(oNewParam);

	//app.xml
	oNewParam.poRels	= NULL;
	oNewParam.nType		= RENDER_TO_OOX_PARAM_APP;
	oNewParam.poWriter	= m_oWriter.m_poDocPropsApp;
	m_oDocument.m_oInformation.RenderToOOX(oNewParam);


	//пишем финальные свойства секции
	oNewParam.poDocument = &m_oDocument;
	oNewParam.poWriter = &m_oWriter;
	oNewParam.poRels = &m_oWriter.m_oDocRels;
	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	sResult.Append( m_oDocument[0]->m_oProperty.RenderToOOX(oNewParam) );

	sResult.Append( _T("</w:body>") );
	sResult.Append( _T("</w:document>") );

	return sResult;
}