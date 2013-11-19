using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using System.Globalization;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class ValueSetter : ValueSimpleSetter
    {
        public ValueSetter(object value, Worksheet parent_sheet, int num_cells)
            : base(value, RecognizeValueType(value), parent_sheet, num_cells)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            updateHeaders = true;
        }

        public ValueSetter(object value, string strValueType, Worksheet parent_sheet, int num_cells, bool bUpdateHeaders)
            : base(value, strValueType, parent_sheet, num_cells)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            updateHeaders = bUpdateHeaders;
        }

        public ValueSetter(Calculation.Formulas.DataObjects.IDataObject value, Worksheet parent_sheet, int num_cells)
            : base(value.UserValue, value.Type, parent_sheet, num_cells)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            updateHeaders = true;
        }

        #region ICellSetter Members

        public override bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell cell = new Cells.Cell(row, col);
            // Remove formula XML tag
            XmlNode f_tag = cell_tag.SelectSingleNode("main:f", ns_manager);
            if(f_tag != null)
            {
                cell_tag.RemoveChild(f_tag);
            }
            // Remove former Formula registration
            Calculation.Formulas.CellParsedFormula existent_formula;
            if (parent_sheet.Formulas.TryGetValue(cell, out existent_formula))
            {
                existent_formula.UnregisterDependencies();  // dependencies are broken here (for example, look into PtgRef.Unregister())
                parent_sheet.Formulas.Remove(cell);
            }

            base.Set(row, col, cell_tag);

            if (updateHeaders)
            {
                // Проверяем, является ли ячейка заголовком таблицы
                int nTableIndex = -1;
                if (parent_sheet.isCellHeader(row, col, out nTableIndex))
                {
                    parent_sheet.changeCellHeader(nTableIndex, col);
                }
            }

            // Notify recalculation engine
            parent_sheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell, parent_sheet);

            return true;
        }

        public override void PostProcessing()
        {
            base.PostProcessing();
        }

        #endregion

        private static string RecognizeValueType(object value)
        {
            if (value is string)
            {
                double val_numeric;
                string val_str = value.ToString().ToUpper();
                if (val_str == "TRUE" || val_str == "FALSE")
                {
                    return "b";
                }
                else if (val_str == "#NULL!" || val_str == "#DIV/0!" || val_str == "#VALUE!" || val_str == "#REF!" ||
                    val_str == "#NAME?" || val_str == "#NUM!" || val_str == "#N/A")
                {
                    return "e";
                }
                else if (double.TryParse(val_str, NumberStyles.Float, new CultureInfo(0x409), out val_numeric))
                {
                    return "";
                }
                else if (val_str.Length == 0)
                {
                    return null;
                }
                else
                {
                    return "s";
                }
            }
            else if (value is double || value is float || value is Int32 || value is Int16 || value is Int64)
            {
                return "";
            }
            else if (value is bool)
            {
                return "b";
            }
            else if (value is Calculation.Formulas.ErrorValue)
            {
                return "e";
            }
            return null;
        }

        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;

        private bool updateHeaders;
    }

    internal class ValueSimpleSetter : ICellSetter
    {
        public ValueSimpleSetter(Calculation.Formulas.DataObjects.IDataObject value, Worksheet parent_sheet, int num_cells)
            : this(value.UserValue, value.Type, parent_sheet, num_cells)
        {
        }
        public ValueSimpleSetter(object value, string value_type, Worksheet parent_sheet, int num_cells)
        {
            this.value_type = value_type;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            if (value_type == "s")
            {
                value_set = parent_sheet.Workbook.RegisterSharedString(value.ToString(), num_cells).ToString();
            }
            else if (value_type == "str")
            {
                value_set = value.ToString();
            }
            else if (value_type == "b")
            {
                string bool_str;
                if ((bool_str = value as string) != null)
                {
                    value_set = bool_str.ToUpper() == "TRUE" ? "1" : "0";
                }
                else 
                {
                    value_set = XMLVariant.ToString(value);
                }
            }
            else if (value_type == null)
            {
                value_set = "";
            }
            else
            {
                value_set = XMLVariant.ToString(value);
            }
            removed_strings_collector = new RemovedStringsCollector(parent_sheet.Workbook);
        }

        #region ICellSetter Members

        public virtual bool Set(int row, int col, XmlElement cell_tag)
        {
            removed_strings_collector.Collect(row, col, cell_tag);
            
            XmlNode val_tag = cell_tag.SelectSingleNode("main:v", ns_manager);
            if (value_type == null)
            {
                if (val_tag != null)
                {
                    cell_tag.RemoveChild(val_tag);
                }
            }
            else
            {
                if (val_tag == null)
                {
                    val_tag = cell_tag.OwnerDocument.CreateElement("v", ns_manager.LookupNamespace("main"));
                    cell_tag.InsertAfter(val_tag, cell_tag.SelectSingleNode("main:f", ns_manager));
                }
                val_tag.InnerText = value_set;
            }

            if (value_type != "" && value_type != null)
            {
                cell_tag.SetAttribute("t", value_type);
            }
            else if (cell_tag.HasAttribute("t"))
            {
                cell_tag.RemoveAttribute("t");
            }
            return true;
        }

        public virtual void PostProcessing()
        {
            removed_strings_collector.DecrementCounters();
        }

        #endregion

        private XmlNamespaceManager ns_manager;
        public string value_set { get; set; }
        private string value_type;
        private RemovedStringsCollector removed_strings_collector;
    }

}
