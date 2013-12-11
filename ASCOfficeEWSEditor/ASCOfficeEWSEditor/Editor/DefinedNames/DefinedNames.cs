using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.DefinedNames
{
    /// <summary>
    /// Collection of defined names contained in a workbook
    /// </summary>
    public class DefinedNames : IEnumerable<DefinedName>
    {
        internal DefinedNames(Workbook workbook)
        {
            this.workbook = workbook;
            ns_manager = workbook.NamespaceManager;

            NameItems = new Dictionary<string, Calculation.NameItem>();

            defnames_xml = workbook.GetRootTag().SelectSingleNode("main:definedNames", ns_manager);

            if (defnames_xml != null)
            {
                foreach (XmlElement defname_xml in defnames_xml.SelectNodes("main:definedName", ns_manager))
                {
                    string name = defname_xml.GetAttribute("name");
                    string formula = defname_xml.InnerText;
                    CreateAndAppendNameItem(defname_xml, name, formula);
                }
            }
        }

        private Calculation.NameItem CreateAndAppendNameItem(XmlElement defname_xml, string name, string formula)
        {
            Calculation.Formulas.NameParsedFormula name_parsed_formula = new Calculation.Formulas.NameParsedFormula(name, workbook);
            if (name_parsed_formula.Parse(formula, Cells.Offset.Null))
            {
                if(name_parsed_formula.HasRelativeParts())
                {
                    workbook.ReportError(new Events.FormulaFormatError("Using of relative references is prohibited in defined names. Wrong formula: \"" + formula + "\""));
                }
                Calculation.NameItem name_item;
                if(NameItems.TryGetValue(name, out name_item))
                {
                    name_item.Formula.UnregisterDependencies();
                    NameItems.Remove(name);
                }
                name_item = new Calculation.NameItem(name, workbook, workbook.ValuesUpdateManager, name_parsed_formula);
                NameItems.Add(name, name_item);
                defname_xml.InnerText = name_parsed_formula.Assemble();
                return name_item;
            }
            // Else just continue with the left correct formulas
            return null;
        }

        internal bool IsNameExists(string name)
        {
            return NameItems.ContainsKey(name);
        }

        internal void RegisterFormulas()
        {
            foreach (Calculation.NameItem name_item in NameItems.Values)
            {
                name_item.Formula.RegisterDependencies(name_item);
            }
        }


        #region IEnumerable<DefinedName> Members

        /// <summary>
        /// Enumerator
        /// </summary>
        /// <returns>Enumerator</returns>
        public IEnumerator<DefinedName> GetEnumerator()
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
        /// Count of defined names currently stored in a workbook
        /// </summary>
        public int Count { get { return NameItems.Count; } }

        /// <summary>
        /// Enumerator through all stored defined names
        /// </summary>
        public class Enumerator : IEnumerator<DefinedName>
        {
            internal Enumerator(DefinedNames defnames)
            {
                name_items_enum = defnames.NameItems.GetEnumerator();
            }

            #region IEnumerator<DefinedName> Members

            /// <summary>
            /// Current defined name
            /// </summary>
            public DefinedName Current
            {
                get
                {
                    KeyValuePair<string, Calculation.NameItem> name_items_current = name_items_enum.Current;
                    return new DefinedName(name_items_current.Key, name_items_current.Value.Formula.ToString());
                }
            }

            #endregion

            #region IDisposable Members
            /// <summary>
            /// Dispose :)
            /// </summary>
            public void Dispose()
            {
            }

            #endregion

            #region IEnumerator Members

            object System.Collections.IEnumerator.Current
            {
                get { return Current; }
            }

            /// <summary>
            /// Switches to the next Current
            /// </summary>
            /// <returns></returns>
            public bool MoveNext()
            {
                return name_items_enum.MoveNext();
            }

            /// <summary>
            /// Reset
            /// </summary>
            public void Reset()
            {
                throw new NotSupportedException();
            }

            #endregion

            private Dictionary<string, Calculation.NameItem>.Enumerator name_items_enum;
        }

        /// <summary>
        /// Add a defined name. Rewrites the existing one.
        /// </summary>
        /// <param name="name">Formula of the defined name</param>
        /// <param name="formula">Name of the defined name</param>
        /// <returns>Whether the passed formula was parsed correctly</returns>
        public bool AddName(string name, string formula)
        {
            XmlElement defname_xml = workbook.GetXmlDocument().CreateElement("definedName", ns_manager.LookupNamespace("main"));
            defname_xml.SetAttribute("name", name);
            defname_xml.InnerText = formula;

            AppendDefinedNamesXml(defname_xml, name);

            Calculation.NameItem name_item = CreateAndAppendNameItem(defname_xml, name, formula);
            if (name_item != null)
            {
                name_item.Formula.RegisterDependencies(name_item);
                return true;
            }
            return false;
        }

        private void AppendDefinedNamesXml(XmlNode defname_xml, string name)
        {
            if (defnames_xml == null)
            {
                defnames_xml = workbook.GetXmlDocument().CreateElement("definedNames", ns_manager.LookupNamespace("main"));
                workbook.XMLSectionsController.AppendXmlSection(defnames_xml);
            }

            XmlNode existent_defname = defnames_xml.SelectSingleNode("main:definedName[@name = '" + name + "']", ns_manager);
            if (existent_defname != null)
            {
                defnames_xml.RemoveChild(existent_defname);
            }

            defnames_xml.AppendChild(defname_xml);
        }

        private XmlNode defnames_xml;

        private Workbook workbook;
        private XmlNamespaceManager ns_manager;
        internal Dictionary<string, Calculation.NameItem> NameItems { get; private set; }
    }
}
