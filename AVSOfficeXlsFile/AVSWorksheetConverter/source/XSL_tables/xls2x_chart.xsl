<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xlsx="http://www.avs4you.ru/XLSXConverter/1.0/DirectoryStructure">


  <xsl:template match="CHART" mode="charts">
    <xsl:variable name="chart_pos" select="position()"/>
    <!--<xsl:variable name="chart_pos" select="count(../../preceding-sibling::ChartSheetSubstream) + position()"/>-->
    <xlsx:file name="chart{$chart_pos}.xml">
      <xsl:apply-templates select="ChartSheetSubstream" mode="charts_actual_chart"/>
    </xlsx:file>
  </xsl:template>

  <xsl:template match="ChartSheetSubstream" mode="charts">
    <xsl:variable name="chart_pos" select="position()"/>
    <!--<xsl:variable name="chart_pos" select="count(preceding-sibling::ChartSheetSubstream) + 
                                           count(../../../preceding-sibling::WorksheetSubstream/OBJECTS/CHART) + 1"/>-->
    <xlsx:file name="chart{$chart_pos}.xml">
      <xsl:apply-templates select="." mode="charts_actual_chart"/>
    </xlsx:file>
  </xsl:template>

  <xsl:template match="ChartSheetSubstream" mode="charts_actual_chart">
    <!-- Current node must be ChartSheetSubstream -->
    <c:chartSpace xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships">

      <xsl:if test="CHARTFOMATS/FRAME/AreaFormat/@fInvertNeg = 'true'">
        <c:roundedCorners val="true"/>
      </xsl:if>

      <c:chart>
        <!-- c:title -->
        <xsl:call-template name="ATTACHEDLABEL2title">
          <xsl:with-param name="ATTACHEDLABEL" select="CHARTFOMATS/ATTACHEDLABEL[ObjectLink/@wLinkObj = 1]"/>
          <xsl:with-param name="DFTTEXT" select="CHARTFOMATS/DFTTEXT[DefaultText/@id = 2]"/>
        </xsl:call-template>

        <!-- c:view3D, c:floor, c:sideWall, c:backWall -->
        <xsl:call-template name="_3D_plane_axes"/>

        <!-- c:plotArea -->
        <xsl:call-template name="plotArea"/>

        <!-- c:legend -->
        <xsl:apply-templates select="CHARTFOMATS/AXISPARENT/CRT/LD" mode="charts"/>

        <c:plotVisOnly val="{CHARTFOMATS/ShtProps/@fPlotVisOnly = 'true'}"/>
        <c:dispBlanksAs>
          <xsl:attribute name="val">
            <xsl:choose>
              <xsl:when test="CHARTFOMATS/ShtProps/@mdBlank = 0">gap</xsl:when>
              <xsl:when test="CHARTFOMATS/ShtProps/@mdBlank = 1">zero</xsl:when>
              <xsl:when test="CHARTFOMATS/ShtProps/@mdBlank = 2">span</xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </c:dispBlanksAs>

      </c:chart>

      <!-- c:spPr -->
      <xsl:apply-templates select="CHARTFOMATS/FRAME" mode="charts"/>

    </c:chartSpace>
  </xsl:template>

  <xsl:template name="ATTACHEDLABEL2title">
    <xsl:param name="ATTACHEDLABEL"/>
    <xsl:param name="DFTTEXT"/>

    <xsl:variable name="stText" select="$ATTACHEDLABEL/AI/SeriesText/@stText"/>
    <xsl:if test="$stText != ''">

      <c:title xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <c:tx>
          <c:rich>


            <a:bodyPr>
              <xsl:if test="$ATTACHEDLABEL/Text/@trot != 0">
                <xsl:attribute name="rot">
                  <xsl:call-template name="angle_b2x">
                    <xsl:with-param name="degrees" select="$ATTACHEDLABEL/Text/@trot"/>
                  </xsl:call-template>
                </xsl:attribute>
              </xsl:if>
            </a:bodyPr>
            <a:p>
              <xsl:variable name="FontX_pos">
                <xsl:choose>
                  <xsl:when test="$ATTACHEDLABEL/FontX">
                    <xsl:value-of select="$ATTACHEDLABEL/FontX/@iFont"/>
                  </xsl:when>
                  <xsl:when test="$DFTTEXT[DefaultText/@id = 2]">
                    <xsl:value-of select="$DFTTEXT[DefaultText/@id = 2]/ATTACHEDLABEL/FontX/@iFont"/>
                  </xsl:when>
                </xsl:choose>
              </xsl:variable>

              <xsl:variable name="color_ready">
                <xsl:choose>
                  <xsl:when test="$ATTACHEDLABEL/Text">
                    <xsl:value-of select="substring($ATTACHEDLABEL/Text/LongRGB/@argb, 3)"/>
                  </xsl:when>
                  <xsl:when test="$DFTTEXT[DefaultText/@id = 2]">
                    <xsl:value-of select="substring($DFTTEXT[DefaultText/@id = 2]/ATTACHEDLABEL/Text/LongRGB/@argb, 3)"/>
                  </xsl:when>
                </xsl:choose>
              </xsl:variable>


              <xsl:variable name="font" select="$FORMATTING/Font[position() = $FontX_pos]"/>

              <a:pPr>
                <xsl:apply-templates select="$ATTACHEDLABEL/Text[1]" mode="charts"/>

                <a:defRPr>
                  <xsl:call-template name="get_chart_props_from_Font">
                    <xsl:with-param name="font" select="$font"/>
                    <xsl:with-param name="color_ready" select="$color_ready"/>
                  </xsl:call-template>
                </a:defRPr>
              </a:pPr>


              <xsl:choose>
                <xsl:when test="$ATTACHEDLABEL/AlRuns">
                  <xsl:variable name="text_len" select="string-length($stText)"/>
                  <xsl:for-each select="$ATTACHEDLABEL/AlRuns/FormatRun">
                    <xsl:if test="@ich &lt; $text_len">
                      <a:r>
                        <xsl:call-template name="gen_run_format"/>
                        <a:t>
                          <xsl:choose>
                            <xsl:when test="position() = last()">
                              <xsl:value-of select="substring($stText, @ich + 1)"/>
                            </xsl:when>
                            <xsl:otherwise>
                              <xsl:variable name="next_ich" select="following-sibling::FormatRun[1]/@ich"/>
                              <xsl:value-of select="substring($stText, @ich + 1, $next_ich - @ich)"/>
                            </xsl:otherwise>
                          </xsl:choose>
                        </a:t>
                      </a:r>
                    </xsl:if>
                  </xsl:for-each>
                </xsl:when>
                <xsl:otherwise>
                  <a:r>
                    <a:t>
                      <xsl:value-of select="$stText"/>
                    </a:t>
                  </a:r>
                </xsl:otherwise>
              </xsl:choose>



            </a:p>
          </c:rich>
        </c:tx>

        <!-- c:spPr -->
        <xsl:apply-templates select="$ATTACHEDLABEL/FRAME" mode="charts"/>

      </c:title>
    </xsl:if>
  </xsl:template>

  <xsl:variable name="FORMATTING" select="/root/WorkbookStreamObject/GlobalsSubstream/FORMATTING"/>

  <!-- Apply this for a:pPr -->
  <xsl:template match="Text" mode="charts">
    <xsl:if test="@at != 1">
      <xsl:attribute name="algn">
        <xsl:choose>
          <xsl:when test="@at = 1">l</xsl:when>
          <xsl:when test="@at = 2">ctr</xsl:when>
          <xsl:when test="@at = 3">r</xsl:when>
          <xsl:when test="@at = 4">just</xsl:when>
          <xsl:when test="@at = 7">dist</xsl:when>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@vat != 2">
      <xsl:attribute name="fontAlgn">
        <xsl:choose>
          <xsl:when test="@at = 1">t</xsl:when>
          <xsl:when test="@at = 2">cntr</xsl:when>
          <xsl:when test="@at = 3">b</xsl:when>
          <xsl:when test="@at = 4">just</xsl:when>
          <xsl:when test="@at = 7">dist</xsl:when>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@iReadingOrder = 2">
      <xsl:attribute name="rtl">true</xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="gen_run_format">
    <!-- current node must point to FormatRun -->
    <xsl:variable name="font_pos" select="@ifnt + 1"/>
    <xsl:variable name="font" select="$FORMATTING/Font[position() = $font_pos]"/>
    <a:rPr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
      <xsl:call-template name="get_chart_props_from_Font">
        <xsl:with-param name="font" select="$font"/>
      </xsl:call-template>
    </a:rPr>
  </xsl:template>

  <xsl:template name="get_chart_props_from_Font">
    <xsl:param name="font"/>
    <xsl:param name="color_ready"/>
    <xsl:if test="$font">

      <xsl:attribute name="sz">
        <xsl:value-of select="$font/@dyHeight * 5"/>
      </xsl:attribute>

      <xsl:attribute name="b">
        <xsl:choose>
          <xsl:when test="$font/@bls >= 700">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="i">
        <xsl:choose>
          <xsl:when test="$font/@fItalic = 'true'">true</xsl:when>
          <xsl:otherwise>false</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="u">
        <xsl:choose>
          <xsl:when test="$font/@uls = 0">none</xsl:when>
          <xsl:when test="$font/@uls = 1">sng</xsl:when>
          <xsl:when test="$font/@uls = 2">dbl</xsl:when>
          <xsl:when test="$font/@uls = 33">sng</xsl:when>
          <xsl:when test="$font/@uls = 34">dbl</xsl:when>
        </xsl:choose>
      </xsl:attribute>
      <xsl:attribute name="strike">
        <xsl:choose>
          <xsl:when test="$font/@fStrikeOut = 'true'">sngStrike</xsl:when>
          <xsl:otherwise>noStrike</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>

      <xsl:if test="$font/@icv != 32767 or $color_ready != ''">
        <a:solidFill xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
          <xsl:call-template name="a_color">
            <xsl:with-param name="icv" select="$font/@icv"/>
            <xsl:with-param name="color_ready" select="$color_ready"/>
          </xsl:call-template>
        </a:solidFill>
      </xsl:if>

      <a:latin typeface="{$font/@fontName}" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"/>
      <a:ea typeface="{$font/@fontName}" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"/>
      <a:cs typeface="{$font/@fontName}" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"/>

    </xsl:if>
  </xsl:template>

  <xsl:template name="a_color">
    <xsl:param name="icv"/>
    <xsl:param name="color_ready"/>
    <a:srgbClr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
      <xsl:variable name="palette_argb" select="$FORMATTING/Palette/LongRGB[position() = $icv - 8 + 1]/@argb"/>
      <xsl:attribute name="val">
        <xsl:choose>
          <xsl:when test="$color_ready != ''">
            <xsl:value-of select="$color_ready"/>
          </xsl:when>
          <xsl:when test="$palette_argb != ''">
            <xsl:value-of select="substring(palette_argb, 3)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="/root/Helpers/indexedColors/rgbColor[position() = $icv + 1]/@rgb"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </a:srgbClr>
  </xsl:template>


  <xsl:template name="plotArea">
    <!-- Current node must be ChartSheetSubstream -->
    <c:plotArea xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <!-- c:layout -->
      <xsl:if test="CHARTFOMATS/ShtProps/@fAlwaysAutoPlotArea = 'true'">
        <xsl:apply-templates select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/Pos" mode="charts"/>
      </xsl:if>

      <!-- lineChart, pieChart, etc -->
      <xsl:for-each select="CHARTFOMATS/AXISPARENT">
        <xsl:apply-templates select="CRT/Begin/following-sibling::*[1]" mode="charts_graph_type">
          <xsl:sort select="../ChartFormat/@icrt" order="descending" data-type="number"/>
        </xsl:apply-templates>
      </xsl:for-each>

      <!-- catAx, valAx, serAx -->
      <xsl:apply-templates select="CHARTFOMATS/AXISPARENT/AXES" mode="charts"/>

      <!-- c:spPr -->
      <xsl:if test="../DROPBAR/DropBar/@pcGap != 0">
        <xsl:variable name="prot_area_frame" select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/AXES/FRAME"/>
        <xsl:apply-templates select="$prot_area_frame" mode="charts"/>
        <xsl:if test="not($prot_area_frame)">
          <c:spPr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
            <a:noFill/>
            <a:ln w="25400">
              <a:noFill/>
            </a:ln>
          </c:spPr>
        </xsl:if>
      </xsl:if>

    </c:plotArea>
  </xsl:template>

  <xsl:template match="Line" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="../Chart3d">
        <xsl:call-template name="Line3D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Line2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Line2D">
    <!-- Current node must be Line  -->

    <xsl:choose>
      <xsl:when test="../DROPBAR/DropBar/@pcGap != 0">
        <c:stockChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

          <xsl:call-template name="LineChartShared"/>

          <c:axId val="{../../AXES/IVAXIS/@id}"/>
          <c:axId val="{../../AXES/DVAXIS/@id}"/>

          <c:hiLowLines>
            <c:spPr>
              <a:ln>
                <a:solidFill>
                  <a:srgbClr val="004586" />
                </a:solidFill>
              </a:ln>
            </c:spPr>
          </c:hiLowLines>
          <c:upDownBars>
            <c:gapWidth val="{../DROPBAR/DropBar/@pcGap}" />
            <c:upBars>
              <c:spPr>
                <a:solidFill>
                  <a:srgbClr val="FFFFFF" />
                </a:solidFill>
                <a:ln>
                  <a:solidFill>
                    <a:srgbClr val="000000" />
                  </a:solidFill>
                </a:ln>
              </c:spPr>
            </c:upBars>
            <c:downBars>
              <c:spPr>
                <a:solidFill>
                  <a:srgbClr val="000000" />
                </a:solidFill>
                <a:ln>
                  <a:solidFill>
                    <a:srgbClr val="B3B3B3" />
                  </a:solidFill>
                </a:ln>
              </c:spPr>
            </c:downBars>
          </c:upDownBars>

        </c:stockChart>
      </xsl:when>
      <xsl:otherwise>
        <c:lineChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

          <xsl:call-template name="LineChartShared"/>

          <c:axId val="{../../AXES/IVAXIS/@id}"/>
          <c:axId val="{../../AXES/DVAXIS/@id}"/>

        </c:lineChart>
      </xsl:otherwise>
    </xsl:choose>

    <!--<c:lineChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">     

      <xsl:call-template name="LineChartShared"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>

    </c:lineChart>-->
  </xsl:template>

  <xsl:template name="Line3D">
    <!-- Current node must be Line  -->
    <c:line3DChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="LineChartShared"/>

      <!-- c:gapDepth -->
      <xsl:if test="../Chart3d/@pcGap != 150">
        <c:gapDepth val="{../Chart3d/@pcGap}"/>
      </xsl:if>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>
      <c:axId val="{../../AXES/SERIESAXIS/@id}"/>

    </c:line3DChart>
  </xsl:template>

  <xsl:template name="LineChartShared">
    <!-- Current node must be Line -->
    <xsl:choose>
      
      <xsl:when test="../../DROPBAR/DropBar/@pcGap != 0">
        <c:grouping xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:attribute name="val">
            <xsl:choose>
              <xsl:when test="@f100 = 'true'">percentStacked</xsl:when>
              <xsl:when test="@fStacked = 'true'">stacked</xsl:when>
              <xsl:otherwise>standard</xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </c:grouping>
      </xsl:when>

      <xsl:otherwise>
        <c:grouping xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:attribute name="val">
            <xsl:choose>
              <xsl:when test="@f100 = 'true'">percentStacked</xsl:when>
              <xsl:when test="@fStacked = 'true'">stacked</xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </c:grouping>

        <!-- c:varyColors -->
        <xsl:if test="../ChartFormat/@fVaried = 'true'">
          <c:varyColors val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
        </xsl:if>
      </xsl:otherwise>
      
    </xsl:choose>

    <!-- c:ser -->
    <xsl:call-template name="LineSer"/>
  </xsl:template>

  <xsl:template name="LineSer">
    <!-- Current node must be Line -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>
    <xsl:variable name="is_linear" select="not(../Chart3d)"/>
    <xsl:variable name="AXISPARENT" select="../.."/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:marker -->
        <xsl:if test="$is_linear">
          <xsl:call-template name="ser_marker">
            <xsl:with-param name="crt" select="$crt"/>
          </xsl:call-template>
        </xsl:if>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
          <xsl:with-param name="is_linear" select="$is_linear"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:trendline -->
        <xsl:variable name="trend_series" select="../SERIESFORMAT[SerParent/@series = $ser_id + 1 and SerAuxTrend]"/>
        <xsl:apply-templates select="$trend_series" mode="trend_series"/>

        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

        <!-- c:smooth -->
        <xsl:call-template name="ser_smooth">
          <xsl:with-param name="SSs" select="SS[DataFormat/@xi = 65535]"/>
          <xsl:with-param name="SSa" select="$crt/SS"/>
        </xsl:call-template>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Bar" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="../Chart3d">
        <xsl:call-template name="Bar3D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Bar2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Bar2D">
    <!-- Current node must be Bar -->
    <c:barChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="BarChartShared"/>

      <!-- c:gapWidth -->
      <xsl:if test="@pcGap != 150">
        <c:gapWidth val="{@pcGap}"/>
      </xsl:if>
      <!-- c:overlap -->
      <xsl:if test="@pcOverlap != 0">
        <c:overlap val="{-@pcOverlap}"/>
      </xsl:if>

      <!-- c:serLines -->
      <xsl:apply-templates select="../CrtLine[@id = 2]" mode="charts"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>

    </c:barChart>
  </xsl:template>

  <xsl:template name="Bar3D">
    <!-- Current node must be Bar -->
    <c:bar3DChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="BarChartShared"/>

      <!-- c:gapWidth -->
      <xsl:if test="@pcGap != 150">
        <c:gapWidth val="{@pcGap}"/>
      </xsl:if>

      <!-- c:gapDepth -->
      <xsl:if test="../Chart3d/@pcGap != 150">
        <c:gapDepth val="{../Chart3d/@pcGap}"/>
      </xsl:if>

      <!-- c:shape -->
      <xsl:apply-templates select="../Chart3DBarShape" mode="charts"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>
      <xsl:if test="../../AXES/SERIESAXIS">
        <c:axId val="{../../AXES/SERIESAXIS/@id}"/>
      </xsl:if>
    </c:bar3DChart>
  </xsl:template>

  <xsl:template name="BarChartShared">
    <!-- Current node must be Bar -->
    <xsl:param name="has_3D"/>

    <c:barDir xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <xsl:when test="@fTranspose = 'true'">bar</xsl:when>
          <xsl:otherwise>col</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:barDir>

    <c:grouping xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <xsl:when test="@f100 = 'true'">percentStacked</xsl:when>
          <xsl:when test="@fStacked = 'true'">stacked</xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="../Chart3d/@fCluster = 'false'">standard</xsl:when>
              <xsl:otherwise>clustered</xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:grouping>

    <!-- c:varyColors -->
    <xsl:if test="../ChartFormat/@fVaried = 'true'">
      <c:varyColors val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>

    <!-- c:ser -->
    <xsl:call-template name="BarSer"/>

  </xsl:template>

  <xsl:template name="BarSer">
    <!-- Current node must be Bar -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:trendline -->
        <xsl:variable name="trend_series" select="../SERIESFORMAT[SerParent/@series = $ser_id + 1 and SerAuxTrend]"/>
        <xsl:apply-templates select="$trend_series" mode="trend_series"/>

        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

        <!-- c:shape -->
        <xsl:apply-templates select="SS[DataFormat/@xi = 65535]/Chart3DBarShape" mode="charts"/>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Pie" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="@pcDonut != 0">
        <xsl:call-template name="doughnutChart2D"/>
      </xsl:when>
      <xsl:when test="../Chart3d">
        <xsl:call-template name="Pie3D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Pie2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Pie2D">
    <!-- Current node must be Pie -->
    <c:pieChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="PieChartShared"/>

      <!-- c:firstSliceAng -->
      <xsl:if test="@anStart != 0">
        <c:firstSliceAng val="{@anStart}"/>
      </xsl:if>
    </c:pieChart>
  </xsl:template>

  <xsl:template name="Pie3D">
    <!-- Current node must be Pie -->
    <c:pie3DChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="PieChartShared">
        <xsl:with-param name="has_3D">true</xsl:with-param>
      </xsl:call-template>

    </c:pie3DChart>
  </xsl:template>

  <xsl:template name="doughnutChart2D">
    <!-- Current node must be Pie -->
    <c:doughnutChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="PieChartShared">
        <xsl:with-param name="has_3D">true</xsl:with-param>
      </xsl:call-template>

      <!-- c:firstSliceAng -->
      <xsl:if test="@anStart != 0">
        <c:firstSliceAng val="{@anStart}"/>
      </xsl:if>

      <!-- c:holeSize -->
      <xsl:if test="@pcDonut != 10">
        <c:holeSize val="{@pcDonut}"/>
      </xsl:if>

    </c:doughnutChart>
  </xsl:template>

  <xsl:template name="PieChartShared">
    <!-- Current node must be Pie -->
    <xsl:param name="has_3D"/>

    <!-- c:varyColors -->
    <xsl:if test="../ChartFormat/@fVaried = 'true'">
      <c:varyColors val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>

    <!-- c:ser -->
    <xsl:call-template name="PieSer"/>

  </xsl:template>

  <xsl:template name="PieSer">
    <!-- Current node must be Pie or BopPop -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:explosion -->
        <xsl:variable name="explosion_value">
          <xsl:choose>
            <xsl:when test="SS[DataFormat/@xi = 65535]/PieFormat">
              <xsl:value-of select="SS[DataFormat/@xi = 65535]/PieFormat/@pcExplode"/>
            </xsl:when>
            <xsl:when test="$crt/SS/PieFormat">
              <xsl:value-of select="$crt/SS/PieFormat/@pcExplode"/>
            </xsl:when>
          </xsl:choose>
        </xsl:variable>
        <xsl:if test="$explosion_value != ''">
          <c:explosion val="{$explosion_value}"/>
        </xsl:if>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Scatter" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="@fBubbles = 'true'">
        <xsl:call-template name="Bubbles2D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Scatter2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Bubbles2D">
    <!-- Current node must be Scatter -->
    <c:bubbleChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <!-- c:varyColors -->
      <xsl:if test="../ChartFormat/@fVaried = 'true'">
        <c:varyColors val="true"/>
      </xsl:if>

      <xsl:call-template name="BubblesSer"/>

      <c:bubbleScale val="{@pcBubbleSizeRatio}"/>
      <!-- c:showNegBubbles -->
      <xsl:if test="@fShowNegBubbles = 'true'">
        <c:showNegBubbles val="true"/>
      </xsl:if>
      <!-- c:sizeRepresents  val="area" is default-->
      <xsl:if test="@wBubbleSize = 2">
        <c:sizeRepresents val="w"/>
      </xsl:if>

      <c:axId val="{../../AXES/DVAXIS[1]/@id}"/>
      <c:axId val="{../../AXES/DVAXIS[2]/@id}"/>
    </c:bubbleChart>
  </xsl:template>

  <xsl:template name="Scatter2D">
    <!-- Current node must be Scatter -->
    <c:scatterChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <c:scatterStyle val="marker"/>
      <!-- c:varyColors -->
      <xsl:if test="../ChartFormat/@fVaried = 'true'">
        <c:varyColors val="true"/>
      </xsl:if>

      <xsl:call-template name="ScatterSer"/>

      <c:axId val="{../../AXES/DVAXIS[1]/@id}"/>
      <c:axId val="{../../AXES/DVAXIS[2]/@id}"/>
    </c:scatterChart>
  </xsl:template>

  <xsl:template name="ScatterSer">
    <!-- Current node must be Scatter -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>
    <xsl:variable name="is_linear" select="not(../Chart3d)"/>
    <xsl:variable name="AXISPARENT" select="../.."/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:marker -->
        <xsl:if test="$is_linear">
          <xsl:call-template name="ser_marker">
            <xsl:with-param name="crt" select="$crt"/>
          </xsl:call-template>
        </xsl:if>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
          <xsl:with-param name="is_linear">true</xsl:with-param>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:trendline -->
        <xsl:variable name="trend_series" select="../SERIESFORMAT[SerParent/@series = $ser_id + 1 and SerAuxTrend]"/>
        <xsl:apply-templates select="$trend_series" mode="trend_series"/>

        <!-- c:xVal, c:yVal, c:bubbleSize -->
        <xsl:call-template name="xVal_yVal_bubbleSize"/>

        <!-- c:smooth -->
        <xsl:call-template name="ser_smooth">
          <xsl:with-param name="SSs" select="SS[DataFormat/@xi = 65535]"/>
          <xsl:with-param name="SSa" select="$crt/SS"/>
        </xsl:call-template>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template name="BubblesSer">
    <!-- Current node must be Scatter -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>
    <xsl:variable name="AXISPARENT" select="../.."/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <xsl:if test="@fShowNegBubbles = 'true'">
          <c:invertIfNegative val="true"/>
        </xsl:if>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:trendline -->
        <xsl:variable name="trend_series" select="../SERIESFORMAT[SerParent/@series = $ser_id + 1 and SerAuxTrend]"/>
        <xsl:apply-templates select="$trend_series" mode="trend_series"/>

        <!-- c:xVal, c:yVal, c:bubbleSize -->
        <xsl:call-template name="xVal_yVal_bubbleSize">
          <xsl:with-param name="bubbles">true</xsl:with-param>
        </xsl:call-template>

        <!-- c:bubble3D -->
        <xsl:choose>
          <xsl:when test="SS[DataFormat/@xi = 65535]/SerFmt">
            <c:bubble3D val="{SS[DataFormat/@xi = 65535]/SerFmt/@f3DBubbles}"/>
          </xsl:when>
          <xsl:when test="$crt/SS/SerFmt">
            <c:bubble3D val="{$crt/SS/SerFmt/@f3DBubbles}"/>
          </xsl:when>
        </xsl:choose>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Area" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="../Chart3d">
        <xsl:call-template name="Area3D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Area2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Area2D">
    <!-- Current node must be Area  -->
    <c:areaChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="AreaChartShared"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>

    </c:areaChart>
  </xsl:template>

  <xsl:template name="Area3D">
    <!-- Current node must be Area  -->
    <c:area3DChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="AreaChartShared"/>

      <!-- c:gapDepth -->
      <xsl:if test="../Chart3d/@pcGap != 150">
        <c:gapDepth val="{../Chart3d/@pcGap}"/>
      </xsl:if>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>
      <xsl:if test="../../AXES/SERIESAXIS">
        <c:axId val="{../../AXES/SERIESAXIS/@id}"/>
      </xsl:if>
    </c:area3DChart>
  </xsl:template>

  <xsl:template name="AreaChartShared">
    <!-- Current node must be Area -->
    <c:grouping xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <xsl:when test="@f100 = 'true'">percentStacked</xsl:when>
          <xsl:when test="@fStacked = 'true'">stacked</xsl:when>
          <xsl:otherwise>standard</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:grouping>
    <!-- c:varyColors -->
    <xsl:if test="../ChartFormat/@fVaried = 'true'">
      <c:varyColors val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>

    <!-- c:ser -->
    <xsl:call-template name="AreaSer"/>

  </xsl:template>

  <xsl:template name="AreaSer">
    <!-- Current node must be Area -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>
    <xsl:variable name="marker_need" select="not(../Chart3d)"/>
    <xsl:variable name="AXISPARENT" select="../.."/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
            <xsl:with-param name="is_area">true</xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:trendline -->
        <xsl:variable name="trend_series" select="../SERIESFORMAT[SerParent/@series = $ser_id + 1 and SerAuxTrend]"/>
        <xsl:apply-templates select="$trend_series" mode="trend_series"/>

        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Radar | RadarArea" mode="charts_graph_type">
    <c:radarChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:choose>
        <xsl:when test="self::Radar">
          <c:radarStyle val="standard"/>
        </xsl:when>
        <xsl:otherwise>
          <c:radarStyle val="filled"/>
        </xsl:otherwise>
      </xsl:choose>
      <!-- c:varyColors -->
      <xsl:if test="../ChartFormat/@fVaried = 'true'">
        <c:varyColors val="true"/>
      </xsl:if>

      <!-- c:ser -->
      <xsl:call-template name="RadarSer"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>

    </c:radarChart>
  </xsl:template>

  <xsl:template name="RadarSer">
    <!-- Current node must be Radar or RadarArea -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>
    <xsl:variable name="AXISPARENT" select="../.."/>
    <xsl:variable name="is_linear" select="not(self::RadarArea)"/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">

      <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:marker -->
        <xsl:if test="$is_linear">
          <xsl:call-template name="ser_marker">
            <xsl:with-param name="crt" select="$crt"/>
          </xsl:call-template>
        </xsl:if>

        <!-- c:dPt -->
        <xsl:call-template name="c_dPt">
          <xsl:with-param name="crt" select="$crt"/>
          <xsl:with-param name="is_linear" select="$is_linear"/>
        </xsl:call-template>

        <!-- c:dLbls -->
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_dLbls">
            <xsl:with-param name="order_id" select="$ser_order"/>
            <xsl:with-param name="is_area" select="not($is_linear)"/>
          </xsl:call-template>
        </xsl:for-each>


        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="Surf" mode="charts_graph_type">
    <xsl:choose>
      <xsl:when test="../Chart3d">
        <xsl:call-template name="Surf3D"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="Surf2D"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="Surf2D">
    <!-- Current node must be Surf -->
    <c:surfaceChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="SurfaceChartShared"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>
      <xsl:if test="../../AXES/SERIESAXIS">
        <c:axId val="{../../AXES/SERIESAXIS/@id}"/>
      </xsl:if>

    </c:surfaceChart>
  </xsl:template>

  <xsl:template name="Surf3D">
    <!-- Current node must be Surf -->
    <c:surface3DChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="SurfaceChartShared"/>

      <c:axId val="{../../AXES/IVAXIS/@id}"/>
      <c:axId val="{../../AXES/DVAXIS/@id}"/>
      <c:axId val="{../../AXES/SERIESAXIS/@id}"/>

    </c:surface3DChart>
  </xsl:template>

  <xsl:template name="SurfaceChartShared">
    <!-- Current node must be Surf -->

    <xsl:if test="@fFillSurface = 'false'">
      <c:wireframe val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>

    <!-- c:ser -->
    <xsl:call-template name="SurfSer"/>

    <!-- c:bandFmts -->

  </xsl:template>

  <xsl:template name="SurfSer">
    <!-- Current node must be Surf -->
    <xsl:variable name="crt" select=".."/>
    <xsl:variable name="crt_id" select="$crt/ChartFormat/@icrt"/>

    <xsl:for-each select="../../../SERIESFORMAT[SerToCrt/@id = $crt_id]">

      <c:ser xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="SerShared"/>

        <!-- c:cat, c:val -->
        <xsl:call-template name="cat_val"/>

      </c:ser>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="BopPop" mode="charts_graph_type">
    <c:ofPieChart xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <c:ofPieType>
        <xsl:attribute name="val">
          <xsl:choose>
            <xsl:when test="@pst = 1">pie</xsl:when>
            <xsl:when test="@pst = 2">bar</xsl:when>
          </xsl:choose>
        </xsl:attribute>
      </c:ofPieType>

      <xsl:call-template name="PieChartShared"/>

      <c:gapWidth val="{@pcGap}"/>

      <c:splitType>
        <xsl:attribute name="val">
          <xsl:choose>
            <xsl:when test="@split = 0">pos</xsl:when>
            <xsl:when test="@split = 1">val</xsl:when>
            <xsl:when test="@split = 2">percent</xsl:when>
            <xsl:when test="@split = 3">cust</xsl:when>
            <xsl:when test="@split = 'true'">auto</xsl:when>
          </xsl:choose>
        </xsl:attribute>
      </c:splitType>

      <c:splitPos val="{@numSplitValue}"/>

      <!-- c:custSplit -->
      <xsl:if test="@split = 3">
        <c:custSplit>
          <xsl:for-each select="following-sibling::BopPopCustom[1]/pie">
            <c:secondPiePt val="{@index}"/>
          </xsl:for-each>
        </c:custSplit>
      </xsl:if>

      <c:secondPieSize val="{@pcPie2Size}"/>

      <!-- c:serLines -->
      <xsl:apply-templates select="../CrtLine[@id = 2]" mode="charts"/>

    </c:ofPieChart>
  </xsl:template>



  <xsl:template match="AXES" mode="charts">
    <xsl:apply-templates select="IVAXIS | DVAXIS | SERIESAXIS" mode="charts"/>
  </xsl:template>

  <xsl:template match="IVAXIS" mode="charts">
    <c:catAx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="AxShared"/>

    </c:catAx>
  </xsl:template>

  <xsl:template match="DVAXIS" mode="charts">
    <c:valAx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="AxShared"/>

      <!-- c:crossBetween -->
      <xsl:if test="../IVAXIS">
        <c:crossBetween xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:attribute name="val">
            <xsl:choose>
              <xsl:when test="../IVAXIS[1]/CatSerRange/@fBetween = 'false'">midCat</xsl:when>
              <xsl:otherwise>between</xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </c:crossBetween>
      </xsl:if>

    </c:valAx>
  </xsl:template>

  <xsl:template match="SERIESAXIS" mode="charts">
    <c:serAx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">

      <xsl:call-template name="AxShared"/>

    </c:serAx>
  </xsl:template>

  <xsl:template name="AxShared">
    <!-- Current node must be SERIESAXIS | DVAXIS | IVAXIS -->
    <c:axId val="{@id}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    <c:scaling xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <!-- c:logBase -->
      <xsl:if test="ValueRange and ValueRange/@fLog = 'true'">
        <xsl:choose>
          <xsl:when test="CRTMLFRT/CrtMlFrt/XmlTkLogBaseFrt">
            <c:logBase val="{CRTMLFRT/CrtMlFrt/XmlTkLogBaseFrt/@logScale}"/>
          </xsl:when>
          <xsl:otherwise>
            <c:logBase val="10"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:if>
      <!-- c:orientation -->
      <xsl:choose>
        <xsl:when test="CatSerRange/@fReverse = 'true' or ValueRange/@fReversed = 'true'">
          <c:orientation val="maxMin"/>
        </xsl:when>
        <xsl:otherwise>
          <c:orientation val="minMax"/>
        </xsl:otherwise>
      </xsl:choose>
      <!-- c:min -->
      <xsl:if test="ValueRange and ValueRange/@fAutoMin != 'true'">
        <c:min val="{ValueRange/@numMin}"/>
      </xsl:if>
      <!-- c:max -->
      <xsl:if test="ValueRange and ValueRange/@fAutoMax != 'true'">
        <c:max val="{ValueRange/@numMax}"/>
      </xsl:if>
    </c:scaling>

    <xsl:variable name="self_LineFormat" select="AXS/LineFormat[preceding-sibling::*[1]/self::AxisLine/@id = 0]"/>

    <!--<xsl:if test="../../AxisParent/@iax = 'true' and (self::IVAXIS or self::DVAXIS and Axis/@wType = 0) or
                  $self_LineFormat/@fAxisOn = 'false' and $self_LineFormat/@lns = 5">
      <c:delete val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>-->

    <xsl:choose>
      <xsl:when test="../../AxisParent/@iax = 'true' and (self::IVAXIS or self::DVAXIS and Axis/@wType = 0) or
                  $self_LineFormat/@fAxisOn = 'false' and $self_LineFormat/@lns = 5">
        <c:delete val="1" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:otherwise>
        <c:delete val="0" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:otherwise>
    </xsl:choose>

    <c:axPos xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <!-- Category axis or Series axis or horizontal Value axix -->
          <xsl:when test="self::IVAXIS or self::SERIESAXIS or self::DVAXIS and Axis/@wType = 0">b</xsl:when>
          <!-- Secondary value axis -->
          <xsl:when test="../../AxisParent/@iax = 'true'">r</xsl:when>
          <!-- Primary value axis -->
          <xsl:otherwise>l</xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:axPos>

    <!-- majorGridlines -->
    <xsl:apply-templates select="AXS/AxisLine[@id = 1]" mode="charts"/>

    <!-- minorGridlines -->
    <xsl:apply-templates select="AXS/AxisLine[@id = 2]" mode="charts"/>

    <!-- title -->
    <xsl:variable name="label_wLinkObj">
      <xsl:choose>
        <!-- Category axis -->
        <xsl:when test="self::IVAXIS">3</xsl:when>
        <!-- Series axis -->
        <xsl:when test="self::SERIESAXIS">7</xsl:when>
        <!-- Value axis (horizontal) -->
        <xsl:when test="self::DVAXIS and Axis/@wType = 0">3</xsl:when>
        <!-- Value axis -->
        <xsl:when test="self::DVAXIS">2</xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:call-template name="ATTACHEDLABEL2title">
      <xsl:with-param name="ATTACHEDLABEL" select="../ATTACHEDLABEL[ObjectLink/@wLinkObj = $label_wLinkObj]"/>
      <xsl:with-param name="DFTTEXT" select="../../../DFTTEXT[DefaultText/@id = 2]"/>
    </xsl:call-template>

    <!-- majorTickMark, minorTickMark, tickLblPos -->
    <xsl:apply-templates select="AXS/Tick" mode="charts"/>

    <!-- spPr of the axis itself -->
    <xsl:choose>
      <xsl:when test="AXS/AxisLine[@id = 0]">
        <c:spPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:apply-templates select="AXS/AxisLine[@id = 0]" mode="charts"/>
        </c:spPr>
      </xsl:when>
      <xsl:otherwise>
        <c:spPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <a:ln w="3175" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
            <a:solidFill>
              <a:srgbClr val="000000"/>
            </a:solidFill>
            <a:prstDash val="solid"/>
          </a:ln>
        </c:spPr>
      </xsl:otherwise>

    </xsl:choose>

    <!-- txPr of the axis tick labels -->
    <xsl:call-template name="cat_val_txPr">
      <xsl:with-param name="where" select="."/>
    </xsl:call-template>

    <xsl:variable name="self_id" select="@id"/>
    <c:crossAx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <!-- Category axis -->
          <xsl:when test="self::IVAXIS">
            <xsl:value-of select="../DVAXIS[1]/@id"/>
          </xsl:when>
          <!-- Series axis -->
          <xsl:when test="self::SERIESAXIS">
            <xsl:value-of select="../DVAXIS[1]/@id"/>
          </xsl:when>
          <!-- Value axis crossed with category axis -->
          <xsl:when test="self::DVAXIS and ../IVAXIS">
            <xsl:value-of select="../IVAXIS[1]/@id"/>
          </xsl:when>
          <!-- Value axis crossed with value axis -->
          <xsl:otherwise>
            <xsl:value-of select="../DVAXIS[@id != $self_id]/@id"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:crossAx>

    <!-- c:crosses -->
    <xsl:if test="self::IVAXIS and ../DVAXIS[1]/ValueRange/@fMaxCross = 'true' or 
            self::DVAXIS and ../IVAXIS[1]/CatSerRange/@fMaxCross = 'true' or 
            self::DVAXIS and ../DVAXIS[@id != $self_id]/ValueRange/@fMaxCross = 'true' or 
            self::SERIESAXIS and ../DVAXIS[1]/ValueRange/@fMaxCross = 'true'">
      <c:crosses val="max" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
    </xsl:if>

  </xsl:template>

  <xsl:template name="tick_num2tick_attr">
    <xsl:param name="tick_num"/>
    <xsl:attribute name="val">
      <xsl:choose>
        <xsl:when test="$tick_num = 0">none</xsl:when>
        <xsl:when test="$tick_num = 1">in</xsl:when>
        <xsl:when test="$tick_num = 2">out</xsl:when>
        <xsl:when test="$tick_num = 3">cross</xsl:when>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="Tick" mode="charts">
    <!-- majorTickMark if not "out" -->
    <xsl:if test="@tktMajor != 2">
      <c:majorTickMark xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="tick_num2tick_attr">
          <xsl:with-param name="tick_num" select="@tktMajor"/>
        </xsl:call-template>
      </c:majorTickMark>
    </xsl:if>
    <!-- minorTickMark  if not "none" -->
    <xsl:if test="@tktMinor != 0">
      <c:minorTickMark xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:call-template name="tick_num2tick_attr">
          <xsl:with-param name="tick_num" select="@tktMinor"/>
        </xsl:call-template>
      </c:minorTickMark>
    </xsl:if>

    <!-- tickLblPos -->
    <xsl:choose>
      <xsl:when test="../../../../CRT/RadarArea/@fRdrAxLab = 'false'">
        <c:tickLblPos val="none" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:otherwise>
        <c:tickLblPos xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:attribute name="val">
            <xsl:choose>
              <xsl:when test="@tlt = 0">none</xsl:when>
              <xsl:when test="@tlt = 1">low</xsl:when>
              <xsl:when test="@tlt = 2">high</xsl:when>
              <xsl:when test="@tlt = 3">nextTo</xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </c:tickLblPos>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="cat_val_txPr">
    <xsl:param name="where"/>

    <xsl:variable name="FontX_pos">
      <xsl:choose>
        <xsl:when test="$where/AXS/FontX">
          <xsl:value-of select="$where/AXS/FontX/@iFont"/>
        </xsl:when>
        <xsl:when test="$where/../../../DFTTEXT[DefaultText/@id = 2]">
          <xsl:value-of select="$where/../../../DFTTEXT[DefaultText/@id = 2]/ATTACHEDLABEL/FontX/@iFont"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="color_ready">
      <xsl:choose>
        <xsl:when test="$where/AXS/Tick">
          <xsl:value-of select="substring($where/AXS/Tick/LongRGB/@argb, 3)"/>
        </xsl:when>
        <xsl:when test="$where/../../../DFTTEXT[DefaultText/@id = 2]">
          <xsl:value-of select="substring($where/../../../DFTTEXT[DefaultText/@id = 2]/ATTACHEDLABEL/Text/LongRGB/@argb, 3)"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if test="$FontX_pos != ''">
      <xsl:variable name="font" select="$FORMATTING/Font[position() = $FontX_pos]"/>
      <xsl:variable name="rot_correction">
        <xsl:choose>
          <xsl:when test="not($where/self::DVAXIS) and $where/../../CRT/Chart3d">-90</xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <c:txPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <a:bodyPr>
          <xsl:variable name="trot">
            <xsl:choose>
              <xsl:when test="not($where/AXS/Tick)">0</xsl:when>
              <xsl:when test="$where/AXS/Tick/@trot = 0">0</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$where/AXS/Tick/@trot + $rot_correction"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:attribute name="rot">
            <xsl:call-template name="angle_b2x">
              <xsl:with-param name="degrees" select="$trot"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="vert">
            <xsl:choose>
              <xsl:when test="$where/AXS/Tick/@rot = 0 or not($where/AXS/Tick)">horz</xsl:when>
              <xsl:when test="$where/AXS/Tick/@rot = 1">wordArtVert</xsl:when>
              <xsl:when test="$where/AXS/Tick/@rot = 2">vert270</xsl:when>
              <xsl:when test="$where/AXS/Tick/@rot = 3">vert</xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </a:bodyPr>
        <a:p>
          <a:pPr>
            <a:defRPr>
              <xsl:call-template name="get_chart_props_from_Font">
                <xsl:with-param name="font" select="$font"/>
                <xsl:with-param name="color_ready" select="$color_ready"/>
              </xsl:call-template>
            </a:defRPr>
          </a:pPr>
        </a:p>
      </c:txPr>

    </xsl:if>
  </xsl:template>

  <xsl:template name="angle_b2x">
    <xsl:param name="degrees"/>
    <xsl:choose>
      <xsl:when test="$degrees &lt;=90">
        <xsl:value-of select="-60000 * $degrees"/>
      </xsl:when>
      <xsl:when test="$degrees > 90 and $degrees &lt;= 180">
        <xsl:value-of select="60000 * ($degrees - 90)"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="SerShared">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:variable name="crt_id" select="SerToCrt/@id"/>
    <xsl:variable name="crt" select="../AXISPARENT/CRT[ChartFormat/@icrt = $crt_id]"/>
    <xsl:variable name="is_linear" select="$crt/Line and not($crt/Chart3d) or $crt/Scatter and $crt/Scatter/@fBubbles = 'false' or $crt/Radar"/>

    <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@yi"/>
    <xsl:variable name="order_id" select="SS/DataFormat[@xi = 65535]/@iss"/>
    <c:idx val="{$order_id}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>

    <c:order val="{$order_id}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>

    <!-- c:tx -->
    <xsl:choose>
      <xsl:when test="AI[1]/BRAI/ChartParsedFormula/@assembled_formula != ''">
        <c:tx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:strRef>
            <xsl:apply-templates select="AI[1]" mode="charts"/>
          </c:strRef>
        </c:tx>
      </xsl:when>
      <xsl:when test="AI[1]/SeriesText/@stText != ''">
        <c:tx xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:v>
            <xsl:value-of select="AI[1]/SeriesText/@stText"/>
          </c:v>
        </c:tx>
      </xsl:when>

    </xsl:choose>

    <!-- c:spPr -->
    <xsl:for-each select="SS[DataFormat/@xi = 65535]">
      <xsl:call-template name="c_spPr">
        <xsl:with-param name="def_settings_idx" select="$order_id"/>
        <xsl:with-param name="is_linear" select="$is_linear"/>
      </xsl:call-template>
    </xsl:for-each>

  </xsl:template>

  <xsl:template match="SERIESFORMAT" mode="trend_series">
    <c:trendline xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:variable name="ser_id" select="SS/DataFormat[@xi = 65535]/@iss"/>

      <xsl:for-each select="SS[DataFormat/@xi = 65535]">
        <xsl:call-template name="c_spPr">
          <xsl:with-param name="def_settings_idx" select="$ser_id"/>
        </xsl:call-template>
      </xsl:for-each>

      <c:trendlineType>
        <xsl:attribute name="val">
          <xsl:choose>
            <xsl:when test="SerAuxTrend/@regt = 0">poly</xsl:when>
            <xsl:when test="SerAuxTrend/@regt = 1">exp</xsl:when>
            <xsl:when test="SerAuxTrend/@regt = 2">log</xsl:when>
            <xsl:when test="SerAuxTrend/@regt = 3">power</xsl:when>
            <xsl:when test="SerAuxTrend/@regt = 4">movingAvg</xsl:when>
          </xsl:choose>
        </xsl:attribute>
      </c:trendlineType>
      <c:order val="{SerAuxTrend/@ordUser}"/>
      <c:dispEq val="{not(not(SerAuxTrend/@fEquation = 'true'))}"/>

      <c:trendlineLbl>
        <c:layout/>
        <xsl:variable name="my_ATTACHEDLABEL" select="../ATTACHEDLABEL[ObjectLink/@wLinkObj = 4 and ObjectLink/@wLinkVar1 = $ser_id]"/>
        <xsl:apply-templates select="$my_ATTACHEDLABEL/FRAME" mode="charts"/>
        <xsl:if test="not($my_ATTACHEDLABEL/FRAME)">
          <c:spPr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
            <a:noFill/>
            <a:ln w="25400">
              <a:noFill/>
            </a:ln>
          </c:spPr>
        </xsl:if>

        <xsl:call-template name="get_chart_props_from_ATTACHEDLABEL">
          <xsl:with-param name="ATTACHEDLABEL" select="$my_ATTACHEDLABEL"/>
          <xsl:with-param name="DFTTEXT" select="../DFTTEXT[DefaultText/@id = 2]"/>
        </xsl:call-template>
      </c:trendlineLbl>

    </c:trendline>
  </xsl:template>
  <xsl:template match="AI" mode="charts">
    <!-- Need to find a way how to distinguish conditions for strRef and numRef -->
    <!--<c:strRef xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">-->
    <c:f xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:value-of select="BRAI/ChartParsedFormula/@assembled_formula"/>
    </c:f>
    <xsl:if test="SeriesText">
      <c:strCache xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <c:ptCount val="1"/>
        <c:pt idx="0">
          <c:v>
            <xsl:value-of select="SeriesText/@stText"/>
          </c:v>
        </c:pt>
      </c:strCache>
    </xsl:if>
    <!--</c:strRef>-->
  </xsl:template>

  <xsl:template match="FRAME" mode="charts">
    <xsl:call-template name="c_spPr"/>
  </xsl:template>

  <xsl:template name="c_spPr">
    <!-- Current node should contain AreaFormat or LineFormat -->
    <xsl:param name="def_settings_idx"/>
    <xsl:param name="is_linear"/>
    <xsl:param name="box3DAxes"/>
    <xsl:if test="AreaFormat or LineFormat or Frame/@frt = 4 or $def_settings_idx != ''">
      <c:spPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

        <xsl:apply-templates select="AreaFormat" mode="charts"/>
        <xsl:if test="(not(AreaFormat) or AreaFormat/@fAuto = 'true') and $def_settings_idx != '' and not($is_linear)">
          <xsl:call-template name="gen_def_fill_color">
            <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
          </xsl:call-template>
        </xsl:if>

        <xsl:choose>
          <!-- 3D walls -->
          <xsl:when test="$box3DAxes != ''">
            <xsl:apply-templates select="AxisLine[@id = 3]" mode="charts"/>
          </xsl:when>
          <!-- Line itself -->
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="AxisLine[@id = 0]">
                <xsl:apply-templates select="AxisLine[@id = 0]" mode="charts"/>
              </xsl:when>
              <xsl:when test="self::SS or self::FRAME">
                <xsl:apply-templates select="LineFormat" mode="charts"/>
                <xsl:if test="(not(LineFormat) or LineFormat/@fAuto = 'true') and $def_settings_idx != ''">
                  <a:ln w="12700">
                    <a:solidFill>
                      <a:srgbClr>
                        <xsl:attribute name="val">
                          <xsl:choose>
                            <xsl:when test="not($is_linear)">
                              <xsl:text>000000</xsl:text>
                            </xsl:when>
                            <xsl:otherwise>
                              <xsl:call-template name="gen_def_ser_rgb_color">
                                <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
                              </xsl:call-template>
                            </xsl:otherwise>
                          </xsl:choose>
                        </xsl:attribute>
                      </a:srgbClr>
                    </a:solidFill>
                    <a:prstDash val="solid"/>
                  </a:ln>
                </xsl:if>
              </xsl:when>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:if test="Frame/@frt = 4 or SerFmt/@fArShadow = 'true' and (not(MarkerFormat) or not($is_linear))">
          <a:effectLst>
            <a:outerShdw dist="35921" dir="2700000" algn="br">
              <a:srgbClr val="000000"/>
            </a:outerShdw>
          </a:effectLst>
        </xsl:if>

      </c:spPr>
    </xsl:if>
  </xsl:template>

  <xsl:template name="c_marker">
    <!-- Current node should contain MarkerFormat -->
    <xsl:param name="def_settings_idx"/>
    <xsl:if test="MarkerFormat or $def_settings_idx != ''">
      <c:marker xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <c:symbol>
          <xsl:choose>
            <xsl:when test="not(MarkerFormat) or MarkerFormat/@fAuto = 'true'">
              <xsl:attribute name="val">
                <xsl:call-template name="gen_def_ser_marker_symbol">
                  <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
                </xsl:call-template>
              </xsl:attribute>
            </xsl:when>

            <xsl:otherwise>
              <xsl:variable name="imk" select="MarkerFormat/@imk"/>
              <xsl:attribute name="val">
                <xsl:choose>
                  <xsl:when test="$imk = 0">none</xsl:when>
                  <xsl:when test="$imk = 1">square</xsl:when>
                  <xsl:when test="$imk = 2">diamond</xsl:when>
                  <xsl:when test="$imk = 3">triangle</xsl:when>
                  <xsl:when test="$imk = 4">x</xsl:when>
                  <xsl:when test="$imk = 5">star</xsl:when>
                  <xsl:when test="$imk = 6">dot</xsl:when>
                  <xsl:when test="$imk = 7">dash</xsl:when>
                  <xsl:when test="$imk = 8">circle</xsl:when>
                  <xsl:when test="$imk = 9">plus</xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:otherwise>

          </xsl:choose>
        </c:symbol>

        <xsl:if test="not(MarkerFormat/@imk = 0)">
          <c:size>
            <xsl:attribute name="val">
              <xsl:choose>
                <xsl:when test="MarkerFormat">
                  <xsl:value-of select="(MarkerFormat/@miSize - MarkerFormat/@miSize mod 20) div 20"/>
                </xsl:when>
                <xsl:otherwise>5</xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </c:size>

          <c:spPr>
            <xsl:if test="not(MarkerFormat/@fNotShowInt = 'true')">
              <a:solidFill>
                <a:srgbClr>
                  <xsl:attribute name="val">
                    <xsl:choose>
                      <xsl:when test="MarkerFormat and MarkerFormat/@fAuto != 'true'">
                        <xsl:value-of select="substring(MarkerFormat/LongRGB[2]/@argb, 3)"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:call-template name="gen_def_ser_rgb_color">
                          <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
                        </xsl:call-template>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                </a:srgbClr>
              </a:solidFill>
            </xsl:if>
            <xsl:if test="not(MarkerFormat/@fNotShowBrd = 'true')">
              <a:ln>
                <a:solidFill>
                  <a:srgbClr>
                    <xsl:attribute name="val">
                      <xsl:choose>
                        <xsl:when test="MarkerFormat and MarkerFormat/@fAuto = 'false'">
                          <xsl:value-of select="substring(MarkerFormat/LongRGB[1]/@argb, 3)"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:call-template name="gen_def_ser_rgb_color">
                            <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
                          </xsl:call-template>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:attribute>
                  </a:srgbClr>
                </a:solidFill>
                <a:prstDash val="solid"/>
              </a:ln>
            </xsl:if>
            <xsl:if test="SerFmt/@fArShadow = 'true'">
              <a:effectLst>
                <a:outerShdw dist="35921" dir="2700000" algn="br">
                  <a:srgbClr val="000000"/>
                </a:outerShdw>
              </a:effectLst>
            </xsl:if>
          </c:spPr>
        </xsl:if>
      </c:marker>
    </xsl:if>
  </xsl:template>

  <xsl:template name="c_dLbls">
    <!-- Current node should contain AttachedLabel -->
    <xsl:param name="order_id"/>
    <xsl:param name="is_area">false</xsl:param>

    <xsl:variable name="my_ATTACHEDLABEL" select="../../ATTACHEDLABEL[ObjectLink/@wLinkObj = 4 and ObjectLink/@wLinkVar1 = $order_id]"/>
    <xsl:variable name="crt_id" select="../SerToCrt/@id"/>
    <xsl:variable name="crt" select="../AXISPARENT/CRT[ChartFormat/@icrt = $crt_id]"/>
    <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents or AttachedLabel or $crt/SS/AttachedLabel">
      <c:dLbls xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <!-- c:spPr -->
        <xsl:choose>
          <xsl:when test="$my_ATTACHEDLABEL/FRAME">
            <xsl:apply-templates select="$my_ATTACHEDLABEL/FRAME" mode="charts"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="$my_ATTACHEDLABEL/Text/@wBkgMode = 2">
              <c:spPr xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
                <a:solidFill>
                  <a:srgbClr val="{substring(../../AXISPARENT[1]/AXES/FRAME/AreaFormat/LongRGB[1]/@argb, 3)}"/>
                </a:solidFill>
                <a:ln w="25400">
                  <a:noFill/>
                </a:ln>
              </c:spPr>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>

        <!-- c:txPr -->
        <xsl:call-template name="get_chart_props_from_ATTACHEDLABEL">
          <xsl:with-param name="ATTACHEDLABEL" select="$my_ATTACHEDLABEL"/>
          <xsl:with-param name="DFTTEXT" select="../../DFTTEXT[DefaultText/@id = 2]"/>
        </xsl:call-template>

        <xsl:if test="$my_ATTACHEDLABEL/Text/@fShowKey = 'true'">
          <c:showLegendKey val="true"/>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="$my_ATTACHEDLABEL/DataLabExtContents">
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@fValue = 'true'">
              <c:showVal val="1"/>
            </xsl:if>
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@fCatName = 'true'">
              <c:showCatName val="1"/>
            </xsl:if>
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@fSerName = 'true'">
              <c:showSerName val="1"/>
            </xsl:if>
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@fPercent = 'true'">
              <c:showPercent val="1"/>
            </xsl:if>
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@fBubSizes = 'true'">
              <c:showBubbleSize val="1"/>
            </xsl:if>
            <xsl:if test="$my_ATTACHEDLABEL/DataLabExtContents/@rgchSep != ''">
              <c:separator>
                <xsl:value-of select="$my_ATTACHEDLABEL/DataLabExtContents/@rgchSep"/>
              </c:separator>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="AttachedLabel">
                <xsl:call-template name="AttachedLabel">
                  <xsl:with-param name="AttachedLabel" select="AttachedLabel"/>
                  <xsl:with-param name="is_area" select="$is_area"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="AttachedLabel">
                  <xsl:with-param name="AttachedLabel" select="$crt/SS/AttachedLabel"/>
                  <xsl:with-param name="is_area" select="$is_area"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>

          </xsl:otherwise>
        </xsl:choose>
        <!-- c:showLeaderLines -->
        <xsl:if test="$crt/Pie/@fShowLdrLines = 'true'">
          <c:showLeaderLines val="true"/>
        </xsl:if>

      </c:dLbls>
    </xsl:if>
  </xsl:template>

  <xsl:template name="AttachedLabel">
    <xsl:param name="AttachedLabel"/>
    <xsl:param name="is_area"/>
    <xsl:choose>
      <xsl:when test="$AttachedLabel/@fShowValue = 'true'">
        <c:showVal val="1" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$AttachedLabel/@fShowLabelAndPerc = 'true'">
        <c:showCatName val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
        <c:showPercent val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$AttachedLabel/@fShowLabel = 'true' and $is_area != 'true'">
        <c:showCatName val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$AttachedLabel/@fShowSeriesName = 'true' or $AttachedLabel/@fShowLabel = 'true' and $is_area = 'true'">
        <c:showSerName val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$AttachedLabel/@fShowPercent = 'true'">
        <c:showPercent val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$AttachedLabel/@fShowBubbleSizes = 'true'">
        <c:showBubbleSize val="true" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="gen_def_ser_rgb_color">
    <xsl:param name="ser_idx"/>
    <xsl:variable name="def_color_id" select="$ser_idx mod 55"/>
    <xsl:choose>
      <xsl:when test="$def_color_id =  0">000080</xsl:when>
      <xsl:when test="$def_color_id =  1">FF00FF</xsl:when>
      <xsl:when test="$def_color_id =  2">FFFF00</xsl:when>
      <xsl:when test="$def_color_id =  3">00FFFF</xsl:when>
      <xsl:when test="$def_color_id =  4">800080</xsl:when>
      <xsl:when test="$def_color_id =  5">800000</xsl:when>
      <xsl:when test="$def_color_id =  6">008080</xsl:when>
      <xsl:when test="$def_color_id =  7">0000FF</xsl:when>
      <xsl:when test="$def_color_id =  8">00CCFF</xsl:when>
      <xsl:when test="$def_color_id =  9">CCFFFF</xsl:when>
      <xsl:when test="$def_color_id = 10">CCFFCC</xsl:when>
      <xsl:when test="$def_color_id = 11">FFFF99</xsl:when>
      <xsl:when test="$def_color_id = 12">99CCFF</xsl:when>
      <xsl:when test="$def_color_id = 13">FF99CC</xsl:when>
      <xsl:when test="$def_color_id = 14">CC99FF</xsl:when>
      <xsl:when test="$def_color_id = 15">FFCC99</xsl:when>
      <xsl:when test="$def_color_id = 16">3366FF</xsl:when>
      <xsl:when test="$def_color_id = 17">33CCCC</xsl:when>
      <xsl:when test="$def_color_id = 18">99CC00</xsl:when>
      <xsl:when test="$def_color_id = 19">FFCC00</xsl:when>
      <xsl:when test="$def_color_id = 20">FF9900</xsl:when>
      <xsl:when test="$def_color_id = 21">FF6600</xsl:when>
      <xsl:when test="$def_color_id = 22">666699</xsl:when>
      <xsl:when test="$def_color_id = 23">969696</xsl:when>
      <xsl:when test="$def_color_id = 24">003366</xsl:when>
      <xsl:when test="$def_color_id = 25">339966</xsl:when>
      <xsl:when test="$def_color_id = 26">003300</xsl:when>
      <xsl:when test="$def_color_id = 27">333300</xsl:when>
      <xsl:when test="$def_color_id = 28">993300</xsl:when>
      <xsl:when test="$def_color_id = 29">993366</xsl:when>
      <xsl:when test="$def_color_id = 30">333399</xsl:when>
      <xsl:when test="$def_color_id = 31">000000</xsl:when>
      <xsl:when test="$def_color_id = 32">FFFFFF</xsl:when>
      <xsl:when test="$def_color_id = 33">FF0000</xsl:when>
      <xsl:when test="$def_color_id = 34">00FF00</xsl:when>
      <xsl:when test="$def_color_id = 35">0000FF</xsl:when>
      <xsl:when test="$def_color_id = 36">FFFF00</xsl:when>
      <xsl:when test="$def_color_id = 37">FF00FF</xsl:when>
      <xsl:when test="$def_color_id = 38">00FFFF</xsl:when>
      <xsl:when test="$def_color_id = 39">800000</xsl:when>
      <xsl:when test="$def_color_id = 40">008000</xsl:when>
      <xsl:when test="$def_color_id = 41">000080</xsl:when>
      <xsl:when test="$def_color_id = 42">808000</xsl:when>
      <xsl:when test="$def_color_id = 43">800080</xsl:when>
      <xsl:when test="$def_color_id = 44">008080</xsl:when>
      <xsl:when test="$def_color_id = 45">C0C0C0</xsl:when>
      <xsl:when test="$def_color_id = 46">808080</xsl:when>
      <xsl:when test="$def_color_id = 47">9999FF</xsl:when>
      <xsl:when test="$def_color_id = 48">993366</xsl:when>
      <xsl:when test="$def_color_id = 49">FFFFCC</xsl:when>
      <xsl:when test="$def_color_id = 50">CCFFFF</xsl:when>
      <xsl:when test="$def_color_id = 51">660066</xsl:when>
      <xsl:when test="$def_color_id = 52">FF8080</xsl:when>
      <xsl:when test="$def_color_id = 53">0066CC</xsl:when>
      <xsl:when test="$def_color_id = 54">CCCCFF</xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="gen_def_fill_color">
    <xsl:param name="ser_idx"/>
    <a:solidFill xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
      <a:srgbClr>
        <xsl:attribute name="val">
          <xsl:variable name="def_color_id" select="$ser_idx mod 56"/>
          <xsl:choose>
            <xsl:when test="$def_color_id =  0">9999FF</xsl:when>
            <xsl:when test="$def_color_id =  1">993366</xsl:when>
            <xsl:when test="$def_color_id =  2">FFFFCC</xsl:when>
            <xsl:when test="$def_color_id =  3">CCFFFF</xsl:when>
            <xsl:when test="$def_color_id =  4">660066</xsl:when>
            <xsl:when test="$def_color_id =  5">FF8080</xsl:when>
            <xsl:when test="$def_color_id =  6">0066CC</xsl:when>
            <xsl:when test="$def_color_id =  7">CCCCFF</xsl:when>
            <xsl:when test="$def_color_id =  8">000080</xsl:when>
            <xsl:when test="$def_color_id =  9">FF00FF</xsl:when>
            <xsl:when test="$def_color_id = 10">FFFF00</xsl:when>
            <xsl:when test="$def_color_id = 11">00FFFF</xsl:when>
            <xsl:when test="$def_color_id = 12">800080</xsl:when>
            <xsl:when test="$def_color_id = 13">800000</xsl:when>
            <xsl:when test="$def_color_id = 14">008080</xsl:when>
            <xsl:when test="$def_color_id = 15">0000FF</xsl:when>
            <xsl:when test="$def_color_id = 16">00CCFF</xsl:when>
            <xsl:when test="$def_color_id = 17">CCFFFF</xsl:when>
            <xsl:when test="$def_color_id = 18">CCFFCC</xsl:when>
            <xsl:when test="$def_color_id = 19">FFFF99</xsl:when>
            <xsl:when test="$def_color_id = 20">99CCFF</xsl:when>
            <xsl:when test="$def_color_id = 21">FF99CC</xsl:when>
            <xsl:when test="$def_color_id = 22">CC99FF</xsl:when>
            <xsl:when test="$def_color_id = 23">FFCC99</xsl:when>
            <xsl:when test="$def_color_id = 24">3366FF</xsl:when>
            <xsl:when test="$def_color_id = 25">33CCCC</xsl:when>
            <xsl:when test="$def_color_id = 26">99CC00</xsl:when>
            <xsl:when test="$def_color_id = 27">FFCC00</xsl:when>
            <xsl:when test="$def_color_id = 28">FF9900</xsl:when>
            <xsl:when test="$def_color_id = 29">FF6600</xsl:when>
            <xsl:when test="$def_color_id = 30">666699</xsl:when>
            <xsl:when test="$def_color_id = 31">969696</xsl:when>
            <xsl:when test="$def_color_id = 32">003366</xsl:when>
            <xsl:when test="$def_color_id = 33">339966</xsl:when>
            <xsl:when test="$def_color_id = 34">003300</xsl:when>
            <xsl:when test="$def_color_id = 35">333300</xsl:when>
            <xsl:when test="$def_color_id = 36">993300</xsl:when>
            <xsl:when test="$def_color_id = 37">993366</xsl:when>
            <xsl:when test="$def_color_id = 38">333399</xsl:when>
            <xsl:when test="$def_color_id = 39">333333</xsl:when>
            <xsl:when test="$def_color_id = 40">000000</xsl:when>
            <xsl:when test="$def_color_id = 41">FFFFFF</xsl:when>
            <xsl:when test="$def_color_id = 42">FF0000</xsl:when>
            <xsl:when test="$def_color_id = 43">00FF00</xsl:when>
            <xsl:when test="$def_color_id = 44">0000FF</xsl:when>
            <xsl:when test="$def_color_id = 45">FFFF00</xsl:when>
            <xsl:when test="$def_color_id = 46">FF00FF</xsl:when>
            <xsl:when test="$def_color_id = 47">00FFFF</xsl:when>
            <xsl:when test="$def_color_id = 48">800000</xsl:when>
            <xsl:when test="$def_color_id = 49">008000</xsl:when>
            <xsl:when test="$def_color_id = 50">000080</xsl:when>
            <xsl:when test="$def_color_id = 51">808000</xsl:when>
            <xsl:when test="$def_color_id = 52">800080</xsl:when>
            <xsl:when test="$def_color_id = 53">008080</xsl:when>
            <xsl:when test="$def_color_id = 54">C0C0C0</xsl:when>
            <xsl:when test="$def_color_id = 55">808080</xsl:when>
          </xsl:choose>
        </xsl:attribute>
      </a:srgbClr>

    </a:solidFill>

  </xsl:template>

  <xsl:template name="gen_def_ser_marker_symbol">
    <xsl:param name="ser_idx"/>
    <xsl:variable name="def_marker_id" select="$ser_idx mod 9"/>
    <xsl:choose>
      <xsl:when test="$def_marker_id =  0">diamond</xsl:when>
      <xsl:when test="$def_marker_id =  1">square</xsl:when>
      <xsl:when test="$def_marker_id =  2">triangle</xsl:when>
      <xsl:when test="$def_marker_id =  3">x</xsl:when>
      <xsl:when test="$def_marker_id =  4">star</xsl:when>
      <xsl:when test="$def_marker_id =  5">circle</xsl:when>
      <xsl:when test="$def_marker_id =  6">plus</xsl:when>
      <xsl:when test="$def_marker_id =  7">dot</xsl:when>
      <xsl:when test="$def_marker_id =  8">dash</xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="AreaFormat" mode="charts">
    <xsl:variable name="options" select="following-sibling::GELFRAME/GelFrame/OfficeArtFOPT/OfficeArtRGFOPTE"/>
    <xsl:choose>
      <xsl:when test="@fAuto = 'true'"/>
      <xsl:when test="@fls = 0">
        <a:noFill xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"/>
      </xsl:when>
      <xsl:when test="@fls = 1">
        <a:solidFill xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
          <a:srgbClr val="{substring(LongRGB[1]/@argb, 3)}"/>
        </a:solidFill>
      </xsl:when>
      <xsl:otherwise>
        <a:pattFill xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
          <xsl:variable name="blip_id" select="$options/fillBlip/OfficeArtBlip/@rgbUid"/>
          <xsl:attribute name="prst">
            <xsl:choose>
              <xsl:when test="@fls = 2">
                <xsl:choose>
                  <xsl:when test="$blip_id = '72E825A24BE5501D2973703A221C4DC3'">pct50</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 3">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'A7CE675CD34FF0790A9DB4ADF7456B91'">pct60</xsl:when>
                  <xsl:when test="$blip_id = 'EBB602B22FEA69678A7EAA685198B759'">pct70</xsl:when>
                  <xsl:when test="$blip_id = '78518046FE6FE2E9D7E2F54A6C2FEE49'">pct75</xsl:when>
                  <xsl:when test="$blip_id = '37CE2E0C698FD2BEC5DF3FFB4F6BCDB9'">pct80</xsl:when>
                  <xsl:when test="$blip_id = 'C7F1B3A91EE811A2BA912BDD07B82EAA'">pct90</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 4">
                <xsl:choose>
                  <xsl:when test="$blip_id = '91B4E677128617BD71D6310E1776D026'">pct25</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 5">
                <xsl:choose>
                  <xsl:when test="$blip_id = '4D2DC32AF41BBC639B68E748C3489FE2'">dkHorz</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 6">
                <xsl:choose>
                  <xsl:when test="$blip_id = '8A36DDA204A001D5F04873D4A0D017A3'">dkVert</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 7">
                <xsl:choose>
                  <xsl:when test="$blip_id = '2E48795DBE1F5F957B608E40A4EDD655'">dkDnDiag</xsl:when>
                  <xsl:when test="$blip_id = '7C6BCD6140E44FF62B06EF38F8C56B94'">wdDnDiag</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 8">
                <xsl:choose>
                  <xsl:when test="$blip_id = '35AA4F63EF7DAEB5D183C7259034E501'">dkUpDiag</xsl:when>
                  <xsl:when test="$blip_id = '24C4453204F8B366595F2CE449962129'">wdUpDiag</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 9">
                <xsl:choose>
                  <xsl:when test="$blip_id = '7417D8913606C4DB4AC6222B7A6361BE'">plaid</xsl:when>
                  <xsl:when test="$blip_id = '3266B8584732F9112E4EE5E6B0BFA9FF'">sphere</xsl:when>
                  <xsl:when test="$blip_id = 'DF804DEE2724F92C7F2CF1D2CC478C8C'">smCheck</xsl:when>
                  <xsl:when test="$blip_id = 'E44A8B4A4454C1262B3EF1A04C8042EB'">lgCheck</xsl:when>
                  <xsl:when test="$blip_id = '3D6DBD0F5267DFDC390592FD774A75B3'">solidDmnd</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 10">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'D4EDE50FEEF42E5B5D2D203DAA27A53E'">trellis</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 11">
                <xsl:choose>
                  <xsl:when test="$blip_id = '96036B0E91D65737847BD47933E5547F'">ltHorz</xsl:when>
                  <xsl:when test="$blip_id = 'EF8DC06D3E805FEF9BECE07A363B2BDC'">narHorz</xsl:when>
                  <xsl:when test="$blip_id = '638EC1655966284849211B3EADA16540'">dashHorz</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 12">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'A4F00B9EC93186AA3D3C2AB649D89C55'">ltVert</xsl:when>
                  <xsl:when test="$blip_id = '0CAE68E81FCB3EF9C8C122BA665B67D2'">narVert</xsl:when>
                  <xsl:when test="$blip_id = '19CDA012DEEFDC13810E4EF00C1A18B7'">dashVert</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 13">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'DC25AD6FA8894FFBC179ADFD799E40B4'">ltDnDiag</xsl:when>
                  <xsl:when test="$blip_id = '24634FFD514D7A3E1F920EF03AADCF8C'">dashDnDiag</xsl:when>
                  <xsl:when test="$blip_id = '60A2E36EFD9678CC18D6BC66089761EB'">zigZag</xsl:when>
                  <xsl:when test="$blip_id = 'C553A1134CBB0D01B1B1A2EE1945C1B5'">wave</xsl:when>
                  <xsl:when test="$blip_id = '8B22812C1A5A5B9EF99F03C84471308B'">diagBrick</xsl:when>
                  <xsl:when test="$blip_id = 'A9F1B40CC0E77E1B313E7550D4C1BD2C'">dotGrid</xsl:when>
                  <xsl:when test="$blip_id = 'DB50DD3025C5B830EAB7A2C36F952725'">shingle</xsl:when>
                  <xsl:when test="$blip_id = '4CD208B5B461BC5D3087977CA5D1E772'">openDmnd</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 14">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'A639435F6979029D75EB13D11555FFEE'">ltUpDiag</xsl:when>
                  <xsl:when test="$blip_id = 'F8539AEDF0F8AC75891EED9FA34E5CD6'">dashUpDiag</xsl:when>
                  <xsl:when test="$blip_id = 'BD05F4D491ABF65D063EC2D4D15D4601'">weave</xsl:when>
                  <xsl:when test="$blip_id = 'E9D531BFEA0FA026539D9FCFF0C915DB'">divot</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 15">
                <xsl:choose>
                  <xsl:when test="$blip_id = '2533C3A0C652FB96484D923BE8E32286'">horzBrick</xsl:when>
                  <xsl:when test="$blip_id = '7C9854BD173EA0E8F3F8FDCC7F8CBAE6'">smGrid</xsl:when>
                  <xsl:when test="$blip_id = 'C17273E0E576C2D1FC972A960922DB9A'">lgGrid</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 16">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'DD0EE2DDDFBFDF9D586FF1E7F20EB998'">pct30</xsl:when>
                  <xsl:when test="$blip_id = 'FFB5901231A112951BEBDA01E1A47526'">pct40</xsl:when>
                  <xsl:when test="$blip_id = 'C97975BD80FF1B4998D398719410CC01'">lgConfetti</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 17">
                <xsl:choose>
                  <xsl:when test="$blip_id = '046607154C3412C2B34031D1F2E48100'">pct20</xsl:when>
                  <xsl:when test="$blip_id = 'C7CFE6FF485FF6A41392C5FD082DEE6E'">smConfetti</xsl:when>
                  <xsl:when test="$blip_id = '42A68C0980E78CA0B89D974BE85E5CDD'">dotDmnd</xsl:when>
                </xsl:choose>
              </xsl:when>
              <xsl:when test="@fls = 18">
                <xsl:choose>
                  <xsl:when test="$blip_id = 'E442EF5D22B2E382493C396B02BBDF5F'">pct5</xsl:when>
                  <xsl:when test="$blip_id = '993378FC217456C324CE01A08E64ECBB'">pct10</xsl:when>
                </xsl:choose>
              </xsl:when>
            </xsl:choose>
          </xsl:attribute>
          <a:fgClr>
            <a:srgbClr val="{substring(LongRGB[1]/@argb, 3)}"/>
          </a:fgClr>
          <a:bgClr>
            <a:srgbClr val="{substring(LongRGB[2]/@argb, 3)}"/>
          </a:bgClr>
        </a:pattFill>

      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match="LineFormat" mode="charts">
    <xsl:if test="@fAuto != 'true'">
      <a:ln xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <!-- if @we = -1 (Hairline) then @w = 0 and omitted -->
        <xsl:if test="@we >= 0">
          <xsl:attribute name="w">
            <xsl:value-of select="12700 * (@we + 1)"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="@lns = 5">
            <a:noFill/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="@lns = 6 or @lns = 7 or @lns = 8">
                <a:pattFill>
                  <xsl:attribute name="prst">
                    <xsl:choose>
                      <xsl:when test="@lns = 6">pct75</xsl:when>
                      <xsl:when test="@lns = 7">pct50</xsl:when>
                      <xsl:when test="@lns = 8">pct25</xsl:when>
                    </xsl:choose>
                  </xsl:attribute>
                  <a:fgClr>
                    <a:srgbClr val="{substring(LongRGB/@argb, 3)}"/>
                  </a:fgClr>
                  <a:bgClr>
                    <a:srgbClr val="FFFFFF"/>
                  </a:bgClr>
                </a:pattFill>
              </xsl:when>
              <xsl:when test="@fAutoCo = 'true'">
                <a:solidFill>
                  <a:srgbClr val="000000"/>
                </a:solidFill>
              </xsl:when>
              <xsl:otherwise>
                <a:solidFill>
                  <a:srgbClr val="{substring(LongRGB/@argb, 3)}"/>
                </a:solidFill>
              </xsl:otherwise>
            </xsl:choose>
            <a:prstDash>
              <xsl:attribute name="val">
                <xsl:choose>
                  <xsl:when test="@lns = 0">solid</xsl:when>
                  <xsl:when test="@lns = 1">dash</xsl:when>
                  <xsl:when test="@lns = 2">sysDash</xsl:when>
                  <xsl:when test="@lns = 3">sysDashDot</xsl:when>
                  <xsl:when test="@lns = 4">sysDashDotDot</xsl:when>

                  <xsl:when test="@lns = 6">solid</xsl:when>
                  <xsl:when test="@lns = 7">solid</xsl:when>
                  <xsl:when test="@lns = 8">solid</xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </a:prstDash>
          </xsl:otherwise>

        </xsl:choose>
      </a:ln>
    </xsl:if>
  </xsl:template>

  <xsl:template match="AxisLine" mode="charts">
    <xsl:variable name="next_LineFormat" select="following-sibling::*[position() = 1 and name() = 'LineFormat']"/>
    <xsl:variable name="prev_AxisLine" select="preceding-sibling::*[position() = 1 and name() = 'AxisLine']"/>
    <xsl:if test="$next_LineFormat">
      <xsl:choose>
        <xsl:when test="@id = 0 or @id = 3">
          <xsl:apply-templates select="$next_LineFormat" mode="charts"/>
          <xsl:if test="$next_LineFormat/@fAuto = 'true'">
            <a:ln w="12700" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
              <a:solidFill>
                <a:srgbClr val="000000"/>
              </a:solidFill>
              <a:prstDash val="solid"/>
            </a:ln>
          </xsl:if>
        </xsl:when>
        <xsl:when test="@id = 1">
          <c:majorGridlines xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
            <c:spPr>
              <xsl:apply-templates select="$next_LineFormat" mode="charts"/>
              <xsl:if test="$next_LineFormat/@fAuto = 'true'">
                <a:ln w="3175">
                  <a:solidFill>
                    <a:srgbClr val="000000"/>
                  </a:solidFill>
                  <a:prstDash val="solid"/>
                </a:ln>
              </xsl:if>
            </c:spPr>
          </c:majorGridlines>
        </xsl:when>
        <xsl:when test="@id = 2">
          <c:minorGridlines xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
            <c:spPr>
              <xsl:apply-templates select="$next_LineFormat" mode="charts"/>
              <xsl:if test="$next_LineFormat/@fAuto = 'true'">
                <a:ln w="3175">
                  <a:solidFill>
                    <a:srgbClr val="000000"/>
                  </a:solidFill>
                  <a:prstDash val="solid"/>
                </a:ln>
              </xsl:if>
            </c:spPr>
          </c:minorGridlines>
        </xsl:when>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template name="get_chart_props_from_ATTACHEDLABEL">
    <xsl:param name="ATTACHEDLABEL"/>
    <xsl:param name="DFTTEXT"/>


    <xsl:variable name="FontX_pos">
      <xsl:choose>
        <xsl:when test="$ATTACHEDLABEL/FontX">
          <xsl:value-of select="$ATTACHEDLABEL/FontX/@iFont"/>
        </xsl:when>
        <xsl:when test="$DFTTEXT">
          <xsl:value-of select="$DFTTEXT/ATTACHEDLABEL/FontX/@iFont"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="color_ready">
      <xsl:choose>
        <xsl:when test="$ATTACHEDLABEL">
          <xsl:value-of select="substring($ATTACHEDLABEL/Text/LongRGB/@argb, 3)"/>
        </xsl:when>
        <xsl:when test="$DFTTEXT">
          <xsl:value-of select="substring($DFTTEXT/ATTACHEDLABEL/Text/LongRGB/@argb, 3)"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:if test="$FontX_pos != ''">
      <xsl:variable name="font" select="$FORMATTING/Font[position() = $FontX_pos]"/>

      <c:txPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
        <a:bodyPr>
          <xsl:if test="$ATTACHEDLABEL/Text/@trot != 0">
            <xsl:attribute name="rot">
              <xsl:call-template name="angle_b2x">
                <xsl:with-param name="degrees" select="$ATTACHEDLABEL/Text/@trot"/>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:if>
        </a:bodyPr>
        <a:p>
          <a:pPr>
            <a:defRPr>
              <xsl:call-template name="get_chart_props_from_Font">
                <xsl:with-param name="font" select="$font"/>
                <xsl:with-param name="color_ready" select="$color_ready"/>
              </xsl:call-template>
            </a:defRPr>
          </a:pPr>
        </a:p>
      </c:txPr>

    </xsl:if>
  </xsl:template>

  <xsl:template match="LD" mode="charts">
    <c:legend xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

      <c:legendPos>
        <xsl:attribute name="val">
          <xsl:choose>
            <xsl:when test="Legend/@fVert = 'true'">
              <xsl:choose>
                <xsl:when test="Legend/@x &lt; 4000 - (Legend/@x + Legend/@dx)">l</xsl:when>
                <!--<xsl:when test="Legend/@y > 2000 - (Legend/@y + Legend/@dy div 2)">r</xsl:when>
                <xsl:otherwise>tr</xsl:otherwise>-->
                <xsl:otherwise>r</xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="Legend/@y &lt; 4000 - (Legend/@y + Legend/@dy)">t</xsl:when>
                <xsl:otherwise>b</xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </c:legendPos>

      <c:layout>
        <c:manualLayout>
          <c:xMode val="edge"/>
          <c:yMode val="edge"/>
          <c:x val="{Legend/@x div 4000}"/>
          <c:y val="{Legend/@y div 4000}"/>
        </c:manualLayout>
      </c:layout>

      <xsl:apply-templates select="FRAME" mode="charts"/>
      <xsl:if test="not(FRAME)">
        <c:spPr>
          <a:solidFill>
            <a:srgbClr val="FFFFFF"/>
          </a:solidFill>
          <a:ln w="3175">
            <a:solidFill>
              <a:srgbClr val="000000"/>
            </a:solidFill>
            <a:prstDash val="solid"/>
          </a:ln>
        </c:spPr>
      </xsl:if>

      <xsl:call-template name="get_chart_props_from_ATTACHEDLABEL">
        <xsl:with-param name="ATTACHEDLABEL" select="ATTACHEDLABEL"/>
        <xsl:with-param name="DFTTEXT" select="../../../DFTTEXT[DefaultText/@id = 2]"/>
      </xsl:call-template>


    </c:legend>
  </xsl:template>

  <xsl:template name="_3D_plane_axes">
    <!-- Current node is ChartSheetSubstream -->
    <xsl:variable name="Chart3d" select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/CRT/Chart3d"/>
    <xsl:if test="$Chart3d">
      <c:view3D xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <!-- c:rotX -->
        <xsl:if test="$Chart3d/@anElev != 0">
          <c:rotX val="{$Chart3d/@anElev}"/>
        </xsl:if>
        <!-- c:hPercent -->
        <xsl:if test="$Chart3d/@pcHeight != 100">
          <c:hPercent val="{$Chart3d/@pcHeight}"/>
        </xsl:if>
        <!-- c:rotY -->
        <c:rotY val="{$Chart3d/@anRot}"/>
        <!-- c:depthPercent -->
        <c:depthPercent val="{$Chart3d/@pcDepth}"/>
        <!-- c:rAngAx, c:perspective -->
        <xsl:choose>
          <xsl:when test="$Chart3d/@fPerspective = 'false'">
            <c:rAngAx val="true"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="$Chart3d/@pcDist != 30">
              <c:perspective val="{$Chart3d/@pcDist}"/>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>

      </c:view3D>

      <c:floor xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:for-each select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/AXES/DVAXIS/AXS">
          <xsl:call-template name="c_spPr">
            <xsl:with-param name="box3DAxes">true</xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </c:floor>
      <c:sideWall xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:for-each select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/AXES/IVAXIS/AXS">
          <xsl:call-template name="c_spPr"/>
        </xsl:for-each>
      </c:sideWall>
      <c:backWall xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <xsl:for-each select="CHARTFOMATS/AXISPARENT[AxisParent/@iax = 'false']/AXES/IVAXIS/AXS">
          <xsl:call-template name="c_spPr"/>
        </xsl:for-each>
      </c:backWall>
    </xsl:if>
  </xsl:template>

  <xsl:template match="Chart3DBarShape" mode="charts">
    <c:shape xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <xsl:attribute name="val">
        <xsl:choose>
          <!-- rectangle -->
          <xsl:when test="@riser = 0">
            <xsl:choose>
              <xsl:when test="@taper = 0">box</xsl:when>
              <xsl:when test="@taper = 1">pyramid</xsl:when>
              <xsl:when test="@taper = 2">pyramidToMax</xsl:when>
            </xsl:choose>
          </xsl:when>
          <!-- circular -->
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="@taper = 0">cylinder</xsl:when>
              <xsl:when test="@taper = 1">cone</xsl:when>
              <xsl:when test="@taper = 2">coneToMax</xsl:when>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </c:shape>
  </xsl:template>

  <xsl:template name="gen_dPts_cycled">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="SS_special"/>
    <xsl:param name="vary_colors_for_filling_shapes"/>
    <xsl:param name="vary_colors_for_lines"/>
    <xsl:param name="vary_marker"/>
    <xsl:param name="SSs"/>
    <xsl:param name="SSa"/>
    <xsl:param name="AreaFormat_definition_location_sa"/>
    <xsl:param name="LineFormat_definition_location_sa"/>
    <xsl:param name="MarkerFormat_definition_location_sa"/>
    <xsl:param name="is_linear"/>
    <xsl:param name="pt_index">0</xsl:param>
    <xsl:param name="count"/>

    <!-- Cycle condition -->
    <xsl:if test="$pt_index &lt; $count">

      <xsl:variable name="mySS" select="SS[DataFormat/@xi = $pt_index]"/>
      <xsl:variable name="AreaFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="SS[DataFormat/@xi = $pt_index]"/>
          <xsl:with-param name="SSs" select="$SSs"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">AreaFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="LineFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="$mySS"/>
          <xsl:with-param name="SSs" select="$SSs"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">LineFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="MarkerFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="$mySS"/>
          <xsl:with-param name="SSs" select="$SSs"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">MarkerFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:choose>
        <!-- If we have SS for this point then generate it from SS -->
        <xsl:when test="$mySS">
          <xsl:call-template name="gen_dPt_from_SS">
            <xsl:with-param name="SSp" select="$mySS"/>
            <xsl:with-param name="SSs" select="$SSs"/>
            <xsl:with-param name="SSa" select="$SSa"/>
            <xsl:with-param name="is_linear" select="$is_linear"/>
          </xsl:call-template>
        </xsl:when>
        <!-- Otherwise generate the point from nothing just by index -->
        <xsl:when test="$vary_colors_for_filling_shapes or $vary_colors_for_lines">
          <xsl:call-template name="gen_dPt_by_index">
            <xsl:with-param name="index" select="$pt_index"/>
            <xsl:with-param name="vary_colors_for_filling_shapes" select="$vary_colors_for_filling_shapes"/>
            <xsl:with-param name="vary_colors_for_lines" select="$vary_colors_for_lines"/>
            <xsl:with-param name="vary_marker" select="$vary_marker"/>
            <xsl:with-param name="AreaFormat_definition_location_sa" select="$AreaFormat_definition_location_sa"/>
            <xsl:with-param name="LineFormat_definition_location_sa" select="$LineFormat_definition_location_sa"/>
            <xsl:with-param name="MarkerFormat_definition_location_sa" select="$MarkerFormat_definition_location_sa"/>
            <xsl:with-param name="SSs" select="$SSs"/>
            <xsl:with-param name="SSa" select="$SSa"/>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>

      <!-- Continue cycle -->
      <xsl:call-template name="gen_dPts_cycled">
        <xsl:with-param name="SS_special" select="$SS_special"/>
        <xsl:with-param name="vary_colors_for_filling_shapes" select="$vary_colors_for_filling_shapes"/>
        <xsl:with-param name="vary_colors_for_lines" select="$vary_colors_for_lines"/>
        <xsl:with-param name="vary_marker" select="$vary_marker"/>
        <xsl:with-param name="SSs" select="$SSs"/>
        <xsl:with-param name="SSa" select="$SSa"/>
        <xsl:with-param name="AreaFormat_definition_location_sa" select="$AreaFormat_definition_location_sa"/>
        <xsl:with-param name="LineFormat_definition_location_sa" select="$LineFormat_definition_location_sa"/>
        <xsl:with-param name="MarkerFormat_definition_location_sa" select="$MarkerFormat_definition_location_sa"/>
        <xsl:with-param name="is_linear" select="$is_linear"/>
        <xsl:with-param name="pt_index" select="$pt_index + 1"/>
        <xsl:with-param name="count" select="$count"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="gen_dPt_from_SS">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="SSp"/>
    <xsl:param name="SSs"/>
    <xsl:param name="SSa"/>
    <xsl:param name="is_linear"/>
    <c:dPt xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
      <xsl:variable name="point_idx" select="$SSp/DataFormat/@xi"/>
      <xsl:variable name="ser_order" select="$SSs/DataFormat[@xi = 65535]/@iss"/>
      <xsl:variable name="crt" select="$SSa/.."/>
      <xsl:variable name="def_settings_idx">
        <xsl:choose>
          <xsl:when test="$crt/ChartFormat/@fVaried = 'true'">
            <xsl:value-of select="$point_idx"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$ser_order"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <c:idx val="{$point_idx}"/>

      <xsl:variable name="AreaFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="$SSp"/>
          <xsl:with-param name="SSs" select="$SSp"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">AreaFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="LineFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="$SSp"/>
          <xsl:with-param name="SSs" select="$SSs"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">LineFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="MarkerFormat_definition_location">
        <xsl:call-template name="determine_format_definition_place">
          <xsl:with-param name="SSp" select="$SSp"/>
          <xsl:with-param name="SSs" select="$SSs"/>
          <xsl:with-param name="SSa" select="$SSa"/>
          <xsl:with-param name="tag_name">MarkerFormat</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <!-- c:marker -->
      <xsl:if test="$is_linear">
        <xsl:choose>
          <xsl:when test="$MarkerFormat_definition_location = 'p'">
            <xsl:for-each select="$SSp">
              <xsl:call-template name="c_marker"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$MarkerFormat_definition_location = 's'">
            <xsl:for-each select="$SSs">
              <xsl:call-template name="c_marker"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$MarkerFormat_definition_location = 'a'">
            <xsl:for-each select="$SSa">
              <xsl:call-template name="c_marker"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$MarkerFormat_definition_location = 'auto'">
            <!-- Current node is SERIESFORMAT so there is no chance to get real data -->
            <xsl:call-template name="c_marker">
              <xsl:with-param name="def_settings_idx" select="$def_settings_idx"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:if>

      <!-- c:spPr -->
      <xsl:if test="$AreaFormat_definition_location != 'auto' or $LineFormat_definition_location != 'auto' or not($is_linear)">
        <c:spPr xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">

          <xsl:choose>
            <xsl:when test="$AreaFormat_definition_location = 'p'">
              <xsl:apply-templates select="$SSp/AreaFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$AreaFormat_definition_location = 's'">
              <xsl:apply-templates select="$SSs/AreaFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$AreaFormat_definition_location = 'a'">
              <xsl:apply-templates select="$SSa/AreaFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$AreaFormat_definition_location = 'auto' and not($is_linear)">
              <xsl:call-template name="gen_def_fill_color">
                <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
              </xsl:call-template>
            </xsl:when>
          </xsl:choose>

          <xsl:choose>
            <xsl:when test="$LineFormat_definition_location = 'p'">
              <xsl:apply-templates select="$SSp/LineFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$LineFormat_definition_location = 's'">
              <xsl:apply-templates select="$SSs/LineFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$LineFormat_definition_location = 'a'">
              <xsl:apply-templates select="$SSa/LineFormat" mode="charts"/>
            </xsl:when>
            <xsl:when test="$LineFormat_definition_location = 'auto'">
              <a:ln w="12700">
                <a:solidFill>
                  <a:srgbClr>
                    <xsl:attribute name="val">
                      <xsl:choose>
                        <xsl:when test="not($is_linear)">
                          <xsl:text>000000</xsl:text>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:call-template name="gen_def_ser_rgb_color">
                            <xsl:with-param name="ser_idx" select="$def_settings_idx"/>
                          </xsl:call-template>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:attribute>
                  </a:srgbClr>
                </a:solidFill>
                <a:prstDash val="solid"/>
              </a:ln>
            </xsl:when>
          </xsl:choose>

        </c:spPr>
      </xsl:if>

      <!-- c:bubble3D -->
      <xsl:if test="$SSp/SerFmt">
        <c:bubble3D val="{$SSp/SerFmt/@f3DBubbles}"/>
      </xsl:if>

      <!-- c:explosion -->
      <xsl:if test="$SSp/PieFormat/@pcExplode != 0">
        <c:explosion val="{$SSp/PieFormat/@pcExplode}"/>
      </xsl:if>

    </c:dPt>
  </xsl:template>

  <xsl:template name="gen_dPt_by_index">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="index"/>
    <xsl:param name="vary_colors_for_filling_shapes"/>
    <xsl:param name="vary_colors_for_lines"/>
    <xsl:param name="vary_marker"/>
    <xsl:param name="AreaFormat_definition_location_sa"/>
    <xsl:param name="LineFormat_definition_location_sa"/>
    <xsl:param name="MarkerFormat_definition_location_sa"/>
    <xsl:param name="SSs"/>
    <xsl:param name="SSa"/>

    <c:dPt xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart" xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main">
      <c:idx val="{$index}"/>


      <!-- c:marker -->
      <xsl:if test="$vary_marker">
        <xsl:call-template name="c_marker">
          <xsl:with-param name="def_settings_idx" select="$index"/>
        </xsl:call-template>
      </xsl:if>

      <c:spPr>
        <xsl:if test="$vary_colors_for_filling_shapes">
          <xsl:call-template name="gen_def_fill_color">
            <xsl:with-param name="ser_idx" select="$index"/>
          </xsl:call-template>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="$vary_colors_for_lines">
            <a:ln w="12700">
              <a:solidFill>
                <a:srgbClr>
                  <xsl:attribute name="val">
                    <xsl:call-template name="gen_def_ser_rgb_color">
                      <xsl:with-param name="ser_idx" select="$index"/>
                    </xsl:call-template>
                  </xsl:attribute>
                </a:srgbClr>
              </a:solidFill>
              <a:prstDash val="solid"/>
            </a:ln>
          </xsl:when>
          <xsl:when test="$LineFormat_definition_location_sa = 's'">
            <xsl:apply-templates select="$SSs/LineFormat" mode="charts"/>
          </xsl:when>
          <xsl:when test="$LineFormat_definition_location_sa = 'a'">
            <xsl:apply-templates select="$SSa/LineFormat" mode="charts"/>
          </xsl:when>
          <xsl:when test="$LineFormat_definition_location_sa = 'auto'">
            <a:ln w="12700">
              <a:solidFill>
                <a:srgbClr val="000000"/>
              </a:solidFill>
              <a:prstDash val="solid"/>
            </a:ln>
          </xsl:when>
        </xsl:choose>
      </c:spPr>

    </c:dPt>
  </xsl:template>

  <xsl:template match="CrtLine" mode="charts">
    <xsl:variable name="next_LineFormat" select="following-sibling::*[position() = 1 and name() = 'LineFormat']"/>
    <xsl:if test="$next_LineFormat">
      <c:serLines xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <c:spPr>
          <xsl:apply-templates select="$next_LineFormat" mode="charts"/>
        </c:spPr>
      </c:serLines>
    </xsl:if>
  </xsl:template>

  <xsl:template name="cat_val">
    <!-- Current node must be SERIESFORMAT -->

    <!-- c:cat -->
    <xsl:choose>
      <xsl:when test="AI[3]/BRAI/ChartParsedFormula/@assembled_formula != ''">
        <c:cat xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:choose>
            <xsl:when test="Series/@sdtX = 1">
              <c:numRef>
                <xsl:apply-templates select="AI[3]" mode="charts"/>
              </c:numRef>
            </xsl:when>
            <xsl:when test="Series/@sdtX = 3">
              <c:strRef>
                <xsl:apply-templates select="AI[3]" mode="charts"/>
              </c:strRef>
            </xsl:when>
          </xsl:choose>
        </c:cat>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="check">
          <xsl:call-template name="check_cat"/>
        </xsl:variable>
        <xsl:if test="$check != ''">
          <c:cat xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
            <xsl:call-template name="get_cache_values_for_SERIES">
              <!-- Category labels or horizontal values -->
              <xsl:with-param name="value_type">2</xsl:with-param>
            </xsl:call-template>
          </c:cat>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

    <!-- c:val -->
    <xsl:choose>
      <xsl:when test="AI[2]/BRAI/ChartParsedFormula/@assembled_formula != ''">
        <c:val xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:numRef>
            <xsl:apply-templates select="AI[2]" mode="charts"/>
          </c:numRef>
        </c:val>
      </xsl:when>
      <xsl:otherwise>
        <c:val xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:call-template name="get_cache_values_for_SERIES">
            <!-- Series values or vertical values -->
            <xsl:with-param name="value_type">1</xsl:with-param>
          </xsl:call-template>
        </c:val>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="check_cat">
    <xsl:variable name="series_id" select="count(preceding-sibling::SERIESFORMAT)"/>

    <xsl:call-template name="get_cache_value_for_SERIES">
      <xsl:with-param name="series_id" select="$series_id"/>
      <xsl:with-param name="value_id">0</xsl:with-param>
      <xsl:with-param name="value_type">2</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="check_val">
    <xsl:variable name="series_id" select="count(preceding-sibling::SERIESFORMAT)"/>

    <xsl:call-template name="get_cache_value_for_SERIES">
      <xsl:with-param name="series_id" select="$series_id"/>
      <xsl:with-param name="value_id">0</xsl:with-param>
      <xsl:with-param name="value_type">1</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="check_bub">
    <xsl:variable name="series_id" select="count(preceding-sibling::SERIESFORMAT)"/>

    <xsl:call-template name="get_cache_value_for_SERIES">
      <xsl:with-param name="series_id" select="$series_id"/>
      <xsl:with-param name="value_id">0</xsl:with-param>
      <xsl:with-param name="value_type">3</xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="get_cache_values_for_SERIES">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="value_type"/>

    <xsl:variable name="num_points">
      <xsl:choose>
        <!-- Category labels or horizontal values -->
        <xsl:when test="$value_type = 2">
          <xsl:value-of select="Series/@cValx"/>
        </xsl:when>
        <!-- Series values or vertical values -->
        <xsl:when test="$value_type = 1">
          <xsl:value-of select="Series/@cValy"/>
        </xsl:when>
        <!-- Bubble sizes -->
        <xsl:when test="$value_type = 3">
          <xsl:value-of select="Series/@cValBSize"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    <xsl:variable name="series_id" select="count(preceding-sibling::SERIESFORMAT)"/>

    <xsl:choose>
      <xsl:when test="Series/@sdtX = 1 or $value_type != 2">
        <c:numLit xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:formatCode>General</c:formatCode>
          <c:ptCount val="{$num_points}"/>
          <xsl:call-template name="get_cache_values_for_SERIES_helper">
            <xsl:with-param name="series_id" select="$series_id"/>
            <xsl:with-param name="num_points" select="$num_points"/>
            <xsl:with-param name="value_type" select="$value_type"/>
          </xsl:call-template>
        </c:numLit>
      </xsl:when>
      <xsl:when test="Series/@sdtX = 3">
        <c:strLit xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:ptCount val="{$num_points}"/>
          <xsl:call-template name="get_cache_values_for_SERIES_helper">
            <xsl:with-param name="series_id" select="$series_id"/>
            <xsl:with-param name="num_points" select="$num_points"/>
            <xsl:with-param name="value_type" select="$value_type"/>
          </xsl:call-template>
        </c:strLit>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="get_cache_values_for_SERIES_helper">
    <xsl:param name="series_id"/>
    <xsl:param name="value_id">0</xsl:param>
    <xsl:param name="num_points"/>
    <xsl:param name="value_type"/>

    <xsl:if test="$value_id &lt; $num_points">
      <c:pt idx="{$value_id}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
        <c:v>
          <xsl:call-template name="get_cache_value_for_SERIES">
            <xsl:with-param name="series_id" select="$series_id"/>
            <xsl:with-param name="value_id" select="$value_id"/>
            <xsl:with-param name="value_type" select="$value_type"/>
          </xsl:call-template>
        </c:v>
      </c:pt>

      <xsl:call-template name="get_cache_values_for_SERIES_helper">
        <xsl:with-param name="series_id" select="$series_id"/>
        <xsl:with-param name="num_points" select="$num_points"/>
        <xsl:with-param name="value_type" select="$value_type"/>
        <xsl:with-param name="value_id" select="$value_id + 1"/>
      </xsl:call-template>

    </xsl:if>
  </xsl:template>

  <xsl:template name="get_cache_value_for_SERIES">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="series_id"/>
    <xsl:param name="value_id"/>
    <xsl:param name="value_type"/>

    <xsl:variable name="ref" select="concat(key('col2ref', $series_id)/@ref, ($value_id + 1))"/>

    <xsl:variable name="value_container" select="../../SERIESDATA/*[Cell/@ref = $ref and preceding-sibling::SIIndex[1]/@numIndex = $value_type]"/>

    <xsl:choose>
      <xsl:when test="$value_container/self::Number">
        <xsl:value-of select="$value_container/@num"/>
      </xsl:when>
      <xsl:when test="$value_container/self::BoolErr">
        <xsl:value-of select="$value_container/Bes/@value"/>
      </xsl:when>
      <!--<xsl:when test="$value_container/self::Blank"/>-->
      <xsl:when test="$value_container/self::Label">
        <xsl:value-of select="$value_container/@st"/>
      </xsl:when>
      <xsl:when test="not($value_container) and $value_type = 3">0</xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="c_dPt">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="crt"/>
    <xsl:param name="is_linear"/>

    <xsl:variable name="SS_special" select="SS[DataFormat/@xi != 65535]"/>
    <xsl:variable name="SS_series" select="SS[DataFormat/@xi = 65535]"/>
    <xsl:variable name="SS_axis" select="$crt/SS"/>
    <xsl:variable name="AreaFormat_definition_location_sa">
      <xsl:call-template name="determine_format_definition_place">
        <xsl:with-param name="SSs" select="$SS_series"/>
        <xsl:with-param name="SSa" select="$SS_axis"/>
        <xsl:with-param name="tag_name">AreaFormat</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="LineFormat_definition_location_sa">
      <xsl:call-template name="determine_format_definition_place">
        <xsl:with-param name="SSs" select="$SS_series"/>
        <xsl:with-param name="SSa" select="$SS_axis"/>
        <xsl:with-param name="tag_name">LineFormat</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="MarkerFormat_definition_location_sa">
      <xsl:call-template name="determine_format_definition_place">
        <xsl:with-param name="SSs" select="$SS_series"/>
        <xsl:with-param name="SSa" select="$SS_axis"/>
        <xsl:with-param name="tag_name">MarkerFormat</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="vary" select="$crt/ChartFormat/@fVaried = 'true'"/>
    <xsl:variable name="vary_fill" select="$vary and $AreaFormat_definition_location_sa = 'auto' and not($is_linear)"/>
    <xsl:variable name="vary_marker" select="$vary and $MarkerFormat_definition_location_sa = 'auto' and $is_linear"/>
    <xsl:variable name="vary_line" select="$vary and $LineFormat_definition_location_sa = 'auto' and $is_linear"/>
    <xsl:variable name="need_dPts_at_all" select="$vary_fill or $vary_marker or $vary_line or SS[DataFormat/@xi != 65535]"/>
    <xsl:if test="$need_dPts_at_all">





      <xsl:call-template name="gen_dPts_cycled">
        <xsl:with-param name="SS_special" select="$SS_special"/>
        <xsl:with-param name="vary_colors_for_filling_shapes" select="$vary_fill"/>
        <xsl:with-param name="vary_colors_for_lines" select="$vary_line"/>
        <xsl:with-param name="vary_marker" select="$vary_marker"/>
        <xsl:with-param name="SSs" select="$SS_series"/>
        <xsl:with-param name="SSa" select="$SS_axis"/>
        <xsl:with-param name="AreaFormat_definition_location_sa" select="$AreaFormat_definition_location_sa"/>
        <xsl:with-param name="LineFormat_definition_location_sa" select="$LineFormat_definition_location_sa"/>
        <xsl:with-param name="MarkerFormat_definition_location_sa" select="$MarkerFormat_definition_location_sa"/>
        <xsl:with-param name="is_linear" select="$is_linear"/>
        <xsl:with-param name="count" select="Series/@cValy"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="xVal_yVal_bubbleSize">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="bubbles"/>

    <!-- c:xVal -->
    <xsl:choose>
      <xsl:when test="AI[3]/BRAI/ChartParsedFormula/@assembled_formula != ''">
        <c:xVal xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <xsl:choose>
            <xsl:when test="Series/@sdtX = 1">
              <c:numRef>
                <xsl:apply-templates select="AI[3]" mode="charts"/>
              </c:numRef>
            </xsl:when>
            <xsl:when test="Series/@sdtX = 3">
              <c:strRef>
                <xsl:apply-templates select="AI[3]" mode="charts"/>
              </c:strRef>
            </xsl:when>
          </xsl:choose>
        </c:xVal>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="check">
          <xsl:call-template name="check_cat"/>
        </xsl:variable>
        <xsl:if test="$check != ''">
          <c:xVal xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
            <xsl:call-template name="get_cache_values_for_SERIES">
              <!-- Category labels or horizontal values -->
              <xsl:with-param name="value_type">2</xsl:with-param>
            </xsl:call-template>
          </c:xVal>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

    <!-- c:yVal -->
    <xsl:choose>
      <xsl:when test="AI[2]/BRAI/ChartParsedFormula/@assembled_formula != ''">
        <c:yVal xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
          <c:numRef>
            <xsl:apply-templates select="AI[2]" mode="charts"/>
          </c:numRef>
        </c:yVal>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="check">
          <xsl:call-template name="check_val"/>
        </xsl:variable>
        <xsl:if test="$check != ''">
          <c:yVal xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
            <xsl:call-template name="get_cache_values_for_SERIES">
              <!-- Series values or vertical values -->
              <xsl:with-param name="value_type">1</xsl:with-param>
            </xsl:call-template>
          </c:yVal>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

    <!-- c:bubbleSize -->
    <xsl:if test="$bubbles = 'true'">
      <xsl:choose>
        <xsl:when test="AI[4]/BRAI/ChartParsedFormula/@assembled_formula != ''">
          <c:bubbleSize xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
            <c:numRef>
              <xsl:apply-templates select="AI[4]" mode="charts"/>
            </c:numRef>
          </c:bubbleSize>
        </xsl:when>
        <xsl:otherwise>
          <xsl:variable name="check">
            <xsl:call-template name="check_bub"/>
          </xsl:variable>
          <c:bubbleSize xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
            <xsl:call-template name="get_cache_values_for_SERIES">
              <!-- Bubble sizes -->
              <xsl:with-param name="value_type">3</xsl:with-param>
            </xsl:call-template>
          </c:bubbleSize>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>

  <xsl:template match="Pos" mode="charts">
    <c:layout xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart">
      <c:manualLayout>
        <c:layoutTarget val="outer"/>
        <c:xMode val="edge"/>
        <c:yMode val="edge"/>
        <c:x val="{@x1 div 4000}"/>
        <c:y val="{@y1 div 4000}"/>
        <c:w val="{@x2 div 4000}"/>
        <c:h val="{@y2 div 4000}"/>
      </c:manualLayout>
    </c:layout>
  </xsl:template>

  <!-- Return values: p - point, s - series, a - axis, auto - not found -->
  <xsl:template name="determine_format_definition_place">
    <!-- Point SS -->
    <xsl:param name="SSp"/>
    <!-- Series SS -->
    <xsl:param name="SSs"/>
    <!-- Axis SS -->
    <xsl:param name="SSa"/>
    <xsl:param name="tag_name"/>

    <xsl:choose>
      <xsl:when test="$SSp">
        <xsl:variable name="format_tag" select="$SSp/*[name() = $tag_name]"/>
        <xsl:choose>
          <xsl:when test="$format_tag">
            <xsl:choose>
              <xsl:when test="not($format_tag/@fAuto = 'true')">p</xsl:when>
              <xsl:otherwise>auto</xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <!-- Once more but without SSp -->
            <xsl:call-template name="determine_format_definition_place">
              <xsl:with-param name="SSs" select="$SSs"/>
              <xsl:with-param name="SSa" select="$SSa"/>
              <xsl:with-param name="tag_name" select="$tag_name"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$SSs">
        <xsl:variable name="format_tag" select="$SSs/*[name() = $tag_name]"/>
        <xsl:choose>
          <xsl:when test="$format_tag">
            <xsl:choose>
              <xsl:when test="not($format_tag/@fAuto = 'true')">s</xsl:when>
              <xsl:otherwise>auto</xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <!-- Once more but without SSs -->
            <xsl:call-template name="determine_format_definition_place">
              <xsl:with-param name="SSa" select="$SSa"/>
              <xsl:with-param name="tag_name" select="$tag_name"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when test="$SSa">
        <xsl:variable name="format_tag" select="$SSa/*[name() = $tag_name]"/>
        <xsl:choose>
          <xsl:when test="$format_tag">
            <xsl:choose>
              <xsl:when test="not($format_tag/@fAuto = 'true')">a</xsl:when>
              <xsl:otherwise>auto</xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>auto</xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>auto</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ser_smooth">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="SSs"/>
    <xsl:param name="SSa"/>

    <xsl:variable name="smooth_dest">
      <xsl:call-template name="determine_format_definition_place">
        <xsl:with-param name="SSs" select="$SSs"/>
        <xsl:with-param name="SSa" select="$SSa"/>
        <xsl:with-param name="tag_name">SerFmt</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$smooth_dest = 's'">
        <c:smooth val="{$SSs/SerFmt/@fSmoothedLine}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
      <xsl:when test="$smooth_dest = 'a'">
        <c:smooth val="{$SSa/SerFmt/@fSmoothedLine}" xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"/>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="ser_marker">
    <!-- Current node must be SERIESFORMAT -->
    <xsl:param name="crt"/>
    <xsl:variable name="ser_order" select="SS/DataFormat[@xi = 65535]/@iss"/>

    <xsl:variable name="MarkerFormat_definition_location">
      <xsl:call-template name="determine_format_definition_place">
        <xsl:with-param name="SSs" select="SS[DataFormat/@xi = 65535]"/>
        <xsl:with-param name="SSa" select="$crt/SS"/>
        <xsl:with-param name="tag_name">MarkerFormat</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$MarkerFormat_definition_location = 's'">
        <xsl:for-each select="SS[DataFormat/@xi = 65535]">
          <xsl:call-template name="c_marker"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$MarkerFormat_definition_location = 'a'">
        <xsl:for-each select="$crt/SS">
          <xsl:call-template name="c_marker"/>
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="$MarkerFormat_definition_location = 'auto'">
        <xsl:call-template name="c_marker">
          <xsl:with-param name="def_settings_idx" select="$ser_order"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>


  </xsl:template>

</xsl:stylesheet>
