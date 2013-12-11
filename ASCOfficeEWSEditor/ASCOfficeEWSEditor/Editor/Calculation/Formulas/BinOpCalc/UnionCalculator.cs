using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class UnionCalculator : ReferenceOperatorsCalculator
    {
        public override DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2)
        {
            return arg_range1.Union(arg_range2);
        }

        public override string OperatorName { get { return "union"; } }
    }
}
