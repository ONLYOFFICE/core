using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgGt : BinOperatorPtg
    {
        private static Regex rx_gt = new Regex("\\G *> *");
        public static bool Extract(string formula, ref int start_pos)
        {
            Match match = rx_gt.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.GtCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.value_type; } }

        public override string ToString()
        {
            return ">";
        }

        public override int Priority { get { return 35; } }
    }
}
