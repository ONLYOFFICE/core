using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgLt : BinOperatorPtg
    {
        private static Regex rx_lt = new Regex("\\G *< *");
        public static bool Extract(string formula, ref int start_pos)
        {
            Match match = rx_lt.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.LtCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.value_type; } }

        public override string ToString()
        {
            return "<";
        }

        public override int Priority { get { return 35; } }
    }
}
