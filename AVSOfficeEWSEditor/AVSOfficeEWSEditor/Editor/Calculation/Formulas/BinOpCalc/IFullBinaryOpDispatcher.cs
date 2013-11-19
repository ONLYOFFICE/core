using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.BinOpCalc
{
    internal interface IFullBinaryOpDispatcher
    {
        #region DataObjects.ObjectNumber
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num1, DataObjects.ObjectNumber arg_num2);
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectBoolean arg_bool);
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectError arg_err);
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectString arg_str);
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectReference arg_range);
        DataObjects.IDataObject Calc(DataObjects.ObjectNumber arg_num, DataObjects.ObjectEmpty arg_empty);
        #endregion
        #region DataObjects.ObjectBoolean
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectNumber arg_num);
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool1, DataObjects.ObjectBoolean arg_bool2);
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectError arg_err);
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectString arg_str);
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectReference arg_range);
        DataObjects.IDataObject Calc(DataObjects.ObjectBoolean arg_bool, DataObjects.ObjectEmpty arg_empty);
        #endregion
        #region DataObjects.ObjectError
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectNumber arg_num);
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectBoolean arg_bool);
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err1, DataObjects.ObjectError arg_err2);
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectString arg_str);
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectReference arg_range);
        DataObjects.IDataObject Calc(DataObjects.ObjectError arg_err, DataObjects.ObjectEmpty arg_empty);
       #endregion
        #region DataObjects.ObjectString
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectNumber arg_num);
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectBoolean arg_bool);
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectError arg_err);
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str1, DataObjects.ObjectString arg_str2);
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectReference arg_range);
        DataObjects.IDataObject Calc(DataObjects.ObjectString arg_str, DataObjects.ObjectEmpty arg_empty);
        #endregion
        #region ObjectReference
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectNumber arg_num);
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectBoolean arg_bool);
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectError arg_err);
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range, DataObjects.ObjectString arg_str);
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectReference arg_range2);
        DataObjects.IDataObject Calc(DataObjects.ObjectReference arg_range1, DataObjects.ObjectEmpty arg_empty);
        #endregion
        #region ObjectEmpty
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectNumber arg_num);
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectBoolean arg_bool);
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectError arg_err);
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectString arg_str);
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty, DataObjects.ObjectReference arg_range);
        DataObjects.IDataObject Calc(DataObjects.ObjectEmpty arg_empty1, DataObjects.ObjectEmpty arg_empty2);
        #endregion
    }
}
