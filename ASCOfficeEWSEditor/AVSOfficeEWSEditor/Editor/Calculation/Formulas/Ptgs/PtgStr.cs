using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgStr : OperandPtg
    {
        public PtgStr(string value)
        {
            this.value = new DataObjects.ObjectString(value);
        }
        private static Regex rx_num = new Regex("\\G\"((\"\"|[^\"])*)\"");
        public static bool Extract(string formula, ref int start_pos, out string operand_str)
        {
            Match match = rx_num.Match(formula, start_pos);
            if (match.Success)
            {
                GroupCollection groups = match.Groups;
                if (groups.Count >= 2)
                {
                    start_pos += match.Length;
                    operand_str = groups[1].Value.Replace("\"\"", "\"");
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
            return "\"" + value.ToString() + "\"";
        }

        public DataObjects.ObjectString value;
    }


}
