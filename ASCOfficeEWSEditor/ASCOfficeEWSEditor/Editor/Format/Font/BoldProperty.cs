using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class BoldProperty : PropertyBooleanTag
    {
        public BoldProperty(bool init_value, Workbook workbook) : base(init_value, workbook)
        {
        }

        public BoldProperty(Workbook workbook) : base(workbook)
        {
        }

        internal override string TagName { get { return "b"; } }
    }
}
