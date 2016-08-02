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
#include "DocumentMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CommentsMapping.h"
#include "TablePropertiesMapping.h"
#include "MainDocumentMapping.h"
#include "DrawingPrimitives.h"

#include "../Common/TextMark.h"
#include "../Common/FormatUtils.h"

namespace DocFileFormat
{
	DocumentMapping::DocumentMapping(ConversionContext* context, IMapping* caller):_skipRuns(0), _lastValidPapx(NULL), _lastValidSepx(NULL), _writeInstrText(false),
		_fldCharCounter(0), AbstractOpenXmlMapping( new XmlUtils::CXmlWriter() ), _sectionNr(0), _footnoteNr(0),
		_endnoteNr(0), _commentNr(0), _caller(caller)
	{
		m_document				=	NULL;
		m_context				=	context;
		m_bInternalXmlWriter	=	false;

		_writeWebHidden			=	false;
		_writeInstrText			=	false;
		_isSectionPageBreak		=	0;
	}

	DocumentMapping::DocumentMapping(ConversionContext* context, XmlUtils::CXmlWriter* writer, IMapping* caller):_skipRuns(0),  _lastValidPapx(NULL), _lastValidSepx(NULL), _writeInstrText(false),
		_fldCharCounter(0), AbstractOpenXmlMapping(writer), _sectionNr(0), _footnoteNr(0), _endnoteNr(0),
		_commentNr(0), _caller(caller)
	{
		m_document				=	NULL;
		m_context				=	context;
		m_bInternalXmlWriter	=	false;
	
		_writeWebHidden			=	false;
		_writeInstrText			=	false;
		_isSectionPageBreak		=	0;
	}

	DocumentMapping::~DocumentMapping()
	{
		if (m_bInternalXmlWriter)
		{
			RELEASEOBJECT(m_pXmlWriter);
		}
	}
}

namespace DocFileFormat
{
	// Looks into the section table to find out if this CP is the end & current  of a sections 
	
	int DocumentMapping::getCurrentSection(int cp)
	{
		//if cp is the last char of a section, the next section will start at cp +1
		int current = 0;

		for (std::vector<int>::iterator iter = m_document->SectionPlex->CharacterPositions.begin() + 1; iter != m_document->SectionPlex->CharacterPositions.end(); ++iter)
		{
			if (cp < *iter)
			{
				break;
			}
			current++;
		}
		return m_document->SectionPlex->CharacterPositions[current + 1];
	}
	bool DocumentMapping::isSectionEnd(int cp)
	{
		bool result = false;

		//if cp is the last char of a section, the next section will start at cp +1
		int search = cp + 1;

		for (std::vector<int>::iterator iter = m_document->SectionPlex->CharacterPositions.begin(); iter != m_document->SectionPlex->CharacterPositions.end(); ++iter)
		{
			if (*iter == search)
			{
				result = true;
				break;
			}
		}

		return result;
	}

	// Writes a Paragraph that starts at the given cp and 
	// ends at the next paragraph end mark or section end mark

	int DocumentMapping::writeParagraph(int cp) 
	{
		//search the paragraph end
		int cpParaEnd = cp;

		if ((m_document->Text) && (cpParaEnd < (int)m_document->Text->size()))
		{
            while ( ( m_document->Text->at( cpParaEnd ) != TextMark::ParagraphEnd ) &&
                    ( m_document->Text->at( cpParaEnd ) != TextMark::CellOrRowMark ) &&
                    !(( m_document->Text->at( cpParaEnd ) == TextMark::PageBreakOrSectionMark )&&
                    isSectionEnd( cpParaEnd ) ) )
			{
                if (cpParaEnd >= (int)m_document->Text->size()-1) break;
				cpParaEnd++;
			}

			if (TextMark::PageBreakOrSectionMark == m_document->Text->at(cpParaEnd))
			{
				// there is a page break OR section mark,
				// write the section only if it's a section mark
				
				bool sectionEnd = isSectionEnd(cpParaEnd);
				cpParaEnd++;
				
				return writeParagraph(cp, cpParaEnd, sectionEnd);
			}
			else
			{
				cpParaEnd++;
				
				return writeParagraph(cp, cpParaEnd, false);
			}
		}

		return cpParaEnd;
	}

	// Writes a Paragraph that starts at the given cpStart and 
	// ends at the given cpEnd
	
