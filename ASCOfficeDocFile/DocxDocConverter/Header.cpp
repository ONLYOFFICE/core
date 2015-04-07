
#include "Header.h"

namespace ASCDocFileFormat
{
	Header::Header()
	{
		headerItemsOffset	=	0;

		Paragraph paragraph;
		paragraph.AddParagraphItem( Run() );
		this->textItems.push_back( HeaderItemWithOffset( TextItemPtr( static_cast<ITextItem*>(paragraph.Clone()) ), this->headerItemsOffset ) );
		this->headerItemsOffset += ( sizeof(wchar_t) * paragraph.GetAllText().size() );
	}

	/*========================================================================================================*/

	Header::Header( const Header& _header ):
	headerItemsOffset(_header.headerItemsOffset)
	{
		for ( list<HeaderItemWithOffset>::const_iterator iter = _header.textItems.begin(); iter != _header.textItems.end(); iter++ )
		{
			this->textItems.push_back( HeaderItemWithOffset( TextItemPtr( static_cast<ITextItem*>( iter->headerItem->Clone() ) ), iter->headerItemOffset ) );
		}
	}

	/*========================================================================================================*/

	void Header::AddTextItem( const ITextItem& _textItem )
	{
		ITextItem* textItem = static_cast<ITextItem*>( _textItem.Clone() );

		if ( !this->textItems.empty() )
		{
			list<HeaderItemWithOffset>::iterator iter = this->textItems.begin();

			for ( unsigned int i = 0; i < ( this->textItems.size() - 1 ); i++, iter++ );

			this->textItems.erase( iter );

			this->headerItemsOffset -= sizeof(wchar_t);
		}

		if ( textItem != NULL )
		{
			this->textItems.push_back( HeaderItemWithOffset( TextItemPtr( textItem ), this->headerItemsOffset ) );
			this->headerItemsOffset += ( sizeof(wchar_t) * textItem->GetAllText().size() );
			Paragraph paragraph;
			paragraph.AddParagraphItem( Run() );
			this->textItems.push_back( HeaderItemWithOffset( TextItemPtr( static_cast<ITextItem*>(paragraph.Clone()) ), this->headerItemsOffset ) );
			this->headerItemsOffset += ( sizeof(wchar_t) * paragraph.GetAllText().size() );
		}
	}

	/*========================================================================================================*/

	Header::~Header()
	{
	}

	/*========================================================================================================*/

	wstring Header::GetAllText() const
	{
		wstring allText;

		for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += iter->headerItem->GetAllText();
		}

		return allText;
	}

	/*========================================================================================================*/

	Header::operator wstring() const
	{
		wstring allText;

		for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += *(iter->headerItem);
		}

		return allText;   
	}

	/*========================================================================================================*/

	vector<TextItemPtr> Header::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			vector<TextItemPtr> textItemParagraphs = iter->headerItem->GetAllParagraphsCopy();

			for ( vector<TextItemPtr>::const_iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	vector<ITextItem*> Header::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			ITextItem* item = (ITextItem*)iter->headerItem.operator->();
			vector<ITextItem*> textItemParagraphs = item->GetAllParagraphs();

			for ( vector<ITextItem*>::iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( *textItemParagraphsIter );
			}
		}

		return allParagraphs;
	}

	/*========================================================================================================*/

	vector<PapxInFkp> Header::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if ( allParagraphsOffsets != NULL )
		{
			for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> headerItemParagraphsOffsets;
				vector<PapxInFkp> headerItemParagraphsProperties = iter->headerItem->GetAllParagraphsProperties( &headerItemParagraphsOffsets );

				for ( unsigned int i = 0; i < headerItemParagraphsProperties.size(); i++ )
				{
					allParagraphsProperties.push_back( headerItemParagraphsProperties[i] );
					allParagraphsOffsets->push_back( paragraphOffset + headerItemParagraphsOffsets[i] );
				}

				paragraphOffset += ( sizeof(wchar_t) * iter->headerItem->GetAllText().size() );
			}
		}

		return allParagraphsProperties;
	}

	/*========================================================================================================*/

	vector<Chpx> Header::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if ( allRunsOffsets != NULL )
		{
			for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> headerItemRunsOffsets;
				vector<Chpx> headerItemRunsProperties = iter->headerItem->GetAllRunProperties( &headerItemRunsOffsets );

				for ( unsigned int i = 0; i < headerItemRunsProperties.size(); i++ )
				{
					allRunsProperties.push_back( headerItemRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + headerItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->headerItem->GetAllText().size() );
			}
		}

		return allRunsProperties;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Header::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemRunsOffsets;
				vector<IParagraphItemPtr> allTextItemRuns = iter->headerItem->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->headerItem->GetAllText().size() );
			}
		}

		return allRuns;
	}

	/*========================================================================================================*/

	vector<IParagraphItemPtr> Header::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int textItemOffset = 0;

			for ( list<HeaderItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemParagraphItemsOffsets;
				vector<IParagraphItemPtr> allTextItemParagraphItems = iter->headerItem->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allTextItemParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( textItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				textItemOffset += ( sizeof(wchar_t) * iter->headerItem->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	/*========================================================================================================*/

	IVirtualConstructor* Header::New() const
	{
		return new Header();
	}

	/*========================================================================================================*/

	IVirtualConstructor* Header::Clone() const
	{
		return new Header( *this );
	}
}