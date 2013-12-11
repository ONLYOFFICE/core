using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs.Functions
{
    internal class PtgFuncUnsupported : PtgFuncVar
    {
        public PtgFuncUnsupported(string func_name)
        {
            this.func_name = func_name;
        }

        public override DataObjects.IDataObject Calculate(LinkedList<DataObjects.IDataObject> argv, PreferredResultType result_type)
        {
            return new DataObjects.ObjectError(ErrorValue.unsupported_function);
        }

        public override string ToString() { return func_name; }
        public override string ArgumentsTip { get { return func_name + "(...)"; } }

        private string func_name;
    }
}
