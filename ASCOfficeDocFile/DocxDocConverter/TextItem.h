#pragma once

#include "ITextItem.h"
#include "ParagraphItem.h"

namespace AVSDocFileFormat
{
	class TextItem : public Aggregat<ITextItem, TextItem>
	{
	public:
		TextItem () : Aggregat()
		{
		}

		explicit TextItem (const ITextItem& oItem) : Aggregat(static_cast<ITextItem*>(oItem.Clone()))
		{
		}

		TextItem (const TextItem& oItem)
		{
			if (oItem.m_item.operator->())
			{
				m_item.reset(static_cast<ITextItem*>(oItem.m_item->Clone()));  
			}
		}

		TextItem& operator = (const TextItem& oItem)
		{
			if (m_item.operator->() != oItem.m_item.operator->())
			{
				m_item.reset(static_cast<ITextItem*>(oItem.m_item->Clone()));
			}

			return *this;
		}

		TextItem& operator = (const ITextItem& oItem)
		{
			if (m_item.operator->() != &oItem)
			{
				m_item.reset(static_cast<ITextItem*>(oItem.Clone()));
			}

			return *this;
		}

		template<class T> vector<ParagraphItem> GetAllRunItemsByType() const
		{
			std::vector<ParagraphItem> allParagraphItems;

			std::vector<unsigned int> paragraphsItemsOffsets;
			std::vector<IParagraphItemPtr> paragraphsItems = m_item->GetAllRunsCopy(&paragraphsItemsOffsets);

			for (size_t i = 0; i < paragraphsItems.size(); ++i)
			{
				Run* run = dynamic_cast<Run*>(paragraphsItems[i].operator->());
				if (run)
				{
					for (std::list<RunItem>::const_iterator iter = run->begin(); iter != run->end(); ++iter)
					{
						if (iter->is<T>())
						{
							allParagraphItems.push_back(ParagraphItem(*run, paragraphsItemsOffsets[i]));
						}
					}
				}
			}

			return allParagraphItems;
		}

		template<class T> vector<ParagraphItem> GetAllParagraphItemsByType() const
		{
			std::vector<ParagraphItem> allParagraphItems;

			std::vector<unsigned int> paragraphsItemsOffsets;
			std::vector<IParagraphItemPtr> paragraphsItems = m_item->GetAllParagraphItemsCopy(&paragraphsItemsOffsets);

			for (size_t i = 0; i < paragraphsItems.size(); ++i)
			{
				T* paragraphItem = dynamic_cast<T*>(paragraphsItems[i].operator->());
				if (paragraphItem)
				{
					allParagraphItems.push_back( ParagraphItem( *paragraphItem, paragraphsItemsOffsets[i] ) );  
				}
			}

			return allParagraphItems;  
		}
	};
}