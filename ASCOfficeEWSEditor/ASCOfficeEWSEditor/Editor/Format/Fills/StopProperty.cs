using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Fills
{
    internal class StopProperty : PropertyBridgeTag 
    {
        public StopProperty(double position, IProperty included_property, Workbook workbook)
            : base(included_property, workbook)
        {
            this.included_property = included_property;
            this.workbook = workbook;
            this.position = position;
            tagName_precalculated = "stop[@position = \"" + XMLDouble.ToString(position) + "\"]";
        }
        public override void SetToEntity(XmlElement entity_tag)
        {
            XmlElement stop_tag = (XmlElement)entity_tag.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            if (stop_tag == null)
            {
                stop_tag = entity_tag.OwnerDocument.CreateElement("stop", workbook.NamespaceManager.LookupNamespace("main"));
                stop_tag.SetAttribute("position", XMLDouble.ToString(position));
                entity_tag.AppendChild(stop_tag);
            }
            included_property.SetToEntity(stop_tag);
        }

        internal override string TagName { get { return tagName_precalculated; } }

        private IProperty included_property;
        private Workbook workbook;
        private double position;
        private string tagName_precalculated;


    }

}
