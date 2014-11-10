#include "Docx.h"

namespace OOX {
	
	BOOL CDocx::Read(const CPath& oFilePath) {
		// »щем "/_rels/.rels" и читаем все файлы по рельсам
		OOX::CRels oRels( oFilePath / L"/" );
		IFileContainer::Read( oRels, oFilePath, oFilePath );

		// ¬ыполн€ем дополнительные действи€ дл€ более удобной работы с файлом

		// »щем основной документ
		smart_ptr<OOX::File> pFile = Find(OOX::FileTypes::Document);
		if (pFile.IsInit() && OOX::FileTypes::Document == pFile->type())
			m_pDocument = (OOX::CDocument*)pFile.operator->();
		else 
			m_pDocument = NULL;

		if ( m_pDocument )
		{
			OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_pDocument;

			// »щем таблицу шрифтов
			pFile = pDocumentContainer->Find( OOX::FileTypes::FontTable );
			if ( pFile.IsInit() && OOX::FileTypes::FontTable == pFile->type() )
				m_pFontTable = (OOX::CFontTable*)pFile.operator->();
			else 
				m_pFontTable = NULL;

			// »щем таблицу нумераций
			pFile = pDocumentContainer->Find( OOX::FileTypes::Numbering );
			if ( pFile.IsInit() && OOX::FileTypes::Numbering == pFile->type() )
				m_pNumbering = (OOX::CNumbering*)pFile.operator->();
			else 
				m_pNumbering = NULL;

			// »щем таблицу стилей
			pFile = pDocumentContainer->Find( OOX::FileTypes::Style );
			if ( pFile.IsInit() && OOX::FileTypes::Style == pFile->type() )
				m_pStyles = (OOX::CStyles*)pFile.operator->();
			else 
				m_pStyles = NULL;

			// »щем сноски дл€ страниц
			pFile = pDocumentContainer->Find( OOX::FileTypes::FootNote );
			if ( pFile.IsInit() && OOX::FileTypes::FootNote == pFile->type() )
				m_pFootnotes = (OOX::CFootnotes*)pFile.operator->();
			else
				m_pFootnotes = NULL;

			pFile = pDocumentContainer->Find( OOX::FileTypes::EndNote );
			if ( pFile.IsInit() && OOX::FileTypes::EndNote == pFile->type() )
				m_pEndnotes = (OOX::CEndnotes*)pFile.operator->();
			else
				m_pEndnotes = NULL;

			// »щем файл с наcтройками
			pFile = pDocumentContainer->Find( OOX::FileTypes::Setting );
			if ( pFile.IsInit() && OOX::FileTypes::Setting == pFile->type() )
				m_pSettings = (OOX::CSettings*)pFile.operator->();
			else 
				m_pSettings = NULL;

			// »щем файл с комментари€ми
			pFile = pDocumentContainer->Find( OOX::FileTypes::Comments );
			if ( pFile.IsInit() && OOX::FileTypes::Comments == pFile->type() )
				m_pComments = (OOX::CComments*)pFile.operator->();
			else 
				m_pComments = NULL;

			// »щем файл с комментари€ми Ext
			pFile = pDocumentContainer->Find( OOX::FileTypes::CommentsExt );
			if ( pFile.IsInit() && OOX::FileTypes::CommentsExt == pFile->type() )
				m_pCommentsExt = (OOX::CCommentsExt*)pFile.operator->();
			else 
				m_pCommentsExt = NULL;

			// »щем файл с авторами
			pFile = pDocumentContainer->Find( OOX::FileTypes::People );
			if ( pFile.IsInit() && OOX::FileTypes::People == pFile->type() )
				m_pPeople = (OOX::CPeople*)pFile.operator->();
			else 
				m_pPeople = NULL;

			//OOX::CRels rels(oFilePath / m_pDocument->DefaultDirectory() / m_pDocument->DefaultFileName());
			//IFileContainer::Read(rels, oFilePath);

			// »щем файл с темами
			pFile = pDocumentContainer->Find(OOX::FileTypes::Theme);
			if (pFile.IsInit() && OOX::FileTypes::Theme == pFile->type())
				m_pTheme = (OOX::CTheme*)pFile.operator->();
			else 
				m_pTheme = NULL;
		}

		// »щем настройки
		pFile = Find( OOX::FileTypes::App );
		if ( pFile.IsInit() && OOX::FileTypes::App == pFile->type() )
			m_pApp = (OOX::CApp*)pFile.operator->();
		else 
			m_pApp = NULL;

		pFile = Find( OOX::FileTypes::Core );
		if ( pFile.IsInit() && OOX::FileTypes::Core == pFile->type() )
			m_pCore = (OOX::CCore*)pFile.operator->();
		else 
			m_pCore = NULL;

		return TRUE;
	}
	
}