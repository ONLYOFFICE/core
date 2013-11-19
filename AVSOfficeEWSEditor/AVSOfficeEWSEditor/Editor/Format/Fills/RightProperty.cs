using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class RightProperty : PropertyDoubleAttrib
    {
        public RightProperty(double init_value)
            : base(init_value, 0)
        {}

        public RightProperty()
            : base(0)
        {}

        internal override string AttribName { get { return "right"; } }
    }
}
