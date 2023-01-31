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

#include "WordDocument.h"

#include "../../Common/OfficeFileErrorDescription.h"
#include "../../Common/MS-LCID.h"

#include "../Common/SummaryInformation/PropertySetStream.h"
#include "../XlsFile/Format/Binary/CFStream.h"

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

#include <unordered_map>

namespace DocFileFormat
{
	static const int aCodePages[][2] = {
		//charset	codepage
		0,	1252, //ANSI
		1,	0,//Default
		2,	42,//Symbol
		77,	10000,//Mac Roman
		78,	10001,//Mac Shift Jis
		79,	10003,//Mac Hangul
		80,	10008,//Mac GB2312
		81,	10002,//Mac Big5
		83,	10005,//Mac Hebrew
		84,	10004,//Mac Arabic
		85,	10006,//Mac Greek
		86,	10081,//Mac Turkish
		87,	10021,//Mac Thai
		88,	10029,//Mac East Europe
		89,	10007,//Mac Russian
		128,	932,//Shift JIS
		129,	949,//Hangul
		130,	1361,//Johab
		134,	936,//GB2312
		136,	950,//Big5
		238,	1250,//Greek
		161,	1253,//Greek
		162,	1254,//Turkish
		163,	1258,//Vietnamese
		177,	1255,//Hebrew
		178,	1256, //Arabic
		186,	1257,//Baltic
		204,	1251,//Russian
		222,	874,//Thai
		238,	1250,//Eastern European
		254,	437,//PC 437
		255,	850//OEM
	};
	WordDocument::WordDocument (const std::wstring & sTempFolder, const int userLCID) :	
		m_PieceTable(NULL), WordDocumentStream(NULL), TableStream(NULL), DataStream(NULL),  FIB(NULL), 
		Text(NULL), RevisionAuthorTable(NULL), FontTable(NULL), BookmarkNames(NULL), AutoTextNames(NULL), 
		IndividualFootnotesPlex(NULL), FootnoteReferenceCharactersPlex(NULL), IndividualEndnotesPlex(NULL),
		EndnoteReferenceCharactersPlex(NULL), FieldsPlex(NULL), FootnoteDocumentFieldsPlex(NULL),
		EndnoteDocumentFieldsPlex(NULL), HeadersAndFootersDocumentFieldsPlex(NULL), HeaderStoriesPlex(NULL), AnnotationsFieldsPlex(NULL),
		AnnotationsReferencePlex(NULL), AnnotationsReferencesEx(NULL), IndividualCommentsPlex(NULL), TextboxBreakPlex(NULL), TextboxBreakPlexHeader(NULL),
		TextboxIndividualPlex(NULL),AssocNames(NULL), BkmkAnnotNames(NULL), Captions(NULL), AutoCaptions(NULL), ListPlex(NULL),
		OfficeDrawingPlex(NULL), OfficeDrawingPlexHeader(NULL), SectionPlex(NULL), BookmarkStartPlex(NULL), BookmarkEndPlex(NULL),
		AutoTextPlex(NULL), AllPapxFkps(NULL), AllChpxFkps(NULL), AllPapx(NULL), AllPapxVector(NULL), AllSepx(NULL), Styles(NULL), listTable(NULL),
		AnnotationOwners(NULL), DocProperties(NULL), listFormatOverrideTable(NULL), headerAndFooterTable(NULL),
		AnnotStartPlex(NULL), AnnotEndPlex(NULL), encryptionHeader(NULL), BkmkProt(NULL), BkmkProtUser(NULL), BookmarkProtStartPlex(NULL), BookmarkProtEndPlex(NULL) 
	{
		m_sTempFolder		= sTempFolder;
		m_nUserLCID			= userLCID;
		
		m_pStorage			= NULL;
		officeArtContent	= NULL;
		nWordVersion		= 0;
		
		bDocumentCodePage	= false;
		nDocumentCodePage	= ENCODING_WINDOWS_1250;
		nFontsCodePage		= ENCODING_WINDOWS_1250;
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
	_UINT32 WordDocument::LoadDocument(const std::wstring & fileName, const std::wstring & password)
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
			if (false == LoadDocumentFlat())
			{
				Clear();
				return AVS_ERROR_FILEFORMAT;
			}
		}
//-----------------------------------------------------------------------------------------------------------------
		if (m_pStorage->GetStream (L"WordDocument", &WordDocumentStream) == false)
		{
			Clear();
            return AVS_ERROR_FILEFORMAT;
		}
//-------------------------------------------------------------------------------------------------------------------
		FIB	 =	new FileInformationBlock(VirtualStreamReader(WordDocumentStream, 0, false ));

