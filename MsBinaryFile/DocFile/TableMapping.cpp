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

#include "TableMapping.h"
#include "TablePropertiesMapping.h"

namespace DocFileFormat
{
	TableCell::TableCell( DocumentMapping* _documentMapping, int _cp, unsigned int _depth) : cp(_cp), depth(_depth), documentMapping(_documentMapping)
	{
	}

	TableCell::TableCell( const TableCell& _tableCell ) : cp(_tableCell.cp), depth(_tableCell.depth), documentMapping(_tableCell.documentMapping)
	{
		for ( std::list<ITableCellElementPtr>::const_iterator iter = _tableCell.cellElements.begin(); iter != _tableCell.cellElements.end(); iter++ )
		{
			AddItem( **iter );
		}
	}

	TableCell::~TableCell()
	{
	}

	TableCell& TableCell::operator = ( const TableCell& _tableCell )
	{
		if ( this != &_tableCell )
		{
			cp = _tableCell.cp;
			depth = _tableCell.depth;
			cellElements.clear();

			for ( std::list<ITableCellElementPtr>::const_iterator iter = _tableCell.cellElements.begin(); iter != _tableCell.cellElements.end(); iter++ )
			{
				AddItem( **iter );
			}

			documentMapping = _tableCell.documentMapping;
		}

		return *this;
	}

	void TableCell::SetCP( int _cp )
	{
		cp = _cp;
	}

	int TableCell::GetCP() const
	{
		return cp;
	}

	void TableCell::SetDepth( unsigned int _depth )
	{
		depth = _depth;
	}

	unsigned int TableCell::GetDepth() const
	{
		return depth;
	}

	void TableCell::AddItem( const ITableCellElement& _tableCellElement )
	{
		cellElements.push_back( ITableCellElementPtr( static_cast<ITableCellElement*>( _tableCellElement.Clone() ) ) );
	}

	bool TableCell::IsEmpty() const
	{
		return cellElements.empty();
	}

	void TableCell::Clear()
	{
		cp = 0;
		cellElements.clear();
	}

	void TableCell::Convert(IMapping* mapping, TablePropertyExceptions* tapx, const std::vector<short>* grid, int& gridIndex, int nCellIndex)
	{
		if (NULL != mapping)
		{
			documentMapping = static_cast<DocumentMapping*>(mapping);
		}

		ParagraphPropertyExceptions* papxBackup = documentMapping->_lastValidPapx;
		SectionPropertyExceptions* sepxBackup	= documentMapping->_lastValidSepx;

	//find cell end
		int cpCellEnd = documentMapping->findCellEndCp(cp, depth);
		
	//convert the properties
		XMLTools::CStringXmlWriter writerTcPr;
		TableCellPropertiesMapping tcpMapping(&writerTcPr, grid, gridIndex, nCellIndex, depth);

		if ( tapx != NULL )
		{
			tapx->Convert( &tcpMapping );
		}

		gridIndex += tcpMapping.GetGridSpan();
		
		if (tcpMapping.IsCoverCell())
		{
			return;
		}		
	//start w:tc
		documentMapping->GetXMLWriter()->WriteNodeBegin( L"w:tc" );
		
		documentMapping->GetXMLWriter()->WriteString(writerTcPr.GetXmlString());

		documentMapping->_lastValidPapx = papxBackup;
		documentMapping->_lastValidSepx = sepxBackup;

		for (std::list<ITableCellElementPtr>::iterator iter = cellElements.begin(); iter != cellElements.end(); ++iter)
		{
			(*iter)->Convert( mapping );
		}

	//end w:tc
        documentMapping->GetXMLWriter()->WriteNodeEnd( L"w:tc" );
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
		cp = _cp;
	}

	int TableRow::GetCP() const
	{
		return cp;
	}

	void TableRow::SetDepth( unsigned int _depth )
	{
		depth = _depth;
	}

	unsigned int TableRow::GetDepth() const
	{
		return depth;
	}

	void TableRow::AddCell( const TableCell& _tableCell )
	{
		cells.push_back( _tableCell );
	}

	bool TableRow::IsEmpty() const
	{
		return cells.empty();
	}

	void TableRow::Clear()
	{
		cp = 0;
		depth = 1;
		cells.clear();
	}

