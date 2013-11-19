using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class DegreeProperty : PropertyIntAttrib
    {
        public DegreeProperty(int init_value)
            : base(init_value, 0)
        {}

        public DegreeProperty()
            : base(0)
        {}

        internal override string AttribName { get { return "degree"; } }
    }
}
