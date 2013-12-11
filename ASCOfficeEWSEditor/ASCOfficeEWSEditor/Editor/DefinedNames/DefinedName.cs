using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.DefinedNames
{
    /// <summary>
    /// DefinedName object
    /// </summary>
    public class DefinedName
    {
        internal DefinedName(string name, string formula)
        {
            Name = name;
            Formula = formula;
        }

        /// <summary>
        /// Formula of the defined name
        /// </summary>
        public string Formula {get; private set;}
        /// <summary>
        /// Name of the defined name
        /// </summary>
        public string Name { get; private set; }
    }
}
