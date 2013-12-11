using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class ShadowProperty : PropertyBooleanTag
    {
        public ShadowProperty(bool init_value, Workbook workbook) : base(init_value, workbook)
        {
        }

        public ShadowProperty(Workbook workbook)
            : base(workbook)
        {
        }

        internal override string TagName { get { return "shadow"; } }
    }
}
