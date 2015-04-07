
#include "WordDocument.h"

namespace DocFileFormat
{
	WordDocument::WordDocument (CString sFileName) : m_sFileName (sFileName), m_PieceTable(NULL), 
		WordDocumentStream(NULL), TableStream(NULL), DataStream(NULL),  FIB(NULL), 
		Text(NULL), RevisionAuthorTable(NULL), FontTable(NULL), BookmarkNames(NULL), AutoTextNames(NULL), 
		IndividualFootnotesPlex(NULL), FootnoteReferenceCharactersPlex(NULL), IndividualEndnotesPlex(NULL),
		EndnoteReferenceCharactersPlex(NULL), FieldsPlex(NULL), FootnoteDocumentFieldsPlex(NULL),
		EndnoteDocumentFieldsPlex(NULL), HeadersAndFootersDocumentFieldsPlex(NULL), HeaderStoriesPlex(NULL),
		AnnotationsReferencePlex(NULL), IndividualCommentsPlex(NULL), TextboxBreakPlex(NULL), TextboxBreakPlexHeader(NULL),
		OfficeDrawingPlex(NULL), OfficeDrawingPlexHeader(NULL), SectionPlex(NULL), BookmarkStartPlex(NULL), BookmarkEndPlex(NULL),
		AutoTextPlex(NULL), AllPapxFkps(NULL), AllChpxFkps(NULL), AllPapx(NULL), AllPapxVector(NULL), AllSepx(NULL), Styles(NULL), listTable(NULL),
		AnnotationOwners(NULL), DocProperties(NULL), listFormatOverrideTable(NULL), headerAndFooterTable(NULL)	
	{
		m_pCallFunc			=	NULL;

		m_pStorage			=	NULL;
		m_pOfficeArtContent	=	NULL;

		fibFC				=	0;
	}

	WordDocument::~WordDocument()
	{
		Clear();  
	}
}

namespace DocFileFormat
{
	long WordDocument::LoadDocument(const ProgressCallback* pCallFunc)
	{
		HRESULT hr		=	S_FALSE;

		m_pCallFunc	=	pCallFunc;
		hr			=	Parse (fibFC);

		return hr;
	}

	void WordDocument::Clear()
	{
		if (AllPapxFkps)
		{
			for (list<FormattedDiskPagePAPX*>::iterator iter = AllPapxFkps->begin(); iter != AllPapxFkps->end(); ++iter)
			{
				RELEASEOBJECT(*iter);
			}

			RELEASEOBJECT(AllPapxFkps);
		}

		if (AllChpxFkps)
		{
			for (list<FormattedDiskPageCHPX*>::iterator iter = AllChpxFkps->begin(); iter != AllChpxFkps->end(); ++iter)
			{
				RELEASEOBJECT(*iter);
			}

			RELEASEOBJECT(AllChpxFkps);
		}

		if (AllSepx)
		{
			for (map<int,SectionPropertyExceptions*>::iterator iter = AllSepx->begin(); iter != AllSepx->end(); ++iter)
			{
				RELEASEOBJECT(iter->second);  
			}

			RELEASEOBJECT(AllSepx);
		}

		RELEASEOBJECT(Text);
		RELEASEOBJECT(m_PieceTable);

		RELEASEOBJECT(FIB);
		RELEASEOBJECT(RevisionAuthorTable);
		RELEASEOBJECT(FontTable);
		RELEASEOBJECT(BookmarkNames);
		RELEASEOBJECT(AutoTextNames);
		RELEASEOBJECT(IndividualFootnotesPlex);
		RELEASEOBJECT(FootnoteReferenceCharactersPlex);
		RELEASEOBJECT(IndividualEndnotesPlex);
		RELEASEOBJECT(EndnoteReferenceCharactersPlex);
		RELEASEOBJECT(FieldsPlex);
		RELEASEOBJECT(FootnoteDocumentFieldsPlex);
		RELEASEOBJECT(EndnoteDocumentFieldsPlex);
		RELEASEOBJECT(HeadersAndFootersDocumentFieldsPlex);
		RELEASEOBJECT(HeaderStoriesPlex);
		RELEASEOBJECT(IndividualCommentsPlex);
		RELEASEOBJECT(AnnotationsReferencePlex);
		RELEASEOBJECT(TextboxBreakPlex);
		RELEASEOBJECT(TextboxBreakPlexHeader);
		RELEASEOBJECT(OfficeDrawingPlex);
		RELEASEOBJECT(OfficeDrawingPlexHeader);
		RELEASEOBJECT(TextboxBreakPlexHeader);
		RELEASEOBJECT(SectionPlex);
		RELEASEOBJECT(BookmarkStartPlex);
		RELEASEOBJECT(BookmarkEndPlex);
		RELEASEOBJECT(AutoTextPlex);
		RELEASEOBJECT(Styles);
		RELEASEOBJECT(listTable);
		RELEASEOBJECT(AllPapx);
		RELEASEOBJECT(AllPapxVector);
		RELEASEOBJECT(AnnotationOwners);
		RELEASEOBJECT(DocProperties);
		RELEASEOBJECT(listFormatOverrideTable);
		RELEASEOBJECT(headerAndFooterTable);

		RELEASEOBJECT(m_pStorage);
		RELEASEOBJECT(m_pOfficeArtContent);

		RELEASEOBJECT(DataStream);
		RELEASEOBJECT(TableStream);
		RELEASEOBJECT(WordDocumentStream);
	}

