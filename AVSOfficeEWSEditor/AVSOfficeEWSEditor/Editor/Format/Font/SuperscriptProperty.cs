using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class SuperscriptProperty : Property<bool>
    {
        public SuperscriptProperty(bool init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public SuperscriptProperty(Workbook workbook)
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
                case "superscript":
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
                    vert_tag.SetAttribute("val", "superscript");
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
