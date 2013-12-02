using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.IO;
using System.Text.RegularExpressions;
using System.Globalization;
using AVSOfficeEWSEditor.Editor.Cells;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.FileFormats
{
    internal class CsvLoader
    {
        public struct WrapCells
        {
            public int m_nRow;
            public int m_nCol;
        };

        static internal void Load(Workbook wb, string file_path, int code_page, char delimiter)
        {
            if(!File.Exists(file_path))
            {
                throw new Events.ReportableException(new Events.InternalError("The specified path \"" + file_path + "\" is not found"));
            }

            Worksheet active_sheet = wb.ActiveSheet as Worksheet;

            //DateTime beforeTime = DateTime.Now;
            //Console.WriteLine(string.Format("Start: {0}", beforeTime.ToString()));

            // Массив для запоминания ячеек, у которых нужно сделать Wrap
            List<WrapCells> arrWrapCells = new List<WrapCells>();

            // fix
            string strCommonXml = "";
            
            if (active_sheet != null)
            {
                using (Microsoft.VisualBasic.FileIO.TextFieldParser oTextFieldParser = new Microsoft.VisualBasic.FileIO.TextFieldParser(file_path, System.Text.Encoding.GetEncoding(code_page)))
                {
                    int nRow = 1;

                    oTextFieldParser.Delimiters = new string[] { delimiter.ToString() };
                    while (true)
                    {
                        string[] arrParts = null;
                        try
                        {
                            arrParts = oTextFieldParser.ReadFields();
                        }
                        catch (Microsoft.VisualBasic.FileIO.MalformedLineException)
                        {
                            arrParts = new string[] { oTextFieldParser.ErrorLine };
                        }
                        if (arrParts == null)
                        {
                            break;
                        }
                        // fix
                        string strRow = string.Format("<row r=\"{0}\" xmlns=\"http://schemas.openxmlformats.org/spreadsheetml/2006/main\">", nRow);
                        int nColumn = 1;
                        foreach (string strValueTmp in arrParts)
                        {
                            string strValue = Unescape(strValueTmp);
                            if (strValue.Length != 0)
                            {
                                // fix
                                string strValType = RecognizeValueType(strValue);
                                Cell oCell = new Cell(nRow, nColumn);
                                CellRange oCellRange = new CellRange(oCell);

                                strValue = stripNonValidXMLCharacters(strValue);
                                string strConvertValue = ConvertValue(strValue, strValType);

                                if (strValType == "s")
                                    strConvertValue = wb.RegisterSharedString(strValue.ToString(), oCellRange.NumCells).ToString();

                                string strItem = string.Format("<c r=\"{0}\" t=\"{1}\"><v>{2}</v></c>", oCell.ToString(), strValType, strConvertValue);
                                strRow += strItem;

                                if (-1 != strValue.IndexOf("\n"))
                                {
                                    // Нужно применить Wrap
                                    WrapCells oWrapCells;
                                    oWrapCells.m_nRow = nRow;
                                    oWrapCells.m_nCol = nColumn;
                                    arrWrapCells.Add(oWrapCells);
                                }
                            }
                            ++nColumn;
                        }

                        ++nRow;

                        // fix
                        strRow += "</row>";
                        strCommonXml += strRow;
                    }
                }
            }

            active_sheet.SheetData.InnerXml = strCommonXml;

            // Выставляем Wrap для нужных ячеек (у которых больше одной строки)
            foreach (WrapCells oWrapCells in arrWrapCells)
            {
                active_sheet.Cell(oWrapCells.m_nRow, oWrapCells.m_nCol).Format.Alignment.WrapText = true;
            }

            //Console.WriteLine(string.Format("Stop: {0}", DateTime.Now.ToString()));
            //TimeSpan workTime = DateTime.Now - beforeTime;
            //Console.WriteLine(workTime.ToString());
        }
        //http://forums.asp.net/t/1483793.aspx
        static internal String stripNonValidXMLCharacters(string textIn)
        {
            StringBuilder textOut = new StringBuilder(); // Used to hold the output.
            char current; // Used to reference the current character.


            if (textIn == null || textIn == string.Empty) return string.Empty; // vacancy test.
            for (int i = 0; i < textIn.Length; i++)
            {
                current = textIn[i];


                if ((current == 0x9 || current == 0xA || current == 0xD) ||
                    ((current >= 0x20) && (current <= 0xD7FF)) ||
                    ((current >= 0xE000) && (current <= 0xFFFD)) ||
                    ((current >= 0x10000) && (current <= 0x10FFFF)))
                {
                    textOut.Append(current);
                }
            }
            return textOut.ToString();
        }
        static private string Unescape(string value_from_file)
        {
            if (value_from_file.StartsWith("\"") && value_from_file.EndsWith("\""))
            {
                return value_from_file.Substring(1, value_from_file.Length - 2).Replace("\"\"", "\"");
            }
            return value_from_file;
        }

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
        private static string ConvertValue(object value, string value_type)
        {
            string value_set = value.ToString();

            if (value_type == "str")
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

            return value_set;
        }
        private static string HtmlEncode(string value)
        {
            string val = value;

            if (!string.IsNullOrEmpty(value))
                val = System.Web.HttpUtility.HtmlEncode(value);

            return val;
        }
    }
}
