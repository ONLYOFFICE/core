using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an unknown operand is found
    /// </summary>
    public class UnknownOperandError : FormulaFormatError
    {
        internal UnknownOperandError(string formula, int error_position)
            : base("Unknown operand found", formula, error_position)
        {
        }
    }
}
