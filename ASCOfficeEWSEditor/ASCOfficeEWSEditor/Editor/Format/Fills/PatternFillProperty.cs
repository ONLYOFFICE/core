using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    internal class PatternFillProperty : PropertyBridgeTag 
    {
        public PatternFillProperty(IProperty included_property, Workbook workbook)
            : base(included_property, workbook)
        {
        }

        internal override string TagName { get { return "patternFill"; } }
    }

}
