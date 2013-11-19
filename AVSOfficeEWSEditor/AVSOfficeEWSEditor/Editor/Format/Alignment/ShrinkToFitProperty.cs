using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
{
    internal class ShrinkToFitProperty : PropertyBooleanAttrib
    {
        public ShrinkToFitProperty(bool init_value)
            : base(init_value, false)
        {
        }

        public ShrinkToFitProperty() : base(false)
        {
        }

        internal override string AttribName { get { return "shrinkToFit"; } }
    }


}