	void TableRow::Convert(IMapping* mapping, const std::vector<short>* grid)
	{
		if ( mapping != NULL )
		{
			documentMapping	=	static_cast<DocumentMapping*>(mapping);
		}

		if ( grid != NULL )
		{
			int gridIndex	=	0;
			int nCellIndex	=	0;

			ParagraphPropertyExceptions* papxBackup = documentMapping->_lastValidPapx;
			SectionPropertyExceptions* sepxBackup = documentMapping->_lastValidSepx;

			//start w:tr
			documentMapping->GetXMLWriter()->WriteNodeBegin( L"w:tr" );

			//convert the properties
			int fcRowEnd = documentMapping->findRowEndFc(cp, depth);
			TablePropertyExceptions tapx (	documentMapping->findValidPapx( fcRowEnd ), 
											documentMapping->m_document->DataStream,
											documentMapping->m_document->nWordVersion);
			
			std::list<CharacterPropertyExceptions*>* chpxs = documentMapping->m_document->GetCharacterPropertyExceptions( fcRowEnd, fcRowEnd + 1 );
			if (chpxs)
			{
				TableRowPropertiesMapping trpMapping( documentMapping->GetXMLWriter(), *(chpxs->begin()) );
				tapx.Convert( &trpMapping );
			}

			documentMapping->_lastValidPapx = papxBackup;
			documentMapping->_lastValidSepx = sepxBackup;

			if (cells.empty())
			{
				documentMapping->GetXMLWriter()->WriteNodeBegin(L"w:tc");
					documentMapping->GetXMLWriter()->WriteNode(L"w:p", L"");
				documentMapping->GetXMLWriter()->WriteNodeEnd(L"w:tc");
			}
			else
			{
				for ( std::list<TableCell>::iterator iter = cells.begin(); iter != cells.end(); iter++ )
				{
					iter->Convert( mapping, &tapx, grid, gridIndex, nCellIndex++);
				}
			}

			//end w:tr
			documentMapping->GetXMLWriter()->WriteNodeEnd( L"w:tr" );

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
		return cpStart;
	}

	void DocParagraph::SetCPStart( int _cpStart )
	{
		cpStart = _cpStart;  
	}

	int DocParagraph::GetCPEnd() const
	{
		return cpEnd;
	}

	void DocParagraph::SetCPEnd( int _cpEnd )
	{
		cpEnd = _cpEnd;
	}

	IVirtualConstructor* DocParagraph::New() const
	{
		return new DocParagraph( documentMapping );
	}

	IVirtualConstructor* DocParagraph::Clone() const
	{
		return new DocParagraph( *this );
	}

	void DocParagraph::Convert( IMapping* mapping )
	{
		if ( mapping != NULL )
		{
			documentMapping = static_cast<DocumentMapping*>(mapping);
		}

		documentMapping->writeParagraph( cpStart, 0x7fffffff );
	}

}

namespace DocFileFormat
{
	Table::~Table()
	{
	}

	bool Table::IsCellMarker(int _cp, bool & bBadMarker)
	{
		bBadMarker = false;

		if ( _cp > documentMapping->m_document->Text->size() - 1) return false;
		
		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = documentMapping->findValidPapx(fc);

		TableInfo tai( papx, documentMapping->m_document->nWordVersion );

		bool bCell = tai.fInTable && (documentMapping->m_document->Text->at(_cp) == 0x0007) && (tai.iTap <= 1) && (!tai.fTtp);
		if (bCell) return true;

		bCell = (documentMapping->m_document->Text->at(_cp) == 0x000D) && (tai.iTap > 1) && tai.fInnerTableCell && !tai.fInnerTtp;
		if (bCell)
		{
			int fc_1 = documentMapping->m_document->FindFileCharPos(_cp - 1);
			int fc_2 = documentMapping->m_document->FindFileCharPos(_cp + 1);
			
			ParagraphPropertyExceptions* papx_1 = documentMapping->findValidPapx(fc_1);
			ParagraphPropertyExceptions* papx_2 = documentMapping->findValidPapx(fc_2);
			
			if (papx_1 == papx_2)
			{
				bBadMarker = true;
				return false;
			}
			return true;
		}

		return false;
	}

	bool Table::IsRowMarker( int _cp )
	{
		if ( _cp > documentMapping->m_document->Text->size() - 1) return false;

		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = NULL;

		papx = documentMapping->findValidPapx( fc );

		TableInfo tai( papx, documentMapping->m_document->nWordVersion );

		bool bRow = tai.fInTable && (documentMapping->m_document->Text->at(_cp) == 0x0007) && (tai.iTap <= 1) && tai.fTtp;
		if (bRow) return true;

		bRow = (documentMapping->m_document->Text->at( _cp ) == 0x000D) && ( tai.iTap > 1 ) && tai.fInnerTtp;
		if (bRow) return true;
		
		return false;
	}

	bool Table::IsParagraphMarker( int _cp )
	{
		if ( _cp > documentMapping->m_document->Text->size() - 1) return false;
		
		int fc = documentMapping->m_document->FindFileCharPos(_cp);
		
		if (fc < 0) return false;

		ParagraphPropertyExceptions* papx = NULL;

		papx = documentMapping->findValidPapx( fc );

		TableInfo tai( papx, documentMapping->m_document->nWordVersion );

		bool bBadMarker = false;
		return (  ( tai.fInTable ) && ( documentMapping->m_document->Text->at( _cp ) == 0x000D ) && 
			( !IsCellMarker( _cp, bBadMarker) ) && ( !IsRowMarker( _cp ) ) );
	}

