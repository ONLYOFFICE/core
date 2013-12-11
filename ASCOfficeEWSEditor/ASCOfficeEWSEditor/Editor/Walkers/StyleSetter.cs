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
    public class StyleSetter : ICellSetter, IRowsSetter, IColsSetter
    {
        public StyleSetter(Format.IStyleEntity style_entity, Format.IProperty property, Workbook workbook)
        {
            this.workbook = workbook;
            CellXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellXfs_OrCreate();
            CellStyleXfs_xml = (XmlElement)workbook.StyleSheet.Get_CellStyleXfs_OrCreate();
            ns_manager = workbook.NamespaceManager;
            this.property = property;
            this.style_entity = style_entity;

            cell_style_ids = new Dictionary<int, int>();
            entity_ids = new Dictionary<int, int>();
        }


        #region ICellSetter Members

        public bool Set(int row, int col, XmlElement cell_tag)
        {
            int xf_index = XMLInt.ParseOrDefault(cell_tag.GetAttribute("s"), 0);
            int xf_new_index = GetSuitableXFIndex(xf_index);
            cell_tag.SetAttribute("s", xf_new_index.ToString());
            return true;
        }

        public void PostProcessing()
        {
             // do nothing
        }

        #endregion

        #region IRowsSetter Members

        public bool Set(XmlElement row_tag, int row_num)
        {
            int xf_index = XMLInt.ParseOrDefault(row_tag.GetAttribute("s"), 0);
            int xf_new_index = GetSuitableXFIndex(xf_index);
            row_tag.SetAttribute("s", xf_new_index.ToString());
            return true;
        }

        #endregion

        #region IColsSetter Members

        public bool WouldLeftTheSame(XmlElement col_tag)
        {
            int xf_index = XMLInt.ParseOrDefault(col_tag.GetAttribute("style"), 0);
            int xf_new_index = GetSuitableXFIndex(xf_index);
            return xf_index == xf_new_index;
        }

        public void Set(XmlElement col_tag)
        {
            int xf_index = XMLInt.ParseOrDefault(col_tag.GetAttribute("style"), 0);
            col_tag.SetAttribute("style", GetSuitableXFIndex(xf_index).ToString());
        }

        #endregion

        
        private int GetSuitableXFIndex(int xf_index)
        {
            if (cell_style_ids.ContainsKey(xf_index))
            {
                return cell_style_ids[xf_index]; // return already calculated
            }

            XmlElement xf_record = GetXFRecord(xf_index);
            XmlElement xf_record_clonned = (XmlElement)xf_record.Clone();
            int entity_id = style_entity.GetEntityIdByXF(xf_record);
            int changed_entity_position;
            if (entity_ids.ContainsKey(entity_id) && entity_id != -1)
            {
                changed_entity_position = entity_ids[entity_id]; // return already calculated
            }
            else
            {
                changed_entity_position = style_entity.ApplyProperty(xf_record_clonned, property);
            }

            entity_ids[entity_id] = changed_entity_position; // Append a new entity_id to the set
            if (changed_entity_position == entity_id && entity_id != -1)
            {
                cell_style_ids[xf_index] = xf_index; // Append a new xf_index to the set
                return xf_index;
            }

            style_entity.SetEntityIdToXF(xf_record_clonned, changed_entity_position);

            
            XmlNode cellXfs_tag = workbook.StyleSheet.Get_CellXfs_OrCreate();
            XmlNodeList xfs = cellXfs_tag.SelectNodes("main:xf", workbook.NamespaceManager);
            int xf_new_position = 0;
            foreach (XmlElement xf in xfs)
            {
                if (CompareXFs(xf_record_clonned, xf))
                {
                    break;
                }
                ++xf_new_position;
            }
            if (xf_new_position == xfs.Count)
            {
                cellXfs_tag.AppendChild(xf_record_clonned);
                ((XmlElement)cellXfs_tag).RemoveAttribute("count");

            }
            cell_style_ids[xf_index] = xf_new_position; // Append a new xf_index to the set
            return xf_new_position;

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
            return cell_xf;
        }


        private bool CompareXFs(XmlElement xf1, XmlElement xf2)
        {
            if (xf1.GetAttribute("fontId") != xf2.GetAttribute("fontId") ||
                xf1.GetAttribute("fillId") != xf2.GetAttribute("fillId") ||
                xf1.GetAttribute("borderId") != xf2.GetAttribute("borderId") ||
                xf1.GetAttribute("numFmtId") != xf2.GetAttribute("numFmtId") ||
                xf1.GetAttribute("xfId") != xf2.GetAttribute("xfId"))
            {
                return false;
            }
            if (XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyAlignment"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyAlignment"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyBorder"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyBorder"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyFill"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyFill"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyFont"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyFont"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyNumberFormat"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyNumberFormat"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("applyProtection"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("applyProtection"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("pivotButton"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("pivotButton"), false) ||
                XMLBoolean.ParseOrDefault(xf1.GetAttribute("quotePrefix"), false) != XMLBoolean.ParseOrDefault(xf2.GetAttribute("quotePrefix"), false)
                )
            {
                return false;
            }
            Format.Alignment.AlignmentEntity alignment_comparer = new Format.Alignment.AlignmentEntity(workbook);
            Format.Protection.ProtectionEntity protection_comparer = new Format.Protection.ProtectionEntity(workbook);
            if (!alignment_comparer.CompareEntities((XmlElement)alignment_comparer.GetEntityTagByXF(xf1), (XmlElement)alignment_comparer.GetEntityTagByXF(xf2)) ||
                !protection_comparer.CompareEntities((XmlElement)protection_comparer.GetEntityTagByXF(xf1), (XmlElement)protection_comparer.GetEntityTagByXF(xf2)))
            {
                return false;
            }
            
            return true;
        }


        private Workbook workbook;
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

}
