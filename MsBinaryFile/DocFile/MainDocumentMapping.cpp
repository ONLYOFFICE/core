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

#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	MainDocumentMapping::MainDocumentMapping (ConversionContext* ctx) : DocumentMapping( ctx, this )
	{
	}

	MainDocumentMapping::~MainDocumentMapping()
	{

	}
}

namespace DocFileFormat
{
	void MainDocumentMapping::Apply(IVisitable* visited)
	{
		m_document = static_cast<WordDocument*>(visited);
		
		// Header
		m_pXmlWriter->WriteNodeBegin(L"w:document", TRUE );

		// Namespaces
		m_pXmlWriter->WriteAttribute(L"xmlns:w",		OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteAttribute(L"xmlns:v",		OpenXmlNamespaces::VectorML );
		m_pXmlWriter->WriteAttribute(L"xmlns:o",		OpenXmlNamespaces::Office );
		m_pXmlWriter->WriteAttribute(L"xmlns:r",		OpenXmlNamespaces::Relationships );
		m_pXmlWriter->WriteAttribute(L"xmlns:w10",		OpenXmlNamespaces::OfficeWord );
		m_pXmlWriter->WriteAttribute(L"xmlns:m",		L"http://schemas.openxmlformats.org/officeDocument/2006/math");

		m_pXmlWriter->WriteAttribute(L"xmlns:wpc",		L"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas");
		m_pXmlWriter->WriteAttribute(L"xmlns:cx",		L"http://schemas.microsoft.com/office/drawing/2014/chartex");
		m_pXmlWriter->WriteAttribute(L"xmlns:cx1",		L"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex");
		m_pXmlWriter->WriteAttribute(L"xmlns:mc",		L"http://schemas.openxmlformats.org/markup-compatibility/2006"); 
		m_pXmlWriter->WriteAttribute(L"xmlns:wp14",		L"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing");
		m_pXmlWriter->WriteAttribute(L"xmlns:wp",		L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing");
		m_pXmlWriter->WriteAttribute(L"xmlns:w14",		L"http://schemas.microsoft.com/office/word/2010/wordml");
		m_pXmlWriter->WriteAttribute(L"xmlns:w15",		L"http://schemas.microsoft.com/office/word/2012/wordml");
		m_pXmlWriter->WriteAttribute(L"xmlns:w16se",	L"http://schemas.microsoft.com/office/word/2015/wordml/symex");
		m_pXmlWriter->WriteAttribute(L"xmlns:wpg",		L"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup");
		m_pXmlWriter->WriteAttribute(L"xmlns:wpi",		L"http://schemas.microsoft.com/office/word/2010/wordprocessingInk");
		m_pXmlWriter->WriteAttribute(L"xmlns:wne",		L"http://schemas.microsoft.com/office/word/2006/wordml");
		m_pXmlWriter->WriteAttribute(L"xmlns:wps",		L"http://schemas.microsoft.com/office/word/2010/wordprocessingShape");
		m_pXmlWriter->WriteAttribute(L"mc:Ignorable",	L"w14 w15 w16se wp14");
	

		  m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

		if ((m_document->GetOfficeArt()) && (m_document->GetOfficeArt()->GetShapeBackgound()))
		{
			bool bFilled = true;
			ShapeContainer* pShape = m_document->GetOfficeArt()->GetShapeBackgound();

			ODRAW::OfficeArtFOPTEPtr boolFill = pShape->ExtractOption(ODRAW::fillStyleBooleanProperties);

			ODRAW::FillStyleBooleanProperties* booleans = dynamic_cast<ODRAW::FillStyleBooleanProperties*>(boolFill.get());

			if (booleans && (booleans->fUsefFilled && !booleans->fFilled))
			{
				bFilled = false;
			}
			if (bFilled)
			{
				m_pXmlWriter->WriteNodeBegin ( L"w:background", TRUE);
					m_pXmlWriter->WriteAttribute ( L"w:color", L"FFFFFF");
				m_pXmlWriter->WriteNodeEnd( L"",  TRUE, FALSE );
				
				VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, NULL, NULL,  _caller);
					pShape->Convert(&oVmlWriter);
				m_pXmlWriter->WriteNodeEnd (L"w:background");
			}
		}

		m_pXmlWriter->WriteNodeBegin( L"w:body", FALSE );

// Convert the document
		_lastValidPapx = NULL;
		if (m_document->AllPapxFkps && m_document->AllPapxFkps->empty() == false)
		{
			for (auto papx : *m_document->AllPapxFkps)
			{
				if ((papx) && (papx->grppapx))
				{
					_lastValidPapx = *papx->grppapx;
					break;
				}
			}
		}

		int cp = 0;
		int fc = 0;
		ParagraphPropertyExceptions* papx = NULL;

		unsigned int index = 0;
		const unsigned int PROGRESS_COUNT = 10;
		const unsigned int progressStep = ( m_document->FIB->m_RgLw97.ccpText / PROGRESS_COUNT );

		int countText		=	m_document->FIB->m_RgLw97.ccpText;
		int countTextRel	=	m_document->FIB->m_RgLw97.ccpText - 1;
							
		while (cp < countText && cp >= 0)
		{
			fc = m_document->FindFileCharPos(cp);
				
			if (fc < 0) break;

			papx = findValidPapx(fc);

			if (papx)
			{
				TableInfo tai(papx, m_document->nWordVersion);
				if (tai.fInTable)
				{
					int cpStart = cp;
					//this PAPX is for a table
					Table table( this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ) );
					table.Convert(this);
					cp = table.GetCPEnd();
					while (cp <= cpStart)	//conv_eznHsm8em2unSv6_2QE__docx.doc
						cp++;
				}
				else
				{
					//this PAPX is for a normal paragraph
					cp = writeParagraph(cp, countTextRel);
				}
			}
			else
			{
				//There are no paragraphs, only text
				int fc =	m_document->FindFileCharPos(cp);
				int fcEnd =	m_document->FindFileCharPos(countTextRel);

				//if (fc < 0 || fcEnd < 0) break;
				//
				//// Read the chars
				//std::vector<wchar_t>* chars =	m_document->GetChars (fc, fcEnd, fc);

				writeParagraph(cp, countTextRel, false, true, START_END_PARAGRAPH);

				cp = m_document->FIB->m_RgLw97.ccpText;
			}
		}

		//write the section properties of the body with the last SEPX
		if (m_document->AllSepx)
		{
			int lastSepxCp = 0;

			for (std::map<int, SectionPropertyExceptions*>::iterator iter = m_document->AllSepx->begin(); iter != m_document->AllSepx->end(); ++iter) 
				lastSepxCp = iter->first;

			SectionPropertyExceptions* lastSepx	=	m_document->AllSepx->operator []( lastSepxCp );

			SectionPropertiesMapping* pSection	=	new SectionPropertiesMapping (m_pXmlWriter, m_context, _sectionNr);
			if (pSection)
			{
				lastSepx->Convert( pSection );
				RELEASEOBJECT(pSection);
			}
		}

		m_pXmlWriter->WriteNodeEnd( L"w:body" );
		m_pXmlWriter->WriteNodeEnd( L"w:document" );

		m_context->_docx->DocumentXML = std::wstring(m_pXmlWriter->GetXmlString());
	}
}