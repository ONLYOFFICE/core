using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class ContentTypes
    {
        public ContentTypes(XmlDocument xml_document)
        {
            ns_manager = new XmlNamespaceManager(xml_document.NameTable);
            ns_manager.AddNamespace("types", "http://schemas.openxmlformats.org/package/2006/content-types");
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            FillInTypes();
            content_types = xml_document.SelectSingleNode("/root/file[@name = '[Content_Types].xml']/types:Types", ns_manager);
        }

        public void RegisterFile(XmlNode file_tag, FileType type)
        {
            string file_path_absolute = Helpers.OOXNavigate.DetermineAbsolutePath(file_tag);
            RegisterFile(file_path_absolute, type);
        }

        public void RegisterFile(string file_path_absolute, FileType type)
        {
            XmlElement type_tag = content_types.OwnerDocument.CreateElement("Override", ns_manager.LookupNamespace("types"));
            content_types.AppendChild(type_tag);
            type_tag.SetAttribute("PartName", file_path_absolute);
            type_tag.SetAttribute("ContentType", types[type]);
        }

        public void DeRegisterFile(XmlNode file_tag)
        {
            string file_path_absolute = Helpers.OOXNavigate.DetermineAbsolutePath(file_tag);
            XmlNode type_tag = content_types.SelectSingleNode("types:Override[@PartName = '" + file_path_absolute + "']", ns_manager);
            if (null != type_tag)
            {
                content_types.RemoveChild(type_tag);
            }
        }

        public void RegisterFileType(string extension, FileType type)
        {
            XmlElement type_tag = content_types.OwnerDocument.CreateElement("Default", ns_manager.LookupNamespace("types"));
            content_types.AppendChild(type_tag);
            type_tag.SetAttribute("Extension", extension);
            type_tag.SetAttribute("ContentType", types[type]);
        }

        private void FillInTypes()
        {
            types = new Dictionary<FileType, string>();
            types[FileType.worksheet] = "application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml";
            types[FileType.chartsheet] = "application/vnd.openxmlformats-officedocument.drawingml.chart+xml";            
            types[FileType.workbook] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml";
            types[FileType.styles] = "application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml";
            types[FileType.sharedStrings] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml";
            types[FileType.calcChain] = "application/vnd.openxmlformats-officedocument.spreadsheetml.calcChain+xml";
            types[FileType.comments] = "application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml";
            types[FileType.drawing] = "application/vnd.openxmlformats-officedocument.drawing+xml";
            types[FileType.theme] = "application/vnd.openxmlformats-officedocument.theme+xml";

            types[FileType.xml] = "application/xml";
            types[FileType.rels] = "application/vnd.openxmlformats-package.relationships+xml";
            types[FileType.bin] = "application/vnd.openxmlformats-officedocument.spreadsheetml.printerSettings";
            types[FileType.png] = "image/png";
            types[FileType.bmp] = "image/bmp";
            types[FileType.jpeg] = "image/jpeg";
            types[FileType.gif] = "image/gif";
            types[FileType.tiff] = "image/tiff";
            types[FileType.emf] = "image/emf";
            types[FileType.wmf] = "image/wmf";
            types[FileType.tga] = "image/tga";
            types[FileType.vml] = "application/vnd.openxmlformats-officedocument.vmlDrawing";

            types[FileType.table] = "application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml";
      }

        public enum FileType { worksheet, chartsheet, workbook, styles, sharedStrings, calcChain, comments, drawing, theme,
                               xml, rels, bin, png, bmp, jpeg, gif, tiff, emf, wmf, tga, vml, table};
#region XML regions
        private XmlNode content_types;
        private XmlNamespaceManager ns_manager;
#endregion

        private Dictionary<FileType, string> types;


    }
}
