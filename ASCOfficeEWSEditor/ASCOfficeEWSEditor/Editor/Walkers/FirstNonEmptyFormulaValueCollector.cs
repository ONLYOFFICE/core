using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    internal class FirstNonEmptyFormulaValueCollector : IStatCollector
    {
        public FirstNonEmptyFormulaValueCollector(Workbook workbook)
        {
            this.workbook = workbook;
            ns_manager = workbook.NamespaceManager;
            Value = null;
            Formula = null;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            ValueGetter value_getter = new ValueGetter(workbook);
            value_getter.Collect(row, col, cell_tag);
            Calculation.Formulas.DataObjects.IDataObject value = value_getter.InternalValue;

            FormulaGetter formula_getter = new FormulaGetter(workbook);
            formula_getter.Collect(row, col, cell_tag);
            string formula = formula_getter.Value;
            if (!(value is Calculation.Formulas.DataObjects.ObjectEmpty || value == null) || formula != null && formula != "")
            {
                Value = value;
                Formula = formula;
                return false; // Stop searching
            }
            return true; // Not found, continue searching
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return true;
        }

        #endregion


        private Workbook workbook;
        private XmlNamespaceManager ns_manager;

        internal Calculation.Formulas.DataObjects.IDataObject Value { get; private set; }
        internal string Formula { get; private set; }
    }

}
