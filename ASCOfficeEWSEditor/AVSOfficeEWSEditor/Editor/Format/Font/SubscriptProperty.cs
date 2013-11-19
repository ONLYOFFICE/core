using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class SubscriptProperty : Property<bool>
    {
        public SubscriptProperty(bool init_val, Workbook workbook)
            : base(init_val)
        {
            this.workbook = workbook;
        }

        public SubscriptProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override bool ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement vert_tag = (XmlElement)font_tag.SelectSingleNode("main:vertAlign", workbook.NamespaceManager);
            if (vert_tag == null)
            {
                return false;
            }
            string vert_val = vert_tag.GetAttribute("val");
            switch (vert_val)
            {
                case "subscript":
                    return true;
                default:
                    return false;
            }
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement vert_tag = (XmlElement)font_tag.SelectSingleNode("main:vertAlign", workbook.NamespaceManager);

                if (Value.Value)
                {
                    if (vert_tag == null)
                    {
                        vert_tag = font_tag.OwnerDocument.CreateElement("vertAlign", workbook.NamespaceManager.LookupNamespace("main"));
                        font_tag.AppendChild(vert_tag);
                    }
                    vert_tag.SetAttribute("val", "subscript");
                }
                else
                {
                    if (vert_tag != null)
                    {
                        font_tag.RemoveChild(vert_tag);
                    }
                }
            }
        }

        private Workbook workbook;
    }
}
