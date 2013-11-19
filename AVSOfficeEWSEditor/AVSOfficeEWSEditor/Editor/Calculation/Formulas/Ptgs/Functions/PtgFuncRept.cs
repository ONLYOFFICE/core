using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncRept : PtgFuncFixed
    {
        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            DataObjects.IDataObject text_arg = null;
            DataObjects.IDataObject number_of_times_arg = null;
            int argument_index = 0;
            foreach (DataObjects.IDataObject arg in argv)
            {
                switch (++argument_index)
                {
                    case 1:
                        text_arg = arg.ConvertIfPossible(PreferredResultType.string_type);
                        break;
                    case 2:
                        number_of_times_arg = arg.ConvertIfPossible(PreferredResultType.numeric_type);
                        break;
                }
            }
            if (argument_index != ParametersNum)
            {
                throw new Events.ReportableException(new Events.InternalError("Wrong parameters number to " + ToString() + " function (" + ParametersNum + " expected)"));
            }

            if (text_arg is DataObjects.ObjectError)
            {
                return text_arg;
            }
            if(number_of_times_arg is DataObjects.ObjectError)
            {
                return number_of_times_arg;
            }

            DataObjects.ObjectString text = text_arg as DataObjects.ObjectString;
            DataObjects.ObjectNumber number_of_times = number_of_times_arg as DataObjects.ObjectNumber;
            if (text == null || number_of_times == null)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
            int num = (int)number_of_times.Value;

            if (number_of_times.Value < 0 || text.Value.Length * num > 32767)
            {
                return new DataObjects.ObjectError(ErrorValue.wrong_value_type);
            }
            
            string res_line = "";
            for(int i = 0; i < num; ++i)
            {
                res_line += text.Value;
            }

            return new DataObjects.ObjectString(res_line).ConvertIfPossible(result_type);
        }

        public override string ToString() { return "REPT"; }
        public override int ParametersNum { get { return 2; } }
        public override int ParametersNumMin { get { return 2; } }
        public override int ParametersNumMax { get { return 2; } }
        public override string ArgumentsTip { get { return "REPT(text; number_of_times)"; } }
    }
}
