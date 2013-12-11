using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncSqrt : PtgFuncMathOneArg
    {
        public override DataObjects.IDataObject DoMath(double num_value)
        {
            if (num_value < 0)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            return new DataObjects.ObjectNumber(Math.Sqrt(num_value));
        }

        public override string ToString() { return "SQRT"; }
    }
}
