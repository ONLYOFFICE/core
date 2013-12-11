using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace ASCOfficeEWSEditor.Editor.Cells
{
    internal class CellRange3D
    {

        public CellRange3D(CellRange cell_range, Worksheet worksheet_from, Worksheet worksheet_to) 
        {
            CellRange = cell_range;
            if(worksheet_from == null || worksheet_to == null)
            {
                WorksheetFrom = WorksheetTo = null;
                return;
            }
            if (worksheet_from.Index <= worksheet_to.Index)
            {
                WorksheetFrom = worksheet_from;
                WorksheetTo = worksheet_to;
            }
            else
            {
                WorksheetFrom = worksheet_to;
                WorksheetTo = worksheet_from;
            }
        }

        public CellRange3D(CellRange cell_range, Worksheet worksheet)
            : this(cell_range, worksheet, worksheet)
        {
        }

        public CellRange3D(Cell cell, Worksheet worksheet)
            : this(new CellRange(cell), worksheet, worksheet)
        {
        }

        public static CellRange3D Empty = new CellRange3D(CellRange.Empty, null, null);

        public bool IsSingleCell
        {
            get { return CellRange.IsSingleCell && WorksheetFrom == WorksheetTo; }
        }

        public bool IsIntersectedWith(CellRange3D another_range)
        {
            return CellRange.IsIntersectedWith(another_range.CellRange) &&
                    !(WorksheetFrom.Index > another_range.WorksheetTo.Index || WorksheetTo.Index < another_range.WorksheetFrom.Index);
        }

        public CellRange3D BoundingBox(CellRange3D another_range)
        {
            Worksheet ws_min = WorksheetFrom.Index < another_range.WorksheetFrom.Index ? WorksheetFrom : another_range.WorksheetFrom;
            Worksheet ws_max = WorksheetTo.Index > another_range.WorksheetTo.Index ? WorksheetTo : another_range.WorksheetTo;
            return new CellRange3D(CellRange.BoundingBox(another_range.CellRange), ws_min, ws_max);
        }

        public CellRange3D Intersect(CellRange3D another_range)
        {
            if (!IsIntersectedWith(another_range))
            {
                return Empty;
            }

            Worksheet ws_min = WorksheetFrom.Index > another_range.WorksheetFrom.Index ? WorksheetFrom : another_range.WorksheetFrom;
            Worksheet ws_max = WorksheetTo.Index < another_range.WorksheetTo.Index ? WorksheetTo : another_range.WorksheetTo;
            return new CellRange3D(CellRange.Intersect(another_range.CellRange), ws_min, ws_max);
        }

        public override string ToString()
        {
            return ToString3DPart(WorksheetFrom, WorksheetTo) + CellRange.ToString();
        }
        private static Regex rx_AllowedName = new Regex("^\\w[\\w\\d.]*$");
        internal static string ToString3DPart(Worksheet ws_from, Worksheet ws_to)
        {
            if (ws_from == ws_to)
            {
                string name = ws_from.Name.Replace("'", "''");
                if (!rx_AllowedName.Match(name).Success)
                {
                    return "\'" + name + "\'!";
                }
                return name + "!";

            }
            string name_from = ws_from.Name.Replace("'", "''");
            string name_to = ws_to.Name.Replace("'", "''");

            if (!rx_AllowedName.Match(name_from).Success || !rx_AllowedName.Match(name_to).Success)
            {
                return "\'" + name_from + ":" + name_to + "\'!";
            }
            return name_from + ":" + name_to + "!";
        }

        public CellRange CellRange { get; private set; }
        public Worksheet WorksheetFrom { get; private set; }
        public Worksheet WorksheetTo { get; private set; }
    }
}
