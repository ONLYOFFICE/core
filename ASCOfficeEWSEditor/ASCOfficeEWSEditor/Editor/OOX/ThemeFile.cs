using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class ThemeFile : XmlFile, IOrderedSections
    {
        public ThemeFile(ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, theme_rel_path, 
                    Relationships.FileType.theme, ContentTypes.FileType.theme)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetXmlDocument().NameTable); ;
            ns_manager.AddNamespace("a", "http://schemas.openxmlformats.org/drawingml/2006/main");
            sections_names_backward = new List<string>() 
            { "extLst",  "custClrLst", "extraClrSchemeLst", "objectDefaults", "themeElements"};
        }

        public XmlNode GetTheme()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "theme")
            {
                return null;
            }
            return file_tag;
        }

        public XmlNode GetThemeOrCreate()
        {
            return GetTheme() ?? CreateTheme();
        }

        private XmlNode CreateTheme()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            XmlElement theme_tag = file_tag.OwnerDocument.CreateElement("theme", ns_manager.LookupNamespace("a"));
            file_tag.AppendChild(theme_tag);
            return theme_tag;
        }

        #region IOrderedSections Members

        public void AppendXmlSection(XmlNode section_node)
        {
            GetThemeOrCreate();
            AppendXmlSection(section_node, sections_names_backward, ns_manager);
        }

        #endregion

        const string theme_rel_path = "theme/theme1.xml";
        XmlNamespaceManager ns_manager;
        private List<string> sections_names_backward;
    }
}
