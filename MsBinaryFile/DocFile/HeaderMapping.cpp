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

#include "HeaderMapping.h"
#include "TableMapping.h"

namespace DocFileFormat
{
	HeaderMapping::HeaderMapping (ConversionContext* context, CharacterRange hdr): DocumentMapping(context, this), _hdr(hdr)
	{
	
	}

	void HeaderMapping::Apply( IVisitable* visited )
	{
		m_document = static_cast<WordDocument*>( visited );

		//start the document
        m_pXmlWriter->WriteNodeBegin( L"w:hdr", TRUE );

		//write namespaces
        m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
        m_pXmlWriter->WriteAttribute( L"xmlns:v", OpenXmlNamespaces::VectorML );
        m_pXmlWriter->WriteAttribute( L"xmlns:o", OpenXmlNamespaces::Office );
        m_pXmlWriter->WriteAttribute( L"xmlns:w10", OpenXmlNamespaces::OfficeWord );
        m_pXmlWriter->WriteAttribute( L"xmlns:r", OpenXmlNamespaces::Relationships );
        m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

		//convert the header text
		_lastValidPapx	=	(*(m_document->AllPapxFkps->begin()))->grppapx[0];
		int cp			=	_hdr.GetCharacterPosition();
		int cpMax		=	_hdr.GetCharacterPosition() + this->_hdr.GetCharacterCount();

		//the CharacterCount of the headers also counts the guard paragraph mark.
		//this additional paragraph mark shall not be converted.
		cpMax--;

		while ( cp < cpMax  && cp < (int)m_document->Text->size())
		{
			int fc = m_document->FindFileCharPos(cp);
			if (fc < 0) break;

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx, m_document->nWordVersion );

			if ( tai.fInTable )
			{
				//this PAPX is for a table
				Table table( this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ) );
				table.Convert( this );
				cp = table.GetCPEnd();
			}
			else
			{
				//this PAPX is for a normal paragraph
				cp = writeParagraph( cp, 0x7fffffff );
			}
		}

        m_pXmlWriter->WriteNodeEnd( L"w:hdr");

		m_context->_docx->HeaderXMLList.push_back( std::wstring( m_pXmlWriter->GetXmlString() ) );
	}
}
