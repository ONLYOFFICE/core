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
using AVSOfficeEWSEditor.Editor.Format.Borders;
using AVSOfficeEWSEditor.Editor.Sorting;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Sorting")]
    public class SortingTest
    {
        public SortingTest()
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
            wb.Close();
            ws = null;
            wb = null;
            editor = null;
        }

        [Test]
        public void TestSimpleSort()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            ws.GetUpdatedCells();
            listener.Reset();

            wb.Save("../../TestFiles/Simple_sort_before.xlsx");

            ws.Range("C4:F14").Sort(SortOrientation.by_rows, ws.Range("D4"), SortOrder.ascending);

            Assert.Equals(44, listener.UpdatedCellsList.Count);

            wb.Save("../../TestFiles/Simple_sort_after.xlsx");

            Assert.Equals(456.123, ws.Range("D5").Value);
            Assert.Equals(456.124, ws.Range("F5").Value);
            Assert.Equals(20, ws.Range("C8").Value);
            Assert.Equals("wtf", ws.Range("D8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F11").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C11").Value);
            Assert.Equals("", ws.Range("F9").Value);
            Assert.Equals("", ws.Range("E9").Value);
            Assert.Equals("", ws.Range("E6").Value);
            Assert.Equals("", ws.Range("E11").Value);
            Assert.Equals("", ws.Range("E12").Value);
            Assert.Equals("", ws.Range("D12").Value);
            Assert.Equals("", ws.Range("D13").Value);
            Assert.Equals("", ws.Range("C12").Value);
            Assert.Equals("яяя", ws.Range("F12").Value);
            Assert.Equals(true, ws.Range("E14").Value);
            Assert.Equals("wtf", ws.Range("C14").Value);
            Assert.Equals("storage", ws.Range("C9").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestSimpleSortDescending()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C4:F14").Sort(SortOrientation.by_rows, ws.Range("D4"), SortOrder.descending);

            wb.Save("../../TestFiles/Simple_sort_desc_after.xlsx");

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D4").Value);
            Assert.Equals("", ws.Range("E4").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F4").Value);

            Assert.Equals(5, ws.Range("C5").Value);
            Assert.Equals(true, ws.Range("D5").Value);
            Assert.Equals(true, ws.Range("E5").Value);
            Assert.Equals(5, ws.Range("F5").Value);

            Assert.Equals("storage", ws.Range("C6").Value);
            Assert.Equals(false, ws.Range("D6").Value);
            Assert.Equals("", ws.Range("F6").Value);
            Assert.Equals("", ws.Range("E6").Value);

            Assert.Equals(20, ws.Range("C7").Value);
            Assert.Equals("wtf", ws.Range("D7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F7").Value);

            Assert.Equals(5, ws.Range("C8").Value);
            Assert.Equals(500, ws.Range("D8").Value);
            Assert.Equals(true, ws.Range("E8").Value);
            Assert.Equals(5, ws.Range("F8").Value);

            Assert.Equals(5, ws.Range("C9").Value);
            Assert.Equals(500, ws.Range("D9").Value);
            Assert.Equals("", ws.Range("E9").Value);
            Assert.Equals(5, ws.Range("F9").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C10").Value);
            Assert.Equals(456.123, ws.Range("D10").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E10").Value);
            Assert.Equals(456.124, ws.Range("F10").Value);

            Assert.Equals(20, ws.Range("C11").Value);
            Assert.Equals(4, ws.Range("D11").Value);
            Assert.Equals(4, ws.Range("E11").Value);
            Assert.Equals(4, ws.Range("F11").Value);

            Assert.Equals("", ws.Range("C12").Value);
            Assert.Equals("", ws.Range("D12").Value);
            Assert.Equals("", ws.Range("E12").Value);
            Assert.Equals("яяя", ws.Range("F12").Value);

            Assert.Equals("", ws.Range("C13").Value);
            Assert.Equals("", ws.Range("D13").Value);
            Assert.Equals("", ws.Range("E13").Value);
            Assert.Equals("", ws.Range("F13").Value);

            Assert.Equals("wtf", ws.Range("C14").Value);
            Assert.Equals("", ws.Range("D14").Value);
            Assert.Equals(true, ws.Range("E14").Value);
            Assert.Equals(5, ws.Range("F14").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestSortByColumns_D4()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C4:F14").Sort(SortOrientation.by_columns, ws.Range("D4"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_by_columns_after.xlsx");

            Assert.Equals(5, ws.Range("C4").Value);
            Assert.Equals(5, ws.Range("D4").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            Assert.Equals(5, ws.Range("C5").Value);
            Assert.Equals("wtf", ws.Range("D5").Value);
            Assert.Equals("", ws.Range("E5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C6").Value);
            Assert.Equals(20, ws.Range("D6").Value);
            Assert.Equals("wtf", ws.Range("E6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F6").Value);

            Assert.Equals(4, ws.Range("C7").Value);
            Assert.Equals(20, ws.Range("D7").Value);
            Assert.Equals(4, ws.Range("E7").Value);
            Assert.Equals(4, ws.Range("F7").Value);

            Assert.Equals(456.124, ws.Range("C8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D8").Value);
            Assert.Equals(456.123, ws.Range("E8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F8").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C9").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D9").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E9").Value);
            Assert.Equals("", ws.Range("F9").Value);

            Assert.Equals("яяя", ws.Range("C10").Value);
            Assert.Equals("", ws.Range("D10").Value);
            Assert.Equals("", ws.Range("E10").Value);
            Assert.Equals("", ws.Range("F10").Value);

            Assert.Equals(5, ws.Range("C11").Value);
            Assert.Equals(5, ws.Range("D11").Value);
            Assert.Equals(500, ws.Range("E11").Value);
            Assert.Equals("", ws.Range("F11").Value);

            Assert.Equals(5, ws.Range("C12").Value);
            Assert.Equals(5, ws.Range("D12").Value);
            Assert.Equals(500, ws.Range("E12").Value);
            Assert.Equals(true, ws.Range("F12").Value);

            Assert.Equals("", ws.Range("C13").Value);
            Assert.Equals("", ws.Range("D13").Value);
            Assert.Equals("", ws.Range("E13").Value);
            Assert.Equals("", ws.Range("F13").Value);

            Assert.Equals("", ws.Range("C14").Value);
            Assert.Equals("storage", ws.Range("D14").Value);
            Assert.Equals(false, ws.Range("E14").Value);
            Assert.Equals("", ws.Range("F14").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestSortByColumns_C14()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C4:F14").Sort(SortOrientation.by_columns, ws.Range("C14"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_by_columns_after_C14.xlsx");

            Assert.Equals(5, ws.Range("E4").Value);
            Assert.Equals(5, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            Assert.Equals(5, ws.Range("E5").Value);
            Assert.Equals("wtf", ws.Range("C5").Value);
            Assert.Equals("", ws.Range("D5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);
            Assert.Equals(20, ws.Range("C6").Value);
            Assert.Equals("wtf", ws.Range("D6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F6").Value);

            Assert.Equals(4, ws.Range("E7").Value);
            Assert.Equals(20, ws.Range("C7").Value);
            Assert.Equals(4, ws.Range("D7").Value);
            Assert.Equals(4, ws.Range("F7").Value);

            Assert.Equals(456.124, ws.Range("E8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C8").Value);
            Assert.Equals(456.123, ws.Range("D8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F8").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("E9").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C9").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D9").Value);
            Assert.Equals("", ws.Range("F9").Value);

            Assert.Equals("яяя", ws.Range("E10").Value);
            Assert.Equals("", ws.Range("C10").Value);
            Assert.Equals("", ws.Range("D10").Value);
            Assert.Equals("", ws.Range("F10").Value);

            Assert.Equals(5, ws.Range("E11").Value);
            Assert.Equals(5, ws.Range("C11").Value);
            Assert.Equals(500, ws.Range("D11").Value);
            Assert.Equals("", ws.Range("F11").Value);

            Assert.Equals(5, ws.Range("E12").Value);
            Assert.Equals(5, ws.Range("C12").Value);
            Assert.Equals(500, ws.Range("D12").Value);
            Assert.Equals(true, ws.Range("F12").Value);

            Assert.Equals("", ws.Range("E13").Value);
            Assert.Equals("", ws.Range("C13").Value);
            Assert.Equals("", ws.Range("D13").Value);
            Assert.Equals("", ws.Range("F13").Value);

            Assert.Equals("", ws.Range("E14").Value);
            Assert.Equals("storage", ws.Range("C14").Value);
            Assert.Equals(false, ws.Range("D14").Value);
            Assert.Equals("", ws.Range("F14").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestSortByColumnsDesc()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A5:K6").Sort(SortOrientation.by_columns, ws.Range("C6"), SortOrder.descending);

            wb.Save("../../TestFiles/Sort_by_columns_desc_after.xlsx");

            Assert.Equals(true, ws.Range("A5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A6").Value);

            Assert.Equals(5, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B6").Value);

            Assert.Equals("", ws.Range("C5").Value);
            Assert.Equals("wtf", ws.Range("C6").Value);

            Assert.Equals("wtf", ws.Range("D5:D6").Value);

            Assert.Equals("55", ws.Range("E5:E6").Value);

            Assert.Equals("wtf", ws.Range("F5").Value);
            Assert.Equals(20, ws.Range("F6").Value);

            Assert.Equals(5, ws.Range("G5:H6").Value);

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("I5").Value);
            Assert.Equals(-58.5, ws.Range("I6").Value);

            Assert.Equals("", ws.Range("J5:K6").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestSortSimpleFormulas()
        {
            PrepareSimpleFormulas();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            wb.Save("../../TestFiles/Sort_formulas_before.xlsx");

            ws.Range("C3:G7").Sort(SortOrientation.by_rows, ws.Range("E3"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_formulas_after.xlsx");

            Assert.Equals(3, ws.Range("G3").Value);
            Assert.Equals(2, ws.Range("G4").Value);
            Assert.Equals(4, ws.Range("G5").Value);
            Assert.Equals(5, ws.Range("G6").Value);
            Assert.Equals(1, ws.Range("G7").Value);

            Assert.Equals(0, ws.Range("E3").Value);
            Assert.Equals(62.7, ws.Range("D4").Value);
            Assert.Equals(2.53591e76, (double)ws.Range("F5").Value, 0.00001e76);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C7").Value);

            
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        [Test]
        public void TestSortLoopFormulas2Rows()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Formula = "$B$1 + 1";
            ws.Range("B1").Value = 2;
            ws.Range("A2").Value = 1;
            ws.Range("B2").Formula = "$A$2 + 1";

            Assert.Equals(3, ws.Range("A1").Value);
            Assert.Equals(2, ws.Range("B2").Value);

            wb.Save("../../TestFiles/Sort_formulas_simple_loop_before.xlsx");

            ws.Range("A1:B2").Sort(SortOrientation.by_rows, ws.Range("A1"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_formulas_simple_loop_after.xlsx");

            Assert.Equals(1, ws.Range("A1").Value);
            Assert.Equals(2, ws.Range("B2").Value);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B1").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestSortLoopFormulasComplex()
        {
            PrepareFormulasMakingLoopAfterSorting();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            wb.Save("../../TestFiles/Sort_formulas_loop_before.xlsx");

            ws.Range("C3:G7").Sort(SortOrientation.by_rows, ws.Range("D3"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_formulas_loop_after.xlsx");

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C3").Value); // Main loop
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D7").Value); // Main loop
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G7").Value); // Use cyclic error results
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C4").Value); // References itself
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C7").Value); // Use main loop result
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D6").Value); // Use cyclic error results
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E4").Value); // References itself
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F7").Value); // Use main loop result

            Assert.Equals(1, ws.Range("D3").Value); // Depends on the cell that is moved from an empty cell

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        [Test]
        public void TestSortLoopFormulasComplexByCol()
        {
            PrepareFormulasMakingLoopAfterSorting();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C3:G7").Sort(SortOrientation.by_columns, ws.Range("C4"), SortOrder.ascending);

            wb.Save("../../TestFiles/Sort_formulas_loop_bycol_after.xlsx");

            Assert.Equals(ErrorValue.bad_reference, ws.Range("D5").Value); // References itself
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D5").Value); // Use cyclic error result
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C4").Value); // Use cyclic error result
            Assert.Equals(ErrorValue.bad_reference, ws.Range("G3").Value); // Use cyclic error result
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E7").Value); // Use cyclic error result

            Assert.Equals("", ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(3.3, ws.Range("E3").Value);
            Assert.Equals("", ws.Range("F3").Value);
            Assert.Equals(5.5, ws.Range("D4").Value);
            Assert.Equals(0, ws.Range("E4").Value);
            Assert.Equals(-61.6, ws.Range("F4").Value);
            Assert.Equals(1, ws.Range("G4").Value);
            Assert.Equals("", ws.Range("C5").Value);
            Assert.Equals("", ws.Range("F5").Value);
            Assert.Equals(5.5, ws.Range("G5").Value);
            Assert.Equals("", ws.Range("C6").Value);
            Assert.Equals(true, ws.Range("D6").Value);
            Assert.Equals("$D$7^8-56*$A$1", ws.Range("E6").Value);
            Assert.Equals("", ws.Range("F6").Value);
            Assert.Equals(8, ws.Range("G6").Value);
            Assert.Equals("", ws.Range("C7:D7").Value);
            Assert.Equals(-300.1, ws.Range("F7").Value);
            Assert.Equals(4.3, ws.Range("G7").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestUpdatingValuesOutOfRange()
        {
            PrepareSimpleFormulas();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B5").Formula = "C5+E5";
            ws.Range("B6").Formula = "D6+E6";
            ws.Range("D2").Formula = "D3-F3-G5--F7";
            ws.Range("E2").Formula = "G3+D4+C5+F7+F5+G6";
            ws.Range("F2").Formula = "E4+E5+E6-F6-D3-C4-C3";


            ws.Range("C3:G7").Sort(SortOrientation.by_rows, ws.Range("E3"), SortOrder.ascending);

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B5").Value);
            Assert.Equals(82, ws.Range("B6").Value);
            Assert.Equals(81, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E2").Value);
            Assert.Equals(-30.2, ws.Range("F2").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestFormatCopiedToo()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Formula = "B1+C1";
            ws.Range("A2").Formula = "B2-C2";
            ws.Range("B1:B2").Value = 500;
            ws.Range("C1:C2").Value = 0.00005;
            ws.Range("A1").Format.Font.Bold = true;

            ws.Range("A1:A2").Sort(SortOrientation.by_rows, ws.Range("A1:A2"), SortOrder.ascending);

            Assert.Equals(499.99995, ws.Range("A1").Value);
            Assert.Equals(500.00005, ws.Range("A2").Value);
            Assert.Equals(false, ws.Range("A1").Format.Font.Bold);
            Assert.Equals(true, ws.Range("A2").Format.Font.Bold);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }


        [Test]
        public void TestFullColumnsSort()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 2;
            ws.Range("B1").Value = 500;
            ws.Range("B1").Value = 200;
            ws.Range("C2:C3").Value = 0.00005;

            ws.Range("A:B").Sort(SortOrientation.by_rows, ws.Range("A1"), SortOrder.ascending);

            Assert.Equals(2, ws.Range("A1").Value);
            Assert.Equals(5, ws.Range("A2").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }
        
        private void PrepareSimpleData()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;

            ws.Range("A1:H12").Value = 5;
            ws.Range("D1:E12").Value = true;
            ws.Range("A4:A8").Formula = "\"55\"";
            ws.Range("C6:F9").Value = ErrorValue.bad_reference;
            ws.Range("D7:H7").Value = 4;
            ws.Range("D11:D12").Value = 500;
            ws.Range("B5:D6").Value = "wtf";
            ws.Range("A2:D2").Value = 0.5;
            ws.Range("B10:E10").Value = null;
            ws.Range("E9:E11").Value = null;
            ws.Range("E3:E4").Value = "TRUE";
            ws.Range("D8").Value = 456.123;
            ws.Range("F8").Value = 456.124;
            ws.Range("F8").Value = 456.124;
            ws.Range("C6:C7").Value = 20;
            ws.Range("F10").Value = "яяя";
            ws.Range("D5").Value = null;
            ws.Range("C14").Value = "storage";
            ws.Range("D14").Value = false;
            ws.Range("A:F").ColumnWidth = 10.625;
            ws.Range("C4:C12").Format.Borders.Left.Style = BorderStyle.thin; // The reason of the holes is the necessity to check sorting of non-existent cells and rows.
            ws.Range("F4:F12").Format.Borders.Right.Style = BorderStyle.thin;
            ws.Range("C14").Format.Borders.Left.Style = BorderStyle.thin;
            ws.Range("C4:F4").Format.Borders.Top.Style = BorderStyle.thin;
            ws.Range("C14:E14").Format.Borders.Bottom.Style = BorderStyle.thin;
            ws.Range("K5").Value = ErrorValue.division_by_zero;
            ws.Range("K6").Value = -58.5;
            ws.Range("K5:K6").Format.Borders.Right.Style = BorderStyle.dotted;
            ws.Range("K5").Format.Borders.Top.Style = BorderStyle.dotted;
            ws.Range("K6").Format.Borders.Bottom.Style = BorderStyle.dotted;
            ws.Range("A5:H5").Format.Borders.Top.Style = BorderStyle.dotted;
            ws.Range("A6:H6").Format.Borders.Bottom.Style = BorderStyle.dotted;
            ws.Range("A5:A6").Format.Borders.Left.Style = BorderStyle.dotted;
        }

        private void PrepareSimpleFormulas()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;

            ws.Range("A1").Value = 1.1;
            ws.Range("B2").Value = 2.2;
            ws.Range("C2").Value = 4.4;
            ws.Range("B3").Value = 5.5;
            ws.Range("C3").Formula = "C2*2";
            ws.Range("D4").Formula = "C3+C4";
            ws.Range("C4").Formula = "B3+B2";
            ws.Range("E5").Formula = "-D4+C3*2";
            ws.Range("F6").Formula = "E5^D4";
            ws.Range("D3").Value = 12;
            ws.Range("E3").Formula = "5^2.5";
            ws.Range("F3").Value = 34;
            ws.Range("G3").Value = 1;
            ws.Range("E4").Value = 24.5;
            ws.Range("F4:G4").Value = 2;
            ws.Range("C5").Value = "55";
            ws.Range("D5").Value = 55;
            ws.Range("F5").Value = 4;
            ws.Range("G5").Value = 3;
            ws.Range("C6").Value = "wtf2";
            ws.Range("D6").Value = 56;
            ws.Range("E6:E7").Value = 34;
            ws.Range("G6").Value = 4;
            ws.Range("C7").Value = "wtf";
            ws.Range("D7").Value = 48;
            ws.Range("F7:G7").Value = 5;

            ws.Range("C3:C7").Format.Borders.Left.Style = BorderStyle.thin;
            ws.Range("G3:G7").Format.Borders.Right.Style = BorderStyle.thin;
            ws.Range("C3:G3").Format.Borders.Top.Style = BorderStyle.thin;
            ws.Range("C7:G7").Format.Borders.Bottom.Style = BorderStyle.thin;
        }

        private void PrepareFormulasMakingLoopAfterSorting()
        {
            wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;

            ws.Range("A1").Value = 1.1;
            ws.Range("B2").Value = 2.2;
            ws.Range("C3").Value = 3.3;
            ws.Range("D4").Formula = "-4.4^$C$3";
            ws.Range("E4").Formula = "5.5";
            ws.Range("D5").Formula = "5.5";
            ws.Range("E5").Formula = "$D$5^$E$4*6%";
            ws.Range("C4").Formula = "$C$3+$C$3";
            ws.Range("C5").Formula = "($C$4)*($D$4)";
            ws.Range("D3").Formula = "($C$4)*($C$3)";
            ws.Range("E3").Formula = "TRUE";
            ws.Range("D7").Formula = "$E$3+1";
            ws.Range("G4").Formula = "$D$7-$D$3-$D$5+$C$4";
            ws.Range("F4").Formula = "$D$7^8-56*$A$1";
            ws.Range("C6").Value = "$D$7^8-56*$A$1";
            ws.Range("D6").Value = "8";
            ws.Range("E6").Value = true;
            ws.Range("C7").Formula = "$D$7+$D$5";
            ws.Range("F7").Formula = "-300.1";

        }

        private EWSEditor editor;
        private Workbook wb;
        private Worksheet ws;
    }
}
