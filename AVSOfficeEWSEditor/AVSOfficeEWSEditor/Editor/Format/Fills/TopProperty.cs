using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class TopProperty : PropertyDoubleAttrib
    {
        public TopProperty(double init_value)
            : base(init_value, 0)
        {}

        public TopProperty()
            : base(0)
        {}

        internal override string AttribName { get { return "top"; } }
    }
}
