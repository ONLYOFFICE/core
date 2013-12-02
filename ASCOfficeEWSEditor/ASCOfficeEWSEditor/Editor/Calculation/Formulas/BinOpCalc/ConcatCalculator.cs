using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal class ConcatCalculator : IFullBinaryOpDispatcher
    {
        private DataObjects.IDataObject Concat(DataObjects.IDataObject elem1, DataObjects.IDataObject elem2)
        {
            return new DataObjects.ObjectString(elem1.ToString() + elem2.ToString());
        }
        private DataObjects.IDataObject Concat(DataObjects.ObjectString str, DataObjects.IDataObject elem2)
        {
            return new DataObjects.ObjectString(str.Value + elem2.ToString());
        }
        private DataObjects.IDataObject Concat(DataObjects.IDataObject elem1, DataObjects.ObjectString str)
        {
            return new DataObjects.ObjectString(elem1.ToString() + str.Value);
        }
        private DataObjects.IDataObject Concat(DataObjects.ObjectEmpty empty, DataObjects.IDataObject elem2)
        {
            return new DataObjects.ObjectString(elem2.ToString());
        }
        private DataObjects.IDataObject Concat(DataObjects.IDataObject elem1, DataObjects.ObjectEmpty empty)
        {
            return new DataObjects.ObjectString(elem1.ToString());
        }
        
        
        #region IFullBinaryOpDispatcher Members

        #region DataObjects.ObjectNumber
        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num1, DataObjects.ObjectNumber arg_num2)
        {
            return Concat(arg_num1, arg_num2);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectBoolean arg_bool)
        {
            return Concat(arg_num, arg_bool);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectString arg_str)
        {
            return Concat(arg_num, arg_str);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectEmpty arg_empty)
        {
            return Concat(arg_num, arg_empty);
        }
        #endregion

        #region DataObjects.ObjectBoolean
        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectNumber arg_num)
        {
            return Concat(arg_bool, arg_num);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool1, DataObjects.ObjectBoolean arg_bool2)
        {
            return Concat(arg_bool1, arg_bool2);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectString arg_str)
        {
            return Concat(arg_bool, arg_str);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectEmpty arg_empty)
        {
            return Concat(arg_bool, arg_empty);
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
            return Concat(arg_str, arg_num);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectBoolean arg_bool)
        {
            return Concat(arg_str, arg_bool);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str1, DataObjects.ObjectString arg_str2)
        {
            return new DataObjects.ObjectString(arg_str1.Value + arg_str2.Value);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectEmpty arg_empty)
        {
            return arg_str;
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
            return Concat(arg_empty, arg_num);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectBoolean arg_bool)
        {
            return Concat(arg_empty, arg_bool);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectError arg_err)
        {
            return arg_err;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectString arg_str)
        {
            return arg_str;
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectReference arg_range)
        {
            return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty1, DataObjects.ObjectEmpty arg_empty2)
        {
            return new DataObjects.ObjectString("");
        }

        #endregion

        #endregion
    }
}
