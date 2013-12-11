using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncExact : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject text_1_arg = null;
            DataObjects.IDataObject text_2_arg = null;
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch (++argument_index)
                {
                    case 1:
                        text_1_arg = arg.ConvertIfPossible(PreferredResultType.string_type);
                        break;
                    case 2:
                        text_2_arg = arg.ConvertIfPossible(PreferredResultType.string_type);
                        break;
                }
            }
            if (argument_index != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            if (text_1_arg is DataObjects.ObjectError)
            {
                return text_1_arg;
            }
            if (text_2_arg is DataObjects.ObjectError)
            {
                return text_2_arg;
            }

            DataObjects.ObjectString text1 = text_1_arg as DataObjects.ObjectString;
            DataObjects.ObjectString text2 = text_2_arg as DataObjects.ObjectString;
            if (text1 == null || text2 == null)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }

            return new DataObjects.ObjectBoolean(text1.Value == text2.Value).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "EXACT"; }
        public override int ParametersNum { get { return 2; } }
        public override int ParametersNumMin { get { return 2; } }
        public override int ParametersNumMax { get { return 2; } }
        public override string ArgumentsTip { get { return "EXACT(text1; text2)"; } }
    }
}
