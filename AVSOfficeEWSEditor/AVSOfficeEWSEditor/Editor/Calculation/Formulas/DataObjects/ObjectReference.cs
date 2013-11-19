using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Calculation.Formulas.DataObjects
{
    internal class ObjectReference : IDataObject, IEnumerable<DataObjects.IDataObject>
    {
        public ObjectReference(Cells.CellRange value, Worksheet parent_sheet, IObserver observer)
            : this(new Cells.CellRange3D(value, parent_sheet), observer)
        {
        }

        public ObjectReference(Cells.CellRange3D value, IObserver observer)
        {
            this.observer = observer;
            ranges = new LinkedList<Cells.CellRange3D>();
            ranges.AddLast(value);
        }

        private ObjectReference(LinkedList<Cells.CellRange3D> ranges, IObserver observer)
        {
            this.observer = observer;
            this.ranges = ranges;
        }


        #region IDataObject Members

        public IDataObject PerformBinaryOperation(IDataObject element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return element.PerformBinaryOperation(this, dispatcher);
        }

        public IDataObject PerformBinaryOperation(ObjectNumber element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectBoolean element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectError element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectString element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectReference element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        public IDataObject PerformBinaryOperation(ObjectEmpty element, BinOpCalc.IFullBinaryOpDispatcher dispatcher)
        {
            return dispatcher.Calc(element, this);
        }

        #endregion

        #region ConvertIfPossible

        public IDataObject ConvertIfPossible(PreferredResultType result_type)
        {
            if (result_type != PreferredResultType.reference_type && result_type != PreferredResultType.as_is)
            {
                if (IsSingleCell)
                {
                    DataObjects.IDataObject result;
                    CellItem cell_subject;
                    Cells.Cell single_cell = ranges.First.Value.CellRange.TopLeft;
                    Worksheet parent_sheet = ranges.First.Value.WorksheetFrom;
                    if (parent_sheet.SubjectsStorage.CellSubjects.TryGetValue(single_cell, out cell_subject))
                    {
                        result = cell_subject.GetValue();
                    }
                    else
                    {
                        result = parent_sheet.CellGetValue(single_cell);
                    }
                    return result.ConvertIfPossible(result_type);
                }
                else
                {
                    return new ObjectError(ErrorValue.wrong_value_type);
                }
            }
            else
            {
                return this;
            }
        }

        #endregion

        #region IComparable<IDataObject> Members

        public int CompareTo(IDataObject other) // Shouldn't be called ever
        {
            if (other is ObjectReference)
            {
                return 0;
            }
            return -1;
        }

        #endregion

        #region IEnumerable<IDataObject> Members

        public IEnumerator<IDataObject> GetEnumerator()
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

        internal class Enumerator : IEnumerator<IDataObject>
        {
            public Enumerator(ObjectReference reference)
            {
                this.reference = reference;

                existent_cells = new List<Cells.CellRange3D>();
                foreach (Cells.CellRange3D range in reference.ranges)
                {
                    Workbook wb = range.WorksheetFrom.Workbook;
                    foreach (Worksheet ws in wb.WorksheetsN(range.WorksheetFrom.Index, range.WorksheetTo.Index))
                    {
                        Walkers.ExistentCellsCollector collector = new Walkers.ExistentCellsCollector(ws);
                        new Walkers.RangeWalker(range.CellRange, ws).CollectStatistics(collector);
                        existent_cells.AddRange(collector.GetFoundCells());
                    }
                }

                cells_enum = existent_cells.GetEnumerator();
            }

            #region IEnumerator<Cell> Members

            public IDataObject Current
            {
                get { return cells_enum.Current.WorksheetFrom.SubjectsStorage.GetCellItemOrCreate(cells_enum.Current.CellRange.TopLeft).GetValue(); }
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
                get { return cells_enum.Current.WorksheetFrom.SubjectsStorage.GetCellItemOrCreate(cells_enum.Current.CellRange.TopLeft).GetValue(); }
            }

            public bool MoveNext()
            {
                return cells_enum.MoveNext();
            }

            public void Reset()
            {
                throw new NotSupportedException();
            }

            #endregion

            private ObjectReference reference;
            private List<Cells.CellRange3D>.Enumerator cells_enum;
            private List<Cells.CellRange3D> existent_cells;
        }

        public IDataObject Union(ObjectReference other_reference)
        {
            CheckPlanarAndCollinear(this, other_reference);
            LinkedList<Cells.CellRange3D> ranges_result = ranges;
            foreach (Cells.CellRange3D range in other_reference.ranges)
            {
                ranges_result.AddLast(range);
            }
            return new ObjectReference(ranges_result, observer);
        }

        public IDataObject Intersect(ObjectReference other_reference)
        {
            CheckPlanarAndCollinear(this, other_reference);
            LinkedList<Cells.CellRange3D> ranges_result = new LinkedList<Cells.CellRange3D>();
            foreach (Cells.CellRange3D range in ranges)
            {
                foreach (Cells.CellRange3D other_range in other_reference.ranges)
                {
                    Cells.CellRange3D intersected_range = range.Intersect(other_range);
                    if (intersected_range != Cells.CellRange3D.Empty)
                    {
                        ranges_result.AddLast(intersected_range);
                    }
                }
            }
            ObjectReference intersected_object = new ObjectReference(ranges_result, observer);
            //Unregister();
            //other_reference.Unregister();
            //intersected_object.Register();
            return intersected_object;
        }

        public IDataObject Range(ObjectReference other_reference)
        {
            CheckPlanarAndCollinear(this, other_reference);
            Cells.CellRange3D bounding_box = Cells.CellRange3D.Empty;
            foreach (Cells.CellRange3D range in ranges)
            {
                if (bounding_box == Cells.CellRange3D.Empty)
                {
                    bounding_box = range;
                }
                else
                {
                    bounding_box = bounding_box.BoundingBox(range);
                }
            }
            foreach (Cells.CellRange3D range in other_reference.ranges)
            {
                if (bounding_box == Cells.CellRange3D.Empty)
                {
                    bounding_box = range;
                }
                else
                {
                    bounding_box = bounding_box.BoundingBox(range);
                }
            }
            
            ObjectReference bounded_object = new ObjectReference(bounding_box, observer);
            //Unregister();
            //other_reference.Unregister();
//             bounded_object.Register();
            return bounded_object;
        }


        private static void CheckPlanarAndCollinear(ObjectReference ref1, ObjectReference ref2)
        {
            if (ref1.ranges.Count == 0 || ref2.ranges.Count == 0)
            {
                throw new Events.ReportableException(new Events.FormulaFormatError("Wrong argument types of a reference operator"));
            }
            Worksheet the_same_ws = ref1.ranges.First.Value.WorksheetFrom;
            foreach (Cells.CellRange3D range in ref1.ranges)
            {
                if (range.WorksheetFrom != the_same_ws || range.WorksheetTo != the_same_ws)
                {
                    throw new Events.ReportableException(new Events.FormulaFormatError("Wrong argument types of a reference operator"));
                }
            }

            foreach (Cells.CellRange3D range in ref2.ranges)
            {
                if (range.WorksheetFrom != the_same_ws || range.WorksheetTo != the_same_ws)
                {
                    throw new Events.ReportableException(new Events.FormulaFormatError("Wrong argument types of a reference operator"));
                }
            }
        }

        internal IObserver Unregister()
        {
            if(observer != null)
            {
                foreach (Cells.CellRange3D range in ranges)
                {
                    Cells.CellRange cell_range = range.CellRange;
                    if (cell_range.IsSingleCell)
                    {
                        Calculation.CellItem cell_item = range.WorksheetFrom.SubjectsStorage.GetCellItem(cell_range.TopLeft);
                        if (cell_item != null)
                        {
                            range.WorksheetFrom.Workbook.ValuesUpdateManager.Unregister(cell_item, observer);
                        }
                    }
                    else
                    {
                        Calculation.RangeSubject range_item = range.WorksheetFrom.SubjectsStorage.GetRangeSubject(cell_range);
                        if (range_item != null)
                        {
                            range.WorksheetFrom.Workbook.ValuesUpdateManager.Unregister(range_item, observer);
                        }
                    }
                }
                return observer;
            }
            return null;
        }

        internal void Register()
        {
            if (observer != null)
            {
                foreach (Cells.CellRange3D range in ranges)
                {
                    Cells.CellRange cell_range = range.CellRange;
                    if (cell_range.IsSingleCell)
                    {
                        Calculation.CellItem cell_item = range.WorksheetFrom.SubjectsStorage.GetCellItemOrCreate(cell_range.TopLeft);
                        if (cell_item != null)
                        {
                            range.WorksheetFrom.Workbook.ValuesUpdateManager.Register(cell_item, observer);
                        }
                    }
                    else
                    {
                        Calculation.RangeSubject range_item = range.WorksheetFrom.SubjectsStorage.GetRangeSubjectOrCreate(cell_range);
                        if (range_item != null)
                        {
                            range.WorksheetFrom.Workbook.ValuesUpdateManager.Register(range_item, observer);
                        }
                    }
                }
            }
        }


        public override string ToString()
        {
            string result = "";
            foreach (Cells.CellRange3D range in ranges)
            {
                if (result.Length == 0)
                {
                    result = range.ToString();
                }
                else
                {
                    result += "," + range.ToString();
                }
            }
            return result;
        }

        private LinkedList<Cells.CellRange3D> ranges;
        private IObserver observer; // Used in reference operators to register/unregister new/former subject
        public bool IsSingleCell { get { return ranges.Count == 1 && ranges.First.Value.IsSingleCell; } }

        public object UserValue { get { return null; } }
        public string Type { get { return null; } }
    }
}
