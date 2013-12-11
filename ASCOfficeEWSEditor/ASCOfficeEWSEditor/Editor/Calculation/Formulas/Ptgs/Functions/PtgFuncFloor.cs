using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncFloor : PtgFuncMathTwoArgs
    {
        public override DataObjects.IDataObject DoMath(double number, double significance)
        {
            if (significance == 0)
            {
                if (number != 0)
                {
                    return new DataObjects.ObjectError(ErrorValue.division_by_zero);
                }
                else
                {
                    return new DataObjects.ObjectNumber(0);
                }
            }
            if (Math.Sign(number) != Math.Sign(significance) && number != 0)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            if (Double.IsInfinity(number / significance))
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            return new DataObjects.ObjectNumber(Precision.Floor(number, significance));
        }

        public override string ToString() { return "FLOOR"; }
        public override string ArgumentsTip { get{ return "FLOOR(number; significance)"; } }
    }
}
