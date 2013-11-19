using System;
using System.Collections.Generic;
using System.Text;
using AVSOfficeEWSEditor.Editor.Walkers;
using AVSOfficeEWSEditor.Editor.NumberFormat;

namespace AVSOfficeEWSEditor.Editor.Format
{
    /// <summary>
    /// Represents a set of cells formatting properties.
    /// Has permanent link to the range where it was created from
    /// </summary>
    public class Format
    {
        internal Format(RangeWalker range_walker, Cells.CellRange range_coords, Worksheet parent_sheet)
        {
            this.range_walker = range_walker;
            this.workbook = parent_sheet.Workbook;
            this.parent_sheet = parent_sheet;
            this.range_coords = range_coords;
            Font = new Font.Font(range_walker, workbook);
            Alignment = new Alignment.Alignment(range_walker, workbook);
            Protection = new Protection.Protection(range_walker, workbook);
            Borders = new Borders.Borders(range_walker, workbook);
            Fills = new Fills.Fills(range_walker, workbook);
        }

        /// <summary>
        /// Returns Font object that is responsible for getting and setting of all font properties of the cells.
        /// Read-only.
        /// </summary>
        public Font.Font Font
        {
            get;
            private set;
        }

        /// <summary>
        /// Returns Alignment object that is responsible for getting and setting of all Alignment properties of the cells.
        /// Read-only.
        /// </summary>
        public Alignment.Alignment Alignment
        {
            get;
            private set;
        }

        /// <summary>
        /// Returns Protection object that is responsible for getting and setting of all Protection properties of the cells.
        /// Read-only.
        /// </summary>
        public Protection.Protection Protection
        {
            get;
            private set;
        }

        /// <summary>
        /// Returns Borders object that is responsible for getting and setting of all properties of all borders of the cells.
        /// Read-only.
        /// </summary>
        public Borders.Borders Borders
        {
            get;
            private set;
        }

        /// <summary>
        /// Returns Fills object that is responsible for getting and setting of all fill properties of the cells.
        /// Read-only.
        /// </summary>
        public Fills.Fills Fills
        {
            get;
            private set;
        }

        /// <summary>
        /// Get/set the number format ID
        /// Note: CellsUpdatedEvent issued on set
        /// </summary>
        public int? NumberFormatId
        {
            get
            {
                PropertyId property = new PropertyId();
                StyleCollector collector = new StyleCollector(new NumberFormat.NumberFormatEntity(property), property, workbook);
                range_walker.CollectStatisticsIncludingEmpty(collector);

                return property.Value;
            }
            set
            {
                if(value.HasValue && workbook.StyleSheet.IsNumberFormatIdValidForThisBook(value.Value))
                {
                    PropertyId property = new PropertyId(value.Value);

                    StyleSetter setter = new StyleSetter(new NumberFormat.NumberFormatEntity(property), property, workbook);
                    range_walker.SetStyles(setter);

                    parent_sheet.SubjectsStorage.MarkRangeCellsUpdated(range_coords);
                    parent_sheet.IssueCellsUpdatedEvent();
                }
            }
        }

        /// <summary>
        /// Get/set the number format code string
        /// Note: CellsUpdatedEvent issued on set
        /// </summary>
        public string NumberFormatCode
        {
            get
            {
                int? format_id = NumberFormatId;
                if(!format_id.HasValue)
                {
                    return null;
                }
                return workbook.StyleSheet.GetNumberFormatString(format_id.Value);
            }
            set
            {
                if(value != null)
                {
                    int format_id = workbook.StyleSheet.SetNumberFormatString(value);
                    NumberFormatId = format_id;
                }
            }
        }

        /// <summary>
        /// Show more precise values by showing more decimal places
        /// Note: CellsUpdatedEvent issued
        /// </summary>
        public void IncreaseDecimal()
        {
            Range TopLeftRange = parent_sheet.Cell(range_coords.TopLeft);
            string num_fmt_code = TopLeftRange.Format.NumberFormatCode;
            if (num_fmt_code != null)
            {
                if (num_fmt_code.ToLower() == "general")
                {
                    NumberFormatCode = NumberFormatParser.IncreaseDecimalGeneral(TopLeftRange.InternalValue);
                }
                else
                {
                    string positive_format;
                    string negative_format;
                    string zero_format;
                    string text_format;
                    if (NumberFormatParser.SplitFormatString(num_fmt_code, out positive_format, out negative_format, out zero_format, out text_format))
                    {

                        NumberFormatCode = NumberFormatParser.MergeFormatString(NumberFormatParser.IncreaseDecimal(positive_format),
                                                                                NumberFormatParser.IncreaseDecimal(negative_format),
                                                                                NumberFormatParser.IncreaseDecimal(zero_format),
                                                                                NumberFormatParser.IncreaseDecimal(text_format));
                    }
                }
            }
        }

