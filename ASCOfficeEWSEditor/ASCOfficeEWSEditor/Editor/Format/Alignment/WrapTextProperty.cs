using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Alignment
{
    internal class WrapTextProperty : PropertyBooleanAttrib
    {
        public WrapTextProperty(bool init_value)
            : base(init_value, false)
        {
        }

        public WrapTextProperty()
            : base(false)
        {
        }

        internal override string AttribName { get { return "wrapText"; } }
    }
}
