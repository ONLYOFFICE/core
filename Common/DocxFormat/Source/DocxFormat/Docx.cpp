/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
#include "Docx.h"

namespace OOX {
	
        bool CDocx::Read(const CPath& oFilePath)
        {
            // Ищем "/_rels/.rels" и читаем все файлы по рельсам
            OOX::CRels oRels( oFilePath / FILE_SEPARATOR_STR );
            IFileContainer::Read( oRels, oFilePath, oFilePath );

            // Выполняем дополнительные действия для более удобной работы с файлом

            // Ищем основной документ
            smart_ptr<OOX::File> pFile = Find(OOX::FileTypes::Document);
			if (pFile.IsInit() == false)
			{
				pFile = Find(OOX::FileTypes::DocumentMacro);
			}
            if (pFile.IsInit())
                m_pDocument = dynamic_cast<OOX::CDocument*>(pFile.operator->());

            if ( m_pDocument )
            {
                // Ищем таблицу шрифтов
                pFile = m_pDocument->Find( OOX::FileTypes::FontTable );
                if ( pFile.IsInit() )
                    m_pFontTable = dynamic_cast<OOX::CFontTable*>(pFile.operator->());

                // Ищем таблицу нумераций
                pFile = m_pDocument->Find( OOX::FileTypes::Numbering );
                if ( pFile.IsInit())
                    m_pNumbering = dynamic_cast<OOX::CNumbering*>(pFile.operator->());

                // Ищем таблицу стилей
                pFile = m_pDocument->Find( OOX::FileTypes::Style );
                if ( pFile.IsInit())
                    m_pStyles = dynamic_cast<OOX::CStyles*>(pFile.operator->());

                // Ищем сноски для страниц
                pFile = m_pDocument->Find( OOX::FileTypes::FootNote );
                if ( pFile.IsInit())
                    m_pFootnotes = dynamic_cast<OOX::CFootnotes*>(pFile.operator->());

                pFile = m_pDocument->Find( OOX::FileTypes::EndNote );
                if ( pFile.IsInit())
                    m_pEndnotes = dynamic_cast<OOX::CEndnotes*>(pFile.operator->());

                // Ищем файл с наcтройками
                pFile = m_pDocument->Find( OOX::FileTypes::Setting );
                if ( pFile.IsInit() )
                    m_pSettings = dynamic_cast<OOX::CSettings*>(pFile.operator->());

                // Ищем файл с комментариями
                pFile = m_pDocument->Find( OOX::FileTypes::Comments );
                if ( pFile.IsInit())
                    m_pComments = dynamic_cast<OOX::CComments*>(pFile.operator->());

                // Ищем файл с комментариями Ext
                pFile = m_pDocument->Find( OOX::FileTypes::CommentsExt );
                if ( pFile.IsInit())
                    m_pCommentsExt = dynamic_cast<OOX::CCommentsExt*>(pFile.operator->());

                // Ищем файл с авторами
                pFile = m_pDocument->Find( OOX::FileTypes::People );
                if ( pFile.IsInit())
                    m_pPeople = dynamic_cast<OOX::CPeople*>(pFile.operator->());

                //OOX::CRels rels(oFilePath / m_pDocument->DefaultDirectory() / m_pDocument->DefaultFileName());
                //IFileContainer::Read(rels, oFilePath);

                // Ищем файл с темами
                pFile = m_pDocument->Find(OOX::FileTypes::Theme);
                if (pFile.IsInit())
					m_pTheme = dynamic_cast<PPTX::Theme*>(pFile.operator->());

                pFile = m_pDocument->Find(OOX::FileTypes::VbaProject);
                if (pFile.IsInit())
					m_pVbaProject = dynamic_cast<OOX::VbaProject*>(pFile.operator->());
			}

            // Ищем настройки
            pFile = Find( OOX::FileTypes::App );
            if ( pFile.IsInit())
                m_pApp = dynamic_cast<OOX::CApp*>(pFile.operator->());

            pFile = Find( OOX::FileTypes::Core );
            if ( pFile.IsInit())
                m_pCore = dynamic_cast<OOX::CCore*>(pFile.operator->());

            return true;
	}
	
}
