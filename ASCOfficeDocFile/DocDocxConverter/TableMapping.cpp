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

#include "TableMapping.h"
#include "TablePropertiesMapping.h"

namespace DocFileFormat
{
	TableCell::TableCell( DocumentMapping* _documentMapping, int _cp, unsigned int _depth) : cp(_cp), depth(_depth), documentMapping(_documentMapping)
	{
	}

	TableCell::TableCell( const TableCell& _tableCell ) : cp(_tableCell.cp), depth(_tableCell.depth), documentMapping(_tableCell.documentMapping)
	{
		for ( list<ITableCellElementPtr>::const_iterator iter = _tableCell.cellElements.begin(); iter != _tableCell.cellElements.end(); iter++ )
		{
			this->AddItem( **iter );
		}
	}

	TableCell::~TableCell()
	{
	}

	TableCell& TableCell::operator = ( const TableCell& _tableCell )
	{
		if ( this != &_tableCell )
		{
			this->cp = _tableCell.cp;
			this->depth = _tableCell.depth;
			this->cellElements.clear();

			for ( list<ITableCellElementPtr>::const_iterator iter = _tableCell.cellElements.begin(); iter != _tableCell.cellElements.end(); iter++ )
			{
				this->AddItem( **iter );
			}

			this->documentMapping = _tableCell.documentMapping;
		}

		return *this;
	}

	void TableCell::SetCP( int _cp )
	{
		this->cp = _cp;
	}

	int TableCell::GetCP() const
	{
		return this->cp;
	}

	void TableCell::SetDepth( unsigned int _depth )
	{
		this->depth = _depth;
	}

	unsigned int TableCell::GetDepth() const
	{
		return this->depth;
	}

	void TableCell::AddItem( const ITableCellElement& _tableCellElement )
	{
		this->cellElements.push_back( ITableCellElementPtr( static_cast<ITableCellElement*>( _tableCellElement.Clone() ) ) );
	}

	bool TableCell::IsEmpty() const
	{
		return this->cellElements.empty();
	}

	void TableCell::Clear()
	{
		this->cp = 0;
		this->cellElements.clear();
	}

	void TableCell::Convert(IMapping* mapping, TablePropertyExceptions* tapx, const vector<short>* grid, int& gridIndex, int nCellIndex)
	{
		if (NULL != mapping)
		{
			documentMapping = static_cast<DocumentMapping*>(mapping);
		}

		ParagraphPropertyExceptions* papxBackup = documentMapping->_lastValidPapx;
		SectionPropertyExceptions* sepxBackup	= documentMapping->_lastValidSepx;

		//start w:tc
		documentMapping->GetXMLWriter()->WriteNodeBegin( _T( "w:tc" ) );

		//find cell end
		int cpCellEnd = documentMapping->findCellEndCp(cp, depth);

		//convert the properties
		TableCellPropertiesMapping tcpMapping(documentMapping->GetXMLWriter(), grid, gridIndex, nCellIndex);

		if ( tapx != NULL )
		{
			tapx->Convert( &tcpMapping );
		}

		gridIndex += tcpMapping.GetGridSpan();

		documentMapping->_lastValidPapx = papxBackup;
		documentMapping->_lastValidSepx = sepxBackup;

		for (list<ITableCellElementPtr>::iterator iter = cellElements.begin(); iter != cellElements.end(); ++iter)
		{
			(*iter)->Convert( mapping );
		}

		//end w:tc
		documentMapping->GetXMLWriter()->WriteNodeEnd( _T( "w:tc" ) );
	}

}

namespace DocFileFormat
{
	TableRow::TableRow (DocumentMapping* _documentMapping, int _cp, unsigned int _depth) : cp(_cp), depth(_depth), documentMapping(_documentMapping)
	{
	}

	TableRow::~TableRow()
	{
	}
		
	void TableRow::SetCP( int _cp )
	{
		this->cp = _cp;
	}

	int TableRow::GetCP() const
	{
		return this->cp;
	}

	void TableRow::SetDepth( unsigned int _depth )
	{
		this->depth = _depth;
	}

	unsigned int TableRow::GetDepth() const
	{
		return this->depth;
	}

	void TableRow::AddCell( const TableCell& _tableCell )
	{
		this->cells.push_back( _tableCell );
	}

	bool TableRow::IsEmpty() const
	{
		return this->cells.empty();
	}

	void TableRow::Clear()
	{
		this->cp = 0;
		this->depth = 1;
		this->cells.clear();
	}

