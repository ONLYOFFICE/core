/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using csUnit;
using AVSOfficeEWSEditor.Editor;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "MergeCellsTest")]
    public class MergeCellsTest
    {
        public MergeCellsTest()
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
            if (wb != null)
            {
                wb.Close();
                wb = null;
            }
            editor = null;
        }


        [Test]
        public void TestMergeInfoObtaining()
        {
            wb = editor.LoadWorkbook("../../TestFiles/MergedAreas.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            List<string> all_merged_ranges = ws.GetMergedAreas();
            Assert.Equals(10, all_merged_ranges.Count);
            Assert.Equals("C2:G2", all_merged_ranges[4]);
            Assert.Equals("C19:E19", all_merged_ranges[9]);

            Assert.NotEquals(ws.Range("B11:E12").HasMergedCells(), MergeInfo.none);
            Assert.NotEquals(ws.Range("A14:H14").HasMergedCells(), MergeInfo.none);
            Assert.NotEquals(ws.Range("14:14").HasMergedCells(), MergeInfo.none);
            Assert.Equals(ws.Range("11:11").HasMergedCells(), MergeInfo.none);

            Assert.Equals("B12:D17", ws.Range("C14").MergedArea);
            Assert.Equals("", ws.Range("C14:C15").MergedArea);
            Assert.Equals("D21", ws.Range("D21").MergedArea);
            Assert.Equals("F12:G13", ws.Range("F12").MergedArea);
            Assert.Equals("F12:G13", ws.Range("G12").MergedArea);

            Assert.Equals(ws.Range("C3:E3").HasMergedCells(), MergeInfo.none);
            Assert.NotEquals(ws.Range("C3:E6").HasMergedCells(), MergeInfo.none);
            Assert.NotEquals(ws.Range("C3:F3").HasMergedCells(), MergeInfo.none);
        }

        [Test]
        public void TestUnMerge()
        {
            wb = editor.LoadWorkbook("../../TestFiles/MergedAreas.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Equals(10, ws.GetMergedAreas().Count);
            
            ws.Range("E12:E14").UnMerge();
            Assert.Equals(10, ws.GetMergedAreas().Count);

            ws.Range("19:19").UnMerge();
            Assert.Equals(8, ws.GetMergedAreas().Count);

            ws.Range("F13:G14").UnMerge();
            Assert.Equals(6, ws.GetMergedAreas().Count);

            ws.Range("D1:D8").UnMerge();
            Assert.Equals(3, ws.GetMergedAreas().Count);

            ws.Range("C14").UnMerge();
            List<string> all_merged_ranges = ws.GetMergedAreas();
            Assert.Equals(2, all_merged_ranges.Count);
            Assert.Equals("B2:B6", all_merged_ranges[0]);
            Assert.Equals("F3:F8", all_merged_ranges[1]);

            Assert.Equals(0, listener.UpdatedCellsList.Count);
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestMerge()
        {
            wb = editor.LoadWorkbook("../../TestFiles/MergedAreas.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("I4:I6").Merge();
            Assert.Equals(11, ws.GetMergedAreas().Count);
            Assert.Equals(11, listener.UpdatedCellsList.Count);
            Assert.Equals(1, ws.Range("I4").Value);
            Assert.Equals("H4", ws.Range("I4").Formula);
            Assert.Equals("", ws.Range("I5").Value);
            Assert.Equals("", ws.Range("I5").Formula);
            listener.Reset();

            ws.Range("I4:I6").UnMerge();
            Assert.Equals(0, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("H6:I6").Merge();
            Assert.Equals(11, ws.GetMergedAreas().Count);
            Assert.Equals(8, listener.UpdatedCellsList.Count);
            Assert.Equals(3, ws.Range("H6").Value);
            Assert.Equals("H4+H5", ws.Range("H6").Formula);
            listener.Reset();

            // Cyclic            
            ws.Range("I5:J5").Merge();
            Assert.Equals(12, ws.GetMergedAreas().Count);
            Assert.Equals(9, listener.UpdatedCellsList.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("H5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K4:K6").Value);
            listener.Reset();

            ws.Range("G13:G14").Merge();
            Assert.Equals(11, ws.GetMergedAreas().Count);
            Assert.Equals("F12:G15", ws.GetMergedAreas()[10]);
            Assert.Equals(1, ws.Range("F12").Value);
            Assert.Equals("", ws.Range("G12").Value);
            Assert.Equals("", ws.Range("F13:G15").Value);

            ws.Range("D16:F16").Merge();
            Assert.Equals(8, ws.GetMergedAreas().Count);
            Assert.Equals("B12:G20", ws.GetMergedAreas()[7]);
            Assert.Equals(1, ws.Range("B12").Value);
            Assert.Equals("", ws.Range("B13:G20").Value);

            ws.Range("E5:F5").Merge();
            Assert.Equals(4, ws.GetMergedAreas().Count);
            Assert.Equals("A2:G8", ws.GetMergedAreas()[3]);
            Assert.Equals(-625, ws.Range("A2").Value);
            Assert.Equals("C2-25^2", ws.Range("A2").Formula);
            
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Save("../../TestFiles/MergedAreas_res1.xlsx");
        }

        [Test]
        public void TestTooEarlyUpdateBug()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A2").Value = 5;
            ws.Range("C3").Formula = "5+A2";

            Assert.Equals(10, ws.Range("C3").Value);

            ws.Range("A1:B2").Merge();

            Assert.Equals(5, ws.Range("C3").Value); // This returns 10 if the dependent recalculation is performed before A2 is deleted (too early).
        }

        [Test]
        public void TestDeleteRowsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:C15").Merge();
            ws.Range("A30:D45").Merge();
            ws.Range("E20:F35").Merge();
            ws.Range("G5:H40").Merge();
            ws.Range("I15:K20").Merge();

            ws.Range("A10:A25").DeleteRows();

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(4, merged_areas.Count);
            Assert.Equals("B1:C9", merged_areas[0]);
            Assert.Equals("A14:D29", merged_areas[1]);
            Assert.Equals("E10:F19", merged_areas[2]);
            Assert.Equals("G5:H24", merged_areas[3]);
        }

        [Test]
        public void TestInsertRowsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A15:B30").Merge();
            ws.Range("C10:D17").Merge();
            ws.Range("C25:D35").Merge();

            ws.Range("A20:A25").InsertRows(5);

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(3, merged_areas.Count);
            Assert.Equals("A15:B35", merged_areas[0]);
            Assert.Equals("C10:D17", merged_areas[1]);
            Assert.Equals("C30:D40", merged_areas[2]);
        }

        [Test]
        public void TestBoundaryDeleteRowsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A5:A10").Merge();
            ws.Range("B5:B15").Merge();
            ws.Range("C5:C9").Merge();
            ws.Range("D5:D16").Merge();
            ws.Range("E9:E20").Merge();
            ws.Range("F10:F20").Merge();
            ws.Range("G15:G20").Merge();
            ws.Range("H16:H20").Merge();
            ws.Range("I10:I15").Merge();
            ws.Range("J10:J16").Merge();
            ws.Range("K9:K15").Merge();
            ws.Range("L9:L16").Merge();

            ws.Range("A10:A15").DeleteRows();

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(9, merged_areas.Count);
            Assert.Equals("A5:A9", merged_areas[0]);
            Assert.Equals("B5:B9", merged_areas[1]);
            Assert.Equals("C5:C9", merged_areas[2]);
            Assert.Equals("D5:D10", merged_areas[3]);
            Assert.Equals("E9:E14", merged_areas[4]);
            Assert.Equals("F10:F14", merged_areas[5]);
            Assert.Equals("G10:G14", merged_areas[6]);
            Assert.Equals("H10:H14", merged_areas[7]);
            Assert.Equals("L9:L10", merged_areas[8]);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestBoundaryInsertRowsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A5:A9").Merge();
            ws.Range("B5:B10").Merge();
            ws.Range("C9:C15").Merge();
            ws.Range("D10:D15").Merge();
            ws.Range("E9:F9").Merge();
            ws.Range("G10:H10").Merge();
            ws.Range("I9:I10").Merge();

            ws.Range("A10").InsertRows(5);

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(7, merged_areas.Count);
            Assert.Equals("A5:A9", merged_areas[0]);
            Assert.Equals("B5:B15", merged_areas[1]);
            Assert.Equals("C9:C20", merged_areas[2]);
            Assert.Equals("D15:D20", merged_areas[3]);
            Assert.Equals("E9:F9", merged_areas[4]);
            Assert.Equals("G15:H15", merged_areas[5]);
            Assert.Equals("I9:I15", merged_areas[6]);
        }

        [Test]
        public void TestDeleteColumnsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:G1").Merge();
            ws.Range("G2:L2").Merge();
            ws.Range("F3:H3").Merge();
            ws.Range("B4:L4").Merge();
            ws.Range("B5:C5").Merge();
            ws.Range("K5:L5").Merge();

            ws.Range("E:I").DeleteColumns();

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(5, merged_areas.Count);
            Assert.Equals("B1:D1", merged_areas[0]);
            Assert.Equals("E2:G2", merged_areas[1]);
            Assert.Equals("B4:G4", merged_areas[2]);
            Assert.Equals("B5:C5", merged_areas[3]);
            Assert.Equals("F5:G5", merged_areas[4]);
        }

        [Test]
        public void TestInsertColumnsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:E1").Merge();
            ws.Range("H1:J1").Merge();
            ws.Range("D2:I2").Merge();

            ws.Range("G1").InsertColumn();

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(3, merged_areas.Count);
            Assert.Equals("B1:E1", merged_areas[0]);
            Assert.Equals("I1:K1", merged_areas[1]);
            Assert.Equals("D2:J2", merged_areas[2]);
        }

        [Test]
        public void TestBoundaryDeleteColumnsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:B2").Merge();
            ws.Range("B3:C4").Merge();
            ws.Range("B5:E5").Merge();
            ws.Range("B6:F6").Merge();
            ws.Range("C7:C8").Merge();
            ws.Range("C9:E9").Merge();
            ws.Range("C10:F10").Merge();
            ws.Range("E11:E12").Merge();
            ws.Range("E13:F14").Merge();
            ws.Range("F15:F16").Merge();
            ws.Range("17:18").Merge();

            ws.Range("C:E").DeleteColumns();

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(6, merged_areas.Count);
            Assert.Equals("B1:B2", merged_areas[0]);
            Assert.Equals("B3:B4", merged_areas[1]);
            Assert.Equals("B6:C6", merged_areas[2]);
            Assert.Equals("C13:C14", merged_areas[3]);
            Assert.Equals("C15:C16", merged_areas[4]);
            Assert.Equals("17:18", merged_areas[5]);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestBoundaryInsertColumnsWithMerge()
        {
            wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:E1").Merge();
            ws.Range("B2:F2").Merge();
            ws.Range("E3:J3").Merge();
            ws.Range("F4:J4").Merge();
            ws.Range("E5:F6").Merge();
            ws.Range("7:9").Merge();

            ws.Range("F10").InsertColumns(2);

            List<string> merged_areas = ws.GetMergedAreas();
            Assert.Equals(6, merged_areas.Count);
            Assert.Equals("B1:E1", merged_areas[0]);
            Assert.Equals("B2:H2", merged_areas[1]);
            Assert.Equals("E3:L3", merged_areas[2]);
            Assert.Equals("H4:L4", merged_areas[3]);
            Assert.Equals("E5:H6", merged_areas[4]);
            Assert.Equals("7:9", merged_areas[5]);
        }

        [Test]
        public void TestWrongValueSetAsTopLeft()
        {
            wb = editor.LoadWorkbook("../../TestFiles/MergeWrongValueSetAsTopLeft.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1:C3").Merge();

            ws.Range("A4").Formula = "TYPE(A1)";
            Assert.Equals(2, ws.Range("A4").Value);
        }

        private EWSEditor editor;
        private Workbook wb;
   }
}
