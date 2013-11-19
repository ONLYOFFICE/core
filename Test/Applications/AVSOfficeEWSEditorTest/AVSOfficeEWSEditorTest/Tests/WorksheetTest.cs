using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using csUnit;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture]
    public class WorksheetTest
    {
        public WorksheetTest()
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
        public void TestPosition()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Worksheet ws_rus = wb.Sheets.AddEmptySheet("Русский лист", null);
            wb.Sheets.AddEmptySheet("だぶゆーてぃーえふ", ws_rus);

            Assert.Equals(2, editor.ActiveWorkbook.Worksheets["だぶゆーてぃーえふ"].Index);
            Assert.Equals(4, editor.ActiveWorkbook.Worksheets.Count);
            editor.ActiveWorkbook.Sheets.AddEmptySheet("new_empty_sheet", editor.ActiveWorkbook.Worksheets[0]);
            Assert.Equals(0, editor.ActiveWorksheet.Index);
            Assert.Equals(true, editor.ActiveWorksheet.Remove());
            Assert.Equals(0, editor.ActiveWorksheet.Index);
            Assert.Equals("Sheet1", editor.ActiveWorksheet.Name);
            Assert.Null(editor.ActiveWorkbook.Worksheets["new_empty_sheet"]);
            editor.ActiveWorkbook.Worksheets[editor.ActiveWorkbook.Worksheets.Count - 1].Activate();
            Assert.Equals(editor.ActiveWorkbook.Worksheets.Count - 1, editor.ActiveWorksheet.Index);
        }

        [Test]
        public void TestDeleteLastSheet()
        {
            Workbook wb= editor.CreateWorkbook("Book2", 2);
            Assert.True(wb.Worksheets[0].Remove());
            Assert.False(wb.Worksheets[0].Remove());
        }

        [Test]
        public void TestName()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Worksheet ws_rus = wb.Sheets.AddEmptySheet("Русский лист", null);
            wb.Sheets.AddEmptySheet("だぶゆーてぃーえふ", ws_rus);

            Assert.Equals("だぶゆーてぃーえふ", editor.ActiveWorksheet.Name);
            Assert.Equals(false, editor.ActiveWorksheet.SetName(""));
            Assert.Equals(true, editor.ActiveWorksheet.SetName("1"));
            Assert.Equals("1", editor.ActiveWorksheet.Name);
            Assert.Equals("1", editor.ActiveWorkbook.Worksheets["1"].Name);
        }

        [Test]
        public void TestBackReference()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Assert.Equals(wb, editor.ActiveWorksheet.Workbook);
        }



        private EWSEditor editor;
    }
}
