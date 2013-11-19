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
    [TestFixture(Categories = "CSVTest")]
    public class CSVTest
    {
        public CSVTest()
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
        public void Test_CSV_SaverLoader()
        {
            Workbook wb = editor.CreateWorkbook("CSV", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Sheets[1].SetName("Hello.Dud");
            wb.Sheets[2].SetName("Sheet#3");
            wb.Sheets[3].SetName("で音楽");

            (wb.Sheets[0] as Worksheet).Range("A1:D6").Value = 6;
            (wb.Sheets[0] as Worksheet).Range("ZZ55").Value = 6;
            (wb.Sheets[1] as Worksheet).Range("B1:E5").Value = 7;
            (wb.Sheets[2] as Worksheet).Range("B3:C9").Value = 8;
            (wb.Sheets[3] as Worksheet).Range("A5:K6").Value = 9;

            wb.Sheets[2].Activate();
            wb.SaveToCSV("../../TestFiles/csv_saver1.csv", System.Text.Encoding.UTF8.CodePage, ',');
            Workbook wb1 = editor.LoadWorkbookFromCSV("../../TestFiles/csv_saver1.csv", 1251);
            Assert.Equals(8, wb1.ActiveWorksheet.Range("B3:C9").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("B2").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("A3").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("A2").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("C10").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("D10").Value);
            Assert.Equals("", wb1.ActiveWorksheet.Range("D9").Value);

            wb.Sheets[0].Activate();
            wb.SaveToCSV("../../TestFiles/csv_saver2.csv", System.Text.Encoding.UTF8.CodePage, ',');
            Workbook wb2 = editor.LoadWorkbookFromCSV("../../TestFiles/csv_saver2.csv", 1251);
            Assert.Equals(6, wb2.ActiveWorksheet.Range("A1:D6").Value);
            Assert.Equals("", wb2.ActiveWorksheet.Range("D7").Value);
            Assert.Equals("", wb2.ActiveWorksheet.Range("E7").Value);
            Assert.Equals("", wb2.ActiveWorksheet.Range("E6").Value);

            wb.Sheets[4].Activate();
            wb.SaveToCSV("../../TestFiles/csv_saver3.csv", System.Text.Encoding.UTF8.CodePage, ',');
            Workbook wb3 = editor.LoadWorkbookFromCSV("../../TestFiles/csv_saver3.csv", 1251);
            Assert.Equals("", wb3.ActiveWorksheet.Range("A1:ZZ66").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_CSV_confusing_data_saver()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/CSV_confuser.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Sheets[0].Activate();
            wb.SaveToCSV("../../TestFiles/CSV_confuser_saved.csv", System.Text.Encoding.UTF8.CodePage, ',');


            StreamReader reader = new StreamReader("../../TestFiles/CSV_confuser_saved.csv");
            string row_string;
            int counter = 0;
            while((row_string = reader.ReadLine()) != null)
            {
                switch (++counter)
                {
                    case 1:
                        Assert.Equals(",\"\"\"----\"\"\",\"Привет, ромашки\",kind of", row_string);
                        break;
                    case 2:
                        Assert.Equals("\"---23-023489-065^5*&547%&376(8()*&0-9-09,\",,,", row_string);
                        break;
                    case 3:
                        Assert.Equals(",,\"\"\"\",Привет%ромашки", row_string);
                        break;
                    case 4:
                        Assert.Equals("25-Jun,19,\"766,726.00\",", row_string);
                        break;
                    case 5:
                        Assert.Equals("        ,-5.58E-13,,", row_string);
                        break;
                    case 6:
                        Assert.Equals(",2.5E+14,,", row_string);
                        break;
                    case 7:
                        Assert.Equals("\",kj,\",,,", row_string);
                        break;
                    default:
                        Assert.Fail("Too many lines in the result file");
                        break;            
                }
            }

            reader.Close();

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_CSV_confusing_data_loader()
        {
            Workbook wb = editor.LoadWorkbookFromCSV("../../TestFiles/CSV_confuser.csv", 1251);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);


            Assert.Equals("", ws.Range("A1").Value);
            Assert.Equals("\"----\"", ws.Range("B1").Value);
            Assert.Equals("Привет, ромашки", ws.Range("C1").Value);
            Assert.Equals("kind of", ws.Range("D1").Value);
            Assert.Equals("---23-023489-065^5*&547%&376(8()*&0-9-09,", ws.Range("A2").Value);
            Assert.Equals("", ws.Range("B2").Value);
            Assert.Equals("", ws.Range("C2").Value);
            Assert.Equals("", ws.Range("D2").Value);
            Assert.Equals("", ws.Range("A3").Value);
            Assert.Equals("", ws.Range("B3").Value);
            Assert.Equals("\"", ws.Range("C3").Value);
            Assert.Equals("Привет%ромашки", ws.Range("D3").Value);
            Assert.Equals("25-Jun", ws.Range("A4").Value);
            Assert.Equals(19, ws.Range("B4").Value);
            Assert.Equals(766726, ws.Range("C4").Value);
            Assert.Equals(-0.000000000000558, ws.Range("B5").Value);
            Assert.Equals(250000000000000, ws.Range("B6").Value);
            Assert.Equals(",kj,", ws.Range("A7").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }


        private EWSEditor editor;
    }
}
