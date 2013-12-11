using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgSub : BinOperatorPtg
    {
        private static Regex rx_minus = new Regex("\\G *- *");
        public static bool Extract(string formula, ref int start_pos, bool operand_expected)
        {
            if (!operand_expected)
            {
                Match match = rx_minus.Match(formula, start_pos);
                if (match.Success)
                {
                    start_pos += match.Length;
                    return true;
                }
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.SubCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.numeric_type; } }

        public override string ToString()
        {
            return "-";
        }

        public override int Priority { get { return 25; } }
    }
}
