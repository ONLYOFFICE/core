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

#include "DocFile.h"

#include <fstream>  


#define WRITE_STREAM_WORD(VAR)	hr=STREAMS::CSWordWriter::Instance()->Write(FIB_OFFSET::VAR,&VAR,sizeof(VAR))
#define WRITE_STREAM_TABLE(VAR,BUFFER) m_nBuffOk=0;hr=Write(m_pTableStream,fc##VAR,(unsigned char*)BUFFER,lcb##VAR,&m_nBuffOk);

#define TEXT_OFFSET_POSITION	2048

namespace ASCDocFileFormat
{
	CDocFile::CDocFile() : m_oFontTable(), listFormatInfo(), listFormatOverrideInfo()
	{
		ccpText								=	0;
		ccpFtn								=	0;
		ccpHdd								=	0;
		ccpAtn								=	0;
		ccpEdn								=	0;
		ccpTxbx								=	0;
		ccpHdrTxbx							=	0;

		m_pIStorage							=	NULL;

		m_pTableStream						=	NULL;
		m_pSummaryInformationStream			=	NULL;
		m_pDocumentSummaryInformationStream	=	NULL;
	}

	CDocFile::~CDocFile()
	{
		RELEASEOBJECT(m_pIStorage);
		RELEASEOBJECT(m_pTableStream);

		RELEASEOBJECT(m_pSummaryInformationStream);
		RELEASEOBJECT(m_pDocumentSummaryInformationStream);
	}
}

namespace ASCDocFileFormat
{
	void CDocFile::AddTextItem (const ITextItem& oItem)
	{
		textItems.push_back (TextItem(oItem));
	}

	void CDocFile::AddFootnote (const Footnote& oFootnote)
	{
		m_Footnotes.push_back (TextItem(oFootnote));
	}

	void CDocFile::AddEndnote (const Endnote& oEndnote)
	{
		endnotes.push_back (TextItem(oEndnote));
	}

	void CDocFile::AddSectionProperties (const SectionProperties& oSectionProperties)
	{
		sectionProperties.push_back(oSectionProperties);
	}

	CP CDocFile::OffsetToCP (unsigned int offset) const
	{
		return CP( ( offset - TEXT_OFFSET_POSITION ) / sizeof(wchar_t) );
	}
}

namespace ASCDocFileFormat
{	
	long CDocFile::WriteInformationBlock ()
	{
		long hr	= S_FALSE;

		STREAMS::CSWordWriter* pBin = STREAMS::CSWordWriter::Instance();
		if (NULL == pBin)
			return S_FALSE;

#define WRITE_FIELD(OFFSET,POINTER,SIZE) hr=pBin->Write(OFFSET,&POINTER,SIZE)	

#define SIZE_F	4096	// 2048		//	начальную область обнуляем 
		unsigned char NILL[SIZE_F];	memset(NILL, 0, SIZE_F);		WRITE_FIELD(0,NILL,SIZE_F);

		// FibBase

		Bool16 wIdent	=	0xA5EC;							WRITE_FIELD(FIB_OFFSET::wIdent,			wIdent,		sizeof(Bool16));
		Bool16 nFib		=	0x00C1;							WRITE_FIELD(FIB_OFFSET::nFib,			nFib,		sizeof(Bool16));
		Bool16 nUnused	=	0;								WRITE_FIELD(FIB_OFFSET::nFib + 2,		nUnused,	sizeof(Bool16));
		Bool16 lid		=	0x419;							WRITE_FIELD(FIB_OFFSET::lid,			lid,		sizeof(Bool16));	
		Bool16 pnNext	=	0;								WRITE_FIELD(FIB_OFFSET::pnNext,			pnNext,		sizeof(Bool16));

		ASCDocFormatUtils::BitSet oBits(1);

		oBits.SetBit(false,0);								//	fDot
		oBits.SetBit(false,1);								//	fGlsy
		oBits.SetBit(false,2);								//	fComplex
		oBits.SetBit(false,3);								//	fHasPic
		oBits.SetBits<Bool16>(0xF,4,4);						//	cQuickSaves

		ASCDocFormatUtils::BitSet oBits2(1);

		oBits2.SetBit(false,0);								//	fEncrypted
		oBits2.SetBit(true, 1);								//+	fWhichTblStm
		oBits2.SetBit(false,2);								//	fReadOnlyRecommended
		oBits2.SetBit(false,3);								//	fWriteReservation
		oBits2.SetBit(true, 4);								//+	fExtChar
		oBits2.SetBit(false,5);								//	fLoadOverride
		oBits2.SetBit(false,6);								//	fFarEast
		oBits2.SetBit(false,7);								//	fObfuscated

		Bool16 buf0		=	(Bool16)(*oBits.GetBytes());	WRITE_FIELD(FIB_OFFSET::pnNext + 2,		buf0,		sizeof(Bool16));	
		Bool16 buf2		=	(Bool16)(*oBits2.GetBytes());	WRITE_FIELD(FIB_OFFSET::pnNext + 3,		buf2,		sizeof(Bool16));	

		Bool16 nFibBack	=	0x00BF;							WRITE_FIELD(FIB_OFFSET::nFibBack,		nFibBack,	sizeof(Bool16));	
		Bool32 lKey		=	0;								WRITE_FIELD(FIB_OFFSET::lKey,			lKey,		sizeof(Bool32));	
		Bool16 envr		=	0;								WRITE_FIELD(FIB_OFFSET::envr,			envr,		sizeof(Bool16));	

		ASCDocFormatUtils::BitSet oBits3(1);

		oBits3.SetBit(false,0);								//	fMac
		oBits3.SetBit(false,1);								//	fEmptySpecial
		oBits3.SetBit(false,2);								//	fLoadOverridePage
		oBits3.SetBit(false,3);								//	-	fFutureSavedUndo	(reserved)
		oBits3.SetBit(true, 4);								//	-	fWord97Saved		(reserved)
		oBits3.SetBit(false,5);								//	fSpare0
		oBits3.SetBit(false,6);								//	fSpare0
		oBits3.SetBit(false,7);								//	fSpare0

		Bool8 buf3		=	(Bool8)(*oBits3.GetBytes());	WRITE_FIELD(FIB_OFFSET::envr + 1,		buf3,		sizeof(Bool8));	

		// Fib

		Bool16 csw		=	0x000E;							WRITE_FIELD(FIB_OFFSET::csw,			csw,		sizeof(Bool16));	

		// FibRgW97 (fibRgW)	-	28 bytes
		//unsigned char fibRgW[88];	memset(fibRgW, 0x0, 28);		WRITE_FIELD(FIB_OFFSET::csw + 2,		fibRgW,					28);	

		// Fib

		Bool16 cslw		=	0x0016;							WRITE_FIELD(FIB_OFFSET::cslw,			cslw,		sizeof(Bool16));	

		//  FibRgLw97 (fibRgLw)	-	88 bytes
		//unsigned char fibRgLw[88];	memset(fibRgLw, 0x0, 88);		WRITE_FIELD(FIB_OFFSET::cbMac,			fibRgLw,				88);	

		//Bool32 cbMac		=	0;							WRITE_FIELD(FIB_OFFSET::cbMac,			cbMac,		sizeof(Bool32));	
		//Bool32 ccpText	=	0;							WRITE_FIELD(FIB_OFFSET::ccpText,		ccpText,	sizeof(Bool32));	
		//Bool32 ccpFtn		=	0;							WRITE_FIELD(FIB_OFFSET::ccpFtn,			ccpFtn,		sizeof(Bool32));	
		//Bool32 ccpHdd		=	0;							WRITE_FIELD(FIB_OFFSET::ccpHdd,			ccpHdd,		sizeof(Bool32));	
		//Bool32 ccpAtn		=	0;							WRITE_FIELD(FIB_OFFSET::ccpAtn,			ccpAtn,		sizeof(Bool32));	
		//Bool32 ccpEdn		=	0;							WRITE_FIELD(FIB_OFFSET::ccpEdn,			ccpEdn,		sizeof(Bool32));	
		//Bool32 ccpTxbx	=	0;							WRITE_FIELD(FIB_OFFSET::ccpTxbx,		ccpTxbx,	sizeof(Bool32));	
		//Bool32 ccpHdrTxbx	=	0;							WRITE_FIELD(FIB_OFFSET::ccpHdrTxbx,		ccpHdrTxbx,	sizeof(Bool32));	

		//	0x00B7 - 0x00C1 - FibRgFcLcb97	-	ошибка в спецификации, перепутаны значения по умолчанию

		Bool16 cbRgFcLcb =	0x00B7;							WRITE_FIELD(FIB_OFFSET::cbRgFcLcb,		cbRgFcLcb,	sizeof(Bool16));	

		//	FibRgFcLcb97	-	186 * 4 bytes

		//	unsigned char FibRgFcLcb97[186*4];	
		//	memset(FibRgFcLcb97, 0x0, 186*4);				WRITE_FIELD(FIB_OFFSET::cbRgFcLcb + 2,	FibRgFcLcb97,		186*4);	

		//	Bool16 cswNew		=	0;						WRITE_FIELD(FIB_OFFSET::cbRgFcLcb + 2 + 186*4,	cswNew,	sizeof(Bool16));	

		// TODO : fill default values 

		return S_OK;
	}

