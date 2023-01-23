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
#include "Docx.h"

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
#include "Media/ActiveX.h"
#include "Media/VbaProject.h"
#include "Media/JsaProject.h"
#include "HeaderFooter.h"
#include "CustomXml.h"

#include "../PPTXFormat/Theme.h"
#include "../../DesktopEditor/common/Directory.h"

namespace OOX
{
	void CDocx::_part_summary::init()
	{
		document = NULL;
		fontTable = NULL;
		numbering = NULL;
		styles = NULL;
		footnotes = NULL;
		endnotes = NULL;
		settings = NULL;
		comments = NULL;
	}

	CDocx::CDocx() : OOX::IFileContainer(dynamic_cast<OOX::Document*>(this))
	{
		init();
	}
	CDocx::CDocx(const CPath& oFilePath) : OOX::IFileContainer(this)
	{
		init();
		Read( oFilePath );
	}
	bool CDocx::Write(const CPath& oFilePath)
	{
		// Создаем папку
        std::wstring filePath = oFilePath.GetPath();
        NSDirectory::CreateDirectory(filePath);

		OOX::CRels         oRels;
		OOX::CContentTypes oContent;

		IFileContainer::Write( oRels, oFilePath, OOX::CPath( L"" ), oContent );
		oRels.Write( oFilePath / FILE_SEPARATOR_STR );
		oContent.Write( oFilePath );

		return true;
	}
    bool CDocx::Read(const CPath& oFilePath)
    {
		m_sDocumentPath = oFilePath.GetPath();

        // Ищем "/_rels/.rels" и читаем все файлы по рельсам
        OOX::CRels oRels( oFilePath / FILE_SEPARATOR_STR );
        IFileContainer::Read( oRels, oFilePath, oFilePath );

		FixAfterRead();
         return true;
	}
	void CDocx::FixAfterRead()
	{
		//solve id conflict between comments and documentComments
		if ((NULL != m_oMain.comments && NULL != m_pDocumentComments) && (false == m_oMain.comments->m_arrComments.empty() && false != m_pDocumentComments->m_arrComments.empty()))
		{
			int maxId = INT_MIN;
			for (size_t i = 0; i < m_oMain.comments->m_arrComments.size(); ++i)
			{
				OOX::CComment* pComment = m_oMain.comments->m_arrComments[i];
				if (pComment->m_oId.IsInit() && maxId < pComment->m_oId->GetValue())
				{
					maxId = pComment->m_oId->GetValue();
				}
			}
			m_pDocumentComments->m_mapComments.clear();
			for (size_t i = 0; i < m_pDocumentComments->m_arrComments.size(); ++i)
			{
				OOX::CComment* pComment = m_pDocumentComments->m_arrComments[i];
				pComment->m_oId.Init();
				pComment->m_oId->SetValue(++maxId);
				m_pDocumentComments->m_mapComments.insert( std::make_pair( pComment->m_oId->GetValue(), i));
			}
		}
	}
	OOX::CHdrFtr *CDocx::GetHeaderOrFooter(const OOX::RId& rId, bool glossary) const
	{
		if (glossary && m_oGlossary.document)
		{
			OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_oGlossary.document;

			smart_ptr<OOX::File> pFile = pDocumentContainer->Find(rId);
			if (pFile.IsInit() && (OOX::FileTypes::Header == pFile->type() || OOX::FileTypes::Footer == pFile->type()))
				return (OOX::CHdrFtr*)pFile.GetPointer();
			else
				return NULL;
		}
		else if (!glossary && m_oMain.document)
		{
			OOX::IFileContainer* pDocumentContainer = (OOX::IFileContainer*)m_oMain.document;

            smart_ptr<OOX::File> pFile = pDocumentContainer->Find( rId );
			if ( pFile.IsInit() && ( OOX::FileTypes::Header == pFile->type() || OOX::FileTypes::Footer == pFile->type() ) )
				return (OOX::CHdrFtr*)pFile.GetPointer();
			else 
				return NULL;
		}
		
		return NULL;
	}
	const std::wstring CDocx::GetCustomSettings() const
	{
		if (m_oMain.document)
		{
			std::vector<smart_ptr<OOX::File>>& container = m_oMain.document->GetContainer();
			for (size_t i = 0; i < container.size(); ++i)
			{
				if (OOX::FileTypes::CustomXml == container[i]->type())
				{
					OOX::CCustomXML* pCustomXml = dynamic_cast<OOX::CCustomXML*>(container[i].GetPointer());
					if(OOX::CSettingsCustom::GetSchemaUrl() == pCustomXml->GetSchemaUrl())
					{
						pCustomXml->m_bUsed = true;
						return NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pCustomXml->m_sXmlA.c_str(), pCustomXml->m_sXmlA.length());
					}
				}
			}
		}
		return L"";
	}
	void CDocx::init()
	{
		m_oMain.init();
		m_oGlossary.init();

		m_pApp       = NULL;
		m_pCore      = NULL;
		m_pTheme     = NULL;

		m_pCommentsExt	= NULL;
		m_pCommentsExtensible	= NULL;
		m_pCommentsIds	= NULL;
		m_pPeople		= NULL;
		m_pDocumentComments  = NULL;
		m_pDocumentCommentsExt	= NULL;
		m_pDocumentCommentsExtensible	= NULL;
		m_pDocumentPeople		= NULL;
		m_pDocumentCommentsIds	= NULL;
		m_pCommentsUserData = NULL;

		m_pVbaProject	= NULL;
		m_pJsaProject	= NULL;

		m_bGlossaryRead = false;
	}
}
