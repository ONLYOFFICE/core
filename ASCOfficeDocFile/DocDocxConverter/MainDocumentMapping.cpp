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
		m_pXmlWriter->WriteAttribute(_T("xmlns:w"),		OpenXmlNamespaces::WordprocessingML );
		m_pXmlWriter->WriteAttribute(_T("xmlns:v"),		OpenXmlNamespaces::VectorML );
		m_pXmlWriter->WriteAttribute(_T("xmlns:o"),		OpenXmlNamespaces::Office );
		m_pXmlWriter->WriteAttribute(_T("xmlns:w10"),	OpenXmlNamespaces::OfficeWord );
		m_pXmlWriter->WriteAttribute(_T("xmlns:r"),		OpenXmlNamespaces::Relationships );
		m_pXmlWriter->WriteAttribute(_T("xmlns:m"),		_T("http://schemas.openxmlformats.org/officeDocument/2006/math"));

		//m_pXmlWriter->WriteAttribute(_T("xmlns:wpc"),	_T("http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:mc"),	_T("http://schemas.openxmlformats.org/markup-compatibility/2006")); 
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wp14"),	_T("http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wp"),	_T("http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:w14"),	_T("http://schemas.microsoft.com/office/word/2010/wordml"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wpg"),	_T("http://schemas.microsoft.com/office/word/2010/wordprocessingGroup"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wpi"),	_T("http://schemas.microsoft.com/office/word/2010/wordprocessingInk"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wne"),	_T("http://schemas.microsoft.com/office/word/2006/wordml"));
		//m_pXmlWriter->WriteAttribute(_T("xmlns:wps"),	_T("http://schemas.microsoft.com/office/word/2010/wordprocessingShape"));
		//m_pXmlWriter->WriteAttribute(_T("mc:Ignorable"), _T("w14 wp14"));
		
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
			if (fc < 0) break;

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
				
				int fc						=	m_document->FindFileCharPos(0);
				int fcEnd					=	m_document->FindFileCharPos(countTextRel);

				if (fc < 0 || fcEnd < 0) break;
				
				// Read the chars
				vector<wchar_t>* chpxChars	=	m_document->m_PieceTable->GetEncodingChars (fc, fcEnd, m_document->WordDocumentStream);		//<! NEED OPTIMIZE
				wstring text (chpxChars->begin(), chpxChars->end());
				
				//open a new w:t element
				m_pXmlWriter->WriteNodeBegin( _T( "w:t" ), TRUE );
				//if (text.find(_T("\x20")) != text.npos)
				{
					m_pXmlWriter->WriteAttribute( _T( "xml:space" ), _T( "preserve" ) ); 
				}
				m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

				// Write text
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