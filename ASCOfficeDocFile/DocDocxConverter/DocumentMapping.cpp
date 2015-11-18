#include "DocumentMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CommentsMapping.h"
#include "TablePropertiesMapping.h"
#include "MainDocumentMapping.h"

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
	}

	DocumentMapping::DocumentMapping(ConversionContext* context, XmlUtils::CXmlWriter* writer, IMapping* caller):_skipRuns(0),  _lastValidPapx(NULL), _lastValidSepx(NULL), _writeInstrText(false),
		_fldCharCounter(0), AbstractOpenXmlMapping(writer), _sectionNr(0), _footnoteNr(0), _endnoteNr(0),
		_commentNr(0), _caller(caller)
	{
		m_document				=	NULL;
		m_context				=	context;
		m_bInternalXmlWriter	=	false;
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
	// Looks into the section table to find out if this CP is the end of a section  

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
	
	int DocumentMapping::writeParagraph (int initialCp, int cpEnd, bool sectionEnd)
	{
		int cp											=	initialCp;
		int fc											=	m_document->FindFileCharPos(cp); 
		int fcEnd										=	m_document->FindFileCharPos(cpEnd);

		ParagraphPropertyExceptions* papx				=	findValidPapx(fc);

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

		// check for section properties
		
		if (sectionEnd)
		{
			// this is the last paragraph of this section
			// write properties with section properties
			
			if (papx)
			{
				ParagraphPropertiesMapping oMapping(m_pXmlWriter, m_context, m_document, paraEndChpx, findValidSepx(cpEnd), _sectionNr);
				papx->Convert(&oMapping);
			}

			++_sectionNr;
		}
		else
		{
			// write properties
			
			if (papx)
			{
				ParagraphPropertiesMapping oMapping(m_pXmlWriter, m_context, m_document, paraEndChpx);
				papx->Convert(&oMapping);
			}
		}

		if ((chpxs != NULL) && (chpxFcs != NULL) && !chpxFcs->empty())
		{
			int i = 0;

			// write a runs for each CHPX

			for (std::list<CharacterPropertyExceptions*>::iterator cpeIter = chpxs->begin(); cpeIter != chpxs->end(); ++cpeIter)
			{
				//get the FC range for this run

				int fcChpxStart	=	chpxFcs->at(i);
				int fcChpxEnd	=	chpxFcs->at(i + 1);

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

				std::vector<wchar_t>* chpxChars = m_document->m_PieceTable->GetChars(fcChpxStart, fcChpxEnd, cp, m_document->WordDocumentStream);

#ifdef _DEBUG
				if (0)
				{
					//ATLTRACE(L"fcBegin : %d, fcEnd : %d\n", fcChpxStart, fcChpxEnd);
					for (size_t j = 0; j < chpxChars->size(); ++j)
					{
						//ATLTRACE (L"%c", chpxChars->operator [](j)); 
						//ATLTRACE(L"\n");
					}
				}
#endif

				//search for bookmarks in the chars
				std::vector<int> bookmarks = searchBookmarks(chpxChars, cp);

				//if there are bookmarks in this run, split the run into several runs
				if (bookmarks.size())
				{
					std::list<vector<wchar_t>>* runs = splitCharList(chpxChars, &bookmarks);
					if (runs) 
					{
						for (std::list<vector<wchar_t> >::iterator iter = runs->begin(); iter != runs->end(); ++iter)
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
				//if it's a inserted run
				m_pXmlWriter->WriteNodeBegin(_T("w:ins"), TRUE);
				m_pXmlWriter->WriteAttribute(_T("w:author"), dynamic_cast<WideString*>(m_document->RevisionAuthorTable->operator[](rev.Isbt))->c_str()); 
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

#ifdef _DEBUG
		if (0)
			if (chars)
			{ 
				//ATLTRACE (L"Run :");
				for (size_t i = 0; i < chars->size(); ++i)
				{
					//ATLTRACE (L"0x%x ", chars->operator [](i));
				}
				//ATLTRACE (L"\n");

				for ( list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); ++iter)
				{
					//if (sprmCPicLocation==iter->OpCode)
					//	ATLTRACE (L"SPRM : sprmCPicLocation, %d\n",	FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize));
					//else if (sprmCFSpec == iter->OpCode)
					//	ATLTRACE (L"SPRM :       sprmCFSpec, %d\n",	FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize));
					//else if (sprmCRsidText == iter->OpCode)
					//	ATLTRACE (L"SPRM :    sprmCRsidText, %d\n",	FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize));
					//else
					//	ATLTRACE (L"SPRM : %d, %d\n",	iter->OpCode,	FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize));
				}
				//ATLTRACE (L"\n");
			}
#endif
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
				wchar_t c = chars->at(i);

				if (TextMark::Tab == c)
				{
                    writeTextElement(text, textType);

                    text.clear();

					XMLTools::XMLElement<wchar_t> elem(_T("w:tab"));

					m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
				}
				else if (TextMark::HardLineBreak == c)
				{
                    writeTextElement(text, textType);

                    text.clear();

                    XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
					elem.AppendAttribute(_T("w:type"), _T("textWrapping"));
					elem.AppendAttribute(_T("w:clear"), _T("all"));

					m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
				}
				else if (TextMark::ParagraphEnd == c)
				{
					//do nothing
				}
				else if (TextMark::PageBreakOrSectionMark == c)
				{
					//write page break, section breaks are written by writeParagraph() method
					if (!isSectionEnd(c))
					{
                        writeTextElement(text, textType);

                        text.clear();

						XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
						elem.AppendAttribute(_T("w:type"), _T("page"));

						m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
					}
				}
				else if (TextMark::ColumnBreak == c)
				{
                    writeTextElement(text, textType);

                    text.clear();

					XMLTools::XMLElement<wchar_t> elem(_T("w:br"));
					elem.AppendAttribute(_T("w:type"), _T("column"));

					m_pXmlWriter->WriteString(elem.GetXMLString().c_str());
				}
				else if (TextMark::FieldBeginMark == c)
				{
					int cpFieldStart = initialCp + i;
					int cpFieldEnd = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::FieldEndMark );
					
					std::wstring f( ( m_document->Text->begin() + cpFieldStart ), ( m_document->Text->begin() + cpFieldEnd + 1 ) );
					std::wstring embed		( _T( " EMBED" ) );
					std::wstring link		( _T( " LINK" ) );
					std::wstring form		( _T( " FORM" ) );
					std::wstring excel		( _T( " Excel" ) );
					std::wstring word		( _T( " Word" ) );
					std::wstring equation	( _T( " Equation" ) ) ;
					std::wstring mergeformat( _T( " MERGEFORMAT" ) );

					if ( search( f.begin(), f.end(), form.begin(), form.end() ) != f.end() )
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

						this->_writeInstrText = true;

						this->_fldCharCounter++;
					}
					else if ((	search(f.begin(), f.end(), mergeformat.begin(), mergeformat.end()) != f.end())	||
							((	search(f.begin(), f.end(), excel.begin(), excel.end()) != f.end()				||
								search(f.begin(), f.end(), word.begin(), word.end()) != f.end()) 
							&& 
							(	search(f.begin(), f.end(), embed.begin(), embed.end()) != f.end() || 
								search( f.begin(), f.end(), link.begin(), link.end() ) != f.end())	))
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) );
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

						int cpPic = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::Picture );

						m_pXmlWriter->WriteNodeEnd( _T( "w:fldChar" ) );

						this->_writeInstrText = true;

						this->_fldCharCounter++;
					}
					else if (	search(f.begin(), f.end(), embed.begin(), embed.end()) != f.end()  
							||	search( f.begin(), f.end(), link.begin(), link.end() ) != f.end())						
					{
						int cpPic		=	searchNextTextMark(m_document->Text, cpFieldStart, TextMark::Picture);
						int cpFieldSep	=	searchNextTextMark(m_document->Text, cpFieldStart, TextMark::FieldSeparator);

						if (cpPic < cpFieldEnd)
						{
							int fcPic = m_document->m_PieceTable->FileCharacterPositions->operator []( cpPic );
							list<CharacterPropertyExceptions*>* chpxs	=	m_document->GetCharacterPropertyExceptions(fcPic, fcPic + 1); 
							
							CharacterPropertyExceptions* chpxPic		=	chpxs->front();

							PictureDescriptor pic(chpxPic, m_document->DataStream);

							RevisionData oData = RevisionData(chpxPic);

							/// <w:rPr>
							CharacterPropertiesMapping* rPr = new CharacterPropertiesMapping(m_pXmlWriter, m_document, &oData, _lastValidPapx, false);
							if(rPr)
							{
								chpxPic->Convert(rPr);
								RELEASEOBJECT(rPr);
							}

							m_pXmlWriter->WriteNodeBegin (_T( "w:object" ), TRUE);

							//append the origin attributes
							m_pXmlWriter->WriteAttribute( _T( "w:dxaOrig" ), FormatUtils::IntToWideString( ( pic.dxaGoal + pic.dxaOrigin ) ).c_str() ); 
							m_pXmlWriter->WriteAttribute( _T( "w:dyaOrig" ), FormatUtils::IntToWideString( ( pic.dyaGoal + pic.dyaOrigin ) ).c_str() ); 
							m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE, FALSE );

							VMLPictureMapping oVmlMapper (m_context, m_pXmlWriter, true, _caller);
							pic.Convert(&oVmlMapper);
							RELEASEOBJECT(chpxs);

							if ( cpFieldSep < cpFieldEnd )
							{
								int fcFieldSep = m_document->m_PieceTable->FileCharacterPositions->operator []( cpFieldSep );
								list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions( fcFieldSep, ( fcFieldSep + 1 ) ); 
								CharacterPropertyExceptions* chpxSep = chpxs->front();
								OleObject ole ( chpxSep, m_document->GetStorage() );
								OleObjectMapping oleObjectMapping( m_pXmlWriter, m_context, &pic, _caller, oVmlMapper.GetShapeId() );
								ole.Convert( &oleObjectMapping );
								RELEASEOBJECT( chpxs );
							}

							m_pXmlWriter->WriteNodeEnd( _T( "w:object" ) );	
						}

						this->_skipRuns = 5;
					}
					else
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:fldChar" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:fldCharType" ), _T( "begin" ) ); 
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );

						this->_writeInstrText = true;

						this->_fldCharCounter++;
					}
				}
				else if (TextMark::FieldSeparator == c)
				{
					if (_fldCharCounter > 0)
					{
						XMLTools::XMLElement<wchar_t> elem( _T( "w:fldChar" ) );
						elem.AppendAttribute( _T( "w:fldCharType" ), _T( "separate" ) );

						m_pXmlWriter->WriteString( elem.GetXMLString().c_str() );
					}
				}
				else if (TextMark::FieldEndMark == c)
				{
					if (_fldCharCounter > 0)
					{
						XMLTools::XMLElement<wchar_t> elem( _T( "w:fldChar" ) );
						elem.AppendAttribute( _T( "w:fldCharType" ), _T( "end" ) );

						m_pXmlWriter->WriteString( elem.GetXMLString().c_str() );

						this->_fldCharCounter--;
					}

					if ( this->_fldCharCounter == 0 )
					{
						this->_writeInstrText = false;
					}
				}
				else if ((TextMark::Symbol == c) && fSpec)
				{
					Symbol s = getSymbol( chpx );

					m_pXmlWriter->WriteNodeBegin(_T("w:sym"), TRUE);
					m_pXmlWriter->WriteAttribute(_T("w:font"), FormatUtils::XmlEncode(s.FontName).c_str()); 
					m_pXmlWriter->WriteAttribute(_T("w:char"), FormatUtils::XmlEncode(s.HexValue).c_str()); 
					m_pXmlWriter->WriteNodeEnd(_T(""), TRUE);
				}
				else if ((TextMark::DrawnObject == c) && fSpec)
				{
					Spa* pSpa			=	NULL;
					if (typeid(*this) == typeid(MainDocumentMapping))
					{
						pSpa			=	static_cast<Spa*>(m_document->OfficeDrawingPlex->GetStruct(cp));
					}
					else if ((typeid(*this) == typeid(HeaderMapping) ) || ( typeid(*this) == typeid(FooterMapping)))
					{
						int headerCp	=	( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn );
						pSpa			=	static_cast<Spa*>(m_document->OfficeDrawingPlexHeader->GetStruct(headerCp));
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
					}
				}
				else if ((TextMark::Picture == c) && fSpec)
				{
					PictureDescriptor oPicture (chpx, m_document->DataStream);

					if ((oPicture.mfp.mm > 98) && (NULL != oPicture.shapeContainer))
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
							VMLPictureMapping oVMLPicture(m_context, m_pXmlWriter, false, _caller);
							oPicture.Convert (&oVMLPicture);
						}else
						{
							VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, NULL, &oPicture,  _caller);
							oPicture.shapeContainer->Convert(&oVmlWriter);
						}
						
						m_pXmlWriter->WriteNodeEnd	 (_T("w:pict"));
					}                   
				}
				else if ((TextMark::AutoNumberedFootnoteReference == c) && fSpec)
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
				else if (TextMark::AnnotationReference == c)
				{
					if (typeid(*this) != typeid(CommentsMapping))
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:commentReference" ), TRUE );
						m_pXmlWriter->WriteAttribute( _T( "w:id" ), FormatUtils::IntToWideString( this->_commentNr ).c_str() ); 
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
					}
					else
					{
						m_pXmlWriter->WriteNodeBegin( _T( "w:annotationRef" ), TRUE );
						m_pXmlWriter->WriteNodeEnd( _T( "" ), TRUE );
					}

					this->_commentNr++;
				}
				else if (!FormatUtils::IsControlSymbol(c) && ((int)c != 0xFFFF))
				{
                    text += FormatUtils::GetXMLSymbol(c);
				}

				cp++;
			}

            if (!text.empty())
			{
                writeTextStart(textType);

                m_pXmlWriter->WriteString(text.c_str());

                writeTextEnd(textType);

#ifdef _DEBUG
				//OutputDebugStringW ( text.c_str() );
				//OutputDebugStringW ( _T("\n") );
#endif
			}
	}

	void DocumentMapping::writeTextElement(const std::wstring& text, const std::wstring& textType)
	{
		if ( !text.empty() )
		{
			writeTextStart( textType );

			m_pXmlWriter->WriteString( text.c_str() );

			writeTextEnd( textType );
		}
	}

    void DocumentMapping::writeTextStart(const std::wstring& textType)
	{
		std::wstring str = ( std::wstring( _T( "w:" ) ) + textType );

		m_pXmlWriter->WriteNodeBegin( str.c_str(), TRUE );
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
		vector<int> ret;

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
		ParagraphPropertyExceptions* ret = NULL;
		vector<int>* pVector = m_document->AllPapxVector;
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
				_lastValidPapx = ret;
			}
		}
		return ret;
	
		//map<int, ParagraphPropertyExceptions*>::const_iterator iterMin = m_document->AllPapx->begin();
		//map<int, ParagraphPropertyExceptions*>::const_iterator iterMax = ++m_document->AllPapx->begin();

		//while ( iterMax != m_document->AllPapx->end() )
		//{
		//  if ( ( fc >= iterMin->first ) && ( fc < iterMax->first ) )
		//  {
		//    ret = iterMin->second;
		//          _lastValidPapx = ret;

		//       break;
		//  }

		//  iterMin++;
		//  iterMax++;
		//}

		//if ( fc >= iterMin->first )
		//{
		//  ret = iterMin->second;
		//        _lastValidPapx = ret;
		//}

		//return ret;
	}

	std::list<vector<wchar_t> >* DocumentMapping::splitCharList(std::vector<wchar_t>* chars, std::vector<int>* splitIndices)
	{
		list<vector<wchar_t> >* ret = new list<vector<wchar_t> >();
		vector<wchar_t> wcharVector;

		int startIndex = 0;

		// add the parts

		for (unsigned int i = 0; i < splitIndices->size(); ++i)
		{
			int cch = splitIndices->at( i ) - startIndex;

			if ( cch > 0 )
			{
				wcharVector = vector<wchar_t>( cch );
				copy( chars->begin() + startIndex, chars->begin() + startIndex + cch, wcharVector.begin() );
				ret->push_back( wcharVector );
			}

			wcharVector.clear();
			startIndex += cch;
		}

		//add the last part
		wcharVector = vector<wchar_t>( chars->size() - startIndex );
		copy( chars->begin() + startIndex, chars->end(), wcharVector.begin() );
		ret->push_back( wcharVector );
		wcharVector.clear();

		return ret;
	}

	// Writes the table starts at the given cp value
	int DocumentMapping::writeTable(int initialCp, unsigned int nestingLevel)
	{
		int cp = initialCp;
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		//build the table grid
		vector<short>* grid = buildTableGrid( cp, nestingLevel );

		//find first row end
		int fcRowEnd = findRowEndFc( cp, nestingLevel );

		TablePropertyExceptions row1Tapx( findValidPapx( fcRowEnd ), m_document->DataStream );

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
				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
		
		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		int fcRowEnd = findRowEndFc( cp, cp, nestingLevel );

		while ( tai.fInTable )
		{
			//check all SPRMs of this TAPX
			for ( list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); iter != papx->grpprl->end(); iter++ )
			{
				//find the tDef SPRM
				if ( iter->OpCode == sprmTDefTable )
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
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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

				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
				while ( m_document->Text->at( cp ) != TextMark::CellOrRowMark )
				{
					cp++;
				}

				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
		int cp = initialCp;
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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

				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
				while ( m_document->Text->at( cp ) != TextMark::CellOrRowMark )
				{
					cp++;
				}

				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
				papx = findValidPapx( fc );
				tai = TableInfo( papx );
				cp++;
			}
		}

		return fc;
	}

	/// Writes the table row that starts at the given cp value and ends at the next row end mark
	int DocumentMapping::writeTableRow(int initialCp, vector<short>* grid, unsigned int nestingLevel)
	{
		int cp = initialCp;
		int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx );

		//start w:tr
		m_pXmlWriter->WriteNodeBegin( _T( "w:tr" ) );

		//convert the properties
		int fcRowEnd = findRowEndFc( cp, nestingLevel );
		TablePropertyExceptions tapx( findValidPapx( fcRowEnd ), m_document->DataStream );
		list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions( fcRowEnd, fcRowEnd + 1 );
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
				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
	int DocumentMapping::writeTableCell(int initialCp, TablePropertyExceptions* tapx, vector<short>* grid, int& gridIndex, int cellIndex, unsigned int nestingLevel )  
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
			int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
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
			int fc = m_document->m_PieceTable->FileCharacterPositions->operator []( initialCp );
			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx );

			while ( !tai.fInnerTableCell )
			{
				cpCellEnd++;

				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cpCellEnd );
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

		for (list<SinglePropertyModifier>::iterator iter = chpx->grpprl->begin(); iter != chpx->grpprl->end(); ++iter)
		{
			if ((sprmCPicLocation == iter->OpCode) || (sprmCHsp == iter->OpCode))	//	PICTURE
			{
				return true;
			}
			else if (sprmCSymbol == iter->OpCode)	// SYMBOL
			{
				return true;
			}
			else if (sprmCFSpec == iter->OpCode)	//	SPECIAL OBJECT
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
				//special symbol
				short fontIndex = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize );
				short code = FormatUtils::BytesToInt16( iter->Arguments, 2, iter->argumentsSize );

				FontFamilyName* ffn = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( fontIndex ) );

				ret.FontName = ffn->xszFtn;
				ret.HexValue = FormatUtils::IntToFormattedWideString( code, _T( "%04x" ) );

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
