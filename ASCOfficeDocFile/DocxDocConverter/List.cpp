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

#include "List.h"

namespace ASCDocFileFormat
{
	List::List(short _numID): numID(_numID), level(0) 
	{
	}

	List::List(const list<TextItemPtr>& _textItems, short _numID) : textItems(_textItems), numID(_numID), level(0)
	{
		SetLevel(level);
		SetNumID(numID);
	}

	List::List(const List& _list) : textItems(_list.textItems), numID(_list.numID), level(_list.level)
	{
	}


	void List::SetLevel(unsigned char _level)
	{
		for (std::list<TextItemPtr>::iterator iter = this->textItems.begin(); iter != this->textItems.end(); ++iter)
		{
			if (typeid(**iter) == typeid(Paragraph))
			{
				Paragraph* paragraph = dynamic_cast<Paragraph*>(iter->operator->());
				if (paragraph)
				{
					paragraph->RemoveProperty( (short)DocFileFormat::sprmPIlvl, (unsigned char*)(&(this->level)) );
					paragraph->AddProperty( (short)DocFileFormat::sprmPIlvl, (unsigned char*)(&_level) );
				}
			}
			else if (typeid(**iter) == typeid(List))
			{
				List* subList = dynamic_cast<List*>(iter->operator->());
				if (subList)
				{
					subList->SetLevel(_level + 1);
				}
			}
		}

		this->level = _level;
	}

	void List::SetNumID( short _numID )
	{
		for (std::list<TextItemPtr>::iterator iter = this->textItems.begin(); iter != textItems.end(); ++iter)
		{
			if (typeid(**iter) == typeid(Paragraph))
			{
				Paragraph* paragraph = dynamic_cast<Paragraph*>(iter->operator->());
				if (paragraph)
				{
					paragraph->RemoveProperty( (short)DocFileFormat::sprmPIlfo, (unsigned char*)(&this->numID) );
					paragraph->AddProperty( (short)DocFileFormat::sprmPIlfo, (unsigned char*)(&_numID) );
				}
			}
			else if (typeid(**iter) == typeid(List))
			{
				List* subList = dynamic_cast<List*>(iter->operator->());
				if (subList)
				{
					subList->SetNumID( _numID );
				}
			}
		}

		numID = _numID;
	}

	void List::AddParagraph( const Paragraph& _paragraph )
	{
		TextItemPtr newTextItem( static_cast<ITextItem*>( _paragraph.Clone() ) );

		this->textItems.push_back( newTextItem );
	}

	void List::AddTextItem( const ITextItem& _textItem )
	{
		TextItemPtr newTextItem( static_cast<ITextItem*>( _textItem.Clone() ) );

		this->textItems.push_back( newTextItem );

		this->SetLevel( this->level );
		this->SetNumID( this->numID );
	}

	wstring List::GetAllText() const
	{
		wstring allListText;

		for ( list<TextItemPtr>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			allListText += (*iter)->GetAllText();
		}

		return allListText;
	}

	List::operator wstring() const
	{
		wstring listText;

		for ( list<TextItemPtr>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			listText += (wstring)(**iter);
		}

		return listText;  
	}

	vector<TextItemPtr> List::GetAllParagraphsCopy() const
	{
		vector<TextItemPtr> allParagraphs;

		for ( list<TextItemPtr>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			if ( typeid(**iter) == typeid(Paragraph) )
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*iter)->Clone() ) ) );  
			}
			else
			{
				vector<TextItemPtr> textItemParagraphs = (*iter)->GetAllParagraphsCopy();

				for ( vector<TextItemPtr>::const_iterator textItemIter = textItemParagraphs.begin(); textItemIter != textItemParagraphs.end(); textItemIter++ )
				{
					allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemIter)->Clone() ) ) );  
				}
			}
		}

		return allParagraphs;
	}

	vector<ITextItem*> List::GetAllParagraphs()
	{
		std::vector<ITextItem*> allParagraphs;

		for (std::list<TextItemPtr>::iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			if (typeid(**iter) == typeid(Paragraph))
			{
				allParagraphs.push_back(iter->operator->());  
			}
			else
			{
				std::vector<ITextItem*> textItemParagraphs = (*iter)->GetAllParagraphs();

				for (std::vector<ITextItem*>::iterator textItemIter = textItemParagraphs.begin(); textItemIter != textItemParagraphs.end(); ++textItemIter)
				{
					allParagraphs.push_back( *textItemIter );  
				}
			}
		}

		return allParagraphs;
	}

	vector<IParagraphItemPtr> List::GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<TextItemPtr>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemRunsOffsets;
				vector<IParagraphItemPtr> allTextItemRuns = (*iter)->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * (*iter)->GetAllText().size() );
			}
		}

		return allRuns;
	}

	vector<IParagraphItemPtr> List::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		vector<IParagraphItemPtr> allParagraphItems;

		if ( allParagraphItemsOffsets != NULL )
		{
			unsigned int paragraphItemOffset = 0;

			for ( list<TextItemPtr>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				vector<unsigned int> allTextItemParagraphItemsOffsets;
				vector<IParagraphItemPtr> allTextItemParagraphItems = (*iter)->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for ( unsigned int i = 0; i < allTextItemParagraphItems.size(); i++ )
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( paragraphItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				paragraphItemOffset += ( sizeof(wchar_t) * (*iter)->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	vector<PapxInFkp> List::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if (allParagraphsOffsets)
		{
			std::vector<TextItemPtr> allParagraphs = GetAllParagraphsCopy();

			for (std::vector<TextItemPtr>::const_iterator iter = allParagraphs.begin(); iter != allParagraphs.end(); ++iter)
			{
				ITextItem* item = (ITextItem*)(iter->operator->());
				Paragraph* paragraph = dynamic_cast<Paragraph*>(item);
				if (paragraph)
				{
					allParagraphsProperties.push_back( paragraph->GetProperties());
					allParagraphsOffsets->push_back( paragraphOffset );
					paragraphOffset += (sizeof(wchar_t) * paragraph->GetAllText().size());
				}
			}
		}

		return allParagraphsProperties;
	}

	vector<Chpx> List::GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const
	{
		vector<Chpx> allRunProperties;
		unsigned int runOffset = 0;

		if (allRunsOffsets)
		{
			std::vector<TextItemPtr> allParagraphs = GetAllParagraphsCopy();

			for (std::vector<TextItemPtr>::const_iterator iter = allParagraphs.begin(); iter != allParagraphs.end(); ++iter)
			{
				std::vector<Chpx> paragraphRunProperties;
				std::vector<unsigned int> paragraphRunsOffsets;

				//

				ITextItem* item = (ITextItem*)(iter->operator->());
				Paragraph* paragraph = dynamic_cast<Paragraph*>(item);
				if (paragraph)
				{
					paragraphRunProperties = paragraph->GetAllRunProperties(&paragraphRunsOffsets);

					for (unsigned int i = 0; i < paragraphRunProperties.size(); ++i)
					{
						allRunProperties.push_back( paragraphRunProperties[i] );
						allRunsOffsets->push_back( runOffset + paragraphRunsOffsets[i] );
					}

					runOffset += (sizeof(wchar_t) * paragraph->GetAllText().size());
				}
			}
		}

		return allRunProperties;
	}

	List::~List() 
	{
	}

	IVirtualConstructor* List::New() const
	{
		return new List();
	}

	IVirtualConstructor* List::Clone() const
	{
		return new List( *this );
	}
}