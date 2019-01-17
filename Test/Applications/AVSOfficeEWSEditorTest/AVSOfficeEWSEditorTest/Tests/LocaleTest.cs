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
