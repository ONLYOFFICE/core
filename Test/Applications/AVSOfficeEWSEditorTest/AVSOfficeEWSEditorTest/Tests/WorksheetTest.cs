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
