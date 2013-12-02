using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format
{
    public interface IStyleEntity
    {
        int GetEntityIdByXF(XmlElement xf_tag);
        void SetEntityIdToXF(XmlElement xf_tag, int entity_id);
        bool CompareEntities(XmlElement ent1, XmlElement ent2);
        int ApplyProperty(XmlElement xf_record, IProperty property);
        bool CollectPropertyFromXF(XmlElement xf_record, IProperty property);
    }
}
