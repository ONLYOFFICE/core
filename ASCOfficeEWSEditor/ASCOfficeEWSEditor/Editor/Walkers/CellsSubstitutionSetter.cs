using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
//using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class CellsSubstitutionSetter : ICellSetter
    {
        public CellsSubstitutionSetter(Dictionary<Cells.Cell, XmlElement> found_references, List<Sorting.ValueRowColumn> sorted_rows_columns,
                                        Sorting.SortOrientation orientation, Cells.Cell top_left, Worksheet parent_sheet)
        {
            this.found_references = found_references;
            this.orientation = orientation;
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            this.sorted_rows_columns = sorted_rows_columns;
            cells_unregistrer = new CellsUnregistrer(parent_sheet);
            this.top_left = top_left;

        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            if ((orientation == Sorting.SortOrientation.by_rows ? row - top_left.Row : col - top_left.Column) < sorted_rows_columns.Count)
            {
                Cells.Cell other_cell = orientation == Sorting.SortOrientation.by_rows ? new Cells.Cell(sorted_rows_columns[row - top_left.Row].row_column, col) :
                                                                                 new Cells.Cell(row, sorted_rows_columns[col - top_left.Column].row_column);
                XmlElement other_cell_tag = found_references[other_cell];
                Cells.Cell cell = new Cells.Cell(row, col);

                // Create and place new cell tag
                XmlElement created_cell_tag = cell_tag.OwnerDocument.CreateElement("c", ns_manager.LookupNamespace("main"));
                created_cell_tag.SetAttribute("r", cell_tag.GetAttribute("r"));
                string style = other_cell_tag != null ? other_cell_tag.GetAttribute("s") : "";
                if(style.Length != 0)
                {
                    created_cell_tag.SetAttribute("s", style);
                }
                cell_tag.ParentNode.InsertBefore(created_cell_tag, cell_tag);
                // Remove former cell
                cell_tag.ParentNode.RemoveChild(cell_tag);

                if (other_cell_tag == null || !other_cell_tag.HasChildNodes) // If the copied cell is empty
                {
                    // Unregister former cell
                    cells_unregistrer.Set(row, col, cell_tag);
                    // Notify recalculation engine
                    parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell, parent_sheet);

                    return true;
                }

                // Set Formula or Value to the created cell
                XmlNode formula_tag = other_cell_tag.SelectSingleNode("main:f", ns_manager);
                if(formula_tag != null)
                {
                    FormulaSetter formula_setter = new FormulaSetter(formula_tag.InnerText, other_cell, parent_sheet);
                    formula_setter.Set(row, col, created_cell_tag);
                    formula_setter.PostProcessing();
                }
                else
                {
                    XmlNode value_tag = other_cell_tag.SelectSingleNode("main:v", ns_manager);
                    Calculation.Formulas.DataObjects.IDataObject elem = ValueGetter.RecognizeValue(value_tag != null ? value_tag.InnerText : null,
                                                other_cell_tag.GetAttribute("t"), parent_sheet.Workbook);
                    ValueSetter value_setter = new ValueSetter(elem, parent_sheet, 1);
                    value_setter.Set(row, col, created_cell_tag);
                    value_setter.PostProcessing();
                }
            }
            else
            {
                ValueSetter value_setter = new ValueSetter(new Calculation.Formulas.DataObjects.ObjectEmpty(), parent_sheet, 1);
                value_setter.Set(row, col, cell_tag);
                value_setter.PostProcessing();
            }
            return true;
        }

        public void PostProcessing()
        {
            cells_unregistrer.PostProcessing();
        }

        #endregion

        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;

        private Dictionary<Cells.Cell, XmlElement> found_references;
        private List<Sorting.ValueRowColumn> sorted_rows_columns;
        private Sorting.SortOrientation orientation;
        private CellsUnregistrer cells_unregistrer;
        private Cells.Cell top_left;
    }


}
