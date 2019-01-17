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
using AVSOfficeEWSEditor.Editor.Events;
using csUnit;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Formulas, 3DReferencesTest")]
    public class _3DReferencesTest
    {
        public _3DReferencesTest()
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
        public void Test_Simple3DArea()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            ws.Range("A20").Formula = "SUM(B1:D8)";
            Assert.Equals(108, ws.Range("A20").Value);
            Assert.Equals("SUM(B1:D8)", ws.Range("A20").Formula);

            ws.Range("A20").Formula = "SUM(Sheet1!B1:D9)";
            Assert.Equals(108, ws.Range("A20").Value);
            Assert.Equals("SUM(Sheet1!B1:D9)", ws.Range("A20").Formula);

            ws.Range("A21").Formula = "SUM(Sheet1:で音楽!B4:C5)";
            Assert.Equals(102, ws.Range("A21").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C5)", ws.Range("A21").Formula);

            ws.Range("B20").Formula = "SUM(Hello.Dud:Sheet#3!B4:C5)";
            Assert.True(err_listener.LastError is UnknownOperandError);
            err_listener.Reset();
            ws.Range("B20").Formula = "SUM('Hello.Dud:Sheet#3'!B4:C5)";
            Assert.Equals(60, ws.Range("B20").Value);
            Assert.Equals("SUM('Hello.Dud:Sheet#3'!B4:C5)", ws.Range("B20").Formula);

            wb.Worksheets["Sheet#3"].Range("A4:D6").Value = 30;
            Assert.Equals(108, ws.Range("A20").Value);
            Assert.Equals(190, ws.Range("A21").Value);
            Assert.Equals(148, ws.Range("B20").Value);

            ws.Range("3:4").DeleteRows();

            Assert.Equals(72, ws.Range("A18").Value);
            Assert.Equals("SUM(Sheet1!B1:D7)", ws.Range("A18").Formula);
            Assert.Equals(178, ws.Range("A19").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C5)", ws.Range("A19").Formula);
            Assert.Equals(148, ws.Range("B18").Value);
            Assert.Equals("SUM('Hello.Dud:Sheet#3'!B4:C5)", ws.Range("B18").Formula);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_3DAreaReferenceOperators()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            wb.Save("../../TestFiles/3DRefSimple.xlsx");

            ws.Range("B21").Formula = "SUM(('Hello.Dud:Sheet#3'!B4:C5,Sheet1!B1:D9) Sheet1:Sheet5!C:C)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM('Hello.Dud:Sheet#3'!B4:C5 : Sheet1!B1:D9)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM(Hello.Dud!B4:C5 : Sheet1!B1:D9)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM(Hello.Dud!B1:C5 : Hello.Dud!B4:D9)";
            Assert.Null(err_listener.LastError);
            Assert.Equals(105, ws.Range("B21").Value);
            wb.Worksheets["Hello.Dud"].Range("B1").Value = 2;
            Assert.Equals(100, ws.Range("B21").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_Simple3DRef()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            ws.Range("A20").Formula = "SUM(B1,B2:D8)";
            Assert.Equals(96, ws.Range("A20").Value);
            Assert.Equals("SUM(B1,B2:D8)", ws.Range("A20").Formula);

            ws.Range("A20").Formula = "SUM(Sheet1!B1:D7,Sheet1!D8)";
            Assert.Equals(108, ws.Range("A20").Value);
            Assert.Equals("SUM(Sheet1!B1:D7,Sheet1!D8)", ws.Range("A20").Formula);

            ws.Range("A21").Formula = "SUM(Sheet1:で音楽!B4)";
            Assert.Equals(21, ws.Range("A21").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", ws.Range("A21").Formula);

            ws.Range("B20").Formula = "SUM(Sheet#3!B5)";
            Assert.True(err_listener.LastError is UnknownOperandError);
            err_listener.Reset();
            ws.Range("B20").Formula = "SUM('Sheet#3'!B5)";
            Assert.Equals(8, ws.Range("B20").Value);
            Assert.Equals("SUM('Sheet#3'!B5)", ws.Range("B20").Formula);

            wb.Worksheets["Sheet#3"].Range("B4").Value = 30;
            Assert.Equals(108, ws.Range("A20").Value);
            Assert.Equals(43, ws.Range("A21").Value);
            Assert.Equals(8, ws.Range("B20").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_NonUpdated3DRefOnDelete()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D5").Value = 6;
            wb.Worksheets[0].Range("A6:D6").Value = 5;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            ws.Range("A20").Formula = "SUM(Sheet1!B1:D7,Sheet1!D8)";
            ws.Range("A21").Formula = "SUM(Sheet1:で音楽!B4)";
            ws.Range("B20").Formula = "SUM('Sheet#3'!B5)";
            wb.Worksheets["Sheet#3"].Range("B4").Value = 30;
            wb.Worksheets["Sheet#3"].Range("C20").Formula = "SUM(Sheet1:で音楽!B4)";
            wb.Worksheets["Sheet5"].Range("C20").Formula = "SUM(Sheet1:で音楽!B4)";
            ws.Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)";
            wb.Worksheets["Sheet#3"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+1";
            wb.Worksheets["Sheet5"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+2";
            ws.Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet#3"].Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet5"].Range("D21").Formula = "D20*2";

            ws.Range("3:4").DeleteRows();

            Assert.Equals(69, ws.Range("A18").Value);
            Assert.Equals("SUM(Sheet1!B1:D5,Sheet1!D6)", ws.Range("A18").Formula);

            Assert.Equals(42, ws.Range("A19").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", ws.Range("A19").Formula);
            Assert.Equals(42, wb.Worksheets["Sheet#3"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", wb.Worksheets["Sheet#3"].Range("C20").Formula);
            Assert.Equals(42, wb.Worksheets["Sheet5"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", wb.Worksheets["Sheet5"].Range("C20").Formula);

            Assert.Equals(62, ws.Range("D18").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)", ws.Range("D18").Formula);
            Assert.Equals(63, wb.Worksheets["Sheet#3"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+1", wb.Worksheets["Sheet#3"].Range("D20").Formula);
            Assert.Equals(64, wb.Worksheets["Sheet5"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+2", wb.Worksheets["Sheet5"].Range("D20").Formula);

            Assert.Equals(124, ws.Range("D19").Value);
            Assert.Equals("D18*2", ws.Range("D19").Formula);
            Assert.Equals(126, wb.Worksheets["Sheet#3"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet#3"].Range("D21").Formula);
            Assert.Equals(128, wb.Worksheets["Sheet5"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet5"].Range("D21").Formula);

            Assert.Equals(8, ws.Range("B18").Value);
            Assert.Equals("SUM('Sheet#3'!B5)", ws.Range("B18").Formula);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_NonUpdated3DAreaOnCut()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D5").Value = 6;
            wb.Worksheets[0].Range("A6:D6").Value = 5;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            ws.Range("A20").Formula = "SUM(Sheet1!B1:D7,Sheet1!D8)";
            ws.Range("A21").Formula = "SUM(Sheet1:で音楽!B4)";
            ws.Range("B20").Formula = "SUM('Sheet#3'!B5)";
            wb.Worksheets["Sheet#3"].Range("B4").Value = 30;
            wb.Worksheets["Sheet#3"].Range("C20").Formula = "SUM(Sheet1:で音楽!B4)";
            wb.Worksheets["Sheet5"].Range("C20").Formula = "SUM(Sheet1:で音楽!B4)";
            ws.Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)";
            wb.Worksheets["Sheet#3"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+1";
            wb.Worksheets["Sheet5"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+2";
            ws.Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet#3"].Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet5"].Range("D21").Formula = "D20*2";

            ws.Range("2:3").DeleteRows();

            Assert.Equals(69, ws.Range("A18").Value);

            Assert.Equals("SUM(Sheet1!B1:D5,Sheet1!D6)", ws.Range("A18").Formula);

            Assert.Equals(42, ws.Range("A19").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", ws.Range("A19").Formula);
            Assert.Equals(42, wb.Worksheets["Sheet#3"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", wb.Worksheets["Sheet#3"].Range("C20").Formula);
            Assert.Equals(42, wb.Worksheets["Sheet5"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4)", wb.Worksheets["Sheet5"].Range("C20").Formula);

            Assert.Equals(62, ws.Range("D18").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)", ws.Range("D18").Formula);
            Assert.Equals(63, wb.Worksheets["Sheet#3"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+1", wb.Worksheets["Sheet#3"].Range("D20").Formula);
            Assert.Equals(64, wb.Worksheets["Sheet5"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+2", wb.Worksheets["Sheet5"].Range("D20").Formula);

            Assert.Equals(124, ws.Range("D19").Value);
            Assert.Equals("D18*2", ws.Range("D19").Formula);
            Assert.Equals(126, wb.Worksheets["Sheet#3"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet#3"].Range("D21").Formula);
            Assert.Equals(128, wb.Worksheets["Sheet5"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet5"].Range("D21").Formula);

            Assert.Equals(8, ws.Range("B18").Value);
            Assert.Equals("SUM('Sheet#3'!B5)", ws.Range("B18").Formula);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_NonUpdated3DOnInsert()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D5").Value = 6;
            wb.Worksheets[0].Range("A6:D6").Value = 5;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            ws.Range("A20").Formula = "SUM(Sheet1!B1:D7,Sheet1!D8)";
            ws.Range("A21").Formula = "SUM(Sheet1:で音楽!D4)";
            wb.Worksheets["Sheet#3"].Range("B4").Value = 30;
            wb.Worksheets["Sheet#3"].Range("C20").Formula = "SUM(Sheet1:で音楽!D4)";
            wb.Worksheets["Sheet5"].Range("C20").Formula = "SUM(Sheet1:で音楽!D4)";
            ws.Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)";
            wb.Worksheets["Sheet#3"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+1";
            wb.Worksheets["Sheet5"].Range("D20").Formula = "SUM(Sheet1:で音楽!B4:C4)+2";
            ws.Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet#3"].Range("D21").Formula = "D20*2";
            wb.Worksheets["Sheet5"].Range("D21").Formula = "D20*2";
            wb.Save("../../TestFiles/3DRefSimple.xlsx");

            ws.Range("C:F").InsertColumns(2);

            Assert.Equals(105, ws.Range("A20").Value);
            Assert.Equals("SUM(Sheet1!B1:F7,Sheet1!F8)", ws.Range("A20").Formula);

            Assert.Equals(7, ws.Range("A21").Value);
            Assert.Equals("SUM(Sheet1:で音楽!D4)", ws.Range("A21").Formula);
            Assert.Equals(7, wb.Worksheets["Sheet#3"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!D4)", wb.Worksheets["Sheet#3"].Range("C20").Formula);
            Assert.Equals(7, wb.Worksheets["Sheet5"].Range("C20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!D4)", wb.Worksheets["Sheet5"].Range("C20").Formula);

            Assert.Equals(58, ws.Range("F20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)", ws.Range("F20").Formula);
            Assert.Equals(59, wb.Worksheets["Sheet#3"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+1", wb.Worksheets["Sheet#3"].Range("D20").Formula);
            Assert.Equals(60, wb.Worksheets["Sheet5"].Range("D20").Value);
            Assert.Equals("SUM(Sheet1:で音楽!B4:C4)+2", wb.Worksheets["Sheet5"].Range("D20").Formula);

            Assert.Equals(116, ws.Range("F21").Value);
            Assert.Equals("F20*2", ws.Range("F21").Formula);
            Assert.Equals(118, wb.Worksheets["Sheet#3"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet#3"].Range("D21").Formula);
            Assert.Equals(120, wb.Worksheets["Sheet5"].Range("D21").Value);
            Assert.Equals("D20*2", wb.Worksheets["Sheet5"].Range("D21").Formula);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_3DRefReferenceOperators()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;


            ws.Range("B21").Formula = "SUM(('Hello.Dud:Sheet#3'!B4,Sheet1!B1) Sheet1!C:C)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM('Hello.Dud:Sheet#3'!B4 : Sheet1!B1)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM(Hello.Dud!C5 : Sheet1!B1)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM(Hello.Dud!C4 : Hello.Dud!B5)";
            Assert.Null(err_listener.LastError);
            Assert.Equals(28, ws.Range("B21").Value);
            wb.Worksheets["Hello.Dud"].Range("C5").Value = 2;
            Assert.Equals(23, ws.Range("B21").Value);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_Wrong3DReferences()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;


            ws.Range("B21").Formula = "SUM(Hello.Dudd!B4,Sheet1!B1)";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "Hello.Dud!#NAME?";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "Hello.Dud!#VALUE!";
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B21").Formula = "SUM(Hello.Dud!#REF! : Hello.Dud!B5)";
            Assert.Null(err_listener.LastError);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B21").Value);

            ws.Range("B21").Formula = "Hello.Dud!#REF!";
            Assert.Null(err_listener.LastError);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B21").Value);
            Assert.Equals("Hello.Dud!#REF!", ws.Range("B21").Formula);

            ws.Range("B21").Formula = "SUM(Hello.Dud!#REF!,Hello.Dud!B5)";
            Assert.Null(err_listener.LastError);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B21").Value);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_ReferencedWorksheetRemove()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            wb.Worksheets[1].Range("A20").Formula = "SUM('Sheet#3:Sheet5'!A1:D8) + SUM(で音楽:Sheet5!C4:D6)";
            wb.Worksheets[1].Range("A21").Formula = "SUM('Sheet#3:で音楽'!A1:D8) - SUM(で音楽!C4:D6)";
            wb.Worksheets[2].Range("A20").Formula = "SUM(Hello.Dud:で音楽!D8) + SUM('Sheet#3:で音楽'!C4:D6)";
            wb.Worksheets[3].Range("A20").Formula = "SUM('Hello.Dud:Sheet#3'!A1:D8) + SUM('Sheet#3'!#REF!)";
            wb.Worksheets[4].Range("A20").Formula = "SUM(Hello.Dud:Sheet5!A1:D8) + SUM('Hello.Dud:で音楽'!C4)";
            wb.Worksheets[4].Range("A21").Formula = "SUM(Hello.Dud:Sheet5!A1:D8) - SUM('Sheet#3:で音楽'!C4)";

            Assert.Equals("SUM('Sheet#3:Sheet5'!A1:D8)+SUM(で音楽:Sheet5!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM('Sheet#3:で音楽'!A1:D8)-SUM(で音楽!C4:D6)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM(Hello.Dud:で音楽!D8)+SUM('Sheet#3:で音楽'!C4:D6)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM('Hello.Dud:Sheet#3'!A1:D8)+SUM('Sheet#3'!#REF!)", wb.Worksheets[3].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM(Hello.Dud:で音楽!C4)", wb.Worksheets[4].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM('Sheet#3:で音楽'!C4)", wb.Worksheets[4].Range("A21").Formula);

            Assert.Equals(284, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(132, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(60, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[3].Range("A20").Value);
            Assert.Equals(348, wb.Worksheets[4].Range("A20").Value);
            Assert.Equals(325, wb.Worksheets[4].Range("A21").Value);

            wb.Worksheets["Sheet#3"].Remove();

            Assert.Equals("SUM(で音楽:Sheet5!A1:D8)+SUM(で音楽:Sheet5!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM(で音楽!A1:D8)-SUM(で音楽!C4:D6)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM(Hello.Dud!A1:D8)+SUM(#REF!)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM(Hello.Dud:で音楽!C4)", wb.Worksheets[3].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM(で音楽!C4)", wb.Worksheets[3].Range("A21").Formula);

            Assert.Equals(188, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(36, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(244, wb.Worksheets[3].Range("A20").Value);
            Assert.Equals(237, wb.Worksheets[3].Range("A21").Value);

            wb.Worksheets["で音楽"].Remove();

            Assert.Equals("SUM(Sheet5!A1:D8)+SUM(Sheet5!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM(#REF!)-SUM(#REF!)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM(Hello.Dud!C4)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM(#REF!)", wb.Worksheets[2].Range("A21").Formula);

            Assert.Equals(80, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(172, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[2].Range("A21").Value);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_MissedDelayedRecalculation()
        {
            Workbook wb = editor.CreateWorkbook("3D", 2);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].Range("B2").Value = 7;
            wb.Worksheets[1].Range("B3:B6").Formula = "B2+4";
            wb.Worksheets[1].Range("C2:E2").Formula = "B2-2";
            wb.Worksheets[1].Range("C3:E6").Formula = "B2+B3-C2";

            wb.Worksheets[0].Range("A20").Formula = "SUM(Sheet1:Sheet2!A4:E5)+SUM(Sheet2!A5)";
            Assert.Equals("SUM(Sheet1:Sheet2!A4:E5)+SUM(Sheet2!A5)", wb.Worksheets[0].Range("A20").Formula);
            Assert.Equals(136, wb.Worksheets[0].Range("A20").Value);

            wb.Save("../../TestFiles/ws_remove.xlsx");

            wb.Worksheets[1].Range("A1").DeleteRows();

            Assert.Equals("SUM(Sheet1:Sheet2!A4:E5)+SUM(Sheet2!A4)", wb.Worksheets[0].Range("A20").Formula);
            Assert.Equals(168, wb.Worksheets[0].Range("A20").Value); // The reason of the test

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        [Test]
        public void Test_ReferencedWorksheetRename()
        {
            Workbook wb = editor.CreateWorkbook("3D", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Worksheets[1].SetName("Hello.Dud");
            wb.Worksheets[2].SetName("Sheet#3");
            wb.Worksheets[3].SetName("で音楽");

            wb.Worksheets[0].Range("A1:D6").Value = 6;
            wb.Worksheets[1].Range("B1:E5").Value = 7;
            wb.Worksheets[2].Range("B3:C9").Value = 8;
            wb.Worksheets[3].Range("A5:K6").Value = 9;
            wb.Worksheets[4].Range("C6:F11").Value = 10;

            wb.Worksheets[1].Range("A20").Formula = "SUM('Sheet#3:Sheet5'!A1:D8) + SUM(で音楽:Sheet5!C4:D6)";
            wb.Worksheets[1].Range("A21").Formula = "SUM('Sheet#3:で音楽'!A1:D8) - SUM(で音楽!C4:D6)";
            wb.Worksheets[2].Range("A20").Formula = "SUM(Hello.Dud:で音楽!D8) + SUM('Sheet#3:で音楽'!C4:D6)";
            wb.Worksheets[3].Range("A20").Formula = "SUM('Hello.Dud:Sheet#3'!A1:D8) + SUM('Sheet#3'!#REF!)";
            wb.Worksheets[4].Range("A20").Formula = "SUM(Hello.Dud:Sheet5!A1:D8) + SUM('Hello.Dud:で音楽'!C4)";
            wb.Worksheets[4].Range("A21").Formula = "SUM(Hello.Dud:Sheet5!A1:D8) - SUM('Sheet#3:で音楽'!C4)";

            Assert.Equals("SUM('Sheet#3:Sheet5'!A1:D8)+SUM(で音楽:Sheet5!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM('Sheet#3:で音楽'!A1:D8)-SUM(で音楽!C4:D6)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM(Hello.Dud:で音楽!D8)+SUM('Sheet#3:で音楽'!C4:D6)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM('Hello.Dud:Sheet#3'!A1:D8)+SUM('Sheet#3'!#REF!)", wb.Worksheets[3].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM(Hello.Dud:で音楽!C4)", wb.Worksheets[4].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM('Sheet#3:で音楽'!C4)", wb.Worksheets[4].Range("A21").Formula);

            Assert.Equals(284, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(132, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(60, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[3].Range("A20").Value);
            Assert.Equals(348, wb.Worksheets[4].Range("A20").Value);
            Assert.Equals(325, wb.Worksheets[4].Range("A21").Value);

            wb.Worksheets["Sheet#3"].SetName("Лист3");

            Assert.Equals("SUM(Лист3:Sheet5!A1:D8)+SUM(で音楽:Sheet5!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM(Лист3:で音楽!A1:D8)-SUM(で音楽!C4:D6)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM(Hello.Dud:で音楽!D8)+SUM(Лист3:で音楽!C4:D6)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Лист3!A1:D8)+SUM(Лист3!#REF!)", wb.Worksheets[3].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM(Hello.Dud:で音楽!C4)", wb.Worksheets[4].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM(Лист3:で音楽!C4)", wb.Worksheets[4].Range("A21").Formula);

            Assert.Equals(284, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(132, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(60, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[3].Range("A20").Value);
            Assert.Equals(348, wb.Worksheets[4].Range("A20").Value);
            Assert.Equals(325, wb.Worksheets[4].Range("A21").Value);

            wb.Worksheets["で音楽"].SetName("#DIV/0!");

            Assert.Equals("SUM(Лист3:Sheet5!A1:D8)+SUM('#DIV/0!:Sheet5'!C4:D6)", wb.Worksheets[1].Range("A20").Formula);
            Assert.Equals("SUM('Лист3:#DIV/0!'!A1:D8)-SUM('#DIV/0!'!C4:D6)", wb.Worksheets[1].Range("A21").Formula);
            Assert.Equals("SUM('Hello.Dud:#DIV/0!'!D8)+SUM('Лист3:#DIV/0!'!C4:D6)", wb.Worksheets[2].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Лист3!A1:D8)+SUM(Лист3!#REF!)", wb.Worksheets[3].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)+SUM('Hello.Dud:#DIV/0!'!C4)", wb.Worksheets[4].Range("A20").Formula);
            Assert.Equals("SUM(Hello.Dud:Sheet5!A1:D8)-SUM('Лист3:#DIV/0!'!C4)", wb.Worksheets[4].Range("A21").Formula);

            Assert.Equals(284, wb.Worksheets[1].Range("A20").Value);
            Assert.Equals(132, wb.Worksheets[1].Range("A21").Value);
            Assert.Equals(60, wb.Worksheets[2].Range("A20").Value);
            Assert.Equals(ErrorValue.bad_reference, wb.Worksheets[3].Range("A20").Value);
            Assert.Equals(348, wb.Worksheets[4].Range("A20").Value);
            Assert.Equals(325, wb.Worksheets[4].Range("A21").Value);

            Assert.Null(err_listener.LastError);
            wb.Close();
        }

        private EWSEditor editor;
    }
}