	long WordDocument::Parse(int fibFC)
	{
		bool res = false;

		m_pStorage	=	new StructuredStorageReader();

		if (m_pStorage)
		{
			if (m_pStorage->SetFile (m_sFileName.GetBuffer()) == false)
			{
				Clear();
                return S_FALSE;
			}
		}
		if (m_pStorage->GetStream ("WordDocument", &WordDocumentStream) == false)
		{
			Clear();
            return S_FALSE;
		}

		// Parse FIB
		FIB	 =	new FileInformationBlock(VirtualStreamReader(WordDocumentStream,0));

#ifdef _DEBUG
		//if (Fib2007 == FIB->m_FibBase.nFib)			ATLTRACE (_T("NOTE: OFFICE 2007 file format\n"));
		//else if (Fib2003 == FIB->m_FibBase.nFib)	ATLTRACE (_T("NOTE: OFFICE 2003 file format\n"));
		//else if (Fib2002 == FIB->m_FibBase.nFib)	ATLTRACE (_T("NOTE: OFFICE 2002 file format\n"));
		//else if (Fib2000 == FIB->m_FibBase.nFib)	ATLTRACE (_T("NOTE: OFFICE 2000 file format\n"));
		//else if (Fib1997 == FIB->m_FibBase.nFib)	ATLTRACE (_T("NOTE: OFFICE 1997 file format\n"));
		//else if (Fib1997 < FIB->m_FibBase.nFib)		ATLTRACE (_T("ERROR: OFFICE file format - UNSUPPORTED\n"));
#endif

		// Check the file version
		if (FIB->m_FibBase.nFib)
		{
			//	nFib (2 bytes): An unsigned integer that specifies the version number of the file format used. 
			//	Superseded by FibRgCswNew.nFibNew if it is present. This value SHOULD<12> be 0x00C1.

			//	<12> Section 2.5.2: A special empty document is installed with Word 97, Word 2000, Word 2002, 
			//	and Office Word 2003 to allow "Create New Word Document" from the operating system. This 
			//	document has an nFib of 0x00C0. In addition the BiDi build of Word 97 differentiates its documents 
			//	by saving 0x00C2 as the nFib. In both cases treat them as if they were 0x00C1. 

			//	FIB version written. This will be >= 101 for all Word 6.0 for Windows and after documents

			if (FIB->m_FibBase.nFib < Fib1997)
			{
				Clear();
                return S_FALSE;
			}
		}
		else
		{
			if (FIB->m_FibNew.nFibNew < Fib1997)
			{
				Clear();
                return S_FALSE;
			}
		}

		// Get the streams
		if (FIB->m_FibBase.fWhichTblStm)
		{
			res	=	m_pStorage->GetStream ("1Table", &TableStream);
		}
		else
		{
			res	=	m_pStorage->GetStream ("0Table", &TableStream);
		}

		if (res == false)
		{
			Clear();
            return S_FALSE;
		}

		try
		{
			m_pStorage->GetStream ("Data", &DataStream);
		}
		catch (...)
		{
			DataStream = NULL;
		}

		//Read all needed STTBs
		RevisionAuthorTable	=	new StringTable<WideString>(TableStream, FIB->m_FibWord97.fcSttbfRMark, FIB->m_FibWord97.lcbSttbfRMark );
		FontTable			=	new StringTable<FontFamilyName>(TableStream, FIB->m_FibWord97.fcSttbfFfn, FIB->m_FibWord97.lcbSttbfFfn );
		BookmarkNames		=	new StringTable<WideString>(TableStream, FIB->m_FibWord97.fcSttbfBkmk, FIB->m_FibWord97.lcbSttbfBkmk );
		AutoTextNames		=	new StringTable<WideString>(TableStream, FIB->m_FibWord97.fcSttbfGlsy, FIB->m_FibWord97.lcbSttbfGlsy );

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress (m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 100000);

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 100000, &bCancel);

