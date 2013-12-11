using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class OutlineProperty : PropertyBooleanTag
    {
        public OutlineProperty(bool init_value, Workbook workbook) : base(init_value, workbook)
        {
        }

        public OutlineProperty(Workbook workbook)
            : base(workbook)
        {
        }

        internal override string TagName { get { return "outline"; } }
    }
}
