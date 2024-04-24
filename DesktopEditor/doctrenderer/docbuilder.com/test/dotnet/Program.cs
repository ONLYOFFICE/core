/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

﻿using static System.Console;
using System.Runtime.InteropServices;

using onlyofficedocbuilder;

using CDocBuilder = onlyofficedocbuilder.CONLYOFFICEDocBuilder;
using CValue = onlyofficedocbuilder.CONLYOFFICEDocBuilderValue;
using CContext = onlyofficedocbuilder.CONLYOFFICEDocBuilderContext;
using CContextScope = onlyofficedocbuilder.CONLYOFFICEDocBuilderContextScope;

namespace Test
{
	public class Program
	{
		public static void Main(string[] args)
		{
			string resultPath = "result.docx";
			Test(resultPath);
		}
		public static void Test(string resultPath)
		{
			var doctype = ".docx";
			CDocBuilder oBuilder = new CDocBuilder();

			oBuilder.Initialize();
			
			oBuilder.CreateFile(doctype);

			CContext oContext = oBuilder.GetContext();
			CContextScope oScope = oContext.CreateScope();
		  
			CValue oGlobal = oContext.GetGlobal();

			CValue oApi = oGlobal.GetProperty("Api");
			CValue oDocument = oApi.Call("GetDocument");
			CValue oParagraph = oApi.Call("CreateParagraph");
			CValue oContent = oContext.CreateArray(1);

			oParagraph.Call("SetSpacingAfter", 1000, false);
			oParagraph.Call("AddText", "Hello from COM -> .net!");
			oContent.Set(0, oParagraph);
			oDocument.Call("InsertContent", oContent);

			oBuilder.SaveFile(doctype, resultPath);
			oBuilder.CloseFile();

			oBuilder.Dispose();
		}
	}
   
}
