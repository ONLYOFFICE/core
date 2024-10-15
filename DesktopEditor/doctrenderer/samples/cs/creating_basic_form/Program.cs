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

namespace Sample
{
    public class CreatingBasicForm
    {
        public static void Main(string[] args)
        {
            string workDirectory = "C:/Program Files/ONLYOFFICE/DocumentBuilder";
            string resultPath = "../../../result.docx";

            // add Docbuilder dlls in path
            System.Environment.SetEnvironmentVariable("PATH", System.Environment.GetEnvironmentVariable("PATH") + ";" + workDirectory);

            CreateBasicForm(workDirectory, resultPath);
        }

        public static void CreateBasicForm(string workDirectory, string resultPath)
        {
            var doctype = (int)OfficeFileTypes.Document.DOCX;

            // Init DocBuilder
            CDocBuilder.Initialize(workDirectory);
            CDocBuilder oBuilder = new CDocBuilder();
            oBuilder.CreateFile(doctype);

            CContext oContext = oBuilder.GetContext();
            CContextScope oScope = oContext.CreateScope();
            CValue oGlobal = oContext.GetGlobal();
            CValue oApi = oGlobal["Api"];

            // Create basic form
            CValue oDocument = oApi.Call("GetDocument");
            CValue oParagraph = oDocument.Call("GetElement", 0);
            CValue oHeadingStyle = oDocument.Call("GetStyle", "Heading 3");

            oParagraph.Call("AddText", "Employee pass card");
            oParagraph.Call("SetStyle", oHeadingStyle);
            oDocument.Call("Push", oParagraph);

            CValue oPictureForm = oApi.Call("CreatePictureForm");
            setPictureFormProperties(oPictureForm, "Photo", "Upload your photo", false, "Photo", "tooBig", true, false, 50, 50, "https://api.onlyoffice.com/content/img/docbuilder/examples/user-profile.png");
            oParagraph = oApi.Call("CreateParagraph");
            oParagraph.Call("AddElement", oPictureForm);
            oDocument.Call("Push", oParagraph);

            CValue oTextForm = oApi.Call("CreateTextForm");
            setTextFormProperties(oTextForm, "First name", "Enter your first name", false, "First name", true, 13, 3, false, false);
            oParagraph = oApi.Call("CreateParagraph");
            oParagraph.Call("AddElement", oTextForm);
            oDocument.Call("Push", oParagraph);

            // Save file and close DocBuilder
            oBuilder.SaveFile(doctype, resultPath);
            oBuilder.CloseFile();

            CDocBuilder.Destroy();
        }

        public static void setPictureFormProperties(CValue oPictureForm, string key, string tip, bool required, string placeholder, string scaleFlag, bool lockAspectRatio, bool respectBorders, int shiftX, int shiftY, string imageUrl)
        {
            oPictureForm.Call("SetFormKey", key);
            oPictureForm.Call("SetTipText", tip);
            oPictureForm.Call("SetRequired", required);
            oPictureForm.Call("SetPlaceholderText", placeholder);
            oPictureForm.Call("SetScaleFlag", scaleFlag);
            oPictureForm.Call("SetLockAspectRatio", lockAspectRatio);
            oPictureForm.Call("SetRespectBorders", respectBorders);
            oPictureForm.Call("SetPicturePosition", shiftX, shiftY);
            oPictureForm.Call("SetImage", imageUrl);
        }

        public static void setTextFormProperties(CValue oTextForm, string key, string tip, bool required, string placeholder, bool comb, int maxCharacters, int cellWidth, bool multiLine, bool autoFit)
        {
            oTextForm.Call("SetFormKey", key);
            oTextForm.Call("SetTipText", tip);
            oTextForm.Call("SetRequired", required);
            oTextForm.Call("SetPlaceholderText", placeholder);
            oTextForm.Call("SetComb", comb);
            oTextForm.Call("SetCharactersLimit", maxCharacters);
            oTextForm.Call("SetCellWidth", cellWidth);
            oTextForm.Call("SetCellWidth", multiLine);
            oTextForm.Call("SetMultiline", autoFit);
        }
    }
}
