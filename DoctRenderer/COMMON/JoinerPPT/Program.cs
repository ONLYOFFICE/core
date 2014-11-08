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

            string[] arrFilesConfig = {
                "Common/Build/License.js",
                "Common/browser.js",
                "Common/FontsFreeType/font_engine.js",
                "Common/FontsFreeType/FontFile.js",
                "Common/FontsFreeType/FontManager.js",
                "Common/AllFonts.js",
                "Word/Drawing/Externals.js",
                "Word/Drawing/GlobalLoaders.js",
                "Common/commonDefines.js",
                "Common/apiCommon.js",
                "Common/downloaderfiles.js",
                "Common/docscoapicommon.js",
                "Common/docscoapi.js",
                "Common/docscoapisettings.js",
                "Common/wordcopypaste.js",
                "Common/editorscommon.js",
                "PowerPoint/Drawing/ThemeLoader.js",
                "Common/Charts/DrawingObjects.js",
                "Common/SerializeCommonWordExcel.js",
                "Common/SerializeChart.js",
                "Common/Charts/charts.js",
                "Common/Charts/ChartsDrawer.js",
                "Common/Charts/DrawingArea.js",
                "Common/NumFormat.js",
                "Word/Editor/Search.js",
                "Word/Editor/Serialize2.js",
                "Common/trackFile.js",
                "Word/Editor/Styles.js",
                "Word/Editor/Numbering.js",
                "Word/Drawing/Metafile.js",
                "PowerPoint/Editor/CollaborativeEditing.js",
                "Word/Drawing/GraphicsEvents.js",
                "Word/Drawing/WorkEvents.js",
                "Word/Drawing/Controls.js",
                "Word/Drawing/Rulers.js",
                "Word/Editor/Table.js",
                "Word/Editor/Common.js",
                "Word/Editor/Sections.js",
                "Common/scroll.js",
                "Word/Editor/History.js",
                "Common/Drawings/Hit.js",
                "Common/Drawings/States.js",
                "Common/Drawings/DrawingObjectsHandlers.js",
                "Common/Drawings/TrackObjects/AdjustmentTracks.js",
                "Common/Drawings/TrackObjects/ResizeTracks.js",
                "Common/Drawings/TrackObjects/RotateTracks.js",
                "Common/Drawings/TrackObjects/NewShapeTracks.js",
                "Common/Drawings/TrackObjects/PolyLine.js",
                "Common/Drawings/TrackObjects/Spline.js",
                "Common/Drawings/TrackObjects/MoveTracks.js",
                "Word/Drawing/HatchPattern.js",
                "Word/Drawing/Graphics.js",
                "Word/Drawing/Overlay.js",
                "Word/Drawing/ShapeDrawer.js",
                "PowerPoint/Drawing/Transitions.js",
                "PowerPoint/Drawing/DrawingDocument.js",
                "PowerPoint/Drawing/HtmlPage.js",
                "Common/Drawings/Format/Constants.js",
                "Common/Shapes/Serialize.js",
                "Common/Shapes/SerializeWriter.js",
                "Word/Editor/SerializeCommon.js",
                "Common/Drawings/Math.js",
                "Common/Drawings/ArcTo.js",
                "Word/Drawing/ColorArray.js",
                "PowerPoint/Editor/Format/Presentation.js",
                "Common/Drawings/CommonController.js",
                "Excel/view/DrawingObjectsController.js",
                "PowerPoint/Editor/DrawingObjectsController.js",
                "Common/Drawings/Format/Constants.js",
                "Common/Drawings/Format/Format.js",
                "Common/Drawings/Format/CreateGeometry.js",
                "Common/Drawings/Format/Geometry.js",
                "Common/Drawings/Format/Path.js",
                "Common/Drawings/Format/Shape.js",
                "Common/Drawings/Format/Image.js",
                "Common/Drawings/Format/GroupShape.js",
                "Common/Drawings/Format/ChartSpace.js",
                "Common/Drawings/Format/ChartFormat.js",
                "Common/Drawings/Format/TextBody.js",
                "PowerPoint/Editor/Format/Slide.js",
                "PowerPoint/Editor/Format/SlideMaster.js",
                "PowerPoint/Editor/Format/Layout.js",
                "PowerPoint/Editor/Format/Comments.js",
                "Word/Math/mathTypes.js",
                "Word/Math/mathText.js",
                "Word/Math/mathContent.js",
                "Word/Math/base.js",
                "Word/Math/fraction.js",
                "Word/Math/degree.js",
                "Word/Math/matrix.js",
                "Word/Math/limit.js",
                "Word/Math/nary.js",
                "Word/Math/radical.js",
                "Word/Math/operators.js",
                "Word/Math/accent.js",
                "Word/Math/borderBox.js",
                "Word/Editor/Styles.js",
                "Word/Editor/Numbering.js",
                "Word/Editor/ParagraphContent.js",
                "Word/Editor/ParagraphContentBase.js",
                "Word/Editor/Run.js",
                "Word/Editor/Hyperlink.js",
                "Word/Editor/FlowObjects.js",
                "Word/Editor/Paragraph.js",
                "Word/Editor/Paragraph_Recalculate.js",
                "Word/Editor/Document.js",
                "Word/Editor/DocumentContent.js",
                "Word/Editor/HeaderFooter.js",
                "Word/Editor/Table.js",
                "Word/Editor/Math.js",
                "Word/Editor/Spelling.js",
                "Word/Editor/FontClassification.js",
                "PowerPoint/Editor/Format/ShapePrototype.js",
                "PowerPoint/Editor/Format/ImagePrototype.js",
                "PowerPoint/Editor/Format/GroupPrototype.js",
                "PowerPoint/Editor/Format/ChartSpacePrototype.js",
                "PowerPoint/Editor/Format/GraphicFrame.js",

                "PowerPoint/apiDefines.js",
                "PowerPoint/api.js",
                "PowerPoint/apiCommon.js",

                "Common/Shapes/EditorSettings.js",
                "PowerPoint/themes/Themes.js"
            };

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

            string strDestPath = strApplication + "\\OfficeWeb\\PowerPoint\\sdk-all.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}
