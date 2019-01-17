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
    [TestFixture(Categories = "Formulas, FormulasFunctions")]
    public class FormulasFunctionsTest
    {
        public FormulasFunctionsTest()
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
        public void Test_SUM()
        {
            Workbook wb = editor.CreateWorkbook("SUM");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = 1;
            ws.Range("F12").Value = 5;
            ws.Range("B12").Formula = "SUM(B8)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "SUM(B8,5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(10, ws.Range("B12").Value);

            ws.Range("B12").Formula = "SUM(B8,\"1\",D6, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(15, ws.Range("B12").Value);

            ws.Range("B12").Formula = "SUM((B8), SUM(\"1\",D6), F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(15, ws.Range("B12").Value);

            ws.Range("B12").Formula = "SUM(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "SUM(1, TRUE, \"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(32, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "SUM($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A2").Value);

            ws.Range("A2").Formula = "SUM($B$2,+C2,D2,ZZ34)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(2, ws.Range("A2").Value);

            ws.Range("A2").Formula = "SUM(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "SUM(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("D10").Formula = "SUM(B6:E9,B2,\"3\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(14, ws.Range("D10").Value);

            ws.Range("D8").Value = "qweqwe";
            ws.Range("D9").Formula = "300";
            ws.Range("C10").Formula = "SUM(D:D)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(618, ws.Range("C10").Value);
            
            wb.Close();
        }

        [Test]
        public void Test_COUNT()
        {
            Workbook wb = editor.CreateWorkbook("COUNT");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = 1;
            ws.Range("F12").Value = 5;
            ws.Range("B12").Formula = "COUNT(B8)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT(B8,5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(2, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT(B8,\"1\",D6, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT((B8), COUNT(\"1\",D6), F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(4, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(14, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT(1, TRUE, \"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3, ws.Range("B12").Value);

            ws.Range("B12").Formula = "COUNT(1, TRUE, \"d30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(2, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "COUNT($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3, ws.Range("A2").Value);

            ws.Range("A2").Formula = "COUNT($B$2,+C2,D2,ZZ34)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(2, ws.Range("A2").Value);

            ws.Range("A2").Formula = "COUNT(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "COUNT(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A4").Value);

            wb.Close();
        }

        [Test]
        public void Test_AVERAGE()
        {
            Workbook wb = editor.CreateWorkbook("AVERAGE");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = 1;
            ws.Range("F12").Value = 5;
            ws.Range("B12").Formula = "AVERAGE(B8)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AVERAGE(B8,5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AVERAGE(B8,\"1\",D6, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AVERAGE((B8), AVERAGE(\"1\",D6), F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3.125, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AVERAGE(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AVERAGE(1, TRUE, \"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(10 + 2.0 / 3, (double)ws.Range("B12").Value, 1e-13);

            ws.Range("B12").Formula = "AVERAGE(1, TRUE, \"d30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "AVERAGE($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1.0 / 3, (double)ws.Range("A2").Value, 1e-13);

            ws.Range("A2").Formula = "AVERAGE($B$2,+C2,D2,ZZ34)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A2").Value);

            ws.Range("A2").Formula = "AVERAGE(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "AVERAGE(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("A4").Formula = "AVERAGE(D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A4").Value);

            wb.Close();
        }

        [Test]
        public void Test_MIN()
        {
            Workbook wb = editor.CreateWorkbook("MIN");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = 1;
            ws.Range("F12").Value = 5;
            ws.Range("B12").Formula = "MIN(B8)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MIN(B8,4)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(4, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MIN(B8,\"1\",D6, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MIN((B8), MIN(\"1\",D6), F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MIN(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MIN(1, TRUE, \"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, (double)ws.Range("B12").Value, 1e-13);

            ws.Range("B12").Formula = "MIN(1, TRUE, \"d30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "MIN($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("A2").Value);

            ws.Range("A2").Formula = "MIN($B$2,+C2,D2,ZZ34)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A2").Value);

            ws.Range("A2").Formula = "MIN(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "MIN(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "MIN(D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("A4").Value);

            wb.Close();
        }

        [Test]
        public void Test_MAX()
        {
            Workbook wb = editor.CreateWorkbook("MAX");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = 1;
            ws.Range("F12").Value = 5;
            ws.Range("B12").Formula = "MAX(B8)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MAX(B8,4)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MAX(B8,\"1\",D6, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MAX((B8), MAX(\"1\",D6), F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(5, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MAX(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("B12").Value);

            ws.Range("B12").Formula = "MAX(1, TRUE, \"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(30, (double)ws.Range("B12").Value, 1e-13);

            ws.Range("B12").Formula = "MAX(1, TRUE, \"d30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "MAX($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, ws.Range("A2").Value);

            ws.Range("A2").Formula = "MAX($B$2,+C2,1+D2,ZZ34)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(31, ws.Range("A2").Value);

            ws.Range("A2").Formula = "MAX(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "MAX(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("A4").Formula = "MAX(D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(0, ws.Range("A4").Value);

            wb.Close();
        }

        [Test]
        public void Test_SIN()
        {
            Workbook wb = editor.CreateWorkbook("SIN");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = true;
            ws.Range("E1").Formula = "\"30\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "SIN(5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(5), (double)ws.Range("B1").Value, 1e-13);

            ws.Range("B2").Formula = "SIN(TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(1), (double)ws.Range("B2").Value, 1e-13);

            ws.Range("B3").Formula = "SIN(\"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(30), (double)ws.Range("B3").Value, 1e-13);

            ws.Range("B4").Formula = "SIN(#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);

            ws.Range("B5").Formula = "SIN(C1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(1), (double)ws.Range("B5").Value, 1e-13);

            ws.Range("B6").Formula = "SIN(D1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(1), (double)ws.Range("B6").Value, 1e-13);

            ws.Range("B7").Formula = "SIN(E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(30), (double)ws.Range("B7").Value, 1e-13);

            ws.Range("B8").Formula = "SIN(F1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("B8").Value);

            ws.Range("B9").Formula = "SIN(G1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B9").Value);

/* Until PtgArea is implemented
            ws.Range("B10").Formula = "SIN(D1:E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B10").Value);
*/

            ws.Range("B11").Formula = "SIN(\"D1-E1\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B11").Value);

            ws.Range("B12").Formula = "SIN(C12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sin(0), ws.Range("B12").Value);

            ws.Range("B13").Formula = "SIN()";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "SIN(1,2)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "SIN(,)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();


            wb.Close();
        }

        [Test]
        public void Test_COS()
        {
            Workbook wb = editor.CreateWorkbook("COS");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = true;
            ws.Range("E1").Formula = "\"30\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "COS(5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(5), (double)ws.Range("B1").Value, 1e-13);

            ws.Range("B2").Formula = "COS(TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(1), (double)ws.Range("B2").Value, 1e-13);

            ws.Range("B3").Formula = "COS(\"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(30), (double)ws.Range("B3").Value, 1e-13);

            ws.Range("B4").Formula = "COS(#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);

            ws.Range("B5").Formula = "COS(C1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(1), (double)ws.Range("B5").Value, 1e-13);

            ws.Range("B6").Formula = "COS(D1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(1), (double)ws.Range("B6").Value, 1e-13);

            ws.Range("B7").Formula = "COS(E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(30), (double)ws.Range("B7").Value, 1e-13);

            ws.Range("B8").Formula = "COS(F1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("B8").Value);

            ws.Range("B9").Formula = "COS(G1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B9").Value);

