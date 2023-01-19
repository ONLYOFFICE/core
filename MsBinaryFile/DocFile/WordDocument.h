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
#pragma once

#include "StructuredStorageReader.h"
#include "FileInformationBlock.h"
#include "PieceTable.h"
#include "StringTable.h"
#include "FontFamilyName.h"
#include "WideString.h"
#include "Plex.h"
#include "AnnotationReferenceDescriptor.h"
#include "Tbkd.h"
#include "Spa.h"
#include "SectionDescriptor.h"
#include "SectionPropertyExceptions.h"
#include "BookmarkFirst.h"
#include "StyleSheet.h"
#include "FormattedDiskPagePAPX.h"
#include "FormattedDiskPageCHPX.h"
#include "ListTable.h"
#include "AnnotationOwnerList.h"
#include "DocumentProperties.h"
#include "EncryptionHeader.h"
#include "ListFormatOverrideTable.h"
#include "OfficeArtContent.h"
#include "HeaderAndFooterTable.h"
#include "FootnoteDescriptor.h"
#include "EndnoteDescriptor.h"
#include "FieldCharacter.h"
#include "IVisitable.h"

namespace CRYPT
{
	class Decryptor;
}

namespace DocFileFormat
{
	class WordDocument: public IVisitable
	{
		friend class FootnotesMapping;
		friend class EndnotesMapping;
        friend class CommentsMapping;
		friend class Converter;
		friend class CharacterPropertiesMapping;
		friend class DocumentMapping;
		friend class MainDocumentMapping;
		friend class NumberingMapping;
		friend class ParagraphPropertiesMapping;
		friend class StyleSheetMapping;
		friend class HeaderMapping;
		friend class FooterMapping;
		friend class SectionPropertiesMapping;
		friend class Table;
		friend class TableRow;
		friend class VMLShapeMapping;
		friend class VMLPictureMapping;
		friend class OpenXmlPackage;
		friend class TextboxMapping;
		friend class SettingsMapping;
		friend class WordprocessingDocument;
	public:

		WordDocument (const std::wstring & tempFolder, const int userLCID);
		virtual ~WordDocument();

		_UINT32 LoadDocument(const std::wstring & fileName, const std::wstring & password);

		int		nWordVersion;
		int		nDocumentCodePage;
		bool	bDocumentCodePage;
		int		nFontsCodePage;
		
		inline StructuredStorageReader* GetStorage() const
		{
			return m_pStorage;
		}
		inline POLE::Stream* GetDocumentStream() const
		{
			return WordDocumentStream;
		}
	private:
		bool LoadDocumentFlat();

		bool DecryptOfficeFile	(CRYPT::Decryptor* Decryptor);
		
		bool DecryptStream		(std::wstring streamName_open, POLE::Storage * storageIn, std::wstring streamName_create, POLE::Storage * storageOut, CRYPT::Decryptor* Decryptor, bool bDecrypt);
		void DecryptStream		(int level, std::wstring streamName, POLE::Storage * storageIn, POLE::Storage * storageOut, CRYPT::Decryptor* Decryptor);
		
		inline OfficeArtContent* GetOfficeArt ()
		{
			return officeArtContent;
		}
		
		inline int FindFileCharPos(int cp)
		{
			if (FIB->m_FibBase.fComplex)
			{
				if (!m_PieceTable) return -1;
				if ((!m_PieceTable->FileCharacterPositions) || (m_PieceTable->FileCharacterPositions->empty())) return -1;

				std::map<int, int>::iterator it = m_PieceTable->FileCharacterPositions->find(cp);
				return (it != m_PieceTable->FileCharacterPositions->end()) ? it->second : -1;
			}
			else
			{
				int fc = cp + FIB->m_FibBase.fcMin;
				if (fc > FIB->m_FibBase.fcMac) 
					return -1;
				return fc;
			}
		}
		std::vector<wchar_t>* GetEncodingChars	(int fcStart, int fcEnd);
		std::vector<wchar_t>* GetChars			(int fcStart, int fcEnd, int cp);
		
		std::vector<int>*							GetFileCharacterPositions		( int fcMin, int fcMax );
		std::list<CharacterPropertyExceptions*>*	GetCharacterPropertyExceptions	( int fcMin, int fcMax );
		
		void Clear();

		std::wstring			m_sFileName;
		std::wstring			m_sPassword;	
		std::wstring			m_sTempFolder;
		std::wstring			m_sTempDecryptFileName;
		int						m_nUserLCID;
		std::wstring			m_sXmlApp;
		std::wstring			m_sXmlCore;

		POLE::Stream			* WordDocumentStream;	// The stream "WordDocument"		
		POLE::Stream			* TableStream;			// The stream "0Table" or "1Table"	
		POLE::Stream			* DataStream;			// The stream called "Data"
		StructuredStorageReader	* m_pStorage;			//POLE::Storage* Storage
		
