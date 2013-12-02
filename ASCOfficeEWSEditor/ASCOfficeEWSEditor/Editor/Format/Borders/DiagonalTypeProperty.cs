using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Borders
{
    internal class DiagonalTypeProperty : Property<DiagonalType>
    {
        public DiagonalTypeProperty(DiagonalType init_value, Workbook workbook)
            : base(init_value)
        {
            this.workbook = workbook;
        }

        public DiagonalTypeProperty(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override DiagonalType ExtractValueFromEntity(XmlElement border_tag)
        {
            if (border_tag == null)
            {
                return DiagonalType.none;
            }
            bool up_val = XMLBoolean.ParseOrDefault(border_tag.GetAttribute("diagonalUp"), false);
            bool down_val = XMLBoolean.ParseOrDefault(border_tag.GetAttribute("diagonalDown"), false);
            if (up_val && down_val)
            {
                return DiagonalType.Both;
            }
            if(up_val)
            {
                return DiagonalType.Up;
            }
            if (down_val)
            {
                return DiagonalType.Down;
            }
            return DiagonalType.none;
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != DiagonalType.none)
                {
                    if (Value.Value == DiagonalType.Up)
                    {
                        alignment_tag.SetAttribute("diagonalUp", "true");
                        alignment_tag.RemoveAttribute("diagonalDown");
                    }
                    else if (Value.Value == DiagonalType.Down)
                    {
                        alignment_tag.SetAttribute("diagonalDown", "true");
                        alignment_tag.RemoveAttribute("diagonalUp");
                    }
                    else
                    {
                        alignment_tag.SetAttribute("diagonalUp", "true");
                        alignment_tag.SetAttribute("diagonalDown", "true");
                    }
                }
                else
                {
                    alignment_tag.RemoveAttribute("diagonalUp");
                    alignment_tag.RemoveAttribute("diagonalDown");
                }
            }
        }


        private Workbook workbook;
    }

}
