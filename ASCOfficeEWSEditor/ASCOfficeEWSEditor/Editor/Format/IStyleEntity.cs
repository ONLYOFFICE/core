using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format
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
