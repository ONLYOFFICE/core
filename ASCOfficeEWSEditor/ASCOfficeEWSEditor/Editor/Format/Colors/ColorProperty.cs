using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Colors
{
    internal class ColorProperty : PropertyColor
    {
        public ColorProperty(Colors.ColorValue init_val, bool fore_or_back, string color_tag_name, Workbook workbook)
            : base(init_val)
        {
            this.workbook = workbook;
            this.color_tag_name = color_tag_name;
            this.fore_or_back = fore_or_back;
        }

        public ColorProperty(string color_tag_name, bool fore_or_back, Workbook workbook)
        {
            this.workbook = workbook;
            this.color_tag_name = color_tag_name;
            this.fore_or_back = fore_or_back;
        }

        public override Colors.ColorValue ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement color_tag = (XmlElement)font_tag.SelectSingleNode("main:" + color_tag_name, workbook.NamespaceManager);
            string val_str;
            if (color_tag != null)                
            {
                if ((val_str = color_tag.GetAttribute("rgb")) != "")
                {
                    return new Colors.ColorValue(val_str, fore_or_back, workbook);
                }
                if ((val_str = color_tag.GetAttribute("theme")) != "")
                {
                    double tint = XMLDouble.ParseOrDefault(color_tag.GetAttribute("tint"), 0);
                    return new Colors.ColorValue(XMLInt.ParseOrDefault(val_str, fore_or_back ? 1 : 0), tint, fore_or_back, workbook);
                }
                if ((val_str = color_tag.GetAttribute("indexed")) != "")
                {
                    return new Colors.ColorValue(XMLInt.ParseOrDefault(val_str, fore_or_back ? 64 : 65), fore_or_back, workbook); 
                }
            }
            return new Colors.ColorValue(fore_or_back, workbook);
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value != null)
            {
                XmlElement color_tag = (XmlElement)font_tag.SelectSingleNode("main:" + color_tag_name, workbook.NamespaceManager);
                if (color_tag == null)
                {
                    color_tag = font_tag.OwnerDocument.CreateElement(color_tag_name, workbook.NamespaceManager.LookupNamespace("main"));
                    font_tag.AppendChild(color_tag);
                }
                color_tag.RemoveAllAttributes();
                Value.SetAttribute(color_tag);
            }
        }

        private Workbook workbook;
        private string color_tag_name;
        private bool fore_or_back;
    }


    internal class FgColorProperty : ColorProperty
    {
        public FgColorProperty(Colors.ColorValue init_val, bool fore_or_back, string color_tag_name, Workbook workbook)
            : base(init_val, true, color_tag_name, workbook)
        {
        }

        public FgColorProperty(string color_tag_name, Workbook workbook)
            : base(color_tag_name, true, workbook)
        {
        }
    }

    internal class BgColorProperty : ColorProperty
    {
        public BgColorProperty(Colors.ColorValue init_val, bool fore_or_back, string color_tag_name, Workbook workbook)
            : base(init_val, false, color_tag_name, workbook)
        {
        }

        public BgColorProperty(string color_tag_name, Workbook workbook)
            : base(color_tag_name, false, workbook)
        {
        }
    }

}
