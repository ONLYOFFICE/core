using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class StyleSheetFile : XmlFile, IOrderedSections
    {
        public StyleSheetFile(ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, styles_rel_path, 
                    Relationships.FileType.styles, ContentTypes.FileType.styles)
        {
            ns_manager = workbook.NamespaceManager;
            sections_names_backward = new List<string>() 
            { "extLst",  "colors", "tableStyles", "dxfs", "cellStyles", "cellXfs", "cellStyleXfs",
              "borders", "fills", "fonts", "numFmts"};

        }

        public XmlNode GetStyleSheet()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "styleSheet")
            {
                return null;
            }
            return file_tag;
        }

        public XmlNode GetStyleSheetOrCreate()
        {
            return GetStyleSheet() ?? CreateStyleSheet();
        }

        private XmlNode CreateStyleSheet()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            XmlElement sst_tag = file_tag.OwnerDocument.CreateElement("styleSheet", ns_manager.LookupNamespace("main"));
            file_tag.AppendChild(sst_tag);
            return sst_tag;
        }

        #region IOrderedSections Members

        public void AppendXmlSection(XmlNode section_node)
        {
            GetStyleSheetOrCreate();
            AppendXmlSection(section_node, sections_names_backward, ns_manager);
        }

        #endregion

        private const string styles_rel_path = "styles.xml";
        private XmlNamespaceManager ns_manager;
        private List<string> sections_names_backward;
    }
}
