using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when trying to save or load to or from an unsupported file format
    /// </summary>
    public class UnsupportedFileFormatError : ErrorDescription
    {
        internal UnsupportedFileFormatError(FileFormats.Format format)
        {
            this.message = "Unsupported file format " + format;
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text { get { return message; } }

        private string message;
    }
}
