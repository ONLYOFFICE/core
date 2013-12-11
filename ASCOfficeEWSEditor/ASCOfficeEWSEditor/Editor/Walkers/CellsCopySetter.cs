using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class CellsCopySetter : ICellSetter
    {
        public CellsCopySetter(Dictionary<Cells.Cell, XmlElement> source_references, Worksheet source_parent_sheet, Worksheet dest_parent_sheet, Cells.Offset offset)
        {
            this.source_parent_sheet = source_parent_sheet;
            this.dest_parent_sheet = dest_parent_sheet;
            source_ns_manager = source_parent_sheet.Workbook.NamespaceManager;
            dest_ns_manager = dest_parent_sheet.Workbook.NamespaceManager;
            this.offset = offset;
            this.source_references = source_references;
            cells_unregistrer = new CellsUnregistrer(dest_parent_sheet);
        }

        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell dest_cell = new Cells.Cell(row, col);
            Cells.Cell source_cell = dest_cell - offset;
            XmlElement source_cell_tag = source_references[source_cell];

            // Create and place new cell tag
            XmlElement created_cell_tag = cell_tag.OwnerDocument.CreateElement("c", dest_ns_manager.LookupNamespace("main"));
            created_cell_tag.SetAttribute("r", cell_tag.GetAttribute("r"));
            string style = source_cell_tag != null ? source_cell_tag.GetAttribute("s") : "";
            if (style.Length != 0 && dest_parent_sheet.Workbook == source_parent_sheet.Workbook)
            {
                created_cell_tag.SetAttribute("s", style);
            }
            cell_tag.ParentNode.InsertBefore(created_cell_tag, cell_tag);
            // Remove former cell
            cell_tag.ParentNode.RemoveChild(cell_tag);

            // Copy hyperlink if it exists in the source cell
            dest_parent_sheet.Hyperlinks[dest_cell.ToString()] = source_parent_sheet.Hyperlinks[source_cell.ToString()];

            if (source_cell_tag == null || !source_cell_tag.HasChildNodes) // If the copied cell is empty
            {
                // Unregister former cell
                cells_unregistrer.Set(row, col, cell_tag);
                // Notify recalculation engine
                dest_parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(dest_cell, dest_parent_sheet);
                return true;
            }

            // Set Formula or Value to the created cell
            XmlNode formula_tag = source_cell_tag.SelectSingleNode("main:f", source_ns_manager);
            if(formula_tag != null)
            {
                FormulaSetter formula_setter = new FormulaSetter(formula_tag.InnerText, source_cell, dest_parent_sheet);
                formula_setter.Set(row, col, created_cell_tag);
                formula_setter.PostProcessing();
            }
            else
            {
                XmlNode value_tag = source_cell_tag.SelectSingleNode("main:v", source_ns_manager);
                Calculation.Formulas.DataObjects.IDataObject elem = ValueGetter.RecognizeValue(value_tag != null ? value_tag.InnerText : null,
                                            source_cell_tag.GetAttribute("t"), source_parent_sheet.Workbook);
                ValueSetter value_setter = new ValueSetter(elem, dest_parent_sheet, 1);
                value_setter.Set(row, col, created_cell_tag);
                value_setter.PostProcessing();
            }
            return true;
        }

        public void PostProcessing()
        {
            cells_unregistrer.PostProcessing();
        }

        #endregion

        private Worksheet source_parent_sheet;
        private Worksheet dest_parent_sheet;
        private XmlNamespaceManager source_ns_manager;
        private XmlNamespaceManager dest_ns_manager;
        private Cells.Offset offset;
        private CellsUnregistrer cells_unregistrer;

        private Dictionary<Cells.Cell, XmlElement> source_references;
    }


}