		if (FIB->m_FibBase.nFib)
		{
            if (FIB->m_FibBase.nFib <= Fib1995)
			{
				nWordVersion = FIB->m_nWordVersion = 1;
				
				if (FIB->m_FibBase.nFib <= Fib1989)
				{
					nWordVersion = FIB->m_nWordVersion = 2;
				}
			}
		}
		else
		{
            if (FIB->m_FibNew.nFibNew <= Fib1995 && FIB->m_FibNew.nFibNew > 0)
			{
				nWordVersion = FIB->m_nWordVersion = 1;

				if (FIB->m_FibNew.nFibNew <= Fib1989)
				{
					nWordVersion = FIB->m_nWordVersion = 2;
				}
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

		if (FIB->m_FibBase.fEncrypted)
		{
			encryptionHeader = new EncryptionHeader	(FIB, TableStream);

			if (encryptionHeader->bXOR)
			{
				CRYPT::XORDecryptor Decryptor(1, encryptionHeader->crypt_data_xor.key, encryptionHeader->crypt_data_xor.hash, m_sPassword);

				if (Decryptor.IsVerify() == false) 
				{
					Clear();

					if (m_sPassword.empty() )	return AVS_ERROR_DRM;
					else						return AVS_ERROR_PASSWORD;
				}
				if (DecryptOfficeFile(&Decryptor) == false)	return AVS_ERROR_DRM;
			}
			else if (encryptionHeader->bStandard)
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
			else if (encryptionHeader->bAES)
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
			
			FIB->reset(VirtualStreamReader(WordDocumentStream, (nWordVersion > 0) ? 36 : 68, false));
		}

//------------------------------------------------------------------------------------------------------------------
		POLE::Stream * Summary		= NULL;
		POLE::Stream * DocSummary	= NULL;
		
		m_pStorage->GetStream (L"SummaryInformation",			&Summary);
		m_pStorage->GetStream (L"DocumentSummaryInformation",	&DocSummary);
		
		OLEPS::PropertySetStream summary_info;
		
		if ((Summary) && (Summary->size() > 0))
		{
			XLS::CFStreamPtr stream = XLS::CFStreamPtr(new XLS::CFStream(Summary));			
			summary_info.read(stream);

			int document_code_page1 = summary_info.GetCodePage(); 
			
			if (document_code_page1 > 0)
			{
				nDocumentCodePage = document_code_page1;		
				bDocumentCodePage = true;
			}
		}
		if ((DocSummary) && (DocSummary->size() > 0))
		{
			XLS::CFStreamPtr stream = XLS::CFStreamPtr(new XLS::CFStream(DocSummary));
			summary_info.read(stream, true);

			int document_code_page2 = summary_info.GetCodePage();

			if (document_code_page2 > 0)
			{
				nDocumentCodePage = document_code_page2;
				bDocumentCodePage = true;
			}
		}
		if (nWordVersion == 0)
		{
			nDocumentCodePage = ENCODING_UTF16;
			bDocumentCodePage = true;
		}

		FIB->m_CodePage =  nDocumentCodePage;

		if (!bDocumentCodePage && m_nUserLCID > 0)
		{
			int user_codepage = msLCID2DefCodePage(m_nUserLCID);
			
			if (user_codepage > 0)
			{
				bDocumentCodePage = true;
				nDocumentCodePage = user_codepage;
			}
		}
		m_sXmlApp = summary_info.GetApp();
		m_sXmlCore = summary_info.GetCore();
//-------------------------------------------------------------------------------------------------
		try
		{
			m_pStorage->GetStream (L"Data", &DataStream);
		}
		catch (...)
		{
			DataStream = NULL;
		}

		if (TableStream->size() < 1 && nWordVersion > 0)
		{
			RELEASEOBJECT(TableStream);
			m_pStorage->GetStream (L"WordDocument", &TableStream);
		}

		RevisionAuthorTable	= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfRMark,		FIB->m_FibWord97.lcbSttbfRMark,			nWordVersion);
		FontTable			= new StringTable<FontFamilyName>	(TableStream, FIB->m_FibWord97.fcSttbfFfn,			FIB->m_FibWord97.lcbSttbfFfn,			nWordVersion);
		BookmarkNames		= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfBkmk,			FIB->m_FibWord97.lcbSttbfBkmk,			nWordVersion);
		AutoTextNames		= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfGlsy,			FIB->m_FibWord97.lcbSttbfGlsy,			nWordVersion);
		AssocNames			= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfAssoc,		FIB->m_FibWord97.lcbSttbfAssoc,			nWordVersion);
		Captions			= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfCaption,		FIB->m_FibWord97.lcbSttbfCaption,		nWordVersion);
		AutoCaptions		= new StringTable<WideString>		(TableStream, FIB->m_FibWord97.fcSttbfAutoCaption,	FIB->m_FibWord97.lcbSttbfAutoCaption,	nWordVersion);

		BkmkProt			= new StringTableEx<ProtInfoBookmark> (TableStream, FIB->m_FibWord2003.fcSttbfBkmkProt, FIB->m_FibWord2003.lcbSttbfBkmkProt, nWordVersion);
		BkmkProtUser		= new StringTable<WideString> (TableStream, FIB->m_FibWord2003.fcSttbProtUser, FIB->m_FibWord2003.lcbSttbProtUser, nWordVersion, true);

		BkmkAnnotNames		= new StringTable<WideString> (TableStream, FIB->m_FibWord97.fcSttbfAtnBkmk, FIB->m_FibWord97.lcbSttbfAtnBkmk, nWordVersion, true);
		
		if (FIB->m_RgLw97.ccpFtn > 0)
		{
			FootnoteReferenceCharactersPlex	= new Plex<FootnoteDescriptor>(FootnoteDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcffndRef, FIB->m_FibWord97.lcbPlcffndRef, nWordVersion);
			IndividualFootnotesPlex			= new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcffndTxt, FIB->m_FibWord97.lcbPlcffndTxt, nWordVersion);
		}
	
		if (nWordVersion > 0 && FIB->m_FibWord97.lcbPlcPad > 0)
		{
			OutlineListDescriptorPlex =	new Plex<OutlineListDescriptor>(OutlineListDescriptor::GetSize(nWordVersion),	TableStream, FIB->m_FibWord97.fcPlcPad, FIB->m_FibWord97.lcbPlcPad, nWordVersion);
		}

		if (FIB->m_RgLw97.ccpEdn > 0)
		{
			EndnoteReferenceCharactersPlex	= new Plex<EndnoteDescriptor>(EndnoteDescriptor::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfendRef, FIB->m_FibWord97.lcbPlcfendRef, nWordVersion);
			IndividualEndnotesPlex			= new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfendTxt, FIB->m_FibWord97.lcbPlcfendTxt, nWordVersion);
		}

		if (FIB->m_RgLw97.ccpHdr > 0)
		{
			HeaderStoriesPlex =	new Plex<EmptyStructure>( EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfHdd, FIB->m_FibWord97.lcbPlcfHdd, nWordVersion);
		}

        if (FIB->m_RgLw97.ccpAtn > 0)
		{
			AnnotationsReferencePlex	=	new Plex<AnnotationReferenceDescriptor>(AnnotationReferenceDescriptor::GetSize(nWordVersion), TableStream, FIB->m_FibWord97.fcPlcfandRef, FIB->m_FibWord97.lcbPlcfandRef, nWordVersion);
			IndividualCommentsPlex		=	new Plex<EmptyStructure> (EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfandTxt, FIB->m_FibWord97.lcbPlcfandTxt,		nWordVersion);
		}

		if (FIB->m_FibWord2002.lcbAtrdExtra > 0 && AnnotationsReferencePlex)
		{
			size_t count = AnnotationsReferencePlex->Elements.size();
			AnnotationsReferencesEx = new AnnotationReferenceExDescriptors(count, TableStream, FIB->m_FibWord2002.fcAtrdExtra, FIB->m_FibWord2002.lcbAtrdExtra);
		}
		OfficeDrawingPlex					= new Plex<Spa>	(Spa::GetSize(nWordVersion),		TableStream, FIB->m_FibWord97.fcPlcSpaMom,		FIB->m_FibWord97.lcbPlcSpaMom,		nWordVersion);
		OfficeDrawingPlexHeader				= new Plex<Spa>	(Spa::GetSize(nWordVersion),		TableStream, FIB->m_FibWord97.fcPlcSpaHdr,		FIB->m_FibWord97.lcbPlcSpaHdr,		nWordVersion);

		TextboxIndividualPlex				= new Plex<FTXBXS> (FTXBXS::STRUCTURE_SIZE,			TableStream, FIB->m_FibWord97.fcPlcftxbxTxt,	FIB->m_FibWord97.lcbPlcftxbxTxt,	nWordVersion);

		SectionPlex							= new Plex<SectionDescriptor> (SectionDescriptor::GetSize(nWordVersion),	TableStream, FIB->m_FibWord97.fcPlcfSed,FIB->m_FibWord97.lcbPlcfSed,		nWordVersion);

		TextboxBreakPlex					= new Plex<Tbkd> (Tbkd::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfTxbxBkd,	FIB->m_FibWord97.lcbPlcfTxbxBkd,	nWordVersion);
		TextboxBreakPlexHeader				= new Plex<Tbkd> (Tbkd::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfTxbxHdrBkd, FIB->m_FibWord97.lcbPlcfTxbxHdrBkd, nWordVersion);
		
		BookmarkStartPlex					= new Plex<BookmarkFirst>	(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfBkf,		FIB->m_FibWord97.lcbPlcfBkf,		nWordVersion);
		BookmarkEndPlex						= new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfBkl,		FIB->m_FibWord97.lcbPlcfBkl,		nWordVersion);

		AnnotStartPlex						= new Plex<BookmarkFirst>	(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord97.fcPlcfAtnBkf,		FIB->m_FibWord97.lcbPlcfAtnBkf,		nWordVersion);
		AnnotEndPlex						= new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfAtnBkl,		FIB->m_FibWord97.lcbPlcfAtnBkl,		nWordVersion);
		
		BookmarkProtStartPlex				= new Plex<BookmarkFirst>	(BookmarkFirst::STRUCTURE_SIZE,		TableStream, FIB->m_FibWord2003.fcPlcfBkfProt,	FIB->m_FibWord2003.lcbPlcfBkfProt, nWordVersion);
		BookmarkProtEndPlex					= new Plex<EmptyStructure>	(EmptyStructure::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord2003.fcPlcfBklProt,	FIB->m_FibWord2003.lcbPlcfBklProt, nWordVersion);
		
		for (size_t i = 0; i < BookmarkStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark = static_cast<BookmarkFirst*>(BookmarkStartPlex->Elements[i]);
			if (pBookmark)
			{
				_bmkStartEnd bmk;
				bmk.start = BookmarkStartPlex->CharacterPositions[i];
				bmk.end = BookmarkEndPlex->CharacterPositions[i];
				bmk.bookmarkId = pBookmark->id;

				BookmarkStartEndCPs.push_back(bmk);
			}
		}
		std::vector<unsigned char*> & bookmarks = BkmkAnnotNames->getDataExtra();
		for (size_t i = 0; AnnotStartPlex && i < AnnotStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark = static_cast<BookmarkFirst*>(AnnotStartPlex->Elements[i]);
			if (pBookmark)
			{
				short bmc = *((short*)bookmarks[i]); //0x0100 always тут
				_UINT32 lTag = *((_UINT32*)(bookmarks[i] + 2));

				if (i < AnnotStartPlex->CharacterPositions.size() - 1 )
				{
					_bmkStartEnd bmk;
					bmk.start = AnnotStartPlex->CharacterPositions[i];
					bmk.end = AnnotEndPlex->CharacterPositions[i];
					bmk.bookmarkId = lTag;
					
					AnnotStartEndCPs.push_back(bmk);
				}
			}
		}
		for (size_t i = 0; BookmarkProtStartPlex && i < BookmarkProtStartPlex->Elements.size(); ++i)
		{
			BookmarkFirst* pBookmark = static_cast<BookmarkFirst*>(BookmarkProtStartPlex->Elements[i]);
			if (pBookmark)
			{
				if (i < BookmarkProtStartPlex->CharacterPositions.size() - 1)
				{
					_bmkStartEnd bmk;
					bmk.start = BookmarkProtStartPlex->CharacterPositions[i];
					bmk.end = BookmarkProtEndPlex->CharacterPositions[i];
					bmk.bookmarkId = pBookmark->id;

					BookmarkProtStartEndCPs.push_back(bmk);
				}
			}
		}
		AutoTextPlex						=	new Plex<EmptyStructure>(EmptyStructure::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfGlsy,   FIB->m_FibWord97.lcbPlcfGlsy, nWordVersion);
		
		FieldsPlex							=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldMom, FIB->m_FibWord97.lcbPlcfFldMom, nWordVersion);
		FootnoteDocumentFieldsPlex			=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldFtn, FIB->m_FibWord97.lcbPlcfFldFtn, nWordVersion);
		EndnoteDocumentFieldsPlex			=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldEdn, FIB->m_FibWord97.lcbPlcfFldEdn, nWordVersion);
		HeadersAndFootersDocumentFieldsPlex	=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldHdr, FIB->m_FibWord97.lcbPlcfFldHdr, nWordVersion);
		AnnotationsFieldsPlex				=	new Plex<FieldCharacter>(FieldCharacter::STRUCTURE_SIZE, TableStream, FIB->m_FibWord97.fcPlcfFldAtn, FIB->m_FibWord97.lcbPlcfFldAtn, nWordVersion);
		
		ListPlex							=	new Plex<ListNumCache>	(ListNumCache::STRUCTURE_SIZE,	TableStream, FIB->m_FibWord97.fcPlcfBteLvc, FIB->m_FibWord97.lcbPlcfBteLvc, nWordVersion);
		
