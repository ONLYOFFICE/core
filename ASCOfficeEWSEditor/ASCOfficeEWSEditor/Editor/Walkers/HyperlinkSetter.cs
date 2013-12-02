using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;
using System.Globalization;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class HyperlinkSetter : ICellSetter
    {
        public HyperlinkSetter( Worksheet parent_sheet, string sValue )
        {
            this.m_oWorksheet = parent_sheet;
            this.m_sValue = sValue;
        }
        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            Cells.Cell cell = new Cells.Cell(row, col);
            Hyperlinks.Hyperlink oCurHyp = m_oWorksheet.Hyperlinks[cell.ToString()];
            //удаляем ссылку если существует( т.к. иначе не менялся тип ссылки с url на внутреннюю )
            if (null != oCurHyp)
                m_oWorksheet.Hyperlinks.RemoveHyperlink(cell);

            Hyperlinks.Hyperlink oNewHyp = new Hyperlinks.Hyperlink(m_oWorksheet);
            oNewHyp.Anchor = cell.ToString();
            ParseValue(ref oNewHyp);
            if (null != oNewHyp)
                m_oWorksheet.Hyperlinks[cell.ToString()] = oNewHyp;
            return true;
        }

        public void PostProcessing()
        {
        }

        #endregion

        private void ParseValue( ref Hyperlinks.Hyperlink hyp)
        {
            //для гиперссылки формат "hyp|{URL}"
            //для внутренней формат "ran|{SheetName}|{Range}"
            string[] aParams = m_sValue.Split( HyperlinkGetter.g_cCharDelimiter );
            if (2 == aParams.Length && "hyp" == aParams[0])
            {
                if ("" != aParams[1])
                {
                    hyp.Target = aParams[1];
                    hyp.ReferredSheet = null;
                    hyp.ReferredRange = "";
                }
                else
                    hyp = null;
            }
            else if (3 == aParams.Length && "ran" == aParams[0])
            {
                if ("" == aParams[1] || "" == aParams[1])
                    hyp = null;
                else
                {
                    string sLocation = aParams[1] + "!" + aParams[2];
                    int start_pos = 0;
                    Worksheet ws_from;
                    Worksheet ws_to;
                    if (Calculation.Formulas.ParsedFormula.extract_3D_part(sLocation, ref start_pos, out ws_from, out ws_to, m_oWorksheet.Workbook))
                    {
                        hyp.Target = "";
                        hyp.ReferredSheet = ws_from;
                        hyp.ReferredRange = sLocation.Substring(start_pos);
                    }
                }
            }
            else
                hyp = null;
        }

        private Worksheet m_oWorksheet;
        private string m_sValue;
    }
}
