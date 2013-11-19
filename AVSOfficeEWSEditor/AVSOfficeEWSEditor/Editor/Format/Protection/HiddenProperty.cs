using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Protection
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
