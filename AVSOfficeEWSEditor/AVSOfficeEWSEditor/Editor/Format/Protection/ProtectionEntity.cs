using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Protection
{
    internal class ProtectionEntity : InlineEntity
    {
        public ProtectionEntity(Workbook workbook)
            : base(workbook)
        {
            hidden_prop = new HiddenProperty();
            locked_prop = new LockedProperty();
        }

        public override bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return hidden_prop.CompareEntities(ent1, ent2) && locked_prop.CompareEntities(ent1, ent2);
        }

        public override string GetEntityTagName()
        {
            return entity_tag_name;
        }

        private const string entity_tag_name = "property";

        private HiddenProperty hidden_prop;
        private LockedProperty locked_prop;

    }
}
