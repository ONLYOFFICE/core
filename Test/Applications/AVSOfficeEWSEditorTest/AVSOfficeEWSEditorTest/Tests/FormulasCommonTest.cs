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
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using csUnit;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Formulas, FormulasCommon")]
    public class FormulasCommonTest
    {
        public FormulasCommonTest()
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
        public void TestFormulaGet()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Fibonacci_chain.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            Assert.Equals("W23+X24", ws.Range("Y25").Formula);
            Assert.Null(ws.Range("Y25:Z26").Formula);
            Assert.Equals("1", ws.Range("B2").Formula);
            Assert.Equals("", ws.Range("A1").Formula);
            wb.Close();
        }

        [Test]
        public void TestSimpleChainRecalculation()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Fibonacci_chain.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            Assert.Equals(46368, ws.Range("Y25").Value);
            ws.Range("A1").Value = true;
            wb.Save("../../TestFiles/Fibonacci_chain_changed.xlsx");
            Assert.Equals(46368, ws.Range("X24").Value);
            Assert.Equals(75025, ws.Range("Y25").Value);
            wb.Close();
        }

        [Test]
        public void TestFormulaSet()
        {
            System.Console.WriteLine("TestFormulaSet()");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Fibonacci_chain.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            System.Console.WriteLine("ws.Range(\"O15:P16\").Formula = \"5000  +  $N14\"");
            ws.Range("O15:P16").Formula = "5000  +  $N14";
            Assert.Equals("5000+$N14", ws.Range("O15").Formula);
            Assert.Equals(5233, ws.Range("O15").Value);
            Assert.Equals("5000+$N14", ws.Range("P15").Formula);
            Assert.Equals(5233, ws.Range("P15").Value);

            Assert.Equals("5000+$N15", ws.Range("O16").Formula);
            Assert.Equals(5000, ws.Range("O16").Value);

            Assert.Equals("5000+$N15", ws.Range("P16").Formula);
            Assert.Equals(5000, ws.Range("O16").Value);

            Assert.Equals(279893, ws.Range("X24").Value);
            Assert.Equals(732815, ws.Range("Z26").Value);
            wb.Close();
        }

        [Test]
        public void TestLateUnregisterProblem()
        {
            System.Console.WriteLine("TestLateUnregisterProblem()");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Fibonacci_chain.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            System.Console.WriteLine("Make a loop around O15:");
            ws.Range("P16").Formula = "N14 + N14";
            ws.Range("Q17").Formula = "N14 + P16";
            Assert.Equals(12815, ws.Range("W23").Value);
            System.Console.WriteLine("Change in O15 must not lead in Recalculation");
            ws.Range("O15").Value = 5000;
            Assert.Equals(12815, ws.Range("W23").Value);
            Assert.Equals(466, ws.Range("P16").Value);
            // Try shorter chain

            System.Console.WriteLine("Make a loop around Y25:");
            ws.Range("Z26").Formula = "X24+X24";
            Assert.Equals(41474, ws.Range("Z26").Value);
            System.Console.WriteLine("Change in Y25 must not lead in Recalculation");
            ws.Range("Y25").Value = 5000;
            Assert.Equals(41474, ws.Range("Z26").Value);

            wb.Close();
        }

        [Test]
        public void TestComplexDependencies()
        {
            System.Console.WriteLine("TestComplexDependencies()");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Formula_complex_dependencies.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            Assert.Equals("B4+C3+C2", ws.Range("B2").Formula);
            Assert.Equals("AB2+AD2", ws.Range("AC4").Formula);

            System.Console.WriteLine("A1 = 2");
            ws.Range("A1").Formula = "2";
            Assert.Equals(14, ws.Range("B1").Value);

            System.Console.WriteLine("B4 = 3");
            ws.Range("B4").Value = 3;
            Assert.Equals(20, ws.Range("B1").Value);
            Assert.Equals(12, ws.Range("B2").Value);
            Assert.Equals(9, ws.Range("D1").Value);

            System.Console.WriteLine("AC1 = 4");
            ws.Range("AC1").Value = 4;
            Assert.Equals(20, ws.Range("AA4").Value);
            Assert.Equals(12, ws.Range("AC4").Value);
            Assert.Equals(16, ws.Range("AE4").Value);

            System.Console.WriteLine("B7 = 3");
            ws.Range("B7").Value = 3;
            Assert.Equals(12, ws.Range("B8").Value);
            Assert.Equals(9, ws.Range("E7").Value);
            Assert.Equals(13, ws.Range("A7").Value);

            System.Console.WriteLine("C8 = 4");
            ws.Range("C8").Value = 4;
            Assert.Equals(11, ws.Range("C7").Value);
            Assert.Equals(7, ws.Range("D7").Value);
            Assert.Equals(14, ws.Range("B8").Value);

            System.Console.WriteLine("AC7 = 3");
            ws.Range("AC7").Value = 3;
            Assert.Equals(12, ws.Range("AA9").Value);
            Assert.Equals(6, ws.Range("AB9").Value);
            Assert.Equals(3, ws.Range("AC9").Value);
            Assert.Equals(3, ws.Range("AD9").Value);
            Assert.Equals(6, ws.Range("AE9").Value);

            wb.Close();
        }

        [Test]
        public void TestRedundantRecalculationsBug()
        {
            System.Console.WriteLine("TestRedundantRecalculationsBug()");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Formula_complex_dependencies.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            System.Console.WriteLine("TestRedundantRecalculationsBug step1 started...");
            DateTime before1 = DateTime.Now;

            ws.Range("A13").Value = 2;

            DateTime after1 = DateTime.Now;
            TimeSpan delta1 = new TimeSpan(after1.Ticks - before1.Ticks);
            System.Console.WriteLine("TestRedundantRecalculationsBug step1 lasted " + delta1.TotalSeconds + " seconds");

            System.Console.WriteLine("TestRedundantRecalculationsBug step2 started...");
            DateTime before2 = DateTime.Now;

            ws.Range("D13").Value = 2;

            DateTime after2 = DateTime.Now;
            TimeSpan delta2 = new TimeSpan(after2.Ticks - before2.Ticks);
            System.Console.WriteLine("TestRedundantRecalculationsBug step2 lasted " + delta2.TotalSeconds + " seconds");

            Assert.Equals(4356618, ws.Range("B45").Value);
            Assert.Equals(1883328, ws.Range("E45").Value);
            Assert.Less(delta1.TotalSeconds, delta2.TotalSeconds * 3);
            wb.Save("../../TestFiles/Formula_complex_dependencies_changed.xlsx");
            wb.Close();
        }

        [Test]
        public void TestCellsUpdatedEvent1()
        {
            System.Console.WriteLine("TestCellsUpdatedEvent()");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Fibonacci_chain.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;

            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ws.Range("A1").Value = 1;

            Assert.Equals(25, listener.UpdatedCellsList.Count);
            Assert.Equals("A1", listener.UpdatedCellsList[0]);
            Assert.Equals("C3", listener.UpdatedCellsList[1]);
            Assert.Equals("Z26", listener.UpdatedCellsList[24]);
            Assert.Equals("N14", listener.UpdatedCellsList[12]);

            listener.Detach();
            wb.Close();
        }

        [Test]
        public void TestCellsUpdatedEvent2()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Formula_complex_dependencies.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.Sheets["Лист1"] as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1").Formula = "2";
            Assert.Equals(2, listener.UpdatedCellsList.Count);
            Assert.Equals("A1", listener.UpdatedCellsList[0]);
            Assert.Equals("B1", listener.UpdatedCellsList[1]);
            listener.Reset();

            ws.Range("B4").Value = 3;
            Assert.Equals(7, listener.UpdatedCellsList.Count);
            Assert.Equals("B1", listener.UpdatedCellsList[2]);
            Assert.Equals("D1", listener.UpdatedCellsList[4]);
            Assert.Equals("B2", listener.UpdatedCellsList[0]);
            Assert.Equals("C2", listener.UpdatedCellsList[1]);
            Assert.Equals("C3", listener.UpdatedCellsList[6]);
            Assert.Equals("B4", listener.UpdatedCellsList[5]);
            Assert.Equals("D4", listener.UpdatedCellsList[3]);
            listener.Reset();

            ws.Range("AC1").Value = 4;
            Assert.Equals(6, listener.UpdatedCellsList.Count);
            Assert.Equals("AC1", listener.UpdatedCellsList[0]);
            Assert.Equals("AB2", listener.UpdatedCellsList[2]);
            Assert.Equals("AE4", listener.UpdatedCellsList[5]);
            listener.Reset();

            ws.Range("B7").Value = 3;
            Assert.Equals(6, listener.UpdatedCellsList.Count);
            Assert.Equals("A7", listener.UpdatedCellsList[1]);
            Assert.Equals("B7", listener.UpdatedCellsList[4]);
            Assert.Equals("B8", listener.UpdatedCellsList[0]);
            listener.Reset();

            ws.Range("A13").Value = 2;
            Assert.Equals(97, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("D13").Value = 2;
            Assert.Equals(97, listener.UpdatedCellsList.Count);
            listener.Reset();

            wb.Close();
        }

        [Test]
        public void TestWrongFormulasHandling()
        {
            editor.SetupLog("WrongFormulasHandling.log");
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B5").Formula = "A1>>A2";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.OperandExpectedError);

            ws.Range("B5").Formula = "UnknownOperand+ A1";
            Assert.Equals(2, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.UnknownOperandError);

            ws.Range("B5").Formula = "A1 +UnknownOperand";
            Assert.Equals(3, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.UnknownOperandError);

            ws.Range("B5").Formula = "A1048580";
            Assert.Equals(4, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.UnknownOperandError);

            ws.Range("B5").Formula = "A10485800";
            Assert.Equals(5, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.UnknownOperandError);

            ws.Range("B5:C6").Formula = "A1Z26";
            Assert.Equals(6, err_listener.AllHappenedErrors.Count); // There must be only one error reported
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.UnknownOperandError);

            ws.Range("B5").Formula = "A1+%A2";
            Assert.Equals(7, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.OperandExpectedError);

            ws.Range("B5").Formula = "A1+";
            Assert.Equals(8, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.OperandExpectedError);

            ws.Range("B5").Formula = "3+(A1+)";
            Assert.Equals(10, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.FormulaFormatError);

            wb.Close();
            editor.StopLogging();
        }

        [Test]
        public void TestFormulaReregistration()
        {
            System.Console.WriteLine("TestFormulaReregistration()");
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A3").Formula = "A1 + A2 + B1 + B2";
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("B1:B2").Formula = "A$1 + A$2";
            Assert.Equals(3, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A1:A2").Value = 5;
            Assert.Equals(5, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("B1").Formula = "A$1 + A$2"; // Reregister 1
            Assert.Equals(2, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A1").Value = 3; // Check 1
            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A3").Formula = "A1 + A2 + B1 + B2"; // Reregister 2
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A2").Value = 4; // Check 2
            Assert.Equals(4, listener.UpdatedCellsList.Count);
            listener.Reset();

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestFormulaSetNull()
        {
            System.Console.WriteLine("TestFormulaSetNull()");
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A3").Formula = "A1 + A2 + B1 + B2";
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("B1:B2").Formula = "A$1 + A$2";
            Assert.Equals(3, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A1:A2").Value = 5;
            Assert.Equals(5, listener.UpdatedCellsList.Count); // A1, A2, B1, B2, A3
            listener.Reset();

            ws.Range("B1").Formula = null;
            Assert.Equals(2, listener.UpdatedCellsList.Count); // B1, A3
            listener.Reset();

            ws.Range("A1").Value = 3;
            Assert.Equals(3, listener.UpdatedCellsList.Count); // A1, B2, A3
            listener.Reset();

            ws.Range("B2").Formula = "";
            Assert.Equals(2, listener.UpdatedCellsList.Count); // B2, A3
            listener.Reset();

            ws.Range("A1").Value = 4;
            Assert.Equals(2, listener.UpdatedCellsList.Count); // A1, A3
            listener.Reset();


            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestValueSetNull()
        {
            System.Console.WriteLine("TestValueSetNull()");
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A3").Formula = "A1 + A2 + B1 + B2";
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("B1:B2").Formula = "A$1 + A$2";
            Assert.Equals(3, listener.UpdatedCellsList.Count);
            listener.Reset();

            ws.Range("A1:A2").Value = 5;
            Assert.Equals(5, listener.UpdatedCellsList.Count); // A1, A2, B1, B2, A3
            listener.Reset();

            ws.Range("B1").Value = null;
            Assert.Equals(2, listener.UpdatedCellsList.Count); // B1, A3
            listener.Reset();

            ws.Range("A1").Value = 3;
            Assert.Equals(3, listener.UpdatedCellsList.Count); // A1, B2, A3
            listener.Reset();

            ws.Range("B2").Value = "";
            Assert.Equals(2, listener.UpdatedCellsList.Count); // B2, A3
            listener.Reset();

            ws.Range("A1").Value = 4;
            Assert.Equals(2, listener.UpdatedCellsList.Count); // A1, A3
            listener.Reset();


            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestSimpleRangeDependency()
        {
            System.Console.WriteLine("SimpleRangeDependency");
            Workbook wb = editor.CreateWorkbook("SimpleRangeDependency");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1:B3").Value = "5";
            ws.Range("C3").Formula = "SUM(A1:B3)";
            Assert.Equals(30, ws.Range("C3").Value);

            ws.Range("B2").Value = "6";
            Assert.Equals(31, ws.Range("C3").Value);

            ws.Range("B2:B3").Value = "7";
            Assert.Equals(34, ws.Range("C3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestComplexRangeDependency()
        {
            System.Console.WriteLine("SimpleRangeDependency");
            Workbook wb = editor.CreateWorkbook("SimpleRangeDependency");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);

            ws.Range("A1").Value = "50";
            ws.Range("B1:C1").Formula = "A1+1";
            ws.Range("A2:C3").Formula = "A1+1";
            ws.Range("C4").Formula = "SUM(A1:C1,A2:C2,B1:B2,B1,B2)";
            Assert.Equals(515, ws.Range("C4").Value);
            ws.Range("D4").Formula = "SUM(C3:C4)";
            Assert.Equals(569, ws.Range("D4").Value);
            ws.Range("D2").Formula = "A1-20";
            ws.Range("E2").Formula = "A1-30";
            ws.Range("E4").Formula = "MAX(D2:E2,B1-28)";
            Assert.Equals(30, ws.Range("E4").Value);
            ws.Range("E5").Formula = "SUM((D2:E4))";
            Assert.Equals(649, ws.Range("E5").Value);

            // The action
            ws.Range("B1").Value = 3;
            Assert.Equals(5, ws.Range("B3").Value);
            Assert.Equals(6, ws.Range("C3").Value);
            Assert.Equals(131, ws.Range("C4").Value);
            Assert.Equals(137, ws.Range("D4").Value);
            Assert.Equals(217, ws.Range("E5").Value);
            // Another action
            ws.Range("A1").Value = -50;
            Assert.Equals(-50, ws.Range("A1").Value);
            Assert.Equals(-48, ws.Range("A3").Value);
            Assert.Equals(5, ws.Range("B3").Value);
            Assert.Equals(6, ws.Range("C3").Value);
            Assert.Equals(-69, ws.Range("C4").Value);
            Assert.Equals(-63, ws.Range("D4").Value);
            Assert.Equals(-238, ws.Range("E5").Value);
            Assert.Equals(-70, ws.Range("D2").Value);
            Assert.Equals(-80, ws.Range("E2").Value);
            Assert.Equals(-25, ws.Range("E4").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestMissedArg()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);


            ws.Range("A1").Formula = "SUM(,,,,,)";
            Assert.Equals("SUM(,,,,,)", ws.Range("A1").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        private EWSEditor editor;
    }

    internal class CellsUpdatedListener
    {
        public CellsUpdatedListener(Worksheet worksheet) // Created already attached
        {
            UpdatedCellsList = new List<string>();
            Attach(worksheet);
        }

        private void CellsUpdated(object sender, EventArgs e)
        {
            UpdatedCellsList.InsertRange(0, (e as AVSOfficeEWSEditor.Editor.Events.CellsUpdatedEventArg).UpdatedCells);
        }

        public void Detach()
        {
            worksheet.CellsUpdatedEvent -= new EventHandler(CellsUpdated);
            Reset();
        }

        public void Attach(Worksheet worksheet)
        {
            this.worksheet = worksheet;
            worksheet.CellsUpdatedEvent += new EventHandler(CellsUpdated);
            Reset();
        }

        public void Reset()
        {
            UpdatedCellsList.Clear();
        }

        public List<string> UpdatedCellsList { get; private set; }
        private Worksheet worksheet;
    }

    internal class ErrorListener
    {
        public ErrorListener(EWSEditor editor) // Created already attached
        {
            AllHappenedErrors = new List<AVSOfficeEWSEditor.Editor.Events.ErrorDescription>();
            Attach(editor);
        }

        private void ErrorHappened(object sender, EventArgs e)
        {
            LastError = e as AVSOfficeEWSEditor.Editor.Events.ErrorDescription;
            if(LastError != null)
            {
                AllHappenedErrors.Add(LastError);
            }
        }

        public void Detach()
        {
            editor.ErrorHappenedEvent -= new EventHandler(ErrorHappened);
            Reset();
        }

        public void Attach(EWSEditor editor)
        {
            this.editor = editor;
            editor.ErrorHappenedEvent += new EventHandler(ErrorHappened);
            Reset();
        }

        public void Reset()
        {
            AllHappenedErrors.Clear();
            LastError = null;
        }

        public List<AVSOfficeEWSEditor.Editor.Events.ErrorDescription> AllHappenedErrors { get; private set; }
        public AVSOfficeEWSEditor.Editor.Events.ErrorDescription LastError { get; private set; }

        private EWSEditor editor;
    }

}
