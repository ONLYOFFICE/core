#include "OOXWriter.h"

#include "../RtfDocument.h"
//#include "OOXContentTypesWriter.h"
#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXDocPropAppWriter.h"
#include "OOXDocPropCoreWriter.h"
#include "OOXFootnoteWriter.h"

OOXWriter::OOXWriter( RtfDocument& oDocument, CString sPath ) : 
		m_oDocument(oDocument),
		m_sTargetFolder(sPath),
		m_oRels( _T(""), oDocument ),
		m_oDocRels( _T("document.xml"), oDocument )
{
	nCurFitWidth = PROP_DEF;
	m_poFootnoteWriter = NULL;
	m_poEndnoteWriter = NULL;
	m_sDocumentFolder = _T("word");

	m_poDocumentWriter = new OOXDocumentWriter( *this, m_oDocument );
	m_poFootnoteWriter = new OOXFootnoteWriter( *this, m_oDocument );
	m_poEndnoteWriter = new OOXEndnoteWriter( *this, m_oDocument );
	m_poDocPropAppWriter = new OOXDocPropAppWriter( *this, m_oDocument );
	m_poDocPropCoreWriter = new OOXDocPropCoreWriter( *this, m_oDocument );
	m_poFontTableWriter = new OOXFontTableWriter( *this, m_oDocument );
	m_poNumberingWriter = new OOXNumberingWriter( *this, m_oDocument );
	m_poSettingsWriter = new OOXSettingsWriter( *this, m_oDocument );
	m_poStylesWriter = new OOXStylesWriter( *this, m_oDocument );

}
OOXWriter::~OOXWriter()
{
	delete ((OOXDocumentWriter*)m_poDocumentWriter);
	delete ((OOXFootnoteWriter*)m_poFootnoteWriter);
	delete ((OOXEndnoteWriter*)m_poEndnoteWriter);
	delete ((OOXDocPropAppWriter*)m_poDocPropAppWriter);
	delete ((OOXDocPropCoreWriter*)m_poDocPropCoreWriter);
	delete ((OOXFontTableWriter*)m_poFontTableWriter);
	delete ((OOXNumberingWriter*)m_poNumberingWriter);
	delete ((OOXSettingsWriter*)m_poSettingsWriter);
	delete ((OOXStylesWriter*)m_poStylesWriter);
}
bool OOXWriter::Save()
{
	int nItemsCount = ((OOXDocumentWriter*)m_poDocumentWriter)->GetCount();
	SaveByItemStart();
	for( int i = 0; i < nItemsCount; i++ )
	{
		SaveByItem();
	}
	return SaveByItemEnd();
}
bool OOXWriter::SaveByItemStart()
{
	//((OOXFootnoteWriter*)m_poFootnoteWriter)->Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);
	//((OOXEndnoteWriter*)m_poEndnoteWriter)->Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);

	//if( m_oFootnoteSep->GetCount() )
	//{
	//	oNewParam = oRenderParameter;
	//	oNewParam.poRels = &poFootnoteWriter->m_oRelsWriter;
	//	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	//	poFootnoteWriter->AddFootnote( _T("separator"), -1, m_oFootnoteSep->RenderToOOX(oNewParam) );
	//}
	//if( m_oFootnoteCon->GetCount() )
	//{
	//	oNewParam = oRenderParameter;
	//	oNewParam.poRels = &poFootnoteWriter->m_oRelsWriter;
	//	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	//	poFootnoteWriter->AddFootnote( _T("continuationSeparator"), 0, m_oFootnoteCon->RenderToOOX( oNewParam ) );
	//}
	//if( m_oEndnoteSep->GetCount() )
	//{
	//	oNewParam = oRenderParameter;
	//	oNewParam.poRels = &poEndnoteWriter->m_oRelsWriter;
	//	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	//	poEndnoteWriter->AddEndnote( _T("separator"), -1, m_oEndnoteSep->RenderToOOX(oNewParam) );
	//}
	//if( m_oEndnoteCon->GetCount() )
	//{
	//	oNewParam = oRenderParameter;
	//	oNewParam.poRels = &poEndnoteWriter->m_oRelsWriter;
	//	oNewParam.nType = RENDER_TO_OOX_PARAM_UNKNOWN;
	//	poEndnoteWriter->AddEndnote( _T("continuationSeparator"), 0, m_oEndnoteSep->RenderToOOX(oNewParam) );
	//}

	return ((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItemStart( m_sTargetFolder );
}
bool OOXWriter::SaveByItem()
{
	return ((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItem();
}
bool OOXWriter::SaveByItemEnd()
{
	//OOX::CContentTypes oContentTypes;
	//
	////docProps
 //   OOX::CPath pathDocProps = sDstPath + FILE_SEPARATOR_STR + _T("docProps");
 //   FileSystem::Directory::CreateDirectory(pathDocProps.GetPath());
	//
 //   OOX::CPath DocProps = CString(_T("docProps"));

	//OOX::CApp* pApp = new OOX::CApp();
	//if (pApp)
	//{
	//	pApp->SetApplication(_T("OnlyOffice"));
	//	pApp->SetAppVersion(_T("3.0000"));
	//	pApp->SetDocSecurity(0);
	//	pApp->SetScaleCrop(false);
	//	pApp->SetLinksUpToDate(false);
	//	pApp->SetSharedDoc(false);
	//	pApp->SetHyperlinksChanged(false);
	//	
	//	pApp->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), DocProps, oContentTypes);
	//	delete pApp;
	//}				
	//OOX::CCore* pCore = new OOX::CCore();
	//if (pCore)
	//{
	//	pCore->SetCreator(_T(""));
	//	pCore->SetLastModifiedBy(_T(""));
	//	pCore->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), DocProps, oContentTypes);
	//	delete pCore;
	//} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	OOXThemeWriter oThemeWr(*this, m_oDocument);
	oThemeWr.Save(m_sTargetFolder);

	((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItemEnd();

	((OOXFootnoteWriter*)m_poFootnoteWriter)->Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);
	((OOXEndnoteWriter*)m_poEndnoteWriter)->Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);
	((OOXNumberingWriter*)m_poNumberingWriter)->Save(m_sTargetFolder);
	((OOXStylesWriter*)m_poStylesWriter)->Save(m_sTargetFolder);
	((OOXFontTableWriter*)m_poFontTableWriter)->Save(m_sTargetFolder);
	((OOXDocPropAppWriter*)m_poDocPropAppWriter)->Save(m_sTargetFolder);
	((OOXDocPropCoreWriter*)m_poDocPropCoreWriter)->Save(m_sTargetFolder);
	((OOXSettingsWriter*)m_poSettingsWriter)->Save(m_sTargetFolder); //setting в последнюю очередь

	bool nResult = true;
	nResult &= m_oContentTypes.Save(m_sTargetFolder);
	nResult &= m_oRels.Save(m_sTargetFolder);
	nResult &= m_oDocRels.Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);
	for( int i = 0; i < (int)m_oCustomRelsWriter.size(); i++ )
		m_oCustomRelsWriter[i]->Save(m_sTargetFolder + _T("\\") + m_sDocumentFolder);
	return nResult;
}