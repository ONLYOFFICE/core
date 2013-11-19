using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal class PtgParen : Ptg
    {
        public PtgParen()
        {
            ParametersNum = 1; // Assume there should be at least one parameter
        }
        private static Regex rx_l_paren = new Regex("\\G *\\( *");
        public static bool Extract_Left(string formula, ref int start_pos)
        {
            Match match = rx_l_paren.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }
        private static Regex rx_r_paren = new Regex("\\G *\\)");
        public static bool Extract_Right(string formula, ref int start_pos)
        {
            Match match = rx_r_paren.Match(formula, start_pos);
            if (match.Success)
            {
                start_pos += match.Length;
                return true;
            }
            return false;
        }

        public override DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Empty parenthesis"));
            }
            return ptgs_enum.Current.Calculate(ref ptgs_enum, result_type);
        }

        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {
            if (!ptgs_enum.MoveNext())
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Empty parenthesis"));
            }
            string arg = ptgs_enum.Current.Assemble(ref ptgs_enum);

            return "(" + arg + ")";
        }

        public override string ToString()
        {
            return "(";
        }

        public int ParametersNum { get; private set; }

        public int ParametersNumMin { get; private set; }

        public int ParametersNumMax { get; private set; }

        public void DecrementParametersNum()
        {
            --ParametersNum;
        }
        public void IncrementParametersNum()
        {
            ++ParametersNum;
        }
    }
}
