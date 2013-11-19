using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class GradientFillProperty : PropertyBridgeTag 
    {
        public GradientFillProperty(IProperty included_property, Workbook workbook)
            : base(included_property, workbook)
        {
            this.workbook = workbook;
        }

        internal int StopsCount(XmlElement ent1)
        {
            return ent1.SelectNodes("main:gradientFill/main:stop", workbook.NamespaceManager).Count;
        }

        internal override string TagName { get { return "gradientFill"; } }

        private Workbook workbook;
    }

}
