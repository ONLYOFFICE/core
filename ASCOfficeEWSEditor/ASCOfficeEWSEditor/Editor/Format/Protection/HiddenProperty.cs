using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Protection
{
    internal class HiddenProperty : PropertyBooleanAttrib
    {
        public HiddenProperty(bool init_value)
            : base(init_value, false)
        {
        }

        public HiddenProperty()
            : base(false)
        {
        }

        internal override string AttribName { get { return "hidden"; } }
    }
}
