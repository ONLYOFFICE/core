#pragma once

#include "ITextItem.h"
#include "FootnoteRef.h"
#include "FootnoteReference.h"
#include "PLC.h"

namespace AVSDocFileFormat
{
	typedef PLC<EmptyPLCData> PlcffndTxt;
	typedef PLC<short> PlcffndRef;

	class Footnote : public ITextItem
	{
		struct FootnoteItemWithOffset
		{
			TextItemPtr footnoteItem;
			unsigned int footnoteItemOffset;

			FootnoteItemWithOffset():
			footnoteItem(), footnoteItemOffset(0)
			{
			}

			FootnoteItemWithOffset( const TextItemPtr& _footnoteItem, unsigned int _footnoteItemOffset ):
			footnoteItem(_footnoteItem), footnoteItemOffset(_footnoteItemOffset)
			{
			}
		};

	private:
		list<FootnoteItemWithOffset> textItems;
		unsigned int footnoteItemsOffset;
		short aFtnIdx;

	public:
		explicit Footnote( short _aFtnIdx = 0 );
		Footnote( const Footnote& _footnote );
		void AddTextItem( const ITextItem& _textItem );
		short GetIndex() const;
		virtual ~Footnote();
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