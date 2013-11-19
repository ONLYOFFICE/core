using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class ExistentCellsCollector : IStatCollector
    {
        public ExistentCellsCollector(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            found_cells = new List<Cells.CellRange3D>();
            found_references = new List<string>();
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            string cell_reference = cell_tag.GetAttribute("r");
            found_cells.Add(new Cells.CellRange3D(new Cells.CellRange(cell_reference, parent_sheet.Workbook), parent_sheet));
            found_references.Add(cell_reference);
            CountRowCol(row, col);
            return true;
        }
        private void CountRowCol(int row, int col)
        {
            if (!row_min.HasValue || row < row_min.Value)
            {
                row_min = row;
            }
            if (!row_max.HasValue || row > row_max.Value)
            {
                row_max = row;
            }
            if (!col_min.HasValue || col < col_min.Value)
            {
                col_min = col;
            }
            if (!col_max.HasValue || col > col_max.Value)
            {
                col_max = col;
            }
        }

        internal Cells.CellRange GetRange()
        {
            if (row_min.HasValue && col_min.HasValue && row_max.HasValue && col_max.HasValue)
            {
                return new Cells.CellRange(row_min.Value, col_min.Value, row_max.Value, col_max.Value);
            }
            return Cells.CellRange.Empty;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return true; // skip
        }

        #endregion

        public List<Cells.CellRange3D> GetFoundCells()
        {
            return found_cells;
        }

        public List<string> GetFoundReferences()
        {
            return found_references;
        }
        public List<string> GetFoundBounds(out int r_min, out int r_max, out int c_min, out int c_max)
        {
            r_min = 0;
            r_max = 0;
            c_min = 0;
            c_max = 0;
            if (true == row_min.HasValue)
                r_min = row_min.Value;
            if (true == row_max.HasValue)
                r_max = row_max.Value;
            if (true == col_min.HasValue)
                c_min = col_min.Value;
            if (true == col_max.HasValue)
                c_max = col_max.Value;
            return found_references;
        }

        private List<Cells.CellRange3D> found_cells;
        private List<string> found_references;
        private Worksheet parent_sheet;

        int? row_min;
        int? row_max;
        int? col_min;
        int? col_max;
    }
}
