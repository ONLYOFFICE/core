using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    internal interface IErrorDescription
    {
        string Text { get; }
    }

    /// <summary>
    /// Arguments for ErrorHappenedEvent
    /// </summary>
    public abstract class ErrorDescription : EventArgs, IErrorDescription
    {
        /// <summary>
        /// Description getter
        /// </summary>
        public abstract string Text { get; }

        /// <summary>
        /// String representation of an ErrorDescription object
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return Text;
        }
    }
}
