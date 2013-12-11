using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class CellsUnregistrer : ICellSetter
    {
        public CellsUnregistrer(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
            removed_strings_collector = new RemovedStringsCollector(parent_sheet.Workbook);
        }

        #region ICellSetter Members

        public virtual bool Set(int row, int col, XmlElement cell_tag)
        {
            removed_strings_collector.Collect(row, col, cell_tag);
            Cells.Cell cell = new Cells.Cell(row, col);

            // Remove former Formula registration
            Calculation.Formulas.CellParsedFormula existent_formula;
            if (parent_sheet.Formulas.TryGetValue(cell, out existent_formula))
            {
                existent_formula.UnregisterDependencies();  // dependencies are broken here (for example, look into PtgRef.Unregister())
                parent_sheet.Formulas.Remove(cell);
            }
            return true;
        }

        public void PostProcessing()
        {
            removed_strings_collector.DecrementCounters();
        }

        #endregion


        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;
        private RemovedStringsCollector removed_strings_collector;
    }


}
