using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when a problem with unknown reason is detected
    /// </summary>
    public class InternalError : ErrorDescription
    {
        internal InternalError(string message)
        {
            this.message = message;
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text { get { return message; } }

        private string message;
    }
}
