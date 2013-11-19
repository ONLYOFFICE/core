using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgNum : OperandPtg
    {
        public PtgNum(string value)
        {
            this.value = new DataObjects.ObjectNumber(value);
        }
        private static Regex rx_num = new Regex("\\G[+-]?\\d*(\\d|\\.)\\d*([eE][+-]?\\d+)?");
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
            return value.ConvertIfPossible(result_type);
        }

        public override string ToString()
        {
            return value.ToString();
        }

        private DataObjects.ObjectNumber value;
    }
}
