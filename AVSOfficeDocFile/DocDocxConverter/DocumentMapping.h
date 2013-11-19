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
		int writeRun( vector<WCHAR>* chars, CharacterPropertyExceptions* chpx, int initialCp );
		/// Writes the given text to the document
		void writeText( vector<WCHAR>* chars, int initialCp, CharacterPropertyExceptions* chpx, bool writeDeletedText );
		void writeTextElement( const wstring& text, const wstring& textType );
		void writeTextStart( const wstring& textType );
		void writeTextEnd( const wstring& textType );
		/// Searches for bookmarks in the list of characters.
		vector<int> searchBookmarks( vector<WCHAR>* chars, int initialCp );
		ParagraphPropertyExceptions* findValidPapx( int fc );
		/// Splits a list of characters into several lists
		list<vector<WCHAR> >* splitCharList( vector<WCHAR>* chars, vector<int>* splitIndices );
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
		int searchNextTextMark( vector<WCHAR>* chars, int initialCp, WCHAR mark );

	private:
		Symbol getSymbol( const CharacterPropertyExceptions* chpx );
		bool							m_bInternalXmlWriter; 

	protected:

		WordDocument*					m_document;
		ConversionContext*				m_context;
		
		ParagraphPropertyExceptions*	_lastValidPapx;
		SectionPropertyExceptions*		_lastValidSepx;
		int								_skipRuns;
		int								_sectionNr;
		int								_footnoteNr;
		int								_endnoteNr;
		int								_commentNr;
		bool							_writeInstrText;
		unsigned int					_fldCharCounter;
		IMapping*						_caller;
	};
}