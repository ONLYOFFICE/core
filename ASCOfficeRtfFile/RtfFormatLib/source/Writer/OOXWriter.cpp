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

OOXWriter::OOXWriter( RtfDocument& oDocument, std::wstring sPath ) : 
		m_oDocument(oDocument),
		m_sTargetFolder(sPath.c_str()),
		m_oRels( _T(""), oDocument ),
		m_oDocRels( _T("document.xml"), oDocument )
{
	nCurFitWidth		= PROP_DEF;
	m_poFootnoteWriter	= NULL;
	m_poEndnoteWriter	= NULL;

	m_poDocumentWriter	= new OOXDocumentWriter	( *this, m_oDocument );
	m_poFootnoteWriter	= new OOXFootnoteWriter	( *this, m_oDocument );
	m_poEndnoteWriter	= new OOXEndnoteWriter	( *this, m_oDocument );
	m_poFontTableWriter = new OOXFontTableWriter( *this, m_oDocument );
	m_poNumberingWriter = new OOXNumberingWriter( *this, m_oDocument );
	m_poSettingsWriter	= new OOXSettingsWriter	( *this, m_oDocument );
	m_poStylesWriter	= new OOXStylesWriter	( *this, m_oDocument );

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
	((OOXDocumentWriter*)	m_poDocumentWriter)->SaveByItemEnd();

	((OOXFootnoteWriter*)	m_poFootnoteWriter)->Save(pathWord.GetPath());
	((OOXEndnoteWriter*)	m_poEndnoteWriter)->Save(pathWord.GetPath());
	((OOXNumberingWriter*)	m_poNumberingWriter)->Save(m_sTargetFolder);
	((OOXStylesWriter*)		m_poStylesWriter)->Save(m_sTargetFolder);
	((OOXFontTableWriter*)	m_poFontTableWriter)->Save(m_sTargetFolder);
	
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
