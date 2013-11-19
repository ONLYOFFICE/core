using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when a requested file is not found on the path specified
    /// </summary>
    public class FileNotFoundError : ErrorDescription
    {
        internal FileNotFoundError(string file_path)
        {
            this.message = "The requested file hasn't been not found at \"" + file_path + "\"";
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text { get { return message; } }

        private string message;
    }
}
