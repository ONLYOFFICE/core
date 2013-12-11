using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncType : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            if (argv.Count != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            DataObjects.IDataObject arg = argv.First.Value.ConvertIfPossible(PreferredResultType.value_type);

            if (arg is DataObjects.ObjectNumber || arg is DataObjects.ObjectEmpty)
            {
                return new DataObjects.ObjectNumber(1);
            }
            if (arg is DataObjects.ObjectString)
            {
                return new DataObjects.ObjectNumber(2);
            }
            if (arg is DataObjects.ObjectBoolean)
            {
                return new DataObjects.ObjectNumber(4);
            }
            if (arg is DataObjects.ObjectError)
            {
                return new DataObjects.ObjectNumber(16);
            }
/* TODO: unlock when PtgArray is implemented
            if (arg is PtgArray)
            {
                return new DataObjects.ObjectNumber(64);
            }
*/
            throw new Events.ReportableException(new Events.InternalError("Argument of unknown type passed as a parameter to TYPE(value)"));
        }

        public override string ToString() { return "TYPE"; }
        public override int ParametersNum { get { return 1; } }
        public override int ParametersNumMin { get { return 1; } }
        public override int ParametersNumMax { get { return 1; } }
        public override string ArgumentsTip { get { return "TYPE(value)"; } }
    }
}
