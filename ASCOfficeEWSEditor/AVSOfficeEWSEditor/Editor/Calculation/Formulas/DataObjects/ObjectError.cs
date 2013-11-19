using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectError : IDataObject
    {
        public ObjectError(ErrorValue value)
        {
            Value = value;
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
            return this;
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other)
        {
            ObjectError other_err = other as ObjectError;
            if (other_err != null)
            {
                return Value.CompareTo(other_err.Value);
            }
            if(other is DataObjects.ObjectEmpty)
            {
                return -1;
            }
            return 1; // More than all other types
        }

        #endregion

        public override string ToString()
        {
            return Value.ToString();
        }

        public ErrorValue Value { get; private set; }
        public object UserValue { get { return Value; } }
        public string Type { get { return Value != ErrorValue.unsupported_function ? "e" : "str"; } }
    }
}
