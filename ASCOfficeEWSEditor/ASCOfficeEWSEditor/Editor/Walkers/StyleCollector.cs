using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Walkers
{
    /// <summary>
    /// Terms of use:
    /// Entity or Style entity - one of the following tags: <fill/>, <font/>, <border/>
    /// Property - one of an entity properties. For example: <b/>, <left/>, <bgColor/>
    /// </summary>
    internal class StyleCollector : IStatCollector 
    {
        public StyleCollector(Format.IStyleEntity style_entity, Format.IProperty property, Workbook workbook)
        {
            CellXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellXfs_OrCreate();
            CellStyleXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellStyleXfs_OrCreate();
            ns_manager = workbook.NamespaceManager;
            this.property = property;
            this.style_entity = style_entity;

            cell_style_ids = new Dictionary<int, int>();
            entity_ids = new Dictionary<int, int>();
        }


        #region IStatCollector Members

        public bool Collect(int row, int col, XmlElement cell_tag)
        {
            int xf_index = XMLInt.ParseOrDefault(cell_tag.GetAttribute("s"), -1);
            if (xf_index == -1)
            {
                xf_index = XMLInt.ParseOrDefault(((XmlElement)cell_tag.ParentNode).GetAttribute("s"), -1);
                if (xf_index == -1)
                {
                    XmlElement col_tag = (XmlElement)cell_tag.ParentNode.ParentNode.ParentNode.SelectSingleNode(
                        "main:cols/main:col[@min <= " + col + " and @max >= " + col + "]", ns_manager);
                    xf_index = col_tag != null ? XMLInt.ParseOrDefault(col_tag.GetAttribute("style"), 0) : 0;
                }
            }
            return CollectFromCellXFIndex(xf_index);
        }

        public bool Collect(int row, int col, Worksheet parent_sheet)
        {
            XmlElement row_tag = (XmlElement)parent_sheet.SheetData.SelectSingleNode("main:row[@r = " + row + "]", ns_manager);
            int xf_index = row_tag != null ? XMLInt.ParseOrDefault(((XmlElement)row_tag).GetAttribute("s"), -1) : -1;
            if (xf_index == -1)
            {
                XmlElement col_tag = (XmlElement)parent_sheet.SheetData.ParentNode.SelectSingleNode(
                    "main:cols/main:col[@min <= " + col + " and @max >= " + col + "]", ns_manager);
                xf_index = col_tag != null ? XMLInt.ParseOrDefault(col_tag.GetAttribute("style"), 0) : 0;
            }
            return CollectFromCellXFIndex(xf_index);
        }

        #endregion

        private bool CollectFromCellXFIndex(int xf_index)
        {
            if(property.IsNull() && cell_style_ids.Count != 0)
            {
                return false; // this defends the collector from bad walkers that don't stop on returning 'false'
            }
            if (cell_style_ids.ContainsKey(xf_index))
            {
                return true; // continue
            }
            cell_style_ids[xf_index] = 0; // Append a new xf_index to the set

            XmlElement xf_record = GetXFRecord(xf_index);
            int entity_id = style_entity.GetEntityIdByXF(xf_record);
            if (entity_id != -1) // if the entity is not inline
            {
                if (entity_ids.ContainsKey(entity_id))
                {
                    return true; // continue
                }
                entity_ids[entity_id] = 0; // Append a new entity_id to the set
            }
            return style_entity.CollectPropertyFromXF(xf_record, property);
        }

        private XmlElement GetXFRecord(int xfId)
        {
            XmlElement cell_xf = (XmlElement)CellXfs_xml.SelectSingleNode("main:xf[position() = " + (xfId + 1).ToString() + "]", ns_manager);
            if (cell_xf == null) // If the style reference in a cell is wrong
            {
                cell_xf = (XmlElement)CellXfs_xml.SelectSingleNode("main:xf", ns_manager); // select first 'xf'
                if (cell_xf == null) // if no 'xf' records found in <cellXfs>
                {
                    CellXfs_xml.InnerXml = "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xfId=\"0\" xmlns=\"" + 
                                            ns_manager.LookupNamespace("main") + "\"/>";
                    CellXfs_xml.SetAttribute("count", "1");
                    cell_xf = (XmlElement)CellXfs_xml.SelectSingleNode("main:xf", ns_manager); // select first 'xf'
                }
            }
            /* Uncomment if the style info shall be used
            if (!XMLBoolean.ParseOrDefault(cell_xf.GetAttribute(style_entity.GetApplyEntityAttribName()), false))
            {
                int cell_style_xfId = XMLInt.ParseOrDefault(cell_xf.GetAttribute("xfId"), 0);
                cell_xf = (XmlElement)CellStyleXfs_xml.SelectSingleNode("main:xf[position() = " + (cell_style_xfId + 1).ToString() + "]", ns_manager);
                if (cell_xf == null) // if no 'xf' records found in <cellStyleXfs>
                {
                    CellStyleXfs_xml.InnerXml = "<xf numFmtId=\"0\" fontId=\"0\" fillId=\"0\" borderId=\"0\" xmlns=\"" +
                                            ns_manager.LookupNamespace("main") + "\"/>";
                    CellStyleXfs_xml.SetAttribute("count", "1");
                    cell_xf = (XmlElement)CellStyleXfs_xml.SelectSingleNode("main:xf", ns_manager); // select first 'xf'
                }

            }
            */
            return cell_xf;
        }

        private XmlElement CellStyleXfs_xml;
        private XmlElement CellXfs_xml;
        private XmlNamespaceManager ns_manager;
        private Format.IProperty property;
        private Format.IStyleEntity style_entity;
        #region optimization info
        Dictionary<int, int> cell_style_ids;
        Dictionary<int, int> entity_ids;
        #endregion

    }

    class CFont
    {
        public bool m_bIsBold = false;
        public bool m_bIsItalic = false;
        public bool m_bIsUnderline = false;
        public string m_strName = "";
        public int m_nSize = -1;

        public Format.Colors.ColorValue m_oColorValue = null;
    }

    class CFill
    {
        public Format.Fills.PatternType? m_oPatternType = null;
        public Format.Colors.ColorValue m_oColorBg = null;
        public Format.Colors.ColorValue m_oColorFg = null;

        public Format.Fills.GradientType? m_oGradientType = null;
        public Format.Colors.ColorValue m_oColorGradient = null;
    }

    class CBorderElem
    {
        public Format.Borders.BorderStyle? m_oBorderStyle = null;
        public Format.Colors.ColorValue m_oBorderColor = null;
    }
    class CBorder
    {
        public CBorderElem m_oBorderElemLeft = new CBorderElem();
        public CBorderElem m_oBorderElemRight = new CBorderElem();
        public CBorderElem m_oBorderElemTop = new CBorderElem();
        public CBorderElem m_oBorderElemBottom = new CBorderElem();
        public CBorderElem m_oBorderElemDiagonal = new CBorderElem();
    }

    public class StyleCollectorCache
    {

        public StyleCollectorCache(Workbook workbook)
        {
            CellXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellXfs_OrCreate();
            CellStyleXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellStyleXfs_OrCreate();
            ns_manager = workbook.NamespaceManager;
            m_oWorkbook = workbook;
            Init();
        }
        public Format.FormatSimple GetStyle(int nIndex)
        {
            Format.FormatSimple oResult = null;
            if (true == m_mapStyles.TryGetValue(nIndex, out oResult))
                return oResult;
            else
                return null;
        }
        private void Init()
        {
            XmlNodeList cell_xf = CellXfs_xml.SelectNodes("main:xf", ns_manager);
            if (cell_xf != null) // If the style reference in a cell is wrong
            {
                #region LoadFonts
                XmlNode style_sheet_tag = m_oWorkbook.StyleSheet.GetStyleSheetOrCreate();
                string entities_tag_name = "fonts"; // Ex: "fonts"
                XmlNode entities_tag_font = style_sheet_tag.SelectSingleNode("main:" + entities_tag_name, m_oWorkbook.NamespaceManager); // Ex: <fonts>
                if (entities_tag_font == null)
                {
                    entities_tag_font = m_oWorkbook.GetXmlDocument().CreateElement(entities_tag_name, m_oWorkbook.NamespaceManager.LookupNamespace("main")); // Ex: <fonts>
                    style_sheet_tag.AppendChild(entities_tag_font);
                }

                List<CFont> arrFonts = new List<CFont>();

                foreach (XmlNode oXmlNodeChild in entities_tag_font.ChildNodes)
                {
                    CFont oFont = new CFont();
                    foreach (XmlNode oXmlNodeFont in oXmlNodeChild.ChildNodes)
                    {
                        XmlAttribute oAttr = null;
                        switch (oXmlNodeFont.Name)
                        {
                            case "name":
                                oAttr = oXmlNodeFont.Attributes["val"];
                                if (null != oAttr)
                                    oFont.m_strName = oAttr.Value;
                                break;
                            case "sz":
                                oAttr = oXmlNodeFont.Attributes["val"];
                                if (null != oAttr)
                                    oFont.m_nSize = XMLInt.ParseOrDefault(oAttr.Value, -1);
                                break;
                            case "b":
                                oAttr = oXmlNodeFont.Attributes["val"];
                                oFont.m_bIsBold = XMLBoolean.ParseOrDefault((null == oAttr) ? "" : oAttr.Value, true);
                                break;
                            case "i":
                                oAttr = oXmlNodeFont.Attributes["val"];
                                oFont.m_bIsItalic = XMLBoolean.ParseOrDefault((null == oAttr) ? "" : oAttr.Value, true);
                                break;
                            case "u":
                                oAttr = oXmlNodeFont.Attributes["val"];
                                oFont.m_bIsUnderline = XMLBoolean.ParseOrDefault((null == oAttr) ? "" : oAttr.Value, true);
                                break;
                            case "color":
                                string strValueColor = "";
                                oAttr = oXmlNodeFont.Attributes["rgb"];
                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                {
                                    oFont.m_oColorValue = new Format.Colors.ColorValue(strValueColor, true, m_oWorkbook);
                                    break;
                                }

                                oAttr = oXmlNodeFont.Attributes["theme"];
                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                {
                                    oAttr = oXmlNodeFont.Attributes["tint"];
                                    string strValue = (null != oAttr) ? oAttr.Value : "";
                                    double tint = XMLDouble.ParseOrDefault(strValue, 0);
                                    oFont.m_oColorValue = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 1 : 0), tint, true, m_oWorkbook);
                                    break;
                                }

                                oAttr = oXmlNodeFont.Attributes["indexed"];
                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                {
                                    oFont.m_oColorValue = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 64 : 65), true, m_oWorkbook);
                                }
                                break;
                        }
                    }

                    if (-1 == oFont.m_nSize)
                        oFont.m_nSize = m_oWorkbook.StyleSheet.GetZeroStyleOrDefaultFontSize();
                    if ("" == oFont.m_strName)
                        oFont.m_strName = m_oWorkbook.StyleSheet.GetZeroStyleOrDefaultFontName();
                    if (null == oFont.m_oColorValue)
                        oFont.m_oColorValue = new Format.Colors.ColorValue(true, m_oWorkbook);

                    arrFonts.Add(oFont);
                }

                if (0 < cell_xf.Count && 0 == arrFonts.Count)
                {
                    entities_tag_font.InnerXml = Resource1.default_font; // Ex: Create <font>[1]
                    ((XmlElement)entities_tag_font).SetAttribute("count", "1");

                    CFont oFont = new CFont();

                    oFont.m_nSize = 11;
                    oFont.m_strName = "Calibri";
                    oFont.m_oColorValue = new Format.Colors.ColorValue(true, m_oWorkbook);
                    arrFonts.Add(oFont);
                }
                #endregion

                #region LoadFills
                entities_tag_name = "fills"; // Ex: "fonts"
                XmlNode entities_tag_fill = style_sheet_tag.SelectSingleNode("main:" + entities_tag_name, m_oWorkbook.NamespaceManager); // Ex: <fonts>
                if (entities_tag_fill == null)
                {
                    entities_tag_fill = m_oWorkbook.GetXmlDocument().CreateElement(entities_tag_name, m_oWorkbook.NamespaceManager.LookupNamespace("main")); // Ex: <fonts>
                    style_sheet_tag.AppendChild(entities_tag_fill);
                }

                List<CFill> arrFills = new List<CFill>();

                foreach (XmlNode oXmlNodeChild in entities_tag_fill.ChildNodes)
                {
                    CFill oFill = new CFill();

                    XmlAttribute oAttr = null;
                    if (0 != oXmlNodeChild.ChildNodes.Count)
                    {
                        switch (oXmlNodeChild.ChildNodes[0].Name)
                        {
                            case "patternFill":
                                #region PatternFillType
                                {
                                    oAttr = oXmlNodeChild.ChildNodes[0].Attributes["patternType"];
                                    if (null != oAttr)
                                    {
                                        switch (oAttr.Value)
                                        {
                                            case "darkDown":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkDown;
                                                break;
                                            case "darkGray":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkGray;
                                                break;
                                            case "darkGrid":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkGrid;
                                                break;
                                            case "darkHorizontal":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkHorizontal;
                                                break;
                                            case "darkTrellis":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkTrellis;
                                                break;
                                            case "darkUp":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkUp;
                                                break;
                                            case "darkVertical":
                                                oFill.m_oPatternType = Format.Fills.PatternType.darkVertical;
                                                break;
                                            case "gray0625":
                                                oFill.m_oPatternType = Format.Fills.PatternType.gray0625;
                                                break;
                                            case "gray125":
                                                oFill.m_oPatternType = Format.Fills.PatternType.gray125;
                                                break;
                                            case "lightDown":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightDown;
                                                break;
                                            case "lightGray":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightGray;
                                                break;
                                            case "lightGrid":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightGrid;
                                                break;
                                            case "lightHorizontal":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightHorizontal;
                                                break;
                                            case "lightTrellis":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightTrellis;
                                                break;
                                            case "lightUp":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightUp;
                                                break;
                                            case "lightVertical":
                                                oFill.m_oPatternType = Format.Fills.PatternType.lightVertical;
                                                break;
                                            case "mediumGray":
                                                oFill.m_oPatternType = Format.Fills.PatternType.mediumGray;
                                                break;
                                            case "solid":
                                                oFill.m_oPatternType = Format.Fills.PatternType.solid;
                                                break;
                                            case "none":
                                            default:
                                                oFill.m_oPatternType = Format.Fills.PatternType.none;
                                                break;
                                        }
                                    }
                                }
                                #endregion
                                #region Colors
                                foreach (XmlNode oXmlNodeColor in oXmlNodeChild.ChildNodes[0].ChildNodes)
                                {
                                    switch (oXmlNodeColor.Name)
                                    {
                                        case "fgColor":
                                            #region fgColor
                                            {
                                                string strValueColor = "";
                                                oAttr = oXmlNodeColor.Attributes["rgb"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oFill.m_oColorFg = new Format.Colors.ColorValue(strValueColor, true, m_oWorkbook);
                                                    break;
                                                }
                                                oAttr = oXmlNodeColor.Attributes["theme"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oAttr = oXmlNodeColor.Attributes["tint"];
                                                    string strValue = (null != oAttr) ? oAttr.Value : "";

                                                    double tint = XMLDouble.ParseOrDefault(strValue, 0);
                                                    oFill.m_oColorFg = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 1 : 0), tint, true, m_oWorkbook);
                                                    break;
                                                }
                                                oAttr = oXmlNodeColor.Attributes["indexed"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oFill.m_oColorFg = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 64 : 65), true, m_oWorkbook);
                                                    break;
                                                }

                                                oFill.m_oColorFg = new Format.Colors.ColorValue(true, m_oWorkbook);
                                            }
                                            #endregion
                                            break;
                                        case "bgColor":
                                            #region bgColor
                                            {
                                                string strValueColor = "";
                                                oAttr = oXmlNodeColor.Attributes["rgb"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oFill.m_oColorBg = new Format.Colors.ColorValue(strValueColor, false, m_oWorkbook);
                                                    break;
                                                }
                                                oAttr = oXmlNodeColor.Attributes["theme"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oAttr = oXmlNodeColor.Attributes["tint"];
                                                    string strValue = (null != oAttr) ? oAttr.Value : "";

                                                    double tint = XMLDouble.ParseOrDefault(strValue, 0);
                                                    oFill.m_oColorBg = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, false ? 1 : 0), tint, false, m_oWorkbook);
                                                    break;
                                                }
                                                oAttr = oXmlNodeColor.Attributes["indexed"];
                                                if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                                {
                                                    oFill.m_oColorBg = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, false ? 64 : 65), false, m_oWorkbook);
                                                    break;
                                                }

                                                oFill.m_oColorBg = new Format.Colors.ColorValue(false, m_oWorkbook);
                                            }
                                            #endregion
                                            break;
                                    }
                                }
                                #endregion
                                break;
                            case "gradientFill":
                                #region gradientFillType
                                {
                                    oAttr = oXmlNodeChild.ChildNodes[0].Attributes["type"];
                                    if (null != oAttr)
                                    {
                                        switch (oAttr.Value)
                                        {
                                            case "path":
                                                oFill.m_oGradientType = Format.Fills.GradientType.path;
                                                break;
                                            case "linear":
                                            default:
                                                oFill.m_oGradientType = Format.Fills.GradientType.linear;
                                                break;
                                        }
                                    }
                                }
                                #endregion
                                #region Color
                                {
                                    if (1 < oXmlNodeChild.ChildNodes[0].ChildNodes.Count && "stop" == oXmlNodeChild.ChildNodes[0].ChildNodes[0].Name &&
                                            1 <= oXmlNodeChild.ChildNodes[0].ChildNodes[0].ChildNodes.Count)
                                    {
                                        XmlNode oXmlNodeColor = oXmlNodeChild.ChildNodes[0].ChildNodes[0].ChildNodes[0];
                                        string strValueColor = "";
                                        oAttr = oXmlNodeColor.Attributes["rgb"];
                                        if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                        {
                                            oFill.m_oColorGradient = new Format.Colors.ColorValue(strValueColor, true, m_oWorkbook);
                                            break;
                                        }
                                        oAttr = oXmlNodeColor.Attributes["theme"];
                                        if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                        {
                                            oAttr = oXmlNodeColor.Attributes["tint"];
                                            string strValue = (null != oAttr) ? oAttr.Value : "";

                                            double tint = XMLDouble.ParseOrDefault(strValue, 0);
                                            oFill.m_oColorGradient = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 1 : 0), tint, true, m_oWorkbook);
                                            break;
                                        }
                                        oAttr = oXmlNodeColor.Attributes["indexed"];
                                        if (null != oAttr && (strValueColor = oAttr.Value) != "")
                                        {
                                            oFill.m_oColorGradient = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 64 : 65), true, m_oWorkbook);
                                            break;
                                        }

                                        oFill.m_oColorGradient = new Format.Colors.ColorValue(true, m_oWorkbook);
                                    }
                                }
                                #endregion
                                break;
                        }
                    }

                    arrFills.Add(oFill);
                }

                if (0 < cell_xf.Count && 0 == arrFills.Count)
                {
                    entities_tag_fill.InnerXml = Resource1.default_fill; // Ex: Create <font>[1]
                    ((XmlElement)entities_tag_fill).SetAttribute("count", "1");

                    CFill oFill = new CFill();
                    arrFills.Add(oFill);
                }
                #endregion

                #region LoadBorders
                entities_tag_name = "borders"; // Ex: "fonts"
                XmlNode entities_tag_border = style_sheet_tag.SelectSingleNode("main:" + entities_tag_name, m_oWorkbook.NamespaceManager); // Ex: <fonts>
                if (entities_tag_border == null)
                {
                    entities_tag_border = m_oWorkbook.GetXmlDocument().CreateElement(entities_tag_name, m_oWorkbook.NamespaceManager.LookupNamespace("main")); // Ex: <fonts>
                    style_sheet_tag.AppendChild(entities_tag_border);
                }

                List<CBorder> arrBorders = new List<CBorder>();

                foreach (XmlNode oXmlNodeChild in entities_tag_border.ChildNodes)
                {
                    CBorder oBorder = new CBorder();
                    XmlAttribute oAttr = null;
                    foreach (XmlNode oXmlNodeBorder in oXmlNodeChild.ChildNodes)
                    {
                        oAttr = oXmlNodeBorder.Attributes["style"];
                        XmlNode oColor = (0 == oXmlNodeBorder.ChildNodes.Count) ? null : oXmlNodeBorder.ChildNodes[0];
                        string strStyle = (null != oAttr) ? oAttr.Value : "";
                        switch (oXmlNodeBorder.Name)
                        {
                            case "left":
                                #region LeftBorder
                                {
                                    SetBorderStyle(ref oBorder.m_oBorderElemLeft.m_oBorderStyle, strStyle);
                                    if (null != oColor)
                                        ReadColor(ref oColor, ref oBorder.m_oBorderElemLeft.m_oBorderColor);
                                    else
                                        oBorder.m_oBorderElemLeft.m_oBorderColor = new Format.Colors.ColorValue(true, m_oWorkbook);
                                }
                                #endregion
                                break;
                            case "right":
                                #region RightBorder
                                {
                                    SetBorderStyle(ref oBorder.m_oBorderElemRight.m_oBorderStyle, strStyle);
                                    if (null != oColor)
                                        ReadColor(ref oColor, ref oBorder.m_oBorderElemRight.m_oBorderColor);
                                    else
                                        oBorder.m_oBorderElemRight.m_oBorderColor = new Format.Colors.ColorValue(true, m_oWorkbook);
                                }
                                #endregion
                                break;
                            case "top":
                                #region TopBorder
                                {
                                    SetBorderStyle(ref oBorder.m_oBorderElemTop.m_oBorderStyle, strStyle);
                                    if (null != oColor)
                                        ReadColor(ref oColor, ref oBorder.m_oBorderElemTop.m_oBorderColor);
                                    else
                                        oBorder.m_oBorderElemTop.m_oBorderColor = new Format.Colors.ColorValue(true, m_oWorkbook);
                                }
                                #endregion
                                break;
                            case "bottom":
                                #region BottomBorder
                                {
                                    SetBorderStyle(ref oBorder.m_oBorderElemBottom.m_oBorderStyle, strStyle);
                                    if (null != oColor)
                                        ReadColor(ref oColor, ref oBorder.m_oBorderElemBottom.m_oBorderColor);
                                    else
                                        oBorder.m_oBorderElemBottom.m_oBorderColor = new Format.Colors.ColorValue(true, m_oWorkbook);
                                }
                                #endregion
                                break;
                            case "diagonal":
                                #region DiagonalBorder
                                {
                                    SetBorderStyle(ref oBorder.m_oBorderElemDiagonal.m_oBorderStyle, strStyle);
                                    if (null != oColor)
                                        ReadColor(ref oColor, ref oBorder.m_oBorderElemDiagonal.m_oBorderColor);
                                    else
                                        oBorder.m_oBorderElemDiagonal.m_oBorderColor = new Format.Colors.ColorValue(true, m_oWorkbook);
                                }
                                #endregion
                                break;
                        }
                    }
                    arrBorders.Add(oBorder);
                }

                if (0 < cell_xf.Count && 0 == arrBorders.Count)
                {
                    entities_tag_border.InnerXml = Resource1.default_border; // Ex: Create <font>[1]
                    ((XmlElement)entities_tag_border).SetAttribute("count", "1");

                    CBorder oBorder = new CBorder();
                    arrBorders.Add(oBorder);
                }
                #endregion

                for (int i = 0; i < cell_xf.Count; i++)
                {
                    Format.FormatSimple oNewStyleItem = new Format.FormatSimple(m_oWorkbook);

                    XmlNode oCurNode = cell_xf.Item(i);
                    XmlAttribute oAttributeTmp = oCurNode.Attributes["fontId"];
                    string strFontId = (null != oAttributeTmp) ? oAttributeTmp.Value : "";
                    int nFontId = XMLInt.ParseOrDefault(strFontId, 0);

                    oAttributeTmp = oCurNode.Attributes["fillId"];
                    string strFillId = (null != oAttributeTmp) ? oAttributeTmp.Value : "";
                    int nFillId = XMLInt.ParseOrDefault(strFillId, 0);

                    oAttributeTmp = oCurNode.Attributes["numFmtId"];
                    string strFormatId = (null != oAttributeTmp) ? oAttributeTmp.Value : "";
                    int nFormatId = XMLInt.ParseOrDefault(strFormatId, 0);

                    oAttributeTmp = oCurNode.Attributes["borderId"];
                    string strBorderId = (null != oAttributeTmp) ? oAttributeTmp.Value : "";
                    int nBorderId = XMLInt.ParseOrDefault(strBorderId, 0);

                    string strHorizontal = "";
                    string strVertical = "";
                    bool bWrapText = false;
                    foreach(XmlNode oXmlNodeChildXF in oCurNode.ChildNodes)
                    {
                        if ("alignment" == oXmlNodeChildXF.Name)
                        {
                            oAttributeTmp = oXmlNodeChildXF.Attributes["horizontal"];
                            if (null != oAttributeTmp)
                                strHorizontal = oAttributeTmp.Value;

                            oAttributeTmp = oXmlNodeChildXF.Attributes["vertical"];
                            if (null != oAttributeTmp)
                                strVertical = oAttributeTmp.Value;

                            oAttributeTmp = oXmlNodeChildXF.Attributes["wrapText"];
                            if (null != oAttributeTmp)
                                bWrapText = XMLBoolean.ParseOrDefault(oAttributeTmp.Value, false);
                            break;
                        }
                    }

                    if (arrFonts.Count <= nFontId)
                        nFontId = 0;
                    if (arrFills.Count <= nFillId)
                        nFillId = 0;
                    if (arrBorders.Count <= nBorderId)
                        nBorderId = 0;
                    CFont oFont = arrFonts[nFontId];
                    CFill oFill = arrFills[nFillId];
                    CBorder oBorder = arrBorders[nBorderId];

                    #region Font
                    oNewStyleItem.Font.Bold = oFont.m_bIsBold;
                    oNewStyleItem.Font.Italic = oFont.m_bIsItalic;
                    oNewStyleItem.Font.Underline = oFont.m_bIsUnderline;
                    oNewStyleItem.Font.Size = oFont.m_nSize;
                    oNewStyleItem.Font.Name = oFont.m_strName;

                    Format.Colors.ColorProperty oColorProp = new Format.Colors.FgColorProperty(oFont.m_oColorValue, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Font.Color, oColorProp);
                    #endregion
                    #region Alignment
                    switch (strVertical)
                    {
                        case "center":
                            oNewStyleItem.Alignment.Vertical = Format.Alignment.VerticalAlignmentType.center;
                            break;
                        case "distributed":
                            oNewStyleItem.Alignment.Vertical = Format.Alignment.VerticalAlignmentType.distributed;
                            break;
                        case "justify":
                            oNewStyleItem.Alignment.Vertical = Format.Alignment.VerticalAlignmentType.justify;
                            break;
                        case "top":
                            oNewStyleItem.Alignment.Vertical = Format.Alignment.VerticalAlignmentType.top;
                            break;
                        case "bottom":
                        default:
                            oNewStyleItem.Alignment.Vertical = Format.Alignment.VerticalAlignmentType.bottom;
                            break;
                    }
                    switch (strHorizontal)
                    {
                        case "center":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.center;
                            break;
                        case "centerContinuous":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.centerContinuous;
                            break;
                        case "distributed":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.distributed;
                            break;
                        case "fill":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.fill;
                            break;
                        case "justify":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.justify;
                            break;
                        case "left":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.left;
                            break;
                        case "right":
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.right;
                            break;
                        case "general":
                        default:
                            oNewStyleItem.Alignment.Horizontal = Format.Alignment.HorizontalAlignmentType.general;
                            break;
                    }
                    oNewStyleItem.Alignment.WrapText = bWrapText;
                    #endregion
                    #region Fill
                    oNewStyleItem.Fills.PatternFill.Pattern = oFill.m_oPatternType;
                    Format.Colors.ColorProperty oColorFgProp = new Format.Colors.FgColorProperty(oFill.m_oColorFg, true, "fgColor", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Fills.PatternFill.FgColor, oColorFgProp);
                    Format.Colors.ColorProperty oColorBgProp = new Format.Colors.FgColorProperty(oFill.m_oColorBg, false, "bgColor", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Fills.PatternFill.BgColor, oColorBgProp);

                    oNewStyleItem.Fills.GradientFill.Type = oFill.m_oGradientType;
                    Format.Colors.ColorProperty oColorGradientProp = new Format.Colors.FgColorProperty(oFill.m_oColorGradient, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Fills.GradientFill.Color, oColorGradientProp);
                    #endregion
                    #region NumberFormat
                    oNewStyleItem.NumberFormatId = nFormatId;
                    #endregion
                    #region Borders
                    oNewStyleItem.Brds.Left.Style = oBorder.m_oBorderElemLeft.m_oBorderStyle;
                    oNewStyleItem.Brds.Top.Style = oBorder.m_oBorderElemTop.m_oBorderStyle;
                    oNewStyleItem.Brds.Right.Style = oBorder.m_oBorderElemRight.m_oBorderStyle;
                    oNewStyleItem.Brds.Bottom.Style = oBorder.m_oBorderElemBottom.m_oBorderStyle;
                    Format.Colors.ColorProperty oBorderPropColorLeft = new Format.Colors.FgColorProperty(oBorder.m_oBorderElemLeft.m_oBorderColor, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Brds.Left.Color, oBorderPropColorLeft);
                    Format.Colors.ColorProperty oBorderPropColorTop = new Format.Colors.FgColorProperty(oBorder.m_oBorderElemTop.m_oBorderColor, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Brds.Top.Color, oBorderPropColorTop);
                    Format.Colors.ColorProperty oColorGradientRight = new Format.Colors.FgColorProperty(oBorder.m_oBorderElemRight.m_oBorderColor, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Brds.Right.Color, oColorGradientRight);
                    Format.Colors.ColorProperty oColorGradientBottom = new Format.Colors.FgColorProperty(oBorder.m_oBorderElemBottom.m_oBorderColor, true, "color", m_oWorkbook);
                    SetColor(ref oNewStyleItem.Brds.Bottom.Color, oColorGradientBottom);
                    #endregion

                    m_mapStyles.Add(i, oNewStyleItem);
                }

                /*for (int i = 0; i < cell_xf.Count; i++)
                {
                    XmlElement oCurNode = (XmlElement)cell_xf.Item(i);
                    Format.FormatSimple oNewStyleItem = new Format.FormatSimple(m_oWorkbook, oCurNode);
                    m_mapStyles.Add( i, oNewStyleItem );
                }*/
            }
        }

        private void SetColor(ref Format.Colors.Color oColor, Format.Colors.ColorProperty oColorProp)
        {
            if (oColorProp.IsNull())
            {
                oColorProp.Value = new Format.Colors.FgColor(m_oWorkbook);
                oColor = new Format.Colors.Color(null, null, null, oColorProp, m_oWorkbook);
            }
            else oColor = new Format.Colors.Color(oColorProp.Value, null, null, oColorProp, m_oWorkbook);
        }

        private void SetBorderStyle(ref Format.Borders.BorderStyle? oBorderStyle, string strBorderStyle)
        {
            switch (strBorderStyle)
            {
                case "dashDot":
                    oBorderStyle = Format.Borders.BorderStyle.dashDot;
                    break;
                case "dashDotDot":
                    oBorderStyle = Format.Borders.BorderStyle.dashDotDot;
                    break;
                case "dashed":
                    oBorderStyle = Format.Borders.BorderStyle.dashed;
                    break;
                case "dotted":
                    oBorderStyle = Format.Borders.BorderStyle.dotted;
                    break;
                case "double":
                    oBorderStyle = Format.Borders.BorderStyle.doubleLine;
                    break;
                case "hair":
                    oBorderStyle = Format.Borders.BorderStyle.hair;
                    break;
                case "medium":
                    oBorderStyle = Format.Borders.BorderStyle.medium;
                    break;
                case "mediumDashDot":
                    oBorderStyle = Format.Borders.BorderStyle.mediumDashDot;
                    break;
                case "mediumDashDotDot":
                    oBorderStyle = Format.Borders.BorderStyle.mediumDashDotDot;
                    break;
                case "mediumDashed":
                    oBorderStyle = Format.Borders.BorderStyle.mediumDashed;
                    break;
                case "slantDashDot":
                    oBorderStyle = Format.Borders.BorderStyle.slantDashDot;
                    break;
                case "thick":
                    oBorderStyle = Format.Borders.BorderStyle.thick;
                    break;
                case "thin":
                    oBorderStyle = Format.Borders.BorderStyle.thin;
                    break;
                case "none":
                default:
                    oBorderStyle = Format.Borders.BorderStyle.none;
                    break;
            }
        }

        private void ReadColor(ref XmlNode oXmlNode, ref Format.Colors.ColorValue oColorValue)
        {
            string strValueColor = "";
            XmlAttribute oAttr = oXmlNode.Attributes["rgb"];
            if (null != oAttr && (strValueColor = oAttr.Value) != "")
            {
                oColorValue = new Format.Colors.ColorValue(strValueColor, true, m_oWorkbook);
                return;
            }
            oAttr = oXmlNode.Attributes["theme"];
            if (null != oAttr && (strValueColor = oAttr.Value) != "")
            {
                oAttr = oXmlNode.Attributes["tint"];
                string strValue = (null != oAttr) ? oAttr.Value : "";

                double tint = XMLDouble.ParseOrDefault(strValue, 0);
                oColorValue = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 1 : 0), tint, true, m_oWorkbook);
                return;
            }
            oAttr = oXmlNode.Attributes["indexed"];
            if (null != oAttr && (strValueColor = oAttr.Value) != "")
            {
                oColorValue = new Format.Colors.ColorValue(XMLInt.ParseOrDefault(strValueColor, true ? 64 : 65), true, m_oWorkbook);
                return;
            }

            oColorValue = new Format.Colors.ColorValue(true, m_oWorkbook);
        }

        private Dictionary<int, Format.FormatSimple> m_mapStyles = new Dictionary<int, Format.FormatSimple>();
        private XmlElement CellStyleXfs_xml;
        private XmlElement CellXfs_xml;
        private XmlNamespaceManager ns_manager;
        private Workbook m_oWorkbook;

    }
}
