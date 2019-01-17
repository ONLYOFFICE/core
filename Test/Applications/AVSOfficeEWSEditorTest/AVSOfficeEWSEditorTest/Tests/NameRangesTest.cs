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
using AVSOfficeEWSEditor.Editor.Hyperlinks;
using AVSOfficeEWSEditor.Editor.DefinedNames;

namespace AVSOfficeEWSEditorTest.Tests
{
    [TestFixture(Categories = "NameRangesTest")]
    public class NameRangesTest
    {
        public NameRangesTest()
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
        public void Test_NameRangesSimpleDependencies()
        {
            ErrorListener err_listener = new ErrorListener(editor);
            Workbook wb = editor.LoadWorkbook("../../TestFiles/NameRanges_simple.xlsx");
            Assert.NotNull(wb);
            Assert.Null(err_listener.LastError);
            Worksheet ws = wb.ActiveSheet as Worksheet;
            CellsUpdatedListener listener = new CellsUpdatedListener(ws);
            
            ws.Range("A1").Value = 2;
            Assert.Equals(8, ws.Range("C2").Value);

            wb.DefinedNames.AddName("CDNAME", "Лист1!C2:D3");
            Assert.True(err_listener.LastError is FormulaFormatError);
            err_listener.Reset();

            wb.DefinedNames.AddName("CDNAME", "Лист1!$C$2:$D$3");
            ws.Range("A2").Formula = "SUM(CDNAME,C1)";
            Assert.Equals(11, ws.Range("A2").Value);

            ws.Range("A1").Value = 1;
            Assert.Equals(9, ws.Range("A2").Value);

            ws.Range("A3").Formula = "SUM((two,four))";
            Assert.Equals(6, ws.Range("A3").Value);

            ws.Range("A1").DeleteRows();

            Assert.Equals("six", ws.Range("C1").Formula);
            Assert.Equals("SUM(CDNAME,#REF!)", ws.Range("A1").Formula);
            Assert.Equals("SUM((two,four))", ws.Range("A2").Formula);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("C1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A1").Value);
            Assert.Equals(ErrorValue.bad_reference, ws.Range("A2").Value);

            int counter = 0;
            foreach (DefinedName def_name in wb.DefinedNames)
            {
                switch (++counter)
                {
                    case 1:
                        Assert.Equals("Лист1!$C$1:$D$2", def_name.Formula);
                        Assert.Equals("CDNAME", def_name.Name);
                        break;
                    case 2:
                        Assert.Equals("Лист1!#REF!", def_name.Formula);
                        Assert.Equals("four", def_name.Name);
                        break;
                    case 3:
                        Assert.Equals("two+four", def_name.Formula);
                        Assert.Equals("six", def_name.Name);
                        break;
                    case 4:
                        Assert.Equals("Лист1!#REF!", def_name.Formula);
                        Assert.Equals("two", def_name.Name);
                        break;
                }
            }

            wb.Save("../../TestFiles/NameRanges_simple_res.xlsx");

            Assert.Null(err_listener.LastError);
            wb.Close();
        }


        private EWSEditor editor;
    }
}
