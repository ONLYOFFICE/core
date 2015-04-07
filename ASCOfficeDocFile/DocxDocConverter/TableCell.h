#pragma once

#include "Paragraph.h"
#include "Picture.h"
#include "Footnote.h"
#include "Endnote.h"
#include "SectionBreak.h"
#include "CSSAOperand.h"
#include "TableBordersOperand.h"
#include "TableBordersOperand80.h"
#include "TDefTableOperand.h"
#include "TableCellWidthOperand.h"
#include "TableBrcOperand.h"
#include "TableBrc80Operand.h"
#include "TLP.h"

namespace ASCDocFileFormat
{
  class TableCell
  {
    friend class TableRow;

	struct TableCellItemWithOffset
    {
      TextItemPtr tableCellItem;
      unsigned int tableCellItemOffset;

      TableCellItemWithOffset():
      tableCellItem(), tableCellItemOffset(0)
      {
      }
 
      TableCellItemWithOffset( const TextItemPtr& _tableCellItem, unsigned int _tableCellItemOffset ):
      tableCellItem(_tableCellItem), tableCellItemOffset(_tableCellItemOffset)
      {
      }
    };
	
    private:
	  list<TableCellItemWithOffset> textItems;
	  list<Prl> properties;
      unsigned int depth;
	  unsigned int tableCellItemsOffset;

    protected:
      void SetDepth( unsigned int _depth );
	  unsigned int GetDepth() const;
  
    public:
	  TableCell();
	  explicit TableCell( unsigned int _depth );
	  ~TableCell();
	  TableCell( const TableCell& tableCell );
	  void AddTextItem( const ITextItem& _textItem );
	  wstring GetAllText() const;
	  operator wstring() const;
	  vector<TextItemPtr> GetAllParagraphsCopy() const;
	  vector<ITextItem*> GetAllParagraphs();
	  void AddProperty( short sprm, void* operand );
	  void AddProperties( const vector<Prl>& prls );
	  void RemoveProperty( short sprm, void* operand );
      void RemoveAllProperties();
      PapxInFkp GetAllProperties() const;
      unsigned int GetAllPropertiesCount() const;
	  vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
	  vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
	  vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
	  vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
  };
}