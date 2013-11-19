using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class FormulaGetter : IStatCollector
    {
        public FormulaGetter(Workbook workbook)
        {
            ns_manager = workbook.NamespaceManager;
            raw_value = null;
//             value_type = null;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            XmlNode formula_tag = cell_tag.SelectSingleNode("main:f", ns_manager);
            return CollectValue(formula_tag == null ? "" : formula_tag.InnerText/*, ((XmlElement)cell_tag).GetAttribute("t")*/);
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return CollectValue(""/*, ""*/);
        }

        #endregion

        private bool CollectValue(string value_extracted/*, string type_extracted*/)
        {
            if (raw_value == null)
            {
                raw_value = value_extracted;
//                value_type = type_extracted;
                return true; // continue
            }
            else if (/*value_type == type_extracted && */raw_value == value_extracted)
            {
                return true; // continue
            }
            else
            {
                raw_value = null;
//                 value_type = null;
                return false;
            }
        }

        public string Value 
        { 
            get 
            {
                return raw_value;
            }
        }

        private XmlNamespaceManager ns_manager;
        private string raw_value;
//         private string value_type;
    }
}
