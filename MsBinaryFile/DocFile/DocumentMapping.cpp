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
#include "DocumentMapping.h"
#include "ParagraphPropertiesMapping.h"
#include "CommentsMapping.h"
#include "TablePropertiesMapping.h"
#include "MainDocumentMapping.h"
#include "DrawingPrimitives.h"

#include "../Common/Base/TextMark.h"
#include "../Common/Base/FormatUtils.h"

#include <boost/algorithm/string.hpp>

namespace DocFileFormat
{
	DocumentMapping::DocumentMapping(ConversionContext* context, IMapping* caller) : _skipRuns(0), _lastValidPapx(NULL), _lastValidSepx(NULL),
		AbstractOpenXmlMapping( new XMLTools::CStringXmlWriter() ), _sectionNr(0), _footnoteNr(0),
		_endnoteNr(0), _commentNr(1), _caller(caller), _permissionNr(1)
	{
		m_document				=	NULL;
		m_context				=	context;
		m_bInternalXmlWriter	=	false;

		_writeWebHidden			=	false;
		_isTextBoxContent		=	false;

		m_context->_docx->_isSectionPageBreak =	0;
//--------------------------------------------
		_embeddedObject			=	false;
	}

	DocumentMapping::DocumentMapping(ConversionContext* context, XMLTools::CStringXmlWriter* writer, IMapping* caller):_skipRuns(0),  _lastValidPapx(NULL), _lastValidSepx(NULL), 
		AbstractOpenXmlMapping(writer), _sectionNr(0), _footnoteNr(0), _endnoteNr(0),
		_commentNr(1), _caller(caller)
	{
		m_document				=	NULL;
		m_context				=	context;
		m_bInternalXmlWriter	=	false;
	
		_writeWebHidden			=	false;
		_isTextBoxContent		=	false;
		_embeddedObject			=	false;

		_cacheListNum			= -1;
		
		m_context->_docx->_isSectionPageBreak =	0;
	}

	DocumentMapping::~DocumentMapping()
	{
		if (m_bInternalXmlWriter)
		{
			RELEASEOBJECT(m_pXmlWriter);
		}
	}

	// Looks into the section table to find out if this CP is the end & current  of a sections 
	int DocumentMapping::getCurrentSection(int cp)
	{
		//if cp is the last char of a section, the next section will start at cp +1
		size_t current = 0;

		if (m_document->SectionPlex->CharacterPositions.empty())
		{
			return cp;
		}

		for (std::vector<int>::iterator iter = m_document->SectionPlex->CharacterPositions.begin() + 1; iter != m_document->SectionPlex->CharacterPositions.end(); ++iter)
		{
			if (cp < *iter)
			{
				break;
			}
			current++;
		}
		if (current + 1 >= m_document->SectionPlex->CharacterPositions.size())
			current = m_document->SectionPlex->CharacterPositions.size() - 2;

		return m_document->SectionPlex->CharacterPositions[current + 1];
	}
	int DocumentMapping::getListNumCache(int fc, int fc_end)
	{
		if ( !m_document->ListPlex ) return -1;

		for (size_t i = 1; i < m_document->ListPlex->CharacterPositions.size(); i++)
		{
			if ((fc >= m_document->ListPlex->CharacterPositions[i-1]) && (fc_end <= m_document->ListPlex->CharacterPositions[i]))
			{
				ListNumCache* listNum = dynamic_cast<ListNumCache*> (m_document->ListPlex->Elements[i - 1]);

				return listNum->value;
			}
		}

		return -1;
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

	int DocumentMapping::writeParagraph(int cp, int cpEnd) 
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
				
                return writeParagraph(cp, (std::min)(cpEnd, cpParaEnd), false);
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

		if (fc < 0 || fcEnd < 0 || fc == fcEnd) 
			return -1;
		
		ParagraphPropertyExceptions* papx	=	findValidPapx(fc);

		// get all CHPX between these boundaries to determine the count of runs
		
		std::list<CharacterPropertyExceptions*>* chpxs	= m_document->GetCharacterPropertyExceptions(fc, fcEnd);
		std::vector<int>* chpxFcs						= m_document->GetFileCharacterPositions(fc, fcEnd);

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
		
        m_pXmlWriter->WriteNodeBegin(L"w:p", true);

		if (false == _paraId.empty())
		{
			m_pXmlWriter->WriteAttribute(L"w14:paraId", _paraId);
		}
		writeParagraphRsid(papx);

// ----------- check for section properties
		bool isBidi = false;
		SectionPropertyExceptions* currentSection = findValidSepx(getCurrentSection(cp));
		if (currentSection)
		{
			isBidi = currentSection->isBidi;
		}
//-----------------------------------------------------------		
		//_cacheListNum		= getListNumCache(fc, fcEnd);
		m_context->_docx->_isSectionPageBreak = 0;
		if (sectionEnd)
		{
			// this is the last paragraph of this section
			// write properties with section properties
			
			if (papx)
			{
				ParagraphPropertiesMapping oMapping(m_pXmlWriter, m_context, m_document, paraEndChpx, isBidi, findValidSepx(cpEnd), _sectionNr);
				papx->Convert(&oMapping);

				m_context->_docx->_isSectionPageBreak = oMapping.get_section_page_break();
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

				std::vector<wchar_t>* chpxChars = m_document->GetChars(fcChpxStart, fcChpxEnd, cp);

				std::vector<int> annot = searchAnnotation(chpxChars, cp);
				if (false == annot.empty())
				{
					std::list<std::vector<wchar_t>>* runs = splitCharList(chpxChars, &annot);
					if (runs) 
					{
						for (std::list<std::vector<wchar_t> >::iterator iter = runs->begin(); iter != runs->end(); ++iter)
						{
							if (writeAnnotations(cp))
							{
								cp = writeRun(&(*iter), *cpeIter, cp);
							}
						}

						RELEASEOBJECT(runs);
					}
				}
				else
				{
					std::vector<int> bookmarks = searchBookmarks(chpxChars, cp);

					if (false == bookmarks.empty())
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
						std::vector<int> permissions = searchPermission(chpxChars, cp);

						if (false == permissions.empty())
						{
							std::list<std::vector<wchar_t>>* runs = splitCharList(chpxChars, &permissions);
							if (runs)
							{
								for (std::list<std::vector<wchar_t> >::iterator iter = runs->begin(); iter != runs->end(); ++iter)
								{
									if (writePermissions(cp))
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
					}
				}

				RELEASEOBJECT(chpxChars);

				i++;
			}

			//end paragraph
            m_pXmlWriter->WriteNodeEnd(L"w:p");
		}
		else
		{
			//end paragraph
            m_pXmlWriter->WriteNodeEnd(L"w:p");
		}

		RELEASEOBJECT(chpxFcs);
		RELEASEOBJECT(chpxs);
		
		return cpEnd++;

		return (std::max)(cp, cpEnd); //ralph_scovile.doc
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
                    std::wstring id = FormatUtils::IntToFormattedWideString(FormatUtils::BytesToInt32(iter->Arguments, 0, iter->argumentsSize), L"%08X");
					m_context->AddRsid(id);
                    m_pXmlWriter->WriteAttribute(L"w:rsidP", id);

					break;
				}
			}    
		}