	Table::Table( DocumentMapping* _documentMapping, int _cp, unsigned int _depth ):
					cpStart(_cp), cpEnd(_cp), depth(_depth), documentMapping(_documentMapping)
	{
		if ( documentMapping != NULL )
		{
			int paragraphBeginCP = _cp;
			int lastCellCP = _cp;

			ParagraphPropertyExceptions* papxBackup = documentMapping->_lastValidPapx;
			SectionPropertyExceptions* sepxBackup = documentMapping->_lastValidSepx;

			int fc = documentMapping->m_document->FindFileCharPos(_cp);
			if (fc < 0) return;

			ParagraphPropertyExceptions* papx = NULL;

			papx = documentMapping->findValidPapx( fc );

			TableInfo tai( papx, documentMapping->m_document->nWordVersion );

			TableRow	tableRow	( documentMapping, _cp );
			TableCell	tableCell	( documentMapping, _cp );

			do
			{
				fc = documentMapping->m_document->FindFileCharPos(_cp);
				if (fc < 0) break;

				papx = documentMapping->findValidPapx( fc );

				tai = TableInfo( papx, documentMapping->m_document->nWordVersion );

				if ( tai.iTap > _depth )
				{
					Table innerTable( documentMapping, _cp, ( _depth + 1 ) );

					tableCell.AddItem( innerTable );

					_cp = innerTable.GetCPEnd();

					fc = documentMapping->m_document->FindFileCharPos(_cp);
					if (fc < 0) break;

					papx = documentMapping->findValidPapx( fc );

					tai = TableInfo( papx, documentMapping->m_document->nWordVersion );

					paragraphBeginCP = _cp;
				}
				else
				{
					bool bBadMarker = false;
					if ( IsCellMarker( _cp, bBadMarker) )
					{
						lastCellCP = _cp;
						tableCell.SetCP( _cp );
						tableCell.SetDepth( _depth );

						tableCell.AddItem( DocParagraph( documentMapping, paragraphBeginCP, _cp ) );  

						tableRow.AddCell( tableCell );
						tableCell.Clear();
						paragraphBeginCP = ( _cp + 1 );
					}
					else if ( IsRowMarker( _cp ) )
					{
						if (_cp - lastCellCP > 10) //???
						{
							tableCell.SetCP(_cp);
							tableCell.SetDepth(_depth);

							tableCell.AddItem(DocParagraph(documentMapping, paragraphBeginCP, _cp));

							tableRow.AddCell(tableCell);
							tableCell.Clear();
							paragraphBeginCP = _cp;
						}
						tableRow.SetCP( _cp );
						tableRow.SetDepth( _depth );
						AddRow( tableRow );
						tableRow.Clear();
						paragraphBeginCP++;
					}
					else if ( !bBadMarker && IsParagraphMarker( _cp ) )
					{
						tableCell.AddItem( DocParagraph( documentMapping, paragraphBeginCP, _cp ) );
						paragraphBeginCP = ( _cp + 1 );
					}
					_cp++;
				}
			}
			while ( ( tai.fInTable ) && ( tai.iTap == _depth ) );

			cpEnd = ( _cp - 1 );

			documentMapping->_lastValidPapx = papxBackup;
			documentMapping->_lastValidSepx = sepxBackup;
		}
	}

	int Table::GetCPStart() const
	{
		return cpStart;
	}

	void Table::SetCPStart( int _cpStart )
	{
		cpStart = _cpStart;  
	}

	int Table::GetCPEnd() const
	{
		return cpEnd;
	}

	void Table::SetCPEnd( int _cpEnd )
	{
		cpEnd = _cpEnd;
	}

	bool Table::IsEmpty() const
	{
		return rows.empty();
	}

	void Table::Clear()
	{
		cpStart = 0;
		cpEnd = 0;
		rows.clear();
	}

	void Table::SetDepth( unsigned int _depth )
	{
		depth = _depth;
	}

	unsigned int Table::GetDepth() const
	{
		return depth;
	}

	int Table::AddRow( const TableRow& _tableRow )
	{
		rows.push_back( _tableRow );

		return cpEnd;
	}

	IVirtualConstructor* Table::New() const
	{
		return new Table( documentMapping );
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
		std::vector<short> grid;
		documentMapping->buildTableGrid( cpStart, depth, grid);

		//find first row end
		int fcRowEnd = documentMapping->findRowEndFc( cpStart, depth );

		TablePropertyExceptions row1Tapx(	documentMapping->findValidPapx( fcRowEnd ), 
											documentMapping->m_document->DataStream ,
											documentMapping->m_document->nWordVersion);

		//start table
        documentMapping->GetXMLWriter()->WriteNodeBegin( L"w:tbl" );

		//Convert it
		TablePropertiesMapping tpMapping( documentMapping->GetXMLWriter(), documentMapping->m_document->Styles, &grid);

		row1Tapx.Convert( &tpMapping );

		documentMapping->_lastValidPapx = papxBackup;
		documentMapping->_lastValidSepx = sepxBackup;

		for ( std::list<TableRow>::iterator iter = rows.begin(); iter != rows.end(); iter++ )
		{
			iter->Convert( mapping, &grid );  
		}

		//close w:tbl
        documentMapping->GetXMLWriter()->WriteNodeEnd( L"w:tbl" );

	}

}
