using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.OOX
{
    internal class Relationships
    {
        public Relationships(XmlNode file_tag)
        {
            ns_manager = new XmlNamespaceManager(file_tag.OwnerDocument.NameTable);
            ns_manager.AddNamespace("r", "http://schemas.openxmlformats.org/package/2006/relationships");
            referenced_file_tag = file_tag;
            rels_tag = GetRelationshipsTag(false);
            FillInTypes();
        }

        public string GetTarget(string rId)
        {
            if (null == rels_tag)
            {
                return "";
            }
            else
            {
                XmlNode target = rels_tag.SelectSingleNode("r:Relationship[@Id = '" + rId + "']/@Target", ns_manager);
                return null == target ? "" : target.InnerText;
            }
        }

        public string GetId(string target)
        {
            if (null == rels_tag)
            {
                return "";
            }
            else
            {
                XmlNode id = rels_tag.SelectSingleNode("r:Relationship[@Target = '" + target + "']/@Id", ns_manager);
                return null == id ? "" : id.InnerText;
            }
        }

        public string GetTarget(FileType rel_type)
        {
            XmlNode target = rels_tag.SelectSingleNode("r:Relationship[@Type = '" + types[rel_type] + "']/@Target", ns_manager);
            return null == target ? "" : target.InnerText;
        }

        public string AddTarget(string target, FileType type, bool external_mode)
        {
            if (rels_tag == null)
            {
                rels_tag = GetRelationshipsTag(true);
            }
            XmlElement rel = rels_tag.OwnerDocument.CreateElement("Relationship", ns_manager.LookupNamespace("r"));
            int max_id = 0;
            foreach (XmlNode id_tag in rels_tag.SelectNodes("r:Relationship/@Id", ns_manager))
            {
                int id = XMLInt.ParseOrDefault(id_tag.InnerText.Substring(3), 0);
                if (id > max_id)
                {
                    max_id = id;
                }
            }
            string rId = "rId" + (max_id + 1).ToString();
            rel.SetAttribute("Id", rId);
            rel.SetAttribute("Type", types[type]);
            rel.SetAttribute("Target", target);
            if(external_mode)
            {
                rel.SetAttribute("TargetMode", "External");
            }
            rels_tag.AppendChild(rel);

            return rId;
        }

        public int GetLastRid()
        {
            if (rels_tag == null)
            {
                rels_tag = GetRelationshipsTag(true);
            }

            int max_id = 0;
            foreach (XmlNode id_tag in rels_tag.SelectNodes("r:Relationship/@Id", ns_manager))
            {
                int id = XMLInt.ParseOrDefault(id_tag.InnerText.Substring(3), 0);
                if (id > max_id)
                {
                    max_id = id;
                }
            }
            return max_id;
        }

        public void RemoveTarget(string rId)
        {
            XmlNode rel = rels_tag.SelectSingleNode("r:Relationship[@Id = '" + rId + "']", ns_manager);
            if (null != rel)
            {
                rels_tag.RemoveChild(rel);
            }
        }

        public void RemoveTarget(FileType rel_type)
        {
            XmlNode rel = rels_tag.SelectSingleNode("r:Relationship[@Type = '" + types[rel_type] + "']", ns_manager);
            if (null != rel)
            {
                rels_tag.RemoveChild(rel);
            }
        }

        private XmlNode GetRelationshipsTag(bool create_if_absent)
        {
            XmlNode file_name_tag = referenced_file_tag.SelectSingleNode("@name");
            if (null == file_name_tag)
            {
                return null;
            }
            string file_name = file_name_tag.InnerText;
            if ("[Content_Types].xml" == file_name)
            {
                file_name = "";
            }
            XmlNode ret_tag = referenced_file_tag.SelectSingleNode("../dir[@name = '_rels']/file[@name = '" + file_name + ".rels']/r:Relationships", ns_manager);
            if (ret_tag == null && create_if_absent)
            {
                XmlNode file_tag = Helpers.OOXNavigate.AddFile("_rels/" + file_name + ".rels", referenced_file_tag.ParentNode, false);
                ret_tag = file_tag.OwnerDocument.CreateElement("Relationships", ns_manager.LookupNamespace("r"));
                file_tag.AppendChild(ret_tag);
            }
            return ret_tag;
        }
        private void FillInTypes()
        {
            types = new Dictionary<FileType, string>();
            types[FileType.worksheet] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet";
            types[FileType.workbook] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument";
            types[FileType.styles] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles";
            types[FileType.sharedStrings] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings";
            types[FileType.calcChain] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain";
            types[FileType.comments] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments";
            types[FileType.drawing] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing";
            types[FileType.vmlDrawing] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/vmlDrawing";
            types[FileType.theme] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme";
            types[FileType.printerSettings] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/printerSettings";
            types[FileType.image] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
            types[FileType.hyperlink] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink";
            types[FileType.chartsheet] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet";
            types[FileType.chart] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart";
            types[FileType.table] = "http://schemas.openxmlformats.org/officeDocument/2006/relationships/table";
        }


        public enum FileType
        {
            worksheet, workbook, styles, sharedStrings, calcChain, comments, drawing, vmlDrawing, theme, printerSettings, image,
            hyperlink, chartsheet, chart, table
        };

#region XML references
        private XmlNode rels_tag;
        private XmlNode referenced_file_tag;
#endregion
        private Dictionary<FileType, string> types;

        XmlNamespaceManager ns_manager;
    }
}
