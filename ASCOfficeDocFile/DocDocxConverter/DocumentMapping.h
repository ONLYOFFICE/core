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

#include "ContentTypes.h"
#include "RevisionData.h"
#include "BookmarkFirst.h"
#include "WideString.h"
#include "PictureDescriptor.h"
#include "VMLPictureMapping.h"
#include "VMLShapeMapping.h"
#include "CharacterPropertyExceptions.h"
#include "TableInfo.h"
#include "TablePropertyExceptions.h"
#include "AbstractOpenXmlMapping.h"
#include "TableRowPropertiesMapping.h"
#include "TableCellPropertiesMapping.h"
#include "OleObject.h"
#include "OleObjectMapping.h"
#include "ConversionContext.h"
#include "IMapping.h"

namespace DocFileFormat
{
	class FootnotesMapping;
	class CommentsMapping;

	struct Symbol
	{
		wstring FontName;
		wstring HexValue;
	};

	class DocumentMapping: public AbstractOpenXmlMapping, public IMapping
	{
		friend class Table;
		friend class TableRow;
		friend class TableCell;
		friend class DocParagraph;

	public:
		DocumentMapping(ConversionContext* context, IMapping* caller);
		DocumentMapping(ConversionContext* context, XmlUtils::CXmlWriter* writer, IMapping* caller);
		virtual ~DocumentMapping();
		virtual void Apply( IVisitable* visited ) = 0;

	protected:

		/// Looks into the section table to find out if this CP is the current of a section
		int getCurrentSection(int cp);
		/// Looks into the section table to find out if this CP is the end of a section
		bool isSectionEnd( int cp );
		/// Writes a Paragraph that starts at the given cp and 
		/// ends at the next paragraph end mark or section end mark
		int writeParagraph( int cp ); 
		/// Writes a Paragraph that starts at the given cpStart and 
		/// ends at the given cpEnd
		int writeParagraph( int initialCp, int cpEnd, bool sectionEnd );
		/// Writes a Paragraph RSID
		void writeParagraphRsid( const ParagraphPropertyExceptions* papx );
		/// Writes a run with the given characters and CHPX
		int writeRun( vector<wchar_t>* chars, CharacterPropertyExceptions* chpx, int initialCp );
		/// Writes the given text to the document
		void writeText( vector<wchar_t>* chars, int initialCp, CharacterPropertyExceptions* chpx, bool writeDeletedText );
		void writeTextElement( const wstring& text, const wstring& textType );
        void writeTextStart( const wstring& textType, bool preserve_space);
		void writeTextEnd( const wstring& textType );
		/// Searches for bookmarks in the list of characters.
		vector<int> searchBookmarks( vector<wchar_t>* chars, int initialCp );
		ParagraphPropertyExceptions* findValidPapx( int fc );
		/// Splits a list of characters into several lists
		list<vector<wchar_t> >* splitCharList( vector<wchar_t>* chars, vector<int>* splitIndices );
		/// Writes the table starts at the given cp value
		int writeTable( int initialCp, unsigned int nestingLevel );
		/// Builds a list that contains the width of the several columns of the table.
		vector<short>* buildTableGrid( int initialCp, unsigned int nestingLevel );
		/// Finds the FC of the next row end mark.
		int findRowEndFc( int initialCp, int& rowEndCp, unsigned int nestingLevel );
		/// Finds the FC of the next row end mark.
		int findRowEndFc( int initialCp, unsigned int nestingLevel );
		/// Writes the table row that starts at the given cp value and ends at the next row end mark
		int writeTableRow( int initialCp, vector<short>* grid, unsigned int nestingLevel );
		/// Writes the table cell that starts at the given cp value and ends at the next cell end mark
		int writeTableCell( int initialCp, TablePropertyExceptions* tapx, vector<short>* grid, int& gridIndex, int cellIndex, unsigned int nestingLevel );
		int findCellEndCp( int initialCp, unsigned int nestingLevel );
		bool writeBookmarks( int cp );
		bool writeBookmarkStart( short id );
		bool writeBookmarkEnd( short id );
		/// Checks if the CHPX is special
		bool isSpecial( CharacterPropertyExceptions* chpx );
		/// Finds the SEPX that is valid for the given CP.
		SectionPropertyExceptions* findValidSepx( int cp );
		/// Searches the given vector for the next FieldEnd character.
		int searchNextTextMark( vector<wchar_t>* chars, int initialCp, wchar_t mark );

	private:
		Symbol getSymbol( const CharacterPropertyExceptions* chpx );
		bool							m_bInternalXmlWriter; 

	protected:

		WordDocument*					m_document;
		ConversionContext*				m_context;
		IMapping*						_caller;
		
		ParagraphPropertyExceptions*	_lastValidPapx;
		SectionPropertyExceptions*		_lastValidSepx;

		int								_skipRuns;
		int								_sectionNr;
		int								_footnoteNr;
		int								_endnoteNr;
		int								_commentNr;
		int								_isSectionPageBreak; //0 - not set, 1 -page break, 2 - continues
		bool							_writeInstrText;
		bool							_writeWebHidden;
		unsigned int					_fldCharCounter;
		std::wstring					_writeAfterRun;
	};
}