/* Until PtgArea is implemented
            ws.Range("B10").Formula = "COS(D1:E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B10").Value);
*/

            ws.Range("B11").Formula = "COS(\"D1-E1\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B11").Value);

            ws.Range("B12").Formula = "COS(C12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(0), ws.Range("B12").Value);

            ws.Range("B13").Formula = "COS()";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "COS(1,2)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "COS(,)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();


            wb.Close();
        }

        [Test]
        public void Test_TAN()
        {
            Workbook wb = editor.CreateWorkbook("TAN");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = true;
            ws.Range("E1").Formula = "\"30\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "TAN(5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(5), (double)ws.Range("B1").Value, 1e-13);

            ws.Range("B2").Formula = "TAN(TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(1), (double)ws.Range("B2").Value, 1e-13);

            ws.Range("B3").Formula = "TAN(\"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(30), (double)ws.Range("B3").Value, 1e-13);

            ws.Range("B4").Formula = "TAN(#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);

            ws.Range("B5").Formula = "TAN(C1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(1), (double)ws.Range("B5").Value, 1e-13);

            ws.Range("B6").Formula = "TAN(D1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(1), (double)ws.Range("B6").Value, 1e-13);

            ws.Range("B7").Formula = "TAN(E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(30), (double)ws.Range("B7").Value, 1e-13);

            ws.Range("B8").Formula = "TAN(F1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("B8").Value);

            ws.Range("B9").Formula = "TAN(G1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B9").Value);

