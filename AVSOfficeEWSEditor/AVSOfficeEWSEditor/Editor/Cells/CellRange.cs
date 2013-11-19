using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Cells
{
    public struct CellRange : IEnumerable<Cell>, IComparable<CellRange>
    {
        /// <summary>
        /// Create cell range by reference
        /// </summary>
        /// <param name="cell_range_ref">reference</param>
        /// <param name="workbook">this is only necessary for errors reporting</param>
        public CellRange(string cell_range_ref, Workbook workbook)
        {
            bool row1_rel;
            bool column1_rel;
            bool row2_rel;
            bool column2_rel;
            if (!Helpers.ReferencesFormat.str2ref(cell_range_ref.ToUpper(), out rowFirst, out row1_rel, out columnFirst, out column1_rel,
                                                                    out rowLast, out row2_rel, out columnLast, out column2_rel))
            {
                workbook.ReportError(new Events.WrongReferenceFormatError(cell_range_ref));
                this = Empty;
            }
        }

        public CellRange(int rowFirst, int columnFirst, int rowLast, int columnLast) 
        {
            this.rowFirst = rowFirst;
            this.columnFirst = columnFirst;
            this.rowLast = rowLast;
            this.columnLast = columnLast;
        }

        public CellRange(Cell cell)
            : this(cell.Row, cell.Column, cell.Row, cell.Column)
        {
        }

        public static CellRange Empty = new CellRange(-1, -1, -1, -1);

        /// <summary>
        ///   Returns number of cells in the range. Read-only.
        /// </summary>
        public int NumCells
        {
            get { return (rowLast - rowFirst + 1) * (columnLast - columnFirst + 1); }
        }

        public int CellIndex(Cell cell)
        {
            return (cell.Row - rowFirst + 1) * (cell.Column - columnFirst + 1) - 1;
        }

        public bool IsSingleCell
        {
            get { return rowFirst == rowLast && columnFirst == columnLast; }
        }

        public Walkers.RangeWalker GetWalker(Worksheet ws)
        {
            return new Walkers.RangeWalker(this, ws);
        }

        public bool Contains(Cell cell)
        {
            return rowFirst <= cell.Row && rowLast >= cell.Row &&
                columnFirst <= cell.Column && columnLast >= cell.Column;
        }

        /// <summary>
        /// If the range fully covers the specified one
        /// </summary>
        /// <param name="another_range"></param>
        /// <returns></returns>
        public bool Covers(CellRange another_range)
        {
            return (another_range.rowFirst >= rowFirst && another_range.rowLast <= rowLast &&
                    another_range.columnFirst >= columnFirst && another_range.columnLast <= columnLast);
        }

        public bool IsIntersectedWith(CellRange another_range)
        {
            return !(another_range.rowLast < rowFirst && another_range.rowFirst < rowFirst ||
                    another_range.rowLast > rowLast && another_range.rowFirst > rowLast ||
                    another_range.columnFirst < columnFirst && another_range.columnLast < columnFirst ||
                    another_range.columnFirst > columnLast && another_range.columnLast > columnLast);
        }

        public bool IsBoundaryIntersectedWith(CellRange another_range)
        {
            bool top_left_inside = another_range.Contains(TopLeft);
            bool top_right_inside = another_range.Contains(TopRight);
            bool bottom_left_inside = another_range.Contains(BottomLeft);
            bool bottom_right_inside = another_range.Contains(BottomRight);

            return (top_left_inside || top_right_inside || bottom_left_inside || bottom_right_inside) &&
                    (!top_left_inside || !top_right_inside || !bottom_left_inside || !bottom_right_inside);
        }

        public CellRange BoundingBox(CellRange another_range)
        {
            return new CellRange(Math.Min(rowFirst, another_range.rowFirst), Math.Min(columnFirst, another_range.columnFirst),
                                    Math.Max(rowLast, another_range.rowLast), Math.Max(columnLast, another_range.columnLast));
        }

        public CellRange Intersect(CellRange another_range)
        {
            if (!IsIntersectedWith(another_range))
            {
                return Empty;
            }

            return new CellRange(Math.Max(rowFirst, another_range.rowFirst), Math.Max(columnFirst, another_range.columnFirst),
                                    Math.Min(rowLast, another_range.rowLast), Math.Min(columnLast, another_range.columnLast));
        }


        public static bool operator ==(CellRange one, CellRange other)
        {
            return one.CompareTo(other) == 0;
        }

        public static bool operator !=(CellRange one, CellRange other)
        {
            return one.CompareTo(other) != 0;
        }

        public override int GetHashCode()
        {
            int hash_top_left = TopLeft.GetHashCode();
            return ((hash_top_left << 16) | (hash_top_left >> 16)) ^ BottomRight.GetHashCode();
        }

        public override bool Equals(object obj)
        {
            if (!(obj is CellRange))
            {
                return false;
            }
            CellRange other_cell = (CellRange)obj;
            return rowFirst == other_cell.rowFirst && columnFirst == other_cell.columnFirst &&
                   rowLast == other_cell.rowLast && columnLast == other_cell.columnLast;
        }

        public override string ToString()
        {
            return Helpers.ReferencesFormat.range2str(rowFirst, true, columnFirst, true, rowLast, true, columnLast, true);
        }

        public Cell TopLeft { get { return new Cell(rowFirst, columnFirst); } }
        public Cell TopRight { get { return new Cell(rowFirst, columnLast); } }
        public Cell BottomLeft { get { return new Cell(rowLast, columnFirst); } }
        public Cell BottomRight { get { return new Cell(rowLast, columnLast); } }

        public static CellRange operator +(CellRange range, Offset offset)
        {
            CellRange result = new CellRange(range.rowFirst + offset.RowOffset, range.columnFirst + offset.ColumnOffset, range.rowLast + offset.RowOffset, range.columnLast + offset.ColumnOffset);
            if (result.RowFirst < 1 || result.ColumnFirst < 1 ||
                result.RowLast > Helpers.ReferencesFormat.maxRow || result.ColumnLast > Helpers.ReferencesFormat.maxCol)
            {
                return Empty;
            }
            return result;
        }

        public static CellRange operator -(CellRange range, Offset offset)
        {
            CellRange result = new CellRange(range.rowFirst - offset.RowOffset, range.columnFirst - offset.ColumnOffset, range.rowLast - offset.RowOffset, range.columnLast - offset.ColumnOffset);
            if (result.RowFirst < 1 || result.ColumnFirst < 1 ||
                result.RowLast > Helpers.ReferencesFormat.maxRow || result.ColumnLast > Helpers.ReferencesFormat.maxCol)
            {
                return Empty;
            }
            return result;
        }

        /// <summary>
        /// Note: We assume here that the range_sub is always a set of columns or set of rows and never the general type range
        /// </summary>
        /// <param name="range_sub"></param>
        /// <returns></returns>
        public CellRange Cut(CellRange range_sub)
        {
            // Rule #1
            if (range_sub.Covers(this))
            {
                return Empty;
            }
            // Rule #2
            if (!IsIntersectedWith(range_sub))
            {
                return this;
            }
            bool top_left_inside = range_sub.Contains(TopLeft);
            bool top_right_inside = range_sub.Contains(TopRight);
            bool bottom_left_inside = range_sub.Contains(BottomLeft);
            bool bottom_right_inside = range_sub.Contains(BottomRight);

            // Rule #3 (When the range_sub is vertically inside or horizontally inside. Like a cross figure in geometry.)
            if (!top_left_inside && !bottom_right_inside && !top_right_inside && !bottom_left_inside) // Based on rules #1 and #2
            {
                Cells.Offset offset = new Cells.Offset(range_sub.IsSetOfColumns ? 0 : -range_sub.Height, range_sub.IsSetOfRows ? 0 : -range_sub.Width);
                return Resize(offset);
            }

            // Rule #4 (When the top shall be cut)
            if (top_left_inside && top_right_inside)
            {
                return new CellRange(range_sub.RowLast + 1, ColumnFirst, RowLast, ColumnLast);
            }

            // Rule #5 (When the bottom shall be cut)
            if (bottom_left_inside && bottom_right_inside)
            {
                return new CellRange(RowFirst, ColumnFirst, range_sub.RowFirst - 1, ColumnLast);
            }

            // Rule #6 (When the left shall be cut)
            if (top_left_inside && bottom_left_inside)
            {
                return new CellRange(RowFirst, range_sub.ColumnLast + 1, RowLast, ColumnLast);
            }

            // Rule #7 (When the right shall be cut)
            if (top_right_inside && bottom_right_inside)
            {
                return new CellRange(RowFirst, ColumnFirst, RowLast, range_sub.ColumnFirst - 1);
            }

            return this;
        }

        public CellRange Resize(Offset offset)
        {
            // Vertically
            if (offset.RowOffset != 0) // Assumed that resized shall be performed vertically
            {
                if (IsSetOfColumns) // If the range is a set of columns, leave it the same size
                {
                    return this;
                }
                return new CellRange(RowFirst - offset.RowOffset, ColumnFirst, RowLast, ColumnLast); // Will be moved down later in ChangeObserverOnRangeRemoved
            }
            // Horizontally
            if (offset.ColumnOffset != 0) // Assumed that resized shall be performed horizontally
            {
                if (IsSetOfRows) // If the range is a set of rows, leave it the same size
                {
                    return this;
                }
                return new CellRange(RowFirst, ColumnFirst - offset.ColumnOffset, RowLast, ColumnLast); // Will be moved right later in ChangeObserverOnRangeRemoved
            }

            return this;
        }
        
        #region IEnumerable<Cell> Members

        public IEnumerator<Cell> GetEnumerator()
        {
            return new Enumerator(this);
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return new Enumerator(this);
        }

        #endregion

        internal class Enumerator : IEnumerator<Cell>
        {
            public Enumerator(CellRange range)
            {
                this.range = range;
                current_row = range.RowFirst - 1;
                current_column = range.ColumnLast;
            }

            #region IEnumerator<Cell> Members

            public Cell Current
            {
                get { return new Cell(current_row, current_column); }
            }

            #endregion

            #region IDisposable Members

            public void Dispose()
            {
            }

            #endregion

            #region IEnumerator Members

            object System.Collections.IEnumerator.Current
            {
                get { return new Cell(current_row, current_column); }
            }

            public bool MoveNext()
            {
                if (range == CellRange.Empty)
                {
                    return false;
                }
                if (current_column + 1 > range.ColumnLast)
                {
                    if (current_row + 1 > range.RowLast)
                    {
                        return false;
                    }
                    ++current_row;
                    current_column = range.ColumnFirst;
                }
                else
                {
                    ++current_column;
                }
                return true;
            }

            public void Reset()
            {
                throw new NotSupportedException();
            }

            #endregion

            private Cells.CellRange range;
            private int current_row;
            private int current_column;
        }

        #region IComparable<CellRange> Members

        public int CompareTo(CellRange other)
        {
            int cmp_cell_result = TopLeft.CompareTo(other.TopLeft);
            if (cmp_cell_result != 0)
            {
                return cmp_cell_result;
            }
            else
            {
                return BottomRight.CompareTo(other.BottomRight);
            }
        }

        #endregion
        
        public bool IsSetOfRows { get { return ColumnFirst == 1 && ColumnLast == Helpers.ReferencesFormat.maxCol; } }
        public bool IsSetOfColumns { get { return RowFirst == 1 && RowLast == Helpers.ReferencesFormat.maxRow; } }
        public int Width { get { return ColumnLast - ColumnFirst + 1; } }
        public int Height { get { return RowLast - RowFirst + 1; } }

        public int RowFirst { get { return rowFirst; } }
        public int ColumnFirst { get { return columnFirst; } }
        public int RowLast { get { return rowLast; } }
        public int ColumnLast { get { return columnLast; } }

        private readonly int rowFirst;
        private readonly int columnFirst;
        private readonly int rowLast;
        private readonly int columnLast;
    }
}