		std::vector<wchar_t>						* Text;			// All text of the Word document
		std::list<FormattedDiskPagePAPX*>			* AllPapxFkps;	// A list of all FKPs that contain PAPX
		std::list<FormattedDiskPageCHPX*>			* AllChpxFkps;	// A list of all FKPs that contain CHPX
		
		std::map<int, ParagraphPropertyExceptions*>	* AllPapx;		// The value is the PAPX that formats the paragraph.
		std::map<int, SectionPropertyExceptions*>	* AllSepx;		// The value is the SEPX that formats the section.
		std::vector<int>							* AllPapxVector;// A vector to quick find in AllPapx

		std::map<int, int> PictureBulletsCPsMap;

		struct _bmkStartEnd
		{
			int start;
			int end;			
			_UINT32 bookmarkId;
		};
		std::vector<_bmkStartEnd> BookmarkStartEndCPs;
		std::vector<_bmkStartEnd> BookmarkProtStartEndCPs;
		std::vector<_bmkStartEnd> AnnotStartEndCPs;
		
		std::map<int, int> mapAnnotBookmarks; //id, index
		std::map<int, int> mapProtBookmarks;

		FileInformationBlock				* FIB;
		StyleSheet							* Styles;					// The style sheet of the document
		PieceTable							* m_PieceTable;				// PieceTable PieceTable;		
		ListFormatOverrideTable				* listFormatOverrideTable;	//  lists and numberings in the document.
		HeaderAndFooterTable				* headerAndFooterTable;
		AnnotationOwnerList					* AnnotationOwners;		
		ListTable							* listTable;				// list numberings in the document.
		WordDocumentProperties				* DocProperties;
		EncryptionHeader					* encryptionHeader;
		OfficeArtContent					* officeArtContent;		// info about the drawings in the document.

		StringTable<WideString>				*RevisionAuthorTable;		
		StringTable<FontFamilyName>			*FontTable;		// A list of all font names, used in the doucument
		StringTable<WideString>				*BookmarkNames;
		StringTable<WideString>				*AutoTextNames;
		StringTable<WideString>				*AssocNames;
		StringTable<WideString>				*BkmkAnnotNames;
		StringTable<WideString>				*Captions;
		StringTable<WideString>				*AutoCaptions;
		
		StringTableEx<ProtInfoBookmark>		*BkmkProt;
		StringTable<WideString>				*BkmkProtUser;

		Plex<EmptyStructure>				*IndividualFootnotesPlex;			//A plex of locations of individual footnotes		
		Plex<FootnoteDescriptor>			*FootnoteReferenceCharactersPlex;	//A plex of footnote reference characters
		
		Plex<EmptyStructure>				*IndividualEndnotesPlex;			//A plex of locations of individual endnotes
		Plex<EndnoteDescriptor>				*EndnoteReferenceCharactersPlex;	//A plex of endnote reference characters
		
		Plex<EmptyStructure>				*HeaderStoriesPlex;					//A plex of the header document
		Plex<EmptyStructure>				*IndividualCommentsPlex;			// A plex with all ATRDPre10 structs
		
		Plex<FTXBXS>						*TextboxIndividualPlex;	
		Plex<Tbkd>							*TextboxBreakPlex;					// Describes the breaks inside the textbox subdocument
		Plex<Tbkd>							*TextboxBreakPlexHeader;			// Describes the breaks inside the header textbox subdocument
		
		Plex<OutlineListDescriptor>			*OutlineListDescriptorPlex;
		Plex<Spa>							*OfficeDrawingPlex;
		Plex<Spa>							*OfficeDrawingPlexHeader;
		
		Plex<SectionDescriptor>				*SectionPlex;	
	
		Plex<BookmarkFirst>					*BookmarkStartPlex;
		Plex<EmptyStructure>				*BookmarkEndPlex;

		Plex<BookmarkFirst>					*AnnotStartPlex;
		Plex<EmptyStructure>				*AnnotEndPlex;

		Plex<BookmarkFirst>					*BookmarkProtStartPlex;
		Plex<EmptyStructure>                *BookmarkProtEndPlex;

		Plex<ListNumCache>					*ListPlex;
		Plex<FieldCharacter>				*FieldsPlex;
		Plex<FieldCharacter>				*FootnoteDocumentFieldsPlex;
		Plex<FieldCharacter>				*EndnoteDocumentFieldsPlex;
		Plex<FieldCharacter>				*HeadersAndFootersDocumentFieldsPlex;
		Plex<FieldCharacter>				*AnnotationsFieldsPlex;
		Plex<AnnotationReferenceDescriptor> *AnnotationsReferencePlex;
		Plex<EmptyStructure>				*AutoTextPlex;		
		

		AnnotationReferenceExDescriptors	*AnnotationsReferencesEx;
//------------------------------------------------------------------------------
		void CorrectColor(ODRAW::OfficeArtCOLORREF & color);
	};
}
