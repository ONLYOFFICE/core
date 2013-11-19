using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using csUnit;
using AVSOfficeEWSEditor.Editor.Charts;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Charts")]
    public class ChartsTest
    {
        public ChartsTest()
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
        public void ChartsheetsEnumeration()
        {
            using (Workbook wb = editor.LoadWorkbook("../../TestFiles/ChartsSheets.xlsx"))
            {
                Assert.NotNull(wb);
                //             Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

                Assert.NotNull(wb.Sheets["Лист1"] as Worksheet);
                Assert.NotNull(wb.Sheets["Диаграмма2"] as Chartsheet);

                Assert.Equals(145, (wb.Sheets["Лист2"] as Worksheet).Range("A1").Value);
                Assert.Null(wb.ActiveSheet as Worksheet);

                int counter = 0;
                foreach (Worksheet ws in wb.Worksheets)
                {
                    counter++;
                }
                Assert.Equals(6, counter);

                counter = 0;
                foreach (Worksheet ws in wb.WorksheetsN(2, 7))
                {
                    counter++;
                }
                Assert.Equals(4, counter);
                counter = 0;
                foreach (Worksheet ws in wb.WorksheetsN(4, 7))
                {
                    counter++;
                }
                Assert.Equals(3, counter);

            }
        }

        [Test]
        public void ChartInsideChartsheet()
        {
            using (Workbook wb = editor.LoadWorkbook("../../TestFiles/ChartsSheets.xlsx"))
            {
                Assert.NotNull(wb);
                Assert.NotNull((wb.Sheets["Диаграмма1"] as Chartsheet).Chart);
            }
        }

        [Test]
        public void AbsoluteAnchor()
        {
            using (Workbook wb = editor.LoadWorkbook("../../TestFiles/ChartsSheets.xlsx"))
            {
                Assert.NotNull(wb);

                Chartsheet chartsheet1 = wb.Sheets["Диаграмма1"] as Chartsheet;
                Chart chart1 = chartsheet1.Chart;
                Assert.NotNull(chart1);

                AbsoluteAnchor anchor1 = chart1.AbsoluteAnchor;
                Assert.NotNull(anchor1);
                Assert.Equals(9303058 / 12700.0, anchor1.Width, 1e-13);
                Assert.Equals(6075655 / 12700.0, anchor1.Height, 1e-13);
                Assert.Equals(0, anchor1.PosX, 1e-13);
                Assert.Equals(0, anchor1.PosY, 1e-13);

                Chartsheet chartsheet2 = wb.Sheets["Диаграмма2"] as Chartsheet;
                Chart chart2 = chartsheet2.Chart;
                Assert.NotNull(chart2);

                AbsoluteAnchor anchor2 = chart2.AbsoluteAnchor;
                Assert.NotNull(anchor2);
                Assert.Equals(8720461 / 12700.0, anchor2.Width, 1e-13);
                Assert.Equals(3153423 / 12700.0, anchor2.Height, 1e-13);
                Assert.Equals(545606 / 12700.0, anchor2.PosX, 1e-13);
                Assert.Equals(360655 / 12700.0, anchor2.PosY, 1e-13);

            }
        }

        [Test]
        public void TwoCellAnchor()
        {
            using (Workbook wb = editor.LoadWorkbook("../../TestFiles/ChartsSheets.xlsx"))
            {
                Assert.NotNull(wb);

                Worksheet ws1 = wb.Worksheets["Лист1"];
                Assert.NotEquals(0, ws1.Charts.Count);
                Chart chart1 = ws1.Charts[0];
                Assert.NotNull(chart1);

                TwoCellAnchor anchor1 = chart1.TwoCellAnchor;
                Assert.NotNull(anchor1);
                Assert.Equals("B7", anchor1.TopLeftRef);
                Assert.Equals("J21", anchor1.BottomRightRef);
                Assert.Equals(561975 / 12700.0, anchor1.TopLeftOffset.x, 1e-13);
                Assert.Equals(66675 / 12700.0, anchor1.TopLeftOffset.y, 1e-13);
                Assert.Equals(257175 / 12700.0, anchor1.BottomRightOffset.x, 1e-13);
                Assert.Equals(142875 / 12700.0, anchor1.BottomRightOffset.y, 1e-13);
            }
        }

        [Test]
        public void BarSeriesOnMixedChart()
        {
            using (Workbook wb = editor.LoadWorkbook("../../TestFiles/chart_7_full_1.xlsx"))
            {
                Assert.NotNull(wb);

                Worksheet ws1 = wb.Worksheets["Лист1"];
                Assert.NotEquals(0, ws1.Charts.Count);
                Chart chart16 = ws1.Charts[16];
                Assert.NotNull(chart16);

                //chart17.PlotArea.SeriesGroups

            }
        }


        private EWSEditor editor;
    }

}
