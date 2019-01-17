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
using AVSOfficeEWSEditor.Editor.Sorting;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Cycles")]
    public class CyclesDetectionTest
    {
        public CyclesDetectionTest()
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
        public void Test3NodesCycleAtFormulaSet()
        {
            wb = editor.CreateWorkbook("3NodesCycle");
            Assert.NotNull(wb);
            ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A2").Formula = "A1 + 1";
            ws.Range("B2").Formula = "A2 + 1";
            ws.Range("A1").Formula = "B2 + 1"; // Cycle

            Assert.Equals(ErrorValue.bad_reference, ws.Range("A1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B2").Formula = "2 + 1"; // Break the cycle

            Assert.Equals(4, ws.Range("A1").Value);
            Assert.Equals(3, ws.Range("B2").Value);
            Assert.Equals(5, ws.Range("A2").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestTheGreatBear()
        {
            wb = editor.CreateWorkbook("TheGreatBear");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B9").Formula = "1";
            ws.Range("D5").Formula = "B9 + 1";
            ws.Range("F6").Formula = "D5 + 1";
            ws.Range("H7").Formula = "F6 + 1 + K3";
            ws.Range("I12").Formula = "H7 + 1";
            ws.Range("K10").Formula = "I12 + 1";
            listener.Reset();
            ws.Range("K3").Formula = "K10 + 1"; // Cycle
            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("H7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I12").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K10").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K3").Value);

            ws.Range("B9").Formula = "2"; // Pull the tail of The GreatBear
            Assert.Equals(7, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(2, ws.Range("B9").Value);
            Assert.Equals(3, ws.Range("D5").Value);
            Assert.Equals(4, ws.Range("F6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("H7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I12").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K10").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K3").Value);

            ws.Range("I12").Formula = "H7 + 1 + 8"; // Change formula but leave the cycle
            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("H7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("I12").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K10").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("K3").Value);

            ws.Range("K10").Value = 9; // Break the cycle
            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(15, ws.Range("H7").Value);
            Assert.Equals(24, ws.Range("I12").Value);
            Assert.Equals(9, ws.Range("K10").Value);
            Assert.Equals(10, ws.Range("K3").Value);

            ws.Range("I12").Formula = "H7+1+K10"; // Append a formula after the loop has been broken

            Assert.Equals(15, ws.Range("H7").Value);
            Assert.Equals(25, ws.Range("I12").Value);
            Assert.Equals(9, ws.Range("K10").Value);
            Assert.Equals(10, ws.Range("K3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestBowCycle()
        {
            wb = editor.CreateWorkbook("BowCycle");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B4").Formula = "C2 + 1";
            ws.Range("E5").Formula = "D2 + 1";
            ws.Range("D7").Formula = "B4+E5";
            listener.Reset();
            ws.Range("C2:D2").Formula = "$D$7 + 1"; // 2 cycles

            Assert.Equals(5, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C2:D2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E5").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D7").Value);

            ws.Range("D7").Value = 4; // Break the cycle

            Assert.Equals(5, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(5, ws.Range("C2:D2").Value);
            Assert.Equals(6, ws.Range("B4").Value);
            Assert.Equals(6, ws.Range("E5").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestUseOfCycledValues()
        {
            wb = editor.CreateWorkbook("UseOfCycledValues");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("F4").Value = 1;
            ws.Range("E6").Formula = "F4+D9";
            ws.Range("D4").Formula = "F4+E6+C7";
            ws.Range("C7").Formula = "E6+1";

            Assert.Equals(1, ws.Range("E6").Value);
            Assert.Equals(4, ws.Range("D4").Value);
            Assert.Equals(2, ws.Range("C7").Value);

            listener.Reset();
            ws.Range("D9").Formula = "C7+2"; // Cycle

            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("D4").Value); // This uses the cycled values
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D9").Value);

            ws.Range("F4").Value = 4; // Pull the tail

            Assert.Equals(5, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("D4").Value); // This uses the cycled values
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C7").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D9").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void Test2NodesCycle()
        {
            wb = editor.CreateWorkbook("2NodesCycle");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("E6").Formula = "F4+D9";
            listener.Reset();
            ws.Range("F4").Formula = "4+E6+C7";

            Assert.Equals(2, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("F4").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);

            ws.Range("E6").Formula = "4+C7+D9"; // Break the cycle

            Assert.Equals(2, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(4, ws.Range("E6").Value);
            Assert.Equals(8, ws.Range("F4").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void Test1NodeCycle()
        {
            wb = editor.CreateWorkbook("1NodeCycle");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("E6").Formula = "$E$6";

            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(ErrorValue.bad_reference, ws.Range("E6").Value);

            ws.Range("E6").Formula = "4+C7+D9"; // Break the cycle

            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(4, ws.Range("E6").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestLoadingDocumentsWithLoop()
        {
            wb = editor.LoadWorkbook("../../TestFiles/Loop.xlsx");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            // The values are as saved in the file
            Assert.Equals(0, ws.Range("A1").Value);
            Assert.Equals(0, ws.Range("B2").Value);
            Assert.Equals(0, ws.Range("C2").Value);
            Assert.Equals(0, ws.Range("B3").Value);
            Assert.Equals(0, ws.Range("B1").Value); // Uses result of the loop

            ws.Range("C1").Value = 1; // Make the loop formulas to be recalculated

            Assert.Equals(ErrorValue.bad_reference, ws.Range("A1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B1").Value); // Uses result of the loop

            Assert.Equals(2, ws.Range("C3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestRangesCycle()
        {
            wb = editor.CreateWorkbook("RangesCycle");
            Assert.NotNull(wb);
            ws = wb.ActiveWorksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            // Cycle 1
            ws.Range("A1:D5").Value = 10;
            ws.Range("D6").Formula = "SUM(A1:D5)";
            Assert.Equals(200, ws.Range("D6").Value);
            ws.Range("E3").Formula = "MAX(C1:D6)";
            Assert.Equals(200, ws.Range("E3").Value);
            ws.Range("B3").Formula = "COUNT(D1:E3)";
            // The following 3 values could be as correct values as bad references. It depends on internal recalculation sequence and is OK.
            Assert.Equals(3, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E3").Value);
            Assert.Equals(193, ws.Range("D6").Value);

            // Cycle 2
            ws.Range("A10").Value = 10;
            ws.Range("A11").Formula = "SUM(A10:B10)";
            Assert.Equals(10, ws.Range("A11").Value);
            ws.Range("B11").Formula = "SUM(A10:A11)";
            Assert.Equals(20, ws.Range("B11").Value);
            ws.Range("B10").Formula = "SUM(A11:B11)";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B10").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A11").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B11").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }



        private EWSEditor editor;
        private Workbook wb;
        private Worksheet ws;
    }
}
