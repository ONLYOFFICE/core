#pragma once

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

				int index = 0; 

				m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
				m_pXmlWriter->WriteNodeBegin( _T( "w:comments" ), TRUE );

				//write namespaces
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:v" ), OpenXmlNamespaces::VectorML );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:o" ), OpenXmlNamespaces::Office );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:w10" ), OpenXmlNamespaces::OfficeWord );
				m_pXmlWriter->WriteAttribute( _T( "xmlns:r" ), OpenXmlNamespaces::Relationships );
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				int cp = m_document->FIB->m_RgLw97.ccpText + m_document->FIB->m_RgLw97.ccpFtn + m_document->FIB->m_RgLw97.ccpHdr;

				size_t count = m_document->AnnotationsReferencePlex->Elements.size();
				for (unsigned int i = 0; i < count; ++i)
				{   
					AnnotationReferenceDescriptor* atrdPre10 = static_cast<AnnotationReferenceDescriptor*>(m_document->AnnotationsReferencePlex->Elements[index]);

					m_pXmlWriter->WriteNodeBegin( _T( "w:comment" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString( index ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "w:author" ), m_document->AnnotationOwners->at( atrdPre10->GetAuthorIndex() ).c_str() );
					m_pXmlWriter->WriteAttribute( _T( "w:initials" ), atrdPre10->GetUserInitials().c_str() );

					//!!!TODO!!!
					/*//ATRDpost10 is optional and not saved in all files
					if (doc.AnnotationReferenceExtraTable != null && 
					doc.AnnotationReferenceExtraTable.Count > index)
					{
					AnnotationReferenceDescriptorExtra atrdPost10 = doc.AnnotationReferenceExtraTable[index];
					atrdPost10.Date.Convert(new DateMapping(_writer));
					}*/

					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					while ( ( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpHdr ) < (*m_document->IndividualCommentsPlex)[index + 1] )
					{
						int fc = m_document->FindFileCharPos(cp);

						ParagraphPropertyExceptions* papx = findValidPapx(fc);
						TableInfo tai(papx);

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
							cp = writeParagraph(cp);
						}
					}

					m_pXmlWriter->WriteNodeEnd(_T( "w:comment" ));

					++index;
				}

				m_pXmlWriter->WriteNodeEnd( _T( "w:comments" ) );
				m_context->_docx->CommentsXML = std::wstring(m_pXmlWriter->GetXmlString()); 
			}
		}
	};
}