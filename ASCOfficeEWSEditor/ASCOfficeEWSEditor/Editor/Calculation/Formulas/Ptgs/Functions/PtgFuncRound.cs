using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncRound : PtgFuncMathTwoArgs
    {
        public override DataObjects.IDataObject DoMath(double number, double num_digits)
        {
            if(num_digits > Precision.ExcelMaxExponent)
            {
                if (Math.Abs(number) < 1 || num_digits < 1e10) // The values are obtained experimentally
                {
                    return new DataObjects.ObjectNumber(number);
                }
                return new DataObjects.ObjectNumber(0);
            }
            else if (num_digits < Precision.ExcelMinExponent)
            {
                if (Math.Abs(number) < 0.01) // The values are obtained experimentally
                {
                    return new DataObjects.ObjectNumber(number);
                }
                return new DataObjects.ObjectNumber(0);
            }

            double significance = Precision.SignZeroPositive(number) * Math.Pow(10, -Math.Truncate(num_digits));

            number += significance / 2;

            if (Double.IsInfinity(number / significance))
            {
                return new DataObjects.ObjectNumber(number);
            }
            return new DataObjects.ObjectNumber(Precision.Floor(number, significance));
        }

        public override string ToString() { return "ROUND"; }
        public override string ArgumentsTip { get{ return "ROUND(number; num_digits)"; } }
    }
}
