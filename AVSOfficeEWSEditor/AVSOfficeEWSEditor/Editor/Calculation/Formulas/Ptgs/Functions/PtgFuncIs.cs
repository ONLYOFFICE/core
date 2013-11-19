using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal abstract class PtgFuncIs : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            if (argv.Count != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }
            return new DataObjects.ObjectBoolean(Is(argv.First.Value));
        }

        public abstract bool Is(DataObjects.IDataObject arg);
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return ToString() + "(value)"; } }
    }

    internal class PtgFuncIsBlank : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectEmpty;
        }
        public override string ToString() { return "ISBLANK"; }
    }

    internal class PtgFuncIsErr : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            DataObjects.ObjectError error_arg = arg.ConvertIfPossible(PreferredResultType.value_type) as DataObjects.ObjectError;
            return error_arg != null && error_arg.Value != ErrorValue.not_available;
        }
        public override string ToString() { return "ISERR"; }
    }

    internal class PtgFuncIsError : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectError;
        }
        public override string ToString() { return "ISERROR"; }
    }

    internal class PtgFuncIsLogical : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectBoolean;
        }
        public override string ToString() { return "ISLOGICAL"; }
    }

    internal class PtgFuncIsNa : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            DataObjects.ObjectError error_arg = arg.ConvertIfPossible(PreferredResultType.value_type) as DataObjects.ObjectError;
            return error_arg != null && error_arg.Value == ErrorValue.not_available;
        }
        public override string ToString() { return "ISNA"; }
    }

    internal class PtgFuncIsNontext : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return !(arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectString);
        }
        public override string ToString() { return "ISNONTEXT"; }
    }

    internal class PtgFuncIsNumber : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectNumber;
        }
        public override string ToString() { return "ISNUMBER"; }
    }

    internal class PtgFuncIsRef : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg is DataObjects.ObjectReference;
        }
        public override string ToString() { return "ISREF"; }
    }

    internal class PtgFuncIsText : PtgFuncIs
    {
        public override bool Is(DataObjects.IDataObject arg)
        {
            return arg.ConvertIfPossible(PreferredResultType.value_type) is DataObjects.ObjectString;
        }
        public override string ToString() { return "ISTEXT"; }
    }

}
