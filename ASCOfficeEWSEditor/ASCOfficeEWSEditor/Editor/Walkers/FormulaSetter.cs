using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class FormulaSetter : ICellSetter
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="formula"></param>
        /// <param name="base_cell">Top left cell of the set range or other base cell if the formula is copied from somewhere</param>
        /// <param name="parent_sheet"></param>
        public FormulaSetter(string formula, Cells.Cell base_cell, Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            this.base_cell = base_cell;
            this.formula_origin = formula;
        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell cell = new Cells.Cell(row, col);
            Calculation.Formulas.CellParsedFormula cell_parsed_formula = new Calculation.Formulas.CellParsedFormula(cell, parent_sheet);
            if (!cell_parsed_formula.Parse(formula_origin, cell - base_cell))
            {
                string formula_origin_error = formula_origin.Replace(";", ",");

                if (cell_parsed_formula.LastError.GetType() != new Events.UnknownOperandError("", 0).GetType() &&
                    cell_parsed_formula.LastError.GetType() != new Events.UnknownOperatorError("", 0).GetType())
                    return false;

                FormulaSimpleSetter simple_setter_error = new FormulaSimpleSetter(formula_origin_error, parent_sheet);
                simple_setter_error.Set(row, col, cell_tag);
                
                // Remove former Formula registration
                Calculation.Formulas.CellParsedFormula existent_formula_error;
                if (parent_sheet.Formulas.TryGetValue(cell, out existent_formula_error))
                {
                    existent_formula_error.UnregisterDependencies();  // dependencies are broken here (for example, look into PtgRef.UnregisterDependency())
                    // The former formula is not removed from parent_sheet.Formulas because the new one is set in the next line
                }

                // New Formula registration
                parent_sheet.Formulas[cell] = cell_parsed_formula;
                cell_parsed_formula.RegisterDependencies(); // After this line we can be sure that the cell has an associated observer

                Calculation.CellItem observer_error = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);

                observer_error.CellSetErrorValue(new Calculation.Formulas.DataObjects.ObjectError(Calculation.Formulas.ErrorValue.wrong_name));

                // Notify recalculation engine
                parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell, parent_sheet);

                return true;
            }
            FormulaSimpleSetter simple_setter = new FormulaSimpleSetter(cell_parsed_formula.Assemble(), parent_sheet);
            simple_setter.Set(row, col, cell_tag);
            
            // Remove former Formula registration
            Calculation.Formulas.CellParsedFormula existent_formula;
            if (parent_sheet.Formulas.TryGetValue(cell, out existent_formula))
            {
                existent_formula.UnregisterDependencies();  // dependencies are broken here (for example, look into PtgRef.UnregisterDependency())
                // The former formula is not removed from parent_sheet.Formulas because the new one is set in the next line
            }
            // New Formula registration
            parent_sheet.Formulas[cell] = cell_parsed_formula;
            cell_parsed_formula.RegisterDependencies(); // After this line we can be sure that the cell has an associated observer

            Calculation.CellItem observer = parent_sheet.SubjectsStorage.GetCellItemOrCreate(cell);
            if (!observer.RecalcFormulaAndUpdateCell())
            {
                cell_parsed_formula.UnregisterDependencies();
                parent_sheet.Formulas.Remove(cell);
            }

            // Notify recalculation engine
            parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell, parent_sheet);

            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private string formula_origin;
        private Cells.Cell base_cell;
        private Worksheet parent_sheet;
    }

    internal class FormulaSimpleSetter : ICellSetter
    {
        public FormulaSimpleSetter(string formula, Worksheet parent_sheet)
        {
            this.formula = formula;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            XmlNode formula_tag = cell_tag.SelectSingleNode("main:f", ns_manager);
            if (formula != null && formula.Length != 0)
            {
                if (formula_tag == null)
                {
                    formula_tag = cell_tag.OwnerDocument.CreateElement("f", ns_manager.LookupNamespace("main"));
                    cell_tag.InsertBefore(formula_tag, cell_tag.SelectSingleNode("main:v", ns_manager));
                }
                else
                {
                    ((XmlElement)formula_tag).RemoveAllAttributes();
                }
                formula_tag.InnerText = formula;
            }
            else
            {
                if (formula_tag != null)
                {
                    cell_tag.RemoveChild(formula_tag);
                }
            }
            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private string formula;
        private XmlNamespaceManager ns_manager;
    }
}
