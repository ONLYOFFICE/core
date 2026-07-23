/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AVSOfficeEWSEditor.Editor;
using csUnit;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture]
    public class WorksheetTest
    {
        public WorksheetTest()
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
        public void TestPosition()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Worksheet ws_rus = wb.Sheets.AddEmptySheet("Русский лист", null);
            wb.Sheets.AddEmptySheet("だぶゆーてぃーえふ", ws_rus);

            Assert.Equals(2, editor.ActiveWorkbook.Worksheets["だぶゆーてぃーえふ"].Index);
            Assert.Equals(4, editor.ActiveWorkbook.Worksheets.Count);
            editor.ActiveWorkbook.Sheets.AddEmptySheet("new_empty_sheet", editor.ActiveWorkbook.Worksheets[0]);
            Assert.Equals(0, editor.ActiveWorksheet.Index);
            Assert.Equals(true, editor.ActiveWorksheet.Remove());
            Assert.Equals(0, editor.ActiveWorksheet.Index);
            Assert.Equals("Sheet1", editor.ActiveWorksheet.Name);
            Assert.Null(editor.ActiveWorkbook.Worksheets["new_empty_sheet"]);
            editor.ActiveWorkbook.Worksheets[editor.ActiveWorkbook.Worksheets.Count - 1].Activate();
            Assert.Equals(editor.ActiveWorkbook.Worksheets.Count - 1, editor.ActiveWorksheet.Index);
        }

        [Test]
        public void TestDeleteLastSheet()
        {
            Workbook wb= editor.CreateWorkbook("Book2", 2);
            Assert.True(wb.Worksheets[0].Remove());
            Assert.False(wb.Worksheets[0].Remove());
        }

        [Test]
        public void TestName()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Worksheet ws_rus = wb.Sheets.AddEmptySheet("Русский лист", null);
            wb.Sheets.AddEmptySheet("だぶゆーてぃーえふ", ws_rus);

            Assert.Equals("だぶゆーてぃーえふ", editor.ActiveWorksheet.Name);
            Assert.Equals(false, editor.ActiveWorksheet.SetName(""));
            Assert.Equals(true, editor.ActiveWorksheet.SetName("1"));
            Assert.Equals("1", editor.ActiveWorksheet.Name);
            Assert.Equals("1", editor.ActiveWorkbook.Worksheets["1"].Name);
        }

        [Test]
        public void TestBackReference()
        {
            Workbook wb = editor.CreateWorkbook("Book1", 2);
            Assert.Equals(wb, editor.ActiveWorksheet.Workbook);
        }



        private EWSEditor editor;
    }
}
