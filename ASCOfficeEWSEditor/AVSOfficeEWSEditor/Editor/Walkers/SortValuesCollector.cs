using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class SortValuesCollector : IStatCollector
    {
        public SortValuesCollector(Workbook workbook, Sorting.SortOrientation orientation)
        {
            this.workbook = workbook;
            this.orientation = orientation;
            ns_manager = workbook.NamespaceManager;

            Values = new List<Sorting.ValueRowColumn>();

        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            XmlNode value_tag = cell_tag.SelectSingleNode("main:v", ns_manager);
            if (value_tag != null)
            {
                string value_type = cell_tag.GetAttribute("t");
                Values.Add(new Sorting.ValueRowColumn(ValueGetter.RecognizeValue(value_tag.InnerText, value_type, workbook), orientation == Sorting.SortOrientation.by_rows ? row : col));
            }
            else
            {
                Values.Add(new Sorting.ValueRowColumn(new Calculation.Formulas.DataObjects.ObjectEmpty(), orientation == Sorting.SortOrientation.by_rows ? row : col));
            }
            return true;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            Values.Add(new Sorting.ValueRowColumn(new Calculation.Formulas.DataObjects.ObjectEmpty(), orientation == Sorting.SortOrientation.by_rows ? row : col));
            return true;
        }

        #endregion

        private Sorting.SortOrientation orientation;

        public List<Sorting.ValueRowColumn> Values { get; private set; }

        private XmlNamespaceManager ns_manager;
        private Workbook workbook;

    }
}

