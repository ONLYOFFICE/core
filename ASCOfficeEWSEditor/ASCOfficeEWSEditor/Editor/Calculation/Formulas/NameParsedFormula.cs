using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas
{
    internal class NameParsedFormula : ParsedFormula
    {
        internal NameParsedFormula(string name, Workbook workbook)
            : base(workbook)
        {
//             this.workbook = workbook;
//             this.name = name;
//             name_observer = null;
        }

/*
        public override void UnregisterDependencies()
        {
            if (name_observer != null)
            {
                name_observer.BecomeSubject();
                name_observer = null;
            }
            base.UnregisterDependencies();
        }
*/

        internal override Worksheet ParentSheet { get { return null; } }

//         private Workbook workbook;
//         private string name;
//         private NameItem name_observer;

    }
}
