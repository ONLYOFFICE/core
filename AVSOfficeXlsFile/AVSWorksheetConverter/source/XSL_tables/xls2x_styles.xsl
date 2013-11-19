<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">

  <xsl:template match="FORMATTING">
    <xlsx:file name="styles.xml">
      <styleSheet xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:call-template name="numFmts"/>
        <xsl:call-template name="fonts"/>
        <xsl:call-template name="fills"/>
        <xsl:call-template name="borders"/>
        <xsl:call-template name="cellStyleXfs"/>
        <xsl:call-template name="cellXfs"/>
        <xsl:call-template name="cellStyles"/>
        <xsl:call-template name="dxfs"/>
        <xsl:call-template name="tableStyles"/>
        <xsl:call-template name="colors"/>
      </styleSheet>
    </xlsx:file>
  </xsl:template>

  <xsl:template name="numFmts">
    <xsl:if test="Format">
      <numFmts count="{count(Format)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="Format">
          <xsl:call-template name="numFmt">
            <xsl:with-param name="numFmtId" select="@ifmt"/>
            <xsl:with-param name="formatCode" select="@stFormat"/>
          </xsl:call-template>
        </xsl:for-each>
      </numFmts>
    </xsl:if>
  </xsl:template>
  
  
  <xsl:template name="fonts">
    <xsl:if test="Font">
      <fonts count="{count(Font)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="Font">
          <xsl:call-template name="font">
            <xsl:with-param name="name" select="@fontName"/>
            <xsl:with-param name="charset" select="@bCharSet"/>
            <xsl:with-param name="family" select="@bFamily"/>
            <xsl:with-param name="b" select="@bls"/>
            <xsl:with-param name="i" select="@fItalic"/>
            <xsl:with-param name="strike" select="@fStrikeOut"/>
            <xsl:with-param name="outline" select="@fOutline"/>
            <xsl:with-param name="shadow" select="@fShadow"/>
            <xsl:with-param name="condense" select="@fCondense"/>
            <xsl:with-param name="extend" select="@fExtend"/>
            <xsl:with-param name="propColor" select="@icv"/>   <!-- Icv -->
            <xsl:with-param name="sz" select="@dyHeight"/>
            <xsl:with-param name="u" select="@uls"/>
            <xsl:with-param name="vertAlign" select="@sss"/>
            <xsl:with-param name="scheme"/>
          </xsl:call-template>
        </xsl:for-each>
      </fonts>
    </xsl:if>
  </xsl:template>


  <xsl:template name="fills">
    <fills xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <!-- We have to place the following 2 records mandatory. I don't know - why. Found out experimentally and only for Microsoft xlsxx client (Excel 2007) -->
      <xsl:call-template name="fill">
        <xsl:with-param name="patternType">0</xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="fill">
        <xsl:with-param name="patternType">17</xsl:with-param>
      </xsl:call-template>
      <xsl:if test="XFS">
        <xsl:for-each select="XFS/XF/*[generate-id(.) = generate-id(key('get_xf_by_fill_id', @fill_x_id))]">
          <xsl:if test="@fls != 0 and not(@fls = 17 and @icvFore = 64 and @icvBack = 65)">
            <xsl:variable name="xf_pos" select="position()"/>
            <xsl:variable name="gradient" select="../../XFExt[@ixfe = $xf_pos - 1]/ExtProp[@extType=6]/XFExtGradient"/> <!-- Nonempty if XFExt contains gradient information -->
            <xsl:call-template name="fill">
              <!-- Gradient info takes precedence of filltype so no condition necessary here -->
              <xsl:with-param name="gradient" select="$gradient/XFPropGradient"/>
              <xsl:with-param name="gradStops" select="$gradient/GradStop"/>
              <xsl:with-param name="patternType" select="@fls"/>
              <xsl:with-param name="fgColor" select="@icvFore"/>
              <xsl:with-param name="bgColor" select="@icvBack"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:for-each>
      </xsl:if>
    </fills>
  </xsl:template>

  <xsl:key name="get_xf_by_border_id" use="@border_x_id" match="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/*"/>
  <xsl:key name="get_xf_by_fill_id" use="@fill_x_id" match="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/XFS/XF/*"/>


  <xsl:template name="borders">
    <xsl:if test="XFS">
      <borders xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="XFS/XF/*[generate-id(.) = generate-id(key('get_xf_by_border_id', @border_x_id))]">
          <xsl:call-template name="border">
            <xsl:with-param name="diagonalDown">
              <xsl:if test="@grbitDiag = 1 or @grbitDiag = 3">true</xsl:if>
            </xsl:with-param>
            <xsl:with-param name="diagonalUp">
              <xsl:if test="@grbitDiag = 2 or @grbitDiag = 3">true</xsl:if>
            </xsl:with-param>
            <xsl:with-param name="dgLeft" select="@dgLeft"/>
            <xsl:with-param name="colorLeft" select="@icvLeft"/>
            <xsl:with-param name="dgRight" select="@dgRight"/>
            <xsl:with-param name="colorRight" select="@icvRight"/>
            <xsl:with-param name="dgTop" select="@dgTop"/>
            <xsl:with-param name="colorTop" select="@icvTop"/>
            <xsl:with-param name="dgBottom" select="@dgBottom"/>
            <xsl:with-param name="colorBottom" select="@icvBottom"/>
            <xsl:with-param name="dgDiag" select="@dgDiag"/>
            <xsl:with-param name="colorDiag" select="@icvDiag"/>
            <!-- 'vertical' and 'horizontal' are ommited here -->
            <xsl:with-param name="dgVert"/>
            <xsl:with-param name="colorVert"/>
            <xsl:with-param name="dgHoriz"/>
            <xsl:with-param name="colorHoriz"/>
          </xsl:call-template>
        </xsl:for-each>
      </borders>
    </xsl:if>
  </xsl:template>


  <xsl:template name="border_property">
    <xsl:param name="element_name"/>
    <xsl:param name="value"/>
    <xsl:param name="color"/>
    <xsl:if test="$value">
      <xsl:element name="{$element_name}" namespace="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$value != 0">
          <xsl:attribute name="style">
            <xsl:call-template name="BorderStyle_IDToString">
              <xsl:with-param name="id" select="$value"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:call-template name="color">
            <xsl:with-param name="tag_name">color</xsl:with-param>
            <xsl:with-param name="propColor" select="$color"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:element>
    </xsl:if>    
  </xsl:template>

  <xsl:template name="cellStyleXfs">
    <xsl:if test="XFS">
      <cellStyleXfs count="{count(XFS/XF/StyleXF)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:apply-templates select="XFS/XF/StyleXF"/>
      </cellStyleXfs>
    </xsl:if>
  </xsl:template>
  
  
  <xsl:template name="cellXfs">
    <xsl:if test="XFS">
      <cellXfs count="{count(XFS/XF/CellXF)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:apply-templates select="XFS/XF/CellXF"/>
      </cellXfs>
    </xsl:if>
  </xsl:template>

  
  <xsl:template match="XF/CellXF | XF/StyleXF">
    <xf xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
      <xsl:attribute name="numFmtId"><xsl:value-of select="../@ifmt"/></xsl:attribute> 
      <xsl:attribute name="fontId"><xsl:value-of select="../@ifnt"/></xsl:attribute> 
      <xsl:attribute name="fillId"><xsl:value-of select="@fill_x_id"/></xsl:attribute> 
      <xsl:attribute name="borderId"><xsl:value-of select="@border_x_id"/></xsl:attribute>
      <xsl:if test="self::CellXF">
        <xsl:variable name="ixfParent" select="../@ixfParent"/>
        <xsl:attribute name="xfId"><xsl:value-of select="key('get_style_xf', $ixfParent)/@style_xf_current_id"/></xsl:attribute>
      </xsl:if>
      <xsl:if test="../@f123Prefix = 'true'">
        <xsl:attribute name="quotePrefix">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fsxButton = 'true'">
        <xsl:attribute name="pivotButton">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrNum = 'true'">
        <xsl:attribute name="applyNumberFormat">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrFnt = 'true'">
        <xsl:attribute name="applyFont">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrPat = 'true'">
        <xsl:attribute name="applyFill">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrBdr = 'true'">
        <xsl:attribute name="applyBorder">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrAlc = 'true'">
        <xsl:attribute name="applyAlignment">true</xsl:attribute>
      </xsl:if>
      <xsl:if test="@fAtrProt = 'true'">
        <xsl:attribute name="applyProtection">true</xsl:attribute>
      </xsl:if>
      <xsl:call-template name="alignment">
        <xsl:with-param name="horizontal" select="@alc"/>
        <xsl:with-param name="vertical" select="@alcV"/>
        <xsl:with-param name="textRotation" select="@trot"/>
        <xsl:with-param name="wrapText" select="@fWrap"/>
        <xsl:with-param name="indent" select="@cIndent"/>
        <xsl:with-param name="relativeIndent"/>
        <xsl:with-param name="justifyLastLine" select="@fJustLast"/>
        <xsl:with-param name="shrinkToFit" select="@fShrinkToFit"/>
        <xsl:with-param name="readingOrder" select="@iReadOrder"/>
      </xsl:call-template>
      <xsl:call-template name="protection">
        <xsl:with-param name="locked" select="../@fLocked"/>
        <xsl:with-param name="hidden" select="../@fHidden"/>
      </xsl:call-template>
    </xf>
  </xsl:template>

  <xsl:template name="cellStyles">
    <xsl:if test="STYLES">
      <cellStyles count="{count(STYLES/Style)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="STYLES/Style">
          <cellStyle>
            <xsl:if test="@user != ''">
              <xsl:attribute name="name"><xsl:value-of select="@user"/></xsl:attribute>
            </xsl:if>
            <xsl:variable name="ixfe" select="@ixfe"/>
            <xsl:attribute name="xfId"><xsl:value-of select="key('get_style_xf', $ixfe)/@style_xf_current_id"/></xsl:attribute>
            <xsl:if test="@fBuiltIn = 'true'">
              <xsl:attribute name="builtinId"><xsl:value-of select="BuiltInStyle/@istyBuiltIn"/></xsl:attribute>
              <xsl:attribute name="iLevel"><xsl:value-of select="BuiltInStyle/@iLevel"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="following-sibling::*[1]/self::StyleExt and following-sibling::*[1]/self::StyleExt/@fHidden = 'true'">
              <xsl:attribute name="hidden">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="following-sibling::*[1]/self::StyleExt and following-sibling::*[1]/self::StyleExt/@fCustom = 'true'">
              <xsl:attribute name="customBuiltin">true</xsl:attribute>
            </xsl:if>
          </cellStyle>
        </xsl:for-each>
      </cellStyles>
    </xsl:if>
  </xsl:template>
  
  
  <xsl:template name="dxfs">
    <xsl:if test="DXF or ../../WorksheetSubstream/CONDFMTS/CONDFMT/CF/DXFN">
      <dxfs count="{count(DXF) + count(../../WorksheetSubstream/CONDFMTS//DXFN)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:for-each select="DXF">
          <dxf>
            <xsl:call-template name="font">
              <xsl:with-param name="name" select="XFProps/XFProp[@xfPropType = 24]/@value"/>
              <xsl:with-param name="charset" select="XFProps/XFProp[@xfPropType = 34]/@value"/>
              <xsl:with-param name="family" select="XFProps/XFProp[@xfPropType = 35]/@value"/>
              <xsl:with-param name="b" select="XFProps/XFProp[@xfPropType = 25]/@value"/>
              <xsl:with-param name="i" select="XFProps/XFProp[@xfPropType = 28]/@value"/>
              <xsl:with-param name="strike" select="XFProps/XFProp[@xfPropType = 29]/@value"/>
              <xsl:with-param name="outline" select="XFProps/XFProp[@xfPropType = 30]/@value"/>
              <xsl:with-param name="shadow" select="XFProps/XFProp[@xfPropType = 31]/@value"/>
              <xsl:with-param name="condense" select="XFProps/XFProp[@xfPropType = 32]/@value"/>
              <xsl:with-param name="extend" select="XFProps/XFProp[@xfPropType = 33]/@value"/>
              <xsl:with-param name="propColor" select="XFProps/XFProp[@xfPropType = 5]/XFPropColor"/>  <!-- XFPropColor -->
              <xsl:with-param name="sz" select="XFProps/XFProp[@xfPropType = 36]/@value"/>
              <xsl:with-param name="u" select="XFProps/XFProp[@xfPropType = 26]/@value"/>
              <xsl:with-param name="vertAlign" select="XFProps/XFProp[@xfPropType = 27]/@value"/>
              <xsl:with-param name="scheme" select="XFProps/XFProp[@xfPropType = 37]/@value"/>
            </xsl:call-template>
            <xsl:call-template name="numFmt">
              <xsl:with-param name="numFmtId" select="XFProps/XFProp[@xfPropType = 41]/@value"/>
              <xsl:with-param name="formatCode" select="XFProps/XFProp[@xfPropType = 38]/@value"/>
            </xsl:call-template>
            <xsl:call-template name="fill">
              <!-- Gradient info takes precedence of filltype so no condition necessary here -->
              <xsl:with-param name="gradient" select="XFProps/XFProp[@xfPropType = 3]/XFPropGradient"/>
              <xsl:with-param name="gradStops" select="XFProps/XFProp[@xfPropType = 4]/XFPropGradientStop"/>
              <xsl:with-param name="patternType" select="XFProps/XFProp[@xfPropType = 0]/@value"/>
              <xsl:with-param name="fgColor" select="XFProps/XFProp[@xfPropType = 1]/XFPropColor"/>
              <xsl:with-param name="bgColor" select="XFProps/XFProp[@xfPropType = 2]/XFPropColor"/>
            </xsl:call-template>
            <xsl:call-template name="alignment">
              <xsl:with-param name="horizontal" select="XFProps/XFProp[@xfPropType = 15]/@value"/>
              <xsl:with-param name="vertical" select="XFProps/XFProp[@xfPropType = 16]/@value"/>
              <xsl:with-param name="textRotation" select="XFProps/XFProp[@xfPropType = 17]/@value"/>
              <xsl:with-param name="wrapText" select="XFProps/XFProp[@xfPropType = 20]/@value"/>
              <xsl:with-param name="indent" select="XFProps/XFProp[@xfPropType = 18]/@value"/>
              <xsl:with-param name="relativeIndent" select="XFProps/XFProp[@xfPropType = 42]/@value"/>
              <xsl:with-param name="justifyLastLine" select="XFProps/XFProp[@xfPropType = 21]/@value"/>
              <xsl:with-param name="shrinkToFit" select="XFProps/XFProp[@xfPropType = 22]/@value"/>
              <xsl:with-param name="readingOrder" select="XFProps/XFProp[@xfPropType = 19]/@value"/>
            </xsl:call-template>
            <xsl:call-template name="border">
              <xsl:with-param name="diagonalDown" select="XFProps/XFProp[@xfPropType = 13]/@value"/>
              <xsl:with-param name="diagonalUp" select="XFProps/XFProp[@xfPropType = 14]/@value"/>
              <xsl:with-param name="dgLeft" select="XFProps/XFProp[@xfPropType = 8]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorLeft" select="XFProps/XFProp[@xfPropType = 8]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgRight" select="XFProps/XFProp[@xfPropType = 9]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorRight" select="XFProps/XFProp[@xfPropType = 9]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgTop" select="XFProps/XFProp[@xfPropType = 6]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorTop" select="XFProps/XFProp[@xfPropType = 6]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgBottom" select="XFProps/XFProp[@xfPropType = 7]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorBottom" select="XFProps/XFProp[@xfPropType = 7]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgDiag" select="XFProps/XFProp[@xfPropType = 10]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorDiag" select="XFProps/XFProp[@xfPropType = 10]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgVert" select="XFProps/XFProp[@xfPropType = 11]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorVert" select="XFProps/XFProp[@xfPropType = 11]/XFPropBorder/XFPropColor"/>
              <xsl:with-param name="dgHoriz" select="XFProps/XFProp[@xfPropType = 12]/XFPropBorder/@dgBorder"/>
              <xsl:with-param name="colorHoriz" select="XFProps/XFProp[@xfPropType = 12]/XFPropBorder/XFPropColor"/>
            </xsl:call-template>
            <xsl:call-template name="protection">
              <xsl:with-param name="locked" select="XFProps/XFProp[@xfPropType = 43]/@value"/>
              <xsl:with-param name="hidden" select="XFProps/XFProp[@xfPropType = 44]/@value"/>
            </xsl:call-template>

          </dxf>
        </xsl:for-each>
        <xsl:for-each select="../../WorksheetSubstream/CONDFMTS//DXFN">
          <dxf>
            <xsl:call-template name="font">
              <xsl:with-param name="useDefaults">false</xsl:with-param>
              <xsl:with-param name="name" select="DXFFntD/@stFontName"/>
              <xsl:with-param name="charset" select="DXFFntD/Stxp/@bCharSet"/>
              <xsl:with-param name="family" select="DXFFntD/Stxp/@bFamily"/>
              <xsl:with-param name="b">
                <xsl:if test="DXFFntD/@fBlsNinch = 'false'">
                  <xsl:value-of select="DXFFntD/Stxp/@bls"/>
                </xsl:if>
              </xsl:with-param>
              <xsl:with-param name="i">
                <xsl:if test="DXFFntD/@ftsItalic = 'false'">
                  <xsl:value-of select="DXFFntD/Stxp/@ftsItalic"/>
                </xsl:if>
              </xsl:with-param>
              <xsl:with-param name="strike">
                <xsl:if test="DXFFntD/@ftsStrikeout = 'false'">
                  <xsl:value-of select="DXFFntD/Stxp/@ftsStrikeout"/>
                </xsl:if>
              </xsl:with-param>
              <xsl:with-param name="propColor" select="DXFFntD/@icvFore"/>  <!-- Icv -->
              <xsl:with-param name="sz" select="DXFFntD/Stxp/@twpHeight"/>
              <xsl:with-param name="u">
                <xsl:if test="DXFFntD/@fUlsNinch = 'false'">
                  <xsl:value-of select="DXFFntD/Stxp/@uls"/>
                </xsl:if>
              </xsl:with-param>
              <xsl:with-param name="vertAlign">
                <xsl:if test="DXFFntD/@fSssNinch = 'false'">
                  <xsl:value-of select="DXFFntD/Stxp/@sss"/>
                </xsl:if>
              </xsl:with-param>
            </xsl:call-template>
            <xsl:call-template name="numFmt">
              <xsl:with-param name="numFmtId" select="DXFNum/@ifmt"/>
              <xsl:with-param name="formatCode" select="DXFNum/@fmt"/>
            </xsl:call-template>
            <xsl:variable name="nID"><xsl:value-of select="../../CondFmt/@nID"/></xsl:variable>
            <xsl:call-template name="fill">
              <!-- Gradient info takes precedence of filltype so no condition necessary here -->
              <xsl:with-param name="gradient" select="../../../CFEx[@nID = $nID]/CFExNonCF12/DXFN12/XFExtNoFRT/ExtProp/XFExtGradient/XFPropGradient"/>
              <xsl:with-param name="gradStops" select="../../../CFEx[@nID = $nID]/CFExNonCF12/DXFN12/XFExtNoFRT/ExtProp/XFExtGradient/GradStop"/>
              <xsl:with-param name="patternType" select="DXFPat/@fls"/>
              <xsl:with-param name="fgColor" select="DXFPat/@icvForeground"/>
              <xsl:with-param name="bgColor" select="DXFPat/@icvBackground"/>
            </xsl:call-template>
            <xsl:call-template name="alignment">
              <xsl:with-param name="horizontal" select="DXFALC/@alc"/>
              <xsl:with-param name="vertical" select="DXFALC/@alcv"/>
              <xsl:with-param name="textRotation" select="DXFALC/@trot"/>
              <xsl:with-param name="wrapText" select="DXFALC/@fWrap"/>
              <xsl:with-param name="indent" select="DXFALC/@cIndent"/>
              <xsl:with-param name="relativeIndent" select="DXFALC/@iIndent"/>
              <xsl:with-param name="justifyLastLine" select="DXFALC/@fJustLast"/>
              <xsl:with-param name="shrinkToFit" select="DXFALC/@fShrinkToFit"/>
              <xsl:with-param name="readingOrder" select="DXFALC/@iReadingOrder"/>
            </xsl:call-template>
            <xsl:call-template name="border">
              <xsl:with-param name="diagonalDown" select="DXFBdr/@bitDiagDown"/>
              <xsl:with-param name="diagonalUp" select="DXFBdr/@bitDiagUp"/>
              <xsl:with-param name="dgLeft" select="DXFBdr/@dgLeft"/>
              <xsl:with-param name="colorLeft" select="DXFBdr/@icvLeft"/>
              <xsl:with-param name="dgRight" select="DXFBdr/@dgRight"/>
              <xsl:with-param name="colorRight" select="DXFBdr/@icvRight"/>
              <xsl:with-param name="dgTop" select="DXFBdr/@dgTop"/>
              <xsl:with-param name="colorTop" select="DXFBdr/@icvTop"/>
              <xsl:with-param name="dgBottom" select="DXFBdr/@dgBottom"/>
              <xsl:with-param name="colorBottom" select="DXFBdr/@icvBottom"/>
              <xsl:with-param name="dgDiag" select="DXFBdr/@dgDiag"/>
              <xsl:with-param name="colorDiag" select="DXFBdr/@icvDiag"/>
            </xsl:call-template>
            <xsl:call-template name="protection">
              <xsl:with-param name="locked" select="DXFProt/@fLocked"/>
              <xsl:with-param name="hidden" select="DXFProt/@fHidden"/>
            </xsl:call-template>

          </dxf>
        </xsl:for-each>
      </dxfs>
    </xsl:if>
  </xsl:template>

  
  <xsl:template name="font">
    <xsl:param name="tag_name">font</xsl:param> <!-- The default tag is 'font' but is used for 'rPr' also -->
    <xsl:param name="name"/>
    <xsl:param name="rFont"/>
    <xsl:param name="charset"/>
    <xsl:param name="family"/>
    <xsl:param name="b"/>
    <xsl:param name="i"/>
    <xsl:param name="strike"/>
    <xsl:param name="outline"/>
    <xsl:param name="shadow"/>
    <xsl:param name="condense"/>
    <xsl:param name="extend"/>
    <xsl:param name="propColor"/> <!-- XFPropColor or Icv -->
    <xsl:param name="sz"/>
    <xsl:param name="u"/>
    <xsl:param name="vertAlign"/>
    <xsl:param name="scheme"/>
    <xsl:param name="useDefaults">true</xsl:param> <!-- If set to false then even default values will be generated -->
    <xsl:if test="$name and $name != '' or $rFont or $charset or $family or $b >= 700 or $i = 'true' or $i = 1 or
                  $strike = 'true' or $strike = 1 or $outline = 'true' or $outline = 1 or
                  $shadow = 'true' or $shadow = 1 or $condense = 'true' or $condense = 1 or
                  $extend = 'true' or $extend = 1 or $propColor or $sz and $sz != -1 or $u != 0 and $u != '' or
                  $vertAlign != 0 and $vertAlign != '' or ($scheme != '' and $scheme != 255 and $scheme != 0)">
      <xsl:element name="{$tag_name}" namespace="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$name and $name != ''"><name val="{$name}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$rFont"><rFont val="{$rFont}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$charset"><charset val="{$charset}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$family"><family val="{$family}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$b != '' and ($useDefaults = 'false' or $b >= 700)">
          <b xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$b &lt; 700">
              <xsl:attribute name="val">false</xsl:attribute>
            </xsl:if>
          </b>
        </xsl:if>
        <!--<xsl:message>i=<xsl:value-of select="$i"/></xsl:message>-->
        <xsl:if test="$i != '' and ($useDefaults = 'false' or $i = 'true' or $i = 1)">
          <i xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$i = 'false' or $i = 0">
              <xsl:attribute name="val">false</xsl:attribute>
            </xsl:if>
          </i>
        </xsl:if>
        <xsl:if test="$strike != '' and ($useDefaults = 'false' or $strike = 'true' or $strike = 1)">
          <strike xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$strike = 'false' or $strike = 0">
              <xsl:attribute name="val">false</xsl:attribute>
            </xsl:if>
          </strike>
        </xsl:if>
        <xsl:if test="$outline = 'true' or $outline = 1"><outline xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$shadow = 'true' or $shadow = 1"><shadow xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$condense = 'true' or $condense = 1"><condense xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$extend = 'true' or $extend = 1"><extend xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:call-template name="color">
          <xsl:with-param name="tag_name">color</xsl:with-param>
          <xsl:with-param name="propColor" select="$propColor"/>
        </xsl:call-template>
        <xsl:if test="$sz and $sz != -1"><sz val="{$sz div 20}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"/></xsl:if>
        <xsl:if test="$strike != '' and ($useDefaults = 'false' or $strike = 'true' or $strike = 1)">
          <strike xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$strike = 'false' or $strike = 0">
              <xsl:attribute name="val">false</xsl:attribute>
            </xsl:if>
          </strike>
        </xsl:if>
        <xsl:if test="$u != '' and ($u != 0 or $useDefaults = 'false')">
          <u xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$u != 1 or $useDefaults = 'false'">
              <xsl:attribute name="val">
                <xsl:choose>
                  <xsl:when test="$u = 0">none</xsl:when>
                  <xsl:when test="$u = 1">single</xsl:when> <!-- default -->
                  <xsl:when test="$u = 2">double</xsl:when>
                  <xsl:when test="$u = 33">singleAccounting</xsl:when>
                  <xsl:when test="$u = 34">doubleAccounting</xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>
          </u>              
        </xsl:if>
        <xsl:if test="$vertAlign != 0 and $vertAlign != ''">
          <vertAlign xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:attribute name="val">
              <xsl:choose>
                <xsl:when test="$vertAlign = 0">baseline</xsl:when> <!-- default -->
                <xsl:when test="$vertAlign = 1">superscript</xsl:when>
                <xsl:when test="$vertAlign = 2">subscript</xsl:when>
              </xsl:choose>
            </xsl:attribute>
          </vertAlign>              
        </xsl:if>
        <xsl:if test="$scheme != '' and $scheme != 255 and $scheme != 0">
          <scheme xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:attribute name="val">
              <xsl:choose>
                <xsl:when test="$scheme = 0">none</xsl:when>
                <xsl:when test="$scheme = 1">major</xsl:when>
                <xsl:when test="$scheme = 2">minor</xsl:when>
              </xsl:choose>
            </xsl:attribute>
          </scheme>
        </xsl:if>
      </xsl:element>
    </xsl:if>
  </xsl:template>

  <xsl:template name="color">
    <xsl:param name="tag_name"/>
    <xsl:param name="propColor"/> <!-- XFPropColor or Icv -->
    <xsl:if test="$propColor">
      <xsl:choose>
        <xsl:when test="$propColor/self::node() and ($propColor/self::CFColor or $propColor/self::FullColorExt or
                        $propColor/self::GradStop or $propColor/self::XFPropColor) and 
                        not($propColor/@xclrType = 1 and $propColor/@xclrValue = 32767)">
          <xsl:element name="{$tag_name}" namespace="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
            <xsl:if test="$propColor/@xclrType = 0">
              <xsl:attribute name="auto">true</xsl:attribute>
            </xsl:if>
            <xsl:if test="$propColor/@xclrType = 1">
              <xsl:attribute name="indexed"><xsl:value-of select="$propColor/@xclrValue"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="$propColor/@xclrType = 2">
              <xsl:attribute name="rgb"><xsl:value-of select="$propColor/LongRGBA/@argb"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="$propColor/@xclrType = 3">
              <xsl:attribute name="theme"><xsl:value-of select="$propColor/@xclrValue"/></xsl:attribute>
            </xsl:if>
            <xsl:if test="$propColor/@numTint != 0">
              <xsl:attribute name="tint"><xsl:value-of select="$propColor/@numTint"/></xsl:attribute>
            </xsl:if>
          </xsl:element>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="$propColor != 32767 and $propColor != 0 and $propColor != -1">
            <xsl:element name="{$tag_name}" namespace="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
              <xsl:attribute name="indexed"><xsl:value-of select="$propColor"/></xsl:attribute>
            </xsl:element>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  
  <xsl:template name="numFmt">
    <xsl:param name="numFmtId"/>
    <xsl:param name="formatCode"/>
    <xsl:if test="$numFmtId != '' or $formatCode != ''">
      <numFmt xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:choose>
          <xsl:when test="$numFmtId != ''"> <!-- TODO: possible empty value-->
            <xsl:attribute name="numFmtId"><xsl:value-of select="$numFmtId"/></xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="numFmtId"><xsl:value-of select="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/Format[@stFormat = $formatCode]/@ifmt"/></xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
        <!--<xsl:message><xsl:value-of select="$numFmtId"/> = <xsl:value-of select="$formatCode"/></xsl:message>-->
        <xsl:choose>
          <xsl:when test="$formatCode != ''">
            <xsl:attribute name="formatCode"><xsl:value-of select="$formatCode"/></xsl:attribute>
          </xsl:when>
          <xsl:when test="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/Format[@ifmt = $numFmtId]">
            <xsl:attribute name="formatCode"><xsl:value-of select="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING/Format[@ifmt = $numFmtId]/@stFormat"/></xsl:attribute>
          </xsl:when>
          <xsl:when test="$numFmtId >= 1 and  $numFmtId &lt; 47">
            <xsl:attribute name="formatCode">
              <xsl:choose>
                <xsl:when test="$numFmtId = 1">0</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 2">0.00</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 3">#,##0</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 4">#,##0.00</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 13">0%</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 14">0.00%</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 15">0.00E+00</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 16">##0.0E+0</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 17">#&quot; &quot;?/?</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 18">#&quot; &quot;??/??</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 19">dd/mm/yyyy</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 20">dd/mmm/yy</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 21">dd/mmm</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 22">mmm/yy</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 23">h:mm\ AM/PM</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 24">h:mm:ss\ AM/PM</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 25">h:mm</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 26">h:mm:ss</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 27">dd/mm/yyyy\ h:mm</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 28">mm:ss</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 29">mm:ss.0</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 30">@</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 31">[h]:mm:ss</xsl:when> <!-- OK -->
                <xsl:when test="$numFmtId = 30">m/d/yy h:mm</xsl:when>
              </xsl:choose>
            </xsl:attribute>
          </xsl:when>
        </xsl:choose>
      </numFmt>
    </xsl:if>
  </xsl:template>

  
  <xsl:template name="fill">
    <xsl:param name="gradient"/> <!-- Assume 'XFPropGradient' passed-->
    <xsl:param name="gradStops"/>
    <xsl:param name="patternType"/>
    <xsl:param name="fgColor"/>
    <xsl:param name="bgColor"/>
    <xsl:if test="$gradient or $patternType or $patternType = 0">
      <fill xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:choose>
          <xsl:when test="$gradient">
            <gradientFill>
              <xsl:if test="$gradient/@type = 'true'"> <!-- Default is 'linear' -->
                <xsl:attribute name="type">path</xsl:attribute>
              </xsl:if>
              <xsl:if test="$gradient/@numDegree != 0"> <!-- Default is 0 -->
                <xsl:attribute name="degree"><xsl:value-of select="$gradient/@numDegree"/></xsl:attribute>
              </xsl:if>
              <xsl:if test="$gradient/@numFillToLeft != 0"> <!-- Default is 0 -->
                <xsl:attribute name="left"><xsl:value-of select="$gradient/@numFillToLeft"/></xsl:attribute>
              </xsl:if>
              <xsl:if test="$gradient/@numFillToRight != 0"> <!-- Default is 0 -->
                <xsl:attribute name="right"><xsl:value-of select="$gradient/@numFillToRight"/></xsl:attribute>
              </xsl:if>
              <xsl:if test="$gradient/@numFillToTop != 0"> <!-- Default is 0 -->
                <xsl:attribute name="top"><xsl:value-of select="$gradient/@numFillToTop"/></xsl:attribute>
              </xsl:if>
              <xsl:if test="$gradient/@numFillToBottom != 0"> <!-- Default is 0 -->
                <xsl:attribute name="bottom"><xsl:value-of select="$gradient/@numFillToBottom"/></xsl:attribute>
              </xsl:if>
              <xsl:for-each select="$gradStops">
                <stop position="{@numPosition}">
                  <xsl:choose>
                    <xsl:when test="self::GradStop">
                      <xsl:call-template name="color">
                        <xsl:with-param name="tag_name">color</xsl:with-param>
                        <xsl:with-param name="propColor" select="."/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="self::XFPropGradientStop">
                      <xsl:call-template name="color">
                        <xsl:with-param name="tag_name">color</xsl:with-param>
                        <xsl:with-param name="propColor" select="XFPropColor"/>
                      </xsl:call-template>
                    </xsl:when>
                  </xsl:choose>
                </stop>
              </xsl:for-each>
            </gradientFill>
          </xsl:when>
          <xsl:when test="$patternType">
            <patternFill>
              <xsl:if test="$patternType != 0">
                <xsl:attribute name="patternType">
                  <xsl:call-template name="PatternType_IDToString">
                    <xsl:with-param name="id" select="$patternType"/>
                  </xsl:call-template>
                </xsl:attribute>
              </xsl:if>
              <xsl:if test="$fgColor != 64 and $fgColor != 0">   <!-- 64 - is the index of default foreground color -->
                <xsl:call-template name="color">
                  <xsl:with-param name="tag_name">fgColor</xsl:with-param>
                  <xsl:with-param name="propColor" select="$fgColor"/>
                </xsl:call-template>
              </xsl:if>
              <xsl:if test="$bgColor != 65 and $bgColor != 0">   <!-- 65 - is the index of default background color -->
                <xsl:call-template name="color">
                  <xsl:with-param name="tag_name">bgColor</xsl:with-param>
                  <xsl:with-param name="propColor" select="$bgColor"/>
                </xsl:call-template>
              </xsl:if>
            </patternFill>
          </xsl:when>
        </xsl:choose>
      </fill>
    </xsl:if>
  </xsl:template>

  
  <xsl:template name="alignment">
    <xsl:param name="horizontal"/>
    <xsl:param name="vertical"/>
    <xsl:param name="textRotation"/>
    <xsl:param name="wrapText"/>
    <xsl:param name="indent"/>
    <xsl:param name="relativeIndent"/>
    <xsl:param name="justifyLastLine"/>
    <xsl:param name="shrinkToFit"/>
    <xsl:param name="readingOrder"/>
    <xsl:if test="($horizontal and $horizontal != 0 and $horizontal != 255) or $vertical and $vertical != 2 or 
                  $textRotation and $textRotation != 0 or $wrapText = 'true' or $wrapText = 1 or
                  $indent and $indent != 0 or $relativeIndent and $relativeIndent != 0 or
                  $justifyLastLine = 'true' or $justifyLastLine = 1 or $shrinkToFit = 'true' or 
                  $shrinkToFit = 1 or $readingOrder and $readingOrder != 0">
      <alignment xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$horizontal and $horizontal != 0 and $horizontal != 255">
          <!-- 0 means 'General alignment' - default. 255 means 'Alignment not specified' -->
          <xsl:attribute name="horizontal">
            <xsl:call-template name="HorizontalAlignment_IDToString">
              <xsl:with-param name="id" select="$horizontal"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$vertical and $vertical != 2">
          <!-- 2 means 'Bottom alignment' - default. -->
          <xsl:attribute name="vertical">
            <xsl:call-template name="VerticalAlignment_IDToString">
              <xsl:with-param name="id" select="$vertical"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$textRotation and $textRotation != 0">
          <!-- value of 255 (vertical text) included despite this is not specified in docs -->
          <xsl:attribute name="textRotation">
            <xsl:value-of select="$textRotation"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$wrapText = 'true' or $wrapText = 1">
          <xsl:attribute name="wrapText">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$indent and $indent != 0">
          <xsl:attribute name="indent"><xsl:value-of select="$indent"/></xsl:attribute>
        </xsl:if>
        <!-- 'relativeIndent' is for dxf only -->
        <xsl:if test="$relativeIndent and $relativeIndent != 255">
          <xsl:attribute name="relativeIndent"><xsl:value-of select="$relativeIndent"/></xsl:attribute>
        </xsl:if>
        <xsl:if test="$justifyLastLine = 'true' or $justifyLastLine = 1">
          <xsl:attribute name="justifyLastLine">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$shrinkToFit = 'true' or $shrinkToFit = 1">
          <xsl:attribute name="shrinkToFit">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$readingOrder and $readingOrder != 0">
          <xsl:attribute name="readingOrder"><xsl:value-of select="$readingOrder"/></xsl:attribute>
        </xsl:if>
      </alignment>
    </xsl:if>
  </xsl:template>

  
  <xsl:template name="border">
    <xsl:param name="diagonalDown"/>
    <xsl:param name="diagonalUp"/>
    <xsl:param name="dgLeft"/>
    <xsl:param name="colorLeft"/>
    <xsl:param name="dgRight"/>
    <xsl:param name="colorRight"/>
    <xsl:param name="dgTop"/>
    <xsl:param name="colorTop"/>
    <xsl:param name="dgBottom"/>
    <xsl:param name="colorBottom"/>
    <xsl:param name="dgDiag"/>
    <xsl:param name="colorDiag"/>
    <xsl:param name="dgVert"/>
    <xsl:param name="colorVert"/>
    <xsl:param name="dgHoriz"/>
    <xsl:param name="colorHoriz"/>
    <xsl:if test="$diagonalDown = 'true' or $diagonalDown = 1 or $diagonalUp = 'true' or $diagonalUp = 1 or 
                  $dgLeft != 0 or $dgRight != 0 or $dgTop != 0 or $dgBottom != 0 or $dgDiag != 0 or
                  $dgVert != 0 or $dgHoriz != 0">
      <border xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$diagonalDown = 'true' or $diagonalDown = 1">
          <xsl:attribute name="diagonalDown">true</xsl:attribute>
        </xsl:if>
        <xsl:if test="$diagonalUp = 'true' or $diagonalUp = 1">
          <xsl:attribute name="diagonalUp">true</xsl:attribute>
        </xsl:if>
        <!-- 'outline' attribute ommited here (left default) -->
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">left</xsl:with-param>
          <xsl:with-param name="value" select="$dgLeft"/>
          <xsl:with-param name="color" select="$colorLeft"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">right</xsl:with-param>
          <xsl:with-param name="value" select="$dgRight"/>
          <xsl:with-param name="color" select="$colorRight"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">top</xsl:with-param>
          <xsl:with-param name="value" select="$dgTop"/>
          <xsl:with-param name="color" select="$colorTop"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">bottom</xsl:with-param>
          <xsl:with-param name="value" select="$dgBottom"/>
          <xsl:with-param name="color" select="$colorBottom"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">diagonal</xsl:with-param>
          <xsl:with-param name="value" select="$dgDiag"/>
          <xsl:with-param name="color" select="$colorDiag"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">vertical</xsl:with-param>
          <xsl:with-param name="value" select="$dgVert"/>
          <xsl:with-param name="color" select="$colorVert"/>
        </xsl:call-template>
        <xsl:call-template name="border_property">
          <xsl:with-param name="element_name">horizontal</xsl:with-param>
          <xsl:with-param name="value" select="$dgHoriz"/>
          <xsl:with-param name="color" select="$colorHoriz"/>
        </xsl:call-template>
      </border>
    </xsl:if>
  </xsl:template>

  <xsl:template name="protection">
    <xsl:param name="locked"/>
    <xsl:param name="hidden"/>
    <xsl:if test="$locked = 'false' or $locked = 0 or $hidden = 'true' or $hidden = 1">
      <protection xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="$locked = 'false' or $locked = 0">
          <xsl:attribute name="locked">false</xsl:attribute>
        </xsl:if>
        <xsl:if test="$hidden = 'true' or $hidden = 1">
          <xsl:attribute name="hidden">true</xsl:attribute>
        </xsl:if>
      </protection>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="tableStyles">
    <xsl:if test="TABLESTYLES">
      <tableStyles count="{count(TABLESTYLES/TableStyle)}" xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="TABLESTYLES/TableStyles/@rgchDefTableStyle">
          <xsl:attribute name="defaultTableStyle"><xsl:value-of select="TABLESTYLES/TableStyles/@rgchDefTableStyle"/></xsl:attribute>
        </xsl:if>
        <xsl:if test="TABLESTYLES/TableStyles/@rgchDefPivotStyle">
          <xsl:attribute name="defaultPivotStyle"><xsl:value-of select="TABLESTYLES/TableStyles/@rgchDefPivotStyle"/></xsl:attribute>
        </xsl:if>
        <xsl:for-each select="TABLESTYLES/TableStyle">
          <tableStyle name="{@rgchName}">
            <xsl:if test="@fIsTable = 'false'">
              <xsl:attribute name="table">false</xsl:attribute>
            </xsl:if>
            <xsl:if test="@fIsPivot = 'false'">
              <xsl:attribute name="pivot">false</xsl:attribute>
            </xsl:if>
            <xsl:attribute name="count"><xsl:value-of select="@ctse"/></xsl:attribute>
            <xsl:variable name="count"><xsl:value-of select="@ctse"/></xsl:variable>
            <xsl:for-each select="following-sibling::TableStyleElement[position() &lt;= $count]">
              <tableStyleElement>
                <xsl:attribute name="type">
                  <xsl:choose>
                    <xsl:when test="@tseType = 0">wholeTable</xsl:when>
                    <xsl:when test="@tseType = 1">headerRow</xsl:when>
                    <xsl:when test="@tseType = 2">totalRow</xsl:when>
                    <xsl:when test="@tseType = 3">firstColumn</xsl:when>
                    <xsl:when test="@tseType = 4">lastColumn</xsl:when>
                    <xsl:when test="@tseType = 5">firstRowStripe</xsl:when>
                    <xsl:when test="@tseType = 6">secondRowStripe</xsl:when>
                    <xsl:when test="@tseType = 7">firstColumnStripe</xsl:when>
                    <xsl:when test="@tseType = 8">secondColumnStripe</xsl:when>
                    <xsl:when test="@tseType = 9">firstHeaderCell</xsl:when>
                    <xsl:when test="@tseType = 10">lastHeaderCell</xsl:when>
                    <xsl:when test="@tseType = 11">firstTotalCell</xsl:when>
                    <xsl:when test="@tseType = 12">lastTotalCell</xsl:when>
                    <xsl:when test="@tseType = 13">firstSubtotalColumn</xsl:when>
                    <xsl:when test="@tseType = 14">secondSubtotalColumn</xsl:when>
                    <xsl:when test="@tseType = 15">thirdSubtotalColumn</xsl:when>
                    <xsl:when test="@tseType = 16">firstSubtotalRow</xsl:when>
                    <xsl:when test="@tseType = 17">secondSubtotalRow</xsl:when>
                    <xsl:when test="@tseType = 18">thirdSubtotalRow</xsl:when>
                    <xsl:when test="@tseType = 19">blankRow</xsl:when>
                    <xsl:when test="@tseType = 20">firstColumnSubheading</xsl:when>
                    <xsl:when test="@tseType = 21">secondColumnSubheading</xsl:when>
                    <xsl:when test="@tseType = 22">thirdColumnSubheading</xsl:when>
                    <xsl:when test="@tseType = 23">firstRowSubheading</xsl:when>
                    <xsl:when test="@tseType = 24">secondRowSubheading</xsl:when>
                    <xsl:when test="@tseType = 25">thirdRowSubheading</xsl:when>
                    <xsl:when test="@tseType = 26">pageFieldLabels</xsl:when>
                    <xsl:when test="@tseType = 27">pageFieldValues</xsl:when>
                  </xsl:choose>
                </xsl:attribute>
                <xsl:if test="@size != 1">
                  <xsl:attribute name="size"><xsl:value-of select="@size"/></xsl:attribute>                  
                </xsl:if>
                <xsl:attribute name="dxfId"><xsl:value-of select="@index"/></xsl:attribute>                  
              </tableStyleElement>
            </xsl:for-each>
          </tableStyle>
        </xsl:for-each>
      </tableStyles>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="colors">
    <xsl:if test="Palette | ClrtClient">
      <colors xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main">
        <xsl:if test="Palette">
          <indexedColors>
            <rgbColor rgb="00000000"/>
            <rgbColor rgb="00FFFFFF"/>
            <rgbColor rgb="00FF0000"/>
            <rgbColor rgb="0000FF00"/>
            <rgbColor rgb="000000FF"/>
            <rgbColor rgb="00FFFF00"/>
            <rgbColor rgb="00FF00FF"/>
            <rgbColor rgb="0000FFFF"/>
            <xsl:for-each select="Palette/LongRGB">
              <rgbColor rgb="{@argb}"/>
            </xsl:for-each>
          </indexedColors>
        </xsl:if>
<!-- The following solution is wrong - ClrtClient has another meaning
        <xsl:if test="ClrtClient">
          <mruColors>
            <xsl:for-each select="ClrtClient/LongRGB">
              <color rgb="{@argb}"/>
            </xsl:for-each>            
          </mruColors>
        </xsl:if>
-->
      </colors>
    </xsl:if>
  </xsl:template>
  
  
</xsl:stylesheet>