using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format
{
    internal abstract class SeparateEntity : IStyleEntity
    {
        public SeparateEntity(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public int GetEntityIdByXF(XmlElement xf_tag)
        {
            return XMLInt.ParseOrDefault(xf_tag.GetAttribute(GetEntityIdAttribName()), 0);
        }

        public void SetEntityIdToXF(XmlElement xf_tag, int entity_id)
        {
            xf_tag.SetAttribute(GetEntityIdAttribName(), entity_id.ToString());
        }

        public abstract string GetEntityIdAttribName();
        public abstract string GetEntitiesTagName();
        public abstract string GetEntityTagName();
        public abstract string GetEntityTemplate();
        public abstract bool CompareEntities(XmlElement ent1, XmlElement ent2);

        private XmlElement GetEntityTagByXF(XmlElement xf_tag)
        {
            XmlNode style_sheet_tag = workbook.StyleSheet.GetStyleSheetOrCreate();
            string entities_tag_name = GetEntitiesTagName(); // Ex: "fonts"
            XmlNode entities_tag = style_sheet_tag.SelectSingleNode("main:" + entities_tag_name, workbook.NamespaceManager); // Ex: <fonts>
            if (entities_tag == null)
            {
                entities_tag = workbook.GetXmlDocument().CreateElement(entities_tag_name, workbook.NamespaceManager.LookupNamespace("main")); // Ex: <fonts>
                style_sheet_tag.AppendChild(entities_tag);
            }
            string entity_tag_name = GetEntityTagName(); // Ex: "font"
            int entity_id = GetEntityIdByXF(xf_tag); // Ex: @fontId = 18
            XmlNode entity_tag = entities_tag.SelectSingleNode("main:" + entity_tag_name +
                                                               "[position() = " + (entity_id + 1).ToString() + "]", workbook.NamespaceManager); // Ex: <font>[18 + 1]
            if (entity_tag == null) // if the entity_id is wrong
            {
                entity_tag = entities_tag.SelectSingleNode("main:" + entity_tag_name, workbook.NamespaceManager); // Ex: <font>[1]
                if (entity_tag == null) // if the entities container is empty
                {
                    entities_tag.InnerXml = GetEntityTemplate(); // Ex: Create <font>[1]
                    ((XmlElement)entities_tag).SetAttribute("count", "1");
                    entity_tag = entities_tag.FirstChild;
                }
            }
            return (XmlElement)entity_tag; // Ex: <font>
        }

        private int GetEqualOrAppliedEntityId(XmlElement entity_with_value_set, XmlNode entities_tag)
        {
            XmlNodeList entities = entities_tag.SelectNodes("main:" + GetEntityTagName(), workbook.NamespaceManager);
            int entity_position = 0;
            foreach (XmlElement entity in entities)
            {
                if (CompareEntities(entity_with_value_set, entity))
                {
                    break;
                }
                ++entity_position;
            }
            if (entity_position == entities.Count)
            {
                entities_tag.AppendChild(entity_with_value_set);
                ((XmlElement)entities_tag).RemoveAttribute("count");
            }
            return entity_position;
        }

        public int ApplyProperty(XmlElement xf_record, IProperty property)
        {
            XmlNode entity_tag = GetEntityTagByXF(xf_record);
            XmlElement entity_clonned = (XmlElement)entity_tag.Clone();
            property.SetToEntity(entity_clonned);
            return GetEqualOrAppliedEntityId(entity_clonned, entity_tag.ParentNode);
        }

        public bool CollectPropertyFromXF(XmlElement xf_record, IProperty property)
        {
            XmlElement entity_tag = GetEntityTagByXF(xf_record);
            return property.CollectFromEntity(entity_tag);
        }


        private Workbook workbook;
    }
}
