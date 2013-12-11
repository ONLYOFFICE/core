using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Events
{
    internal class ReportableException : SystemException
    {
        public ReportableException(ErrorDescription error_description)
        {
            this.error_description = error_description;
        }

        public override string ToString()
        {
            return error_description.ToString();
        }
        public ErrorDescription error_description { get; private set; }
    }
}
