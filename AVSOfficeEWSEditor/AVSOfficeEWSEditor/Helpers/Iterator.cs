using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Helpers
{
    internal class Iterator<T> 
    {
        public Iterator(IEnumerator<T> enumerator)
        {
            this.enumerator = enumerator;
            MoveNext();
        }

        public void MoveNext ()
        {
            End = !enumerator.MoveNext();
        }

        public T Current { get { return enumerator.Current; } }
        public bool End { get; private set; }

        public override string ToString()
        {
            return enumerator.Current.ToString();
        }

        private IEnumerator<T> enumerator;
    }
}
