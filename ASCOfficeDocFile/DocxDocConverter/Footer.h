#pragma once

#include "Paragraph.h"

namespace ASCDocFileFormat
{
	class Footer: public ITextItem
	{
		struct FooterItemWithOffset
		{
			TextItemPtr		footerItem;
			unsigned int	footerItemOffset;

			FooterItemWithOffset() : footerItem(), footerItemOffset(0)
			{

			}

			FooterItemWithOffset(const TextItemPtr& _footerItem, unsigned int _footerItemOffset) : footerItem(_footerItem), footerItemOffset(_footerItemOffset)
			{

			}
		};

	private:
		list<FooterItemWithOffset>	textItems;
		unsigned int				footerItemsOffset;

	public:
		explicit Footer();
		Footer( const Footer& _footer );
		void AddTextItem( const ITextItem& _textItem );
		virtual ~Footer();
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