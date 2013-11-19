using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Font
{
    internal class FontEntity : SeparateEntity
    {
        public FontEntity(Workbook workbook) : base(workbook)
        {
            b_prop = new BoldProperty(workbook);
            charset_prop = new CharsetProperty(workbook);
            color_prop = new Colors.FgColorProperty("color", workbook);
            condese_prop = new CondenseProperty(workbook);
            extend_prop = new ExtendProperty(workbook);
            family_prop = new FamilyProperty(workbook);
            i_prop = new ItalicProperty(workbook);
            name_prop = new FontNameProperty(workbook);
            outline_prop = new OutlineProperty(workbook);
            scheme_prop = new SchemeProperty(workbook);
            shadow_prop = new ShadowProperty(workbook);
            strike_prop = new StrikethroughProperty(workbook);
            sz_prop = new FontSizeProperty(workbook);
            u_prop = new UnderlineProperty(workbook);
            sub_prop = new SubscriptProperty(workbook);
            sup_prop = new SuperscriptProperty(workbook);
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
            return Resource1.default_font;
        }

        public override bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return b_prop.CompareEntities(ent1, ent2) &&
                charset_prop.CompareEntities(ent1, ent2) &&
                color_prop.CompareEntities(ent1, ent2) &&
                condese_prop.CompareEntities(ent1, ent2) &&
                extend_prop.CompareEntities(ent1, ent2) &&
                family_prop.CompareEntities(ent1, ent2) &&
                i_prop.CompareEntities(ent1, ent2) &&
                name_prop.CompareEntities(ent1, ent2) &&
                outline_prop.CompareEntities(ent1, ent2) &&
                scheme_prop.CompareEntities(ent1, ent2) &&
                shadow_prop.CompareEntities(ent1, ent2) &&
                strike_prop.CompareEntities(ent1, ent2) &&
                sz_prop.CompareEntities(ent1, ent2) &&
                u_prop.CompareEntities(ent1, ent2) &&
                sub_prop.CompareEntities(ent1, ent2) &&
                sup_prop.CompareEntities(ent1, ent2);
        }

        private const string apply_entity_attrib_name = "applyFont";
        private const string entity_id_attrib_name = "fontId";
        private const string entities_tag_name = "fonts";
        private const string entity_tag_name = "font";

        private BoldProperty b_prop;
        private CharsetProperty charset_prop;
        private Colors.ColorProperty color_prop;
        private CondenseProperty condese_prop;
        private ExtendProperty extend_prop;
        private FamilyProperty family_prop;
        private ItalicProperty i_prop;
        private FontNameProperty name_prop;
        private OutlineProperty outline_prop;
        private SchemeProperty scheme_prop;
        private ShadowProperty shadow_prop;
        private StrikethroughProperty strike_prop;
        private FontSizeProperty sz_prop;
        private UnderlineProperty u_prop;
        private SubscriptProperty sub_prop;
        private SuperscriptProperty sup_prop;


    }
}
