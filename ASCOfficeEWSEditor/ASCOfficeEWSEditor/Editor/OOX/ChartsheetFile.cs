using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.Helpers;

namespace ASCOfficeEWSEditor.Editor.OOX
{
    internal class ChartsheetFile : XmlFile, IOrderedSections
    {
        public ChartsheetFile(OOX.ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, 
                    OOXNavigate.CreateUniqueFilePath(cs_rel_path_template, workbook.GetRootTag().ParentNode.ParentNode),
                    Relationships.FileType.chartsheet, ContentTypes.FileType.chartsheet)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            InitOrderedSections();
        }

        // Called when the file already exists
        public ChartsheetFile(string sheet_path, ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, sheet_path,
                    Relationships.FileType.chartsheet, ContentTypes.FileType.chartsheet)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            InitOrderedSections();
        }

        private void InitOrderedSections()
        {
            sections_names_backward = new List<string>() 
            { "extLst",  "webPublishItems", "picture", "drawingHF", "drawing", "headerFooter", 
              "pageSetup", "pageMargins", "customSheetViews", "sheetProtection", "sheetViews", "sheetPr", };
        }

        public XmlNode GetChartsheetTag()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "chartsheet")
            {
                return null;
            }
            return file_tag;
        }

        #region IOrderedSections Members

        public void AppendXmlSection(XmlNode section_node)
        {
            GetChartsheetTag();
            AppendXmlSection(section_node, sections_names_backward, ns_manager);
        }

        #endregion

        const string cs_rel_path_template = "chartsheets/sheet.xml";
        XmlNamespaceManager ns_manager;
        private List<string> sections_names_backward;
    }
}
