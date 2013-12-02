using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    /// <summary>
    /// Designed to be called with RangeWalker.UpdateCells or CellWalker.UpdateCells
    /// </summary>
    internal class FormulaTextAndValueUpdater : ICellSetter
    {
        public FormulaTextAndValueUpdater(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell current_cell = new Cells.Cell(row, col);
            Calculation.CellItem cell_item = parent_sheet.SubjectsStorage.GetCellItemOrCreate(current_cell);
            //Update formula
            Calculation.Formulas.CellParsedFormula current_formula;
            if (cell_item.CellOrFormulaUpdated && parent_sheet.Formulas.TryGetValue(current_cell, out current_formula))
            {
                FormulaSimpleSetter simple_setter = new FormulaSimpleSetter(current_formula.Assemble(), parent_sheet);
                simple_setter.Set(current_cell.Row, current_cell.Column, cell_tag);
            }
            // Update Value
            if (cell_item.HasFormula() && cell_item.UpdateDelayed)
            {
                cell_item.RecalcFormulaAndUpdateCell();
                parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell_item.Cell, parent_sheet);
                cell_item.UpdateDelayed = false;
            }

            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private Worksheet parent_sheet;
    }

}
