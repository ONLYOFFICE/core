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

#include "Endnote.h"

namespace ASCDocFileFormat
{
	Endnote::Endnote( short _aFtnIdx ) : endnoteItemsOffset(0), aFtnIdx(_aFtnIdx)
	{
	}

	Endnote::Endnote( const Endnote& _endnote ) : endnoteItemsOffset(_endnote.endnoteItemsOffset), aFtnIdx(_endnote.aFtnIdx)
	{
		for ( list<EndnoteItemWithOffset>::const_iterator iter = _endnote.textItems.begin(); iter != _endnote.textItems.end(); iter++ )
		{
			this->textItems.push_back( EndnoteItemWithOffset( TextItemPtr( static_cast<ITextItem*>( iter->endnoteItem->Clone() ) ), iter->endnoteItemOffset ) );
		}
	}

	void Endnote::AddTextItem( const ITextItem& _textItem )
	{
		ITextItem* textItem = static_cast<ITextItem*>( _textItem.Clone() );

		if ( textItem != NULL )
		{
			this->textItems.push_back( EndnoteItemWithOffset( TextItemPtr( textItem ), this->endnoteItemsOffset ) );
			this->endnoteItemsOffset += ( sizeof(wchar_t) * textItem->GetAllText().size() );
		}
	}

	short Endnote::GetIndex() const
	{
		return this->aFtnIdx;
	}

	Endnote::~Endnote()
	{
	}

	wstring Endnote::GetAllText() const
	{
		wstring allText;

		for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += iter->endnoteItem->GetAllText();
		}

		return allText;
	}

	Endnote::operator wstring() const
	{
		wstring allText;

		for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allText += *(iter->endnoteItem);
		}

		return allText;   
	}

	vector<TextItemPtr> Endnote::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			vector<TextItemPtr> textItemParagraphs = iter->endnoteItem->GetAllParagraphsCopy();

			for ( vector<TextItemPtr>::const_iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}

	vector<ITextItem*> Endnote::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for (std::list<EndnoteItemWithOffset>::iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			vector<ITextItem*> textItemParagraphs = iter->endnoteItem->GetAllParagraphs();

			for ( vector<ITextItem*>::iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( *textItemParagraphsIter );
			}
		}

		return allParagraphs;
	}

	vector<PapxInFkp> Endnote::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if ( allParagraphsOffsets != NULL )
		{
			for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> endnoteItemParagraphsOffsets;
				vector<PapxInFkp> endnoteItemParagraphsProperties = iter->endnoteItem->GetAllParagraphsProperties( &endnoteItemParagraphsOffsets );

				for ( unsigned int i = 0; i < endnoteItemParagraphsProperties.size(); i++ )
				{
					allParagraphsProperties.push_back( endnoteItemParagraphsProperties[i] );
					allParagraphsOffsets->push_back( paragraphOffset + endnoteItemParagraphsOffsets[i] );
				}

				paragraphOffset += ( sizeof(wchar_t) * iter->endnoteItem->GetAllText().size() );
			}
		}

		return allParagraphsProperties;
	}

	vector<Chpx> Endnote::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if ( allRunsOffsets != NULL )
		{
			for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> endnoteItemRunsOffsets;
				vector<Chpx> endnoteItemRunsProperties = iter->endnoteItem->GetAllRunProperties( &endnoteItemRunsOffsets );

				for ( unsigned int i = 0; i < endnoteItemRunsProperties.size(); i++ )
				{
					allRunsProperties.push_back( endnoteItemRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + endnoteItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->endnoteItem->GetAllText().size() );
			}
		}

		return allRunsProperties;
	}

	vector<IParagraphItemPtr> Endnote::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemRunsOffsets;
				vector<IParagraphItemPtr> allTextItemRuns = iter->endnoteItem->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->endnoteItem->GetAllText().size() );
			}
		}

		return allRuns;
	}

	vector<IParagraphItemPtr> Endnote::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int textItemOffset = 0;

			for ( list<EndnoteItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemParagraphItemsOffsets;
				vector<IParagraphItemPtr> allTextItemParagraphItems = iter->endnoteItem->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allTextItemParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( textItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				textItemOffset += ( sizeof(wchar_t) * iter->endnoteItem->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	IVirtualConstructor* Endnote::New() const
	{
		return new Endnote();
	}

	IVirtualConstructor* Endnote::Clone() const
	{
		return new Endnote(*this);
	}
}