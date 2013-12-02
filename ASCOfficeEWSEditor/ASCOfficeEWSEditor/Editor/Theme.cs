using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor
{
    internal class Theme
    {
        public Theme(OOX.ContentTypes content_types, OOX.Relationships relationships, Workbook workbook)
        {
            this.workbook = workbook;
            theme_file = new OOX.ThemeFile(content_types, relationships, workbook);
            ns_manager = new XmlNamespaceManager(workbook.GetXmlDocument().NameTable); ;
            ns_manager.AddNamespace("a", "http://schemas.openxmlformats.org/drawingml/2006/main");
        }

        private XmlNode Get_clrScheme_OrCreate()
        {
            XmlNode theme_tag = theme_file.GetThemeOrCreate();
            XmlElement themeElements_xml = (XmlElement)theme_tag.SelectSingleNode("a:themeElements", ns_manager);
            if (themeElements_xml == null)
            {
                themeElements_xml = workbook.GetXmlDocument().CreateElement("themeElements", ns_manager.LookupNamespace("a"));
                XMLSectionsController.AppendXmlSection(themeElements_xml);
            }
            XmlElement clrScheme_xml = (XmlElement)themeElements_xml.SelectSingleNode("a:clrScheme", ns_manager);
            if (clrScheme_xml == null)
            {
                clrScheme_xml = workbook.GetXmlDocument().CreateElement("clrScheme", ns_manager.LookupNamespace("a"));
                clrScheme_xml.SetAttribute("name", "Standard"); // TODO: duplicate names possible.
                themeElements_xml.AppendChild(clrScheme_xml);
            }

            return clrScheme_xml;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index">Real theme element (srgbClr or sysClr) index</param>
        /// <returns>Empty string means the color hasn't been found and shall be set automatic.
        /// Textual string means the color has to be detected by its textual representation.</returns>
        public string GetRGBByColor(int index)
        {
            XmlNode clrScheme_xml = Get_clrScheme_OrCreate();
            XmlNode color_item = clrScheme_xml.SelectSingleNode("*[position() = " + (index + 1).ToString() + "]", ns_manager);
            if (color_item == null)
            {
                return "";
            }
            XmlElement srgbClr_xml = (XmlElement)color_item.SelectSingleNode("a:srgbClr", ns_manager);
            if (srgbClr_xml != null)
            {
                return srgbClr_xml.GetAttribute("val");
            }
            XmlElement sysClr_xml = (XmlElement)color_item.SelectSingleNode("a:sysClr", ns_manager);
            if (sysClr_xml != null)
            {
                string lastClr = sysClr_xml.GetAttribute("lastClr");
                if(lastClr.Length != 0)
                {
                    return lastClr;
                }
                return sysClr_xml.GetAttribute("val"); // Textual value
            }
            return "";
        }

        internal OOX.IOrderedSections XMLSectionsController { get { return theme_file; } }

        private OOX.ThemeFile theme_file;
        #region Tree references
         private Workbook workbook;
        #endregion
        XmlNamespaceManager ns_manager;

    }
}
