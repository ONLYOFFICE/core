using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncOdd : PtgFuncMathOneArg
    {
        public override DataObjects.IDataObject DoMath(double num_value)
        {
            return new DataObjects.ObjectNumber(num_value == 0 ? 1 : (Math.Ceiling((Math.Abs(num_value) - 1) / 2) * 2 + 1) * Math.Sign(num_value));
        }

        public override string ToString() { return "ODD"; }
    }
}
