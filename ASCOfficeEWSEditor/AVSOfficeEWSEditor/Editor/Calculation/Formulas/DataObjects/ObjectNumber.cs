using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectNumber : IDataObject
    {
        public ObjectNumber(double value)
        {
            Value = value;
        }

        public ObjectNumber(string value)
        {
            Value = XMLDouble.ParseOrDefault(value, 0);
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
                    return new ObjectBoolean(Value != 0.0);

                case PreferredResultType.string_type:
                    return new ObjectString(Value.ToString());
            }
            return this;
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other)
        {
            ObjectNumber other_num = other as ObjectNumber;
            if (other_num != null)
            {
                return Value.CompareTo(other_num.Value);
            }
            return -1; // Less than all other types
        }

        #endregion

        public override string ToString()
        {
            return XMLDouble.ToString(Value);
        }

        public double Value { get; private set; }
        public object UserValue { get { return Value; } }
        public string Type { get { return ""; } }

    }
}
