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
#pragma once

#include "TableCell.h"

namespace ASCDocFileFormat
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