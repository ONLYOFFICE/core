using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgPower : BinOperatorPtg
    {
        private static Regex rx_pow = new Regex("\\G *\\^ *");
        public static bool Extract(string formula, ref int start_pos)
        {
            Match match = rx_pow.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.PowerCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.numeric_type; } }

        public override string ToString()
        {
            return "^";
        }

        public override int Priority { get { return 15; } }
    }
}
