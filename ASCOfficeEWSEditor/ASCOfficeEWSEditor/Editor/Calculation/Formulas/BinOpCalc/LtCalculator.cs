using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class LtCalculator : IFullBinaryOpDispatcher
    {
        #region IFullBinaryOpDispatcher Members

        #region DataObjects.ObjectNumber
        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num1, DataObjects.ObjectNumber arg_num2)
        {
            return new DataObjects.ObjectBoolean(arg_num1.Value < arg_num2.Value);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectBoolean arg_bool)
        {
            return new DataObjects.ObjectBoolean(true);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectBoolean(true);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectBoolean(arg_num.Value < 0);
        }
        #endregion

        #region DataObjects.ObjectBoolean
        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectBoolean(false);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool1, DataObjects.ObjectBoolean arg_bool2)
        {
            return new DataObjects.ObjectBoolean(!arg_bool1.Value && arg_bool2.Value);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectBoolean(false);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool,DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectBoolean(false);
        }
        #endregion

        #region DataObjects.ObjectError
        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectNumber arg_num)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectBoolean arg_bool)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err1, DataObjects.ObjectError arg_err2)
        {
            return arg_err1;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectString arg_str)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectReference arg_range)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectEmpty arg_empty)
        {
            return arg_err;
        }
        #endregion

        #region DataObjects.ObjectString
        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectBoolean(false);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectBoolean arg_bool)
        {
            return new DataObjects.ObjectBoolean(true);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str1, DataObjects.ObjectString arg_str2)
        {
            return new DataObjects.ObjectBoolean(string.Compare(arg_str1.Value, arg_str2.Value, true) < 0);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectBoolean(false);
        }
        #endregion

        #region ObjectReference
        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectBoolean arg_bool)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectError arg_err)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectEmpty arg_empty)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }
        #endregion

        #region DataObjects.ObjectEmpty
        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectNumber arg_num)
        {
            return new DataObjects.ObjectBoolean(0 < arg_num.Value);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectBoolean arg_bool)
        {
            return arg_bool;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectString arg_str)
        {
            return new DataObjects.ObjectBoolean(arg_str.Value.Length != 0);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty1, DataObjects.ObjectEmpty arg_empty2)
        {
            return new DataObjects.ObjectBoolean(false);
        }

        #endregion

        #endregion
    }
}