	long CDocFile::WriteDocument()
	{
		long hr	= S_FALSE;

		STREAMS::CSWordWriter* pBin	= STREAMS::CSWordWriter::Instance();
		if (NULL == pBin)
			return hr;

		m_arChpx.clear();
		m_arRunsOffsets.clear();
		m_arParagraphsOffSets.clear();
		m_arPapxInFkp.clear();

		unsigned long writtenSize	=	0;
		unsigned int textPosition	=	TEXT_OFFSET_POSITION;

		writtenSize = 0;

		ccpText	= GetFibRgLw97_ccpText();	
		if (ccpText)
		{
			m_oartStorage			=	COArtStorage::Instance();

			vector<CP> AllFootnotesReferences;
			vector<CP> AllEndnotesReferences;

			WriteMainDocument(&textPosition, &AllFootnotesReferences, &AllEndnotesReferences);

			if ((!AllFootnotesReferences.empty()) && (!m_Footnotes.empty()))
			{
				WriteFootnoteDocument(&textPosition, &AllFootnotesReferences);
			}

			WriteHeadersAndFootersDocument (&textPosition);

			if ((!AllEndnotesReferences.empty()) && (!endnotes.empty()))
			{
				WriteEndnoteDocument(&textPosition, &AllEndnotesReferences);
			}

			//if ((!m_Footnotes.empty()) || (!endnotes.empty()) || (ccpHdd > 0))
			//{
			//	hr = Write (pBin->Get(), textPosition, std::wstring(&TextMark::ParagraphEnd).c_str(), (std::wstring( &TextMark::ParagraphEnd ).size() * sizeof(wchar_t)), &writtenSize);
			//	textPosition += writtenSize;
			//}

			CalculateOffTbRefs (textPosition);
			textPosition += WriteStrTbRefs (textPosition);
		
			WriteFibMainTBRefs();
			WriteFibHeadTBRefs();

			m_arParagraphsOffSets.push_back (textPosition);
			m_arRunsOffsets.push_back (textPosition);

			// Write fcMin in FIB
			unsigned int fcMin = TEXT_OFFSET_POSITION;
			hr = pBin->Write (24, &fcMin, sizeof(fcMin));

			// Write fcMac in FIB
			unsigned int fcMac = textPosition;
			hr = pBin->Write (28, &fcMac, sizeof(fcMac));

			//write Clx
			vector<unsigned int> aCP;
			vector<Pcd> aPcd;

			aCP.push_back(0);

			unsigned int lastACP = (ccpText + ccpFtn + ccpHdd + ccpAtn + ccpEdn + ccpTxbx + ccpHdrTxbx);

			if ( ( ccpFtn != 0 ) || ( ccpHdd != 0 ) || ( ccpAtn != 0 ) || ( ccpEdn != 0 ) )
			{
				lastACP++;
			}

			aCP.push_back( lastACP );

			aPcd.push_back( Pcd( false, TEXT_OFFSET_POSITION, 0 ) );

			Clx clx( NULL, Pcdt( aCP, aPcd ) );

			unsigned int fcClx		=	m_pTableStream->size();
			unsigned int lcbClx		=	clx.Size();

			WRITE_STREAM_WORD(fcClx);	// 418
			WRITE_STREAM_WORD(lcbClx);	// 422

			clx.WriteToStream( m_pTableStream, fcClx );

			const unsigned int PapxFkpOffset	=	( ( ( textPosition ) / 512 ) + 1 );
			unsigned long ChpxFkpOffset			=	( PapxFkpOffset + 1 );

			const unsigned int PlcBteOffset		=	m_pTableStream->size();

			// Write paragraphs properties
			vector<PapxFkp> allPapxFkps			=	PapxFkp::GetAllPapxFkps(m_arParagraphsOffSets, m_arPapxInFkp);

			vector<unsigned int> _aFC;
			vector<unsigned int> aPnBtePapx;

			_aFC.push_back( TEXT_OFFSET_POSITION );

			for (unsigned int i = 0; i < allPapxFkps.size(); ++i)
			{
				allPapxFkps[i].WriteToStream (pBin->Get(), ( ( PapxFkpOffset * 512 ) + ( 512 * i ) ));

				textPosition = ( ( PapxFkpOffset * 512 ) + ( 512 * i ) ) + allPapxFkps[i].GetEndOffset();

				_aFC.push_back( allPapxFkps[i].GetEndOffset() );
				aPnBtePapx.push_back( PapxFkpOffset + i );
				ChpxFkpOffset = ( PapxFkpOffset + 1 + i );
			}

			// Write lcbPlcfBtePapx
			unsigned int lcbPlcfBtePapx = ( ( ( _aFC.size() - 1 ) * 8 ) + 4 );
			hr	=	pBin->Write (262, &lcbPlcfBtePapx, sizeof(lcbPlcfBtePapx) );

			// Write fcPlcfBtePapx
			unsigned int fcPlcfBtePapx = PlcBteOffset;
			hr	=	pBin->Write (258, &fcPlcfBtePapx, sizeof(fcPlcfBtePapx) );

			PlcBtePapx plcBtePapx( _aFC, aPnBtePapx );
			plcBtePapx.WriteToStream( m_pTableStream, PlcBteOffset );

			//write runs properties
			vector<ChpxFkp> allChpxFkps = ChpxFkp::GetAllChpxFkps(m_arRunsOffsets, m_arChpx);

			_aFC.clear();
			vector<unsigned int> aPnBteChpx;

			_aFC.push_back(TEXT_OFFSET_POSITION);

			for (unsigned int i = 0; i < allChpxFkps.size(); ++i)
			{
				allChpxFkps[i].WriteToStream( pBin->Get(), ( ( ChpxFkpOffset * 512 ) + ( 512 * i ) ) );

				textPosition = ( ( ChpxFkpOffset * 512 ) + ( 512 * i ) ) + allChpxFkps[i].GetEndOffset();

				_aFC.push_back( allChpxFkps[i].GetEndOffset() );
				aPnBteChpx.push_back( ChpxFkpOffset + i );
			}

			unsigned int lcbPlcfBteChpx = ( ( ( _aFC.size() - 1 ) * 8 ) + 4 );
			hr	=	pBin->Write (254, &lcbPlcfBteChpx, sizeof(lcbPlcfBteChpx) );

			unsigned int fcPlcfBteChpx = ( PlcBteOffset + lcbPlcfBtePapx );
			hr	=	pBin->Write (250, &fcPlcfBteChpx, sizeof(fcPlcfBteChpx) );

			PlcBteChpx plcBteChpx(_aFC, aPnBteChpx);
			plcBteChpx.WriteToStream(m_pTableStream, (PlcBteOffset + lcbPlcfBtePapx));

			pBin->SetPosition (textPosition);

			// дописываем в поток все изображения

			if (m_oartStorage)
				m_oartStorage->WriteBlips ();
		}

		return hr;
	}

