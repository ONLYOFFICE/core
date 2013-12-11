using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncMod : PtgFuncMathTwoArgs
    {
        // MOD(n, d) = n - d*INT(n/d)
        public override DataObjects.IDataObject DoMath(double number, double divisor)
        {
            if (divisor == 0)
            {
                return new DataObjects.ObjectError(ErrorValue.division_by_zero);
            }

            double quotient = number / divisor;

            if (quotient < 0 && quotient >= -1e-16)
            {
                return new DataObjects.ObjectNumber(0);
            }
            if (Double.IsInfinity(quotient))
            {
                return new DataObjects.ObjectNumber(number);
            }
            
            return new DataObjects.ObjectNumber(number - divisor * Math.Floor(quotient));
        }

        public override string ToString() { return "MOD"; }
        public override string ArgumentsTip { get { return "MOD(number; divisor)"; } }
    }
}
