using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncConcatenate : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject text_result = new DataObjects.ObjectString("");
            foreach (DataObjects.IDataObject arg in argv)
            {
                text_result = text_result.PerformBinaryOperation(arg.ConvertIfPossible(PreferredResultType.value_type), BinOpCalc.Arithmetic.ConcatCalculator);
            }

            return text_result.ConvertIfPossible(result_type);
        }

        public override string ToString() { return "CONCATENATE"; }
        public override string ArgumentsTip { get { return "CONCATENATE(text1; text2; ...)"; } }
    }
}
