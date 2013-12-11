using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class SharedStringsFile : XmlFile
    {
        public SharedStringsFile(ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, sst_rel_path, 
                    Relationships.FileType.sharedStrings, ContentTypes.FileType.sharedStrings)
        {
            ns_manager = workbook.NamespaceManager;
        }

        public XmlElement GetSST()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "sst")
            {
                return null;
            }
            return (XmlElement)file_tag;
        }

        public XmlElement GetSSTOrCreate()
        {
            return GetSST() ?? CreateSST();
        }

        private XmlElement CreateSST()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            XmlElement sst_tag = file_tag.OwnerDocument.CreateElement("sst", ns_manager.LookupNamespace("main"));
            sst_tag.SetAttribute("count", "0");
            sst_tag.SetAttribute("uniqueCount", "0");
            file_tag.AppendChild(sst_tag);
            return sst_tag;
        }

        const string sst_rel_path = "sharedStrings.xml";
        XmlNamespaceManager ns_manager;
    }
}
