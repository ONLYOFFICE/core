using System;
using System.Collections.Generic;
using System.Text;

namespace CleverAge.OdfConverter.OdfConverterLib
{
    public class OdfEventArgs : EventArgs
    {
        private string message;

        public OdfEventArgs(string message)
        {
            this.message = message;
        }

        public string Message
        {
            get
            {
                return message;
            }
        }

    }
}
