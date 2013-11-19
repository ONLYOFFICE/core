using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal abstract class ReferenceOperatorsCalculator : IFullBinaryOpDispatcher
    {
        #region IFullBinaryOpDispatcher Members

        #region DataObjects.ObjectNumber
        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num1, DataObjects.ObjectNumber arg_num2)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectBoolean arg_bool)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectError arg_err)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectString arg_str)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectReference arg_range)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectEmpty arg_empty)
        {
            return ThrowWrongArgumentsType();
        }
        #endregion

        #region DataObjects.ObjectBoolean
        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectNumber arg_num)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool1, DataObjects.ObjectBoolean arg_bool2)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectError arg_err)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectString arg_str)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectReference arg_range)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectEmpty arg_empty)
        {
            return ThrowWrongArgumentsType();
        }
        #endregion

        #region DataObjects.ObjectError
        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectNumber arg_num)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectBoolean arg_bool)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err1, DataObjects.ObjectError arg_err2)
        {
            if(arg_err1.Value == ErrorValue.bad_reference && arg_err2.Value == ErrorValue.bad_reference)
            {
                return arg_err1;
            }
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectString arg_str)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectReference arg_range)
        {
            if (arg_err.Value == ErrorValue.bad_reference)
            {
                return arg_err;
            }
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectEmpty arg_empty)
        {
            return ThrowWrongArgumentsType();
        }
        #endregion

        #region DataObjects.ObjectString
        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectNumber arg_num)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectBoolean arg_bool)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectError arg_err)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str1, DataObjects.ObjectString arg_str2)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectReference arg_range)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectEmpty arg_empty)
        {
            return ThrowWrongArgumentsType();
        }
        #endregion

        #region DataObjects.ObjectReference
        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectNumber arg_num)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectBoolean arg_bool)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectError arg_err)
        {
            if (arg_err.Value == ErrorValue.bad_reference)
            {
                return arg_err;
            }
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectString arg_str)
        {
            return ThrowWrongArgumentsType();
        }

        // The main dispatcher. Shall be overridden in successors
        public abstract DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2);

        public DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectEmpty arg_empty)
        {
            return ThrowWrongArgumentsType();
        }
        #endregion

        #region DataObjects.ObjectEmpty
        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectNumber arg_num)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectBoolean arg_bool)
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectError arg_err) // Call inversed
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectString arg_str) // Call inversed
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectReference arg_range) // Call inversed
        {
            return ThrowWrongArgumentsType();
        }

        public DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty1, DataObjects.ObjectEmpty arg_empty2)
        {
            return ThrowWrongArgumentsType();
        }

        #endregion

        #endregion

        private DataObjects.IDataObject ThrowWrongArgumentsType()
        {
            throw new Events.ReportableException(new Events.FormulaFormatError("Wrong argument types of " + OperatorName + " operator"));
        }

        public abstract string OperatorName { get; }
    }
}