        /// <summary>
        /// Show less precise values by showing fewer decimal places
        /// Note: CellsUpdatedEvent issued
        /// </summary>
        public void DecreaseDecimal()
        {
            Range TopLeftRange = parent_sheet.Cell(range_coords.TopLeft);
            string num_fmt_code = TopLeftRange.Format.NumberFormatCode;
            if (num_fmt_code != null)
            {
                if (num_fmt_code.ToLower() == "general")
                {
                    NumberFormatCode = NumberFormatParser.DecreaseDecimalGeneral(TopLeftRange.InternalValue);
                }
                else
                {
                    string positive_format;
                    string negative_format;
                    string zero_format;
                    string text_format;
                    if (NumberFormatParser.SplitFormatString(num_fmt_code, out positive_format, out negative_format, out zero_format, out text_format))
                    {

                        NumberFormatCode = NumberFormatParser.MergeFormatString(NumberFormatParser.DecreaseDecimal(positive_format),
                                                                                NumberFormatParser.DecreaseDecimal(negative_format),
                                                                                NumberFormatParser.DecreaseDecimal(zero_format),
                                                                                NumberFormatParser.DecreaseDecimal(text_format));
                    }
                }
            }
        }

        private RangeWalker range_walker;
        private Workbook workbook;
        private Cells.CellRange range_coords;
        private Worksheet parent_sheet;
    }
    public class FormatSimple
    { //здесь не все свойства, а только те которые использет сайт
        public Font.FontSimple Font = new AVSOfficeEWSEditor.Editor.Format.Font.FontSimple();
        public Alignment.AlignmentSimple Alignment = new AVSOfficeEWSEditor.Editor.Format.Alignment.AlignmentSimple();
        public Fills.FillsSimple Fills = new AVSOfficeEWSEditor.Editor.Format.Fills.FillsSimple();
        public Borders.BordersSimple Brds = new AVSOfficeEWSEditor.Editor.Format.Borders.BordersSimple();
        public int? NumberFormatId = 0;
        public FormatSimple(Workbook workbook, System.Xml.XmlElement oCurNode)
        {
            m_oWorkbook = workbook;

            Init(oCurNode);
        }
        public FormatSimple(Workbook workbook)
        {
            m_oWorkbook = workbook;
        }
        public FormatSimple()
        {
        }
        public FormatSimple( int nIndex, Workbook workbook )
        {
            m_oWorkbook = workbook;
            if (-1 != nIndex)
            {
                System.Xml.XmlNode CellXfs_xml = workbook.StyleSheet.Get_CellXfs_OrCreate();

                System.Xml.XmlElement cell_xf = (System.Xml.XmlElement)CellXfs_xml.SelectSingleNode("main:xf[position() = " + (nIndex + 1).ToString() + "]", workbook.NamespaceManager);
                if (cell_xf != null) // If the style reference in a cell is wrong
                    Init(cell_xf);
            }
        }
        private void Init(System.Xml.XmlElement oCurNode)
        {
            Font.BoldProperty oBold = new Font.BoldProperty(m_oWorkbook);
            Font.ItalicProperty oItalic = new Font.ItalicProperty(m_oWorkbook);
            Font.UnderlineProperty oUnderline = new Font.UnderlineProperty(m_oWorkbook);
            Font.FontSizeProperty oSize = new Font.FontSizeProperty(m_oWorkbook);
            Font.FontNameProperty oName = new Font.FontNameProperty(m_oWorkbook);
            Colors.ColorProperty oColorProp = new Colors.FgColorProperty("color", m_oWorkbook);
            //Alignment
            Alignment.HorizontalProperty oHorProp = new Alignment.HorizontalProperty(m_oWorkbook);
            Alignment.VerticalProperty oVerProp = new Alignment.VerticalProperty(m_oWorkbook);
            Alignment.WrapTextProperty oWrapProp = new Alignment.WrapTextProperty();
            //Fill
            Fills.PatternProperty oPatTypeProp = new Fills.PatternProperty();
            Fills.PatternFillProperty oPatTypePropProp = new Fills.PatternFillProperty(oPatTypeProp, m_oWorkbook);
            Colors.ColorProperty oPatFgProp = new Colors.FgColorProperty("fgColor", m_oWorkbook);
            Fills.PatternFillProperty oPatFgPropProp = new Fills.PatternFillProperty(oPatFgProp, m_oWorkbook);
            Colors.ColorProperty oPatBgProp = new Colors.BgColorProperty("bgColor", m_oWorkbook);
            Fills.PatternFillProperty oPatBgPropProp = new Fills.PatternFillProperty(oPatBgProp, m_oWorkbook);
            Fills.GradientTypeProperty oGraTypeProp = new Fills.GradientTypeProperty();
            Fills.GradientFillProperty oGraTypePropProp = new Fills.GradientFillProperty(oGraTypeProp, m_oWorkbook);
            Colors.ColorProperty oGraColProp = new Colors.FgColorProperty("color", m_oWorkbook);
            Fills.StopProperty oGraColPropProp = new Fills.StopProperty(0, oGraColProp, m_oWorkbook);
            Fills.GradientFillProperty oGraColPropPropProp = new Fills.GradientFillProperty(oGraColPropProp, m_oWorkbook);
            //Border
            Borders.StyleProperty oBrdrLeftTypeProp = new Borders.StyleProperty(m_oWorkbook);
            Borders.BorderProperty oBrdrLeftTypePropProp = new Borders.BorderProperty(Borders.BorderPosition.left, oBrdrLeftTypeProp, m_oWorkbook);
            Borders.StyleProperty oBrdrTopTypeProp = new Borders.StyleProperty(m_oWorkbook);
            Borders.BorderProperty oBrdrTopTypePropProp = new Borders.BorderProperty(Borders.BorderPosition.top, oBrdrTopTypeProp, m_oWorkbook);
            Borders.StyleProperty oBrdrRightTypeProp = new Borders.StyleProperty(m_oWorkbook);
            Borders.BorderProperty oBrdrRightTypePropProp = new Borders.BorderProperty(Borders.BorderPosition.right, oBrdrRightTypeProp, m_oWorkbook);
            Borders.StyleProperty oBrdrBottomTypeProp = new Borders.StyleProperty(m_oWorkbook);
            Borders.BorderProperty oBrdrBottomTypePropProp = new Borders.BorderProperty(Borders.BorderPosition.bottom, oBrdrBottomTypeProp, m_oWorkbook);
            Colors.ColorProperty oBrdrLeftColorProp = new Colors.FgColorProperty("color", m_oWorkbook);
            Borders.BorderProperty oBrdrLeftColorPropProp = new Borders.BorderProperty(Borders.BorderPosition.left, oBrdrLeftColorProp, m_oWorkbook);
            Colors.ColorProperty oBrdrTopColorProp = new Colors.FgColorProperty("color", m_oWorkbook);
            Borders.BorderProperty oBrdrTopColorPropProp = new Borders.BorderProperty(Borders.BorderPosition.top, oBrdrTopColorProp, m_oWorkbook);
            Colors.ColorProperty oBrdrRightColorProp = new Colors.FgColorProperty("color", m_oWorkbook);
            Borders.BorderProperty oBrdrRightColorPropProp = new Borders.BorderProperty(Borders.BorderPosition.right, oBrdrRightColorProp, m_oWorkbook);
            Colors.ColorProperty oBrdrBottomColorProp = new Colors.FgColorProperty("color", m_oWorkbook);
            Borders.BorderProperty oBrdrBottomColorPropProp = new Borders.BorderProperty(Borders.BorderPosition.bottom, oBrdrBottomColorProp, m_oWorkbook);
            //NumberFormat
            PropertyId oNumFormatProp = new PropertyId();

            Font.FontEntity oFontEntity = new Font.FontEntity(m_oWorkbook);
            Alignment.AlignmentEntity oAlignmentEntity = new Alignment.AlignmentEntity(m_oWorkbook);
            NumberFormat.NumberFormatEntity oNumberFormatEntity = new NumberFormat.NumberFormatEntity(oNumFormatProp);
            Fills.FillEntity oFillEntity = new Fills.FillEntity(m_oWorkbook);
            Borders.BorderEntity oBorderEntity = new Borders.BorderEntity(m_oWorkbook);

            oFontEntity.CollectPropertyFromXF(oCurNode, oBold);
            oFontEntity.CollectPropertyFromXF(oCurNode, oItalic);
            oFontEntity.CollectPropertyFromXF(oCurNode, oUnderline);
            oFontEntity.CollectPropertyFromXF(oCurNode, oSize);
            oFontEntity.CollectPropertyFromXF(oCurNode, oName);
            oFontEntity.CollectPropertyFromXF(oCurNode, oColorProp);
            oAlignmentEntity.CollectPropertyFromXF(oCurNode, oHorProp);
            oAlignmentEntity.CollectPropertyFromXF(oCurNode, oVerProp);
            oAlignmentEntity.CollectPropertyFromXF(oCurNode, oWrapProp);
            oFillEntity.CollectPropertyFromXF(oCurNode, oPatTypePropProp);
            oFillEntity.CollectPropertyFromXF(oCurNode, oPatFgPropProp);
            oFillEntity.CollectPropertyFromXF(oCurNode, oPatBgPropProp);
            oFillEntity.CollectPropertyFromXF(oCurNode, oGraTypePropProp);
            oFillEntity.CollectPropertyFromXF(oCurNode, oGraColPropPropProp);
            oNumberFormatEntity.CollectPropertyFromXF(oCurNode, oNumFormatProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrLeftTypePropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrTopTypePropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrRightTypePropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrBottomTypePropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrLeftColorPropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrTopColorPropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrRightColorPropProp);
            oBorderEntity.CollectPropertyFromXF(oCurNode, oBrdrBottomColorPropProp);


            Font.Bold = oBold.Value;
            Font.Italic = oItalic.Value;
            Font.Underline = oUnderline.Value;
            Font.Size = oSize.Value;
            Font.Name = oName.Value;
            SetColor(ref Font.Color, oColorProp);
            Alignment.Horizontal = oHorProp.Value;
            Alignment.Vertical = oVerProp.Value;
            Alignment.WrapText = oWrapProp.Value;
            Fills.PatternFill.Pattern = oPatTypeProp.Value;
            SetColor(ref Fills.PatternFill.FgColor, oPatFgProp);
            SetColor(ref Fills.PatternFill.BgColor, oPatBgProp);
            Fills.GradientFill.Type = oGraTypeProp.Value;
            SetColor(ref Fills.GradientFill.Color, oGraColProp);
            //Border
            if (oBrdrLeftTypeProp.Value == null)
                Brds.Left.Style = AVSOfficeEWSEditor.Editor.Format.Borders.BorderStyle.none;
            else
                Brds.Left.Style = oBrdrLeftTypeProp.Value;

            if (oBrdrTopTypeProp.Value == null)
                Brds.Top.Style = AVSOfficeEWSEditor.Editor.Format.Borders.BorderStyle.none;
            else
                Brds.Top.Style = oBrdrTopTypeProp.Value;

            if(oBrdrRightTypeProp.Value == null)
                Brds.Right.Style = AVSOfficeEWSEditor.Editor.Format.Borders.BorderStyle.none;
            else
                Brds.Right.Style = oBrdrRightTypeProp.Value;

            if(oBrdrBottomTypeProp.Value == null)
                Brds.Bottom.Style = AVSOfficeEWSEditor.Editor.Format.Borders.BorderStyle.none;
            else
                Brds.Bottom.Style = oBrdrBottomTypeProp.Value;
            SetColor(ref Brds.Left.Color, oBrdrLeftColorProp);
            SetColor(ref Brds.Top.Color, oBrdrTopColorProp);
            SetColor(ref Brds.Right.Color, oBrdrRightColorProp);
            SetColor(ref Brds.Bottom.Color, oBrdrBottomColorProp);
            //NumberFormat
            NumberFormatId = oNumFormatProp.Value;
        }
        private void SetColor(ref Colors.Color oColor, Colors.ColorProperty oColorProp)
        {
            if (oColorProp.IsNull())
            {
                oColorProp.Value = new Colors.FgColor(m_oWorkbook);
                oColor = new Colors.Color(null, null, null, oColorProp, m_oWorkbook);
            }
            else oColor = new Colors.Color(oColorProp.Value, null, null, oColorProp, m_oWorkbook);
        }
        private Workbook m_oWorkbook;

    };

    public struct CCollFormat
    {
        public int m_nMin;
        public int m_nMax;
        public FormatSimple m_oFormatSimple;
    };
}
