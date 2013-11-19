using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectEmpty : IDataObject
    {
        public ObjectEmpty()
        {
        }

        #region IDataObject Members

        public IDataObject PerformBinaryOperation(IDataObject element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return element.PerformBinaryOperation(this, dispatcher);
        }

        public IDataObject PerformBinaryOperation(ObjectNumber element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectBoolean element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectError element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectString element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectReference element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectEmpty element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        #endregion

        #region ConvertIfPossible

        public IDataObject ConvertIfPossible(PreferredResultType result_type)
        {
            switch (result_type)
            {
                case PreferredResultType.boolean_type:
                    return new ObjectBoolean(false);

                case PreferredResultType.numeric_type:
                    return new ObjectNumber(0.0);

                case PreferredResultType.string_type:
                    return new ObjectString("");

                case PreferredResultType.value_type:
                case PreferredResultType.as_is:
                    return this;
            }
            return this;
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other)
        {
            if (other is ObjectEmpty)
            {
                return 0;
            }
            return 1;
        }

        #endregion

        public override string ToString()
        {
            return "empty cell";
        }

        public object UserValue { get{ return "";} }
        public string Type { get { return null; } }
    }
}
