using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class IsectCalculator : ReferenceOperatorsCalculator
    {
        public override DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2)
        {
            return arg_range1.Intersect(arg_range2);
        }

        public override string OperatorName { get { return "intersection"; } }
    }
}
