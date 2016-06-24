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
#pragma once

#include "StructuredStorageReader.h"
#include "VirtualStreamReader.h"
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
#include "ListFormatOverrideTable.h"
#include "OfficeArtContent.h"
#include "HeaderAndFooterTable.h"
#include "FootnoteDescriptor.h"
#include "EndnoteDescriptor.h"
#include "FieldCharacter.h"
#include "IVisitable.h"
#include "../Common/Callback.h"

namespace DocFileFormat
{
	class WordDocument: public IVisitable
	{
		/*Mapping classes with direct access to the Word Document.*/
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
		friend class OpenXmlPackage;
		friend class TextboxMapping;

	public:

		WordDocument (CString sFileName);
		virtual ~WordDocument();

		long LoadDocument (const ProgressCallback* pCallFunc);

		inline StructuredStorageReader* GetStorage() const
		{
			return m_pStorage;
		}

		inline OfficeArtContent* GetOfficeArt ()
		{
			return m_pOfficeArtContent;
		}

		inline int FindFileCharPos(int cp)
		{
			std::map<int, int>::iterator it = m_PieceTable->FileCharacterPositions->find(cp);
			return (it != m_PieceTable->FileCharacterPositions->end()) ? it->second : -1;
		}

	private:

		vector<int>* GetFileCharacterPositions( int fcMin, int fcMax );
		list<CharacterPropertyExceptions*>* GetCharacterPropertyExceptions( int fcMin, int fcMax );
		long Parse (int fibFC);
		void Clear();

	private:
		
		CString m_sFileName;
		
		const ProgressCallback* m_pCallFunc;

		int fibFC;

		//PieceTable PieceTable;
		PieceTable* m_PieceTable;

		// The stream "WordDocument"
		POLE::Stream* WordDocumentStream;
		// The stream "0Table" or "1Table"
		POLE::Stream* TableStream;
		// The stream called "Data"
		POLE::Stream* DataStream;
		// The StructuredStorageFile itself
		//POLE::Storage* Storage;
		StructuredStorageReader* m_pStorage;
		// The file information block of the word document
		FileInformationBlock *FIB;
		// All text of the Word document
		vector<wchar_t> *Text;

		/// The style sheet of the document
		StyleSheet* Styles;

		StringTable<WideString> *RevisionAuthorTable;
		// A list of all font names, used in the doucument
		StringTable<FontFamilyName> *FontTable;
		StringTable<WideString> *BookmarkNames;
		StringTable<WideString> *AutoTextNames;

		//A plex of locations of individual footnotes
		Plex<EmptyStructure> *IndividualFootnotesPlex;
		//A plex of footnote reference characters
		Plex<FootnoteDescriptor> *FootnoteReferenceCharactersPlex;

		//A plex of locations of individual endnotes
		Plex<EmptyStructure> *IndividualEndnotesPlex;
		//A plex of endnote reference characters
		Plex<EndnoteDescriptor> *EndnoteReferenceCharactersPlex;

		Plex<FieldCharacter> *FieldsPlex;
		Plex<FieldCharacter> *FootnoteDocumentFieldsPlex;
		Plex<FieldCharacter> *EndnoteDocumentFieldsPlex;
		Plex<FieldCharacter> *HeadersAndFootersDocumentFieldsPlex;

		//A plex of the header document
		Plex<EmptyStructure> *HeaderStoriesPlex;

		Plex<EmptyStructure> *IndividualCommentsPlex;
		/// A plex with all ATRDPre10 structs
		Plex<AnnotationReferenceDescriptor> *AnnotationsReferencePlex;

		AnnotationOwnerList* AnnotationOwners;

		/// A list that contains all formatting information of 
		/// the lists and numberings in the document.
		ListTable*			listTable;

		// That contains information about the drawings in the document.
		OfficeArtContent*	m_pOfficeArtContent;

		/// Describes the breaks inside the textbox subdocument
		Plex<Tbkd>*			TextboxBreakPlex;
		Plex<FTXBXS>*		m_arFTBXS;	//	 FOR TEST
		/// Describes the breaks inside the header textbox subdocument
		Plex<Tbkd>*			TextboxBreakPlexHeader;
		Plex<Spa>*			OfficeDrawingPlex;
		Plex<Spa>*			OfficeDrawingPlexHeader;
		/// A Plex containing all section descriptors
		Plex<SectionDescriptor> *SectionPlex;
		Plex<BookmarkFirst> *BookmarkStartPlex;
		Plex<EmptyStructure> *BookmarkEndPlex;
		vector<pair<int, int>> BookmarkStartEndCPs;
		map<int, int> PictureBulletsCPsMap;
		/// Each character position specifies the beginning of a range of text 
		/// that constitutes the contents of an AutoText item.
		Plex<EmptyStructure> *AutoTextPlex;

		/// The DocumentProperties of the word document
		WordDocumentProperties* DocProperties;

		/// A list that contains all overriding formatting information
		/// of the lists and numberings in the document.
		ListFormatOverrideTable* listFormatOverrideTable;

		/// A list of all FKPs that contain PAPX
		list<FormattedDiskPagePAPX*> *AllPapxFkps;
		/// A list of all FKPs that contain CHPX
		list<FormattedDiskPageCHPX*> *AllChpxFkps;

		/// A dictionary that contains all SEPX of the document.
		/// The key is the CP at which sections ends.
		/// The value is the SEPX that formats the section.
		map<int, SectionPropertyExceptions*>* AllSepx;

		/// A dictionary that contains all PAPX of the document.
		/// The key is the FC at which the paragraph starts.
		/// The value is the PAPX that formats the paragraph.
		map<int, ParagraphPropertyExceptions*>* AllPapx;

		/// A vector to quick find in AllPapx
		vector<int>* AllPapxVector;

		/// A table that contains the positions of the headers and footer in the text.
		HeaderAndFooterTable* headerAndFooterTable;
	};
}
