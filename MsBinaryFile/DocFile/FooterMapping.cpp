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


#include "FooterMapping.h"
#include "TableMapping.h"

namespace DocFileFormat
{
	FooterMapping::FooterMapping (ConversionContext* ctx, CharacterRange ftr) : DocumentMapping(ctx, this), _ftr(ftr)
	{
	}

	void FooterMapping::Apply( IVisitable* visited )
	{
		m_document = static_cast<WordDocument*>( visited );

		//start the document
        m_pXmlWriter->WriteNodeBegin( L"w:ftr", TRUE );

		//write namespaces
        m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
        m_pXmlWriter->WriteAttribute( L"xmlns:v", OpenXmlNamespaces::VectorML );
        m_pXmlWriter->WriteAttribute( L"xmlns:o", OpenXmlNamespaces::Office );
        m_pXmlWriter->WriteAttribute( L"xmlns:w10", OpenXmlNamespaces::OfficeWord );
        m_pXmlWriter->WriteAttribute( L"xmlns:r", OpenXmlNamespaces::Relationships );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

		//convert the footer text
		_lastValidPapx = (*(m_document->AllPapxFkps->begin()))->grppapx[0];
		int cp		=	_ftr.GetCharacterPosition();
		int cpMax	=	_ftr.GetCharacterPosition() + _ftr.GetCharacterCount();

		//the CharacterCount of the footers also counts the guard paragraph mark.
		//this additional paragraph mark shall not be converted.
		cpMax--;

		while ( cp < cpMax && cp < (int)m_document->Text->size())
		{
			int fc =  m_document->FindFileCharPos(cp);
			if (fc < 0) break;

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx, m_document->nWordVersion );

			if ( tai.fInTable )
			{
				//this PAPX is for a table
				int start_table_cp = cp;
				
				Table table( this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ) );
				table.Convert( this );
				cp = table.GetCPEnd();

				if (cp == start_table_cp)
					cp++;
			}
			else
			{
				//this PAPX is for a normal paragraph
				cp = writeParagraph( cp, 0x7fffffff );
			}
		}

        m_pXmlWriter->WriteNodeEnd( L"w:ftr" );

		m_context->_docx->FooterXMLList.push_back( std::wstring( m_pXmlWriter->GetXmlString() ) );
	}
}
