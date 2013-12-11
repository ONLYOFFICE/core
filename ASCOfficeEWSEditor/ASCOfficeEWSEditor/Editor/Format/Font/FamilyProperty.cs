using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Font
{
    internal class FamilyProperty : Property<int>
    {
        public FamilyProperty(int init_value, Workbook workbook) : base(init_value)
        {
            this.workbook = workbook;
        }

        public FamilyProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override int ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement family_tag = (XmlElement)font_tag.SelectSingleNode("main:family", workbook.NamespaceManager);
            if (family_tag == null)
            {
                return 0;
            }
            return XMLInt.ParseOrDefault(family_tag.GetAttribute("val"), 0);
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement family_tag = (XmlElement)font_tag.SelectSingleNode("main:family", workbook.NamespaceManager);
                if (family_tag == null)
                {
                    family_tag = font_tag.OwnerDocument.CreateElement("family", workbook.NamespaceManager.LookupNamespace("main"));
                    font_tag.AppendChild(family_tag);
                }
                family_tag.SetAttribute("val", Value.Value.ToString());
            }
        }

        private Workbook workbook;
    }
}