	void TableRow::Convert(IMapping* mapping, const vector<short>* grid)
	{
		if ( mapping != NULL )
		{
			documentMapping	=	static_cast<DocumentMapping*>(mapping);
		}

		if ( grid != NULL )
		{
			int gridIndex	=	0;
			int nCellIndex	=	0;

			ParagraphPropertyExceptions* papxBackup = this->documentMapping->_lastValidPapx;
			SectionPropertyExceptions* sepxBackup = this->documentMapping->_lastValidSepx;

			//start w:tr
			this->documentMapping->GetXMLWriter()->WriteNodeBegin( _T( "w:tr" ) );

			//convert the properties
			int fcRowEnd = documentMapping->findRowEndFc(cp, depth);
			TablePropertyExceptions tapx( this->documentMapping->findValidPapx( fcRowEnd ), this->documentMapping->m_document->DataStream );
			list<CharacterPropertyExceptions*>* chpxs = this->documentMapping->m_document->GetCharacterPropertyExceptions( fcRowEnd, fcRowEnd + 1 );
			TableRowPropertiesMapping trpMapping( this->documentMapping->GetXMLWriter(), *(chpxs->begin()) );
			tapx.Convert( &trpMapping );

			this->documentMapping->_lastValidPapx = papxBackup;
			this->documentMapping->_lastValidSepx = sepxBackup;

			for ( list<TableCell>::iterator iter = this->cells.begin(); iter != this->cells.end(); iter++ )
			{
				iter->Convert( mapping, &tapx, grid, gridIndex, nCellIndex++ );
			}

			//end w:tr
			this->documentMapping->GetXMLWriter()->WriteNodeEnd( _T( "w:tr" ) );

			RELEASEOBJECT( chpxs );
		}
	}
}

namespace DocFileFormat
{
	DocParagraph::DocParagraph (DocumentMapping* _documentMapping, int _cpStart, int _cpEnd):	cpStart(_cpStart), cpEnd(_cpEnd), documentMapping(_documentMapping)
	{
	}

	DocParagraph::~DocParagraph()
	{
	}

	int DocParagraph::GetCPStart() const
	{
		return this->cpStart;
	}

	void DocParagraph::SetCPStart( int _cpStart )
	{
		this->cpStart = _cpStart;  
	}

	int DocParagraph::GetCPEnd() const
	{
		return this->cpEnd;
	}

	void DocParagraph::SetCPEnd( int _cpEnd )
	{
		this->cpEnd = _cpEnd;
	}

	IVirtualConstructor* DocParagraph::New() const
	{
		return new DocParagraph( this->documentMapping );
	}

	IVirtualConstructor* DocParagraph::Clone() const
	{
		return new DocParagraph( *this );
	}

	void DocParagraph::Convert( IMapping* mapping )
	{
		if ( mapping != NULL )
		{
			this->documentMapping = static_cast<DocumentMapping*>(mapping);
		}

		this->documentMapping->writeParagraph( this->cpStart );
	}

}

namespace DocFileFormat
{
	Table::~Table()
	{
	}

	bool Table::IsCellMarker(int _cp)
	{
		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = NULL;

		papx = documentMapping->findValidPapx(fc);

		TableInfo tai( papx );

		return ( ( tai.fInTable ) && ( ( ( documentMapping->m_document->Text->at( _cp ) == 0x0007 ) && ( tai.iTap <= 1 ) && 
			( !tai.fTtp ) ) ||
			( ( documentMapping->m_document->Text->at( _cp ) == 0x000D ) && ( tai.iTap > 1 ) && 
			( tai.fInnerTableCell ) && ( !tai.fInnerTtp ) ) ) );
	}

	bool Table::IsRowMarker( int _cp )
	{
		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = NULL;

		papx = this->documentMapping->findValidPapx( fc );

		TableInfo tai( papx );

		return ( ( tai.fInTable ) && ( ( ( this->documentMapping->m_document->Text->at( _cp ) == 0x0007 ) && ( tai.iTap <= 1 ) &&
			( tai.fTtp ) ) ||
			( ( this->documentMapping->m_document->Text->at( _cp ) == 0x000D ) && ( tai.iTap > 1 ) && 
			( tai.fInnerTtp ) ) ) );
	}

	bool Table::IsParagraphMarker( int _cp )
	{
		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = NULL;

		papx = this->documentMapping->findValidPapx( fc );

		TableInfo tai( papx );

		return (  ( tai.fInTable ) && ( this->documentMapping->m_document->Text->at( _cp ) == 0x000D ) && 
			( !this->IsCellMarker( _cp ) ) && ( !this->IsRowMarker( _cp ) ) );
	}

