#pragma once

#include "Paragraph.h"

namespace AVSDocFileFormat
{
	class Header: public ITextItem
	{
		struct HeaderItemWithOffset
		{
			TextItemPtr headerItem;
			unsigned int headerItemOffset;

			HeaderItemWithOffset():
			headerItem(), headerItemOffset(0)
			{
			}

			HeaderItemWithOffset( const TextItemPtr& _headerItem, unsigned int _headerItemOffset ):
			headerItem(_headerItem), headerItemOffset(_headerItemOffset)
			{
			}
		};

	private:
		list<HeaderItemWithOffset> textItems;
		unsigned int headerItemsOffset;

	public:
		explicit Header();
		Header( const Header& _header );
		void AddTextItem( const ITextItem& _textItem );
		virtual ~Header();
		virtual wstring GetAllText() const;
		virtual operator wstring() const;
		virtual vector<TextItemPtr> GetAllParagraphsCopy() const;
		virtual vector<ITextItem*> GetAllParagraphs();
		virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
		virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
		virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
		virtual IVirtualConstructor* New() const;
		virtual IVirtualConstructor* Clone() const;
	};
}