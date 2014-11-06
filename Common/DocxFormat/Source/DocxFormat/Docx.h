#pragma once
#ifndef OOX_DOCX_INCLUDE_H_
#define OOX_DOCX_INCLUDE_H_

#include "../Base/SmartPtr.h"

#include "Rels.h"
#include "IFileContainer.h"
#include "FileTypes.h"
#include "App.h"
#include "Core.h"
#include "Document.h"
#include "FontTable.h"
#include "Numbering.h"
#include "Comments.h"
#include "Styles.h"
#include "Footnote.h"
#include "Endnote.h"
#include "Settings/WebSettings.h"
#include "Settings/Settings.h"
#include "External/HyperLink.h"
#include "Media/Image.h"
#include "Media/OleObject.h"
#include "HeaderFooter.h"
#include "Theme/Theme.h"


#ifndef _WIN32
#include <sys/stat.h>
#endif

namespace OOX
{
	class CDocx : public OOX::IFileContainer
	{
	public:

		CDocx()
		{
			m_pDocument  = NULL;
			m_pFontTable = NULL;
			m_pNumbering = NULL;
			m_pStyles    = NULL;
			m_pApp       = NULL;
			m_pCore      = NULL;
			m_pEndnotes  = NULL;
			m_pFootnotes = NULL;
			m_pSettings  = NULL;
			m_pTheme     = NULL;
			m_pComments  = NULL;
			m_pCommentsExt = NULL;
			m_pPeople	= NULL;
		}
		CDocx(const CPath& oFilePath)
		{
			m_pDocument  = NULL;
			m_pFontTable = NULL;
			m_pNumbering = NULL;
			m_pStyles    = NULL;
			m_pApp       = NULL;
			m_pCore      = NULL;
			m_pEndnotes  = NULL;
			m_pFootnotes = NULL;
			m_pSettings  = NULL;
			m_pTheme     = NULL;
			m_pComments  = NULL;
			m_pCommentsExt = NULL;
			m_pPeople	= NULL;

			Read( oFilePath );
		}

	public:

		BOOL Read(const CPath& oFilePath);
		BOOL Write(const CPath& oFilePath)
		{

			// TO DO: Запись надо править. Она НЕ РАБОТАЕТ!!!! Проблемы в IFileContainer.

            //return FALSE;

			// Создаем папку
#ifdef _WIN32
			CreateDirectoryW( oFilePath.GetPath(), NULL );
#else
            std::string sFileParthUtf8 = stringWstingToUtf8String (oFilePath.GetPath());
            mkdir (sFileParthUtf8.c_str(),  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

			OOX::CRels         oRels;
			OOX::CContentTypes oContent;

			IFileContainer::Write( oRels, oFilePath, OOX::CPath( L"" ), oContent );
			oRels.Write( oFilePath / L"/" );
			oContent.Write( oFilePath );

			return TRUE;
		}



	public:

		OOX::CDocument  *GetDocument () const
		{
			return m_pDocument;
		}
		OOX::CFontTable *GetFontTable() const
		{
			return m_pFontTable;
		}
		OOX::CNumbering *GetNumbering() const
		{
			return m_pNumbering;
		}
		OOX::CStyles    *GetStyles   () const
		{
			return m_pStyles;
		}
		OOX::CEndnotes  *GetEndnotes () const
		{
			return m_pEndnotes;
		}
		OOX::CFootnotes *GetFootnotes() const
		{
			return m_pFootnotes;
		}
		OOX::CApp       *GetApp      () const
		{
			return m_pApp;
		}
		OOX::CCore      *GetCore     () const
		{
			return m_pCore;
		}
		OOX::CSettings  *GetSettings () const
		{
			return m_pSettings;
		}
		OOX::CComments  *GetComments () const
		{
			return m_pComments;
		}
		OOX::CCommentsExt  *GetCommentsExt () const
		{
			return m_pCommentsExt;
		}
		OOX::CPeople  *GetPeople () const
		{
			return m_pPeople;
		}

		OOX::CTheme		*GetTheme () const
		{
			return m_pTheme;
		}

		OOX::CHdrFtr    *GetHeaderOrFooter(const OOX::RId& rId) const
		{
			if ( m_pDocument )
			{
				OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_pDocument;

				smart_ptr<OOX::File> pFile = pDocumentContainer->Find( rId );
				if ( pFile.IsInit() && ( OOX::FileTypes::Header == pFile->type() || OOX::FileTypes::Footer == pFile->type() ) )
					return (OOX::CHdrFtr*)pFile.operator->();
				else 
					return NULL;
			}
			
			return NULL;
		}

	public:

		OOX::CApp       *m_pApp;
		OOX::CCore      *m_pCore;

		OOX::CDocument  *m_pDocument;   // Основной документ word/document.xml
		OOX::CFontTable *m_pFontTable;  // Таблица шрифтов   word/fonttable.xml
		OOX::CNumbering *m_pNumbering;  // Нумерации         word/numbering.xml
		OOX::CStyles    *m_pStyles;     // Стили             word/styles.xml
		OOX::CFootnotes *m_pFootnotes;  // Сноски            word/footnotes.xml
		OOX::CEndnotes  *m_pEndnotes;   // Сноски            word/endnotes.xml
		OOX::CSettings  *m_pSettings;   // Настройки         word/settings.xml
		OOX::CComments  *m_pComments;   //			         word/comments.xml
		OOX::CCommentsExt *m_pCommentsExt;//		         word/commentsExtended.xml
		OOX::CPeople	*m_pPeople;		//				     word/people.xml
		
		OOX::CTheme		*m_pTheme;

	};
} // OOX

#endif // OOX_DOCX_INCLUDE_H_
