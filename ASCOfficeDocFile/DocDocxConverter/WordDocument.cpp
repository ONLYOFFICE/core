/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "WordDocument.h"

#include "../../Common/OfficeFileErrorDescription.h"

#include "../../ASCOfficeXlsFile2/source/XlsFormat/Logic/SummaryInformationStream/SummaryInformation.h"
#include "../../ASCOfficeXlsFile2/source/XlsFormat/Binary/CFStream.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

namespace DocFileFormat
{
	WordDocument::WordDocument (const ProgressCallback* pCallFunc, const std::wstring & sTempFolder ) :	
		m_PieceTable(NULL), WordDocumentStream(NULL), TableStream(NULL), DataStream(NULL),  FIB(NULL), 
		Text(NULL), RevisionAuthorTable(NULL), FontTable(NULL), BookmarkNames(NULL), AutoTextNames(NULL), 
		IndividualFootnotesPlex(NULL), FootnoteReferenceCharactersPlex(NULL), IndividualEndnotesPlex(NULL),
		EndnoteReferenceCharactersPlex(NULL), FieldsPlex(NULL), FootnoteDocumentFieldsPlex(NULL),
		EndnoteDocumentFieldsPlex(NULL), HeadersAndFootersDocumentFieldsPlex(NULL), HeaderStoriesPlex(NULL),
		AnnotationsReferencePlex(NULL), IndividualCommentsPlex(NULL), TextboxBreakPlex(NULL), TextboxBreakPlexHeader(NULL),
		TextboxIndividualPlex(NULL),AssocNames(NULL), BookmarkAnnotNames(NULL), Captions(NULL), AutoCaptions(NULL), ListPlex(NULL),
		OfficeDrawingPlex(NULL), OfficeDrawingPlexHeader(NULL), SectionPlex(NULL), BookmarkStartPlex(NULL), BookmarkEndPlex(NULL),
		AutoTextPlex(NULL), AllPapxFkps(NULL), AllChpxFkps(NULL), AllPapx(NULL), AllPapxVector(NULL), AllSepx(NULL), Styles(NULL), listTable(NULL),
		AnnotationOwners(NULL), DocProperties(NULL), listFormatOverrideTable(NULL), headerAndFooterTable(NULL),
		AnnotStartPlex(NULL), AnnotEndPlex(NULL), encryptionHeader(NULL)
	{
		m_pCallFunc			=	pCallFunc;	
		m_sTempFolder		=	sTempFolder;
		
		m_pStorage			=	NULL;
		officeArtContent	=	NULL;
		bOlderVersion		=	false;
	}

	WordDocument::~WordDocument()
	{
		Clear();

		if (!m_sTempDecryptFileName.empty())
		{
			NSFile::CFileBinary::Remove(m_sTempDecryptFileName);
		}
	}
}

