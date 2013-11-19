using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectString : IDataObject
    {
        public ObjectString(string value)
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
            string extracted_str_value;
            int start_pos = 0;
            if (result_type == PreferredResultType.boolean_type && Ptgs.PtgBool.Extract(Value, ref start_pos, out extracted_str_value))
            {
                if(start_pos == Value.Length)
                {
                    return new ObjectBoolean(extracted_str_value);
                }
            }
            else if (result_type == PreferredResultType.numeric_type && Ptgs.PtgNum.Extract(Value, ref start_pos, out extracted_str_value))
            {
                if (start_pos == Value.Length)
                {
                    return new ObjectNumber(extracted_str_value);
                }
            }
            return this;
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other)
        {
            ObjectString other_str = other as ObjectString;
            if (other_str != null)
            {
                return Value.CompareTo(other_str.Value);
            }
            if (other is ObjectNumber)
            {
                return 1;
            }
            return -1; // Less than all other types
        }

        #endregion

        public override string ToString()
        {
            return Value;
        }

        public string Value { get; private set; }
        public object UserValue { get { return Value; } }
        public string Type { get { return "str"; } }

    }


}
