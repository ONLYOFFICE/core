#include "stdafx.h"
#include "TableCell.h"

namespace ASCDocFileFormat
{
	void TableCell::SetDepth( unsigned int _depth )
	{
		this->depth = _depth;

		for ( list<TableCellItemWithOffset>::iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			Paragraph* paragraph = dynamic_cast<Paragraph*>(iter->tableCellItem.operator->());

			if ( paragraph != NULL )
			{
				paragraph->SetDepth( this->depth );
			}
		}
		if ( this->depth > 1 )
		{
			Paragraph* paragraph = dynamic_cast<Paragraph*>(this->textItems.back().tableCellItem.operator->());

			if ( paragraph != NULL )
			{
				const byte PFInnerTableCell = 1;

				paragraph->AddProperty( (short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell) );
			}
		}
	}

	/*========================================================================================================*/

	unsigned int TableCell::GetDepth() const
	{
		return this->depth;
	}

	/*========================================================================================================*/

	TableCell::TableCell():
	depth(1), tableCellItemsOffset(0)
	{
	}

	/*========================================================================================================*/

	TableCell::TableCell( unsigned int _depth ):
	depth(_depth), tableCellItemsOffset(0)
	{
	}

	/*========================================================================================================*/

	TableCell::~TableCell()
	{
	}

	/*========================================================================================================*/

	TableCell::TableCell( const TableCell& tableCell ):
	depth(tableCell.depth), properties(tableCell.properties), tableCellItemsOffset(tableCell.tableCellItemsOffset)
	{
		for ( list<TableCellItemWithOffset>::const_iterator iter = tableCell.textItems.begin(); iter != tableCell.textItems.end(); iter++ )
		{
			this->textItems.push_back( TableCellItemWithOffset( TextItemPtr( static_cast<ITextItem*>( iter->tableCellItem->Clone() ) ), iter->tableCellItemOffset ) );
		}
	}

	/*========================================================================================================*/

	void TableCell::AddTextItem( const ITextItem& _textItem )
	{
		TextItemPtr newTextItem( static_cast<ITextItem*>( _textItem.Clone() ) );

		const byte PFInnerTableCell = 1;
		const byte PFInTable = 1;

		if ( !textItems.empty() )
		{
			Paragraph* paragraph = dynamic_cast<Paragraph*>(textItems.back().tableCellItem.operator->());

			if ( paragraph != NULL )
			{
				paragraph->RemoveProperty((short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell));
			}
		}

		Paragraph* paragraph = dynamic_cast<Paragraph*>(newTextItem.operator->());

		if ( paragraph != NULL )
		{
			if ( this->depth > 1 )
			{
				paragraph->AddProperty( (short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell) );
			}

			paragraph->SetDepth( this->depth );

			this->RemoveProperty( (short)DocFileFormat::sprmPFInTable, (void*)(&PFInTable) );
			this->RemoveProperty( (short)DocFileFormat::sprmPItap, (void*)(&this->depth) );
			this->RemoveProperty( (short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell) );
		}
		else
		{
			this->AddProperty( (short)DocFileFormat::sprmPFInTable, (void*)(&PFInTable) );
			this->AddProperty( (short)DocFileFormat::sprmPItap, (void*)(&this->depth) );
			this->AddProperty( (short)DocFileFormat::sprmPFInnerTableCell, (void*)(&PFInnerTableCell) );
		}

		this->textItems.push_back( TableCellItemWithOffset( newTextItem, this->tableCellItemsOffset ) );
		this->tableCellItemsOffset += ( sizeof(WCHAR) * newTextItem.operator->()->GetAllText().size() );

		//!!!TODO: Add empty run, if this is a paragraph.
		//(There is issue with table content, when last item in table cell is a picture)!!!
		if ( paragraph != NULL )
		{
			paragraph->AddParagraphItem( Run() );
		}
	}

	/*========================================================================================================*/

	wstring TableCell::GetAllText() const
	{
		wstring allText;

		for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += iter->tableCellItem->GetAllText();
		}

		if ( !this->textItems.empty() )
		{
			if ( typeid(*this->textItems.back().tableCellItem) == typeid(Paragraph) )
			{
				if ( this->depth > 1 )
				{
					allText[allText.size() - 1] = TextMark::ParagraphEnd;
				}
				else
				{
					allText[allText.size() - 1] = TextMark::CellOrRowMark;
				}
			}
			else
			{
				if ( this->depth > 1 )
				{
					allText += TextMark::ParagraphEnd;
				}
				else
				{
					allText += TextMark::CellOrRowMark;
				}
			}
		}

