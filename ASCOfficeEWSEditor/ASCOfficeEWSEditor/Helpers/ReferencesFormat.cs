using System;
using System.Collections.Generic;
using System.Text;
//using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Helpers
{
    internal class ReferencesFormat
    {
        /// <summary>
        /// Extracts 1-based column number from reference
        /// </summary>
        /// <param name="ref_str">Cell reference</param>
        /// <returns>1-based column number</returns>
        static internal int str2column(string ref_str)
        {
            int column = -1;

            int nLength = ref_str.Length;
            for (int nIndex = 0; nIndex < nLength; ++nIndex)
            {
                char symb = ref_str[nIndex];
                if (symb < 'A' || symb > 'Z')
                    break;

                column = (column + 1) * radixZA + (symb - 'A');
            }

            ++column; // makes it 1-based
            if (column > maxCol)
            {
                column = maxCol;
            }

            return column;
        }

        static internal int str2loc(string str, out int row, out bool row_rel, out int column, out bool col_rel)
        {
            col_rel = true;
            row_rel = true;

            column = -1;
            row = 0;

            int nLength = str.Length;
            int nIndex = 0;

            // Calculate column
            if (0 < nLength && '$' == str[nIndex])
            {
                ++nIndex;
                col_rel = false;
            }
            for (; nIndex < nLength; ++nIndex)
            {
                char symb = str[nIndex];
                if (symb < 'A' || symb > 'Z')
                    break;

                column = (column + 1) * radixZA + (symb - 'A');
            }
            ++column; // makes it 1-based
            if (column > maxCol)
            {
                column = maxCol;
            }

            // Calculate row
            if (nIndex < nLength && '$' == str[nIndex])
            {
                ++nIndex;
                row_rel = false;
            }
            for (; nIndex < nLength; ++nIndex)
            {
                char symb = str[nIndex];
                if ('0' > symb || '9' < symb)
                {
                    break;
                }
                row = row * 10 + (symb - '0');
            }
            if (row > maxRow)
            {
                row = maxRow;
            }

            return nIndex;
        }

        static internal bool str2ref(string ref_str, out int rowFirst, out bool rowFirstRelative, out int columnFirst, out bool columnFirstRelative,
                                                                out int rowLast, out bool rowLastRelative, out int columnLast, out bool columnLastRelative)
        {
            int nLength = ref_str.Length;
            int nIndex = str2loc(ref_str, out rowFirst, out rowFirstRelative, out columnFirst, out columnFirstRelative);

            if (nIndex >= nLength) // single cell
            {
                rowLast = rowFirst;
                rowLastRelative = rowFirstRelative;
                columnLast = columnFirst;
                columnLastRelative = columnFirstRelative;
                return true;
            }
            if (':' != ref_str[nIndex]) // Example: "A2-A3"
            {
                rowLast = 0;
                rowLastRelative = true;
                columnLast = 0;
                columnLastRelative = true;
                return false;
            }
            ++nIndex; // skip ':'
            str2loc(ref_str.Substring(nIndex), out rowLast, out rowLastRelative, out columnLast, out columnLastRelative);
	
            if(rowFirst != 0 && rowLast != 0 && columnFirst != 0 && columnLast != 0)
            {
                return true;
            }
            else if (rowFirst == 0 && rowLast == 0 && columnFirst != 0 && columnLast != 0) // no row specified - means whole column or range of columns
            {
                rowFirst = 1;
                rowLast = maxRow;
                return true;
            }
            else if (columnFirst == 0 && columnLast == 0 && rowFirst != 0 && rowLast != 0) // no column specified - means whole row or range of rows
            {
                columnFirst = 1;
                columnLast = maxCol;
                return true;
            }
            return false;
        }


        /// <summary>
        /// Convert 1-based column number to string form
        /// </summary>
        /// <param name="column">column is 1-based</param>
        /// <param name="columnRelative"></param>
        /// <returns></returns>
        static internal string column2str(int column, bool columnRelative)
        {
	        string ret_val = "";
            while(0 != column)
	        {
		        char sym = (char)((column - 1) % radixZA + 'A');
		        ret_val = sym + ret_val;
                column = (column - 1) / radixZA;
	        }
            if (!columnRelative)
            {
                ret_val = '$' + ret_val;
            }
            return ret_val;
        }

        /// <summary>
        /// Convert 1-based row number to string form
        /// </summary>
        /// <param name="row">row is 1-based</param>
        /// <param name="rowRelative"></param>
        /// <returns></returns>
        static internal string row2str(int row, bool rowRelative)
        {
            if (!rowRelative)
            {
                return '$' + row.ToString();
            }
            return row.ToString();
        }

        /// <summary>
        /// All input parameters are 1-based
        /// </summary>
        static internal string range2str(int rowFirst, bool rowFirstRelative, int columnFirst, bool columnFirstRelative,
                                        int rowLast, bool rowLastRelative, int columnLast, bool columnLastRelative)
        {
            const bool useShortForm = true;
	        if (1 == rowFirst && maxRow == rowLast) // whole column or range of columns
	        {
                if (useShortForm) // TODO: mind the relativity
		        {
			        return column2str(columnFirst, columnFirstRelative) + ':' + column2str(columnLast, columnFirstRelative);
		        }
	        }
	        if (1 == columnFirst && maxCol == columnLast) // whole row or range of rows
	        {
                if (useShortForm) // TODO: mind the relativity
		        {
			        return row2str(rowFirst, rowFirstRelative) + ':' + row2str(rowLast, rowLastRelative);
		        }
	        }
	        if (columnLast == columnFirst && rowFirst == rowLast) // single cell
	        {
		        return column2str(columnFirst, columnFirstRelative) + row2str(rowFirst, rowFirstRelative);
	        }
	        return column2str(columnFirst, columnFirstRelative) + row2str(rowFirst, rowFirstRelative) + ':' + 
                    column2str(columnLast, columnLastRelative) + row2str(rowLast, rowLastRelative);
        }

        /// <summary>
        /// All input parameters are 1-based
        /// </summary>
        static internal string ref2str(int row, bool rowRelative, int column, bool columnRelative)
        {
	        return column2str(column, columnRelative) + row2str(row, rowRelative);
        }

        static internal Editor.Cells.CellRange maxRange()
        {
            return new Editor.Cells.CellRange(1, 1, Helpers.ReferencesFormat.maxRow, Helpers.ReferencesFormat.maxCol);
        }

        const int radixZA = 'Z' - 'A' + 1;
        public const int maxRow = 1048576;
        public  const int maxCol = 16384;
    }
}
