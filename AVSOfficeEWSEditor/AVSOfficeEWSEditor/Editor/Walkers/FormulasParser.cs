using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    class CFormulaInfo
    {
        public string m_strFormulaText;
        public string m_strFormulaR;
    }

    internal class FormulasParser : IStatCollector
    {
        public FormulasParser(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            XmlElement formula_tag = null;// (XmlElement)cell_tag.SelectSingleNode("main:f", ns_manager);
            for (int i = 0; i < cell_tag.ChildNodes.Count; i++)
            {
                if ("f" == cell_tag.ChildNodes[i].Name)
                {
                    formula_tag = (XmlElement)cell_tag.ChildNodes[i];
                    break;
                }
            }
            if (formula_tag != null)
            {
                Cells.Cell cell = new Cells.Cell(row, col);
                Calculation.Formulas.CellParsedFormula cell_parsed_formula = new Calculation.Formulas.CellParsedFormula(cell, parent_sheet);
                if (formula_tag.InnerText.Length == 0 && formula_tag.GetAttribute("t") == "shared")
                {
                    string si = formula_tag.GetAttribute("si");
                    CFormulaInfo oInfo = null;
                    if (false == m_mapFormulas.TryGetValue(si, out oInfo) || null == oInfo)
                    {
                        XmlNode shared_formula_tag = cell_tag.SelectSingleNode("../../main:row/main:c/main:f[@si = " + si + "]", ns_manager);
                        if (null == shared_formula_tag)
                            return true;  //Just continue with the left correct formulas

                        oInfo = new CFormulaInfo();
                        oInfo.m_strFormulaText = shared_formula_tag.InnerText;
                        oInfo.m_strFormulaR = ((XmlElement)shared_formula_tag.ParentNode).GetAttribute("r");
                        m_mapFormulas.Add(si, oInfo);
                    }
                    Cells.Cell shared_cell = new Cells.Cell(oInfo.m_strFormulaR);
                    if (!cell_parsed_formula.Parse(oInfo.m_strFormulaText, cell - shared_cell))
                    {
                        return true;  //Just continue with the left correct formulas
                    }
                    formula_tag.RemoveAttribute("si");
                    formula_tag.RemoveAttribute("t");
                    formula_tag.InnerText = cell_parsed_formula.Assemble();

                }
                else
                {
                    string strSI = formula_tag.GetAttribute("si");
                    if (formula_tag.InnerText.Length != 0 && formula_tag.GetAttribute("t") == "shared" && !string.IsNullOrEmpty(strSI))
                    {
                        CFormulaInfo oFormulaInfo = new CFormulaInfo();
                        oFormulaInfo.m_strFormulaText = formula_tag.InnerText;
                        oFormulaInfo.m_strFormulaR = ((XmlElement)formula_tag.ParentNode).GetAttribute("r");
                        m_mapFormulas.Add(strSI, oFormulaInfo);
                    }

                    if (!cell_parsed_formula.Parse(formula_tag.InnerText, Cells.Offset.Null))
                    {
                        return true;  //Just continue with the left correct formulas
                    }
                }
                parent_sheet.Formulas[cell] = cell_parsed_formula;
                cell_parsed_formula.RegisterDependencies();
            }
            return true;
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return true;
        }

        #endregion

        #region Methods
        private Dictionary<string, CFormulaInfo> m_mapFormulas = new Dictionary<string, CFormulaInfo>();
        #endregion

        private XmlNamespaceManager ns_manager;
        private Worksheet parent_sheet;

    }
}
