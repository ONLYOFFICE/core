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

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Workbook")]
    public class WorkbookTest
    {
        public WorkbookTest()
        {
        }

        [Test]
        public void TestCreation()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Workbook wb = editor.CreateWorkbook("Book1", 5);
            Assert.NotNull(wb);
            Assert.NotNull(editor.ActiveWorkbook);
            Assert.NotNull(editor.Workbooks["Book1"]);
            try
            {
                Workbook wb2 = editor.Workbooks["Book2"];
                Assert.Fail("System.Collections.Generic.KeyNotFoundException hasn't been issued.");
            }
            catch (System.Collections.Generic.KeyNotFoundException)
            {
            }
            Assert.Equals(1, editor.Workbooks.Count);
            editor.StopLogging();
        }

        [Test]
        public void TestCreation300Sheets()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Workbook wb = editor.CreateWorkbook("Book1", 300);
            Assert.NotNull(wb);
            Assert.NotNull(editor.ActiveWorkbook);
            Assert.NotNull(editor.Workbooks["Book1"]);
            try
            {
                Workbook wb2 = editor.Workbooks["Book2"];
                Assert.Fail("System.Collections.Generic.KeyNotFoundException hasn't been issued.");
            }
            catch (System.Collections.Generic.KeyNotFoundException)
            {
            }
            Assert.Equals(1, editor.Workbooks.Count);
            Assert.Equals(300, editor.ActiveWorkbook.Worksheets.Count);
            editor.StopLogging();
        }

        [Test]
        public void TestCreationSeveralBooks()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Workbook wb1 = editor.CreateWorkbook("Book1", 3);
            Workbook wb2 = editor.CreateWorkbook("Book2", 1);
            Workbook wb3 = editor.CreateWorkbook("Книга3", 1);
            
            Assert.NotNull(wb1);
            Assert.NotNull(wb2);
            Assert.NotNull(wb3);

            Assert.NotNull(editor.ActiveWorkbook);

            Assert.Equals(editor.ActiveWorkbook, wb3);
            Assert.NotEquals(editor.ActiveWorkbook, wb1);

            Assert.NotNull(editor.Workbooks["Книга3"]);
            Assert.Equals(3, editor.Workbooks.Count);
            Assert.Equals(editor.Workbooks["Book2"], wb2);
            editor.StopLogging();
        }

        [Test]
        public void TestLoadSaveBook()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Workbook wb = editor.LoadWorkbook("../../TestFiles/DataOfVarTypes.xlsx");
            Assert.NotNull(wb);
            wb.ActiveWorksheet.Range("I12:M22").Value = 5.5; // Do some change

            Assert.True(wb.Save("../../TestFiles/DataOfVarTypes_changed.xlsx"));

            wb = editor.LoadWorkbook("../../TestFiles/DataOfVarTypes_changed.xlsx");
            Assert.NotNull(wb);
            // Do some change
            wb.ActiveWorksheet.Range("C3:D5").Formula = "B2 + 8"; 
            Assert.True(wb.Save("../../TestFiles/DataOfVarTypes_changed.xls"));

            wb.Close();
            wb = editor.LoadWorkbook("../../TestFiles/DataOfVarTypes_changed.xls");
            Assert.NotNull(wb);
            // Do some change
            wb.ActiveWorksheet.Range("4:5").Value = false;
            wb.ActiveWorksheet.Range("C3:E9").Value = "#REF!";
            wb.ActiveWorksheet.Range("4:8").Value = true;
            Assert.True(wb.Save("../../TestFiles/DataOfVarTypes_changed2.xlsx"));
            editor.StopLogging();
        }

        [Test]
        public void TestBookName()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            Workbook wb1 = editor.CreateWorkbook("Book1", 3);

            Assert.NotNull(wb1);

            Assert.Equals("Book1", wb1.Name);
            Assert.Equals(true, wb1.SetName("Книга1"));
            Assert.Equals("Книга1", wb1.Name);
            Workbook wb2 = editor.CreateWorkbook("Book1", 3);
            Assert.Equals(false, wb1.SetName("Book1"));
            Assert.Equals("Книга1", wb1.Name);
            Assert.Equals(true, wb2.SetName("Book1"));
            Assert.Equals(false, wb2.SetName(""));
            editor.StopLogging();
        }

        [Test]
        public void TestLoadAFileWithSpacesInFilename()
        {
            EWSEditor editor = new EWSEditor();
            editor.SetupLog("unittests.log");
            ErrorListener err_listener = new ErrorListener(editor);
            Workbook wb1 = editor.LoadWorkbook("../../TestFiles/Имя файла на русском и с пробелами.xlsx");
            Assert.NotNull(wb1);

            Assert.Equals("Имя файла на русском и с пробелами", wb1.Name);
            Assert.Equals(123456789112345, wb1.ActiveWorksheet.Range("E7").Value);
            Assert.Equals("Sheet1", wb1.ActiveWorksheet.Name);

            Assert.Equals(0, err_listener.AllHappenedErrors.Count);
            editor.StopLogging();
        }
    }
}
