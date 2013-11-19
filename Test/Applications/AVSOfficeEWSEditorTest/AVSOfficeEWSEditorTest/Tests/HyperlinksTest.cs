using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using AVSOfficeEWSEditor.Editor.Events;
using csUnit;
using AVSOfficeEWSEditor.Editor.Hyperlinks;
using AVSOfficeEWSEditor.Editor.Sorting;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "HyperlinksTest")]
    public class HyperlinksTest
    {
        public HyperlinksTest()
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
        public void Test_LoadVariousTypesOfLinks()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Equals(4, ws.Hyperlinks.Count);
            int counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch(++counter)
                {
                    case 1:
                        Assert.Equals("B3", hyp.Anchor);
                        Assert.Equals("http://teamlab/products/projects/tasks.aspx?prjID=18&id=1476", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                    case 2:
                        Assert.Equals("D3", hyp.Anchor);
                        Assert.Equals("http://yandex.ru/yandsearch?clid=14585&text=%D0%9D%D0%B0%D0%B9%D1%82%D0%B8+%D0%B2+%D0%AF%D0%BD%D0%B4%D0%B5%D0%BA%D1%81%D0%B5&lr=47", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                    case 3:
                        Assert.Equals("B5", hyp.Anchor);
                        Assert.Equals("Лист3", hyp.ReferredSheet.Name);
                        Assert.Equals("A1:B2", hyp.ReferredRange);
                        Assert.Equals("", hyp.Target);
                        break;
                    case 4:
                        Assert.Equals("D5", hyp.Anchor);
                        Assert.Equals("Лист1", hyp.ReferredSheet.Name);
                        Assert.Equals("B3", hyp.ReferredRange);
                        Assert.Equals("", hyp.Target);
                        break;
                }
            }

            Assert.Equals(counter, ws.Hyperlinks.Count);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkAppendToExistentOnes()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Hyperlinks.AddAddressHyperlink("A1", "www.ya.ru");
            ws.Hyperlinks.AddLocationHyperlink("B1", wb.Worksheets[1], "ZZ55");



            Assert.Equals(6, ws.Hyperlinks.Count);
            int counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch(++counter)
                {
                    case 5:
                        Assert.Equals("www.ya.ru", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                    case 6:
                        Assert.Equals("Лист2", hyp.ReferredSheet.Name);
                        Assert.Equals("ZZ55", hyp.ReferredRange);
                        Assert.Equals("", hyp.Target);
                        break;
                }
            }

            wb.Save("../../TestFiles/Hyperlinks_res1.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkAppendToEmptySection()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Worksheets[1];
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Hyperlinks.AddAddressHyperlink("A1", "file://D:/temp");



            Assert.Equals(1, ws.Hyperlinks.Count);
            int counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch (++counter)
                {
                    case 1:
                        Assert.Equals("file://D:/temp", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                }
            }

            wb.Save("../../TestFiles/Hyperlinks_res2.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkRedefining()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Hyperlinks.AddAddressHyperlink("B5", "file://D:/temp");



            Assert.Equals(4, ws.Hyperlinks.Count);
            int counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch (++counter)
                {
                    case 4:
                        Assert.Equals("file://D:/temp", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                }
            }

            wb.Save("../../TestFiles/Hyperlinks_res3.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkSetToEmptyCell()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Hyperlinks.AddAddressHyperlink("A1", "file://D:/temp2");
            ws.Hyperlinks.AddAddressHyperlink("A2", "file://D:/temp2");

            Assert.Equals(1, ws.Range("A1").Value);
            Assert.Equals("file://D:/temp2", ws.Range("A2").Value);

            wb.Save("../../TestFiles/Hyperlinks_res4.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkDeletionUponCellsDeletion()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B3:D3").DeleteRows();


            Assert.Equals(2, ws.Hyperlinks.Count);
            int counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch (++counter)
                {
                    case 1:
                        Assert.Equals("B4", hyp.Anchor);
                        Assert.Equals("Лист3", hyp.ReferredSheet.Name);
                        Assert.Equals("A1:B2", hyp.ReferredRange);
                        Assert.Equals("", hyp.Target);
                        break;

                    case 2:
                        Assert.Equals("D4", hyp.Anchor);
                        Assert.Equals("Лист1", hyp.ReferredSheet.Name);
                        Assert.Equals("B3", hyp.ReferredRange);
                        Assert.Equals("", hyp.Target);
                        break;
                }
            }

            ws.Hyperlinks.AddAddressHyperlink("D4", "http://microsoft.com/");

            Assert.Equals(2, ws.Hyperlinks.Count);
            counter = 0;
            foreach (Hyperlink hyp in ws.Hyperlinks)
            {
                switch (++counter)
                {
                    case 2:
                        Assert.Equals("D4", hyp.Anchor);
                        Assert.Equals("http://microsoft.com/", hyp.Target);
                        Assert.Null(hyp.ReferredSheet);
                        break;
                }
            }

            Assert.Equals(counter, ws.Hyperlinks.Count);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkMovingOnSort()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:D5").Sort(SortOrientation.by_rows, ws.Range("B:B"), SortOrder.ascending);

            Assert.Equals(15, listener.UpdatedCellsList.Count);
            Assert.Equals(4, ws.Hyperlinks.Count);
            // link 1
            Hyperlink hyp1 = ws.Hyperlinks["B2"];
            Assert.Equals("http://teamlab/products/projects/tasks.aspx?prjID=18&id=1476", hyp1.Target);
            Assert.Equals(false, hyp1.IsLocationOrAddressType);

            // link 2
            Hyperlink hyp2 = ws.Hyperlinks["B3"];
            Assert.Equals("Лист3", hyp2.ReferredSheet.Name);
            Assert.Equals("A1:B2", hyp2.ReferredRange);
            Assert.Equals(true, hyp2.IsLocationOrAddressType);
            Assert.Null(ws.Hyperlinks["B5"]);

            // link 3
            Hyperlink hyp3 = ws.Hyperlinks["D2"];
            Assert.Equals("http://yandex.ru/yandsearch?clid=14585&text=%D0%9D%D0%B0%D0%B9%D1%82%D0%B8+%D0%B2+%D0%AF%D0%BD%D0%B4%D0%B5%D0%BA%D1%81%D0%B5&lr=47", hyp3.Target);
            Assert.Equals(false, hyp3.IsLocationOrAddressType);
            
            // link 4
            Hyperlink hyp4 = ws.Hyperlinks["D3"];
            Assert.Equals("Лист1", hyp4.ReferredSheet.Name);
            Assert.Equals("B3", hyp4.ReferredRange);
            Assert.Equals(true, hyp2.IsLocationOrAddressType);
            Assert.Null(ws.Hyperlinks["D5"]);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_HyperlinkCopyOnRangeCopy()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Hyperlinks.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:C5").CopyTo(wb.Worksheets["Лист2"].Range("A2"));

            Assert.Equals(0, listener.UpdatedCellsList.Count);
            Assert.Equals(4, ws.Hyperlinks.Count);

            Worksheet ws_dest = wb.Worksheets["Лист2"];
            Assert.Equals(2, ws_dest.Hyperlinks.Count);

            // link 1
            Hyperlink hyp1 = ws_dest.Hyperlinks["A4"];
            Assert.Equals("http://teamlab/products/projects/tasks.aspx?prjID=18&id=1476", hyp1.Target);
            Assert.Equals(false, hyp1.IsLocationOrAddressType);

            // link 2
            Hyperlink hyp2 = ws_dest.Hyperlinks["A6"];
            Assert.Equals("Лист3", hyp2.ReferredSheet.Name);
            Assert.Equals("A1:B2", hyp2.ReferredRange);
            Assert.Equals(true, hyp2.IsLocationOrAddressType);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }


        private EWSEditor editor;
    }
}
