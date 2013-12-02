using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncEven : PtgFuncMathOneArg
    {
        public override DataObjects.IDataObject DoMath(double num_value)
        {
            return new DataObjects.ObjectNumber(Math.Ceiling(Math.Abs(num_value) / 2) * 2 * Math.Sign(num_value));
        }

        public override string ToString() { return "EVEN"; }
    }
}
