using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class ValueGetter : IStatCollector
    {
        public ValueGetter(Workbook workbook)
        {
            this.workbook = workbook;
            ns_manager = workbook.NamespaceManager;
            raw_value = null;
            value_type = null;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            XmlNode value_tag = cell_tag.SelectSingleNode("main:v", ns_manager);
            return CollectValue(value_tag == null ? "" : value_tag.InnerText, cell_tag.GetAttribute("t"));
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return CollectValue("", "");
        }

        #endregion

        private bool CollectValue(string value_extracted, string type_extracted)
        {
            if (raw_value == null)
            {
                raw_value = value_extracted;
                value_type = type_extracted;
                return true; // continue
            }
            else if (value_type == type_extracted && raw_value == value_extracted)
            {
                return true; // continue
            }
            else
            {
                raw_value = null;
                value_type = null;
                return false;
            }
        }

        public object Value 
        { 
            get 
            {
                Calculation.Formulas.DataObjects.IDataObject internal_value = InternalValue;
                if (null == internal_value)
                {
                    return null;
                }
                return internal_value.UserValue;
            }
        }

        internal Calculation.Formulas.DataObjects.IDataObject InternalValue
        {
            get
            {
                if (null == raw_value)
                {
                    return null;
                }
                return RecognizeValue(raw_value, value_type, workbook);
            }
        }


        internal static Calculation.Formulas.DataObjects.IDataObject RecognizeValue(string value, string value_type, Workbook workbook)
        {
            if (null == value)
            {
                return new Calculation.Formulas.DataObjects.ObjectEmpty();
            }
            switch (value_type.ToLower())
            {
                case "b":
                    return new Calculation.Formulas.DataObjects.ObjectBoolean(value.ToLower() == "true" || value == "1");
                case "n":
                case "":
                    {
                        double val_numeric;
                        if (XMLDouble.TryParse(value, out val_numeric))
                        {
                            return new Calculation.Formulas.DataObjects.ObjectNumber(val_numeric);
                        }
                        else
                        {
                            if(value.Length != 0)
                            {
                                return new Calculation.Formulas.DataObjects.ObjectString(value);
                            }
                            return new Calculation.Formulas.DataObjects.ObjectEmpty();
                        }
                    }
                case "s":
                    {
                        int string_id = XMLInt.ParseOrDefault(value, -1);
                        return new Calculation.Formulas.DataObjects.ObjectString(string_id != -1 ? workbook.SharedStrings.GetString(string_id) : "");
                    }
                case "e":
                    {
                        return new Calculation.Formulas.DataObjects.ObjectError(Calculation.Formulas.ErrorValue.Parse(value));
                    }
                case "d":
                case "inlinestr":
                case "str":
                default:
                    return new Calculation.Formulas.DataObjects.ObjectString(value);
            }
        }

        private XmlNamespaceManager ns_manager;
        private string raw_value;
        private string value_type;
        private Workbook workbook;

    }
}
