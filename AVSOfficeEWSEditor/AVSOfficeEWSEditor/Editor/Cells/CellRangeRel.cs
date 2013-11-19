using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Cells
{
    internal class CellRangeRel
    {
        public CellRangeRel(string ref_str, Workbook parent_book)
        {
            if (!Helpers.ReferencesFormat.str2ref(ref_str.ToUpper(), out rowFirst, out rowFirstRelative, out columnFirst, out columnFirstRelative,
                                                                    out rowLast, out rowLastRelative, out columnLast, out columnLastRelative))
            {
                parent_book.ReportError(new Events.WrongReferenceFormatError(ref_str));
            }
        }

        private CellRangeRel(int rowFirst, bool rowFirstRelative, int columnFirst, bool columnFirstRelative,
                            int rowLast, bool rowLastRelative, int columnLast, bool columnLastRelative)
        {
            this.rowFirst = rowFirst;
            this.rowFirstRelative = rowFirstRelative;
            this.columnFirst = columnFirst;
            this.columnFirstRelative = columnFirstRelative;
            this.rowLast = rowLast;
            this.rowLastRelative = rowLastRelative;
            this.columnLast = columnLast;
            this.columnLastRelative = columnLastRelative;
        }

        public static CellRangeRel Empty = new CellRangeRel(-1, true, -1, true, -1, true, -1, true);

        override public string ToString()
        {
            return Helpers.ReferencesFormat.range2str(rowFirst, rowFirstRelative, columnFirst, columnFirstRelative,
                                                     rowLast, rowLastRelative, columnLast, columnLastRelative);
        }

        public Cell ToCell()
        {
            return new Cell(rowFirst, columnFirst);
        }

        public CellRange ToCellRange()
        {
            return new CellRange(rowFirst, columnFirst, rowLast, columnLast);
        }

        public static CellRangeRel operator+(CellRangeRel range, Offset offset)
        {
            CellRangeRel result =  new CellRangeRel(range.rowFirstRelative ? range.rowFirst + offset.RowOffset : range.rowFirst, range.rowFirstRelative,
                                                    range.columnFirstRelative ? range.columnFirst + offset.ColumnOffset : range.columnFirst, range.columnFirstRelative,
                                                    range.rowLastRelative ? range.rowLast + offset.RowOffset : range.rowLast, range.rowLastRelative,
                                                    range.columnLastRelative ? range.columnLast + offset.ColumnOffset : range.columnLast, range.columnLastRelative);
            if (result.rowFirst < 1 || result.columnFirst < 1 ||
                result.rowLast > Helpers.ReferencesFormat.maxRow || result.columnLast > Helpers.ReferencesFormat.maxCol)
            {
                return Empty;
            }
            return result;

        }

        /// <summary>
        /// Shifts the range independent on relativity
        /// </summary>
        /// <param name="offset"></param>
        public CellRangeRel Shift(Offset offset)
        {
            CellRange range = ToCellRange();
            if (offset.RowOffset != 0 && range.IsSetOfColumns || offset.ColumnOffset != 0 && range.IsSetOfRows)
            {
                return this;
            }
            range += offset;
            if (range == CellRange.Empty)
            {
                return Empty;
            }
            return new CellRangeRel(range.RowFirst, rowFirstRelative, range.ColumnFirst, columnFirstRelative,
                                    range.RowLast, rowLastRelative, range.ColumnLast, columnLastRelative);
        }

        public CellRangeRel Cut(CellRange range_sub)
        {
            CellRange range = ToCellRange().Cut(range_sub);
            if (range != CellRange.Empty)
            {
                return new CellRangeRel(range.RowFirst, rowFirstRelative, range.ColumnFirst, columnFirstRelative, 
                                        range.RowLast, rowLastRelative, range.ColumnLast, columnLastRelative);
            }
            return Empty;
        }

        public CellRangeRel Resize(Offset offset)
        {
            CellRange range = ToCellRange().Resize(offset);
            if (range != CellRange.Empty)
            {
                return new CellRangeRel(range.RowFirst, rowFirstRelative, range.ColumnFirst, columnFirstRelative,
                                        range.RowLast, rowLastRelative, range.ColumnLast, columnLastRelative);
            }
            return Empty;
        }

        public bool IsRelative { get { return rowFirstRelative || rowLastRelative || columnFirstRelative || columnLastRelative; } }

        // All numbers are 1-based
        private readonly int rowFirst;
        private readonly int rowLast;
        private readonly int columnFirst;
        private readonly int columnLast;
        private readonly bool rowFirstRelative;
        private readonly bool rowLastRelative;
        private readonly bool columnFirstRelative;
        private readonly bool columnLastRelative;
    }
}
