using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Format.Borders
{
    internal class BorderEntity : SeparateEntity
    {
        public BorderEntity(Workbook workbook)
            : base(workbook)
        {
            color_left_prop = new BorderProperty(BorderPosition.left, new Colors.FgColorProperty("color", workbook), workbook);
            color_right_prop = new BorderProperty(BorderPosition.right, new Colors.FgColorProperty("color", workbook), workbook);
            color_top_prop = new BorderProperty(BorderPosition.top, new Colors.FgColorProperty("color", workbook), workbook);
            color_bottom_prop = new BorderProperty(BorderPosition.bottom, new Colors.FgColorProperty("color", workbook), workbook);
            color_diagonal_prop = new BorderProperty(BorderPosition.diagonal, new Colors.FgColorProperty("color", workbook), workbook);
            style_left_prop = new BorderProperty(BorderPosition.left, new StyleProperty(workbook), workbook);
            style_right_prop = new BorderProperty(BorderPosition.right, new StyleProperty(workbook), workbook);
            style_top_prop = new BorderProperty(BorderPosition.top, new StyleProperty(workbook), workbook);
            style_bottom_prop = new BorderProperty(BorderPosition.bottom, new StyleProperty(workbook), workbook);
            style_diagonal_prop = new BorderProperty(BorderPosition.diagonal, new StyleProperty(workbook), workbook);
            diag_type_prop = new DiagonalTypeProperty(workbook);
            outline_prop = new OutlineProperty();
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
            return Resource1.default_border;
        }

        public override bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return color_left_prop.CompareEntities(ent1, ent2) &&
                color_right_prop.CompareEntities(ent1, ent2) &&
                color_top_prop.CompareEntities(ent1, ent2) &&
                color_bottom_prop.CompareEntities(ent1, ent2) &&
                color_diagonal_prop.CompareEntities(ent1, ent2) &&
                style_left_prop.CompareEntities(ent1, ent2) &&
                style_right_prop.CompareEntities(ent1, ent2) &&
                style_top_prop.CompareEntities(ent1, ent2) &&
                style_bottom_prop.CompareEntities(ent1, ent2) &&
                style_diagonal_prop.CompareEntities(ent1, ent2) &&
                diag_type_prop.CompareEntities(ent1, ent2) &&
                outline_prop.CompareEntities(ent1, ent2);
        }

        private const string apply_entity_attrib_name = "applyBorder";
        private const string entity_id_attrib_name = "borderId";
        private const string entities_tag_name = "borders";
        private const string entity_tag_name = "border";
        
        private BorderProperty color_left_prop;
        private BorderProperty color_right_prop;
        private BorderProperty color_top_prop;
        private BorderProperty color_bottom_prop;
        private BorderProperty color_diagonal_prop;
        private BorderProperty style_left_prop;
        private BorderProperty style_right_prop;
        private BorderProperty style_top_prop;
        private BorderProperty style_bottom_prop;
        private BorderProperty style_diagonal_prop;
        private DiagonalTypeProperty diag_type_prop;
        private OutlineProperty outline_prop;


    }
}
