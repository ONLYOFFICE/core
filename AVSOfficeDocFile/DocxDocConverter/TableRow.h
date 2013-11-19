#pragma once

#include "TableCell.h"

namespace AVSDocFileFormat
{
  class TableRow
  {
    friend class Table;

	struct TableCellWithOffset
    {
      TableCell tableCell;
      unsigned int tableCellOffset;

      TableCellWithOffset():
      tableCell(), tableCellOffset(0)
      {
      }
 
      TableCellWithOffset( const TableCell& _tableCell, unsigned int _tableCellOffset ):
      tableCell(_tableCell), tableCellOffset(_tableCellOffset)
      {
      }
    };

    private:
      list<TableCellWithOffset> cells;
	  list<Prl> properties;
	  unsigned int depth;
	  unsigned int tableCellsOffset;

    protected:
      void SetDepth( unsigned int _depth );
	  unsigned int GetDepth() const;
	
    public:
	  TableRow();
	  explicit TableRow( unsigned int _depth );
	  ~TableRow();
	  TableRow( const TableRow& tableRow );
	  void AddCell( const TableCell& cell );
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