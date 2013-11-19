using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class StrikethroughProperty : PropertyBooleanTag
    {
        public StrikethroughProperty(bool init_value, Workbook workbook)
            : base(init_value, workbook)
        {
        }

        public StrikethroughProperty(Workbook workbook)
            : base(workbook)
        {
        }

        internal override string TagName { get { return "strike"; } }
    }
}
