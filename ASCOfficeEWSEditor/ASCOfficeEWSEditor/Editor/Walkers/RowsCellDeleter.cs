using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class RowsCellDeleter : IRowsSetter
    {
        public RowsCellDeleter(int columnFirst, int columnLast, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            this.columnFirst = columnFirst;
            this.columnLast = columnLast;
            delete_whole_row = columnFirst == 1 && columnLast == Helpers.ReferencesFormat.maxCol;
            cells_unregistrer = new CellsUnregistrer(parent_sheet);
        }

        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            bool at_least_one_nondeleted_cell = false;
            foreach (XmlElement cell_tag in row_tag.SelectNodes("main:c", ns_manager))
            {
                Cells.Cell cell = new Cells.Cell(cell_tag.GetAttribute("r"));
                if (delete_whole_row || cell.Column >= columnFirst && cell.Column <= columnLast)
                {
                    // Unregister formula and shared strings
                    cells_unregistrer.Set(cell.Row, cell.Column, cell_tag);

                    // Mark Updated
                    Calculation.CellItem cell_subj = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
                    cell_subj.CellOrFormulaUpdated = true;

                    if (!delete_whole_row) // If the whole row is deleted then it is not necessary to delete the contained cells one by one
                    {
                        row_tag.RemoveChild(cell_tag);
                    }
                }
                else
                {
                    at_least_one_nondeleted_cell = true;
                }
            }
            if (!at_least_one_nondeleted_cell)
            {
                row_tag.ParentNode.RemoveChild(row_tag);
            }
            return true;
        }

        public void PostProcessing()
        {
            cells_unregistrer.PostProcessing();
        }

        #endregion


        private CellsUnregistrer cells_unregistrer;
        
        private XmlNamespaceManager ns_manager;
        private readonly int columnFirst;
        private readonly int columnLast;
        private readonly bool delete_whole_row;
        private Worksheet parent_sheet;
    }
}
