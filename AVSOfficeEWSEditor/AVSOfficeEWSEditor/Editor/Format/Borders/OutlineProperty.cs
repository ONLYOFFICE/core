using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Borders
{
    internal class OutlineProperty : PropertyBooleanAttrib
    {
        public OutlineProperty(bool init_value) : base(init_value, false)
        {
        }

        public OutlineProperty() : base(false)
        {
        }

        internal override string AttribName { get { return "outline"; } }
    }
}
