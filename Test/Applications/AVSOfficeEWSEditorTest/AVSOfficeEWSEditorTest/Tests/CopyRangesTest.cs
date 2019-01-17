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
    [TestFixture(Categories = "CopyRanges")]
    public class CopyRangesTest
    {
        public CopyRangesTest()
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
        public void TestCopyWithinOneSheet()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            ws.GetUpdatedCells();
            listener.Reset();

            ws.Range("I10").Formula = "B4+A3";
            ws.Range("I10").Format.Font.Bold = true;
            listener.Reset();
            //wb.Save("../../TestFiles/Simple_sort_before.xlsx");

            ws.Range("C4:F14").CopyTo(ws.Range("H9:H23"));

            Assert.Equals(44, listener.UpdatedCellsList.Count);

            wb.Save("../../TestFiles/Copy_inside_sheet_forward.xlsx");

            Assert.Equals(5, ws.Range("H9").Value);
            Assert.Equals(true, ws.Range("I9").Value);
            Assert.Equals("wtf", ws.Range("H10").Value);
            Assert.Equals("", ws.Range("I10").Value);
            Assert.Equals("", ws.Range("I10").Formula);
            Assert.Equals(false, ws.Range("I10").Format.Font.Bold);
            Assert.Equals(BorderStyle.dotted, ws.Range("J11").Format.Borders.Bottom.Style);
            Assert.Equals(BorderStyle.thin, ws.Range("H15").Format.Borders.Left.Style);
            Assert.Equals("", ws.Range("K18").Value);
            Assert.Equals(BorderStyle.none, ws.Range("K18").Format.Borders.Right.Style);

            ws.Range("H19:K19").CopyTo(ws.Range("A14"));
            Assert.Equals(BorderStyle.none, ws.Range("D14").Format.Borders.Bottom.Style);
            Assert.Equals("", ws.Range("C14").Value);
            Assert.Equals("storage", ws.Range("A14").Value);
            Assert.Equals(BorderStyle.thin, ws.Range("A14").Format.Borders.Bottom.Style);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestOverlappedCopy()
        {
            PrepareSimpleData();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            ws.GetUpdatedCells();
            listener.Reset();

            ws.Range("C4:F14").CopyTo(ws.Range("C4:F14"));
            Assert.Equals(0, listener.UpdatedCellsList.Count);

            ws.Range("C4:F14").CopyTo(ws.Range("E2:H12"));
            Assert.Equals(44, listener.UpdatedCellsList.Count);

            wb.Save("../../TestFiles/Copy_overlapped.xlsx");

            Assert.Equals(5, ws.Range("E2").Value);
            Assert.Equals(BorderStyle.thin, ws.Range("E2").Format.Borders.Left.Style);
            Assert.Equals(BorderStyle.thin, ws.Range("E2").Format.Borders.Top.Style);
            Assert.Equals("wtf", ws.Range("E3").Value);
            Assert.Equals(BorderStyle.none, ws.Range("E3").Format.Borders.Bottom.Style);
            Assert.Equals(20, ws.Range("E4").Value);
            Assert.Equals("storage", ws.Range("E12").Value);
            Assert.Equals(BorderStyle.thin, ws.Range("E12").Format.Borders.Bottom.Style);
            Assert.Equals("", ws.Range("E11").Value);
            Assert.Equals(456.123, ws.Range("F6").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

        }

        [Test]
        public void TestCopyWithFormulasToAnotherSheet()
        {
            PrepareSimpleFormulas();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            ws = wb.Sheets.AddEmptySheet("Sheet2");
            ws.Range("A1").Value = 10;
            wb.Worksheets[0].Range("C3:G7").CopyTo(ws.Range("A2"));
            wb.Save("../../TestFiles/Copy_to_another_sheet.xlsx");

            Assert.Equals(20, ws.Range("A2").Value);
            Assert.Equals("55.90169944", ws.Range("C2").DispValue);
            Assert.Equals("5^2.5", ws.Range("C2").Formula);

            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);
            Assert.Equals("#REF!+#REF!", ws.Range("A3").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B3").Value);
            Assert.Equals("A2+A3", ws.Range("B3").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C4").Value);
            Assert.Equals("-B3+A2*2", ws.Range("C4").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D5").Value);
            Assert.Equals("C4^B3", ws.Range("D5").Formula);

            Assert.Equals(BorderStyle.thin, ws.Range("C6").Format.Borders.Bottom.Style);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }


        [Test]
        public void TestCopyWithFormulasToAnotherWorkbook()
        {
            PrepareSimpleFormulas();
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);
            ws = wb.ActiveWorksheet;

            Workbook dest_wb = editor.CreateWorkbook("www", 2);
            Worksheet dest_ws = dest_wb.Worksheets[1];
            dest_ws.Range("D10").Formula = "E10 * F10";
            dest_ws.Range("D8").Value = 3;
            dest_ws.Range("E8").Value = 2;

            ws.Range("C3:G7").CopyTo(dest_ws.Range("E9"));

            Assert.Equals(21, dest_ws.Range("D10").Value); // Check formulas out of copied range update
            Assert.Equals(BorderStyle.none, dest_ws.Range("E9").Format.Borders.Top.Style); // Check that no format copied
            Assert.Equals(BorderStyle.none, dest_ws.Range("E9").Format.Borders.Left.Style); // Check that no format copied
            Assert.Equals(4, dest_ws.Range("E9").Value);
            Assert.Equals(1, dest_ws.Range("H12").Value);

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

        private EWSEditor editor;
        private Workbook wb;
        private Worksheet ws;
    }
}
