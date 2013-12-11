using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Cells
{
    public struct Offset
    {
        public static Offset Null = new Offset(0, 0);
        internal Offset(int row_offset, int column_offset)
        {
            RowOffset = row_offset;
            ColumnOffset = column_offset;
        }

        public override string ToString()
        {
            return "{" + RowOffset + ", " + ColumnOffset + "}";
        }

        public int RowOffset;
        public int ColumnOffset;
    }
}
