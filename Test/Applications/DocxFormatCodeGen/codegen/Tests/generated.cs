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


/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
[System.Xml.Serialization.XmlRootAttribute("chartSpace", Namespace="http://purl.oclc.org/ooxml/drawingml/chart", IsNullable=false)]
public partial class CT_ChartSpace {
    
    private CT_Boolean date1904Field;
    
    private CT_TextLanguageID langField;
    
    private CT_Boolean roundedCornersField;
    
    private AlternateContent alternateContentField;
    
    private CT_Style1 styleField;
    
    private CT_ColorMapping clrMapOvrField;
    
    private CT_PivotSource pivotSourceField;
    
    private CT_Protection protectionField;
    
    private CT_Chart chartField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_ExternalData externalDataField;
    
    private CT_PrintSettings printSettingsField;
    
    private CT_RelId userShapesField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean date1904 {
        get {
            return this.date1904Field;
        }
        set {
            this.date1904Field = value;
        }
    }
    
    /// <remarks/>
    public CT_TextLanguageID lang {
        get {
            return this.langField;
        }
        set {
            this.langField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean roundedCorners {
        get {
            return this.roundedCornersField;
        }
        set {
            this.roundedCornersField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute(Namespace="http://schemas.openxmlformats.org/markup-compatibility/2006")]
    public AlternateContent AlternateContent {
        get {
            return this.alternateContentField;
        }
        set {
            this.alternateContentField = value;
        }
    }
    
    /// <remarks/>
    public CT_Style1 style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
    
    /// <remarks/>
    public CT_ColorMapping clrMapOvr {
        get {
            return this.clrMapOvrField;
        }
        set {
            this.clrMapOvrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PivotSource pivotSource {
        get {
            return this.pivotSourceField;
        }
        set {
            this.pivotSourceField = value;
        }
    }
    
    /// <remarks/>
    public CT_Protection protection {
        get {
            return this.protectionField;
        }
        set {
            this.protectionField = value;
        }
    }
    
    /// <remarks/>
    public CT_Chart chart {
        get {
            return this.chartField;
        }
        set {
            this.chartField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ExternalData externalData {
        get {
            return this.externalDataField;
        }
        set {
            this.externalDataField = value;
        }
    }
    
    /// <remarks/>
    public CT_PrintSettings printSettings {
        get {
            return this.printSettingsField;
        }
        set {
            this.printSettingsField = value;
        }
    }
    
    /// <remarks/>
    public CT_RelId userShapes {
        get {
            return this.userShapesField;
        }
        set {
            this.userShapesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Boolean {
    
    private bool valField;
    
    public CT_Boolean() {
        this.valField = true;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
[System.Xml.Serialization.XmlRootAttribute("chart", Namespace="http://purl.oclc.org/ooxml/drawingml/chart", IsNullable=false)]
public partial class CT_RelId {
    
    private string idField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    public string id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PageSetup {
    
    private uint paperSizeField;
    
    private string paperHeightField;
    
    private string paperWidthField;
    
    private uint firstPageNumberField;
    
    private ST_PageSetupOrientation orientationField;
    
    private bool blackAndWhiteField;
    
    private bool draftField;
    
    private bool useFirstPageNumberField;
    
    private int horizontalDpiField;
    
    private int verticalDpiField;
    
    private uint copiesField;
    
    public CT_PageSetup() {
        this.paperSizeField = ((uint)(1));
        this.firstPageNumberField = ((uint)(1));
        this.orientationField = ST_PageSetupOrientation.@default;
        this.blackAndWhiteField = false;
        this.draftField = false;
        this.useFirstPageNumberField = false;
        this.horizontalDpiField = 600;
        this.verticalDpiField = 600;
        this.copiesField = ((uint)(1));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(uint), "1")]
    public uint paperSize {
        get {
            return this.paperSizeField;
        }
        set {
            this.paperSizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string paperHeight {
        get {
            return this.paperHeightField;
        }
        set {
            this.paperHeightField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string paperWidth {
        get {
            return this.paperWidthField;
        }
        set {
            this.paperWidthField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(uint), "1")]
    public uint firstPageNumber {
        get {
            return this.firstPageNumberField;
        }
        set {
            this.firstPageNumberField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_PageSetupOrientation.@default)]
    public ST_PageSetupOrientation orientation {
        get {
            return this.orientationField;
        }
        set {
            this.orientationField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool blackAndWhite {
        get {
            return this.blackAndWhiteField;
        }
        set {
            this.blackAndWhiteField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool draft {
        get {
            return this.draftField;
        }
        set {
            this.draftField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool useFirstPageNumber {
        get {
            return this.useFirstPageNumberField;
        }
        set {
            this.useFirstPageNumberField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(600)]
    public int horizontalDpi {
        get {
            return this.horizontalDpiField;
        }
        set {
            this.horizontalDpiField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(600)]
    public int verticalDpi {
        get {
            return this.verticalDpiField;
        }
        set {
            this.verticalDpiField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(uint), "1")]
    public uint copies {
        get {
            return this.copiesField;
        }
        set {
            this.copiesField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_PageSetupOrientation {
    
    /// <remarks/>
    @default,
    
    /// <remarks/>
    portrait,
    
    /// <remarks/>
    landscape,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PageMargins {
    
    private double lField;
    
    private double rField;
    
    private double tField;
    
    private double bField;
    
    private double headerField;
    
    private double footerField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double l {
        get {
            return this.lField;
        }
        set {
            this.lField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double r {
        get {
            return this.rField;
        }
        set {
            this.rField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double t {
        get {
            return this.tField;
        }
        set {
            this.tField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double b {
        get {
            return this.bField;
        }
        set {
            this.bField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double header {
        get {
            return this.headerField;
        }
        set {
            this.headerField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double footer {
        get {
            return this.footerField;
        }
        set {
            this.footerField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_HeaderFooter {
    
    private string oddHeaderField;
    
    private string oddFooterField;
    
    private string evenHeaderField;
    
    private string evenFooterField;
    
    private string firstHeaderField;
    
    private string firstFooterField;
    
    private bool alignWithMarginsField;
    
    private bool differentOddEvenField;
    
    private bool differentFirstField;
    
    public CT_HeaderFooter() {
        this.alignWithMarginsField = true;
        this.differentOddEvenField = false;
        this.differentFirstField = false;
    }
    
    /// <remarks/>
    public string oddHeader {
        get {
            return this.oddHeaderField;
        }
        set {
            this.oddHeaderField = value;
        }
    }
    
    /// <remarks/>
    public string oddFooter {
        get {
            return this.oddFooterField;
        }
        set {
            this.oddFooterField = value;
        }
    }
    
    /// <remarks/>
    public string evenHeader {
        get {
            return this.evenHeaderField;
        }
        set {
            this.evenHeaderField = value;
        }
    }
    
    /// <remarks/>
    public string evenFooter {
        get {
            return this.evenFooterField;
        }
        set {
            this.evenFooterField = value;
        }
    }
    
    /// <remarks/>
    public string firstHeader {
        get {
            return this.firstHeaderField;
        }
        set {
            this.firstHeaderField = value;
        }
    }
    
    /// <remarks/>
    public string firstFooter {
        get {
            return this.firstFooterField;
        }
        set {
            this.firstFooterField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool alignWithMargins {
        get {
            return this.alignWithMarginsField;
        }
        set {
            this.alignWithMarginsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool differentOddEven {
        get {
            return this.differentOddEvenField;
        }
        set {
            this.differentOddEvenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool differentFirst {
        get {
            return this.differentFirstField;
        }
        set {
            this.differentFirstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PrintSettings {
    
    private CT_HeaderFooter headerFooterField;
    
    private CT_PageMargins pageMarginsField;
    
    private CT_PageSetup pageSetupField;
    
    /// <remarks/>
    public CT_HeaderFooter headerFooter {
        get {
            return this.headerFooterField;
        }
        set {
            this.headerFooterField = value;
        }
    }
    
    /// <remarks/>
    public CT_PageMargins pageMargins {
        get {
            return this.pageMarginsField;
        }
        set {
            this.pageMarginsField = value;
        }
    }
    
    /// <remarks/>
    public CT_PageSetup pageSetup {
        get {
            return this.pageSetupField;
        }
        set {
            this.pageSetupField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ExternalData {
    
    private CT_Boolean autoUpdateField;
    
    private string idField;
    
    /// <remarks/>
    public CT_Boolean autoUpdate {
        get {
            return this.autoUpdateField;
        }
        set {
            this.autoUpdateField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    public string id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DispBlanksAs {
    
    private ST_DispBlanksAs valField;
    
    public CT_DispBlanksAs() {
        this.valField = ST_DispBlanksAs.zero;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_DispBlanksAs.zero)]
    public ST_DispBlanksAs val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_DispBlanksAs {
    
    /// <remarks/>
    span,
    
    /// <remarks/>
    gap,
    
    /// <remarks/>
    zero,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LegendEntry {
    
    private CT_UnsignedInt idxField;
    
    private object itemField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("delete", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("txPr", typeof(CT_TextBody))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_UnsignedInt {
    
    private uint valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBody {
    
    private CT_TextBodyProperties bodyPrField;
    
    private CT_TextListStyle lstStyleField;
    
    private CT_TextParagraph[] pField;
    
    /// <remarks/>
    public CT_TextBodyProperties bodyPr {
        get {
            return this.bodyPrField;
        }
        set {
            this.bodyPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextListStyle lstStyle {
        get {
            return this.lstStyleField;
        }
        set {
            this.lstStyleField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("p")]
    public CT_TextParagraph[] p {
        get {
            return this.pField;
        }
        set {
            this.pField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBodyProperties {
    
    private CT_PresetTextShape prstTxWarpField;
    
    private CT_TextNoAutofit noAutofitField;
    
    private CT_TextNormalAutofit normAutofitField;
    
    private CT_TextShapeAutofit spAutoFitField;
    
    private CT_Scene3D scene3dField;
    
    private CT_Shape3D sp3dField;
    
    private CT_FlatText flatTxField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private int rotField;
    
    private bool rotFieldSpecified;
    
    private bool spcFirstLastParaField;
    
    private bool spcFirstLastParaFieldSpecified;
    
    private ST_TextVertOverflowType vertOverflowField;
    
    private bool vertOverflowFieldSpecified;
    
    private ST_TextHorzOverflowType horzOverflowField;
    
    private bool horzOverflowFieldSpecified;
    
    private ST_TextVerticalType vertField;
    
    private bool vertFieldSpecified;
    
    private ST_TextWrappingType wrapField;
    
    private bool wrapFieldSpecified;
    
    private string lInsField;
    
    private string tInsField;
    
    private string rInsField;
    
    private string bInsField;
    
    private int numColField;
    
    private bool numColFieldSpecified;
    
    private int spcColField;
    
    private bool spcColFieldSpecified;
    
    private bool rtlColField;
    
    private bool rtlColFieldSpecified;
    
    private bool fromWordArtField;
    
    private bool fromWordArtFieldSpecified;
    
    private ST_TextAnchoringType anchorField;
    
    private bool anchorFieldSpecified;
    
    private bool anchorCtrField;
    
    private bool anchorCtrFieldSpecified;
    
    private bool forceAAField;
    
    private bool forceAAFieldSpecified;
    
    private bool uprightField;
    
    private bool compatLnSpcField;
    
    private bool compatLnSpcFieldSpecified;
    
    public CT_TextBodyProperties() {
        this.uprightField = false;
    }
    
    /// <remarks/>
    public CT_PresetTextShape prstTxWarp {
        get {
            return this.prstTxWarpField;
        }
        set {
            this.prstTxWarpField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextNoAutofit noAutofit {
        get {
            return this.noAutofitField;
        }
        set {
            this.noAutofitField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextNormalAutofit normAutofit {
        get {
            return this.normAutofitField;
        }
        set {
            this.normAutofitField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextShapeAutofit spAutoFit {
        get {
            return this.spAutoFitField;
        }
        set {
            this.spAutoFitField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scene3D scene3d {
        get {
            return this.scene3dField;
        }
        set {
            this.scene3dField = value;
        }
    }
    
    /// <remarks/>
    public CT_Shape3D sp3d {
        get {
            return this.sp3dField;
        }
        set {
            this.sp3dField = value;
        }
    }
    
    /// <remarks/>
    public CT_FlatText flatTx {
        get {
            return this.flatTxField;
        }
        set {
            this.flatTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int rot {
        get {
            return this.rotField;
        }
        set {
            this.rotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool rotSpecified {
        get {
            return this.rotFieldSpecified;
        }
        set {
            this.rotFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool spcFirstLastPara {
        get {
            return this.spcFirstLastParaField;
        }
        set {
            this.spcFirstLastParaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool spcFirstLastParaSpecified {
        get {
            return this.spcFirstLastParaFieldSpecified;
        }
        set {
            this.spcFirstLastParaFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextVertOverflowType vertOverflow {
        get {
            return this.vertOverflowField;
        }
        set {
            this.vertOverflowField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool vertOverflowSpecified {
        get {
            return this.vertOverflowFieldSpecified;
        }
        set {
            this.vertOverflowFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextHorzOverflowType horzOverflow {
        get {
            return this.horzOverflowField;
        }
        set {
            this.horzOverflowField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool horzOverflowSpecified {
        get {
            return this.horzOverflowFieldSpecified;
        }
        set {
            this.horzOverflowFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextVerticalType vert {
        get {
            return this.vertField;
        }
        set {
            this.vertField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool vertSpecified {
        get {
            return this.vertFieldSpecified;
        }
        set {
            this.vertFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextWrappingType wrap {
        get {
            return this.wrapField;
        }
        set {
            this.wrapField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool wrapSpecified {
        get {
            return this.wrapFieldSpecified;
        }
        set {
            this.wrapFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string lIns {
        get {
            return this.lInsField;
        }
        set {
            this.lInsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string tIns {
        get {
            return this.tInsField;
        }
        set {
            this.tInsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string rIns {
        get {
            return this.rInsField;
        }
        set {
            this.rInsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string bIns {
        get {
            return this.bInsField;
        }
        set {
            this.bInsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int numCol {
        get {
            return this.numColField;
        }
        set {
            this.numColField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool numColSpecified {
        get {
            return this.numColFieldSpecified;
        }
        set {
            this.numColFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int spcCol {
        get {
            return this.spcColField;
        }
        set {
            this.spcColField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool spcColSpecified {
        get {
            return this.spcColFieldSpecified;
        }
        set {
            this.spcColFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool rtlCol {
        get {
            return this.rtlColField;
        }
        set {
            this.rtlColField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool rtlColSpecified {
        get {
            return this.rtlColFieldSpecified;
        }
        set {
            this.rtlColFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool fromWordArt {
        get {
            return this.fromWordArtField;
        }
        set {
            this.fromWordArtField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool fromWordArtSpecified {
        get {
            return this.fromWordArtFieldSpecified;
        }
        set {
            this.fromWordArtFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextAnchoringType anchor {
        get {
            return this.anchorField;
        }
        set {
            this.anchorField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool anchorSpecified {
        get {
            return this.anchorFieldSpecified;
        }
        set {
            this.anchorFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool anchorCtr {
        get {
            return this.anchorCtrField;
        }
        set {
            this.anchorCtrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool anchorCtrSpecified {
        get {
            return this.anchorCtrFieldSpecified;
        }
        set {
            this.anchorCtrFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool forceAA {
        get {
            return this.forceAAField;
        }
        set {
            this.forceAAField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool forceAASpecified {
        get {
            return this.forceAAFieldSpecified;
        }
        set {
            this.forceAAFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool upright {
        get {
            return this.uprightField;
        }
        set {
            this.uprightField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool compatLnSpc {
        get {
            return this.compatLnSpcField;
        }
        set {
            this.compatLnSpcField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool compatLnSpcSpecified {
        get {
            return this.compatLnSpcFieldSpecified;
        }
        set {
            this.compatLnSpcFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PresetTextShape {
    
    private CT_GeomGuide[] avLstField;
    
    private ST_TextShapeType prstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("gd", IsNullable=false)]
    public CT_GeomGuide[] avLst {
        get {
            return this.avLstField;
        }
        set {
            this.avLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextShapeType prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GeomGuide {
    
    private string nameField;
    
    private string fmlaField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string fmla {
        get {
            return this.fmlaField;
        }
        set {
            this.fmlaField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextShapeType {
    
    /// <remarks/>
    textNoShape,
    
    /// <remarks/>
    textPlain,
    
    /// <remarks/>
    textStop,
    
    /// <remarks/>
    textTriangle,
    
    /// <remarks/>
    textTriangleInverted,
    
    /// <remarks/>
    textChevron,
    
    /// <remarks/>
    textChevronInverted,
    
    /// <remarks/>
    textRingInside,
    
    /// <remarks/>
    textRingOutside,
    
    /// <remarks/>
    textArchUp,
    
    /// <remarks/>
    textArchDown,
    
    /// <remarks/>
    textCircle,
    
    /// <remarks/>
    textButton,
    
    /// <remarks/>
    textArchUpPour,
    
    /// <remarks/>
    textArchDownPour,
    
    /// <remarks/>
    textCirclePour,
    
    /// <remarks/>
    textButtonPour,
    
    /// <remarks/>
    textCurveUp,
    
    /// <remarks/>
    textCurveDown,
    
    /// <remarks/>
    textCanUp,
    
    /// <remarks/>
    textCanDown,
    
    /// <remarks/>
    textWave1,
    
    /// <remarks/>
    textWave2,
    
    /// <remarks/>
    textDoubleWave1,
    
    /// <remarks/>
    textWave4,
    
    /// <remarks/>
    textInflate,
    
    /// <remarks/>
    textDeflate,
    
    /// <remarks/>
    textInflateBottom,
    
    /// <remarks/>
    textDeflateBottom,
    
    /// <remarks/>
    textInflateTop,
    
    /// <remarks/>
    textDeflateTop,
    
    /// <remarks/>
    textDeflateInflate,
    
    /// <remarks/>
    textDeflateInflateDeflate,
    
    /// <remarks/>
    textFadeRight,
    
    /// <remarks/>
    textFadeLeft,
    
    /// <remarks/>
    textFadeUp,
    
    /// <remarks/>
    textFadeDown,
    
    /// <remarks/>
    textSlantUp,
    
    /// <remarks/>
    textSlantDown,
    
    /// <remarks/>
    textCascadeUp,
    
    /// <remarks/>
    textCascadeDown,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextNoAutofit {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextNormalAutofit {
    
    private string fontScaleField;
    
    private string lnSpcReductionField;
    
    public CT_TextNormalAutofit() {
        this.fontScaleField = "100%";
        this.lnSpcReductionField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string fontScale {
        get {
            return this.fontScaleField;
        }
        set {
            this.fontScaleField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string lnSpcReduction {
        get {
            return this.lnSpcReductionField;
        }
        set {
            this.lnSpcReductionField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextShapeAutofit {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Scene3D {
    
    private CT_Camera cameraField;
    
    private CT_LightRig lightRigField;
    
    private CT_Backdrop backdropField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_Camera camera {
        get {
            return this.cameraField;
        }
        set {
            this.cameraField = value;
        }
    }
    
    /// <remarks/>
    public CT_LightRig lightRig {
        get {
            return this.lightRigField;
        }
        set {
            this.lightRigField = value;
        }
    }
    
    /// <remarks/>
    public CT_Backdrop backdrop {
        get {
            return this.backdropField;
        }
        set {
            this.backdropField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Camera {
    
    private CT_SphereCoords rotField;
    
    private ST_PresetCameraType prstField;
    
    private int fovField;
    
    private bool fovFieldSpecified;
    
    private string zoomField;
    
    public CT_Camera() {
        this.zoomField = "100%";
    }
    
    /// <remarks/>
    public CT_SphereCoords rot {
        get {
            return this.rotField;
        }
        set {
            this.rotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PresetCameraType prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int fov {
        get {
            return this.fovField;
        }
        set {
            this.fovField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool fovSpecified {
        get {
            return this.fovFieldSpecified;
        }
        set {
            this.fovFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string zoom {
        get {
            return this.zoomField;
        }
        set {
            this.zoomField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SphereCoords {
    
    private int latField;
    
    private int lonField;
    
    private int revField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int lat {
        get {
            return this.latField;
        }
        set {
            this.latField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int lon {
        get {
            return this.lonField;
        }
        set {
            this.lonField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int rev {
        get {
            return this.revField;
        }
        set {
            this.revField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetCameraType {
    
    /// <remarks/>
    legacyObliqueTopLeft,
    
    /// <remarks/>
    legacyObliqueTop,
    
    /// <remarks/>
    legacyObliqueTopRight,
    
    /// <remarks/>
    legacyObliqueLeft,
    
    /// <remarks/>
    legacyObliqueFront,
    
    /// <remarks/>
    legacyObliqueRight,
    
    /// <remarks/>
    legacyObliqueBottomLeft,
    
    /// <remarks/>
    legacyObliqueBottom,
    
    /// <remarks/>
    legacyObliqueBottomRight,
    
    /// <remarks/>
    legacyPerspectiveTopLeft,
    
    /// <remarks/>
    legacyPerspectiveTop,
    
    /// <remarks/>
    legacyPerspectiveTopRight,
    
    /// <remarks/>
    legacyPerspectiveLeft,
    
    /// <remarks/>
    legacyPerspectiveFront,
    
    /// <remarks/>
    legacyPerspectiveRight,
    
    /// <remarks/>
    legacyPerspectiveBottomLeft,
    
    /// <remarks/>
    legacyPerspectiveBottom,
    
    /// <remarks/>
    legacyPerspectiveBottomRight,
    
    /// <remarks/>
    orthographicFront,
    
    /// <remarks/>
    isometricTopUp,
    
    /// <remarks/>
    isometricTopDown,
    
    /// <remarks/>
    isometricBottomUp,
    
    /// <remarks/>
    isometricBottomDown,
    
    /// <remarks/>
    isometricLeftUp,
    
    /// <remarks/>
    isometricLeftDown,
    
    /// <remarks/>
    isometricRightUp,
    
    /// <remarks/>
    isometricRightDown,
    
    /// <remarks/>
    isometricOffAxis1Left,
    
    /// <remarks/>
    isometricOffAxis1Right,
    
    /// <remarks/>
    isometricOffAxis1Top,
    
    /// <remarks/>
    isometricOffAxis2Left,
    
    /// <remarks/>
    isometricOffAxis2Right,
    
    /// <remarks/>
    isometricOffAxis2Top,
    
    /// <remarks/>
    isometricOffAxis3Left,
    
    /// <remarks/>
    isometricOffAxis3Right,
    
    /// <remarks/>
    isometricOffAxis3Bottom,
    
    /// <remarks/>
    isometricOffAxis4Left,
    
    /// <remarks/>
    isometricOffAxis4Right,
    
    /// <remarks/>
    isometricOffAxis4Bottom,
    
    /// <remarks/>
    obliqueTopLeft,
    
    /// <remarks/>
    obliqueTop,
    
    /// <remarks/>
    obliqueTopRight,
    
    /// <remarks/>
    obliqueLeft,
    
    /// <remarks/>
    obliqueRight,
    
    /// <remarks/>
    obliqueBottomLeft,
    
    /// <remarks/>
    obliqueBottom,
    
    /// <remarks/>
    obliqueBottomRight,
    
    /// <remarks/>
    perspectiveFront,
    
    /// <remarks/>
    perspectiveLeft,
    
    /// <remarks/>
    perspectiveRight,
    
    /// <remarks/>
    perspectiveAbove,
    
    /// <remarks/>
    perspectiveBelow,
    
    /// <remarks/>
    perspectiveAboveLeftFacing,
    
    /// <remarks/>
    perspectiveAboveRightFacing,
    
    /// <remarks/>
    perspectiveContrastingLeftFacing,
    
    /// <remarks/>
    perspectiveContrastingRightFacing,
    
    /// <remarks/>
    perspectiveHeroicLeftFacing,
    
    /// <remarks/>
    perspectiveHeroicRightFacing,
    
    /// <remarks/>
    perspectiveHeroicExtremeLeftFacing,
    
    /// <remarks/>
    perspectiveHeroicExtremeRightFacing,
    
    /// <remarks/>
    perspectiveRelaxed,
    
    /// <remarks/>
    perspectiveRelaxedModerately,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LightRig {
    
    private CT_SphereCoords rotField;
    
    private ST_LightRigType rigField;
    
    private ST_LightRigDirection dirField;
    
    /// <remarks/>
    public CT_SphereCoords rot {
        get {
            return this.rotField;
        }
        set {
            this.rotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LightRigType rig {
        get {
            return this.rigField;
        }
        set {
            this.rigField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LightRigDirection dir {
        get {
            return this.dirField;
        }
        set {
            this.dirField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LightRigType {
    
    /// <remarks/>
    legacyFlat1,
    
    /// <remarks/>
    legacyFlat2,
    
    /// <remarks/>
    legacyFlat3,
    
    /// <remarks/>
    legacyFlat4,
    
    /// <remarks/>
    legacyNormal1,
    
    /// <remarks/>
    legacyNormal2,
    
    /// <remarks/>
    legacyNormal3,
    
    /// <remarks/>
    legacyNormal4,
    
    /// <remarks/>
    legacyHarsh1,
    
    /// <remarks/>
    legacyHarsh2,
    
    /// <remarks/>
    legacyHarsh3,
    
    /// <remarks/>
    legacyHarsh4,
    
    /// <remarks/>
    threePt,
    
    /// <remarks/>
    balanced,
    
    /// <remarks/>
    soft,
    
    /// <remarks/>
    harsh,
    
    /// <remarks/>
    flood,
    
    /// <remarks/>
    contrasting,
    
    /// <remarks/>
    morning,
    
    /// <remarks/>
    sunrise,
    
    /// <remarks/>
    sunset,
    
    /// <remarks/>
    chilly,
    
    /// <remarks/>
    freezing,
    
    /// <remarks/>
    flat,
    
    /// <remarks/>
    twoPt,
    
    /// <remarks/>
    glow,
    
    /// <remarks/>
    brightRoom,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LightRigDirection {
    
    /// <remarks/>
    tl,
    
    /// <remarks/>
    t,
    
    /// <remarks/>
    tr,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    bl,
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    br,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Backdrop {
    
    private CT_Point3D anchorField;
    
    private CT_Vector3D normField;
    
    private CT_Vector3D upField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_Point3D anchor {
        get {
            return this.anchorField;
        }
        set {
            this.anchorField = value;
        }
    }
    
    /// <remarks/>
    public CT_Vector3D norm {
        get {
            return this.normField;
        }
        set {
            this.normField = value;
        }
    }
    
    /// <remarks/>
    public CT_Vector3D up {
        get {
            return this.upField;
        }
        set {
            this.upField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Point3D {
    
    private string xField;
    
    private string yField;
    
    private string zField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string x {
        get {
            return this.xField;
        }
        set {
            this.xField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string y {
        get {
            return this.yField;
        }
        set {
            this.yField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string z {
        get {
            return this.zField;
        }
        set {
            this.zField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Vector3D {
    
    private string dxField;
    
    private string dyField;
    
    private string dzField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string dx {
        get {
            return this.dxField;
        }
        set {
            this.dxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string dy {
        get {
            return this.dyField;
        }
        set {
            this.dyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string dz {
        get {
            return this.dzField;
        }
        set {
            this.dzField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_OfficeArtExtensionList {
    
    private CT_OfficeArtExtension[] extField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ext")]
    public CT_OfficeArtExtension[] ext {
        get {
            return this.extField;
        }
        set {
            this.extField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_OfficeArtExtension {
    
    private System.Xml.XmlElement[] anyField;
    
    private string uriField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAnyElementAttribute()]
    public System.Xml.XmlElement[] Any {
        get {
            return this.anyField;
        }
        set {
            this.anyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string uri {
        get {
            return this.uriField;
        }
        set {
            this.uriField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Shape3D {
    
    private CT_Bevel bevelTField;
    
    private CT_Bevel bevelBField;
    
    private CT_Color extrusionClrField;
    
    private CT_Color contourClrField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private string zField;
    
    private long extrusionHField;
    
    private long contourWField;
    
    private ST_PresetMaterialType prstMaterialField;
    
    public CT_Shape3D() {
        this.zField = "0";
        this.extrusionHField = ((long)(0));
        this.contourWField = ((long)(0));
        this.prstMaterialField = ST_PresetMaterialType.warmMatte;
    }
    
    /// <remarks/>
    public CT_Bevel bevelT {
        get {
            return this.bevelTField;
        }
        set {
            this.bevelTField = value;
        }
    }
    
    /// <remarks/>
    public CT_Bevel bevelB {
        get {
            return this.bevelBField;
        }
        set {
            this.bevelBField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color extrusionClr {
        get {
            return this.extrusionClrField;
        }
        set {
            this.extrusionClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color contourClr {
        get {
            return this.contourClrField;
        }
        set {
            this.contourClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string z {
        get {
            return this.zField;
        }
        set {
            this.zField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long extrusionH {
        get {
            return this.extrusionHField;
        }
        set {
            this.extrusionHField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long contourW {
        get {
            return this.contourWField;
        }
        set {
            this.contourWField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_PresetMaterialType.warmMatte)]
    public ST_PresetMaterialType prstMaterial {
        get {
            return this.prstMaterialField;
        }
        set {
            this.prstMaterialField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Bevel {
    
    private long wField;
    
    private long hField;
    
    private ST_BevelPresetType prstField;
    
    public CT_Bevel() {
        this.wField = ((long)(76200));
        this.hField = ((long)(76200));
        this.prstField = ST_BevelPresetType.circle;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "76200")]
    public long w {
        get {
            return this.wField;
        }
        set {
            this.wField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "76200")]
    public long h {
        get {
            return this.hField;
        }
        set {
            this.hField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_BevelPresetType.circle)]
    public ST_BevelPresetType prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_BevelPresetType {
    
    /// <remarks/>
    relaxedInset,
    
    /// <remarks/>
    circle,
    
    /// <remarks/>
    slope,
    
    /// <remarks/>
    cross,
    
    /// <remarks/>
    angle,
    
    /// <remarks/>
    softRound,
    
    /// <remarks/>
    convex,
    
    /// <remarks/>
    coolSlant,
    
    /// <remarks/>
    divot,
    
    /// <remarks/>
    riblet,
    
    /// <remarks/>
    hardEdge,
    
    /// <remarks/>
    artDeco,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Color {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ScRgbColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private string rField;
    
    private string gField;
    
    private string bField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string r {
        get {
            return this.rField;
        }
        set {
            this.rField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string g {
        get {
            return this.gField;
        }
        set {
            this.gField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string b {
        get {
            return this.bField;
        }
        set {
            this.bField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PositiveFixedPercentage {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ComplementTransform {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_InverseTransform {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GrayscaleTransform {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_FixedPercentage {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PositivePercentage {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PositiveFixedAngle {
    
    private int valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Angle {
    
    private int valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Percentage {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GammaTransform {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_InverseGammaTransform {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SRgbColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private byte[] valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="hexBinary")]
    public byte[] val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_HslColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private int hue1Field;
    
    private string sat1Field;
    
    private string lum1Field;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute("hue")]
    public int hue1 {
        get {
            return this.hue1Field;
        }
        set {
            this.hue1Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute("sat")]
    public string sat1 {
        get {
            return this.sat1Field;
        }
        set {
            this.sat1Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute("lum")]
    public string lum1 {
        get {
            return this.lum1Field;
        }
        set {
            this.lum1Field = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SystemColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private ST_SystemColorVal valField;
    
    private byte[] lastClrField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_SystemColorVal val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="hexBinary")]
    public byte[] lastClr {
        get {
            return this.lastClrField;
        }
        set {
            this.lastClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_SystemColorVal {
    
    /// <remarks/>
    scrollBar,
    
    /// <remarks/>
    background,
    
    /// <remarks/>
    activeCaption,
    
    /// <remarks/>
    inactiveCaption,
    
    /// <remarks/>
    menu,
    
    /// <remarks/>
    window,
    
    /// <remarks/>
    windowFrame,
    
    /// <remarks/>
    menuText,
    
    /// <remarks/>
    windowText,
    
    /// <remarks/>
    captionText,
    
    /// <remarks/>
    activeBorder,
    
    /// <remarks/>
    inactiveBorder,
    
    /// <remarks/>
    appWorkspace,
    
    /// <remarks/>
    highlight,
    
    /// <remarks/>
    highlightText,
    
    /// <remarks/>
    btnFace,
    
    /// <remarks/>
    btnShadow,
    
    /// <remarks/>
    grayText,
    
    /// <remarks/>
    btnText,
    
    /// <remarks/>
    inactiveCaptionText,
    
    /// <remarks/>
    btnHighlight,
    
    /// <remarks/>
    [System.Xml.Serialization.XmlEnumAttribute("3dDkShadow")]
    Item3dDkShadow,
    
    /// <remarks/>
    [System.Xml.Serialization.XmlEnumAttribute("3dLight")]
    Item3dLight,
    
    /// <remarks/>
    infoText,
    
    /// <remarks/>
    infoBk,
    
    /// <remarks/>
    hotLight,
    
    /// <remarks/>
    gradientActiveCaption,
    
    /// <remarks/>
    gradientInactiveCaption,
    
    /// <remarks/>
    menuHighlight,
    
    /// <remarks/>
    menuBar,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SchemeColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private ST_SchemeColorVal valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_SchemeColorVal val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_SchemeColorVal {
    
    /// <remarks/>
    bg1,
    
    /// <remarks/>
    tx1,
    
    /// <remarks/>
    bg2,
    
    /// <remarks/>
    tx2,
    
    /// <remarks/>
    accent1,
    
    /// <remarks/>
    accent2,
    
    /// <remarks/>
    accent3,
    
    /// <remarks/>
    accent4,
    
    /// <remarks/>
    accent5,
    
    /// <remarks/>
    accent6,
    
    /// <remarks/>
    hlink,
    
    /// <remarks/>
    folHlink,
    
    /// <remarks/>
    phClr,
    
    /// <remarks/>
    dk1,
    
    /// <remarks/>
    lt1,
    
    /// <remarks/>
    dk2,
    
    /// <remarks/>
    lt2,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PresetColor {
    
    private CT_PositiveFixedPercentage[] tintField;
    
    private CT_PositiveFixedPercentage[] shadeField;
    
    private CT_ComplementTransform[] compField;
    
    private CT_InverseTransform[] invField;
    
    private CT_GrayscaleTransform[] grayField;
    
    private CT_PositiveFixedPercentage[] alphaField;
    
    private CT_FixedPercentage[] alphaOffField;
    
    private CT_PositivePercentage[] alphaModField;
    
    private CT_PositiveFixedAngle[] hueField;
    
    private CT_Angle[] hueOffField;
    
    private CT_PositivePercentage[] hueModField;
    
    private CT_Percentage[] satField;
    
    private CT_Percentage[] satOffField;
    
    private CT_Percentage[] satModField;
    
    private CT_Percentage[] lumField;
    
    private CT_Percentage[] lumOffField;
    
    private CT_Percentage[] lumModField;
    
    private CT_Percentage[] redField;
    
    private CT_Percentage[] redOffField;
    
    private CT_Percentage[] redModField;
    
    private CT_Percentage[] greenField;
    
    private CT_Percentage[] greenOffField;
    
    private CT_Percentage[] greenModField;
    
    private CT_Percentage[] blueField;
    
    private CT_Percentage[] blueOffField;
    
    private CT_Percentage[] blueModField;
    
    private CT_GammaTransform[] gammaField;
    
    private CT_InverseGammaTransform[] invGammaField;
    
    private ST_PresetColorVal valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("tint")]
    public CT_PositiveFixedPercentage[] tint {
        get {
            return this.tintField;
        }
        set {
            this.tintField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("shade")]
    public CT_PositiveFixedPercentage[] shade {
        get {
            return this.shadeField;
        }
        set {
            this.shadeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("comp")]
    public CT_ComplementTransform[] comp {
        get {
            return this.compField;
        }
        set {
            this.compField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("inv")]
    public CT_InverseTransform[] inv {
        get {
            return this.invField;
        }
        set {
            this.invField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gray")]
    public CT_GrayscaleTransform[] gray {
        get {
            return this.grayField;
        }
        set {
            this.grayField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alpha")]
    public CT_PositiveFixedPercentage[] alpha {
        get {
            return this.alphaField;
        }
        set {
            this.alphaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaOff")]
    public CT_FixedPercentage[] alphaOff {
        get {
            return this.alphaOffField;
        }
        set {
            this.alphaOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaMod")]
    public CT_PositivePercentage[] alphaMod {
        get {
            return this.alphaModField;
        }
        set {
            this.alphaModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hue")]
    public CT_PositiveFixedAngle[] hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueOff")]
    public CT_Angle[] hueOff {
        get {
            return this.hueOffField;
        }
        set {
            this.hueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hueMod")]
    public CT_PositivePercentage[] hueMod {
        get {
            return this.hueModField;
        }
        set {
            this.hueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sat")]
    public CT_Percentage[] sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satOff")]
    public CT_Percentage[] satOff {
        get {
            return this.satOffField;
        }
        set {
            this.satOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("satMod")]
    public CT_Percentage[] satMod {
        get {
            return this.satModField;
        }
        set {
            this.satModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lum")]
    public CT_Percentage[] lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumOff")]
    public CT_Percentage[] lumOff {
        get {
            return this.lumOffField;
        }
        set {
            this.lumOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("lumMod")]
    public CT_Percentage[] lumMod {
        get {
            return this.lumModField;
        }
        set {
            this.lumModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("red")]
    public CT_Percentage[] red {
        get {
            return this.redField;
        }
        set {
            this.redField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redOff")]
    public CT_Percentage[] redOff {
        get {
            return this.redOffField;
        }
        set {
            this.redOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("redMod")]
    public CT_Percentage[] redMod {
        get {
            return this.redModField;
        }
        set {
            this.redModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("green")]
    public CT_Percentage[] green {
        get {
            return this.greenField;
        }
        set {
            this.greenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenOff")]
    public CT_Percentage[] greenOff {
        get {
            return this.greenOffField;
        }
        set {
            this.greenOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("greenMod")]
    public CT_Percentage[] greenMod {
        get {
            return this.greenModField;
        }
        set {
            this.greenModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blue")]
    public CT_Percentage[] blue {
        get {
            return this.blueField;
        }
        set {
            this.blueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueOff")]
    public CT_Percentage[] blueOff {
        get {
            return this.blueOffField;
        }
        set {
            this.blueOffField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blueMod")]
    public CT_Percentage[] blueMod {
        get {
            return this.blueModField;
        }
        set {
            this.blueModField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("gamma")]
    public CT_GammaTransform[] gamma {
        get {
            return this.gammaField;
        }
        set {
            this.gammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("invGamma")]
    public CT_InverseGammaTransform[] invGamma {
        get {
            return this.invGammaField;
        }
        set {
            this.invGammaField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PresetColorVal val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetColorVal {
    
    /// <remarks/>
    aliceBlue,
    
    /// <remarks/>
    antiqueWhite,
    
    /// <remarks/>
    aqua,
    
    /// <remarks/>
    aquamarine,
    
    /// <remarks/>
    azure,
    
    /// <remarks/>
    beige,
    
    /// <remarks/>
    bisque,
    
    /// <remarks/>
    black,
    
    /// <remarks/>
    blanchedAlmond,
    
    /// <remarks/>
    blue,
    
    /// <remarks/>
    blueViolet,
    
    /// <remarks/>
    brown,
    
    /// <remarks/>
    burlyWood,
    
    /// <remarks/>
    cadetBlue,
    
    /// <remarks/>
    chartreuse,
    
    /// <remarks/>
    chocolate,
    
    /// <remarks/>
    coral,
    
    /// <remarks/>
    cornflowerBlue,
    
    /// <remarks/>
    cornsilk,
    
    /// <remarks/>
    crimson,
    
    /// <remarks/>
    cyan,
    
    /// <remarks/>
    darkBlue,
    
    /// <remarks/>
    darkCyan,
    
    /// <remarks/>
    darkGoldenrod,
    
    /// <remarks/>
    darkGray,
    
    /// <remarks/>
    darkGrey,
    
    /// <remarks/>
    darkGreen,
    
    /// <remarks/>
    darkKhaki,
    
    /// <remarks/>
    darkMagenta,
    
    /// <remarks/>
    darkOliveGreen,
    
    /// <remarks/>
    darkOrange,
    
    /// <remarks/>
    darkOrchid,
    
    /// <remarks/>
    darkRed,
    
    /// <remarks/>
    darkSalmon,
    
    /// <remarks/>
    darkSeaGreen,
    
    /// <remarks/>
    darkSlateBlue,
    
    /// <remarks/>
    darkSlateGray,
    
    /// <remarks/>
    darkSlateGrey,
    
    /// <remarks/>
    darkTurquoise,
    
    /// <remarks/>
    darkViolet,
    
    /// <remarks/>
    dkBlue,
    
    /// <remarks/>
    dkCyan,
    
    /// <remarks/>
    dkGoldenrod,
    
    /// <remarks/>
    dkGray,
    
    /// <remarks/>
    dkGrey,
    
    /// <remarks/>
    dkGreen,
    
    /// <remarks/>
    dkKhaki,
    
    /// <remarks/>
    dkMagenta,
    
    /// <remarks/>
    dkOliveGreen,
    
    /// <remarks/>
    dkOrange,
    
    /// <remarks/>
    dkOrchid,
    
    /// <remarks/>
    dkRed,
    
    /// <remarks/>
    dkSalmon,
    
    /// <remarks/>
    dkSeaGreen,
    
    /// <remarks/>
    dkSlateBlue,
    
    /// <remarks/>
    dkSlateGray,
    
    /// <remarks/>
    dkSlateGrey,
    
    /// <remarks/>
    dkTurquoise,
    
    /// <remarks/>
    dkViolet,
    
    /// <remarks/>
    deepPink,
    
    /// <remarks/>
    deepSkyBlue,
    
    /// <remarks/>
    dimGray,
    
    /// <remarks/>
    dimGrey,
    
    /// <remarks/>
    dodgerBlue,
    
    /// <remarks/>
    firebrick,
    
    /// <remarks/>
    floralWhite,
    
    /// <remarks/>
    forestGreen,
    
    /// <remarks/>
    fuchsia,
    
    /// <remarks/>
    gainsboro,
    
    /// <remarks/>
    ghostWhite,
    
    /// <remarks/>
    gold,
    
    /// <remarks/>
    goldenrod,
    
    /// <remarks/>
    gray,
    
    /// <remarks/>
    grey,
    
    /// <remarks/>
    green,
    
    /// <remarks/>
    greenYellow,
    
    /// <remarks/>
    honeydew,
    
    /// <remarks/>
    hotPink,
    
    /// <remarks/>
    indianRed,
    
    /// <remarks/>
    indigo,
    
    /// <remarks/>
    ivory,
    
    /// <remarks/>
    khaki,
    
    /// <remarks/>
    lavender,
    
    /// <remarks/>
    lavenderBlush,
    
    /// <remarks/>
    lawnGreen,
    
    /// <remarks/>
    lemonChiffon,
    
    /// <remarks/>
    lightBlue,
    
    /// <remarks/>
    lightCoral,
    
    /// <remarks/>
    lightCyan,
    
    /// <remarks/>
    lightGoldenrodYellow,
    
    /// <remarks/>
    lightGray,
    
    /// <remarks/>
    lightGrey,
    
    /// <remarks/>
    lightGreen,
    
    /// <remarks/>
    lightPink,
    
    /// <remarks/>
    lightSalmon,
    
    /// <remarks/>
    lightSeaGreen,
    
    /// <remarks/>
    lightSkyBlue,
    
    /// <remarks/>
    lightSlateGray,
    
    /// <remarks/>
    lightSlateGrey,
    
    /// <remarks/>
    lightSteelBlue,
    
    /// <remarks/>
    lightYellow,
    
    /// <remarks/>
    ltBlue,
    
    /// <remarks/>
    ltCoral,
    
    /// <remarks/>
    ltCyan,
    
    /// <remarks/>
    ltGoldenrodYellow,
    
    /// <remarks/>
    ltGray,
    
    /// <remarks/>
    ltGrey,
    
    /// <remarks/>
    ltGreen,
    
    /// <remarks/>
    ltPink,
    
    /// <remarks/>
    ltSalmon,
    
    /// <remarks/>
    ltSeaGreen,
    
    /// <remarks/>
    ltSkyBlue,
    
    /// <remarks/>
    ltSlateGray,
    
    /// <remarks/>
    ltSlateGrey,
    
    /// <remarks/>
    ltSteelBlue,
    
    /// <remarks/>
    ltYellow,
    
    /// <remarks/>
    lime,
    
    /// <remarks/>
    limeGreen,
    
    /// <remarks/>
    linen,
    
    /// <remarks/>
    magenta,
    
    /// <remarks/>
    maroon,
    
    /// <remarks/>
    medAquamarine,
    
    /// <remarks/>
    medBlue,
    
    /// <remarks/>
    medOrchid,
    
    /// <remarks/>
    medPurple,
    
    /// <remarks/>
    medSeaGreen,
    
    /// <remarks/>
    medSlateBlue,
    
    /// <remarks/>
    medSpringGreen,
    
    /// <remarks/>
    medTurquoise,
    
    /// <remarks/>
    medVioletRed,
    
    /// <remarks/>
    mediumAquamarine,
    
    /// <remarks/>
    mediumBlue,
    
    /// <remarks/>
    mediumOrchid,
    
    /// <remarks/>
    mediumPurple,
    
    /// <remarks/>
    mediumSeaGreen,
    
    /// <remarks/>
    mediumSlateBlue,
    
    /// <remarks/>
    mediumSpringGreen,
    
    /// <remarks/>
    mediumTurquoise,
    
    /// <remarks/>
    mediumVioletRed,
    
    /// <remarks/>
    midnightBlue,
    
    /// <remarks/>
    mintCream,
    
    /// <remarks/>
    mistyRose,
    
    /// <remarks/>
    moccasin,
    
    /// <remarks/>
    navajoWhite,
    
    /// <remarks/>
    navy,
    
    /// <remarks/>
    oldLace,
    
    /// <remarks/>
    olive,
    
    /// <remarks/>
    oliveDrab,
    
    /// <remarks/>
    orange,
    
    /// <remarks/>
    orangeRed,
    
    /// <remarks/>
    orchid,
    
    /// <remarks/>
    paleGoldenrod,
    
    /// <remarks/>
    paleGreen,
    
    /// <remarks/>
    paleTurquoise,
    
    /// <remarks/>
    paleVioletRed,
    
    /// <remarks/>
    papayaWhip,
    
    /// <remarks/>
    peachPuff,
    
    /// <remarks/>
    peru,
    
    /// <remarks/>
    pink,
    
    /// <remarks/>
    plum,
    
    /// <remarks/>
    powderBlue,
    
    /// <remarks/>
    purple,
    
    /// <remarks/>
    red,
    
    /// <remarks/>
    rosyBrown,
    
    /// <remarks/>
    royalBlue,
    
    /// <remarks/>
    saddleBrown,
    
    /// <remarks/>
    salmon,
    
    /// <remarks/>
    sandyBrown,
    
    /// <remarks/>
    seaGreen,
    
    /// <remarks/>
    seaShell,
    
    /// <remarks/>
    sienna,
    
    /// <remarks/>
    silver,
    
    /// <remarks/>
    skyBlue,
    
    /// <remarks/>
    slateBlue,
    
    /// <remarks/>
    slateGray,
    
    /// <remarks/>
    slateGrey,
    
    /// <remarks/>
    snow,
    
    /// <remarks/>
    springGreen,
    
    /// <remarks/>
    steelBlue,
    
    /// <remarks/>
    tan,
    
    /// <remarks/>
    teal,
    
    /// <remarks/>
    thistle,
    
    /// <remarks/>
    tomato,
    
    /// <remarks/>
    turquoise,
    
    /// <remarks/>
    violet,
    
    /// <remarks/>
    wheat,
    
    /// <remarks/>
    white,
    
    /// <remarks/>
    whiteSmoke,
    
    /// <remarks/>
    yellow,
    
    /// <remarks/>
    yellowGreen,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetMaterialType {
    
    /// <remarks/>
    legacyMatte,
    
    /// <remarks/>
    legacyPlastic,
    
    /// <remarks/>
    legacyMetal,
    
    /// <remarks/>
    legacyWireframe,
    
    /// <remarks/>
    matte,
    
    /// <remarks/>
    plastic,
    
    /// <remarks/>
    metal,
    
    /// <remarks/>
    warmMatte,
    
    /// <remarks/>
    translucentPowder,
    
    /// <remarks/>
    powder,
    
    /// <remarks/>
    dkEdge,
    
    /// <remarks/>
    softEdge,
    
    /// <remarks/>
    clear,
    
    /// <remarks/>
    flat,
    
    /// <remarks/>
    softmetal,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_FlatText {
    
    private string zField;
    
    public CT_FlatText() {
        this.zField = "0";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string z {
        get {
            return this.zField;
        }
        set {
            this.zField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextVertOverflowType {
    
    /// <remarks/>
    overflow,
    
    /// <remarks/>
    ellipsis,
    
    /// <remarks/>
    clip,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextHorzOverflowType {
    
    /// <remarks/>
    overflow,
    
    /// <remarks/>
    clip,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextVerticalType {
    
    /// <remarks/>
    horz,
    
    /// <remarks/>
    vert,
    
    /// <remarks/>
    vert270,
    
    /// <remarks/>
    wordArtVert,
    
    /// <remarks/>
    eaVert,
    
    /// <remarks/>
    mongolianVert,
    
    /// <remarks/>
    wordArtVertRtl,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextWrappingType {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    square,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextAnchoringType {
    
    /// <remarks/>
    t,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    just,
    
    /// <remarks/>
    dist,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextListStyle {
    
    private CT_TextParagraphProperties defPPrField;
    
    private CT_TextParagraphProperties lvl1pPrField;
    
    private CT_TextParagraphProperties lvl2pPrField;
    
    private CT_TextParagraphProperties lvl3pPrField;
    
    private CT_TextParagraphProperties lvl4pPrField;
    
    private CT_TextParagraphProperties lvl5pPrField;
    
    private CT_TextParagraphProperties lvl6pPrField;
    
    private CT_TextParagraphProperties lvl7pPrField;
    
    private CT_TextParagraphProperties lvl8pPrField;
    
    private CT_TextParagraphProperties lvl9pPrField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_TextParagraphProperties defPPr {
        get {
            return this.defPPrField;
        }
        set {
            this.defPPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl1pPr {
        get {
            return this.lvl1pPrField;
        }
        set {
            this.lvl1pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl2pPr {
        get {
            return this.lvl2pPrField;
        }
        set {
            this.lvl2pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl3pPr {
        get {
            return this.lvl3pPrField;
        }
        set {
            this.lvl3pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl4pPr {
        get {
            return this.lvl4pPrField;
        }
        set {
            this.lvl4pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl5pPr {
        get {
            return this.lvl5pPrField;
        }
        set {
            this.lvl5pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl6pPr {
        get {
            return this.lvl6pPrField;
        }
        set {
            this.lvl6pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl7pPr {
        get {
            return this.lvl7pPrField;
        }
        set {
            this.lvl7pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl8pPr {
        get {
            return this.lvl8pPrField;
        }
        set {
            this.lvl8pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties lvl9pPr {
        get {
            return this.lvl9pPrField;
        }
        set {
            this.lvl9pPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextParagraphProperties {
    
    private CT_TextSpacing lnSpcField;
    
    private CT_TextSpacing spcBefField;
    
    private CT_TextSpacing spcAftField;
    
    private CT_TextBulletColorFollowText buClrTxField;
    
    private CT_Color buClrField;
    
    private CT_TextBulletSizeFollowText buSzTxField;
    
    private CT_TextBulletSizePercent buSzPctField;
    
    private CT_TextBulletSizePoint buSzPtsField;
    
    private CT_TextBulletTypefaceFollowText buFontTxField;
    
    private CT_TextFont buFontField;
    
    private CT_TextNoBullet buNoneField;
    
    private CT_TextAutonumberBullet buAutoNumField;
    
    private CT_TextCharBullet buCharField;
    
    private CT_TextBlipBullet buBlipField;
    
    private CT_TextTabStop[] tabLstField;
    
    private CT_TextCharacterProperties defRPrField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private int marLField;
    
    private bool marLFieldSpecified;
    
    private int marRField;
    
    private bool marRFieldSpecified;
    
    private int lvlField;
    
    private bool lvlFieldSpecified;
    
    private int indentField;
    
    private bool indentFieldSpecified;
    
    private ST_TextAlignType algnField;
    
    private bool algnFieldSpecified;
    
    private string defTabSzField;
    
    private bool rtlField;
    
    private bool rtlFieldSpecified;
    
    private bool eaLnBrkField;
    
    private bool eaLnBrkFieldSpecified;
    
    private ST_TextFontAlignType fontAlgnField;
    
    private bool fontAlgnFieldSpecified;
    
    private bool latinLnBrkField;
    
    private bool latinLnBrkFieldSpecified;
    
    private bool hangingPunctField;
    
    private bool hangingPunctFieldSpecified;
    
    /// <remarks/>
    public CT_TextSpacing lnSpc {
        get {
            return this.lnSpcField;
        }
        set {
            this.lnSpcField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextSpacing spcBef {
        get {
            return this.spcBefField;
        }
        set {
            this.spcBefField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextSpacing spcAft {
        get {
            return this.spcAftField;
        }
        set {
            this.spcAftField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBulletColorFollowText buClrTx {
        get {
            return this.buClrTxField;
        }
        set {
            this.buClrTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color buClr {
        get {
            return this.buClrField;
        }
        set {
            this.buClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBulletSizeFollowText buSzTx {
        get {
            return this.buSzTxField;
        }
        set {
            this.buSzTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBulletSizePercent buSzPct {
        get {
            return this.buSzPctField;
        }
        set {
            this.buSzPctField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBulletSizePoint buSzPts {
        get {
            return this.buSzPtsField;
        }
        set {
            this.buSzPtsField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBulletTypefaceFollowText buFontTx {
        get {
            return this.buFontTxField;
        }
        set {
            this.buFontTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextFont buFont {
        get {
            return this.buFontField;
        }
        set {
            this.buFontField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextNoBullet buNone {
        get {
            return this.buNoneField;
        }
        set {
            this.buNoneField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextAutonumberBullet buAutoNum {
        get {
            return this.buAutoNumField;
        }
        set {
            this.buAutoNumField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextCharBullet buChar {
        get {
            return this.buCharField;
        }
        set {
            this.buCharField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBlipBullet buBlip {
        get {
            return this.buBlipField;
        }
        set {
            this.buBlipField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("tab", IsNullable=false)]
    public CT_TextTabStop[] tabLst {
        get {
            return this.tabLstField;
        }
        set {
            this.tabLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextCharacterProperties defRPr {
        get {
            return this.defRPrField;
        }
        set {
            this.defRPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int marL {
        get {
            return this.marLField;
        }
        set {
            this.marLField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool marLSpecified {
        get {
            return this.marLFieldSpecified;
        }
        set {
            this.marLFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int marR {
        get {
            return this.marRField;
        }
        set {
            this.marRField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool marRSpecified {
        get {
            return this.marRFieldSpecified;
        }
        set {
            this.marRFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int lvl {
        get {
            return this.lvlField;
        }
        set {
            this.lvlField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool lvlSpecified {
        get {
            return this.lvlFieldSpecified;
        }
        set {
            this.lvlFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int indent {
        get {
            return this.indentField;
        }
        set {
            this.indentField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool indentSpecified {
        get {
            return this.indentFieldSpecified;
        }
        set {
            this.indentFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextAlignType algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool algnSpecified {
        get {
            return this.algnFieldSpecified;
        }
        set {
            this.algnFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string defTabSz {
        get {
            return this.defTabSzField;
        }
        set {
            this.defTabSzField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool rtl {
        get {
            return this.rtlField;
        }
        set {
            this.rtlField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool rtlSpecified {
        get {
            return this.rtlFieldSpecified;
        }
        set {
            this.rtlFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool eaLnBrk {
        get {
            return this.eaLnBrkField;
        }
        set {
            this.eaLnBrkField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool eaLnBrkSpecified {
        get {
            return this.eaLnBrkFieldSpecified;
        }
        set {
            this.eaLnBrkFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextFontAlignType fontAlgn {
        get {
            return this.fontAlgnField;
        }
        set {
            this.fontAlgnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool fontAlgnSpecified {
        get {
            return this.fontAlgnFieldSpecified;
        }
        set {
            this.fontAlgnFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool latinLnBrk {
        get {
            return this.latinLnBrkField;
        }
        set {
            this.latinLnBrkField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool latinLnBrkSpecified {
        get {
            return this.latinLnBrkFieldSpecified;
        }
        set {
            this.latinLnBrkFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool hangingPunct {
        get {
            return this.hangingPunctField;
        }
        set {
            this.hangingPunctField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool hangingPunctSpecified {
        get {
            return this.hangingPunctFieldSpecified;
        }
        set {
            this.hangingPunctFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextSpacing {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("spcPct", typeof(CT_TextSpacingPercent))]
    [System.Xml.Serialization.XmlElementAttribute("spcPts", typeof(CT_TextSpacingPoint))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextSpacingPercent {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextSpacingPoint {
    
    private int valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBulletColorFollowText {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBulletSizeFollowText {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBulletSizePercent {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBulletSizePoint {
    
    private int valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBulletTypefaceFollowText {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextFont {
    
    private string typefaceField;
    
    private byte[] panoseField;
    
    private sbyte pitchFamilyField;
    
    private sbyte charsetField;
    
    public CT_TextFont() {
        this.pitchFamilyField = ((sbyte)(0));
        this.charsetField = ((sbyte)(1));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string typeface {
        get {
            return this.typefaceField;
        }
        set {
            this.typefaceField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="hexBinary")]
    public byte[] panose {
        get {
            return this.panoseField;
        }
        set {
            this.panoseField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(sbyte), "0")]
    public sbyte pitchFamily {
        get {
            return this.pitchFamilyField;
        }
        set {
            this.pitchFamilyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(sbyte), "1")]
    public sbyte charset {
        get {
            return this.charsetField;
        }
        set {
            this.charsetField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextNoBullet {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextAutonumberBullet {
    
    private ST_TextAutonumberScheme typeField;
    
    private int startAtField;
    
    public CT_TextAutonumberBullet() {
        this.startAtField = 1;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextAutonumberScheme type {
        get {
            return this.typeField;
        }
        set {
            this.typeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(1)]
    public int startAt {
        get {
            return this.startAtField;
        }
        set {
            this.startAtField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextAutonumberScheme {
    
    /// <remarks/>
    alphaLcParenBoth,
    
    /// <remarks/>
    alphaUcParenBoth,
    
    /// <remarks/>
    alphaLcParenR,
    
    /// <remarks/>
    alphaUcParenR,
    
    /// <remarks/>
    alphaLcPeriod,
    
    /// <remarks/>
    alphaUcPeriod,
    
    /// <remarks/>
    arabicParenBoth,
    
    /// <remarks/>
    arabicParenR,
    
    /// <remarks/>
    arabicPeriod,
    
    /// <remarks/>
    arabicPlain,
    
    /// <remarks/>
    romanLcParenBoth,
    
    /// <remarks/>
    romanUcParenBoth,
    
    /// <remarks/>
    romanLcParenR,
    
    /// <remarks/>
    romanUcParenR,
    
    /// <remarks/>
    romanLcPeriod,
    
    /// <remarks/>
    romanUcPeriod,
    
    /// <remarks/>
    circleNumDbPlain,
    
    /// <remarks/>
    circleNumWdBlackPlain,
    
    /// <remarks/>
    circleNumWdWhitePlain,
    
    /// <remarks/>
    arabicDbPeriod,
    
    /// <remarks/>
    arabicDbPlain,
    
    /// <remarks/>
    ea1ChsPeriod,
    
    /// <remarks/>
    ea1ChsPlain,
    
    /// <remarks/>
    ea1ChtPeriod,
    
    /// <remarks/>
    ea1ChtPlain,
    
    /// <remarks/>
    ea1JpnChsDbPeriod,
    
    /// <remarks/>
    ea1JpnKorPlain,
    
    /// <remarks/>
    ea1JpnKorPeriod,
    
    /// <remarks/>
    arabic1Minus,
    
    /// <remarks/>
    arabic2Minus,
    
    /// <remarks/>
    hebrew2Minus,
    
    /// <remarks/>
    thaiAlphaPeriod,
    
    /// <remarks/>
    thaiAlphaParenR,
    
    /// <remarks/>
    thaiAlphaParenBoth,
    
    /// <remarks/>
    thaiNumPeriod,
    
    /// <remarks/>
    thaiNumParenR,
    
    /// <remarks/>
    thaiNumParenBoth,
    
    /// <remarks/>
    hindiAlphaPeriod,
    
    /// <remarks/>
    hindiNumPeriod,
    
    /// <remarks/>
    hindiNumParenR,
    
    /// <remarks/>
    hindiAlpha1Period,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextCharBullet {
    
    private string charField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string @char {
        get {
            return this.charField;
        }
        set {
            this.charField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextBlipBullet {
    
    private CT_Blip blipField;
    
    /// <remarks/>
    public CT_Blip blip {
        get {
            return this.blipField;
        }
        set {
            this.blipField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Blip {
    
    private object[] itemsField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private string embedField;
    
    private string linkField;
    
    private ST_BlipCompression cstateField;
    
    public CT_Blip() {
        this.embedField = "";
        this.linkField = "";
        this.cstateField = ST_BlipCompression.none;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaBiLevel", typeof(CT_AlphaBiLevelEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaCeiling", typeof(CT_AlphaCeilingEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaFloor", typeof(CT_AlphaFloorEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaInv", typeof(CT_AlphaInverseEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaMod", typeof(CT_AlphaModulateEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaModFix", typeof(CT_AlphaModulateFixedEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaRepl", typeof(CT_AlphaReplaceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("biLevel", typeof(CT_BiLevelEffect))]
    [System.Xml.Serialization.XmlElementAttribute("blur", typeof(CT_BlurEffect))]
    [System.Xml.Serialization.XmlElementAttribute("clrChange", typeof(CT_ColorChangeEffect))]
    [System.Xml.Serialization.XmlElementAttribute("clrRepl", typeof(CT_ColorReplaceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("duotone", typeof(CT_DuotoneEffect))]
    [System.Xml.Serialization.XmlElementAttribute("fillOverlay", typeof(CT_FillOverlayEffect))]
    [System.Xml.Serialization.XmlElementAttribute("grayscl", typeof(CT_GrayscaleEffect))]
    [System.Xml.Serialization.XmlElementAttribute("hsl", typeof(CT_HSLEffect))]
    [System.Xml.Serialization.XmlElementAttribute("lum", typeof(CT_LuminanceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("tint", typeof(CT_TintEffect))]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string embed {
        get {
            return this.embedField;
        }
        set {
            this.embedField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string link {
        get {
            return this.linkField;
        }
        set {
            this.linkField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_BlipCompression.none)]
    public ST_BlipCompression cstate {
        get {
            return this.cstateField;
        }
        set {
            this.cstateField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaBiLevelEffect {
    
    private string threshField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string thresh {
        get {
            return this.threshField;
        }
        set {
            this.threshField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaCeilingEffect {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaFloorEffect {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaInverseEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaModulateEffect {
    
    private CT_EffectContainer contField;
    
    /// <remarks/>
    public CT_EffectContainer cont {
        get {
            return this.contField;
        }
        set {
            this.contField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_EffectContainer {
    
    private object[] itemsField;
    
    private ST_EffectContainerType typeField;
    
    private string nameField;
    
    public CT_EffectContainer() {
        this.typeField = ST_EffectContainerType.sib;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("alphaBiLevel", typeof(CT_AlphaBiLevelEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaCeiling", typeof(CT_AlphaCeilingEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaFloor", typeof(CT_AlphaFloorEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaInv", typeof(CT_AlphaInverseEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaMod", typeof(CT_AlphaModulateEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaModFix", typeof(CT_AlphaModulateFixedEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaOutset", typeof(CT_AlphaOutsetEffect))]
    [System.Xml.Serialization.XmlElementAttribute("alphaRepl", typeof(CT_AlphaReplaceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("biLevel", typeof(CT_BiLevelEffect))]
    [System.Xml.Serialization.XmlElementAttribute("blend", typeof(CT_BlendEffect))]
    [System.Xml.Serialization.XmlElementAttribute("blur", typeof(CT_BlurEffect))]
    [System.Xml.Serialization.XmlElementAttribute("clrChange", typeof(CT_ColorChangeEffect))]
    [System.Xml.Serialization.XmlElementAttribute("clrRepl", typeof(CT_ColorReplaceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("cont", typeof(CT_EffectContainer))]
    [System.Xml.Serialization.XmlElementAttribute("duotone", typeof(CT_DuotoneEffect))]
    [System.Xml.Serialization.XmlElementAttribute("effect", typeof(CT_EffectReference))]
    [System.Xml.Serialization.XmlElementAttribute("fill", typeof(CT_FillEffect))]
    [System.Xml.Serialization.XmlElementAttribute("fillOverlay", typeof(CT_FillOverlayEffect))]
    [System.Xml.Serialization.XmlElementAttribute("glow", typeof(CT_GlowEffect))]
    [System.Xml.Serialization.XmlElementAttribute("grayscl", typeof(CT_GrayscaleEffect))]
    [System.Xml.Serialization.XmlElementAttribute("hsl", typeof(CT_HSLEffect))]
    [System.Xml.Serialization.XmlElementAttribute("innerShdw", typeof(CT_InnerShadowEffect))]
    [System.Xml.Serialization.XmlElementAttribute("lum", typeof(CT_LuminanceEffect))]
    [System.Xml.Serialization.XmlElementAttribute("outerShdw", typeof(CT_OuterShadowEffect))]
    [System.Xml.Serialization.XmlElementAttribute("prstShdw", typeof(CT_PresetShadowEffect))]
    [System.Xml.Serialization.XmlElementAttribute("reflection", typeof(CT_ReflectionEffect))]
    [System.Xml.Serialization.XmlElementAttribute("relOff", typeof(CT_RelativeOffsetEffect))]
    [System.Xml.Serialization.XmlElementAttribute("softEdge", typeof(CT_SoftEdgesEffect))]
    [System.Xml.Serialization.XmlElementAttribute("tint", typeof(CT_TintEffect))]
    [System.Xml.Serialization.XmlElementAttribute("xfrm", typeof(CT_TransformEffect))]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_EffectContainerType.sib)]
    public ST_EffectContainerType type {
        get {
            return this.typeField;
        }
        set {
            this.typeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaModulateFixedEffect {
    
    private string amtField;
    
    public CT_AlphaModulateFixedEffect() {
        this.amtField = "100%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string amt {
        get {
            return this.amtField;
        }
        set {
            this.amtField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaOutsetEffect {
    
    private string radField;
    
    public CT_AlphaOutsetEffect() {
        this.radField = "0";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string rad {
        get {
            return this.radField;
        }
        set {
            this.radField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AlphaReplaceEffect {
    
    private string aField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string a {
        get {
            return this.aField;
        }
        set {
            this.aField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_BiLevelEffect {
    
    private string threshField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string thresh {
        get {
            return this.threshField;
        }
        set {
            this.threshField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_BlendEffect {
    
    private CT_EffectContainer contField;
    
    private ST_BlendMode blendField;
    
    /// <remarks/>
    public CT_EffectContainer cont {
        get {
            return this.contField;
        }
        set {
            this.contField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_BlendMode blend {
        get {
            return this.blendField;
        }
        set {
            this.blendField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_BlendMode {
    
    /// <remarks/>
    over,
    
    /// <remarks/>
    mult,
    
    /// <remarks/>
    screen,
    
    /// <remarks/>
    darken,
    
    /// <remarks/>
    lighten,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_BlurEffect {
    
    private long radField;
    
    private bool growField;
    
    public CT_BlurEffect() {
        this.radField = ((long)(0));
        this.growField = true;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long rad {
        get {
            return this.radField;
        }
        set {
            this.radField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool grow {
        get {
            return this.growField;
        }
        set {
            this.growField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ColorChangeEffect {
    
    private CT_Color clrFromField;
    
    private CT_Color clrToField;
    
    private bool useAField;
    
    public CT_ColorChangeEffect() {
        this.useAField = true;
    }
    
    /// <remarks/>
    public CT_Color clrFrom {
        get {
            return this.clrFromField;
        }
        set {
            this.clrFromField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color clrTo {
        get {
            return this.clrToField;
        }
        set {
            this.clrToField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool useA {
        get {
            return this.useAField;
        }
        set {
            this.useAField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ColorReplaceEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_DuotoneEffect {
    
    private CT_ScRgbColor[] scrgbClrField;
    
    private CT_SRgbColor[] srgbClrField;
    
    private CT_HslColor[] hslClrField;
    
    private CT_SystemColor[] sysClrField;
    
    private CT_SchemeColor[] schemeClrField;
    
    private CT_PresetColor[] prstClrField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("scrgbClr")]
    public CT_ScRgbColor[] scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("srgbClr")]
    public CT_SRgbColor[] srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("hslClr")]
    public CT_HslColor[] hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("sysClr")]
    public CT_SystemColor[] sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("schemeClr")]
    public CT_SchemeColor[] schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("prstClr")]
    public CT_PresetColor[] prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_EffectReference {
    
    private string refField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string @ref {
        get {
            return this.refField;
        }
        set {
            this.refField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_FillEffect {
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_GroupFillProperties grpFillField;
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupFillProperties grpFill {
        get {
            return this.grpFillField;
        }
        set {
            this.grpFillField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NoFillProperties {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SolidColorFillProperties {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GradientFillProperties {
    
    private CT_GradientStop[] gsLstField;
    
    private CT_LinearShadeProperties linField;
    
    private CT_PathShadeProperties pathField;
    
    private CT_RelativeRect tileRectField;
    
    private ST_TileFlipMode flipField;
    
    private bool flipFieldSpecified;
    
    private bool rotWithShapeField;
    
    private bool rotWithShapeFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("gs", IsNullable=false)]
    public CT_GradientStop[] gsLst {
        get {
            return this.gsLstField;
        }
        set {
            this.gsLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_LinearShadeProperties lin {
        get {
            return this.linField;
        }
        set {
            this.linField = value;
        }
    }
    
    /// <remarks/>
    public CT_PathShadeProperties path {
        get {
            return this.pathField;
        }
        set {
            this.pathField = value;
        }
    }
    
    /// <remarks/>
    public CT_RelativeRect tileRect {
        get {
            return this.tileRectField;
        }
        set {
            this.tileRectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TileFlipMode flip {
        get {
            return this.flipField;
        }
        set {
            this.flipField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool flipSpecified {
        get {
            return this.flipFieldSpecified;
        }
        set {
            this.flipFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool rotWithShape {
        get {
            return this.rotWithShapeField;
        }
        set {
            this.rotWithShapeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool rotWithShapeSpecified {
        get {
            return this.rotWithShapeFieldSpecified;
        }
        set {
            this.rotWithShapeFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GradientStop {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private string posField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string pos {
        get {
            return this.posField;
        }
        set {
            this.posField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LinearShadeProperties {
    
    private int angField;
    
    private bool angFieldSpecified;
    
    private bool scaledField;
    
    private bool scaledFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int ang {
        get {
            return this.angField;
        }
        set {
            this.angField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool angSpecified {
        get {
            return this.angFieldSpecified;
        }
        set {
            this.angFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool scaled {
        get {
            return this.scaledField;
        }
        set {
            this.scaledField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool scaledSpecified {
        get {
            return this.scaledFieldSpecified;
        }
        set {
            this.scaledFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PathShadeProperties {
    
    private CT_RelativeRect fillToRectField;
    
    private ST_PathShadeType pathField;
    
    private bool pathFieldSpecified;
    
    /// <remarks/>
    public CT_RelativeRect fillToRect {
        get {
            return this.fillToRectField;
        }
        set {
            this.fillToRectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PathShadeType path {
        get {
            return this.pathField;
        }
        set {
            this.pathField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool pathSpecified {
        get {
            return this.pathFieldSpecified;
        }
        set {
            this.pathFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_RelativeRect {
    
    private string lField;
    
    private string tField;
    
    private string rField;
    
    private string bField;
    
    public CT_RelativeRect() {
        this.lField = "0%";
        this.tField = "0%";
        this.rField = "0%";
        this.bField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string l {
        get {
            return this.lField;
        }
        set {
            this.lField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string t {
        get {
            return this.tField;
        }
        set {
            this.tField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string r {
        get {
            return this.rField;
        }
        set {
            this.rField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string b {
        get {
            return this.bField;
        }
        set {
            this.bField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PathShadeType {
    
    /// <remarks/>
    shape,
    
    /// <remarks/>
    circle,
    
    /// <remarks/>
    rect,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TileFlipMode {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    x,
    
    /// <remarks/>
    y,
    
    /// <remarks/>
    xy,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_BlipFillProperties {
    
    private CT_Blip blipField;
    
    private CT_RelativeRect srcRectField;
    
    private CT_TileInfoProperties tileField;
    
    private CT_StretchInfoProperties stretchField;
    
    private uint dpiField;
    
    private bool dpiFieldSpecified;
    
    private bool rotWithShapeField;
    
    private bool rotWithShapeFieldSpecified;
    
    /// <remarks/>
    public CT_Blip blip {
        get {
            return this.blipField;
        }
        set {
            this.blipField = value;
        }
    }
    
    /// <remarks/>
    public CT_RelativeRect srcRect {
        get {
            return this.srcRectField;
        }
        set {
            this.srcRectField = value;
        }
    }
    
    /// <remarks/>
    public CT_TileInfoProperties tile {
        get {
            return this.tileField;
        }
        set {
            this.tileField = value;
        }
    }
    
    /// <remarks/>
    public CT_StretchInfoProperties stretch {
        get {
            return this.stretchField;
        }
        set {
            this.stretchField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint dpi {
        get {
            return this.dpiField;
        }
        set {
            this.dpiField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool dpiSpecified {
        get {
            return this.dpiFieldSpecified;
        }
        set {
            this.dpiFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool rotWithShape {
        get {
            return this.rotWithShapeField;
        }
        set {
            this.rotWithShapeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool rotWithShapeSpecified {
        get {
            return this.rotWithShapeFieldSpecified;
        }
        set {
            this.rotWithShapeFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TileInfoProperties {
    
    private string txField;
    
    private string tyField;
    
    private string sxField;
    
    private string syField;
    
    private ST_TileFlipMode flipField;
    
    private bool flipFieldSpecified;
    
    private ST_RectAlignment algnField;
    
    private bool algnFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string ty {
        get {
            return this.tyField;
        }
        set {
            this.tyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string sx {
        get {
            return this.sxField;
        }
        set {
            this.sxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string sy {
        get {
            return this.syField;
        }
        set {
            this.syField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TileFlipMode flip {
        get {
            return this.flipField;
        }
        set {
            this.flipField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool flipSpecified {
        get {
            return this.flipFieldSpecified;
        }
        set {
            this.flipFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_RectAlignment algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool algnSpecified {
        get {
            return this.algnFieldSpecified;
        }
        set {
            this.algnFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_RectAlignment {
    
    /// <remarks/>
    tl,
    
    /// <remarks/>
    t,
    
    /// <remarks/>
    tr,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    bl,
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    br,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_StretchInfoProperties {
    
    private CT_RelativeRect fillRectField;
    
    /// <remarks/>
    public CT_RelativeRect fillRect {
        get {
            return this.fillRectField;
        }
        set {
            this.fillRectField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PatternFillProperties {
    
    private CT_Color fgClrField;
    
    private CT_Color bgClrField;
    
    private ST_PresetPatternVal prstField;
    
    private bool prstFieldSpecified;
    
    /// <remarks/>
    public CT_Color fgClr {
        get {
            return this.fgClrField;
        }
        set {
            this.fgClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color bgClr {
        get {
            return this.bgClrField;
        }
        set {
            this.bgClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PresetPatternVal prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool prstSpecified {
        get {
            return this.prstFieldSpecified;
        }
        set {
            this.prstFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetPatternVal {
    
    /// <remarks/>
    pct5,
    
    /// <remarks/>
    pct10,
    
    /// <remarks/>
    pct20,
    
    /// <remarks/>
    pct25,
    
    /// <remarks/>
    pct30,
    
    /// <remarks/>
    pct40,
    
    /// <remarks/>
    pct50,
    
    /// <remarks/>
    pct60,
    
    /// <remarks/>
    pct70,
    
    /// <remarks/>
    pct75,
    
    /// <remarks/>
    pct80,
    
    /// <remarks/>
    pct90,
    
    /// <remarks/>
    horz,
    
    /// <remarks/>
    vert,
    
    /// <remarks/>
    ltHorz,
    
    /// <remarks/>
    ltVert,
    
    /// <remarks/>
    dkHorz,
    
    /// <remarks/>
    dkVert,
    
    /// <remarks/>
    narHorz,
    
    /// <remarks/>
    narVert,
    
    /// <remarks/>
    dashHorz,
    
    /// <remarks/>
    dashVert,
    
    /// <remarks/>
    cross,
    
    /// <remarks/>
    dnDiag,
    
    /// <remarks/>
    upDiag,
    
    /// <remarks/>
    ltDnDiag,
    
    /// <remarks/>
    ltUpDiag,
    
    /// <remarks/>
    dkDnDiag,
    
    /// <remarks/>
    dkUpDiag,
    
    /// <remarks/>
    wdDnDiag,
    
    /// <remarks/>
    wdUpDiag,
    
    /// <remarks/>
    dashDnDiag,
    
    /// <remarks/>
    dashUpDiag,
    
    /// <remarks/>
    diagCross,
    
    /// <remarks/>
    smCheck,
    
    /// <remarks/>
    lgCheck,
    
    /// <remarks/>
    smGrid,
    
    /// <remarks/>
    lgGrid,
    
    /// <remarks/>
    dotGrid,
    
    /// <remarks/>
    smConfetti,
    
    /// <remarks/>
    lgConfetti,
    
    /// <remarks/>
    horzBrick,
    
    /// <remarks/>
    diagBrick,
    
    /// <remarks/>
    solidDmnd,
    
    /// <remarks/>
    openDmnd,
    
    /// <remarks/>
    dotDmnd,
    
    /// <remarks/>
    plaid,
    
    /// <remarks/>
    sphere,
    
    /// <remarks/>
    weave,
    
    /// <remarks/>
    divot,
    
    /// <remarks/>
    shingle,
    
    /// <remarks/>
    wave,
    
    /// <remarks/>
    trellis,
    
    /// <remarks/>
    zigZag,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GroupFillProperties {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_FillOverlayEffect {
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_GroupFillProperties grpFillField;
    
    private ST_BlendMode blendField;
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupFillProperties grpFill {
        get {
            return this.grpFillField;
        }
        set {
            this.grpFillField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_BlendMode blend {
        get {
            return this.blendField;
        }
        set {
            this.blendField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GlowEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private long radField;
    
    public CT_GlowEffect() {
        this.radField = ((long)(0));
    }
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long rad {
        get {
            return this.radField;
        }
        set {
            this.radField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GrayscaleEffect {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_HSLEffect {
    
    private int hueField;
    
    private string satField;
    
    private string lumField;
    
    public CT_HSLEffect() {
        this.hueField = 0;
        this.satField = "0%";
        this.lumField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string sat {
        get {
            return this.satField;
        }
        set {
            this.satField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string lum {
        get {
            return this.lumField;
        }
        set {
            this.lumField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_InnerShadowEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private long blurRadField;
    
    private long distField;
    
    private int dirField;
    
    public CT_InnerShadowEffect() {
        this.blurRadField = ((long)(0));
        this.distField = ((long)(0));
        this.dirField = 0;
    }
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long blurRad {
        get {
            return this.blurRadField;
        }
        set {
            this.blurRadField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long dist {
        get {
            return this.distField;
        }
        set {
            this.distField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int dir {
        get {
            return this.dirField;
        }
        set {
            this.dirField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LuminanceEffect {
    
    private string brightField;
    
    private string contrastField;
    
    public CT_LuminanceEffect() {
        this.brightField = "0%";
        this.contrastField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string bright {
        get {
            return this.brightField;
        }
        set {
            this.brightField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string contrast {
        get {
            return this.contrastField;
        }
        set {
            this.contrastField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_OuterShadowEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private long blurRadField;
    
    private long distField;
    
    private int dirField;
    
    private string sxField;
    
    private string syField;
    
    private int kxField;
    
    private int kyField;
    
    private ST_RectAlignment algnField;
    
    private bool rotWithShapeField;
    
    public CT_OuterShadowEffect() {
        this.blurRadField = ((long)(0));
        this.distField = ((long)(0));
        this.dirField = 0;
        this.sxField = "100%";
        this.syField = "100%";
        this.kxField = 0;
        this.kyField = 0;
        this.algnField = ST_RectAlignment.b;
        this.rotWithShapeField = true;
    }
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long blurRad {
        get {
            return this.blurRadField;
        }
        set {
            this.blurRadField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long dist {
        get {
            return this.distField;
        }
        set {
            this.distField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int dir {
        get {
            return this.dirField;
        }
        set {
            this.dirField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sx {
        get {
            return this.sxField;
        }
        set {
            this.sxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sy {
        get {
            return this.syField;
        }
        set {
            this.syField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int kx {
        get {
            return this.kxField;
        }
        set {
            this.kxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int ky {
        get {
            return this.kyField;
        }
        set {
            this.kyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_RectAlignment.b)]
    public ST_RectAlignment algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool rotWithShape {
        get {
            return this.rotWithShapeField;
        }
        set {
            this.rotWithShapeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PresetShadowEffect {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private ST_PresetShadowVal prstField;
    
    private long distField;
    
    private int dirField;
    
    public CT_PresetShadowEffect() {
        this.distField = ((long)(0));
        this.dirField = 0;
    }
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PresetShadowVal prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long dist {
        get {
            return this.distField;
        }
        set {
            this.distField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int dir {
        get {
            return this.dirField;
        }
        set {
            this.dirField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetShadowVal {
    
    /// <remarks/>
    shdw1,
    
    /// <remarks/>
    shdw2,
    
    /// <remarks/>
    shdw3,
    
    /// <remarks/>
    shdw4,
    
    /// <remarks/>
    shdw5,
    
    /// <remarks/>
    shdw6,
    
    /// <remarks/>
    shdw7,
    
    /// <remarks/>
    shdw8,
    
    /// <remarks/>
    shdw9,
    
    /// <remarks/>
    shdw10,
    
    /// <remarks/>
    shdw11,
    
    /// <remarks/>
    shdw12,
    
    /// <remarks/>
    shdw13,
    
    /// <remarks/>
    shdw14,
    
    /// <remarks/>
    shdw15,
    
    /// <remarks/>
    shdw16,
    
    /// <remarks/>
    shdw17,
    
    /// <remarks/>
    shdw18,
    
    /// <remarks/>
    shdw19,
    
    /// <remarks/>
    shdw20,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ReflectionEffect {
    
    private long blurRadField;
    
    private string stAField;
    
    private string stPosField;
    
    private string endAField;
    
    private string endPosField;
    
    private long distField;
    
    private int dirField;
    
    private int fadeDirField;
    
    private string sxField;
    
    private string syField;
    
    private int kxField;
    
    private int kyField;
    
    private ST_RectAlignment algnField;
    
    private bool rotWithShapeField;
    
    public CT_ReflectionEffect() {
        this.blurRadField = ((long)(0));
        this.stAField = "100%";
        this.stPosField = "0%";
        this.endAField = "0%";
        this.endPosField = "100%";
        this.distField = ((long)(0));
        this.dirField = 0;
        this.fadeDirField = 5400000;
        this.sxField = "100%";
        this.syField = "100%";
        this.kxField = 0;
        this.kyField = 0;
        this.algnField = ST_RectAlignment.b;
        this.rotWithShapeField = true;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long blurRad {
        get {
            return this.blurRadField;
        }
        set {
            this.blurRadField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string stA {
        get {
            return this.stAField;
        }
        set {
            this.stAField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string stPos {
        get {
            return this.stPosField;
        }
        set {
            this.stPosField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string endA {
        get {
            return this.endAField;
        }
        set {
            this.endAField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string endPos {
        get {
            return this.endPosField;
        }
        set {
            this.endPosField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long dist {
        get {
            return this.distField;
        }
        set {
            this.distField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int dir {
        get {
            return this.dirField;
        }
        set {
            this.dirField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(5400000)]
    public int fadeDir {
        get {
            return this.fadeDirField;
        }
        set {
            this.fadeDirField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sx {
        get {
            return this.sxField;
        }
        set {
            this.sxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sy {
        get {
            return this.syField;
        }
        set {
            this.syField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int kx {
        get {
            return this.kxField;
        }
        set {
            this.kxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int ky {
        get {
            return this.kyField;
        }
        set {
            this.kyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_RectAlignment.b)]
    public ST_RectAlignment algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool rotWithShape {
        get {
            return this.rotWithShapeField;
        }
        set {
            this.rotWithShapeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_RelativeOffsetEffect {
    
    private string txField;
    
    private string tyField;
    
    public CT_RelativeOffsetEffect() {
        this.txField = "0%";
        this.tyField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string ty {
        get {
            return this.tyField;
        }
        set {
            this.tyField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_SoftEdgesEffect {
    
    private long radField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public long rad {
        get {
            return this.radField;
        }
        set {
            this.radField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TintEffect {
    
    private int hueField;
    
    private string amtField;
    
    public CT_TintEffect() {
        this.hueField = 0;
        this.amtField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int hue {
        get {
            return this.hueField;
        }
        set {
            this.hueField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string amt {
        get {
            return this.amtField;
        }
        set {
            this.amtField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TransformEffect {
    
    private string sxField;
    
    private string syField;
    
    private int kxField;
    
    private int kyField;
    
    private string txField;
    
    private string tyField;
    
    public CT_TransformEffect() {
        this.sxField = "100%";
        this.syField = "100%";
        this.kxField = 0;
        this.kyField = 0;
        this.txField = "0";
        this.tyField = "0";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sx {
        get {
            return this.sxField;
        }
        set {
            this.sxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string sy {
        get {
            return this.syField;
        }
        set {
            this.syField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int kx {
        get {
            return this.kxField;
        }
        set {
            this.kxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int ky {
        get {
            return this.kyField;
        }
        set {
            this.kyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string ty {
        get {
            return this.tyField;
        }
        set {
            this.tyField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_EffectContainerType {
    
    /// <remarks/>
    sib,
    
    /// <remarks/>
    tree,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_BlipCompression {
    
    /// <remarks/>
    email,
    
    /// <remarks/>
    screen,
    
    /// <remarks/>
    print,
    
    /// <remarks/>
    hqprint,
    
    /// <remarks/>
    none,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextTabStop {
    
    private string posField;
    
    private ST_TextTabAlignType algnField;
    
    private bool algnFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string pos {
        get {
            return this.posField;
        }
        set {
            this.posField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextTabAlignType algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool algnSpecified {
        get {
            return this.algnFieldSpecified;
        }
        set {
            this.algnFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextTabAlignType {
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    dec,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextCharacterProperties {
    
    private CT_LineProperties lnField;
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_GroupFillProperties grpFillField;
    
    private CT_EffectList effectLstField;
    
    private CT_EffectContainer effectDagField;
    
    private CT_Color highlightField;
    
    private CT_TextUnderlineLineFollowText uLnTxField;
    
    private CT_LineProperties uLnField;
    
    private CT_TextUnderlineFillFollowText uFillTxField;
    
    private CT_TextUnderlineFillGroupWrapper uFillField;
    
    private CT_TextFont latinField;
    
    private CT_TextFont eaField;
    
    private CT_TextFont csField;
    
    private CT_TextFont symField;
    
    private CT_Hyperlink hlinkClickField;
    
    private CT_Hyperlink hlinkMouseOverField;
    
    private CT_Boolean1 rtlField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool kumimojiField;
    
    private bool kumimojiFieldSpecified;
    
    private string langField;
    
    private string altLangField;
    
    private int szField;
    
    private bool szFieldSpecified;
    
    private bool bField;
    
    private bool bFieldSpecified;
    
    private bool iField;
    
    private bool iFieldSpecified;
    
    private ST_TextUnderlineType uField;
    
    private bool uFieldSpecified;
    
    private ST_TextStrikeType strikeField;
    
    private bool strikeFieldSpecified;
    
    private int kernField;
    
    private bool kernFieldSpecified;
    
    private ST_TextCapsType capField;
    
    private bool capFieldSpecified;
    
    private string spcField;
    
    private bool normalizeHField;
    
    private bool normalizeHFieldSpecified;
    
    private string baselineField;
    
    private bool noProofField;
    
    private bool noProofFieldSpecified;
    
    private bool dirtyField;
    
    private bool errField;
    
    private bool smtCleanField;
    
    private uint smtIdField;
    
    private string bmkField;
    
    public CT_TextCharacterProperties() {
        this.dirtyField = true;
        this.errField = false;
        this.smtCleanField = true;
        this.smtIdField = ((uint)(0));
    }
    
    /// <remarks/>
    public CT_LineProperties ln {
        get {
            return this.lnField;
        }
        set {
            this.lnField = value;
        }
    }
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupFillProperties grpFill {
        get {
            return this.grpFillField;
        }
        set {
            this.grpFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectList effectLst {
        get {
            return this.effectLstField;
        }
        set {
            this.effectLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectContainer effectDag {
        get {
            return this.effectDagField;
        }
        set {
            this.effectDagField = value;
        }
    }
    
    /// <remarks/>
    public CT_Color highlight {
        get {
            return this.highlightField;
        }
        set {
            this.highlightField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextUnderlineLineFollowText uLnTx {
        get {
            return this.uLnTxField;
        }
        set {
            this.uLnTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineProperties uLn {
        get {
            return this.uLnField;
        }
        set {
            this.uLnField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextUnderlineFillFollowText uFillTx {
        get {
            return this.uFillTxField;
        }
        set {
            this.uFillTxField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextUnderlineFillGroupWrapper uFill {
        get {
            return this.uFillField;
        }
        set {
            this.uFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextFont latin {
        get {
            return this.latinField;
        }
        set {
            this.latinField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextFont ea {
        get {
            return this.eaField;
        }
        set {
            this.eaField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextFont cs {
        get {
            return this.csField;
        }
        set {
            this.csField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextFont sym {
        get {
            return this.symField;
        }
        set {
            this.symField = value;
        }
    }
    
    /// <remarks/>
    public CT_Hyperlink hlinkClick {
        get {
            return this.hlinkClickField;
        }
        set {
            this.hlinkClickField = value;
        }
    }
    
    /// <remarks/>
    public CT_Hyperlink hlinkMouseOver {
        get {
            return this.hlinkMouseOverField;
        }
        set {
            this.hlinkMouseOverField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean1 rtl {
        get {
            return this.rtlField;
        }
        set {
            this.rtlField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool kumimoji {
        get {
            return this.kumimojiField;
        }
        set {
            this.kumimojiField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool kumimojiSpecified {
        get {
            return this.kumimojiFieldSpecified;
        }
        set {
            this.kumimojiFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string lang {
        get {
            return this.langField;
        }
        set {
            this.langField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string altLang {
        get {
            return this.altLangField;
        }
        set {
            this.altLangField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int sz {
        get {
            return this.szField;
        }
        set {
            this.szField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool szSpecified {
        get {
            return this.szFieldSpecified;
        }
        set {
            this.szFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool b {
        get {
            return this.bField;
        }
        set {
            this.bField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool bSpecified {
        get {
            return this.bFieldSpecified;
        }
        set {
            this.bFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool i {
        get {
            return this.iField;
        }
        set {
            this.iField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool iSpecified {
        get {
            return this.iFieldSpecified;
        }
        set {
            this.iFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextUnderlineType u {
        get {
            return this.uField;
        }
        set {
            this.uField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool uSpecified {
        get {
            return this.uFieldSpecified;
        }
        set {
            this.uFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextStrikeType strike {
        get {
            return this.strikeField;
        }
        set {
            this.strikeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool strikeSpecified {
        get {
            return this.strikeFieldSpecified;
        }
        set {
            this.strikeFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int kern {
        get {
            return this.kernField;
        }
        set {
            this.kernField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool kernSpecified {
        get {
            return this.kernFieldSpecified;
        }
        set {
            this.kernFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_TextCapsType cap {
        get {
            return this.capField;
        }
        set {
            this.capField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool capSpecified {
        get {
            return this.capFieldSpecified;
        }
        set {
            this.capFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string spc {
        get {
            return this.spcField;
        }
        set {
            this.spcField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool normalizeH {
        get {
            return this.normalizeHField;
        }
        set {
            this.normalizeHField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool normalizeHSpecified {
        get {
            return this.normalizeHFieldSpecified;
        }
        set {
            this.normalizeHFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string baseline {
        get {
            return this.baselineField;
        }
        set {
            this.baselineField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool noProof {
        get {
            return this.noProofField;
        }
        set {
            this.noProofField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool noProofSpecified {
        get {
            return this.noProofFieldSpecified;
        }
        set {
            this.noProofFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool dirty {
        get {
            return this.dirtyField;
        }
        set {
            this.dirtyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool err {
        get {
            return this.errField;
        }
        set {
            this.errField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool smtClean {
        get {
            return this.smtCleanField;
        }
        set {
            this.smtCleanField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(uint), "0")]
    public uint smtId {
        get {
            return this.smtIdField;
        }
        set {
            this.smtIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string bmk {
        get {
            return this.bmkField;
        }
        set {
            this.bmkField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LineProperties {
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_PresetLineDashProperties prstDashField;
    
    private CT_DashStop[] custDashField;
    
    private CT_LineJoinRound roundField;
    
    private CT_LineJoinBevel bevelField;
    
    private CT_LineJoinMiterProperties miterField;
    
    private CT_LineEndProperties headEndField;
    
    private CT_LineEndProperties tailEndField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private int wField;
    
    private bool wFieldSpecified;
    
    private ST_LineCap capField;
    
    private bool capFieldSpecified;
    
    private ST_CompoundLine cmpdField;
    
    private bool cmpdFieldSpecified;
    
    private ST_PenAlignment algnField;
    
    private bool algnFieldSpecified;
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetLineDashProperties prstDash {
        get {
            return this.prstDashField;
        }
        set {
            this.prstDashField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ds", IsNullable=false)]
    public CT_DashStop[] custDash {
        get {
            return this.custDashField;
        }
        set {
            this.custDashField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineJoinRound round {
        get {
            return this.roundField;
        }
        set {
            this.roundField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineJoinBevel bevel {
        get {
            return this.bevelField;
        }
        set {
            this.bevelField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineJoinMiterProperties miter {
        get {
            return this.miterField;
        }
        set {
            this.miterField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineEndProperties headEnd {
        get {
            return this.headEndField;
        }
        set {
            this.headEndField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineEndProperties tailEnd {
        get {
            return this.tailEndField;
        }
        set {
            this.tailEndField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public int w {
        get {
            return this.wField;
        }
        set {
            this.wField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool wSpecified {
        get {
            return this.wFieldSpecified;
        }
        set {
            this.wFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LineCap cap {
        get {
            return this.capField;
        }
        set {
            this.capField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool capSpecified {
        get {
            return this.capFieldSpecified;
        }
        set {
            this.capFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_CompoundLine cmpd {
        get {
            return this.cmpdField;
        }
        set {
            this.cmpdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool cmpdSpecified {
        get {
            return this.cmpdFieldSpecified;
        }
        set {
            this.cmpdFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PenAlignment algn {
        get {
            return this.algnField;
        }
        set {
            this.algnField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool algnSpecified {
        get {
            return this.algnFieldSpecified;
        }
        set {
            this.algnFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PresetLineDashProperties {
    
    private ST_PresetLineDashVal valField;
    
    private bool valFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PresetLineDashVal val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool valSpecified {
        get {
            return this.valFieldSpecified;
        }
        set {
            this.valFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PresetLineDashVal {
    
    /// <remarks/>
    solid,
    
    /// <remarks/>
    dot,
    
    /// <remarks/>
    dash,
    
    /// <remarks/>
    lgDash,
    
    /// <remarks/>
    dashDot,
    
    /// <remarks/>
    lgDashDot,
    
    /// <remarks/>
    lgDashDotDot,
    
    /// <remarks/>
    sysDash,
    
    /// <remarks/>
    sysDot,
    
    /// <remarks/>
    sysDashDot,
    
    /// <remarks/>
    sysDashDotDot,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_DashStop {
    
    private string dField;
    
    private string spField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string d {
        get {
            return this.dField;
        }
        set {
            this.dField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string sp {
        get {
            return this.spField;
        }
        set {
            this.spField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LineJoinRound {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LineJoinBevel {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LineJoinMiterProperties {
    
    private string limField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string lim {
        get {
            return this.limField;
        }
        set {
            this.limField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_LineEndProperties {
    
    private ST_LineEndType typeField;
    
    private bool typeFieldSpecified;
    
    private ST_LineEndWidth wField;
    
    private bool wFieldSpecified;
    
    private ST_LineEndLength lenField;
    
    private bool lenFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LineEndType type {
        get {
            return this.typeField;
        }
        set {
            this.typeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool typeSpecified {
        get {
            return this.typeFieldSpecified;
        }
        set {
            this.typeFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LineEndWidth w {
        get {
            return this.wField;
        }
        set {
            this.wField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool wSpecified {
        get {
            return this.wFieldSpecified;
        }
        set {
            this.wFieldSpecified = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LineEndLength len {
        get {
            return this.lenField;
        }
        set {
            this.lenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool lenSpecified {
        get {
            return this.lenFieldSpecified;
        }
        set {
            this.lenFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LineEndType {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    triangle,
    
    /// <remarks/>
    stealth,
    
    /// <remarks/>
    diamond,
    
    /// <remarks/>
    oval,
    
    /// <remarks/>
    arrow,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LineEndWidth {
    
    /// <remarks/>
    sm,
    
    /// <remarks/>
    med,
    
    /// <remarks/>
    lg,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LineEndLength {
    
    /// <remarks/>
    sm,
    
    /// <remarks/>
    med,
    
    /// <remarks/>
    lg,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_LineCap {
    
    /// <remarks/>
    rnd,
    
    /// <remarks/>
    sq,
    
    /// <remarks/>
    flat,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_CompoundLine {
    
    /// <remarks/>
    sng,
    
    /// <remarks/>
    dbl,
    
    /// <remarks/>
    thickThin,
    
    /// <remarks/>
    thinThick,
    
    /// <remarks/>
    tri,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PenAlignment {
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    @in,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_EffectList {
    
    private CT_BlurEffect blurField;
    
    private CT_FillOverlayEffect fillOverlayField;
    
    private CT_GlowEffect glowField;
    
    private CT_InnerShadowEffect innerShdwField;
    
    private CT_OuterShadowEffect outerShdwField;
    
    private CT_PresetShadowEffect prstShdwField;
    
    private CT_ReflectionEffect reflectionField;
    
    private CT_SoftEdgesEffect softEdgeField;
    
    /// <remarks/>
    public CT_BlurEffect blur {
        get {
            return this.blurField;
        }
        set {
            this.blurField = value;
        }
    }
    
    /// <remarks/>
    public CT_FillOverlayEffect fillOverlay {
        get {
            return this.fillOverlayField;
        }
        set {
            this.fillOverlayField = value;
        }
    }
    
    /// <remarks/>
    public CT_GlowEffect glow {
        get {
            return this.glowField;
        }
        set {
            this.glowField = value;
        }
    }
    
    /// <remarks/>
    public CT_InnerShadowEffect innerShdw {
        get {
            return this.innerShdwField;
        }
        set {
            this.innerShdwField = value;
        }
    }
    
    /// <remarks/>
    public CT_OuterShadowEffect outerShdw {
        get {
            return this.outerShdwField;
        }
        set {
            this.outerShdwField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetShadowEffect prstShdw {
        get {
            return this.prstShdwField;
        }
        set {
            this.prstShdwField = value;
        }
    }
    
    /// <remarks/>
    public CT_ReflectionEffect reflection {
        get {
            return this.reflectionField;
        }
        set {
            this.reflectionField = value;
        }
    }
    
    /// <remarks/>
    public CT_SoftEdgesEffect softEdge {
        get {
            return this.softEdgeField;
        }
        set {
            this.softEdgeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextUnderlineLineFollowText {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextUnderlineFillFollowText {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextUnderlineFillGroupWrapper {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("blipFill", typeof(CT_BlipFillProperties))]
    [System.Xml.Serialization.XmlElementAttribute("gradFill", typeof(CT_GradientFillProperties))]
    [System.Xml.Serialization.XmlElementAttribute("grpFill", typeof(CT_GroupFillProperties))]
    [System.Xml.Serialization.XmlElementAttribute("noFill", typeof(CT_NoFillProperties))]
    [System.Xml.Serialization.XmlElementAttribute("pattFill", typeof(CT_PatternFillProperties))]
    [System.Xml.Serialization.XmlElementAttribute("solidFill", typeof(CT_SolidColorFillProperties))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Hyperlink {
    
    private CT_EmbeddedWAVAudioFile sndField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private string idField;
    
    private string invalidUrlField;
    
    private string actionField;
    
    private string tgtFrameField;
    
    private string tooltipField;
    
    private bool historyField;
    
    private bool highlightClickField;
    
    private bool endSndField;
    
    public CT_Hyperlink() {
        this.invalidUrlField = "";
        this.actionField = "";
        this.tgtFrameField = "";
        this.tooltipField = "";
        this.historyField = true;
        this.highlightClickField = false;
        this.endSndField = false;
    }
    
    /// <remarks/>
    public CT_EmbeddedWAVAudioFile snd {
        get {
            return this.sndField;
        }
        set {
            this.sndField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    public string id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string invalidUrl {
        get {
            return this.invalidUrlField;
        }
        set {
            this.invalidUrlField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string action {
        get {
            return this.actionField;
        }
        set {
            this.actionField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string tgtFrame {
        get {
            return this.tgtFrameField;
        }
        set {
            this.tgtFrameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string tooltip {
        get {
            return this.tooltipField;
        }
        set {
            this.tooltipField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool history {
        get {
            return this.historyField;
        }
        set {
            this.historyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool highlightClick {
        get {
            return this.highlightClickField;
        }
        set {
            this.highlightClickField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool endSnd {
        get {
            return this.endSndField;
        }
        set {
            this.endSndField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_EmbeddedWAVAudioFile {
    
    private string embedField;
    
    private string nameField;
    
    public CT_EmbeddedWAVAudioFile() {
        this.nameField = "";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(Form=System.Xml.Schema.XmlSchemaForm.Qualified, Namespace="http://purl.oclc.org/ooxml/officeDocument/relationships")]
    public string embed {
        get {
            return this.embedField;
        }
        set {
            this.embedField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(TypeName="CT_Boolean", Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Boolean1 {
    
    private string valField;
    
    public CT_Boolean1() {
        this.valField = "0";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextUnderlineType {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    words,
    
    /// <remarks/>
    sng,
    
    /// <remarks/>
    dbl,
    
    /// <remarks/>
    heavy,
    
    /// <remarks/>
    dotted,
    
    /// <remarks/>
    dottedHeavy,
    
    /// <remarks/>
    dash,
    
    /// <remarks/>
    dashHeavy,
    
    /// <remarks/>
    dashLong,
    
    /// <remarks/>
    dashLongHeavy,
    
    /// <remarks/>
    dotDash,
    
    /// <remarks/>
    dotDashHeavy,
    
    /// <remarks/>
    dotDotDash,
    
    /// <remarks/>
    dotDotDashHeavy,
    
    /// <remarks/>
    wavy,
    
    /// <remarks/>
    wavyHeavy,
    
    /// <remarks/>
    wavyDbl,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextStrikeType {
    
    /// <remarks/>
    noStrike,
    
    /// <remarks/>
    sngStrike,
    
    /// <remarks/>
    dblStrike,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextCapsType {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    small,
    
    /// <remarks/>
    all,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextAlignType {
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    just,
    
    /// <remarks/>
    justLow,
    
    /// <remarks/>
    dist,
    
    /// <remarks/>
    thaiDist,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_TextFontAlignType {
    
    /// <remarks/>
    auto,
    
    /// <remarks/>
    t,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    @base,
    
    /// <remarks/>
    b,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextParagraph {
    
    private CT_TextParagraphProperties pPrField;
    
    private CT_RegularTextRun[] rField;
    
    private CT_TextLineBreak[] brField;
    
    private CT_TextField[] fldField;
    
    private CT_TextCharacterProperties endParaRPrField;
    
    /// <remarks/>
    public CT_TextParagraphProperties pPr {
        get {
            return this.pPrField;
        }
        set {
            this.pPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("r")]
    public CT_RegularTextRun[] r {
        get {
            return this.rField;
        }
        set {
            this.rField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("br")]
    public CT_TextLineBreak[] br {
        get {
            return this.brField;
        }
        set {
            this.brField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("fld")]
    public CT_TextField[] fld {
        get {
            return this.fldField;
        }
        set {
            this.fldField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextCharacterProperties endParaRPr {
        get {
            return this.endParaRPrField;
        }
        set {
            this.endParaRPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_RegularTextRun {
    
    private CT_TextCharacterProperties rPrField;
    
    private string tField;
    
    /// <remarks/>
    public CT_TextCharacterProperties rPr {
        get {
            return this.rPrField;
        }
        set {
            this.rPrField = value;
        }
    }
    
    /// <remarks/>
    public string t {
        get {
            return this.tField;
        }
        set {
            this.tField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextLineBreak {
    
    private CT_TextCharacterProperties rPrField;
    
    /// <remarks/>
    public CT_TextCharacterProperties rPr {
        get {
            return this.rPrField;
        }
        set {
            this.rPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_TextField {
    
    private CT_TextCharacterProperties rPrField;
    
    private CT_TextParagraphProperties pPrField;
    
    private string tField;
    
    private string idField;
    
    private string typeField;
    
    /// <remarks/>
    public CT_TextCharacterProperties rPr {
        get {
            return this.rPrField;
        }
        set {
            this.rPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextParagraphProperties pPr {
        get {
            return this.pPrField;
        }
        set {
            this.pPrField = value;
        }
    }
    
    /// <remarks/>
    public string t {
        get {
            return this.tField;
        }
        set {
            this.tField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string type {
        get {
            return this.typeField;
        }
        set {
            this.typeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Extension {
    
    private System.Xml.XmlElement anyField;
    
    private string uriField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAnyElementAttribute()]
    public System.Xml.XmlElement Any {
        get {
            return this.anyField;
        }
        set {
            this.anyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string uri {
        get {
            return this.uriField;
        }
        set {
            this.uriField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LegendPos {
    
    private ST_LegendPos valField;
    
    public CT_LegendPos() {
        this.valField = ST_LegendPos.r;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_LegendPos.r)]
    public ST_LegendPos val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_LegendPos {
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    tr,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    t,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Legend {
    
    private CT_LegendPos legendPosField;
    
    private CT_LegendEntry[] legendEntryField;
    
    private CT_Layout layoutField;
    
    private CT_Boolean overlayField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_LegendPos legendPos {
        get {
            return this.legendPosField;
        }
        set {
            this.legendPosField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("legendEntry")]
    public CT_LegendEntry[] legendEntry {
        get {
            return this.legendEntryField;
        }
        set {
            this.legendEntryField = value;
        }
    }
    
    /// <remarks/>
    public CT_Layout layout {
        get {
            return this.layoutField;
        }
        set {
            this.layoutField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean overlay {
        get {
            return this.overlayField;
        }
        set {
            this.overlayField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Layout {
    
    private CT_ManualLayout manualLayoutField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_ManualLayout manualLayout {
        get {
            return this.manualLayoutField;
        }
        set {
            this.manualLayoutField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ManualLayout {
    
    private CT_LayoutTarget layoutTargetField;
    
    private CT_LayoutMode xModeField;
    
    private CT_LayoutMode yModeField;
    
    private CT_LayoutMode wModeField;
    
    private CT_LayoutMode hModeField;
    
    private CT_Double xField;
    
    private CT_Double yField;
    
    private CT_Double wField;
    
    private CT_Double hField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_LayoutTarget layoutTarget {
        get {
            return this.layoutTargetField;
        }
        set {
            this.layoutTargetField = value;
        }
    }
    
    /// <remarks/>
    public CT_LayoutMode xMode {
        get {
            return this.xModeField;
        }
        set {
            this.xModeField = value;
        }
    }
    
    /// <remarks/>
    public CT_LayoutMode yMode {
        get {
            return this.yModeField;
        }
        set {
            this.yModeField = value;
        }
    }
    
    /// <remarks/>
    public CT_LayoutMode wMode {
        get {
            return this.wModeField;
        }
        set {
            this.wModeField = value;
        }
    }
    
    /// <remarks/>
    public CT_LayoutMode hMode {
        get {
            return this.hModeField;
        }
        set {
            this.hModeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double x {
        get {
            return this.xField;
        }
        set {
            this.xField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double y {
        get {
            return this.yField;
        }
        set {
            this.yField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double w {
        get {
            return this.wField;
        }
        set {
            this.wField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double h {
        get {
            return this.hField;
        }
        set {
            this.hField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LayoutTarget {
    
    private ST_LayoutTarget valField;
    
    public CT_LayoutTarget() {
        this.valField = ST_LayoutTarget.outer;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_LayoutTarget.outer)]
    public ST_LayoutTarget val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_LayoutTarget {
    
    /// <remarks/>
    inner,
    
    /// <remarks/>
    outer,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LayoutMode {
    
    private ST_LayoutMode valField;
    
    public CT_LayoutMode() {
        this.valField = ST_LayoutMode.factor;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_LayoutMode.factor)]
    public ST_LayoutMode val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_LayoutMode {
    
    /// <remarks/>
    edge,
    
    /// <remarks/>
    factor,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Double {
    
    private double valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ShapeProperties {
    
    private CT_Transform2D xfrmField;
    
    private CT_CustomGeometry2D custGeomField;
    
    private CT_PresetGeometry2D prstGeomField;
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_GroupFillProperties grpFillField;
    
    private CT_LineProperties lnField;
    
    private CT_EffectList effectLstField;
    
    private CT_EffectContainer effectDagField;
    
    private CT_Scene3D scene3dField;
    
    private CT_Shape3D sp3dField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private ST_BlackWhiteMode bwModeField;
    
    private bool bwModeFieldSpecified;
    
    /// <remarks/>
    public CT_Transform2D xfrm {
        get {
            return this.xfrmField;
        }
        set {
            this.xfrmField = value;
        }
    }
    
    /// <remarks/>
    public CT_CustomGeometry2D custGeom {
        get {
            return this.custGeomField;
        }
        set {
            this.custGeomField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetGeometry2D prstGeom {
        get {
            return this.prstGeomField;
        }
        set {
            this.prstGeomField = value;
        }
    }
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupFillProperties grpFill {
        get {
            return this.grpFillField;
        }
        set {
            this.grpFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_LineProperties ln {
        get {
            return this.lnField;
        }
        set {
            this.lnField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectList effectLst {
        get {
            return this.effectLstField;
        }
        set {
            this.effectLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectContainer effectDag {
        get {
            return this.effectDagField;
        }
        set {
            this.effectDagField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scene3D scene3d {
        get {
            return this.scene3dField;
        }
        set {
            this.scene3dField = value;
        }
    }
    
    /// <remarks/>
    public CT_Shape3D sp3d {
        get {
            return this.sp3dField;
        }
        set {
            this.sp3dField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_BlackWhiteMode bwMode {
        get {
            return this.bwModeField;
        }
        set {
            this.bwModeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool bwModeSpecified {
        get {
            return this.bwModeFieldSpecified;
        }
        set {
            this.bwModeFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Transform2D {
    
    private CT_Point2D offField;
    
    private CT_PositiveSize2D extField;
    
    private int rotField;
    
    private bool flipHField;
    
    private bool flipVField;
    
    public CT_Transform2D() {
        this.rotField = 0;
        this.flipHField = false;
        this.flipVField = false;
    }
    
    /// <remarks/>
    public CT_Point2D off {
        get {
            return this.offField;
        }
        set {
            this.offField = value;
        }
    }
    
    /// <remarks/>
    public CT_PositiveSize2D ext {
        get {
            return this.extField;
        }
        set {
            this.extField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int rot {
        get {
            return this.rotField;
        }
        set {
            this.rotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool flipH {
        get {
            return this.flipHField;
        }
        set {
            this.flipHField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool flipV {
        get {
            return this.flipVField;
        }
        set {
            this.flipVField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Point2D {
    
    private string xField;
    
    private string yField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string x {
        get {
            return this.xField;
        }
        set {
            this.xField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string y {
        get {
            return this.yField;
        }
        set {
            this.yField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PositiveSize2D {
    
    private long cxField;
    
    private long cyField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public long cx {
        get {
            return this.cxField;
        }
        set {
            this.cxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public long cy {
        get {
            return this.cyField;
        }
        set {
            this.cyField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_CustomGeometry2D {
    
    private CT_GeomGuide[] avLstField;
    
    private CT_GeomGuide[] gdLstField;
    
    private object[] ahLstField;
    
    private CT_ConnectionSite[] cxnLstField;
    
    private CT_GeomRect rectField;
    
    private CT_Path2D[] pathLstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("gd", IsNullable=false)]
    public CT_GeomGuide[] avLst {
        get {
            return this.avLstField;
        }
        set {
            this.avLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("gd", IsNullable=false)]
    public CT_GeomGuide[] gdLst {
        get {
            return this.gdLstField;
        }
        set {
            this.gdLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ahPolar", typeof(CT_PolarAdjustHandle), IsNullable=false)]
    [System.Xml.Serialization.XmlArrayItemAttribute("ahXY", typeof(CT_XYAdjustHandle), IsNullable=false)]
    public object[] ahLst {
        get {
            return this.ahLstField;
        }
        set {
            this.ahLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("cxn", IsNullable=false)]
    public CT_ConnectionSite[] cxnLst {
        get {
            return this.cxnLstField;
        }
        set {
            this.cxnLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_GeomRect rect {
        get {
            return this.rectField;
        }
        set {
            this.rectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("path", IsNullable=false)]
    public CT_Path2D[] pathLst {
        get {
            return this.pathLstField;
        }
        set {
            this.pathLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PolarAdjustHandle {
    
    private CT_AdjPoint2D posField;
    
    private string gdRefRField;
    
    private string minRField;
    
    private string maxRField;
    
    private string gdRefAngField;
    
    private string minAngField;
    
    private string maxAngField;
    
    /// <remarks/>
    public CT_AdjPoint2D pos {
        get {
            return this.posField;
        }
        set {
            this.posField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string gdRefR {
        get {
            return this.gdRefRField;
        }
        set {
            this.gdRefRField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string minR {
        get {
            return this.minRField;
        }
        set {
            this.minRField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string maxR {
        get {
            return this.maxRField;
        }
        set {
            this.maxRField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string gdRefAng {
        get {
            return this.gdRefAngField;
        }
        set {
            this.gdRefAngField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string minAng {
        get {
            return this.minAngField;
        }
        set {
            this.minAngField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string maxAng {
        get {
            return this.maxAngField;
        }
        set {
            this.maxAngField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_AdjPoint2D {
    
    private string xField;
    
    private string yField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string x {
        get {
            return this.xField;
        }
        set {
            this.xField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string y {
        get {
            return this.yField;
        }
        set {
            this.yField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_XYAdjustHandle {
    
    private CT_AdjPoint2D posField;
    
    private string gdRefXField;
    
    private string minXField;
    
    private string maxXField;
    
    private string gdRefYField;
    
    private string minYField;
    
    private string maxYField;
    
    /// <remarks/>
    public CT_AdjPoint2D pos {
        get {
            return this.posField;
        }
        set {
            this.posField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string gdRefX {
        get {
            return this.gdRefXField;
        }
        set {
            this.gdRefXField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string minX {
        get {
            return this.minXField;
        }
        set {
            this.minXField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string maxX {
        get {
            return this.maxXField;
        }
        set {
            this.maxXField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string gdRefY {
        get {
            return this.gdRefYField;
        }
        set {
            this.gdRefYField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string minY {
        get {
            return this.minYField;
        }
        set {
            this.minYField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string maxY {
        get {
            return this.maxYField;
        }
        set {
            this.maxYField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ConnectionSite {
    
    private CT_AdjPoint2D posField;
    
    private string angField;
    
    /// <remarks/>
    public CT_AdjPoint2D pos {
        get {
            return this.posField;
        }
        set {
            this.posField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string ang {
        get {
            return this.angField;
        }
        set {
            this.angField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GeomRect {
    
    private string lField;
    
    private string tField;
    
    private string rField;
    
    private string bField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string l {
        get {
            return this.lField;
        }
        set {
            this.lField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string t {
        get {
            return this.tField;
        }
        set {
            this.tField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string r {
        get {
            return this.rField;
        }
        set {
            this.rField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string b {
        get {
            return this.bField;
        }
        set {
            this.bField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2D {
    
    private object[] itemsField;
    
    private ItemsChoiceType[] itemsElementNameField;
    
    private long wField;
    
    private long hField;
    
    private ST_PathFillMode fillField;
    
    private bool strokeField;
    
    private bool extrusionOkField;
    
    public CT_Path2D() {
        this.wField = ((long)(0));
        this.hField = ((long)(0));
        this.fillField = ST_PathFillMode.norm;
        this.strokeField = true;
        this.extrusionOkField = true;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("arcTo", typeof(CT_Path2DArcTo))]
    [System.Xml.Serialization.XmlElementAttribute("close", typeof(CT_Path2DClose))]
    [System.Xml.Serialization.XmlElementAttribute("cubicBezTo", typeof(CT_Path2DCubicBezierTo))]
    [System.Xml.Serialization.XmlElementAttribute("lnTo", typeof(CT_Path2DLineTo))]
    [System.Xml.Serialization.XmlElementAttribute("moveTo", typeof(CT_Path2DMoveTo))]
    [System.Xml.Serialization.XmlElementAttribute("quadBezTo", typeof(CT_Path2DQuadBezierTo))]
    [System.Xml.Serialization.XmlChoiceIdentifierAttribute("ItemsElementName")]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ItemsElementName")]
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public ItemsChoiceType[] ItemsElementName {
        get {
            return this.itemsElementNameField;
        }
        set {
            this.itemsElementNameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long w {
        get {
            return this.wField;
        }
        set {
            this.wField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(long), "0")]
    public long h {
        get {
            return this.hField;
        }
        set {
            this.hField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_PathFillMode.norm)]
    public ST_PathFillMode fill {
        get {
            return this.fillField;
        }
        set {
            this.fillField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool stroke {
        get {
            return this.strokeField;
        }
        set {
            this.strokeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool extrusionOk {
        get {
            return this.extrusionOkField;
        }
        set {
            this.extrusionOkField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DArcTo {
    
    private string wrField;
    
    private string hrField;
    
    private string stAngField;
    
    private string swAngField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string wR {
        get {
            return this.wrField;
        }
        set {
            this.wrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string hR {
        get {
            return this.hrField;
        }
        set {
            this.hrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string stAng {
        get {
            return this.stAngField;
        }
        set {
            this.stAngField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string swAng {
        get {
            return this.swAngField;
        }
        set {
            this.swAngField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DClose {
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DCubicBezierTo {
    
    private CT_AdjPoint2D[] ptField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("pt")]
    public CT_AdjPoint2D[] pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DLineTo {
    
    private CT_AdjPoint2D ptField;
    
    /// <remarks/>
    public CT_AdjPoint2D pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DMoveTo {
    
    private CT_AdjPoint2D ptField;
    
    /// <remarks/>
    public CT_AdjPoint2D pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Path2DQuadBezierTo {
    
    private CT_AdjPoint2D[] ptField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("pt")]
    public CT_AdjPoint2D[] pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main", IncludeInSchema=false)]
public enum ItemsChoiceType {
    
    /// <remarks/>
    arcTo,
    
    /// <remarks/>
    close,
    
    /// <remarks/>
    cubicBezTo,
    
    /// <remarks/>
    lnTo,
    
    /// <remarks/>
    moveTo,
    
    /// <remarks/>
    quadBezTo,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_PathFillMode {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    norm,
    
    /// <remarks/>
    lighten,
    
    /// <remarks/>
    lightenLess,
    
    /// <remarks/>
    darken,
    
    /// <remarks/>
    darkenLess,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PresetGeometry2D {
    
    private CT_GeomGuide[] avLstField;
    
    private ST_ShapeType prstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("gd", IsNullable=false)]
    public CT_GeomGuide[] avLst {
        get {
            return this.avLstField;
        }
        set {
            this.avLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ShapeType prst {
        get {
            return this.prstField;
        }
        set {
            this.prstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_ShapeType {
    
    /// <remarks/>
    line,
    
    /// <remarks/>
    lineInv,
    
    /// <remarks/>
    triangle,
    
    /// <remarks/>
    rtTriangle,
    
    /// <remarks/>
    rect,
    
    /// <remarks/>
    diamond,
    
    /// <remarks/>
    parallelogram,
    
    /// <remarks/>
    trapezoid,
    
    /// <remarks/>
    nonIsoscelesTrapezoid,
    
    /// <remarks/>
    pentagon,
    
    /// <remarks/>
    hexagon,
    
    /// <remarks/>
    heptagon,
    
    /// <remarks/>
    octagon,
    
    /// <remarks/>
    decagon,
    
    /// <remarks/>
    dodecagon,
    
    /// <remarks/>
    star4,
    
    /// <remarks/>
    star5,
    
    /// <remarks/>
    star6,
    
    /// <remarks/>
    star7,
    
    /// <remarks/>
    star8,
    
    /// <remarks/>
    star10,
    
    /// <remarks/>
    star12,
    
    /// <remarks/>
    star16,
    
    /// <remarks/>
    star24,
    
    /// <remarks/>
    star32,
    
    /// <remarks/>
    roundRect,
    
    /// <remarks/>
    round1Rect,
    
    /// <remarks/>
    round2SameRect,
    
    /// <remarks/>
    round2DiagRect,
    
    /// <remarks/>
    snipRoundRect,
    
    /// <remarks/>
    snip1Rect,
    
    /// <remarks/>
    snip2SameRect,
    
    /// <remarks/>
    snip2DiagRect,
    
    /// <remarks/>
    plaque,
    
    /// <remarks/>
    ellipse,
    
    /// <remarks/>
    teardrop,
    
    /// <remarks/>
    homePlate,
    
    /// <remarks/>
    chevron,
    
    /// <remarks/>
    pieWedge,
    
    /// <remarks/>
    pie,
    
    /// <remarks/>
    blockArc,
    
    /// <remarks/>
    donut,
    
    /// <remarks/>
    noSmoking,
    
    /// <remarks/>
    rightArrow,
    
    /// <remarks/>
    leftArrow,
    
    /// <remarks/>
    upArrow,
    
    /// <remarks/>
    downArrow,
    
    /// <remarks/>
    stripedRightArrow,
    
    /// <remarks/>
    notchedRightArrow,
    
    /// <remarks/>
    bentUpArrow,
    
    /// <remarks/>
    leftRightArrow,
    
    /// <remarks/>
    upDownArrow,
    
    /// <remarks/>
    leftUpArrow,
    
    /// <remarks/>
    leftRightUpArrow,
    
    /// <remarks/>
    quadArrow,
    
    /// <remarks/>
    leftArrowCallout,
    
    /// <remarks/>
    rightArrowCallout,
    
    /// <remarks/>
    upArrowCallout,
    
    /// <remarks/>
    downArrowCallout,
    
    /// <remarks/>
    leftRightArrowCallout,
    
    /// <remarks/>
    upDownArrowCallout,
    
    /// <remarks/>
    quadArrowCallout,
    
    /// <remarks/>
    bentArrow,
    
    /// <remarks/>
    uturnArrow,
    
    /// <remarks/>
    circularArrow,
    
    /// <remarks/>
    leftCircularArrow,
    
    /// <remarks/>
    leftRightCircularArrow,
    
    /// <remarks/>
    curvedRightArrow,
    
    /// <remarks/>
    curvedLeftArrow,
    
    /// <remarks/>
    curvedUpArrow,
    
    /// <remarks/>
    curvedDownArrow,
    
    /// <remarks/>
    swooshArrow,
    
    /// <remarks/>
    cube,
    
    /// <remarks/>
    can,
    
    /// <remarks/>
    lightningBolt,
    
    /// <remarks/>
    heart,
    
    /// <remarks/>
    sun,
    
    /// <remarks/>
    moon,
    
    /// <remarks/>
    smileyFace,
    
    /// <remarks/>
    irregularSeal1,
    
    /// <remarks/>
    irregularSeal2,
    
    /// <remarks/>
    foldedCorner,
    
    /// <remarks/>
    bevel,
    
    /// <remarks/>
    frame,
    
    /// <remarks/>
    halfFrame,
    
    /// <remarks/>
    corner,
    
    /// <remarks/>
    diagStripe,
    
    /// <remarks/>
    chord,
    
    /// <remarks/>
    arc,
    
    /// <remarks/>
    leftBracket,
    
    /// <remarks/>
    rightBracket,
    
    /// <remarks/>
    leftBrace,
    
    /// <remarks/>
    rightBrace,
    
    /// <remarks/>
    bracketPair,
    
    /// <remarks/>
    bracePair,
    
    /// <remarks/>
    straightConnector1,
    
    /// <remarks/>
    bentConnector2,
    
    /// <remarks/>
    bentConnector3,
    
    /// <remarks/>
    bentConnector4,
    
    /// <remarks/>
    bentConnector5,
    
    /// <remarks/>
    curvedConnector2,
    
    /// <remarks/>
    curvedConnector3,
    
    /// <remarks/>
    curvedConnector4,
    
    /// <remarks/>
    curvedConnector5,
    
    /// <remarks/>
    callout1,
    
    /// <remarks/>
    callout2,
    
    /// <remarks/>
    callout3,
    
    /// <remarks/>
    accentCallout1,
    
    /// <remarks/>
    accentCallout2,
    
    /// <remarks/>
    accentCallout3,
    
    /// <remarks/>
    borderCallout1,
    
    /// <remarks/>
    borderCallout2,
    
    /// <remarks/>
    borderCallout3,
    
    /// <remarks/>
    accentBorderCallout1,
    
    /// <remarks/>
    accentBorderCallout2,
    
    /// <remarks/>
    accentBorderCallout3,
    
    /// <remarks/>
    wedgeRectCallout,
    
    /// <remarks/>
    wedgeRoundRectCallout,
    
    /// <remarks/>
    wedgeEllipseCallout,
    
    /// <remarks/>
    cloudCallout,
    
    /// <remarks/>
    cloud,
    
    /// <remarks/>
    ribbon,
    
    /// <remarks/>
    ribbon2,
    
    /// <remarks/>
    ellipseRibbon,
    
    /// <remarks/>
    ellipseRibbon2,
    
    /// <remarks/>
    leftRightRibbon,
    
    /// <remarks/>
    verticalScroll,
    
    /// <remarks/>
    horizontalScroll,
    
    /// <remarks/>
    wave,
    
    /// <remarks/>
    doubleWave,
    
    /// <remarks/>
    plus,
    
    /// <remarks/>
    flowChartProcess,
    
    /// <remarks/>
    flowChartDecision,
    
    /// <remarks/>
    flowChartInputOutput,
    
    /// <remarks/>
    flowChartPredefinedProcess,
    
    /// <remarks/>
    flowChartInternalStorage,
    
    /// <remarks/>
    flowChartDocument,
    
    /// <remarks/>
    flowChartMultidocument,
    
    /// <remarks/>
    flowChartTerminator,
    
    /// <remarks/>
    flowChartPreparation,
    
    /// <remarks/>
    flowChartManualInput,
    
    /// <remarks/>
    flowChartManualOperation,
    
    /// <remarks/>
    flowChartConnector,
    
    /// <remarks/>
    flowChartPunchedCard,
    
    /// <remarks/>
    flowChartPunchedTape,
    
    /// <remarks/>
    flowChartSummingJunction,
    
    /// <remarks/>
    flowChartOr,
    
    /// <remarks/>
    flowChartCollate,
    
    /// <remarks/>
    flowChartSort,
    
    /// <remarks/>
    flowChartExtract,
    
    /// <remarks/>
    flowChartMerge,
    
    /// <remarks/>
    flowChartOfflineStorage,
    
    /// <remarks/>
    flowChartOnlineStorage,
    
    /// <remarks/>
    flowChartMagneticTape,
    
    /// <remarks/>
    flowChartMagneticDisk,
    
    /// <remarks/>
    flowChartMagneticDrum,
    
    /// <remarks/>
    flowChartDisplay,
    
    /// <remarks/>
    flowChartDelay,
    
    /// <remarks/>
    flowChartAlternateProcess,
    
    /// <remarks/>
    flowChartOffpageConnector,
    
    /// <remarks/>
    actionButtonBlank,
    
    /// <remarks/>
    actionButtonHome,
    
    /// <remarks/>
    actionButtonHelp,
    
    /// <remarks/>
    actionButtonInformation,
    
    /// <remarks/>
    actionButtonForwardNext,
    
    /// <remarks/>
    actionButtonBackPrevious,
    
    /// <remarks/>
    actionButtonEnd,
    
    /// <remarks/>
    actionButtonBeginning,
    
    /// <remarks/>
    actionButtonReturn,
    
    /// <remarks/>
    actionButtonDocument,
    
    /// <remarks/>
    actionButtonSound,
    
    /// <remarks/>
    actionButtonMovie,
    
    /// <remarks/>
    gear6,
    
    /// <remarks/>
    gear9,
    
    /// <remarks/>
    funnel,
    
    /// <remarks/>
    mathPlus,
    
    /// <remarks/>
    mathMinus,
    
    /// <remarks/>
    mathMultiply,
    
    /// <remarks/>
    mathDivide,
    
    /// <remarks/>
    mathEqual,
    
    /// <remarks/>
    mathNotEqual,
    
    /// <remarks/>
    cornerTabs,
    
    /// <remarks/>
    squareTabs,
    
    /// <remarks/>
    plaqueTabs,
    
    /// <remarks/>
    chartX,
    
    /// <remarks/>
    chartStar,
    
    /// <remarks/>
    chartPlus,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_BlackWhiteMode {
    
    /// <remarks/>
    clr,
    
    /// <remarks/>
    auto,
    
    /// <remarks/>
    gray,
    
    /// <remarks/>
    ltGray,
    
    /// <remarks/>
    invGray,
    
    /// <remarks/>
    grayWhite,
    
    /// <remarks/>
    blackGray,
    
    /// <remarks/>
    blackWhite,
    
    /// <remarks/>
    black,
    
    /// <remarks/>
    white,
    
    /// <remarks/>
    hidden,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DTable {
    
    private CT_Boolean showHorzBorderField;
    
    private CT_Boolean showVertBorderField;
    
    private CT_Boolean showOutlineField;
    
    private CT_Boolean showKeysField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean showHorzBorder {
        get {
            return this.showHorzBorderField;
        }
        set {
            this.showHorzBorderField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean showVertBorder {
        get {
            return this.showVertBorderField;
        }
        set {
            this.showVertBorderField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean showOutline {
        get {
            return this.showOutlineField;
        }
        set {
            this.showOutlineField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean showKeys {
        get {
            return this.showKeysField;
        }
        set {
            this.showKeysField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SerAx {
    
    private CT_UnsignedInt axIdField;
    
    private CT_Scaling scalingField;
    
    private CT_Boolean deleteField;
    
    private CT_AxPos axPosField;
    
    private CT_ChartLines majorGridlinesField;
    
    private CT_ChartLines minorGridlinesField;
    
    private CT_Title titleField;
    
    private CT_NumFmt numFmtField;
    
    private CT_TickMark majorTickMarkField;
    
    private CT_TickMark minorTickMarkField;
    
    private CT_TickLblPos tickLblPosField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_UnsignedInt crossAxField;
    
    private object itemField;
    
    private CT_Skip tickLblSkipField;
    
    private CT_Skip tickMarkSkipField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scaling scaling {
        get {
            return this.scalingField;
        }
        set {
            this.scalingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean delete {
        get {
            return this.deleteField;
        }
        set {
            this.deleteField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxPos axPos {
        get {
            return this.axPosField;
        }
        set {
            this.axPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines majorGridlines {
        get {
            return this.majorGridlinesField;
        }
        set {
            this.majorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines minorGridlines {
        get {
            return this.minorGridlinesField;
        }
        set {
            this.minorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_Title title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumFmt numFmt {
        get {
            return this.numFmtField;
        }
        set {
            this.numFmtField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark majorTickMark {
        get {
            return this.majorTickMarkField;
        }
        set {
            this.majorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark minorTickMark {
        get {
            return this.minorTickMarkField;
        }
        set {
            this.minorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickLblPos tickLblPos {
        get {
            return this.tickLblPosField;
        }
        set {
            this.tickLblPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt crossAx {
        get {
            return this.crossAxField;
        }
        set {
            this.crossAxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("crosses", typeof(CT_Crosses))]
    [System.Xml.Serialization.XmlElementAttribute("crossesAt", typeof(CT_Double))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    public CT_Skip tickLblSkip {
        get {
            return this.tickLblSkipField;
        }
        set {
            this.tickLblSkipField = value;
        }
    }
    
    /// <remarks/>
    public CT_Skip tickMarkSkip {
        get {
            return this.tickMarkSkipField;
        }
        set {
            this.tickMarkSkipField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Scaling {
    
    private CT_LogBase logBaseField;
    
    private CT_Orientation orientationField;
    
    private CT_Double maxField;
    
    private CT_Double minField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_LogBase logBase {
        get {
            return this.logBaseField;
        }
        set {
            this.logBaseField = value;
        }
    }
    
    /// <remarks/>
    public CT_Orientation orientation {
        get {
            return this.orientationField;
        }
        set {
            this.orientationField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double max {
        get {
            return this.maxField;
        }
        set {
            this.maxField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double min {
        get {
            return this.minField;
        }
        set {
            this.minField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LogBase {
    
    private double valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Orientation {
    
    private ST_Orientation valField;
    
    public CT_Orientation() {
        this.valField = ST_Orientation.minMax;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_Orientation.minMax)]
    public ST_Orientation val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_Orientation {
    
    /// <remarks/>
    maxMin,
    
    /// <remarks/>
    minMax,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_AxPos {
    
    private ST_AxPos valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_AxPos val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_AxPos {
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    t,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ChartLines {
    
    private CT_ShapeProperties spPrField;
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Title {
    
    private CT_Tx txField;
    
    private CT_Layout layoutField;
    
    private CT_Boolean overlayField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Tx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_Layout layout {
        get {
            return this.layoutField;
        }
        set {
            this.layoutField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean overlay {
        get {
            return this.overlayField;
        }
        set {
            this.overlayField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Tx {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("rich", typeof(CT_TextBody))]
    [System.Xml.Serialization.XmlElementAttribute("strRef", typeof(CT_StrRef))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_StrRef {
    
    private string fField;
    
    private CT_StrData strCacheField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public string f {
        get {
            return this.fField;
        }
        set {
            this.fField = value;
        }
    }
    
    /// <remarks/>
    public CT_StrData strCache {
        get {
            return this.strCacheField;
        }
        set {
            this.strCacheField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_StrData {
    
    private CT_UnsignedInt ptCountField;
    
    private CT_StrVal[] ptField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt ptCount {
        get {
            return this.ptCountField;
        }
        set {
            this.ptCountField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("pt")]
    public CT_StrVal[] pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_StrVal {
    
    private string vField;
    
    private uint idxField;
    
    /// <remarks/>
    public string v {
        get {
            return this.vField;
        }
        set {
            this.vField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_NumFmt {
    
    private string formatCodeField;
    
    private bool sourceLinkedField;
    
    private bool sourceLinkedFieldSpecified;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string formatCode {
        get {
            return this.formatCodeField;
        }
        set {
            this.formatCodeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public bool sourceLinked {
        get {
            return this.sourceLinkedField;
        }
        set {
            this.sourceLinkedField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool sourceLinkedSpecified {
        get {
            return this.sourceLinkedFieldSpecified;
        }
        set {
            this.sourceLinkedFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TickMark {
    
    private ST_TickMark valField;
    
    public CT_TickMark() {
        this.valField = ST_TickMark.cross;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_TickMark.cross)]
    public ST_TickMark val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_TickMark {
    
    /// <remarks/>
    cross,
    
    /// <remarks/>
    @in,
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    @out,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TickLblPos {
    
    private ST_TickLblPos valField;
    
    public CT_TickLblPos() {
        this.valField = ST_TickLblPos.nextTo;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_TickLblPos.nextTo)]
    public ST_TickLblPos val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_TickLblPos {
    
    /// <remarks/>
    high,
    
    /// <remarks/>
    low,
    
    /// <remarks/>
    nextTo,
    
    /// <remarks/>
    none,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Crosses {
    
    private ST_Crosses valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_Crosses val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_Crosses {
    
    /// <remarks/>
    autoZero,
    
    /// <remarks/>
    max,
    
    /// <remarks/>
    min,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Skip {
    
    private uint valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TimeUnit {
    
    private ST_TimeUnit valField;
    
    public CT_TimeUnit() {
        this.valField = ST_TimeUnit.days;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_TimeUnit.days)]
    public ST_TimeUnit val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_TimeUnit {
    
    /// <remarks/>
    days,
    
    /// <remarks/>
    months,
    
    /// <remarks/>
    years,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DateAx {
    
    private CT_UnsignedInt axIdField;
    
    private CT_Scaling scalingField;
    
    private CT_Boolean deleteField;
    
    private CT_AxPos axPosField;
    
    private CT_ChartLines majorGridlinesField;
    
    private CT_ChartLines minorGridlinesField;
    
    private CT_Title titleField;
    
    private CT_NumFmt numFmtField;
    
    private CT_TickMark majorTickMarkField;
    
    private CT_TickMark minorTickMarkField;
    
    private CT_TickLblPos tickLblPosField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_UnsignedInt crossAxField;
    
    private object itemField;
    
    private CT_Boolean autoField;
    
    private CT_LblOffset lblOffsetField;
    
    private CT_TimeUnit baseTimeUnitField;
    
    private CT_AxisUnit majorUnitField;
    
    private CT_TimeUnit majorTimeUnitField;
    
    private CT_AxisUnit minorUnitField;
    
    private CT_TimeUnit minorTimeUnitField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scaling scaling {
        get {
            return this.scalingField;
        }
        set {
            this.scalingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean delete {
        get {
            return this.deleteField;
        }
        set {
            this.deleteField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxPos axPos {
        get {
            return this.axPosField;
        }
        set {
            this.axPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines majorGridlines {
        get {
            return this.majorGridlinesField;
        }
        set {
            this.majorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines minorGridlines {
        get {
            return this.minorGridlinesField;
        }
        set {
            this.minorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_Title title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumFmt numFmt {
        get {
            return this.numFmtField;
        }
        set {
            this.numFmtField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark majorTickMark {
        get {
            return this.majorTickMarkField;
        }
        set {
            this.majorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark minorTickMark {
        get {
            return this.minorTickMarkField;
        }
        set {
            this.minorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickLblPos tickLblPos {
        get {
            return this.tickLblPosField;
        }
        set {
            this.tickLblPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt crossAx {
        get {
            return this.crossAxField;
        }
        set {
            this.crossAxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("crosses", typeof(CT_Crosses))]
    [System.Xml.Serialization.XmlElementAttribute("crossesAt", typeof(CT_Double))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean auto {
        get {
            return this.autoField;
        }
        set {
            this.autoField = value;
        }
    }
    
    /// <remarks/>
    public CT_LblOffset lblOffset {
        get {
            return this.lblOffsetField;
        }
        set {
            this.lblOffsetField = value;
        }
    }
    
    /// <remarks/>
    public CT_TimeUnit baseTimeUnit {
        get {
            return this.baseTimeUnitField;
        }
        set {
            this.baseTimeUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxisUnit majorUnit {
        get {
            return this.majorUnitField;
        }
        set {
            this.majorUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_TimeUnit majorTimeUnit {
        get {
            return this.majorTimeUnitField;
        }
        set {
            this.majorTimeUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxisUnit minorUnit {
        get {
            return this.minorUnitField;
        }
        set {
            this.minorUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_TimeUnit minorTimeUnit {
        get {
            return this.minorTimeUnitField;
        }
        set {
            this.minorTimeUnitField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LblOffset {
    
    private string valField;
    
    public CT_LblOffset() {
        this.valField = "100%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_AxisUnit {
    
    private double valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LblAlgn {
    
    private ST_LblAlgn valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_LblAlgn val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_LblAlgn {
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    r,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_CatAx {
    
    private CT_UnsignedInt axIdField;
    
    private CT_Scaling scalingField;
    
    private CT_Boolean deleteField;
    
    private CT_AxPos axPosField;
    
    private CT_ChartLines majorGridlinesField;
    
    private CT_ChartLines minorGridlinesField;
    
    private CT_Title titleField;
    
    private CT_NumFmt numFmtField;
    
    private CT_TickMark majorTickMarkField;
    
    private CT_TickMark minorTickMarkField;
    
    private CT_TickLblPos tickLblPosField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_UnsignedInt crossAxField;
    
    private object itemField;
    
    private CT_Boolean autoField;
    
    private CT_LblAlgn lblAlgnField;
    
    private CT_LblOffset lblOffsetField;
    
    private CT_Skip tickLblSkipField;
    
    private CT_Skip tickMarkSkipField;
    
    private CT_Boolean noMultiLvlLblField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scaling scaling {
        get {
            return this.scalingField;
        }
        set {
            this.scalingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean delete {
        get {
            return this.deleteField;
        }
        set {
            this.deleteField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxPos axPos {
        get {
            return this.axPosField;
        }
        set {
            this.axPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines majorGridlines {
        get {
            return this.majorGridlinesField;
        }
        set {
            this.majorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines minorGridlines {
        get {
            return this.minorGridlinesField;
        }
        set {
            this.minorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_Title title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumFmt numFmt {
        get {
            return this.numFmtField;
        }
        set {
            this.numFmtField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark majorTickMark {
        get {
            return this.majorTickMarkField;
        }
        set {
            this.majorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark minorTickMark {
        get {
            return this.minorTickMarkField;
        }
        set {
            this.minorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickLblPos tickLblPos {
        get {
            return this.tickLblPosField;
        }
        set {
            this.tickLblPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt crossAx {
        get {
            return this.crossAxField;
        }
        set {
            this.crossAxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("crosses", typeof(CT_Crosses))]
    [System.Xml.Serialization.XmlElementAttribute("crossesAt", typeof(CT_Double))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean auto {
        get {
            return this.autoField;
        }
        set {
            this.autoField = value;
        }
    }
    
    /// <remarks/>
    public CT_LblAlgn lblAlgn {
        get {
            return this.lblAlgnField;
        }
        set {
            this.lblAlgnField = value;
        }
    }
    
    /// <remarks/>
    public CT_LblOffset lblOffset {
        get {
            return this.lblOffsetField;
        }
        set {
            this.lblOffsetField = value;
        }
    }
    
    /// <remarks/>
    public CT_Skip tickLblSkip {
        get {
            return this.tickLblSkipField;
        }
        set {
            this.tickLblSkipField = value;
        }
    }
    
    /// <remarks/>
    public CT_Skip tickMarkSkip {
        get {
            return this.tickMarkSkipField;
        }
        set {
            this.tickMarkSkipField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean noMultiLvlLbl {
        get {
            return this.noMultiLvlLblField;
        }
        set {
            this.noMultiLvlLblField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DispUnitsLbl {
    
    private CT_Layout layoutField;
    
    private CT_Tx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    /// <remarks/>
    public CT_Layout layout {
        get {
            return this.layoutField;
        }
        set {
            this.layoutField = value;
        }
    }
    
    /// <remarks/>
    public CT_Tx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BuiltInUnit {
    
    private ST_BuiltInUnit valField;
    
    public CT_BuiltInUnit() {
        this.valField = ST_BuiltInUnit.thousands;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_BuiltInUnit.thousands)]
    public ST_BuiltInUnit val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_BuiltInUnit {
    
    /// <remarks/>
    hundreds,
    
    /// <remarks/>
    thousands,
    
    /// <remarks/>
    tenThousands,
    
    /// <remarks/>
    hundredThousands,
    
    /// <remarks/>
    millions,
    
    /// <remarks/>
    tenMillions,
    
    /// <remarks/>
    hundredMillions,
    
    /// <remarks/>
    billions,
    
    /// <remarks/>
    trillions,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DispUnits {
    
    private object itemField;
    
    private CT_DispUnitsLbl dispUnitsLblField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("builtInUnit", typeof(CT_BuiltInUnit))]
    [System.Xml.Serialization.XmlElementAttribute("custUnit", typeof(CT_Double))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    public CT_DispUnitsLbl dispUnitsLbl {
        get {
            return this.dispUnitsLblField;
        }
        set {
            this.dispUnitsLblField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_CrossBetween {
    
    private ST_CrossBetween valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_CrossBetween val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_CrossBetween {
    
    /// <remarks/>
    between,
    
    /// <remarks/>
    midCat,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ValAx {
    
    private CT_UnsignedInt axIdField;
    
    private CT_Scaling scalingField;
    
    private CT_Boolean deleteField;
    
    private CT_AxPos axPosField;
    
    private CT_ChartLines majorGridlinesField;
    
    private CT_ChartLines minorGridlinesField;
    
    private CT_Title titleField;
    
    private CT_NumFmt numFmtField;
    
    private CT_TickMark majorTickMarkField;
    
    private CT_TickMark minorTickMarkField;
    
    private CT_TickLblPos tickLblPosField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_UnsignedInt crossAxField;
    
    private object itemField;
    
    private CT_CrossBetween crossBetweenField;
    
    private CT_AxisUnit majorUnitField;
    
    private CT_AxisUnit minorUnitField;
    
    private CT_DispUnits dispUnitsField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scaling scaling {
        get {
            return this.scalingField;
        }
        set {
            this.scalingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean delete {
        get {
            return this.deleteField;
        }
        set {
            this.deleteField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxPos axPos {
        get {
            return this.axPosField;
        }
        set {
            this.axPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines majorGridlines {
        get {
            return this.majorGridlinesField;
        }
        set {
            this.majorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines minorGridlines {
        get {
            return this.minorGridlinesField;
        }
        set {
            this.minorGridlinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_Title title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumFmt numFmt {
        get {
            return this.numFmtField;
        }
        set {
            this.numFmtField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark majorTickMark {
        get {
            return this.majorTickMarkField;
        }
        set {
            this.majorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickMark minorTickMark {
        get {
            return this.minorTickMarkField;
        }
        set {
            this.minorTickMarkField = value;
        }
    }
    
    /// <remarks/>
    public CT_TickLblPos tickLblPos {
        get {
            return this.tickLblPosField;
        }
        set {
            this.tickLblPosField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt crossAx {
        get {
            return this.crossAxField;
        }
        set {
            this.crossAxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("crosses", typeof(CT_Crosses))]
    [System.Xml.Serialization.XmlElementAttribute("crossesAt", typeof(CT_Double))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
    
    /// <remarks/>
    public CT_CrossBetween crossBetween {
        get {
            return this.crossBetweenField;
        }
        set {
            this.crossBetweenField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxisUnit majorUnit {
        get {
            return this.majorUnitField;
        }
        set {
            this.majorUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxisUnit minorUnit {
        get {
            return this.minorUnitField;
        }
        set {
            this.minorUnitField = value;
        }
    }
    
    /// <remarks/>
    public CT_DispUnits dispUnits {
        get {
            return this.dispUnitsField;
        }
        set {
            this.dispUnitsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SizeRepresents {
    
    private ST_SizeRepresents valField;
    
    public CT_SizeRepresents() {
        this.valField = ST_SizeRepresents.area;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_SizeRepresents.area)]
    public ST_SizeRepresents val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_SizeRepresents {
    
    /// <remarks/>
    area,
    
    /// <remarks/>
    w,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BubbleScale {
    
    private string valField;
    
    public CT_BubbleScale() {
        this.valField = "100%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BubbleSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Boolean invertIfNegativeField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_Trendline[] trendlineField;
    
    private CT_ErrBars[] errBarsField;
    
    private CT_AxDataSource xValField;
    
    private CT_NumDataSource yValField;
    
    private CT_NumDataSource bubbleSizeField;
    
    private CT_Boolean bubble3DField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean invertIfNegative {
        get {
            return this.invertIfNegativeField;
        }
        set {
            this.invertIfNegativeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("trendline")]
    public CT_Trendline[] trendline {
        get {
            return this.trendlineField;
        }
        set {
            this.trendlineField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("errBars")]
    public CT_ErrBars[] errBars {
        get {
            return this.errBarsField;
        }
        set {
            this.errBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource xVal {
        get {
            return this.xValField;
        }
        set {
            this.xValField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource yVal {
        get {
            return this.yValField;
        }
        set {
            this.yValField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource bubbleSize {
        get {
            return this.bubbleSizeField;
        }
        set {
            this.bubbleSizeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean bubble3D {
        get {
            return this.bubble3DField;
        }
        set {
            this.bubble3DField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SerTx {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("strRef", typeof(CT_StrRef))]
    [System.Xml.Serialization.XmlElementAttribute("v", typeof(string))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DPt {
    
    private CT_UnsignedInt idxField;
    
    private CT_Boolean invertIfNegativeField;
    
    private CT_Marker markerField;
    
    private CT_Boolean bubble3DField;
    
    private CT_UnsignedInt explosionField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_PictureOptions pictureOptionsField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean invertIfNegative {
        get {
            return this.invertIfNegativeField;
        }
        set {
            this.invertIfNegativeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean bubble3D {
        get {
            return this.bubble3DField;
        }
        set {
            this.bubble3DField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt explosion {
        get {
            return this.explosionField;
        }
        set {
            this.explosionField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureOptions pictureOptions {
        get {
            return this.pictureOptionsField;
        }
        set {
            this.pictureOptionsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Marker {
    
    private CT_MarkerStyle symbolField;
    
    private CT_MarkerSize sizeField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_MarkerStyle symbol {
        get {
            return this.symbolField;
        }
        set {
            this.symbolField = value;
        }
    }
    
    /// <remarks/>
    public CT_MarkerSize size {
        get {
            return this.sizeField;
        }
        set {
            this.sizeField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_MarkerStyle {
    
    private ST_MarkerStyle valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_MarkerStyle val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_MarkerStyle {
    
    /// <remarks/>
    circle,
    
    /// <remarks/>
    dash,
    
    /// <remarks/>
    diamond,
    
    /// <remarks/>
    dot,
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    picture,
    
    /// <remarks/>
    plus,
    
    /// <remarks/>
    square,
    
    /// <remarks/>
    star,
    
    /// <remarks/>
    triangle,
    
    /// <remarks/>
    x,
    
    /// <remarks/>
    auto,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_MarkerSize {
    
    private byte valField;
    
    public CT_MarkerSize() {
        this.valField = ((byte)(5));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(byte), "5")]
    public byte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PictureOptions {
    
    private CT_Boolean applyToFrontField;
    
    private CT_Boolean applyToSidesField;
    
    private CT_Boolean applyToEndField;
    
    private CT_PictureFormat pictureFormatField;
    
    private CT_PictureStackUnit pictureStackUnitField;
    
    /// <remarks/>
    public CT_Boolean applyToFront {
        get {
            return this.applyToFrontField;
        }
        set {
            this.applyToFrontField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean applyToSides {
        get {
            return this.applyToSidesField;
        }
        set {
            this.applyToSidesField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean applyToEnd {
        get {
            return this.applyToEndField;
        }
        set {
            this.applyToEndField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureFormat pictureFormat {
        get {
            return this.pictureFormatField;
        }
        set {
            this.pictureFormatField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureStackUnit pictureStackUnit {
        get {
            return this.pictureStackUnitField;
        }
        set {
            this.pictureStackUnitField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PictureFormat {
    
    private ST_PictureFormat valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_PictureFormat val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_PictureFormat {
    
    /// <remarks/>
    stretch,
    
    /// <remarks/>
    stack,
    
    /// <remarks/>
    stackScale,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PictureStackUnit {
    
    private double valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public double val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DLbls {
    
    private CT_DLbl[] dLblField;
    
    private object[] itemsField;
    
    private ItemsChoiceType2[] itemsElementNameField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dLbl")]
    public CT_DLbl[] dLbl {
        get {
            return this.dLblField;
        }
        set {
            this.dLblField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dLblPos", typeof(CT_DLblPos))]
    [System.Xml.Serialization.XmlElementAttribute("delete", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("leaderLines", typeof(CT_ChartLines))]
    [System.Xml.Serialization.XmlElementAttribute("numFmt", typeof(CT_NumFmt))]
    [System.Xml.Serialization.XmlElementAttribute("separator", typeof(string))]
    [System.Xml.Serialization.XmlElementAttribute("showBubbleSize", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showCatName", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showLeaderLines", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showLegendKey", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showPercent", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showSerName", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showVal", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("spPr", typeof(CT_ShapeProperties))]
    [System.Xml.Serialization.XmlElementAttribute("txPr", typeof(CT_TextBody))]
    [System.Xml.Serialization.XmlChoiceIdentifierAttribute("ItemsElementName")]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ItemsElementName")]
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public ItemsChoiceType2[] ItemsElementName {
        get {
            return this.itemsElementNameField;
        }
        set {
            this.itemsElementNameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DLbl {
    
    private CT_UnsignedInt idxField;
    
    private object[] itemsField;
    
    private ItemsChoiceType1[] itemsElementNameField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dLblPos", typeof(CT_DLblPos))]
    [System.Xml.Serialization.XmlElementAttribute("delete", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("layout", typeof(CT_Layout))]
    [System.Xml.Serialization.XmlElementAttribute("numFmt", typeof(CT_NumFmt))]
    [System.Xml.Serialization.XmlElementAttribute("separator", typeof(string))]
    [System.Xml.Serialization.XmlElementAttribute("showBubbleSize", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showCatName", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showLegendKey", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showPercent", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showSerName", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("showVal", typeof(CT_Boolean))]
    [System.Xml.Serialization.XmlElementAttribute("spPr", typeof(CT_ShapeProperties))]
    [System.Xml.Serialization.XmlElementAttribute("tx", typeof(CT_Tx))]
    [System.Xml.Serialization.XmlElementAttribute("txPr", typeof(CT_TextBody))]
    [System.Xml.Serialization.XmlChoiceIdentifierAttribute("ItemsElementName")]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ItemsElementName")]
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public ItemsChoiceType1[] ItemsElementName {
        get {
            return this.itemsElementNameField;
        }
        set {
            this.itemsElementNameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DLblPos {
    
    private ST_DLblPos valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_DLblPos val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_DLblPos {
    
    /// <remarks/>
    bestFit,
    
    /// <remarks/>
    b,
    
    /// <remarks/>
    ctr,
    
    /// <remarks/>
    inBase,
    
    /// <remarks/>
    inEnd,
    
    /// <remarks/>
    l,
    
    /// <remarks/>
    outEnd,
    
    /// <remarks/>
    r,
    
    /// <remarks/>
    t,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart", IncludeInSchema=false)]
public enum ItemsChoiceType1 {
    
    /// <remarks/>
    dLblPos,
    
    /// <remarks/>
    delete,
    
    /// <remarks/>
    layout,
    
    /// <remarks/>
    numFmt,
    
    /// <remarks/>
    separator,
    
    /// <remarks/>
    showBubbleSize,
    
    /// <remarks/>
    showCatName,
    
    /// <remarks/>
    showLegendKey,
    
    /// <remarks/>
    showPercent,
    
    /// <remarks/>
    showSerName,
    
    /// <remarks/>
    showVal,
    
    /// <remarks/>
    spPr,
    
    /// <remarks/>
    tx,
    
    /// <remarks/>
    txPr,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart", IncludeInSchema=false)]
public enum ItemsChoiceType2 {
    
    /// <remarks/>
    dLblPos,
    
    /// <remarks/>
    delete,
    
    /// <remarks/>
    leaderLines,
    
    /// <remarks/>
    numFmt,
    
    /// <remarks/>
    separator,
    
    /// <remarks/>
    showBubbleSize,
    
    /// <remarks/>
    showCatName,
    
    /// <remarks/>
    showLeaderLines,
    
    /// <remarks/>
    showLegendKey,
    
    /// <remarks/>
    showPercent,
    
    /// <remarks/>
    showSerName,
    
    /// <remarks/>
    showVal,
    
    /// <remarks/>
    spPr,
    
    /// <remarks/>
    txPr,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Trendline {
    
    private string nameField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TrendlineType trendlineTypeField;
    
    private CT_Order orderField;
    
    private CT_Period periodField;
    
    private CT_Double forwardField;
    
    private CT_Double backwardField;
    
    private CT_Double interceptField;
    
    private CT_Boolean dispRSqrField;
    
    private CT_Boolean dispEqField;
    
    private CT_TrendlineLbl trendlineLblField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TrendlineType trendlineType {
        get {
            return this.trendlineTypeField;
        }
        set {
            this.trendlineTypeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Order order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_Period period {
        get {
            return this.periodField;
        }
        set {
            this.periodField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double forward {
        get {
            return this.forwardField;
        }
        set {
            this.forwardField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double backward {
        get {
            return this.backwardField;
        }
        set {
            this.backwardField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double intercept {
        get {
            return this.interceptField;
        }
        set {
            this.interceptField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean dispRSqr {
        get {
            return this.dispRSqrField;
        }
        set {
            this.dispRSqrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean dispEq {
        get {
            return this.dispEqField;
        }
        set {
            this.dispEqField = value;
        }
    }
    
    /// <remarks/>
    public CT_TrendlineLbl trendlineLbl {
        get {
            return this.trendlineLblField;
        }
        set {
            this.trendlineLblField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TrendlineType {
    
    private ST_TrendlineType valField;
    
    public CT_TrendlineType() {
        this.valField = ST_TrendlineType.linear;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_TrendlineType.linear)]
    public ST_TrendlineType val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_TrendlineType {
    
    /// <remarks/>
    exp,
    
    /// <remarks/>
    linear,
    
    /// <remarks/>
    log,
    
    /// <remarks/>
    movingAvg,
    
    /// <remarks/>
    poly,
    
    /// <remarks/>
    power,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Order {
    
    private byte valField;
    
    public CT_Order() {
        this.valField = ((byte)(2));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(byte), "2")]
    public byte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Period {
    
    private uint valField;
    
    public CT_Period() {
        this.valField = ((uint)(2));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(uint), "2")]
    public uint val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TrendlineLbl {
    
    private CT_Layout layoutField;
    
    private CT_Tx txField;
    
    private CT_NumFmt numFmtField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Layout layout {
        get {
            return this.layoutField;
        }
        set {
            this.layoutField = value;
        }
    }
    
    /// <remarks/>
    public CT_Tx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumFmt numFmt {
        get {
            return this.numFmtField;
        }
        set {
            this.numFmtField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ErrBars {
    
    private CT_ErrDir errDirField;
    
    private CT_ErrBarType errBarTypeField;
    
    private CT_ErrValType errValTypeField;
    
    private CT_Boolean noEndCapField;
    
    private CT_NumDataSource plusField;
    
    private CT_NumDataSource minusField;
    
    private CT_Double valField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_ErrDir errDir {
        get {
            return this.errDirField;
        }
        set {
            this.errDirField = value;
        }
    }
    
    /// <remarks/>
    public CT_ErrBarType errBarType {
        get {
            return this.errBarTypeField;
        }
        set {
            this.errBarTypeField = value;
        }
    }
    
    /// <remarks/>
    public CT_ErrValType errValType {
        get {
            return this.errValTypeField;
        }
        set {
            this.errValTypeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean noEndCap {
        get {
            return this.noEndCapField;
        }
        set {
            this.noEndCapField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource plus {
        get {
            return this.plusField;
        }
        set {
            this.plusField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource minus {
        get {
            return this.minusField;
        }
        set {
            this.minusField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ErrDir {
    
    private ST_ErrDir valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ErrDir val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_ErrDir {
    
    /// <remarks/>
    x,
    
    /// <remarks/>
    y,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ErrBarType {
    
    private ST_ErrBarType valField;
    
    public CT_ErrBarType() {
        this.valField = ST_ErrBarType.both;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_ErrBarType.both)]
    public ST_ErrBarType val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_ErrBarType {
    
    /// <remarks/>
    both,
    
    /// <remarks/>
    minus,
    
    /// <remarks/>
    plus,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ErrValType {
    
    private ST_ErrValType valField;
    
    public CT_ErrValType() {
        this.valField = ST_ErrValType.fixedVal;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_ErrValType.fixedVal)]
    public ST_ErrValType val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_ErrValType {
    
    /// <remarks/>
    cust,
    
    /// <remarks/>
    fixedVal,
    
    /// <remarks/>
    percentage,
    
    /// <remarks/>
    stdDev,
    
    /// <remarks/>
    stdErr,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_NumDataSource {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("numLit", typeof(CT_NumData))]
    [System.Xml.Serialization.XmlElementAttribute("numRef", typeof(CT_NumRef))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_NumData {
    
    private string formatCodeField;
    
    private CT_UnsignedInt ptCountField;
    
    private CT_NumVal[] ptField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public string formatCode {
        get {
            return this.formatCodeField;
        }
        set {
            this.formatCodeField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt ptCount {
        get {
            return this.ptCountField;
        }
        set {
            this.ptCountField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("pt")]
    public CT_NumVal[] pt {
        get {
            return this.ptField;
        }
        set {
            this.ptField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_NumVal {
    
    private string vField;
    
    private uint idxField;
    
    private string formatCodeField;
    
    /// <remarks/>
    public string v {
        get {
            return this.vField;
        }
        set {
            this.vField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string formatCode {
        get {
            return this.formatCodeField;
        }
        set {
            this.formatCodeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_NumRef {
    
    private string fField;
    
    private CT_NumData numCacheField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public string f {
        get {
            return this.fField;
        }
        set {
            this.fField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumData numCache {
        get {
            return this.numCacheField;
        }
        set {
            this.numCacheField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_AxDataSource {
    
    private object itemField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("multiLvlStrRef", typeof(CT_MultiLvlStrRef))]
    [System.Xml.Serialization.XmlElementAttribute("numLit", typeof(CT_NumData))]
    [System.Xml.Serialization.XmlElementAttribute("numRef", typeof(CT_NumRef))]
    [System.Xml.Serialization.XmlElementAttribute("strLit", typeof(CT_StrData))]
    [System.Xml.Serialization.XmlElementAttribute("strRef", typeof(CT_StrRef))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_MultiLvlStrRef {
    
    private string fField;
    
    private CT_MultiLvlStrData multiLvlStrCacheField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public string f {
        get {
            return this.fField;
        }
        set {
            this.fField = value;
        }
    }
    
    /// <remarks/>
    public CT_MultiLvlStrData multiLvlStrCache {
        get {
            return this.multiLvlStrCacheField;
        }
        set {
            this.multiLvlStrCacheField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_MultiLvlStrData {
    
    private CT_UnsignedInt ptCountField;
    
    private CT_StrVal[][] lvlField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt ptCount {
        get {
            return this.ptCountField;
        }
        set {
            this.ptCountField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("pt", typeof(CT_StrVal), IsNullable=false)]
    public CT_StrVal[][] lvl {
        get {
            return this.lvlField;
        }
        set {
            this.lvlField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BubbleChart {
    
    private CT_Boolean varyColorsField;
    
    private CT_BubbleSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_Boolean bubble3DField;
    
    private CT_BubbleScale bubbleScaleField;
    
    private CT_Boolean showNegBubblesField;
    
    private CT_SizeRepresents sizeRepresentsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_BubbleSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean bubble3D {
        get {
            return this.bubble3DField;
        }
        set {
            this.bubble3DField = value;
        }
    }
    
    /// <remarks/>
    public CT_BubbleScale bubbleScale {
        get {
            return this.bubbleScaleField;
        }
        set {
            this.bubbleScaleField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean showNegBubbles {
        get {
            return this.showNegBubblesField;
        }
        set {
            this.showNegBubblesField = value;
        }
    }
    
    /// <remarks/>
    public CT_SizeRepresents sizeRepresents {
        get {
            return this.sizeRepresentsField;
        }
        set {
            this.sizeRepresentsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Surface3DChart {
    
    private CT_Boolean wireframeField;
    
    private CT_SurfaceSer[] serField;
    
    private CT_BandFmt[] bandFmtsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean wireframe {
        get {
            return this.wireframeField;
        }
        set {
            this.wireframeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_SurfaceSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("bandFmt", IsNullable=false)]
    public CT_BandFmt[] bandFmts {
        get {
            return this.bandFmtsField;
        }
        set {
            this.bandFmtsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SurfaceSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BandFmt {
    
    private CT_UnsignedInt idxField;
    
    private CT_ShapeProperties spPrField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SurfaceChart {
    
    private CT_Boolean wireframeField;
    
    private CT_SurfaceSer[] serField;
    
    private CT_BandFmt[] bandFmtsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean wireframe {
        get {
            return this.wireframeField;
        }
        set {
            this.wireframeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_SurfaceSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("bandFmt", IsNullable=false)]
    public CT_BandFmt[] bandFmts {
        get {
            return this.bandFmtsField;
        }
        set {
            this.bandFmtsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SecondPieSize {
    
    private string valField;
    
    public CT_SecondPieSize() {
        this.valField = "75%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("75%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_SplitType {
    
    private ST_SplitType valField;
    
    public CT_SplitType() {
        this.valField = ST_SplitType.auto;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_SplitType.auto)]
    public ST_SplitType val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_SplitType {
    
    /// <remarks/>
    auto,
    
    /// <remarks/>
    cust,
    
    /// <remarks/>
    percent,
    
    /// <remarks/>
    pos,
    
    /// <remarks/>
    val,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_OfPieType {
    
    private ST_OfPieType valField;
    
    public CT_OfPieType() {
        this.valField = ST_OfPieType.pie;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_OfPieType.pie)]
    public ST_OfPieType val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_OfPieType {
    
    /// <remarks/>
    pie,
    
    /// <remarks/>
    bar,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_OfPieChart {
    
    private CT_OfPieType ofPieTypeField;
    
    private CT_Boolean varyColorsField;
    
    private CT_PieSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_GapAmount gapWidthField;
    
    private CT_SplitType splitTypeField;
    
    private CT_Double splitPosField;
    
    private CT_UnsignedInt[] custSplitField;
    
    private CT_SecondPieSize secondPieSizeField;
    
    private CT_ChartLines[] serLinesField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_OfPieType ofPieType {
        get {
            return this.ofPieTypeField;
        }
        set {
            this.ofPieTypeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_PieSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapWidth {
        get {
            return this.gapWidthField;
        }
        set {
            this.gapWidthField = value;
        }
    }
    
    /// <remarks/>
    public CT_SplitType splitType {
        get {
            return this.splitTypeField;
        }
        set {
            this.splitTypeField = value;
        }
    }
    
    /// <remarks/>
    public CT_Double splitPos {
        get {
            return this.splitPosField;
        }
        set {
            this.splitPosField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("secondPiePt", IsNullable=false)]
    public CT_UnsignedInt[] custSplit {
        get {
            return this.custSplitField;
        }
        set {
            this.custSplitField = value;
        }
    }
    
    /// <remarks/>
    public CT_SecondPieSize secondPieSize {
        get {
            return this.secondPieSizeField;
        }
        set {
            this.secondPieSizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("serLines")]
    public CT_ChartLines[] serLines {
        get {
            return this.serLinesField;
        }
        set {
            this.serLinesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PieSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_UnsignedInt explosionField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt explosion {
        get {
            return this.explosionField;
        }
        set {
            this.explosionField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_GapAmount {
    
    private string valField;
    
    public CT_GapAmount() {
        this.valField = "150%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("150%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Bar3DChart {
    
    private CT_BarDir barDirField;
    
    private CT_BarGrouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_BarSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_GapAmount gapWidthField;
    
    private CT_GapAmount gapDepthField;
    
    private CT_Shape shapeField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_BarDir barDir {
        get {
            return this.barDirField;
        }
        set {
            this.barDirField = value;
        }
    }
    
    /// <remarks/>
    public CT_BarGrouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_BarSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapWidth {
        get {
            return this.gapWidthField;
        }
        set {
            this.gapWidthField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapDepth {
        get {
            return this.gapDepthField;
        }
        set {
            this.gapDepthField = value;
        }
    }
    
    /// <remarks/>
    public CT_Shape shape {
        get {
            return this.shapeField;
        }
        set {
            this.shapeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BarDir {
    
    private ST_BarDir valField;
    
    public CT_BarDir() {
        this.valField = ST_BarDir.col;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_BarDir.col)]
    public ST_BarDir val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_BarDir {
    
    /// <remarks/>
    bar,
    
    /// <remarks/>
    col,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BarGrouping {
    
    private ST_BarGrouping valField;
    
    public CT_BarGrouping() {
        this.valField = ST_BarGrouping.clustered;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_BarGrouping.clustered)]
    public ST_BarGrouping val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_BarGrouping {
    
    /// <remarks/>
    percentStacked,
    
    /// <remarks/>
    clustered,
    
    /// <remarks/>
    standard,
    
    /// <remarks/>
    stacked,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BarSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Boolean invertIfNegativeField;
    
    private CT_PictureOptions pictureOptionsField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_Trendline[] trendlineField;
    
    private CT_ErrBars errBarsField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Shape shapeField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean invertIfNegative {
        get {
            return this.invertIfNegativeField;
        }
        set {
            this.invertIfNegativeField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureOptions pictureOptions {
        get {
            return this.pictureOptionsField;
        }
        set {
            this.pictureOptionsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("trendline")]
    public CT_Trendline[] trendline {
        get {
            return this.trendlineField;
        }
        set {
            this.trendlineField = value;
        }
    }
    
    /// <remarks/>
    public CT_ErrBars errBars {
        get {
            return this.errBarsField;
        }
        set {
            this.errBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    public CT_Shape shape {
        get {
            return this.shapeField;
        }
        set {
            this.shapeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Shape {
    
    private ST_Shape valField;
    
    public CT_Shape() {
        this.valField = ST_Shape.box;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_Shape.box)]
    public ST_Shape val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_Shape {
    
    /// <remarks/>
    cone,
    
    /// <remarks/>
    coneToMax,
    
    /// <remarks/>
    box,
    
    /// <remarks/>
    cylinder,
    
    /// <remarks/>
    pyramid,
    
    /// <remarks/>
    pyramidToMax,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Overlap {
    
    private string valField;
    
    public CT_Overlap() {
        this.valField = "0%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("0%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_BarChart {
    
    private CT_BarDir barDirField;
    
    private CT_BarGrouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_BarSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_GapAmount gapWidthField;
    
    private CT_Overlap overlapField;
    
    private CT_ChartLines[] serLinesField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_BarDir barDir {
        get {
            return this.barDirField;
        }
        set {
            this.barDirField = value;
        }
    }
    
    /// <remarks/>
    public CT_BarGrouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_BarSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapWidth {
        get {
            return this.gapWidthField;
        }
        set {
            this.gapWidthField = value;
        }
    }
    
    /// <remarks/>
    public CT_Overlap overlap {
        get {
            return this.overlapField;
        }
        set {
            this.overlapField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("serLines")]
    public CT_ChartLines[] serLines {
        get {
            return this.serLinesField;
        }
        set {
            this.serLinesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_HoleSize {
    
    private string valField;
    
    public CT_HoleSize() {
        this.valField = "10%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("10%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DoughnutChart {
    
    private CT_Boolean varyColorsField;
    
    private CT_PieSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_FirstSliceAng firstSliceAngField;
    
    private CT_HoleSize holeSizeField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_PieSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_FirstSliceAng firstSliceAng {
        get {
            return this.firstSliceAngField;
        }
        set {
            this.firstSliceAngField = value;
        }
    }
    
    /// <remarks/>
    public CT_HoleSize holeSize {
        get {
            return this.holeSizeField;
        }
        set {
            this.holeSizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_FirstSliceAng {
    
    private ushort valField;
    
    public CT_FirstSliceAng() {
        this.valField = ((ushort)(0));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(ushort), "0")]
    public ushort val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Pie3DChart {
    
    private CT_Boolean varyColorsField;
    
    private CT_PieSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_PieSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PieChart {
    
    private CT_Boolean varyColorsField;
    
    private CT_PieSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_FirstSliceAng firstSliceAngField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_PieSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_FirstSliceAng firstSliceAng {
        get {
            return this.firstSliceAngField;
        }
        set {
            this.firstSliceAngField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ScatterSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Marker markerField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_Trendline[] trendlineField;
    
    private CT_ErrBars[] errBarsField;
    
    private CT_AxDataSource xValField;
    
    private CT_NumDataSource yValField;
    
    private CT_Boolean smoothField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("trendline")]
    public CT_Trendline[] trendline {
        get {
            return this.trendlineField;
        }
        set {
            this.trendlineField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("errBars")]
    public CT_ErrBars[] errBars {
        get {
            return this.errBarsField;
        }
        set {
            this.errBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource xVal {
        get {
            return this.xValField;
        }
        set {
            this.xValField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource yVal {
        get {
            return this.yValField;
        }
        set {
            this.yValField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean smooth {
        get {
            return this.smoothField;
        }
        set {
            this.smoothField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ScatterStyle {
    
    private ST_ScatterStyle valField;
    
    public CT_ScatterStyle() {
        this.valField = ST_ScatterStyle.marker;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_ScatterStyle.marker)]
    public ST_ScatterStyle val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_ScatterStyle {
    
    /// <remarks/>
    none,
    
    /// <remarks/>
    line,
    
    /// <remarks/>
    lineMarker,
    
    /// <remarks/>
    marker,
    
    /// <remarks/>
    smooth,
    
    /// <remarks/>
    smoothMarker,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_ScatterChart {
    
    private CT_ScatterStyle scatterStyleField;
    
    private CT_Boolean varyColorsField;
    
    private CT_ScatterSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_ScatterStyle scatterStyle {
        get {
            return this.scatterStyleField;
        }
        set {
            this.scatterStyleField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_ScatterSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_RadarSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Marker markerField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_RadarStyle {
    
    private ST_RadarStyle valField;
    
    public CT_RadarStyle() {
        this.valField = ST_RadarStyle.standard;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_RadarStyle.standard)]
    public ST_RadarStyle val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_RadarStyle {
    
    /// <remarks/>
    standard,
    
    /// <remarks/>
    marker,
    
    /// <remarks/>
    filled,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_RadarChart {
    
    private CT_RadarStyle radarStyleField;
    
    private CT_Boolean varyColorsField;
    
    private CT_RadarSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_RadarStyle radarStyle {
        get {
            return this.radarStyleField;
        }
        set {
            this.radarStyleField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_RadarSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_StockChart {
    
    private CT_LineSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_ChartLines dropLinesField;
    
    private CT_ChartLines hiLowLinesField;
    
    private CT_UpDownBars upDownBarsField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_LineSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines dropLines {
        get {
            return this.dropLinesField;
        }
        set {
            this.dropLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines hiLowLines {
        get {
            return this.hiLowLinesField;
        }
        set {
            this.hiLowLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_UpDownBars upDownBars {
        get {
            return this.upDownBarsField;
        }
        set {
            this.upDownBarsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LineSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Marker markerField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_Trendline[] trendlineField;
    
    private CT_ErrBars errBarsField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Boolean smoothField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("trendline")]
    public CT_Trendline[] trendline {
        get {
            return this.trendlineField;
        }
        set {
            this.trendlineField = value;
        }
    }
    
    /// <remarks/>
    public CT_ErrBars errBars {
        get {
            return this.errBarsField;
        }
        set {
            this.errBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean smooth {
        get {
            return this.smoothField;
        }
        set {
            this.smoothField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_UpDownBars {
    
    private CT_GapAmount gapWidthField;
    
    private CT_UpDownBar upBarsField;
    
    private CT_UpDownBar downBarsField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_GapAmount gapWidth {
        get {
            return this.gapWidthField;
        }
        set {
            this.gapWidthField = value;
        }
    }
    
    /// <remarks/>
    public CT_UpDownBar upBars {
        get {
            return this.upBarsField;
        }
        set {
            this.upBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_UpDownBar downBars {
        get {
            return this.downBarsField;
        }
        set {
            this.downBarsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_UpDownBar {
    
    private CT_ShapeProperties spPrField;
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Line3DChart {
    
    private CT_Grouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_LineSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_ChartLines dropLinesField;
    
    private CT_GapAmount gapDepthField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Grouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_LineSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines dropLines {
        get {
            return this.dropLinesField;
        }
        set {
            this.dropLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapDepth {
        get {
            return this.gapDepthField;
        }
        set {
            this.gapDepthField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Grouping {
    
    private ST_Grouping valField;
    
    public CT_Grouping() {
        this.valField = ST_Grouping.standard;
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(ST_Grouping.standard)]
    public ST_Grouping val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public enum ST_Grouping {
    
    /// <remarks/>
    percentStacked,
    
    /// <remarks/>
    standard,
    
    /// <remarks/>
    stacked,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_LineChart {
    
    private CT_Grouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_LineSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_ChartLines dropLinesField;
    
    private CT_ChartLines hiLowLinesField;
    
    private CT_UpDownBars upDownBarsField;
    
    private CT_Boolean markerField;
    
    private CT_Boolean smoothField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Grouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_LineSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines dropLines {
        get {
            return this.dropLinesField;
        }
        set {
            this.dropLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines hiLowLines {
        get {
            return this.hiLowLinesField;
        }
        set {
            this.hiLowLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_UpDownBars upDownBars {
        get {
            return this.upDownBarsField;
        }
        set {
            this.upDownBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean smooth {
        get {
            return this.smoothField;
        }
        set {
            this.smoothField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Area3DChart {
    
    private CT_Grouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_AreaSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_ChartLines dropLinesField;
    
    private CT_GapAmount gapDepthField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Grouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_AreaSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines dropLines {
        get {
            return this.dropLinesField;
        }
        set {
            this.dropLinesField = value;
        }
    }
    
    /// <remarks/>
    public CT_GapAmount gapDepth {
        get {
            return this.gapDepthField;
        }
        set {
            this.gapDepthField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_AreaSer {
    
    private CT_UnsignedInt idxField;
    
    private CT_UnsignedInt orderField;
    
    private CT_SerTx txField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_PictureOptions pictureOptionsField;
    
    private CT_DPt[] dPtField;
    
    private CT_DLbls dLblsField;
    
    private CT_Trendline[] trendlineField;
    
    private CT_ErrBars[] errBarsField;
    
    private CT_AxDataSource catField;
    
    private CT_NumDataSource valField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt order {
        get {
            return this.orderField;
        }
        set {
            this.orderField = value;
        }
    }
    
    /// <remarks/>
    public CT_SerTx tx {
        get {
            return this.txField;
        }
        set {
            this.txField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureOptions pictureOptions {
        get {
            return this.pictureOptionsField;
        }
        set {
            this.pictureOptionsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("dPt")]
    public CT_DPt[] dPt {
        get {
            return this.dPtField;
        }
        set {
            this.dPtField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("trendline")]
    public CT_Trendline[] trendline {
        get {
            return this.trendlineField;
        }
        set {
            this.trendlineField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("errBars")]
    public CT_ErrBars[] errBars {
        get {
            return this.errBarsField;
        }
        set {
            this.errBarsField = value;
        }
    }
    
    /// <remarks/>
    public CT_AxDataSource cat {
        get {
            return this.catField;
        }
        set {
            this.catField = value;
        }
    }
    
    /// <remarks/>
    public CT_NumDataSource val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_AreaChart {
    
    private CT_Grouping groupingField;
    
    private CT_Boolean varyColorsField;
    
    private CT_AreaSer[] serField;
    
    private CT_DLbls dLblsField;
    
    private CT_ChartLines dropLinesField;
    
    private CT_UnsignedInt[] axIdField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Grouping grouping {
        get {
            return this.groupingField;
        }
        set {
            this.groupingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean varyColors {
        get {
            return this.varyColorsField;
        }
        set {
            this.varyColorsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("ser")]
    public CT_AreaSer[] ser {
        get {
            return this.serField;
        }
        set {
            this.serField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbls dLbls {
        get {
            return this.dLblsField;
        }
        set {
            this.dLblsField = value;
        }
    }
    
    /// <remarks/>
    public CT_ChartLines dropLines {
        get {
            return this.dropLinesField;
        }
        set {
            this.dropLinesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("axId")]
    public CT_UnsignedInt[] axId {
        get {
            return this.axIdField;
        }
        set {
            this.axIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PlotArea {
    
    private CT_Layout layoutField;
    
    private object[] itemsField;
    
    private object[] items1Field;
    
    private CT_DTable dTableField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Layout layout {
        get {
            return this.layoutField;
        }
        set {
            this.layoutField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("area3DChart", typeof(CT_Area3DChart))]
    [System.Xml.Serialization.XmlElementAttribute("areaChart", typeof(CT_AreaChart))]
    [System.Xml.Serialization.XmlElementAttribute("bar3DChart", typeof(CT_Bar3DChart))]
    [System.Xml.Serialization.XmlElementAttribute("barChart", typeof(CT_BarChart))]
    [System.Xml.Serialization.XmlElementAttribute("bubbleChart", typeof(CT_BubbleChart))]
    [System.Xml.Serialization.XmlElementAttribute("doughnutChart", typeof(CT_DoughnutChart))]
    [System.Xml.Serialization.XmlElementAttribute("line3DChart", typeof(CT_Line3DChart))]
    [System.Xml.Serialization.XmlElementAttribute("lineChart", typeof(CT_LineChart))]
    [System.Xml.Serialization.XmlElementAttribute("ofPieChart", typeof(CT_OfPieChart))]
    [System.Xml.Serialization.XmlElementAttribute("pie3DChart", typeof(CT_Pie3DChart))]
    [System.Xml.Serialization.XmlElementAttribute("pieChart", typeof(CT_PieChart))]
    [System.Xml.Serialization.XmlElementAttribute("radarChart", typeof(CT_RadarChart))]
    [System.Xml.Serialization.XmlElementAttribute("scatterChart", typeof(CT_ScatterChart))]
    [System.Xml.Serialization.XmlElementAttribute("stockChart", typeof(CT_StockChart))]
    [System.Xml.Serialization.XmlElementAttribute("surface3DChart", typeof(CT_Surface3DChart))]
    [System.Xml.Serialization.XmlElementAttribute("surfaceChart", typeof(CT_SurfaceChart))]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("catAx", typeof(CT_CatAx))]
    [System.Xml.Serialization.XmlElementAttribute("dateAx", typeof(CT_DateAx))]
    [System.Xml.Serialization.XmlElementAttribute("serAx", typeof(CT_SerAx))]
    [System.Xml.Serialization.XmlElementAttribute("valAx", typeof(CT_ValAx))]
    public object[] Items1 {
        get {
            return this.items1Field;
        }
        set {
            this.items1Field = value;
        }
    }
    
    /// <remarks/>
    public CT_DTable dTable {
        get {
            return this.dTableField;
        }
        set {
            this.dTableField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Thickness {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Surface {
    
    private CT_Thickness thicknessField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_PictureOptions pictureOptionsField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Thickness thickness {
        get {
            return this.thicknessField;
        }
        set {
            this.thicknessField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PictureOptions pictureOptions {
        get {
            return this.pictureOptionsField;
        }
        set {
            this.pictureOptionsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Perspective {
    
    private byte valField;
    
    public CT_Perspective() {
        this.valField = ((byte)(30));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(byte), "30")]
    public byte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_DepthPercent {
    
    private string valField;
    
    public CT_DepthPercent() {
        this.valField = "100%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_RotY {
    
    private ushort valField;
    
    public CT_RotY() {
        this.valField = ((ushort)(0));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(ushort), "0")]
    public ushort val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_HPercent {
    
    private string valField;
    
    public CT_HPercent() {
        this.valField = "100%";
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("100%")]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_RotX {
    
    private sbyte valField;
    
    public CT_RotX() {
        this.valField = ((sbyte)(0));
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(typeof(sbyte), "0")]
    public sbyte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_View3D {
    
    private CT_RotX rotXField;
    
    private CT_HPercent hPercentField;
    
    private CT_RotY rotYField;
    
    private CT_DepthPercent depthPercentField;
    
    private CT_Boolean rAngAxField;
    
    private CT_Perspective perspectiveField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_RotX rotX {
        get {
            return this.rotXField;
        }
        set {
            this.rotXField = value;
        }
    }
    
    /// <remarks/>
    public CT_HPercent hPercent {
        get {
            return this.hPercentField;
        }
        set {
            this.hPercentField = value;
        }
    }
    
    /// <remarks/>
    public CT_RotY rotY {
        get {
            return this.rotYField;
        }
        set {
            this.rotYField = value;
        }
    }
    
    /// <remarks/>
    public CT_DepthPercent depthPercent {
        get {
            return this.depthPercentField;
        }
        set {
            this.depthPercentField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean rAngAx {
        get {
            return this.rAngAxField;
        }
        set {
            this.rAngAxField = value;
        }
    }
    
    /// <remarks/>
    public CT_Perspective perspective {
        get {
            return this.perspectiveField;
        }
        set {
            this.perspectiveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PivotFmt {
    
    private CT_UnsignedInt idxField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_TextBody txPrField;
    
    private CT_Marker markerField;
    
    private CT_DLbl dLblField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_UnsignedInt idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txPr {
        get {
            return this.txPrField;
        }
        set {
            this.txPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker marker {
        get {
            return this.markerField;
        }
        set {
            this.markerField = value;
        }
    }
    
    /// <remarks/>
    public CT_DLbl dLbl {
        get {
            return this.dLblField;
        }
        set {
            this.dLblField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Chart {
    
    private CT_Title titleField;
    
    private CT_Boolean autoTitleDeletedField;
    
    private CT_PivotFmt[] pivotFmtsField;
    
    private CT_View3D view3DField;
    
    private CT_Surface floorField;
    
    private CT_Surface sideWallField;
    
    private CT_Surface backWallField;
    
    private CT_PlotArea plotAreaField;
    
    private CT_Legend legendField;
    
    private CT_Boolean plotVisOnlyField;
    
    private CT_DispBlanksAs dispBlanksAsField;
    
    private CT_Boolean showDLblsOverMaxField;
    
    private CT_Extension[] extLstField;
    
    /// <remarks/>
    public CT_Title title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean autoTitleDeleted {
        get {
            return this.autoTitleDeletedField;
        }
        set {
            this.autoTitleDeletedField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("pivotFmt", IsNullable=false)]
    public CT_PivotFmt[] pivotFmts {
        get {
            return this.pivotFmtsField;
        }
        set {
            this.pivotFmtsField = value;
        }
    }
    
    /// <remarks/>
    public CT_View3D view3D {
        get {
            return this.view3DField;
        }
        set {
            this.view3DField = value;
        }
    }
    
    /// <remarks/>
    public CT_Surface floor {
        get {
            return this.floorField;
        }
        set {
            this.floorField = value;
        }
    }
    
    /// <remarks/>
    public CT_Surface sideWall {
        get {
            return this.sideWallField;
        }
        set {
            this.sideWallField = value;
        }
    }
    
    /// <remarks/>
    public CT_Surface backWall {
        get {
            return this.backWallField;
        }
        set {
            this.backWallField = value;
        }
    }
    
    /// <remarks/>
    public CT_PlotArea plotArea {
        get {
            return this.plotAreaField;
        }
        set {
            this.plotAreaField = value;
        }
    }
    
    /// <remarks/>
    public CT_Legend legend {
        get {
            return this.legendField;
        }
        set {
            this.legendField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean plotVisOnly {
        get {
            return this.plotVisOnlyField;
        }
        set {
            this.plotVisOnlyField = value;
        }
    }
    
    /// <remarks/>
    public CT_DispBlanksAs dispBlanksAs {
        get {
            return this.dispBlanksAsField;
        }
        set {
            this.dispBlanksAsField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean showDLblsOverMax {
        get {
            return this.showDLblsOverMaxField;
        }
        set {
            this.showDLblsOverMaxField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", IsNullable=false)]
    public CT_Extension[] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Protection {
    
    private CT_Boolean chartObjectField;
    
    private CT_Boolean dataField;
    
    private CT_Boolean formattingField;
    
    private CT_Boolean selectionField;
    
    private CT_Boolean userInterfaceField;
    
    /// <remarks/>
    public CT_Boolean chartObject {
        get {
            return this.chartObjectField;
        }
        set {
            this.chartObjectField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean data {
        get {
            return this.dataField;
        }
        set {
            this.dataField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean formatting {
        get {
            return this.formattingField;
        }
        set {
            this.formattingField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean selection {
        get {
            return this.selectionField;
        }
        set {
            this.selectionField = value;
        }
    }
    
    /// <remarks/>
    public CT_Boolean userInterface {
        get {
            return this.userInterfaceField;
        }
        set {
            this.userInterfaceField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_PivotSource {
    
    private string nameField;
    
    private CT_UnsignedInt fmtIdField;
    
    private CT_Extension[][] extLstField;
    
    /// <remarks/>
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
    
    /// <remarks/>
    public CT_UnsignedInt fmtId {
        get {
            return this.fmtIdField;
        }
        set {
            this.fmtIdField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlArrayItemAttribute("ext", typeof(CT_Extension), IsNullable=false)]
    public CT_Extension[][] extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ColorMapping {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private ST_ColorSchemeIndex bg1Field;
    
    private ST_ColorSchemeIndex tx1Field;
    
    private ST_ColorSchemeIndex bg2Field;
    
    private ST_ColorSchemeIndex tx2Field;
    
    private ST_ColorSchemeIndex accent1Field;
    
    private ST_ColorSchemeIndex accent2Field;
    
    private ST_ColorSchemeIndex accent3Field;
    
    private ST_ColorSchemeIndex accent4Field;
    
    private ST_ColorSchemeIndex accent5Field;
    
    private ST_ColorSchemeIndex accent6Field;
    
    private ST_ColorSchemeIndex hlinkField;
    
    private ST_ColorSchemeIndex folHlinkField;
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex bg1 {
        get {
            return this.bg1Field;
        }
        set {
            this.bg1Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex tx1 {
        get {
            return this.tx1Field;
        }
        set {
            this.tx1Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex bg2 {
        get {
            return this.bg2Field;
        }
        set {
            this.bg2Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex tx2 {
        get {
            return this.tx2Field;
        }
        set {
            this.tx2Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent1 {
        get {
            return this.accent1Field;
        }
        set {
            this.accent1Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent2 {
        get {
            return this.accent2Field;
        }
        set {
            this.accent2Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent3 {
        get {
            return this.accent3Field;
        }
        set {
            this.accent3Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent4 {
        get {
            return this.accent4Field;
        }
        set {
            this.accent4Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent5 {
        get {
            return this.accent5Field;
        }
        set {
            this.accent5Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex accent6 {
        get {
            return this.accent6Field;
        }
        set {
            this.accent6Field = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex hlink {
        get {
            return this.hlinkField;
        }
        set {
            this.hlinkField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_ColorSchemeIndex folHlink {
        get {
            return this.folHlinkField;
        }
        set {
            this.folHlinkField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_ColorSchemeIndex {
    
    /// <remarks/>
    dk1,
    
    /// <remarks/>
    lt1,
    
    /// <remarks/>
    dk2,
    
    /// <remarks/>
    lt2,
    
    /// <remarks/>
    accent1,
    
    /// <remarks/>
    accent2,
    
    /// <remarks/>
    accent3,
    
    /// <remarks/>
    accent4,
    
    /// <remarks/>
    accent5,
    
    /// <remarks/>
    accent6,
    
    /// <remarks/>
    hlink,
    
    /// <remarks/>
    folHlink,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(TypeName="CT_Style", Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_Style1 {
    
    private byte valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public byte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://schemas.microsoft.com/office/drawing/2007/8/2/chart")]
public partial class CT_Style {
    
    private byte valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public byte val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chart")]
public partial class CT_TextLanguageID {
    
    private string valField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string val {
        get {
            return this.valField;
        }
        set {
            this.valField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://schemas.openxmlformats.org/markup-compatibility/2006")]
public partial class AlternateContent {
    
    private AlternateContentChoice[] choiceField;
    
    private AlternateContentFallback fallbackField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("Choice")]
    public AlternateContentChoice[] Choice {
        get {
            return this.choiceField;
        }
        set {
            this.choiceField = value;
        }
    }
    
    /// <remarks/>
    public AlternateContentFallback Fallback {
        get {
            return this.fallbackField;
        }
        set {
            this.fallbackField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://schemas.openxmlformats.org/markup-compatibility/2006")]
public partial class AlternateContentChoice {
    
    private CT_Style styleField;
    
    private string requiresField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute(Namespace="http://schemas.microsoft.com/office/drawing/2007/8/2/chart")]
    public CT_Style style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string Requires {
        get {
            return this.requiresField;
        }
        set {
            this.requiresField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(AnonymousType=true, Namespace="http://schemas.openxmlformats.org/markup-compatibility/2006")]
public partial class AlternateContentFallback {
    
    private CT_Style1 styleField;
    
    /// <remarks/>
    public CT_Style1 style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
[System.Xml.Serialization.XmlRootAttribute("userShapes", Namespace="http://purl.oclc.org/ooxml/drawingml/chart", IsNullable=false)]
public partial class CT_Drawing {
    
    private CT_RelSizeAnchor[] relSizeAnchorField;
    
    private CT_AbsSizeAnchor[] absSizeAnchorField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("relSizeAnchor")]
    public CT_RelSizeAnchor[] relSizeAnchor {
        get {
            return this.relSizeAnchorField;
        }
        set {
            this.relSizeAnchorField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("absSizeAnchor")]
    public CT_AbsSizeAnchor[] absSizeAnchor {
        get {
            return this.absSizeAnchorField;
        }
        set {
            this.absSizeAnchorField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_RelSizeAnchor {
    
    private CT_Marker1 fromField;
    
    private CT_Marker1 toField;
    
    private object itemField;
    
    /// <remarks/>
    public CT_Marker1 from {
        get {
            return this.fromField;
        }
        set {
            this.fromField = value;
        }
    }
    
    /// <remarks/>
    public CT_Marker1 to {
        get {
            return this.toField;
        }
        set {
            this.toField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("cxnSp", typeof(CT_Connector))]
    [System.Xml.Serialization.XmlElementAttribute("graphicFrame", typeof(CT_GraphicFrame))]
    [System.Xml.Serialization.XmlElementAttribute("grpSp", typeof(CT_GroupShape))]
    [System.Xml.Serialization.XmlElementAttribute("pic", typeof(CT_Picture))]
    [System.Xml.Serialization.XmlElementAttribute("sp", typeof(CT_Shape1))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(TypeName="CT_Marker", Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_Marker1 {
    
    private double xField;
    
    private double yField;
    
    /// <remarks/>
    public double x {
        get {
            return this.xField;
        }
        set {
            this.xField = value;
        }
    }
    
    /// <remarks/>
    public double y {
        get {
            return this.yField;
        }
        set {
            this.yField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_Connector {
    
    private CT_ConnectorNonVisual nvCxnSpPrField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_ShapeStyle styleField;
    
    private string macroField;
    
    private bool fPublishedField;
    
    public CT_Connector() {
        this.fPublishedField = false;
    }
    
    /// <remarks/>
    public CT_ConnectorNonVisual nvCxnSpPr {
        get {
            return this.nvCxnSpPrField;
        }
        set {
            this.nvCxnSpPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeStyle style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string macro {
        get {
            return this.macroField;
        }
        set {
            this.macroField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool fPublished {
        get {
            return this.fPublishedField;
        }
        set {
            this.fPublishedField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_ConnectorNonVisual {
    
    private CT_NonVisualDrawingProps cNvPrField;
    
    private CT_NonVisualConnectorProperties cNvCxnSpPrField;
    
    /// <remarks/>
    public CT_NonVisualDrawingProps cNvPr {
        get {
            return this.cNvPrField;
        }
        set {
            this.cNvPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_NonVisualConnectorProperties cNvCxnSpPr {
        get {
            return this.cNvCxnSpPrField;
        }
        set {
            this.cNvCxnSpPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualDrawingProps {
    
    private CT_Hyperlink hlinkClickField;
    
    private CT_Hyperlink hlinkHoverField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private uint idField;
    
    private string nameField;
    
    private string descrField;
    
    private bool hiddenField;
    
    private string titleField;
    
    public CT_NonVisualDrawingProps() {
        this.descrField = "";
        this.hiddenField = false;
        this.titleField = "";
    }
    
    /// <remarks/>
    public CT_Hyperlink hlinkClick {
        get {
            return this.hlinkClickField;
        }
        set {
            this.hlinkClickField = value;
        }
    }
    
    /// <remarks/>
    public CT_Hyperlink hlinkHover {
        get {
            return this.hlinkHoverField;
        }
        set {
            this.hlinkHoverField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string name {
        get {
            return this.nameField;
        }
        set {
            this.nameField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string descr {
        get {
            return this.descrField;
        }
        set {
            this.descrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool hidden {
        get {
            return this.hiddenField;
        }
        set {
            this.hiddenField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string title {
        get {
            return this.titleField;
        }
        set {
            this.titleField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualConnectorProperties {
    
    private CT_ConnectorLocking cxnSpLocksField;
    
    private CT_Connection stCxnField;
    
    private CT_Connection endCxnField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_ConnectorLocking cxnSpLocks {
        get {
            return this.cxnSpLocksField;
        }
        set {
            this.cxnSpLocksField = value;
        }
    }
    
    /// <remarks/>
    public CT_Connection stCxn {
        get {
            return this.stCxnField;
        }
        set {
            this.stCxnField = value;
        }
    }
    
    /// <remarks/>
    public CT_Connection endCxn {
        get {
            return this.endCxnField;
        }
        set {
            this.endCxnField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ConnectorLocking {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool noGrpField;
    
    private bool noSelectField;
    
    private bool noRotField;
    
    private bool noChangeAspectField;
    
    private bool noMoveField;
    
    private bool noResizeField;
    
    private bool noEditPointsField;
    
    private bool noAdjustHandlesField;
    
    private bool noChangeArrowheadsField;
    
    private bool noChangeShapeTypeField;
    
    public CT_ConnectorLocking() {
        this.noGrpField = false;
        this.noSelectField = false;
        this.noRotField = false;
        this.noChangeAspectField = false;
        this.noMoveField = false;
        this.noResizeField = false;
        this.noEditPointsField = false;
        this.noAdjustHandlesField = false;
        this.noChangeArrowheadsField = false;
        this.noChangeShapeTypeField = false;
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noGrp {
        get {
            return this.noGrpField;
        }
        set {
            this.noGrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noSelect {
        get {
            return this.noSelectField;
        }
        set {
            this.noSelectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noRot {
        get {
            return this.noRotField;
        }
        set {
            this.noRotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeAspect {
        get {
            return this.noChangeAspectField;
        }
        set {
            this.noChangeAspectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noMove {
        get {
            return this.noMoveField;
        }
        set {
            this.noMoveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noResize {
        get {
            return this.noResizeField;
        }
        set {
            this.noResizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noEditPoints {
        get {
            return this.noEditPointsField;
        }
        set {
            this.noEditPointsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noAdjustHandles {
        get {
            return this.noAdjustHandlesField;
        }
        set {
            this.noAdjustHandlesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeArrowheads {
        get {
            return this.noChangeArrowheadsField;
        }
        set {
            this.noChangeArrowheadsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeShapeType {
        get {
            return this.noChangeShapeTypeField;
        }
        set {
            this.noChangeShapeTypeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_Connection {
    
    private uint idField;
    
    private uint idxField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint id {
        get {
            return this.idField;
        }
        set {
            this.idField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ShapeStyle {
    
    private CT_StyleMatrixReference lnRefField;
    
    private CT_StyleMatrixReference fillRefField;
    
    private CT_StyleMatrixReference effectRefField;
    
    private CT_FontReference fontRefField;
    
    /// <remarks/>
    public CT_StyleMatrixReference lnRef {
        get {
            return this.lnRefField;
        }
        set {
            this.lnRefField = value;
        }
    }
    
    /// <remarks/>
    public CT_StyleMatrixReference fillRef {
        get {
            return this.fillRefField;
        }
        set {
            this.fillRefField = value;
        }
    }
    
    /// <remarks/>
    public CT_StyleMatrixReference effectRef {
        get {
            return this.effectRefField;
        }
        set {
            this.effectRefField = value;
        }
    }
    
    /// <remarks/>
    public CT_FontReference fontRef {
        get {
            return this.fontRefField;
        }
        set {
            this.fontRefField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_StyleMatrixReference {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private uint idxField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public uint idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_FontReference {
    
    private CT_ScRgbColor scrgbClrField;
    
    private CT_SRgbColor srgbClrField;
    
    private CT_HslColor hslClrField;
    
    private CT_SystemColor sysClrField;
    
    private CT_SchemeColor schemeClrField;
    
    private CT_PresetColor prstClrField;
    
    private ST_FontCollectionIndex idxField;
    
    /// <remarks/>
    public CT_ScRgbColor scrgbClr {
        get {
            return this.scrgbClrField;
        }
        set {
            this.scrgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SRgbColor srgbClr {
        get {
            return this.srgbClrField;
        }
        set {
            this.srgbClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_HslColor hslClr {
        get {
            return this.hslClrField;
        }
        set {
            this.hslClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SystemColor sysClr {
        get {
            return this.sysClrField;
        }
        set {
            this.sysClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_SchemeColor schemeClr {
        get {
            return this.schemeClrField;
        }
        set {
            this.schemeClrField = value;
        }
    }
    
    /// <remarks/>
    public CT_PresetColor prstClr {
        get {
            return this.prstClrField;
        }
        set {
            this.prstClrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_FontCollectionIndex idx {
        get {
            return this.idxField;
        }
        set {
            this.idxField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public enum ST_FontCollectionIndex {
    
    /// <remarks/>
    major,
    
    /// <remarks/>
    minor,
    
    /// <remarks/>
    none,
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_GraphicFrame {
    
    private CT_GraphicFrameNonVisual nvGraphicFramePrField;
    
    private CT_Transform2D xfrmField;
    
    private CT_GraphicalObject graphicField;
    
    private string macroField;
    
    private bool fPublishedField;
    
    public CT_GraphicFrame() {
        this.fPublishedField = false;
    }
    
    /// <remarks/>
    public CT_GraphicFrameNonVisual nvGraphicFramePr {
        get {
            return this.nvGraphicFramePrField;
        }
        set {
            this.nvGraphicFramePrField = value;
        }
    }
    
    /// <remarks/>
    public CT_Transform2D xfrm {
        get {
            return this.xfrmField;
        }
        set {
            this.xfrmField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
    public CT_GraphicalObject graphic {
        get {
            return this.graphicField;
        }
        set {
            this.graphicField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string macro {
        get {
            return this.macroField;
        }
        set {
            this.macroField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool fPublished {
        get {
            return this.fPublishedField;
        }
        set {
            this.fPublishedField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_GraphicFrameNonVisual {
    
    private CT_NonVisualDrawingProps cNvPrField;
    
    private CT_NonVisualGraphicFrameProperties cNvGraphicFramePrField;
    
    /// <remarks/>
    public CT_NonVisualDrawingProps cNvPr {
        get {
            return this.cNvPrField;
        }
        set {
            this.cNvPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_NonVisualGraphicFrameProperties cNvGraphicFramePr {
        get {
            return this.cNvGraphicFramePrField;
        }
        set {
            this.cNvGraphicFramePrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualGraphicFrameProperties {
    
    private CT_GraphicalObjectFrameLocking graphicFrameLocksField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_GraphicalObjectFrameLocking graphicFrameLocks {
        get {
            return this.graphicFrameLocksField;
        }
        set {
            this.graphicFrameLocksField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GraphicalObjectFrameLocking {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool noGrpField;
    
    private bool noDrilldownField;
    
    private bool noSelectField;
    
    private bool noChangeAspectField;
    
    private bool noMoveField;
    
    private bool noResizeField;
    
    public CT_GraphicalObjectFrameLocking() {
        this.noGrpField = false;
        this.noDrilldownField = false;
        this.noSelectField = false;
        this.noChangeAspectField = false;
        this.noMoveField = false;
        this.noResizeField = false;
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noGrp {
        get {
            return this.noGrpField;
        }
        set {
            this.noGrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noDrilldown {
        get {
            return this.noDrilldownField;
        }
        set {
            this.noDrilldownField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noSelect {
        get {
            return this.noSelectField;
        }
        set {
            this.noSelectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeAspect {
        get {
            return this.noChangeAspectField;
        }
        set {
            this.noChangeAspectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noMove {
        get {
            return this.noMoveField;
        }
        set {
            this.noMoveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noResize {
        get {
            return this.noResizeField;
        }
        set {
            this.noResizeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GraphicalObject {
    
    private CT_GraphicalObjectData graphicDataField;
    
    /// <remarks/>
    public CT_GraphicalObjectData graphicData {
        get {
            return this.graphicDataField;
        }
        set {
            this.graphicDataField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GraphicalObjectData {
    
    private System.Xml.XmlElement[] anyField;
    
    private string uriField;
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAnyElementAttribute()]
    public System.Xml.XmlElement[] Any {
        get {
            return this.anyField;
        }
        set {
            this.anyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute(DataType="token")]
    public string uri {
        get {
            return this.uriField;
        }
        set {
            this.uriField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_GroupShape {
    
    private CT_GroupShapeNonVisual nvGrpSpPrField;
    
    private CT_GroupShapeProperties grpSpPrField;
    
    private object[] itemsField;
    
    /// <remarks/>
    public CT_GroupShapeNonVisual nvGrpSpPr {
        get {
            return this.nvGrpSpPrField;
        }
        set {
            this.nvGrpSpPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupShapeProperties grpSpPr {
        get {
            return this.grpSpPrField;
        }
        set {
            this.grpSpPrField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("cxnSp", typeof(CT_Connector))]
    [System.Xml.Serialization.XmlElementAttribute("graphicFrame", typeof(CT_GraphicFrame))]
    [System.Xml.Serialization.XmlElementAttribute("grpSp", typeof(CT_GroupShape))]
    [System.Xml.Serialization.XmlElementAttribute("pic", typeof(CT_Picture))]
    [System.Xml.Serialization.XmlElementAttribute("sp", typeof(CT_Shape1))]
    public object[] Items {
        get {
            return this.itemsField;
        }
        set {
            this.itemsField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_GroupShapeNonVisual {
    
    private CT_NonVisualDrawingProps cNvPrField;
    
    private CT_NonVisualGroupDrawingShapeProps cNvGrpSpPrField;
    
    /// <remarks/>
    public CT_NonVisualDrawingProps cNvPr {
        get {
            return this.cNvPrField;
        }
        set {
            this.cNvPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_NonVisualGroupDrawingShapeProps cNvGrpSpPr {
        get {
            return this.cNvGrpSpPrField;
        }
        set {
            this.cNvGrpSpPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualGroupDrawingShapeProps {
    
    private CT_GroupLocking grpSpLocksField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    /// <remarks/>
    public CT_GroupLocking grpSpLocks {
        get {
            return this.grpSpLocksField;
        }
        set {
            this.grpSpLocksField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GroupLocking {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool noGrpField;
    
    private bool noUngrpField;
    
    private bool noSelectField;
    
    private bool noRotField;
    
    private bool noChangeAspectField;
    
    private bool noMoveField;
    
    private bool noResizeField;
    
    public CT_GroupLocking() {
        this.noGrpField = false;
        this.noUngrpField = false;
        this.noSelectField = false;
        this.noRotField = false;
        this.noChangeAspectField = false;
        this.noMoveField = false;
        this.noResizeField = false;
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noGrp {
        get {
            return this.noGrpField;
        }
        set {
            this.noGrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noUngrp {
        get {
            return this.noUngrpField;
        }
        set {
            this.noUngrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noSelect {
        get {
            return this.noSelectField;
        }
        set {
            this.noSelectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noRot {
        get {
            return this.noRotField;
        }
        set {
            this.noRotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeAspect {
        get {
            return this.noChangeAspectField;
        }
        set {
            this.noChangeAspectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noMove {
        get {
            return this.noMoveField;
        }
        set {
            this.noMoveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noResize {
        get {
            return this.noResizeField;
        }
        set {
            this.noResizeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GroupShapeProperties {
    
    private CT_GroupTransform2D xfrmField;
    
    private CT_NoFillProperties noFillField;
    
    private CT_SolidColorFillProperties solidFillField;
    
    private CT_GradientFillProperties gradFillField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_PatternFillProperties pattFillField;
    
    private CT_GroupFillProperties grpFillField;
    
    private CT_EffectList effectLstField;
    
    private CT_EffectContainer effectDagField;
    
    private CT_Scene3D scene3dField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private ST_BlackWhiteMode bwModeField;
    
    private bool bwModeFieldSpecified;
    
    /// <remarks/>
    public CT_GroupTransform2D xfrm {
        get {
            return this.xfrmField;
        }
        set {
            this.xfrmField = value;
        }
    }
    
    /// <remarks/>
    public CT_NoFillProperties noFill {
        get {
            return this.noFillField;
        }
        set {
            this.noFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_SolidColorFillProperties solidFill {
        get {
            return this.solidFillField;
        }
        set {
            this.solidFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GradientFillProperties gradFill {
        get {
            return this.gradFillField;
        }
        set {
            this.gradFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_PatternFillProperties pattFill {
        get {
            return this.pattFillField;
        }
        set {
            this.pattFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_GroupFillProperties grpFill {
        get {
            return this.grpFillField;
        }
        set {
            this.grpFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectList effectLst {
        get {
            return this.effectLstField;
        }
        set {
            this.effectLstField = value;
        }
    }
    
    /// <remarks/>
    public CT_EffectContainer effectDag {
        get {
            return this.effectDagField;
        }
        set {
            this.effectDagField = value;
        }
    }
    
    /// <remarks/>
    public CT_Scene3D scene3d {
        get {
            return this.scene3dField;
        }
        set {
            this.scene3dField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public ST_BlackWhiteMode bwMode {
        get {
            return this.bwModeField;
        }
        set {
            this.bwModeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlIgnoreAttribute()]
    public bool bwModeSpecified {
        get {
            return this.bwModeFieldSpecified;
        }
        set {
            this.bwModeFieldSpecified = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_GroupTransform2D {
    
    private CT_Point2D offField;
    
    private CT_PositiveSize2D extField;
    
    private CT_Point2D chOffField;
    
    private CT_PositiveSize2D chExtField;
    
    private int rotField;
    
    private bool flipHField;
    
    private bool flipVField;
    
    public CT_GroupTransform2D() {
        this.rotField = 0;
        this.flipHField = false;
        this.flipVField = false;
    }
    
    /// <remarks/>
    public CT_Point2D off {
        get {
            return this.offField;
        }
        set {
            this.offField = value;
        }
    }
    
    /// <remarks/>
    public CT_PositiveSize2D ext {
        get {
            return this.extField;
        }
        set {
            this.extField = value;
        }
    }
    
    /// <remarks/>
    public CT_Point2D chOff {
        get {
            return this.chOffField;
        }
        set {
            this.chOffField = value;
        }
    }
    
    /// <remarks/>
    public CT_PositiveSize2D chExt {
        get {
            return this.chExtField;
        }
        set {
            this.chExtField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(0)]
    public int rot {
        get {
            return this.rotField;
        }
        set {
            this.rotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool flipH {
        get {
            return this.flipHField;
        }
        set {
            this.flipHField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool flipV {
        get {
            return this.flipVField;
        }
        set {
            this.flipVField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_Picture {
    
    private CT_PictureNonVisual nvPicPrField;
    
    private CT_BlipFillProperties blipFillField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_ShapeStyle styleField;
    
    private string macroField;
    
    private bool fPublishedField;
    
    public CT_Picture() {
        this.macroField = "";
        this.fPublishedField = false;
    }
    
    /// <remarks/>
    public CT_PictureNonVisual nvPicPr {
        get {
            return this.nvPicPrField;
        }
        set {
            this.nvPicPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_BlipFillProperties blipFill {
        get {
            return this.blipFillField;
        }
        set {
            this.blipFillField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeStyle style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute("")]
    public string macro {
        get {
            return this.macroField;
        }
        set {
            this.macroField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool fPublished {
        get {
            return this.fPublishedField;
        }
        set {
            this.fPublishedField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_PictureNonVisual {
    
    private CT_NonVisualDrawingProps cNvPrField;
    
    private CT_NonVisualPictureProperties cNvPicPrField;
    
    /// <remarks/>
    public CT_NonVisualDrawingProps cNvPr {
        get {
            return this.cNvPrField;
        }
        set {
            this.cNvPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_NonVisualPictureProperties cNvPicPr {
        get {
            return this.cNvPicPrField;
        }
        set {
            this.cNvPicPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualPictureProperties {
    
    private CT_PictureLocking picLocksField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool preferRelativeResizeField;
    
    public CT_NonVisualPictureProperties() {
        this.preferRelativeResizeField = true;
    }
    
    /// <remarks/>
    public CT_PictureLocking picLocks {
        get {
            return this.picLocksField;
        }
        set {
            this.picLocksField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool preferRelativeResize {
        get {
            return this.preferRelativeResizeField;
        }
        set {
            this.preferRelativeResizeField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_PictureLocking {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool noGrpField;
    
    private bool noSelectField;
    
    private bool noRotField;
    
    private bool noChangeAspectField;
    
    private bool noMoveField;
    
    private bool noResizeField;
    
    private bool noEditPointsField;
    
    private bool noAdjustHandlesField;
    
    private bool noChangeArrowheadsField;
    
    private bool noChangeShapeTypeField;
    
    private bool noCropField;
    
    public CT_PictureLocking() {
        this.noGrpField = false;
        this.noSelectField = false;
        this.noRotField = false;
        this.noChangeAspectField = false;
        this.noMoveField = false;
        this.noResizeField = false;
        this.noEditPointsField = false;
        this.noAdjustHandlesField = false;
        this.noChangeArrowheadsField = false;
        this.noChangeShapeTypeField = false;
        this.noCropField = false;
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noGrp {
        get {
            return this.noGrpField;
        }
        set {
            this.noGrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noSelect {
        get {
            return this.noSelectField;
        }
        set {
            this.noSelectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noRot {
        get {
            return this.noRotField;
        }
        set {
            this.noRotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeAspect {
        get {
            return this.noChangeAspectField;
        }
        set {
            this.noChangeAspectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noMove {
        get {
            return this.noMoveField;
        }
        set {
            this.noMoveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noResize {
        get {
            return this.noResizeField;
        }
        set {
            this.noResizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noEditPoints {
        get {
            return this.noEditPointsField;
        }
        set {
            this.noEditPointsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noAdjustHandles {
        get {
            return this.noAdjustHandlesField;
        }
        set {
            this.noAdjustHandlesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeArrowheads {
        get {
            return this.noChangeArrowheadsField;
        }
        set {
            this.noChangeArrowheadsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeShapeType {
        get {
            return this.noChangeShapeTypeField;
        }
        set {
            this.noChangeShapeTypeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noCrop {
        get {
            return this.noCropField;
        }
        set {
            this.noCropField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(TypeName="CT_Shape", Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_Shape1 {
    
    private CT_ShapeNonVisual nvSpPrField;
    
    private CT_ShapeProperties spPrField;
    
    private CT_ShapeStyle styleField;
    
    private CT_TextBody txBodyField;
    
    private string macroField;
    
    private string textlinkField;
    
    private bool fLocksTextField;
    
    private bool fPublishedField;
    
    public CT_Shape1() {
        this.fLocksTextField = true;
        this.fPublishedField = false;
    }
    
    /// <remarks/>
    public CT_ShapeNonVisual nvSpPr {
        get {
            return this.nvSpPrField;
        }
        set {
            this.nvSpPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeProperties spPr {
        get {
            return this.spPrField;
        }
        set {
            this.spPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_ShapeStyle style {
        get {
            return this.styleField;
        }
        set {
            this.styleField = value;
        }
    }
    
    /// <remarks/>
    public CT_TextBody txBody {
        get {
            return this.txBodyField;
        }
        set {
            this.txBodyField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string macro {
        get {
            return this.macroField;
        }
        set {
            this.macroField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    public string textlink {
        get {
            return this.textlinkField;
        }
        set {
            this.textlinkField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(true)]
    public bool fLocksText {
        get {
            return this.fLocksTextField;
        }
        set {
            this.fLocksTextField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool fPublished {
        get {
            return this.fPublishedField;
        }
        set {
            this.fPublishedField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_ShapeNonVisual {
    
    private CT_NonVisualDrawingProps cNvPrField;
    
    private CT_NonVisualDrawingShapeProps cNvSpPrField;
    
    /// <remarks/>
    public CT_NonVisualDrawingProps cNvPr {
        get {
            return this.cNvPrField;
        }
        set {
            this.cNvPrField = value;
        }
    }
    
    /// <remarks/>
    public CT_NonVisualDrawingShapeProps cNvSpPr {
        get {
            return this.cNvSpPrField;
        }
        set {
            this.cNvSpPrField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_NonVisualDrawingShapeProps {
    
    private CT_ShapeLocking spLocksField;
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool txBoxField;
    
    public CT_NonVisualDrawingShapeProps() {
        this.txBoxField = false;
    }
    
    /// <remarks/>
    public CT_ShapeLocking spLocks {
        get {
            return this.spLocksField;
        }
        set {
            this.spLocksField = value;
        }
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool txBox {
        get {
            return this.txBoxField;
        }
        set {
            this.txBoxField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/main")]
public partial class CT_ShapeLocking {
    
    private CT_OfficeArtExtensionList extLstField;
    
    private bool noGrpField;
    
    private bool noSelectField;
    
    private bool noRotField;
    
    private bool noChangeAspectField;
    
    private bool noMoveField;
    
    private bool noResizeField;
    
    private bool noEditPointsField;
    
    private bool noAdjustHandlesField;
    
    private bool noChangeArrowheadsField;
    
    private bool noChangeShapeTypeField;
    
    private bool noTextEditField;
    
    public CT_ShapeLocking() {
        this.noGrpField = false;
        this.noSelectField = false;
        this.noRotField = false;
        this.noChangeAspectField = false;
        this.noMoveField = false;
        this.noResizeField = false;
        this.noEditPointsField = false;
        this.noAdjustHandlesField = false;
        this.noChangeArrowheadsField = false;
        this.noChangeShapeTypeField = false;
        this.noTextEditField = false;
    }
    
    /// <remarks/>
    public CT_OfficeArtExtensionList extLst {
        get {
            return this.extLstField;
        }
        set {
            this.extLstField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noGrp {
        get {
            return this.noGrpField;
        }
        set {
            this.noGrpField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noSelect {
        get {
            return this.noSelectField;
        }
        set {
            this.noSelectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noRot {
        get {
            return this.noRotField;
        }
        set {
            this.noRotField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeAspect {
        get {
            return this.noChangeAspectField;
        }
        set {
            this.noChangeAspectField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noMove {
        get {
            return this.noMoveField;
        }
        set {
            this.noMoveField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noResize {
        get {
            return this.noResizeField;
        }
        set {
            this.noResizeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noEditPoints {
        get {
            return this.noEditPointsField;
        }
        set {
            this.noEditPointsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noAdjustHandles {
        get {
            return this.noAdjustHandlesField;
        }
        set {
            this.noAdjustHandlesField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeArrowheads {
        get {
            return this.noChangeArrowheadsField;
        }
        set {
            this.noChangeArrowheadsField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noChangeShapeType {
        get {
            return this.noChangeShapeTypeField;
        }
        set {
            this.noChangeShapeTypeField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlAttributeAttribute()]
    [System.ComponentModel.DefaultValueAttribute(false)]
    public bool noTextEdit {
        get {
            return this.noTextEditField;
        }
        set {
            this.noTextEditField = value;
        }
    }
}

/// <remarks/>
[System.CodeDom.Compiler.GeneratedCodeAttribute("codepeg2", "1.0.0.0")]
[System.SerializableAttribute()]
[System.Diagnostics.DebuggerStepThroughAttribute()]
[System.ComponentModel.DesignerCategoryAttribute("code")]
[System.Xml.Serialization.XmlTypeAttribute(Namespace="http://purl.oclc.org/ooxml/drawingml/chartDrawing")]
public partial class CT_AbsSizeAnchor {
    
    private CT_Marker1 fromField;
    
    private CT_PositiveSize2D extField;
    
    private object itemField;
    
    /// <remarks/>
    public CT_Marker1 from {
        get {
            return this.fromField;
        }
        set {
            this.fromField = value;
        }
    }
    
    /// <remarks/>
    public CT_PositiveSize2D ext {
        get {
            return this.extField;
        }
        set {
            this.extField = value;
        }
    }
    
    /// <remarks/>
    [System.Xml.Serialization.XmlElementAttribute("cxnSp", typeof(CT_Connector))]
    [System.Xml.Serialization.XmlElementAttribute("graphicFrame", typeof(CT_GraphicFrame))]
    [System.Xml.Serialization.XmlElementAttribute("grpSp", typeof(CT_GroupShape))]
    [System.Xml.Serialization.XmlElementAttribute("pic", typeof(CT_Picture))]
    [System.Xml.Serialization.XmlElementAttribute("sp", typeof(CT_Shape1))]
    public object Item {
        get {
            return this.itemField;
        }
        set {
            this.itemField = value;
        }
    }
}
