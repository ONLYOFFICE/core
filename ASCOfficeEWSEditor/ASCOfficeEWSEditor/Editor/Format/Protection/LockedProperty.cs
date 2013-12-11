using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Protection
{
    internal class LockedProperty : PropertyBooleanAttrib
    {
        public LockedProperty(bool init_value)
            : base(init_value, true)
        {
        }

        public LockedProperty()
            : base(true)
        {
        }

        internal override string AttribName { get { return "locked"; } }
    }
}
