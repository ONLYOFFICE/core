using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an operator has been found where an operand is expected
    /// </summary>
    public class OperandExpectedError : FormulaFormatError
    {
        internal OperandExpectedError(string formula, int error_position)
            : base("Operand expected, but an operator has been found", formula, error_position)
        {
        }
    }
}
