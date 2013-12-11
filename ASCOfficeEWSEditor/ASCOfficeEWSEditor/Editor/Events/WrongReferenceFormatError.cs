using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    /// <summary>
    /// This happens when a passed reference has wrong format
    /// </summary>
    public class WrongReferenceFormatError : ErrorDescription
    {
        internal WrongReferenceFormatError(string wrong_reference)
        {
            this.wrong_reference = wrong_reference;
        }

        /// <summary>
        /// Description getter
        /// </summary>
        public override string Text
        {
            get { return "Wrong reference format in \"" + wrong_reference + "\""; }
        }

        private string wrong_reference;
    }
}
