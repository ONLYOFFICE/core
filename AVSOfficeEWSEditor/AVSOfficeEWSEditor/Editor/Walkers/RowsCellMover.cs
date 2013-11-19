using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class RowsCellMover : IRowsSetter
    {
        public RowsCellMover(int columnFirst, int columnLast, Cells.Offset offset, Worksheet parent_sheet)
        {
            this.offset = offset;
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            this.columnFirst = columnFirst;
            this.columnLast = columnLast;
            
            RowCorrector = new RowsRowNumberCorrector(offset, parent_sheet);
            CellCorrector = new CellRowNumberCorrector(offset);

            formulas_to_move = new LinkedList<Calculation.Formulas.CellParsedFormula>();
        }

        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            RowCorrector.Set(row_tag, row_num);
            if (row_num + offset.RowOffset <= Helpers.ReferencesFormat.maxRow)
            {
                foreach (XmlElement cell_tag in row_tag.SelectNodes("main:c", ns_manager))
                {
                    Cells.Cell cell = new Cells.Cell(cell_tag.GetAttribute("r"));
                    if (cell.Column >= columnFirst && cell.Column <= columnLast)
                    {
                        Cells.Cell corrected_cell = cell + offset;
                        // Change row or column number of the cell
                        CellCorrector.Set(cell.Row, cell.Column, cell_tag);

                        // Move Formula
                        Calculation.Formulas.CellParsedFormula moved_formula;
                        if (parent_sheet.Formulas.TryGetValue(cell, out moved_formula))
                        {
                            if (offset.RowOffset > 0 || offset.ColumnOffset > 0)
                            {
                                formulas_to_move.AddFirst(moved_formula);
                            }
                            else
                            {
                                formulas_to_move.AddLast(moved_formula);
                            }
                        }

                        Calculation.CellItem former_cell_item = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
                        Calculation.CellItem corrected_cell_item = parent_sheet.SubjectsStorage.GetCellItemOrCreate(corrected_cell);
                        former_cell_item.CellOrFormulaUpdated = true;
                        corrected_cell_item.CellOrFormulaUpdated = true;

                        bool update_delayed = former_cell_item.UpdateDelayed;
                        former_cell_item.UpdateDelayed = false;
                        corrected_cell_item.UpdateDelayed = update_delayed;
                        // The necessary notification is performed outside of the setter (Range.DeleteRows)
                    }
                }
            }
            return true;
        }

        public void PostProcessing()
        {
            RowCorrector.PostProcessing();
            CellCorrector.PostProcessing();
            foreach (Calculation.Formulas.CellParsedFormula moved_formula in formulas_to_move)
            {
                Cells.Cell cell = moved_formula.Cell;
                Cells.Cell corrected_cell = cell + offset;
                parent_sheet.Formulas.Remove(cell);
                parent_sheet.Formulas.Add(corrected_cell, moved_formula);
                moved_formula.MoveToAnotherCell(corrected_cell);
            }
        }

        #endregion

        private Cells.Offset offset;

        private RowsRowNumberCorrector RowCorrector;
        private CellRowNumberCorrector CellCorrector;
        private LinkedList<Calculation.Formulas.CellParsedFormula> formulas_to_move;

        private readonly int columnFirst;
        private readonly int columnLast;

        private XmlNamespaceManager ns_manager;
        private Worksheet parent_sheet;
    }
}
