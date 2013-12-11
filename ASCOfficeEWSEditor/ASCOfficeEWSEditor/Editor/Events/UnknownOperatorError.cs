using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an unknown operator is found
    /// </summary>
    public class UnknownOperatorError : FormulaFormatError
    {
        internal UnknownOperatorError(string formula, int error_position)
            : base("Unknown operator found", formula, error_position)
        {
        }
    }
}
