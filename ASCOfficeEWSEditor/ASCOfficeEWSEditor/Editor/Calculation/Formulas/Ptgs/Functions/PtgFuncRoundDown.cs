using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncRoundDown : PtgFuncMathTwoArgs
    {
        public override DataObjects.IDataObject DoMath(double number, double num_digits)
        {
            if(num_digits > Precision.ExcelMaxExponent)
            {
                if (Math.Abs(number) >= 1e-100 || num_digits <= 98303) // The values are obtained experimentally
                {
                    return new DataObjects.ObjectNumber(number);
                }
                return new DataObjects.ObjectNumber(0);
            }
            else if (num_digits < Precision.ExcelMinExponent)
            {
                if (Math.Abs(number) >= 1e100) // The values are obtained experimentally
                {
                    return new DataObjects.ObjectNumber(number);
                }
                return new DataObjects.ObjectNumber(0);
            }

            double significance = Math.Pow(10, -Math.Truncate(num_digits));

            if (Double.IsInfinity(number / significance))
            {
                return new DataObjects.ObjectNumber(number);
            }
            return new DataObjects.ObjectNumber(Precision.Floor(number, significance));
        }

        public override string ToString() { return "ROUNDDOWN"; }
        public override string ArgumentsTip { get{ return "ROUNDDOWN(number; num_digits)"; } }
    }
}
