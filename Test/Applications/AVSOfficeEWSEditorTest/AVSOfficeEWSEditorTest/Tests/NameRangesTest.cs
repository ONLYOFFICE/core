using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using AVSOfficeEWSEditor.Editor.Events;
using csUnit;
using AVSOfficeEWSEditor.Editor.Hyperlinks;
using AVSOfficeEWSEditor.Editor.DefinedNames;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "NameRangesTest")]
    public class NameRangesTest
    {
        public NameRangesTest()
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
        public void Test_NameRangesSimpleDependencies()
        {
            ErrorListener err_listener = new ErrorListener(editor);
            Workbook wb = editor.LoadWorkbook("../../TestFiles/NameRanges_simple.xlsx");
            Assert.NotNull(wb);
            Assert.Null(err_listener.LastError);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            
            ws.Range("A1").Value = 2;
            Assert.Equals(8, ws.Range("C2").Value);

            wb.DefinedNames.AddName("CDNAME", "Лист1!C2:D3");
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            wb.DefinedNames.AddName("CDNAME", "Лист1!$C$2:$D$3");
            ws.Range("A2").Formula = "SUM(CDNAME,C1)";
            Assert.Equals(11, ws.Range("A2").Value);

            ws.Range("A1").Value = 1;
            Assert.Equals(9, ws.Range("A2").Value);

            ws.Range("A3").Formula = "SUM((two,four))";
            Assert.Equals(6, ws.Range("A3").Value);

            ws.Range("A1").DeleteRows();

            Assert.Equals("six", ws.Range("C1").Formula);
            Assert.Equals("SUM(CDNAME,#REF!)", ws.Range("A1").Formula);
            Assert.Equals("SUM((two,four))", ws.Range("A2").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            int counter = 0;
            foreach (DefinedName def_name in wb.DefinedNames)
            {
                switch (++counter)
                {
                    case 1:
                        Assert.Equals("Лист1!$C$1:$D$2", def_name.Formula);
                        Assert.Equals("CDNAME", def_name.Name);
                        break;
                    case 2:
                        Assert.Equals("Лист1!#REF!", def_name.Formula);
                        Assert.Equals("four", def_name.Name);
                        break;
                    case 3:
                        Assert.Equals("two+four", def_name.Formula);
                        Assert.Equals("six", def_name.Name);
                        break;
                    case 4:
                        Assert.Equals("Лист1!#REF!", def_name.Formula);
                        Assert.Equals("two", def_name.Name);
                        break;
                }
            }

            wb.Save("../../TestFiles/NameRanges_simple_res.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }


        private EWSEditor editor;
    }
}
