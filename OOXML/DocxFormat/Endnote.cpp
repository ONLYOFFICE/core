/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "Docx.h"
#include "Endnote.h"
#include "../SystemUtility/File.h"

namespace OOX
{
	//--------------------------------------------------------------------------------
	// CEndnotes 17.11.15
	//--------------------------------------------------------------------------------

	CEndnotes::CEndnotes(OOX::Document *pMain) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.endnotes = this;
			else						docx->m_oMain.endnotes = this;
		}
	}
	CEndnotes::CEndnotes(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath) : OOX::File(pMain), OOX::IFileContainer(pMain)
	{
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);

		if (docx)
		{
			if (docx->m_bGlossaryRead)	docx->m_oGlossary.endnotes = this;
			else						docx->m_oMain.endnotes = this;
		}
		read( oRootPath, oPath );
	}
	CEndnotes::~CEndnotes()
	{
		for (unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
		{
			if ( m_arrEndnote[nIndex] )	delete m_arrEndnote[nIndex]; m_arrEndnote[nIndex] = NULL;
		}

		m_arrEndnote.clear();
		m_mapEndnote.clear();
	}
	void CEndnotes::read(const CPath& oPath)
	{
		//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
		CPath oRootPath;
		read(oRootPath, oPath);
	}
	void CEndnotes::read(const CPath& oRootPath, const CPath& oFilePath)
	{
		m_oReadPath = oFilePath;
		IFileContainer::Read( oRootPath, oFilePath );

		XmlUtils::CXmlLiteReader oReader;

		if ( !oReader.FromFile( oFilePath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( _T("w:endnotes") == sName && !oReader.IsEmptyNode() )
		{
			int nNumberingDepth = oReader.GetDepth();
			while ( oReader.ReadNextSiblingNode( nNumberingDepth ) )
			{
				sName = oReader.GetName();
				if ( _T("w:endnote") == sName )
				{
					CFtnEdn *pEndnote = new CFtnEdn();
					*pEndnote = oReader;

					if (pEndnote)
					{
						m_arrEndnote.push_back( pEndnote );

						if (pEndnote->m_oId.IsInit())
						{
							m_mapEndnote.insert(std::make_pair(pEndnote->m_oId->GetValue(), pEndnote));
						}
					}

				}
			}
		}
	}
	void CEndnotes::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml;
		sXml = _T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\
<w:endnotes \
xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
mc:Ignorable=\"w14 w15 wp14\">");

		for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
		{
			if ( m_arrEndnote[nIndex] )
			{
				sXml += m_arrEndnote[nIndex]->toXML();
			}
		}
		sXml += _T("</w:endnotes>");
		CDirectory::SaveToFile( oPath.GetPath(), sXml );

		oContent.Registration( type().OverrideType(), oDirectory, oPath.GetFilename() );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}
	const OOX::FileType CEndnotes::type() const
	{
		return FileTypes::EndNote;
	}
	const CPath CEndnotes::DefaultDirectory() const
	{
		return type().DefaultDirectory();
	}
	const CPath CEndnotes::DefaultFileName() const
	{
		return type().DefaultFileName();
	}

	OOX::CFtnEdn* CEndnotes::Find(const OOX::Logic::CEndnoteReference& oReference)
	{
		if ( !oReference.m_oId.IsInit() )
			return NULL;

		//for ( unsigned int nIndex = 0; nIndex < m_arrEndnote.size(); nIndex++ )
		//{
		//	if ( m_arrEndnote[nIndex]->m_oId.IsInit() && ( m_arrEndnote[nIndex]->m_oId == oReference.m_oId ) )
		//		return m_arrEndnote[nIndex];
		//}
		std::map<int, OOX::CFtnEdn*>::iterator pFind = m_mapEndnote.find(oReference.m_oId->GetValue());

		if (pFind != m_mapEndnote.end())	return pFind->second;
		else								return NULL;
	}
	void CEndnotes::Add(OOX::CFtnEdn* pEndnote)
	{
		if (!pEndnote) return;
		if (!pEndnote->m_oId.IsInit()) return;

		m_arrEndnote.push_back( pEndnote );

		m_mapEndnote.insert(std::make_pair(pEndnote->m_oId->GetValue(), pEndnote));
	}
	const unsigned int CEndnotes::GetCount() const
{
	return (unsigned int)m_arrEndnote.size();
}

} // namespace OOX

