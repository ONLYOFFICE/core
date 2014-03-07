using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace Joiner
{
    class Program
    {
        static void Main(string[] args)
        {
            string strApplication = Directory.GetCurrentDirectory();
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);
            strApplication = Path.GetDirectoryName(strApplication);

            string strRoot = strApplication + "\\OfficeWeb\\";
            List<string> files = new List<string>();

            //files.Add("../ServerComponents/DoctRenderer/COMMON/native.js");
            //files.Add("Common/3rdparty/XRegExp/xregexp-all.js");

            string[] arrFilesConfig = {
                "Common/browser.js",
                "Common/docscoapisettings.js",
                "Common/docscoapicommon.js",
                "Common/docscoapi.js",
                "Common/apiCommon.js",
                "Common/spellcheckapisettings.js",
                "Common/spellcheckapi.js",
                "Common/wordcopypaste.js",
                "Common/spellCheckLanguage.js",
                "Common/spellCheckLanguagesAll.js",
                "Common/downloaderfiles.js",
                "Common/commonDefines.js",
                "Common/editorscommon.js",
                "Common/NumFormat.js",
                "Common/Shapes/Serialize.js",
                "Common/Shapes/SerializeWriter.js",
                "Common/SerializeCommonWordExcel.js",
                "Common/Drawings/Format/Constants.js",
                "Common/Drawings/Format/Format.js",
                "Common/trackFile.js",
			    "Common/Charts/libraries/OfficeExcel.common.core.js",
                "Common/Charts/libraries/OfficeExcel.common.key.js",
                "Common/Charts/libraries/OfficeExcel.bar.js",
                "Common/Charts/libraries/OfficeExcel.hbar.js",
                "Common/Charts/libraries/OfficeExcel.line.js",
                "Common/Charts/libraries/OfficeExcel.pie.js",
                "Common/Charts/libraries/OfficeExcel.scatter.js",
                "Common/Charts/libraries/OfficeExcel.chartProperties.js",
                "Common/Charts/charts.js",
                "Common/Charts/DrawingObjects.js",
                "Common/FontsFreeType/font_engine.js",
                "Common/FontsFreeType/FontFile.js",
                "Common/FontsFreeType/FontManager.js",
                "Word/Drawing/Externals.js",
                "Word/Drawing/GlobalLoaders.js",
                "Word/Drawing/translations.js",
                "Word/Editor/GraphicObjects/GraphicObjects.js",
                "Word/Editor/GraphicObjects/GraphicPage.js",
                "Word/Editor/GraphicObjects/States.js",
                "Word/Editor/GraphicObjects/Math.js",
                "Word/Editor/GraphicObjects/TrackGraphicObjects.js",
                "Word/Editor/GraphicObjects/WrapManager.js",
                "Word/Editor/GraphicObjects/Spline.js",
                "Word/Editor/GraphicObjects/PolyLine.js",
                "Word/Editor/GraphicObjects/ObjectTypes/WordShape.js",
                "Word/Editor/GraphicObjects/ObjectTypes/WordImage.js",
                "Word/Editor/GraphicObjects/ObjectTypes/WordGroupShapes.js",
                "Word/Editor/GraphicObjects/ObjectTypes/Geometry.js",
                "Word/Editor/GraphicObjects/ObjectTypes/ShapeTracks.js",
                "Word/Editor/GraphicObjects/ObjectTypes/GroupTracks.js",
                "Word/Editor/GraphicObjects/ObjectTypes/CreateGeometry.js",
                "Word/Editor/GraphicObjects/ObjectTypes/Path.js",
                "Word/Editor/GraphicObjects/ObjectTypes/Chart.js",
                "Word/Editor/GraphicObjects/ObjectTypes/ChartLayout.js",
                "Word/Editor/GraphicObjects/ObjectTypes/ChartLegend.js",
                "Word/Editor/GraphicObjects/ObjectTypes/ChartTitle.js",
                "Word/Editor/GraphicObjects/ObjectTypes/TextBody.js",
                "Word/Editor/CollaborativeEditing.js",
                "Word/Editor/Comments.js",
                "Word/Editor/History.js",
                "Word/Editor/Styles.js",
                "Word/Editor/DrawingObjects.js",
                "Word/Editor/FlowObjects.js",
                "Word/Editor/ParagraphContent.js",
                "Word/Editor/Hyperlink.js",
                "Word/Editor/Run.js",
                "Word/Editor/Paragraph.js",
                "Word/Editor/Sections.js",
                "Word/Editor/Numbering.js",
                "Word/Editor/HeaderFooter.js",
                "Word/Editor/Document.js",
                "Word/Editor/Common.js",
                "Word/Editor/DocumentContent.js",
                "Word/Editor/Table.js",
                "Word/Editor/Serialize2.js",
                "Word/Editor/Search.js",
                "Word/Editor/FontClassification.js",
                "Word/Editor/Spelling.js",
                "Word/Drawing/Graphics.js",
                "Word/Drawing/ArcTo.js",
                "Word/Drawing/Hit.js",
                "Word/Drawing/ColorArray.js",
                "Word/Drawing/Overlay.js",
                "Word/Drawing/HatchPattern.js",
                "Word/Drawing/ShapeDrawer.js",
                "Word/Drawing/Metafile.js",
                "Word/Drawing/DrawingDocument.js",
                "Word/Drawing/GraphicsEvents.js",
                "Common/Scrolls/iscroll.js",
                "Word/Drawing/WorkEvents.js",
                "Word/Drawing/Controls.js",
                "Word/Drawing/Rulers.js",
                "Word/Drawing/HtmlPage.js",
                "Word/Drawing/documentrenderer.js",
                "Common/scroll.js",
                "Word/Editor/SerializeCommon.js",
                "Word/apiDefines.js",
                "Word/apiCommon.js",
                "Word/api.js",
                "Word/document/empty.js",
                "Excel/graphics/DrawingContextWord.js",

                "Common/Shapes/EditorSettings.js",
                "Common/commonDefines.js",
            };


            files.Add("Common/browser.js");
            files.Add("Common/editorscommon.js");
            
            files.Add("Common/downloaderfiles.js");
            files.Add("Common/docscoapicommon.js");
            files.Add("Common/docscoapi.js");
            files.Add("Common/spellcheckapi.js");

            files.Add("Common/wordcopypaste.js");
    		
            files.Add("Common/apiCommon.js");

            files.Add("Excel/apiDefines.js");
            files.Add("Common/Charts/DrawingObjects.js");
            files.Add("Common/Charts/charts.js");
            files.Add("Common/commonDefines.js");
            files.Add("Common/SerializeCommonWordExcel.js");
            files.Add("Common/Charts/libraries/OfficeExcel.common.core.js");
            files.Add("Common/Charts/libraries/OfficeExcel.common.key.js");
            files.Add("Common/Charts/libraries/OfficeExcel.bar.js");
            files.Add("Common/Charts/libraries/OfficeExcel.hbar.js");
            files.Add("Common/Charts/libraries/OfficeExcel.line.js");
            files.Add("Common/Charts/libraries/OfficeExcel.pie.js");
            files.Add("Common/Charts/libraries/OfficeExcel.scatter.js");
            files.Add("Common/Charts/libraries/OfficeExcel.chartProperties.js");

            files.Add("Common/FontsFreeType/font_engine.js");

            files.Add("Common/FontsFreeType/FontFile.js");
            files.Add("Common/FontsFreeType/FontManager.js");
            files.Add("Word/Drawing/Externals.js");
            files.Add("Word/Drawing/GlobalLoaders.js");

            files.Add("Word/Drawing/translations.js");

            files.Add("Word/Drawing/Controls.js");
            files.Add("Word/Drawing/WorkEvents.js");

            files.Add("Word/Editor/GraphicObjects/GraphicObjects.js");
            files.Add("Word/Editor/GraphicObjects/GraphicPage.js");
            files.Add("Word/Editor/GraphicObjects/States.js");
            files.Add("Word/Editor/GraphicObjects/Math.js");
            files.Add("Word/Editor/GraphicObjects/TrackGraphicObjects.js");
            files.Add("Word/Editor/GraphicObjects/WrapManager.js");
            files.Add("Word/Editor/GraphicObjects/Spline.js");
            files.Add("Word/Editor/GraphicObjects/PolyLine.js");


            files.Add("Word/Editor/GraphicObjects/ObjectTypes/WordShape.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/WordImage.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/WordGroupShapes.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/Format.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/Geometry.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/ShapeTracks.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/GroupTracks.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/CreateGeometry.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/Path.js");

            files.Add("Word/Editor/GraphicObjects/ObjectTypes/Chart.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/ChartLayout.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/ChartLegend.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/ChartTitle.js");
            files.Add("Word/Editor/GraphicObjects/ObjectTypes/TextBody.js");

            files.Add("Word/Drawing/Metafile.js");
            files.Add("Word/Editor/CollaborativeEditing.js");
            files.Add("Word/Editor/Comments.js");
            files.Add("Word/Editor/History.js");
            files.Add("Word/Editor/Styles.js");
            files.Add("Word/Editor/DrawingObjects.js");
            files.Add("Word/Editor/FlowObjects.js");
            files.Add("Word/Editor/ParagraphContent.js");
            files.Add("Word/Editor/Paragraph.js");
            files.Add("Word/Editor/Sections.js");
            files.Add("Word/Editor/Numbering.js");
            files.Add("Word/Editor/HeaderFooter.js");
            files.Add("Word/Editor/Document.js");
            files.Add("Word/Editor/Common.js");
            files.Add("Word/Editor/Numbering.js");
            files.Add("Word/Editor/DocumentContent.js");
            files.Add("Word/Editor/Table.js");
            files.Add("Word/Editor/Serialize2.js");
            files.Add("Word/Editor/Search.js");
            files.Add("Word/Editor/FontClassification.js");
            files.Add("Word/Editor/Spelling.js");

            files.Add("Word/Drawing/HatchPattern.js");
            files.Add("Word/Drawing/Graphics.js");
            files.Add("Word/Drawing/ArcTo.js");
            files.Add("Word/Drawing/Hit.js");
            files.Add("Word/Drawing/ColorArray.js");
            files.Add("Word/Drawing/Overlay.js");
            files.Add("Word/Drawing/ShapeDrawer.js");
            files.Add("Word/Drawing/DrawingDocument.js");
            files.Add("Word/Drawing/GraphicsEvents.js");
            files.Add("Word/Drawing/HtmlPage.js");
            files.Add("Word/Drawing/Rulers.js");
            files.Add("Word/apiDefines.js");
            files.Add("Word/apiCommon.js");
            files.Add("Word/api.js");

            files.Add("Common/Shapes/EditorSettings.js");
            files.Add("Common/Shapes/Serialize.js");
            files.Add("Common/Shapes/SerializeWriter.js");
            files.Add("Word/Editor/SerializeCommon.js");

            files.Add("Excel/graphics/DrawingContextWord.js");

            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_word/DrawingDocument.js");
            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_word/HtmlPage.js");
            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_word/api.js");
            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_word/corrector.js");

            if (true)
            {
                files.Clear();
                for (int i = 0; i < arrFilesConfig.Length; ++i)
                {
                    files.Add(arrFilesConfig[i]);
                }
            }

            StringBuilder oBuilder = new StringBuilder();

            for (int i = 0; i < files.Count; i++)
            {
                StreamReader oReader = new StreamReader(strRoot + files[i]);
                oBuilder.Append(oReader.ReadToEnd());

                oBuilder.Append("\n\n");
            }

            string strDestPath = strApplication + "\\OfficeWeb\\Word\\sdk-all.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}
