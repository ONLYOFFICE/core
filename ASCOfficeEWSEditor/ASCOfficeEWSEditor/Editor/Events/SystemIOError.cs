using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when an unknown operand is found
    /// </summary>
    public class SystemIOError : ErrorDescription
    {
        internal SystemIOError(System.SystemException io_exception)
        {
            this.io_exception = io_exception;
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text { get { return io_exception.Message; } }

        private System.SystemException io_exception;
    }
}