// Read the FKPs
		AllPapxFkps	=	FormattedDiskPagePAPX::GetAllPAPXFKPs (FIB, WordDocumentStream, TableStream, DataStream);
		AllChpxFkps	=	FormattedDiskPageCHPX::GetAllCHPXFKPs (FIB, WordDocumentStream, TableStream);

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

		if (FontTable)
		{
			std::unordered_map<int, int> fonts_charsets;
			bool bFontsCodePage = false;

            for ( size_t i = 0; FIB->m_FibWord97.lcbClx > 0 && !bFontsCodePage &&  i < FontTable->Data.size(); ++i)
			{
                FontFamilyName* font = dynamic_cast<FontFamilyName*>( FontTable->Data[i]);
				if (!font) continue;

				if (fonts_charsets.find(font->chs) == fonts_charsets.end())
				{
					fonts_charsets.insert(std::make_pair(font->chs, font->ff));
					
                    for (size_t j = 0 ; j < 32; j++)
                    {
                        if (aCodePages[j][0] == font->chs && font->chs > 2)
                        {
                            nFontsCodePage = aCodePages[j][1];
                            bFontsCodePage = true;
                            break;
                        }
                    }
				}
			}
		}

		if (FIB->m_FibWord97.lcbClx > 0)
		{
			FIB->m_FibBase.fComplex = true;
			// Parse the piece table and construct a list that contains all chars
			m_PieceTable	= new PieceTable (FIB, TableStream, WordDocumentStream );
			Text			= m_PieceTable->GetAllEncodingText (WordDocumentStream);
		}
		
        if (FIB->m_FibWord97.lcbClx < 1 || ((Text) && (Text->empty())))
		{
			int cb = FIB->m_FibBase.fcMac - FIB->m_FibBase.fcMin;

			if (cb > 0)
			{
				FIB->m_FibBase.fComplex = false;
				unsigned char *bytes = new unsigned char[cb];

				WordDocumentStream->seek (FIB->m_FibBase.fcMin);
				WordDocumentStream->read (bytes, cb);

				RELEASEOBJECT(Text);
				Text = new std::vector<wchar_t>();

				FormatUtils::GetSTLCollectionFromBytes<std::vector<wchar_t>>(Text, bytes, cb, nFontsCodePage != ENCODING_WINDOWS_1250 ? nFontsCodePage : nDocumentCodePage);

				RELEASEARRAYOBJECTS(bytes);
			}
		}

		if (BookmarkNames)
		{
			for (size_t i = 0; i < BookmarkStartEndCPs.size(); ++i)
			{
				WideString* bookmarkName = static_cast<WideString*>(BookmarkNames->operator[]( i ));

                if ( ( bookmarkName != NULL ) && ( *bookmarkName == L"_PictureBullets" ) )
				{
					for (size_t j = BookmarkStartEndCPs[i].start, k = 0; j < Text->size(); ++j, ++k )
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

		//build a dictionary of all SEPX
		if ( !SectionPlex->Elements.empty() )
		{
			AllSepx = new std::map<int, SectionPropertyExceptions*>();

			for (size_t i = 0; i < SectionPlex->Elements.size(); ++i)
			{
				//Read the SED
				SectionDescriptor* sed = static_cast<SectionDescriptor*>(SectionPlex->Elements.at(i));
				int cp = SectionPlex->CharacterPositions[i + 1];

				//Get the SEPX
				VirtualStreamReader wordReader( WordDocumentStream, sed->fcSepx, nWordVersion);

				//!!!TODO: cbSepx is the size in bytes of the rest properties part!!!
				short cbSepx			=	nWordVersion == 2 ? wordReader.ReadByte() : wordReader.ReadInt16();
				unsigned char* bytes	=	wordReader.ReadBytes( ( cbSepx /*- 2*/ ), true );

				AllSepx->insert( std::pair<int, SectionPropertyExceptions*>( cp, new SectionPropertyExceptions( bytes, ( cbSepx /*- 2*/ ), nWordVersion ) ) );

				RELEASEARRAYOBJECTS( bytes );
			}
		}

		return 0;
	}
	bool WordDocument::LoadDocumentFlat()
	{
		NSFile::CFileBinary file;

		if (false == file.OpenFile(m_sFileName)) return false;

		DWORD sz_read = 0;
		unsigned short wIdent = 0;
		file.ReadFile((BYTE*)&wIdent, 2, sz_read);

		if ( wIdent != 0xA5EC && wIdent != 0xA5DC && wIdent != 0xA5DB)
		{
			file.CloseFile();
			return false;
		}
		file.SeekFile(0);

		if (m_sTempFolder.empty())
		{
            m_sTempFolder = NSFile::CFileBinary::GetTempPath();
		}
		m_sTempDecryptFileName	= m_sTempFolder + FILE_SEPARATOR_STR + L"~tempFile.doc";
		
		POLE::Storage *storageOut	= new POLE::Storage(m_sTempDecryptFileName.c_str());

		if (!storageOut)
		{
			file.CloseFile();
			return false;
		}
		
		if (!storageOut->open(true, true))
		{
			file.CloseFile();
			delete storageOut;
			return false;
		}
		DWORD size_stream = file.GetFileSize();

		POLE::Stream *streamNew = new POLE::Stream(storageOut, L"WordDocument", true, size_stream);
		if (!streamNew) return false;

		unsigned char* data_stream = new unsigned char[size_stream];
		file.ReadFile(data_stream, size_stream, size_stream);

		streamNew->write(data_stream, size_stream);

		RELEASEARRAYOBJECTS(data_stream);

		streamNew->flush();
				
		delete streamNew;

		storageOut->close();
		delete storageOut;
		file.CloseFile();

//reset streams
		RELEASEOBJECT(WordDocumentStream);
		
		m_pStorage->SetFile(m_sTempDecryptFileName.c_str());
		
		return true;
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

		//for (std::list<std::string>::iterator it = listStream.begin(); it != listStream.end(); ++it)
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
		
		for( std::list<std::wstring>::iterator it = entries.begin(); it != entries.end(); ++it )
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
			std::wstring fullname_create = path + *it;

			if (it->at(0) < 32)
			{
				*it = it->substr(1);  // without prefix
			}
			std::wstring fullname_open	= path + *it;
			
			bool bDecrypt = false;

			if ( std::wstring::npos != fullname_open.find(L"WordDocument") ||
				 std::wstring::npos != fullname_open.find(L"Data") ||
				 std::wstring::npos != fullname_open.find(L"Table") ||
				(std::wstring::npos != fullname_open.find(L"SummaryInformation") &&
					encryptionHeader->bAES && encryptionHeader->crypt_data_aes.fDocProps)
				)
			{
				bDecrypt = true;
			}	
			DecryptStream(fullname_open, storageIn, fullname_create, storageOut, Decryptor, bDecrypt);
		}  
	}
	bool WordDocument::DecryptStream(std::wstring streamName_open, POLE::Storage * storageIn, std::wstring streamName_create, POLE::Storage * storageOut, CRYPT::Decryptor* Decryptor, bool bDecrypt)
	{
		POLE::Stream *stream = new POLE::Stream(storageIn, streamName_open);
		if (!stream) return false;

		stream->seek(0);
		POLE::uint64 size_stream = stream->size();
		
		POLE::Stream *streamNew = new POLE::Stream(storageOut, streamName_create, true, size_stream);
		if (!streamNew) return false;

		unsigned char* data_stream = new unsigned char[size_stream];
		stream->read(data_stream, size_stream);

		unsigned char* data_store = NULL;
		int size_data_store = 0;		
		
		if ( std::wstring::npos != streamName_open.find(L"WordDocument") )
		{
			size_data_store = (nWordVersion > 0) ? 36 : 68;
			data_store = new unsigned char[size_data_store];
		}
		
		if (data_store)
			memcpy(data_store, data_stream, size_data_store);

		size_t			size_block	= 0x200;
		unsigned long	block		= 0;

		for (POLE::uint64 pos = /*(nWordVersion > 0) ? size_data_store :*/ 0; pos < size_stream; pos += size_block, block++)
		{
			if (pos + size_block > size_stream)
				size_block = size_stream - pos;

			if (bDecrypt)
			{
				Decryptor->Decrypt((char*)data_stream + pos, size_block, block);
			}
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
		nWordVersion =	0;
		
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
		RELEASEOBJECT(BkmkAnnotNames);
		RELEASEOBJECT(AssocNames);

		RELEASEOBJECT(IndividualFootnotesPlex);
		RELEASEOBJECT(FootnoteReferenceCharactersPlex);
		RELEASEOBJECT(IndividualEndnotesPlex);
		RELEASEOBJECT(EndnoteReferenceCharactersPlex);
		RELEASEOBJECT(FieldsPlex);
		RELEASEOBJECT(FootnoteDocumentFieldsPlex);
		RELEASEOBJECT(EndnoteDocumentFieldsPlex);
		RELEASEOBJECT(HeadersAndFootersDocumentFieldsPlex);
		RELEASEOBJECT(AnnotationsFieldsPlex);
		RELEASEOBJECT(HeaderStoriesPlex);
		RELEASEOBJECT(IndividualCommentsPlex);
		RELEASEOBJECT(AnnotationsReferencePlex);
		RELEASEOBJECT(AnnotationsReferencesEx);
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
		RELEASEOBJECT(BkmkProt);
		RELEASEOBJECT(BkmkProtUser);
		RELEASEOBJECT(BookmarkProtStartPlex);
		RELEASEOBJECT(BookmarkProtEndPlex);

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
	void WordDocument::CorrectColor(ODRAW::OfficeArtCOLORREF & color)
	{
#if 0
		if (false == color.sColorRGB.empty()) return;

		if (color.fSysIndex)
		{
			oox::_color sys_color;
			_UINT32 nColorCode = color.index;

			unsigned short nParameter = (unsigned short)((nColorCode >> 16) & 0x00ff);  // the HiByte of nParameter is not zero, an exclusive AND is helping :o
			unsigned short nFunctionBits = (unsigned short)((nColorCode & 0x00000f00) >> 8);
			unsigned short nAdditionalFlags = (unsigned short)((nColorCode & 0x0000f000) >> 8);
			unsigned short nColorIndex = (unsigned short)(nColorCode & 0x00ff);
			unsigned short nPropColor = 0;

			_UINT32 systemColors[25] = 
			{
				0xc0c0c0, 0x008080, 0x000080, 0x808080, 0xc0c0c0, 0xffffff, 0x000000,
				0x000000, 0x000000, 0xffffff, 0xc0c0c0, 0xc0c0c0, 0x808080, 0x000080,
				0xffffff, 0xc0c0c0, 0x808080, 0x808080, 0x000000, 0xc0c0c0, 0xffffff,
				0x000000, 0xc0c0c0, 0x000000, 0xffffc0
			};

			if (nColorIndex < 25)
			{
				sys_color.SetRGB((unsigned char)(systemColors[nColorIndex]>>16), (unsigned char)(systemColors[nColorIndex]>>8), (unsigned char)(systemColors[nColorIndex]));
			}
			else return;

			if (nAdditionalFlags & 0x80)   // make color gray
			{
				BYTE nZwi = 0x7f;//= aColor.GetLuminance();
				sys_color.SetRGB(nZwi, nZwi, nZwi);
			}
			switch (nFunctionBits)
			{
			case 0x01:     // darken color by parameter
			{
				BYTE R = static_cast<BYTE>	((nParameter * sys_color.GetR()) >> 8);
				BYTE G = static_cast<BYTE>	((nParameter * sys_color.GetG()) >> 8);
				BYTE B = static_cast<BYTE> ((nParameter * sys_color.GetB()) >> 8);

				sys_color.SetRGB(R, G, B);
			}
			break;
			case 0x02:     // lighten color by parameter
			{
				unsigned short nInvParameter = (0x00ff - nParameter) * 0xff;
				BYTE R = static_cast<BYTE>((nInvParameter + (nParameter * sys_color.GetR())) >> 8);
				BYTE G = static_cast<BYTE>((nInvParameter + (nParameter * sys_color.GetG())) >> 8);
				BYTE B = static_cast<BYTE>((nInvParameter + (nParameter * sys_color.GetB())) >> 8);

				sys_color.SetRGB(R, G, B);
			}break;			
			case 0x03:     // add grey level RGB(p,p,p)
			{
				short nR = (short)sys_color.GetR() + (short)nParameter;
				short nG = (short)sys_color.GetG() + (short)nParameter;
				short nB = (short)sys_color.GetB() + (short)nParameter;

				if (nR > 0x00ff)	nR = 0x00ff;
				if (nG > 0x00ff)	nG = 0x00ff;
				if (nB > 0x00ff)	nB = 0x00ff;

				sys_color.SetRGB((BYTE)nR, (BYTE)nG, (BYTE)nB);
			}break;		
			case 0x04:     // substract grey level RGB(p,p,p)
			{
				short nR = (short)sys_color.GetR() - (short)nParameter;
				short nG = (short)sys_color.GetG() - (short)nParameter;
				short nB = (short)sys_color.GetB() - (short)nParameter;
				if (nR < 0) nR = 0;
				if (nG < 0) nG = 0;
				if (nB < 0) nB = 0;
				sys_color.SetRGB((BYTE)nR, (BYTE)nG, (BYTE)nB);
			}	break;		
			case 0x05:     // substract from gray level RGB(p,p,p)
			{
				short nR = (short)nParameter - (short)sys_color.GetR();
				short nG = (short)nParameter - (short)sys_color.GetG();
				short nB = (short)nParameter - (short)sys_color.GetB();
				if (nR < 0) nR = 0;
				if (nG < 0) nG = 0;
				if (nB < 0) nB = 0;
				sys_color.SetRGB((BYTE)nR, (BYTE)nG, (BYTE)nB);
			}break;			
			case 0x06:     // per component: black if < p, white if >= p
			{
				BYTE R = sys_color.GetR() < nParameter ? 0x00 : 0xff;
				BYTE G = sys_color.GetG() < nParameter ? 0x00 : 0xff;
				BYTE B = sys_color.GetB() < nParameter ? 0x00 : 0xff;

				sys_color.SetRGB(R, G, B);
			}break;			
			}
			if (nAdditionalFlags & 0x40)                  // top-bit invert
				sys_color.SetRGB(sys_color.GetR() ^ 0x80, sys_color.GetG() ^ 0x80, sys_color.GetB() ^ 0x80);

			if (nAdditionalFlags & 0x20)                  // invert color
				sys_color.SetRGB(0xff - sys_color.GetR(), 0xff - sys_color.GetG(), 0xff - sys_color.GetB());

			color.sColorRGB = sys_color.sRGB;
		}
		//else if (color.fPaletteIndex)
		//{
		//	std::map<int, std::wstring>::iterator it = colors_palette.find(color.index);
		//	if (it != colors_palette.end())
		//	{
		//		color.sColorRGB = it->second;
		//	}
		//}
#endif
	}

}
