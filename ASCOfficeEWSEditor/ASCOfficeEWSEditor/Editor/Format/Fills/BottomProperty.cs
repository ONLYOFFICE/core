using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    internal class BottomProperty : PropertyDoubleAttrib
    {
        public BottomProperty(double init_value)
            : base(init_value, 0)
        {}

        public BottomProperty()
            : base(0)
        {}

        internal override string AttribName { get { return "bottom"; } }
    }
}
