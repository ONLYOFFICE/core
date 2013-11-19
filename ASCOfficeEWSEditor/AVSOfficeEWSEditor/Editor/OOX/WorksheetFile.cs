using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.Helpers;

namespace AVSOfficeEWSEditor.Editor.OOX
{
    internal class WorksheetFile : XmlFile, IOrderedSections
    {
        public WorksheetFile(OOX.ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, 
                    OOXNavigate.CreateUniqueFilePath(ws_rel_path_template, workbook.GetRootTag().ParentNode.ParentNode), 
                    Relationships.FileType.worksheet, ContentTypes.FileType.worksheet)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            InitOrderedSections();
        }

        // Called when the file already exists
        public WorksheetFile(string sheet_path, ContentTypes content_types, Relationships relationships, Workbook workbook)
            : base(content_types, relationships, workbook.GetRootTag().ParentNode.ParentNode, sheet_path, 
                    Relationships.FileType.worksheet, ContentTypes.FileType.worksheet)
        {
            ns_manager = new XmlNamespaceManager(workbook.GetRootTag().OwnerDocument.NameTable);
            ns_manager.AddNamespace("main", "http://schemas.openxmlformats.org/spreadsheetml/2006/main");
            InitOrderedSections();
        }

        private void InitOrderedSections()
        {
            sections_names_backward = new List<string>() 
            { "extLst",  "tableParts", "webPublishItems", "controls", "oleObjects", "picture", "drawingHF",
              "drawing", "smartTags", "ignoredErrors", "cellWatches", "customProperties", "colBreaks", "rowBreaks", 
              "headerFooter", "pageSetup", "pageMargins", "printOptions", "hyperlinks","dataValidations", 
              "conditionalFormatting", "phoneticPr", "mergeCells", "customSheetViews", "dataConsolidate", 
              "sortState", "autoFilter", "scenarios", "protectedRanges", "sheetProtection", "sheetCalcPr", 
              "sheetData", "cols", "sheetFormatPr", "sheetViews", "dimension", "sheetPr", };
        }

        public XmlNode GetWorksheetTag()
        {
            XmlNode file_tag = GetRootTag();
            if (file_tag == null || file_tag.LocalName != "worksheet")
            {
                return null;
            }
            return file_tag;
        }

        public XmlNode GetWorksheetTagOrCreate()
        {
            return GetWorksheetTag() ?? CreateWorksheetTag();
        }

        private XmlNode CreateWorksheetTag()
        {
            XmlNode file_tag = GetFileTagOrCreate();
            file_tag.InnerXml = Resource1.empty_sheet; // Creates worksheet from template
            return file_tag.FirstChild;
        }

        #region IOrderedSections Members

        public void AppendXmlSection(XmlNode section_node)
        {
            GetWorksheetTagOrCreate();
            AppendXmlSection(section_node, sections_names_backward, ns_manager);
        }

        #endregion

        const string ws_rel_path_template = "worksheets/sheet.xml";
        XmlNamespaceManager ns_manager;
        private List<string> sections_names_backward;
    }
}
