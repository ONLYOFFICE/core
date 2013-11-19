using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when parenthesis sequence in formula is broken
    /// </summary>
    public class WrongParenthesisSequenceError : FormulaFormatError
    {
        internal WrongParenthesisSequenceError(string formula, int error_position)
            : base("Wrong parenthesis sequence", formula, error_position)
        {
        }
    }
}
