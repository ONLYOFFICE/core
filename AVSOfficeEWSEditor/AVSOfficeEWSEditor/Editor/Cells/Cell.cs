using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Cells
{
    public struct Cell : IComparable<Cell>
    {
        public Cell(string cell_ref)
        {
            bool row_rel;
            bool column_rel;
            Helpers.ReferencesFormat.str2loc(cell_ref, out row, out row_rel, out column, out column_rel);
        }

        /// <summary>
        /// Cell creation by row/column numbers
        /// </summary>
        /// <param name="row">1-based row number</param>
        /// <param name="column">1-based column number</param>
        public Cell(int row, int column) 
        {
            this.row = row;
            this.column = column;
        }

        public override int GetHashCode()
        {
            return ((row - 1) ^ ((column - 1) << 18)); // because (maxCol - 1) = 3fffh and (maxRow - 1)= fffffh
        }

        public override bool Equals(object obj)
        {
            if(!(obj is Cell))
            {
                return false;
            }
            Cell other_cell = (Cell)obj;
            return row == other_cell.row && column == other_cell.column;
        }

        #region IComparable<Cell> Members

        public int CompareTo(Cell other)
        {
            int cmp_row = row.CompareTo(other.row);
            if (cmp_row != 0)
            {
                return cmp_row;
            }
            else
            {
                return column.CompareTo(other.column);
            }
        }

        #endregion

        public static Offset operator-(Cell cell1, Cell cell2)
        {
            return new Offset(cell1.row - cell2.row, cell1.column - cell2.column);
        }

        public static Cell operator -(Cell cell, Offset offset)
        {
            return new Cell(cell.row - offset.RowOffset, cell.column - offset.ColumnOffset);
        }

        public static Cell operator+(Cell cell, Offset offset)
        {
            return new Cell(cell.row + offset.RowOffset, cell.column + offset.ColumnOffset);
        }

        public static bool operator ==(Cell one, Cell other)
        {
            return one.CompareTo(other) == 0;
        }

        public static bool operator !=(Cell one, Cell other)
        {
            return one.CompareTo(other) != 0;
        }

        public override string ToString()
        {
            return Helpers.ReferencesFormat.ref2str(row, true, column, true);
        }

        public int Row { get { return row; } }
        public int Column { get { return column; } }

        private readonly int row;
        private readonly int column;
    }
}