	long CDocFile::WriteMainDocument (unsigned int* _textPosition, vector<CP>* _AllFootnotesReferences, vector<CP>* _AllEndnotesReferences)
	{
		long hr	= S_FALSE;

		if ( ( _textPosition != NULL ) && ( _AllFootnotesReferences != NULL ) && ( _AllEndnotesReferences != NULL ))
		{ 
			unsigned long writtenSize = 0;

			WriteFibRgLw97 ();

			map<CP, Fld> cpFldMap;
			vector<pair<CP, wstring>> bookmarksStartsCPsWithIDs;
			vector<pair<CP, wstring>> bookmarksEndsCPsWithIDs;
			vector<wstring> bookmarksNames;

			//write all text to the document
			for (list<TextItem>::iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			{
				vector<unsigned int> allTextItemOffsets;
				vector<PapxInFkp> allTextItemProperties = (*iter)->GetAllParagraphsProperties( &allTextItemOffsets );

				for (unsigned int i = 0; i < allTextItemProperties.size(); ++i)
				{
					m_arPapxInFkp.push_back(allTextItemProperties[i]);
					m_arParagraphsOffSets.push_back(*_textPosition + allTextItemOffsets[i]); 
				}

				vector<unsigned int> allTextItemRunsOffsets;
				vector<Chpx> allTextItemChpxs = (*iter)->GetAllRunProperties( &allTextItemRunsOffsets );

				for (unsigned int i = 0; i < allTextItemChpxs.size(); ++i)
				{
					m_arChpx.push_back(allTextItemChpxs[i]); 
					m_arRunsOffsets.push_back(*_textPosition + allTextItemRunsOffsets[i]);
				}

				vector<ParagraphItem> allTextItemFootnotes = iter->GetAllRunItemsByType<FootnoteReference>();

				for (vector<ParagraphItem>::const_iterator footnotesIter = allTextItemFootnotes.begin(); footnotesIter != allTextItemFootnotes.end(); ++footnotesIter)
				{
					_AllFootnotesReferences->push_back(OffsetToCP( *_textPosition + footnotesIter->GetOffset()));
				}

				vector<ParagraphItem> allTextItemEndnotes = iter->GetAllRunItemsByType<EndnoteReference>();

				for (vector<ParagraphItem>::const_iterator endnotesIter = allTextItemEndnotes.begin(); endnotesIter != allTextItemEndnotes.end(); ++endnotesIter)
				{
					_AllEndnotesReferences->push_back(OffsetToCP( *_textPosition + endnotesIter->GetOffset() ) );
				}

				vector<ParagraphItem> allTextItemBookmarksStarts = iter->GetAllParagraphItemsByType<BookmarkStart>();

				for ( vector<ParagraphItem>::const_iterator bookmarksStartsIter = allTextItemBookmarksStarts.begin(); bookmarksStartsIter != allTextItemBookmarksStarts.end(); bookmarksStartsIter++ )
				{
					if ( bookmarksStartsIter->is<BookmarkStart>() )
					{
						const BookmarkStart& bookmarkStart = bookmarksStartsIter->as<BookmarkStart>();

						bookmarksStartsCPsWithIDs.push_back( make_pair(OffsetToCP( *_textPosition + bookmarksStartsIter->GetOffset() ), bookmarkStart.GetID() ) );
						bookmarksNames.push_back( bookmarkStart.GetName() );
					}
				}

				vector<ParagraphItem> allTextItemBookmarksEnds = iter->GetAllParagraphItemsByType<BookmarkEnd>();

				for ( vector<ParagraphItem>::const_iterator bookmarksEndsIter = allTextItemBookmarksEnds.begin(); bookmarksEndsIter != allTextItemBookmarksEnds.end(); bookmarksEndsIter++ )
				{
					if ( bookmarksEndsIter->is<BookmarkEnd>() )
					{
						const BookmarkEnd& bookmarkEnd = bookmarksEndsIter->as<BookmarkEnd>();

						bookmarksEndsCPsWithIDs.push_back( make_pair(OffsetToCP( *_textPosition + bookmarksEndsIter->GetOffset() ), bookmarkEnd.GetID() ) );
					}
				}

				AddFldCharsData		(*_textPosition, (*iter), &cpFldMap);
				AddHyperlinksData	(*_textPosition, (*iter), &cpFldMap);
				AddInlineShapesData	(*_textPosition, (*iter), &cpFldMap);

				std::wstring strText = (*iter)->GetAllText();

				hr = Write(STREAMS::CSWordWriter::Instance()->Get(), *_textPosition, strText.c_str(), (sizeof(wchar_t) * strText.size()), &writtenSize);
				*_textPosition += writtenSize;
			}

			hr = WriteMainDocumentFields(cpFldMap);
			hr = WriteBookmarks(bookmarksStartsCPsWithIDs, bookmarksEndsCPsWithIDs, bookmarksNames);
		}

		return hr;
	}

	void CDocFile::AddFldCharsData (unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap)
	{
		if(cpFldMap)
		{
			vector<ParagraphItem>& arrFldChars	=	oItem.GetAllRunItemsByType<FldChar>();
			if (arrFldChars.empty())
				return;

			for (vector<ParagraphItem>::const_iterator fldCharsIter = arrFldChars.begin(); fldCharsIter != arrFldChars.end(); ++fldCharsIter)
			{
				if (fldCharsIter->is<Run>())
				{
					const Run& run = fldCharsIter->as<Run>();

					for (list<RunItem>::const_iterator runIter = run.begin(); runIter != run.end(); ++runIter)
					{
						if (runIter->is<FldChar>())
						{
							const FldChar& fldChar = runIter->as<FldChar>();

							cpFldMap->insert( make_pair( OffsetToCP( nTextPos + fldCharsIter->GetOffset() ), fldChar.GetField() ) );
						}
					}
				}
			}
		}
	}

	void CDocFile::AddHyperlinksData (unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap)
	{
		if (cpFldMap)
		{
			const vector<ParagraphItem>& oHyperlinks	=	oItem.GetAllParagraphItemsByType<Hyperlink>();
			if (oHyperlinks.empty())
				return;

			for (size_t j = 0; j < oHyperlinks.size(); ++j)
			{
				if (oHyperlinks[j].is<Hyperlink>())
				{
					Hyperlink oHyperlink			=	oHyperlinks[j].as<Hyperlink>();

					vector<CP> hyperlinkACP			=	oHyperlink.GetFieldCharactersPositions();
					vector<Fld> hyperlinkAFld		=	oHyperlink.GetFieldCharactersProperties();

					for (size_t i = 0; i < hyperlinkACP.size(); ++i)
					{
						cpFldMap->insert ( make_pair( CP(OffsetToCP(nTextPos + oHyperlinks[j].GetOffset()) + hyperlinkACP[i] ), hyperlinkAFld[i]));  
					}
				}
			}  
		}
	}  	  


	void CDocFile::AddInlineShapesData (unsigned int nTextPos, const TextItem& oItem, map<CP, Fld>* cpFldMap)
	{
		if (cpFldMap)
		{
			const vector<ParagraphItem>& oInlineShapes	=	oItem.GetAllParagraphItemsByType<InlineShape>();
			if (oInlineShapes.empty())
				return;

			for (size_t j = 0; j < oInlineShapes.size(); ++j)
			{
				if (oInlineShapes[j].is<InlineShape>())
				{
					InlineShape oShape			=	oInlineShapes[j].as<InlineShape>();

					vector<CP> oShapeACP		=	oShape.GetFieldCharactersPositions();
					vector<Fld> oShapeAFld		=	oShape.GetFieldCharactersProperties();

					for (size_t i = 0; i < oShapeACP.size(); ++i)
					{
						cpFldMap->insert ( make_pair( CP(OffsetToCP(nTextPos + oInlineShapes[j].GetOffset()) + oShapeACP[i] ), oShapeAFld[i]));  
					}
				}
			}  
		}
	}  	  



	long CDocFile::WriteMainDocumentFields( const map<CP, Fld>& _cpFldMap )
	{
		long hr	= S_FALSE;

		if ( !_cpFldMap.empty() )
		{
			vector<CP> _aCP;
			vector<Fld> _aFld;

			for ( map<CP, Fld>::const_iterator iter = _cpFldMap.begin(); iter != _cpFldMap.end(); iter++ )
			{
				_aCP.push_back( iter->first );
				_aFld.push_back( iter->second );
			}

			_aCP.push_back(CP(ccpText + 1));

			ULONG writtenSize = 0;

			Plcfld plcfld( _aCP, _aFld );

			unsigned int fcPlcfFldMom	=	m_pTableStream->size();
			unsigned int lcbPlcfFldMom	=	plcfld.Size();

			hr = Write(m_pTableStream, fcPlcfFldMom, plcfld, lcbPlcfFldMom, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (282, &fcPlcfFldMom, sizeof(fcPlcfFldMom));
			hr = STREAMS::CSWordWriter::Instance()->Write (286, &lcbPlcfFldMom, sizeof(lcbPlcfFldMom));
		}

		return hr;
	}

	long CDocFile::WriteFootnoteDocumentFields (const map<CP, Fld>& _cpFldMap)
	{
		long hr	= S_FALSE;

		if ( !_cpFldMap.empty() )
		{
			std::vector<CP> _aCP;
			std::vector<Fld> _aFld;

			for (map<CP, Fld>::const_iterator iter = _cpFldMap.begin(); iter != _cpFldMap.end(); ++iter)
			{
				_aCP.push_back(iter->first);
				_aFld.push_back(iter->second);
			}

			_aCP.push_back(CP(ccpFtn + 1));

			ULONG writtenSize = 0;

			Plcfld plcfld(_aCP, _aFld);

			unsigned int fcPlcfFldFtn	=	m_pTableStream->size();
			unsigned int lcbPlcfFldFtn	=	plcfld.Size();

			hr = Write(m_pTableStream, fcPlcfFldFtn, plcfld, lcbPlcfFldFtn, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (298, &fcPlcfFldFtn, sizeof(fcPlcfFldFtn));
			hr = STREAMS::CSWordWriter::Instance()->Write (302, &lcbPlcfFldFtn, sizeof(lcbPlcfFldFtn));
		}

		return hr;
	}

	long CDocFile::WriteEndnoteDocumentFields( const map<CP, Fld>& _cpFldMap )
	{
		long hr	= S_FALSE;

		if ( !_cpFldMap.empty() )
		{
			vector<CP> _aCP;
			vector<Fld> _aFld;

			for (map<CP, Fld>::const_iterator iter = _cpFldMap.begin(); iter != _cpFldMap.end(); ++iter)
			{
				_aCP.push_back( iter->first );
				_aFld.push_back( iter->second );
			}

			_aCP.push_back(CP(ccpEdn + 1));

			ULONG writtenSize = 0;

			Plcfld plcfld( _aCP, _aFld );

			unsigned int fcPlcfFldEdn	=	m_pTableStream->size();
			unsigned int lcbPlcfFldEdn	=	plcfld.Size();

			hr = Write(m_pTableStream, fcPlcfFldEdn, plcfld, lcbPlcfFldEdn, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (538, &fcPlcfFldEdn, sizeof(fcPlcfFldEdn));
			hr = STREAMS::CSWordWriter::Instance()->Write (542, &lcbPlcfFldEdn, sizeof(lcbPlcfFldEdn));
		}

		return hr;
	}

	long CDocFile::WriteHeadersAndFootersDocumentFields( const map<CP, Fld>& _cpFldMap )
	{
		long hr	= S_FALSE;

		if ( !_cpFldMap.empty() )
		{
			vector<CP> _aCP;
			vector<Fld> _aFld;

			for (map<CP, Fld>::const_iterator iter = _cpFldMap.begin(); iter != _cpFldMap.end(); ++iter)
			{
				_aCP.push_back( iter->first );
				_aFld.push_back( iter->second );
			}

			_aCP.push_back(CP(ccpHdd + 1));

			ULONG writtenSize = 0;

			Plcfld plcfld(_aCP, _aFld);

			unsigned int fcPlcfFldHdr	=	m_pTableStream->size();
			unsigned int lcbPlcfFldHdr	=	plcfld.Size();

			hr = Write(m_pTableStream, fcPlcfFldHdr, plcfld, lcbPlcfFldHdr, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (290, &fcPlcfFldHdr, sizeof(fcPlcfFldHdr));
			hr = STREAMS::CSWordWriter::Instance()->Write (294, &lcbPlcfFldHdr, sizeof(lcbPlcfFldHdr));
		}

		return hr;
	}

	long CDocFile::WriteBookmarks( const vector<pair<CP, wstring>>& _bookmarksStartsCPsWithIDs, const vector<pair<CP, wstring>>& _bookmarksEndsCPsWithIDs, const vector<wstring>& _bookmarksNames )
	{
		long hr	= S_FALSE;

		if ( ( !_bookmarksStartsCPsWithIDs.empty() ) && ( !_bookmarksEndsCPsWithIDs.empty() ) && ( !_bookmarksNames.empty() ) )
		{
			vector<CP> aCP;
			vector<FBKF> aData;
			vector<bool> bookmarksNamesPresent( _bookmarksNames.size(), false );
			vector<bool> bookmarksEndsPresent( _bookmarksEndsCPsWithIDs.size(), false );

			for ( unsigned int i = 0; i < _bookmarksStartsCPsWithIDs.size(); i++ )
			{
				short index = this->GetBookmarkIndexByID( _bookmarksStartsCPsWithIDs[i].second, _bookmarksEndsCPsWithIDs );

				if ( index != -1 )
				{
					aCP.push_back( _bookmarksStartsCPsWithIDs[i].first );
					aData.push_back( FBKF( index, BKC() ) );
					bookmarksNamesPresent[i] = true;
					bookmarksEndsPresent[index] = true;
				}
			}

			aCP.push_back(CP(ccpText + 1));

			ULONG writtenSize = 0;

			Plcfbkf plcfbkf(aCP, aData );

			unsigned int fcPlcfBkf = m_pTableStream->size();
			unsigned int lcbPlcfBkf = plcfbkf.Size();

			hr = Write(m_pTableStream, fcPlcfBkf, plcfbkf, lcbPlcfBkf, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (330, &fcPlcfBkf, sizeof(fcPlcfBkf));
			hr = STREAMS::CSWordWriter::Instance()->Write (334, &lcbPlcfBkf, sizeof(lcbPlcfBkf));

			aCP.clear();
			aData.clear();

			for ( unsigned int i = 0; i < _bookmarksEndsCPsWithIDs.size(); i++ )
			{
				if ( bookmarksEndsPresent[i] )
				{
					aCP.push_back( _bookmarksEndsCPsWithIDs[i].first );
				}
			}

			aCP.push_back( CP( this->ccpText + 1 ) );

			Plcfbkl plcfbkl( aCP );


			unsigned int fcPlcfBkl = m_pTableStream->size();
			unsigned int lcbPlcfBkl = plcfbkl.Size();

			hr = Write(m_pTableStream, fcPlcfBkl, plcfbkl, lcbPlcfBkl, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (338, &fcPlcfBkl, sizeof(fcPlcfBkl));
			hr = STREAMS::CSWordWriter::Instance()->Write (342, &lcbPlcfBkl, sizeof(lcbPlcfBkl));

			vector<BookmarkName> bookmarksNames;

			for ( unsigned int i = 0; i < _bookmarksNames.size(); i++ )
			{
				if ( bookmarksNamesPresent[i] )
				{
					bookmarksNames.push_back( BookmarkName( _bookmarksNames[i].c_str() ) );
				}
			}

			SttbfBkmk sttbfBkmk( true, &bookmarksNames );

			unsigned int fcSttbfBkmk = m_pTableStream->size();
			unsigned int lcbSttbfBkmk = sttbfBkmk.Size();

			hr = Write(m_pTableStream, fcSttbfBkmk, sttbfBkmk, lcbSttbfBkmk, &writtenSize );

			hr = STREAMS::CSWordWriter::Instance()->Write (322, &fcSttbfBkmk, sizeof(fcSttbfBkmk));
			hr = STREAMS::CSWordWriter::Instance()->Write (326, &lcbSttbfBkmk, sizeof(lcbSttbfBkmk));
		}

		return hr;
	}

	short CDocFile::GetBookmarkIndexByID(const wstring& _id, const vector<pair<CP, wstring>>& _bookmarksCPsWithIDs)
	{
		for ( short index = 0; index < (short)_bookmarksCPsWithIDs.size(); index++ )
		{
			if ( _bookmarksCPsWithIDs[index].second == _id )
			{
				return index;
			}
		}

		return -1;
	}

	long CDocFile::WriteFootnoteDocument( unsigned int* _textPosition, vector<CP>* _AllFootnotesReferences )
	{
		long hr	= S_FALSE;

		if ( ( _textPosition != NULL ) && ( _AllFootnotesReferences != NULL ))
		{  
			ULONG writtenSize = 0;
			//STATSTG stg;

			if ( !m_Footnotes.empty() )
			{
				Paragraph footnotesEndParagraph;
				footnotesEndParagraph.AddParagraphItem( Run() );
				Footnote& lastFootnote = m_Footnotes.back().as<Footnote>();
				lastFootnote.AddTextItem( footnotesEndParagraph );
				lastFootnote.AddTextItem( footnotesEndParagraph );
			}

			wstring allFootnotesText;
			map<CP, Fld> cpFldMap;
			vector<short> allFootnotesIndexes;
			vector<CP> allFootnotesOffsets;
			unsigned int footnotesOffset = *_textPosition;
			unsigned int footnoteOffset = 0;

			for (list<TextItem>::const_iterator iter = m_Footnotes.begin(); iter != m_Footnotes.end(); ++iter)
			{
				Footnote footnoteIter = iter->as<Footnote>();

				vector<unsigned int> allFootnoteOffsets;
				vector<PapxInFkp> allFootnoteProperties = footnoteIter.GetAllParagraphsProperties( &allFootnoteOffsets );

				for (unsigned int i = 0; i < allFootnoteProperties.size(); ++i)
				{
					m_arPapxInFkp.push_back(allFootnoteProperties[i]);
					m_arParagraphsOffSets.push_back( footnotesOffset + allFootnoteOffsets[i] ); 
				}

				vector<unsigned int> allFootnoteRunsOffsets;
				vector<Chpx> allFootnoteChpxs = footnoteIter.GetAllRunProperties( &allFootnoteRunsOffsets );

				for (unsigned int i = 0; i < allFootnoteChpxs.size(); ++i)
				{
					m_arChpx.push_back(allFootnoteChpxs[i]); 
					m_arRunsOffsets.push_back(footnotesOffset + allFootnoteRunsOffsets[i]);
				}

				allFootnotesIndexes.push_back(footnoteIter.GetIndex());
				allFootnotesOffsets.push_back(CP( allFootnotesText.size()));

				AddFldCharsData	 (( TEXT_OFFSET_POSITION + footnoteOffset ), (*iter), &cpFldMap);
				AddHyperlinksData(( TEXT_OFFSET_POSITION + footnoteOffset ), (*iter), &cpFldMap);
				AddInlineShapesData(( TEXT_OFFSET_POSITION + footnoteOffset ), (*iter), &cpFldMap);

				footnoteOffset += ( footnoteIter.GetAllText().size() * sizeof(wchar_t) );
				allFootnotesText += footnoteIter.GetAllText();
				footnotesOffset = ( *_textPosition + footnoteOffset );
			}

			WriteFootnoteDocumentFields(cpFldMap);

			allFootnotesOffsets.push_back( CP( allFootnotesText.size() - 1 ) );
			allFootnotesOffsets.push_back( CP( allFootnotesText.size() + 1 ) );

			//write ccpFtn in FIB
			ccpFtn = allFootnotesText.size();

			if (ccpFtn > 0)
			{
				PlcffndTxt plcffndTxt( allFootnotesOffsets );
				_AllFootnotesReferences->push_back( CP( this->ccpText + this->ccpFtn + 1 ) );
				PlcffndRef plcffndRef( *_AllFootnotesReferences, allFootnotesIndexes );

				//m_pTableStream->Stat( &stg, STATFLAG_NONAME );

				unsigned int fcPlcffndTxt = 0;//stg.cbSize.LowPart;
				unsigned int lcbPlcffndTxt = plcffndTxt.Size();

				hr = Write(m_pTableStream, fcPlcffndTxt, plcffndTxt, lcbPlcffndTxt, &writtenSize );

				//m_pTableStream->Stat( &stg, STATFLAG_NONAME );

				unsigned int fcPlcffndRef = 0;//stg.cbSize.LowPart;
				unsigned int lcbPlcffndRef = plcffndRef.Size();

				hr = Write(m_pTableStream, fcPlcffndRef, plcffndRef, lcbPlcffndRef, &writtenSize );

				hr = STREAMS::CSWordWriter::Instance()->Write (80, &(this->ccpFtn), sizeof(this->ccpFtn));
				hr = STREAMS::CSWordWriter::Instance()->Write (178, &fcPlcffndTxt, sizeof(fcPlcffndTxt));
				hr = STREAMS::CSWordWriter::Instance()->Write (182, &lcbPlcffndTxt, sizeof(lcbPlcffndTxt));
				hr = STREAMS::CSWordWriter::Instance()->Write (170, &fcPlcffndRef, sizeof(fcPlcffndRef));
				hr = STREAMS::CSWordWriter::Instance()->Write (174, &lcbPlcffndRef, sizeof(lcbPlcffndRef));
				hr = Write(STREAMS::CSWordWriter::Instance()->Get(), ( *_textPosition ), allFootnotesText.c_str(), ( allFootnotesText.size() * sizeof(wchar_t) ), &writtenSize );
				
				*_textPosition += writtenSize;
			}
		}

		return hr;
	}

	long CDocFile::WriteEndnoteDocument( unsigned int* _textPosition, vector<CP>* _AllEndnotesReferences)
	{
		long hr	= S_FALSE;

		if ( ( _textPosition != NULL ) && ( _AllEndnotesReferences != NULL ))
		{  
			ULONG writtenSize = 0;
			//STATSTG stg;

			if ( !this->endnotes.empty() )
			{
				Paragraph endnotesEndParagraph;
				endnotesEndParagraph.AddParagraphItem( Run() );
				Endnote& lastEndnote = this->endnotes.back().as<Endnote>();
				lastEndnote.AddTextItem( endnotesEndParagraph );
				lastEndnote.AddTextItem( endnotesEndParagraph );
			}

			wstring allEndnotesText;
			map<CP, Fld> cpFldMap;
			vector<short> allEndnotesIndexes;
			vector<CP> allEndnotesOffsets;
			unsigned int endnotesOffset = *_textPosition;
			unsigned int endnoteOffset = 0;

			for (std::list<TextItem>::const_iterator iter = endnotes.begin(); iter != endnotes.end(); ++iter)
			{
				Endnote endnoteIter = iter->as<Endnote>();

				vector<unsigned int> allEndnoteOffsets;
				vector<PapxInFkp> allEndnoteProperties = endnoteIter.GetAllParagraphsProperties( &allEndnoteOffsets );

				for ( unsigned int i = 0; i < allEndnoteProperties.size(); i++ )
				{
					m_arPapxInFkp.push_back( allEndnoteProperties[i] );
					m_arParagraphsOffSets.push_back( endnotesOffset + allEndnoteOffsets[i] ); 
				}

				vector<unsigned int> allEndnoteRunsOffsets;
				vector<Chpx> allEndnoteChpxs = endnoteIter.GetAllRunProperties( &allEndnoteRunsOffsets );

				for ( unsigned int i = 0; i < allEndnoteChpxs.size(); i++ )
				{
					m_arChpx.push_back( allEndnoteChpxs[i] ); 
					m_arRunsOffsets.push_back( endnotesOffset + allEndnoteRunsOffsets[i] );
				}

				allEndnotesIndexes.push_back( endnoteIter.GetIndex() );
				allEndnotesOffsets.push_back( CP( allEndnotesText.size() ) );

				AddFldCharsData	    (( TEXT_OFFSET_POSITION + endnoteOffset), (*iter), &cpFldMap);
				AddHyperlinksData   (( TEXT_OFFSET_POSITION + endnoteOffset), (*iter), &cpFldMap);
				AddInlineShapesData (( TEXT_OFFSET_POSITION + endnoteOffset), (*iter), &cpFldMap);

				endnoteOffset += ( endnoteIter.GetAllText().size() * sizeof(wchar_t) );
				allEndnotesText += endnoteIter.GetAllText();
				endnotesOffset = ( *_textPosition + endnoteOffset );
			}

			WriteEndnoteDocumentFields(cpFldMap);

			allEndnotesOffsets.push_back( CP( allEndnotesText.size() - 1 ) );
			allEndnotesOffsets.push_back( CP( allEndnotesText.size() + 1 ) );

			//write ccpEdn in FIB
			ccpEdn = allEndnotesText.size();

			if ( this->ccpEdn > 0 )
			{
				PlcfendTxt plcfendTxt( allEndnotesOffsets );
				_AllEndnotesReferences->push_back( CP( this->ccpText + this->ccpFtn + this->ccpHdd + this->ccpAtn + this->ccpEdn + 1 ) );
				PlcfendRef plcfendRef( *_AllEndnotesReferences, allEndnotesIndexes );


				unsigned int fcPlcfendTxt = m_pTableStream->size();//stg.cbSize.LowPart;
				unsigned int lcbPlcfendTxt = plcfendTxt.Size();

				hr = Write(m_pTableStream, fcPlcfendTxt, plcfendTxt, lcbPlcfendTxt, &writtenSize );


				unsigned int fcPlcfendRef = m_pTableStream->size();//stg.cbSize.LowPart;
				unsigned int lcbPlcfendRef = plcfendRef.Size();

				hr = Write(m_pTableStream, fcPlcfendRef, plcfendRef, lcbPlcfendRef, &writtenSize );

				WRITE_STREAM_WORD(ccpEdn);	
				//	hr = STREAMS::CSWordWriter::Instance()->Write (FIB_OFFSET::ccpEdn, &(ccpEdn), sizeof(ccpEdn));
				hr = STREAMS::CSWordWriter::Instance()->Write (530, &fcPlcfendTxt, sizeof(fcPlcfendTxt));
				hr = STREAMS::CSWordWriter::Instance()->Write (534, &lcbPlcfendTxt, sizeof(lcbPlcfendTxt));
				hr = STREAMS::CSWordWriter::Instance()->Write (522, &fcPlcfendRef, sizeof(fcPlcfendRef));
				hr = STREAMS::CSWordWriter::Instance()->Write (526, &lcbPlcfendRef, sizeof(lcbPlcfendRef));
				hr = Write(STREAMS::CSWordWriter::Instance()->Get(), ( *_textPosition ), allEndnotesText.c_str(), ( allEndnotesText.size() * sizeof(wchar_t) ), &writtenSize );

				*_textPosition += writtenSize;
			}
		}

		return hr;  
	}

	std::wstring CDocFile::GetHeadersOrFootersProperties (const ITextItem* pHeaderOrFooter, unsigned int& _headersOrFootersOffset, unsigned int* _headerOrFooterOffset, map<CP, Fld>* cpFldMap )
	{
		if ( ( pHeaderOrFooter != NULL ) && ( _headerOrFooterOffset != NULL ) && ( cpFldMap != NULL ) )
		{
			vector<unsigned int> allParagraphsOffsets;
			vector<PapxInFkp> allParagraphsProperties;

			vector<unsigned int> allRunsOffsets;
			vector<Chpx> allChpxs;  

			allParagraphsProperties = pHeaderOrFooter->GetAllParagraphsProperties(&allParagraphsOffsets);

			for (size_t i = 0; i < allParagraphsProperties.size(); ++i)
			{
				m_arPapxInFkp.push_back( allParagraphsProperties[i] );
				m_arParagraphsOffSets.push_back( _headersOrFootersOffset + allParagraphsOffsets[i] );
			}

			allChpxs = pHeaderOrFooter->GetAllRunProperties( &allRunsOffsets );

			for (size_t i = 0; i < allChpxs.size(); ++i)
			{
				m_arChpx.push_back (allChpxs[i]); 
				m_arRunsOffsets.push_back (_headersOrFootersOffset + allRunsOffsets[i]);
			} 

			AddFldCharsData((TEXT_OFFSET_POSITION + (*_headerOrFooterOffset * sizeof(wchar_t))), TextItem (*pHeaderOrFooter), //	TextItem( *pHeaderOrFooter ).GetAllRunItemsByType<FldChar>(), 
				cpFldMap);

			AddHyperlinksData((TEXT_OFFSET_POSITION + (*_headerOrFooterOffset * sizeof(wchar_t))), TextItem(*pHeaderOrFooter), //	TextItem( *pHeaderOrFooter ).GetAllParagraphItemsByType<Hyperlink>(), 
				cpFldMap);

			AddInlineShapesData((TEXT_OFFSET_POSITION + (*_headerOrFooterOffset * sizeof(wchar_t))), TextItem(*pHeaderOrFooter), 
				cpFldMap);

			std::wstring content		=	pHeaderOrFooter->GetAllText();

			*_headerOrFooterOffset		+=	content.size();
			_headersOrFootersOffset		+=	content.size() * sizeof(wchar_t);

			return content;
		}

		return std::wstring();
	}

	long CDocFile::WriteHeadersAndFootersDocument (unsigned int* _textPosition)
	{
		long hr	= S_FALSE;

		if ( ( NULL != _textPosition ))
		{  
			ULONG writtenSize = 0;
			unsigned int headerOrFooterOffset = 0;//m_pTableStream->size();????
			unsigned int headersOrFootersOffset = ( *_textPosition + ( headerOrFooterOffset * sizeof(wchar_t) ) );
			vector<CP> _aCP;
			map<CP, Fld> cpFldMap;
			wstring allHeadersAndFootersDocumentText;

			_aCP.push_back( CP( headerOrFooterOffset ) );
			_aCP.push_back( CP( headerOrFooterOffset ) );
			_aCP.push_back( CP( headerOrFooterOffset ) );
			_aCP.push_back( CP( headerOrFooterOffset ) );
			_aCP.push_back( CP( headerOrFooterOffset ) );
			_aCP.push_back( CP( headerOrFooterOffset ) );

			for (list<SectionProperties>::const_iterator sectionIter = sectionProperties.begin(); sectionIter != sectionProperties.end(); ++sectionIter)
			{
				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetEvenPageHeader(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );

				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetOddPageHeader(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );

				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetEvenPageFooter(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );

				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetOddPageFooter(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );

				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetFirstPageHeader(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );

				_aCP.push_back( CP( headerOrFooterOffset ) );

				allHeadersAndFootersDocumentText += GetHeadersOrFootersProperties(sectionIter->GetFirstPageFooter(), headersOrFootersOffset, &headerOrFooterOffset, &cpFldMap );
			}

			ccpHdd = allHeadersAndFootersDocumentText.size();

			WriteHeadersAndFootersDocumentFields( cpFldMap );

			_aCP.push_back( CP(ccpHdd - 1 ) );
			_aCP.push_back( CP(ccpHdd + 1 ) );

			if (ccpHdd > 0)
			{
				Plcfhdd plcfhdd (_aCP);

				//m_pTableStream->Stat( &stg, STATFLAG_NONAME );

				unsigned int fcPlcfHdd	=	0;//stg.cbSize.LowPart;
				unsigned int lcbPlcfHdd	=	plcfhdd.Size();

				hr = Write (m_pTableStream, fcPlcfHdd, plcfhdd, lcbPlcfHdd, &writtenSize );
				hr = STREAMS::CSWordWriter::Instance()->Write (242, &fcPlcfHdd, sizeof(fcPlcfHdd));
				hr = STREAMS::CSWordWriter::Instance()->Write (246, &lcbPlcfHdd, sizeof(lcbPlcfHdd));
				WRITE_STREAM_WORD(ccpHdd);	//	hr = STREAMS::CSWordWriter::Instance()->Write (FIB_OFFSET::ccpHdd, &ccpHdd, sizeof(ccpHdd) );
				hr = Write (STREAMS::CSWordWriter::Instance()->Get(), ( *_textPosition ), allHeadersAndFootersDocumentText.c_str(), ( allHeadersAndFootersDocumentText.size() * sizeof(wchar_t) ), &writtenSize );
				*_textPosition += writtenSize;
			}
		}

		return hr;
	}

	void CDocFile::WriteSectionProperties()
	{
		long hr	= S_FALSE;

		int fcSepx = STREAMS::CSWordWriter::Instance()->Get()->size();

		vector<CP> aCP;
		unsigned int cp = 0;

		aCP.push_back( CP( cp ) );

		for (list<TextItem>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			if ( iter->is<SectionBreak>() )
			{
				aCP.push_back( CP( cp + 1 ) );
			}

			cp += (*iter)->GetAllText().size();
		}

		aCP.push_back(CP(ccpText - 1));

		if ( aCP.size() == sectionProperties.size() )
		{
			aCP.push_back(CP(ccpText + ccpFtn + ccpHdd + ccpAtn + ccpEdn));
		}
		else
		{
			aCP.back() = CP(ccpText);
		}

		vector<Sed> aSed;

		for (list<SectionProperties>::const_iterator iter = sectionProperties.begin(); iter != sectionProperties.end(); ++iter)
		{
			aSed.push_back(Sed(fcSepx));
			fcSepx += iter->GetSepx().Size();
		}

		PlcfSed plcfSed( aCP, aSed );

		unsigned int fcPlcfSed	=	STREAMS::CSWordWriter::Instance()->Get()->size();
		unsigned int lcbPlcfSed	=	plcfSed.Size();

		WRITE_STREAM_WORD(fcPlcfSed);		//	202
		WRITE_STREAM_WORD(lcbPlcfSed);		//	206

		ULONG writtenSize = 0;
		hr = Write (m_pTableStream, fcPlcfSed, (unsigned char*)plcfSed, lcbPlcfSed, &writtenSize);

		unsigned int i = 0;

		for (list<SectionProperties>::const_iterator iter = sectionProperties.begin(); iter != sectionProperties.end(); ++iter)
		{
			hr = Write(STREAMS::CSWordWriter::Instance()->Get(), aSed[i++].GetFcSepx(), (unsigned char*)(iter->GetSepx()), iter->GetSepx().Size(), &writtenSize);
		}
	}

	//
	long CDocFile::WriteFontTable ()
	{
		long hr	= S_OK;

		{
			unsigned int fcSttbfFfn		=	m_pTableStream->size();
			unsigned int lcbSttbfFfn	=	m_oFontTable.Size();

			if (lcbSttbfFfn > 0)
			{
				WRITE_STREAM_WORD(fcSttbfFfn);		//	274
				WRITE_STREAM_WORD(lcbSttbfFfn);		//	278

				hr	=	Write (m_pTableStream, fcSttbfFfn, (unsigned char*)(m_oFontTable), lcbSttbfFfn);
			}
		}

		return hr;
	}

	long CDocFile::WriteStyleSheet ()
	{
		long hr	= S_OK;

		{
			unsigned int fcStshf		=	m_pTableStream->size();
			unsigned int lcbStshf		=	m_oStyleSheet.Size();
			unsigned int fcStshfOrig	=	fcStshf;
			unsigned int lcbStshfOrig	=	lcbStshf;

			if (lcbStshf > 0)
			{
				WRITE_STREAM_WORD(fcStshfOrig);		//	154
				WRITE_STREAM_WORD(lcbStshfOrig);	//	158

				WRITE_STREAM_WORD(fcStshf);			//	162
				WRITE_STREAM_WORD(lcbStshf);		//	166

				hr	=	Write (m_pTableStream, fcStshf, (unsigned char*)(m_oStyleSheet), lcbStshf);
			}
		}

		return hr;
	}

	long CDocFile::WriteNumbering()
	{
		long hr	= S_FALSE;

		ULONG writtenSize = 0;

		const unsigned int fcPlfLst		=	m_pTableStream->size();
		const unsigned int lcbPlfLst	=	listFormatInfo.ListFormattingInformationSize();

		if (lcbPlfLst > 0)
		{
			hr = STREAMS::CSWordWriter::Instance()->Write (738, &fcPlfLst, sizeof(fcPlfLst) );
			hr = STREAMS::CSWordWriter::Instance()->Write (742, &lcbPlfLst, sizeof(lcbPlfLst) );
			hr = Write (m_pTableStream, fcPlfLst, (unsigned char*)(listFormatInfo), listFormatInfo.Size(), &writtenSize );

			const unsigned int fcPlfLfo		=	m_pTableStream->size();
			const unsigned int lcbPlfLfo	=	listFormatOverrideInfo.Size();

			if ( lcbPlfLfo > 0 )
			{
				hr = STREAMS::CSWordWriter::Instance()->Write (746, &fcPlfLfo, sizeof(fcPlfLfo) );
				hr = STREAMS::CSWordWriter::Instance()->Write (750, &lcbPlfLfo, sizeof(lcbPlfLfo) );
				hr = Write (m_pTableStream, fcPlfLfo, (unsigned char*)(listFormatOverrideInfo), listFormatOverrideInfo.Size(), &writtenSize );
			}
		}

		return hr;
	}

	//
	std::wstring CDocFile::GetMainDocumentText() const
	{
		std::wstring mainDocumentText;

		for (list<TextItem>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			mainDocumentText += (*iter)->GetAllText();
		}

		return mainDocumentText;
	}

	unsigned long CDocFile::GetMainDocumentTextItemsCount() const
	{
		return (unsigned long)(textItems.size());
	}

	unsigned long CDocFile::GetAllTextSize() const
	{
		unsigned long documentTextSize = 0;

		for (list<TextItem>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); ++iter)
		{
			documentTextSize += (*iter)->GetAllText().size();  
		}

		for (list<TextItem>::const_iterator iter = m_Footnotes.begin(); iter != m_Footnotes.end(); ++iter)
		{
			documentTextSize += (*iter)->GetAllText().size();
		}

		for (list<TextItem>::const_iterator iter = this->endnotes.begin(); iter != this->endnotes.end(); ++iter)
		{
			documentTextSize += (*iter)->GetAllText().size();
		}

		return documentTextSize;
	}

	unsigned long CDocFile::GetAllTextItemsCount() const
	{
		return (unsigned long)( textItems.size() + m_Footnotes.size() + endnotes.size() );
	}

	std::wstring CDocFile::GetAllText() const
	{
		std::wstring allText;

		for (list<TextItem>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			allText += (*iter)->GetAllText();  

		for (list<TextItem>::const_iterator iter = m_Footnotes.begin(); iter != m_Footnotes.end(); ++iter)
			allText += (*iter)->GetAllText();

		for (list<TextItem>::const_iterator iter = endnotes.begin(); iter != endnotes.end(); ++iter)
			allText += (*iter)->GetAllText();

		return allText;  
	}
}

namespace ASCDocFileFormat
{	
	int CDocFile::WriteFibRgLw97()
	{
		long hr		=	S_OK;

		//ccpText	=	GetFibRgLw97_ccpText();			//	
		ccpTxbx		=	GetFibRgLw97_ccpTxbx();			//	A signed integer that specifies the count of CPs in the textbox subdocument of the main document. This value MUST be zero, 1, or greater.
		ccpHdrTxbx	=	GetFibRgLw97_ccpHdrTxbx();		//	A signed integer that specifies the count of CPs in the textbox subdocument of the header. This value MUST be zero, 1, or greater

		WRITE_STREAM_WORD(ccpText);
		WRITE_STREAM_WORD(ccpTxbx);	
		WRITE_STREAM_WORD(ccpHdrTxbx);

		return TRUE;
	}

	int CDocFile::GetFibRgLw97_ccpText()
	{
		int documentTextSize = 0;

		for (list<TextItem>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			documentTextSize += (*iter)->GetAllText().size();  

		return documentTextSize;
	}

	int CDocFile::GetFibRgLw97_ccpTxbx()
	{
		unsigned long tbMRefSize = 0;

		if (m_oartStorage)
		{
			const std::vector<CTextBoxRef*>& arMTbRefs = m_oartStorage->GetTbRefs(MAIN_DOCUMENT);
			for (size_t i = 0; i < arMTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText = arMTbRefs[i]->GetText ();
				for (size_t j = 0; j < arText.size(); ++j)
					tbMRefSize += arText[j]->GetAllText().size();
			}
		}

		return tbMRefSize;
	}

	int CDocFile::GetFibRgLw97_ccpHdrTxbx()
	{
		unsigned long tbHRefSize = 0;

		if (m_oartStorage)
		{
			const std::vector<CTextBoxRef*>& arHTbRefs = m_oartStorage->GetTbRefs(HEADER_DOCUMENT);
			for (size_t i = 0; i < arHTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText = arHTbRefs[i]->GetText ();
				for (size_t j = 0; j < arText.size(); ++j)
					tbHRefSize += arText[j]->GetAllText().size();
			}
		}

		return tbHRefSize;
	}

	int CDocFile::WriteFibMainTBRefs()
	{
		long hr	= S_OK;

		m_arTxbxCP.clear();
		m_arTxbxBkdCP.clear();

		if (m_oartStorage)
		{
			const std::vector<CTextBoxRef*>& arTbRefs = m_oartStorage->GetTbRefs(MAIN_DOCUMENT);
			if (0==arTbRefs.size())
				return FALSE;

			unsigned int cp = 0;
			if (arTbRefs.size())
			{
				m_arTxbxCP.push_back (CP(cp));
				m_arTxbxBkdCP.push_back (CP(cp));
			}

			std::vector<FTXBXS>	aFTXBXS;
			std::vector<Tbkd>	aTbkd;
			
			for (size_t i = 0; i < arTbRefs.size(); ++i)
			{
				int nLID = arTbRefs[i]->GetID();

				const std::vector<TextItem>& arText = arTbRefs[i]->GetText ();		//	один элемент - один текcт бокс
				for (size_t j = 0; j < arText.size(); ++j)
					cp	+=	arText[j]->GetAllText().size();

				m_arTxbxCP.push_back (CP(cp));
				aFTXBXS.push_back (FTXBXS(nLID));									//	Привязка OArtShapeID к текстовому полю

				m_arTxbxBkdCP.push_back (CP(cp));
				aTbkd.push_back (Tbkd(i,0));										//	Индекс FTXBSX
			}

			int ccpFull	= ccpText + ccpFtn + ccpHdd + ccpAtn + ccpEdn + ccpTxbx;// - 1;

			m_arTxbxCP.push_back (CP(ccpFull));
			aFTXBXS.push_back (FTXBXS());

			m_arTxbxBkdCP.push_back (CP(ccpTxbx + 1));
			aTbkd.push_back (Tbkd());

			if (m_arTxbxCP.size())
			{
				{
					PlcftxbxTxt oPlcftxbxTxt (m_arTxbxCP, aFTXBXS);					//	PlcftxbxTxt 

					unsigned int fcPlcftxbxTxt		=	m_pTableStream->size();
					unsigned int lcbPlcftxbxTxt		=	oPlcftxbxTxt.Size();

					WRITE_STREAM_WORD(fcPlcftxbxTxt);								//	602
					WRITE_STREAM_WORD(lcbPlcftxbxTxt);								//	606

					WRITE_STREAM_TABLE(PlcftxbxTxt, oPlcftxbxTxt);
				}

				{
					PlcfTxbxBkd	oPlcfTxbxBkd (m_arTxbxBkdCP, aTbkd);				//	PlcfTxbxBkd

					unsigned int fcPlcfTxbxBkd		=	m_pTableStream->size();
					unsigned int lcbPlcfTxbxBkd		=	oPlcfTxbxBkd.Size();

					WRITE_STREAM_WORD(fcPlcfTxbxBkd);								//	754
					WRITE_STREAM_WORD(lcbPlcfTxbxBkd);								//	758

					WRITE_STREAM_TABLE(PlcfTxbxBkd, oPlcfTxbxBkd);
				}

				return TRUE;
			}
		}

		return FALSE;
	}

	int CDocFile::WriteFibHeadTBRefs()
	{
		long hr	= S_OK;

		m_arTxbxHdrCP.clear();
		m_arTxbxHdrBkdCP.clear();

		if (m_oartStorage)
		{
			const std::vector<CTextBoxRef*>& arTbRefs = m_oartStorage->GetTbRefs(HEADER_DOCUMENT);
			if (0==arTbRefs.size())
				return FALSE;

			unsigned int cp = 0;
			if (arTbRefs.size())
			{
				m_arTxbxHdrCP.push_back (CP(cp));
				m_arTxbxHdrBkdCP.push_back (CP(cp));
			}

			std::vector<FTXBXS>	aFTXBXS;
			std::vector<Tbkd>	aTbkd;
			
			for (size_t i = 0; i < arTbRefs.size(); ++i)
			{
				int nLID = arTbRefs[i]->GetID();

				const std::vector<TextItem>& arText = arTbRefs[i]->GetText ();		//	один элемент - один текcт бокс
				for (size_t j = 0; j < arText.size(); ++j)
					cp	+=	arText[j]->GetAllText().size();

				m_arTxbxHdrCP.push_back (CP(cp));
				aFTXBXS.push_back (FTXBXS(nLID));									//	Привязка OArtShapeID к текстовому полю

				m_arTxbxHdrBkdCP.push_back (CP(cp));
				aTbkd.push_back (Tbkd(i,0));										//	Индекс FTXBSX
			}

			int ccpFull	= ccpText + ccpFtn + ccpHdd + ccpAtn + ccpEdn + ccpTxbx + ccpHdrTxbx - 1;

			m_arTxbxHdrCP.push_back (CP(ccpFull));
			aFTXBXS.push_back (FTXBXS());

			m_arTxbxHdrBkdCP.push_back (CP(ccpHdrTxbx + 1));
			aTbkd.push_back (Tbkd());

			if (m_arTxbxHdrCP.size())
			{
				{
					PlcfHdrtxbxTxt oPlcfHdrtxbxTxt (m_arTxbxHdrCP, aFTXBXS);		//	PlcfHdrtxbxTxt

					unsigned int fcPlcfHdrtxbxTxt	=	m_pTableStream->size();
					unsigned int lcbPlcfHdrtxbxTxt	=	oPlcfHdrtxbxTxt.Size();

					WRITE_STREAM_WORD(fcPlcfHdrtxbxTxt);							//	602
					WRITE_STREAM_WORD(lcbPlcfHdrtxbxTxt);							//	606

					WRITE_STREAM_TABLE(PlcfHdrtxbxTxt, oPlcfHdrtxbxTxt);
				}

				{
					PlcfTxbxHdrBkd oPlcfTxbxHdrBkd (m_arTxbxHdrBkdCP, aTbkd);		//	PlcfTxbxHdrBkd

					unsigned int fcPlcfTxbxHdrBkd	=	m_pTableStream->size();
					unsigned int lcbPlcfTxbxHdrBkd	=	oPlcfTxbxHdrBkd.Size();

					WRITE_STREAM_WORD(fcPlcfTxbxHdrBkd);							//	762
					WRITE_STREAM_WORD(lcbPlcfTxbxHdrBkd);							//	766

					WRITE_STREAM_TABLE(PlcfTxbxHdrBkd, oPlcfTxbxHdrBkd);
				}

				return TRUE;
			}
		}

		return FALSE;
	}

	int CDocFile::CalculateOffTbRefs (long nBuffPos)
	{
		// индексируем текст из "текстовых надписях"

		if (m_oartStorage)
		{
			//	MAIN_DOCUMENT

			const std::vector<CTextBoxRef*>& arMTbRefs		=	m_oartStorage->GetTbRefs(MAIN_DOCUMENT);
			for (size_t i = 0; i < arMTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText			=	arMTbRefs[i]->GetText ();

				for (size_t ind = 0; ind < arText.size(); ++ind)
				{
					vector<unsigned int> allTextItemOffsets;
					vector<PapxInFkp> allTextItemProperties	=	arText[ind]->GetAllParagraphsProperties(&allTextItemOffsets);

					for (unsigned int j = 0; j < allTextItemProperties.size(); ++j)
					{
						m_arPapxInFkp.push_back(allTextItemProperties[j]);
						m_arParagraphsOffSets.push_back(nBuffPos + allTextItemOffsets[j]); 
					}

					vector<unsigned int> allTextItemRunsOffsets;
					vector<Chpx> allTextItemChpxs			=	arText[ind]->GetAllRunProperties(&allTextItemRunsOffsets);

					for (unsigned int j = 0; j < allTextItemChpxs.size(); ++j)
					{
						m_arChpx.push_back(allTextItemChpxs[j]); 
						m_arRunsOffsets.push_back(nBuffPos + allTextItemRunsOffsets[j]);
					}

					nBuffPos += sizeof(wchar_t) * arText[ind]->GetAllText().size();
				}
			}

			// HEADER_DOCUMENT

			const std::vector<CTextBoxRef*>& arHTbRefs		=	m_oartStorage->GetTbRefs(HEADER_DOCUMENT);
			for (size_t i = 0; i < arHTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText			=	arHTbRefs[i]->GetText ();
				for (size_t ind = 0; ind < arText.size(); ++ind)
				{
					vector<unsigned int> allTextItemOffsets;
					vector<PapxInFkp> allTextItemProperties	=	arText[ind]->GetAllParagraphsProperties(&allTextItemOffsets);

					for (unsigned int j = 0; j < allTextItemProperties.size(); ++j)
					{
						m_arPapxInFkp.push_back(allTextItemProperties[j]);
						m_arParagraphsOffSets.push_back(nBuffPos + allTextItemOffsets[j]); 
					}

					vector<unsigned int> allTextItemRunsOffsets;
					vector<Chpx> allTextItemChpxs			=	arText[ind]->GetAllRunProperties(&allTextItemRunsOffsets);

					for (unsigned int j = 0; j < allTextItemChpxs.size(); ++j)
					{
						m_arChpx.push_back(allTextItemChpxs[j]); 
						m_arRunsOffsets.push_back(nBuffPos + allTextItemRunsOffsets[j]);
					}

					nBuffPos += sizeof(wchar_t) * arText[ind]->GetAllText().size();
				}
			}
		}

		return TRUE;
	}

	int CDocFile::WriteStrTbRefs (long nBuffPos)
	{
		//	Запись символов текста в основной поток (пишем после записи текста документа, футеров и прочего)

		if (m_oartStorage)
		{
			unsigned long hr			=	0;
			unsigned long bufferTbSize	=	0;

			unsigned long mainTbSize	=	0;
			unsigned long headTbSize	=	0;
			unsigned long endPrTbSize	=	0;

			//	MAIN_DOCUMENT

			const std::vector<CTextBoxRef*>& arMTbRefs	=	m_oartStorage->GetTbRefs(MAIN_DOCUMENT);
			for (size_t i = 0; i < arMTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText		=	arMTbRefs[i]->GetText ();
				for (size_t ind = 0; ind < arText.size(); ++ind)
				{
					hr = Write(STREAMS::CSWordWriter::Instance()->Get(), nBuffPos, arText[ind]->GetAllText().c_str(), (sizeof(wchar_t) * arText[ind]->GetAllText().size()), &bufferTbSize);
					nBuffPos	+=	bufferTbSize;
					mainTbSize	+=	bufferTbSize;
				}
			}

			// HEADER_DOCUMENT
			
			const std::vector<CTextBoxRef*>& arHTbRefs	=	m_oartStorage->GetTbRefs(HEADER_DOCUMENT);
			for (size_t i = 0; i < arHTbRefs.size(); ++i)
			{
				const std::vector<TextItem>& arText		=	arHTbRefs[i]->GetText ();
				for (size_t ind = 0; ind < arText.size(); ++ind)
				{
					hr = Write(STREAMS::CSWordWriter::Instance()->Get(), nBuffPos, arText[ind]->GetAllText().c_str(), (sizeof(wchar_t) * arText[ind]->GetAllText().size()), &bufferTbSize);
					nBuffPos	+=	bufferTbSize;
					headTbSize	+=	bufferTbSize;
				}
			}

			return (mainTbSize + headTbSize);
		}

		return 0;
	}
}

namespace ASCDocFileFormat
{
	void CDocFile::CalculateMainSpa ()
	{
		m_aSpaCP.clear();

		unsigned int cp = 0;

		for (list<TextItem>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			vector<unsigned int> paragraphsItemsOffsets;
			vector<IParagraphItemPtr> paragraphsItems	=	(*iter)->GetAllRunsCopy (&paragraphsItemsOffsets);

			if (paragraphsItems.size())
			{
				unsigned int rCP	=	0;

				for (size_t i = 0; i < paragraphsItems.size(); ++i)
				{
					Run* run		=	dynamic_cast<Run*>(paragraphsItems[i].operator->());
					if (run)
					{
						for (list<RunItem>::const_iterator runiter = run->begin(); runiter != run->end(); ++runiter)
						{		
							if (runiter->is<ASCDocFileFormat::CShapeRun>())
								m_aSpaCP.push_back (CP(rCP + cp));

							rCP		+=	(*runiter)->GetAllText().size();
						}
					}
				}
			}

			cp	+=	(*iter)->GetAllText().size();
		}

		if (0 == m_aSpaCP.size())
			return;

		m_aSpaCP.push_back (CP(ccpText - 1));
	}

	void CDocFile::CalculateHeaderSpa ()
	{
		m_aHeadSpaCP.clear();

		unsigned int cp = 0;

		//  TODO : требуется небольшой рефакторинг ( в хедере может быть много разной инфы и каждый раз пересчитывать каретку не правильно )

		for (list<SectionProperties>::const_iterator sectionIter = sectionProperties.begin(); sectionIter != sectionProperties.end(); ++sectionIter)
		{
			list<TextItem> headerItems;

			headerItems.push_back (TextItem(*sectionIter->GetEvenPageHeader()));
			headerItems.push_back (TextItem(*sectionIter->GetOddPageHeader()));
			headerItems.push_back (TextItem(*sectionIter->GetEvenPageFooter()));
			headerItems.push_back (TextItem(*sectionIter->GetOddPageFooter()));
			headerItems.push_back (TextItem(*sectionIter->GetFirstPageHeader()));
			headerItems.push_back (TextItem(*sectionIter->GetFirstPageFooter()));

			for (list<TextItem>::const_iterator iter = headerItems.begin(); iter != headerItems.end(); ++iter)
			{
				vector<unsigned int> paragraphsItemsOffsets;
				vector<IParagraphItemPtr> paragraphsItems	=	(*iter)->GetAllRunsCopy (&paragraphsItemsOffsets);

				if (paragraphsItems.size())
				{
					unsigned int rCP	=	0;

					for (size_t i = 0; i < paragraphsItems.size(); ++i)
					{
						Run* run		=	dynamic_cast<Run*>(paragraphsItems[i].operator->());
						if (run)
						{
							for (list<RunItem>::const_iterator runiter = run->begin(); runiter != run->end(); ++runiter)
							{		
								if (runiter->is<ASCDocFileFormat::CShapeRun>())
									m_aHeadSpaCP.push_back (CP(rCP + cp));

								rCP		+=	(*runiter)->GetAllText().size();
							}
						}
					}
				}

				cp	+=	(*iter)->GetAllText().size();
			}
		}

		if (0==m_aHeadSpaCP.size())
			return;

		m_aHeadSpaCP.push_back (CP(ccpText + ccpFtn + ccpHdd - 1));
	}

	long CDocFile::WriteOfficeDrawings ()
	{
		long hr	= S_OK;

		if (m_oartStorage)
		{
			if (m_oartStorage->GetSpa(MAIN_DOCUMENT).size() || m_oartStorage->GetSpa(HEADER_DOCUMENT).size())
			{
				CalculateMainSpa ();

				if (m_aSpaCP.size())
				{
					{
						PlcfSpa oPlcfSpa (m_aSpaCP, m_oartStorage->GetSpa(MAIN_DOCUMENT));

						unsigned int fcPlcSpaMom	=	m_pTableStream->size();
						unsigned int lcbPlcSpaMom	=	oPlcfSpa.Size();

						WRITE_STREAM_WORD(fcPlcSpaMom);			//	474
						WRITE_STREAM_WORD(lcbPlcSpaMom);		//	478

						WRITE_STREAM_TABLE(PlcSpaMom, oPlcfSpa);
					}
				}

				CalculateHeaderSpa ();

				if (m_aHeadSpaCP.size())
				{
					std::string n = m_pTableStream->fullName();
					{
						PlcfSpa oPlcfSpa (m_aHeadSpaCP, m_oartStorage->GetSpa(HEADER_DOCUMENT));

						unsigned int fcPlcSpaHdr	=	m_pTableStream->size();
						unsigned int lcbPlcSpaHdr	=	oPlcfSpa.Size();

						WRITE_STREAM_WORD(fcPlcSpaHdr);			//	482
						WRITE_STREAM_WORD(lcbPlcSpaHdr);		//	486

						WRITE_STREAM_TABLE(PlcSpaHdr, oPlcfSpa);
					}
				}
			}

			if (m_oartStorage->Compile())
			{
				///////////////////////////////////////////////////////
				// ONLY FOR TEST SUB ELEMENTS
				///////////////////////////////////////////////////////
				//CAtlFile oFile;
				//if (SUCCEEDED(oFile.Create(L"d:\\office_art.dat", GENERIC_READ, FILE_SHARE_READ, OPEN_ALWAYS)))
				//{
				//	ULONGLONG size = 0;
				//	if (SUCCEEDED(oFile.GetSize(size)))
				//	{
				//		if (size)
				//		{
				//			unsigned char* pBuffer = new unsigned char [size];
				//			if (pBuffer)
				//			{
				//				oFile.Read (pBuffer, size);
				//				oFile.Close ();

				//				STATSTG oStatus;	
				//				if (SUCCEEDED(m_pTableStream->Stat (&oStatus, STATFLAG_NONAME)) )
				//				{
				//					unsigned int nStPos	=	oStatus.cbSize.LowPart;
				//					unsigned long nBufW	=	0L;
				//					unsigned int nSize	=	size;

				//					STREAMS::CSWordWriter::Instance()->Write (FIB_OFFSET::fcDggInfo, &nStPos, sizeof(unsigned int));
				//					STREAMS::CSWordWriter::Instance()->Write (FIB_OFFSET::lcbDggInfo, &nSize, sizeof(unsigned int));
				//					Write (m_pTableStream, nStPos, pBuffer, nSize, &nBufW);
				//	
				//					return hr;
				//				}
				//			}
				//		}
				//	}
				//}				
				///////////////////////////////////////////////////////

				unsigned int lcbDggInfo = m_oartStorage->Size();
				if (lcbDggInfo)
				{
					{
						unsigned int fcDggInfo	=	m_pTableStream->size();

						WRITE_STREAM_WORD(fcDggInfo);		//	554
						WRITE_STREAM_WORD(lcbDggInfo);		//	558

						WRITE_STREAM_TABLE(DggInfo, m_oartStorage->Get());
					}
				}
			}
		}

		return hr;
	}
}

namespace ASCDocFileFormat
{
	long CDocFile::SaveToFile (const CString& sFileName)
	{
		if (sFileName.GetLength() < 1) return S_FALSE;
	
		RELEASEOBJECT(m_pIStorage);
		m_pIStorage = new POLE::Storage(sFileName);

		if (m_pIStorage == NULL) return S_FALSE;
			
		if (m_pIStorage->open(true,true) == false) return S_FALSE;

		long hr = S_OK;
		
		STREAMS::CSWordWriter* pBin	= STREAMS::CSWordWriter::Instance();
		if (NULL == pBin)	return S_FALSE;

		if (FAILED(pBin->Init (m_pIStorage, TEXT_OFFSET_POSITION)))	return S_FALSE;

		InitStream(L"1Table", m_pTableStream, false);

		/*

		TODO : сделать нормально копирование параметров из исходника (пока будут пустые поля в настройках файла Property Summary)			

		http://msdn.microsoft.com/ru-ru/library/windows/desktop/aa380376(v=vs.85).aspx
		http://msdn.microsoft.com/ru-ru/library/windows/desktop/aa380376(v=vs.85).aspx
		http://msdn.microsoft.com/ru-ru/library/windows/desktop/aa379016(v=vs.85).aspx
		http://msdn.microsoft.com/ru-ru/library/windows/desktop/aa380387(v=vs.85).aspx
		http://msdn.microsoft.com/ru-ru/library/windows/desktop/aa380326(v=vs.85).aspx

		*/

		// InitStream(L"/\05SummaryInformation", m_pSummaryInformationStream, false);
		InitStream(L"\05DocumentSummaryInformation", m_pDocumentSummaryInformationStream, false);

		WriteInformationBlock ();
		WriteDocument ();
		WriteSectionProperties();	//	202
		WriteFontTable ();			//	274
		WriteStyleSheet();			//	162
		WriteNumbering ();			//	746
		WriteOfficeDrawings ();		//	474,482,554

		// End
		{
			Bool32 cbMac	=	(pBin->Get())->size();	
			WRITE_FIELD(FIB_OFFSET::cbMac,	cbMac,	sizeof(Bool32));
		}

		/*

		//// TODO : заполнить данные по умолчанию (обязательная структура по спецификации)
		//ReloadFromFileBuffer (L"C:\\dop.dat", FIB_OFFSET::fcDop, FIB_OFFSET::lcbDop);

		//// TODO : заполнить стрим по спецификации
		//POLE::Stream* pCompObj = NULL;
		//if(InitStream(L"/1CompObj", pCompObj, false))
		//{
		//	ReloadStreamFileBuffer (L"C:\\[1]CompObj", pCompObj);
		//	RELEASEINTERFACE(pCompObj);
		//}

		*/

		BinaryStorageSingleton* binaryStorage = BinaryStorageSingleton::Instance();
		if (binaryStorage)
		{
			binaryStorage->BindToStorage(m_pIStorage, "Data");
			binaryStorage->WriteData();
		}

		// Custom Stream

		//POLE::Stream* pBinTeamLab	=	NULL;
		//if(InitStream (L"/0DOCXINFO", pBinTeamLab, false))
		//{
		//	CAtlFile oFile;
		//	if (SUCCEEDED(oFile.Create (L"E:\\Editor.jpg", GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
		//	{
		//		ULONGLONG size = 0;
		//		if (SUCCEEDED(oFile.GetSize(size)))
		//		{
		//			if (size)
		//			{
		//				unsigned char* pBuffer = new unsigned char [size];
		//				if (pBuffer)
		//				{
		//					oFile.Read (pBuffer, size);
		//					oFile.Close ();
		//
		//					STATSTG oStatus;
		//					if (SUCCEEDED(pBinTeamLab->Stat (&oStatus, STATFLAG_NONAME)))
		//					{
		//						unsigned long nBufW	= 0L;
		//						Write (pBinTeamLab, 0, pBuffer, size, &nBufW);
		//					}
		//
		//					RELEASEARRAYOBJECTS(pBuffer);
		//				}
		//			}
		//		}
		//	}
		//}				

		m_pTableStream->flush();
		m_pDocumentSummaryInformationStream->flush();
		
		pBin	=	STREAMS::CSWordWriter::Instance();
		if (pBin)
		{
			pBin->Get()->flush();
			pBin->FreeInstance();
		}		
		m_pIStorage->close();

		return S_OK;
	}


	bool CDocFile::InitStream (const CStringA& stName, POLE::Stream*& pStream, bool bDefaultSizes)
	{
		RELEASEOBJECT(pStream);

		pStream = new POLE::Stream(m_pIStorage,string2std_string(stName),true);
		//long hr = m_pIStorage->OpenStream (stName, NULL, STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE, NULL, &pStream);
		if (pStream)
		{
			if (bDefaultSizes)
			{
				pStream->setSize(0);
			}
		}
		if (pStream)return true;

		return false;
	}

	long CDocFile::Write (POLE::Stream* stream, unsigned long position, const void* data, ULONG size, ULONG* writtenSize)
	{
		long hr = S_FALSE;

		if ( ( stream != NULL ) && ( data != NULL ) )
		{
			stream->seek( position/*, STREAM_SEEK_SET, NULL*/ );
			*writtenSize	=	stream->write( (unsigned char*)data, size );
		}

		return hr;
	}

	long CDocFile::Write (POLE::Stream* stream, unsigned long position, const void* data, ULONG size)
	{
		long hr	= S_FALSE;

		if ( ( stream != NULL ) && ( data != NULL ) )
		{
			ULONG writtenSize = 0;

			stream->seek( position/*, STREAM_SEEK_SET, NULL*/ );
			writtenSize	=	stream->write( (unsigned char*)data, size );
		}

		return hr;
	}

	long CDocFile::ReloadFromFileBuffer (CString strFileData, DWORD dwOffTbID, DWORD dwSizefTbID)
	{
		long hr	= S_FALSE;

		std::ifstream oFile;
		oFile.open(strFileData.GetBuffer(), std::iostream::binary);
		if (oFile.is_open())
		{
			__int64 size = 0;
			oFile.seekg (0, ios::end);
			size = oFile.tellg();
			oFile.seekg (0, ios::beg);
			if (size>0)
			{
				unsigned char* pBuffer = new unsigned char [(DWORD)size];
				if (pBuffer)
				{
					oFile.read((char*)pBuffer, (DWORD)size);
					oFile.close ();

					{
						unsigned int nStPos	=	m_pTableStream->size();
						unsigned long nBufW	=	0L;
						unsigned int nSize	=	(unsigned int)size;

						hr	=	STREAMS::CSWordWriter::Instance()->Write (dwOffTbID, &nStPos, sizeof(unsigned int));
						hr	=	STREAMS::CSWordWriter::Instance()->Write (dwSizefTbID, &nSize, sizeof(unsigned int));

						ULONG writtenSize = 0;

						m_pTableStream->seek(nStPos/*, STREAM_SEEK_SET, NULL*/);
						writtenSize	=	m_pTableStream->write(pBuffer, nSize);
					}

					delete [] pBuffer;
				}
			}
		}

		return hr;
	}

	long CDocFile::ReloadStreamFileBuffer (CString strFileData, POLE::Stream* pStream)
	{
		long hr	= S_FALSE;

		std::ifstream oFile;
		oFile.open(strFileData.GetBuffer(), std::iostream::binary);
		if (oFile.is_open())
		{
			__int64 size = 0;
			oFile.seekg (0, ios::end);
			size = oFile.tellg();
			oFile.seekg (0, ios::beg);
			if (size>0)
			{
				unsigned char* pBuffer = new unsigned char [(DWORD)size];
				if (pBuffer)
				{
					oFile.read ((char*)pBuffer, (DWORD)size);
					oFile.close();

					{
						unsigned long nBufW	=	0L;
						unsigned long nSize	=	(unsigned long)size;

						ULONG writtenSize = 0;
						pStream->seek(0);
						writtenSize	=	pStream->write(pBuffer, nSize);
					}

					delete [] pBuffer;
				}
			}
		}

		return hr;
	}
}