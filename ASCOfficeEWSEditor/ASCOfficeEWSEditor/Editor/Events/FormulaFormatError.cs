using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// Issued when a user entered a wrong formula or a wrong formula is contained in the loading document
    /// </summary>
    public class FormulaFormatError : ErrorDescription
    {
        internal FormulaFormatError(string message, string formula, int error_position)
        {
            Formula = formula;
            Position = error_position;
            error_message = message;
        }

        internal FormulaFormatError(string message, string formula)
            : this(message, formula, -1)
        {
        }

        internal FormulaFormatError(string message)
            : this(message, null, -1)
        {
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text
        {
            get
            {
                string text = error_message;
                if (Formula != null && Formula.Length != 0)
                {
                    text += " in formula \"" + Formula + "\"";
                    if (Position != -1)
                    {
                        text += " at position " + Position;
                    }
                }
                return text;
            }
        }

        /// <summary>
        /// Formula entered by a user
        /// </summary>
        public string Formula { get; set; }

        /// <summary>
        /// Position of the erroneous symbols in Formula
        /// </summary>
        public int Position { get; set; }

        private string error_message;
    }
}
