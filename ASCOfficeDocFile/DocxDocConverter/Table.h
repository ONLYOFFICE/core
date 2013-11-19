#pragma once

#include "TableRow.h"

namespace AVSDocFileFormat
{
  class Table: public ITextItem
  {
    struct TableRowWithOffset
    {
      TableRow tableRow;
      unsigned int tableRowOffset;

      TableRowWithOffset():
      tableRow(), tableRowOffset(0)
      {
      }
 
      TableRowWithOffset( const TableRow& _tableRow, unsigned int _tableRowOffset ):
      tableRow(_tableRow), tableRowOffset(_tableRowOffset)
      {
      }
    };
  
    private:
      list<TableRowWithOffset> rows;
	  unsigned int depth;
	  unsigned int tableRowsOffset;

    protected:
      void SetDepth( unsigned int _depth );
	  unsigned int GetDepth() const;

    public:
	  Table();
	  Table( const Table& table );
	  void AddRow( const TableRow& row );
	  virtual ~Table();
	  virtual wstring GetAllText() const;
	  virtual operator wstring() const;
	  virtual vector<TextItemPtr> GetAllParagraphsCopy() const;
	  virtual vector<ITextItem*> GetAllParagraphs();
	  virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
	  virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
  	  virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
	  virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
	  virtual IVirtualConstructor* New() const;
	  virtual IVirtualConstructor* Clone() const;
  };
}