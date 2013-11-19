using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
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
