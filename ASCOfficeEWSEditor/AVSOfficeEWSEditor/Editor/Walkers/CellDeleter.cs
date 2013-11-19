using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class CellDeleter : ICellSetter
    {
        public CellDeleter(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            cells_unregistrer = new CellsUnregistrer(parent_sheet);
        }

        #region ICellSetter Members

        public virtual bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell cell = new Cells.Cell(row, col);
            // Unregister formula and shared strings
            cells_unregistrer.Set(row, col, cell_tag);
            // Remove cell XML tag
            cell_tag.ParentNode.RemoveChild(cell_tag);
                    
            // Notify recalculation engine
            parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell, parent_sheet);

            return true;
        }

        public void PostProcessing()
        {
            cells_unregistrer.PostProcessing();
        }

        #endregion


        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;
        private CellsUnregistrer cells_unregistrer;
    }

    internal class ExceptCellDeleter : CellDeleter
    {
        public ExceptCellDeleter(int excluded_cell_row, int excluded_cell_column, Worksheet parent_sheet)
            : base(parent_sheet)
        {
            this.excluded_cell_row = excluded_cell_row;
            this.excluded_cell_column = excluded_cell_column;
        }

        #region ICellSetter Members

        public override bool Set(int row, int col, XmlElement cell_tag)
        {
            if (row == excluded_cell_row && col == excluded_cell_column)
            {
                return true;
            }
            return base.Set(row, col, cell_tag);
        }

        #endregion

        private int excluded_cell_row;
        private int excluded_cell_column;
    }


}