namespace DocFileFormat
{
	long WordDocument::LoadDocument(const std::wstring & fileName, const std::wstring & password)
	{
		m_sFileName = fileName;
		m_sPassword = password;

		m_pStorage	=	new StructuredStorageReader();

		if (!m_pStorage)
		{
			Clear();
            return AVS_ERROR_FILEFORMAT;
		}

		if (m_pStorage->SetFile (m_sFileName.c_str()) == false)
		{
			Clear();
            return AVS_ERROR_FILEFORMAT;
		}
//-----------------------------------------------------------------------------------------------------------------
		if (m_pStorage->GetStream (L"WordDocument", &WordDocumentStream) == false)
		{
			Clear();
            return AVS_ERROR_FILEFORMAT;
		}
//-------------------------------------------------------------------------------------------------------------------
		FIB	 =	new FileInformationBlock(VirtualStreamReader(WordDocumentStream,0, false ));

		if (FIB->m_FibBase.nFib)
		{
            if (FIB->m_FibBase.nFib <= Fib1995)
			{
				bOlderVersion = FIB->m_bOlderVersion = true;
			}
		}
		else
		{
            if (FIB->m_FibNew.nFibNew <= Fib1995 && FIB->m_FibNew.nFibNew > 0)
			{
				bOlderVersion = FIB->m_bOlderVersion = true;
			}
		}
		bool res = false;

		if (FIB->m_FibBase.fWhichTblStm)
		{
			if (!m_pStorage->GetStream (L"1Table", &TableStream))
			{
				res	=	m_pStorage->GetStream (L"0Table", &TableStream);
			}
		}
		else
		{
			if (!m_pStorage->GetStream (L"0Table", &TableStream))
			{
				res	=	m_pStorage->GetStream (L"1Table", &TableStream);
			}
		}

		if (FIB->m_FibBase.fEncrypted && !FIB->m_bOlderVersion)
		{
			encryptionHeader	=	new EncryptionHeader	(FIB, TableStream);

			if (encryptionHeader->bStandard)
			{
				CRYPT::RC4Decryptor Decryptor(encryptionHeader->crypt_data_rc4, m_sPassword);

				if (Decryptor.IsVerify() == false) 
				{
					Clear();

					if (m_sPassword.empty() )	return AVS_ERROR_DRM;
					else						return AVS_ERROR_PASSWORD;
				}
				
				if (DecryptOfficeFile(&Decryptor) == false)	return AVS_ERROR_DRM;
			}
			else
			{
				CRYPT::ECMADecryptor Decryptor;

				Decryptor.SetCryptData(encryptionHeader->crypt_data_aes);

				if (Decryptor.SetPassword(m_sPassword) == false)
				{
					Clear();

					if (m_sPassword.empty() )	return AVS_ERROR_DRM;
					else						return AVS_ERROR_PASSWORD;
				}
				if (DecryptOfficeFile(&Decryptor) == false)	return AVS_ERROR_DRM;
			}
			
			FIB->reset(VirtualStreamReader(WordDocumentStream, 68, false));
		}
		else if (FIB->m_FibBase.fEncrypted)  return AVS_ERROR_DRM;

//------------------------------------------------------------------------------------------------------------------
		POLE::Stream			* Summary		= NULL;
		POLE::Stream			* DocSummary	= NULL;
		
		m_pStorage->GetStream (L"SummaryInformation",			&Summary);
		m_pStorage->GetStream (L"DocumentSummaryInformation",	&DocSummary);

		document_code_page = ENCODING_WINDOWS_1250;
		
		if ((Summary) && (Summary->size() > 0))
		{
			XLS::CFStreamPtr stream = XLS::CFStreamPtr(new XLS::CFStream(Summary));
			OLEPS::SummaryInformation summary_info(stream);
			int document_code_page1 = summary_info.GetCodePage(); //from software last open 
			
			if (document_code_page1 > 0)
				document_code_page = document_code_page1;		
		}
		if ((DocSummary) && (DocSummary->size() > 0))
		{
			XLS::CFStreamPtr stream = XLS::CFStreamPtr(new XLS::CFStream(DocSummary));
			OLEPS::SummaryInformation doc_summary_info(stream);
			int document_code_page2 = doc_summary_info.GetCodePage();

			if (document_code_page2 > 0)
				document_code_page = document_code_page2;
		}
		if (!bOlderVersion)
			document_code_page = ENCODING_UTF16;

		FIB->m_CodePage =  document_code_page;
//-------------------------------------------------------------------------------------------------
		try
		{
			m_pStorage->GetStream (L"Data", &DataStream);
		}
		catch (...)
		{
			DataStream = NULL;
		}

		if (TableStream->size() < 1 && bOlderVersion)
		{
			RELEASEOBJECT(TableStream);
			m_pStorage->GetStream (L"WordDocument", &TableStream);
		}

		RevisionAuthorTable	=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfRMark,		FIB->m_FibWord97.lcbSttbfRMark,			bOlderVersion);
		FontTable			=	new StringTable<FontFamilyName>	(TableStream, FIB->m_FibWord97.fcSttbfFfn,			FIB->m_FibWord97.lcbSttbfFfn,			bOlderVersion);
		BookmarkNames		=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfBkmk,			FIB->m_FibWord97.lcbSttbfBkmk,			bOlderVersion);
		AutoTextNames		=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfGlsy,			FIB->m_FibWord97.lcbSttbfGlsy,			bOlderVersion);
		AssocNames			=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfAssoc,		FIB->m_FibWord97.lcbSttbfAssoc,			bOlderVersion);
		BookmarkAnnotNames	=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfAtnBkmk,		FIB->m_FibWord97.lcbSttbfAtnBkmk,		bOlderVersion);
		Captions			=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfCaption,		FIB->m_FibWord97.lcbSttbfCaption,		bOlderVersion);
		AutoCaptions		=	new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfAutoCaption,	FIB->m_FibWord97.lcbSttbfAutoCaption,	bOlderVersion);

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress (m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 100000);

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 100000, &bCancel);

			if (bCancel)
			{
				Clear();
				return AVS_ERROR_FILEFORMAT;
			}
		}

		// Read all needed PLCFs
		if (FIB->m_RgLw97.ccpFtn > 0)
		{
			IndividualFootnotesPlex			=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcffndTxt, FIB->m_FibWord97.lcbPlcffndTxt, bOlderVersion);
			FootnoteReferenceCharactersPlex	=	new Plex<FootnoteDescriptor>(FootnoteDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcffndRef, FIB->m_FibWord97.lcbPlcffndRef, bOlderVersion);
		}
	
		if (FIB->m_FibWord97.lcbPlcPad > 0)
		{
			OutlineListDescriptorPlex		=	new Plex<OutlineListDescriptor>(OutlineListDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcPad, FIB->m_FibWord97.lcbPlcPad, bOlderVersion);
		}

		if (FIB->m_RgLw97.ccpEdn > 0)
		{
			IndividualEndnotesPlex			=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcfendTxt, FIB->m_FibWord97.lcbPlcfendTxt, bOlderVersion);
			EndnoteReferenceCharactersPlex	=	new Plex<EndnoteDescriptor>(EndnoteDescriptor::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfendRef, FIB->m_FibWord97.lcbPlcfendRef, bOlderVersion);
		}

		if (FIB->m_RgLw97.ccpHdr > 0)
		{
			HeaderStoriesPlex				=	new Plex<EmptyStructure>( EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfHdd, FIB->m_FibWord97.lcbPlcfHdd, bOlderVersion);
		}

		if (FIB->m_RgLw97.ccpAtn > 0)
		{
			AnnotationsReferencePlex		=	new Plex<AnnotationReferenceDescriptor>(AnnotationReferenceDescriptor::GetSize(bOlderVersion), TableStream, FIB->m_FibWord97.fcPlcfandRef, FIB->m_FibWord97.lcbPlcfandRef, bOlderVersion);
			IndividualCommentsPlex			=	new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfandTxt,		FIB->m_FibWord97.lcbPlcfandTxt,		bOlderVersion);
		}
		OfficeDrawingPlex					=	new Plex<Spa>				(Spa::GetSize(bOlderVersion),		TableStream, FIB->m_FibWord97.fcPlcSpaMom,		FIB->m_FibWord97.lcbPlcSpaMom,		bOlderVersion);
		OfficeDrawingPlexHeader				=	new Plex<Spa>				(Spa::GetSize(bOlderVersion),		TableStream, FIB->m_FibWord97.fcPlcSpaHdr,		FIB->m_FibWord97.lcbPlcSpaHdr,		bOlderVersion);

		TextboxIndividualPlex				=	new Plex<FTXBXS>			(FTXBXS::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcftxbxTxt,	FIB->m_FibWord97.lcbPlcftxbxTxt,	bOlderVersion);

		SectionPlex							=	new Plex<SectionDescriptor>	(SectionDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfSed,		FIB->m_FibWord97.lcbPlcfSed,		bOlderVersion);
		
		BookmarkStartPlex					=	new Plex<BookmarkFirst>		(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfBkf,		FIB->m_FibWord97.lcbPlcfBkf,		bOlderVersion);
		BookmarkEndPlex						=	new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfBkl,		FIB->m_FibWord97.lcbPlcfBkl,		bOlderVersion);

		TextboxBreakPlex					=	new Plex<Tbkd>				(Tbkd::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcfTxbxBkd,	FIB->m_FibWord97.lcbPlcfTxbxBkd,	bOlderVersion);
		TextboxBreakPlexHeader				=	new Plex<Tbkd>				(Tbkd::STRUCTURE_SIZE,				TableStream, FIB->m_FibWord97.fcPlcfTxbxHdrBkd, FIB->m_FibWord97.lcbPlcfTxbxHdrBkd, bOlderVersion);

		AnnotStartPlex						=	new Plex<BookmarkFirst>		(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfAtnBkf,		FIB->m_FibWord97.lcbPlcfAtnBkf,		bOlderVersion);
		AnnotEndPlex							=	new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfAtnBkl,		FIB->m_FibWord97.lcbPlcfAtnBkl,		bOlderVersion);
		

		for (size_t i = 0; i < BookmarkStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark		=	static_cast<BookmarkFirst*>(BookmarkStartPlex->Elements[i]);
			if (pBookmark)
			{
				BookmarkStartEndCPs.push_back(std::make_pair(BookmarkStartPlex->CharacterPositions[i], BookmarkEndPlex->CharacterPositions[pBookmark->GetIndex()]));
			}
		}
		for (size_t i = 0; i < AnnotStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark		=	static_cast<BookmarkFirst*>(AnnotStartPlex->Elements[i]);
			if (pBookmark)
			{
				AnnotStartEndCPs.push_back(std::make_pair(AnnotStartPlex->CharacterPositions[i], AnnotEndPlex->CharacterPositions[pBookmark->GetIndex()]));
			}
		}
		AutoTextPlex						=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfGlsy,   FIB->m_FibWord97.lcbPlcfGlsy, bOlderVersion);
		FieldsPlex							=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldMom, FIB->m_FibWord97.lcbPlcfFldMom, bOlderVersion);
		FootnoteDocumentFieldsPlex			=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldFtn, FIB->m_FibWord97.lcbPlcfFldFtn, bOlderVersion);
		EndnoteDocumentFieldsPlex			=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldEdn, FIB->m_FibWord97.lcbPlcfFldEdn, bOlderVersion);
		HeadersAndFootersDocumentFieldsPlex	=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldHdr, FIB->m_FibWord97.lcbPlcfFldHdr, bOlderVersion);
		ListPlex							=	new Plex<ListNumCache>	(ListNumCache::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfBteLvc, FIB->m_FibWord97.lcbPlcfBteLvc, bOlderVersion);
		
		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 200000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 200000, &bCancel);

			if (bCancel)
			{
				Clear();
				return AVS_ERROR_FILEFORMAT;
			}
		}

