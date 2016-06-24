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
#pragma once

#include "ContentTypes.h"
#include "WordDocument.h"
#include "DocumentMapping.h"

namespace DocFileFormat
{
	class EndnotesMapping: public DocumentMapping
	{
	public: 
		EndnotesMapping (ConversionContext* ctx) : DocumentMapping(ctx, this)
		{
		}

		virtual void Apply( IVisitable* visited )
		{
			m_document = static_cast<WordDocument*>( visited );

			if ( ( m_document != NULL ) && ( m_document->FIB->m_RgLw97.ccpEdn > 0 ) )
			{
				m_context->_docx->RegisterEndnotes();

				int id = 0;

				m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
				m_pXmlWriter->WriteNodeBegin( _T( "w:endnotes" ), TRUE );

				//write namespaces
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:v" ), OpenXmlNamespaces::VectorML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:o" ), OpenXmlNamespaces::Office );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w10" ), OpenXmlNamespaces::OfficeWord );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:r" ), OpenXmlNamespaces::Relationships );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				int cp = ( m_document->FIB->m_RgLw97.ccpText + m_document->FIB->m_RgLw97.ccpFtn + m_document->FIB->m_RgLw97.ccpHdr + m_document->FIB->m_RgLw97.ccpAtn );

				while ( cp <= ( m_document->FIB->m_RgLw97.ccpText + m_document->FIB->m_RgLw97.ccpFtn + m_document->FIB->m_RgLw97.ccpHdr + m_document->FIB->m_RgLw97.ccpAtn + m_document->FIB->m_RgLw97.ccpEdn - 2 ) )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:endnote" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString( id ).c_str() );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					while ( ( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpHdr - m_document->FIB->m_RgLw97.ccpAtn ) < (*m_document->IndividualEndnotesPlex)[id + 1] )
					{
						int fc =  m_document->FindFileCharPos(cp); 
						if (fc < 0) break;

						ParagraphPropertyExceptions* papx = findValidPapx( fc );
						TableInfo tai( papx );

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
							cp = writeParagraph( cp );
						}
					}

					m_pXmlWriter->WriteNodeEnd( _T( "w:endnote" ) );
					id++;
				}

				m_pXmlWriter->WriteNodeEnd( _T( "w:endnotes" ) );

				m_context->_docx->EndnotesXML = wstring( m_pXmlWriter->GetXmlString() );
			}
		}
	};
}