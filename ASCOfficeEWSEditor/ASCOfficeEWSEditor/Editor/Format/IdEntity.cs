using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format
{
    internal abstract class IdStyleEntity : IStyleEntity
    {
        public IdStyleEntity(PropertyId property)
        {
            this.property = property;
        }

        public int GetEntityIdByXF(XmlElement xf_tag)
        {
            return XMLInt.ParseOrDefault(xf_tag.GetAttribute(GetEntityIdAttribName()), 0);
        }

        public abstract string GetEntityIdAttribName();

        public bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            throw new NotImplementedException();
        }

        public void SetEntityIdToXF(XmlElement xf_tag, int entity_id)
        {
            xf_tag.SetAttribute(GetEntityIdAttribName(), entity_id.ToString());
        }

        public int ApplyProperty(XmlElement xf_record, IProperty invalid_property)
        {
            return property.Value.HasValue ?  property.Value.Value : 0;
        }

        public bool CollectPropertyFromXF(XmlElement xf_record, IProperty invalid_property)
        {
            int entity_id = GetEntityIdByXF(xf_record);
            return property.CollectFromValue(entity_id);
        }

        private PropertyId property;
    }
}
