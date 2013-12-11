using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgPercent : OperatorPtg
    {
        private static Regex rx_percent = new Regex("\\G *% *");
        public static bool Extract(string formula, ref int start_pos, bool operand_expected)
        {
            if (!operand_expected)
            {
                Match match = rx_percent.Match(formula, start_pos);
                if (match.Success)
                {
                    start_pos += match.Length;
                    return true;
                }
            }
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Argument of operator unary \"%\" was not found"));
            }
            DataObjects.IDataObject arg = ptgs_enum.Current.Calculate(ref ptgs_enum, PreferredResultType.numeric_type);

            return arg.PerformBinaryOperation((DataObjects.IDataObject)new DataObjects.ObjectNumber(100.0), BinOpCalc.Arithmetic.DivCalculator).ConvertIfPossible(result_type);
        }

        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Argument of operator unary \"%\" was not found"));
            }
            string arg = ptgs_enum.Current.Assemble(ref ptgs_enum);

            return arg + ToString();
        }

        public override string ToString()
        {
            return "%";
        }

        public override int Priority { get { return 11; } }
    }
}
