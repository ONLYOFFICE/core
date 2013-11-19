using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor
{
    internal class StyleSheet
    {
        public StyleSheet(OOX.ContentTypes content_types, OOX.Relationships relationships, Workbook workbook)
        {
            this.workbook = workbook;
            style_sheet_file = new OOX.StyleSheetFile(content_types, relationships, workbook);
            ns_manager = workbook.NamespaceManager;
            default_font_name = null;
            default_font_size = null;
            Palette = new Format.Colors.Palette(style_sheet_file.GetRootTag(), workbook);
        }

        public XmlNode Get_CellXfs_OrCreate()
        {
            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlNode cellXfs_xml = style_sheet.SelectSingleNode("main:cellXfs", ns_manager);
            if (cellXfs_xml == null)
            {
                cellXfs_xml = workbook.GetXmlDocument().CreateElement("cellXfs", ns_manager.LookupNamespace("main"));
                XMLSectionsController.AppendXmlSection(cellXfs_xml);
            }
            return cellXfs_xml;
        }

        public XmlNode Get_CellStyles_OrCreate()
        {
            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlNode cellStyles_xml = style_sheet.SelectSingleNode("main:cellStyles", ns_manager);
            if (cellStyles_xml == null)
            {
                cellStyles_xml = workbook.GetXmlDocument().CreateElement("cellStyles", ns_manager.LookupNamespace("main"));
                XMLSectionsController.AppendXmlSection(cellStyles_xml);
            }
            return cellStyles_xml;
        }

        public XmlNode Get_CellStyleXfs_OrCreate()
        {
            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlNode cellStyleXfs_xml = style_sheet.SelectSingleNode("main:cellStyleXfs", ns_manager);
            if (cellStyleXfs_xml == null)
            {
                cellStyleXfs_xml = workbook.GetXmlDocument().CreateElement("cellStyleXfs", ns_manager.LookupNamespace("main"));
                XMLSectionsController.AppendXmlSection(cellStyleXfs_xml);
            }
            return cellStyleXfs_xml;
        }

        public XmlNode GetStyleSheetOrCreate()
        {
            return style_sheet_file.GetStyleSheetOrCreate();
        }

        public int GetZeroStyleOrDefaultFontSize()
        {
            if(default_font_size == null)
            {
                int? font_size0 = Get0StyleFontSize();
                if (font_size0 != null)
                {
                    default_font_size = font_size0.Value;
                }
                else
                {
                    default_font_size = workbook.Editor.DefaultFontSizeOnBooksCreation;
                }
            }
            return default_font_size.Value;
        }

        public string GetZeroStyleOrDefaultFontName()
        {
            if (default_font_name == null)
            {
                string font_name0 = Get0StyleFontName();
                if(font_name0 != null)
                {
                    default_font_name = font_name0;
                }
                else
                {
                    default_font_name = workbook.Editor.DefaultFontNameOnBooksCreation;
                }
            }
            return default_font_name;
        }
        
        internal string Get0StyleFontName()
        {
            XmlNode font_entity = Get0StyleFont();
            if (font_entity != null)
            {
                XmlElement name_tag = (XmlElement)font_entity.SelectSingleNode("main:name", workbook.NamespaceManager);
                string name;
                if (name_tag != null && (name = name_tag.GetAttribute("val")) != "")
                {
                    return name;
                }
            }
            return null;
        }

        internal int? Get0StyleFontSize()
        {
            XmlNode font_entity = Get0StyleFont();
            if (font_entity != null)
            {
                XmlElement sz_tag = (XmlElement)font_entity.SelectSingleNode("main:sz", workbook.NamespaceManager);
                int sz;
                if (sz_tag != null && (sz = XMLInt.ParseOrDefault(sz_tag.GetAttribute("val"), -1)) != -1)
                {
                    return sz;
                }
            }
            return null;
        }

        private XmlNode Get0StyleFont()
        {
            XmlNode cell_styles_tag = Get_CellStyles_OrCreate();
            XmlElement cell_0_style = (XmlElement)cell_styles_tag.SelectSingleNode("main:cellStyle[@builtinId = 0]", ns_manager);
            if (cell_0_style != null)
            {
                int xf_id = XMLInt.ParseOrDefault(cell_0_style.GetAttribute("xfId"), -1);
                if(xf_id != -1)
                {
                    XmlNode cell_style_xfs_tag = Get_CellStyleXfs_OrCreate();
                    XmlElement xf_tag = (XmlElement)cell_style_xfs_tag.SelectSingleNode("main:xf[position() = " + (xf_id + 1).ToString() + "]", ns_manager);
                    if(xf_tag != null)
                    {
                        int font_id = XMLInt.ParseOrDefault(xf_tag.GetAttribute("fontId"), -1);
                        if(font_id != -1)
                        {
                            return style_sheet_file.GetRootTag().SelectSingleNode("main:fonts/main:font[position() = " + (font_id + 1).ToString() + "]", ns_manager);
                        }
                    }
                }
            }
            return null;
        }

        internal bool IsNumberFormatIdValidForThisBook(int num_fmt_id)
        {
            if(workbook.Editor.NumberFormatsStorage.FormatStrings.ContainsKey(num_fmt_id))
            {
                return true;
            }
            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlNode numFmt = style_sheet.SelectSingleNode("main:numFmts/main:numFmt[@numFmtId = " + num_fmt_id + "]", ns_manager);
            return numFmt != null;
        }

        internal string GetNumberFormatString(int num_fmt_id)
        {
            string format_string;
            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlElement numFmt = (XmlElement)style_sheet.SelectSingleNode("main:numFmts/main:numFmt[@numFmtId = " + num_fmt_id + "]", ns_manager);
            if(numFmt == null)
            {
                if (workbook.Editor.NumberFormatsStorage.FormatStrings.TryGetValue(num_fmt_id, out format_string))
                {
                    return format_string;
                }
                return "";
            }
            return numFmt.GetAttribute("formatCode");
        }

        internal int SetNumberFormatString(string num_fmt_string)
        {
            int num_fmt_standard_id = workbook.Editor.NumberFormatsStorage.GetIdByCode(num_fmt_string);
            if (num_fmt_standard_id != -1)
            {
                return num_fmt_standard_id;
            }

            XmlNode style_sheet = style_sheet_file.GetStyleSheetOrCreate();
            XmlElement numFmt_exact = (XmlElement)style_sheet.SelectSingleNode("main:numFmts/main:numFmt[@formatCode = \"" + 
                                                                                num_fmt_string.Replace("\"", "&quot;") + "\"]", ns_manager);
            if (numFmt_exact != null)
            {
                int num_fmt_already_stored_id = XMLInt.ParseOrDefault(numFmt_exact.GetAttribute("numFmtId"), -1);
                if(num_fmt_already_stored_id != -1)
                {
                    return num_fmt_already_stored_id;
                }
            }

            int max_id = 0;
            foreach (XmlElement numFmt_tag in style_sheet.SelectNodes("main:numFmts/main:numFmt", ns_manager))
            {
                int num_fmt_id = XMLInt.ParseOrDefault(numFmt_tag.GetAttribute("numFmtId"), -1);
                if(num_fmt_id > max_id)
                {
                    max_id = num_fmt_id;
                }
            }

            if(max_id < 163)
            {
                max_id = 163;
            }
            XmlNode numFmts = style_sheet.SelectSingleNode("main:numFmts", ns_manager);
            if(numFmts == null)
            {
                numFmts = style_sheet.OwnerDocument.CreateElement("numFmts", ns_manager.LookupNamespace("main"));
                XMLSectionsController.AppendXmlSection(numFmts);
            }
            XmlElement numFmt_insert_tag = numFmts.OwnerDocument.CreateElement("numFmt", ns_manager.LookupNamespace("main"));
            numFmt_insert_tag.SetAttribute("numFmtId", (max_id + 1).ToString());
            numFmt_insert_tag.SetAttribute("formatCode", num_fmt_string);
            numFmts.AppendChild(numFmt_insert_tag);

            return max_id + 1;
        }

        public Format.Colors.Palette Palette { get; private set; }
        internal OOX.IOrderedSections XMLSectionsController { get { return style_sheet_file; } }

        private OOX.StyleSheetFile style_sheet_file;
        #region Tree references
         private Workbook workbook;
        #endregion
        XmlNamespaceManager ns_manager;

        private int? default_font_size;
        private string default_font_name;
    }
}
