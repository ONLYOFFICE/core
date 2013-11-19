using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class HyperlinkGetter : IStatCollector
    {
        public const char g_cCharDelimiter = (char)5;
        public HyperlinkGetter(Worksheet worksheet)
        {
            this.m_oWorksheet = worksheet;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            return CollectValue( row, col, m_oWorksheet);
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return CollectValue(row, col, parent_sheet);
        }

        #endregion

        private bool CollectValue(int row, int col, Worksheet sheet)
        {
            m_sValue = "";
            Cells.Cell oCell = new Cells.Cell(row, col);
            Hyperlinks.Hyperlink oCurHyp = sheet.Hyperlinks[oCell.ToString()];
            if (null != oCurHyp)
            {
                //для гиперссылки возвращаем строку "hyp|{URL}"
                //для внутренней возвращаем строку "ran|{SheetName}|{Range}"
                if (true == oCurHyp.IsLocationOrAddressType)
                    m_sValue = "ran" + g_cCharDelimiter + oCurHyp.ReferredSheet.Name + g_cCharDelimiter + oCurHyp.ReferredRange;
                else
                    m_sValue = "hyp" + g_cCharDelimiter + oCurHyp.Target;
            }
            return true;
        }

        public string Value
        {
            get{return m_sValue;}
        }

        private XmlNamespaceManager ns_manager;
        private string raw_value;
        private string m_sValue;
        private Worksheet m_oWorksheet;

    }
}
