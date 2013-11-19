using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using AVSOfficeEWSEditor.Editor.Events;
using csUnit;
using System.IO;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Locale")]
    public class LocaleTest
    {
        public LocaleTest()
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
        public void Test_En_FloatNumbersSet()
        {
            Workbook wb = editor.CreateWorkbook("float", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:B4").Formula = "TYPE(A1)";
            ws.Range("A1").Value = "100.9";
            ws.Range("A2").Value = "100,9";
            ws.Range("A3").Value = "1009";
            ws.Range("A4").Value = "f1009";

            Assert.Equals(100.9, ws.Range("A1").Value);
            Assert.Equals("100,9", ws.Range("A2").Value);
            Assert.Equals(1009, ws.Range("A3").Value);
            Assert.Equals("f1009", ws.Range("A4").Value);
            Assert.Equals(1, ws.Range("B1").Value);
            Assert.Equals(2, ws.Range("B2").Value);
            Assert.Equals(1, ws.Range("B3").Value);
            Assert.Equals(2, ws.Range("B4").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        private EWSEditor editor;
    }
}
