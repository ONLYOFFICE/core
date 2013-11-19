using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.Helpers;

namespace AVSOfficeEWSEditor.Editor.OOX
{
    internal class TableFile : XmlFile
    {
        public TableFile(OOX.ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode,
                    OOXNavigate.CreateUniqueFilePath(table_rel_path_template, workbook.GetRootTag().ParentNode),
                    Relationships.FileType.table, ContentTypes.FileType.table)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
        }

        // Called when the file already exists
        public TableFile(string table_path, ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode, table_path,
                    Relationships.FileType.table, ContentTypes.FileType.table)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
        }

        public XmlElement GetTable()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "table")
            {
                return null;
            }
            return (XmlElement)file_tag;
        }

        public XmlElement GetTableOrCreate()
        {
            return GetTable() ?? CreateTable();
        }

        private XmlElement CreateTable()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            XmlElement sst_tag = file_tag.OwnerDocument.CreateElement("table", ns_manager.LookupNamespace("main"));
            sst_tag.SetAttribute("id", "-1");
            sst_tag.SetAttribute("totalsRowShown", "0");
            file_tag.AppendChild(sst_tag);
            return sst_tag;
        }

        const string table_rel_path_template = "../tables/table.xml";
        XmlNamespaceManager ns_manager;
    }
}
