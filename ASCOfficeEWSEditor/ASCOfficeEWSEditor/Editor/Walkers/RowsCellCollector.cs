using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RowsCellCollector : IRowsCollector
    {
        public RowsCellCollector(Cells.CellRange range_coords, Worksheet parent_sheet, IStatCollector collector)
        {
            this.range_coords = range_coords;
            this.parent_sheet = parent_sheet;
            this.collector = collector;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }
        #region IRowsCollector Members

        public bool CollectIncludingEmpty(XmlElement row_tag, int row_num)
        {
            if(row_tag == null)
            {
                for (int col = 0; col < range_coords.ColumnLast - range_coords.ColumnFirst + 1; ++col)
                {
                    if (!collector.Collect(row_num, col + range_coords.ColumnFirst, parent_sheet))
                    {
                        return false;
                    }
                }
                return true;
            }
            XmlNodeList cells = row_tag.SelectNodes("main:c", ns_manager);
            int last_updated_column = range_coords.ColumnFirst - 1;
            foreach (XmlElement cell_tag in cells)
            {
                int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));

                // If inside the range
                if (col >= last_updated_column + 1 && col <= range_coords.ColumnLast)
                {
                    for (int i = last_updated_column + 1; i < col; ++i)
                    {
                        if (!collector.Collect(row_num, i, parent_sheet))
                        {
                            return false;
                        }
                    }
                    if (!collector.Collect(row_num, col, cell_tag))
                    {
                        return false;
                    }
                    last_updated_column = col;
                }
            }
            if (last_updated_column != range_coords.ColumnLast)
            {
                for (int i = last_updated_column + 1; i <= range_coords.ColumnLast; ++i)
                {
                    if (!collector.Collect(row_num, i, parent_sheet))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        #endregion

        private Cells.CellRange range_coords;

        private XmlNamespaceManager ns_manager;
        private IStatCollector collector;
        private Worksheet parent_sheet;
    }
}