			if (bCancel)
			{
				Clear();
				return S_FALSE;
			}
		}

		// Read all needed PLCFs
		if (FIB->m_RgLw97.ccpFtn > 0)
		{
			IndividualFootnotesPlex			=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcffndTxt, FIB->m_FibWord97.lcbPlcffndTxt );
			FootnoteReferenceCharactersPlex	=	new Plex<FootnoteDescriptor>(FootnoteDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcffndRef, FIB->m_FibWord97.lcbPlcffndRef );
		}

		if (FIB->m_RgLw97.ccpEdn > 0)
		{
			IndividualEndnotesPlex			=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcfendTxt, FIB->m_FibWord97.lcbPlcfendTxt );
			EndnoteReferenceCharactersPlex	=	new Plex<EndnoteDescriptor>(EndnoteDescriptor::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfendRef, FIB->m_FibWord97.lcbPlcfendRef );
		}

		if (FIB->m_RgLw97.ccpHdr > 0)
		{
			HeaderStoriesPlex				=	new Plex<EmptyStructure>( EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfHdd, FIB->m_FibWord97.lcbPlcfHdd );
		}

		if (FIB->m_RgLw97.ccpAtn > 0)
		{
			IndividualCommentsPlex			=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfandTxt, FIB->m_FibWord97.lcbPlcfandTxt);
			AnnotationsReferencePlex		=	new Plex<AnnotationReferenceDescriptor>(AnnotationReferenceDescriptor::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfandRef, FIB->m_FibWord97.lcbPlcfandRef);
		}

		TextboxBreakPlex					=	new Plex<Tbkd>				(Tbkd::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcfTxbxBkd,	FIB->m_FibWord97.lcbPlcfTxbxBkd);
		TextboxBreakPlexHeader				=	new Plex<Tbkd>				(Tbkd::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcfTxbxHdrBkd, FIB->m_FibWord97.lcbPlcfTxbxHdrBkd);
		OfficeDrawingPlex					=	new Plex<Spa>				(Spa::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcSpaMom,		FIB->m_FibWord97.lcbPlcSpaMom);
		OfficeDrawingPlexHeader				=	new Plex<Spa>				(Spa::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcSpaHdr,		FIB->m_FibWord97.lcbPlcSpaHdr);
		SectionPlex							=	new Plex<SectionDescriptor>	(SectionDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfSed,		FIB->m_FibWord97.lcbPlcfSed);
		BookmarkStartPlex					=	new Plex<BookmarkFirst>		(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfBkf,		FIB->m_FibWord97.lcbPlcfBkf);
		BookmarkEndPlex						=	new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfBkl,		FIB->m_FibWord97.lcbPlcfBkl);

		// только для тестов
		//m_arFTBXS	=	new Plex<FTXBXS> (FTXBXS::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcftxbxTxt,	FIB->m_FibWord97.lcbPlcftxbxTxt);

		//#ifdef _DEBUG
		//		DumpBuffer(L"C:\\office_spa.dat", VirtualStreamReader(TableStream), FIB->m_FibWord97.fcPlcSpaMom, FIB->m_FibWord97.lcbPlcSpaMom);
		//#endif

		for (size_t i = 0; i < BookmarkStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark		=	static_cast<BookmarkFirst*>(BookmarkStartPlex->Elements[i]);
			if (pBookmark)
			{
				BookmarkStartEndCPs.push_back(make_pair(BookmarkStartPlex->CharacterPositions[i], BookmarkEndPlex->CharacterPositions[pBookmark->GetIndex()]));
			}
		}

		AutoTextPlex						=	new Plex<EmptyStructure> (EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfGlsy,   FIB->m_FibWord97.lcbPlcfGlsy);
		FieldsPlex							=	new Plex<FieldCharacter> (FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldMom, FIB->m_FibWord97.lcbPlcfFldMom);
		FootnoteDocumentFieldsPlex			=	new Plex<FieldCharacter> (FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldFtn, FIB->m_FibWord97.lcbPlcfFldFtn);
		EndnoteDocumentFieldsPlex			=	new Plex<FieldCharacter> (FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldEdn, FIB->m_FibWord97.lcbPlcfFldEdn);
		HeadersAndFootersDocumentFieldsPlex	=	new Plex<FieldCharacter> (FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldHdr, FIB->m_FibWord97.lcbPlcfFldHdr);

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 200000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 200000, &bCancel);

			if (bCancel)
			{
				Clear();
				return S_FALSE;
			}
		}

		// Read the FKPs
		AllPapxFkps				=	FormattedDiskPagePAPX::GetAllPAPXFKPs (FIB, WordDocumentStream, TableStream, DataStream);
		AllChpxFkps				=	FormattedDiskPageCHPX::GetAllCHPXFKPs (FIB, WordDocumentStream, TableStream);

		// Read custom tables
		DocProperties			=	new WordDocumentProperties	(FIB, TableStream);
		Styles					=	new StyleSheet				(FIB, TableStream, DataStream);
		listTable				=	new ListTable				(FIB, TableStream);
		listFormatOverrideTable	=	new ListFormatOverrideTable	(FIB, TableStream);
		m_pOfficeArtContent		=	new OfficeArtContent		(FIB, TableStream);
		headerAndFooterTable	=	new HeaderAndFooterTable	(FIB, TableStream);
		AnnotationOwners		=	new AnnotationOwnerList		(FIB, TableStream);

