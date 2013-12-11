using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal interface IDataObject : IComparable<IDataObject>
    {
        IDataObject PerformBinaryOperation(IDataObject element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectNumber element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectBoolean element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectError element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectString element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectReference element, BinOpCalc.IFullBinaryOpDispatcher visitor);
        IDataObject PerformBinaryOperation(ObjectEmpty element, BinOpCalc.IFullBinaryOpDispatcher visitor);

        IDataObject ConvertIfPossible(PreferredResultType result_type);

        object UserValue { get; }
        string Type { get; }
    }
}
