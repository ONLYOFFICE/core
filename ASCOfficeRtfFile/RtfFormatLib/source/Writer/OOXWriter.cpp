#include "OOXWriter.h"

#include "../RtfDocument.h"

#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXFootnoteWriter.h"

#include "../../../../Common/DocxFormat/Source/DocxFormat/Docx.h"

#include "../../../../ASCOfficeDocxFile2/BinReader/DefaultThemeWriter.h"

OOXWriter::OOXWriter( RtfDocument& oDocument, CString sPath ) : 
		m_oDocument(oDocument),
		m_sTargetFolder(sPath),
		m_oRels( _T(""), oDocument ),
		m_oDocRels( _T("document.xml"), oDocument )
{
	nCurFitWidth		= PROP_DEF;
	m_poFootnoteWriter	= NULL;
	m_poEndnoteWriter	= NULL;

	m_poDocumentWriter	= new OOXDocumentWriter( *this, m_oDocument );
	m_poFootnoteWriter	= new OOXFootnoteWriter( *this, m_oDocument );
	m_poEndnoteWriter	= new OOXEndnoteWriter( *this, m_oDocument );
	m_poFontTableWriter = new OOXFontTableWriter( *this, m_oDocument );
	m_poNumberingWriter = new OOXNumberingWriter( *this, m_oDocument );
	m_poSettingsWriter	= new OOXSettingsWriter( *this, m_oDocument );
	m_poStylesWriter	= new OOXStylesWriter( *this, m_oDocument );

	m_poDocPropsApp		= new OOX::CApp();
	m_poDocPropsCore	= new OOX::CCore();

//default properties

	if (m_poDocPropsApp)
	{		
		((OOX::CApp*)m_poDocPropsApp)->SetDocSecurity(0);
		((OOX::CApp*)m_poDocPropsApp)->SetScaleCrop(false);
		((OOX::CApp*)m_poDocPropsApp)->SetLinksUpToDate(false);
		((OOX::CApp*)m_poDocPropsApp)->SetSharedDoc(false);
		((OOX::CApp*)m_poDocPropsApp)->SetHyperlinksChanged(false);
	}
	if (m_poDocPropsCore)
	{
		((OOX::CCore*)m_poDocPropsCore)->SetCreator(_T(""));
		((OOX::CCore*)m_poDocPropsCore)->SetLastModifiedBy(_T(""));
	}
}
OOXWriter::~OOXWriter()
{
	delete ((OOXDocumentWriter*)m_poDocumentWriter);
	delete ((OOXFootnoteWriter*)m_poFootnoteWriter);
	delete ((OOXEndnoteWriter*)m_poEndnoteWriter);
	delete ((OOXFontTableWriter*)m_poFontTableWriter);
	delete ((OOXNumberingWriter*)m_poNumberingWriter);
	delete ((OOXSettingsWriter*)m_poSettingsWriter);
	delete ((OOXStylesWriter*)m_poStylesWriter);
	delete ((OOX::CApp*)m_poDocPropsApp);
	delete ((OOX::CCore*)m_poDocPropsCore);
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
	return ((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItemStart( m_sTargetFolder );
}
bool OOXWriter::SaveByItem()
{
	return ((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItem();
}
bool OOXWriter::SaveByItemEnd()
{
	OOX::CContentTypes oContentTypes;

	OOX::CPath pathWord = m_sTargetFolder + FILE_SEPARATOR_STR + _T("word");
    FileSystem::Directory::CreateDirectory(pathWord.GetPath());
	
	OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + _T("theme");
    FileSystem::Directory::CreateDirectory(pathTheme.GetPath()) ;
	Writers::DefaultThemeWriter themeWriter;

	themeWriter.Write(pathTheme.GetPath() + FILE_SEPARATOR_STR + _T("theme1.xml"));
	m_oDocRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme"), _T("theme/theme1.xml") );
	m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.theme+xml"), _T("/word/theme/theme1.xml") );
//-----------------------------------------------------------------------------------------------------
	((OOXDocumentWriter*)m_poDocumentWriter)->SaveByItemEnd();

	((OOXFootnoteWriter*)m_poFootnoteWriter)->Save(pathWord.GetPath());
	((OOXEndnoteWriter*)m_poEndnoteWriter)->Save(pathWord.GetPath());
	((OOXNumberingWriter*)m_poNumberingWriter)->Save(m_sTargetFolder);
	((OOXStylesWriter*)m_poStylesWriter)->Save(m_sTargetFolder);
	((OOXFontTableWriter*)m_poFontTableWriter)->Save(m_sTargetFolder);
	
	((OOXSettingsWriter*)m_poSettingsWriter)->Save(m_sTargetFolder); //setting в последнюю очередь

//-------------------------------------------------------------------------------------
	OOX::CPath pathDocProps = m_sTargetFolder + FILE_SEPARATOR_STR + _T("docProps");
    FileSystem::Directory::CreateDirectory(pathDocProps.GetPath());
	
	if (m_poDocPropsApp)
	{
		((OOX::CApp*)m_poDocPropsApp)->SetApplication(_T("OnlyOffice"));
		((OOX::CApp*)m_poDocPropsApp)->SetAppVersion(_T("3.0000"));
		
		((OOX::CApp*)m_poDocPropsApp)->write(pathDocProps + FILE_SEPARATOR_STR + _T("app.xml"), pathDocProps.GetDirectory(), oContentTypes);
		
		m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"), _T("docProps/app.xml") );
		m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-officedocument.extended-properties+xml"), _T("/docProps/app.xml") );
	}				
	if (m_poDocPropsCore)
	{
		((OOX::CCore*)m_poDocPropsCore)->write(pathDocProps + FILE_SEPARATOR_STR + _T("core.xml"), pathDocProps.GetDirectory(), oContentTypes);
		
		m_oRels.AddRelationship( _T("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"), _T("docProps/core.xml") );
		m_oContentTypes.AddContent( _T("application/vnd.openxmlformats-package.core-properties+xml"), _T("/docProps/core.xml") );
	} 
//-----------------------------------------------------------------------------------------------------

	bool nResult = true;
	nResult &= m_oContentTypes.Save(m_sTargetFolder);
	nResult &= m_oRels.Save(m_sTargetFolder);
	nResult &= m_oDocRels.Save(pathWord.GetPath());
	
	for( int i = 0; i < (int)m_oCustomRelsWriter.size(); i++ )
		m_oCustomRelsWriter[i]->Save(pathWord.GetPath());
	return nResult;
}
