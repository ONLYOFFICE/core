using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Sorting;
using csUnit;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Relative")]
    public class RelativeReferences
    {
        public RelativeReferences()
        {
        }

        [SetUp]
        public void SetUp()
        {
            editor = new EWSEditor();
            editor.SetupLog("unittests.log");
        }

        [TearDown]
        public void TearDown()
        {
            editor.StopLogging();
            editor = null;
        }

        [Test]
        public void TestParsingAtLoad()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Shared_formulas.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1").Value = 2;

            Assert.Equals(7, ws.Range("A6").Value);
            Assert.Equals("A5+1", ws.Range("A6").Formula);
            Assert.Equals(10, ws.Range("A9").Value);
            Assert.Equals("A8+1", ws.Range("A9").Formula);
            Assert.Equals(11, ws.Range("B4").Value);
            Assert.Equals("A4+A5", ws.Range("B4").Formula);
            Assert.Equals(21, ws.Range("B9").Value);
            Assert.Equals("A9+A10", ws.Range("B9").Formula);
            Assert.Equals(21, ws.Range("A11").Value);
            Assert.Equals("A10+A9", ws.Range("A11").Formula);
            Assert.Equals(53, ws.Range("B11").Value);
            Assert.Equals("B10+B9", ws.Range("B11").Formula);
            Assert.Equals(427, ws.Range("M11").Value);
            Assert.Equals("M10+M9", ws.Range("M11").Formula);
            Assert.Equals(23, ws.Range("C9").Value);
            Assert.Equals("B9+2", ws.Range("C9").Formula);
            Assert.Equals(25, ws.Range("D9").Value);
            Assert.Equals("C9+2", ws.Range("D9").Formula);
            Assert.Equals(43, ws.Range("M9").Value);
            Assert.Equals("L9+2", ws.Range("M9").Formula);
            Assert.Equals(54, ws.Range("C10").Value);
            Assert.Equals("B11+1", ws.Range("C10").Formula);
            Assert.Equals(78, ws.Range("D10").Value);
            Assert.Equals("C11+1", ws.Range("D10").Formula);
            Assert.Equals(384, ws.Range("M10").Value);
            Assert.Equals("L11+1", ws.Range("M10").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        [Test]
        public void TestSortRelativeFormulas()
        {
            Workbook wb = editor.CreateWorkbook("RelativeFormulas");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1:C1").Value = 1;
            ws.Range("A2:C2").Value = 2;
            ws.Range("A3:C3").Value = 3;
            ws.Range("A4:C4").Value = 4;
            ws.Range("A5:C5").Value = 5;
            ws.Range("A6:C6").Value = 6;
            ws.Range("A7:C7").Value = 7;
            ws.Range("A8:C8").Value = 8;
            ws.Range("A9:C9").Value = 9;

            ws.Range("B1:B9").Formula = "A1 + A2";

            Assert.Equals(3, ws.Range("B1").Value);
            Assert.Equals(5, ws.Range("B2").Value);
            Assert.Equals(15, ws.Range("B7").Value);
            Assert.Equals(9, ws.Range("B9").Value);

            Assert.Equals("A1+A2", ws.Range("B1").Formula);
            Assert.Equals("A2+A3", ws.Range("B2").Formula);
            Assert.Equals("A7+A8", ws.Range("B7").Formula);
            Assert.Equals("A9+A10", ws.Range("B9").Formula);

            ws.Range("B1:C9").Sort(SortOrientation.by_rows, ws.Range("B1:B9"), SortOrder.descending);

            Assert.Equals(3, ws.Range("B1").Value);
            Assert.Equals(5, ws.Range("B2").Value);
            Assert.Equals(15, ws.Range("B7").Value);
            Assert.Equals(9, ws.Range("B9").Value);

            Assert.Equals(8, ws.Range("C1").Value);
            Assert.Equals(7, ws.Range("C2").Value);
            Assert.Equals(3, ws.Range("C7").Value);
            Assert.Equals(1, ws.Range("C9").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestSortRelativeFormulas_columns()
        {
            Workbook wb = editor.CreateWorkbook("RelativeFormulas");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1:A3").Value = 1;
            ws.Range("B1:B3").Value = 2;
            ws.Range("C1:C3").Value = 3;
            ws.Range("D1:D3").Value = 4;
            ws.Range("E1:E3").Value = 5;
            ws.Range("F1:F3").Value = 6;
            ws.Range("G1:G3").Value = 7;
            ws.Range("H1:H3").Value = 8;
            ws.Range("I1:I3").Value = 9;

            ws.Range("A2:I2").Formula = "$A1+B1+B2";

            Assert.Equals(53, ws.Range("A2").Value);
            Assert.Equals(50, ws.Range("B2").Value);
            Assert.Equals(35, ws.Range("E2").Value);
            Assert.Equals(11, ws.Range("H2").Value);
            Assert.Equals(1, ws.Range("I2").Value);

            Assert.Equals("$A1+B1+B2", ws.Range("A2").Formula);
            Assert.Equals("$A1+C1+C2", ws.Range("B2").Formula);
            Assert.Equals("$A1+F1+F2", ws.Range("E2").Formula);
            Assert.Equals("$A1+I1+I2", ws.Range("H2").Formula);
            Assert.Equals("$A1+J1+J2", ws.Range("I2").Formula);

            ws.Range("A2:I3").Sort(SortOrientation.by_columns, ws.Range("A2"), SortOrder.ascending);

            Assert.Equals(53, ws.Range("A2").Value);
            Assert.Equals(50, ws.Range("B2").Value);
            Assert.Equals(35, ws.Range("E2").Value);
            Assert.Equals(11, ws.Range("H2").Value);
            Assert.Equals(1, ws.Range("I2").Value);

            Assert.Equals(9, ws.Range("A3").Value);
            Assert.Equals(8, ws.Range("B3").Value);
            Assert.Equals(5, ws.Range("E3").Value);
            Assert.Equals(2, ws.Range("H3").Value);
            Assert.Equals(1, ws.Range("I3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestMergeRelativeFormulas()
        {
            Workbook wb = editor.CreateWorkbook("MergeRelativeFormulas");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("C2:C4").Merge();
            ws.Range("C2").Formula = "B3+$D$3";
            ws.Range("B3").Value = 1;
            ws.Range("D3").Value = 2;
            Assert.Equals(3, ws.Range("C2").Value);
            Assert.Equals("B3+$D$3", ws.Range("C2").Formula);

            ws.Range("B3:D3").Merge();

            Assert.Equals("", ws.Range("C2").Value);
            Assert.Equals("", ws.Range("C2").Formula);

            Assert.Equals(0, ws.Range("B2").Value);
            Assert.Equals("B3+$D$3", ws.Range("B2").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        private EWSEditor editor;
    }
}