/* Until PtgArea is implemented
            ws.Range("B10").Formula = "TAN(D1:E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B10").Value);
*/

            ws.Range("B11").Formula = "TAN(\"D1-E1\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B11").Value);

            ws.Range("B12").Formula = "TAN(C12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(0), ws.Range("B12").Value);

            ws.Range("B13").Formula = "TAN()";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "TAN(1,2)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "TAN(,)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "TAN(1.57)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(1.57), (double)ws.Range("B13").Value, 1e-12);
            err_listener.Reset();

            wb.Close();
        }

        [Test]
        public void Test_PI()
        {
            Workbook wb = editor.CreateWorkbook("PI");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = true;
            ws.Range("E1").Formula = "\"30\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "PI()";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3.14159265358979, (double)ws.Range("B1").Value, 1e-13);

            ws.Range("B2").Formula = "PI(A1)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "TAN(PI()/2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Tan(3.14159265358979 / 2), (double)ws.Range("B13").Value, 1);
            err_listener.Reset();

            ws.Range("B13").Formula = "COS(PI())";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Cos(3.14159265358979), (double)ws.Range("B13").Value, 1e-12);
            err_listener.Reset();

            wb.Close();
        }

        [Test]
        public void Test_SQRT()
        {
            Workbook wb = editor.CreateWorkbook("SQRT");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = true;
            ws.Range("E1").Formula = "\"30\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "SQRT(5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(5), (double)ws.Range("B1").Value, 1e-13);

            ws.Range("B2").Formula = "SQRT(TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(1), (double)ws.Range("B2").Value, 1e-13);

            ws.Range("B3").Formula = "SQRT(\"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(30), (double)ws.Range("B3").Value, 1e-13);

            ws.Range("B4").Formula = "SQRT(#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);

            ws.Range("B5").Formula = "SQRT(C1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(1), (double)ws.Range("B5").Value, 1e-13);

            ws.Range("B6").Formula = "SQRT(D1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(1), (double)ws.Range("B6").Value, 1e-13);

            ws.Range("B7").Formula = "SQRT(E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(30), (double)ws.Range("B7").Value, 1e-13);

            ws.Range("B8").Formula = "SQRT(F1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("B8").Value);

            ws.Range("B9").Formula = "SQRT(G1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B9").Value);

            ws.Range("B10").Formula = "TAN(D1:E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B10").Value);

            ws.Range("B11").Formula = "SQRT(\"D1-E1\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B11").Value);

            ws.Range("B12").Formula = "SQRT(C12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(Math.Sqrt(0), ws.Range("B12").Value);

            ws.Range("B13").Formula = "SQRT()";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "SQRT(1,2)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "SQRT(,)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "SQRT(-1.57)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.not_numeric, ws.Range("B13").Value);
            err_listener.Reset();

            wb.Close();
        }

        [Test]
        public void Test_GetFunctionsList()
        {
            Workbook wb = editor.CreateWorkbook("GetFunctionsList");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            List<FunctionDescription> all_functions = editor.FunctionsStorage.GetFunctionsList();

            FunctionDescription desc_sum = all_functions.Find(delegate(FunctionDescription desc) { return (desc.FuncName == "SUM"); } );
            Assert.Equals("SUM", desc_sum.FuncName);

            FunctionDescription desc_count = all_functions.Find(delegate(FunctionDescription desc) { return (desc.FuncName == "COUNT"); });
            Assert.Equals("COUNT", desc_count.FuncName);

            FunctionDescription desc_sqrt = all_functions.Find(delegate(FunctionDescription desc) { return (desc.FuncName == "SQRT"); });
            Assert.Equals("SQRT", desc_sqrt.FuncName);

            FunctionDescription desc_pi = all_functions.Find(delegate(FunctionDescription desc) { return (desc.FuncName == "PI"); });
            Assert.Equals("PI", desc_pi.FuncName);

            wb.Close();
        }

        [Test]
        public void Test_NOT()
        {
            Workbook wb = editor.CreateWorkbook("NOT");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1").Value = 1;
            ws.Range("D1").Value = false;
            ws.Range("E1").Formula = "\"TRUE\"";
            ws.Range("F1").Formula = "#NULL!";
            ws.Range("G1").Value = "string";

            ws.Range("B1").Formula = "NOT(5)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B1").Value);

            ws.Range("B2").Formula = "NOT(TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B2").Value);

            ws.Range("B3").Formula = "NOT(\"30\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B3").Value);

            ws.Range("B4").Formula = "NOT(#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("B4").Value);

            ws.Range("B5").Formula = "NOT(C1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B5").Value);

            ws.Range("B6").Formula = "NOT(D1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B6").Value);

            ws.Range("B7").Formula = "NOT(E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B7").Value);

            ws.Range("B8").Formula = "NOT(F1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("B8").Value);

            ws.Range("B9").Formula = "NOT(G1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B9").Value);

            ws.Range("B10").Formula = "NOT(D1:E1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B10").Value);

            ws.Range("B11").Formula = "NOT(\"FALSE1\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B11").Value);

            ws.Range("B11").Formula = "NOT(\"FALSE\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B11").Value);

            ws.Range("B12").Formula = "NOT(C12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B13").Formula = "NOT()";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "NOT(1,2)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "NOT(,)";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            ws.Range("B13").Formula = "NOT(-1.57)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B13").Value);
            err_listener.Reset();

            wb.Close();
        }

        [Test]
        public void Test_AND()
        {
            Workbook wb = editor.CreateWorkbook("AND");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = true;
            ws.Range("F12").Formula = "\"FALSE\"";

            ws.Range("B12").Formula = "AND(B8, D6)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(B8,4)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(B8,\"1\",D6)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(B8, F12, A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(B8, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND((B8), MAX(\"1\",D6), NOT(F12))";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND((B8), MAX(\"1\",D6), F12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND((B8), MAX(\"1\",D6), \"FALSE\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(,,,,,,,,,,,,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(A1, A2, F12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(1, TRUE, \"TRUE\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "AND(1, TRUE, \"TRUE2000\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";
            ws.Range("A2").Formula = "AND($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("A2").Value);

            ws.Range("A2").Formula = "AND($B$2,+C2,1+D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("A2").Value);

            ws.Range("A2").Formula = "AND($B$2,C2-1,1+D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("A2").Value);

            ws.Range("A2").Formula = "AND(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "AND(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("A4").Formula = "AND(D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A4").Value);

            wb.Close();
        }

        [Test]
        public void Test_OR()
        {
            Workbook wb = editor.CreateWorkbook("OR");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = true;
            ws.Range("F12").Formula = "\"TRUE\"";

            ws.Range("B12").Formula = "OR(B8, D6)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(B8,4)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(B8,\"1\",D6)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(B8, F12, A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(B8, F12, , A1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR((B8), MAX(\"1\",D6), NOT(F12))";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR((B8), MAX(\"1\",D6), F12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR((B8), MAX(\"1\",D6), \"FALSE\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(,,,,5/2-2.5,,,0,,,,FALSE,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(A1, A2, F12)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(1, TRUE, \"TRUE\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(1, TRUE, \"TRUE2000\")";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B12").Value);

            ws.Range("B2").Value = 1;
            ws.Range("C2").Value = true;
            ws.Range("D2").Formula = "\"30\"";

            ws.Range("A2").Formula = "OR($B$2,C2,D2,,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("A2").Value);

            ws.Range("A2").Formula = "OR($B$2,+C2,1+D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("A2").Value);

            ws.Range("A2").Formula = "OR($B$2,C2-1,1+D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("A2").Value);

            ws.Range("A2").Formula = "OR(#REF!,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            ws.Range("B4").Value = ErrorValue.bad_reference;
            ws.Range("A4").Formula = "OR(B4,1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A4").Value);

            ws.Range("A4").Formula = "OR(D2)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A4").Value);

            ws.Range("B12").Formula = "OR(F$12,5/2-2.5,A1,\"FALSE\",0,)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);


            wb.Close();
        }

        [Test]
        public void Test_TRUE_FALSE()
        {
            Workbook wb = editor.CreateWorkbook("TRUE_FALSE");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B8").Value = 5;
            ws.Range("D6").Value = 4;
            ws.Range("E8").Value = true;
            ws.Range("F12").Formula = "\"TRUE\"";
            ws.Range("F13").Formula = "\"FALSE\"";

            ws.Range("B12").Formula = "AND(B8, D6, TRUE(), F12, TRUE)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(OR(TRUE(),FALSE, FALSE()),1-1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(true, ws.Range("B12").Value);

            ws.Range("B12").Formula = "OR(OR(FALSE,FALSE(),),OR(AND(FALSE(),TRUE()),FALSE,FALSE()))";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(false, ws.Range("B12").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        [Test]
        public void Test_IF()
        {
            Workbook wb = editor.CreateWorkbook("IF");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 9;
            ws.Range("B1").Value = 1;
            ws.Range("A2:A8").Formula = "A1-1";
            ws.Range("B2:B8").Formula = "B1+1";

            ws.Range("C1:C8").Formula = "IF(AND(B1>=4, MAX(A1,B1)<8),TRUE(),#REF!)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C8").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("C5").Value);
            Assert.Equals(true, ws.Range("C6").Value);
            Assert.Equals(true, ws.Range("C7").Value);

            ws.Range("A1").Formula = "IF(IF(5>6,\"B1\",),FALSE,TRUE)*5";
            Assert.Equals(5, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(IF(5<6,\"B1\",),FALSE,TRUE)*5";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(IF(5<6,B1,),FALSE,TRUE)*5";
            Assert.Equals(0, ws.Range("A1").Value);

            ws.Range("B1").Value = true;
            ws.Range("C1").Value = false;
            ws.Range("D1").Formula = "\"TRUE\"";
            ws.Range("E1").Formula = "\"ISTINA\"";
            ws.Range("A1").Formula = "IF(B1, 200, 400)";
            Assert.Equals(200, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(C1, 200, 400)";
            Assert.Equals(400, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(B1:C1, 200, 400)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(D1, 200, 400)";
            Assert.Equals(200, ws.Range("A1").Value);
            ws.Range("A1").Formula = "IF(E1, 200, 400)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A1").Value);

            wb.Close();
        }

        [Test]
        public void Test_IFERROR()
        {
            Workbook wb = editor.CreateWorkbook("IFERROR");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = 2;
            ws.Range("A3").Value = 0;
            ws.Range("A4").Formula = "\"\"";
            ws.Range("A6").Value = 4;
            ws.Range("A7").Formula = "\"4\"";
            ws.Range("A8").Value = ErrorValue.null_value;

            ws.Range("B1:B8").Formula = "IFERROR(3/A1,\"Error!!!\")";

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3, ws.Range("B1").Value);
            Assert.Equals(1.5, ws.Range("B2").Value);
            Assert.Equals("Error!!!", ws.Range("B3").Value);
            Assert.Equals("Error!!!", ws.Range("B4").Value);
            Assert.Equals("Error!!!", ws.Range("B5").Value);
            Assert.Equals(0.75, ws.Range("B6").Value);
            Assert.Equals(0.75, ws.Range("B7").Value);
            Assert.Equals("Error!!!", ws.Range("B8").Value);

            ws.Range("A1").Formula = "IFERROR(D8:F16,21)";
            Assert.Equals(21, ws.Range("A1").Value);

            ws.Range("A1").Formula = "IFERROR(A8,21)";
            Assert.Equals(21, ws.Range("A1").Value);

            ws.Range("A1").Formula = "IFERROR(,21)";
            Assert.Equals("", ws.Range("A1").Value);

            wb.Close();
        }

        [Test]
        public void Test_CONCATENATE()
        {
            Workbook wb = editor.CreateWorkbook("IFERROR");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = true;
            ws.Range("A3").Value = 0;
            ws.Range("A4").Formula = "\"\"";
            ws.Range("A5").Value = 4;

            ws.Range("B1").Value = ErrorValue.null_value;
            ws.Range("B2").Formula = "\"true\"";
            ws.Range("B3").Value = false;
            ws.Range("B4").Formula = "\"123\"";
            ws.Range("B5").Value = ErrorValue.null_value;

            ws.Range("C1:C5").Formula = "CONCATENATE(A1,B1)";

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(ErrorValue.null_value, ws.Range("C1").Value);
            Assert.Equals("TRUEtrue", ws.Range("C2").Value);
            Assert.Equals("0FALSE", ws.Range("C3").Value);
            Assert.Equals("123", ws.Range("C4").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("C5").Value);

            ws.Range("A1").Formula = "CONCATENATE(D8:F16,21)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A1").Value);

            ws.Range("A1").Formula = "CONCATENATE(A8,21)";
            Assert.Equals("21", ws.Range("A1").Value);

            ws.Range("A1").Formula = "CONCATENATE(,,,)";
            Assert.Equals("", ws.Range("A1").Value);

            wb.Close();
        }

        [Test]
        public void Test_POWER()
        {
            Workbook wb = editor.CreateWorkbook("POWER");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = true;
            ws.Range("A3").Value = 0;
            ws.Range("A4").Formula = "\"\"";
            ws.Range("A5").Value = ErrorValue.null_value;
            ws.Range("A6").Value = -3;
            ws.Range("A7").Value = 2;

            ws.Range("B1").Value = ErrorValue.null_value;
            ws.Range("B2").Formula = "\"true\"";
            ws.Range("B3").Value = false;
            ws.Range("B4").Formula = "\"123\"";
            ws.Range("B5").Value = 0.5;
            ws.Range("B6").Value = -1.3;
            ws.Range("B7").Value = 8;

            ws.Range("C1:C7").Formula = "POWER(A1,B1)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);

            Assert.Equals(ErrorValue.null_value, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.not_numeric, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("C5").Value);
            Assert.Equals(ErrorValue.not_numeric, ws.Range("C6").Value);
            Assert.Equals(256, ws.Range("C7").Value);

            ws.Range("A1").Formula = "POWER(D8:F16,21)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A1").Value);

            ws.Range("A1").Formula = "POWER(A8,21)";
            Assert.Equals(0, ws.Range("A1").Value);

            ws.Range("A1").Formula = "POWER(,21)";
            Assert.Equals(0, ws.Range("A1").Value);

            wb.Close();
        }

        [Test]
        public void Test_LOWER_UPPER()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 12;
            ws.Range("A2").Value = true;
            ws.Range("A3").Value = ErrorValue.null_value;
            ws.Range("A4").Formula = "\"Qwerty\"";

            ws.Range("C1:C5").Formula = "LOWER(A1)";
            ws.Range("C6").Formula = "LOWER(D8:R15)";

            Assert.Equals("12", ws.Range("C1").Value);
            Assert.Equals("true", ws.Range("C2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("C3").Value);
            Assert.Equals("qwerty", ws.Range("C4").Value);
            Assert.Equals("", ws.Range("C5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C6").Value);

            ws.Range("D1:D5").Formula = "UPPER(A1)";
            ws.Range("D6").Formula = "UPPER(D8:R15)";

            Assert.Equals("12", ws.Range("D1").Value);
            Assert.Equals("TRUE", ws.Range("D2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("D3").Value);
            Assert.Equals("QWERTY", ws.Range("D4").Value);
            Assert.Equals("", ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D6").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_T_N()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 12;
            ws.Range("A2").Value = true;
            ws.Range("A3").Value = ErrorValue.null_value;
            ws.Range("A4").Formula = "\"Qwerty\"";

            ws.Range("C1:C5").Formula = "T(A1)";
            ws.Range("C6").Formula = "T(D8:R15)";

            Assert.Equals("", ws.Range("C1").Value);
            Assert.Equals("", ws.Range("C2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("C3").Value);
            Assert.Equals("Qwerty", ws.Range("C4").Value);
            Assert.Equals("", ws.Range("C5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C6").Value);

            ws.Range("D1:D5").Formula = "N(A1)";
            ws.Range("D6").Formula = "N(D8:R15)";

            Assert.Equals(12, ws.Range("D1").Value);
            Assert.Equals(1, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("D3").Value);
            Assert.Equals(0, ws.Range("D4").Value);
            Assert.Equals(0, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D6").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_REPEAT()
        {
            Workbook wb = LoadTwoParamsAllCombinations("REPT");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

//            wb.Save("../../TestFiles/REPT_results.xlsx");

            Assert.Equals("121212121212121212121212", ws.Range("C1").Value);
            Assert.Equals("12", ws.Range("C2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals("", ws.Range("C5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C6").Value);
            Assert.Equals("TRUETRUETRUETRUETRUETRUETRUETRUETRUETRUETRUETRUE", ws.Range("C7").Value);
            Assert.Equals("TRUE", ws.Range("C8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C9").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C10").Value);
            Assert.Equals("", ws.Range("C11").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C12").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C13").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C14").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C15").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C16").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C17").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C18").Value);
            Assert.Equals("QwertyQwertyQwertyQwertyQwertyQwertyQwertyQwertyQwertyQwertyQwertyQwerty", ws.Range("C19").Value);
            Assert.Equals("Qwerty", ws.Range("C20").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C21").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C22").Value);
            Assert.Equals("", ws.Range("C23").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C24").Value);
            Assert.Equals("", ws.Range("C25").Value);
            Assert.Equals("", ws.Range("C26").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C27").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C28").Value);
            Assert.Equals("", ws.Range("C29").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C30").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C31").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C32").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C33").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C34").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C35").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C36").Value);

            ws.Range("C37").Formula = "REPT(REPT(REPT(REPT(\"1234567890\",10),10),10),3)";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            ws.Range("D37").Formula = "IFERROR(D37, 12)";
            Assert.NotEquals(12, ws.Range("D37").Value);

            ws.Range("C38").Formula = "REPT(REPT(REPT(REPT(\"1234567890\",10),10),10),4)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C38").Value);

            ws.Range("C38").Formula = "REPT(\"1234567890\",-10)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C38").Value);

            ws.Range("C38").Formula = "REPT(\"1234567890\",2.5)";
            Assert.Equals("12345678901234567890", ws.Range("C38").Value);

            ws.Range("C38").Formula = "REPT(\"1234567890\",-0.5)";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C38").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_EXACT()
        {
            Workbook wb = LoadTwoParamsAllCombinations("EXACT");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            wb.Save("../../TestFiles/EXACT_results.xlsx");

            Assert.Equals(true, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(false, ws.Range("C5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C6").Value);
            Assert.Equals(false, ws.Range("C7").Value);
            Assert.Equals(true, ws.Range("C8").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C9").Value);
            Assert.Equals(false, ws.Range("C10").Value);
            Assert.Equals(false, ws.Range("C11").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C12").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C13").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C14").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C15").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C16").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C17").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C18").Value);
            Assert.Equals(false, ws.Range("C19").Value);
            Assert.Equals(false, ws.Range("C20").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C21").Value);
            Assert.Equals(true, ws.Range("C22").Value);
            Assert.Equals(false, ws.Range("C23").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C24").Value);
            Assert.Equals(false, ws.Range("C25").Value);
            Assert.Equals(false, ws.Range("C26").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C27").Value);
            Assert.Equals(false, ws.Range("C28").Value);
            Assert.Equals(true, ws.Range("C29").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C30").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C31").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C32").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C33").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C34").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C35").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C36").Value);

            ws.Range("C37").Formula = "EXACT(\"1234567890\",1234567890)";
            Assert.Equals(true, ws.Range("C37").Value);

            ws.Range("C38").Formula = "EXACT(\"1234Q\",\"1234q\")";
            Assert.Equals(false, ws.Range("C38").Value);

            ws.Range("C39").Formula = "EXACT(\"-10\",-10)";
            Assert.Equals(true, ws.Range("C39").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_IS_functions()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "ISBLANK");

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(false, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(true, ws.Range("B5").Value);
            Assert.Equals(false, ws.Range("B6").Value);

            SetFunctionOneParamAllCombinations(ws, "ISERR");
            ws.Range("A7").Value = ErrorValue.not_available;

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(true, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(true, ws.Range("B6").Value);
            Assert.Equals(false, ws.Range("B7").Value);

            SetFunctionOneParamAllCombinations(ws, "ISERROR");

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(true, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(true, ws.Range("B6").Value);
            Assert.Equals(true, ws.Range("B7").Value);

            SetFunctionOneParamAllCombinations(ws, "ISLOGICAL");

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(true, ws.Range("B2").Value);
            Assert.Equals(false, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(false, ws.Range("B6").Value);

            SetFunctionOneParamAllCombinations(ws, "ISNA");

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(false, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(false, ws.Range("B6").Value);
            Assert.Equals(true, ws.Range("B7").Value);

            SetFunctionOneParamAllCombinations(ws, "ISNONTEXT");
            ws.Range("B7").Formula = "ISNONTEXT(T(5))";

            Assert.Equals(true, ws.Range("B1").Value);
            Assert.Equals(true, ws.Range("B2").Value);
            Assert.Equals(true, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(true, ws.Range("B5").Value);
            Assert.Equals(true, ws.Range("B6").Value);
            Assert.Equals(false, ws.Range("B7").Value);

            SetFunctionOneParamAllCombinations(ws, "ISNUMBER");

            Assert.Equals(true, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(false, ws.Range("B3").Value);
            Assert.Equals(false, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(false, ws.Range("B6").Value);

            SetFunctionOneParamAllCombinations(ws, "ISREF");
            ws.Range("B7").Formula = "ISREF(23)";
            ws.Range("B8").Formula = "ISREF(#REF!)";

            Assert.Equals(true, ws.Range("B1").Value);
            Assert.Equals(true, ws.Range("B2").Value);
            Assert.Equals(true, ws.Range("B3").Value);
            Assert.Equals(true, ws.Range("B4").Value);
            Assert.Equals(true, ws.Range("B5").Value);
            Assert.Equals(true, ws.Range("B6").Value);
            Assert.Equals(false, ws.Range("B7").Value);
            Assert.Equals(false, ws.Range("B8").Value);

            SetFunctionOneParamAllCombinations(ws, "ISTEXT");
            ws.Range("B7").Formula = "ISTEXT(T(5))";

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(false, ws.Range("B2").Value);
            Assert.Equals(false, ws.Range("B3").Value);
            Assert.Equals(true, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(false, ws.Range("B6").Value);
            Assert.Equals(true, ws.Range("B7").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ISODD_IS_EVEN()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "ISODD");
            ws.Range("A7").Value = -11.5;
            ws.Range("B8").Formula = "ISODD(12&4.7)";

            Assert.Equals(false, ws.Range("B1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(false, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(true, ws.Range("B7").Value);
            Assert.Equals(false, ws.Range("B8").Value);

            SetFunctionOneParamAllCombinations(ws, "ISEVEN");
            ws.Range("B8").Formula = "ISEVEN(12&4.7)";

            Assert.Equals(true, ws.Range("B1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(true, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(false, ws.Range("B7").Value);
            Assert.Equals(true, ws.Range("B8").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_TYPE()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "TYPE");
            ws.Range("A7").Value = "-11-";

            Assert.Equals(1, ws.Range("B1").Value);
            Assert.Equals(4, ws.Range("B2").Value);
            Assert.Equals(16, ws.Range("B3").Value);
            Assert.Equals(2, ws.Range("B4").Value);
            Assert.Equals(1, ws.Range("B5").Value);
            Assert.Equals(16, ws.Range("B6").Value);
            Assert.Equals(2, ws.Range("B7").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ERROR_TYPE()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "ERROR.TYPE");
            ws.Range("A7").Formula = "-11.5/P8";
            ws.Range("B8").Formula = "ERROR.TYPE(#REF!)";
            ws.Range("B9").Formula = "ERROR.TYPE(#NAME?)";
            ws.Range("B10").Formula = "ERROR.TYPE(#NUM!)";
            ws.Range("B11").Formula = "ERROR.TYPE(#N/A)";

            Assert.Equals(ErrorValue.not_available, ws.Range("B1").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("B2").Value);
            Assert.Equals(1, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("B4").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("B5").Value);
            Assert.Equals(3, ws.Range("B6").Value);
            Assert.Equals(2, ws.Range("B7").Value);
            Assert.Equals(4, ws.Range("B8").Value);
            Assert.Equals(5, ws.Range("B9").Value);
            Assert.Equals(6, ws.Range("B10").Value);
            Assert.Equals(7, ws.Range("B11").Value);



            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_UNSUPPORTED_FUNCTION()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B11").Formula = "BESSELJ(#N/A,5,B3)";
            Assert.Equals("#UNSUPPORTED_FUNCTION!", ws.Range("B11").Value);
            Assert.Equals("BESSELJ(#N/A,5,B3)", ws.Range("B11").Formula);

            ws.Range("B12").Formula = "SUM(5+INFO(#N/A,5,B3))";
            Assert.Equals("#UNSUPPORTED_FUNCTION!", ws.Range("B12").Value);
            Assert.Equals("SUM(5+INFO(#N/A,5,B3))", ws.Range("B12").Formula);

            ws.Range("B13").Formula = "SUM(5+ATTACH(#N/A,5,B3))";
            Assert.Equals(1, err_listener.AllHappenedErrors.Count);
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();
            
            List<FunctionDescription> all_functions = editor.FunctionsStorage.GetFunctionsList();
            FunctionDescription desc_sum = all_functions.Find(delegate(FunctionDescription desc) { return (desc.FuncName == "BESSELJ"); });
            Assert.Null(desc_sum);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_SIGN()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "SIGN");
            ws.Range("A7").Formula = "-11.5/0.00000000001";

            Assert.Equals(1, ws.Range("B1").Value);
            Assert.Equals(1, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(0, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(-1, ws.Range("B7").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ABS()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "ABS");
            ws.Range("A7").Formula = "-11.5";

            Assert.Equals(12.34, ws.Range("B1").Value);
            Assert.Equals(1, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(0, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(11.5, ws.Range("B7").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_EVEN()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "EVEN");
            ws.Range("A7").Formula = "-11.5";
            ws.Range("A8").Formula = "-11.4";
            ws.Range("A9").Formula = "-12.000001";
            ws.Range("A10").Formula = "100.000001";
            ws.Range("A11").Formula = "99.99999";
            ws.Range("A12").Formula = "2";
            ws.Range("A13").Formula = "3";
            ws.Range("B8:B13").Formula = "EVEN(A8)";

            Assert.Equals(14, ws.Range("B1").Value);
            Assert.Equals(2, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(0, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(-12, ws.Range("B7").Value);
            Assert.Equals(-12, ws.Range("B8").Value);
            Assert.Equals(-14, ws.Range("B9").Value);
            Assert.Equals(102, ws.Range("B10").Value);
            Assert.Equals(100, ws.Range("B11").Value);
            Assert.Equals(2, ws.Range("B12").Value);
            Assert.Equals(4, ws.Range("B13").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ODD()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "ODD");
            ws.Range("A7").Formula = "-11.5";
            ws.Range("A8").Formula = "-11.4";
            ws.Range("A9").Formula = "-12.000001";
            ws.Range("A10").Formula = "100.000001";
            ws.Range("A11").Formula = "99.99999";
            ws.Range("A12").Formula = "2";
            ws.Range("A13").Formula = "3";
            ws.Range("A14").Formula = "0";
            ws.Range("A15").Formula = "0.1";
            ws.Range("A16").Formula = "2.99999999";
            ws.Range("A17").Formula = "1";
            ws.Range("A18").Formula = "-0.1";
            ws.Range("A19").Formula = "-2.1";
            ws.Range("A20").Formula = "-3.1";
            ws.Range("A21").Formula = "-3";
            ws.Range("A22").Formula = "-4";
            ws.Range("B8:B22").Formula = "ODD(A8)";

            Assert.Equals(13, ws.Range("B1").Value);
            Assert.Equals(1, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(1, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(-13, ws.Range("B7").Value);
            Assert.Equals(-13, ws.Range("B8").Value);
            Assert.Equals(-13, ws.Range("B9").Value);
            Assert.Equals(101, ws.Range("B10").Value);
            Assert.Equals(101, ws.Range("B11").Value);
            Assert.Equals(3, ws.Range("B12").Value);
            Assert.Equals(3, ws.Range("B13").Value);
            Assert.Equals(1, ws.Range("B14").Value);
            Assert.Equals(1, ws.Range("B15").Value);
            Assert.Equals(3, ws.Range("B16").Value);
            Assert.Equals(1, ws.Range("B17").Value);
            Assert.Equals(-1, ws.Range("B18").Value);
            Assert.Equals(-3, ws.Range("B19").Value);
            Assert.Equals(-5, ws.Range("B20").Value);
            Assert.Equals(-3, ws.Range("B21").Value);
            Assert.Equals(-5, ws.Range("B22").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_FLOOR()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsFloorTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);
            
            CheckFormulaResultAgainstTemplate("FLOOR", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_CEILING()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsCeilingTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);
            
            CheckFormulaResultAgainstTemplate("CEILING", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_MROUND()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsMRoundTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);

            CheckFormulaResultAgainstTemplate("MROUND", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ROUNDDOWN()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsRoundDownTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);

            CheckFormulaResultAgainstTemplate("ROUNDDOWN", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ROUNDUP()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsRoundUpTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);

            CheckFormulaResultAgainstTemplate("ROUNDUP", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_ROUND()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsRoundTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);

            CheckFormulaResultAgainstTemplate("ROUND", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test, Ignore("Precision is not enough")]
        public void Test_MOD()
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/MathTwoArgsModTemplate.xlsx");
            Assert.NotNull(wb);
            ErrorListener err_listener = new ErrorListener(editor);

            CheckFormulaResultAgainstTemplate("MOD", wb.Worksheets["Working Place"], wb.Worksheets["Template Values"]);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void Test_INT()
        {
            Workbook wb = editor.CreateWorkbook("book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            SetFunctionOneParamAllCombinations(ws, "INT");
            ws.Range("A7").Value = -2E+201;
            ws.Range("A8").Value = -20000.6454564;
            ws.Range("A9").Value = -2.132345;
            ws.Range("A10").Value = -0.2285;
            ws.Range("A11").Value = -5E-200;
            ws.Range("A12").Value = 0;
            ws.Range("A13").Value = 5E-56;
            ws.Range("A14").Value = 5.2545;
            ws.Range("A15").Value = 5E+200;
            ws.Range("B8:B15").Formula = "INT(A8)";

            Assert.Equals(12, ws.Range("B1").Value);
            Assert.Equals(1, ws.Range("B2").Value);
            Assert.Equals(ErrorValue.null_value, ws.Range("B3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B4").Value);
            Assert.Equals(0, ws.Range("B5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B6").Value);
            Assert.Equals(-2E+201, ws.Range("B7").Value);
            Assert.Equals(-20001, ws.Range("B8").Value);
            Assert.Equals(-3, ws.Range("B9").Value);
            Assert.Equals(-1, ws.Range("B10").Value);
            Assert.Equals(-1, ws.Range("B11").Value);
            Assert.Equals(0, ws.Range("B12").Value);
            Assert.Equals(0, ws.Range("B13").Value);
            Assert.Equals(5, ws.Range("B14").Value);
            Assert.Equals(5e200, ws.Range("B15").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        private void CheckFormulaResultAgainstTemplate(string func_name, Worksheet working_sheet, Worksheet template_sheet)
        {
            Console.WriteLine("Formula set started");
            DateTime before = DateTime.Now;

            working_sheet.Range("B2:CG85").Formula = func_name + "($A2,B$1)";
            DateTime after = DateTime.Now;
            TimeSpan delta = new TimeSpan(after.Ticks - before.Ticks);
            Console.WriteLine("Formula set finished (" + delta.TotalSeconds + " seconds)");

            Console.WriteLine("Range verification started");
            for (int row = 2; row <= 85; ++row)
            {
                for (int col = 2; col <= 85; ++col)
                {
                    Range range_tmpl = template_sheet.Cell(row, col);
                    object template_value = range_tmpl.Value;
                    object working_value = working_sheet.Cell(row, col).Value;
                    if (template_value is double && working_value is double)
                    {
                        double template_value_numeric = (double)template_value;
                        Assert.Equals(template_value_numeric, (double)working_value, template_value_numeric / 1e12, "Wrong formula calculation in cell " + range_tmpl.ToString());
                    }
                    else
                    {
                        Assert.Equals(template_value, working_value, "Wrong formula calculation in cell " + range_tmpl.ToString());
                    }
                }
            }
            Console.WriteLine("Range verification finished");
        }

        private Workbook LoadTwoParamsAllCombinations(string func_name)
        {
            Workbook wb = editor.LoadWorkbook("../../TestFiles/Function_2_params_all_comb.xlsx");
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C1:C29").Formula = func_name + "(A1,B1)";
            ws.Range("C6").Formula = func_name + "(A1,E10:R20)";
            ws.Range("C12").Formula = func_name + "(A1,E10:R20)";
            ws.Range("C18").Formula = func_name + "(A1,E10:R20)";
            ws.Range("C24").Formula = func_name + "(A1,E10:R20)";
            ws.Range("C30").Formula = func_name + "(A1,E10:R20)";
            ws.Range("C31:C35").Formula = func_name + "(F7:Q22,B1)";
            ws.Range("C36").Formula = func_name + "(F7:Q22,E10:R20)";

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            return wb;
        }

        private void SetFunctionOneParamAllCombinations(Worksheet ws, string func_name)
        {
            ws.Range("A1").Value = 12.34;
            ws.Range("A2").Value = true;
            ws.Range("A3").Value = ErrorValue.null_value;
            ws.Range("A4").Formula = "\"Qwerty\"";

            ws.Range("B1:B7").Formula = func_name + "(A1)";
            ws.Range("B6").Formula = func_name + "(D8:R15)";
        }

        private EWSEditor editor;
    }
}
