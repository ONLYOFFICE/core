using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;
using System.IO;

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// Represents a sheet (worksheet, chartsheet, macrosheet, etc).
    /// </summary>
    public abstract class Sheet
    {
        private Sheet(Sheets container, Workbook workbook)
        {
            ns_manager = workbook.NamespaceManager;
            this.container = container;
            Workbook = workbook;
        }

        protected Sheet(Sheets container, Workbook workbook, XmlNode xml_workbook_sheet)
            : this(container, workbook)
        {
            this.xml_workbook_sheet = (XmlElement)xml_workbook_sheet;
        }


        /// <summary>
        /// Create a new sheet
        /// </summary>
        protected Sheet(string name, string rId, Sheet before, XmlNode xml_workbook_sheets, Sheets container, Workbook workbook)
            : this(container, workbook)
        {
            xml_workbook_sheet = CreateWorkbookSheet(name, rId, before, xml_workbook_sheets);
        }


        /// <summary>
        /// Create from existent worksheet
        /// </summary>
        internal Sheet(string sheet_path, XmlNode xml_workbook_sheet, Sheets container, Workbook workbook)
            : this(container, workbook)
        {
            this.xml_workbook_sheet = (XmlElement)xml_workbook_sheet;
        }

        private XmlElement CreateWorkbookSheet(string name, string rId, Sheet before, XmlNode xml_workbook_sheets)
        {
            XmlElement sheet = xml_workbook_sheets.OwnerDocument.CreateElement("sheet", ns_manager.LookupNamespace("main"));
            sheet.SetAttribute("name", name);
            int max_id = 0;
            foreach (XmlNode sheetId_node in xml_workbook_sheets.SelectNodes("main:sheet/@sheetId", ns_manager))
            {
                int sheetId = XMLInt.ParseOrDefault(sheetId_node.InnerText, 0);
                if (sheetId > max_id)
                {
                    max_id = sheetId;
                }
            }
            sheet.SetAttribute("sheetId", (max_id + 1).ToString());
            sheet.SetAttribute("id", ns_manager.LookupNamespace("mainr"), rId);

            XmlNode sheet_before = before == null ? null : xml_workbook_sheets.SelectSingleNode("main:sheet[@name = '" + before.Name + "']", ns_manager);
            xml_workbook_sheets.InsertBefore(sheet, sheet_before);

            return sheet;
        }

        /// <summary>
        /// Sets the sheet active.
        /// </summary>
        public virtual void Activate()
        {
            Workbook.ActivateSheet(Index);
        }

        /// <summary>
        /// Remove sheet active
        /// </summary>
        public virtual void RemoveActive()
        {
        }

        /// <summary>
        /// Name of the sheet. Read-only. (use SetName to change the name of the sheet)
        /// </summary>
        public string Name
        {
            get
            {
                return xml_workbook_sheet.GetAttribute("name");
            }
            internal set
            {
                xml_workbook_sheet.SetAttribute("name", value);
            }
        }

        /// <summary>
        /// Visible of the sheet.
        /// </summary>
        public bool Hidden
        {
            get
            {
                return ("hidden" == xml_workbook_sheet.GetAttribute("state"));
            }
            set
            {
                if (value)
                    xml_workbook_sheet.SetAttribute("state", "hidden");
                else
                    xml_workbook_sheet.RemoveAttribute("state");
            }
        }

        /// <summary>
        /// Has sheet data. Read-only.
        /// </summary>
        public abstract bool HasSheetData { get; }

        /// <summary>
        /// Returns the name of the sheet
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return Name;
        }

        /// <summary>
        /// Sets the new name for the sheet
        /// </summary>
        /// <param name="name">new name</param>
        /// <returns>Whether the assignment was successful</returns>
        public bool SetName(string name)
        {
            if (name.Length == 0)
            {
                return false;
            }
            if(container.ChangeSheetName(Name, name))
            {
                Workbook.ValuesUpdateManager.NotifyRenamedSheet(this);
                return true;
            }
            return false;
        }

        /// <summary>
        /// Index of the sheet. Zero-based. Read-only.
        /// </summary>
        public int Index
        {
            get
            {
                return xml_workbook_sheet.SelectNodes("preceding-sibling::main:sheet", ns_manager).Count;
            }
        }

        /// <summary>
        /// Permanently removes the sheet from the workbook (
        /// </summary>
        /// <returns>Whether the operation successful. Return false if the removed sheet is the only sheet in the workbook</returns>
        protected bool RemoveSheet()
        {
            if(container.Count == 1)
            {
                return false;
            }

            Workbook.ValuesUpdateManager.NotifyRemovedSheet(this);
            
            container.DetachSheet(Name);

            xml_workbook_sheet.ParentNode.RemoveChild(xml_workbook_sheet); // Remove workbook/sheet

            return true;
        }

        /// <summary>
        /// Permanently removes the sheet from the workbook
        /// </summary>
        /// <returns>Whether the operation successful. Return false if the removed sheet is the only sheet in the workbook</returns>
        public abstract bool Remove();

        /// <summary>
        /// Workbook that contains this sheet
        /// </summary>
        public Workbook Workbook { get; private set; }

        #region XML references
        private XmlElement xml_workbook_sheet;
        #endregion

        #region tree references
        protected Sheets container;
        #endregion

        protected XmlNamespaceManager ns_manager;
    }
}
