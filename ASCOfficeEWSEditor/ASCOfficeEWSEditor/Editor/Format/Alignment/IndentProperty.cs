using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Alignment
{
    internal class IndentProperty : Property<int>
    {
        public IndentProperty(int init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public IndentProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override int ExtractValueFromEntity(XmlElement alignment_tag)
        {
            if (alignment_tag == null)
            {
                return 0;
            }
            return XMLInt.ParseOrDefault(alignment_tag.GetAttribute("indent"), 0);
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != 0)
                {
                    alignment_tag.SetAttribute("indent", Value.Value.ToString());
                }
                else
                {
                    alignment_tag.RemoveAttribute("indent");
                }
            }
        }


        private Workbook workbook;
    }


}
