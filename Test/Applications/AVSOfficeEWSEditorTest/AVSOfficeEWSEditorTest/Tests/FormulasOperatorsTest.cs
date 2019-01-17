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
    [TestFixture(Categories = "Formulas, FormulasOperators")]
    public class FormulasOperatorsTest
    {
        public FormulasOperatorsTest()
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
        public void TestAddOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A3").Formula = "A1 + A2  ";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(3, listener.UpdatedCellsList.Count);
            Assert.Equals("A3", listener.UpdatedCellsList[0]);
            Assert.Equals(11, ws.Range("A3").Value);
            Assert.Equals("A1+A2", ws.Range("A3").Formula);
            listener.Reset();

            ws.Range("A3").Formula = "5.5 + A$2";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            Assert.Equals(11.5, ws.Range("A3").Value);
            Assert.Equals("5.5+A$2", ws.Range("A3").Formula);
            listener.Reset();

            ws.Range("A3").Formula = "5.5 + #REF!";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);
            Assert.Equals("5.5+#REF!", ws.Range("A3").Formula);
            listener.Reset();

            ws.Range("A3").Formula = "A1 + A2 + \"dddd\"";
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            Assert.Equals(1, listener.UpdatedCellsList.Count);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("A1+A2+\"dddd\"", ws.Range("A3").Formula);
            listener.Reset();

            ws.Range("A4").Value = true;
            ws.Range("A3").Formula = "A1 + A4 + 2.3";
            Assert.Equals(8.3, ws.Range("A3").Value);
            Assert.Equals("A1+A4+2.3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "A1 + FALSE + 2.3";
            Assert.Equals(7.3, ws.Range("A3").Value);
            Assert.Equals("A1+FALSE+2.3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "4000000000 + 4000000000 + 55";
            Assert.Equals(8000000055.0, ws.Range("A3").Value);
            Assert.Equals("4000000000+4000000000+55", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "A1 + 5.5 + #REF! + TRUE + 4000000000 + \"wtf\"";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);
            Assert.Equals("A1+5.5+#REF!+TRUE+4000000000+\"wtf\"", ws.Range("A3").Formula);

            wb.Close();
        }

        [Test]
        public void TestSubOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A3").Formula = "  A1 - A2  ";
            Assert.Equals(-1, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 - A$2";
            Assert.Equals(-0.5, ws.Range("A3").Value);
            Assert.Equals("5.5-A$2", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "5.5 - TRUE";
            Assert.Equals(4.5, ws.Range("A3").Value);
            Assert.Equals("5.5-TRUE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "5.5 - #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);
            Assert.Equals("5.5-#REF!", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "A1 - A2 - \"dddd\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("A1-A2-\"dddd\"", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "A1 - P8";
            Assert.Equals(5, ws.Range("A3").Value);
            Assert.Equals("A1-P8", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "TRUE - 5.5";
            Assert.Equals(-4.5, ws.Range("A3").Value);
            Assert.Equals("TRUE-5.5", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "FALSE - TRUE";
            Assert.Equals(-1, ws.Range("A3").Value);
            Assert.Equals("FALSE-TRUE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "FALSE - #NULL!";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);
            Assert.Equals("FALSE-#NULL!", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "FALSE - \"#NULL!\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("FALSE-\"#NULL!\"", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "TRUE - P8";
            Assert.Equals(1, ws.Range("A3").Value);
            Assert.Equals("TRUE-P8", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#N/A - 5.5";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("#N/A-5.5", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#N/A - FALSE";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("#N/A-FALSE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#NULL! - #N/A";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);
            Assert.Equals("#NULL!-#N/A", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#N/A - #NULL!";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("#N/A-#NULL!", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#N/A - \"5--5\"";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("#N/A-\"5--5\"", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "#N/A - P8";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("#N/A-P8", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "\"Минус\" - 0";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("\"Минус\"-0", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "\"Минус\" - FALSE";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("\"Минус\"-FALSE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "\"Минус\" - #N/A";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("\"Минус\"-#N/A", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "\"Минус\" - \"Плюс\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("\"Минус\"-\"Плюс\"", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "\"Минус\" - P8";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("\"Минус\"-P8", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 - 6.8e-5";
            Assert.Equals(-6.8e-5, ws.Range("A3").Value);
            Assert.Equals("P8-6.8E-05", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 - TRUE";
            Assert.Equals(-1, ws.Range("A3").Value);
            Assert.Equals("P8-TRUE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 - #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);
            Assert.Equals("P8-#N/A", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 - \"Минус\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("P8-\"Минус\"", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 - P9";
            Assert.Equals(0, ws.Range("A3").Value);
            Assert.Equals("P8-P9", ws.Range("A3").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestMulOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A3").Formula = "  A1 * A2  ";
            Assert.Equals(30, ws.Range("A3").Value);
            Assert.Equals("A1*A2", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "5.5 * A$2";
            Assert.Equals(33, ws.Range("A3").Value);
            Assert.Equals("5.5*A$2", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "5.5 * TRUE";
            Assert.Equals(5.5, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 * #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 * A2 * \"dddd\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 * P8";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE * 5.5";
            Assert.Equals(5.5, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE * TRUE";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE * TRUE";
            Assert.Equals(1, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE * #NULL!";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE * \"#NULL!\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE * P8";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A * 5.5";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A * FALSE";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#NULL! * #N/A";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A * #NULL!";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A * \"5--5\"";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A * P8";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" * 0";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" * FALSE";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" * #N/A";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" * \"Плюс\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" * P8";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 * 6.8e-5";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 * TRUE";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 * #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 * \"Минус\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 * P9";
            Assert.Equals(0, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestDivOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A4").Value = 0.0;
            ws.Range("A3").Formula = "  A1 / A2  ";
            Assert.Equals(5.0 / 6.0, (double)ws.Range("A3").Value, 1e-15);
            Assert.Equals("A1/A2", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "5.5 / A$2";
            Assert.Equals(5.5 / 6.0, (double)ws.Range("A3").Value, 1e-15);

            ws.Range("A3").Formula = "A1 / A4";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 / TRUE";
            Assert.Equals(5.5, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 / FALSE";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 / #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 / \"dddd\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 / P8";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE / 5.5";
            Assert.Equals(1.0 / 5.5, (double)ws.Range("A3").Value, 1e-15);

            ws.Range("A3").Formula = "FALSE / TRUE";
            Assert.Equals(0.0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE / TRUE";
            Assert.Equals(1.0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE / #NULL!";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE / \"#NULL!\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE / P8";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A / 5.5";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A / FALSE";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#NULL! / #N/A";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A / #NULL!";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A / \"5--5\"";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A / P8";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" / 0";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" / FALSE";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" / #N/A";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" / \"Плюс\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" / P8";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 / 6.8e-5";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 / TRUE";
            Assert.Equals(0, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 / #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 / \"Минус\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 / P9";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestFourOperatorsTogether()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A4").Value = 0.0;

            ws.Range("A3").Formula = "P9+54 - 56 *A1";
            Assert.Equals(-226, ws.Range("A3").Value);
            Assert.Equals("P9+54-56*A1", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "P8 / P9+54 - 56*A4";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);
            Assert.Equals("P8/P9+54-56*A4", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "1+2*3+4";
            Assert.Equals(11, ws.Range("A3").Value);
            Assert.Equals("1+2*3+4", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "1+2*\"3\"+4";
            Assert.Equals(11, ws.Range("A3").Value);
            Assert.Equals("1+2*\"3\"+4", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "1+2*\"ZX\"+4";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);
            Assert.Equals("1+2*\"ZX\"+4", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "2*2*2*2*2*2*2*2*2/2/2/2/2/2/2/2/2*TRUE";
            Assert.Equals(2, ws.Range("A3").Value);
            Assert.Equals("2*2*2*2*2*2*2*2*2/2/2/2/2/2/2/2/2*TRUE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "2*2*2*2*2*2*2*2*2+2/2/2/2/2/2/2/2*TRUE";
            Assert.Equals(512.015625, ws.Range("A3").Value);
            Assert.Equals("2*2*2*2*2*2*2*2*2+2/2/2/2/2/2/2/2*TRUE", ws.Range("A3").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestUnaryOperators()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A4").Value = 0.0;

            ws.Range("A3").Formula = "3-3";
            Assert.Equals(0, ws.Range("A3").Value);
            Assert.Equals("3-3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "3*-3";
            Assert.Equals(-9, ws.Range("A3").Value);
            Assert.Equals("3*-3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "3%-3";
            Assert.Equals(-2.97, ws.Range("A3").Value);
            Assert.Equals("3%-3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-3%-3%";
            Assert.Equals(-0.06, ws.Range("A3").Value);
            Assert.Equals("-3%-3%", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-3%%-3";
            Assert.Equals(-3.0003, ws.Range("A3").Value);
            Assert.Equals("-3%%-3", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-3*TRUE";
            Assert.Equals(-3, ws.Range("A3").Value);
            Assert.Equals("-3*TRUE", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "+-5";
            Assert.Equals(-5, ws.Range("A3").Value);
            Assert.Equals("+-5", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "4++++---5";
            Assert.Equals(-1, ws.Range("A3").Value);
            Assert.Equals("4++++---5", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "++--+-5%%";
            Assert.Equals(-0.0005, ws.Range("A3").Value);
            Assert.Equals("++--+-5%%", ws.Range("A3").Formula);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestPowerOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = false;
            ws.Range("A4").Value = 0.0;

            ws.Range("A3").Formula = "-50^-4";
            Assert.Equals(0.00000016, ws.Range("A3").Value);
            Assert.Equals("-50^-4", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-50^-1";
            Assert.Equals(-0.02, ws.Range("A3").Value);
            Assert.Equals("-50^-1", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-50^-0.6";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);
            Assert.Equals("-50^-0.6", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "-50^-0";
            Assert.Equals(1, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50^0.6";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50^A1";
            Assert.Equals(-50, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50^4";
            Assert.Equals(6250000, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-0.5^-4";
            Assert.Equals(16, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-0.5^0.6";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-A2^-4";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A4^-A1";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A4^-0.6";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A4^A2";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A2^A4";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "23^4";
            Assert.Equals(279841, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5+2/1--4*2^-3";
            Assert.Equals(7.5, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestParenthesis()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 45;
            ws.Range("A2").Value = false;
            ws.Range("A4").Value = 256.0;
            ws.Range("G2").Value = 17;

            ws.Range("A3").Formula = "  (  4  +  4  )*5+ 5";
            Assert.Equals(45, ws.Range("A3").Value);
            Assert.Equals("(4+4)*5+5", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "((25-25))";
            Assert.Equals(0, ws.Range("A3").Value);
            Assert.Equals("((25-25))", ws.Range("A3").Formula);

// Until named ranges are implemented
//             ws.Range("A3").Formula = "1,11*-(qwqw33+B11- 4)";
//             Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "(55)%";
            Assert.Equals(0.55, ws.Range("A3").Value);
            Assert.Equals("(55)%", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "---+-(--+-++(--(A4)%+1)%%)*200%";
            Assert.Equals(-0.000712, ws.Range("A3").Value);
            Assert.Equals("---+-(--+-++(--(A4)%+1)%%)*200%", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "((A1+2)*(28-G2))";
            Assert.Equals(517, ws.Range("A3").Value);
            Assert.Equals("((A1+2)*(28-G2))", ws.Range("A3").Formula);

            ws.Range("A3").Formula = "(4+5)+6";
            Assert.Equals(15, ws.Range("A3").Value);
            Assert.Equals("(4+5)+6", ws.Range("A3").Formula);


            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestPowerBug()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("C13").Value = true;
            ws.Range("D8").Value = 345;
            ws.Range("D10").Formula = "-D8^(C13+D8/100)";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("D10").Value);
            ws.Range("D11").Formula = "(4-8.7)^-(589/100)";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("D11").Value);
            ws.Range("D11").Formula = "(4-8.7)^-(500/100)";
            Assert.Equals(-0.000436, (double)ws.Range("D11").Value, 0.0000001);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestEqOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 5;
            ws.Range("A2").Value = 6;
            ws.Range("A4").Value = 0.0;
            ws.Range("A3").Formula = "  A1 = A2  ";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "  A1 = A1  ";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "55 = 5.5e+1";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.500000001 = 5.5";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 = A4";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 = TRUE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "0 = FALSE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 = #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 = \"5\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1 = P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE = 1";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE = TRUE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE = TRUE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE = #NULL!";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE = \"#NULL!\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE = P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE = P8";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A = 5.5";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A = FALSE";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#NULL! = #N/A";
            Assert.Equals(ErrorValue.null_value, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A = #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A = \"5--5\"";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#N/A = P8";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = 0";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = FALSE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"\" = FALSE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = \"Плюс\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = \"Минус\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"Минус\" = P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"\" = P8";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = 6.8e-20";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = 0";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = TRUE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = FALSE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = #N/A";
            Assert.Equals(ErrorValue.not_available, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = \"Минус\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 = P9";
            Assert.Equals(true, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestNeOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A3").Formula = "1<>2";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "500<>5e2";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "0<>FALSE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-1.1<>TRUE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5.5 <> #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5 <> \"5\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5 <> \"6\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5 <> A1:A2";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "5<>P8";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "0<>P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE<>1";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE<>-0.99";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE<>TRUE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE<>TRUE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE <> #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE <> \"FALSE\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE <> \"\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE <> A1:A2";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "FALSE<>P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE<>P8";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF!<>1";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF!<>TRUE";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF! <> #VALUE!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF! <> #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF! <> \"#REF!\"";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF! <> A1:A2";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF!<>P8";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"TRUE\"<>1";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"qwe\"<>\"qwe\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"qwe\"<>\"Qwe\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"TRUE\"<>TRUE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"qwe\" <> #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"qwe\" <> A1:A2";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"qwe\"<>P8";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"\"<>P8";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2<>1";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2<>\"Qwe\"";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2<>TRUE";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2 <> #REF!";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2 <> A1:A2";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A2<>P8";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A1:A1<>A1";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>1";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>0";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>\"Qwe\"";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>\"\"";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>TRUE";
            Assert.Equals(true, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>FALSE";
            Assert.Equals(false, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 <> #REF!";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8 <> A1:A2";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8<>P8";
            Assert.Equals(false, ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestLtLeGtGeOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);


            #region numeric as first argument
            ws.Range("A1").Value = 0;
            ws.Range("A2").Value = 0;
            ws.Range("A3").Value = 1.5;
            ws.Range("A4").Value = -5.5E-3;

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            ws.Range("C1:C4").Formula = "A1 < B1";
            ws.Range("D1:D4").Formula = "A1 <= B1";
            ws.Range("E1:E4").Formula = "A1 > B1";
            ws.Range("F1:F4").Formula = "A1 >= B1";

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(true, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(true, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(false, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(false, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"\"";
            ws.Range("B4").Value = "DDD";

            Assert.Equals(true, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(true, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(false, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(false, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);

            ws.Range("B1").Formula = "G5:K8"; // Test Range set as value
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("B1").Value);

            ws.Range("C5").Formula = "A3 < G5:K8";
            ws.Range("D5").Formula = "A3 <= G5:G5";
            ws.Range("E5").Formula = "A3 > G5:K8";
            ws.Range("F5").Formula = "A3 >= G5:G5";
            
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C5").Value);
            Assert.Equals(false, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);
            #endregion

            #region boolean as first argument
            ws.Range("A1").Value = false;
            ws.Range("A2").Value = false;
            ws.Range("A3").Value = true;
            ws.Range("A4").Value = true;

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(false, ws.Range("D1").Value);
            Assert.Equals(false, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(true, ws.Range("E1").Value);
            Assert.Equals(true, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"\"";
            ws.Range("B4").Value = "DDD";

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(false, ws.Range("D1").Value);
            Assert.Equals(false, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(true, ws.Range("E1").Value);
            Assert.Equals(true, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C5").Value);
            Assert.Equals(false, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);
            #endregion

            #region error value as first argument
            ws.Range("A1").Value = ErrorValue.division_by_zero;
            ws.Range("A2").Value = ErrorValue.division_by_zero;
            ws.Range("A3").Value = ErrorValue.not_available;
            ws.Range("A4").Value = ErrorValue.not_available;

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"\"";
            ws.Range("B4").Value = "DDD";

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F4").Value);

            Assert.Equals(ErrorValue.not_available, ws.Range("C5").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E5").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.not_available, ws.Range("F4").Value);
            #endregion

            #region string as first argument
            ws.Range("A1").Formula = "\"\"";
            ws.Range("A2").Formula = "\"\"";
            ws.Range("A3").Formula = "\"FALSE\"";
            ws.Range("A4").Formula = "\"123\"";

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(false, ws.Range("D1").Value);
            Assert.Equals(false, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(true, ws.Range("E1").Value);
            Assert.Equals(true, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(true, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(true, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(false, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(false, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"FaLsE\"";
            ws.Range("B4").Formula = "\"122\"";

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C5").Value);
            Assert.Equals(false, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(false, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(true, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);
            #endregion

            #region null as first argument
            ws.Range("A1:A4").Value = null;

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(false, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(true, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"FaLsE\"";
            ws.Range("B4").Formula = "\"122\"";

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(true, ws.Range("C2").Value);
            Assert.Equals(true, ws.Range("C3").Value);
            Assert.Equals(true, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(false, ws.Range("F2").Value);
            Assert.Equals(false, ws.Range("F3").Value);
            Assert.Equals(false, ws.Range("F4").Value);

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C5").Value);
            Assert.Equals(true, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E5").Value);
            Assert.Equals(true, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(false, ws.Range("C1").Value);
            Assert.Equals(false, ws.Range("C2").Value);
            Assert.Equals(false, ws.Range("C3").Value);
            Assert.Equals(false, ws.Range("C4").Value);
            Assert.Equals(true, ws.Range("D1").Value);
            Assert.Equals(true, ws.Range("D2").Value);
            Assert.Equals(true, ws.Range("D3").Value);
            Assert.Equals(true, ws.Range("D4").Value);
            Assert.Equals(false, ws.Range("E1").Value);
            Assert.Equals(false, ws.Range("E2").Value);
            Assert.Equals(false, ws.Range("E3").Value);
            Assert.Equals(false, ws.Range("E4").Value);
            Assert.Equals(true, ws.Range("F1").Value);
            Assert.Equals(true, ws.Range("F2").Value);
            Assert.Equals(true, ws.Range("F3").Value);
            Assert.Equals(true, ws.Range("F4").Value);
            #endregion

            #region range as first argument
            ws.Range("C1:C4").Formula = "E11:G20 < B1";
            ws.Range("D1:D4").Formula = "E11:G20 <= B1";
            ws.Range("E1:E4").Formula = "E11:G20 > B1";
            ws.Range("F1:F4").Formula = "E11:G20 >= B1";

            ws.Range("B1").Value = 0;
            ws.Range("B2").Value = 400000000;
            ws.Range("B3").Value = 0;
            ws.Range("B4").Value = -5.5E-3;

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F4").Value);

            ws.Range("B1").Value = false;
            ws.Range("B2").Value = true;
            ws.Range("B3").Value = false;
            ws.Range("B4").Value = true;

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F4").Value);

            ws.Range("B1").Value = ErrorValue.bad_reference;

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F1").Value);

            ws.Range("B1").Formula = "\"\"";
            ws.Range("B2").Value = "DDD";
            ws.Range("B3").Formula = "\"FaLsE\"";
            ws.Range("B4").Formula = "\"122\"";

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F4").Value);

            ws.Range("C5").Formula = "E11:G20 < G5:K8";
            ws.Range("D5").Formula = "E11:G20 <= G5:G5";
            ws.Range("E5").Formula = "E11:G20 > G5:K8";
            ws.Range("F5").Formula = "E11:G20 >= G5:G5";
            
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E5").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F5").Value);

            ws.Range("B1:B4").Value = null;

            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("C4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("D4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("E4").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F1").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F2").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F3").Value);
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("F4").Value);

            #endregion

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestConcatOperator()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("A1").Value = 1;
            ws.Range("A2").Value = false;
            ws.Range("A4").Value = 0.0;
            ws.Range("A5").Value = "FALSE";
            ws.Range("A6").Value = ErrorValue.not_numeric;

            ws.Range("A3").Formula = "-50&-4.5";
            Assert.Equals("-50-4.5", ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&A2";
            Assert.Equals("-50FALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&A6";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&A1/A4";
            Assert.Equals(ErrorValue.division_by_zero, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&D4:E5";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&\"D4:E5\"";
            Assert.Equals("-50D4:E5", ws.Range("A3").Value);

            ws.Range("A3").Formula = "-50&P8";
            Assert.Equals("-50", ws.Range("A3").Value);

            ws.Range("A3").Formula = "A2&0";
            Assert.Equals("FALSE0", ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE()&FALSE";
            Assert.Equals("TRUEFALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE&\"&\"";
            Assert.Equals("TRUE&", ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE&A6/0";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE&A6:B7";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "TRUE&P8";
            Assert.Equals("TRUE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6&0";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6&FALSE";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF!&\"&\"";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "#REF!&A6/0";
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6&A6:B7";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6&P8";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"A6\"&0";
            Assert.Equals("A60", ws.Range("A3").Value);

            ws.Range("A3").Formula = "A5&FALSE";
            Assert.Equals("FALSEFALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "A5&A5";
            Assert.Equals("FALSEFALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "\"#REF!\"&(A6/0)";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A5&A6:B7";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A5&P8";
            Assert.Equals("FALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&0";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&FALSE";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&A5";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&A6/0";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&A6:B7";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "A6:B7&P8";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&0";
            Assert.Equals("0", ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&TRUE()";
            Assert.Equals("TRUE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&A5";
            Assert.Equals("FALSE", ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&A6/0";
            Assert.Equals(ErrorValue.not_numeric, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&A6:B7";
            Assert.Equals(ErrorValue.wrong_value_type, ws.Range("A3").Value);

            ws.Range("A3").Formula = "P8&P8";
            Assert.Equals("", ws.Range("A3").Value);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }

        [Test]
        public void TestReferenceOperators()
        {
            Workbook wb = editor.CreateWorkbook("Book1");
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("E4:F7").Value = 1;
            ws.Range("F6:H9").Value = 1;
            ws.Range("E8:E9").Value = 100;
            ws.Range("G4:H5").Value = 100;

            ws.Range("F13").Formula = "SUM((E4:F7,F6:H9) D4:I9)";
            Assert.Equals(20, ws.Range("F13").Value);

            ws.Range("F13").Formula = "SUM((E4:F4:F6:H6) D5:I5)";
            Assert.Equals(202, ws.Range("F13").Value);

            ws.Range("H:H").DeleteColumns();
            Assert.Equals(102, ws.Range("F13").Value);
            Assert.Equals("SUM((E4:F4:F6:G6) D5:H5)", ws.Range("F13").Formula);

            ws.Range("F11").Formula = "SUM((E4:F7,F6:G9):G3:G4)";
            Assert.Equals(414, ws.Range("F11").Value);

            ws.Range("3:4").DeleteRows();
            Assert.Equals("SUM((E3:F5,F4:G7):#REF!)", ws.Range("F9").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("F9").Value);
            
            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            wb.Close();
        }



        private EWSEditor editor;
    }
}
