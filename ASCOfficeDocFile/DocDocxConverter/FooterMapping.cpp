#include "stdafx.h"

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
		m_pXmlWriter->WriteNodeBegin( _T( "?xml version=\"1.0\" encoding=\"UTF-8\"?" ) );
		m_pXmlWriter->WriteNodeBegin( _T( "w:ftr" ), TRUE );

		//write namespaces
		m_pXmlWriter->WriteAttribute( _T( "xmlns:w" ), OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteAttribute( _T( "xmlns:v" ), OpenXmlNamespaces::VectorML );
		m_pXmlWriter->WriteAttribute( _T( "xmlns:o" ), OpenXmlNamespaces::Office );
		m_pXmlWriter->WriteAttribute( _T( "xmlns:w10" ), OpenXmlNamespaces::OfficeWord );
		m_pXmlWriter->WriteAttribute( _T( "xmlns:r" ), OpenXmlNamespaces::Relationships );
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

		//convert the footer text
		_lastValidPapx = (*(m_document->AllPapxFkps->begin()))->grppapx[0];
		int cp		=	_ftr.GetCharacterPosition();
		int cpMax	=	_ftr.GetCharacterPosition() + _ftr.GetCharacterCount();

		//the CharacterCount of the footers also counts the guard paragraph mark.
		//this additional paragraph mark shall not be converted.
		cpMax--;

		while ( cp < cpMax )
		{
			int fc =  m_document->FindFileCharPos(cp);
			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx );

			if ( tai.fInTable )
			{
				//this PAPX is for a table
				//cp = writeTable( cp, tai.iTap );
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

		m_pXmlWriter->WriteNodeEnd( _T( "w:ftr" ) );

		m_context->_docx->FooterXMLList.push_back( wstring( m_pXmlWriter->GetXmlString() ) );
	}
}