#ifdef _DEBUG
		//if (FIB->m_FibWord97.lcbDop)
		//	DumpBuffer(L"C:\\dop.dat", VirtualStreamReader(TableStream), FIB->m_FibWord97.fcDop, FIB->m_FibWord97.lcbDop);
#endif

#ifdef _DEBUG
		//DumpBuffer(L"d:\\office_art.dat", VirtualStreamReader(TableStream), FIB->m_FibWord97.fcDggInfo, FIB->m_FibWord97.lcbDggInfo);
#endif
		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 300000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 300000, &bCancel);

			if (bCancel)
			{
				Clear();
				return S_FALSE;
			}
		}

		// Parse the piece table and construct a list that contains all chars
		m_PieceTable	= new PieceTable (FIB, TableStream);
		Text			= m_PieceTable->GetAllEncodingText (WordDocumentStream);

#ifdef _DEBUG
		if (0)
		{
			//ATLTRACE (L"\n=========================================================\n");
			for (size_t j = 0; j < Text->size(); ++j) 
			{
				//ATLTRACE (L"%c", Text->operator [](j));
			}
			//ATLTRACE (L"\n=======================================================\n");
		}
#endif
		if (BookmarkNames)
		{
			for (unsigned int i = 0; i < BookmarkStartEndCPs.size(); ++i)
			{
				WideString* bookmarkName = static_cast<WideString*>(BookmarkNames->operator[]( i ));

				if ( ( bookmarkName != NULL ) && ( *bookmarkName == _T( "_PictureBullets" ) ) )
				{
					for (unsigned int j = BookmarkStartEndCPs[i].first, k = 0; j < this->Text->size(); ++j, ++k )
					{
						if ( this->Text->at( j ) == 1 )
						{
							PictureBulletsCPsMap.insert (make_pair( k, j ));
						}
					}
				}
			}
		}

		// Build a dictionaries of all PAPX
		AllPapx			=	new map<int, ParagraphPropertyExceptions*>();
		AllPapxVector	=	new vector<int>();

		for (list<FormattedDiskPagePAPX*>::iterator iter = AllPapxFkps->begin(); iter != AllPapxFkps->end(); ++iter)
		{
			for (unsigned int j = 0; j < (*iter)->grppapxSize; ++j)
			{
				int nVal = (*iter)->rgfc[j];
				AllPapx->insert( pair<int, ParagraphPropertyExceptions*>( nVal, (*iter)->grppapx[j] ) );
				AllPapxVector->push_back(nVal);
			}
		}

		std::sort (AllPapxVector->begin(), AllPapxVector->end());

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 400000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 400000, &bCancel);

			if (bCancel)
			{
				Clear();
				return S_FALSE;
			}
		}

		//build a dictionary of all SEPX
		if ( !SectionPlex->Elements.empty() )
		{
			AllSepx = new map<int, SectionPropertyExceptions*>();

			for (unsigned int i = 0; i < SectionPlex->Elements.size(); ++i)
			{
				//Read the SED
				SectionDescriptor* sed = static_cast<SectionDescriptor*>(SectionPlex->Elements.at(i));
				int cp = SectionPlex->CharacterPositions[i + 1];

				//Get the SEPX
				VirtualStreamReader wordReader( this->WordDocumentStream, sed->fcSepx );

				//!!!TODO: cbSepx is the size in bytes of the rest properties part!!!
				short cbSepx	=	wordReader.ReadInt16();
				unsigned char* bytes		=	wordReader.ReadBytes( ( cbSepx /*- 2*/ ), true );

				AllSepx->insert( pair<int, SectionPropertyExceptions*>( cp, new SectionPropertyExceptions( bytes, ( cbSepx /*- 2*/ ) ) ) );

				RELEASEARRAYOBJECTS( bytes );
			}
		}

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 500000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 500000, &bCancel);

			if (bCancel)
			{
				Clear();
				return S_FALSE;
			}
		}

		return S_OK;
	}

	// Returns a list of all CHPX which are valid for the given FCs.

	std::vector<int>* WordDocument::GetFileCharacterPositions(int fcMin, int fcMax)
	{
		vector<int>* intVector = new vector<int>();

		int i = 0;

		for (std::list<FormattedDiskPageCHPX*>::iterator iter = AllChpxFkps->begin(); iter != AllChpxFkps->end(); ++iter)
		{
			FormattedDiskPageCHPX *fkp = (*iter);

			//if the last fc of this fkp is smaller the fcMin
			//this fkp is before the requested range
			if ( fkp->rgfc[fkp->rgfcSize - 1] < fcMin )
			{
				continue;
			}

			//if the first fc of this fkp is larger the Max
			//this fkp is beyond the requested range
			if ( fkp->rgfc[0] > fcMax )
			{
				break;
			}

			//don't add the duplicated values of the FKP boundaries (Length-1)
			int max = fkp->rgfcSize - 1;

			//last fkp? 
			//use full table
			if ( i++ == ( this->AllChpxFkps->size() - 1 ) )
			{
				max = fkp->rgfcSize;
			}

			for (int j = 0; j < max; ++j)
			{
				if ( ( fkp->rgfc[j] < fcMin ) && ( fkp->rgfc[j + 1] > fcMin ) )
				{
					//this chpx starts before fcMin
					intVector->push_back( fkp->rgfc[j] );
				}
				else if ( ( fkp->rgfc[j] >= fcMin ) && ( fkp->rgfc[j] < fcMax ) )
				{
					//this chpx is in the range
					intVector->push_back( fkp->rgfc[j] );
				}
			}
		}

		if ( intVector->empty() )
		{
			RELEASEOBJECT( intVector );
		}

		return intVector;
	}

	// Returnes a list of all CharacterPropertyExceptions which correspond to text 

	std::list<CharacterPropertyExceptions*>* WordDocument::GetCharacterPropertyExceptions(int fcMin, int fcMax)
	{
		std::list<CharacterPropertyExceptions*>* cpeList = new list<CharacterPropertyExceptions*>();

		for (std::list<FormattedDiskPageCHPX*>::iterator iter = AllChpxFkps->begin(); iter != AllChpxFkps->end(); ++iter)
		{
			FormattedDiskPageCHPX *fkp = (*iter);      

			// get the CHPX

			for (unsigned int j = 0; j < fkp->grpchpxSize; ++j)
			{
				if ( ( fkp->rgfc[j] < fcMin ) && ( fkp->rgfc[j + 1] > fcMin ) )
				{
					//this chpx starts before fcMin
					cpeList->push_back( fkp->grpchpx[j] );
				}
				else if ( ( fkp->rgfc[j] >= fcMin ) && ( fkp->rgfc[j] < fcMax ) )
				{
					//this chpx is in the range
					cpeList->push_back( fkp->grpchpx[j] );
				}
			}
		}

		if (cpeList->empty())
		{
			RELEASEOBJECT(cpeList);
		}

		return cpeList;
	}
}
