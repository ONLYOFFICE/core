/*
* (c) Copyright Ascensio System SIA 2010-2023
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

using docbuilder_net;

using OfficeFileTypes = docbuilder_net.FileTypes;
using CValue = docbuilder_net.CDocBuilderValue;
using CContext = docbuilder_net.CDocBuilderContext;
using CContextScope = docbuilder_net.CDocBuilderContextScope;

using System;
using System.Collections.Generic;

namespace Sample
{
    public class FillingForm
    {
        public static void Main(string[] args)
        {
            string workDirectory = "C:/Program Files/ONLYOFFICE/DocumentBuilder";
            string filePath = "../../../../../../resources/docs/form.docx";
            string resultPath = "../../../result.docx";

            IDictionary<string, string> formData = new Dictionary<string, string>() {
                { "Photo", "https://api.onlyoffice.com/content/img/docbuilder/examples/blue_cloud.png" },
                { "Serial","A1345" },
                { "Company Name", "Blue Cloud" },
                { "Date", "25.12.2023" },
                { "Recipient", "Space Corporation" },
                { "Qty1", "25" },
                { "Description1", "Frame" },
                { "Qty2", "2" },
                { "Description2", "Stack" },
                { "Qty3", "34" },
                { "Description3", "Shifter" }
            };
            // add Docbuilder dlls in path
            System.Environment.SetEnvironmentVariable("PATH", System.Environment.GetEnvironmentVariable("PATH") + ";" + workDirectory);

            FillForm(workDirectory, resultPath, filePath, formData);
        }

        public static void FillForm(string workDirectory, string resultPath, string filePath, IDictionary<string, string> formData)
        {
            var doctype = (int)OfficeFileTypes.Document.DOCX;

            // Init DocBuilder
            CDocBuilder.Initialize(workDirectory);
            CDocBuilder oBuilder = new CDocBuilder();
            oBuilder.OpenFile(filePath, "docxf");

            CContext oContext = oBuilder.GetContext();
            CContextScope oScope = oContext.CreateScope();
            CValue oGlobal = oContext.GetGlobal();
            CValue oApi = oGlobal["Api"];

            // Fill form
            CValue oDocument = oApi.Call("GetDocument");
            CValue aForms = oDocument.Call("GetAllForms");
            int formNum = 0;
            while (formNum < aForms.GetLength())
            {
                CValue form = aForms[formNum];
                string type = form.Call("GetFormType").ToString();
                string value;
                try
                {
                    value = formData[form.Call("GetFormKey").ToString()];
                }
                catch (Exception e)
                {
                    value = "";
                }
                if (type == "textForm") form.Call("SetText", value);
                if (type == "pictureForm") form.Call("SetImage", value);
                formNum++;
            }

            // Save file and close DocBuilder
            oBuilder.SaveFile(doctype, resultPath);
            oBuilder.CloseFile();

            CDocBuilder.Destroy();
        }
    }
}
