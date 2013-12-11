using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal abstract class Ptg
    {
        public abstract DataObjects.IDataObject Calculate(ref LinkedList<Ptg>.Enumerator ptgs_enum, PreferredResultType result_type);
        public abstract string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum);
    }

    internal interface IHavingSubjectPtg
    {
        IObserver UnregisterDependency();
        void RegisterDependency(IObserver observer);
    }
}
