using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using System.Xml.XPath;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class RowsRowNumberCorrector : IRowsSetter
    {
        public RowsRowNumberCorrector(Cells.Offset offset, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            correction_value = offset.RowOffset;
            nothing_to_do = offset.RowOffset == 0;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            cells_unregistrer = new CellsUnregistrer(parent_sheet);
        }

        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            if(nothing_to_do)
            {
                return true;
            }
            int row_corrected = row_num + correction_value;
            if (row_corrected > Helpers.ReferencesFormat.maxRow)
            {
                foreach (XmlElement cell_tag in row_tag.SelectNodes("main:c", ns_manager))
                {
                    Cells.Cell cell = new Cells.Cell(cell_tag.GetAttribute("r"));
                    // Unregister formula and shared strings
                    cells_unregistrer.Set(cell.Row, cell.Column, cell_tag);

                    // Mark Updated
                    Calculation.CellItem cell_subj = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
                    cell_subj.CellOrFormulaUpdated = true;
                }
                // Notify recalculation engine
                parent_sheet.Workbook.ValuesUpdateManager.NotifyDeleted(new Cells.CellRange(row_num, 1, row_num, Helpers.ReferencesFormat.maxCol), parent_sheet);
                row_tag.ParentNode.RemoveChild(row_tag);
            }
            else
            {
                row_tag.SetAttribute("r", row_corrected.ToString());
            }
            return true;
        }

        public void PostProcessing()
        {
            cells_unregistrer.PostProcessing();
        }

        #endregion

        private CellsUnregistrer cells_unregistrer;
        private readonly int correction_value;
        private readonly bool nothing_to_do;
        private XmlNamespaceManager ns_manager;
        private Worksheet parent_sheet;
    }
}
