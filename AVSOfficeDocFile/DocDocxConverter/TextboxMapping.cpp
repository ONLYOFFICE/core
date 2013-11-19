#include "stdafx.h"

#include "TextboxMapping.h"
#include "TableMapping.h"
#include "WordDocument.h"

namespace DocFileFormat
{
	int TextboxMapping::TextboxCount = 0;

	TextboxMapping::TextboxMapping (ConversionContext* ctx, int nTBIndex, XmlUtils::CXmlWriter* writer, IMapping* caller ): DocumentMapping( ctx, writer, caller ), m_nTBIndex(0)
	{
		TextboxCount++;
		m_nTBIndex	=	nTBIndex;
		SetInset(-1,-1,-1,-1);
	}


	TextboxMapping::TextboxMapping (ConversionContext* ctx, XmlUtils::CXmlWriter* writer, IMapping* caller) : DocumentMapping( ctx, writer, caller ), m_nTBIndex(0)
	{
		TextboxCount++;
		m_nTBIndex	=	TextboxCount - 1;
		SetInset(-1,-1,-1,-1);
	}

	TextboxMapping::~TextboxMapping()
	{
		//TextboxCount--;  
	}
}

namespace DocFileFormat
{	
	void TextboxMapping::SetInset (int nLeft, int nTop, int nRight, int nBottom)
	{
		m_ndxTextLeft	=	nLeft;
		m_ndyTextTop	=	nTop;
		m_ndxTextRight	=	nRight;
		m_ndyTextBottom	=	nBottom;
	}

	void TextboxMapping::Apply (IVisitable* visited)
	{
		m_document = static_cast<WordDocument*>(visited);

		if (m_document)
		{
			//if (m_nTBIndex < m_document->TextboxBreakPlex->Elements.size())
			//	return;

			//TODO найти, есть ли в документации эквивалент inset
			if(m_ndxTextLeft >= 0 && m_ndyTextTop >= 0 && m_ndxTextRight >= 0 && m_ndyTextBottom >= 0)
				m_pXmlWriter->WriteNodeBegin( (_T("v:textbox inset=\"")+FormatUtils::IntToWideString(m_ndxTextLeft)+_T(",")+FormatUtils::IntToWideString(m_ndyTextTop)+_T(",")+FormatUtils::IntToWideString(m_ndxTextRight)+_T(",")+FormatUtils::IntToWideString(m_ndyTextBottom)+_T("\"")).c_str() );
			else
				m_pXmlWriter->WriteNodeBegin( _T( "v:textbox" ) );
			
			m_pXmlWriter->WriteNodeBegin( _T( "w:txbxContent" ) );

			int cp		=	0;
			int cpEnd	=	0;
			Tbkd* bkd	=	NULL;

			int txtbxSubdocStart	=	
				m_document->FIB->m_RgLw97.ccpText + 
				m_document->FIB->m_RgLw97.ccpFtn + 
				m_document->FIB->m_RgLw97.ccpHdr +
				m_document->FIB->m_RgLw97.ccpAtn +
				m_document->FIB->m_RgLw97.ccpEdn;

			if (typeid(*_caller) == typeid(MainDocumentMapping))
			{
				bkd					=	static_cast<Tbkd*>(m_document->TextboxBreakPlex->Elements[m_nTBIndex]);
				
				cp					=	txtbxSubdocStart + m_document->TextboxBreakPlex->CharacterPositions[m_nTBIndex];
				cpEnd				=	txtbxSubdocStart + m_document->TextboxBreakPlex->CharacterPositions[m_nTBIndex + 1];				
			}
			else if ((typeid(*_caller) == typeid(HeaderMapping)) || (typeid(*_caller) == typeid(FooterMapping)))
			{
				txtbxSubdocStart	+=	m_document->FIB->m_RgLw97.ccpTxbx;
				
				bkd					=	static_cast<Tbkd*>(m_document->TextboxBreakPlexHeader->Elements[m_nTBIndex]);
			
				cp					=	txtbxSubdocStart + m_document->TextboxBreakPlexHeader->CharacterPositions[m_nTBIndex];
				cpEnd				=	txtbxSubdocStart + m_document->TextboxBreakPlexHeader->CharacterPositions[m_nTBIndex + 1];
			}

			//convert the textbox text
			_lastValidPapx = (*(m_document->AllPapxFkps->begin()))->grppapx[0];

			while (cp < cpEnd)
			{
				int fc = m_document->FindFileCharPos(cp);

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

			m_pXmlWriter->WriteNodeEnd( _T( "w:txbxContent" ) );
			m_pXmlWriter->WriteNodeEnd( _T( "v:textbox" ) );
		}
	}
}