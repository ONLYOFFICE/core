using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an unknown function is found
    /// </summary>
    public class UnknownFunctionError : FormulaFormatError
    {
        internal UnknownFunctionError(string func_name, string formula, int error_position)
            : base("Unknown function \"" + func_name + "\" found", formula, error_position)
        {
        }
    }
}