		return allText;
	}

	/*========================================================================================================*/

	TableCell::operator wstring() const
	{
		wstring allText;

		for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += *(iter->tableCellItem);
		}

		return allText;
	}

	/*========================================================================================================*/

	vector<TextItemPtr> TableCell::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			vector<TextItemPtr> textItemParagraphs = iter->tableCellItem->GetAllParagraphsCopy();

			for ( vector<TextItemPtr>::const_iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	vector<ITextItem*> TableCell::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for ( list<TableCellItemWithOffset>::iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			vector<ITextItem*> textItemParagraphs = iter->tableCellItem->GetAllParagraphs();

			for ( vector<ITextItem*>::iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( *textItemParagraphsIter );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	void TableCell::AddProperty( short sprm, void* operand )
	{
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
		this->properties.push_back( prl );
	}

	/*========================================================================================================*/

	void TableCell::RemoveProperty( short sprm, void* operand )
	{
		Prl prl( sprm, reinterpret_cast<byte*>( operand ) );
		this->properties.remove( prl );  
	}

	/*========================================================================================================*/

	void TableCell::AddProperties( const vector<Prl>& prls )
	{
		for ( vector<Prl>::const_iterator iter = prls.begin(); iter != prls.end(); iter++ )
		{
			this->properties.push_back( *iter ); 
		}
	}

	/*========================================================================================================*/

	void TableCell::RemoveAllProperties()
	{
		this->properties.clear();
	}

	/*========================================================================================================*/

	PapxInFkp TableCell::GetAllProperties() const
	{
		vector<Prl> prls( this->properties.begin(), this->properties.end() );
		GrpPrlAndIstd gpai( 0, prls );
		return PapxInFkp( gpai );
	}

	/*========================================================================================================*/

	unsigned int TableCell::GetAllPropertiesCount() const
	{
		return this->properties.size();
	}

	/*========================================================================================================*/

	vector<PapxInFkp> TableCell::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if ( allParagraphsOffsets != NULL )
		{
			for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> tableCellItemParagraphsOffsets;
				vector<PapxInFkp> tableCellItemParagraphsProperties = iter->tableCellItem->GetAllParagraphsProperties( &tableCellItemParagraphsOffsets );

				for ( unsigned int i = 0; i < tableCellItemParagraphsProperties.size(); i++ )
				{
					allParagraphsProperties.push_back( tableCellItemParagraphsProperties[i] );
					allParagraphsOffsets->push_back( paragraphOffset + tableCellItemParagraphsOffsets[i] );
				}

				paragraphOffset += ( sizeof(WCHAR) * iter->tableCellItem->GetAllText().size() );
			}

			if ( !this->properties.empty() )
			{
				allParagraphsProperties.push_back( this->GetAllProperties() );
				allParagraphsOffsets->push_back( paragraphOffset );
			}
		}

		return allParagraphsProperties;
	}

	/*========================================================================================================*/

	vector<Chpx> TableCell::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if ( allRunsOffsets != NULL )
		{
			for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> tableCellRunsOffsets;
				vector<Chpx> tableCellItemRunsProperties = iter->tableCellItem->GetAllRunProperties( &tableCellRunsOffsets );

				for ( unsigned int i = 0; i < tableCellItemRunsProperties.size(); i++ )
				{
					allRunsProperties.push_back( tableCellItemRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + tableCellRunsOffsets[i] );
				}

				runOffset += ( sizeof(WCHAR) * iter->tableCellItem->GetAllText().size() );
			}
		}

		return allRunsProperties;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> TableCell::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemRunsOffsets;
				vector<IParagraphItemPtr> allTextItemRuns = iter->tableCellItem->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(WCHAR) * iter->tableCellItem->GetAllText().size() );
			}
		}

		return allRuns;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> TableCell::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int paragraphItemOffset = 0;

			for ( list<TableCellItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemParagraphItemsOffsets;
				vector<IParagraphItemPtr> allTextItemParagraphItems = iter->tableCellItem->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allTextItemParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( paragraphItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				paragraphItemOffset += ( sizeof(WCHAR) * iter->tableCellItem->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}
}