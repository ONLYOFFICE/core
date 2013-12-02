using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class CondenseProperty : PropertyBooleanTag
    {
        public CondenseProperty(bool init_value, Workbook workbook) : base(init_value, workbook)
        {
        }

        public CondenseProperty(Workbook workbook)
            : base(workbook)
        {
        }

        internal override string TagName { get { return "condese"; } }
    }
}
