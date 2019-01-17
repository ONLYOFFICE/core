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
    [TestFixture(Categories = "Range")]
    public class RangeTest
    {
        public RangeTest()
        {
            editor = new EWSEditor();
        }

        [SetUp]
        public void SetUp()
        {
            editor.SetupLog("unittests.log");
            editor.CreateWorkbook("Book1", 5);
            empty_sheet = editor.ActiveWorksheet;
            Assert.NotNull(editor.LoadWorkbook("../../TestFiles/DataOfVarTypes.xlsx"));
            var_data_sheet = editor.ActiveWorksheet;
        }

        [TearDown]
        public void TearDown()
        {
            editor.Workbooks["Book1"].Close();
            editor.Workbooks["DataOfVarTypes"].Close();
            editor.StopLogging();
            empty_sheet = null;
            var_data_sheet = null;
        }

        [Test]
        public void TestRangeRef()
        {
            Range r = var_data_sheet.Range("F5");
            object value = r.Value;
            Assert.Equals(10.5, value);
            value = var_data_sheet.Range("$F5").Value;
            Assert.Equals(10.5, value);
            value = var_data_sheet.Range("$F$5").Value;
            Assert.Equals(10.5, value);
            value = var_data_sheet.Range("F$5").Value;
            Assert.Equals(10.5, value);

            Assert.True(var_data_sheet.Range("F$5").IsSingleCell);
            Assert.True(var_data_sheet.Range("F$5:f5").IsSingleCell);
            Assert.False(var_data_sheet.Range("F$5:f6").IsSingleCell);
            Assert.False(var_data_sheet.Range("$F:f").IsSingleCell);
            Assert.False(var_data_sheet.Range("A1:FF55").IsSingleCell);

            value = var_data_sheet.Range("A2:G2").Value;
            Assert.Equals(true, value);
            value = var_data_sheet.Range("$A2:G$2").Value;
            Assert.Equals(true, value);
            value = var_data_sheet.Range("A$2:$G2").Value;
            Assert.Equals(true, value);
            value = var_data_sheet.Range("$A$2:$G$2").Value;
            Assert.Equals(true, value);
            value = var_data_sheet.Range("2:2").Value;
            Assert.Equals(null, value);
            value = var_data_sheet.Range("A:AA").Value;
            Assert.Equals(null, value);

            ErrorListener err_listener = new ErrorListener(editor);

            value = var_data_sheet.Range("A:2").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range("2:A2").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range("A2-A2").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range("A2:").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range("Плохая_ссылка:#REF!").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range(":#REF!").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();

            value = var_data_sheet.Range(":A2").Value;
            Assert.True(err_listener.LastError is AVSOfficeEWSEditor.Editor.Events.WrongReferenceFormatError);
            err_listener.Reset();
        }

        [Test]
        public void TestValueTypes()
        {
            object value;
            value = var_data_sheet.Range("D1").Value;
            Assert.True(value is double);
            Assert.Equals(4, value);
            value = var_data_sheet.Range("D2").Value;
            Assert.True(value is bool);
            Assert.Equals(true, value);
            value = var_data_sheet.Range("D3").Value; // Shared string
            Assert.True(value is string);
            Assert.Equals(value, "Дерево18");
            value = var_data_sheet.Range("D4").Value;
            Assert.True(value is ErrorValue);
            Assert.Equals(ErrorValue.bad_reference, value);
            Assert.Equals("#REF!", value.ToString());
            value = var_data_sheet.Range("D5").Value;
            Assert.True(value is double);
            Assert.Equals(8.5, value);
            value = var_data_sheet.Range("D6").Value;
            Assert.True(value is double);
            Assert.Equals(0.57361111111111118, value);
            value = var_data_sheet.Range("D7").Value;
            Assert.True(value is ErrorValue);
            Assert.Equals(ErrorValue.null_value, value);
            Assert.Equals("#NULL!", value.ToString());
            value = var_data_sheet.Range("A7").Value; // Inline string
            Assert.True(value is string);
            Assert.Equals("согласен", value);
        }

        [Test]
        public void TestValueGet()
        {
            object value;
            value = var_data_sheet.Range("B2:B2").Value;
            Assert.True(value is bool);
            Assert.Equals(true, value);
            value = var_data_sheet.Range("B2:B3").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("B2:C2").Value;
            Assert.True(value is bool);
            Assert.Equals(true, value);
            value = var_data_sheet.Range("B2:H2").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("B2:M2").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("I12:M22").Value;
            Assert.True(value is string);
            Assert.Equals("", value);
            value = var_data_sheet.Range("H2:H5").Value;
            Assert.True(value is double);
            Assert.Equals(8, value);
            value = var_data_sheet.Range("H2:H7").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("H6:H7").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("H6:H17").Value;
            Assert.Null(value);
            value = var_data_sheet.Range("H1:K5").Value;
            Assert.True(value is double);
            Assert.Equals(8, value);
            value = var_data_sheet.Range("I2:L3").Value;
            Assert.Null(value);
        }

        [Test]
        public void TestValueSet()
        {
            object value;

            var_data_sheet.Range("B2:B2").Value = false; // set to single cell
            value = var_data_sheet.Range("B2:B2").Value;
            Assert.True(value is bool);
            Assert.Equals(false, value);
            var_data_sheet.Range("B2:C3").Value = false; // set to range
            value = var_data_sheet.Range("B2:B3").Value;
            Assert.True(value is bool);
            Assert.Equals(false, value);
            value = var_data_sheet.Range("B2:D2").Value;
            Assert.Null(value);
            var_data_sheet.Range("B2:M2").Value = false; // create additional cells
            value = var_data_sheet.Range("B2:M2").Value;
            Assert.True(value is bool);
            Assert.Equals(false, value);
            value = var_data_sheet.Range("B1:M2").Value;
            Assert.Null(value);

            var_data_sheet.Range("I12:M22").Value = 5.5; // create all new cells
            value = var_data_sheet.Range("I12:M22").Value;
            Assert.True(value is double);
            Assert.Equals(5.5, value);
            value = var_data_sheet.Range("J18:L20").Value;
            Assert.True(value is double);
            Assert.Equals(5.5, value);

            var_data_sheet.Range("H2:H5").Value = "string"; // set string type
            value = var_data_sheet.Range("H2:H5").Value;
            Assert.True(value is string);
            Assert.Equals("string", value);
            Assert.Equals(var_data_sheet.Range("H3").Value, var_data_sheet.Range("H4").Value);
            
            var_data_sheet.Range("H2:H3").Value = 8; // change type from string to numeric
            value = var_data_sheet.Range("H3:K3").Value;
            Assert.True(value is double);
            Assert.Equals(8, value);

            var_data_sheet.Range("C7:E7").Value = null; // setting C7:E7 to null is OK
            var_data_sheet.Range("B7").Value = "";
            Assert.Equals("", var_data_sheet.Range("D7").Value); // should be changed - OK
            Assert.Equals("", var_data_sheet.Range("B7").Value); // should be changed - OK


            var_data_sheet.Range("I1").Value = 5;
            var_data_sheet.Range("F1:G1").Value = "sdffg";
            Assert.NotEquals("sdffg", var_data_sheet.Range("I1").Value.ToString()); // I don't remember what is checked here
            var_data_sheet.Range("F1:I1").Value = 5;
            System.Console.WriteLine("Two string values removed (F1:I1) check sst/@count manually");

            var_data_sheet.Range("A1").Value = "43e'";
            Assert.Equals("43e'", var_data_sheet.Range("A1").Value); // Check that quotation is set and obtained OK

        }

        [Test]
        public void TestExistentCells()
        {
            var_data_sheet.Range("C7:E7").Value = null; // setting C7:E7 to null is OK
            var_data_sheet.Range("B7").Value = "";
            // The above shall be counted too
            List<string> cells_set1 = var_data_sheet.Range("B2:B2").ExistentCells();
            Assert.Equals(1, cells_set1.Count);
            List<string> cells_set2 = var_data_sheet.Range("C6:D9").ExistentCells();
            Assert.Equals(4, cells_set2.Count);
            Assert.Equals("C6", cells_set2[0]);
            Assert.Equals("D6", cells_set2[1]);
            Assert.Equals("C7", cells_set2[2]);
            Assert.Equals("D7", cells_set2[3]);
            List<string> cells_set3 = var_data_sheet.Range("H5:I7").ExistentCells();
            Assert.Equals(3, cells_set3.Count);
            Assert.Equals("H5", cells_set3[0]);
            Assert.Equals("I5", cells_set3[1]);
            Assert.Equals("H7", cells_set3[2]);
            List<string> cells_set4 = var_data_sheet.Range("9:200").ExistentCells();
            Assert.Equals(0, cells_set4.Count);
            List<string> cells_set5 = var_data_sheet.Cells.ExistentCells();
            Assert.Equals(70, cells_set5.Count);
            Assert.Equals("H7", cells_set5[69]);
            Assert.Equals("G3", cells_set5[28]);
        }

        [Test(Categories = "TestNonEmptyCells")]
        public void TestNonEmptyCells()
        {
            var_data_sheet.Range("C7:E7").Value = null; // setting C7:E7 to null is OK
            var_data_sheet.Range("B7").Value = "";
            // The above shall not be counted
            List<string> cells_set1 = var_data_sheet.Range("B2:B2").NonEmptyCells();
            Assert.Equals(1, cells_set1.Count);
            List<string> cells_set2 = var_data_sheet.Range("C6:D9").NonEmptyCells();
            Assert.Equals(2, cells_set2.Count);
            Assert.Equals("C6", cells_set2[0]);
            Assert.Equals("D6", cells_set2[1]);
            List<string> cells_set3 = var_data_sheet.Range("H5:I7").NonEmptyCells();
            Assert.Equals(3, cells_set3.Count);
            Assert.Equals("H5", cells_set3[0]);
            Assert.Equals("I5", cells_set3[1]);
            Assert.Equals("H7", cells_set3[2]);
            List<string> cells_set4 = var_data_sheet.Range("9:200").NonEmptyCells();
            Assert.Equals(0, cells_set4.Count);
            List<string> cells_set5 = var_data_sheet.Cells.NonEmptyCells();
            Assert.Equals(66, cells_set5.Count);
            Assert.Equals("H7", cells_set5[65]);
            Assert.Equals("G3", cells_set5[28]);
        }

        private EWSEditor editor;
        private Worksheet empty_sheet;
        private Worksheet var_data_sheet;
    }
}
