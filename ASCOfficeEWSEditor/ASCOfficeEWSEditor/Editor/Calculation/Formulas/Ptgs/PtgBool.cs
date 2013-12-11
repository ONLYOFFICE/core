using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgBool : OperandPtg
    {
        public PtgBool(string value)
        {
            this.value = new DataObjects.ObjectBoolean(value);
        }
        private static Regex rx_num = new Regex("\\G(TRUE|FALSE)([-+*/^&%<=>: ;),]|$)", RegexOptions.IgnoreCase); // searched case insensitive
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                if (groups.Count >= 2)
                {
                    start_pos += groups[1].Length;
                    operand_str = groups[1].Value;
                    return true;
                }
            }
            operand_str = null;
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            return value.ConvertIfPossible(result_type);
        }

        public override string ToString()
        {
            return value.ToString();
        }

        private DataObjects.ObjectBoolean value;
    }
}
