using System;
using System.Collections.Generic;
using System.Text;

namespace ASCOfficeEWSEditor.Editor.Calculation.Formulas.Ptgs
{
    internal abstract class OperandPtg : Ptg
    {
        public override string Assemble(ref LinkedList<Ptg>.Enumerator ptgs_enum)
        {
            return ToString();
        }
    }

    internal interface IRefOperandPtg
    {
        OperandPtg Shift(Cells.Offset offset, Worksheet affected_sheet);
        Cells.CellRange GetReferredRange();
        bool ReferencesCellOrRange(ISubject subject);
        OperandPtg RemoveRange(Cells.CellRange removed_range, Worksheet removed_range_sheet);
        OperandPtg ResizeInsertionAffected(Cells.CellRange moved_rows_range, Cells.Offset offset, Worksheet affected_sheet);
        bool IsRelative { get; }
    }
}
