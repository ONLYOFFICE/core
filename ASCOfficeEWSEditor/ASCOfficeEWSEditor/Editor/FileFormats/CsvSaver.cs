using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;

namespace ASCOfficeEWSEditor.Editor.FileFormats
{
    internal class CsvSaver
    {
        static internal void Save(Workbook wb, string file_path, int code_page, char delimiter)
        {
            Worksheet active_sheet = wb.ActiveSheet as Worksheet;

            if (active_sheet != null)
            {
                using (StreamWriter writer = new StreamWriter(file_path, false, System.Text.Encoding.GetEncoding(code_page)))
                {
                    Cells.CellRange cells_boundary = active_sheet.GetNonEmptyCells0Range();
                    if (cells_boundary == Cells.CellRange.Empty)
                    {
                        writer.WriteLine();
                        return;
                    }
                    int nColumnCount = cells_boundary.ColumnLast;
                    int nRowCount = cells_boundary.RowLast;
                    if (0 >= nColumnCount && 0 >= nRowCount)
                    {
                        writer.WriteLine();
                        return;
                    }


                    ASCOfficeEWSEditor.Editor.Walkers.StyleCollectorCache oCache = new ASCOfficeEWSEditor.Editor.Walkers.StyleCollectorCache(wb);

                    Range oRange = active_sheet.Range("A1:" + Helpers.ReferencesFormat.column2str(nColumnCount, true) + nRowCount.ToString());

                    Dictionary<int, ASCOfficeEWSEditor.Editor.Format.FormatSimple> arrRowsDefaultStyle;
                    List<ASCOfficeEWSEditor.Editor.Format.CCollFormat> arrCollsDefaultStyle;
                    List<CellSimple> arrCells = oRange.GetCellSimple(oCache, out arrRowsDefaultStyle, out arrCollsDefaultStyle);

                    int nColumnCurrent = 1;
                    int nRowCurrent = 1;

                    int nColumnTmp = 0;
                    int nRowTmp = 0;
                    bool bColumnTmp_rel = false;
                    bool bRowTmp_rel = false;

                    foreach (CellSimple oCellSimple in arrCells)
                    {
                        Helpers.ReferencesFormat.str2loc(oCellSimple.ID, out nRowTmp, out bRowTmp_rel, out nColumnTmp, out bColumnTmp_rel);
                        for (; nRowCurrent < nRowTmp; ++nRowCurrent)
                        {
                            for (; nColumnCurrent <= nColumnCount; ++nColumnCurrent)
                                writer.Write(delimiter);

                            writer.WriteLine();
                            nColumnCurrent = 1;
                        }

                        for (; nColumnCurrent < nColumnTmp; ++nColumnCurrent)
                            writer.Write(delimiter);

                        string strCellText = Escape(oCellSimple.DispValue, delimiter);
                        writer.Write(strCellText);
                    }
                }
            }
        }

        static private string Escape(string cell_value, char delimiter)
        {
            if (cell_value.IndexOf(delimiter) != -1 || cell_value.IndexOf('\"') != -1 || -1 != cell_value.IndexOf("\n"))
            {
                return "\"" + cell_value.Replace("\"", "\"\"") + "\"";
            }
            return cell_value;
        }
    }
}
