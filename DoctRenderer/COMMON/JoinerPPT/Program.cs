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

            files.Add("../ServerComponents/DoctRenderer/COMMON/native.js");
            files.Add("Common/browser.js");
            files.Add("Common/3rdparty/XRegExp/xregexp-all.js");
            files.Add("Common/editorscommon.js");
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
            files.Add("Word/Drawing/Metafile.js");

            files.Add("Word/Editor/Sections.js");
            files.Add("Word/Editor/Common.js");
            files.Add("Word/Editor/Serialize2.js");


            files.Add("PowerPoint/Drawing/ThemeLoader.js");
            files.Add("PowerPoint/Editor/Styles.js");
            files.Add("PowerPoint/Editor/Format/CollaborativeEditing.js");
            files.Add("PowerPoint/Editor/Format/Table.js");
            files.Add("PowerPoint/Editor/History.js");
	        files.Add("PowerPoint/Editor/FlowObjects.js");
	        files.Add("PowerPoint/Editor/Format/Paragraph.js");
            files.Add("PowerPoint/Editor/Format/FontClassification.js");
            files.Add("PowerPoint/Editor/Format/ParagraphContent.js");
            files.Add("PowerPoint/Editor/Format/Spelling.js");

	        files.Add("PowerPoint/Editor/SlideShowInfo.js");
	        files.Add("PowerPoint/Editor/PresentationProperties.js");
	        files.Add("PowerPoint/Editor/Format/GraphicFrame.js");
	        files.Add("PowerPoint/Editor/Numbering.js");

            files.Add("PowerPoint/Editor/TrackObjects/AdjustmentTracks.js");
            files.Add("PowerPoint/Editor/TrackObjects/MoveTracks.js");
            files.Add("PowerPoint/Editor/TrackObjects/NewShapeTracks.js");
            files.Add("PowerPoint/Editor/TrackObjects/PolyLine.js");
            files.Add("PowerPoint/Editor/TrackObjects/ResizeTracks.js");
            files.Add("PowerPoint/Editor/TrackObjects/RotateTracks.js");
            files.Add("PowerPoint/Editor/TrackObjects/Spline.js");

	        files.Add("PowerPoint/Editor/Drawing/Geometry.js");
	        files.Add("PowerPoint/Editor/Drawing/CreateGoemetry.js");
	        files.Add("PowerPoint/Editor/Drawing/ColorArray.js");
	        files.Add("PowerPoint/Editor/Drawing/Math.js");
	        files.Add("PowerPoint/Editor/Drawing/Path.js");
	        files.Add("PowerPoint/Editor/Drawing/ArcTo.js");
	        files.Add("PowerPoint/Editor/Drawing/GraphicFigure.js");
	        files.Add("PowerPoint/Editor/Drawing/Clone.js");
	        files.Add("PowerPoint/Editor/Drawing/CImage.js");
	        files.Add("PowerPoint/Editor/Drawing/Spline.js");
	        files.Add("PowerPoint/Editor/Drawing/PolyLine.js");
	        files.Add("PowerPoint/Editor/Drawing/Hit.js");
	        files.Add("PowerPoint/Editor/Drawing/Joined.js");

            files.Add("PowerPoint/Drawing/Transitions.js");
            files.Add("PowerPoint/Editor/Format.js");
	        files.Add("PowerPoint/themes/Themes.js");

            files.Add("Word/Drawing/HatchPattern.js");
            files.Add("Word/Drawing/Graphics.js");
            files.Add("Word/Drawing/ArcTo.js");
            files.Add("Word/Drawing/Hit.js");
            files.Add("Word/Drawing/ColorArray.js");
            files.Add("Word/Drawing/Overlay.js");
            files.Add("Word/Drawing/ShapeDrawer.js");
            files.Add("Word/Drawing/GraphicsEvents.js");
            files.Add("PowerPoint/apiDefines.js");
            files.Add("PowerPoint/apiCommon.js");

            files.Add("Common/Shapes/EditorSettings.js");
            files.Add("Common/Shapes/Serialize.js");
            files.Add("Common/Shapes/SerializeWriter.js");
            files.Add("Word/Editor/SerializeCommon.js");
            
            files.Add("Excel/graphics/DrawingContextWord.js");

            files.Add("PowerPoint/Editor/Format/Presentation.js");
            files.Add("PowerPoint/Editor/Format/Shape.js");
            files.Add("PowerPoint/Editor/Format/TextBody.js");

            files.Add("PowerPoint/Editor/Format/Image.js");
            files.Add("PowerPoint/Editor/Format/GroupShape.js");
            files.Add("PowerPoint/Editor/Format/ChartTitle.js");
            files.Add("PowerPoint/Editor/Format/Chart.js");
            files.Add("PowerPoint/Editor/Format/ChartLayout.js");

            files.Add("PowerPoint/Editor/Format/Slide.js");
            files.Add("PowerPoint/Editor/Format/SlideMaster.js");
            files.Add("PowerPoint/Editor/Format/Layout.js");
            files.Add("PowerPoint/Editor/Format/Paragraph.js");
            files.Add("PowerPoint/Editor/Format/FontClassification.js");
            files.Add("PowerPoint/Editor/Format/ParagraphContent.js");
            files.Add("PowerPoint/Editor/Format/Spelling.js");
            files.Add("PowerPoint/Editor/Format/Comments.js");


            files.Add("PowerPoint/Editor/Format/DocumentContent.js");
            files.Add("PowerPoint/Editor/GraphicObjects.js");
            files.Add("PowerPoint/Editor/States.js");

            files.Add("../ServerComponents/DoctRenderer/COMMON/native_ppt/DrawingDocument.js");
            files.Add("../ServerComponents/DoctRenderer/COMMON/native_ppt/HtmlPage.js");
            files.Add("../ServerComponents/DoctRenderer/COMMON/native_ppt/api.js");
            files.Add("../ServerComponents/DoctRenderer/COMMON/native_ppt/corrector.js");

            StringBuilder oBuilder = new StringBuilder();

            for (int i = 0; i < files.Count; i++)
            {
                StreamReader oReader = new StreamReader(strRoot + files[i]);
                oBuilder.Append(oReader.ReadToEnd());

                oBuilder.Append("\n\n");
            }

            string strDestPath = strApplication + "\\ServerComponents\\DoctRenderer\\Resources\\ppt_editor.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}
