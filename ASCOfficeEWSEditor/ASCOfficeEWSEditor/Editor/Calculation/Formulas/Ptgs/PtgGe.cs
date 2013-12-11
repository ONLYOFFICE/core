using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgGe : BinOperatorPtg
    {
        private static Regex rx_lt = new Regex("\\G *>= *");
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

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            DataObjects.IDataObject res_lt = base.Calculate(ref ptgs_enum, PreferredResultType.boolean_type);

            DataObjects.ObjectBoolean res_bool = res_lt as DataObjects.ObjectBoolean;
            if (res_bool != null)
            {
                return new DataObjects.ObjectBoolean(!res_bool.Value).ConvertIfPossible(result_type);
            }
            return res_lt.ConvertIfPossible(result_type);
        }

        public override BinOpCalc.IFullBinaryOpDispatcher Calculator { get { return BinOpCalc.Arithmetic.LtCalculator; } }

        public override PreferredResultType OperandsPreferredType { get { return PreferredResultType.value_type; } }

        public override string ToString()
        {
            return ">=";
        }

        public override int Priority { get { return 35; } }
    }
}
