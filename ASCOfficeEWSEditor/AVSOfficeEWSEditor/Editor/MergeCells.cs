using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor
{
    internal class MergeCells
    {
        internal MergeCells(Worksheet worksheet, XmlNode xml_worksheet)
        {
            this.worksheet = worksheet;
            this.xml_worksheet = xml_worksheet;
            ns_manager = worksheet.Workbook.NamespaceManager;

            mergeCells_xml = xml_worksheet.SelectSingleNode("main:mergeCells", ns_manager);
        }

        internal void CalculateMerge()
        {
            if (m_bIsCalculateMerge)
                return;
            if (null == mergeCells_xml)
            {
                m_bIsCalculateMerge = true;
                return;
            }

            foreach (XmlNode oXmlNodeChild in mergeCells_xml.ChildNodes)
            {
                try
                {
                    Cells.CellRange oCellRange = new Cells.CellRange(oXmlNodeChild.Attributes["ref"].Value, worksheet.Workbook);
                    m_arrMergedCells.Add(oCellRange);
                }
                catch { }
            }

            m_bIsCalculateMerge = true;
        }

        internal List<string> GetMergedAreas()
        {
            List<string> list = new List<string>();

            CalculateMerge();

            foreach (Cells.CellRange oRange in m_arrMergedCells)
            {
                list.Add(oRange.ToString());
            }
            return list;
        }

        internal List<Cells.CellRange> GetMergedAreas2()
        {
            CalculateMerge();

            return new List<AVSOfficeEWSEditor.Editor.Cells.CellRange>(m_arrMergedCells);
        }

        internal Cells.CellRange MergedArea(Cells.Cell cell)
        {
            CalculateMerge();

            foreach (Cells.CellRange oRange in m_arrMergedCells)
            {
                if (oRange.Contains(cell))
                    return oRange;
            }

            return new Cells.CellRange(cell);
        }

        public MergeInfo HasMergedCells(Cells.CellRange selected_range)
        {
            CalculateMerge();

            foreach (Cells.CellRange oRange in m_arrMergedCells)
            {
                if (oRange.IsIntersectedWith(selected_range))
                {
                    if (1 == selected_range.NumCells && oRange.RowFirst == selected_range.RowFirst &&
                        oRange.ColumnFirst == selected_range.ColumnFirst)
                        return MergeInfo.firstmerge;

                    return MergeInfo.merge;
                }
            }

            return MergeInfo.none;
        }

        public void RemoveRange(Cells.CellRange removed_range, Cells.CellRange moved_range, Cells.Offset offset)
        {
            for (int nIndexCell = 0; nIndexCell < m_arrMergedCells.Count; ++nIndexCell)
            {
                Cells.CellRange oRange = m_arrMergedCells[nIndexCell];
                bool bIsEdit = false;
                if (oRange.IsIntersectedWith(removed_range))
                {
                    oRange = oRange.Cut(removed_range);
                    if (oRange == Cells.CellRange.Empty || oRange.IsSingleCell)
                    {
                        RemoveMergeCell(nIndexCell);
                        --nIndexCell;
                        continue;
                    }
                    else
                        bIsEdit = true;
                }
                if (moved_range.Contains(oRange.BottomRight))
                {
                    if (!(oRange.IsSetOfColumns && offset.RowOffset != 0 || oRange.IsSetOfRows && offset.ColumnOffset != 0))
                    {
                        oRange += offset;
                        bIsEdit = true;
                    }
                }

                if (bIsEdit)
                {
                    m_arrMergedCells[nIndexCell] = oRange;
                    ((XmlElement)mergeCells_xml.ChildNodes[nIndexCell]).SetAttribute("ref", oRange.ToString());
                }
            }
        }

        public void InsertRange(Cells.CellRange moved_range, Cells.Offset offset)
        {
            for (int nIndexCell = 0; nIndexCell < m_arrMergedCells.Count; ++nIndexCell)
            {
                Cells.CellRange oRange = m_arrMergedCells[nIndexCell];
                if (oRange.IsBoundaryIntersectedWith(moved_range))
                {
                    oRange = oRange.Resize(offset);
                }
                if (moved_range.Contains(oRange.BottomRight))
                {
                    if (!(oRange.IsSetOfColumns && offset.RowOffset != 0 || oRange.IsSetOfRows && offset.ColumnOffset != 0))
                    {
                        oRange += offset;
                    }
                }

                m_arrMergedCells[nIndexCell] = oRange;
                ((XmlElement)mergeCells_xml.ChildNodes[nIndexCell]).SetAttribute("ref", oRange.ToString());
            }
        }

        public void UnMerge(Cells.CellRange selected_range)
        {
            CalculateMerge();

            for (int nIndexCell = 0; nIndexCell < m_arrMergedCells.Count; ++nIndexCell)
            {
                Cells.CellRange oRange = m_arrMergedCells[nIndexCell];
                if (oRange.IsIntersectedWith(selected_range))
                {
                    RemoveMergeCell(nIndexCell);
                    --nIndexCell;
                }
            }
        }

        /// <summary>
        /// Merges the specified range
        /// </summary>
        /// <param name="selected_range"></param>
        /// <returns>Resulting range that may differ from the original if the original intersects with another merged range</returns>
        public Cells.CellRange Merge(Cells.CellRange selected_range)
        {
            CalculateMerge();

            Cells.CellRange current_range = selected_range;
            bool start_from_the_beginning = true;
            while (start_from_the_beginning)
            {
                start_from_the_beginning = false;
                int nIndexCell = 0;
                foreach (Cells.CellRange oRange in m_arrMergedCells)
                {
                    if (oRange.IsIntersectedWith(current_range))
                    {
                        RemoveMergeCell(nIndexCell);
                        current_range = current_range.BoundingBox(oRange);
                        start_from_the_beginning = true;
                        break;
                    }
                    ++nIndexCell;
                }
            }
            AddMergeCell(current_range);
            return current_range;
        }

        private void AddMergeCell(Cells.CellRange appended_range)
        {
            if (mergeCells_xml == null)
            {
                mergeCells_xml = xml_worksheet.OwnerDocument.CreateElement("mergeCells", ns_manager.LookupNamespace("main"));
                worksheet.XMLSectionsController.AppendXmlSection(mergeCells_xml);
            }

            XmlElement new_mergeCell_tag = mergeCells_xml.OwnerDocument.CreateElement("mergeCell", ns_manager.LookupNamespace("main"));
            new_mergeCell_tag.SetAttribute("ref", appended_range.ToString());
            mergeCells_xml.AppendChild(new_mergeCell_tag);
            ((XmlElement)mergeCells_xml).RemoveAttribute("count");

            m_arrMergedCells.Add(appended_range);
        }

        private void RemoveMergeCell(int nIndexCell)
        {
            m_arrMergedCells.RemoveAt(nIndexCell);
            if (mergeCells_xml != null)
            {
                XmlNode mergeCell_tag = mergeCells_xml.ChildNodes[nIndexCell];
                mergeCell_tag.ParentNode.RemoveChild(mergeCell_tag);
                if (0 == m_arrMergedCells.Count)
                {
                    xml_worksheet.RemoveChild(mergeCells_xml);
                    mergeCells_xml = null;
                }
                else
                {
                    ((XmlElement)mergeCells_xml).RemoveAttribute("count");
                }
            }
        }

        public XmlNode MergeCellsXml
        {
            get { return mergeCells_xml; }
            set { mergeCells_xml = value.Clone(); }
        }

        private XmlNode mergeCells_xml;
        private XmlNode xml_worksheet;

        private Worksheet worksheet;
        private XmlNamespaceManager ns_manager;

        private List<Cells.CellRange> m_arrMergedCells = new List<AVSOfficeEWSEditor.Editor.Cells.CellRange>();
        private bool m_bIsCalculateMerge = false;
    }
}
