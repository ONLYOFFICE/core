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
using AVSOfficeEWSEditor.Editor.Format.Colors;
using AVSOfficeEWSEditor.Editor.Format.Alignment;
using AVSOfficeEWSEditor.Editor.Format.Borders;
using AVSOfficeEWSEditor.Editor.Format.Fills;
using csUnit;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Format")]
    public class FormatTest
    {
        public FormatTest()
        {
        }

        [SetUp]
        public void SetUp()
        {
            editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Assert.NotNull(editor.LoadWorkbook("../../TestFiles/Format.xlsx"));
            wb = editor.ActiveWorkbook;
            ws = editor.ActiveWorksheet;
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

        [Test(Categories = "Font")]
        public void TestFont()
        {
            Assert.Equals(false, wb.ActiveWorksheet.Range("A1:R1").Format.Font.Bold);
            Assert.Equals(true, wb.ActiveWorksheet.Range("A2:B2").Format.Font.Bold);
            Assert.Equals(true, wb.ActiveWorksheet.Range("C4").Format.Font.Bold);
            Assert.Equals(false, wb.ActiveWorksheet.Range("A1:R1").Format.Font.Italic);
            Assert.Null(wb.ActiveWorksheet.Range("A2:B2").Format.Font.Italic);
            Assert.Equals(true, wb.ActiveWorksheet.Range("B2:C2").Format.Font.Italic);
            Assert.Equals(false, wb.ActiveWorksheet.Range("BB2").Format.Font.Italic);
            Worksheet ws = wb.ActiveSheet as Worksheet;

            ws.Range("B2:E3").Format.Font.Bold = true;
            Assert.Equals(true, wb.ActiveWorksheet.Range("B3").Format.Font.Bold);

            ws.Range("C1:D7").Format.Font.Italic = true;
            ws.Range("A1:A1").Format.Font.Underline = true;

            System.Console.WriteLine("Setting Range(\"2:3\") to Bold started ...");
            DateTime before1 = DateTime.Now;
            ws.Range("2:3").Format.Font.Bold = true;
            DateTime after1 = DateTime.Now;
            TimeSpan delta1 = new TimeSpan(after1.Ticks - before1.Ticks);
            System.Console.WriteLine("Setting Range(\"2:3\") to Bold lasted " + delta1.TotalSeconds + " seconds");

            ws.Range("A2:A7").Format.Font.Name = "Mistral";
            ws.Range("A2:A7").ColumnWidth = 13.7;
            ws.Range("C2:D2").Format.Font.Italic = false;
            Assert.Equals("Mistral", wb.ActiveWorksheet.Range("A4").Format.Font.Name);

            Assert.Equals(2, wb.ActiveWorksheet.Range("A1").Format.Font.Color.GetExactIndex());
            Assert.Equals(51, wb.ActiveWorksheet.Range("B2").Format.Font.Color.GetClosestIndex());

            RGB color1 = new RGB(12, 112, 212);
            ws.Range("C2:D2").Format.Font.Color.Rgb = color1;
            RGB color2 = new RGB(112, 212, 12);
            ws.Range("D1:E1").Format.Font.Color.Rgb = color2;
            Assert.Equals(new RGB(12, 112, 212), wb.ActiveWorksheet.Range("C2").Format.Font.Color.Rgb);
            Assert.Equals(new RGB(12, 112, 212), wb.ActiveWorksheet.Range("C2").Format.Font.Color.Rgb);

            Assert.Equals("31859B", wb.ActiveWorksheet.Range("B8").Format.Font.Color.ToString()); // Parse color set by theme number

            // Bug with font size set to 10 check
            ws.Range("H15").Value = "Text size";
            ws.Range("H15").Format.Font.Size = 9;
            Assert.Equals(9, ws.Range("H15").Format.Font.Size);
            ws.Range("H15").Format.Font.Size = 10;
            Assert.Equals(10, ws.Range("H15").Format.Font.Size);
            ws.Range("H15").Format.Font.Size = 11;
            Assert.Equals(11, ws.Range("H15").Format.Font.Size);
            ws.Range("H15").Format.Font.Size = 10;
            Assert.Equals(10, ws.Range("H15").Format.Font.Size);



            wb.Save("../../TestFiles/Format_Font_result.xlsx");
        }

        [Test(Categories = "Alignment")]
        public void TestAlignment()
        {
            Assert.Equals(HorizontalAlignmentType.left, wb.ActiveWorksheet.Range("B2").Format.Alignment.Horizontal);
            Assert.Equals(HorizontalAlignmentType.general, wb.ActiveWorksheet.Range("C2").Format.Alignment.Horizontal);
            Assert.Equals(HorizontalAlignmentType.right, wb.ActiveWorksheet.Range("D2").Format.Alignment.Horizontal);
            Assert.Null(wb.ActiveWorksheet.Range("B2:D2").Format.Alignment.Horizontal);

            wb.ActiveWorksheet.Range("B2:D2").Format.Alignment.Horizontal = HorizontalAlignmentType.center;
            Assert.Equals(HorizontalAlignmentType.center, wb.ActiveWorksheet.Range("C2").Format.Alignment.Horizontal);

            wb.ActiveWorksheet.Range("D7:E7").Format.Alignment.Horizontal = HorizontalAlignmentType.left;
            Assert.Equals(HorizontalAlignmentType.general, wb.ActiveWorksheet.Range("F7").Format.Alignment.Horizontal);
            wb.ActiveWorksheet.Range("D7:E7").Format.Alignment.Indent = 4;
            Assert.Equals(4, wb.ActiveWorksheet.Range("D7").Format.Alignment.Indent);
            // Vertical
            Assert.Equals(VerticalAlignmentType.bottom, wb.ActiveWorksheet.Range("B2").Format.Alignment.Vertical);
            wb.ActiveWorksheet.Range("C2").Format.Alignment.Vertical = VerticalAlignmentType.center;
            Assert.Equals(VerticalAlignmentType.center, wb.ActiveWorksheet.Range("C2").Format.Alignment.Vertical);
            wb.ActiveWorksheet.Range("D2").Format.Alignment.Vertical = VerticalAlignmentType.distributed;
            Assert.Equals(VerticalAlignmentType.distributed, wb.ActiveWorksheet.Range("D2").Format.Alignment.Vertical);
            wb.ActiveWorksheet.Range("E2").Format.Alignment.Vertical = VerticalAlignmentType.justify;
            Assert.Equals(VerticalAlignmentType.justify, wb.ActiveWorksheet.Range("E2").Format.Alignment.Vertical);
            wb.ActiveWorksheet.Range("B2").Format.Alignment.Vertical = VerticalAlignmentType.top;
            Assert.Equals(VerticalAlignmentType.top, wb.ActiveWorksheet.Range("B2").Format.Alignment.Vertical);

            wb.Save("../../TestFiles/Format_Alignment_result.xlsx");
        }

        [Test(Categories = "Protection")]
        public void TestProtection()
        {
            Assert.Equals(false, wb.ActiveWorksheet.Range("A7:EE20").Format.Protection.Hidden);
        }

        [Test(Categories = "Borders")]
        public void TestBorders()
        {
            Assert.Equals("CC0066", wb.ActiveWorksheet.Range("B2").Format.Borders.Left.Color.ToString());
            Assert.True(wb.ActiveWorksheet.Range("B2:C7").Format.Borders.Left.Color.HasDifferentValues());
            Assert.True(wb.ActiveWorksheet.Range("B2:C2").Format.Borders.Left.Color.HasDifferentValues());
            Assert.Equals("", wb.ActiveWorksheet.Range("C2:C3").Format.Borders.Left.Color.ToString()); // no color
            Assert.Equals(BorderStyle.mediumDashed, wb.ActiveWorksheet.Range("C2:C3").Format.Borders.Right.Style);
            Assert.False(wb.ActiveWorksheet.Range("C4:C6").Format.Borders.Left.Color.HasDifferentValues());
            Assert.Equals(BorderStyle.doubleLine, wb.ActiveWorksheet.Range("B3").Format.Borders.Right.Style);
            Assert.Null(wb.ActiveWorksheet.Range("B3:C3").Format.Borders.Right.Style);

            wb.ActiveWorksheet.Range("B4:C5").Format.Borders.Top.Style = BorderStyle.doubleLine;
            wb.ActiveWorksheet.Range("C6").Format.Borders.Right.Style = BorderStyle.none;
            wb.ActiveWorksheet.Range("C6").Format.Borders.Right.Color.Rgb = new RGB(122, 212, 212);
            wb.ActiveWorksheet.Range("C6").Format.Borders.Right.Style = BorderStyle.mediumDashDotDot;
            wb.ActiveWorksheet.Range("B5:B7").Format.Borders.Diagonal.Style = BorderStyle.doubleLine;
            wb.ActiveWorksheet.Range("B5:B7").Format.Borders.Diagonal.DiagonalType = DiagonalType.Both;
            wb.ActiveWorksheet.Range("C7").Format.Borders.Outline = true;
            Assert.Equals(DiagonalType.Both, wb.ActiveWorksheet.Range("B6").Format.Borders.Diagonal.DiagonalType);

            wb.Save("../../TestFiles/Format_Borders_result.xlsx");
        }

        [Test(Categories = "PatternFill")]
        public void TestPatternFill()
        {
            Assert.Equals(PatternType.solid, wb.ActiveWorksheet.Range("E2:F2").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, wb.ActiveWorksheet.Range("E3:F3").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.lightGrid, wb.ActiveWorksheet.Range("E4:F4").Format.Fills.PatternFill.Pattern);
            Assert.Null(wb.ActiveWorksheet.Range("B4:F4").Format.Fills.PatternFill.Pattern);
            Assert.Equals("98F0EC", wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.BgColor.ToString());
            Assert.Equals("CC0066", wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.FgColor.ToString());

            wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.BgColor.Rgb = new RGB(0x11, 0x55, 0x88);
            wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.FgColor.Rgb = new RGB(0xEE, 0x99, 0x77);
            Assert.Equals("115588", wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.BgColor.ToString());
            Assert.Equals("EE9977", wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.FgColor.ToString());
            wb.ActiveWorksheet.Range("F4").Format.Fills.PatternFill.Pattern = PatternType.gray0625;
            wb.ActiveWorksheet.Range("B3").Format.Fills.PatternFill.Pattern = PatternType.darkHorizontal;

            wb.Save("../../TestFiles/Format_PatternFill_result.xlsx");
        }

        [Test(Categories = "GradientFill")]
        public void TestGradientFill()
        {
            Assert.Equals(GradientType.path, wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Type);
            wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Type = GradientType.linear;
            wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Color(1).Rgb = new RGB(0x00, 00, 255);
            wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Color(0.5).Rgb = new RGB(255, 255, 55);
            Assert.True(wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Color(0.3).HasDifferentValues());
            Assert.Equals("FFFF37", wb.ActiveWorksheet.Range("F7").Format.Fills.GradientFill.Color(0.5).ToString());
            wb.Save("../../TestFiles/Format_GradientFill_result.xlsx");
        }

        [Test]
        public void TestAutoColorSetAsIndex()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Auto_color_as_index.xls");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Null(ws.Range("A3").Format.Borders.Top.Color.Rgb);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
        }

        [Test]
        public void TestNumberFormatSetGet()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/NumberFormat.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Less(163, ws.Range("C2").Format.NumberFormatId);
            Assert.Equals(10, ws.Range("B5").Format.NumberFormatId);
            Assert.Equals(14, ws.Range("B7").Format.NumberFormatId);

            ws.Range("C1").Format.NumberFormatId = 9;
            Assert.Equals(9, ws.Range("C1").Format.NumberFormatId);
            ws.Range("B1").Format.NumberFormatId = 164;
            Assert.Equals(164, ws.Range("B1").Format.NumberFormatId);
            ws.Range("B1").Format.NumberFormatId = 300;
            Assert.Equals(164, ws.Range("B1").Format.NumberFormatId);

            ws.Range("B2").Format.NumberFormatCode = "0.00-00";
            Assert.Less(163, ws.Range("B2").Format.NumberFormatId);
            Assert.Equals("0.00-00", ws.Range("B2").Format.NumberFormatCode);
            ws.Range("B3").Format.NumberFormatCode = "0.00-00";
            Assert.Equals(ws.Range("B2").Format.NumberFormatId, ws.Range("B3").Format.NumberFormatId);
            Assert.Equals(ws.Range("B2").Format.NumberFormatId, ws.Range("B3").Format.NumberFormatId);
            Assert.Equals(ws.Range("B2").Format.NumberFormatCode, ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.NumberFormatCode = "0.00";
            Assert.Equals(2, ws.Range("B3").Format.NumberFormatId);
            ws.Range("B3").Format.NumberFormatId = 40;
            Assert.Equals("#,##0.00;[Red](#,##0.00)", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatCode = "\"$ \"0.00";
            Assert.Equals("\"$ \"0.00", ws.Range("B3").Format.NumberFormatCode);

            listener.Reset();
            ws.Range("A1:D3").Format.NumberFormatId = 0;
            Assert.Equals(12, listener.UpdatedCellsList.Count);


            wb.Save("../../TestFiles/NumberFormat_result.xlsx");

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestNumberFormatApply()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/NumberFormat.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Equals("0.00", ws.Range("B2").DispValue);
            Assert.Equals("123.00", ws.Range("C2").DispValue);
            Assert.Equals("1,234,560,000,000.00", ws.Range("D2").DispValue);
            Assert.Equals("123.46", ws.Range("E2").DispValue);
            Assert.Equals("(25.78)", ws.Range("F2").DispValue);
            Assert.Equals("(.26)", ws.Range("G2").DispValue);
            Assert.Equals(".26", ws.Range("H2").DispValue);
            Assert.Equals("sales Text", ws.Range("I2").DispValue);

            Assert.Equals("-", ws.Range("B3").DispValue);
            Assert.Equals("123.00", ws.Range("C3").DispValue);
            Assert.Equals("1,234,560,000,000.00", ws.Range("D3").DispValue);
            Assert.Equals("123.46", ws.Range("E3").DispValue);
            Assert.Equals("-25.78", ws.Range("F3").DispValue);
            Assert.Equals("-0.26", ws.Range("G3").DispValue);
            Assert.Equals("0.26", ws.Range("H3").DispValue);
            Assert.Equals("Text", ws.Range("I3").DispValue);

            Assert.Equals("0.00000E+00", ws.Range("B4").DispValue);
            Assert.Equals("1.23000E+02", ws.Range("C4").DispValue);
            Assert.Equals("1.23456E+12", ws.Range("D4").DispValue);
            Assert.Equals("1.23456E+02", ws.Range("E4").DispValue);
            Assert.Equals("-2.57800E+01", ws.Range("F4").DispValue);
            Assert.Equals("-2.56000E-01", ws.Range("G4").DispValue);
            Assert.Equals("2.56000E-01", ws.Range("H4").DispValue);
            Assert.Equals("Text", ws.Range("I4").DispValue);

            Assert.Equals("0.00%", ws.Range("B5").DispValue);
            Assert.Equals("12300.00%", ws.Range("C5").DispValue);
            Assert.Equals("123456000000000.00%", ws.Range("D5").DispValue);
            Assert.Equals("12345.60%", ws.Range("E5").DispValue);
            Assert.Equals("-2578.00%", ws.Range("F5").DispValue);
            Assert.Equals("-25.60%", ws.Range("G5").DispValue);
            Assert.Equals("25.60%", ws.Range("H5").DispValue);
            Assert.Equals("Text", ws.Range("I5").DispValue);

            Assert.Equals("0-0", ws.Range("B6").DispValue);
            Assert.Equals("0-0", ws.Range("C6").DispValue);
            Assert.Equals("123456-0", ws.Range("D6").DispValue);
            Assert.Equals("0-0", ws.Range("E6").DispValue);
            Assert.Equals("0-0", ws.Range("F6").DispValue);
            Assert.Equals("0-0", ws.Range("G6").DispValue);
            Assert.Equals("0-0", ws.Range("H6").DispValue);
            Assert.Equals("Text", ws.Range("I6").DispValue);

            Assert.Equals("03-29-10", ws.Range("B7").DispValue);
            Assert.Equals("05-02-00", ws.Range("C7").DispValue);
            Assert.Equals("#####", ws.Range("D7").DispValue);
            Assert.Equals("05-02-00", ws.Range("E7").DispValue);
            Assert.Equals("#####", ws.Range("F7").DispValue);
            Assert.Equals("#####", ws.Range("G7").DispValue);
            Assert.Equals("12-30-99", ws.Range("H7").DispValue);
            Assert.Equals("Text", ws.Range("I7").DispValue);

            Assert.Equals("29 March 2010 г.", ws.Range("B8").DispValue);

            Assert.Equals("2010------03---50---48---29", ws.Range("B9").DispValue);

            Assert.Equals("2010------03---50---48.456---29", ws.Range("B10").DispValue);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestIncreaseDecreaseDecimal()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/NumberFormat.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);


            ws.Range("B3").Format.NumberFormatId = 2;
            Assert.Equals("0.00", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("0.0", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("0", ws.Range("B3").Format.NumberFormatCode);
            Assert.Equals(1, ws.Range("B3").Format.NumberFormatId);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("0", ws.Range("B3").Format.NumberFormatCode);
            Assert.Equals(1, ws.Range("B3").Format.NumberFormatId);

            ws.Range("B3").Format.NumberFormatCode = "--###--";
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("--###--", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("--###.0--", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("--###.00--", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("--###.0--", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("--###--", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("--###--", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatId = 37;
            ws.Range("B3").Format.IncreaseDecimal();
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("#,##0.00 ;(#,##0.00)", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatId = 47;
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals(47, ws.Range("B3").Format.NumberFormatId);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals(47, ws.Range("B3").Format.NumberFormatId);

            ws.Range("B3").Format.NumberFormatId = 10;
            ws.Range("B3").Format.DecreaseDecimal();
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals(9, ws.Range("B3").Format.NumberFormatId);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestIncreaseDecreaseDecimalFromGeneral()
        {
            Workbook wb = editor.CreateWorkbook("Красная Заря");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B3").Value = 64.444;
            Assert.Equals(0, ws.Range("B3").Format.NumberFormatId);
            Assert.Equals("64.444", ws.Range("B3").DispValue);

            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("64.44", ws.Range("B3").DispValue);
            Assert.Equals("0.00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("64.4440", ws.Range("B3").DispValue);
            Assert.Equals("0.0000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 64.4440000059;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("64.44400001", ws.Range("B3").DispValue);

            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("64.4440000", ws.Range("B3").DispValue);
            Assert.Equals("0.0000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 4.44400000099;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("4.4440000010", ws.Range("B3").DispValue);
            Assert.Equals("0.0000000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = -64.4440000059;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("-64.44400001", ws.Range("B3").DispValue);

            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("-64.4440000", ws.Range("B3").DispValue);
            Assert.Equals("0.0000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("-64.444000006", ws.Range("B3").DispValue);
            Assert.Equals("0.000000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 595656;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("595656", ws.Range("B3").DispValue);

            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("595656", ws.Range("B3").DispValue);
            Assert.Equals("0", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("595656.0", ws.Range("B3").DispValue);
            Assert.Equals("0.0", ws.Range("B3").Format.NumberFormatCode);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestIncreaseDecreaseDecimalFromGeneralBigSmall()
        {
            Workbook wb = editor.CreateWorkbook("TestIncreaseDecreaseDecimalFromGeneralBigSmall");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B3").Value = 0.0001234567891123456;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("0.0001234568", ws.Range("B3").DispValue);
            Assert.Equals("0.0000000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 0.000123;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("0.00012", ws.Range("B3").DispValue);
            Assert.Equals("0.00000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 0.0001234567891123456;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("0.00012346", ws.Range("B3").DispValue);
            Assert.Equals("0.00000000", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 0.0000123456789112345;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("1.23457E-05", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("1.234568E-05", ws.Range("B3").DispValue);
            Assert.Equals("0.000000E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 9.234567891123E-45;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("9.23457E-45", ws.Range("B3").DispValue);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("9.2346E-45", ws.Range("B3").DispValue);
            Assert.Equals("0.0000E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 9.234567891123E-145;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("9.2346E-145", ws.Range("B3").DispValue);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("9.235E-145", ws.Range("B3").DispValue);
            Assert.Equals("0.000E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 9.23E-145;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("9.23E-145", ws.Range("B3").DispValue);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("9.2E-145", ws.Range("B3").DispValue);
            Assert.Equals("0.0E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 123456789112345.0;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("1.23457E+14", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("1.234568E+14", ws.Range("B3").DispValue);
            Assert.Equals("0.000000E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 100000000000005.0;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("1E+14", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("1.0E+14", ws.Range("B3").DispValue);
            Assert.Equals("0.0E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 9.23E145;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("9.23E+145", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("9.230E+145", ws.Range("B3").DispValue);
            Assert.Equals("0.000E+00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 100000000.25;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("100000000.3", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("100000000.25", ws.Range("B3").DispValue);
            Assert.Equals("0.00", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 1000000000.25;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("1000000000", ws.Range("B3").DispValue);
            ws.Range("B3").Format.DecreaseDecimal();
            Assert.Equals("1000000000", ws.Range("B3").DispValue);
            Assert.Equals("0", ws.Range("B3").Format.NumberFormatCode);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("1000000000.3", ws.Range("B3").DispValue);
            Assert.Equals("0.0", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 10000000000.25;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("10000000000", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("10000000000.3", ws.Range("B3").DispValue);
            Assert.Equals("0.0", ws.Range("B3").Format.NumberFormatCode);

            ws.Range("B3").Value = 100000000000.25;
            ws.Range("B3").Format.NumberFormatId = 0; // General
            Assert.Equals("1E+11", ws.Range("B3").DispValue);
            ws.Range("B3").Format.IncreaseDecimal();
            Assert.Equals("1.0E+11", ws.Range("B3").DispValue);
            Assert.Equals("0.0E+00", ws.Range("B3").Format.NumberFormatCode);




            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestGeneralApply()
        {
            Workbook wb = editor.CreateWorkbook("temp");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B3").Value = 64.444555666777111;
            Assert.Equals(0, ws.Range("B3").Format.NumberFormatId);
            Assert.Equals("64.44455567", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 164.4555000222111;
            Assert.Equals("164.4555", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 64.4555000222111;
            Assert.Equals("64.45550002", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 64.444555007;
            Assert.Equals("64.44455501", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8.41245678912E+50;
            Assert.Equals("8.41246E+50", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8.41245678912E+98;
            Assert.Equals("8.41246E+98", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8.41245678912E+99;
            Assert.Equals("8.4125E+99", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8.41245678912E+250;
            Assert.Equals("8.4125E+250", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8234567890;
            Assert.Equals("8234567890", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 82345678901;
            Assert.Equals("82345678901", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 823456789012;
            Assert.Equals("8.23457E+11", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8234;
            Assert.Equals("8234", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 8234.567890123456;
            Assert.Equals("8234.56789", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 0.112345678911234;
            Assert.Equals("0.112345679", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 0.000123456789;
            Assert.Equals("0.000123457", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 0.0000123456789;
            Assert.Equals("1.23457E-05", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 1.234567891E-99;
            Assert.Equals("1.2346E-99", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 1.234567891E-98;
            Assert.Equals("1.23457E-98", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 7.234567891E-300;
            Assert.Equals("7.2346E-300", ws.Range("B3").DispValue);

            ws.Range("B3").Value = 0;
            Assert.Equals("0", ws.Range("B3").DispValue);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestLoadThickBoxBug()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/thick_box.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            Assert.Equals(BorderStyle.medium, ws.Range("B2").Format.Borders.Left.Style);
            Assert.Equals(BorderStyle.none, ws.Range("B2").Format.Borders.Right.Style);

            Assert.Equals(BorderStyle.medium, ws.Range("C2").Format.Borders.Top.Style);
            Assert.Equals(BorderStyle.medium, ws.Range("C2").Format.Borders.Bottom.Style);

            Assert.Equals(BorderStyle.thin, ws.Range("E2").Format.Borders.Top.Style);
            Assert.Equals(BorderStyle.thin, ws.Range("E2").Format.Borders.Right.Style);

            Assert.Null(err_listener.LastError);
        }

        [Test]
        public void TestYellowWholeColumnAnd20CellsGet()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/thick_box.xlsx");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            System.Console.WriteLine("C:C = Yellow started...");
            DateTime before_set = DateTime.Now;

            ws.Range("C:F").Format.Fills.PatternFill.FgColor.Rgb = new RGB(255, 255, 0);
            ws.Range("C:F").Format.Fills.PatternFill.Pattern = PatternType.solid;

            DateTime after_set = DateTime.Now;
            TimeSpan delta_set = new TimeSpan(after_set.Ticks - before_set.Ticks);
            System.Console.WriteLine("C:C = Yellow lasted " + delta_set.TotalSeconds + " seconds");

            DateTime before_get_colors = DateTime.Now;

            Assert.Equals(new RGB(255, 255, 0), ws.Range("C923450").Format.Fills.PatternFill.FgColor.Rgb);
            Assert.Equals(new RGB(255, 255, 0), ws.Range("C923450:f999999").Format.Fills.PatternFill.FgColor.Rgb);

            DateTime after_get_colors = DateTime.Now;
            TimeSpan delta_get_colors = new TimeSpan(after_get_colors.Ticks - before_get_colors.Ticks);
            System.Console.WriteLine("Colors obtained " + delta_get_colors.TotalSeconds + " seconds");

            DateTime before_get_pattern = DateTime.Now;

            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);
            Assert.Equals(PatternType.solid, ws.Range("C923450").Format.Fills.PatternFill.Pattern);

            DateTime after_get_pattern = DateTime.Now;
            TimeSpan delta_get_pattern = new TimeSpan(after_get_pattern.Ticks - before_get_pattern.Ticks);
            System.Console.WriteLine("Patterns obtained " + delta_get_pattern.TotalSeconds + " seconds");


            Assert.Null(err_listener.LastError);
        }

        private EWSEditor editor;
        private Workbook wb;
        private Worksheet ws;
   }
}