        m_pXmlWriter->WriteNodeEnd(L"", true, false);
	}

	// Writes a run with the given characters and CHPX

	int DocumentMapping::writeRun (std::vector<wchar_t>* chars, CharacterPropertyExceptions* chpx, int initialCp)
	{
		int cp			= initialCp;
		int result_cp	= cp + (int)chars->size();

		if ((_skipRuns <= 0) && (chars->size() > 0))
		{
			RevisionData rev = RevisionData(chpx);

			if (Deleted == rev.Type)
			{
				WideString* author = dynamic_cast<WideString*>(m_document->RevisionAuthorTable->operator[](rev.Isbt));

                m_pXmlWriter->WriteNodeBegin(L"w:del", true);
				m_pXmlWriter->WriteAttribute(L"w:author", FormatUtils::XmlEncode(*author));
				m_pXmlWriter->WriteAttribute(L"w:date", FormatUtils::XmlEncode(rev.Dttm.getString()));
				m_pXmlWriter->WriteNodeEnd(L"", true, false);
			}
			else if ( rev.Type == Inserted )
			{
				WideString* author = dynamic_cast<WideString*>(m_document->RevisionAuthorTable->operator[](rev.Isbt));

                m_pXmlWriter->WriteNodeBegin(L"w:ins", true);
                m_pXmlWriter->WriteAttribute(L"w:author", FormatUtils::XmlEncode(*author));
                m_pXmlWriter->WriteAttribute(L"w:date", FormatUtils::XmlEncode(rev.Dttm.getString()));
                m_pXmlWriter->WriteNodeEnd(L"", true, false);
			}

			//start run
            m_pXmlWriter->WriteNodeBegin(L"w:r", true);

			//append rsids
			if (0 != rev.Rsid)
			{
                std::wstring rsid = FormatUtils::IntToFormattedWideString(rev.Rsid, L"%08X");
                m_pXmlWriter->WriteAttribute(L"w:rsidR", rsid);
				m_context->AddRsid(rsid);
			}

			if (0 != rev.RsidDel)
			{
                std::wstring rsidDel = FormatUtils::IntToFormattedWideString(rev.RsidDel, L"%08X");
                m_pXmlWriter->WriteAttribute(L"w:rsidDel", rsidDel);
				m_context->AddRsid(rsidDel);
			}

			if (0 != rev.RsidProp)
			{
                std::wstring rsidProp = FormatUtils::IntToFormattedWideString(rev.RsidProp, L"%08X");
                m_pXmlWriter->WriteAttribute(L"w:rsidRPr", rsidProp);
				m_context->AddRsid(rsidProp);
			}

            m_pXmlWriter->WriteNodeEnd(L"", true, false);

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
				int new_result_cp = writeText(chars, cp, chpx, false);
				if (new_result_cp > result_cp)
					result_cp = new_result_cp;
			}

			//end run
            m_pXmlWriter->WriteNodeEnd(L"w:r");

			if (Deleted == rev.Type)
			{
                m_pXmlWriter->WriteNodeEnd(L"w:del");
			}
			else if (Inserted == rev.Type)
			{
                m_pXmlWriter->WriteNodeEnd(L"w:ins");
			}

			if (!_writeAfterRun.empty())
			{
				m_pXmlWriter->WriteString(_writeAfterRun);
				_writeAfterRun.clear();
			}
		}
		else
		{
			--_skipRuns;
		}

		return result_cp;
	}

	void DocumentMapping::writeField(const std::wstring& sFieldString, int cpFieldStart, int cpFieldEnd)
	{
		_fieldLevels.push_back(fieldLevels());
		
		std::vector<std::wstring> arField;
		boost::algorithm::split(arField, sFieldString, boost::algorithm::is_any_of(L"\\"), boost::algorithm::token_compress_on);
		
		std::wstring f;
		std::wstring EMBED		( L"EMBED" );
		std::wstring embed		( L"embed" );
		std::wstring LINK		( L"LINK" );
        std::wstring FORM		( L"FORM" );
        std::wstring Excel		( L"Excel" );
        std::wstring Word		( L"Word" );
        std::wstring opendocument(L"opendocument" );
        std::wstring Equation	( L"Equation" );
        std::wstring MERGEFORMAT( L"MERGEFORMAT" );
        std::wstring QUOTE		( L"QUOTE" );
        std::wstring chart		( L"Chart" );
        std::wstring PBrush		( L"PBrush" );
        std::wstring TOC		( L"TOC" );
        std::wstring HYPERLINK	( L"HYPERLINK" );
        std::wstring PAGEREF	( L"PAGEREF" );
        std::wstring PAGE		( L"PAGE" );
        std::wstring SHAPE		( L"SHAPE" );
		std::wstring NREF		( L"NREF");

		if (arField.empty() == false)
			f = arField[0];
		else
			f = sFieldString;

		bool bChart			= search( f.begin(), f.end(), chart.begin(),		chart.end())			!= f.end();
		bool bEMBED			= search( f.begin(), f.end(), EMBED.begin(),		EMBED.end())			!= f.end() ||
							  search( f.begin(), f.end(), embed.begin(),		embed.end())			!= f.end();
		bool bLINK			= search( f.begin(), f.end(), LINK.begin(),			LINK.end())				!= f.end();
		bool bOpendocument	= search( f.begin(), f.end(), opendocument.begin(), opendocument.end())		!= f.end();
		bool bFORM			= search( f.begin(), f.end(), FORM.begin(),			FORM.end())				!= f.end();
		bool bMERGEFORMAT	= search( f.begin(), f.end(), MERGEFORMAT.begin(),	MERGEFORMAT.end())		!= f.end();
		bool bExcel			= search( f.begin(), f.end(), Excel.begin(),		Excel.end())			!= f.end();
		bool bWord			= search( f.begin(), f.end(), Word.begin(),			Word.end())				!= f.end();
		bool bHYPERLINK		= search( f.begin(), f.end(), HYPERLINK.begin(),	HYPERLINK.end())		!= f.end();
		bool bQUOTE			= search( f.begin(), f.end(), QUOTE.begin(),		QUOTE.end())			!= f.end();
		bool bEquation		= search( f.begin(), f.end(), Equation.begin(),		Equation.end())			!= f.end();
		bool bPAGE			= search( f.begin(), f.end(), PAGE.begin(),			PAGE.end())				!= f.end();
		bool bTOC			= search( f.begin(), f.end(), TOC.begin(),			TOC.end())				!= f.end();
		bool bSHAPE			= search( f.begin(), f.end(), SHAPE.begin(),		SHAPE.end())			!= f.end();
		bool bNREF			= search( f.begin(), f.end(), NREF.begin(),			NREF.end())				!= f.end();

		bool bPAGEREF = false; 
		if (bHYPERLINK && arField.size() > 1)
		{
			std::wstring f1 = arField[1];
			bPAGEREF	= search( f1.begin(), f1.end(), PAGEREF.begin(), PAGEREF.end())	!= f1.end();
		}			

		if (bTOC)
			_bContentWrite = true;

		if ( bFORM )
		{
			std::wstring FORMTEXT		( L" FORMTEXT" );
			std::wstring FORMCHECKBOX	( L" FORMCHECKBOX" );
			std::wstring FORMDROPDOWN	( L" FORMDROPDOWN" );
			
			m_pXmlWriter->WriteNodeBegin( L"w:fldChar" , true );
            m_pXmlWriter->WriteAttribute( L"w:fldCharType" , L"begin" );
            m_pXmlWriter->WriteNodeEnd( L"", true, false );

			bool bFORMTEXT		= search( f.begin(), f.end(), FORMTEXT.begin(),		FORMTEXT.end())	!= f.end();
			bool bFORMCHECKBOX	= search( f.begin(), f.end(), FORMCHECKBOX.begin(),	FORMCHECKBOX.end())	!= f.end();
			bool bFORMDROPDOWN	= search( f.begin(), f.end(), FORMDROPDOWN.begin(),	FORMDROPDOWN.end())	!= f.end();

			if (bFORMTEXT || bFORMCHECKBOX || bFORMDROPDOWN)
			{
				int cpPic = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::Picture );
				if (cpPic < cpFieldEnd)
				{
					int fcPic = m_document->FindFileCharPos( cpPic );
					std::list<CharacterPropertyExceptions*>* chpxs	= m_document->GetCharacterPropertyExceptions(fcPic, fcPic + 1); 

					if (chpxs)
					{
						CharacterPropertyExceptions* chpxSep = chpxs->front();
						
						FormFieldData ffdata (2, chpxSep, m_document->DataStream, false);
						FormFieldDataMapping data_mapping(m_pXmlWriter, m_context, _caller);
						ffdata.Convert(&data_mapping);
						
						RELEASEOBJECT( chpxs );
					}
				}
			}

            m_pXmlWriter->WriteNodeEnd( L"w:fldChar" );

			_fieldLevels.back().bBegin = true;
		}
		else if ( ( bMERGEFORMAT || bExcel || bWord || bOpendocument )
				&& 
				( ( bEMBED || bLINK ) && bChart) )
		{
            m_pXmlWriter->WriteNodeBegin( L"w:fldChar", true );
                m_pXmlWriter->WriteAttribute( L"w:fldCharType", L"begin" );
            m_pXmlWriter->WriteNodeEnd( L"", true, false );

			int cpPic = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::Picture );

            m_pXmlWriter->WriteNodeEnd( L"w:fldChar" );

			_fieldLevels.back().bBegin = true;
		}
		else if (bHYPERLINK && bPAGEREF)
		{
			int cpFieldSep2 = cpFieldStart, cpFieldSep1 = cpFieldStart;
			std::vector<std::wstring> toc;

			if (arField.size() > 1)
				f = arField[1];

			if ( _bContentWrite )
			{
				m_pXmlWriter->WriteNodeBegin( L"w:fldChar", true );
					m_pXmlWriter->WriteAttribute( L"w:fldCharType", L"begin" );
				m_pXmlWriter->WriteNodeEnd( L"", true );

				_fieldLevels.back().bBegin = true;
			}
			else
			{
				for (size_t i = 1; i < arField.size(); i++)
				{
					std::wstring f1 = arField[1];
					int d = (int)f1.find(PAGEREF);

					if (d > 0)
					{
						_writeWebHidden = true;
						std::wstring _writeTocLink =f1.substr(d + 9);
						d = (int)_writeTocLink.find(L" ");
						_writeTocLink = _writeTocLink.substr(0, d);
						
						_writeAfterRun	=	std::wstring (L"<w:hyperlink w:anchor=\"");
						_writeAfterRun +=	XmlUtils::EncodeXmlString(_writeTocLink);
						_writeAfterRun +=	std::wstring (L"\" w:history=\"1\">");

						break;								
						//cp = cpFieldSep1;
					}
					//cpFieldSep1 = cpFieldSep2;
				}
				_skipRuns = 5; //with separator
			}
		}
		else if ( bEMBED ||	(bLINK && !bHYPERLINK)|| bQUOTE)						
		{
			int cpPic		=	searchNextTextMark(m_document->Text, cpFieldStart,	TextMark::Picture);
			int cpFieldSep	=	searchNextTextMark(m_document->Text, cpFieldStart,	TextMark::FieldSeparator);

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
				XMLTools::CStringXmlWriter	oleWriter;
				XMLTools::CStringXmlWriter	oleObjectWriter;
				
				VMLPictureMapping	oVmlMapper (m_context, &oleWriter, true, _caller);

				if (false == m_shapeIdOwner.empty())		//4571833.doc
					oVmlMapper.m_shapeId = m_shapeIdOwner;

				if (m_document->nWordVersion > 0)
				{							
					OleObject ole ( chpxObj, m_document);
					
                    oleWriter.WriteNodeBegin (L"w:object", true);
                        oleWriter.WriteAttribute( L"w:dxaOrig", FormatUtils::IntToWideString( ( ole.pictureDesciptor.dxaGoal + ole.pictureDesciptor.dxaOrigin ) ));
                        oleWriter.WriteAttribute( L"w:dyaOrig", FormatUtils::IntToWideString( ( ole.pictureDesciptor.dyaGoal + ole.pictureDesciptor.dyaOrigin ) ));
                    oleWriter.WriteNodeEnd( L"", true, false );

					ole.pictureDesciptor.Convert(&oVmlMapper);
					OleObjectMapping oleObjectMapping( &oleObjectWriter, m_context, &ole.pictureDesciptor, _caller, oVmlMapper.m_shapeId);
					
					ole.Convert( &oleObjectMapping );

					_lastOLEObject = oleObjectWriter.GetXmlString();
				}
				else
				{
					PictureDescriptor pic(chpxObj, m_document->DataStream, 0x7fffffff, m_document->nWordVersion);
					
                    oleWriter.WriteNodeBegin (L"w:object", true);
                        oleWriter.WriteAttribute( L"w:dxaOrig", FormatUtils::IntToWideString( ( pic.dxaGoal + pic.dxaOrigin ) ) );
                        oleWriter.WriteAttribute( L"w:dyaOrig", FormatUtils::IntToWideString( ( pic.dyaGoal + pic.dyaOrigin ) ) );
                    oleWriter.WriteNodeEnd( L"", true, false );
					
					pic.Convert(&oVmlMapper);
					RELEASEOBJECT(chpxs);

					if ( cpFieldSep < cpFieldEnd  && m_document->m_PieceTable)
					{
						int fcFieldSep = m_document->m_PieceTable->FileCharacterPositions->operator []( cpFieldSep );
						int fcFieldSep1 = m_document->FindFileCharPos( cpFieldSep );
						
						std::list<CharacterPropertyExceptions*>* chpxs = m_document->GetCharacterPropertyExceptions( fcFieldSep, ( fcFieldSep + 1 ) ); 
						CharacterPropertyExceptions* chpxSep = chpxs->front();
						
						OleObject ole ( chpxSep, m_document);
						OleObjectMapping oleObjectMapping( &oleObjectWriter, m_context, &pic, _caller, oVmlMapper.m_shapeId );
						
						if (oVmlMapper.m_isEmbedded)
						{
							ole.isEquation = oVmlMapper.m_isEquation;
							ole.isEmbedded = oVmlMapper.m_isEmbedded;
							ole.embeddedData = oVmlMapper.m_embeddedData;
						}
						ole.Convert( &oleObjectMapping );

						_lastOLEObject = oleObjectWriter.GetXmlString();
						
						RELEASEOBJECT( chpxs );
					}
				}
				oleWriter.WriteString( _lastOLEObject );
				
				if (false == m_bOleInPicture)
					_lastOLEObject.clear();

                oleWriter.WriteNodeEnd( L"w:object" );

				if (!oVmlMapper.m_isEmbedded && oVmlMapper.m_isEquation)
				{
					//нельзя в Run писать oMath
					//m_pXmlWriter->WriteString(oVmlMapper.m_equationXml);
					_writeAfterRun = oVmlMapper.m_equationXml;
				}
				else
				{
					m_pXmlWriter->WriteString(oleWriter.GetXmlString());
				}	
			}

			_skipRuns		= 3;
			_embeddedObject	= true;
		}					
		else
		{
            m_pXmlWriter->WriteNodeBegin( L"w:fldChar", true );
                m_pXmlWriter->WriteAttribute( L"w:fldCharType", L"begin" );
            m_pXmlWriter->WriteNodeEnd( L"", true );

			_fieldLevels.back().bBegin = true;
		}
	}

	// Writes the given text to the document
	int DocumentMapping::writeText(std::vector<wchar_t>* chars, int initialCp, CharacterPropertyExceptions* chpx, bool writeDeletedText)
	{
		int cp = initialCp;

		bool fSpec = isSpecial (chpx);

		//detect text type
		
        std::wstring textType = L"t";
        std::wstring text;

		if (writeDeletedText)
		{
            textType = std::wstring(L"delText");
		}
		else if ((!_fieldLevels.empty()) && (_fieldLevels.back().bBegin && !_fieldLevels.back().bSeparate))
		{
            textType = std::wstring(L"instrText");
		}

		//write text
		for (size_t i = 0; i < chars->size(); ++i)
		{
			wchar_t c = chars->at(i), code = c;

			if (TextMark::Tab == code)
			{
                writeTextElement(text, textType);

                text.clear();

                XMLTools::XMLElement elem(L"w:tab");

				m_pXmlWriter->WriteString(elem.GetXMLString());
			}
			else if (TextMark::HardLineBreak == code)
			{
                writeTextElement(text, textType);

                text.clear();

				if (textType == L"instrText")
				{
					m_pXmlWriter->WriteNodeEnd(L"w:r");
					m_pXmlWriter->WriteNodeBegin(L"w:r");
					CharacterPropertiesMapping* rPr = new CharacterPropertiesMapping(m_pXmlWriter, m_document, NULL, _lastValidPapx, false);
					if (rPr)
					{
						rPr->_webHidden = _writeWebHidden;
						
						chpx->Convert(rPr);
						RELEASEOBJECT(rPr);
					}
				}
				XMLTools::XMLElement elem(L"w:br");
				//СЗ в МРФ Техноград о предоставлении ТП 1 квартал 2019_MO_Q31.doc
				//Документ на бланке 2.doc
                elem.AppendAttribute(L"w:type", L"textWrapping");
                elem.AppendAttribute(L"w:clear", L"all");

				m_pXmlWriter->WriteString(elem.GetXMLString());
			}
			else if (TextMark::ParagraphEnd == code)
			{
				//do nothing
			}
			else if (TextMark::PageBreakOrSectionMark == code)
			{
				if (m_context->_docx->_isSectionPageBreak == 0/* || m_context->_docx->_isSectionPageBreak == 2*/)
				{
					m_context->_docx->_isSectionPageBreak = -1;

                    writeTextElement(text, textType);

                    text.clear();

                    XMLTools::XMLElement elem(L"w:br");
                    elem.AppendAttribute(L"w:type", L"page");

					m_pXmlWriter->WriteString(elem.GetXMLString());
				}
				m_context->_docx->_isSectionPageBreak = -1;
			}
			else if (TextMark::ColumnBreak == code)
			{
                writeTextElement(text, textType);

                text.clear();

                XMLTools::XMLElement elem(L"w:br");
                elem.AppendAttribute(L"w:type", L"column");

				m_pXmlWriter->WriteString(elem.GetXMLString());
			}
			else if (TextMark::FieldBeginMark == code)
			{
				_embeddedObject = false;
				
				int cpFieldStart = initialCp + i;
				int cpFieldEnd = searchNextTextMark( m_document->Text, cpFieldStart, TextMark::FieldEndMark );
				
				std::wstring sFieldString;
				if (cpFieldEnd < (int)m_document->Text->size())
					sFieldString = std::wstring( ( m_document->Text->begin() + cpFieldStart ), ( m_document->Text->begin() + cpFieldEnd + 1 ) );

				writeField(sFieldString, cpFieldStart, cpFieldEnd);

			}
			else if (TextMark::FieldSeparator == code)
			{
				if (!_fieldLevels.empty())
				{
					_fieldLevels.back().bSeparate = true;
                    
					XMLTools::XMLElement elem( L"w:fldChar" );
                    elem.AppendAttribute( L"w:fldCharType", L"separate" );

					m_pXmlWriter->WriteString( elem.GetXMLString() );
				}
				if (_embeddedObject)	_skipRuns += 2;
			}
			else if (TextMark::FieldEndMark == code)
			{
				if (!_fieldLevels.empty())
				{					
					if (!text.empty())
					{
						writeTextElement(text, textType);
						text.clear();
					}			
					if (_fieldLevels.back().bBegin)
					{					
						_fieldLevels.back().bEnd = true;
					
						XMLTools::XMLElement elem( L"w:fldChar" );
						elem.AppendAttribute( L"w:fldCharType", L"end" );

						m_pXmlWriter->WriteString( elem.GetXMLString());
					}

					_fieldLevels.pop_back();
				}
				if (_writeWebHidden)
				{
                    _writeAfterRun	=	std::wstring (L"</w:hyperlink>");
				}
				_writeWebHidden	= false;				
				_embeddedObject = false;

				if (_fieldLevels.empty())
					_bContentWrite = false;
			}
			else if ((TextMark::Symbol == code) && fSpec)
			{
				writeNotesReferences(cp);//for word95 & non-automatic notes

				Symbol s = getSymbol( chpx );

				//<w:sym w:font="Symbol" w:char="F062"/>

				if (false == s.HexValue.empty()) //09FluGuide.doc - поврежденный
				{
					m_pXmlWriter->WriteNodeBegin(L"w:sym", true);
					if (false == s.FontName.empty()) // ??? default
						m_pXmlWriter->WriteAttribute(L"w:font", FormatUtils::XmlEncode(s.FontName));
					m_pXmlWriter->WriteAttribute(L"w:char", FormatUtils::XmlEncode(s.HexValue));
					m_pXmlWriter->WriteNodeEnd(L"", true);
				}
			}
			else if ((TextMark::DrawnObject == code) && fSpec)
			{
				Spa* pSpa =	NULL;
				if (typeid(*this) == typeid(MainDocumentMapping))
				{
					pSpa = static_cast<Spa*>(m_document->OfficeDrawingPlex->GetStructByCP(cp));
				}
				else if ((typeid(*this) == typeid(HeaderMapping) ) || ( typeid(*this) == typeid(FooterMapping)))
				{
					int headerCp = ( cp - m_document->FIB->m_RgLw97.ccpText - m_document->FIB->m_RgLw97.ccpFtn );
					pSpa = static_cast<Spa*>(m_document->OfficeDrawingPlexHeader->GetStructByCP(headerCp));
				}

				bool bPicture = false;
				if (pSpa)
				{
					PictureDescriptor pictDiscr(chpx, m_document->WordDocumentStream, 0x7fffffff, m_document->nWordVersion);
					ShapeContainer* pShape = m_document->GetOfficeArt()->GetShapeContainer(pSpa->GetShapeID());

					if (pShape) ///*&& (false == pShape->isLastIdentify())
					{
						VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, pSpa, &pictDiscr,  _caller);
						
						std::wstring strNode = pShape->m_bOLE ? L"w:object" : L"w:pict";

						pShape->m_bOleInPicture = pShape->m_bOLE ? true : false;
						m_pXmlWriter->WriteNodeBegin (strNode);
							
						pShape->Convert(&oVmlWriter);
						m_pXmlWriter->WriteNodeEnd (strNode);

						bPicture = true;
					}
					
					if (!pSpa->primitives.empty())
					{
                        m_pXmlWriter->WriteNodeBegin (L"w:pict");
						VMLShapeMapping oVmlWriter (m_context, m_pXmlWriter, pSpa, &pictDiscr,  _caller);
						pSpa->primitives.Convert(&oVmlWriter);
                        m_pXmlWriter->WriteNodeEnd (L"w:pict");
						
						bPicture = true;
					}
				}

				if ((false == _fieldLevels.empty()) && (_fieldLevels.back().bSeparate))
					_fieldLevels.back().bResult = bPicture;
			}
			else if (TextMark::Picture == code && fSpec)
			{
				PictureDescriptor oPicture (chpx, m_document->nWordVersion > 0 ? m_document->WordDocumentStream : m_document->DataStream, 0x7fffffff, m_document->nWordVersion);

				bool isInline = _isTextBoxContent;

				if (oPicture.embeddedData && oPicture.embeddedDataSize > 0)
				{
                    m_pXmlWriter->WriteNodeBegin (L"w:pict");
					
					VMLPictureMapping oVmlMapper(m_context, m_pXmlWriter, false, _caller, isInline);
					oPicture.Convert (&oVmlMapper);
					
                    m_pXmlWriter->WriteNodeEnd	 (L"w:pict");
				}
				else if ((oPicture.mfp.mm > 98) && (NULL != oPicture.shapeContainer)/* && (false == oPicture.shapeContainer->isLastIdentify())*/)
				{
					bool bPicture	= true;
					bool m_bSkip	= false;

					if (oPicture.shapeContainer)
					{
						if (oPicture.shapeContainer->m_nShapeType != msosptPictureFrame)
							bPicture = false;//шаблон 1.doc картинка в колонтитуле

						m_bSkip = oPicture.shapeContainer->m_bSkip;
					}
					if (!m_bSkip)
					{
						bool bFormula	= false;
						XMLTools::CStringXmlWriter pictWriter;
						pictWriter.WriteNodeBegin (L"w:pict");

						if (bPicture)
						{
							VMLPictureMapping oVmlMapper(m_context, &pictWriter, false, _caller, isInline);
							oPicture.Convert (&oVmlMapper);
							
							if (oVmlMapper.m_isEmbedded)
							{
								OleObject ole ( chpx, m_document);
								OleObjectMapping oleObjectMapping( &pictWriter, m_context, &oPicture, _caller, oVmlMapper.m_shapeId );
								
								ole.isEquation = oVmlMapper.m_isEquation;
								ole.isEmbedded = oVmlMapper.m_isEmbedded;
								ole.embeddedData = oVmlMapper.m_embeddedData;
							
								ole.Convert( &oleObjectMapping );
							}
							else if (oVmlMapper.m_isEquation)
							{
								//нельзя в Run писать oMath
								//m_pXmlWriter->WriteString(oVmlMapper.m_equationXml);
								_writeAfterRun = oVmlMapper.m_equationXml;
								bFormula = true;
							}
							else if (oVmlMapper.m_isBlob)
							{
								_writeAfterRun = oVmlMapper.m_blobXml;
								bFormula = true;
							}
						}
						else
						{
							VMLShapeMapping oVmlMapper(m_context, &pictWriter, NULL, &oPicture,  _caller, isInline, false);
							oPicture.shapeContainer->Convert(&oVmlMapper);
						}
						
						pictWriter.WriteNodeEnd (L"w:pict");

						if (!bFormula)
						{
							m_pXmlWriter->WriteString(pictWriter.GetXmlString());
							
							if ((false == _fieldLevels.empty()) && (_fieldLevels.back().bSeparate && !_fieldLevels.back().bResult))	//ege15.doc
							{
								_fieldLevels.back().bResult = true;
							}//imrtemplate(endnotes).doc
						}
					}
				}                   
			}
			else if ((TextMark::AutoNumberedFootnoteReference == code) && fSpec)
			{
				writeNotesReferences(cp);
			}
			else if (TextMark::AnnotationReference == code)
			{
				if (typeid(*this) == typeid(CommentsMapping))
				{
                    m_pXmlWriter->WriteNodeBegin( L"w:annotationRef", true );
                    m_pXmlWriter->WriteNodeEnd( L"", true );
				}	
				else if (m_document->AnnotationsReferencePlex) 
				{
					m_pXmlWriter->WriteNodeBegin( L"w:commentReference", true );

					AnnotationReferenceDescriptor* atrdPre10 = dynamic_cast<AnnotationReferenceDescriptor*>( m_document->AnnotationsReferencePlex->GetStructByCP(cp));

					if (atrdPre10)
					{
						if (atrdPre10->m_CommentId < 0)
						{
							if (atrdPre10->m_BookmarkId < 0)
							{
								atrdPre10->m_CommentId = _commentNr++;
							}
							else
							{
								std::map<int, int>::iterator pFind = m_document->mapAnnotBookmarks.find(atrdPre10->m_BookmarkId);
								if (pFind == m_document->mapAnnotBookmarks.end())
								{
									atrdPre10->m_CommentId = _commentNr++;
									m_document->mapAnnotBookmarks.insert(std::make_pair(atrdPre10->m_BookmarkId, atrdPre10->m_CommentId));
								}
								else
								{
									atrdPre10->m_CommentId = pFind->second;
								}
							}
						}
						m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::IntToWideString(atrdPre10->m_CommentId));
					}
					
					m_pXmlWriter->WriteNodeEnd( L"", true );
				}
			}
			else if (!FormatUtils::IsControlSymbol(c) && ((int)c != 0xFFFF))
			{
                text += FormatUtils::GetXMLSymbol(c);
				
				//non-automatic notes
				if ((m_document->IndividualFootnotesPlex != NULL) && (m_document->IndividualFootnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpText)))
				{
					_writeNoteRef = L"<w:footnoteRef/>";
				}
				else if ((m_document->IndividualEndnotesPlex != NULL) &&
					(m_document->IndividualEndnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpAtn - m_document->FIB->m_RgLw97.ccpHdr - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpText)))
				{
					_writeNoteRef = L"<w:endnoteRef/>";
				}
				else 
					writeNotesReferences(cp);//for word95
			}

			cp++;
		}
		if (std::wstring::npos != text.find(L"EMBED"))
		{//если есть мааркер конца поля и маркер замещающей картинки и нету маркера начала
			//О реорганизации территориальных органов ПФР с 01.11.2018.doc
			int cpFieldStart	= initialCp;
			int cpPic			= searchNextTextMark(m_document->Text, cpFieldStart, TextMark::Picture);
			int cpFieldEnd		= searchNextTextMark( m_document->Text, cpFieldStart, TextMark::FieldEndMark );

			bool bStartField = _fieldLevels.empty() ? false : (_fieldLevels.back().bBegin && !_fieldLevels.back().bSeparate);

			if (cpFieldStart < cpPic && cpPic < cpFieldEnd && !bStartField)
			{
				writeField(text, cpFieldStart, cpFieldEnd);
				text.clear();
			}
		}
        if (!text.empty())
		{
            //bool preserve_space = (text.find(L"\x20")) != text.npos) ? true : false;

			writeTextStart(textType, true/*preserve_space*/);

            m_pXmlWriter->WriteString(text);

            writeTextEnd(textType);
		}
		return cp;
	}

	void DocumentMapping::writeTextElement(const std::wstring& text, const std::wstring& textType)
	{
		if ( !text.empty() )
		{
            bool preserve_space = true;//(text.find(L"\x20")) != text.npos) ? true : false;
            if (textType == L"instrText")
				preserve_space = false;
			
			writeTextStart( textType, preserve_space );

			m_pXmlWriter->WriteString( text);

			writeTextEnd( textType );
		}
	}

    void DocumentMapping::writeTextStart(const std::wstring& textType, bool preserve_space)
	{
        std::wstring str = ( L"w:"  + textType );

		m_pXmlWriter->WriteNodeBegin( str, true );
        if (preserve_space)
		{
            m_pXmlWriter->WriteAttribute( L"xml:space", L"preserve" );
        }
        m_pXmlWriter->WriteNodeEnd( L"", true, false );
	}

	void DocumentMapping::writeTextEnd(const std::wstring& textType)
	{
        std::wstring str = ( std::wstring( L"w:" ) + textType );

		m_pXmlWriter->WriteNodeEnd( str );
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
	std::vector<int> DocumentMapping::searchAnnotation(std::vector<wchar_t>* chars, int initialCp)
	{
		std::vector<int> ret;

		if (m_document->AnnotStartPlex->IsValid())
		{
			int cp = initialCp;

			size_t count = chars->size();

			for (size_t i = 0; i < count; ++i)
			{
				if ((m_document->AnnotStartPlex->IsCpExists(cp)) ||	(m_document->AnnotEndPlex->IsCpExists(cp)))
				{
					ret.push_back(i);
				}

				++cp;
			}
		}

		return ret;
	}
	std::vector<int> DocumentMapping::searchPermission(std::vector<wchar_t>* chars, int initialCp)
	{
		std::vector<int> ret;

		if (m_document->BookmarkProtStartPlex->IsValid())
		{
			int cp = initialCp;

			size_t count = chars->size();

			for (size_t i = 0; i < count; ++i)
			{
				if ((m_document->BookmarkProtStartPlex->IsCpExists(cp)) || (m_document->BookmarkProtEndPlex->IsCpExists(cp)))
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

		for (size_t i = 0; i < splitIndices->size(); ++i)
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
		
		TableInfo tai( papx, m_document->nWordVersion );

	//build the table grid
		std::vector<short> grid;
		buildTableGrid( cp, nestingLevel, grid);

		//find first row end
		int fcRowEnd = findRowEndFc( cp, nestingLevel );

		TablePropertyExceptions row1Tapx( findValidPapx( fcRowEnd ), m_document->DataStream, m_document->nWordVersion);

		//start table
        m_pXmlWriter->WriteNodeBegin( L"w:tbl" );

		//Convert it
		TablePropertiesMapping *tpMapping = new TablePropertiesMapping( m_pXmlWriter, m_document->Styles, &grid);

		row1Tapx.Convert( tpMapping );

		RELEASEOBJECT( tpMapping );

		//convert all rows
		if ( nestingLevel > 1 )
		{
			//It's an inner table
			//only convert the cells with the given nesting level
			while ( tai.iTap == nestingLevel )
			{
				cp = writeTableRow( cp, &grid, nestingLevel );
				//?fc = m_document->FindFileCharPos(cp );
				fc = m_document->m_PieceTable->FileCharacterPositions->operator []( cp );
				papx = findValidPapx( fc );
				tai = TableInfo( papx, m_document->nWordVersion );
			}
		}
		else
		{
			//It's a outer table (nesting level 1)
			//convert until the end of table is reached
			while ( tai.fInTable )
			{
				cp = writeTableRow( cp, &grid, nestingLevel );
				fc = m_document->FindFileCharPos( cp );

				papx = findValidPapx( fc );
				tai = TableInfo( papx, m_document->nWordVersion );
			}
		}

		//close w:tbl
        m_pXmlWriter->WriteNodeEnd( L"w:tbl" );

		return cp;
	}

	// Builds a list that contains the width of the several columns of the table.
	bool DocumentMapping::buildTableGrid(int initialCp, unsigned int nestingLevel, std::vector<short>& grid)
	{
		ParagraphPropertyExceptions* backup = _lastValidPapx;

		std::map<short, short> mapBoundaries;
		
		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx, m_document->nWordVersion );

		int fcRowEnd = findRowEndFc( cp, cp, nestingLevel );
		ParagraphPropertyExceptions* papx_prev = NULL;

		short max_boundary = -1;

		bool fEndNestingLevel = false;

		unsigned int iTap_current = 1;
		
		while ( tai.fInTable )
		{
			fEndNestingLevel = false;

			for ( std::list<SinglePropertyModifier>::iterator iter = papx->grpprl->begin(); !fEndNestingLevel && iter != papx->grpprl->end(); iter++ )
			{
				DWORD code = iter->OpCode;

				switch(iter->OpCode)
				{
					case sprmPFInnerTableCell:
					case sprmPFInnerTtp:
					{
						fEndNestingLevel = ( iter->Arguments[0] == 1 && (nestingLevel == iTap_current)) ? (true) : (false);
					}break;

					case sprmOldPFInTable:
					case sprmPItap:
					{
						iTap_current = FormatUtils::BytesToUInt32( iter->Arguments, 0, iter->argumentsSize );
					}break;
					default:
						break;
				}
			}
			if (nestingLevel == iTap_current)
			{ 
				bool bPresent = false; //118854.doc
				for ( std::list<SinglePropertyModifier>::reverse_iterator iter = papx->grpprl->rbegin(); !bPresent && iter != papx->grpprl->rend(); iter++ )
				{
					//find the tDef SPRM
					DWORD code = iter->OpCode;

					switch(iter->OpCode)
					{
						case sprmTDefTable:
						case sprmOldTDefTable:
						{
							unsigned char itcMac = iter->Arguments[0];

							short boundary1, boundary2;
							for (unsigned char i = 0; i < itcMac; i++)
							{
								boundary1 = FormatUtils::BytesToInt16( iter->Arguments + 1, i * 2 , iter->argumentsSize );
								boundary2 = FormatUtils::BytesToInt16( iter->Arguments + 1, ( i + 1 ) * 2, iter->argumentsSize );

								mapBoundaries.insert(std::make_pair(boundary1, 0));
								mapBoundaries.insert(std::make_pair(boundary2, 0));
							}
							bPresent = true;
						}break;
						default:
							break;
					}
				}
			}
			if ((nestingLevel != iTap_current || fEndNestingLevel) && !mapBoundaries.empty())
				break;
	//get the next papx
			papx = findValidPapx( fcRowEnd );
			tai = TableInfo( papx, m_document->nWordVersion );
			fcRowEnd = findRowEndFc( cp, cp, nestingLevel );

			if (papx_prev && papx_prev == papx )
				break;//file(12).doc
			papx_prev = papx;

		}

		if ( !mapBoundaries.empty() )
		{
			std::map<short, short>::iterator it = mapBoundaries.begin(); 
			std::map<short, short>::iterator it_next = it; it_next++;

			for ( ; it_next != mapBoundaries.end(); ++it_next, ++it)
			{
				int sz = it_next->first - it->first;
				if (sz > 2)
					grid.push_back(it_next->first - it->first);
			}
		}
		_lastValidPapx = backup;

		return true;
	}

	void DocumentMapping::AddBoundary(short boundary1, short boundary2, std::map<short, short> &boundaries)
	{
		if (boundary2 - boundary1 < 3)
			return;

		std::map<short, short>::iterator pFind = boundaries.find(boundary1);

		while(true)
		{
			if (pFind == boundaries.end())
			{
				boundaries.insert(std::make_pair(boundary1, boundary2 - boundary1));
				break;
			}
			else if (pFind->second != boundary2 - boundary1)
			{
				if (pFind->second > boundary2 - boundary1)
				{
					short new_size = boundary2 - boundary1;
					boundary1 = boundary2;
					boundary2 = pFind->second + pFind->first;
					pFind->second = new_size;
				}
				else
				{
					boundary1 = pFind->second + pFind->first;
				}
				pFind = boundaries.find(boundary1);
			}
			else
				break;
		}
	}

	// Finds the FC of the next row end mark.
	int DocumentMapping::findRowEndFc(int initialCp, int& rowEndCp, unsigned int nestingLevel )
	{
		int cp = initialCp;
		int fc = m_document->FindFileCharPos( cp );

		ParagraphPropertyExceptions* papx = findValidPapx( fc );
		TableInfo tai( papx, m_document->nWordVersion );

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
				tai = TableInfo( papx, m_document->nWordVersion );

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
				tai = TableInfo( papx, m_document->nWordVersion );
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
		TableInfo tai( papx, m_document->nWordVersion );

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
				tai = TableInfo( papx, m_document->nWordVersion );

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
				tai = TableInfo( papx, m_document->nWordVersion );
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
		TableInfo tai( papx, m_document->nWordVersion );

		//start w:tr
        m_pXmlWriter->WriteNodeBegin( L"w:tr" );

		//convert the properties
		int fcRowEnd = findRowEndFc( cp, nestingLevel );
		TablePropertyExceptions tapx( findValidPapx( fcRowEnd ), m_document->DataStream, m_document->nWordVersion);

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
				tai = TableInfo( papx, m_document->nWordVersion );
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
				tai = TableInfo( papx, m_document->nWordVersion );
			}
		}

		//end w:tr
        m_pXmlWriter->WriteNodeEnd( L"w:tr" );

		//skip the row end mark
		cp++;

		RELEASEOBJECT( chpxs );

		return cp;
	}

	/// Writes the table cell that starts at the given cp value and ends at the next cell end mark
	int DocumentMapping::writeTableCell(int initialCp, TablePropertyExceptions* tapx, std::vector<short>* grid, int& gridIndex, int cellIndex, unsigned int nestingLevel )  
	{
		int cp = initialCp;
		int cpCellEnd	= findCellEndCp( initialCp, nestingLevel );

		XMLTools::CStringXmlWriter writerTcPr;
		TableCellPropertiesMapping* tcpMapping = new TableCellPropertiesMapping( &writerTcPr, grid, gridIndex, cellIndex, nestingLevel );

		if ( tapx != NULL )
		{
			tapx->Convert( tcpMapping );
		}

		gridIndex = gridIndex + tcpMapping->GetGridSpan();

		bool bCoverCell = tcpMapping->IsCoverCell();

		RELEASEOBJECT( tcpMapping );

		if (bCoverCell)
		{
			return cpCellEnd;
		}

		//start w:tc
        m_pXmlWriter->WriteNodeBegin( L"w:tc" );
		m_pXmlWriter->WriteString(writerTcPr.GetXmlString());

		//write the paragraphs of the cell
		while ( cp < cpCellEnd )
		{
			//cp = writeParagraph(cp);
			int fc = m_document->FindFileCharPos( cp );

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx, m_document->nWordVersion );

			if ( tai.iTap > nestingLevel )
			{
				cp = writeTable( cp, tai.iTap );
			}
			else
			{
				//this PAPX is for a normal paragraph
				cp = writeParagraph( cp, 0x7fffffff );
			}
		}

		//end w:tc
        m_pXmlWriter->WriteNodeEnd( L"w:tc" );

		return cp;
	}

	int DocumentMapping::findCellEndCp(int initialCp, unsigned int nestingLevel)
	{
		int cpCellEnd = initialCp;

		if ( nestingLevel > 1 )
		{
			int fc = m_document->FindFileCharPos( initialCp );

			ParagraphPropertyExceptions* papx = findValidPapx( fc );
			TableInfo tai( papx, m_document->nWordVersion );

			while ( !tai.fInnerTableCell )
			{
				cpCellEnd++;

				fc = m_document->FindFileCharPos( cpCellEnd );
				
				papx = findValidPapx( fc );
				tai = TableInfo( papx, m_document->nWordVersion );
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
	bool DocumentMapping::writeNotesReferences(int cp)
	{
		if ((m_document->FootnoteReferenceCharactersPlex != NULL) && (m_document->FootnoteReferenceCharactersPlex->IsCpExists(cp)))
		{
			FootnoteDescriptor* desc = NULL;
			
			if (_footnoteNr < m_document->FootnoteReferenceCharactersPlex->Elements.size())
				desc = dynamic_cast<FootnoteDescriptor*>(m_document->FootnoteReferenceCharactersPlex->Elements[_footnoteNr]);
			
			if ((desc) && (false == desc->bUsed))
			{
				desc->bUsed = true;
				m_pXmlWriter->WriteNodeBegin( L"w:footnoteReference", true );
				//if (desc->aFtnIdx == 0)
				//{
				//	m_pXmlWriter->WriteAttribute( L"w:customMarkFollows", L"1");
				//}
				m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::IntToWideString(_footnoteNr++ ) );
				m_pXmlWriter->WriteNodeEnd( L"", true );
			}
		}
		else if ((m_document->IndividualFootnotesPlex != NULL) && (m_document->IndividualFootnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpText)))
		{
            m_pXmlWriter->WriteNodeBegin( L"w:footnoteRef", true );
            m_pXmlWriter->WriteNodeEnd( L"", true );
		}
		else if ((m_document->EndnoteReferenceCharactersPlex != NULL) && (m_document->EndnoteReferenceCharactersPlex->IsCpExists(cp)))
		{
			EndnoteDescriptor* desc = dynamic_cast<EndnoteDescriptor*>(m_document->EndnoteReferenceCharactersPlex->Elements[_endnoteNr]);
			if ((desc) && (false == desc->bUsed))
			{
				desc->bUsed = true;
				m_pXmlWriter->WriteNodeBegin( L"w:endnoteReference", true );
				//if (desc->aEndIdx == 0)
				//{
				//	m_pXmlWriter->WriteAttribute( L"w:customMarkFollows", L"1");
				//}
				m_pXmlWriter->WriteAttribute( L"w:id", FormatUtils::IntToWideString(_endnoteNr++ ));
				m_pXmlWriter->WriteNodeEnd( L"", true );
			}
		}
		else if ((m_document->IndividualEndnotesPlex != NULL) && 
			(m_document->IndividualEndnotesPlex->IsCpExists(cp - m_document->FIB->m_RgLw97.ccpAtn - m_document->FIB->m_RgLw97.ccpHdr - m_document->FIB->m_RgLw97.ccpFtn - m_document->FIB->m_RgLw97.ccpText)))
		{
            m_pXmlWriter->WriteNodeBegin( L"w:endnoteRef", true );
            m_pXmlWriter->WriteNodeEnd( L"", true );
		}
		else if (false == _writeNoteRef.empty())
		{
			m_pXmlWriter->WriteString(_writeNoteRef);
			_writeNoteRef.clear();
		}
		return true;
	}
	bool DocumentMapping::writeBookmarks(int cp)
	{
		bool result =	true;

		for (size_t b = 0; b < m_document->BookmarkStartEndCPs.size(); ++b)
		{
			if (m_document->BookmarkStartEndCPs[b].start == cp)
			{
				result = writeBookmarkStart(b);
			}

			if (m_document->BookmarkStartEndCPs[b].end == cp)
			{
				result = writeBookmarkEnd(b);  
			}
		}

		return result;
	}
	bool DocumentMapping::writeAnnotations(int cp)
	{
		bool result =	true;

		for (size_t i = 0; i < m_document->AnnotStartEndCPs.size(); i++)
		{
			if (m_document->AnnotStartEndCPs[i].start == cp)
			{
				int index = -1;
				std::map<int, int>::iterator pFind = m_document->mapAnnotBookmarks.find(m_document->AnnotStartEndCPs[i].bookmarkId);
				if (pFind == m_document->mapAnnotBookmarks.end())
				{
					index = _commentNr++;
					m_document->mapAnnotBookmarks.insert(std::make_pair(m_document->AnnotStartEndCPs[i].bookmarkId, index ));
				}
				else index = pFind->second;

				result = writeAnnotationStart(index);
			}

			if (m_document->AnnotStartEndCPs[i].end == cp)
			{
				int index = -1;
				std::map<int, int>::iterator pFind = m_document->mapAnnotBookmarks.find(m_document->AnnotStartEndCPs[i].bookmarkId);
				if (pFind == m_document->mapAnnotBookmarks.end())
				{
					index = _commentNr++;
					m_document->mapAnnotBookmarks.insert(std::make_pair(m_document->AnnotStartEndCPs[i].bookmarkId, index ));
				}
				else index = pFind->second;

				result = writeAnnotationEnd(index);  
			}
		}

		return result;
	}
	bool DocumentMapping::writePermissions(int cp)
	{
		bool result = true;

		for (size_t i = 0; i < m_document->BookmarkProtStartEndCPs.size(); i++)
		{
			if (m_document->BookmarkProtStartEndCPs[i].start == cp)
			{
				int index = -1;
				std::map<int, int>::iterator pFind = m_document->mapProtBookmarks.find(m_document->BookmarkProtStartEndCPs[i].bookmarkId);
				if (pFind == m_document->mapProtBookmarks.end())
				{
					index = _permissionNr++;
					m_document->mapProtBookmarks.insert(std::make_pair(m_document->BookmarkProtStartEndCPs[i].bookmarkId, index));
				}
				else index = pFind->second;

				result = writePermissionStart(index, i);
			}

			if (m_document->BookmarkProtStartEndCPs[i].end == cp)
			{
				int index = -1;
				std::map<int, int>::iterator pFind = m_document->mapProtBookmarks.find(m_document->BookmarkProtStartEndCPs[i].bookmarkId);
				if (pFind == m_document->mapProtBookmarks.end())
				{
					index = _permissionNr++;
					m_document->mapProtBookmarks.insert(std::make_pair(m_document->BookmarkProtStartEndCPs[i].bookmarkId, index));
				}
				else index = pFind->second;

				result = writePermissionEnd(index);
			}
		}

		return result;
	}
	bool DocumentMapping::writeBookmarkStart(short id)
	{
		WideString* bookmarkName = static_cast<WideString*>(m_document->BookmarkNames->operator [](id));

        if ((bookmarkName != NULL) && (*bookmarkName != L"_PictureBullets"))
		{
            XMLTools::XMLElement bookmarkElem(L"w:bookmarkStart");

            bookmarkElem.AppendAttribute(L"w:id", FormatUtils::IntToWideString(id));
            bookmarkElem.AppendAttribute(L"w:name", XmlUtils::EncodeXmlString(*bookmarkName));

			m_pXmlWriter->WriteString(bookmarkElem.GetXMLString());

			return true;
		}
		return false;
	}

	bool DocumentMapping::writeBookmarkEnd(short id)
	{
		WideString* bookmarkName = static_cast<WideString*>( m_document->BookmarkNames->operator [] ( id ) );

        if ( ( bookmarkName != NULL ) && ( *bookmarkName != L"_PictureBullets" ) )
		{
            XMLTools::XMLElement bookmarkElem( L"w:bookmarkEnd" );

            bookmarkElem.AppendAttribute( L"w:id", FormatUtils::IntToWideString( id ));

			m_pXmlWriter->WriteString( bookmarkElem.GetXMLString()); 

			return true;
		}

		return false;
	}
	bool DocumentMapping::writeAnnotationStart(short id)
	{
        XMLTools::XMLElement bookmarkElem(L"w:commentRangeStart");
        bookmarkElem.AppendAttribute(L"w:id", FormatUtils::IntToWideString(id));
		m_pXmlWriter->WriteString(bookmarkElem.GetXMLString());

		return true;
	}
	bool DocumentMapping::writeAnnotationEnd(short id)
	{
		XMLTools::XMLElement bookmarkElem(L"w:commentRangeEnd");
		bookmarkElem.AppendAttribute(L"w:id", FormatUtils::IntToWideString(id));
		m_pXmlWriter->WriteString(bookmarkElem.GetXMLString());

		return true;
	}
	bool DocumentMapping::writePermissionStart(short id, size_t index)
	{
		XMLTools::XMLElement bookmarkElem(L"w:permStart");
		bookmarkElem.AppendAttribute(L"w:id", FormatUtils::IntToWideString(id));

		ProtInfoBookmark *prot = static_cast<ProtInfoBookmark*>(m_document->BkmkProt->operator[](index));
		BookmarkFirst *prot_base = static_cast<BookmarkFirst*>(m_document->BookmarkProtStartPlex->GetStruct(index));

		if (prot_base && prot_base->fCol)
		{
			bookmarkElem.AppendAttribute(L"w:colFirst", FormatUtils::IntToWideString(prot_base->itcFirst));
			bookmarkElem.AppendAttribute(L"w:colLast", FormatUtils::IntToWideString(prot_base->itcLim - 1));
		}
		if (prot && (prot->uidSel == 0 || prot->uidSel > 0xfff0))
		{
			switch (prot->uidSel)
			{
			case 0xFFFF: bookmarkElem.AppendAttribute(L"w:edGrp", L"everyone");
			case 0xFFFE: bookmarkElem.AppendAttribute(L"w:edGrp", L"administrators");
			case 0xFFFD: bookmarkElem.AppendAttribute(L"w:edGrp", L"contributors");
			case 0xFFFC: bookmarkElem.AppendAttribute(L"w:edGrp", L"owners");
			case 0xFFFB: bookmarkElem.AppendAttribute(L"w:edGrp", L"editors");
			case 0xFFFA: bookmarkElem.AppendAttribute(L"w:edGrp", L"current");
			case 0x0000: bookmarkElem.AppendAttribute(L"w:edGrp", L"none");
			}
		}
		if (prot && (prot->uidSel > 0 && prot->uidSel < 0xfff0))
		{
			WideString* user = static_cast<WideString*>(m_document->BkmkProtUser->operator[](prot->uidSel - 1));

			if (user && false == user->empty())
				bookmarkElem.AppendAttribute(L"w:ed", *user);
		}
		m_pXmlWriter->WriteString(bookmarkElem.GetXMLString());

		return true;
	}
	bool DocumentMapping::writePermissionEnd(short id)
	{
		XMLTools::XMLElement bookmarkElem(L"w:permEnd");
		bookmarkElem.AppendAttribute(L"w:id", FormatUtils::IntToWideString(id));
		m_pXmlWriter->WriteString(bookmarkElem.GetXMLString());

		return true;
	}
	bool DocumentMapping::isSpecial(CharacterPropertyExceptions* chpx)
	{
		if (!chpx) return false;
		if (!chpx->grpprl)	 return false;

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
                ret.HexValue = FormatUtils::IntToFormattedWideString( code, L"%04X" );

				break;
			}
			else if (DocFileFormat::sprmOldCSymbol == iter->OpCode)
			{
				short fontIndex = FormatUtils::BytesToInt16( iter->Arguments, 0, iter->argumentsSize ) ;
				
				short code = FormatUtils::BytesToUChar( iter->Arguments, 2, iter->argumentsSize );

				FontFamilyName* ffn = static_cast<FontFamilyName*>( m_document->FontTable->operator [] ( fontIndex ) );

				ret.FontName = ffn->xszFtn;
                ret.HexValue = L"f0" + FormatUtils::IntToFormattedWideString( code, L"%02x" );//-123 - ShortToFormattedWideString

				break;
			}		
		}

		if (ret.HexValue.length() > 4)
		{
			ret.HexValue = ret.HexValue.substr(ret.HexValue.length() - 4, 4);
		}

		return ret;
	}

	/// Finds the SEPX that is valid for the given CP.
	SectionPropertyExceptions* DocumentMapping::findValidSepx (int cp)
	{
		SectionPropertyExceptions* ret = NULL;

		if (!m_document->AllSepx)
		{
			return ret;
		}

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
