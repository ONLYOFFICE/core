using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// A collection of all the sheet objects in the specified or active workbook
    /// </summary>
    public class Sheets : IEnumerable<Sheet>
    {
        internal Sheets(Workbook workbook)
        {
            ns_manager = workbook.NamespaceManager;
            this.workbook = workbook;
            sheets = new Dictionary<string, Sheet>();
            sheets_tag = workbook.GetRootTag().SelectSingleNode("main:sheets", ns_manager);
            FindSheets(workbook.GetRootTag());
        }

        /// <summary>
        /// Obtains a sheet reference by its name
        /// </summary>
        /// <param name="name">Name of the required sheet.</param>
        public Sheet this[string name]
        {
            get
            {
                Sheet result;
                if (sheets.TryGetValue(name, out result))
                {
                    return result;
                }
                return null;
            }
            private set{}
        }

        /// <summary>
        /// Obtains a sheet reference by its index
        /// </summary>
        /// <param name="index">Index of the required sheet. Zero-based.</param>
        public Sheet this[int index]
        {
            get
            {
                foreach (Sheet sheet in sheets.Values)
                {
                    if (sheet.Index == index)
                    {
                        return sheet;
                    }
                }
                return null;
            }
            private set { }
        }

        /// <summary>
        /// Gets the number of sheets contained in the workbook
        /// </summary>
        public int Count { get{ return sheets.Count;}}

        /// <summary>
        /// Creates a new worksheet and makes it active.
        /// </summary>
        /// <param name="name">Name of the new sheet</param>
        /// <param name="before">Existent sheet that the new sheet shall be inserted before. If 'null' the new sheet is appended at the end</param>
        /// <returns>Just created worksheet</returns>
        public Worksheet AddEmptySheet(string name, Sheet before)
        {
            Worksheet ws = Worksheet.CreateNew(name, before, sheets_tag, this, workbook);

            sheets.Add(name, ws);
            if (null != ws.Workbook.ActiveSheet)
                ws.Workbook.ActiveSheet.RemoveActive();
            ws.Activate();
            return ws;
        }


        public bool ReplaceSheets(string name, int index)
        {
            if (index < 0 || index >= sheets_tag.ChildNodes.Count)
                return false;

            XmlNode oNode = null;
            foreach (XmlNode sheetId_node in sheets_tag.SelectNodes("main:sheet", ns_manager))
            {
                if (name == sheetId_node.Attributes["name"].InnerText)
                    oNode = sheetId_node;
            }

            sheets_tag.RemoveChild(oNode);

            if (0 == index)
                sheets_tag.InsertBefore(oNode, sheets_tag.ChildNodes[index]);
            else
                sheets_tag.InsertAfter(oNode, sheets_tag.ChildNodes[index-1]);
            sheets.Clear();
            FindSheets(workbook.GetRootTag());
            Sheet oCurWoorksheet;
            if (true == sheets.TryGetValue(name, out oCurWoorksheet))
            {
                workbook.ActiveSheet.RemoveActive();
                oCurWoorksheet.Activate();
            }
            return true;
        }


        public bool AddCopy(string name, Worksheet before)
        {
            Worksheet sheet = AddEmptySheet(name, null);
            sheet.CopyData(before);

            //sheets.Add(name, sheet);
            if (null != sheet.Workbook.ActiveSheet)
                sheet.Workbook.ActiveSheet.RemoveActive();
            sheet.Activate();
            return true;
        }
        /// <summary>
        /// Creates a new worksheet, append it to the end and makes it active.
        /// </summary>
        /// <param name="name">Name of the new sheet</param>
        /// <returns>Just created worksheet</returns>
        public Worksheet AddEmptySheet(string name)
        {
            return AddEmptySheet(name, null);
        }

        internal void DetachSheet(string name)
        {
            sheets.Remove(name);
        }

        private void FindSheets(XmlNode workbook_tag)
        {
            XmlNode containing_dir = (workbook_tag.ParentNode ?? workbook_tag).ParentNode;
            XmlNodeList sheets_xml = sheets_tag.SelectNodes("main:sheet", ns_manager);
            foreach (XmlElement sheet_xml in sheets_xml)
            {
                string rId = sheet_xml.GetAttribute("id", ns_manager.LookupNamespace("mainr"));
                string sheet_path = workbook.Relationships.GetTarget(rId);
                XmlNode file_tag = Helpers.OOXNavigate.getReferenceByPath(sheet_path, workbook.GetRootTag().ParentNode.ParentNode);
                if (file_tag != null)
                {
                    switch (file_tag.FirstChild.Name)
                    {
                        case "worksheet":
                            sheets.Add(sheet_xml.GetAttribute("name"), new Worksheet(sheet_path, sheet_xml, this, workbook));
                            break;

                        case "chartsheet":
                            sheets.Add(sheet_xml.GetAttribute("name"), new Chartsheet(sheet_path, sheet_xml, this, workbook));
                            break;

                        case "dialogsheet":
//                             sheets.Add(sheet_xml.GetAttribute("name"), new Dialogsheet(sheet_path, sheet_xml, this, workbook));
                            break;

                    }
                }
            }
        }

        internal bool ChangeSheetName(string old_name, string new_name)
        {
            if (old_name == new_name)
            {
                return true;
            }
            if(sheets.ContainsKey(new_name))
            {
                return false;
            }

            Sheet sheet = sheets[old_name];
            sheets.Remove(old_name);
            sheets.Add(new_name, sheet);
            sheet.Name = new_name;
            return true;
        }

        #region IEnumerable<Sheet> Members

        /// <summary>
        /// Enumerator
        /// </summary>
        /// <returns>Enumerator</returns>
        public IEnumerator<Sheet> GetEnumerator()
        {
            return new Enumerator(sheets.Values.GetEnumerator());
        }

        #endregion

        #region IEnumerable Members

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(sheets.Values.GetEnumerator());
        }

        #endregion

        /// <summary>
        /// Enumerator
        /// </summary>
        public class Enumerator : IEnumerator<Sheet>
        {
            internal Enumerator(IEnumerator<Sheet> orig_enumerator)
            {
                this.orig_enumerator = orig_enumerator;
            }

            /// <summary>
            /// Current sheet
            /// </summary>
            public Sheet Current
            {
                get
                {
                    return orig_enumerator.Current;
                }
            }

            #region IDisposable Members
            /// <summary>
            /// Dispose :)
            /// </summary>
            public void Dispose()
            {
            }

            #endregion

            /// <summary>
            /// Switches to the next Current
            /// </summary>
            /// <returns></returns>
            public bool MoveNext()
            {
                return orig_enumerator.MoveNext();
            }

            /// <summary>
            /// Reset
            /// </summary>
            public void Reset()
            {
                orig_enumerator.Reset();
            }

            object System.Collections.IEnumerator.Current
            {
                get { return Current; }
            }


            IEnumerator<Sheet> orig_enumerator;
        }


        private Dictionary<string, Sheet> sheets;
        private XmlNode sheets_tag;
        private XmlNamespaceManager ns_manager;
        private Workbook workbook;

    }
}
