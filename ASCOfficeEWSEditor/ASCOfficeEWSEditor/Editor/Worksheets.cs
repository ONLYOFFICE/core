using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor
{
    /// <summary>
    /// A collection of all the Worksheet objects in the specified or active workbook
    /// </summary>
    public class Worksheets : IEnumerable<Worksheet>
    {
        internal Worksheets(Sheets sheets)
        {
            this.sheets = sheets;
        }

        /// <summary>
        /// Obtains a worksheet reference by its name
        /// </summary>
        /// <param name="name">Name of the required sheet.</param>
        public Worksheet this[string name]
        {
            get
            {
                return sheets[name] as Worksheet;
            }
        }

        /// <summary>
        /// Obtains a worksheet reference by its index among worksheets only
        /// </summary>
        /// <param name="index">Index of the required sheet. Zero-based.</param>
        public Worksheet this[int index]
        {
            get
            {
                return sheets[index] as Worksheet;
            }
        }

        /// <summary>
        /// Gets the number of worksheets contained in the workbook
        /// </summary>
        public int Count
        {
            get
            {
                int count = 0;
                foreach (Sheet sheet in sheets)
                {
                    if (sheet is Worksheet) count++;
                }
                return count;
            }
        }


        #region IEnumerable<Worksheet> Members

        IEnumerator<Worksheet> IEnumerable<Worksheet>.GetEnumerator()
        {
            return new Enumerator(sheets.GetEnumerator());
        }

        #endregion

        #region IEnumerable Members

        IEnumerator IEnumerable.GetEnumerator()
        {
            return new Enumerator(sheets.GetEnumerator());
        }

        #endregion

        /// <summary>
        /// Enumerator
        /// </summary>
        public class Enumerator : IEnumerator<Worksheet>
        {
            internal Enumerator(IEnumerator<Sheet> parent_enumerator)
            {
                this.parent_enumerator = parent_enumerator;
                current = null;
            }

            /// <summary>
            /// Current worksheet
            /// </summary>
            public Worksheet Current
            {
                get 
                {
                    if (current == null)
                    {
                        parent_enumerator.Reset();
                        return parent_enumerator.Current as Worksheet; // Generates System.InvalidOperationException
                    }
                    return current; 
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
                if (parent_enumerator.MoveNext())
                {
                    while (!(parent_enumerator.Current is Worksheet))
                    {
                        if (!parent_enumerator.MoveNext())
                        {
                            return false;
                        }
                    }
                    current = parent_enumerator.Current as Worksheet;
                    return true;
                }
                return false;
            }

            /// <summary>
            /// Reset
            /// </summary>
            public void Reset()
            {
                parent_enumerator.Reset();
                current = null;
            }

            object System.Collections.IEnumerator.Current
            {
                get { return Current; }
            }


            private IEnumerator<Sheet> parent_enumerator;
            private Worksheet current;


        }

       
        private Sheets sheets;


    }


    	


}
