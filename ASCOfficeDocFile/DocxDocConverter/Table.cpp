
#include "Table.h"

namespace ASCDocFileFormat
{
	void Table::SetDepth( unsigned int _depth )
	{
		this->depth = _depth;
	}

	/*========================================================================================================*/

	unsigned int Table::GetDepth() const
	{
		return this->depth;
	}

	/*========================================================================================================*/

	Table::Table():
	depth(1), tableRowsOffset(0)
	{
	}

	/*========================================================================================================*/

	Table::~Table()
	{
	}

	/*========================================================================================================*/

	Table::Table( const Table& table ):
	rows(table.rows), depth(table.depth), tableRowsOffset(table.tableRowsOffset)
	{
	}

	/*========================================================================================================*/

	void Table::AddRow( const TableRow& row )
	{
		TableRow newRow( row );

		const unsigned char PFInTable = 1;
		const int PItap = row.GetDepth();
		const unsigned char PFInnerTableCell = 1;

		newRow.AddProperty( (short)DocFileFormat::sprmPFInTable, (void*)(&PFInTable) );
		newRow.AddProperty( (short)DocFileFormat::sprmPItap, (void*)(&PItap) );

		if ( row.GetDepth() <= 1 )
		{
			const unsigned char PFTtp = 1;
			newRow.AddProperty( (short)DocFileFormat::sprmPFTtp, (void*)(&PFTtp) );
		}
		else
		{
			const unsigned char PFInnerTtp = 1;
			newRow.AddProperty( (short)DocFileFormat::sprmPFInnerTtp, (void*)(&PFInnerTtp) );
			newRow.AddProperty( (short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell) );
		}

		this->rows.push_back( TableRowWithOffset( newRow, this->tableRowsOffset ) );
		this->tableRowsOffset += ( sizeof(wchar_t) * row.GetAllText().size() );
	}

	/*========================================================================================================*/

	wstring Table::GetAllText() const
	{
		wstring allText;

		for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
		{
			allText += iter->tableRow.GetAllText();
		}

		return allText;
	}

	/*========================================================================================================*/

	Table::operator wstring() const
	{
		wstring allText;

		for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
		{
			allText += iter->tableRow;
		}

		return allText;
	}

	/*========================================================================================================*/

	IVirtualConstructor* Table::New() const
	{
		return new Table();
	}

	/*========================================================================================================*/

	IVirtualConstructor* Table::Clone() const
	{
		return new Table( *this );
	}

	/*========================================================================================================*/

	vector<TextItemPtr> Table::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
		{
			vector<TextItemPtr> tableRowParagraphs = iter->tableRow.GetAllParagraphsCopy();

			for ( vector<TextItemPtr>::const_iterator tableRowParagraphsIter = tableRowParagraphs.begin(); tableRowParagraphsIter != tableRowParagraphs.end(); tableRowParagraphsIter++ )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*tableRowParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	vector<ITextItem*> Table::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for ( list<TableRowWithOffset>::iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
		{
			vector<ITextItem*> tableRowParagraphs = iter->tableRow.GetAllParagraphs();

			for ( vector<ITextItem*>::iterator tableRowParagraphsIter = tableRowParagraphs.begin(); tableRowParagraphsIter != tableRowParagraphs.end(); tableRowParagraphsIter++ )
			{
				allParagraphs.push_back( *tableRowParagraphsIter );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Table::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
			{
				vector<unsigned int> allRowRunsOffsets;
				vector<IParagraphItemPtr> allRowRuns = iter->tableRow.GetAllRunsCopy( &allRowRunsOffsets );

				for ( unsigned int i = 0; i < allRowRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allRowRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allRowRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->tableRow.GetAllText().size() );
			}
		}

		return allRuns;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Table::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int paragraphItemOffset = 0;

			for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
			{
				vector<unsigned int> allRowParagraphItemsOffsets;
				vector<IParagraphItemPtr> allRowParagraphItems = iter->tableRow.GetAllParagraphItemsCopy( &allRowParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allRowParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allRowParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( paragraphItemOffset + allRowParagraphItemsOffsets[i] );
				}

				paragraphItemOffset += ( sizeof(wchar_t) * iter->tableRow.GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	/*========================================================================================================*/

	vector<PapxInFkp> Table::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if ( allParagraphsOffsets != NULL )
		{
			for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
			{
				vector<unsigned int> tableRowParagraphsOffsets;
				vector<PapxInFkp> tableRowParagraphsProperties = iter->tableRow.GetAllParagraphsProperties( &tableRowParagraphsOffsets );

				for ( unsigned int i = 0; i < tableRowParagraphsProperties.size(); i++ )
				{
					allParagraphsProperties.push_back( tableRowParagraphsProperties[i] );
					allParagraphsOffsets->push_back( paragraphOffset + tableRowParagraphsOffsets[i] );
				}

				paragraphOffset += ( sizeof(wchar_t) * iter->tableRow.GetAllText().size() );
			}
		}

		return allParagraphsProperties;
	}

	/*========================================================================================================*/

	vector<Chpx> Table::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if ( allRunsOffsets != NULL )
		{
			for ( list<TableRowWithOffset>::const_iterator iter = this->rows.begin(); iter != this->rows.end(); iter++ )
			{
				vector<unsigned int> tableRowRunsOffsets;
				vector<Chpx> tableRowRunsProperties = iter->tableRow.GetAllRunProperties( &tableRowRunsOffsets );

				for ( unsigned int i = 0; i < tableRowRunsProperties.size(); i++ )
				{
					allRunsProperties.push_back( tableRowRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + tableRowRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->tableRow.GetAllText().size() );
			}
		}

		return allRunsProperties;
	}
}