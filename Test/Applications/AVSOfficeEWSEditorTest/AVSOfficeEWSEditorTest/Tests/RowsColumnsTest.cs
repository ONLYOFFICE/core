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
using AVSOfficeEWSEditor.Editor;
using csUnit;
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "RowsColumns")]
    public class RowsColumnsTest
    {
        public RowsColumnsTest()
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
        public void TestRowSet()
        {
            editor.CreateWorkbook("Book1", 5);
            Worksheet empty_sheet = editor.ActiveWorksheet;

            System.Console.WriteLine("TestRowSet started...");
            DateTime before = DateTime.Now;

            empty_sheet.Range("4:5").Value = false;
            empty_sheet.Range("C3:E9").Value = "#REF!";
            empty_sheet.Range("4:8").Value = true;

            DateTime after = DateTime.Now;
            TimeSpan delta = new TimeSpan(after.Ticks - before.Ticks);
            System.Console.WriteLine("TestRowSet lasted " + delta.TotalSeconds + " seconds");

            empty_sheet.Workbook.Close();
        }

        [Test, Ignore("Time consuming")]
        public void TestColumnSet()
        {
            editor.CreateWorkbook("Book1", 5);
            Worksheet empty_sheet = editor.ActiveWorksheet;

            System.Console.WriteLine("TestColumnSet started...");
            DateTime before = DateTime.Now;

            empty_sheet.Range("A:A").Value = "nothing";

            DateTime after = DateTime.Now;
            TimeSpan delta = new TimeSpan(after.Ticks - before.Ticks);
            System.Console.WriteLine("TestColumnSet lasted " + delta.TotalSeconds + " seconds");

            empty_sheet.Workbook.Close();
        }

        [Test]
        public void TestRowHeight()
        {
            Assert.NotNull(editor.LoadWorkbook("../../TestFiles/DataOfVarTypes.xlsx"));
            Worksheet var_data_sheet = editor.ActiveWorksheet;
            
            var_data_sheet.Range("A1:F3").RowHeight = 45.1;
            var_data_sheet.Range("8:15").RowHeight = 5.4;
            Assert.Equals(45.1, var_data_sheet.Range("M2:M3").RowHeight);
            Assert.Equals(14.25, var_data_sheet.Range("ZZ4").RowHeight);
            Assert.Null(var_data_sheet.Range("M3:ZZ4").RowHeight);
            Assert.Equals(90.2, var_data_sheet.Range("M2:M3").Height);
            Assert.Equals(14.25, var_data_sheet.Range("ZZ4").Height);
            Assert.Equals(14.25 + 45.1, var_data_sheet.Range("M3:ZZ4").Height);

            var_data_sheet.Workbook.Close();
        }

        [Test]
        public void TestColumnWidth()
        {
            Assert.NotNull(editor.LoadWorkbook("../../TestFiles/DataOfVarTypes.xlsx"));
            Worksheet var_data_sheet = editor.ActiveWorksheet;
            
            var_data_sheet.Range("M2:O3").ColumnWidth = 8;
            var_data_sheet.Range("N2:Z13").ColumnWidth = 5.5;
            Assert.Equals(5.5, var_data_sheet.Range("N2:O3").ColumnWidth);
            Assert.Equals(8.43, var_data_sheet.Range("ZZ4").ColumnWidth);
            Assert.Null(var_data_sheet.Range("M3:ZZ4").ColumnWidth);
            Assert.Equals(8 + 5.5, var_data_sheet.Range("M:N").Width);
            Assert.Equals(5.5, var_data_sheet.Range("O1:O33").Width);
            Assert.Equals(8.43, var_data_sheet.Range("ZZ4").Width);
            var_data_sheet.Range("J:S").ColumnWidth = 3;
            Assert.Equals(3 + 3, var_data_sheet.Range("M:N").Width);
            Assert.Equals(8.43, var_data_sheet.Range("I4").Width);
            Assert.Equals(3, var_data_sheet.Range("J4").Width);

            var_data_sheet.Workbook.Close();
        }

        [Test]
        public void TestInsertDeleteRowsColumns()
        {
            Assert.NotNull(editor.LoadWorkbook("../../TestFiles/DataOfVarTypes.xlsx"));
            Worksheet var_data_sheet = editor.ActiveWorksheet;

            var_data_sheet.Range("J14:DDD55").Value = 55;

            System.Console.WriteLine("TestInsertDeleteRowsColumns started...");
            DateTime before = DateTime.Now;

            var_data_sheet.Range("C3:D5").InsertRow();
            var_data_sheet.Range("C3:D5").DeleteRows();
            var_data_sheet.Range("C3:D5").InsertRows(4);
            var_data_sheet.Range("M3:Z5").InsertColumns(2);
            var_data_sheet.Range("H:P").DeleteColumns();
            var_data_sheet.Range("M1:N1").DeleteColumns();
            var_data_sheet.Range("Q:Q").DeleteColumns();

            DateTime after = DateTime.Now;
            TimeSpan delta = new TimeSpan(after.Ticks - before.Ticks);
            System.Console.WriteLine("TestInsertDeleteRowsColumns lasted " + delta.TotalSeconds + " seconds");

            var_data_sheet.Workbook.Close();
        }

        #region Remove rows tests

        [Test]
        public void TestRemovedCellsReferencesUpdate()
        {
            Workbook wb = editor.CreateWorkbook("RemovedCellsReferencesUpdate");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1:D1").Value = 5;
            ws.Range("A2").Formula = "A1 + B1";
            ws.Range("B2").Formula = "A2 + B1";
            ws.Range("C2").Formula = "(D1^(D3+1))";
            ws.Range("D2").Formula = "--+-A1+5";
            ws.Range("A3").Formula = "A2-B2";
            ws.Range("A4").Formula = "A3%";


            Assert.Equals(10, ws.Range("A2").Value);
            Assert.Equals(15, ws.Range("B2").Value);
            Assert.Equals(5, ws.Range("C2").Value);
            Assert.Equals(0, ws.Range("D2").Value);
            Assert.Equals(-5, ws.Range("A3").Value);
            Assert.Equals(-0.05, ws.Range("A4").Value);

            ws.Range("A1").DeleteRows();

            Assert.Equals("#REF!+#REF!", ws.Range("A1").Formula);
            Assert.Equals("A1+#REF!", ws.Range("B1").Formula);
            Assert.Equals("(#REF!^(D2+1))", ws.Range("C1").Formula); // Test that non-existent subjects have been updated too
            Assert.Equals("--+-#REF!+5", ws.Range("D1").Formula);
            Assert.Equals("A1-B1", ws.Range("A2").Formula);
            Assert.Equals("A2%", ws.Range("A3").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRemovedCellsWrongSubstitution()
        {
            Workbook wb = editor.CreateWorkbook("TestRemovedCellsWrongSubstitution");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A2").Value = 2;
            ws.Range("A3").Value = 4;
            ws.Range("B3").Formula = "A2+A3";

            ws.Range("D2").Value = 2;
            ws.Range("D3").Formula = "D2*2";
            ws.Range("E3").Formula = "D2^D3";
            ws.Range("C3").Formula = "D2-5";

            Assert.Equals(6, ws.Range("B3").Value);
            Assert.Equals(4, ws.Range("D3").Value);
            Assert.Equals(16, ws.Range("E3").Value);
            Assert.Equals(-3, ws.Range("C3").Value);

            ws.Range("A6").Value = 1;
            ws.Range("A5").Formula = "A6/2";
            ws.Range("B5").Formula = "A6/4+A5";

            Assert.Equals(0.75, ws.Range("B5").Value);

            ws.Range("A1").DeleteRows();

            Assert.Equals("A1+A2", ws.Range("B2").Formula);
            Assert.Equals(6, ws.Range("B2").Value);
            ws.Range("A2").Value = 5;
            ws.Range("A3").Value = 12;
            Assert.Equals(7, ws.Range("B2").Value);

            Assert.Equals("D1*2", ws.Range("D2").Formula);
            Assert.Equals(4, ws.Range("D2").Value);
            Assert.Equals("D1^D2", ws.Range("E2").Formula);
            Assert.Equals(16, ws.Range("E2").Value);
            Assert.Equals("D1-5", ws.Range("C2").Formula);
            Assert.Equals(-3, ws.Range("C2").Value);
            ws.Range("D1").Value = 3;
            Assert.Equals(6, ws.Range("D2").Value);
            Assert.Equals(729, ws.Range("E2").Value);
            Assert.Equals(-2, ws.Range("C2").Value);

            Assert.Equals("A5/2", ws.Range("A4").Formula);
            Assert.Equals(0.5, ws.Range("A4").Value);
            Assert.Equals("A5/4+A4", ws.Range("B4").Formula);
            Assert.Equals(0.75, ws.Range("B4").Value);
            ws.Range("A5").Value = 4;
            Assert.Equals(2, ws.Range("A4").Value);
            Assert.Equals(3, ws.Range("B4").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRemoveRowsComplex()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/DeleteRowsComplex.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);


            ws.Range("A8:A12").DeleteRows();

            Assert.Equals(46, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals("D4+E4", ws.Range("D6").Formula);
            Assert.Equals(3, ws.Range("D6").Value);
            Assert.Equals("E4+F4+E11+C11", ws.Range("E6").Formula);
            Assert.Equals(141, ws.Range("E6").Value);
            Assert.Equals("G4+#REF!+G11", ws.Range("G6").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G6").Value);
            Assert.Equals("N10*5", ws.Range("N5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("N5").Value);
            Assert.Equals("B11-C11^2+C9", ws.Range("B9").Formula);
            Assert.Equals(-4421, ws.Range("B9").Value);
            Assert.Equals("J13-J11", ws.Range("K9").Formula);
            Assert.Equals(-261, ws.Range("K9").Value);
            Assert.Equals("#REF!*5", ws.Range("L10").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("L10").Value);
            Assert.Equals("L10*5", ws.Range("N10").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("N10").Value);
            Assert.Equals("C11+1", ws.Range("B11").Formula);
            Assert.Equals(68, ws.Range("B11").Value);
            Assert.Equals("72+H15", ws.Range("H11").Formula);
            Assert.Equals(72, ws.Range("H11").Value);
            Assert.Equals("I11+H11", ws.Range("J11").Formula);
            Assert.Equals(145, ws.Range("J11").Value);
            Assert.Equals("J11/5", ws.Range("K11").Formula);
            Assert.Equals(29, ws.Range("K11").Value);
            Assert.Equals("D11+E11", ws.Range("E13").Formula);
            Assert.Equals(137, ws.Range("E13").Value);
            Assert.Equals("E4+#REF!", ws.Range("F13").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F13").Value);
            Assert.Equals("K11-J11", ws.Range("J13").Formula);
            Assert.Equals(-116, ws.Range("J13").Value);

            ws.Range("H15").Value = 12;
            Assert.Equals(84, ws.Range("H11").Value);
            Assert.Equals(157, ws.Range("J11").Value);
            Assert.Equals(31.4, ws.Range("K11").Value);
            Assert.Equals(-125.6, ws.Range("J13").Value);
            Assert.Equals(-282.6, ws.Range("K9").Value);

            ws.Range("C11").Value = 68;
            Assert.Equals(142, ws.Range("E6").Value);

            ws.Range("E4").Value = 4;
            Assert.Equals(144, ws.Range("E6").Value);
            Assert.Equals(5, ws.Range("D6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F13").Value);



            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRemovedCellsRedundantUpdate()
        {
            Workbook wb = editor.CreateWorkbook("TestRemovedCellsRedundantUpdate");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = 2;
            ws.Range("A3").Value = 3;
            ws.Range("A4").Value = 4;
            ws.Range("A5").Value = 5;
            ws.Range("A6").Value = 6;
            ws.Range("A7").Value = 7;
            ws.Range("A8").Value = 8;
            ws.Range("A9").Value = 9;
            ws.Range("A10").Value = 10;


            ws.Range("B1:B10").Formula = "A1+3";

            ws.Range("4:4").DeleteRows();

            Assert.Equals(1, ws.Range("A1").Value);
            Assert.Equals(2, ws.Range("A2").Value);
            Assert.Equals(3, ws.Range("A3").Value);
            Assert.Equals(5, ws.Range("A4").Value);
            Assert.Equals(6, ws.Range("A5").Value);
            Assert.Equals(7, ws.Range("A6").Value);
            Assert.Equals(8, ws.Range("A7").Value);
            Assert.Equals(9, ws.Range("A8").Value);
            Assert.Equals(10, ws.Range("A9").Value);
            Assert.Equals("", ws.Range("A10").Value);

            Assert.Equals(4, ws.Range("B1").Value);
            Assert.Equals(5, ws.Range("B2").Value);
            Assert.Equals(6, ws.Range("B3").Value);
            Assert.Equals(8, ws.Range("B4").Value);
            Assert.Equals(9, ws.Range("B5").Value);
            Assert.Equals(10, ws.Range("B6").Value);
            Assert.Equals(11, ws.Range("B7").Value);
            Assert.Equals(12, ws.Range("B8").Value);
            Assert.Equals(13, ws.Range("B9").Value);
            Assert.Equals("", ws.Range("B10").Value);

            Assert.Equals("A1+3", ws.Range("B1").Formula);
            Assert.Equals("A2+3", ws.Range("B2").Formula);
            Assert.Equals("A3+3", ws.Range("B3").Formula);
            Assert.Equals("A4+3", ws.Range("B4").Formula);
            Assert.Equals("A5+3", ws.Range("B5").Formula);
            Assert.Equals("A6+3", ws.Range("B6").Formula);
            Assert.Equals("A7+3", ws.Range("B7").Formula);
            Assert.Equals("A8+3", ws.Range("B8").Formula);
            Assert.Equals("A9+3", ws.Range("B9").Formula);
            Assert.Equals("", ws.Range("B10").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestWrongRangeReferenceDeletion()
        {
            Workbook wb = editor.CreateWorkbook("WrongRangeReferenceDeletion");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B3:C4").Value = 5;
            ws.Range("B1").Formula = "MAX(B3:C4)";

            ws.Range("3:6").DeleteRows();

            Assert.Equals("MAX(#REF!)", ws.Range("B1").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestWrongRangeReferenceCut()
        {
            Workbook wb = editor.CreateWorkbook("TestWrongRangeReferenceCut");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D1").Formula = "COUNT(B1:C10)";

            ws.Range("8:13").DeleteRows();

            Assert.Equals("COUNT(B1:C7)", ws.Range("D1").Formula);
            Assert.Equals(14, ws.Range("D1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestWrongCutRangeReferenceUpdate()
        {
            Workbook wb = editor.CreateWorkbook("TestWrongRangeReferenceCut");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D1").Formula = "SUM(B1:C10)";

            ws.Range("5:8").DeleteRows();

            Assert.Equals("SUM(B1:C6)", ws.Range("D1").Formula);
            Assert.Equals(60, ws.Range("D1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRangesReferencesOverDeletedRows()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/RangesReferencesOverDeletedRows.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            // Verify correct load:

            Assert.Equals("SUM(F14:G14)", ws.Range("C1").Formula);
            Assert.Equals(2, ws.Range("C1").Value);
            Assert.Equals("SUM(F14,G14)", ws.Range("D1").Formula);
            Assert.Equals(2, ws.Range("D1").Value);
            Assert.Equals("MAX(C8:D9)", ws.Range("C5").Formula);
            Assert.Equals(12, ws.Range("C5").Value);
            Assert.Equals("SUM(COUNT(B5:C10)*7,F8:G14)", ws.Range("D5").Formula);
            Assert.Equals(333, ws.Range("D5").Value);
            Assert.Equals("MIN(1:1)", ws.Range("E5").Formula);
            Assert.Equals(2, ws.Range("E5").Value);
            Assert.Equals("MAX(F14:G14)", ws.Range("F5").Formula);
            Assert.Equals(2, ws.Range("F5").Value);
            Assert.Equals("SUM(E5:F14)", ws.Range("G5").Formula);
            Assert.Equals(211, ws.Range("G5").Value);
            Assert.Equals("SUM(C:D)", ws.Range("H5").Formula);
            Assert.Equals(1019.0416666666666, (double)ws.Range("H5").Value, 1e-10);
            Assert.Equals("SUM(C1:D1)", ws.Range("F12").Formula);
            Assert.Equals(4, ws.Range("F12").Value);
            Assert.Equals("SUM(F10:G14)", ws.Range("H12").Formula);
            Assert.Equals(201, ws.Range("H12").Value);
            Assert.Equals("SUM(G5:H5,D15:F15)", ws.Range("I14").Formula);
            Assert.Equals(1245.0416666666665, (double)ws.Range("I14").Value, 1e-10);
            Assert.Equals("SUM(D5:E16)", ws.Range("C16").Formula);
            Assert.Equals(496, ws.Range("C16").Value);
            Assert.Equals("AVERAGE(C5:D10)", ws.Range("C17").Formula);
            Assert.Equals(53.75, ws.Range("C17").Value);
            Assert.Equals("SUM(D15:F17)", ws.Range("G17").Formula);
            Assert.Equals(72, ws.Range("G17").Value);
            Assert.Equals("AVERAGE(C1:D5)", ws.Range("H17").Formula);
            Assert.Equals(87.25, ws.Range("H17").Value);
            Assert.Equals("AVERAGE(5,D8:F17,C1:D1,G13)", ws.Range("D18").Formula);
            Assert.Equals(14.291666666666666, (double)ws.Range("D18").Value, 1e-10);
            Assert.Equals("SUM(B8:C13)", ws.Range("E18").Formula);
            Assert.Equals(45, ws.Range("E18").Value);
            Assert.Equals("SUM(C10:C10)", ws.Range("F18").Formula);
            Assert.Equals(23, ws.Range("F18").Value);

            // The action
            ws.Range("8:13").DeleteRows();

            wb.Save("../../TestFiles/RangesReferencesOverDeletedRows_editor_result.xlsx");

            // The action effect
            Assert.Equals("SUM(F8:G8)", ws.Range("C1").Formula);
            Assert.Equals(2, ws.Range("C1").Value);
            Assert.Equals("SUM(F8,G8)", ws.Range("D1").Formula);
            Assert.Equals(2, ws.Range("D1").Value);

            Assert.Equals("MAX(#REF!)", ws.Range("C5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C5").Value);
            Assert.Equals("SUM(COUNT(B5:C7)*7,F8:G8)", ws.Range("D5").Formula);
            Assert.Equals(2, ws.Range("D5").Value);
            Assert.Equals("MIN(1:1)", ws.Range("E5").Formula);
            Assert.Equals(2, ws.Range("E5").Value);
            Assert.Equals("MAX(F8:G8)", ws.Range("F5").Formula);
            Assert.Equals(2, ws.Range("F5").Value);
            Assert.Equals("SUM(E5:F8)", ws.Range("G5").Formula);
            Assert.Equals(6, ws.Range("G5").Value);
            Assert.Equals("SUM(C:D)", ws.Range("H5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("H5").Value);
            Assert.Equals("SUM(G5:H5,D9:F9)", ws.Range("I8").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I8").Value);
            Assert.Equals("SUM(D5:E10)", ws.Range("C10").Formula);
            Assert.Equals(28, ws.Range("C10").Value);
            Assert.Equals("AVERAGE(C5:D7)", ws.Range("C11").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C11").Value);
            Assert.Equals("SUM(D9:F11)", ws.Range("G11").Formula);
            Assert.Equals(72, ws.Range("G11").Value);
            Assert.Equals("AVERAGE(C1:D5)", ws.Range("H11").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("H11").Value);
            Assert.Equals("AVERAGE(5,D8:F11,C1:D1,#REF!)", ws.Range("D12").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D12").Value);
            Assert.Equals("SUM(#REF!)", ws.Range("E12").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E12").Value);
            Assert.Equals("SUM(#REF!)", ws.Range("F12").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F12").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestDoubleRangeReferenceCut()
        {
            Workbook wb = editor.CreateWorkbook("TestWrongRangeReferenceCut");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D1").Formula = "COUNT(B1:C10)";
            ws.Range("D2").Formula = "COUNT(B1:C10)";

            ws.Range("8:13").DeleteRows();

            Assert.Equals("COUNT(B1:C7)", ws.Range("D1").Formula);
            Assert.Equals(14, ws.Range("D1").Value);
            Assert.Equals("COUNT(B1:C7)", ws.Range("D2").Formula);
            Assert.Equals(14, ws.Range("D2").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        [Test]
        public void TestRangeMiddleCutNotMoved()
        {
            Workbook wb = editor.CreateWorkbook("TestWrongRangeReferenceCut");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D1").Formula = "COUNT(B1:C10)";

            ws.Range("2:9").DeleteRows();

            Assert.Equals("COUNT(B1:C2)", ws.Range("D1").Formula);
            Assert.Equals(4, ws.Range("D1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestWholeColumnWrongCut_Rows()
        {
            Workbook wb = editor.CreateWorkbook("TestWholeLineWrongCut_Columns");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D1").Formula = "COUNT(B:C)";

            ws.Range("2:9").DeleteRows();

            Assert.Equals("COUNT(B:C)", ws.Range("D1").Formula);
            Assert.Equals(4, ws.Range("D1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        #endregion

        #region Insert rows tests

        [Test]
        public void TestSimpleReferencesMovingOnRowInsert()
        {
            Workbook wb = editor.CreateWorkbook("TestSimpleReferencesMovingOnRowInsert");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D5").Formula = "COUNT(B5:C10)";
            ws.Range("D6").Formula = "COUNT($B$6:$C$10)";
            ws.Range("D1").Formula = "COUNT(B6:$C$10)";
            listener.Reset();

            ws.Range("5:500").InsertRows(4);

            Assert.Equals("COUNT(B9:C14)", ws.Range("D9").Formula);
            Assert.Equals(12, ws.Range("D9").Value);
            Assert.Equals("COUNT($B$10:$C$14)", ws.Range("D10").Formula);
            Assert.Equals(10, ws.Range("D10").Value);
            Assert.Equals("COUNT(B10:$C$14)", ws.Range("D1").Formula);
            Assert.Equals(10, ws.Range("D1").Value);

            Assert.Equals(25, listener.UpdatedCellsList.Count);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestAdjacentAndIntersectedReferencesMovedOnRowInsert()
        {
            Workbook wb = editor.CreateWorkbook("TestAdjacentReferencesMovedOnRowInsert");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:C10").Value = 5;
            ws.Range("D5").Formula = "SUM(B2:C4)+B$5";
            ws.Range("D6").Formula = "SUM($B$5:$C$10)";
            ws.Range("D4").Formula = "SUM(B6:$C$10)";
            ws.Range("D3").Formula = "SUM(B4:$C5)-B1";
            ws.Range("D7").Formula = "SUM(B$1:C$80)";
            ws.Range("D8").Formula = "SUM(B:C)";
            listener.Reset();

            ws.Range("5:5").InsertRows(4);

            Assert.Equals("SUM(B2:C4)+B$9", ws.Range("D9").Formula);
            Assert.Equals(35, ws.Range("D9").Value);
            Assert.Equals("SUM($B$9:$C$14)", ws.Range("D10").Formula);
            Assert.Equals(60, ws.Range("D10").Value);
            Assert.Equals("SUM(B10:$C$14)", ws.Range("D4").Formula);
            Assert.Equals(50, ws.Range("D4").Value);
            Assert.Equals("SUM(B4:$C9)-B1", ws.Range("D3").Formula);
            Assert.Equals(15, ws.Range("D3").Value);
            Assert.Equals("SUM(B$1:C$84)", ws.Range("D11").Formula);
            Assert.Equals(100, ws.Range("D11").Value);
            Assert.Equals("SUM(B:C)", ws.Range("D12").Formula);
            Assert.Equals(100, ws.Range("D12").Value);

            Assert.Equals(30, listener.UpdatedCellsList.Count);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        #endregion

        #region Remove columns tests
  
        [Test]
        public void TestRemovedCellsReferencesUpdate_Columns()
        {
            Workbook wb = editor.CreateWorkbook("TestRemovedCellsReferencesUpdate_Columns");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1:M4").Value = 5;
            ws.Range("A2").Formula = "E1 + F1";
            ws.Range("B2").Formula = "A2 + F1";
            ws.Range("I2").Formula = "F2+J2+1";
            ws.Range("J2").Formula = "B2+G3";
            ws.Range("F2").Formula = "G2+I3";
            ws.Range("K4").Formula = "G4/2";


            Assert.Equals(10, ws.Range("A2").Value);
            Assert.Equals(15, ws.Range("B2").Value);
            Assert.Equals(31, ws.Range("I2").Value);
            Assert.Equals(20, ws.Range("J2").Value);
            Assert.Equals(10, ws.Range("F2").Value);
            Assert.Equals(2.5, ws.Range("K4").Value);

            ws.Range("E:H").DeleteColumns();

            Assert.Equals("#REF!+#REF!", ws.Range("A2").Formula);
            Assert.Equals("A2+#REF!", ws.Range("B2").Formula);
            Assert.Equals("#REF!+F2+1", ws.Range("E2").Formula); // Test that non-existent subjects have been updated too
            Assert.Equals("B2+#REF!", ws.Range("F2").Formula);
            Assert.Equals("#REF!/2", ws.Range("G4").Formula);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G4").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRemoveColumnsComplex()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/DeleteRowsComplex.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("F:J").DeleteColumns();

            Assert.Equals(26, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals("D4+E4", ws.Range("D6").Formula);
            Assert.Equals(3, ws.Range("D6").Value);
            Assert.Equals("E4+#REF!+E16+C16", ws.Range("E6").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);
            Assert.Equals("I15*5", ws.Range("I5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I5").Value);
            Assert.Equals("B16-C16^2+C14", ws.Range("B14").Formula);
            Assert.Equals(-4421, ws.Range("B14").Value);
            Assert.Equals("#REF!-#REF!", ws.Range("F14").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F14").Value);
            Assert.Equals("G11*5", ws.Range("G15").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G15").Value);
            Assert.Equals("G15*5", ws.Range("I15").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I15").Value);
            Assert.Equals("C16+1", ws.Range("B16").Formula);
            Assert.Equals(68, ws.Range("B16").Value);
            Assert.Equals("#REF!/5", ws.Range("F16").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F16").Value);
            Assert.Equals("D16+E16", ws.Range("E18").Formula);
            Assert.Equals(137, ws.Range("E18").Value);
            Assert.Equals("E11-#REF!/5", ws.Range("C10").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C10").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRemovedCellsRedundantUpdate_Columns()
        {
            Workbook wb = editor.CreateWorkbook("TestRemovedCellsRedundantUpdate_Columns");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1").Value = 1;
            ws.Range("B1").Value = 2;
            ws.Range("C1").Value = 3;
            ws.Range("D1").Value = 4;
            ws.Range("E1").Value = 5;

            ws.Range("A2:E2").Formula = "A1+3";

            ws.Range("C:C").DeleteColumns();

            Assert.Equals(1, ws.Range("A1").Value);
            Assert.Equals(2, ws.Range("B1").Value);
            Assert.Equals(4, ws.Range("C1").Value);
            Assert.Equals(5, ws.Range("D1").Value);
            Assert.Equals("", ws.Range("E1").Value);

            Assert.Equals(4, ws.Range("A2").Value);
            Assert.Equals(5, ws.Range("B2").Value);
            Assert.Equals(7, ws.Range("C2").Value);
            Assert.Equals(8, ws.Range("D2").Value);
            Assert.Equals("", ws.Range("E2").Value);

            Assert.Equals("A1+3", ws.Range("A2").Formula);
            Assert.Equals("B1+3", ws.Range("B2").Formula);
            Assert.Equals("C1+3", ws.Range("C2").Formula);
            Assert.Equals("D1+3", ws.Range("D2").Formula);
            Assert.Equals("", ws.Range("E2").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRangesReferencesOverDeletedColumns()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/RangesReferencesOverDeletedRows.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);


            // The action
            ws.Range("D:E").DeleteColumns();

            wb.Save("../../TestFiles/RangesReferencesOverDeletedColumns_editor_result.xlsx");

            // The action effect
            Assert.Equals("SUM(D14:E14)", ws.Range("C1").Formula);
            Assert.Equals(2, ws.Range("C1").Value);
            Assert.Equals("MAX(C8:C9)", ws.Range("C5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C5").Value);
            Assert.Equals("MAX(D14:E14)", ws.Range("D5").Formula);
            Assert.Equals(2, ws.Range("D5").Value);
            Assert.Equals("SUM(D5:D14)", ws.Range("E5").Formula);
            Assert.Equals(124, ws.Range("E5").Value);
            Assert.Equals("SUM(C:C)", ws.Range("F5").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F5").Value);
            Assert.Equals("B8*C8", ws.Range("B9").Formula);
            Assert.Equals(2, ws.Range("B9").Value);
            Assert.Equals("C8*#REF!", ws.Range("C9").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C9").Value);
            Assert.Equals("D8*E8", ws.Range("D9").Formula);
            Assert.Equals(30, ws.Range("D9").Value);
            Assert.Equals("E8*F8", ws.Range("E9").Formula);
            Assert.Equals(42, ws.Range("E9").Value);
            Assert.Equals("F8*G8", ws.Range("F9").Formula);
            Assert.Equals(56, ws.Range("F9").Value);
            Assert.Equals("G8*H8", ws.Range("G9").Formula);
            Assert.Equals(0, ws.Range("G9").Value);
            Assert.Equals("SUM(B8:C9)", ws.Range("B10").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B10").Value);
            Assert.Equals("SUM(C8:C9)", ws.Range("C10").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C10").Value);
            Assert.Equals("SUM(D8:E9)", ws.Range("D10").Formula);
            Assert.Equals(83, ws.Range("D10").Value);
            Assert.Equals("SUM(E8:F9)", ws.Range("E10").Formula);
            Assert.Equals(111, ws.Range("E10").Value);
            Assert.Equals("SUM(F8:G9)", ws.Range("F10").Formula);
            Assert.Equals(71, ws.Range("F10").Value);
            Assert.Equals("SUM(G8:H9)", ws.Range("G10").Formula);
            Assert.Equals(8, ws.Range("G10").Value);
            Assert.Equals("SUM(C1)", ws.Range("D12").Formula);
            Assert.Equals(2, ws.Range("D12").Value);
            Assert.Equals("SUM(D10:E14)", ws.Range("F12").Formula);
            Assert.Equals(199, ws.Range("F12").Value);
            Assert.Equals("SUM(E5:F5,D15)", ws.Range("G14").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G14").Value);
            Assert.Equals("SUM(#REF!)", ws.Range("C16").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C16").Value);
            Assert.Equals("AVERAGE(C5:C10)", ws.Range("C17").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C17").Value);
            Assert.Equals("SUM(D15:D17)", ws.Range("E17").Formula);
            Assert.Equals(27, ws.Range("E17").Value);
            Assert.Equals("AVERAGE(C1:C5)", ws.Range("F17").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F17").Value);
            Assert.Equals("SUM(C10)", ws.Range("D18").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D18").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestDoubleRangeReferenceCut_Columns()
        {
            Workbook wb = editor.CreateWorkbook("TestDoubleRangeReferenceCut_Columns");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:G10").Value = 5;
            ws.Range("A1").Formula = "COUNT(B1:G10)";
            ws.Range("A2").Formula = "COUNT(B1:G10)";

            ws.Range("E:M").DeleteColumns();

            Assert.Equals("COUNT(B1:D10)", ws.Range("A1").Formula);
            Assert.Equals(30, ws.Range("A1").Value);
            Assert.Equals("COUNT(B1:D10)", ws.Range("A2").Formula);
            Assert.Equals(30, ws.Range("A2").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        [Test]
        public void TestRangeMiddleCutNotMoved2()
        {
            Workbook wb = editor.CreateWorkbook("TestRangeMiddleCutNotMoved2");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:G2").Value = 5;
            ws.Range("B3").Formula = "COUNT(B1:G2)";

            ws.Range("C:F").DeleteColumns();

            Assert.Equals("COUNT(B1:C2)", ws.Range("B3").Formula);
            Assert.Equals(4, ws.Range("B3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestWholeLineWrongCut_Columns()
        {
            Workbook wb = editor.CreateWorkbook("TestWholeLineWrongCut_Columns");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:G2").Value = 5;
            ws.Range("A3").Formula = "COUNT(1:2)";

            ws.Range("C:F").DeleteColumns();

            Assert.Equals("COUNT(1:2)", ws.Range("A3").Formula);
            Assert.Equals(4, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
 
        #endregion

        #region Insert columns tests

        [Test]
        public void TestSimpleReferencesMovingOnColumnInsert()
        {
            Workbook wb = editor.CreateWorkbook("TestSimpleReferencesMovingOnColumnInsert");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:G2").Value = 5;
            ws.Range("D3").Formula = "COUNT(D1:G2)";
            ws.Range("E3").Formula = "COUNT($E$1:$G$2)";
            ws.Range("A3").Formula = "COUNT(E1:$G$2)";
            listener.Reset();

            ws.Range("D1").InsertColumn();

            Assert.Equals("COUNT(E1:H2)", ws.Range("E3").Formula);
            Assert.Equals(8, ws.Range("E3").Value);
            Assert.Equals("COUNT($F$1:$H$2)", ws.Range("F3").Formula);
            Assert.Equals(6, ws.Range("F3").Value);
            Assert.Equals("COUNT(F1:$H$2)", ws.Range("A3").Formula);
            Assert.Equals(6, ws.Range("A3").Value);

            Assert.Equals(14, listener.UpdatedCellsList.Count);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestAdjacentAndIntersectedReferencesMovedOnColumnInsert()
        {
            Workbook wb = editor.CreateWorkbook("TestAdjacentAndIntersectedReferencesMovedOnColumnInsert");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("B1:J7").Value = 5;
            ws.Range("A8").Formula = "SUM(B1:E1)";
            ws.Range("A9").Formula = "SUM(B2:F2)";
            ws.Range("A10").Formula = "SUM(E3:J3)";
            ws.Range("A11").Formula = "SUM(F4:J4)";
            ws.Range("A12").Formula = "SUM(E5:F6)";
            ws.Range("A13").Formula = "SUM(7:7)";
            listener.Reset();

            ws.Range("F1").InsertColumns(2);

            Assert.Equals("SUM(B1:E1)", ws.Range("A8").Formula);
            Assert.Equals(20, ws.Range("A8").Value);
            Assert.Equals("SUM(B2:H2)", ws.Range("A9").Formula);
            Assert.Equals(25, ws.Range("A9").Value);
            Assert.Equals("SUM(E3:L3)", ws.Range("A10").Formula);
            Assert.Equals(30, ws.Range("A10").Value);
            Assert.Equals("SUM(H4:L4)", ws.Range("A11").Formula);
            Assert.Equals(25, ws.Range("A11").Value);
            Assert.Equals("SUM(E5:H6)", ws.Range("A12").Formula);
            Assert.Equals(20, ws.Range("A12").Value);
            Assert.Equals("SUM(7:7)", ws.Range("A13").Formula);
            Assert.Equals(45, ws.Range("A13").Value);

            Assert.Equals(54, listener.UpdatedCellsList.Count);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        #endregion

        private EWSEditor editor;
    }
}