// Read the FKPs
		AllPapxFkps				=	FormattedDiskPagePAPX::GetAllPAPXFKPs (FIB, WordDocumentStream, TableStream, DataStream);
		AllChpxFkps				=	FormattedDiskPageCHPX::GetAllCHPXFKPs (FIB, WordDocumentStream, TableStream);

// Read custom tables
		if (TableStream)
		{
			DocProperties			=	new WordDocumentProperties	(FIB, TableStream);
			Styles					=	new StyleSheet				(FIB, TableStream, DataStream);
			listTable				=	new ListTable				(FIB, TableStream);
			listFormatOverrideTable	=	new ListFormatOverrideTable	(FIB, TableStream);
			officeArtContent		=	new OfficeArtContent		(FIB, TableStream);
			headerAndFooterTable	=	new HeaderAndFooterTable	(FIB, TableStream);
			AnnotationOwners		=	new AnnotationOwnerList		(FIB, TableStream);
		}

		if (m_pCallFunc)
		{
			m_pCallFunc->OnProgress(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 300000 );

			SHORT bCancel = 0;
			m_pCallFunc->OnProgressEx(m_pCallFunc->caller, DOC_ONPROGRESSEVENT_ID, 300000, &bCancel);

			if (bCancel)
			{
				Clear();
				return AVS_ERROR_FILEFORMAT;
			}
		}

		if (FIB->m_FibWord97.lcbClx > 0)
		{
			FIB->m_FibBase.fComplex = true;
			// Parse the piece table and construct a list that contains all chars
			m_PieceTable	= new PieceTable (FIB, TableStream, WordDocumentStream );
			Text			= m_PieceTable->GetAllEncodingText (WordDocumentStream);
		}
		else
		{
			int cb = FIB->m_FibBase.fcMac - FIB->m_FibBase.fcMin;

			unsigned char *bytes = new unsigned char[cb];

			WordDocumentStream->seek (FIB->m_FibBase.fcMin);
			WordDocumentStream->read (bytes, cb);

			Text = new std::vector<wchar_t>();
			FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t> >(Text, bytes, cb, document_code_page);

			RELEASEARRAYOBJECTS(bytes);
		}

		if (BookmarkNames)
		{
			for (unsigned int i = 0; i < BookmarkStartEndCPs.size(); ++i)
			{
				WideString* bookmarkName = static_cast<WideString*>(BookmarkNames->operator[]( i ));

                if ( ( bookmarkName != NULL ) && ( *bookmarkName == L"_PictureBullets" ) )
				{
					for (unsigned int j = BookmarkStartEndCPs[i].first, k = 0; j < Text->size(); ++j, ++k )
					{
						if ( Text->at( j ) == 1 )
						{
							PictureBulletsCPsMap.insert (std::make_pair( k, j ));
						}
					}
				}
			}
		}

		// Build a dictionaries of all PAPX
		AllPapx			=	new std::map<int, ParagraphPropertyExceptions*>();
		AllPapxVector	=	new std::vector<int>();

		for (std::list<FormattedDiskPagePAPX*>::iterator iter = AllPapxFkps->begin(); iter != AllPapxFkps->end(); ++iter)
		{
			for (unsigned int j = 0; j < (*iter)->grppapxSize; ++j)
			{
				int nVal = (*iter)->rgfc[j];
				AllPapx->insert( std::pair<int, ParagraphPropertyExceptions*>( nVal, (*iter)->grppapx[j] ) );
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
				return AVS_ERROR_FILEFORMAT;
			}
		}

		//build a dictionary of all SEPX
		if ( !SectionPlex->Elements.empty() )
		{
			AllSepx = new std::map<int, SectionPropertyExceptions*>();

			for (unsigned int i = 0; i < SectionPlex->Elements.size(); ++i)
			{
				//Read the SED
				SectionDescriptor* sed = static_cast<SectionDescriptor*>(SectionPlex->Elements.at(i));
				int cp = SectionPlex->CharacterPositions[i + 1];

				//Get the SEPX
				VirtualStreamReader wordReader( WordDocumentStream, sed->fcSepx, bOlderVersion);

				//!!!TODO: cbSepx is the size in bytes of the rest properties part!!!
				short cbSepx	=	wordReader.ReadInt16();
				unsigned char* bytes		=	wordReader.ReadBytes( ( cbSepx /*- 2*/ ), true );

				AllSepx->insert( std::pair<int, SectionPropertyExceptions*>( cp, new SectionPropertyExceptions( bytes, ( cbSepx /*- 2*/ ), bOlderVersion ) ) );

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
				return AVS_ERROR_FILEFORMAT;
			}
		}

		return 0;
	}
	bool WordDocument::DecryptOfficeFile(CRYPT::Decryptor* Decryptor)
	{
		if (m_sTempFolder.empty())
		{
            m_sTempFolder = NSFile::CFileBinary::GetTempPath();
		}
		m_sTempDecryptFileName	= m_sTempFolder + FILE_SEPARATOR_STR + L"~tempFile.doc";
		
		POLE::Storage *storageIn	= m_pStorage->GetStorage();
		POLE::Storage *storageOut	= new POLE::Storage(m_sTempDecryptFileName.c_str());

		if (!storageOut || !storageIn) return false;
		
		if (!storageOut->open(true, true))
		{
			delete storageOut;
			return false;
		}
		DecryptStream( 0, L"/", storageIn, storageOut, Decryptor);

		//std::list<std::string> listStream = storageIn->entries();

		//for (std::list<std::string>::iterator it = listStream.begin(); it != listStream.end(); it++)
		//{
		//	if (storageIn->isDirectory(*it)) 
		//	{
		//		std::list<std::string> list_entry = storageIn->GetAllStreams(*it);
		//		
		//		for (std::list<std::string>::iterator it2 = list_entry.begin(); it2 != list_entry.end(); it2++)
		//		{
		//			DecryptStream(Decryptor, *it2, storageIn, storageOut);
		//		}
		//	}
		//	else 
		//	{
		//		DecryptStream(Decryptor, *it, storageIn, storageOut);
		//	}

		//}

		storageOut->close();
		delete storageOut;

//reset streams
		RELEASEOBJECT(TableStream);
		RELEASEOBJECT(WordDocumentStream);
		
		m_pStorage->SetFile(m_sTempDecryptFileName.c_str());
		
		if (m_pStorage->GetStream (L"WordDocument", &WordDocumentStream) == false) return false;

		if (FIB->m_FibBase.fWhichTblStm)
		{
			if (!m_pStorage->GetStream (L"1Table", &TableStream))	m_pStorage->GetStream (L"0Table", &TableStream);
		}
		else
		{
			if (!m_pStorage->GetStream (L"0Table", &TableStream))	m_pStorage->GetStream (L"1Table", &TableStream);
		}
		return true;
	}
	void WordDocument::DecryptStream( int level, std::wstring path, POLE::Storage * storageIn, POLE::Storage * storageOut, CRYPT::Decryptor* Decryptor)
	{
		std::list<std::wstring> entries, entries_files, entries_dir;
		entries = storageIn->entries_with_prefix( path );
		
		for( std::list<std::wstring>::iterator it = entries.begin(); it != entries.end(); it++ )
		{
			std::wstring name = *it;
			std::wstring fullname = path + name;
	       
			if( storageIn->isDirectory( fullname ) )
			{
				entries_dir.push_back(name);
			}
			else
			{
				entries_files.push_front(name);
			}
		}	    
		for( std::list<std::wstring>::iterator it = entries_dir.begin(); it != entries_dir.end(); ++it )
		{
			std::wstring fullname = path + *it;
	       
			DecryptStream( level + 1, fullname + L"/", storageIn, storageOut, Decryptor );

		}    
	//if (bSortFiles)
		entries_files.sort();

		for( std::list<std::wstring>::iterator it = entries_files.begin(); it != entries_files.end(); ++it )
		{
			std::wstring fullname = path + *it;
			DecryptStream(fullname, storageIn, storageOut, Decryptor );
		}  
	}
	bool WordDocument::DecryptStream(std::wstring streamName, POLE::Storage * storageIn, POLE::Storage * storageOut, CRYPT::Decryptor* Decryptor)
	{
		POLE::Stream *stream = new POLE::Stream(storageIn, streamName);
		if (!stream) return false;

		stream->seek(0);
		POLE::uint64 size_stream = stream->size();
		
		POLE::Stream *streamNew = new POLE::Stream(storageOut, streamName, true, size_stream);
		if (!streamNew) return false;

		unsigned char* data_stream = new unsigned char[size_stream];
		stream->read(data_stream, size_stream);

		unsigned char* data_store = NULL;
		int size_data_store = 0;		
		
		if ( std::wstring::npos != streamName.find(L"WordDocument") )
		{
			size_data_store = 68;
			data_store = new unsigned char[size_data_store];
		}
		
		if (data_store)
			memcpy(data_store, data_stream, size_data_store);

		size_t			size_block	= 0x200;
		unsigned long	block		= 0;

		for (POLE::uint64 pos = 0; pos < size_stream; pos += size_block, block++)
		{
			if (pos + size_block > size_stream)
				size_block = size_stream - pos;

			Decryptor->Decrypt((char*)data_stream + pos, size_block, block);
		}

		
		if (data_store)
			memcpy(data_stream, data_store, size_data_store);

		streamNew->write(data_stream, size_stream);

		RELEASEARRAYOBJECTS(data_store);
		RELEASEARRAYOBJECTS(data_stream);

		streamNew->flush();
				
		delete streamNew;
		delete stream;
		
		return true;
	}


	void WordDocument::Clear()
	{
		bOlderVersion	=	false;
		
		if (AllPapxFkps)
		{
			for (std::list<FormattedDiskPagePAPX*>::iterator iter = AllPapxFkps->begin(); iter != AllPapxFkps->end(); ++iter)
			{
				RELEASEOBJECT(*iter);
			}

			RELEASEOBJECT(AllPapxFkps);
		}

		if (AllChpxFkps)
		{
			for (std::list<FormattedDiskPageCHPX*>::iterator iter = AllChpxFkps->begin(); iter != AllChpxFkps->end(); ++iter)
			{
				RELEASEOBJECT(*iter);
			}

			RELEASEOBJECT(AllChpxFkps);
		}

		if (AllSepx)
		{
			for (std::map<int,SectionPropertyExceptions*>::iterator iter = AllSepx->begin(); iter != AllSepx->end(); ++iter)
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
		RELEASEOBJECT(AutoCaptions);
		RELEASEOBJECT(Captions);
		RELEASEOBJECT(BookmarkAnnotNames);
		RELEASEOBJECT(AssocNames);

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
		RELEASEOBJECT(TextboxIndividualPlex);
		RELEASEOBJECT(TextboxBreakPlexHeader);
		RELEASEOBJECT(SectionPlex);
		RELEASEOBJECT(BookmarkStartPlex);
		RELEASEOBJECT(BookmarkEndPlex);
		RELEASEOBJECT(AnnotStartPlex);
		RELEASEOBJECT(AnnotEndPlex);
		RELEASEOBJECT(AutoTextPlex);
		RELEASEOBJECT(ListPlex);
		RELEASEOBJECT(Styles);
		RELEASEOBJECT(listTable);
		RELEASEOBJECT(AllPapx);
		RELEASEOBJECT(AllPapxVector);
		RELEASEOBJECT(AnnotationOwners);
		RELEASEOBJECT(DocProperties);
		RELEASEOBJECT(listFormatOverrideTable);
		RELEASEOBJECT(headerAndFooterTable);
		RELEASEOBJECT(encryptionHeader);

		RELEASEOBJECT(m_pStorage);
		RELEASEOBJECT(officeArtContent);

		RELEASEOBJECT(DataStream);
		RELEASEOBJECT(TableStream);
		RELEASEOBJECT(WordDocumentStream);
	}

	// Returns a list of all CHPX which are valid for the given FCs.
	std::vector<int>* WordDocument::GetFileCharacterPositions(int fcMin, int fcMax)
	{
		std::vector<int>* intVector = new std::vector<int>();

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
			if ( i++ == ( AllChpxFkps->size() - 1 ) )
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
		std::list<CharacterPropertyExceptions*>* cpeList = new std::list<CharacterPropertyExceptions*>();

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
	std::vector<wchar_t>* WordDocument::GetChars (int fcStart, int fcEnd, int cp)
	{
		if (FIB->m_FibBase.fComplex)
		{
			return	m_PieceTable->GetChars (fcStart, fcEnd, cp, WordDocumentStream);		//<! NEED OPTIMIZE
		}
		else
		{	
			std::vector<wchar_t> *chars = new std::vector<wchar_t>();
			
			fcStart -= FIB->m_FibBase.fcMin;
			fcEnd	-= FIB->m_FibBase.fcMin;

			for (int i = fcStart; i < fcEnd; i++)
			{
				chars->push_back(Text->at(i));
			}

			return chars;
		}
	}
	std::vector<wchar_t>* WordDocument::GetEncodingChars (int fcStart, int fcEnd)
	{
		if (FIB->m_FibBase.fComplex)
		{
			return	m_PieceTable->GetEncodingChars (fcStart, fcEnd, WordDocumentStream);		//<! NEED OPTIMIZE
		}
		else
		{	
			std::vector<wchar_t> *encodingChars = new std::vector<wchar_t>();
			
			fcStart -= FIB->m_FibBase.fcMin;
			fcEnd	-= FIB->m_FibBase.fcMin;

			for (int i = fcStart; i < fcEnd; i++)
			{
				encodingChars->push_back(Text->at(i));
			}

			return encodingChars;
		}
	}

}
