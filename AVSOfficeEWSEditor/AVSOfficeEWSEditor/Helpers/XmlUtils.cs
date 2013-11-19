using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Helpers
{
    internal static class XmlUtils
    {
        private static int GetIntValFromChild(XmlNode xml_parent, string node_name, XmlNamespaceManager ns_manager, int? default_value)
        {
            int num;
            XmlElement num_xml = (XmlElement)xml_parent.SelectSingleNode(node_name, ns_manager);
            if (num_xml != null)
            {
                if(int.TryParse(num_xml.GetAttribute("val"), out num))
                {
                    return num;
                }                
            }
            if(default_value.HasValue)
            {
                return default_value.Value;
            }
            else
            {
                throw new AVSOfficeEWSEditor.Editor.Events.ReportableException(new AVSOfficeEWSEditor.Editor.Events.InternalError("Wrong OOX format. Required '" + node_name + "' tag is absent inside '" + xml_parent.LocalName + "' or has incorrect format."));
            }
        }

        internal static int GetIntValFromChild(XmlNode xml_parent, string node_name, XmlNamespaceManager ns_manager, int default_value)
        {
            return GetIntValFromChild(xml_parent, node_name, ns_manager, default_value);
        }

        internal static int GetIntValFromChild(XmlNode xml_parent, string node_name, XmlNamespaceManager ns_manager)
        {
            return GetIntValFromChild(xml_parent, node_name, ns_manager, null);
        }


    }
}
