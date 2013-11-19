using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.OOX
{
    internal class WorkbookFile : XmlFile, IOrderedSections
    {
        public WorkbookFile(ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, workbook_rel_path, 
                    Relationships.FileType.styles, ContentTypes.FileType.styles)
        {
            ns_manager = workbook.NamespaceManager;
            sections_names_backward = new List<string>() 
            { "extLst",  "webPublishObjects", "fileRecoveryPr", "webPublishing", "smartTagTypes", "smartTagPr", "pivotCaches",
              "customWorkbookViews", "oleSize", "calcPr", "definedNames", "externalReferences", "functionGroups", "sheets", 
              "bookViews", "workbookProtection", "workbookPr", "fileSharing", "fileVersion"};

        }

        public XmlNode GetWorkbook()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "styleSheet")
            {
                return null;
            }
            return file_tag;
        }

        public XmlNode GetWorkbookOrCreate()
        {
            return GetWorkbook() ?? CreateWorkbook();
        }

        private XmlNode CreateWorkbook()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            XmlElement sst_tag = file_tag.OwnerDocument.CreateElement("workbook", ns_manager.LookupNamespace("main"));
            file_tag.AppendChild(sst_tag);
            return sst_tag;
        }

        #region IOrderedSections Members

        public void AppendXmlSection(XmlNode section_node)
        {
            GetWorkbookOrCreate();
            AppendXmlSection(section_node, sections_names_backward, ns_manager);
        }

        #endregion

        private const string workbook_rel_path = "workbook.xml";
        private XmlNamespaceManager ns_manager;
        private List<string> sections_names_backward;
    }
}
