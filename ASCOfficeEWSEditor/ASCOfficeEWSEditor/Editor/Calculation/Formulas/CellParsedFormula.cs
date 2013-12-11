using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas
{
    internal class CellParsedFormula : ParsedFormula
    {
        internal CellParsedFormula(Cells.Cell cell, Worksheet parent_sheet)
            : base(parent_sheet.Workbook)
        {
            this.parent_sheet = parent_sheet;
            this.cell = cell;
            cell_observer = null;
        }

        public override void UnregisterDependencies()
        {
            if (cell_observer != null)
            {
                cell_observer.BecomeSubject();
                cell_observer = null;
            }
            base.UnregisterDependencies();
        }

        public void RegisterDependencies()
        {
            // Create or update an Observer
            cell_observer = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
            cell_observer.BecomeObserver(this);
            base.RegisterDependencies(cell_observer);
        }

        public void MoveToAnotherCell(Cells.Cell another_cell)
        {
            UnregisterDependencies();
            cell = another_cell;
            RegisterDependencies();
        }

        internal override Worksheet ParentSheet { get { return parent_sheet; } }
        public Cells.Cell Cell { get { return cell; } } // For GetUpdatedCells()

        private Cells.Cell cell;
        private Worksheet parent_sheet;
        CellItem cell_observer;
    }
}
