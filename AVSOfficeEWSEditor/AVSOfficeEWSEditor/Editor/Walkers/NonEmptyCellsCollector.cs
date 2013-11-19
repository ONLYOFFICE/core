using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class NonEmptyCellsCollector : IStatCollector
    {
        public NonEmptyCellsCollector(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            found_cells = new List<Cells.CellRange3D>();
            found_references = new List<string>();
            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            XmlNode v_tag = cell_tag.SelectSingleNode("main:v",ns_manager);
            if(v_tag != null && v_tag.InnerText.Length != 0)
            {
                string cell_reference = cell_tag.GetAttribute("r");
                found_cells.Add(new Cells.CellRange3D(new Cells.CellRange(cell_reference, parent_sheet.Workbook), parent_sheet));
                found_references.Add(cell_reference);
                CountRowCol(row, col);
            }
            return true;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return true; // skip
        }

        #endregion

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

        public List<Cells.CellRange3D> GetFoundCells()
        {
            return found_cells;
        }
        
        public List<string> GetFoundReferences()
        {
            return found_references;
        }

        internal Cells.CellRange GetRange()
        {
            if (row_min.HasValue && col_min.HasValue && row_max.HasValue && col_max.HasValue)
            {
                return new Cells.CellRange(row_min.Value, col_min.Value, row_max.Value, col_max.Value);
            }
            return Cells.CellRange.Empty;
        }

        /// <summary>
        /// The difference from GetRange is that Get0Range returns range started in A1 and ended in max used cell
        /// </summary>
        /// <returns></returns>
        internal Cells.CellRange Get0Range()
        {
            if (row_max.HasValue && col_max.HasValue)
            {
                return new Cells.CellRange(1, 1, row_max.Value, col_max.Value);
            }
            return Cells.CellRange.Empty;
        }

        private List<Cells.CellRange3D> found_cells;
        private List<string> found_references;
        private XmlNamespaceManager ns_manager;
        private Worksheet parent_sheet;

        int? row_min;
        int? row_max;
        int? col_min;
        int? col_max;
    }
}
