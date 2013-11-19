#pragma once

#include "ITextItem.h"
#include "EndnoteRef.h"
#include "EndnoteReference.h"
#include "PLC.h"

namespace AVSDocFileFormat
{
  typedef PLC<EmptyPLCData> PlcfendTxt;
  typedef PLC<short> PlcfendRef;
	
  class Endnote: public ITextItem
  {
    struct EndnoteItemWithOffset
    {
      TextItemPtr endnoteItem;
      unsigned int endnoteItemOffset;

      EndnoteItemWithOffset():
      endnoteItem(), endnoteItemOffset(0)
      {
      }
 
      EndnoteItemWithOffset( const TextItemPtr& _endnoteItem, unsigned int _endnoteItemOffset ):
      endnoteItem(_endnoteItem), endnoteItemOffset(_endnoteItemOffset)
      {
      }
    };

    private:
	  list<EndnoteItemWithOffset> textItems;
	  unsigned int endnoteItemsOffset;
	  short aFtnIdx;

    public:
	  explicit Endnote( short _aFtnIdx = 0 );
	  Endnote( const Endnote& _endnote );
      void AddTextItem( const ITextItem& _textItem );
	  short GetIndex() const;
	  virtual ~Endnote();
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