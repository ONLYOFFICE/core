using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncSum : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject sum = new DataObjects.ObjectNumber(0);
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.ObjectReference range = arg as DataObjects.ObjectReference;
                if(range != null)
                {
                    foreach (DataObjects.IDataObject arg_referenced in range)
                    {
                        if (arg_referenced is DataObjects.ObjectNumber || arg_referenced is DataObjects.ObjectError)
                        {
                            sum = sum.PerformBinaryOperation(arg_referenced, BinOpCalc.Arithmetic.AddCalculator);
                        }
                    }
                }
                else // If non-reference
                {
                    sum = sum.PerformBinaryOperation(arg.ConvertIfPossible(PreferredResultType.numeric_type), BinOpCalc.Arithmetic.AddCalculator);
                }
            }

            return sum.ConvertIfPossible(result_type);
        }

        public override string ToString() { return "SUM"; }
        public override string ArgumentsTip { get { return "SUM(number1; number2; ...)"; } }
    }
}
