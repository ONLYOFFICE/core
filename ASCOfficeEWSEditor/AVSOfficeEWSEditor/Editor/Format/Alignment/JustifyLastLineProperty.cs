using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
{
    internal class JustifyLastLineProperty : PropertyBooleanAttrib
    {
        public JustifyLastLineProperty(bool init_value)
            : base(init_value, false)
        {
        }

        public JustifyLastLineProperty()
            : base(false)
        {
        }

        internal override string AttribName { get { return "justifyLastLine"; } }
    }


}
