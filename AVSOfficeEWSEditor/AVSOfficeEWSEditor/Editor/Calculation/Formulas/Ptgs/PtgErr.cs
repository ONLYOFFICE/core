using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgErr : OperandPtg
    {
        public PtgErr(string value)
        {
            this.value = new DataObjects.ObjectError(ErrorValue.Parse(value));
        }

        public PtgErr(ErrorValue value)
        {
            this.value = new DataObjects.ObjectError(value);
        }
        private static Regex rx_num = new Regex("\\G(#NULL!|#DIV/0!|#VALUE!|#REF!|#NAME\\?|#NUM!|#N/A)", RegexOptions.IgnoreCase); // searched case insensitive
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                operand_str = match.Value;
                return true;
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            return value;
        }

        public override string ToString()
        {
            return value.ToString();
        }

        private DataObjects.ObjectError value;
    }
}
