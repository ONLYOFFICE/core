using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgConcat : BinOperatorPtg
    {
        private static Regex rx_mul = new Regex("\\G *& *");
        public static bool Extract(string formula, ref int start_pos)
        {
            Match match = rx_mul.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.ConcatCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.value_type; } }

        public override string ToString()
        {
            return "&";
        }

        public override int Priority { get { return 30; } }
    }
}
