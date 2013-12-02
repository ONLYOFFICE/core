using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncCeiling : PtgFuncMathTwoArgs
    {
        public override DataObjects.IDataObject DoMath(double number, double significance)
        {
            if (significance == 0)
            {
                return new DataObjects.ObjectNumber(0);
            }
            if (Math.Sign(number) != Math.Sign(significance) && number != 0)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            if (Double.IsInfinity(number / significance))
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            return new DataObjects.ObjectNumber(Precision.Ceiling(number, significance));
        }

        public override string ToString() { return "CEILING"; }
        public override string ArgumentsTip { get { return "CEILING(number; significance)"; } }
    }
}
