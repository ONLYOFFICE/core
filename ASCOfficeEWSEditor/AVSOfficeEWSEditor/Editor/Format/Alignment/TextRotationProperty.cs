using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
{
    internal class TextRotationProperty : Property<int>
    {
        public TextRotationProperty(int init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public TextRotationProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override int ExtractValueFromEntity(XmlElement alignment_tag)
        {
            if (alignment_tag == null)
            {
                return 0;
            }
            return XMLInt.ParseOrDefault(alignment_tag.GetAttribute("textRotation"), 0);
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != 0)
                {
                    alignment_tag.SetAttribute("textRotation", Value.Value.ToString());
                }
                else
                {
                    alignment_tag.RemoveAttribute("textRotation");
                }
            }
        }


        private Workbook workbook;
    }


}
