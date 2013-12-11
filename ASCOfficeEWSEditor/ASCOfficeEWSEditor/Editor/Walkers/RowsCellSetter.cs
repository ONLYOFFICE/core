using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RowsCellSetter : IRowsSetter
    {
        public RowsCellSetter(Cells.CellRange range_coords, ICellSetter setter, Worksheet parent_sheet)
        {
            this.range_coords = range_coords;
            this.setter = setter;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

        }
        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            XmlNodeList cells = row_tag.SelectNodes("main:c", ns_manager);
            int last_updated_column = range_coords.ColumnFirst - 1;
            foreach (XmlElement cell_tag in cells)
            {
                int col = Helpers.ReferencesFormat.str2column(cell_tag.GetAttribute("r"));

                if (col >= last_updated_column + 1 && col <= range_coords.ColumnLast)
                {
                    for (int i = last_updated_column + 1; i < col; ++i)
                    {
                        XmlElement cell_ins = row_tag.OwnerDocument.CreateElement("c", ns_manager.LookupNamespace("main"));
                        cell_ins.SetAttribute("r", Helpers.ReferencesFormat.ref2str(row_num, true, i, true));
                        row_tag.InsertBefore(cell_ins, cell_tag);

                        if(!setter.Set(row_num, i, cell_ins))
                        {
                            return false;
                        }
                    }
                    if(!setter.Set(row_num, col, cell_tag))
                    {
                        return false;
                    }
                    last_updated_column = col;
                }
                else if (col > range_coords.ColumnLast)
                {
                    if (last_updated_column != range_coords.ColumnLast)
                    {
                        for (int i = last_updated_column + 1; i <= range_coords.ColumnLast; ++i)
                        {
                            XmlElement cell_ins = (XmlElement)row_tag.OwnerDocument.CreateElement("c", ns_manager.LookupNamespace("main"));
                            cell_ins.SetAttribute("r", Helpers.ReferencesFormat.ref2str(row_num, true, i, true));
                            row_tag.InsertBefore(cell_ins, cell_tag);

                            if(!setter.Set(row_num, i, cell_ins))
                            {
                                return false;
                            }
                        }
                        last_updated_column = range_coords.ColumnLast;
                    }
                    break;
                }
            }
            if (last_updated_column != range_coords.ColumnLast)
            {
                for (int i = last_updated_column + 1; i <= range_coords.ColumnLast; ++i)
                {
                    XmlElement cell_ins = (XmlElement)row_tag.OwnerDocument.CreateElement("c", ns_manager.LookupNamespace("main"));
                    cell_ins.SetAttribute("r", Helpers.ReferencesFormat.ref2str(row_num, true, i, true));
                    row_tag.InsertBefore(cell_ins, null);
                    if(!setter.Set(row_num, i, cell_ins))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

        public void PostProcessing()
        {
            setter.PostProcessing();
        }

        #endregion

        private Cells.CellRange range_coords;
        private XmlNamespaceManager ns_manager;
        private ICellSetter setter;
    }
}
