using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas
{
    /// <summary>
    /// Formula format exception
    /// </summary>
    public abstract class FormulaFormatException : System.ArgumentException
    {
        internal FormulaFormatException(string message, string formula, int start_pos)
            : base(message + " in formula \"" + formula + "\" at position " + start_pos)
        {
        }
    }

    /// <summary>
    /// Thrown when an unknown operator found or operator/operand sequence is broken
    /// </summary>
    public class UnknownOperatorException : FormulaFormatException
    {
        internal UnknownOperatorException(string formula, int start_pos)
            : base("Unknown operator format", formula,  start_pos)
        {
        }
    }

    /// <summary>
    /// Thrown when an unknown operand found or operator/operand sequence is broken
    /// </summary>
    public class UnknownOperandException : FormulaFormatException
    {
        internal UnknownOperandException(string formula, int start_pos)
            : base("Unknown operand format", formula,  start_pos)
        {
        }
    }

    /// <summary>
    /// Thrown when parenthesis sequence is wrong
    /// </summary>
    public class WrongParenthesisSequence : FormulaFormatException
    {
        internal WrongParenthesisSequence(string formula, int start_pos)
            : base("Wrong parenthesis sequence", formula,  start_pos)
        {
        }
    }
}