	int DocumentMapping::writeParagraph (int initialCp, int cpEnd, bool sectionEnd, bool lastBad)
	{
		int		cp							=	initialCp;
		int		fc							=	m_document->FindFileCharPos(cp); 
		int		fcEnd						=	m_document->FindFileCharPos(cpEnd);

		if (fc < 0 || fcEnd < 0) 
			return 0;
		
		ParagraphPropertyExceptions* papx	=	findValidPapx(fc);

		// get all CHPX between these boundaries to determine the count of runs
		
		std::list<CharacterPropertyExceptions*>* chpxs	=	m_document->GetCharacterPropertyExceptions(fc, fcEnd);
		std::vector<int>* chpxFcs						=	m_document->GetFileCharacterPositions(fc, fcEnd);

		CharacterPropertyExceptions* paraEndChpx		=	NULL;

		if (chpxFcs)
		{
			chpxFcs->push_back(fcEnd);
		}

		if (chpxs)
		{
			// the last of these CHPX formats the paragraph end mark
			paraEndChpx = *(chpxs->rbegin());
		}

		// start paragraph
		
		m_pXmlWriter->WriteNodeBegin(_T("w:p"), TRUE);
		writeParagraphRsid(papx);

// ----------- check for section properties
		bool isBidi = false;
		SectionPropertyExceptions* currentSection = findValidSepx(getCurrentSection(cp));
		if (currentSection)
		{
			isBidi = currentSection->isBidi;
		}

//-----------------------------------------------------------		
		_isSectionPageBreak = 0;
		if (sectionEnd)
		{
			// this is the last paragraph of this section
			// write properties with section properties
			
			if (papx)
			{
				ParagraphPropertiesMapping oMapping(m_pXmlWriter, m_context, m_document, paraEndChpx, isBidi, findValidSepx(cpEnd), _sectionNr);
				papx->Convert(&oMapping);

				_isSectionPageBreak = oMapping.get_section_page_break();
			}

			++_sectionNr;
		}
		else
		{
			// write properties
			
			if (papx)
			{
				ParagraphPropertiesMapping oMapping(m_pXmlWriter, m_context, m_document, paraEndChpx, isBidi);
				papx->Convert(&oMapping);
			}
		}

		if ((chpxs != NULL) && (chpxFcs != NULL) && !chpxFcs->empty())//? второе
		{
			int i = 0;

			// write a runs for each CHPX
			std::list<CharacterPropertyExceptions*>::iterator cpeIter_last = chpxs->end(); cpeIter_last--;

			for (std::list<CharacterPropertyExceptions*>::iterator cpeIter = chpxs->begin(); cpeIter != chpxs->end(); ++cpeIter)
			{
				//get the FC range for this run

				int fcChpxStart	=	chpxFcs ? chpxFcs->at(i)		: fc;
				int fcChpxEnd	=	chpxFcs ? chpxFcs->at(i + 1)	: fcEnd;

		//?		if (lastBad && cpeIter == cpeIter_last)
		//?		{
		//?			fcChpxEnd = fcEnd;
		//?		}

				//it's the first chpx and it starts before the paragraph

				if ( ( i == 0 ) && ( fcChpxStart < fc ) )
				{
					//so use the FC of the paragraph
					fcChpxStart = fc;
				}

				//it's the last chpx and it exceeds the paragraph

				if ( ( i == ( chpxs->size() - 1 ) ) && ( fcChpxEnd > fcEnd ) )//todooo убрать обращение к size() !!!
				{
					//so use the FC of the paragraph
					fcChpxEnd = fcEnd;
				}

				//read the chars that are formatted via this CHPX
				std::vector<wchar_t>* chpxChars = m_document->GetChars(fcChpxStart, fcChpxEnd, cp);

				//search for bookmarks in the chars
				std::vector<int> bookmarks = searchBookmarks(chpxChars, cp);

				//if there are bookmarks in this run, split the run into several runs
				if (!bookmarks.empty())
				{
					std::list<std::vector<wchar_t>>* runs = splitCharList(chpxChars, &bookmarks);
					if (runs) 
					{
						for (std::list<std::vector<wchar_t> >::iterator iter = runs->begin(); iter != runs->end(); ++iter)
						{
							if (writeBookmarks(cp))
							{
								cp = writeRun(&(*iter), *cpeIter, cp);
							}
						}

						RELEASEOBJECT(runs);
					}
				}
				else
				{
					cp = writeRun(chpxChars, *cpeIter, cp);
				}

				RELEASEOBJECT(chpxChars);

				i++;
			}

			//end paragraph
			m_pXmlWriter->WriteNodeEnd(_T("w:p"));
		}
		else
		{
			//end paragraph
			m_pXmlWriter->WriteNodeEnd(_T("w:p"));
		}

		RELEASEOBJECT(chpxFcs);
		RELEASEOBJECT(chpxs);

		return cpEnd++;
	}

