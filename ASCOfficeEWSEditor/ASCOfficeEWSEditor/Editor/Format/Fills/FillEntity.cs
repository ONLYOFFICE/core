using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class FillEntity : SeparateEntity
    {
        public FillEntity(Workbook workbook)
            : base(workbook)
        {
            this.workbook = workbook;
            pattern_prop = new PatternFillProperty(new PatternProperty(), workbook);
            bgcolor_prop = new PatternFillProperty(new Colors.BgColorProperty("bgColor", workbook), workbook);
            fgcolor_prop = new PatternFillProperty(new Colors.FgColorProperty("fgColor", workbook), workbook);
            fg_color_prop = new Colors.FgColorProperty("color", workbook);
            gradient_type_prop = new GradientFillProperty(new GradientTypeProperty(), workbook);
            gradient_left_prop = new GradientFillProperty(new LeftProperty(), workbook);
            gradient_right_prop = new GradientFillProperty(new RightProperty(), workbook);
            gradient_top_prop = new GradientFillProperty(new TopProperty(), workbook);
            gradient_bottom_prop = new GradientFillProperty(new BottomProperty(), workbook);

        }

        public override string GetEntityIdAttribName()
        {
            return entity_id_attrib_name;
        }

        public override string GetEntitiesTagName()
        {
            return entities_tag_name;
        }

        public override string GetEntityTagName()
        {
            return entity_tag_name;
        }

        public override string GetEntityTemplate()
        {
            return Resource1.default_fill;
        }

        public override bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            if (!pattern_prop.CompareEntities(ent1, ent2) ||
                !bgcolor_prop.CompareEntities(ent1, ent2) ||
                !fgcolor_prop.CompareEntities(ent1, ent2) ||
                !gradient_type_prop.CompareEntities(ent1, ent2) ||
                !gradient_left_prop.CompareEntities(ent1, ent2) ||
                !gradient_right_prop.CompareEntities(ent1, ent2) ||
                !gradient_top_prop.CompareEntities(ent1, ent2) ||
                !gradient_bottom_prop.CompareEntities(ent1, ent2))
            {
                return false;
            }
            int stops1 = gradient_type_prop.StopsCount(ent1);
            if (stops1 != gradient_type_prop.StopsCount(ent2))
            {
                return false;
            }
            if (gradient_type_prop.StopsCount(ent1) != 0)
            {
                foreach (XmlElement stop_tag in ent1.SelectNodes("main:gradientFill/main:stop", workbook.NamespaceManager))
                {
                    XmlElement stop2_tag = (XmlElement)ent2.SelectSingleNode("main:gradientFill/main:stop[@position = \"" + 
                                                            stop_tag.GetAttribute("position") + "\"]", workbook.NamespaceManager);
                    if(stop2_tag == null)
                    {
                        return false;
                    }
                    if(!fg_color_prop.CompareEntities(stop_tag, stop2_tag))
                    {
                        return false;
                    }
                }
                
            }

            return true;
        }

        private const string apply_entity_attrib_name = "applyFill";
        private const string entity_id_attrib_name = "fillId";
        private const string entities_tag_name = "fills";
        private const string entity_tag_name = "fill";

        private PatternFillProperty pattern_prop;
        private PatternFillProperty bgcolor_prop;
        private PatternFillProperty fgcolor_prop;
        private GradientFillProperty gradient_type_prop;
        private GradientFillProperty gradient_left_prop;
        private GradientFillProperty gradient_right_prop;
        private GradientFillProperty gradient_top_prop;
        private GradientFillProperty gradient_bottom_prop;
        private Colors.FgColorProperty fg_color_prop;

        private Workbook workbook;
        
    }
}
