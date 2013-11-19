using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Walkers
{
    internal class CellSimpleGetter : IStatCollector
    {
        public CellSimpleGetter(Workbook workbook, StyleCollectorCache oStyleCache)
        {
            ns_manager = workbook.NamespaceManager;
            m_oWorkbook = workbook;
            m_oWorksheet = workbook.ActiveSheet as Worksheet;
            m_oStyleCache = oStyleCache;
        }

        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            CellSimple oNewCellSimple = new CellSimple();
            oNewCellSimple.ID = (new AVSOfficeEWSEditor.Editor.Cells.Cell(row, col)).ToString();

            //Style
            int xf_index = XMLInt.ParseOrDefault(cell_tag.GetAttribute("s"), -1);
            if (xf_index == -1)
            {
                xf_index = XMLInt.ParseOrDefault(((XmlElement)cell_tag.ParentNode).GetAttribute("s"), -1);
                if (xf_index == -1)
                {
                    XmlElement col_tag = (XmlElement)cell_tag.ParentNode.ParentNode.ParentNode.SelectSingleNode(
                        "main:cols/main:col[@min <= " + col + " and @max >= " + col + "]", ns_manager);
                    xf_index = col_tag != null ? XMLInt.ParseOrDefault(col_tag.GetAttribute("style"), 0) : 0;
                }
            }
            if (-1 != xf_index)
            {
                if (null != m_oStyleCache)
                    oNewCellSimple.Format = m_oStyleCache.GetStyle(xf_index);
                else
                    oNewCellSimple.Format = new Format.FormatSimple(xf_index, m_oWorkbook);
            }
            else
                oNewCellSimple.Format = new Format.FormatSimple();

            //Formula
            XmlNode formula_tag = cell_tag.SelectSingleNode("main:f", ns_manager);
            oNewCellSimple.Formula = (formula_tag == null) ? "" : formula_tag.InnerText.Replace(',', ';');

            //Display value
            XmlNode value_tag = cell_tag.SelectSingleNode("main:v", ns_manager);
            //если в ячейке есть формула, но нет значения - пересчитывать формулу, с целью получения значения.
            if (!string.IsNullOrEmpty(oNewCellSimple.Formula) && (value_tag == null || string.IsNullOrEmpty(value_tag.InnerText)))
            {
                Cells.Cell current_cell = new Cells.Cell(row, col);
                Calculation.CellItem cell_item = m_oWorksheet.SubjectsStorage.GetCellItemOrCreate(current_cell);
                cell_item.RecalcFormulaAndUpdateCell();
                m_oWorksheet.Workbook.ValuesUpdateManager.NotifyUpdatedCell(cell_item.Cell, m_oWorksheet);
                cell_item.UpdateDelayed = false;
                //Повторно запрашиваем значение, потому что оно пересчиталось
                value_tag = cell_tag.SelectSingleNode("main:v", ns_manager);
            }
            if (value_tag == null)
            {
                oNewCellSimple.DispValue = "";
                oNewCellSimple.InternalValue = "";
            }
            else
            {
                Calculation.Formulas.DataObjects.IDataObject piDataObject = ValueGetter.RecognizeValue(value_tag.InnerText, cell_tag.GetAttribute("t"), m_oWorkbook);
                if (piDataObject is Calculation.Formulas.DataObjects.ObjectEmpty || piDataObject == null)
                {
                    oNewCellSimple.DispValue = "";
                    oNewCellSimple.InternalValue = "";
                }
                else
                {
                    int? number_format_id = oNewCellSimple.Format.NumberFormatId;
                    if (!number_format_id.HasValue)
                    {
                        oNewCellSimple.DispValue = "";
                        oNewCellSimple.InternalValue = "";
                    }
                    else
                    {
                        string number_format_string = m_oWorkbook.StyleSheet.GetNumberFormatString(number_format_id.Value);
                        oNewCellSimple.DispValue = (string)NumberFormat.NumberFormatParser.MakeDisplayValue(piDataObject, number_format_string);
                        oNewCellSimple.ValueType = piDataObject.Type;
                        oNewCellSimple.InternalValue = piDataObject.ToString();
                        //значения формата взяты из web интерфейса
                        string sLowerFormat = number_format_string.ToLower();
                        if ((-1 != sLowerFormat.IndexOf('h') || -1 != sLowerFormat.IndexOf('s')) &&
                             (-1 != sLowerFormat.IndexOf('d') || -1 != sLowerFormat.IndexOf('y')))
                        {
                            number_format_string = "m/d/yy h:mm";
                            oNewCellSimple.InternalDate = (string)NumberFormat.NumberFormatParser.MakeDisplayValue(piDataObject, number_format_string);

                        }
                        else if (-1 != sLowerFormat.IndexOf('h') || -1 != sLowerFormat.IndexOf('s'))
                        {
                            number_format_string = "h:mm:ss";
                            oNewCellSimple.InternalDate = (string)NumberFormat.NumberFormatParser.MakeDisplayValue(piDataObject, number_format_string);

                        }
                        else if (-1 != sLowerFormat.IndexOf('d') || -1 != sLowerFormat.IndexOf('y'))
                        {
                            number_format_string = "mm-dd-yyyy";
                            oNewCellSimple.InternalDate = (string)NumberFormat.NumberFormatParser.MakeDisplayValue(piDataObject, number_format_string);
                        }
                        else
                        {
                            number_format_string = "General";
                            oNewCellSimple.InternalDate = (string)NumberFormat.NumberFormatParser.MakeDisplayValue(piDataObject, number_format_string);
                        }
                    }
                }
            }

            //Hyperlink
            oNewCellSimple.Hyperlink = "";
            Cells.Cell oCell = new Cells.Cell(row, col);
            Hyperlinks.Hyperlink oCurHyp = m_oWorksheet.Hyperlinks[oCell.ToString()];
            if (null != oCurHyp)
            {
                //для гиперссылки возвращаем строку "hyp|{URL}"
                //для внутренней возвращаем строку "ran|{SheetName}|{Range}"
                if (true == oCurHyp.IsLocationOrAddressType)
                    oNewCellSimple.Hyperlink = "ran" + HyperlinkGetter.g_cCharDelimiter + oCurHyp.ReferredSheet.Name + HyperlinkGetter.g_cCharDelimiter + oCurHyp.ReferredRange;
                else
                    oNewCellSimple.Hyperlink = "hyp" + HyperlinkGetter.g_cCharDelimiter + oCurHyp.Target;
            }

            //HasMergedCells
            oNewCellSimple.HasMergedCells = m_oWorksheet.MergeCells.HasMergedCells(new Cells.CellRange(oCell.ToString(), m_oWorkbook));

            m_oCells.Add(oNewCellSimple);
            return true;
        }

        public Format.FormatSimple GetFormat(int xf_index)
        {
            if (null != m_oStyleCache)
                return m_oStyleCache.GetStyle(xf_index);

            return new Format.FormatSimple(xf_index, m_oWorkbook);
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            return false;
        }

        #endregion

        public List<CellSimple> Value
        {
            get
            {
                return m_oCells;
            }
        }

        private XmlNamespaceManager ns_manager;
        private string raw_value;
        Workbook m_oWorkbook;
        Worksheet m_oWorksheet;
        StyleCollectorCache m_oStyleCache;
        List<CellSimple> m_oCells = new List<CellSimple>();

        public Dictionary<int, Format.FormatSimple> m_arrRowsDefaultStyle = new Dictionary<int, Format.FormatSimple>();
        public List<Format.CCollFormat> m_arrCollsDefaultStyle = new List<Format.CCollFormat>();
    }
}
