using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectBoolean : IDataObject
    {
        public ObjectBoolean(bool value)
        {
            Value = value;
        }

        public ObjectBoolean(string value)
        {
            Value = XMLBoolean.ParseOrDefault(value, false);
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
                case PreferredResultType.numeric_type:
                    return new ObjectNumber(Value ? 1.0 : 0.0);

                case PreferredResultType.string_type:
                    return new ObjectString(ToString());

                default:
                    return this;
            }
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other)
        {
            ObjectBoolean other_bool = other as ObjectBoolean;
            if (other_bool != null)
            {
                return Value.CompareTo(other_bool.Value);
            }
            if (other is ObjectNumber || other is ObjectString)
            {
                return 1;
            }
            return -1; // Less than all other types
        }

        #endregion

        public override string ToString()
        {
            return Value ? "TRUE" : "FALSE";
        }

        public bool Value { get; private set; }
        public object UserValue { get { return Value; } }
        public string Type { get { return "b"; } }

    }
}
