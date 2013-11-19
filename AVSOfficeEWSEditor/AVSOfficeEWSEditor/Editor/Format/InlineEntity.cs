using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format
{
    internal abstract class InlineEntity : IStyleEntity
    {
        public InlineEntity(Workbook workbook)
        {
            this.workbook = workbook;
        }


        public int GetEntityIdByXF(XmlElement xf_tag)
        {
            return -1;
        }

        abstract public string GetEntityTagName();
        abstract public bool CompareEntities(XmlElement ent1, XmlElement ent2);

        public XmlElement GetEntityTagByXF(XmlElement xf_tag)
        {
            return (XmlElement)xf_tag.SelectSingleNode("main:" + GetEntityTagName(), workbook.NamespaceManager);
        }

        public int ApplyProperty(XmlElement xf_record, IProperty property)
        {
            XmlElement entity_tag = GetEntityTagByXF(xf_record);
            if (entity_tag == null)
            {
                entity_tag = xf_record.OwnerDocument.CreateElement(GetEntityTagName(), workbook.NamespaceManager.LookupNamespace("main"));
                xf_record.AppendChild(entity_tag);
            }

            property.SetToEntity(entity_tag);
            return -1;
        }

        public bool CollectPropertyFromXF(XmlElement xf_record, IProperty property)
        {
            XmlElement entity_tag = GetEntityTagByXF(xf_record);
            return property.CollectFromEntity(entity_tag);
        }

        public void SetEntityIdToXF(XmlElement xf_tag, int entity_id)
        {
        }

        private Workbook workbook;
    }
}
