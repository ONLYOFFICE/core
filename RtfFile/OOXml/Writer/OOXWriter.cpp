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
#include "OOXWriter.h"

#include "../../Format/RtfDocument.h"

#include "OOXRelsWriter.h"
#include "OOXDocumentWriter.h"
#include "OOXNumberingWriter.h"
#include "OOXFontTableWriter.h"
#include "OOXStylesWriter.h"
#include "OOXSettingsWriter.h"
#include "OOXThemeWriter.h"
#include "OOXFootnoteWriter.h"
#include "OOXCommentsWriter.h"

#include "../../../OOXML/DocxFormat/Docx.h"
#include "../../../OOXML/DocxFormat/App.h"
#include "../../../OOXML/DocxFormat/Core.h"

#include "../../../DesktopEditor/common/SystemUtils.h"
#include "../../../OOXML/Binary/Document/BinReader/DefaultThemeWriter.h"

OOXWriter::OOXWriter( RtfDocument& oDocument, std::wstring sPath ) : 
		m_oDocument			( oDocument ),
		m_sTargetFolder		( sPath),
		m_oRels				( L"", oDocument ),
		m_nCurTrackChangesId( 0),
		m_nextParaId		( 0),
		m_nFormFieldId		( 0),
		m_oDocRels			( L"document.xml", oDocument )
{
	m_nCurFitWidth		= PROP_DEF;
	m_poFootnoteWriter	= NULL;
	m_poEndnoteWriter	= NULL;
	m_poCommentsWriter	= NULL;

	m_poDocumentWriter	= new OOXDocumentWriter	( *this, m_oDocument );
	m_poFootnoteWriter	= new OOXFootnoteWriter	( *this, m_oDocument );
	m_poEndnoteWriter	= new OOXEndnoteWriter	( *this, m_oDocument );
	m_poFontTableWriter = new OOXFontTableWriter( *this, m_oDocument );
	m_poNumberingWriter = new OOXNumberingWriter( *this, m_oDocument );
	m_poSettingsWriter	= new OOXSettingsWriter	( *this, m_oDocument );
	m_poStylesWriter	= new OOXStylesWriter	( *this, m_oDocument );
	m_poCommentsWriter	= new OOXCommentsWriter	( *this, m_oDocument );

	m_poDocPropsApp		= new OOX::CApp(NULL);
	m_poDocPropsCore	= new OOX::CCore(NULL);

//default properties

	if (m_poDocPropsApp)
	{		
		((OOX::CApp*)m_poDocPropsApp)->SetDefaults();
	}
	if (m_poDocPropsCore)
	{
		((OOX::CCore*)m_poDocPropsCore)->SetCreator(L"");
		((OOX::CCore*)m_poDocPropsCore)->SetLastModifiedBy(L"");
	}
}
OOXWriter::~OOXWriter()
{
	delete ((OOXCommentsWriter*)	m_poCommentsWriter);
	delete ((OOXDocumentWriter*)	m_poDocumentWriter);
	delete ((OOXFootnoteWriter*)	m_poFootnoteWriter);
	delete ((OOXEndnoteWriter*)		m_poEndnoteWriter);
	delete ((OOXFontTableWriter*)	m_poFontTableWriter);
	delete ((OOXNumberingWriter*)	m_poNumberingWriter);
	delete ((OOXSettingsWriter*)	m_poSettingsWriter);
	delete ((OOXStylesWriter*)		m_poStylesWriter);
	delete ((OOX::CApp*)			m_poDocPropsApp);
	delete ((OOX::CCore*)			m_poDocPropsCore);
}
bool OOXWriter::Save()
{
	int nItemsCount = ((OOXDocumentWriter*)m_poDocumentWriter)->GetCountSections();
	
	SaveByItemStart();
	
	for( int i = 0; i < nItemsCount; i++ )
	{
		SaveBySection();
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
bool OOXWriter::SaveBySection()
{
	return ((OOXDocumentWriter*)m_poDocumentWriter)->SaveBySection();
}
bool OOXWriter::SaveByItemEnd()
{
	OOX::CContentTypes oContentTypes;

	OOX::CPath pathWord = m_sTargetFolder + FILE_SEPARATOR_STR + L"word";
    NSDirectory::CreateDirectory(pathWord.GetPath());
	
	OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + L"theme";
    NSDirectory::CreateDirectory(pathTheme.GetPath()) ;
	Writers::DefaultThemeWriter themeWriter;

	if (false == m_oDocument.m_sThemeXml.empty())
	{
		themeWriter.m_sContent = m_oDocument.m_sThemeXml;
	}
	themeWriter.Write(pathTheme.GetPath() + FILE_SEPARATOR_STR + L"theme1.xml");
	m_oDocRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme", L"theme/theme1.xml" );
	m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.theme+xml", L"/word/theme/theme1.xml" );
//-----------------------------------------------------------------------------------------------------
	((OOXDocumentWriter*)	m_poDocumentWriter)->SaveByItemEnd();

	((OOXFootnoteWriter*)	m_poFootnoteWriter)->Save	(pathWord.GetPath());
	((OOXEndnoteWriter*)	m_poEndnoteWriter)->Save	(pathWord.GetPath());
	((OOXCommentsWriter*)	m_poCommentsWriter)->Save	(pathWord.GetPath());
	((OOXNumberingWriter*)	m_poNumberingWriter)->Save	(m_sTargetFolder);
	((OOXStylesWriter*)		m_poStylesWriter)->Save		(m_sTargetFolder);
	((OOXFontTableWriter*)	m_poFontTableWriter)->Save	(m_sTargetFolder);

	((OOXSettingsWriter*)	m_poSettingsWriter)->Save	(m_sTargetFolder); //setting в последнюю очередь

//-------------------------------------------------------------------------------------
	OOX::CPath pathDocProps = m_sTargetFolder + FILE_SEPARATOR_STR + L"docProps";
    NSDirectory::CreateDirectory(pathDocProps.GetPath());
	
	if (m_poDocPropsApp)
	{
		std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
		if (sApplication.empty())
			sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
		std::string s = VALUE2STR(INTVER);
		sApplication += L"/" + std::wstring(s.begin(), s.end());
#endif		
		((OOX::CApp*)m_poDocPropsApp)->m_sApplication = sApplication;
		((OOX::CApp*)m_poDocPropsApp)->write(pathDocProps + FILE_SEPARATOR_STR + L"app.xml", pathDocProps.GetDirectory(), oContentTypes);
		
		m_oRels.AddRelationship( L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties", L"docProps/app.xml" );
		m_oContentTypes.AddContent( L"application/vnd.openxmlformats-officedocument.extended-properties+xml", L"/docProps/app.xml" );
	}				
	if (m_poDocPropsCore)
	{
		((OOX::CCore*)m_poDocPropsCore)->write(pathDocProps + FILE_SEPARATOR_STR + L"core.xml", pathDocProps.GetDirectory(), oContentTypes);
		
		m_oRels.AddRelationship( L"http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties", L"docProps/core.xml" );
		m_oContentTypes.AddContent( L"application/vnd.openxmlformats-package.core-properties+xml", L"/docProps/core.xml" );
	} 
//-----------------------------------------------------------------------------------------------------

	bool nResult = true;
	nResult &= m_oContentTypes.Save(m_sTargetFolder);
	nResult &= m_oRels.Save(m_sTargetFolder);
	nResult &= m_oDocRels.Save(pathWord.GetPath());
	
	for (size_t i = 0; i < m_oCustomRelsWriter.size(); i++ )
		m_oCustomRelsWriter[i]->Save(pathWord.GetPath());
	return nResult;
}
