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
using AVSOfficeEWSEditor.Editor.Calculation.Formulas;
using AVSOfficeEWSEditor.Editor.Events;
using csUnit;
using System.IO;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "Locale")]
    public class LocaleTest
    {
        public LocaleTest()
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
        public void Test_En_FloatNumbersSet()
        {
            Workbook wb = editor.CreateWorkbook("float", 5);
            Assert.NotNull(wb);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            ErrorListener err_listener = new ErrorListener(editor);

            ws.Range("B1:B4").Formula = "TYPE(A1)";
            ws.Range("A1").Value = "100.9";
            ws.Range("A2").Value = "100,9";
            ws.Range("A3").Value = "1009";
            ws.Range("A4").Value = "f1009";

            Assert.Equals(100.9, ws.Range("A1").Value);
            Assert.Equals("100,9", ws.Range("A2").Value);
            Assert.Equals(1009, ws.Range("A3").Value);
            Assert.Equals("f1009", ws.Range("A4").Value);
            Assert.Equals(1, ws.Range("B1").Value);
            Assert.Equals(2, ws.Range("B2").Value);
            Assert.Equals(1, ws.Range("B3").Value);
            Assert.Equals(2, ws.Range("B4").Value);

            Assert.Null(err_listener.LastError);

            wb.Close();
        }

        private EWSEditor editor;
    }
}
