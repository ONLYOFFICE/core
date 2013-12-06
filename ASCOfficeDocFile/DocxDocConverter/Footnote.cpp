#include "stdafx.h"
#include "Footnote.h"

namespace ASCDocFileFormat
{
	Footnote::Footnote( short _aFtnIdx ) : footnoteItemsOffset(0), aFtnIdx(_aFtnIdx)
	{
	}

	Footnote::Footnote( const Footnote& _footnote ) : footnoteItemsOffset(_footnote.footnoteItemsOffset), aFtnIdx(_footnote.aFtnIdx)
	{
		for ( list<FootnoteItemWithOffset>::const_iterator iter = _footnote.textItems.begin(); iter != _footnote.textItems.end(); iter++ )
		{
			this->textItems.push_back( FootnoteItemWithOffset( TextItemPtr( static_cast<ITextItem*>( iter->footnoteItem->Clone() ) ), iter->footnoteItemOffset ) );
		}
	}

	void Footnote::AddTextItem( const ITextItem& _textItem )
	{
		ITextItem* textItem = static_cast<ITextItem*>( _textItem.Clone() );

		if ( textItem != NULL )
		{
			this->textItems.push_back( FootnoteItemWithOffset( TextItemPtr( textItem ), this->footnoteItemsOffset ) );
			this->footnoteItemsOffset += ( sizeof(WCHAR) * textItem->GetAllText().size() );
		}
	}

	short Footnote::GetIndex() const
	{
		return this->aFtnIdx;
	}

	Footnote::~Footnote()
	{
	}

	wstring Footnote::GetAllText() const
	{
		wstring allText;

		for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += iter->footnoteItem->GetAllText();
		}

		return allText;
	}

	Footnote::operator wstring() const
	{
		wstring allText;

		for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += *(iter->footnoteItem);
		}

		return allText;   
	}

	vector<TextItemPtr> Footnote::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			vector<TextItemPtr> textItemParagraphs = iter->footnoteItem->GetAllParagraphsCopy();

			for ( vector<TextItemPtr>::const_iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}

	vector<ITextItem*> Footnote::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			ITextItem* item = (ITextItem*)iter->footnoteItem.operator->();

			vector<ITextItem*> textItemParagraphs = item->GetAllParagraphs();

			for ( vector<ITextItem*>::iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( *textItemParagraphsIter );
			}
		}

		return allParagraphs;
	}

	vector<PapxInFkp> Footnote::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if ( allParagraphsOffsets != NULL )
		{
			for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> footnoteItemParagraphsOffsets;
				vector<PapxInFkp> footnoteItemParagraphsProperties = iter->footnoteItem->GetAllParagraphsProperties( &footnoteItemParagraphsOffsets );

				for ( unsigned int i = 0; i < footnoteItemParagraphsProperties.size(); i++ )
				{
					allParagraphsProperties.push_back( footnoteItemParagraphsProperties[i] );
					allParagraphsOffsets->push_back( paragraphOffset + footnoteItemParagraphsOffsets[i] );
				}

				paragraphOffset += ( sizeof(WCHAR) * iter->footnoteItem->GetAllText().size() );
			}
		}

		return allParagraphsProperties;
	}

	vector<Chpx> Footnote::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if ( allRunsOffsets != NULL )
		{
			for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> footnoteItemRunsOffsets;
				vector<Chpx> footnoteItemRunsProperties = iter->footnoteItem->GetAllRunProperties( &footnoteItemRunsOffsets );

				for ( unsigned int i = 0; i < footnoteItemRunsProperties.size(); i++ )
				{
					allRunsProperties.push_back( footnoteItemRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + footnoteItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(WCHAR) * iter->footnoteItem->GetAllText().size() );
			}
		}

		return allRunsProperties;
	}

	vector<IParagraphItemPtr> Footnote::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemRunsOffsets;
				vector<IParagraphItemPtr> allTextItemRuns = iter->footnoteItem->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(WCHAR) * iter->footnoteItem->GetAllText().size() );
			}
		}

		return allRuns;
	}

	vector<IParagraphItemPtr> Footnote::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int textItemOffset = 0;

			for ( list<FootnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemParagraphItemsOffsets;
				vector<IParagraphItemPtr> allTextItemParagraphItems = iter->footnoteItem->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allTextItemParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( textItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				textItemOffset += ( sizeof(WCHAR) * iter->footnoteItem->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	IVirtualConstructor* Footnote::New() const
	{
		return new Footnote();
	}

	IVirtualConstructor* Footnote::Clone() const
	{
		return new Footnote( *this );
	}
}