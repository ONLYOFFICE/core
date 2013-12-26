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
            //files.Add("../ServerComponents/DoctRenderer/COMMON/jquery_native.js");

            files.Add("Common/browser.js");
            //files.Add("Excel/jquery/jquery-1.7.1.js");
            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_xls/jquery_corrector.js");
            
	        files.Add("Common/editorscommon.js");
	        files.Add("Common/downloaderfiles.js");
	        files.Add("Common/docscoapicommon.js");
	        files.Add("Common/docscoapi.js");
	        files.Add("Common/Charts/DrawingObjects.js");
	        files.Add("Common/Charts/charts.js");
	        files.Add("Common/commonDefines.js");
	        files.Add("Common/apiCommon.js");

            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_xls/corrector2.js");

	        files.Add("Common/FontsFreeType/font_engine.js");
	        files.Add("Common/FontsFreeType/FontFile.js");
	        files.Add("Common/FontsFreeType/FontManager.js");

	        files.Add("Word/Drawing/HatchPattern.js");
	        files.Add("Word/Drawing/Metafile.js");
	        files.Add("Common/AllFonts.js");
	        files.Add("Word/Drawing/Externals.js");
        	
	        files.Add("Excel/apiDefines.js");

	        files.Add("Excel/utils/utils.js");
	        files.Add("Excel/model/ConditionalFormatting.js");
	        files.Add("Excel/model/clipboard.js");
	        files.Add("Excel/model/autofilters.js");

	        files.Add("Excel/graphics/DrawingContext.js");
	        files.Add("Excel/graphics/pdfprinter.js");

            files.Add("Excel/offlinedocs/empty-workbook.js");

	        files.Add("Excel/model/CollaborativeEditing.js");
	        files.Add("Excel/model/FormulaObjects/parserFormula.js");
	        files.Add("Excel/model/FormulaObjects/dateandtimeFunctions.js");
	        files.Add("Excel/model/FormulaObjects/engineeringFunctions.js");
	        files.Add("Excel/model/FormulaObjects/cubeFunctions.js");
	        files.Add("Excel/model/FormulaObjects/databaseFunctions.js");
	        files.Add("Excel/model/FormulaObjects/textanddataFunctions.js");
	        files.Add("Excel/model/FormulaObjects/statisticalFunctions.js");
	        files.Add("Excel/model/FormulaObjects/financialFunctions.js");
	        files.Add("Excel/model/FormulaObjects/mathematicFunctions.js");
	        files.Add("Excel/model/FormulaObjects/lookupandreferenceFunctions.js");
	        files.Add("Excel/model/FormulaObjects/informationFunctions.js");
	        files.Add("Excel/model/FormulaObjects/logicalFunctions.js");
	        files.Add("Excel/model/CellComment.js");
	        files.Add("Common/NumFormat.js");
	        files.Add("Excel/model/Serialize.js");
	        files.Add("Excel/model/WorkbookElems.js");
	        files.Add("Excel/model/Workbook.js");
	        files.Add("Excel/model/CellInfo.js");
	        files.Add("Excel/model/AdvancedOptions.js");
	        files.Add("Excel/model/History.js");
	        files.Add("Excel/model/UndoRedo.js");

	        files.Add("Common/scroll.js");
	        files.Add("Excel/view/StringRender.js");
	        files.Add("Excel/view/CellTextRender.js");
	        files.Add("Excel/view/CellEditorView.js");
	        files.Add("Excel/view/WorksheetView.js");
	        files.Add("Excel/view/HandlerList.js");
	        files.Add("Excel/view/EventsController.js");
	        files.Add("Excel/view/WorkbookView.js");

	        files.Add("Excel/api.js");

	        //<!--for theme-->
	        files.Add("Common/Shapes/EditorSettings.js");
	        files.Add("Common/Shapes/Serialize.js");
	        files.Add("Common/Shapes/SerializeWriter.js");
	        files.Add("Word/Editor/SerializeCommon.js");
	        files.Add("Word/Editor/GraphicObjects/ObjectTypes/Format.js");
	        files.Add("Word/Editor/GraphicObjects/ObjectTypes/CreateGeometry.js");
	        files.Add("Excel/model/DrawingObjects/Format/Geometry.js");
	        files.Add("Excel/model/DrawingObjects/Format/Path.js");
	        files.Add("Word/Editor/GraphicObjects/Math.js");
	        files.Add("Word/Drawing/ArcTo.js");
	        files.Add("Word/Drawing/ColorArray.js");
	        files.Add("Word/apiCommon.js");

	        //<!--for chart-->
	        files.Add("Common/SerializeCommonWordExcel.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.common.core.js");

	        files.Add("Common/Charts/libraries/OfficeExcel.common.key.js");

	        files.Add("Common/Charts/libraries/OfficeExcel.bar.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.hbar.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.line.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.pie.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.scatter.js");
	        files.Add("Common/Charts/libraries/OfficeExcel.chartProperties.js");
        	
	        //<!--for shapes-->
	        files.Add("Excel/model/DrawingObjects/Graphics.js");
	        files.Add("Excel/model/DrawingObjects/Overlay.js");
	        files.Add("Excel/model/DrawingObjects/Controls.js");
	        files.Add("Excel/model/DrawingObjects/DrawingObjectsController.js");
	        files.Add("Excel/model/DrawingObjects/DrawingDocument.js");
	        files.Add("Excel/model/DrawingObjects/States.js");
	        files.Add("Excel/model/DrawingObjects/GlobalLoaders.js");
	        files.Add("Excel/model/DrawingObjects/ShapeDrawer.js");
        		
	        files.Add("Excel/model/DrawingObjects/Format/GroupShape.js");
	        files.Add("Excel/model/DrawingObjects/Format/Image.js");
	        files.Add("Excel/model/DrawingObjects/Format/Shape.js");
            files.Add("Excel/model/DrawingObjects/Format/TextBody.js");
            files.Add("Excel/model/DrawingObjects/Format/Styles.js");
            files.Add("Excel/model/DrawingObjects/Format/Path.js");
            files.Add("Excel/model/DrawingObjects/Format/Geometry.js");
            files.Add("Excel/model/DrawingObjects/Format/Paragraph.js");
            files.Add("Excel/model/DrawingObjects/Format/ParagraphContent.js");
            files.Add("Excel/model/DrawingObjects/Format/Numbering.js");
            files.Add("Excel/model/DrawingObjects/Format/Format.js");
            files.Add("Excel/model/DrawingObjects/Format/DocumentContent.js");
            files.Add("Excel/model/DrawingObjects/Format/FontClassification.js");

            files.Add("Excel/model/DrawingObjects/Tracks/AdjustmentTracks.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/ResizeTracks.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/RotateTracks.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/NewShapeTracks.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/PolyLine.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/Spline.js");
	        files.Add("Excel/model/DrawingObjects/Tracks/MoveTracks.js");
	        files.Add("Excel/model/DrawingObjects/Hit.js");
	        files.Add("Excel/model/DrawingObjects/GlobalCounters.js");
            files.Add("Excel/model/DrawingObjects/Format/Chart.js");
            files.Add("Excel/model/DrawingObjects/Format/ChartLayout.js");
            files.Add("Excel/model/DrawingObjects/Format/ChartTitle.js");
            files.Add("Excel/model/DrawingObjects/Format/ChartLegend.js");

            //files.Add("../ServerComponents/DoctRenderer/COMMON/native_xls/corrector.js");

            StringBuilder oBuilder = new StringBuilder();

            for (int i = 0; i < files.Count; i++)
            {
                StreamReader oReader = new StreamReader(strRoot + files[i]);
                oBuilder.Append(oReader.ReadToEnd());

                oBuilder.Append("\n\n");
            }

            string strDestPath = strApplication + "\\OfficeWeb\\Excel\\sdk-all.js";
            StreamWriter oWriter = new StreamWriter(strDestPath, false, Encoding.UTF8);
            
            oWriter.Write(oBuilder.ToString());
            oWriter.Close();
        }
    }
}
