using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncMRound : PtgFuncMathTwoArgs
    {
        public override DataObjects.IDataObject DoMath(double number, double multiple)
        {
            if (multiple == 0)
            {
                return new DataObjects.ObjectNumber(0);
            }
            if (Math.Sign(number) != Math.Sign(multiple) && number != 0)
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }

            number = Precision.CorrectPrecisionAfterMultiplyDivision(number + multiple / 2);

            if (Double.IsInfinity(number / multiple))
            {
                return new DataObjects.ObjectError(ErrorValue.not_numeric);
            }
            return new DataObjects.ObjectNumber(Precision.Floor(number, multiple));
        }

        public override string ToString() { return "MROUND"; }
        public override string ArgumentsTip { get { return "MROUND(number; multiple)"; } }
    }
}
