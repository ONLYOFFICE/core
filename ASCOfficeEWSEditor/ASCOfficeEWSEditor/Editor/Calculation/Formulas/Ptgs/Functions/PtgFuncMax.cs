using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncMax : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.ObjectNumber max = new DataObjects.ObjectNumber(double.MinValue);
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.ObjectReference range = arg as DataObjects.ObjectReference;
                if(range != null)
                {
                    foreach (DataObjects.IDataObject arg_referenced in range)
                    {
                        DataObjects.ObjectNumber arg_referenced_num = arg_referenced as DataObjects.ObjectNumber;
                        if (arg_referenced_num != null)
                        {
                            if (arg_referenced_num.Value > max.Value)
                            {
                                max = arg_referenced_num;
                            }
                        }
                        else if(arg_referenced is DataObjects.ObjectError)
                        {
                           return arg_referenced.ConvertIfPossible(result_type);;
                        }
                    }
                }
                else // If non-reference
                {
                    DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                    if(arg_converted is DataObjects.ObjectError)
                    {
                       return arg.ConvertIfPossible(result_type);;
                    }
                    else if(arg_converted is DataObjects.ObjectString)
                    {
                       return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                    }
                    DataObjects.ObjectNumber arg_numeric = arg_converted as DataObjects.ObjectNumber;
                    if(arg_numeric != null)
                    {
                        if (arg_numeric.Value > max.Value)
                        {
                            max = arg_numeric;
                        }
                    }
                }
            }
            if (max.Value == double.MinValue)
            {
                max = new DataObjects.ObjectNumber(0);
            }
            return max.ConvertIfPossible(result_type);
        }

        public override string ToString() { return "MAX"; }
        public override string ArgumentsTip { get { return "MAX(number1; number2; ...)"; } }
    }
}
