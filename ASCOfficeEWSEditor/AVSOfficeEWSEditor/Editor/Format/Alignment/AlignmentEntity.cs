using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Alignment
{
    internal class AlignmentEntity : InlineEntity
    {
        public AlignmentEntity(Workbook workbook)
            : base(workbook)
        {
             horiz_prop = new HorizontalProperty(workbook);
             indent_prop = new IndentProperty(workbook);
             just_last_prop = new JustifyLastLineProperty();
             reading_order_prop = new ReadingOrderProperty(workbook);
             rel_indent_prop = new RelativeIndentProperty(workbook);
             shrink_to_fit_prop = new ShrinkToFitProperty();
             rotation_prop = new TextRotationProperty(workbook);
             vert_prop = new VerticalProperty(workbook);
             wrap_prop = new WrapTextProperty();
        }

        public override bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return horiz_prop.CompareEntities(ent1, ent2) &&
                indent_prop.CompareEntities(ent1, ent2) &&
                just_last_prop.CompareEntities(ent1, ent2) &&
                reading_order_prop.CompareEntities(ent1, ent2) &&
                rel_indent_prop.CompareEntities(ent1, ent2) &&
                shrink_to_fit_prop.CompareEntities(ent1, ent2) &&
                rotation_prop.CompareEntities(ent1, ent2) &&
                vert_prop.CompareEntities(ent1, ent2) &&
                wrap_prop.CompareEntities(ent1, ent2);
        }

        public override string GetEntityTagName()
        {
            return entity_tag_name;
        }

        private const string entity_tag_name = "alignment";

        private HorizontalProperty horiz_prop;
        private IndentProperty indent_prop;
        private JustifyLastLineProperty just_last_prop;
        private ReadingOrderProperty reading_order_prop;
        private RelativeIndentProperty rel_indent_prop;
        private ShrinkToFitProperty shrink_to_fit_prop;
        private TextRotationProperty rotation_prop;
        private VerticalProperty vert_prop;
        private WrapTextProperty wrap_prop;

    }
}