	Table::Table( DocumentMapping* _documentMapping, int _cp, unsigned int _depth ):
	cpStart(_cp), cpEnd(_cp), depth(_depth), documentMapping(_documentMapping)
	{
		if ( this->documentMapping != NULL )
		{
			int paragraphBeginCP = _cp;

			ParagraphPropertyExceptions* papxBackup = this->documentMapping->_lastValidPapx;
			SectionPropertyExceptions* sepxBackup = this->documentMapping->_lastValidSepx;

			int fc = documentMapping->m_document->FindFileCharPos(_cp);
			if (fc < 0) return;

			ParagraphPropertyExceptions* papx = NULL;

			papx = this->documentMapping->findValidPapx( fc );

			TableInfo tai( papx );

			TableRow tableRow( this->documentMapping, _cp );
			TableCell tableCell( this->documentMapping, _cp );

			do
			{
				fc = documentMapping->m_document->FindFileCharPos(_cp);
				if (fc < 0) break;

				papx = this->documentMapping->findValidPapx( fc );

				tai = TableInfo( papx );

				if ( tai.iTap > _depth )
				{
					Table innerTable( this->documentMapping, _cp, ( _depth + 1 ) );

					tableCell.AddItem( innerTable );

					_cp = innerTable.GetCPEnd();

					fc = documentMapping->m_document->FindFileCharPos(_cp);
					if (fc < 0) break;

					papx = this->documentMapping->findValidPapx( fc );

					tai = TableInfo( papx );

					paragraphBeginCP = _cp;
				}
				else
				{
					if ( this->IsCellMarker( _cp ) )
					{
						tableCell.SetCP( _cp );
						tableCell.SetDepth( _depth );

						tableCell.AddItem( DocParagraph( this->documentMapping, paragraphBeginCP, _cp ) );  

						tableRow.AddCell( tableCell );
						tableCell.Clear();
						paragraphBeginCP = ( _cp + 1 );
					}
					else if ( this->IsRowMarker( _cp ) )
					{
						tableRow.SetCP( _cp );
						tableRow.SetDepth( _depth );
						this->AddRow( tableRow );
						tableRow.Clear();
						paragraphBeginCP++;
					}
					else if ( this->IsParagraphMarker( _cp ) )
					{
						tableCell.AddItem( DocParagraph( this->documentMapping, paragraphBeginCP, _cp ) );
						paragraphBeginCP = ( _cp + 1 );
					}

					_cp++;
				}
			}
			while ( ( tai.fInTable ) && ( tai.iTap == _depth ) );

			this->cpEnd = ( _cp - 1 );

			this->documentMapping->_lastValidPapx = papxBackup;
			this->documentMapping->_lastValidSepx = sepxBackup;
		}
	}

	int Table::GetCPStart() const
	{
		return this->cpStart;
	}

	void Table::SetCPStart( int _cpStart )
	{
		this->cpStart = _cpStart;  
	}

	int Table::GetCPEnd() const
	{
		return this->cpEnd;
	}

	void Table::SetCPEnd( int _cpEnd )
	{
		this->cpEnd = _cpEnd;
	}

	bool Table::IsEmpty() const
	{
		return this->rows.empty();
	}

	void Table::Clear()
	{
		this->cpStart = 0;
		this->cpEnd = 0;
		this->rows.clear();
	}

	void Table::SetDepth( unsigned int _depth )
	{
		this->depth = _depth;
	}

	unsigned int Table::GetDepth() const
	{
		return this->depth;
	}

	int Table::AddRow( const TableRow& _tableRow )
	{
		this->rows.push_back( _tableRow );

		return this->cpEnd;
	}

	IVirtualConstructor* Table::New() const
	{
		return new Table( this->documentMapping );
	}

	IVirtualConstructor* Table::Clone() const
	{
		return new Table( *this );
	}

	void Table::Convert (IMapping* mapping)
	{
		if (mapping)
			documentMapping	= static_cast<DocumentMapping*>(mapping);

		ParagraphPropertyExceptions* papxBackup	=	documentMapping->_lastValidPapx;
		SectionPropertyExceptions* sepxBackup	=	documentMapping->_lastValidSepx;

		//build the table grid
		vector<short>* grid = this->documentMapping->buildTableGrid( this->cpStart, this->depth );

		//find first row end
		int fcRowEnd = this->documentMapping->findRowEndFc( this->cpStart, this->depth );

		TablePropertyExceptions row1Tapx( this->documentMapping->findValidPapx( fcRowEnd ), this->documentMapping->m_document->DataStream );

		//start table
		this->documentMapping->GetXMLWriter()->WriteNodeBegin( _T( "w:tbl" ) );

		//Convert it
		TablePropertiesMapping tpMapping( this->documentMapping->GetXMLWriter(), this->documentMapping->m_document->Styles, grid );

		row1Tapx.Convert( &tpMapping );

		this->documentMapping->_lastValidPapx = papxBackup;
		this->documentMapping->_lastValidSepx = sepxBackup;

		for ( list<TableRow>::iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
		{
			iter->Convert( mapping, grid );  
		}

		//close w:tbl
		this->documentMapping->GetXMLWriter()->WriteNodeEnd( _T( "w:tbl" ) );

		RELEASEOBJECT( grid );
	}

}