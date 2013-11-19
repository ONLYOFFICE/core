using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncOr : PtgFuncVar
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.ObjectBoolean and_result = null;
            foreach (DataObjects.IDataObject arg in argv)
            {
                DataObjects.ObjectReference range = arg as DataObjects.ObjectReference;
                if(range != null)
                {
                    foreach (DataObjects.IDataObject arg_referenced in range)
                    {
                        if(arg_referenced is DataObjects.ObjectError)
                        {
                           return arg_referenced.ConvertIfPossible(result_type);;
                        }
                        else if(arg_referenced is DataObjects.ObjectString || arg_referenced is DataObjects.ObjectEmpty)
                        {
                            continue;
                        }
                        DataObjects.ObjectBoolean arg_referenced_bool = arg_referenced as DataObjects.ObjectBoolean;
                        if (arg_referenced_bool == null)
                        {
                            arg_referenced_bool = arg_referenced.ConvertIfPossible(PreferredResultType.boolean_type) as DataObjects.ObjectBoolean;
                            if (arg_referenced_bool == null)
                            {
                                continue;
                            }
                        }

                        if(and_result == null)
                        {
                            and_result = arg_referenced_bool;
                        }
                        else
                        {
                            and_result = new DataObjects.ObjectBoolean(and_result.Value || arg_referenced_bool.Value);
                        }
                    }
                }
                else // If non-reference
                {
                    DataObjects.IDataObject arg_converted = arg.ConvertIfPossible(PreferredResultType.boolean_type);
                    if(arg_converted is DataObjects.ObjectError)
                    {
                       return arg.ConvertIfPossible(result_type);;
                    }
                    else if(arg_converted is DataObjects.ObjectString)
                    {
                       return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
                    }
                    DataObjects.ObjectBoolean arg_boolean = arg_converted as DataObjects.ObjectBoolean;
                    if (arg_boolean != null)
                    {
                        if (and_result == null)
                        {
                            and_result = arg_boolean;
                        }
                        else
                        {
                            and_result = new DataObjects.ObjectBoolean(and_result.Value || arg_boolean.Value);
                        }
                    }
                }
            }
            if (and_result == null)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
            return and_result.ConvertIfPossible(result_type);
        }

        public override string ToString() { return "OR"; }
        public override string ArgumentsTip { get { return "OR(logical1; logical2; ...)"; } }
    }
}
