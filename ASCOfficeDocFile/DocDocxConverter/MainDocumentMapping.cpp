
#include "MainDocumentMapping.h"

namespace DocFileFormat
{
	MainDocumentMapping::MainDocumentMapping (ConversionContext* ctx, const ProgressCallback* ffCallBack) : DocumentMapping( ctx, this ), m_ffCallBack(NULL)
	{
		m_ffCallBack	=	ffCallBack;
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
		m_context->_docx->RegisterDocument();

		// Header
		m_pXmlWriter->WriteNodeBegin(_T("?xml version=\"1.0\" encoding=\"UTF-8\"?"));
		m_pXmlWriter->WriteNodeBegin(_T("w:document"), TRUE );

		// Namespaces
		m_pXmlWriter->WriteAttribute(_T("xmlns:w"), OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteAttribute(_T("xmlns:v"), OpenXmlNamespaces::VectorML );
		m_pXmlWriter->WriteAttribute(_T("xmlns:o"), OpenXmlNamespaces::Office );
		m_pXmlWriter->WriteAttribute(_T("xmlns:w10"), OpenXmlNamespaces::OfficeWord );
		m_pXmlWriter->WriteAttribute(_T("xmlns:r"), OpenXmlNamespaces::Relationships );

		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

		m_pXmlWriter->WriteNodeBegin( _T("w:body"), FALSE );

		// Convert the document
		_lastValidPapx = (*(m_document->AllPapxFkps->begin()))->grppapx[0];

		int cp = 0;
		int fc = 0;
		ParagraphPropertyExceptions* papx = NULL;

		unsigned int index = 0;
		const unsigned int PROGRESS_COUNT = 10;
		const unsigned int progressStep = ( m_document->FIB->m_RgLw97.ccpText / PROGRESS_COUNT );

		int countText		=	m_document->FIB->m_RgLw97.ccpText;
		int countTextRel	=	m_document->FIB->m_RgLw97.ccpText - 1;
							
		while (cp < countText)
		{
			fc = m_document->FindFileCharPos(cp);

			papx = findValidPapx(fc);

			if (papx)
			{
				TableInfo tai(papx);
				if (tai.fInTable)
				{
					//this PAPX is for a table
					//cp = writeTable( cp, tai.iTap );
					Table table( this, cp, ( ( tai.iTap > 0 ) ? ( 1 ) : ( 0 ) ) );
					table.Convert(this);
					cp = table.GetCPEnd();
				}
				else
				{
					//this PAPX is for a normal paragraph
					cp = writeParagraph(cp);
				}
			}
			else
			{
				//There are no paragraphs, only text

				//start paragraph
				m_pXmlWriter->WriteNodeBegin( _T( "w:p" ) );
				//start run
				m_pXmlWriter->WriteNodeBegin( _T( "w:r" ) );
				//open a new w:t element
				m_pXmlWriter->WriteNodeBegin( _T( "w:t" ), TRUE );
				m_pXmlWriter->WriteAttribute( _T( "xml:space" ), _T( "preserve" ) ); 
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				// Write text
				
				int fc						=	m_document->FindFileCharPos(0);
				int fcEnd					=	m_document->FindFileCharPos(countTextRel);
				
				// Read the chars
				vector<wchar_t>* chpxChars	=	m_document->m_PieceTable->GetEncodingChars (fc, fcEnd, m_document->WordDocumentStream);		//<! NEED OPTIMIZE
				wstring text (chpxChars->begin(), chpxChars->end());
				m_pXmlWriter->WriteString(text.c_str());
				RELEASEOBJECT(chpxChars);

				//close previous w:t ...
				m_pXmlWriter->WriteNodeEnd( _T( "w:t" ) );
				//close previous w:r ...
				m_pXmlWriter->WriteNodeEnd( _T( "w:r" ) );
				//close previous w:p ...
				m_pXmlWriter->WriteNodeEnd( _T( "w:p" ) );

				cp = m_document->FIB->m_RgLw97.ccpText;
			}

			if (m_ffCallBack)
			{
				if (( (unsigned int) cp > (progressStep * index)  ) && (m_ffCallBack))
				{
					double progress = ( double( 800000 - 500000 ) / m_document->FIB->m_RgLw97.ccpText * cp );

					m_ffCallBack->OnProgress (m_ffCallBack->caller, DOC_ONPROGRESSEVENT_ID, long( 500000 + progress ));

					short bCancel = 0;
					m_ffCallBack->OnProgressEx (m_ffCallBack->caller, DOC_ONPROGRESSEVENT_ID, long( 500000 + progress ), &bCancel);

					if (0 != bCancel)
						return;

					++index;
				}
			}
		}

		//write the section properties of the body with the last SEPX
		if (m_document->AllSepx)
		{
			int lastSepxCp = 0;

			for (map<int, SectionPropertyExceptions*>::iterator iter = m_document->AllSepx->begin(); iter != m_document->AllSepx->end(); ++iter) 
				lastSepxCp = iter->first;

			SectionPropertyExceptions* lastSepx	=	m_document->AllSepx->operator []( lastSepxCp );

			SectionPropertiesMapping* pSection	=	new SectionPropertiesMapping (m_pXmlWriter, m_context, _sectionNr);
			if (pSection)
			{
				lastSepx->Convert( pSection );
				RELEASEOBJECT(pSection);
			}
		}

		m_pXmlWriter->WriteNodeEnd( _T( "w:body" ) );
		m_pXmlWriter->WriteNodeEnd( _T( "w:document" ) );

		m_context->_docx->DocumentXML = wstring(m_pXmlWriter->GetXmlString());
	}
}