﻿/*
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
#pragma once

#include "TableMapping.h"
#include "DocumentMapping.h"
#include "AnnotationReferenceDescriptor.h"

namespace DocFileFormat
{
	class CommentsMapping: public DocumentMapping
	{
	public:
		CommentsMapping (ConversionContext* ctx) : DocumentMapping( ctx, this )
		{
		}

		virtual void Apply( IVisitable* visited )
		{
			m_document = static_cast<WordDocument*>( visited );

			if ( ( m_document != NULL ) && ( m_document->FIB->m_RgLw97.ccpAtn > 0 ) )
			{
				m_context->_docx->RegisterComments();

                m_pXmlWriter->WriteNodeBegin( L"?xml version=\"1.0\" encoding=\"UTF-8\"?");
                m_pXmlWriter->WriteNodeBegin( L"w:comments", TRUE );

				//write namespaces
                m_pXmlWriter->WriteAttribute( L"xmlns:w", OpenXmlNamespaces::WordprocessingML );
                m_pXmlWriter->WriteAttribute( L"xmlns:v", OpenXmlNamespaces::VectorML );
                m_pXmlWriter->WriteAttribute( L"xmlns:o", OpenXmlNamespaces::Office );
                m_pXmlWriter->WriteAttribute( L"xmlns:w10", OpenXmlNamespaces::OfficeWord );
                m_pXmlWriter->WriteAttribute( L"xmlns:r", OpenXmlNamespaces::Relationships );
                m_pXmlWriter->WriteAttribute( L"xmlns:wpc", L"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas" ); 
                m_pXmlWriter->WriteAttribute( L"xmlns:cx", L"http://schemas.microsoft.com/office/drawing/2014/chartex"  );
                m_pXmlWriter->WriteAttribute( L"xmlns:cx1", L"http://schemas.microsoft.com/office/drawing/2015/9/8/chartex"  );
                m_pXmlWriter->WriteAttribute( L"xmlns:mc", L"http://schemas.openxmlformats.org/markup-compatibility/2006" );
                m_pXmlWriter->WriteAttribute( L"xmlns:m", L"http://schemas.openxmlformats.org/officeDocument/2006/math" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wp14", L"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wp", L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing" );
                m_pXmlWriter->WriteAttribute( L"xmlns:w14", L"http://schemas.microsoft.com/office/word/2010/wordml" );
                m_pXmlWriter->WriteAttribute( L"xmlns:w15", L"http://schemas.microsoft.com/office/word/2012/wordml" );
                m_pXmlWriter->WriteAttribute( L"xmlns:w16se", L"http://schemas.microsoft.com/office/word/2015/wordml/symex" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wpg", L"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wpi", L"http://schemas.microsoft.com/office/word/2010/wordprocessingInk" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wne", L"http://schemas.microsoft.com/office/word/2006/wordml" );
                m_pXmlWriter->WriteAttribute( L"xmlns:wps", L"http://schemas.microsoft.com/office/word/2010/wordprocessingShape" );
                m_pXmlWriter->WriteAttribute( L"mc:Ignorable", L"w14 w15 w16se wp14");

                m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

				int cp = m_document->FIB->m_RgLw97.ccpText + m_document->FIB->m_RgLw97.ccpFtn + m_document->FIB->m_RgLw97.ccpHdr;

				size_t count = m_document->AnnotationsReferencePlex->Elements.size();

				for (size_t index = 0; index < count; ++index)
				{   
					AnnotationReferenceDescriptor* atrdPre10 = static_cast<AnnotationReferenceDescriptor*>(m_document->AnnotationsReferencePlex->Elements[index]);
					AnnotationReferenceExDescriptor* atrdPost10 = NULL;
					if ((m_document->AnnotationsReferenceExPlex) && (index < m_document->AnnotationsReferenceExPlex->Elements.size()))
					{
						atrdPost10 = static_cast<AnnotationReferenceExDescriptor*>(m_document->AnnotationsReferenceExPlex->Elements[index]);
					}

                    m_pXmlWriter->WriteNodeBegin( L"w:comment", TRUE );

					m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::SizeTToWideString(atrdPre10->m_CommentId));
					//if (atrdPre10->m_BookmarkId < 0)//-1 - easy ref (not start/end comment ref)
					//{
     //                   m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::SizeTToWideString( index + 1 + count + 1024 ));
					//}
					//else
					//{
                    //    m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::SizeTToWideString( index + 1 ));
					//}

					if (atrdPost10)
					{
						m_pXmlWriter->WriteAttribute( L"w:date", atrdPost10->m_nDTTM.getString());
					}
					if (atrdPre10->m_AuthorIndex < m_document->AnnotationOwners->size())	//conv_253l2H1CehgKwsxCtNk__docx.doc
					{
						m_pXmlWriter->WriteAttribute( L"w:author",
							FormatUtils::XmlEncode(m_document->AnnotationOwners->at( atrdPre10->m_AuthorIndex ) ));
					}
                    m_pXmlWriter->WriteAttribute( L"w:initials", FormatUtils::XmlEncode(atrdPre10->m_UserInitials));



                    m_pXmlWriter->WriteNodeEnd( L"", TRUE, FALSE );

					while ( ( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpHdr ) < (*m_document->IndividualCommentsPlex)[index + 1] )
					{
						int fc = m_document->FindFileCharPos(cp);
						if (fc < 0) break;

						ParagraphPropertyExceptions* papx = findValidPapx(fc);
						TableInfo tai(papx, m_document->nWordVersion);

						if ( tai.fInTable )
						{
							//this PAPX is for a table
							Table table(this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ));
							table.Convert(this);
							cp = table.GetCPEnd();
						}
						else
						{
							//this PAPX is for a normal paragraph
							cp = writeParagraph(cp, 0x7fffffff);
						}
					}

                    m_pXmlWriter->WriteNodeEnd(L"w:comment" );
				}

                m_pXmlWriter->WriteNodeEnd( L"w:comments" );
				m_context->_docx->CommentsXML = std::wstring(m_pXmlWriter->GetXmlString()); 
			}
		}
	};
}
