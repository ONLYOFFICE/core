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

#include "TextBox.h"

namespace ASCDocFileFormat
{
	TextBox::TextBox (short _aFtnIdx) : textBoxItemsOffset(0), aFtnIdx(_aFtnIdx)
	{

	}

	TextBox::TextBox (const TextBox& oTextBox) : textBoxItemsOffset(oTextBox.textBoxItemsOffset), aFtnIdx(oTextBox.aFtnIdx)
	{
		for (std::list<TextBoxItemWithOffset>::const_iterator iter = oTextBox.textItems.begin(); iter != oTextBox.textItems.end(); ++iter)
		{
			textItems.push_back( TextBoxItemWithOffset( TextItemPtr( static_cast<ITextItem*>(iter->textBoxItem->Clone() ) ), iter->textBoxItemOffset ) );
		}
	}
}

namespace ASCDocFileFormat
{
	void TextBox::AddTextItem(const ITextItem& oTextItem)
	{
		ITextItem* pTextItem = static_cast<ITextItem*>(oTextItem.Clone());

		if (pTextItem)
		{
			textItems.push_back(TextBoxItemWithOffset(TextItemPtr(pTextItem), textBoxItemsOffset));
			textBoxItemsOffset	+= (sizeof(wchar_t) * pTextItem->GetAllText().size());
		}
	}

	short TextBox::GetIndex() const
	{
		return aFtnIdx;
	}

	TextBox::~TextBox()
	{

	}

	std::wstring TextBox::GetAllText() const
	{
		std::wstring allText;

		for (std::list<TextBoxItemWithOffset>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			allText += iter->textBoxItem->GetAllText();
		}

		return allText;
	}


	TextBox::operator wstring() const
	{
		std::wstring allText;

		for (std::list<TextBoxItemWithOffset>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
		{
			allText += *(iter->textBoxItem);
		}

		return allText;   
	}

	std::vector<TextItemPtr> TextBox::GetAllParagraphsCopy() const
	{
		std::vector<TextItemPtr> allParagraphs;

		for (std::list<TextBoxItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			std::vector<TextItemPtr> textItemParagraphs = iter->textBoxItem->GetAllParagraphsCopy();

			for (std::vector<TextItemPtr>::const_iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); ++textItemParagraphsIter)
			{
				allParagraphs.push_back( TextItemPtr( static_cast<ITextItem*>( (*textItemParagraphsIter)->Clone() ) ) );
			}
		}

		return allParagraphs;
	}


	vector<ITextItem*> TextBox::GetAllParagraphs()
	{
		vector<ITextItem*> allParagraphs;

		for ( list<TextBoxItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
		{
			ITextItem* item = (ITextItem*)iter->textBoxItem.operator->();
			vector<ITextItem*> textItemParagraphs = item->GetAllParagraphs();

			for ( vector<ITextItem*>::iterator textItemParagraphsIter = textItemParagraphs.begin(); textItemParagraphsIter != textItemParagraphs.end(); textItemParagraphsIter++ )
			{
				allParagraphs.push_back( *textItemParagraphsIter );
			}
		}

		return allParagraphs;
	}

	std::vector<PapxInFkp> TextBox::GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const
	{
		std::vector<PapxInFkp> allParagraphsProperties;
		unsigned int paragraphOffset = 0;

		if (allParagraphsOffsets)
		{
			for ( list<TextBoxItemWithOffset>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			{
				std::vector<unsigned int> footnoteItemParagraphsOffsets;
				std::vector<PapxInFkp> itemParagraphsProperties = iter->textBoxItem->GetAllParagraphsProperties( &footnoteItemParagraphsOffsets );

				for (unsigned int i = 0; i < itemParagraphsProperties.size(); ++i)
				{
					allParagraphsProperties.push_back(itemParagraphsProperties[i]);
					allParagraphsOffsets->push_back(paragraphOffset + footnoteItemParagraphsOffsets[i]);
				}

				paragraphOffset += ( sizeof(wchar_t) * iter->textBoxItem->GetAllText().size() );
			}
		}

		return allParagraphsProperties;
	}

	std::vector<Chpx> TextBox::GetAllRunProperties(vector<unsigned int>* allRunsOffsets) const
	{
		std::vector<Chpx> allRunsProperties;
		unsigned int runOffset = 0;

		if (allRunsOffsets)
		{
			for (std::list<TextBoxItemWithOffset>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			{
				std::vector<unsigned int> footnoteItemRunsOffsets;
				std::vector<Chpx> footnoteItemRunsProperties = iter->textBoxItem->GetAllRunProperties(&footnoteItemRunsOffsets);

				for (unsigned int i = 0; i < footnoteItemRunsProperties.size(); ++i)
				{
					allRunsProperties.push_back( footnoteItemRunsProperties[i] );
					allRunsOffsets->push_back( runOffset + footnoteItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->textBoxItem->GetAllText().size() );
			}
		}

		return allRunsProperties;
	}


	std::vector<IParagraphItemPtr> TextBox::GetAllRunsCopy(vector<unsigned int>* allRunsOffsets) const
	{
		std::vector<IParagraphItemPtr> allRuns;

		if ( allRunsOffsets != NULL )
		{
			unsigned int runOffset = 0;

			for ( list<TextBoxItemWithOffset>::const_iterator iter = this->textItems.begin(); iter != this->textItems.end(); iter++ )
			{
				std::vector<unsigned int> allTextItemRunsOffsets;
				std::vector<IParagraphItemPtr> allTextItemRuns = iter->textBoxItem->GetAllRunsCopy( &allTextItemRunsOffsets );

				for ( unsigned int i = 0; i < allTextItemRuns.size(); i++ )
				{
					allRuns.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemRuns[i]->Clone()) ) );
					allRunsOffsets->push_back( runOffset + allTextItemRunsOffsets[i] );
				}

				runOffset += ( sizeof(wchar_t) * iter->textBoxItem->GetAllText().size() );
			}
		}

		return allRuns;
	}


	vector<IParagraphItemPtr> TextBox::GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const
	{
		std::vector<IParagraphItemPtr> allParagraphItems;

		if (allParagraphItemsOffsets)
		{
			unsigned int textItemOffset = 0;

			for (std::list<TextBoxItemWithOffset>::const_iterator iter = textItems.begin(); iter != textItems.end(); ++iter)
			{
				std::vector<unsigned int> allTextItemParagraphItemsOffsets;
				std::vector<IParagraphItemPtr> allTextItemParagraphItems = iter->textBoxItem->GetAllParagraphItemsCopy( &allTextItemParagraphItemsOffsets );

				for (unsigned int i = 0; i < allTextItemParagraphItems.size(); ++i)
				{
					allParagraphItems.push_back( IParagraphItemPtr( static_cast<IParagraphItem*>(allTextItemParagraphItems[i]->Clone()) ) );
					allParagraphItemsOffsets->push_back( textItemOffset + allTextItemParagraphItemsOffsets[i] );
				}

				textItemOffset += ( sizeof(wchar_t) * iter->textBoxItem->GetAllText().size() );
			}
		}

		return allParagraphItems;
	}

	IVirtualConstructor* TextBox::New() const
	{
		return new TextBox();
	}

	IVirtualConstructor* TextBox::Clone() const
	{
		return new TextBox( *this );
	}
}