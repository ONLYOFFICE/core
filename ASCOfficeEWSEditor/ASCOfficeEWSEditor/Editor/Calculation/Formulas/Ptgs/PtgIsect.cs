using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgIsect : BinOperatorPtg
    {
        private static Regex rx_before_operators = new Regex("\\G *[,()]");
        private static Regex rx_space = new Regex("\\G ");

        public static bool IsNextPtg(string formula, int start_pos)
        {
            return !rx_before_operators.Match(formula, start_pos).Success && 
                    rx_space.Match(formula, start_pos).Success;
        }

        public static bool Extract(string formula, ref int start_pos)
        {
            if(IsNextPtg(formula, start_pos))
            {
                ++start_pos; // Only one symbol can be recognized
                return true;
            }
            return false;
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.IsectCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.reference_type; } }

        public override string ToString()
        {
            return " ";
        }

        public override int Priority { get { return 6; } }
    }
}
