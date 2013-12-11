using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Hyperlinks
{
    /// <summary>
    /// Collection of hyperlinks contained in a worksheet
    /// </summary>
    public class Hyperlinks : IEnumerable<Hyperlink>
    {
        internal Hyperlinks(Worksheet parent_sheet)
        {
            this.parent_sheet = parent_sheet;
            ns_manager = parent_sheet.Workbook.NamespaceManager;

            hyperlinks_xml = parent_sheet.SheetData.ParentNode.SelectSingleNode("main:hyperlinks", ns_manager);
            hyperlink_items = new Dictionary<Cells.Cell, Calculation.HyperlinkItem>();
            if (hyperlinks_xml != null)
            {
                foreach (XmlElement hyperlink_xml in hyperlinks_xml.SelectNodes("main:hyperlink", ns_manager))
                {
                    RegisterHyperlinkItem(new Cells.CellRange(hyperlink_xml.GetAttribute("ref"), parent_sheet.Workbook).TopLeft);
                }
            }
        }

        #region IEnumerable<Hyperlink> Members

        /// <summary>
        /// Enumerator
        /// </summary>
        /// <returns>Enumerator</returns>
        public IEnumerator<Hyperlink> GetEnumerator()
        {
            return new Enumerator(this);
        }

        #endregion

        #region IEnumerable Members

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return new Enumerator(this);
        }

        #endregion

        /// <summary>
        /// Count of hyperlinks currently stored in a worksheet
        /// </summary>
        public int Count { get { return hyperlinks_xml != null ? hyperlinks_xml.SelectNodes("main:hyperlink", ns_manager).Count : 0; } }

        /// <summary>
        /// Access certain cell Hyperlink object.
        /// </summary>
        /// <param name="cell_reference"></param>
        /// <returns>Hyperlink object or null if the cell doesn't have a hyperlink</returns>
        public Hyperlink this[string cell_reference]
        {
            get
            {
                XmlNode hyp_xml = hyperlinks_xml != null ? hyperlinks_xml.SelectSingleNode("main:hyperlink[@ref = '" + cell_reference + "']", ns_manager) : null;
                return hyp_xml != null ? new Hyperlink((XmlElement)hyp_xml, parent_sheet) : null;
            }
            set
            {
                if(value != null)
                {
                    if (value.IsLocationOrAddressType)
                    {
                        AddLocationHyperlink(cell_reference, value.ReferredSheet, value.ReferredRange);
                    }
                    else
                    {
                        AddAddressHyperlink(cell_reference, value.Target);
                    }
                }
            }
        }

        internal class Enumerator : IEnumerator<Hyperlink>
        {
            public Enumerator(Hyperlinks hyps)
            {
                this.hyps = hyps;
                position = 0;
                hyps_num = hyps.Count;
            }

            #region IEnumerator<Hyperlink> Members

            public Hyperlink Current
            {
                get 
                {
                    XmlNode hyp_xml = hyps.hyperlinks_xml != null ? hyps.hyperlinks_xml.SelectSingleNode("main:hyperlink[position() = " + position + "]", hyps.ns_manager) : null;
                    return hyp_xml != null ? new Hyperlink((XmlElement)hyp_xml, hyps.parent_sheet) : null; 
                }
            }

            #endregion

            #region IDisposable Members

            public void Dispose()
            {
            }

            #endregion

            #region IEnumerator Members

            object System.Collections.IEnumerator.Current
            {
                get { return Current; }
            }

            public bool MoveNext()
            {
                if (position + 1 > hyps_num)
                {
                    return false;
                }
                ++position;
                return true;
            }

            public void Reset()
            {
                throw new NotSupportedException();
            }

            #endregion

            private Hyperlinks hyps;
            private int position;
            private int hyps_num;
        }

        /// <summary>
        /// Add a hyperlink referencing a remote target like file path or Internet Address.
        /// Note: Redefine a previously set hyperlink
        /// </summary>
        /// <param name="cell_reference">Hyperlink location (anchor)</param>
        /// <param name="target_address">Target address</param>
        public void AddAddressHyperlink(string cell_reference, string target_address)
        {
            XmlElement hyperlink_xml = parent_sheet.SheetData.OwnerDocument.CreateElement("hyperlink", ns_manager.LookupNamespace("main"));
            hyperlink_xml.SetAttribute("ref", cell_reference);
            string rId = parent_sheet.Relationships.AddTarget(target_address, OOX.Relationships.FileType.hyperlink, true);
            hyperlink_xml.SetAttribute("id", ns_manager.LookupNamespace("mainr"), rId);
            AppendHyperlinkXml(hyperlink_xml, cell_reference);
            SetValueToTheCellIfAbsent(cell_reference, target_address);
            SetHyperlinkStyle(cell_reference);
            RegisterHyperlinkItem(new Cells.CellRange(cell_reference, parent_sheet.Workbook).TopLeft);
        }

        /// <summary>
        /// Add a hyperlink referencing a local place like certain worksheet and certain cells range.
        /// Note: Redefine a previously set hyperlink
        /// </summary>
        /// <param name="cell_reference">Hyperlink location (anchor)</param>
        /// <param name="location_sheet">Referred worksheet</param>
        /// <param name="location_range_reference">Referred cells range</param>
        public void AddLocationHyperlink(string cell_reference,Worksheet location_sheet, string location_range_reference)
        {
            Cells.CellRange3D location = new Cells.CellRange3D(new Cells.CellRange(location_range_reference, parent_sheet.Workbook), location_sheet);
            XmlElement hyperlink_xml = parent_sheet.SheetData.OwnerDocument.CreateElement("hyperlink", ns_manager.LookupNamespace("main"));
            hyperlink_xml.SetAttribute("ref", cell_reference);
            hyperlink_xml.SetAttribute("location", location.ToString());
            AppendHyperlinkXml(hyperlink_xml, cell_reference);
            SetValueToTheCellIfAbsent(cell_reference, location.ToString());
            SetHyperlinkStyle(cell_reference);
            RegisterHyperlinkItem(new Cells.CellRange(cell_reference, parent_sheet.Workbook).TopLeft);
        }

        internal void RemoveHyperlink(Cells.Cell anchor)
        {
            Calculation.HyperlinkItem h_item;
            if(hyperlink_items.TryGetValue(anchor, out h_item))
            {
                parent_sheet.Workbook.ValuesUpdateManager.Unregister(parent_sheet.SubjectsStorage.GetCellItemOrCreate(anchor), h_item);
                hyperlink_items.Remove(anchor);
            }
            if(hyperlinks_xml != null)
            {
                XmlNode existent_hyperlink = hyperlinks_xml.SelectSingleNode("main:hyperlink[@ref = '" + anchor.ToString() + "']", ns_manager);
                if (existent_hyperlink != null)
                {
                    hyperlinks_xml.RemoveChild(existent_hyperlink);
                }
            }
            RemoveHyperlinkStyle(anchor.ToString());
        }

        private void AppendHyperlinkXml(XmlNode hyperlink_xml, string cell_reference)
        {
            if(hyperlinks_xml == null)
            {
                hyperlinks_xml = parent_sheet.SheetData.OwnerDocument.CreateElement("hyperlinks", ns_manager.LookupNamespace("main"));
                parent_sheet.XMLSectionsController.AppendXmlSection(hyperlinks_xml);
            }

            XmlNode existent_hyperlink = hyperlinks_xml.SelectSingleNode("main:hyperlink[@ref = '" + cell_reference + "']", ns_manager);
            if(existent_hyperlink != null)
            {
                hyperlinks_xml.RemoveChild(existent_hyperlink);
            }
            hyperlinks_xml.AppendChild(hyperlink_xml);
        }

        private void SetHyperlinkStyle(string cell_reference)
        {
            Range range = parent_sheet.Range(cell_reference);
            range.Format.Font.Underline = true;
            range.Format.Font.Color.Rgb = new Format.Colors.RGB(0, 0, 255);
        }
        private void RemoveHyperlinkStyle(string cell_reference)
        {
            Range range = parent_sheet.Range(cell_reference);
            range.Format.Font.Underline = false;
            range.Format.Font.Color.Rgb = new Format.Colors.RGB(0, 0, 0);
        }
        private void SetValueToTheCellIfAbsent(string cell_reference, string value)
        {
            Range range = parent_sheet.Range(cell_reference);
            if(range.Value.ToString().Length == 0)
            {
                range.Value = value;
            }
        }

        public XmlNode HyperlinksXml
        {
            get { return hyperlinks_xml; }
            set { hyperlinks_xml = value.Clone(); }
        }

        private void RegisterHyperlinkItem(Cells.Cell anchor)
        {
            if(!hyperlink_items.ContainsKey(anchor))
            {
                Calculation.HyperlinkItem h_item = new Calculation.HyperlinkItem(anchor, parent_sheet);
                Calculation.CellItem anchor_item = parent_sheet.SubjectsStorage.GetCellItemOrCreate(anchor);
                hyperlink_items.Add(anchor, h_item);
                parent_sheet.Workbook.ValuesUpdateManager.Register(anchor_item, h_item);
            }
        }

        internal void MoveHyperlink(Cells.Cell anchor, Cells.Cell new_anchor)
        {
            Calculation.HyperlinkItem h_item;
            if (hyperlink_items.TryGetValue(anchor, out h_item))
            {
                // Re-register
                hyperlink_items.Remove(anchor);
                parent_sheet.Workbook.ValuesUpdateManager.Unregister(parent_sheet.SubjectsStorage.GetCellItemOrCreate(anchor), h_item);

                Calculation.HyperlinkItem h_new_item = new Calculation.HyperlinkItem(new_anchor, parent_sheet);
                hyperlink_items.Add(new_anchor, h_new_item);
                parent_sheet.Workbook.ValuesUpdateManager.Register(parent_sheet.SubjectsStorage.GetCellItemOrCreate(new_anchor), h_new_item);

                // Reflect changes in XML
                if (hyperlinks_xml != null)
                {
                    XmlElement existent_hyperlink = (XmlElement)hyperlinks_xml.SelectSingleNode("main:hyperlink[@ref = '" + anchor.ToString() + "']", ns_manager);
                    if (existent_hyperlink != null)
                    {
                        existent_hyperlink.SetAttribute("ref", new_anchor.ToString());
                    }
                }
            }
        }

        private XmlNode hyperlinks_xml;

        private Worksheet parent_sheet;
        private XmlNamespaceManager ns_manager;

        private Dictionary<Cells.Cell, Calculation.HyperlinkItem> hyperlink_items;
    }
}
