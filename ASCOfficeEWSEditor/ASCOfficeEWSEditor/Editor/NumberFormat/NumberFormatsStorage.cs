using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.NumberFormat
{
    /// <summary>
    /// Internal number formats storage
    /// </summary>
    public class NumberFormatsStorage
    {
        internal NumberFormatsStorage()
        {
            FormatStrings = new SortedList<int, string>();
            FillIn();
        }

        private void FillIn()
        {
            FormatStrings.Add(0, "General");
            FormatStrings.Add(1, "0");
            FormatStrings.Add(2, "0.00");
            FormatStrings.Add(3, "#,##0");
            FormatStrings.Add(4, "#,##0.00");
            FormatStrings.Add(9, "0%");
            FormatStrings.Add(10, "0.00%");
            FormatStrings.Add(11, "0.00E+00");
            FormatStrings.Add(12, "# ?/?");
            FormatStrings.Add(13, "# ??/??");
            FormatStrings.Add(14, "mm-dd-yy");
            FormatStrings.Add(15, "d-mmm-yy");
            FormatStrings.Add(16, "d-mmm");
            FormatStrings.Add(17, "mmm-yy");
            FormatStrings.Add(18, "h:mm AM/PM");
            FormatStrings.Add(19, "h:mm:ss AM/PM");
            FormatStrings.Add(20, "h:mm");
            FormatStrings.Add(21, "h:mm:ss");
            FormatStrings.Add(22, "m/d/yy h:mm");
            FormatStrings.Add(37, "#,##0 ;(#,##0)");
            FormatStrings.Add(38, "#,##0 ;[Red](#,##0)");
            FormatStrings.Add(39, "#,##0.00;(#,##0.00)");
            FormatStrings.Add(40, "#,##0.00;[Red](#,##0.00)");
            FormatStrings.Add(45, "mm:ss");
            FormatStrings.Add(46, "[h]:mm:ss");
            FormatStrings.Add(47, "mmss.0");
            FormatStrings.Add(48, "##0.0E+0");
            FormatStrings.Add(49, "@");
        }

        internal int GetIdByCode(string number_format)
        {
            foreach (KeyValuePair<int, string> format in FormatStrings)
            {
                if(format.Value == number_format)
                {
                    return format.Key;
                }
            }
            return -1;
        }

        /// <summary>
        /// List of all standard number format strings
        /// </summary>
        public SortedList<int, string> FormatStrings { get; private set; }
    }
}
