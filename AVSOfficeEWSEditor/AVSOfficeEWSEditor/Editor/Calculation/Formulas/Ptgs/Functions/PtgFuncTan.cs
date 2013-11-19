using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncTan : PtgFuncMathOneArg
    {
        public override DataObjects.IDataObject DoMath(double num_value)
        {
            return new DataObjects.ObjectNumber(Math.Tan(num_value));
        }

        public override string ToString() { return "TAN"; }
    }
}