	void DocumentMapping::writeParagraphRsid (const ParagraphPropertyExceptions* papx)
	{
		if (papx)  
		{
			for (std::list<SinglePropertyModifier>::const_iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); ++iter)
			{
				// rsid for paragraph property enditing (write to parent element)
				
				if (DocFileFormat::sprmPRsid == iter->OpCode)
				{
					std::wstring id = FormatUtils::IntToFormattedWideString(FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize), _T("%08x"));
					m_context->AddRsid(id);
					m_pXmlWriter->WriteAttribute(_T("w:rsidP"), id.c_str());

					break;
				}
			}    
		}

		m_pXmlWriter->WriteNodeEnd(_T(""), TRUE, FALSE);
	}

	// Writes a run with the given characters and CHPX

	int DocumentMapping::writeRun (std::vector<wchar_t>* chars, CharacterPropertyExceptions* chpx, int initialCp)
	{
		int cp = initialCp;

		if ((_skipRuns <= 0) && (chars->size() > 0))
		{
			RevisionData rev = RevisionData(chpx);

			if (Deleted == rev.Type)
			{
				//If it's a deleted run
				m_pXmlWriter->WriteNodeBegin(_T("w:del"), TRUE);
				m_pXmlWriter->WriteAttribute(_T("w:author"), _T("[b2x: could not retrieve author]")); 
				m_pXmlWriter->WriteAttribute(_T("w:date"), _T("[b2x: could not retrieve date]")); 
				m_pXmlWriter->WriteNodeEnd(_T(""), TRUE, FALSE);
			}
			else if ( rev.Type == Inserted )
			{
				WideString* author = dynamic_cast<WideString*>(m_document->RevisionAuthorTable->operator[](rev.Isbt));
				//if it's a inserted run
				m_pXmlWriter->WriteNodeBegin(_T("w:ins"), TRUE);
				m_pXmlWriter->WriteAttribute(_T("w:author"), FormatUtils::XmlEncode(*author).c_str()); 
				m_pXmlWriter->WriteNodeEnd(_T(""), TRUE, FALSE);
				//rev.Dttm.Convert(new DateMapping(m_pXmlWriter));
			}

			//start run
			m_pXmlWriter->WriteNodeBegin(_T("w:r"), TRUE);

			//append rsids
			if (0 != rev.Rsid)
			{
				std::wstring rsid = FormatUtils::IntToFormattedWideString(rev.Rsid, _T("%08x"));
				m_pXmlWriter->WriteAttribute(_T("w:rsidR"), rsid.c_str());
				m_context->AddRsid(rsid);
			}

			if (0 != rev.RsidDel)
			{
				std::wstring rsidDel = FormatUtils::IntToFormattedWideString(rev.RsidDel, _T("%08x"));
				m_pXmlWriter->WriteAttribute(_T("w:rsidDel"), rsidDel.c_str());
				m_context->AddRsid(rsidDel);
			}

			if (0 != rev.RsidProp)
			{
				std::wstring rsidProp = FormatUtils::IntToFormattedWideString(rev.RsidProp, _T("%08x")); 
				m_pXmlWriter->WriteAttribute(_T("w:rsidRPr"), rsidProp.c_str());
				m_context->AddRsid(rsidProp);
			}

			m_pXmlWriter->WriteNodeEnd(_T(""), TRUE, FALSE);

			/// <w:rPr>
			CharacterPropertiesMapping* rPr = new CharacterPropertiesMapping(m_pXmlWriter, m_document, &rev, _lastValidPapx, false);
			if (rPr)
			{
				rPr->_webHidden = _writeWebHidden;
				
				chpx->Convert(rPr);
				RELEASEOBJECT(rPr);
			}

			if (Deleted == rev.Type)
			{
				writeText(chars, cp, chpx, true);
			}
			else
			{
				writeText(chars, cp, chpx, false);
			}

			//end run
			m_pXmlWriter->WriteNodeEnd(_T("w:r"));

			if (Deleted == rev.Type)
			{
				m_pXmlWriter->WriteNodeEnd(_T("w:del"));
			}
			else if (Inserted == rev.Type)
			{
				m_pXmlWriter->WriteNodeEnd(_T("w:ins"));
			}

			if (!_writeAfterRun.empty())
			{
				m_pXmlWriter->WriteString(_writeAfterRun.c_str());
				_writeAfterRun.clear();
			}
		}
		else
		{
			--_skipRuns;
		}

		return cp + (int)chars->size();
	}

	// Writes the given text to the document
	void DocumentMapping::writeText(std::vector<wchar_t>* chars, int initialCp, CharacterPropertyExceptions* chpx, bool writeDeletedText)
	{
		int cp = initialCp;

		bool fSpec = isSpecial (chpx);

		//detect text type
		
		std::wstring textType = _T( "t" );
        std::wstring text;

		if (writeDeletedText)
		{
			textType = std::wstring(_T("delText"));
		}
		else if (_writeInstrText)
		{
			textType = std::wstring(_T("instrText"));
		}

		//write text
		for (unsigned int i = 0; i < chars->size(); ++i)
		{
			wchar_t c = chars->at(i), code = c;

			if (TextMark::Tab == code)
			{
                writeTextElement(text, textType);

                text.clear();

				XMLTools::XMLElement<wchar_t> elem(_T("w:tab"));

				m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
			}
			else if (TextMark::HardLineBreak == code)
			{
                writeTextElement(text, textType);

                text.clear();

                XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
				elem.AppendAttribute(_T("w:type"), _T("textWrapping"));
				elem.AppendAttribute(_T("w:clear"), _T("all"));

				m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
			}
			else if (TextMark::ParagraphEnd == code)
			{
				//do nothing
			}
			else if (TextMark::PageBreakOrSectionMark == code)
			{
				//write page break, section breaks are written by writeParagraph() method
				if (/*!isSectionEnd(c)*/_isSectionPageBreak == 0)
				{
                    writeTextElement(text, textType);

                    text.clear();

					XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
					elem.AppendAttribute(_T("w:type"), _T("page"));

					m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
				}
			}
			else if (TextMark::ColumnBreak == code)
			{
                writeTextElement(text, textType);

                text.clear();

				XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
				elem.AppendAttribute(_T("w:type"), _T("column"));

				m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
			}
			else if (TextMark::FieldBeginMark == code)
			{
				int cpFieldStart = initialCp + i;
				int cpFieldEnd = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::FieldEndMark );
				
				std::wstring f;
				if (cpFieldEnd < m_document->Text->size())
					f = std::wstring( ( m_document->Text->begin() + cpFieldStart ), ( m_document->Text->begin() + cpFieldEnd + 1 ) );

				std::wstring EMBED		( _T( " EMBED" ) );
				std::wstring LINK		( _T( " LINK" ) );
				std::wstring FORM		( _T( " FORM" ) );
				std::wstring Excel		( _T( " Excel" ) );
				std::wstring Word		( _T( " Word" ) );
				std::wstring opendocument(_T( " opendocument" ) );
				std::wstring Equation	( _T( " Equation" ) ) ;
				std::wstring MERGEFORMAT( _T( " MERGEFORMAT" ) );
				std::wstring QUOTE		( _T( " QUOTE" ) );
				std::wstring chart		( _T( "Chart" ) );
				std::wstring PBrush		( _T( " PBrush" ) );
				std::wstring TOC		( _T( " TOC" ) );
				std::wstring HYPERLINK	( _T( " HYPERLINK" ) );
				std::wstring PAGEREF	( _T( " PAGEREF" ) );

				bool bChart			= search( f.begin(), f.end(), chart.begin(),		chart.end())			!= f.end();
				bool bEMBED			= search( f.begin(), f.end(), EMBED.begin(),		EMBED.end())			!= f.end();
				bool bLINK			= search( f.begin(), f.end(), LINK.begin(),			LINK.end())				!= f.end();
				bool bOpendocument	= search( f.begin(), f.end(), opendocument.begin(), opendocument.end())		!= f.end();
				bool bFORM			= search( f.begin(), f.end(), FORM.begin(),			FORM.end())				!= f.end();
				bool bMERGEFORMAT	= search( f.begin(), f.end(), MERGEFORMAT.begin(),	MERGEFORMAT.end())		!= f.end();
				bool bExcel			= search( f.begin(), f.end(), Excel.begin(),		Excel.end())			!= f.end();
				bool bWord			= search( f.begin(), f.end(), Word.begin(),			Word.end())				!= f.end();
				bool bHYPERLINK		= search( f.begin(), f.end(), HYPERLINK.begin(),	HYPERLINK.end())		!= f.end();
				bool bPAGEREF		= search( f.begin(), f.end(), PAGEREF.begin(),		PAGEREF.end())			!= f.end();
				bool bQUOTE			= search( f.begin(), f.end(), QUOTE.begin(),		QUOTE.end())			!= f.end();
				bool bEquation		= search( f.begin(), f.end(), Equation.begin(),		Equation.end())			!= f.end();
			
				if ( bFORM )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					int cpPic = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::Picture );

					/*if (cpPic < cpFieldEnd)
					{
					int fcPic = _doc.PieceTable.FileCharacterPositions[cpPic];
					CharacterPropertyExceptions chpxPic = _doc.GetCharacterPropertyExceptions(fcPic, fcPic + 1)[0];
					NilPicfAndBinData npbd = new NilPicfAndBinData(chpxPic, _doc.DataStream);
					FormFieldData ffdata = new FormFieldData(npbd.binData);
					ffdata.Convert(new FormFieldDataMapping(m_pXmlWriter));
					}*/

					m_pXmlWriter->WriteNodeEnd( _T( "w:fldChar" ) );

					_writeInstrText = true;

					_fldCharCounter++;
				}
				else if ( ( bMERGEFORMAT || bExcel || bWord || bOpendocument )
						&& 
						( ( bEMBED || bLINK ) && bChart) )
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

					int cpPic = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::Picture );

					m_pXmlWriter->WriteNodeEnd( _T( "w:fldChar" ) );

					_writeInstrText = true;

					_fldCharCounter++;
				}
				else if ( bHYPERLINK && bPAGEREF )
				{
					int cpFieldSep2 = cpFieldStart, cpFieldSep1 = cpFieldStart;
					std::vector<std::wstring> toc;

					if (search( f.begin(),	f.end(), TOC.begin(),	TOC.end()) != f.end())
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
							m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) ); 
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

						_writeInstrText = true;	
						_fldCharCounter++;
					}
					else
					{
						while ( cpFieldSep2 < cpFieldEnd)
						{
							cpFieldSep2	=	searchNextTextMark(m_document->Text, cpFieldSep1 + 1, TextMark::FieldSeparator);
							std::wstring f1( ( m_document->Text->begin() + cpFieldSep1 ), ( m_document->Text->begin() + cpFieldSep2 + 1 ) );
							toc.push_back(f1);
							
							if (search( f1.begin(),	f1.end(), PAGEREF.begin(), PAGEREF.end()) != f1.end())
							{
								int d = f1.find(PAGEREF);

								_writeWebHidden = true;
								std::wstring _writeTocLink =f1.substr(d + 9);
								d = _writeTocLink.find(_T(" "));
								_writeTocLink = _writeTocLink.substr(0, d);
								
								_writeAfterRun	=	std::wstring (_T("<w:hyperlink w:anchor = \""));
								_writeAfterRun +=	_writeTocLink;
								_writeAfterRun +=	std::wstring (_T("\" w:history=\"1\">"));

								//if (_writeInstrText == true)
								//{
								//	m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
								//		m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "separate" ) ); 
								//	m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );								
								//}
								_writeInstrText = false;
								
								//cp = cpFieldSep1;
							}
							cpFieldSep1 = cpFieldSep2;
						}
						_skipRuns = 5;
					}
				}
				else if (	bEMBED  ||	bLINK ||	bQUOTE)						
				{
					int cpPic		=	searchNextTextMark(m_document->Text, cpFieldStart, TextMark::Picture);
					int cpFieldSep	=	searchNextTextMark(m_document->Text, cpFieldStart, TextMark::FieldSeparator);

					if (cpPic < cpFieldEnd)
					{
						int fcPic = m_document->FindFileCharPos( cpPic );
						std::list<CharacterPropertyExceptions*>* chpxs	=	m_document->GetCharacterPropertyExceptions(fcPic, fcPic + 1); 
						
						CharacterPropertyExceptions* chpxObj =	chpxs->front();

						RevisionData oData = RevisionData(chpxObj);

						CharacterPropertiesMapping* rPr = new CharacterPropertiesMapping(m_pXmlWriter, m_document, &oData, _lastValidPapx, false);
						if(rPr)
						{
							chpxObj->Convert(rPr);
							RELEASEOBJECT(rPr);
						}					
						XmlUtils::CXmlWriter	OleWriter;
						VMLPictureMapping		oVmlMapper (m_context, &OleWriter, true, _caller);

						if (m_document->bOlderVersion)
						{
							OleObject ole ( chpxObj, m_document->GetStorage(), m_document->bOlderVersion);
							
							OleWriter.WriteNodeBegin (_T( "w:object" ), TRUE);
								OleWriter.WriteAttribute( _T( "w:dxaOrig" ), FormatUtils::IntToWideString( ( ole.pictureDesciptor.dxaGoal + ole.pictureDesciptor.dxaOrigin ) ).c_str() ); 
								OleWriter.WriteAttribute( _T( "w:dyaOrig" ), FormatUtils::IntToWideString( ( ole.pictureDesciptor.dyaGoal + ole.pictureDesciptor.dyaOrigin ) ).c_str() ); 
							OleWriter.WriteNodeEnd( _T( "" ), TRUE, FALSE );

							ole.pictureDesciptor.Convert(&oVmlMapper);
							OleObjectMapping oleObjectMapping( &OleWriter, m_context, &ole.pictureDesciptor, _caller, oVmlMapper.GetShapeId() );
							
							ole.Convert( &oleObjectMapping );
						}
						else
						{
							PictureDescriptor pic(chpxObj, m_document->DataStream, 0x7fffffff, m_document->bOlderVersion);
							
							OleWriter.WriteNodeBegin (_T( "w:object" ), TRUE);
								OleWriter.WriteAttribute( _T( "w:dxaOrig" ), FormatUtils::IntToWideString( ( pic.dxaGoal + pic.dxaOrigin ) ).c_str() ); 
								OleWriter.WriteAttribute( _T( "w:dyaOrig" ), FormatUtils::IntToWideString( ( pic.dyaGoal + pic.dyaOrigin ) ).c_str() ); 
							OleWriter.WriteNodeEnd( _T( "" ), TRUE, FALSE );
							
							pic.Convert(&oVmlMapper);
							RELEASEOBJECT(chpxs);

							if ( cpFieldSep < cpFieldEnd  && m_document->m_PieceTable)
							{
								int fcFieldSep = m_document->m_PieceTable->FileCharacterPositions->operator []( cpFieldSep );
								int fcFieldSep1 = m_document->FindFileCharPos( cpFieldSep );
								
								std::list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions( fcFieldSep, ( fcFieldSep + 1 ) ); 
								CharacterPropertyExceptions* chpxSep = chpxs->front();
								
								OleObject ole ( chpxSep, m_document->GetStorage(), m_document->bOlderVersion);
								OleObjectMapping oleObjectMapping( &OleWriter, m_context, &pic, _caller, oVmlMapper.GetShapeId() );
								
								if (oVmlMapper.m_isEmbedded)
								{
									ole.isEquation		= oVmlMapper.m_isEquation;
									ole.isEmbedded		= oVmlMapper.m_isEmbedded;
									ole.emeddedData		= oVmlMapper.m_embeddedData;
								}
								ole.Convert( &oleObjectMapping );
								
								RELEASEOBJECT( chpxs );
							}
						}
						OleWriter.WriteNodeEnd( _T( "w:object" ) );	

						if (!oVmlMapper.m_isEmbedded && oVmlMapper.m_isEquation)
						{
							//нельзя в Run писать oMath
							//m_pXmlWriter->WriteString(oVmlMapper.m_equationXml.c_str());
							_writeAfterRun = oVmlMapper.m_equationXml;
						}
						else
						{
							m_pXmlWriter->WriteString(OleWriter.GetXmlString());
						}	
					}

					if (bEMBED)	_skipRuns = 3;
					else		_skipRuns = 5;
				}					
				else
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) ); 
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

					_writeInstrText = true;
					_fldCharCounter++;
				}
			}
			else if (TextMark::FieldSeparator == code)
			{
				if (_fldCharCounter > 0)
				{
					XMLTools::XMLElement<wchar_t> elem( _T( "w:fldChar" ) );
					elem.AppendAttribute( _T( "w:fldCharType" ), _T( "separate" ) );

					m_pXmlWriter->WriteString( elem.GetXMLString().c_str() );
				}
			}
			else if (TextMark::FieldEndMark == code)
			{
				if (_fldCharCounter > 0)
				{
					XMLTools::XMLElement<wchar_t> elem( _T( "w:fldChar" ) );
					elem.AppendAttribute( _T( "w:fldCharType" ), _T( "end" ) );

					m_pXmlWriter->WriteString( elem.GetXMLString().c_str() );

					_fldCharCounter--;
				}
				if (_writeWebHidden)
				{
					_writeAfterRun	=	std::wstring (_T("</w:hyperlink>"));
				}
				_writeWebHidden	= false;
				
				if ( _fldCharCounter == 0 )
				{	
					_writeInstrText	= false;
				}
			}
			else if ((TextMark::Symbol == code) && fSpec)
			{
				Symbol s = getSymbol( chpx );

				m_pXmlWriter->WriteNodeBegin(_T("w:sym"), TRUE);
				m_pXmlWriter->WriteAttribute(_T("w:font"), FormatUtils::XmlEncode(s.FontName).c_str()); 
				m_pXmlWriter->WriteAttribute(_T("w:char"), FormatUtils::XmlEncode(s.HexValue).c_str()); 
				m_pXmlWriter->WriteNodeEnd(_T(""), TRUE);
			}
			else if ((TextMark::DrawnObject == code) && fSpec)
			{
				Spa* pSpa	=	NULL;
				if (typeid(*this) == typeid(MainDocumentMapping))
				{
					pSpa	=	static_cast<Spa*>(m_document->OfficeDrawingPlex->GetStruct(cp));
				}
				else if ((typeid(*this) == typeid(HeaderMapping) ) || ( typeid(*this) == typeid(FooterMapping)))
				{
					int headerCp	=	( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn );
					pSpa	=	static_cast<Spa*>(m_document->OfficeDrawingPlexHeader->GetStruct(headerCp));
				}

				if (pSpa)
				{
					ShapeContainer* pShape = m_document->GetOfficeArt()->GetShapeContainer(pSpa->GetShapeID());

					if (pShape)
					{
						m_pXmlWriter->WriteNodeBegin (_T("w:pict"));
						VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, pSpa, NULL,  _caller);
						pShape->Convert(&oVmlWriter);
						m_pXmlWriter->WriteNodeEnd (_T("w:pict"));
					}
					
					if (!pSpa->primitives.empty())
					{
						m_pXmlWriter->WriteNodeBegin (_T("w:pict"));
						VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, pSpa, NULL,  _caller);
						pSpa->primitives.Convert(&oVmlWriter);
						m_pXmlWriter->WriteNodeEnd (_T("w:pict"));
					}
				}
			}
			else if ((TextMark::Picture == code) && fSpec )
			{
				PictureDescriptor oPicture (chpx, m_document->bOlderVersion ? m_document->WordDocumentStream : m_document->DataStream, 0x7fffffff, m_document->bOlderVersion);

				if (oPicture.embeddedData && oPicture.embeddedDataSize > 0)
				{
					m_pXmlWriter->WriteNodeBegin (_T("w:pict"));
					
					VMLPictureMapping oVmlMapper(m_context, m_pXmlWriter, false, _caller);
					oPicture.Convert (&oVmlMapper);
					
					m_pXmlWriter->WriteNodeEnd	 (_T("w:pict"));
				}
				else if ((oPicture.mfp.mm > 98) && (NULL != oPicture.shapeContainer))
				{
					m_pXmlWriter->WriteNodeBegin (_T("w:pict"));

					bool picture = true;

					if (oPicture.shapeContainer)
					{
						int shape_type = oPicture.shapeContainer->getShapeType();

						if (shape_type != msosptPictureFrame) picture = false;
					}
					
					if (picture)
					{
						VMLPictureMapping oVmlMapper(m_context, m_pXmlWriter, false, _caller);
						oPicture.Convert (&oVmlMapper);
						
						if (oVmlMapper.m_isEmbedded)
						{
							OleObject ole ( chpx, m_document->GetStorage(), m_document->bOlderVersion);
							OleObjectMapping oleObjectMapping( m_pXmlWriter, m_context, &oPicture, _caller, oVmlMapper.GetShapeId() );
							
							ole.isEquation		= oVmlMapper.m_isEquation;
							ole.isEmbedded		= oVmlMapper.m_isEmbedded;
							ole.emeddedData		= oVmlMapper.m_embeddedData;
						
							ole.Convert( &oleObjectMapping );
						}
					}else
					{
						VMLShapeMapping oVmlMapper(m_context, m_pXmlWriter, NULL, &oPicture,  _caller);
						oPicture.shapeContainer->Convert(&oVmlMapper);
					}
					
					m_pXmlWriter->WriteNodeEnd	 (_T("w:pict"));
				}                   
			}
			else if ((TextMark::AutoNumberedFootnoteReference == code) && fSpec)
			{
				if ((m_document->FootnoteReferenceCharactersPlex != NULL) && (m_document->FootnoteReferenceCharactersPlex->IsCpExists(cp)))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:footnoteReference" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString(_footnoteNr++ ).c_str() ); 
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}
				else if ((m_document->IndividualFootnotesPlex != NULL) && (m_document->IndividualFootnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpText)))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:footnoteRef" ), TRUE );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}
				else if ((m_document->EndnoteReferenceCharactersPlex != NULL) && (m_document->EndnoteReferenceCharactersPlex->IsCpExists(cp)))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:endnoteReference" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString(_endnoteNr++ ).c_str() ); 
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}
				else if ((m_document->IndividualEndnotesPlex != NULL) && 
					(m_document->IndividualEndnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpAtn - m_document->FIB->m_RgLw97.ccpHdr - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpText)))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:endnoteRef" ), TRUE );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}
			}
			else if (TextMark::AnnotationReference == code)
			{
				if (typeid(*this) != typeid(CommentsMapping))
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:commentReference" ), TRUE );
					m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString( _commentNr ).c_str() ); 
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}
				else
				{
					m_pXmlWriter->WriteNodeBegin( _T( "w:annotationRef" ), TRUE );
					m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
				}

				_commentNr++;
			}
			else if (!FormatUtils::IsControlSymbol(c) && ((int)c != 0xFFFF))
			{
                text += FormatUtils::GetXMLSymbol(c);
			}

			cp++;
		}

        if (!text.empty())
		{
			//bool preserve_space = (text.find(_T("\x20")) != text.npos) ? true : false;

			writeTextStart(textType, true/*preserve_space*/);

            m_pXmlWriter->WriteString(text.c_str());

            writeTextEnd(textType);
		}
	}

	void DocumentMapping::writeTextElement(const std::wstring& text, const std::wstring& textType)
	{
		if ( !text.empty() )
		{
			bool preserve_space = true;//(text.find(_T("\x20")) != text.npos) ? true : false;
			if (textType == _T("instrText"))
				preserve_space = false;
			
			writeTextStart( textType, preserve_space );

			m_pXmlWriter->WriteString( text.c_str() );

			writeTextEnd( textType );
		}
	}

    void DocumentMapping::writeTextStart(const std::wstring& textType, bool preserve_space)
	{
		std::wstring str = ( std::wstring( _T( "w:" ) ) + textType );

		m_pXmlWriter->WriteNodeBegin( str.c_str(), TRUE );
        if (preserve_space)
		{
            m_pXmlWriter->WriteAttribute( _T( "xml:space" ), _T( "preserve" ) );
        }
		m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );
	}

	void DocumentMapping::writeTextEnd(const std::wstring& textType)
	{
		std::wstring str = ( std::wstring( _T( "w:" ) ) + textType );

		m_pXmlWriter->WriteNodeEnd( str.c_str() );
	}

	// Searches for bookmarks in the list of characters.
	std::vector<int> DocumentMapping::searchBookmarks(std::vector<wchar_t>* chars, int initialCp)
	{
		std::vector<int> ret;

		if (m_document->BookmarkStartPlex->IsValid())
		{
			int cp = initialCp;

			size_t count = chars->size();

			for (size_t i = 0; i < count; ++i)
			{
				if ((m_document->BookmarkStartPlex->IsCpExists(cp)) ||	(m_document->BookmarkEndPlex->IsCpExists(cp)))
				{
					ret.push_back(i);
				}

				++cp;
			}
		}

		return ret;
	}

	ParagraphPropertyExceptions* DocumentMapping::findValidPapx(int fc)
	{
		ParagraphPropertyExceptions* ret	= NULL;
		std::vector<int>* pVector			= m_document->AllPapxVector;
		
		int nMin = 0;
		int nMax = (int)pVector->size();
		if( nMax > 0 )
		{
			while( nMin + 1 < nMax )
			{
				int nCurPos = (nMax + nMin) / 2;
				int nCurVal = pVector->at(nCurPos);
				if( nCurVal > fc )
					nMax = nCurPos;
				else if( nCurVal < fc )
					nMin = nCurPos;
				else
				{
					nMin = nCurPos;
					break;
				}
			}
			int nMinVal = pVector->at(nMin);
			if ( fc >= nMinVal )
			{
				ret = m_document->AllPapx->find(nMinVal)->second;
//?				if (!ret && m_document->AllPapx->size() > 0)
//?				{
//?					map<int, ParagraphPropertyExceptions*>::iterator it = m_document->AllPapx->end();
//?					it--;
//?					do
//?					{
//?						if (it->first < nMinVal && it->second)
//?							break;
//?						it--;
//?					}
//?					while(it != m_document->AllPapx->begin());
//?
//?					ret = it->second;
//?				}
				_lastValidPapx = ret;
			}
		}
		return ret;
	}

	std::list<std::vector<wchar_t> >* DocumentMapping::splitCharList(std::vector<wchar_t>* chars, std::vector<int>* splitIndices)
	{
		std::list<std::vector<wchar_t> >* ret = new std::list<std::vector<wchar_t> >();
		std::vector<wchar_t>		wcharVector;

		int startIndex = 0;

		// add the parts

		for (unsigned int i = 0; i < splitIndices->size(); ++i)
		{
			int cch = splitIndices->at( i ) - startIndex;

			if ( cch > 0 )
			{
				wcharVector = std::vector<wchar_t>( cch );
				copy( chars->begin() + startIndex, chars->begin() + startIndex + cch, wcharVector.begin() );
				ret->push_back( wcharVector );
			}

			wcharVector.clear();
			startIndex += cch;
		}

		//add the last part
		wcharVector = std::vector<wchar_t>( chars->size() - startIndex );
		copy( chars->begin() + startIndex, chars->end(), wcharVector.begin() );
		ret->push_back( wcharVector );
		wcharVector.clear();

		return ret;
	}

	// Writes the table starts at the given cp value
	int DocumentMapping::writeTable(int initialCp, unsigned int nestingLevel)
	{
		int cp = initialCp;
		int fc2 = m_document->FindFileCharPos( cp );
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		//build the table grid
		std::vector<short>* grid = buildTableGrid( cp, nestingLevel );

		//find first row end
		int fcRowEnd = findRowEndFc( cp, nestingLevel );

		TablePropertyExceptions row1Tapx( findValidPapx( fcRowEnd ), m_document->DataStream, m_document->bOlderVersion);

		//start table
		m_pXmlWriter->WriteNodeBegin( _T( "w:tbl" ) );

		//Convert it
		TablePropertiesMapping *tpMapping = new TablePropertiesMapping( m_pXmlWriter, m_document->Styles, grid );

		row1Tapx.Convert( tpMapping );

		RELEASEOBJECT( tpMapping );

		//convert all rows
		if ( nestingLevel > 1 )
		{
			//It's an inner table
			//only convert the cells with the given nesting level
			while ( tai.iTap == nestingLevel )
			{
				cp = writeTableRow( cp, grid, nestingLevel );
				//?fc = m_document->FindFileCharPos(cp );
				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
				papx = findValidPapx( fc );
				tai = TableInfo( papx );
			}
		}
		else
		{
			//It's a outer table (nesting level 1)
			//convert until the end of table is reached
			while ( tai.fInTable )
			{
				cp = writeTableRow( cp, grid, nestingLevel );
				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );
			}
		}

		//close w:tbl
		m_pXmlWriter->WriteNodeEnd( _T( "w:tbl" ) );

		RELEASEOBJECT( grid );

		return cp;
	}

	// Builds a list that contains the width of the several columns of the table.
	std::vector<short>* DocumentMapping::buildTableGrid(int initialCp, unsigned int nestingLevel)
	{
		ParagraphPropertyExceptions* backup = _lastValidPapx;

		std::vector<short> boundaries;
		std::vector<short>* grid = new std::vector<short>();
		
		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		int fcRowEnd = findRowEndFc( cp, cp, nestingLevel );
		ParagraphPropertyExceptions* papx_prev = NULL;

		while ( tai.fInTable )
		{
			//check all SPRMs of this TAPX
			for ( std::list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); iter++ )
			{
				//find the tDef SPRM
				if ( iter->OpCode == sprmTDefTable ||  iter->OpCode == sprmOldTDefTable)
				{
					unsigned char itcMac = iter->Arguments[0];

					for (int i = 0; i < itcMac; i++)
					{
						short boundary1 = FormatUtils::BytesToInt16( iter->Arguments, 1 + ( i * 2 ), iter->argumentsSize );

						if ( find( boundaries.begin(), boundaries.end(), boundary1 ) == boundaries.end() )
						{
							boundaries.push_back( boundary1 );
						}

						short boundary2 = FormatUtils::BytesToInt16( iter->Arguments, 1 + ( ( i + 1 ) * 2 ), iter->argumentsSize );

						if ( find( boundaries.begin(), boundaries.end(), boundary2 ) == boundaries.end() )
						{
							boundaries.push_back( boundary2 );
						}
					}
				}
			}

			//get the next papx
			papx = findValidPapx( fcRowEnd );
			tai = TableInfo( papx );
			fcRowEnd = findRowEndFc( cp, cp, nestingLevel );

			if (papx_prev && papx_prev == papx )
				break;//file(12).doc
			papx_prev = papx;
		}

		//build the grid based on the boundaries
		sort( boundaries.begin(), boundaries.end() );

		if ( !boundaries.empty() )
		{
			for ( unsigned int i = 0; i < ( boundaries.size() - 1 ); i++ )
			{
				grid->push_back( boundaries[i + 1] - boundaries[i] );
			}
		}

		_lastValidPapx = backup;

		return grid;
	}

	// Finds the FC of the next row end mark.
	int DocumentMapping::findRowEndFc(int initialCp, int& rowEndCp, unsigned int nestingLevel )
	{
		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		if ( nestingLevel > 1 )
		{
			//Its an inner table.
			//Search the "inner table trailer paragraph"
			while ( ( tai.fInnerTtp == false ) && ( tai.fInTable == true ) )
			{
				while ( m_document->Text->at( cp ) != TextMark::ParagraphEnd )
				{
					cp++;
				}

				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );

				cp++;
			}
		}
		else 
		{
			//Its an outer table.
			//Search the "table trailer paragraph"
			while ( ( tai.fTtp == false ) && ( tai.fInTable == true ) )
			{
				while ( true )
				{
					if (cp >= m_document->Text->size())
						break;
					if (m_document->Text->at( cp ) == TextMark::CellOrRowMark)
						break;
					cp++;
				}

				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );
				cp++;
			}
		}

		rowEndCp = cp;
		return fc;
	}

	// Finds the FC of the next row end mark.
	int DocumentMapping::findRowEndFc(int initialCp, unsigned int nestingLevel )
	{
		if ( initialCp > m_document->Text->size() - 1) 
			return initialCp;

		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		if ( nestingLevel > 1 )
		{
			//Its an inner table.
			//Search the "inner table trailer paragraph"
			while ( ( tai.fInnerTtp == false ) && ( tai.fInTable == true ) )
			{
				while ( true )
				{
					if (cp > m_document->Text->size() - 1 ) 
						break;
					if (m_document->Text->at( cp ) == TextMark::ParagraphEnd)
						break;
					cp++;
				}

				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );

				cp++;
			}
		}
		else 
		{
			//Its an outer table.
			//Search the "table trailer paragraph"
			while ( ( tai.fTtp == false ) && ( tai.fInTable == true ) )
			{
				while (true)
				{
					if (cp > m_document->Text->size() - 1 ) 
						break;
					if (m_document->Text->at( cp ) == TextMark::CellOrRowMark)
						break;
					cp++;
				}

				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );
				cp++;
			}
		}

		return fc;
	}

	/// Writes the table row that starts at the given cp value and ends at the next row end mark
	int DocumentMapping::writeTableRow(int initialCp, std::vector<short>* grid, unsigned int nestingLevel)
	{
		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		//start w:tr
		m_pXmlWriter->WriteNodeBegin( _T( "w:tr" ) );

		//convert the properties
		int fcRowEnd = findRowEndFc( cp, nestingLevel );
		TablePropertyExceptions tapx( findValidPapx( fcRowEnd ), m_document->DataStream, m_document->bOlderVersion);

		std::list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions( fcRowEnd, fcRowEnd + 1 );
		TableRowPropertiesMapping* trpMapping = new TableRowPropertiesMapping( m_pXmlWriter, *(chpxs->begin()) );
		tapx.Convert( trpMapping );
		
		RELEASEOBJECT( trpMapping );

		int gridIndex = 0;
		int cellIndex = 0;

		if ( nestingLevel > 1 )
		{
			//It's an inner table.
			//Write until the first "inner trailer paragraph" is reached
			while ( !( ( m_document->Text->at( cp ) == TextMark::ParagraphEnd ) && ( tai.fInnerTtp ) ) && tai.fInTable )
			{
				cp = writeTableCell( cp, &tapx, grid, gridIndex, cellIndex, nestingLevel );
				cellIndex++;

				//each cell has it's own PAPX
				fc = m_document->FindFileCharPos(cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );
			}
		}
		else
		{
			//It's a outer table
			//Write until the first "row end trailer paragraph" is reached
			while ( !( ( m_document->Text->at( cp ) == TextMark::CellOrRowMark ) && ( tai.fTtp ) ) 
				&& tai.fInTable )
			{
				cp = writeTableCell( cp, &tapx, grid, gridIndex, cellIndex, nestingLevel );
				cellIndex++;

				//each cell has it's own PAPX
				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx );
			}
		}

		//end w:tr
		m_pXmlWriter->WriteNodeEnd( _T( "w:tr" ) );

		//skip the row end mark
		cp++;

		RELEASEOBJECT( chpxs );

		return cp;
	}

	/// Writes the table cell that starts at the given cp value and ends at the next cell end mark
	int DocumentMapping::writeTableCell(int initialCp, TablePropertyExceptions* tapx, std::vector<short>* grid, int& gridIndex, int cellIndex, unsigned int nestingLevel )  
	{
		int cp = initialCp;

		//start w:tc
		m_pXmlWriter->WriteNodeBegin( _T( "w:tc" ) );

		//find cell end
		int cpCellEnd = findCellEndCp( initialCp, nestingLevel );

		//convert the properties
		TableCellPropertiesMapping* tcpMapping = new TableCellPropertiesMapping( m_pXmlWriter, grid, gridIndex, cellIndex );

		if ( tapx != NULL )
		{
			tapx->Convert( tcpMapping );
		}

		gridIndex = gridIndex + tcpMapping->GetGridSpan();

		RELEASEOBJECT( tcpMapping );

		//write the paragraphs of the cell
		while ( cp < cpCellEnd )
		{
			//cp = writeParagraph(cp);
			int fc = m_document->FindFileCharPos( cp );

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx );

			//cp = writeParagraph(cp);

			//!!!TODO: Inner Tables!!!
			if ( tai.iTap > nestingLevel )
			{
				//write the inner table if this is not a inner table (endless loop)
				cp = writeTable( cp, tai.iTap );

				//after a inner table must be at least one paragraph
				/*if ( cp >= cpCellEnd )
				{
				m_pXmlWriter->WriteNodeBegin( _T( "w:p" ) );
				m_pXmlWriter->WriteNodeEnd( _T( "w:p" ) );
				}*/
			}
			else
			{
				//this PAPX is for a normal paragraph
				cp = writeParagraph( cp );
			}
		}

		//end w:tc
		m_pXmlWriter->WriteNodeEnd( _T( "w:tc" ) );

		return cp;
	}

	int DocumentMapping::findCellEndCp(int initialCp, unsigned int nestingLevel)
	{
		int cpCellEnd = initialCp;

		if ( nestingLevel > 1 )
		{
			int fc = m_document->FindFileCharPos( initialCp );

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx );

			while ( !tai.fInnerTableCell )
			{
				cpCellEnd++;

				fc = m_document->FindFileCharPos( cpCellEnd );
				
				papx = findValidPapx( fc );
				tai = TableInfo( papx );
			}

			cpCellEnd++;
		}
		else 
		{
			while ( m_document->Text->at( cpCellEnd ) != TextMark::CellOrRowMark )
			{
				cpCellEnd++;
			}

			cpCellEnd++;
		}

		return cpCellEnd;
	}

	//
	bool DocumentMapping::writeBookmarks(int cp)
	{
		bool result =	true;
		int count	=	(int)m_document->BookmarkStartEndCPs.size();

		for (int b = 0; b < count; ++b)
		{
			if (m_document->BookmarkStartEndCPs[b].first == cp)
			{
				result = writeBookmarkStart(b);
			}

			if (m_document->BookmarkStartEndCPs[b].second == cp)
			{
				result = writeBookmarkEnd(b);  
			}
		}

		return result;
	}

	bool DocumentMapping::writeBookmarkStart(short id)
	{
		// write bookmark start
		
		WideString* bookmarkName = static_cast<WideString*>(m_document->BookmarkNames->operator [](id));

		if ((bookmarkName != NULL) && (*bookmarkName != _T("_PictureBullets")))
		{
			XMLTools::XMLElement<wchar_t> bookmarkElem(_T("w:bookmarkStart"));

			bookmarkElem.AppendAttribute(_T("w:id"), FormatUtils::IntToWideString(id).c_str());
			bookmarkElem.AppendAttribute(_T("w:name"), bookmarkName->c_str());

			m_pXmlWriter->WriteString(bookmarkElem.GetXMLString().c_str());

			return true;
		}

		return false;
	}

	bool DocumentMapping::writeBookmarkEnd(short id)
	{
		// write bookmark end
		
		WideString* bookmarkName = static_cast<WideString*>( m_document->BookmarkNames->operator [] ( id ) );

		if ( ( bookmarkName != NULL ) && ( *bookmarkName != _T( "_PictureBullets" ) ) )
		{
			XMLTools::XMLElement<wchar_t> bookmarkElem( _T( "w:bookmarkEnd" ) );

			bookmarkElem.AppendAttribute( _T( "w:id" ), FormatUtils::IntToWideString( id ).c_str() );

			m_pXmlWriter->WriteString( bookmarkElem.GetXMLString().c_str() ); 

			return true;
		}

		return false;
	}

	// Checks if the CHPX is special
	bool DocumentMapping::isSpecial(CharacterPropertyExceptions* chpx)
	{
		/*
		for (list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); ++iter)
		{
		short value	=	FormatUtils::BytesToInt16 (iter->Arguments, 0, iter->argumentsSize);
		int c = 0;
		}
		*/

		for (std::list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); ++iter)
		{
			if ((sprmCPicLocation == iter->OpCode) || (sprmCHsp == iter->OpCode))	//	PICTURE
			{
				return true;
			}
			else if (	sprmCSymbol		== iter->OpCode ||
						sprmOldCSymbol	== iter->OpCode)	// SYMBOL
			{
				return true;
			}
			else if (	sprmOldCFSpec	== iter->OpCode ||
						sprmCFSpec		== iter->OpCode)	//	SPECIAL OBJECT
			{
				return ((0 != iter->Arguments[0]) ? true : false);
			}
		}

		return false;
	}

	Symbol DocumentMapping::getSymbol(const CharacterPropertyExceptions* chpx)
	{
		Symbol ret;

		for (std::list<SinglePropertyModifier>::const_iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); ++iter)
		{
			if (DocFileFormat::sprmCSymbol == iter->OpCode)
			{
				short fontIndex = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				
				short code = FormatUtils::BytesToInt16( iter->Arguments, 2, iter->argumentsSize );

				FontFamilyName* ffn = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( fontIndex ) );

				ret.FontName = ffn->xszFtn;
				ret.HexValue = FormatUtils::IntToFormattedWideString( code, _T( "%04x" ) );

				break;
			}
			else if (DocFileFormat::sprmOldCSymbol == iter->OpCode)
			{
				short fontIndex = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ;
				
				short code = FormatUtils::BytesToUChar( iter->Arguments, 2, iter->argumentsSize );

				FontFamilyName* ffn = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( fontIndex ) );

				ret.FontName = ffn->xszFtn;
				ret.HexValue = L"f0" + FormatUtils::IntToFormattedWideString( code, _T( "%02x" ) );

				break;
			}		
		}

		return ret;
	}

	/// Finds the SEPX that is valid for the given CP.
	SectionPropertyExceptions* DocumentMapping::findValidSepx (int cp)
	{
		SectionPropertyExceptions* ret = NULL;

		try
		{
			ret = m_document->AllSepx->operator [](cp);
			_lastValidSepx = ret;
		}
		///!!!TODO!!!
		catch ( ... )
		{
			//there is no SEPX at this position, 
			//so the previous SEPX is valid for this cp

			int lastKey = m_document->SectionPlex->CharacterPositions[1];

			for (std::map<int, SectionPropertyExceptions*>::iterator iter = m_document->AllSepx->begin(); iter != m_document->AllSepx->end(); ++iter)
			{
				if ( ( cp > lastKey ) && ( cp < iter->first ) )
				{
					ret = m_document->AllSepx->operator []( lastKey );

					break;
				}
				else
				{
					lastKey = iter->first;
				}
			}
		}

		return ret;
	}

	// Searches the given vector for the next FieldEnd character.
	
	int DocumentMapping::searchNextTextMark (std::vector<wchar_t>* chars, int initialCp, wchar_t mark)
	{
		int ret			=	initialCp;
		size_t count	=	chars->size();

		for (size_t i = initialCp; i < count; ++i)
		{
			if (chars->at(i) == mark)
			{
				ret = i;
				break;
			}
		}

		return ret;
	}
}
