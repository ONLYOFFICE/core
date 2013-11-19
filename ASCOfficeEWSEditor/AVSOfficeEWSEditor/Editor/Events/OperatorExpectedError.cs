using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an operand has been found where an operator is expected
    /// </summary>
    public class OperatorExpectedError : FormulaFormatError
    {
        internal OperatorExpectedError(string formula, int error_position)
            : base("Operator expected, but an operand has been found", formula, error_position)
        {
        }
    }
}
