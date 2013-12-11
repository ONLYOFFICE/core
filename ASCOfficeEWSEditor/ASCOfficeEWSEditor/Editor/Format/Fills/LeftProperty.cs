using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    internal class LeftProperty : PropertyDoubleAttrib
    {
        public LeftProperty(double init_value)
            : base(init_value, 0)
        {}

        public LeftProperty()
            : base(0)
        {}

        internal override string AttribName { get { return "left"; } }
    }
}
