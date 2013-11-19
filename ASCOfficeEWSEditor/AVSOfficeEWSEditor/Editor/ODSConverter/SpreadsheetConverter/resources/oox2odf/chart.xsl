<?xml version="1.0" encoding="UTF-8"?>
<!--
  * Copyright (c) 2006, Clever Age
  * All rights reserved.
  * 
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted provided that the following conditions are met:
  *
  *     * Redistributions of source code must retain the above copyright
  *       notice, this list of conditions and the following disclaimer.
  *     * Redistributions in binary form must reproduce the above copyright
  *       notice, this list of conditions and the following disclaimer in the
  *       documentation and/or other materials provided with the distribution.
  *     * Neither the name of Clever Age nor the names of its contributors 
  *       may be used to endorse or promote products derived from this software
  *       without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
  * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
  * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-->
<!--
Modification Log
LogNo. |Date       |ModifiedBy   |BugNo.   |Modification                                                      |
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-1 12-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance                                              
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:xdr="http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:c="http://schemas.openxmlformats.org/drawingml/2006/chart"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" xmlns:oox="urn:oox"
  exclude-result-prefixes="e r c a xdr oox fo">
  
  <!-- @Filename: chart.xsl -->
  <!-- @Description: This stylesheet is used for charts conversion -->
  <!-- @Created: 2007-05-24 -->

	<!--<xsl:import href="relationships.xsl"/>-->
	<!-- <xsl:import href="Gradient.xsl"/>-->

  <xsl:key name="dataSeries" match="c:ser" use="@oox:part"/>
  <xsl:key name="numPoints" match="c:val" use="@oox:part"/>
  <xsl:key name="xNumPoints" match="c:xVal" use="@oox:part"/>
  <xsl:key name="categories" match="c:cat" use="@oox:part"/>
  <xsl:key name="plotArea" match="c:plotArea" use="@oox:part"/>
  <xsl:key name="grouping" match="c:grouping" use="@oox:part"/>
  <xsl:key name="spPr" match="c:spPr" use="@oox:part"/>
  <xsl:key name="error" match="c:errBars" use="@oox:part"/>
  <xsl:key name="dataLable" match="c:dLbl" use="''"/>
  <xsl:key name="dataLables" match="c:dLbls" use="''"/>
  <!-- Sonata: line style constants-->
  <xsl:variable name ="dot">
    <xsl:value-of select ="'0.07'"/>
  </xsl:variable>
  <xsl:variable name ="dash">
    <xsl:value-of select ="'0.282'"/>
  </xsl:variable>
  <xsl:variable name ="longDash">
    <xsl:value-of select ="'0.564'"/>
  </xsl:variable>
  <xsl:variable name ="distance">
    <xsl:value-of select ="'0.211'"/>
  </xsl:variable>
  <xsl:template name="CreateObjectCharts">
    <!-- @Description: Searches for all charts within workbook and starts conversion. -->
    <!-- @Context: None -->

    <!-- get all sheet Id's -->
    <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet">

      <xsl:variable name="sheet">
        <!-- path to sheet file from xl/ catalog (i.e. $sheet = worksheets/sheet1.xml) -->
        <xsl:call-template name="GetTarget">
          <xsl:with-param name="id">
            <xsl:value-of select="@r:id"/>
          </xsl:with-param>
          <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <!-- go to sheet file and search for drawing -->
      <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet/e:drawing">

        <xsl:variable name="drawing">
          <!-- path to drawing file from xl/ catalog (i.e. $drawing = ../drawings/drawing2.xml) -->
          <xsl:call-template name="GetTarget">
            <xsl:with-param name="id">
              <xsl:value-of select="@r:id"/>
            </xsl:with-param>
            <xsl:with-param name="document">
              <xsl:value-of select="concat('xl/',$sheet)"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <!-- go to sheet drawing file and search for charts -->
				<!--<xsl:for-each
          select="key('Part', concat('xl/',substring-after($drawing,'/')))/xdr:wsDr/xdr:twoCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart">-->
				<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,ChartLost because SP2 created xlsx has oneCellAnchor in Place of twoCellAnchor-->
        <xsl:for-each
				 select="key('Part', concat('xl/',substring-after($drawing,'/')))/xdr:wsDr//xdr:graphicFrame/a:graphic/a:graphicData/c:chart">
					<!--SP2,Scenario:ods_SP2_xlsx_2.5_ods,End-->
					<xsl:variable name="chartRelId">
						<xsl:value-of select="@r:id"/>
					</xsl:variable>
          <xsl:variable name="chart">
            <!-- path to chart file from xl/ catalog (i.e. $chart = ../charts/chart1.xml) -->
            <xsl:call-template name="GetTarget">
              <xsl:with-param name="id">
                <xsl:value-of select="@r:id"/>
              </xsl:with-param>
              <xsl:with-param name="document">
                <xsl:value-of select="concat('xl/',substring-after($drawing,'/'))"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="chartId">
            <!-- unique chart identifier -->
            <xsl:value-of select="generate-id(.)"/>
          </xsl:variable>

          <!-- finally go to a chart file -->
          <xsl:for-each select="key('Part', concat('xl/',substring-after($chart,'/')))">

            <xsl:call-template name="InsertChart">
              <xsl:with-param name="chartId" select="$chartId"/>
              <xsl:with-param name="inputChart">
                <xsl:value-of select="concat('xl/',substring-after($chart,'/'))"/>
              </xsl:with-param>

							<xsl:with-param name="sheetname" select="$sheet"/>
							<xsl:with-param name="drawingname" select="$drawing"/>
							<xsl:with-param name="chartRelId" select="$chartRelId"/>



            </xsl:call-template>

          </xsl:for-each>
        </xsl:for-each>
      </xsl:for-each>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="CreateSheetCharts">

    <xsl:for-each
      select="key('Part', 'xl/_rels/workbook.xml.rels')//node()[name() = 'Relationship' and starts-with(@Target,'chartsheets/')]">

      <xsl:variable name="sheet">
        <xsl:value-of select="@Target"/>
      </xsl:variable>

      <!-- go to sheet file and search for drawing -->
      <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:chartsheet/e:drawing">

        <xsl:variable name="drawing">
          <!-- path to drawing file from xl/ catalog (i.e. $drawing = ../drawings/drawing2.xml) -->
          <xsl:call-template name="GetTarget">
            <xsl:with-param name="id">
              <xsl:value-of select="@r:id"/>
            </xsl:with-param>
            <xsl:with-param name="document">
              <xsl:value-of select="concat('xl/',$sheet)"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <!-- go to sheet drawing file and search for charts -->
        <xsl:for-each
          select="key('Part', concat('xl/',substring-after($drawing,'/')))/xdr:wsDr/xdr:absoluteAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart">
					<xsl:variable name="chartRelId">
						<xsl:value-of select="@r:id"/>
					</xsl:variable>
          <xsl:variable name="chart">
            <!-- path to chart file from xl/ catalog (i.e. $chart = ../charts/chart1.xml) -->
            <xsl:call-template name="GetTarget">
              <xsl:with-param name="id">
                <xsl:value-of select="@r:id"/>
              </xsl:with-param>
              <xsl:with-param name="document">
                <xsl:value-of select="concat('xl/',substring-after($drawing,'/'))"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:variable name="chartId">
            <!-- unique chart identifier -->
            <xsl:value-of select="generate-id(.)"/>
          </xsl:variable>

          <!-- finally go to a chart file -->
          <xsl:for-each select="key('Part', concat('xl/',substring-after($chart,'/')))">

            <xsl:call-template name="InsertChart">
              <xsl:with-param name="chartId" select="$chartId"/>
              <xsl:with-param name="inputChart">
                <xsl:value-of select="concat('xl/',substring-after($chart,'/'))"/>
              </xsl:with-param>
							<xsl:with-param name="chartRelId" select="$chartRelId"/>
							<xsl:with-param name="sheetname" select="$sheet"/>
							<xsl:with-param name="drawingname" select="$drawing"/>
            </xsl:call-template>

          </xsl:for-each>
        </xsl:for-each>
      </xsl:for-each>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="InsertChart">
    <!-- @Description: Creates output chart files -->
    <!-- @Context: Input chart file root -->

    <xsl:param name="chartId"/>
    <!-- (string) unique chart identifier -->
    <xsl:param name="inputChart"/>
    <!-- (string) input chart file directory -->

		<!--changed by sonata for bug no:2016060-->
		<xsl:param name="sheetname"/>
		<xsl:param name="drawingname"/>
		<xsl:param name="chartRelId"/>
		<!--end-->

    <xsl:call-template name="InsertChartContent">
      <xsl:with-param name="chartId" select="$chartId"/>
			<!--changed by sonata for bug no:2016060-->
			<xsl:with-param name="sheetname" select="$sheetname"/>
			<xsl:with-param name="drawingname" select="$drawingname"/>
			<xsl:with-param name="chartRelId" select="$chartRelId"/>
			<!--end-->

    </xsl:call-template>

    <xsl:call-template name="InsertChartStyles">
      <xsl:with-param name="chartId" select="$chartId"/>
      <xsl:with-param name="inputChart" select="$inputChart"/>
    </xsl:call-template>

  </xsl:template>

  <xsl:template name="InsertChartContent">
    <!-- @Description: Creates output chart content file -->
    <!-- @Context: Input chart file root -->

    <xsl:param name="chartId"/>
    <!-- (string) unique chart identifier -->
		<xsl:param name="sheetname"/>
		<xsl:param name="drawingname"/>
		<xsl:param name="chartRelId"/>

    <pzip:entry pzip:target="{concat('Object ',$chartId,'/content.xml')}">
			<!--RefNo-1:ODF1.1:Added office:version="1.1" attribute:06-feb-09-->
			<office:document-content xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0" office:version="1.1">
        <office:automatic-styles>
          <number:number-style style:name="N0">
            <number:number number:min-integer-digits="1"/>
          </number:number-style>
          <xsl:call-template name="InsertChartProperties"/>
          <xsl:call-template name="InsertChartTitleProperties"/>
          <xsl:call-template name="InsertLegendProperties"/>
          <xsl:call-template name="InsertPlotAreaProperties"/>

          <xsl:for-each select="key('dataLables','')">
            <!-- Sonata: Series- default Number format-->
            <xsl:apply-templates select="c:numFmt"/>
          </xsl:for-each>
          <xsl:for-each select="key('dataLable','')">
            <!-- Sonata: Data lableNumber format-->
            <xsl:apply-templates select="c:numFmt"/>
          </xsl:for-each>

          <!--<xsl:for-each select="key('numPoints', @oox:part)//c:numRef/c:numCache">-->
          <!--changed by sonata for bug no:2107193-->
          <xsl:for-each select="key('numPoints', c:chartSpace/@oox:part)/descendant::c:numRef/c:numCache">
            <xsl:apply-templates select="c:formatCode"/>
          </xsl:for-each>
          <!--end-->


          <xsl:choose>
            <!-- for stock chart type 3 and 4 -->
            <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart and key('plotArea', c:chartSpace/@oox:part)/c:barChart">

              <xsl:variable name="axisYId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>
              
              <xsl:variable name="axisXId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/child::node()[ contains(name(),'Ax')][1]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>

              <xsl:variable name="secondaryAxisXId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/child::node()[ contains(name(),'Ax')][last()]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>
              
              <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[2]">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[1]">
                  <!--test number format of the axis text-->
                  <xsl:variable name="formatcode">
                    <xsl:if test="c:numFmt">
                      <xsl:value-of select="c:numFmt/@formatCode"/>
                    </xsl:if>
                  </xsl:variable>
                  <!-- Sonata: Number format-->
                  <xsl:apply-templates select="c:numFmt"/>
 <!--Edited by Sonata -->
                  <xsl:call-template name="InsertAxisXProperties">
                    <xsl:with-param name="axisYId" select="$axisYId"/>
                    <xsl:with-param name="numberformat" select="$formatcode"/>
                    <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                  </xsl:call-template>
                  <!--added by Sonata for bug no 2015014-->
                  <xsl:call-template name="InsertAxisXTitleProperties"/>
                  <xsl:call-template name="InsertMinorGridXProperties"/>
                  <xsl:call-template name="InsertMajorGridXProperties"/>
                  <!--end-->
                </xsl:for-each>
              </xsl:if>
              <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:catAx[2]">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
                  <!--test number format of the axis text-->
                  <xsl:variable name="formatcode">
                    <xsl:if test="c:numFmt">
                      <xsl:value-of select="c:numFmt/@formatCode"/>
                    </xsl:if>
                  </xsl:variable>
                  <!-- Sonata: Number format-->
                  <xsl:apply-templates select="c:numFmt"/>
<!--Edited by Sonata -->
                  <xsl:call-template name="InsertAxisXProperties">
                    <xsl:with-param name="axisYId" select="$axisYId"/>
                    <xsl:with-param name="numberformat" select="$formatcode"/>
                    <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                  </xsl:call-template>
                  <!--added by Sonata for bug no 2015014-->
                  <xsl:call-template name="InsertAxisXTitleProperties"/>
                  <xsl:call-template name="InsertMinorGridXProperties"/>
                  <xsl:call-template name="InsertMajorGridXProperties"/>
                  <!--end-->
                </xsl:for-each>
              </xsl:if>

              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                
                <xsl:choose>
                  <xsl:when test="c:axId/@val = $axisYId">
                    <!--test number format of the axis text-->
                    <xsl:variable name="formatcode">
                      <xsl:if test="c:numFmt">
                        <xsl:value-of select="c:numFmt/@formatCode"/>
                      </xsl:if>
                    </xsl:variable>
                    <!-- Sonata: Number format-->
                    <xsl:apply-templates select="c:numFmt"/>
                    <xsl:call-template name="InsertAxisYProperties">
                      <xsl:with-param name="axisXId" select="$axisXId"/>
                      <xsl:with-param name="numberformat" select="$formatcode"/>
                      <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                    </xsl:call-template>
                    <xsl:call-template name="InsertAxisYTitleProperties"/>
                    <xsl:call-template name="InsertMinorGridYProperties"/>
                    <xsl:call-template name="InsertMajorGridYProperties"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:call-template name="InsertSecondaryAxisYProperties">
                      <xsl:with-param name="axisXId" select="$secondaryAxisXId"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:for-each>

              <!--xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                <xsl:call-template name="InsertSecondaryAxisYProperties"/>
              </xsl:for-each>
              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[2]">
                <xsl:call-template name="InsertAxisYProperties"/>
              </xsl:for-each-->
            </xsl:when>
            <!-- scatter chart has two value axes -->
            <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart or key('plotArea', c:chartSpace/@oox:part)/c:bubbleChart">
              <xsl:variable name="axisYId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[2]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>

              <xsl:variable name="axisXId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>
              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[c:axPos/@val = 'b' or c:axPos/@val = 't'][1]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <!-- Sonata: Number format-->
                <xsl:apply-templates select="c:numFmt"/>
                <xsl:call-template name="InsertAxisXProperties">
                  <xsl:with-param name="type">
                    <xsl:text>valAx</xsl:text>
                  </xsl:with-param>
                  <xsl:with-param name="axisYId" select="$axisYId"/>
                  <xsl:with-param name="numberformat" select="$formatcode"/>
                  <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                </xsl:call-template>
                <xsl:call-template name="InsertAxisXTitleProperties"/>
                <xsl:call-template name="InsertMajorGridXProperties"/>
                <xsl:call-template name="InsertMinorGridXProperties"/>
            
              </xsl:for-each>
              <xsl:for-each
                select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[c:axPos/@val = 'l' or c:axPos/@val = 'r'][1]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <!-- Sonata: Number format-->
                <xsl:apply-templates select="c:numFmt"/>
                <xsl:call-template name="InsertAxisYProperties">
                  <xsl:with-param name="axisXId" select="$axisXId"/>
                  <xsl:with-param name="numberformat" select="$formatcode"/>
                  <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                </xsl:call-template>
                <xsl:call-template name="InsertAxisYTitleProperties"/>
                <xsl:call-template name="InsertMajorGridYProperties"/>
                <xsl:call-template name="InsertMinorGridYProperties"/>
              </xsl:for-each>
            </xsl:when>
            <!-- insert axis X properties for stock chart -->
            <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart">
              <!-- stock chart may contain catAx or dateAx as X axis -->
              <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:catAx">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx">
                  <!--test number format of the axis text-->
                  <xsl:variable name="formatcode">
                    <xsl:if test="c:numFmt">
                      <xsl:value-of select="c:numFmt/@formatCode"/>
                    </xsl:if>
                  </xsl:variable>
                  <!-- Sonata: Number format-->
                  <xsl:apply-templates select="c:numFmt"/>
                  <xsl:call-template name="InsertAxisXProperties">
                    <xsl:with-param name="numberformat" select="$formatcode"/>
                    <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                  </xsl:call-template>
                  <xsl:call-template name="InsertAxisXTitleProperties"/>
                  <xsl:call-template name="InsertMajorGridXProperties"/>
                  <xsl:call-template name="InsertMinorGridXProperties"/>
                </xsl:for-each>
              </xsl:if>
              <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:dateAx">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx">
                  <!--test number format of the axis text-->
                  <xsl:variable name="formatcode">
                    <xsl:if test="c:numFmt">
                      <xsl:value-of select="c:numFmt/@formatCode"/>
                    </xsl:if>
                  </xsl:variable>
                  <!-- Sonata: Number format-->
                  <xsl:apply-templates select="c:numFmt"/>
                  <xsl:call-template name="InsertAxisXProperties">
                    <xsl:with-param name="numberformat" select="$formatcode"/>
                    <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                  </xsl:call-template>
                  <xsl:call-template name="InsertAxisXTitleProperties"/>
                  <xsl:call-template name="InsertMajorGridXProperties"/>
                  <xsl:call-template name="InsertMinorGridXProperties"/>
                </xsl:for-each>
              </xsl:if>
              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <!-- Sonata: Number format-->
                <xsl:apply-templates select="c:numFmt"/>
                <xsl:call-template name="InsertAxisYProperties">
                  <xsl:with-param name="numberformat" select="$formatcode"/>
                  <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                </xsl:call-template>
                <xsl:call-template name="InsertAxisYTitleProperties"/>
                <xsl:call-template name="InsertMajorGridYProperties"/>
                <xsl:call-template name="InsertMinorGridYProperties"/>
              </xsl:for-each>
            </xsl:when>
            <xsl:otherwise>
              <xsl:variable name="axisYId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>

              <xsl:variable name="axisXId">
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
                  <xsl:value-of select="c:axId/@val"/>
                </xsl:for-each>
              </xsl:variable>
              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <!-- Sonata: Number format-->
                <xsl:apply-templates select="c:numFmt"/>
                <xsl:call-template name="InsertAxisXProperties">
                  <xsl:with-param name="type">
                    <xsl:text>catAx</xsl:text>
                  </xsl:with-param>
                  <xsl:with-param name="axisYId" select="$axisYId"/>
                  <xsl:with-param name="numberformat" select="$formatcode"/>
                  <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                </xsl:call-template>
                <xsl:call-template name="InsertAxisXTitleProperties"/>
                <xsl:call-template name="InsertMajorGridXProperties"/>
                <xsl:call-template name="InsertMinorGridXProperties"/>
              </xsl:for-each>
              <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <!-- Sonata: Number format-->
                  <xsl:apply-templates select="c:numFmt"/>
                
                <xsl:call-template name="InsertAxisYProperties">
                  <xsl:with-param name="numberformat" select="$formatcode"/>
                  <xsl:with-param name="dataStyleName" select="generate-id(c:numFmt)"/>
                  <xsl:with-param name="axisXId" select="$axisXId"/>
                </xsl:call-template>
                <xsl:call-template name="InsertAxisYTitleProperties"/>
                <xsl:call-template name="InsertMajorGridYProperties"/>
                <xsl:call-template name="InsertMinorGridYProperties"/>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>

          <xsl:call-template name="InsertSeriesProperties"/>
          <xsl:call-template name="InsertErrorProperties"/>
          <xsl:call-template name="InsertWallProperties"/>
          <xsl:call-template name="InsertFloorProperties"/>
          <!-- Sonata: Stock range Properties-->
          <xsl:call-template name="InsertStockRangeProperties"/>
        </office:automatic-styles>
        <office:body>
          <office:chart>
						<xsl:call-template name="InsertChartType">
							<xsl:with-param name="chartRelId" select="$chartRelId"/>
							<xsl:with-param name="sheetname" select="$sheetname"/>
							<xsl:with-param name="drawingname" select="$drawingname"/>
						</xsl:call-template>
          </office:chart>
        </office:body>
      </office:document-content>

    </pzip:entry>
  </xsl:template>

  <xsl:template name="InsertChartStyles">
    <!-- @Description: Creates output chart styles file -->
    <!-- @Context: Input chart file root -->

    <xsl:param name="chartId"/>
    <!-- unique chart identifier -->
    <xsl:param name="inputChart"/>
    <!-- input chart file directory -->

    <pzip:entry pzip:target="{concat('Object ',$chartId,'/styles.xml')}">
			<!--RefNo-1:ODF1.1:Added office:version="1.1":06-feb-09-->
			<office:document-styles office:version="1.1">
        <office:styles>
          <xsl:call-template name="InsertDrawFillImage">
            <xsl:with-param name="chartId" select="$chartId"/>
            <xsl:with-param name="inputChart" select="$inputChart"/>
          </xsl:call-template>
          <xsl:call-template name="InsertChartLineStyle">
            <xsl:with-param name="chartId" select="$chartId"/>
            <xsl:with-param name="inputChart" select="$inputChart"/>
          </xsl:call-template>
        </office:styles>
      </office:document-styles>
    </pzip:entry>
  </xsl:template>
  <xsl:template name="InsertChartLineStyle">
    <!-- @Description: Creates output chart line styles file -->
    <!-- @Context: Input chart file root -->
    <!-- @Author: Sonata-->
    <xsl:param name="chartId"/>
    <!-- unique chart identifier -->
    <xsl:param name="inputChart"/>
    <xsl:for-each select="key('spPr', c:chartSpace/@oox:part)/a:ln">
      <!--Dash types-->
      <xsl:if test ="a:prstDash/@val">
        <xsl:call-template name ="getDashType">
          <xsl:with-param name ="val" select ="a:prstDash/@val" />
          <xsl:with-param name ="cap" select ="@cap" />
        </xsl:call-template>
      </xsl:if>
    </xsl:for-each>
    
  </xsl:template>
  <xsl:template name ="getDashType">
    <!-- @Description: dash types for chart line style-->
    <!-- @Author: Sonata-->
    <xsl:param name ="val" />
    <xsl:param name ="cap" />
    <xsl:choose>
      <xsl:when test ="($val='sysDot')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'Fine_20_Dotted'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$dot" />
          <xsl:with-param name ="distance" select ="$dot" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='sysDash')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'Dash'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$dot" />
          <xsl:with-param name ="distance" select ="$dot" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='1 1' and $cap='round')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'Fine_20_Dotted'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='dash')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'Ultrafine_20_Dashed'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$dash" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$dash" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='1 1')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'SquareDot'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$distance" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$distance" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='dashDot')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'_1_Dot_1_Dash'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$dot" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$dash" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='lgDash')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'Fine_20_Dashed'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$longDash" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$longDash" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='lgDashDot')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'_1_Dot_2_Dash'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'1'" />
          <xsl:with-param name ="dot1-length" select = "$dot" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$longDash" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>
      <xsl:when test ="($val='lgDashDotDot')">
        <xsl:call-template name ="AddDashType">
          <xsl:with-param name ="name" select ="'_32__20_Dots_20_1_20_Dash'" />
          <xsl:with-param name ="cap" select ="$cap" />
          <xsl:with-param name ="dot1" select ="'2'" />
          <xsl:with-param name ="dot1-length" select = "$dot" />
          <xsl:with-param name ="dot2" select ="'1'" />
          <xsl:with-param name ="dot2-length" select = "$longDash" />
          <xsl:with-param name ="distance" select ="$distance" />
        </xsl:call-template>
      </xsl:when>

    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertSeries">
    <!-- @Description: Outputs chart series in normal order  -->
    <!-- @Context: inside input chart file -->

    <xsl:param name="reverseCategories"/>
    <!-- (bool) if categories have to be reversed during conversion -->

    <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">
      <xsl:call-template name="InsertSeriesData">
        <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertSeriesReversed">
    <!-- @Description: Outputs chart series in reverse order  -->
    <!-- @Context: c:ser -->

    <xsl:param name="reverseCategories"/>
    <!-- (bool) if categories have to be reversed during conversion -->

    <xsl:call-template name="InsertSeriesData">
      <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
    </xsl:call-template>
<!--commented by sonata for bug no:2107193-->
    <!--<xsl:for-each select="preceding-sibling::c:ser[1]">
      <xsl:call-template name="InsertSeriesReversed">
        <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
      </xsl:call-template>
    </xsl:for-each>-->
		<!--changed by sonata for bug no:2594979-->
		<xsl:if test="parent::node()[name()='c:areaChart']/c:grouping[@val='standard']">
			<xsl:for-each select="preceding-sibling::c:ser[1]">
				<xsl:call-template name="InsertSeriesReversed">
					<xsl:with-param name="reverseCategories" select="$reverseCategories"/>
				</xsl:call-template>
			</xsl:for-each>
		</xsl:if>
<!--end-->
    
  </xsl:template>
   
  <xsl:template name="ConvertCellRange">
    <xsl:param name="cellRange"/>
    <xsl:param name="finalChrtRng" select="''"/>    

		<!--end-->
		<xsl:variable name ="temp">
			<xsl:call-template name="translate-expression">
				<xsl:with-param name="isRangeAddress" select="true()"/>
				<xsl:with-param name="cell-row-pos" select="0"/>
				<xsl:with-param name="cell-column-pos" select="0"/>
				<xsl:with-param name="expression" select="$cellRange"/>
				<xsl:with-param name="return-value" select="''"/>
			</xsl:call-template>
    </xsl:variable>
		<xsl:value-of select="$temp"/>
  </xsl:template>

  <xsl:template name="InsertSeriesData">
    <!-- @Description: Outputs chart series data -->
    <!-- @Context: c:ser -->

    <xsl:param name="reverseCategories"/>
    <!-- (bool) if categories have to be reversed during conversion -->

    <!-- calculate this series number -->
    <xsl:variable name="seriesNumber">
      <xsl:value-of select="c:idx/@val"/>
    </xsl:variable>

    <chart:series chart:style-name="{concat('series',$seriesNumber)}">
			<xsl:variable name ="pos">
				<xsl:value-of select ="position()"/>
			</xsl:variable>
      <!--added by sonata for bug no:2107116-->
      <!--reference to the series value-->
			<!--added by chhavi for sp2-->
			<xsl:if test="c:val//c:f or c:yVal//c:f">
       <xsl:variable name="cellRange">
					<xsl:choose>
						<xsl:when test="c:val//c:f">
        <xsl:value-of select="c:val//c:f"/>
						</xsl:when>
						<xsl:when test="c:yVal//c:f">
							<xsl:value-of select="c:yVal//c:f"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="tempRangeVal">
					<xsl:choose>
						<xsl:when test="(starts-with($cellRange, '(') or 
								  contains(substring($cellRange,string-length($cellRange)-1),')')) and 
								  contains($cellRange,',') and 
								  contains(substring-after($cellRange,'!'),',')">
							<xsl:value-of select="substring(substring-before($cellRange,','),2)"/>							
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$cellRange"/>
								</xsl:otherwise>
							</xsl:choose>
				</xsl:variable>
				<xsl:variable name="apos">
					<xsl:text>&apos;</xsl:text>
				</xsl:variable>				
				<xsl:variable name="sheetname">
					<xsl:choose>
						<xsl:when test ="starts-with($tempRangeVal,$apos)">
							<xsl:variable name ="temp">
								<xsl:value-of select="substring(substring-before($tempRangeVal,'!'),2)"/>
							</xsl:variable>
							<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="substring-before($tempRangeVal,'!')"/>
						</xsl:otherwise>
					</xsl:choose>
					
				</xsl:variable>
				<!--added for bug no:2557071-->

				<xsl:variable name ="sheetNames">
					<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
						<xsl:if test="@name = $sheetname">
							<xsl:variable name="checkedName">
								<xsl:call-template name="CheckSheetName">
									<xsl:with-param name="sheetNumber">
										<xsl:value-of select="position()"/>
									</xsl:with-param>
									<xsl:with-param name="name">
										<xsl:value-of select="@name"/>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:variable>
							<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
							<xsl:value-of select ="$checkedName"/>
						</xsl:if>
					</xsl:for-each>
				</xsl:variable>
				<xsl:variable name ="temp1">
					<xsl:value-of select ="concat($apos,$sheetNames,$apos,'!',substring-after($tempRangeVal,'!'))"/>
        </xsl:variable>
        <xsl:variable name="rangeVal">
          <xsl:call-template name="ConvertCellRange">
						<xsl:with-param name="cellRange" select="$temp1"/>
          </xsl:call-template>
        </xsl:variable>
				<xsl:if test="not(contains($cellRange,'#REF!')) and $sheetNames!=''">
          <xsl:attribute name="chart:values-cell-range-address">
						<xsl:choose>
							<xsl:when test="contains($rangeVal,':')">
										<xsl:value-of select="concat(translate(substring-before($rangeVal,':'),'$',''),':.',translate(substring-after($rangeVal,':'),'$',''))"/>
										<!--<xsl:value-of select="concat(substring-before($rangeVal,':'),':.',substring-after($rangeVal,':'))"/>-->
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rangeVal"/>
							</xsl:otherwise>
						</xsl:choose>
          </xsl:attribute>
        </xsl:if> 
          </xsl:if>
			<!-- Defect:  2682600 , 2186921 
                             Code Changes By : Vijayeta
                             Desc:           : On Reverse converssion, the source data range is not retained in ODF
                                               Additional attributes added to maintain the link between chart abd source data.
                         -->
			<xsl:if test ="not(key('plotArea', c:chartSpace/@oox:part)/c:doughnutChart)">
									<xsl:variable name="apos">
						<xsl:text>&apos;</xsl:text>
					</xsl:variable>
                                     <xsl:if test="c:tx//c:f">
					<xsl:variable name ="tempRangeVal">
						<xsl:value-of select="c:tx//c:f"/>
					</xsl:variable>
					<xsl:variable name="sheetname1">
						<xsl:choose>
							<xsl:when test ="starts-with(substring-before($tempRangeVal,'!'),$apos)">
								<xsl:variable name ="temp">
									<xsl:value-of select="substring(substring-before($tempRangeVal,'!'),2)"/>
								</xsl:variable>
								<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="substring-before($tempRangeVal,'!')"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name ="sheetNames">
						<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
							<xsl:if test="@name = $sheetname1">
								<xsl:variable name="checkedName">
									<xsl:call-template name="CheckSheetName">
										<xsl:with-param name="sheetNumber">
											<xsl:value-of select="position()"/>
										</xsl:with-param>
										<xsl:with-param name="name">
											<xsl:value-of select="@name"/>
										</xsl:with-param>
									</xsl:call-template>
								</xsl:variable>
								<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
								<xsl:value-of select ="$checkedName"/>
							</xsl:if>
						</xsl:for-each>
					</xsl:variable>
					<xsl:variable name ="temp1">
						<xsl:value-of select ="concat($apos,$sheetNames,$apos,'!',substring-after($tempRangeVal,'!'))"/>
					</xsl:variable>
					<xsl:variable name="rangeVal">
						<xsl:call-template name="ConvertCellRange">
							<xsl:with-param name="cellRange" select="$temp1"/>
						</xsl:call-template>
					</xsl:variable>
					<xsl:attribute name ="chart:label-cell-address">
						<xsl:choose>
							<xsl:when test="contains($rangeVal,':')">
								<xsl:value-of select="concat(translate(substring-before($rangeVal,':'),'$',''),':.',translate(substring-after($rangeVal,':'),'$',''))"/>
								<!--<xsl:value-of select="concat(substring-before($rangeVal,':'),':.',substring-after($rangeVal,':'))"/>-->
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$rangeVal"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:attribute>
				</xsl:if>
			</xsl:if>
			<!--Vijayeta-->
      <!--end of code change, for defects   2682600  and  2186921 -->
      <xsl:if
        test="(key('plotArea', @oox:part)/c:scatterChart or key('plotArea', @oox:part)/c:bubbleChart) and position() = 1">
        <chart:domain>
          <!--added for filename:c1 f1 miseryindexrev.xlsx-->          
          <xsl:if test="key('xNumPoints', @oox:part)//c:f">
          <xsl:variable name="cellRange">
              <xsl:value-of select="key('xNumPoints', @oox:part)//c:f"/>
            </xsl:variable>
						<xsl:variable name ="tempRangeVal">
							<xsl:choose>
								<xsl:when test="(starts-with($cellRange, '(') or 
								  contains(substring($cellRange,string-length($cellRange)-1),')')) and 
								  contains($cellRange,',') and 
								  contains(substring-after($cellRange,'!'),',')">
									<xsl:value-of select="substring(substring-before($cellRange,','),2)"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$cellRange"/>
										</xsl:otherwise>
									</xsl:choose>
						</xsl:variable>
						<xsl:variable name="apos">
							<xsl:text>&apos;</xsl:text>
						</xsl:variable>
						<xsl:variable name="sheetname">
							<xsl:choose>
								<xsl:when test ="starts-with($tempRangeVal,$apos)">
									<xsl:variable name ="temp">
										<xsl:value-of select="substring(substring-before($tempRangeVal,'!'),2)"/>
									</xsl:variable>
									<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="substring-before($tempRangeVal,'!')"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<!--added for bug no:2557071-->
						<xsl:variable name ="sheetNames">
							<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
								<xsl:if test="@name = $sheetname">
									<xsl:variable name="checkedName">
										<xsl:call-template name="CheckSheetName">
											<xsl:with-param name="sheetNumber">
												<xsl:value-of select="position()"/>
											</xsl:with-param>
											<xsl:with-param name="name">
												<xsl:value-of select="@name"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:variable>
									<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
									<xsl:value-of select ="$checkedName"/>
								</xsl:if>
							</xsl:for-each>
						</xsl:variable>
						<xsl:variable name ="temp1">
							<xsl:value-of select ="concat($apos,$sheetNames,$apos,'!',substring-after($tempRangeVal,'!'))"/>
						</xsl:variable>
            <xsl:variable name="rangeVal">
              <xsl:call-template name="ConvertCellRange">
								<xsl:with-param name="cellRange" select="$temp1"/>
              </xsl:call-template>
            </xsl:variable>            
						<xsl:if test="not(contains($cellRange,'#REF!')) and $sheetNames!=''">
              <xsl:attribute name="table:cell-range-address">
								<xsl:choose>
									<xsl:when test="contains($rangeVal,':')">
										<xsl:value-of select="concat(substring-before($rangeVal,':'),':.',substring-after($rangeVal,':'))"/>
									</xsl:when>
									<xsl:otherwise>
                <xsl:value-of select="$rangeVal"/>
									</xsl:otherwise>
								</xsl:choose>
              </xsl:attribute>
            </xsl:if>
          </xsl:if>
          <!--end-->
        </chart:domain>
      </xsl:if>
      <!-- for stock chart type 3 and type 4 -->
      <xsl:if test="key('plotArea', @oox:part)/c:stockChart and key('plotArea', @oox:part)/c:barChart">
        <xsl:attribute name="chart:attached-axis">

          <xsl:variable name="axisYId">
            <xsl:for-each select="parent::node()">
              <xsl:value-of select="c:axId[2]/@val"/>
            </xsl:for-each>
          </xsl:variable>

          <xsl:for-each select="key('plotArea', @oox:part)/child::node()[contains(name(),'Ax')][2]">
            <xsl:choose>
              <xsl:when test="c:axId/@val = $axisYId">
                <xsl:text>primary-y</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>secondary-y</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>

          <!-- first series belong to primary y -->
        </xsl:attribute>
        <xsl:if test="position()=1">
          <xsl:attribute name="chart:class">
            <xsl:text>chart:bar</xsl:text>
          </xsl:attribute>
        </xsl:if>
      </xsl:if>



      <!-- error indicator -->
      <xsl:for-each select="c:errBars">
        <xsl:if test="c:errValType/@val != 'stdErr' and c:errValType/@val != 'stdDev' ">
          <chart:error-indicator chart:style-name="{concat('error',generate-id(.))}"/>
        </xsl:if>
      </xsl:for-each>

      <xsl:if test="c:trendline">
        <chart:regression-curve chart:style-name="{concat('trend',position())}"/>
      </xsl:if>

      <!-- insert data points -->
      <xsl:variable name="numPoints">
        <xsl:choose>
          <!-- for scatter chart -->
          <xsl:when test="key('xNumPoints', @oox:part)/descendant::c:ptCount">
            <xsl:value-of select="key('xNumPoints', @oox:part)/descendant::c:ptCount/@val"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="key('numPoints', @oox:part)/descendant::c:ptCount/@val"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:choose>
        <!-- insert data points to maintain properties -->
        <xsl:when
          test="key('plotArea',@oox:part)/c:pieChart or key('plotArea',@oox:part)/c:pie3DChart ">
          <xsl:if test="$seriesNumber = 0">
            <xsl:call-template name="InsertPieDataPoints">
              <xsl:with-param name="numPoints" select="$numPoints - 1"/>
              <xsl:with-param name="countSer" select="0"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:when>
        <!-- when all data points have default properties -->
        <xsl:when test="not(c:dPt) and not(c:dLbls)">
          <chart:data-point>
            <xsl:attribute name="chart:repeated">
              <xsl:value-of select="$numPoints"/>
            </xsl:attribute>
          </chart:data-point>
        </xsl:when>
        <xsl:otherwise>

          <xsl:choose>
            <xsl:when test="$reverseCategories = 'true' ">
              <!-- insert data points in reverse order -->
              <xsl:call-template name="InsertDataPointsReverse">
                <xsl:with-param name="seriesNum">
                  <xsl:value-of select="$seriesNumber"/>
                </xsl:with-param>
                <xsl:with-param name="numPoints">
                  <xsl:value-of select="$numPoints"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <!-- insert data points in normal order -->
              <xsl:call-template name="InsertDataPoints">
                <xsl:with-param name="seriesNum">
                  <xsl:value-of select="$seriesNumber"/>
                </xsl:with-param>
                <xsl:with-param name="numPoints">
                  <xsl:value-of select="$numPoints"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:otherwise>
      </xsl:choose>

    </chart:series>
  </xsl:template>

  <xsl:template name="InsertPieDataPoints">
    <!-- @Description: Inserts data points properties for pie charts  -->
    <!-- @Context: c:ser -->

    <xsl:param name="numPoints"/>
    <!-- (int) total number of points in chart reduced by 1-->
    <xsl:param name="current" select="$numPoints"/>
    <!-- (int) number of currently processed point (zero-based) -->
    <xsl:param name="countSer"></xsl:param>

    <!--changed by sonata for bug no:2557071-->
    <!--<chart:data-point chart:style-name="{concat('data0-',$current)}"/>-->
    <chart:data-point chart:style-name="{concat('data0-',$countSer)}"/>

    <!--<xsl:if test="$current != 0">
      <xsl:call-template name="InsertPieDataPoints">
        <xsl:with-param name="numPoints" select="$numPoints"/>
        <xsl:with-param name="current" select="$current -1"/>
      </xsl:call-template>
    </xsl:if>-->
    <xsl:if test="$countSer &lt; $numPoints">
      <xsl:call-template name="InsertPieDataPoints">
        <xsl:with-param name="numPoints" select="$numPoints"/>
        <xsl:with-param name="countSer" select="$countSer +1"/>
      </xsl:call-template>
    </xsl:if>
    <!--end-->
    
  </xsl:template>

  <xsl:template name="InsertDataPoints">
    <!-- @Description: Inserts data points for series  in normal order -->
    <!-- @Context: c:ser-->

    <xsl:param name="seriesNum"/>
    <!-- (int) sequential number of currently processed series -->
    <xsl:param name="numPoints"/>
    <!-- (int) number of all data points in series -->
    <xsl:param name="current" select="0"/>
    <!-- (int) current data point number (zero based) -->
    <xsl:param name="prev" select="-1"/>
    <!-- (int) previous non-default data point number (zero based) -->

    <!-- does this data point has label -->
    <xsl:variable name="hasLabel">
      <xsl:choose>
        <xsl:when test="c:dLbls/c:dLbl/c:idx[@val = $current]">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- process this data point -->
    <xsl:choose>
      <!-- if data point is not default -->
      <xsl:when test="c:dPt[c:idx/@val = $current] or $hasLabel = 'true' ">
        <!-- insert default data points before -->
        <xsl:if test="$current - $prev &gt; 1  ">
          <chart:data-point>
            <xsl:if test="$current - $prev &gt; 2">
              <xsl:attribute name="chart:repeated">
                <xsl:value-of select="$current - $prev - 1"/>
              </xsl:attribute>
            </xsl:if>
          </chart:data-point>
        </xsl:if>

        <!-- insert current data point -->
        <chart:data-point>
          <!-- 
            style-name format: dataS-D 
            S - series number
            D - data point number
            i.e. data2-3
          -->
          <xsl:attribute name="chart:style-name">
            <!--<xsl:value-of select="concat('data',$seriesNum,'-',$current)"/>-->
            <!--changed by sonata for bug no: 2636243 and  2633431 -->
            <xsl:value-of select="concat('data',$current,'-',$seriesNum)"/>
          </xsl:attribute>
        </chart:data-point>

        <!-- next data point -->
        <xsl:if test="$current != $numPoints - 1">
          <xsl:call-template name="InsertDataPoints">
            <xsl:with-param name="seriesNum" select="$seriesNum"/>
            <xsl:with-param name="numPoints" select="$numPoints"/>
            <xsl:with-param name="prev" select="$current"/>
            <xsl:with-param name="current" select="$current + 1"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:when>

      <!-- if data point is default process next data point or output default data-points if the last one -->
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$current = $numPoints - 1">
            <chart:data-point>
              <xsl:if test="$current - $prev &gt; 1">
                <xsl:attribute name="chart:repeated">
                  <xsl:value-of select="$current - $prev "/>
                </xsl:attribute>
              </xsl:if>
            </chart:data-point>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="InsertDataPoints">
              <xsl:with-param name="seriesNum" select="$seriesNum"/>
              <xsl:with-param name="numPoints" select="$numPoints"/>
              <xsl:with-param name="prev" select="$prev"/>
              <xsl:with-param name="current" select="$current + 1"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertDataPointsReverse">
    <!-- @Description: Inserts data points for series in reverse order -->
    <!-- @Context: c:ser -->

    <xsl:param name="seriesNum"/>
    <!-- (int) sequential number of currently processed series -->
    <xsl:param name="numPoints"/>
    <!-- (int) number of all data points in series -->
    <xsl:param name="current" select="$numPoints - 1"/>
    <!-- (int) current data point number (zero based) -->
    <xsl:param name="prev" select="$numPoints"/>
    <!-- (int) previous non-default data point number (zero based) -->

    <!-- does this data point has label -->
    <xsl:variable name="hasLabel">
      <xsl:choose>
        <xsl:when test="c:dLbls/c:dLbl/c:idx[@val = $current]">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- process this data point -->
    <xsl:choose>
      <!-- if data point is not default -->
      <xsl:when test="c:dPt[c:idx/@val = $current] or $hasLabel = 'true' ">
        <!-- insert default data points before -->
        <xsl:if test="$prev - $current &gt; 1  ">
          <chart:data-point>
            <xsl:if test="$prev - $current &gt; 2">
              <xsl:attribute name="chart:repeated">
                <xsl:value-of select="$prev - $current - 1"/>
              </xsl:attribute>
            </xsl:if>
          </chart:data-point>
        </xsl:if>

        <!-- insert current data point -->
        <chart:data-point>
          <!-- 
            style-name format: dataS-D 
            S - series number
            D - data point number
            i.e. data2-3
          -->
          <xsl:attribute name="chart:style-name">
            <xsl:value-of select="concat('data',$seriesNum,'-',$current)"/>
          </xsl:attribute>
        </chart:data-point>

        <!-- next data point -->
        <xsl:if test="$current != 0">
          <xsl:call-template name="InsertDataPointsReverse">
            <xsl:with-param name="seriesNum" select="$seriesNum"/>
            <xsl:with-param name="numPoints" select="$numPoints"/>
            <xsl:with-param name="prev" select="$current"/>
            <xsl:with-param name="current" select="$current - 1"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:when>

      <!-- if data point is default process next data point -->
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$current = 0">
            <chart:data-point>
              <xsl:if test="$prev &gt; 1">
                <xsl:attribute name="chart:repeated">
                  <xsl:value-of select="$prev"/>
                </xsl:attribute>
              </xsl:if>
            </chart:data-point>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="InsertDataPointsReverse">
              <xsl:with-param name="seriesNum" select="$seriesNum"/>
              <xsl:with-param name="numPoints" select="$numPoints"/>
              <xsl:with-param name="current" select="$current - 1"/>
              <xsl:with-param name="prev" select="$prev"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertChartTable">
    <!-- @Description: Outputs chart data table containing visualized values  -->
    <!-- @Context: inside input chart file -->

    <xsl:variable name="reverseSeries">
      <xsl:call-template name="CheckIfSeriesReversed"/>
    </xsl:variable>

    <xsl:variable name="reverseCategories">
      <xsl:call-template name="CheckIfCategoriesReversed"/>
    </xsl:variable>

    <table:table table:name="local-table">
      <table:table-header-columns>
        <table:table-column/>
      </table:table-header-columns>
      <table:table-columns>
        <table:table-column table:number-columns-repeated="3"/>
      </table:table-columns>

      <table:table-header-rows>
        <xsl:choose>

          <xsl:when test="$reverseSeries = 'true' ">
            <table:table-row>
              <table:table-cell>
                <text:p/>
              </table:table-cell>
              <!--changed by sonata for bug no:2107193-->
							<!--changed by sonata for bug no:2594979-->
              <!--<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[1]/parent::node()/c:ser[last()]">-->
							<xsl:choose>
								<xsl:when test="key('dataSeries', c:chartSpace/@oox:part)[1]/parent::node()[name()='c:areaChart']/c:grouping[@val='standard']">
									<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[1]/parent::node()/c:ser[last()]">
										<xsl:call-template name="InsertHeaderRowsReverse"/>
									</xsl:for-each>
								</xsl:when>
								<xsl:otherwise>
              <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[1]/parent::node()/c:ser">
                <xsl:call-template name="InsertHeaderRowsReverse"/>
              </xsl:for-each>
								</xsl:otherwise>
							</xsl:choose>

              <!--end-->

            </table:table-row>
          </xsl:when>

          <xsl:otherwise>
            <xsl:call-template name="InsertHeaderRows"/>
          </xsl:otherwise>
        </xsl:choose>
      </table:table-header-rows>

      <xsl:variable name="points">
        <xsl:choose>
          <!-- for scatter charts -->
          <xsl:when test="key('xNumPoints', c:chartSpace/@oox:part)/descendant::c:ptCount">
            <xsl:value-of select="key('xNumPoints', c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="key('numPoints', c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:variable>

      <!--RefNo-1:ODF1.1:Added condtion to chk for data-->
      <xsl:if test="0 &lt; $points">
      <table:table-rows>
        <xsl:call-template name="InsertDataRows">
          <xsl:with-param name="points" select="$points"/>
          <xsl:with-param name="categories" select="key('categories', c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
          <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
          <xsl:with-param name="reverseSeries" select="$reverseSeries"/>
          <!--changed by sonata for bug no:2107193-->
          <xsl:with-param name="pointIndex" select="'-1'"/>
        </xsl:call-template>
      </table:table-rows>
      </xsl:if>

    </table:table>
  </xsl:template>

  <xsl:template name="InsertHeaderRows">
    <!-- @Description: Outputs series names -->
    <!-- @Context: inside input chart file -->

    <table:table-row>
      <table:table-cell>
        <text:p/>
      </table:table-cell>

      <!-- insert cell responding to axis x values -->
      <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart or key('plotArea', c:chartSpace/@oox:part)/c:bubbleChart">
        <table:table-cell>
          <text:p/>
        </table:table-cell>
      </xsl:if>

      <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">
        <table:table-cell office:value-type="string">
          <text:p>
            <xsl:choose>
              <xsl:when test="c:tx/descendant::c:v">
                <xsl:value-of select="c:tx/descendant::c:v"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat('Series',position())"/>
              </xsl:otherwise>
            </xsl:choose>
          </text:p>
        </table:table-cell>
      </xsl:for-each>
    </table:table-row>
  </xsl:template>

  <xsl:template name="InsertHeaderRowsReverse">
    <!-- @Description: Outputs series names -->
    <!-- @Context: last series node -->

    <xsl:param name="count" select="1"/>

    <table:table-cell office:value-type="string">
      <text:p>
        <xsl:choose>
          <xsl:when test="c:tx/descendant::c:v">
            <xsl:value-of select="c:tx/descendant::c:v"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of
              select="concat('Series',key('numPoints', @oox:part)/descendant::c:ptCount/@val - $count)"
            />
          </xsl:otherwise>
        </xsl:choose>
      </text:p>
    </table:table-cell>
    <!--changed by sonata for bug no:2107193-->
    <!--<xsl:if test="preceding-sibling::c:ser">
      <xsl:for-each select="preceding-sibling::c:ser[1]">
        <xsl:call-template name="InsertHeaderRowsReverse">
          <xsl:with-param name="count" select="$count + 1"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:if>-->
		<!--changed by sonata for bug no:2594979-->
		<xsl:if test="parent::node()[name()='c:areaChart']/c:grouping[@val='standard']">
			<xsl:if test="preceding-sibling::c:ser">
				<xsl:for-each select="preceding-sibling::c:ser[1]">
					<xsl:call-template name="InsertHeaderRowsReverse">
						<xsl:with-param name="count" select="$count + 1"/>
					</xsl:call-template>
				</xsl:for-each>
			</xsl:if>
		</xsl:if>
    <!--end-->
  </xsl:template>

  <xsl:template name="InsertDataRows">
    <!-- @Description: Outputs chart data values -->
    <!-- @Context: inside input chart file -->

    <xsl:param name="points"/>
    <!-- (number) maximum number of data points in series -->
    <xsl:param name="categories"/>
    <!-- (number) number of categories -->
    <xsl:param name="count" select="0"/>
    <!-- (number) loop counter -->
    <xsl:param name="reverseCategories"/>
    <xsl:param name="reverseSeries"/>
    <!--changed by sonata for bug no:2107193-->
    <xsl:param name="pointIndex"/>

    <xsl:variable name="categoryNumber">
      <xsl:choose>
        <xsl:when test="$reverseCategories = 'true' ">
          <xsl:value-of select="$points - $count - 1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$count"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$count &lt; $points">
        <table:table-row>
          <!-- category name -->
          <table:table-cell office:value-type="string">
            <text:p>
              <xsl:choose>
                <xsl:when test="not($categories)">
                  <xsl:value-of select="$categoryNumber + 1"/>
                </xsl:when>
                <xsl:when test="$count &lt; $categories">
                  <!--changed by sonata for bug no:2152759-->
                  <xsl:choose>
                    <!--condition is modified for the file Pie_Chart_Legend.xlsx-->
                    <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:barChart/c:barDir/@ val='bar' or key('plotArea', c:chartSpace/@oox:part)/c:bar3DChart or key('plotArea', c:chartSpace/@oox:part)/c:pieChart">
                      <xsl:value-of
                                        select="key('categories', c:chartSpace/@oox:part)/descendant::c:pt[@idx= $count]"/>
                    </xsl:when>
                    <xsl:otherwise>
                  <xsl:value-of
                    select="key('categories', c:chartSpace/@oox:part)/descendant::c:pt[@idx= $categoryNumber]"/>
                    </xsl:otherwise>
                  </xsl:choose>
                  <!--end-->


                </xsl:when>
              </xsl:choose>
            </text:p>
          </table:table-cell>

          <!-- insert cell responding to axis x value -->
          <xsl:if test="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart or key('plotArea', c:chartSpace/@oox:part)/c:bubbleChart">
            <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[position() = 1]">
              <xsl:choose>
                <xsl:when test="c:xVal">
                  <table:table-cell>
                    <!--Start of RefNo-1: ODF1.1 Changes to avoid office:value when office:value-type="string"-->
                    <!--office:value-type="float"
                    office:value="{c:xVal/descendant::c:pt[@idx = $categoryNumber]/c:v}">-->
                    <!--Sonata: fix for defect 2168527 XLSX-roundtrip: Data contend of all charts lost-->
                    <xsl:variable name="value" select="c:xVal/descendant::c:pt[@idx = $categoryNumber]/c:v"/>                      
                      <xsl:choose>
                        <xsl:when test="c:xVal/c:strRef">
                          <xsl:attribute name="office:value-type">
                          <xsl:value-of select="'string'"/>
                          </xsl:attribute>                         
                        </xsl:when>
                        <xsl:when test="c:xVal/c:numRef">
                          <xsl:attribute name="office:value-type">
                          <xsl:value-of select="'float'"/>
                          </xsl:attribute>                          
                          <xsl:attribute name="office:value">
                            <xsl:choose>
                              <xsl:when test="$value != ''">
                                <xsl:value-of select="$value"/>
                              </xsl:when>
                              <xsl:otherwise>
                                <xsl:value-of select="'0'"/>
                              </xsl:otherwise>
                              </xsl:choose>
                            </xsl:attribute>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:attribute name="office:value-type">
                          <xsl:value-of select="'float'"/>
                          </xsl:attribute>
                          <xsl:attribute name="office:value">
                            <xsl:choose>
                              <xsl:when test="$value != ''">
                                <xsl:value-of select="$value"/>
                        </xsl:when>
                              <xsl:otherwise>
                                <xsl:value-of select="'0'"/>
                              </xsl:otherwise>
                      </xsl:choose>
                    </xsl:attribute>
                        </xsl:otherwise>
                      </xsl:choose>                    
                    <text:p>
                      <xsl:choose>
                        <xsl:when test="c:xVal/c:strRef">
                          <xsl:value-of select="$value"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:choose>
                            <xsl:when test="$value != ''">
                              <xsl:value-of select="$value"/>
                            </xsl:when>
                            <xsl:otherwise>
                              <xsl:value-of select="'0'"/>
                            </xsl:otherwise>
                          </xsl:choose>
                        </xsl:otherwise>
                      </xsl:choose>
                    </text:p>
                    <!--End of RefNo-1-->
                  </table:table-cell>
                </xsl:when>
                <xsl:otherwise>
                  <table:table-cell office:value-type="float" office:value="{$categoryNumber + 1}">
                    <text:p>
                      <xsl:value-of select="$categoryNumber + 1"/>
                    </text:p>
                  </table:table-cell>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </xsl:if>

          <!-- category values -->
          <xsl:choose>
            <xsl:when test="$reverseSeries = 'true' ">
              <xsl:call-template name="InsertValuesReverse">
                <xsl:with-param name="point" select="$categoryNumber"/>
                <!--changed by sonata for bug no:2107193-->
                <xsl:with-param name="pointIndex" select="$pointIndex+1"/>

              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="InsertValues">
                <xsl:with-param name="point" select="$categoryNumber"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </table:table-row>

        <xsl:call-template name="InsertDataRows">
          <xsl:with-param name="points" select="$points"/>
          <xsl:with-param name="categories" select="$categories"/>
          <xsl:with-param name="count" select="$count + 1"/>
          <!--changed by sonata for bug no:2107193-->
          <xsl:with-param name="pointIndex" select="$pointIndex+1"/>

          <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
          <xsl:with-param name="reverseSeries" select="$reverseSeries"/>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertValues">
    <!-- @Description: Outputs chart data values for selected series -->
    <!-- @Context: inside input chart file -->

    <xsl:param name="point"/>
    <!-- (number) series number -->

    <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">
      <xsl:choose>
        <xsl:when test="c:val/descendant::c:pt[@idx = $point]">
          <table:table-cell office:value-type="float"
            office:value="{c:val/descendant::c:pt[@idx = $point]/c:v}">
            <text:p>
              <xsl:value-of select="c:val/descendant::c:pt[@idx = $point]/c:v"/>
            </text:p>
          </table:table-cell>
        </xsl:when>
        <!-- values to scatter chart -->
        <xsl:when test="c:yVal/descendant::c:pt[@idx = $point]">
          <table:table-cell office:value-type="float"
            office:value="{c:yVal/descendant::c:pt[@idx = $point]/c:v}">
            <text:p>
              <xsl:value-of select="c:yVal/descendant::c:pt[@idx = $point]/c:v"/>
            </text:p>
          </table:table-cell>
        </xsl:when>
        <xsl:otherwise>
          <!--Start of RefNo-1:ODF1.1:Avoid office:value="1.#NAN", fix for 2632796
              the type for 1.NaN is not specified, which solves both the defect and Conformance-->
			<!--<table:table-cell office:value-type="float" office:value="1.#NAN">
            <text:p>1.#NAN</text:p>
          </table:table-cell>-->          
          <!--<table:table-cell>
            <text:p>1.#NAN</text:p>
          </table:table-cell>-->
			<table:table-cell>
            <text:p></text:p>
          </table:table-cell>          
          <!--End of RefNo-1-->
        </xsl:otherwise>		  
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertValuesReverse">
    <!-- @Description: Outputs chart data values for selected series -->
    <!-- @Context: inside input chart file -->

    <xsl:param name="point"/>
    <!-- (number) series number -->
    <xsl:param name="count" select="1"/>
		<!--changed by sonata for bug no:2594979-->
		<xsl:param name="positionCount" select="0"/>
		<!--end-->
<xsl:param name="pointIndex"/>

    <xsl:variable name="noOfSeries">
      <xsl:value-of select="count(key('dataSeries', c:chartSpace/@oox:part))"/>
    </xsl:variable>

		<!--changed by sonata for bug no:2594979-->
    <!--changed by sonata for bug no:2107193-->
		<xsl:choose>
			<xsl:when test="key('dataSeries', c:chartSpace/@oox:part)/parent::node()[name()='c:areaChart']/c:grouping[@val='standard']">
				<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[last() - $positionCount]">
					<xsl:choose>
						<xsl:when test="c:val/descendant::c:pt[@idx = $point]">
							<table:table-cell office:value-type="float"
							  office:value="{c:val/descendant::c:pt[@idx = $point]/c:v}">
								<text:p>
									<xsl:value-of select="c:val/descendant::c:pt[@idx = $point]/c:v"/>
								</text:p>
							</table:table-cell>
						</xsl:when>
						<xsl:otherwise>
<!--Start of RefNo-1:ODF1.1:Avoid office:value="1.#NAN", fix for 2632796
              the type for 1.NaN is not specified, which solves both the defect and Conformance-->
							<table:table-cell>
								<text:p></text:p>
							</table:table-cell>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:for-each>

				<xsl:if test="key('dataSeries', c:chartSpace/@oox:part)[last() - $positionCount - 1]">
					<xsl:call-template name="InsertValuesReverse">
						<xsl:with-param name="point" select="$point"/>
						<xsl:with-param name="positionCount" select="$positionCount + 1"/>
					</xsl:call-template>
				</xsl:if>
			</xsl:when>
			<xsl:otherwise>
    <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[position() = $count]">   
      <xsl:choose>
        <xsl:when test="c:val/descendant::c:pt[@idx = $pointIndex]">
          <table:table-cell office:value-type="float"
            office:value="{c:val/descendant::c:pt[@idx = $pointIndex]/c:v}">
            <text:p>
              <xsl:value-of select="c:val/descendant::c:pt[@idx = $pointIndex]/c:v"/>
            </text:p>
          </table:table-cell>
        </xsl:when>
        <xsl:otherwise>
          <!--Start of RefNo-1:ODF1.1:Avoid office:value="1.#NAN", fix for 2632796
              the type for 1.NaN is not specified, which solves both the defect and Conformance-->
          <table:table-cell>
            <text:p></text:p>
          </table:table-cell>
          <!--End of RefNo-1-->
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
    <!--end-->


    <xsl:if test="$count + 1 &lt;= $noOfSeries">
      <!--<xsl:if test="key('dataSeries', c:chartSpace/@oox:part)[$count+1]">-->

    <xsl:call-template name="InsertValuesReverse">
        <xsl:with-param name="point" select="$point"/>
        <xsl:with-param name="count" select="$count + 1"/>
        <!--changed by sonata for bug no:2107193-->
        <xsl:with-param name="pointIndex" select="$pointIndex"/>
      </xsl:call-template>
    </xsl:if>
			</xsl:otherwise>
		</xsl:choose>
		<!--end-->

  </xsl:template>

  <xsl:template name="InsertTitle">
    <!-- @Description: Outputs chart title -->
    <!-- @Context: input chart file root -->

    <xsl:choose>
      <!-- title is set by user -->
      <xsl:when test="c:chartSpace/c:chart/c:title/c:tx">
        <chart:title svg:x="3.834cm" svg:y="0.184cm" chart:style-name="chart_title">
          <text:p>
            <xsl:for-each select="c:chartSpace/c:chart/c:title/c:tx/c:rich/a:p">
              <!-- [ENTER] -->
              <xsl:if test="preceding-sibling::node()[1][name() = 'a:p']">
                <xsl:value-of select="'&#xD;&#xA;'"/>
              </xsl:if>

              <xsl:for-each select="a:r">
                <xsl:value-of select="a:t"/>
              </xsl:for-each>
            </xsl:for-each>
          </text:p>
        </chart:title>
      </xsl:when>

      <!-- one series chart default title is first series name -->
      <!--changed by sonata for bug no:2174314-->
      <xsl:when
        test="c:chartSpace/c:chart/c:title and count(key('dataSeries', c:chartSpace/@oox:part)) = 1">
        <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)/c:tx/descendant::c:v[1]">
          <chart:title svg:x="3.834cm" svg:y="0.184cm" chart:style-name="chart_title">
            <text:p>
              <xsl:value-of select="text()"/>
            </text:p>
          </chart:title>
        </xsl:for-each>
      </xsl:when>

      <xsl:when test="c:chartSpace/c:chart/c:title">
        <chart:title svg:x="3cm" svg:y="0.14cm" chart:style-name="chart_title">
          <text:p>
            <xsl:text>Chart Title</xsl:text>
          </text:p>
        </chart:title>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertLegend">
    <!-- @Description: Outputs chart legend -->
    <!-- @Context: input chart file root -->

    <xsl:for-each select="c:chartSpace/c:chart/c:legend">
      <chart:legend chart:legend-position="end" chart:style-name="legend">
        <!-- legend position -->
        <xsl:if test="c:legendPos/@val != '' ">
          <xsl:attribute name="chart:legend-position">
            <xsl:choose>
              <xsl:when test="c:legendPos/@val = 't' ">
                <xsl:text>top</xsl:text>
              </xsl:when>
              <xsl:when test="c:legendPos/@val = 'b' ">
                <xsl:text>bottom</xsl:text>
              </xsl:when>
              <xsl:when test="c:legendPos/@val = 'l' ">
                <xsl:text>start</xsl:text>
              </xsl:when>
              <xsl:when test="c:legendPos/@val = 'r' ">
                <xsl:text>end</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>
      </chart:legend>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertXAxis">
    <!-- @Description: Outputs chart X Axis -->
    <!-- @Context: axis element (c:catAx, c:dateAx, c:AxPos) -->
    <xsl:param name="type"/>

    <chart:axis chart:dimension="x" chart:name="primary-x" chart:style-name="axis-x">

      <xsl:choose>
        <!-- title is set by user -->
        <xsl:when test="c:title/c:tx">
          <chart:title chart:style-name="axis-x_title">
            <text:p>
              <xsl:for-each select="c:title/c:tx/c:rich/a:p">
                <!-- [ENTER] -->
                <xsl:if test="preceding-sibling::node()[1][name() = 'a:p']">
                  <xsl:value-of select="'&#xD;&#xA;'"/>
                </xsl:if>

                <xsl:for-each select="a:r">
                  <xsl:value-of select="a:t"/>
                </xsl:for-each>
              </xsl:for-each>

            </text:p>
          </chart:title>
        </xsl:when>
        
        <xsl:when test="parent::c:valAx/c:title/c:tx">
          <chart:title chart:style-name="axis-x_title">
            <text:p>
              
              <xsl:for-each select="parent::c:valAx/c:title/c:tx/c:rich/a:p">
                
                <!-- [ENTER] -->
                <xsl:if test="preceding-sibling::node()[1][name() = 'a:p']">
                  <xsl:value-of select="'&#xD;&#xA;'"/>
                </xsl:if>
                
                <xsl:for-each select="a:r">
                  <xsl:value-of select="a:t"/>
                </xsl:for-each>
              </xsl:for-each>
              
            </text:p>
          </chart:title>
        </xsl:when>

        <!-- default Axis-X title -->
        <xsl:when test="c:title">
          <chart:title chart:style-name="axis-x_title">
            <text:p>
              <xsl:text>Axis Title</xsl:text>
            </text:p>
          </chart:title>
        </xsl:when>
      </xsl:choose>
      <!--start of RefNo-1: ODF1.1 :Moved chart chart:categories tag above chart:grid -->
      <xsl:variable name="points">
        <xsl:choose>
          <!-- for scatter chart -->
          <xsl:when test="key('xNumPoints', ancestor::c:chartSpace/@oox:part)/descendant::c:ptCount">
            <xsl:value-of select="key('xNumPoints', ancestor::c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
          </xsl:when>
					<!--Chavi,SP2,Adding X and Y axis in Pie Chart-->
					<xsl:when test ="c:chartSpace/c:chart/c:plotArea/c:pieChart">
						<xsl:value-of select ="c:chartSpace/c:chart/c:plotArea/c:pieChart/c:ser/c:val/descendant::c:ptCount/@val"/>
					</xsl:when>
					<xsl:when test ="c:chartSpace/c:chart/c:plotArea/c:pie3DChart">
						<xsl:value-of select ="c:chartSpace/c:chart/c:plotArea/c:pie3DChart/c:ser/c:val/descendant::c:ptCount/@val"/>
					</xsl:when>
					<!--Chavi,SP2,Adding X and Y axis in Pie Chart end-->
          <xsl:otherwise>
            <xsl:value-of select="key('numPoints', ancestor::c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <chart:categories>
		  <xsl:variable name ="apos">
			  <xsl:text >&apos;</xsl:text>
		  </xsl:variable>
        <!--RefNo-1:ODF1.1:Avoided attribute if wrong address-->
			<!-- Defect:  2682600 , 2186921 
                             Code Changes By : Vijayeta
                             Desc:           : On Reverse converssion, the source data range is not retained in ODF
                                               Additional attributes added to maintain the link between chart abd source data.
             -->	  
				<xsl:choose>
					<xsl:when test="1 + $points = 'NaN'">
        <xsl:attribute name="table:cell-range-address">
          <xsl:value-of select="concat('local-table.A2:.A',1 + $points)"/>
        </xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:variable name ="formulaRange">
							<xsl:choose>
								<xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:pieChart or  key('plotArea', c:chartSpace/@oox:part)/c:pie3DChart">
									<xsl:value-of select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:cat//c:f"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:choose>
										<xsl:when test="./parent::node()/c:scatterChart">
											<xsl:value-of select ="./parent::node()//c:ser[position()=1]/c:xVal//c:f"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select ="./parent::node()//c:ser[position()=1]/c:cat//c:f"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>

						<xsl:variable name ="tempRangeVal">
							<xsl:choose>
								<xsl:when test="(starts-with($formulaRange, '(') or 
								  contains(substring($formulaRange,string-length($formulaRange)-1),')')) and 
								  contains($formulaRange,',') and 
								  contains(substring-after($formulaRange,'!'),',')">
									<xsl:value-of select="substring(substring-before($formulaRange,','),2)"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$formulaRange"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:variable>
						<!--<xsl:variable name="apos">
							<xsl:text>&apos;</xsl:text>
						</xsl:variable>-->
						<xsl:variable name="sheetname">
							<xsl:choose>
								<xsl:when test ="starts-with($formulaRange,$apos)">
									<xsl:variable name ="temp">
										<xsl:value-of select="substring(substring-before($tempRangeVal,'!'),2)"/>
									</xsl:variable>
									<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="substring-before($tempRangeVal,'!')"/>
								</xsl:otherwise>
							</xsl:choose>

						</xsl:variable>
						<!--added for bug no:2557071-->

						<xsl:variable name ="sheetNames">
							<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
								<xsl:if test="@name = $sheetname">
									<xsl:variable name="checkedName">
										<xsl:call-template name="CheckSheetName">
											<xsl:with-param name="sheetNumber">
												<xsl:value-of select="position()"/>
											</xsl:with-param>
											<xsl:with-param name="name">
												<xsl:value-of select="@name"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:variable>
									<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
									<xsl:value-of select ="$checkedName"/>
        </xsl:if>
							</xsl:for-each>
						</xsl:variable>
						<xsl:variable name ="temp1">
							<xsl:value-of select ="concat($apos,$sheetNames,$apos,'!',substring-after($tempRangeVal,'!'))"/>
						</xsl:variable>
						<xsl:variable name="rangeVal">
							<xsl:call-template name="ConvertCellRange">
								<xsl:with-param name="cellRange" select="$temp1"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:if test="not(contains($formulaRange,'#REF!')) and $sheetNames!=''">
							<xsl:attribute name="table:cell-range-address">
								<xsl:choose>
									<xsl:when test="contains($rangeVal,':')">
										<xsl:value-of select="concat(substring-before($rangeVal,':'),':.',substring-after($rangeVal,':'))"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$rangeVal"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:attribute>
						</xsl:if>						
					</xsl:otherwise>
				</xsl:choose>
				<!--End of code change for defects  2682600 and  2186921 -->
				<!--<xsl:if test="1 + $points = 'NaN'">
					<xsl:attribute name="table:cell-range-address">
						<xsl:value-of select="concat('local-table.A2:.A',1 + $points)"/>
					</xsl:attribute>
				</xsl:if>-->
      </chart:categories>
      <!--End of RefNo-1-->
      <xsl:if test="c:majorGridlines">
        <chart:grid chart:style-name="majorGridX" chart:class="major"/>
      </xsl:if>
      <xsl:if test="c:minorGridlines">
        <chart:grid chart:style-name="minorGridX" chart:class="minor"/>
      </xsl:if>
    </chart:axis>

  </xsl:template>

  <xsl:template name="InsertYAxis">
    <xsl:param name="primaryOrSecondary">
      <xsl:text>primary-y</xsl:text>
    </xsl:param>
    <!-- @Description: Outputs chart X Axis -->
    <!-- @Context: inside input chart file -->

    <chart:axis chart:dimension="y" chart:name="primary-y" chart:style-name="axis-y">
     
      <xsl:if test="$primaryOrSecondary = 'secondary-y'">
        <xsl:attribute name="chart:name">
          <xsl:text>secondary-y</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="chart:style-name">
          <xsl:text>secondary_axis-y</xsl:text>
        </xsl:attribute>
      </xsl:if>
      <xsl:choose>
        <!-- title is set by user -->
        <xsl:when test="c:title/c:tx">
          <chart:title chart:style-name="axis-y_title">
            <text:p>
              <xsl:for-each select="c:title/c:tx/c:rich/a:p">
                <!-- [ENTER] -->
                <xsl:if test="preceding-sibling::node()[1][name() = 'a:p']">
                  <xsl:value-of select="'&#xD;&#xA;'"/>
                </xsl:if>

                <xsl:for-each select="a:r">
                  <xsl:value-of select="a:t"/>
                </xsl:for-each>
              </xsl:for-each>

            </text:p>
          </chart:title>
        </xsl:when>

        <xsl:when test="c:title">
          <chart:title chart:style-name="axis-y_title">
            <text:p>
              <xsl:text>Axis Title</xsl:text>
            </text:p>
          </chart:title>
        </xsl:when>
      </xsl:choose>
      <!--Sonata:2016097-Gridlines for secondary axis is not supported in calc-->
      <xsl:if test="$primaryOrSecondary != 'secondary-y'">
      <xsl:if test="c:majorGridlines">
        <chart:grid chart:style-name="majorGridY" chart:class="major"/>
      </xsl:if>
      </xsl:if>
      

      <!--test number format of the axis text-->
      <xsl:if test="c:numFmt">
        <xsl:variable name="formatcode" select="@formatCode"/>
      </xsl:if>
      <!--Sonata:2016097-Gridlines for secondary axis is not supported in calc-->
      <xsl:if test="$primaryOrSecondary != 'secondary-y'">
      <xsl:if test="c:minorGridlines">
        <chart:grid chart:style-name="minorGridY" chart:class="minor"/>
      </xsl:if>
      </xsl:if>
    </chart:axis>

  </xsl:template>

  <xsl:template name="InsertChartType">
    <!-- @Description: Inserts desired type of chart -->
    <!-- @Context: input chart file root -->
		<xsl:param name="chartRelId"/>
		<xsl:param name="sheetname"/>
		<xsl:param name="drawingname"/>

    <chart:chart svg:width="13.066cm" svg:height="9.279cm" chart:class="chart:bar"
      chart:style-name="chart">

			<!--calculating chart height-->
			<!--<xsl:for-each
         select="key('Part', concat('xl/',substring-after($drawingname,'/')))/xdr:wsDr/xdr:twoCellAnchor">-->
			<!--<xsl:for-each
      select="key('Part', concat('xl/',substring-after($drawingname,'/')))/xdr:wsDr/xdr:twoCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">-->
			<xsl:for-each
		   select="key('Part', concat('xl/',substring-after($drawingname,'/')))/xdr:wsDr">
				<xsl:choose>
					<xsl:when test="xdr:twoCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
						<xsl:for-each
					select="xdr:twoCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
							<xsl:for-each select="parent::node()/parent::node()/parent::node()/parent::node()">

								<xsl:variable name="startrowno" select="xdr:from/xdr:row">
								</xsl:variable>
								<xsl:variable name="startrowoffset">
									<xsl:value-of select="xdr:from/xdr:rowOff"/>
								</xsl:variable>
								<xsl:variable name="endrowno" select="xdr:to/xdr:row">
								</xsl:variable>
								<xsl:variable name="endrowoffset">
									<xsl:value-of select="xdr:to/xdr:rowOff"/>
								</xsl:variable>
								<xsl:variable name="startrowoffsetincm">
									<xsl:call-template name="ConvertEmu">
										<xsl:with-param name="length" select="$startrowoffset"/>
										<xsl:with-param name="unit" select="'cm'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="endrowoffsetincm">
									<xsl:call-template name="ConvertEmu">
										<xsl:with-param name="length" select="$endrowoffset"/>
										<xsl:with-param name="unit" select="'cm'"/>
									</xsl:call-template>
								</xsl:variable>

								<xsl:variable name="startcolno" select="xdr:from/xdr:col">

								</xsl:variable>
								<xsl:variable name="startcoloffset">
									<xsl:call-template name="ConvertEmu">
										<xsl:with-param name="length" select="xdr:from/xdr:colOff"/>
										<xsl:with-param name="unit" select="'cm'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="endcolno" select="xdr:to/xdr:col">
								</xsl:variable>
								<xsl:variable name="endcoloffset">
									<xsl:call-template name="ConvertEmu">
										<xsl:with-param name="length" select="xdr:to/xdr:colOff"/>
										<xsl:with-param name="unit" select="'cm'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:for-each select="key('Part', concat('xl/',$sheetname))/e:worksheet/e:drawing">
									<xsl:variable name="defaultrowheight">
										<xsl:value-of select="parent::node()/child::node()[name()='sheetFormatPr']/@defaultRowHeight"/>
									</xsl:variable>
									<xsl:variable name="chartrowheight">
										<xsl:call-template name="CalculateRowHeight">
											<xsl:with-param name="sheetname" select="$sheetname"/>
											<xsl:with-param name="startrowno" select="$startrowno"/>
											<xsl:with-param name="endrowno" select="$endrowno +1"/>
											<xsl:with-param name="total" select="0"/>
											<xsl:with-param name="defaultrowheight" select="$defaultrowheight"/>
											<xsl:with-param name="countchartrow" select="$startrowno + 1"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:variable name="chartrowheightincm">
										<xsl:call-template name="ConvertToCentimeters">
											<xsl:with-param name="length">
												<xsl:value-of
												  select="concat($chartrowheight,'pt')"
                      />
											</xsl:with-param>
										</xsl:call-template>
									</xsl:variable>
									<xsl:variable name="finalchartrowheight">
										<xsl:value-of select="concat((substring-before($chartrowheightincm,'cm') - substring-before($startrowoffsetincm,'cm') + substring-before($endrowoffsetincm,'cm')),'cm')"/>
									</xsl:variable>
									<xsl:attribute name="svg:height">
										<xsl:value-of select="$finalchartrowheight"/>
									</xsl:attribute>

									<!--calculating coloumnwidth-->
									<xsl:variable name="defaultFontSize">
										<xsl:for-each select="key('Part', 'xl/styles.xml')">
											<xsl:choose>
												<xsl:when test="e:styleSheet/e:fonts/e:font">
													<xsl:value-of select="e:styleSheet/e:fonts/e:font[1]/e:sz/@val"/>
												</xsl:when>
												<xsl:otherwise>
													<xsl:text>11</xsl:text>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:for-each>
									</xsl:variable>
									<xsl:variable name="defaultFontStyle">
										<xsl:for-each select="key('Part', 'xl/styles.xml')">
											<xsl:choose>
												<xsl:when test="e:styleSheet/e:fonts/e:font">
													<xsl:value-of select="e:styleSheet/e:fonts/e:font[1]/e:name/@val"/>
												</xsl:when>
												<xsl:otherwise>
													<xsl:text>11</xsl:text>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:for-each>
									</xsl:variable>



									<xsl:variable name="defaultcolWidth">
										<xsl:choose>
											<xsl:when test="parent::node()/child::node()[name()='sheetFormatPr']/@defaultColWidth">

												<xsl:value-of
												   select="parent::node()/child::node()[name()='sheetFormatPr']/@defaultColWidth"/>



											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="'null'"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:variable>

									<xsl:variable name="colWidPlusdefaultColCount">
										<!--<xsl:if test="parent::node()/child::node()[name()='cols']">-->
										<xsl:call-template name="CalculateColWidth">
											<xsl:with-param name="sheetname" select="$sheetname"/>
											<xsl:with-param name="startcolno" select="$startcolno"/>
											<xsl:with-param name="endcolno" select="$endcolno +1"/>
											<xsl:with-param name="total" select="0"/>
											<xsl:with-param name="defaultcolWidth" select="$defaultcolWidth"/>
											<xsl:with-param name="countchartcol" select="$startcolno + 1"/>
											<xsl:with-param name="totalDefaultColCount"/>
											<xsl:with-param name="startcoloffset" select="$startcoloffset"/>
											<xsl:with-param name="endcoloffset" select="$endcoloffset"/>
										</xsl:call-template>


										<!--</xsl:if>-->


									</xsl:variable>
									<xsl:variable name="customColWid">
										<xsl:variable name="customColWidPt">
											<xsl:value-of select="substring-before($colWidPlusdefaultColCount,'|')"/>
										</xsl:variable>
            <xsl:choose>
                      <xsl:when test="$customColWidPt = 0 or $customColWidPt = ''">
                        <xsl:value-of select="'0'"/>
              </xsl:when>
              <xsl:otherwise>
                        <xsl:value-of select="$customColWidPt"/>
              </xsl:otherwise>
            </xsl:choose>
                  </xsl:variable>
									<xsl:variable name="defaultColCount">
								<xsl:value-of select="substring-after($colWidPlusdefaultColCount,'|')"/>
									</xsl:variable>
									<xsl:attribute name="svg:width">
										<xsl:choose>
                      <xsl:when test="$defaultcolWidth != 'null' and $customColWid =0 ">
												<xsl:variable name="totalDefWid">
													<xsl:value-of select="number($defaultColCount) * number($defaultcolWidth) "/>
												</xsl:variable>
												<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|','0','|',$totalDefWid,'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
																				</xsl:when>
                      <xsl:when test="$defaultcolWidth != 'null' and $customColWid !=0">
												<xsl:variable name="totalDefWid">
                          <xsl:value-of select="number($defaultColCount) * number($defaultcolWidth)+ $customColWid"/>
												</xsl:variable>
												<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|','0','|',$totalDefWid,'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
											</xsl:when>
											<xsl:otherwise>
                        <xsl:if test ="$customColWid !=0">
                          <xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|',$defaultColCount,'|',$customColWid,'|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
												</xsl:if>
                        <xsl:if test ="$customColWid =0">
													<xsl:value-of select="concat('sonataChartWidth:','False|',$defaultFontStyle,'|',$defaultFontSize,'|',$defaultColCount,'|','0','|',substring-before($startcoloffset,'cm'),'|',substring-before($endcoloffset,'cm'))"/>
												</xsl:if>
																	</xsl:otherwise>
										</xsl:choose>
										<!-- Call Post Processor Here-->
									</xsl:attribute>

								</xsl:for-each>

							</xsl:for-each>
						</xsl:for-each>
					</xsl:when>
					<xsl:when test="xdr:oneCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
            <xsl:for-each select ="xdr:oneCellAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
              <xsl:for-each select="parent::node()/parent::node()/parent::node()/parent::node()">
						<xsl:choose>
							<xsl:when test="number(xdr:ext/@cx)=0 and number(xdr:ext/@cy)=0">
								<xsl:attribute name="svg:width">
									<xsl:value-of select="'12.7cm'"/>
								</xsl:attribute>
								<xsl:attribute name="svg:height">
									<xsl:value-of select="'7.62cm'"/>
								</xsl:attribute>
							</xsl:when>
							<xsl:when test="number(xdr:ext/@cx)!=0 and number(xdr:ext/@cy)!=0">
								<xsl:call-template name="InsertAbsoluteSize"/>
							</xsl:when>
						</xsl:choose>
              </xsl:for-each>
              </xsl:for-each>					
					</xsl:when>
					<xsl:when test="xdr:absoluteAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
						<xsl:for-each select="xdr:absoluteAnchor/xdr:graphicFrame/a:graphic/a:graphicData/c:chart[@r:id=$chartRelId]">
							<xsl:for-each select="parent::node()/parent::node()/parent::node()/parent::node()">
								<xsl:call-template name="InsertAbsoluteSize"/>
							</xsl:for-each>
						</xsl:for-each>
					</xsl:when>
				</xsl:choose>
			</xsl:for-each>

			<!--end-->
      <xsl:variable name="chartType">
        <xsl:choose>
          <!-- for Stock Chart type 3 and type 4-->
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart and key('plotArea', c:chartSpace/@oox:part)/c:barChart">
            <xsl:text>chart:stock</xsl:text>
          </xsl:when>

          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:barChart or key('plotArea', c:chartSpace/@oox:part)/c:bar3DChart">
            <xsl:text>chart:bar</xsl:text>
          </xsl:when>

          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:lineChart or key('plotArea', c:chartSpace/@oox:part)/c:line3DChart">
            <xsl:text>chart:line</xsl:text>
          </xsl:when>

          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:areaChart or key('plotArea', c:chartSpace/@oox:part)/c:area3DChart">
            <xsl:text>chart:area</xsl:text>
          </xsl:when>

          <!--  or key('plotArea', c:chartSpace/@oox:part)/c:ofPieChart or key('plotArea', c:chartSpace/@oox:part)/c:doughnutChart -->
          <xsl:when
            test="key('plotArea', c:chartSpace/@oox:part)/c:pieChart or key('plotArea', c:chartSpace/@oox:part)/c:pie3DChart or key('plotArea', c:chartSpace/@oox:part)/c:ofPieChart">
            <xsl:text>chart:circle</xsl:text>
          </xsl:when>

          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:radarChart">
            <xsl:text>chart:radar</xsl:text>
          </xsl:when>

          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:doughnutChart">
            <xsl:text>chart:ring</xsl:text>
          </xsl:when>

          <!-- making problems at this time -->
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart or key('plotArea', c:chartSpace/@oox:part)/c:bubbleChart">
            <xsl:text>chart:scatter</xsl:text>
          </xsl:when>


          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart">
            <xsl:text>chart:stock</xsl:text>
          </xsl:when>

          <xsl:otherwise>
            <xsl:text>chart:bar</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:attribute name="chart:class">
        <xsl:value-of select="$chartType"/>
      </xsl:attribute>

      <xsl:call-template name="InsertTitle"/>
      <xsl:call-template name="InsertLegend"/>

      <!--RefNo-1:ODF1.1:removed chart:table-number-list-->
      <!--<chart:plot-area chart:style-name="plot_area" chart:table-number-list="0" svg:x="0.26cm"  svg:y="2.087cm" svg:width="10.472cm" svg:height="7.008cm">-->
			<chart:plot-area chart:style-name="plot_area" svg:x="0.26cm"  svg:y="2.087cm" chart:data-source-has-labels="both">
			<!-- Defect:  2682600 , 2186921 
                             Code Changes By : Vijayeta
                             Desc:           : On Reverse converssion, the source data range is not retained in ODF
                                               Additional attributes added to maintain the link between chart abd source data.
                         -->
				<xsl:if test ="not(key('plotArea', c:chartSpace/@oox:part)/c:doughnutChart) and not(key('plotArea', c:chartSpace/@oox:part)/c:scatterChart)">
					<xsl:variable name="apos">
						<xsl:text>&apos;</xsl:text>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test ="starts-with(key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:cat//c:f,'(') and contains(key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:cat//c:f,',')">
							<xsl:variable name ="startRange">
								<xsl:choose>
									<xsl:when test ="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart">
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:xVal//c:f">
											<xsl:call-template name ="GetStartMultipleRanges">
												<xsl:with-param name ="multRanges">
													<xsl:value-of select ="."/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:for-each>
									</xsl:when>
									<xsl:otherwise>
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:cat//c:f">
											<xsl:call-template name ="GetStartMultipleRanges">
												<xsl:with-param name ="multRanges">
													<xsl:value-of select ="."/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:for-each>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name ="endRange">
								<xsl:choose>
									<xsl:when test ="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart">
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[last()]/c:yVal//c:f">
											<xsl:call-template name ="GetEndMultipleRanges">
												<xsl:with-param name ="multRanges">
													<xsl:value-of select ="."/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:for-each>
									</xsl:when>
									<xsl:otherwise>
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[last()]/c:val//c:f">
											<xsl:call-template name ="GetEndMultipleRanges">
												<xsl:with-param name ="multRanges">
													<xsl:value-of select ="."/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:for-each>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name="sheetname1">
								<xsl:choose>
									<xsl:when test ="starts-with(substring-before($startRange,'!'),$apos)">
										<xsl:variable name ="temp">
											<xsl:value-of select="substring(substring-before($startRange,'!'),2)"/>
										</xsl:variable>
										<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="substring-before($startRange,'!')"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name ="sheetNames">
								<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
									<xsl:if test="@name = $sheetname1">
										<xsl:variable name="checkedName">
											<xsl:call-template name="CheckSheetName">
												<xsl:with-param name="sheetNumber">
													<xsl:value-of select="position()"/>
												</xsl:with-param>
												<xsl:with-param name="name">
													<xsl:value-of select="@name"/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:variable>
										<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
										<xsl:value-of select ="$checkedName"/>
									</xsl:if>
								</xsl:for-each>
							</xsl:variable>
							<xsl:variable name ="temp1">
								<xsl:call-template name ="getCompleteRange">
									<xsl:with-param name ="startRange">
										<xsl:value-of select ="$startRange"/>
									</xsl:with-param>
									<xsl:with-param name ="endRange">
										<xsl:value-of select ="$endRange"/>
									</xsl:with-param>
								</xsl:call-template>
							</xsl:variable>
							<xsl:if test="not(contains($temp1,'#REF!')) and $sheetNames!=''">
								<xsl:attribute name ="table:cell-range-address">
									<xsl:value-of select="$temp1"/>
								</xsl:attribute>
							</xsl:if>
						</xsl:when>
						<xsl:otherwise>
							<xsl:variable name ="startRange">
								<xsl:variable name ="start">
									<xsl:choose>
										<xsl:when test ="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart">
											<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:xVal//c:f">
												<xsl:if test ="contains(.,':')">
													<xsl:value-of select ="substring-before(.,':')"/>
												</xsl:if>
												<xsl:if test ="not(contains(.,':'))">
													<xsl:value-of select ="."/>
												</xsl:if>
											</xsl:for-each>
										</xsl:when>
										<xsl:otherwise>
											<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[position()=1]/c:cat//c:f">											
														<xsl:if test ="contains(.,':')">
															<xsl:value-of select ="substring-before(.,':')"/>
														</xsl:if>
														<xsl:if test ="not(contains(.,':'))">
															<xsl:value-of select ="."/>
														</xsl:if>													
											</xsl:for-each>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:if test ="$start!='' and contains($start,'!') and not(contains($start,'#REF!'))">
									<xsl:variable name ="startCol">
										<xsl:call-template name="NumbersToChars">
											<xsl:with-param name="num">
												<xsl:variable name ="num">
													<xsl:call-template name="GetColNum">
														<xsl:with-param name="cell">
															<xsl:value-of select="translate(substring-after($start,'!'),'$','')"/>
														</xsl:with-param>
													</xsl:call-template>
												</xsl:variable>
												<xsl:value-of select ="$num - 1"/>
											</xsl:with-param>
										</xsl:call-template>
									</xsl:variable>
									<xsl:variable name ="startRow">
										<xsl:call-template name="GetRowNum">
											<xsl:with-param name="cell" select="translate(substring-after($start,'!'),'$','')"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:if test ="$startRow - 1 &gt; 0">
									<xsl:value-of select ="concat(substring-before($start,'!'),'!$',$startCol,'$',$startRow - 1)"/>
								</xsl:if>
									<xsl:if test ="$startRow - 1 = 0">
										<xsl:value-of select ="concat(substring-before($start,'!'),'!$',$startCol,'$',$startRow)"/>
									</xsl:if>
								</xsl:if>
							</xsl:variable>
							<xsl:variable name ="endRange">
								<xsl:choose>
									<xsl:when test ="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart">
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[last()]/c:yVal//c:f">										
													<xsl:if test ="contains(.,':')">
														<xsl:value-of select ="substring-after(.,':')"/>
													</xsl:if>
													<xsl:if test ="not(contains(.,':'))">
														<xsl:value-of select ="."/>
													</xsl:if>											
										</xsl:for-each>
									</xsl:when>
									<xsl:when test ="key('plotArea', c:chartSpace/@oox:part)/c:stockChart">
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)/c:stockChart/c:ser[last()]/c:val//c:f">
											<xsl:if test ="contains(.,':')">
												<xsl:value-of select ="substring-after(.,':')"/>
											</xsl:if>
											<xsl:if test ="not(contains(.,':'))">
												<xsl:value-of select ="."/>
											</xsl:if>
										</xsl:for-each>
									</xsl:when>
									<xsl:otherwise>
										<xsl:for-each select ="key('plotArea', c:chartSpace/@oox:part)//c:ser[last()]/c:val//c:f">
											<xsl:if test ="contains(.,':')">
												<xsl:value-of select ="substring-after(.,':')"/>
											</xsl:if>
											<xsl:if test ="not(contains(.,':'))">
												<xsl:value-of select ="."/>
											</xsl:if>
										</xsl:for-each>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name="sheetname1">
								<xsl:choose>
									<xsl:when test ="starts-with(substring-before($startRange,'!'),$apos)">
										<xsl:variable name ="temp">
											<xsl:value-of select="substring(substring-before($startRange,'!'),2)"/>
										</xsl:variable>
										<xsl:value-of select="substring($temp,1,string-length($temp)-1)"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="substring-before($startRange,'!')"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:variable>
							<xsl:variable name ="sheetNames">
								<xsl:for-each select ="key('Part','xl/workbook.xml')//e:sheets/e:sheet">
									<xsl:if test="@name = $sheetname1">
										<xsl:variable name="checkedName">
											<xsl:call-template name="CheckSheetName">
												<xsl:with-param name="sheetNumber">
													<xsl:value-of select="position()"/>
												</xsl:with-param>
												<xsl:with-param name="name">
													<xsl:value-of select="@name"/>
												</xsl:with-param>
											</xsl:call-template>
										</xsl:variable>
										<!--<xsl:value-of select ="concat(@name,':',$checkedName,'::')"/>-->
										<xsl:value-of select ="$checkedName"/>
									</xsl:if>
								</xsl:for-each>
							</xsl:variable>
							<xsl:variable name ="temp1">
								<xsl:value-of select ="concat($apos,$sheetNames,$apos,'!',substring-after($startRange,'!'),':',$endRange)"/>
							</xsl:variable>
							<xsl:variable name ="Range">
								<xsl:call-template name="ConvertCellRange">
									<xsl:with-param name="cellRange" select="$temp1"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:if test="not(contains($Range,'#REF!')) and $sheetNames!=''">
								<xsl:attribute name ="table:cell-range-address">
									<xsl:choose>
										<xsl:when test="contains($Range,':')">
											<xsl:value-of select="concat(substring-before($Range,':'),':.',substring-after($Range,':'))"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$Range"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:attribute>
							</xsl:if>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:if>
				<!--End of code change for defects  2682600 and  2186921 -->
        <!-- Axes -->
        <xsl:choose>
          <!-- stock chart type 3 and stock chart type 4 -->
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart and key('plotArea', c:chartSpace/@oox:part)/c:barChart">
            <!-- if both category axes are date axis-->
            <xsl:choose>
              <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[2]">
                <!-- insert the one that corresponds to stock chart -->
                <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[2]">
                  <xsl:call-template name="InsertXAxis"/>
                </xsl:for-each>
              </xsl:when>
              <!-- if both are cat axes -->
              <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:catAx[2]">
                <!-- insert the one that corresponds to stock chart -->
   <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
                  <xsl:call-template name="InsertXAxis"/>
                </xsl:for-each>
              </xsl:when>
            </xsl:choose>
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[2]">
              <xsl:call-template name="InsertYAxis">
                <xsl:with-param name="primaryOrSecondary">
                  <xsl:text>secondary-y</xsl:text>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          
          <!-- scatter chart has two value axes -->
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:scatterChart or key('plotArea', c:chartSpace/@oox:part)/c:bubbleChart">
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>
            <xsl:for-each
              select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[c:axPos/@val = 'l' or c:axPos/@val = 'r'][1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>
          </xsl:when>
          
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:stockChart">
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>
            
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>
            
            <xsl:for-each
              select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[c:axPos/@val = 'l' or c:axPos/@val = 'r'][1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>
          </xsl:when>
          
          <!--code added by Mallika for the bug no 2030295 and file name Pilot_Temperatures.xlsx -->
          
          <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:barChart">
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>

            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>

            <!--<xsl:for-each
              select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[c:axPos/@val = 'l' or c:axPos/@val = 'r'][1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>-->

            <xsl:for-each
             select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>
          </xsl:when>
          <!--end of added code-->
					<!--Chavi,SP2,Adding X and Y axis in Pie Chart-->
					<xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:pieChart or  key('plotArea', c:chartSpace/@oox:part)/c:pie3DChart">
						<xsl:call-template name="InsertXAxis"/>
						<xsl:call-template name="InsertYAxis"/>
					</xsl:when>
          
					<!--Chavi,SP2,Adding X and Y axis in Pie Chart,End-->
          <xsl:otherwise>
			  <!--Condition Added By Sateesh-->
			  <!-- Bug#: 1877279, BugDescription: XLSX Roundtrip - Failure on open -->
			  <xsl:choose>
				  <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:catAx[1]">
              <xsl:call-template name="InsertXAxis"/>
            </xsl:for-each>
				  </xsl:when>
				  <xsl:when test="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[1]">
					  <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:dateAx[1]">
						  <xsl:call-template name="InsertXAxis"/>
					  </xsl:for-each>	  
				  </xsl:when>
			  </xsl:choose>
			  <!--End-->
            <xsl:for-each select="key('plotArea', c:chartSpace/@oox:part)/c:valAx[1]">
              <xsl:call-template name="InsertYAxis"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>

        <!-- check if series are reversed -->
        <xsl:variable name="reverseSeries">
          <xsl:call-template name="CheckIfSeriesReversed"/>
        </xsl:variable>

        <!-- check if data points are to be inserted in reverse order -->
        <xsl:variable name="reverseCategories">
          <xsl:call-template name="CheckIfCategoriesReversed"/>
        </xsl:variable>


        <xsl:choose>
          <xsl:when test="$reverseSeries = 'false' ">
            <xsl:call-template name="InsertSeries">
              <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <!--chnaged by sonata for bug no:2107193-->
						<!--changed by sonata for bug no:2594979-->
            <!--<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[last()]">-->
						<xsl:choose>
							<xsl:when test="key('dataSeries', c:chartSpace/@oox:part)[1]/parent::node()[name()='c:areaChart']/c:grouping[@val='standard']">
								<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)[last()]">
									<xsl:call-template name="InsertSeriesReversed">
										<xsl:with-param name="reverseCategories" select="$reverseCategories"/>
									</xsl:call-template>
								</xsl:for-each>
							</xsl:when>
							<xsl:otherwise>
            <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">
              <xsl:call-template name="InsertSeriesReversed">
                <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>

						<!--end-->
						<!--<xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">
              <xsl:call-template name="InsertSeriesReversed">
                <xsl:with-param name="reverseCategories" select="$reverseCategories"/>
              </xsl:call-template>
            </xsl:for-each>-->


					</xsl:otherwise>
				</xsl:choose>
				<!--added by sonata for bug no:2571146-->
				<chart:stock-range-line chart:style-name="stock-range"/>
				<!--end-->
        <chart:wall chart:style-name="wall"/>
        <chart:floor chart:style-name="floor"/>
        <!--RefNo-1:ODF1.1:commented ellement chart:stock-range-line:chk for the bug fix:2015044-->
        <!--<chart:stock-range-line chart:style-name="stock-range"/>-->
      </chart:plot-area>

      <xsl:call-template name="InsertChartTable"/>

    </chart:chart>
  </xsl:template>

	<xsl:template name="CalculateRowHeight">
		<xsl:param name="sheetname"/>
		<xsl:param name="startrowno"/>
		<xsl:param name="endrowno"/>
		<xsl:param name="total"/>
		<xsl:param name="defaultrowheight"/>
		<xsl:param name="countchartrow"/>

		<xsl:variable name="totalrowheight">
			<xsl:choose>
				<xsl:when test="($countchartrow &gt;= $startrowno+1) and ($countchartrow &lt;= $endrowno)">
					<xsl:choose>
						<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()/@r=$countchartrow">
							<xsl:choose>
								<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()[@r=$countchartrow]/@ht">
									<!--<xsl:when test="parent::node()/child::node()[name()='sheetData']/child::node()/@ht">-->
									<!--<xsl:variable name="customrowheight" select="parent::node()/child::node()[name()='sheetData']/child::node()/@ht"/>-->
									<xsl:variable name="customrowheight" select="parent::node()/child::node()[name()='sheetData']/child::node()[@r=$countchartrow]/@ht"/>
									<xsl:value-of select="$total + $customrowheight"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$total+$defaultrowheight"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$total+$defaultrowheight"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$total"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:choose>

			<xsl:when test="($countchartrow &gt;= $startrowno+1) and ($countchartrow &lt; $endrowno -1)">
				<xsl:call-template name="CalculateRowHeight">
					<xsl:with-param name="sheetname" select="$sheetname"/>
					<xsl:with-param name="startrowno" select="$startrowno"/>
					<xsl:with-param name="endrowno" select="$endrowno"/>
					<xsl:with-param name="total" select="$totalrowheight"/>
					<xsl:with-param name="defaultrowheight" select="$defaultrowheight"/>
					<xsl:with-param name="countchartrow" select="$countchartrow + 1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$totalrowheight"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="CalculateColWidth">
		<xsl:param name="sheetname"/>
		<xsl:param name="startcolno"/>
		<xsl:param name="endcolno"/>
		<xsl:param name="total"/>
		<xsl:param name="defaultcolWidth"/>
		<xsl:param name="countchartcol"/>
		<xsl:param name="totalDefaultColCount"/>
		<xsl:param name="startcoloffset"/>
		<xsl:param name="endcoloffset"/>


		<xsl:variable name="totalCustomColWidth">
			<xsl:choose>
				<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt;= $endcolno)">
					<xsl:choose>
						<xsl:when test="parent::node()/child::node()[name()='cols']">
							<xsl:variable name="customColWidth">
								<xsl:for-each select="parent::node()/child::node()[name()='cols']/child::node()[($countchartcol &gt; @min or $countchartcol=@min) and ($countchartcol &lt; @max or $countchartcol =@max)]">
									<xsl:choose>
										<xsl:when test="($countchartcol &gt; @min or $countchartcol=@min) and ($countchartcol &lt; @max or $countchartcol =@max)">
											<xsl:choose>
												<xsl:when test="@width">
													<xsl:value-of select="@width"/>
												</xsl:when>
											</xsl:choose>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$total"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:for-each>
							</xsl:variable>
							<xsl:if test ="$customColWidth!=''">
								<xsl:value-of select="$total + $customColWidth"/>
							</xsl:if>
							<xsl:if test ="$customColWidth = ''">
								<xsl:value-of select="$total"/>
							</xsl:if>
						</xsl:when>
					</xsl:choose>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>

		<xsl:variable name="countDefaultColWidth">
			<xsl:variable name ="flag">
				<xsl:choose>
					<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt;= $endcolno)">
						<xsl:choose>
							<xsl:when test="parent::node()/child::node()[name()='cols']">
								<xsl:for-each select="parent::node()/child::node()[name()='cols']/child::node()">
									<!-- NewCode-->
									<xsl:choose >
										<xsl:when test="($countchartcol &gt;= @min) and ($countchartcol &lt;= @max)">
											<xsl:value-of select ="'true'"/>
										</xsl:when>
										<xsl:when test="($countchartcol &lt;= @min)">
											<xsl:value-of select ="'false'"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select ="'false'"/>
										</xsl:otherwise>
									</xsl:choose>

									<!-- NewCode-->
								</xsl:for-each>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$endcolno -($startcolno)"/>
							</xsl:otherwise>
						</xsl:choose>

					</xsl:when>
				</xsl:choose>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test ="contains($flag,'true')">
					<xsl:value-of select ="concat($totalDefaultColCount,'false|')"/>
				</xsl:when>
				<xsl:when test ="not(contains($flag,'true'))">
					<xsl:value-of select ="concat($totalDefaultColCount,'true|')"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>


		<xsl:choose>

			<xsl:when test="($countchartcol &gt;= $startcolno + 1) and ($countchartcol &lt; $endcolno -1)">
				<xsl:call-template name="CalculateColWidth">
					<xsl:with-param name="sheetname" select="$sheetname"/>
					<xsl:with-param name="startcolno" select="$startcolno"/>
					<xsl:with-param name="endcolno" select="$endcolno"/>
					<xsl:with-param name="total">
						<xsl:if test="$totalCustomColWidth != 0 ">
							<xsl:value-of select="$totalCustomColWidth"/>
						</xsl:if>
						<xsl:if test="$totalCustomColWidth = 0 ">
							<xsl:value-of select="0"/>
						</xsl:if>
					</xsl:with-param>
					<xsl:with-param name="totalDefaultColCount" select="$countDefaultColWidth"/>
					<xsl:with-param name="defaultcolWidth" select="$defaultcolWidth"/>
					<xsl:with-param name="countchartcol" select="$countchartcol + 1"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:variable name="finalCount">
					<xsl:call-template name="increaseCount">
						<xsl:with-param name ="totalCount" select ="0"/>
						<xsl:with-param name="countDefaultColWidth" select ="$countDefaultColWidth"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="concat($totalCustomColWidth,'|',$finalCount)"/>

			</xsl:otherwise>
		</xsl:choose>


	</xsl:template>

	<xsl:template name="increaseCount">
		<xsl:param name ="totalCount"/>
		<xsl:param name="countDefaultColWidth"/>
		<xsl:variable name ="totalNo">
			<xsl:value-of select ="$totalCount"/>
		</xsl:variable>
		<xsl:choose>


			<xsl:when test ="$countDefaultColWidth!='' and substring-before($countDefaultColWidth,'|')='true'">
				<xsl:call-template name ="increaseCount">
					<xsl:with-param name ="totalCount">
						<xsl:value-of select ="$totalNo+1"/>
					</xsl:with-param>
					<xsl:with-param name ="countDefaultColWidth" select ="substring-after($countDefaultColWidth,'|')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test ="$countDefaultColWidth!='' and substring-before($countDefaultColWidth,'|')='false'">
				<xsl:call-template name ="increaseCount">
					<xsl:with-param name ="totalCount">
						<xsl:value-of select ="$totalNo"/>
					</xsl:with-param>
					<xsl:with-param name ="countDefaultColWidth" select ="substring-after($countDefaultColWidth,'|')"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$totalNo"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

  <xsl:template name="InsertChartProperties">
    <style:style style:name="chart" style:family="chart">
      <style:graphic-properties draw:fill-color="#ffffff" draw:stroke="solid"
        svg:stroke-color="#898989">
        <!-- Sonata: chart fill properties-->
        <xsl:for-each select="c:chartSpace/c:spPr">
          <xsl:call-template name="InsertChartFillProperties"/>
        </xsl:for-each>
      </style:graphic-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="InsertChartTitleProperties">
    <xsl:for-each select="c:chartSpace/c:chart/c:title">
      <style:style style:name="chart_title" style:family="chart">
        <!--changed by sonata for bug no:2163376-->
        <style:chart-properties style:direction="ltr">
          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:for-each select="c:tx/c:rich/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
              <xsl:for-each select="c:txPr/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
        </style:chart-properties>
        <style:graphic-properties draw:stroke="none" draw:fill="none">
          <!-- Sonata: chart Title fill properties-->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>
        </style:graphic-properties>
        <style:text-properties fo:font-family="Calibri" style:font-family-generic="swiss"
          style:font-pitch="variable" fo:font-size="18pt" fo:font-weight="bold"
          style:font-family-asian="&apos;MS Gothic&apos;"
          style:font-family-generic-asian="system" style:font-pitch-asian="variable"
          style:font-size-asian="18pt" style:font-family-complex="Tahoma"
          style:font-family-generic-complex="system" style:font-pitch-complex="variable"
          style:font-size-complex="18pt">

          <!-- default "bold" font attribute for default title -->
          <xsl:if test="not(c:tx) and not(c:txPr)">
            <xsl:attribute name="fo:font-weight">
              <xsl:text>bold</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </style:text-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>
  <xsl:template name="InsertChartFillProperties">
            <!-- Insert fill -->
            <xsl:choose>
              <xsl:when test="a:gradFill">
                <xsl:attribute name="draw:fill">
                  <xsl:text>gradient</xsl:text>
                </xsl:attribute>
                <xsl:for-each select="a:gradFill">
                  <xsl:attribute name="draw:fill-gradient-name">
                    <xsl:value-of select="generate-id()"/>
                  </xsl:attribute>
          <xsl:for-each select="a:gsLst/a:gs[1]">
            <xsl:choose>
              <xsl:when test="a:srgbClr/a:alpha/@val">
                <xsl:variable name="alpha">
                  <xsl:value-of select="a:srgbClr/a:alpha/@val"/>
                </xsl:variable>
                <xsl:if test="($alpha != '') or ($alpha != 0)">
                  <xsl:attribute name="draw:opacity">
                    <xsl:value-of select="concat(($alpha div 1000), '%')"/>
                  </xsl:attribute>
                </xsl:if>

              </xsl:when>
              <xsl:when test="a:schemeClr/a:alpha/@val">
                <xsl:variable name="alpha">
                  <xsl:value-of select="a:schemeClr/a:alpha/@val"/>
                </xsl:variable>

                <xsl:if test="($alpha != '') or ($alpha != 0)">
                  <xsl:attribute name="draw:opacity">
                    <xsl:value-of select="concat(($alpha div 1000), '%')"/>
                  </xsl:attribute>
                </xsl:if>
              </xsl:when>
            </xsl:choose>
          </xsl:for-each>

                </xsl:for-each>
              </xsl:when>
              <xsl:when test="a:blipFill">
                <xsl:call-template name="InsertBitmapFill"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="InsertFill"/>
              </xsl:otherwise>
            </xsl:choose>

    <!-- Insert Borders style, color -->
            <xsl:call-template name="InsertLineColor"/>
            <xsl:call-template name="InsertLineStyle"/>
  </xsl:template>
  <xsl:template name="InsertAxisXTitleProperties">
    <xsl:for-each select="c:title">
      <style:style style:name="axis-x_title" style:family="chart">
        <!--changed by sonata for bug no:2163376-->
        <style:chart-properties style:direction="ltr">
          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:for-each select="c:tx/c:rich/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
              <xsl:for-each select="c:txPr/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
        </style:chart-properties>
        <style:graphic-properties draw:stroke="none" draw:fill="none">
          <!-- Sonata: chart X Axis Title fill properties-->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>
        </style:graphic-properties>
        <style:text-properties fo:font-family="Calibri" fo:font-weight="bold" style:font-family-generic="swiss"
          style:font-pitch="variable" fo:font-size="10pt"
          style:font-family-asian="&apos;MS Gothic&apos;"
          style:font-family-generic-asian="system" style:font-pitch-asian="variable"
          style:font-size-asian="10pt" style:font-family-complex="Tahoma"
          style:font-family-generic-complex="system" style:font-pitch-complex="variable"
          style:font-size-complex="10pt">
          <!--added for bug no:2016060-->
          <xsl:if test="not(c:tx) and not(c:txPr)">
              <xsl:attribute name="fo:font-weight">
              <xsl:text>bold</xsl:text>
              </xsl:attribute>
          </xsl:if>
            
            <xsl:choose>
              <!-- custom title -->
              <xsl:when test="c:tx">
                <xsl:call-template name="TextBoxRunProperties">
                  <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                  <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                  <xsl:with-param name="deftxPr" select="../../../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                </xsl:call-template>
              </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
                <xsl:call-template name="TextBoxRunProperties">
                  <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                  <xsl:with-param name="defRPr" select="../../../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
 </style:text-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertAxisYTitleProperties">
    <xsl:for-each select="c:title">
      <style:style style:name="axis-y_title" style:family="chart">
        <style:chart-properties style:direction="ltr">
          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:for-each select="c:tx/c:rich/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
              <xsl:for-each select="c:txPr/a:bodyPr">
                <xsl:call-template name="TextRotation"/>
              </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
        </style:chart-properties>
        <style:graphic-properties draw:stroke="none" draw:fill="none">
          <!-- Sonata: chart Y axis Title fill properties-->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>
        </style:graphic-properties>
        <style:text-properties fo:font-family="Calibri" fo:font-weight="bold" style:font-family-generic="swiss"
         style:font-pitch="variable" fo:font-size="10pt"
         style:font-family-asian="&apos;MS Gothic&apos;"
         style:font-family-generic-asian="system" style:font-pitch-asian="variable"
         style:font-size-asian="10pt" style:font-family-complex="Tahoma"
         style:font-family-generic-complex="system" style:font-pitch-complex="variable"
         style:font-size-complex="10pt">

          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:when>
            <!-- default title -->
         <xsl:otherwise>
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="defRPr" select="../../../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </style:text-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertMajorGridXProperties">
    <xsl:for-each select="c:majorGridlines/c:spPr">
      <style:style style:name="majorGridX" style:family="chart">
        <style:graphic-properties svg:stroke-width="0.03cm" svg:stroke-color="#000000"
          draw:marker-start-width="0.35cm" draw:marker-end-width="0.35cm">
          <xsl:call-template name="InsertLineColor"/>
          <xsl:call-template name="InsertLineStyle"/>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertMinorGridXProperties">
    <xsl:for-each select="c:minorGridlines/c:spPr">
      <style:style style:name="minorGridX" style:family="chart">
        <style:graphic-properties svg:stroke-width="0.03cm" svg:stroke-color="#000000"
          draw:marker-start-width="0.35cm" draw:marker-end-width="0.35cm">
          <xsl:call-template name="InsertLineColor"/>
          <xsl:call-template name="InsertLineStyle"/>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>
  <xsl:template name="InsertMajorGridYProperties">
    <xsl:for-each select="c:majorGridlines">
      <style:style style:name="majorGridY" style:family="chart">
        <style:graphic-properties svg:stroke-width="0.03cm" svg:stroke-color="#000000"
          draw:marker-start-width="0.35cm" draw:marker-end-width="0.35cm">
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertLineColor"/>
            <xsl:call-template name="InsertLineStyle"/>
          </xsl:for-each>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>
  <xsl:template name="InsertMinorGridYProperties">
    <xsl:for-each select="c:minorGridlines">
      <style:style style:name="minorGridY" style:family="chart">
        <style:graphic-properties svg:stroke-width="0.03cm" svg:stroke-color="#000000"
          draw:marker-start-width="0.35cm" draw:marker-end-width="0.35cm">
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertLineColor"/>
            <xsl:call-template name="InsertLineStyle"/>
          </xsl:for-each>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertLegendProperties">
    <xsl:for-each select="c:chartSpace/c:chart/c:legend">
      <style:style style:name="legend" style:family="chart">
        <style:graphic-properties draw:fill="none" draw:stroke="none">
          <!-- Sonata: chart Legend fill properties-->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>
        </style:graphic-properties>
        <style:text-properties fo:font-family="Calibri" style:font-family-generic="swiss"
          style:font-pitch="variable" fo:font-size="10pt"
          style:font-family-asian="&apos;MS Gothic&apos;"
          style:font-family-generic-asian="system" style:font-pitch-asian="variable"
          style:font-size-asian="10pt" style:font-family-complex="Tahoma"
          style:font-family-generic-complex="system" style:font-pitch-complex="variable"
          style:font-size-complex="10pt">

          <xsl:choose>
            <!-- custom title -->
            <!-- sonata: defect#2154917  -->

            <!--commented by sonata for bug no:2203898-->
            <!--<xsl:when test="c:legendEntry/c:tx">
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:legendEntry/c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                <xsl:with-param name="defRPr" select="c:legendEntry/c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:when>-->
            <!--end-->

            <!--added by sonata for bug no:2203898-->
            <xsl:when test="c:legendEntry/c:txPr">
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:legendEntry/c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                <xsl:with-param name="defRPr" select="c:legendEntry/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:when>
            <!--end-->


            <!-- default title -->
            <xsl:otherwise>
              <xsl:call-template name="TextBoxRunProperties">
                <!-- Soanta: [ 2181962 ] XLSX-Legend-Font Style is lost-->
                <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="defRPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </style:text-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertPlotAreaProperties">
    <xsl:for-each select="c:chartSpace/c:chart/c:plotArea">
      <style:style style:name="plot_area" style:family="chart">
        <style:chart-properties chart:japanese-candle-stick="false" chart:stock-with-volume="false"
          chart:three-dimensional="false" chart:deep="false" chart:lines="false"
          chart:interpolation="none" chart:symbol-type="none" chart:vertical="false"
          chart:lines-used="0" chart:connect-bars="false" chart:series-source="columns"
          chart:mean-value="false" chart:error-margin="0" chart:error-lower-limit="0"
          chart:error-upper-limit="0" chart:error-category="none" chart:error-percentage="0"
          chart:data-label-number="none" chart:data-label-text="false"
          chart:data-label-symbol="false">

          <!-- insert markers for Stock Chart if necessary -->
          <xsl:if test="descendant::node()/c:marker and key('plotArea', @oox:part)/c:stockChart">
            <xsl:attribute name="chart:symbol-type">
              <xsl:text>automatic</xsl:text>
            </xsl:attribute>
          </xsl:if>
          
          <!-- japanese candle-stick can be true for stockChart or lineChart -->
          <xsl:if
            test="key('plotArea', @oox:part)/c:stockChart/c:upDownBars or key('plotArea', @oox:part)/c:lineChart/c:upDownBars">
            <xsl:attribute name="chart:japanese-candle-stick">
              <xsl:text>true</xsl:text>
            </xsl:attribute>
          </xsl:if>
          
          <xsl:if test="key('plotArea', @oox:part)/c:stockChart and key('plotArea', @oox:part)/c:barChart">
            <xsl:attribute name="chart:stock-with-volume">
              <xsl:text>true</xsl:text>
            </xsl:attribute>
          </xsl:if>
            
          <!-- insert markers for Stock Chart if necessary -->
          <xsl:if test="descendant::node()/c:marker and key('plotArea', @oox:part)/c:stockChart">
            <xsl:attribute name="chart:symbol-type">
              <xsl:text>automatic</xsl:text>
            </xsl:attribute>
          </xsl:if>
          <!-- japanese candle-stick can be true only for stockChart or lineChart -->
          <xsl:if
            test="key('plotArea', @oox:part)/c:stockChart/c:upDownBars or key('plotArea', @oox:part)/c:lineChart/c:upDownBars">
            <xsl:attribute name="chart:japanese-candle-stick">
              <xsl:text>true</xsl:text>
            </xsl:attribute>
          </xsl:if>
          <!-- data grouping-->
          <xsl:choose>
            <xsl:when test="key('grouping', @oox:part)[1]/@val = 'stacked' ">
              <xsl:attribute name="chart:stacked">
                <xsl:text>true</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <xsl:when test="key('grouping', @oox:part)[1]/@val = 'percentStacked' ">
              <xsl:attribute name="chart:percentage">
                <xsl:text>true</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <xsl:when test="key('grouping', @oox:part)[1]/@val = 'standard' and key('plotArea', @oox:part)/c:bar3DChart">
              <xsl:attribute name="chart:deep">
                <xsl:text>true</xsl:text>
              </xsl:attribute>
            </xsl:when>
          </xsl:choose>

          <!-- 3D chart -->
          <xsl:choose>
            <xsl:when test="c:bar3DChart or c:line3DChart or c:area3DChart or c:pie3DChart">
              <xsl:attribute name="chart:three-dimensional">
                <xsl:text>true</xsl:text>
              </xsl:attribute>

              <!-- 3D shape -->
              <xsl:if test="c:bar3DChart">
                <xsl:choose>
                  <xsl:when test="descendant:: c:shape/@val != '' ">
                    <xsl:for-each select="descendant::c:shape[1]">
                      <xsl:if test="@val != 'box' ">
                        <xsl:attribute name="chart:solid-type">
                          <xsl:value-of select="@val"/>
                        </xsl:attribute>
                      </xsl:if>
                    </xsl:for-each>
                  </xsl:when>
                </xsl:choose>
              </xsl:if>

            </xsl:when>
          </xsl:choose>

          <!-- bar charts -->
          <xsl:if test="c:barChart/c:barDir/@val = 'bar' ">
            <xsl:attribute name="chart:vertical">
              <xsl:text>true</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <!-- 3D bar charts  -->
          <xsl:if test="c:bar3DChart/c:barDir/@val = 'bar' ">
            <xsl:attribute name="chart:vertical">
              <xsl:text>true</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <!-- interpolation line charts -->
          <xsl:if
            test="(c:lineChart/c:ser/c:smooth/@val = 1 and c:lineChart/c:grouping/@val = 'standard') or
            c:scatterChart/c:ser/c:smooth/@val = 1">
            <xsl:attribute name="chart:interpolation">
              <xsl:text>cubic-spline</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <!-- trend line -->
          <xsl:if test="count(key('dataSeries',@oox:part)/c:trendline) = 1 and count(key('dataSeries',@oox:part)) = 1">
            <xsl:attribute name="chart:regression-type">
              <xsl:choose>
                <xsl:when test="c:ser/c:trendline/c:trendlineType/@val = 'linear' ">
                  <xsl:text>linear</xsl:text>
                </xsl:when>
                <xsl:when test="c:ser/c:trendline/c:trendlineType/@val = 'log' ">
                  <xsl:text>logarithmic</xsl:text>
                </xsl:when>
                <xsl:when test="c:ser/c:trendline/c:trendlineType/@val = 'exp' ">
                  <xsl:text>exponential</xsl:text>
                </xsl:when>
                <xsl:when test="c:ser/c:trendline/c:trendlineType/@val = 'power' ">
                  <xsl:text>power</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>linear</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </xsl:if>

          <!-- lines between points in a scatter chart -->
          <xsl:if test="c:scatterChart or c:bubbleChart">
            <xsl:attribute name="chart:lines">
              <xsl:choose>
                <!-- if at least one series has line -->
                <xsl:when
                  test="c:scatterChart/c:ser/c:spPr/a:ln[not(a:noFill)] or
                  c:scatterChart/c:ser[not(c:spPr/a:ln)]">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <xsl:when
                  test="c:bubbleChart/c:ser/c:spPr/a:ln[not(a:noFill)] or
                  c:bubbleChart/c:ser[not(c:spPr/a:ln)]">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>false</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </xsl:if>

          <!-- line, radar or scatter charts with symbols -->
          <xsl:if
            test="c:lineChart/c:ser[not(c:marker/c:symbol/@val = 'none')] or 
            (c:radarChart/c:radarStyle/@val = 'marker' and c:radarChart/c:ser[not(c:marker/c:symbol/@val = 'none')])
            or c:scatterChart/c:ser[not(c:marker/c:symbol/@val = 'none')] or c:bubbleChart/c:ser[not(c:marker/c:symbol/@val = 'none')]">
            <xsl:attribute name="chart:symbol-type">
              <xsl:text>automatic</xsl:text>
            </xsl:attribute>
          </xsl:if>
        </style:chart-properties>
        <style:graphic-properties draw:stroke="none" draw:fill="solid" draw:fill-color="#ffffff">
          <!-- sonata:Insert Ploat Area Background style, color, fill, transparency -->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertAxisXProperties">
    <xsl:param name="axisYId"/>
    <xsl:param name="numberformat"/>
    <xsl:param name="dataStyleName"/>
    <style:style style:name="axis-x" style:family="chart" style:data-style-name="N0">
      <!--code added by sonata for bug no:2107205-->
      <xsl:choose>
        <xsl:when test="$numberformat!=''">
          <xsl:attribute name="style:data-style-name">
            <xsl:value-of select="$dataStyleName"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="style:data-style-name">
            <xsl:value-of select="N0"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <!--end-->
      <style:chart-properties chart:display-label="true" chart:tick-marks-major-inner="false"
        chart:tick-marks-major-outer="true" chart:tick-marks-minor-inner="false"
        chart:tick-marks-minor-outer="false" chart:logarithmic="false" chart:text-overlap="true"
        text:line-break="true" chart:label-arrangement="side-by-side" chart:visible="true"
        style:direction="ltr">

        <xsl:call-template name="SetAxisChartProperties">
          <xsl:with-param name="axisId" select="$axisYId"/>
        </xsl:call-template>

      </style:chart-properties>
      <style:graphic-properties draw:stroke="solid" svg:stroke-width="0cm"
        svg:stroke-color="#000000">
        <xsl:for-each select="c:spPr">
          <xsl:call-template name="InsertLineColor"/>
          <xsl:call-template name="InsertLineStyle"/>
        </xsl:for-each>
      </style:graphic-properties>
      <style:text-properties fo:font-family="Calibri"  style:font-family-generic="swiss"
         style:font-pitch="variable" fo:font-size="10pt"
        style:font-family-asian="&apos;MS Gothic&apos;"
        style:font-family-generic-asian="system" style:font-pitch-asian="variable"
         style:font-size-asian="10pt" style:font-family-complex="Tahoma"
        style:font-family-generic-complex="system" style:font-pitch-complex="variable"
         style:font-size-complex="10pt">
        <xsl:choose>
          <!-- custom title -->
          <xsl:when test="c:tx">
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
              <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="deftxPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
           </xsl:call-template>
          </xsl:when>
          <!-- default title -->
          <xsl:otherwise>
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="defRPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
             </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      
      </style:text-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="InsertAxisYProperties">
    <xsl:param name="axisXId"/>
    <xsl:param name="numberformat"/>
    <xsl:param name="dataStyleName"/>
    <style:style style:name="axis-y" style:family="chart">
      <!--code added by sonata for bug no:2107205-->
      <xsl:choose>
        <xsl:when test="$numberformat!=''">
          <xsl:attribute name="style:data-style-name">
            <xsl:value-of select="$dataStyleName"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="style:data-style-name">
            <xsl:value-of select="N0"/>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>
      <!--end-->
      <style:chart-properties chart:display-label="true" chart:tick-marks-major-inner="false"
        chart:tick-marks-major-outer="true" chart:logarithmic="false" chart:text-overlap="true"
        text:line-break="true" chart:label-arrangement="side-by-side" chart:visible="true"
        style:direction="ltr">

        <xsl:call-template name="SetAxisChartProperties">
          <xsl:with-param name="axisId" select="$axisXId"/>
        </xsl:call-template>

      </style:chart-properties>
      <style:graphic-properties draw:stroke="solid" svg:stroke-width="0cm"
        svg:stroke-color="#000000">
        <xsl:for-each select="c:spPr">
          <xsl:call-template name="InsertLineColor"/>
          <xsl:call-template name="InsertLineStyle"/>
        </xsl:for-each>
      </style:graphic-properties>
      <style:text-properties fo:font-family="Calibri"  style:font-family-generic="swiss"
        style:font-pitch="variable" fo:font-size="10pt"
        style:font-family-asian="&apos;MS Gothic&apos;"
        style:font-family-generic-asian="system" style:font-pitch-asian="variable"
        style:font-size-asian="10pt" style:font-family-complex="Tahoma"
        style:font-family-generic-complex="system" style:font-pitch-complex="variable"
        style:font-size-complex="10pt">
        <xsl:choose>
          <!-- custom title -->
          <xsl:when test="c:tx">
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
              <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="deftxPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
           </xsl:call-template>
          </xsl:when>
          <!-- default title -->
          <xsl:otherwise>
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="defRPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </style:text-properties>
    </style:style>
    
    <!--end-->

  </xsl:template>

 
  <!--changed by sonata for bug no:2107193-->

  <xsl:template match="c:formatCode">

    <!-- @Descripition: inserts number format style -->
    <!-- @Context: None -->

    <xsl:variable name="formatingMarks">
      <xsl:call-template name="StripText">
        <xsl:with-param name="formatCode" select="."/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>

      <!-- date style -->
      <xsl:when
        test="(contains($formatingMarks,'y') or (contains($formatingMarks,'m') and not(contains($formatingMarks,'h') or contains($formatingMarks,'s')))or (contains($formatingMarks,'d') and not(contains($formatingMarks,'Red'))))">
        <number:date-style style:name="{generate-id(.)}">
          <xsl:call-template name="ProcessFormat">
            <xsl:with-param name="format">
              <xsl:choose>
                <xsl:when test="contains(.,']')">
                  <xsl:value-of select="substring-after(.,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="processedFormat">
              <xsl:choose>
                <xsl:when test="contains(.,']')">
                  <xsl:value-of select="substring-after(.,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
          </xsl:call-template>
        </number:date-style>
      </xsl:when>

      <!-- time style -->
      <!-- 'and' at the end is for latvian currency -->
      <xsl:when
        test="contains($formatingMarks,'h') or contains($formatingMarks,'s') and not(contains($formatingMarks, '$Ls-426' ))">
        <number:time-style style:name="{generate-id(.)}">
          <xsl:if test="contains($formatingMarks,'[h')">
            <xsl:attribute name="number:truncate-on-overflow">false</xsl:attribute>
          </xsl:if>
          <xsl:call-template name="ProcessFormat">
            <xsl:with-param name="format">
              <xsl:choose>
                <xsl:when test="contains(.,'[h')">
                  <xsl:value-of select="translate(translate(.,'[h','h'),']','')"/>
                </xsl:when>
                <xsl:when test="contains(.,']')">
                  <xsl:value-of select="substring-after(.,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="processedFormat">
              <xsl:choose>
                <xsl:when test="contains(.,'[h')">
                  <xsl:value-of select="translate(translate(.,'[h','h'),']','')"/>
                </xsl:when>
                <xsl:when test="contains(.,']')">
                  <xsl:value-of select="substring-after(.,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="."/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
          </xsl:call-template>
        </number:time-style>
      </xsl:when>

      <!-- when there are different formats for positive and negative numbers -->
      <xsl:when
        test="contains(.,';') and not(contains(substring-after(.,';'),';'))">
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(substring-before(.,';'),'$') or contains(substring-before(.,';'),'zł') or contains(substring-before(.,';'),'€') or contains(substring-before(.,';'),'£')">
            <number:currency-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
            <number:currency-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:currency-style>
          </xsl:when>

          <!--percentage style -->
          <xsl:when test="contains(substring-before(.,';'),'%')">
            <number:percentage-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
            </number:percentage-style>
            <number:percentage-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:percentage-style>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>
            <number:number-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:number-style>
          </xsl:otherwise>

        </xsl:choose>
      </xsl:when>

      <!-- when there are separate formats for positive numbers, negative numbers and zeros -->
      <xsl:when test="contains(.,';') and contains(substring-after(.,';'),';')">
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(substring-before(@formatCode,';'),'$') or contains(substring-before(@formatCode,';'),'zł') or contains(substring-before(@formatCode,';'),'€') or contains(substring-before(@formatCode,';'),'£')">
            <number:currency-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
            <number:currency-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(.,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(.,';'),';'),';')">
                <number:currency-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(.,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:currency-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(.,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:currency-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(.,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:currency-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <!-- percentage style -->
          <xsl:when test="contains(substring-before(.,';'),'%')">
            <number:percentage-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:percentage-style>
            <number:percentage-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(.,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:percentage-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(.,';'),';'),';')">
                <number:percentage-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(.,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:percentage-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(.,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:percentage-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(.,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:percentage-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(.,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>
            <number:number-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(.,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(@formatCode,';'),';'),';')">
                <number:number-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(.,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:number-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(.,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:number-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(.,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:number-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>

      <xsl:otherwise>
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(.,'$') or contains(.,'zł') or contains(.,'€') or contains(.,'£')">
            <number:currency-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="."/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
          </xsl:when>

          <!--percentage style -->
          <xsl:when test="contains(.,'%')">
            <number:percentage-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="."/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
            </number:percentage-style>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode" select="."/>
              </xsl:call-template>
            </number:number-style>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>

    </xsl:choose>

  </xsl:template>

  <!--end-->






 
  <xsl:template match="c:numFmt">

    <!-- @Descripition: inserts number format style -->
    <!-- @Context: None -->

    <xsl:variable name="formatingMarks">
      <xsl:call-template name="StripText">
        <xsl:with-param name="formatCode" select="@formatCode"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>

      <!-- date style -->
      <xsl:when
        test="(contains($formatingMarks,'y') or (contains($formatingMarks,'m') and not(contains($formatingMarks,'h') or contains($formatingMarks,'s')))or (contains($formatingMarks,'d') and not(contains($formatingMarks,'Red'))))">
        <number:date-style style:name="{generate-id(.)}">
          <xsl:call-template name="ProcessFormat">
            <xsl:with-param name="format">
              <xsl:choose>
                <xsl:when test="contains(@formatCode,']')">
                  <xsl:value-of select="substring-after(@formatCode,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@formatCode"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="processedFormat">
              <xsl:choose>
                <xsl:when test="contains(@formatCode,']')">
                  <xsl:value-of select="substring-after(@formatCode,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@formatCode"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
          </xsl:call-template>
        </number:date-style>
      </xsl:when>

      <!-- time style -->
      <!-- 'and' at the end is for latvian currency -->
      <xsl:when
        test="contains($formatingMarks,'h') or contains($formatingMarks,'s') and not(contains($formatingMarks, '$Ls-426' ))">
        <number:time-style style:name="{generate-id(.)}">
          <xsl:if test="contains($formatingMarks,'[h')">
            <xsl:attribute name="number:truncate-on-overflow">false</xsl:attribute>
          </xsl:if>
          <xsl:call-template name="ProcessFormat">
            <xsl:with-param name="format">
              <xsl:choose>
                <xsl:when test="contains(@formatCode,'[h')">
                  <xsl:value-of select="translate(translate(@formatCode,'[h','h'),']','')"/>
                </xsl:when>
                <xsl:when test="contains(@formatCode,']')">
                  <xsl:value-of select="substring-after(@formatCode,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@formatCode"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
            <xsl:with-param name="processedFormat">
              <xsl:choose>
                <xsl:when test="contains(@formatCode,'[h')">
                  <xsl:value-of select="translate(translate(@formatCode,'[h','h'),']','')"/>
                </xsl:when>
                <xsl:when test="contains(@formatCode,']')">
                  <xsl:value-of select="substring-after(@formatCode,']')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@formatCode"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:with-param>
          </xsl:call-template>
        </number:time-style>
      </xsl:when>

      <!-- when there are different formats for positive and negative numbers -->
      <xsl:when
        test="contains(@formatCode,';') and not(contains(substring-after(@formatCode,';'),';'))">
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(substring-before(@formatCode,';'),'$') or contains(substring-before(@formatCode,';'),'zł') or contains(substring-before(@formatCode,';'),'€') or contains(substring-before(@formatCode,';'),'£')">
            <number:currency-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
            <number:currency-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:currency-style>
          </xsl:when>

          <!--percentage style -->
          <xsl:when test="contains(substring-before(@formatCode,';'),'%')">
            <number:percentage-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
            </number:percentage-style>
            <number:percentage-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:percentage-style>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>
            <number:number-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-after(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
              <style:map style:condition="value()&gt;=0"
                style:apply-style-name="{concat(generate-id(.),'P0')}"/>
            </number:number-style>
          </xsl:otherwise>

        </xsl:choose>
      </xsl:when>

      <!-- when there are separate formats for positive numbers, negative numbers and zeros -->
      <xsl:when test="contains(@formatCode,';') and contains(substring-after(@formatCode,';'),';')">
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(substring-before(@formatCode,';'),'$') or contains(substring-before(@formatCode,';'),'zł') or contains(substring-before(@formatCode,';'),'€') or contains(substring-before(@formatCode,';'),'£')">
            <number:currency-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
            <number:currency-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(@formatCode,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(@formatCode,';'),';'),';')">
                <number:currency-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(@formatCode,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:currency-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(@formatCode,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <!--RefNo-1:ODF1.1:Added <number:text> element to include the text value-->
                      <number:text>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                      </number:text>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:currency-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(@formatCode,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:currency-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <!-- percentage style -->
          <xsl:when test="contains(substring-before(@formatCode,';'),'%')">
            <number:percentage-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:percentage-style>
            <number:percentage-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(@formatCode,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:percentage-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(@formatCode,';'),';'),';')">
                <number:percentage-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(@formatCode,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:percentage-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(@formatCode,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <!--RefNo-1:ODF1.1:Added <number:text> element to include the text value-->
                      <number:text>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                      </number:text>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:percentage-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(@formatCode,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:percentage-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{concat(generate-id(.),'P0')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(@formatCode,';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>
            <number:number-style style:name="{concat(generate-id(.),'P1')}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="substring-before(substring-after(@formatCode,';'),';')"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:number-style>

            <xsl:choose>
              <xsl:when test="contains(substring-after(substring-after(@formatCode,';'),';'),';')">
                <number:number-style style:name="{concat(generate-id(.),'P2')}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of
                        select="substring-before(substring-after(substring-after(@formatCode,';'),';'),';')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </number:number-style>
                <number:text-style style:name="{generate-id(.)}">
                  <xsl:variable name="text">
                    <xsl:value-of
                      select="substring-after(substring-after(substring-after(@formatCode,';'),';'),';')"
                    />
                  </xsl:variable>
                  <xsl:choose>

                    <!-- text content -->
                    <xsl:when test="contains($text,'@')">
                      <number:text>
                        <xsl:value-of select="translate(substring-before($text,'@'),'_-',' ')"/>
                      </number:text>
                      <number:text-content/>
                      <number:text>
                        <xsl:value-of select="translate(substring-after($text,'@'),'_-',' ')"/>
                      </number:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <!--RefNo-1:ODF1.1:Added <number:text> element to include the text value-->
                      <number:text>
                      <xsl:value-of select="translate($text,'_-',' ')"/>
                      </number:text>
                    </xsl:otherwise>
                  </xsl:choose>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                  <style:map style:condition="value()=0"
                    style:apply-style-name="{concat(generate-id(.),'P2')}"/>
                </number:text-style>
              </xsl:when>
              <xsl:otherwise>
                <number:number-style style:name="{generate-id(.)}">
                  <xsl:call-template name="InsertNumberFormatting">
                    <xsl:with-param name="formatCode">
                      <xsl:value-of select="substring-after(substring-after(@formatCode,';'),';')"/>
                    </xsl:with-param>
                  </xsl:call-template>
                  <style:map style:condition="value()&gt;0"
                    style:apply-style-name="{concat(generate-id(.),'P0')}"/>
                  <style:map style:condition="value()&lt;0"
                    style:apply-style-name="{concat(generate-id(.),'P1')}"/>
                </number:number-style>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:when>

      <xsl:otherwise>
        <xsl:choose>

          <!-- currency style -->
          <xsl:when
            test="contains(@formatCode,'$') or contains(@formatCode,'zł') or contains(@formatCode,'€') or contains(@formatCode,'£')">
            <number:currency-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="@formatCode"/>
                </xsl:with-param>
              </xsl:call-template>
            </number:currency-style>
          </xsl:when>

          <!--percentage style -->
          <xsl:when test="contains(@formatCode,'%')">
            <number:percentage-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode">
                  <xsl:value-of select="@formatCode"/>
                </xsl:with-param>
              </xsl:call-template>
              <number:text>%</number:text>
            </number:percentage-style>
          </xsl:when>

          <!-- number style -->
          <xsl:otherwise>
            <number:number-style style:name="{generate-id(.)}">
              <xsl:call-template name="InsertNumberFormatting">
                <xsl:with-param name="formatCode" select="@formatCode"/>
              </xsl:call-template>
            </number:number-style>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>

    </xsl:choose>

  </xsl:template>
  <xsl:template name="InsertSecondaryAxisYProperties">
    <xsl:param name="axisXId"/>

    <style:style style:name="secondary_axis-y" style:family="chart" style:data-style-name="N0">
      <style:chart-properties chart:display-label="true" chart:tick-marks-major-inner="false"
        chart:tick-marks-major-outer="true" chart:logarithmic="false" chart:text-overlap="true"
        text:line-break="true" chart:label-arrangement="side-by-side" chart:visible="true"
        style:direction="ltr">

        <xsl:call-template name="SetAxisChartProperties">
          <xsl:with-param name="axisId" select="$axisXId"/>
        </xsl:call-template>

      </style:chart-properties>
      <style:graphic-properties draw:stroke="solid" svg:stroke-width="0cm"
        svg:stroke-color="#000000">
        <xsl:for-each select="c:spPr">
          <xsl:call-template name="InsertLineColor"/>
          <xsl:call-template name="InsertLineStyle"/>
        </xsl:for-each>
      </style:graphic-properties>
      <style:text-properties fo:font-family="Calibri"  style:font-family-generic="swiss"
         style:font-pitch="variable" fo:font-size="10pt"
        style:font-family-asian="&apos;MS Gothic&apos;"
        style:font-family-generic-asian="system" style:font-pitch-asian="variable"
         style:font-size-asian="10pt" style:font-family-complex="Tahoma"
        style:font-family-generic-complex="system" style:font-pitch-complex="variable"
         style:font-size-complex="10pt">
        <xsl:choose>
          <!-- custom title -->
          <xsl:when test="c:tx">
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
              <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="deftxPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
            </xsl:call-template>
          </xsl:when>
          <!-- default title -->
          <xsl:otherwise>
            <xsl:call-template name="TextBoxRunProperties">
              <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              <xsl:with-param name="defRPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </style:text-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="InsertSeriesProperties">
    <!-- @Description: Inserts series style properties -->
    <!-- @Context: Input chart file root -->

    <!-- check if data points are to be inserted in reverse order -->
    <xsl:variable name="reverseCategories">
      <xsl:call-template name="CheckIfCategoriesReversed"/>
    </xsl:variable>

    <!-- check if series are reversed -->
    <xsl:variable name="reverseSeries">
      <xsl:call-template name="CheckIfSeriesReversed"/>
    </xsl:variable>

    <!-- insert data points -->
    <xsl:variable name="numPoints">
      <xsl:choose>
        <!-- for scatter chart -->
        <xsl:when test="key('xNumPoints', c:chartSpace/@oox:part)/descendant::c:ptCount">
          <xsl:value-of
            select="key('xNumPoints', c:chartSpace/@oox:part)/descendant::c:ptCount/@val"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="key('numPoints', c:chartSpace/@oox:part)/descendant::c:ptCount/@val"
          />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:for-each select="key('dataSeries', c:chartSpace/@oox:part)">

      <!-- calculate this series number -->
      <xsl:variable name="seriesNumber">
        <xsl:value-of select="c:idx/@val"/>
      </xsl:variable>

      <style:style style:name="{concat('series',$seriesNumber)}" style:family="chart">

            <xsl:for-each select="c:dLbls[1]">
              <!--test number format of the axis text-->
              <xsl:variable name="formatcode">
                <xsl:if test="c:numFmt">
                  <xsl:value-of select="c:numFmt/@formatCode"/>
                </xsl:if>
              </xsl:variable>
              <xsl:choose>
                <xsl:when test="$formatcode!=''">
                  <xsl:attribute name="style:data-style-name">
                    <xsl:value-of select="generate-id(c:numFmt)"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:attribute name="style:data-style-name">
                    <xsl:value-of select="N0"/>
                  </xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
        <!--changed by sonata for bug no:2107193-->

				<!--<xsl:for-each select="c:val/c:numRef/c:numCache">

          <xsl:variable name="formatcode">
            <xsl:if test="c:formatCode">
              <xsl:value-of select="."/>
            </xsl:if>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="$formatcode!=''">
              <xsl:attribute name="style:data-style-name">
                <xsl:value-of select="generate-id(c:formatCode)"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="style:data-style-name">
                <xsl:value-of select="N0"/>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:for-each>-->
        <!--end-->

        
      
        <style:chart-properties>
          <!-- symbols -->
          <xsl:choose>
            <!-- radar chart symbols -->
            <xsl:when
              test="key('plotArea', @oox:part)/c:radarChart/c:radarStyle/@val = 'marker' and not(c:marker/c:symbol/@val = 'none')">
              <xsl:attribute name="chart:symbol-type">
                <xsl:text>automatic</xsl:text>
              </xsl:attribute>
            </xsl:when>
            
            <xsl:when test="c:marker/c:symbol/@val = 'none' ">
            <xsl:attribute name="chart:symbol-type">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
            </xsl:when>
            <xsl:when test="c:marker/c:symbol/@val != 'none' ">
              <xsl:choose>
                <xsl:when test="c:marker/c:symbol[@val='triangle' or @val='square' or @val='diamond']">
                  <xsl:attribute name="chart:symbol-type">
                    <xsl:text>named-symbol</xsl:text>
                  </xsl:attribute>
                  <xsl:attribute name="chart:symbol-name">
                    <xsl:choose>
                      <xsl:when test="c:marker/c:symbol/@val='triangle'">
                        <xsl:text>arrow-up</xsl:text>
                      </xsl:when>
                      <xsl:when test="c:marker/c:symbol/@val='square'">
                        <xsl:text>square</xsl:text>
                      </xsl:when>
                      <xsl:when test="c:marker/c:symbol/@val='diamond'">
                        <xsl:text>diamond</xsl:text>
                      </xsl:when>
                    </xsl:choose>

                  </xsl:attribute>
                  <xsl:if test="c:marker/c:size/@val">
                    <xsl:variable name="marker_size">
                      <xsl:value-of select="concat(c:marker/c:size/@val * 0.03526087,'cm') "/>
                    </xsl:variable>
                    <xsl:attribute name="chart:symbol-width">
                      <xsl:value-of select="$marker_size"/>
                    </xsl:attribute>
                    <xsl:attribute name="chart:symbol-height">
                      <xsl:value-of select="$marker_size"/>
                    </xsl:attribute>
                  </xsl:if>
                </xsl:when>
                <xsl:otherwise>
            <xsl:attribute name="chart:symbol-type">
              <xsl:text>automatic</xsl:text>
            </xsl:attribute>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
          </xsl:choose>
          
          <xsl:if test="c:explosion">
            <xsl:attribute name="chart:pie-offset">
              <xsl:value-of select="c:explosion/@val"/>
            </xsl:attribute>
          </xsl:if>

          <!-- label -->
          <xsl:for-each select="c:dLbls">
            <xsl:if test="not(c:delete/@val = 1 or c:dLbl/c:delete/@val = 1)">
              <!-- value and/or percentage -->
              <xsl:choose>
                <xsl:when test="c:showVal/@val = 1 and c:showPercent/@val=1 ">
                  <xsl:attribute name="chart:data-label-number">
                    <xsl:text>value-and-percentage</xsl:text>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:showVal/@val = 1 ">
                  <xsl:attribute name="chart:data-label-number">
                    <xsl:text>value</xsl:text>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:showPercent/@val=1 ">
                  <xsl:attribute name="chart:data-label-number">
                    <xsl:text>percentage</xsl:text>
                  </xsl:attribute>
                </xsl:when>
              </xsl:choose>
              <!-- defect#2155191-->
              <xsl:if test="contains(c:numFmt/@formatCode,'%') ">
                <xsl:attribute name="chart:data-label-number">
                  <xsl:text>percentage</xsl:text>
                </xsl:attribute>
              </xsl:if>
              <!-- name -->
              <xsl:if test="c:showCatName/@val = 1 ">
                <xsl:attribute name="chart:data-label-text">
                  <xsl:text>true</xsl:text>
                </xsl:attribute>
              </xsl:if>
              <!-- legend icon -->
              <xsl:if test="c:showLegendKey/@val = 1 ">
                <xsl:attribute name="chart:data-label-symbol">
                  <xsl:text>true</xsl:text>
                </xsl:attribute>
              </xsl:if>
              <!-- Sonata:data lable position -->
              <xsl:choose>
                
                <xsl:when test="c:dLblPos[@val='outEnd']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'outside'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='inEnd']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'inside'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='ctr']">
               <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'center'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='b']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'bottom'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='t']">
                   <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'top'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='r']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'right'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='l']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'left'"/>
                  </xsl:attribute>
                </xsl:when>
                 <xsl:when test="c:dLblPos[@val='inBase']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'near-origin'"/>
                  </xsl:attribute>
                </xsl:when>
                <xsl:when test="c:dLblPos[@val='bestFit']">
                  <xsl:attribute name="chart:label-position">
                    <xsl:value-of select="'near-origin'"/>
                  </xsl:attribute>
                </xsl:when>
              </xsl:choose>
             
            </xsl:if>
          </xsl:for-each>

          <!-- error marker -->
          <xsl:for-each
            select="c:errBars[c:errValType/@val != 'stdErr' and c:errValType/@val != 'stdDev' ]">

            <!-- error type -->
            <xsl:choose>
              <xsl:when test="c:errValType/@val = 'percentage' ">
                <xsl:attribute name="chart:error-category">
                  <xsl:text>percentage</xsl:text>
                </xsl:attribute>
              </xsl:when>
              <xsl:when test="c:errValType/@val = 'fixedVal' or c:errValType/@val = 'cust' ">
                <xsl:attribute name="chart:error-category">
                  <xsl:text>constant</xsl:text>
                </xsl:attribute>
              </xsl:when>
            </xsl:choose>

            <!-- error range -->
            <xsl:if test="c:errValType/@val = 'percentage' ">
              <xsl:attribute name="chart:error-percentage">
                <xsl:value-of select="c:val/@val"/>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="c:errValType/@val = 'fixedVal' ">
              <xsl:for-each select="c:val">
                <xsl:attribute name="chart:error-lower-limit">
                  <xsl:value-of select="@val"/>
                </xsl:attribute>
                <xsl:attribute name="chart:error-upper-limit">
                  <xsl:value-of select="@val"/>
                </xsl:attribute>
              </xsl:for-each>
            </xsl:if>
            <xsl:if test="c:errValType/@val = 'cust' ">
              <xsl:attribute name="chart:error-lower-limit">
                <xsl:value-of select="c:minus/c:numLit/c:pt[1]/c:v"/>
              </xsl:attribute>
              <xsl:attribute name="chart:error-upper-limit">
                <xsl:value-of select="c:plus/c:numLit/c:pt[1]/c:v"/>
              </xsl:attribute>
            </xsl:if>

            <!-- indicators -->
            <xsl:if test="c:errBarType/@val = 'plus' or c:errBarType/@val = 'both' ">
              <xsl:attribute name="chart:error-upper-indicator">
                <xsl:text>true</xsl:text>
              </xsl:attribute>
            </xsl:if>
            <xsl:if test="c:errBarType/@val = 'minus' or c:errBarType/@val = 'both' ">
              <xsl:attribute name="chart:error-lower-indicator">
                <xsl:text>true</xsl:text>
              </xsl:attribute>
            </xsl:if>

          </xsl:for-each>

          <!-- trend line -->
          <xsl:if test="c:trendline">
            <xsl:attribute name="chart:regression-type">
              <xsl:choose>
                <xsl:when test="c:trendline/c:trendlineType/@val = 'linear' ">
                  <xsl:text>linear</xsl:text>
                </xsl:when>
                <xsl:when test="c:trendline/c:trendlineType/@val = 'log' ">
                  <xsl:text>logarithmic</xsl:text>
                </xsl:when>
                <xsl:when test="c:trendline/c:trendlineType/@val = 'exp' ">
                  <xsl:text>exponential</xsl:text>
                </xsl:when>
                <xsl:when test="c:trendline/c:trendlineType/@val = 'power' ">
                  <xsl:text>power</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>linear</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </xsl:if>

        </style:chart-properties>

        <style:graphic-properties>

          <!-- default stroke for scatter and bubble chart -->
          <xsl:if
            test="key('plotArea', @oox:part)/c:bubbleChart or key('plotArea', @oox:part)/c:barChart or key('plotArea', @oox:part)/c:bar3DChart or 
            key('plotArea', @oox:part)/c:pie3DChart or key('plotArea', @oox:part)/c:pieChart">
            <xsl:attribute name="draw:stroke">
              <xsl:text>none</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <xsl:if test="key('plotArea', @oox:part)/c:scatterChart and not(c:smooth)">
            <xsl:attribute name="draw:stroke">
              <xsl:text>solid</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <!-- default line width for line chart -->
          <xsl:if
            test="key('plotArea', @oox:part)/c:lineChart or key('plotArea', @oox:part)/c:scatterChart or c:smooth or key('plotArea', @oox:part)/c:radarChart">
            <xsl:attribute name="svg:stroke-width">
              <xsl:text>0.079cm</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <!-- default line chart line color -->
          <xsl:if test="key('plotArea', @oox:part)/c:lineChart and not(c:spPr/a:ln/a:noFill)">
            <xsl:variable name="defaultColor">
              <xsl:call-template name="InsertDefaultChartSeriesColor">
                <xsl:with-param name="number">
                  <xsl:value-of select="$seriesNumber"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:attribute name="svg:stroke-color">
              <xsl:value-of select="$defaultColor"/>
            </xsl:attribute>
          </xsl:if>
          
          <!-- default fill color  -->
          <xsl:if test="not(c:spPr/a:noFill or c:spPr/a:gradFill or c:spPr/a:blipFill)">
            <xsl:variable name="defaultColor">
              <xsl:call-template name="InsertDefaultChartSeriesColor">
                <xsl:with-param name="number">
                  <xsl:value-of select="$seriesNumber"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:attribute name="draw:fill-color">
              <xsl:value-of select="$defaultColor"/>
            </xsl:attribute>
          </xsl:if>
          <!-- Sonata: chart Data series fill properties-->
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
          </xsl:for-each>

        </style:graphic-properties>
        <style:text-properties fo:font-family="Calibri" style:font-family-generic="swiss"
          style:font-pitch="variable" fo:font-size="10pt"
          style:font-family-asian="&apos;MS Gothic&apos;"
          style:font-family-generic-asian="system" style:font-pitch-asian="variable"
          style:font-size-asian="10pt" style:font-family-complex="Tahoma"
          style:font-family-generic-complex="system" style:font-pitch-complex="variable"
          style:font-size-complex="10pt">
          <xsl:for-each select="c:dLbls">
            <xsl:if test="not(c:dLbls/c:delete/@val = 1)">
              <xsl:choose>
                <!-- custom title -->
                <xsl:when test="c:tx">
                  <xsl:call-template name="TextBoxRunProperties">
                    <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                    <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                    <xsl:with-param name="deftxPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                  </xsl:call-template>
                </xsl:when>
                <!-- default title -->
                <xsl:otherwise>
                  <xsl:call-template name="TextBoxRunProperties">
                    <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                    <xsl:with-param name="defRPr" select="ancestor::c:chartSpace/c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                 </xsl:call-template>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>
          </xsl:for-each>
        </style:text-properties>
      </style:style>

      <xsl:if test="c:trendline">
        <style:style style:name="{concat('trend',$seriesNumber)}" style:family="chart">
          <style:graphic-properties draw:stroke="solid" svg:stroke-width="0cm"
            svg:stroke-color="#004586" svg:stroke-opacity="100%" draw:stroke-linejoin="none">
            <!-- Sonata: chart trendline properties-->
            <xsl:for-each select="c:trendline/c:spPr">
              <xsl:call-template name="InsertChartFillProperties"/>
            </xsl:for-each>
          </style:graphic-properties>
        </style:style>
      </xsl:if>

      <!-- insert this series data points styles -->
      <xsl:choose>
        <xsl:when
          test="key('plotArea',@oox:part)/c:pieChart or key('plotArea',@oox:part)/c:pie3DChart">
          <xsl:if test="position() = 1">
            <xsl:call-template name="InsertPieDataPointsProperties">
              <xsl:with-param name="numPoints" select="$numPoints - 1"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:when>
        <xsl:when test="c:dPt or c:dLbls">
          <xsl:call-template name="InsertDataPointsProperties">
            <xsl:with-param name="seriesNum">
              <xsl:value-of select="$seriesNumber"/>
            </xsl:with-param>
            <xsl:with-param name="numPoints">
              <xsl:value-of select="$numPoints - 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
      </xsl:choose>

    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertPieDataPointsProperties">
    <!-- @Description: Inserts data points properties for pie charts  -->
    <!-- @Context: c:ser -->

    <xsl:param name="numPoints"/>
    <!-- (int) total number of points in chart reduced by 1-->
    <xsl:param name="current" select="0"/>
    <!-- (int) number of currently processed point (zero-based) -->

    <xsl:choose>
      <xsl:when test="c:dPt[c:idx/@val = $numPoints - $current]">
        <xsl:call-template name="InsertDataPointsPropertiesStyle">
          <xsl:with-param name="seriesNum" select="0"/>
          <xsl:with-param name="current" select="$numPoints - $current"/>
          <xsl:with-param name="point" select="$numPoints - $current"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <style:style style:name="{concat('data0-',$numPoints - $current)}" style:family="chart">
          <xsl:choose>
            <xsl:when test="c:dLbls/c:dLbl[c:idx/@val = $numPoints - $current]">
              <xsl:for-each select="c:dLbls/c:dLbl[c:idx/@val = $numPoints - $current]">
                <!--test number format of the axis text-->
                <xsl:variable name="formatcode">
                  <xsl:if test="c:numFmt">
                    <xsl:value-of select="c:numFmt/@formatCode"/>
                  </xsl:if>
                </xsl:variable>
                <xsl:choose>
                  <xsl:when test="$formatcode!=''">
                    <xsl:attribute name="style:data-style-name">
                      <xsl:value-of select="generate-id(c:numFmt)"/>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:attribute name="style:data-style-name">
                      <xsl:value-of select="N0"/>
                    </xsl:attribute>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:for-each>
            </xsl:when>
            </xsl:choose>
          
          <!--Edited by Sonata -->
          <!-- label -->
          <xsl:for-each select="c:dLbls">
            <!-- if label wasn't deleted from data point -->
            <xsl:if test="not(c:dLbl[c:idx/@val = $numPoints - $current]/c:delete/@val=1)">
              <xsl:choose>
                <!-- take data point label type -->
                <xsl:when test="c:dLbl[c:idx/@val = $numPoints - $current]">
                  <xsl:for-each select="c:dLbl[c:idx/@val = $numPoints - $current]">
                    <style:chart-properties>
                      <xsl:call-template name="InsertDataPointLabelAttributes"/>
                    </style:chart-properties>
                  </xsl:for-each>
                </xsl:when>
                <!-- take series label type -->
                <xsl:otherwise>
                  <style:chart-properties>
                    <xsl:call-template name="InsertDataPointLabelAttributes"/>
                  </style:chart-properties>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>
          </xsl:for-each>

          <style:graphic-properties draw:fill="solid">
            <xsl:attribute name="draw:fill-color">
              <xsl:call-template name="InsertDefaultChartSeriesColor">
                <xsl:with-param name="number" select="$numPoints - $current"/>
              </xsl:call-template>
            </xsl:attribute>
          </style:graphic-properties>
        </style:style>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:if test="$current != $numPoints">
      <xsl:call-template name="InsertPieDataPointsProperties">
        <xsl:with-param name="numPoints" select="$numPoints"/>
        <xsl:with-param name="current" select="$current +1"/>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertDataPointsProperties">
    <!-- @Description: Inserts data points properties  -->
    <!-- @Context: c:ser -->
    <xsl:param name="seriesNum"/>
    <!-- (int) sequential number of currently processed series -->
    <xsl:param name="numPoints"/>
    <!-- (int) number of all data points in series -->
    <xsl:param name="current" select="0"/>
    <!-- (int) current data point number (zero based) -->
    <xsl:param name="prev" select="-1"/>
    <!-- (int) previous non-default data point number (zero based) -->

    <!-- does this data point has label -->
    <xsl:variable name="hasLabel">
      <xsl:choose>
        <xsl:when test="c:dLbls/c:dLbl/c:idx[@val = $current]">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="c:dPt[c:idx/@val = $current] or $hasLabel = 'true' ">

        <xsl:call-template name="InsertDataPointsPropertiesStyle">
          <xsl:with-param name="seriesNum" select="$seriesNum"/>
          <xsl:with-param name="current" select="$current"/>
          <xsl:with-param name="prev" select="$prev"/>
        </xsl:call-template>

        <!-- next data point style -->
        <xsl:if test="$current != $numPoints">
          <xsl:call-template name="InsertDataPointsProperties">
            <xsl:with-param name="seriesNum" select="$seriesNum"/>
            <xsl:with-param name="numPoints" select="$numPoints"/>
            <xsl:with-param name="prev" select="$current"/>
            <xsl:with-param name="current" select="$current + 1"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:when>

      <!-- if data point is default process next data point style -->
      <xsl:otherwise>
        <xsl:if test="$current != $numPoints">
          <xsl:call-template name="InsertDataPointsProperties">
            <xsl:with-param name="seriesNum" select="$seriesNum"/>
            <xsl:with-param name="numPoints" select="$numPoints"/>
            <xsl:with-param name="prev" select="$prev"/>
            <xsl:with-param name="current" select="$current + 1"/>
          </xsl:call-template>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertDataPointsPropertiesStyle">
    <!-- @Description: Inserts data points properties style  -->
    <!-- @Context: c:ser -->

    <xsl:param name="seriesNum"/>
    <!-- (int) sequential number of currently processed series -->
    <xsl:param name="current"/>
    <!-- (int) current data point number (zero based) -->
    <xsl:param name="point" select="$current"/>

    <style:style style:name="{concat('data',$seriesNum,'-',$point)}" style:family="chart">

      <xsl:choose>
        <xsl:when test="c:dLbls/c:dLbl[c:idx/@val =$current]">
          <xsl:for-each select="c:dLbls/c:dLbl[c:idx/@val =$current]">
            <!--test number format of the axis text-->
            <xsl:variable name="formatcode">
              <xsl:if test="c:numFmt">
                <xsl:value-of select="c:numFmt/@formatCode"/>
              </xsl:if>
            </xsl:variable>
            <xsl:choose>
              <xsl:when test="$formatcode!=''">
                <xsl:attribute name="style:data-style-name">
                  <xsl:value-of select="generate-id(c:numFmt)"/>
                </xsl:attribute>
              </xsl:when>
              <xsl:otherwise>
                <xsl:attribute name="style:data-style-name">
                  <xsl:value-of select="N0"/>
                </xsl:attribute>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
      <!-- label -->
      <xsl:for-each select="c:dLbls">
        <!-- if label wasn't deleted from data point -->
        <xsl:if test="not(c:dLbl[c:idx/@val =$current]/c:delete/@val=1)">
          <xsl:choose>
            <!-- take data point label type -->
            <xsl:when test="c:dLbl[c:idx/@val =$current]">
              <xsl:for-each select="c:dLbl[c:idx/@val =$current]">
                <style:chart-properties>
                  <xsl:call-template name="InsertDataPointLabelAttributes"/>
                </style:chart-properties>
              </xsl:for-each>
            </xsl:when>
            <!-- take series label type -->
            <xsl:otherwise>
              <style:chart-properties>
                <xsl:call-template name="InsertDataPointLabelAttributes"/>
              </style:chart-properties>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>
      </xsl:for-each>

      <style:graphic-properties>

        <!-- default fill color  -->
        <xsl:if
          test="key('plotArea', @oox:part)/c:pieChart or key('plotArea', @oox:part)/c:pie3DChart">
          <xsl:if
            test="not(c:dPt[c:idx/@val = $current]/c:spPr/a:noFill or c:dPt[c:idx/@val = $current]/c:spPr/a:gradFill or c:dPt[c:idx/@val = $current]/c:spPr/a:blipFill)">
            <xsl:variable name="defaultColor">
              <xsl:call-template name="InsertDefaultChartSeriesColor">
                <xsl:with-param name="number">
                  <xsl:value-of select="c:idx/@val"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:attribute name="draw:fill-color">
              <xsl:value-of select="$defaultColor"/>
            </xsl:attribute>
          </xsl:if>
        </xsl:if>

        <xsl:for-each select="c:dPt[c:idx/@val = $current]/c:spPr">
          <!-- Sonata: chart data point properties-->
          <xsl:call-template name="InsertChartFillProperties"/>
        </xsl:for-each>

      </style:graphic-properties>
      <xsl:for-each select="c:dLbls/c:dLbl[c:idx/@val =$current]">
        <style:text-properties fo:font-family="Calibri"  style:font-family-generic="swiss"
        style:font-pitch="variable" fo:font-size="8pt"
        style:font-family-asian="&apos;MS Gothic&apos;"
        style:font-family-generic-asian="system" style:font-pitch-asian="variable"
        style:font-size-asian="8pt" style:font-family-complex="Tahoma"
        style:font-family-generic-complex="system" style:font-pitch-complex="variable"
        style:font-size-complex="8pt">

          <!-- default "bold" font attribute for default title -->
          <xsl:if test="not(c:tx) and not(c:txPr)">
            <xsl:attribute name="fo:font-weight">
              <xsl:text>bold</xsl:text>
            </xsl:attribute>
          </xsl:if>
          <xsl:choose>
            <!-- custom title -->
            <xsl:when test="c:tx">
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:tx/c:rich/a:p[1]/a:r[1]/a:rPr"/>
                <xsl:with-param name="defRPr" select="c:tx/c:rich/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:when>
            <!-- default title -->
            <xsl:otherwise>
              <xsl:call-template name="TextBoxRunProperties">
                <xsl:with-param name="rPr" select="c:txPr/a:p[1]/a:pPr/a:defRPr"/>
                <xsl:with-param name="deftxPr" select="../../c:txPr/a:p[1]/a:pPr/a:defRPr"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </style:text-properties>
      </xsl:for-each>

    </style:style>
  </xsl:template>

  <xsl:template name="InsertErrorProperties">
    <xsl:for-each
      select="key('error', c:chartSpace/@oox:part)[c:errValType/@val != 'stdErr' and c:errValType/@val != 'stdDev' ]">
      <style:style style:name="{concat('error',generate-id(.))}" style:family="chart">
        <style:graphic-properties draw:stroke="solid" svg:stroke-width="0cm"
          svg:stroke-color="#000000" draw:marker-start="" draw:marker-start-width="0.2cm"
          draw:marker-start-center="false" draw:marker-end="" draw:marker-end-width="0.2cm"
          draw:marker-end-center="false" svg:stroke-opacity="100%">
          <xsl:for-each select="c:spPr">
            <xsl:call-template name="InsertLineColor"/>
            <xsl:call-template name="InsertLineStyle"/>
          </xsl:for-each>
        </style:graphic-properties>
      </style:style>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertWallProperties">
    <!-- c:chartSpace/c:chart/c:backWall -->
    <style:style style:name="wall" style:family="chart">
      <style:graphic-properties draw:stroke="none" draw:fill="solid" draw:fill-color="#ffffff">
     <xsl:choose>
        <xsl:when test="c:chartSpace/c:chart/c:sideWall">
          <!-- Sonata: chart ploat area fill properties-->
          <xsl:for-each select="c:chartSpace/c:chart/c:sideWall/c:spPr">
            <xsl:call-template name="InsertChartFillProperties"/>
              </xsl:for-each>
            </xsl:when>
        <xsl:when test="c:chartSpace/c:chart/c:plotArea">
          <xsl:for-each select="c:chartSpace/c:chart/c:plotArea">
            <!-- Insert Ploat Area Background style, color, fill, transparency -->
            <xsl:for-each select="c:spPr">
              <xsl:call-template name="InsertChartFillProperties"/>
            </xsl:for-each>
          </xsl:for-each>
            </xsl:when>
      </xsl:choose>
  </style:graphic-properties>
    </style:style>
  </xsl:template>
<!--added by Sonata-->
  <xsl:template name="InsertStockRangeProperties">

   <style:style style:name="stock-range" style:family="chart">
      <style:graphic-properties >
        <!-- Sonata: chart stock range fill properties-->
        <xsl:for-each select="c:chartSpace/c:chart/c:plotArea/c:stockChart/c:hiLowLines/c:spPr">
          <xsl:call-template name="InsertChartFillProperties"/>
        </xsl:for-each>
      </style:graphic-properties>
    </style:style>
  </xsl:template>
  <!--end-->
  <xsl:template name="InsertFloorProperties">
    <!-- c:chartSpace/c:chart/c:floor -->
    <style:style style:name="floor" style:family="chart">
      <style:graphic-properties draw:stroke="solid" svg:stroke-width="0.03cm"
        draw:marker-start-width="0.25cm" draw:marker-end-width="0.25cm" draw:fill="none"
        draw:fill-color="#999999">
        <!-- Sonata: chart floor fill properties-->
        <xsl:for-each select="c:chartSpace/c:chart/c:backWall/c:spPr">
          <xsl:call-template name="InsertChartFillProperties"/>
    </xsl:for-each>
      </style:graphic-properties>
    </style:style>
  </xsl:template>
  <xsl:template name ="AddDashType">
    <!-- Sonata: Insert dash style for line style of chart-->
    <xsl:param name ="name" />
    <xsl:param name ="cap" />
    <xsl:param name ="dot1" />
    <xsl:param name ="dot1-length" />
    <xsl:param name ="dot2" />
    <xsl:param name ="dot2-length" />
    <xsl:param name ="distance" />
 <draw:stroke-dash>
      <xsl:if test ="$cap='rnd'">
        <xsl:attribute name ="draw:name">
          <xsl:value-of select ="concat($name,'Round')"/>
        </xsl:attribute>
        <xsl:attribute name ="draw:style">
          <xsl:value-of select ="'round'"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test ="not($cap) or ($cap!='rnd')">
        <xsl:attribute name ="draw:name">
          <xsl:value-of select ="$name"/>
        </xsl:attribute>
        <xsl:attribute name ="draw:style">
          <xsl:value-of select ="'rect'"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:attribute name ="draw:display-name">
        <xsl:value-of select ="$name"/>
      </xsl:attribute>
      <xsl:if test ="(string-length($dot1) != 0)">
        <xsl:attribute name ="draw:dots1">
          <xsl:value-of select ="$dot1" />
        </xsl:attribute>
      </xsl:if>
      <xsl:if test ="(string-length($dot1-length) != 0)">
        <xsl:attribute name ="draw:dots1-length">
          <xsl:value-of select ="concat($dot1-length,'cm')" />
        </xsl:attribute>
      </xsl:if>
      <xsl:if test ="(string-length($dot2) != 0)">
        <xsl:attribute name ="draw:dots2">
          <xsl:value-of select ="$dot2" />
        </xsl:attribute>
      </xsl:if>
      <xsl:if test ="(string-length($dot2-length) != 0)">
        <xsl:attribute name ="draw:dots2-length">
          <xsl:value-of select ="concat($dot2-length,'cm')" />
        </xsl:attribute>
      </xsl:if>
      <xsl:if test ="(string-length($distance) != 0)">
        <xsl:attribute name ="draw:distance">
          <xsl:value-of select ="concat($distance,'cm')"/>
        </xsl:attribute>
      </xsl:if>
    </draw:stroke-dash>

  </xsl:template>
  <xsl:template name="InsertDrawFillImage">
    <xsl:param name="chartId"/>
    <xsl:param name="inputChart"/>

    <xsl:for-each select="key('spPr', c:chartSpace/@oox:part)/a:blipFill">


      <xsl:variable name="pzipsource">
        <xsl:call-template name="GetTarget">
          <xsl:with-param name="document">
            <xsl:value-of select="$inputChart"/>
          </xsl:with-param>
          <xsl:with-param name="id">
            <xsl:value-of select="a:blip/@r:embed"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="pziptarget">
        <xsl:value-of
          select="concat('Object ',$chartId,'/Pictures/',substring-after(substring-after($pzipsource, '/'), '/'))"
        />
      </xsl:variable>

      <pzip:copy pzip:source="{concat('xl/',substring-after($pzipsource, '/'))}"
        pzip:target="{$pziptarget}"/>

      <draw:fill-image xlink:type="simple" xlink:show="embed" xlink:actuate="onLoad">

        <xsl:attribute name="draw:name">
          <xsl:value-of select="generate-id()"/>
        </xsl:attribute>

        <xsl:attribute name="xlink:href">
          <xsl:value-of
            select="concat('Pictures/',substring-after(substring-after($pzipsource, '/'), '/'))"/>
        </xsl:attribute>
      </draw:fill-image>
    </xsl:for-each>

    <xsl:for-each select="key('spPr', c:chartSpace/@oox:part)/a:gradFill">
      <draw:gradient>
        <xsl:attribute name="draw:name">
          <xsl:value-of select="generate-id()"/>
        </xsl:attribute>
        <xsl:attribute name="draw:display-name">
          <xsl:value-of select="generate-id()"/>
        </xsl:attribute>
        <xsl:call-template name="tmpGradientFillTiletoRect"/>
      </draw:gradient>
    </xsl:for-each>

  </xsl:template>

  <xsl:template name="TextRotation">

    <!-- text rotation -->
    <xsl:if test="@rot">
      <xsl:attribute name="style:rotation-angle">
        <xsl:choose>
          <xsl:when test="@rot = 0">
            <xsl:text>0</xsl:text>
          </xsl:when>
          <xsl:when test="@rot &lt; 0">
            <xsl:value-of select="substring-after(@rot,'-') div 60000"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="360 - @rot div 60000"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>

    <!-- vertiacally stacked-->
    <xsl:if test="@vert != '' and @vert != 'horz' ">
      <xsl:attribute name="style:direction">
        <xsl:text>ttb</xsl:text>
      </xsl:attribute>
    </xsl:if>

  </xsl:template>

  <xsl:template name="SetAxisChartProperties">
    <xsl:param name="axisId"/>

    <!-- valAx id (for axis at) -->
    <xsl:variable name="id">
      <xsl:value-of select="generate-id()"/>
    </xsl:variable>

    <!-- axis labels -->
    <xsl:if test="c:tickLblPos/@val = 'none' ">
      <xsl:attribute name="chart:display-label">
        <xsl:text>false</xsl:text>
      </xsl:attribute>
    </xsl:if>

    <!-- axis label rotation -->
    <xsl:for-each select="c:txPr/a:bodyPr">
      <xsl:call-template name="TextRotation"/>
    </xsl:for-each>

    <!-- logarythmic scale -->
    <xsl:if test="c:scaling/c:logBase">
      <xsl:attribute name="chart:logarithmic">
        <xsl:text>true</xsl:text>
      </xsl:attribute>
    </xsl:if>

	  <!--reverse direction-->

	  <xsl:if test="c:scaling/c:orientation/@val='minMax'">
		  <xsl:attribute name="chart:reverse-direction">
			  <xsl:text>false</xsl:text>
		  </xsl:attribute>
	  </xsl:if>

	  <xsl:if test="c:scaling/c:orientation/@val='maxMin'">
		  <xsl:attribute name="chart:reverse-direction">
			  <xsl:text>true</xsl:text>
		  </xsl:attribute>
	  </xsl:if>
	  
    <!-- min/max value-->
    <xsl:if test="c:scaling/c:max">
      <xsl:attribute name="chart:maximum">
        <xsl:value-of select="c:scaling/c:max/@val"/>
      </xsl:attribute>
    </xsl:if>

    <!--xsl:choose-->
    <!-- for stock chart type 3 or type 4 -->
    <!--xsl:when
        test="c:scaling/c:min and key('plotArea', ancestor::c:chartSpace/@oox:part)/c:valAx[2]/c:scaling[1]/c:min/@val and name(self::node()) = 'valAx[2]'">
        <xsl:attribute name="chart:minimum">
          <xsl:value-of select="key('plotArea', ancestor::c:chartSpace/@oox:part)/c:valAx[2]/c:scaling[1]/c:min/@val"/>
        </xsl:attribute>
      </xsl:when-->
    <!-- other charts -->
    <!--xsl:when test="c:scaling/c:min"-->
    <xsl:if test="c:scaling/c:min">
      <xsl:attribute name="chart:minimum">
        <xsl:value-of select="c:scaling/c:min/@val"/>
      </xsl:attribute>
    </xsl:if>
    <!--/xsl:when>
    </xsl:choose-->

    <!-- major interval marks-->
    <xsl:if test="c:majorTickMark">
      <xsl:choose>
        <!-- cross-->
        <xsl:when test="c:majorTickMark/@val = 'cross' ">
          <xsl:attribute name="chart:tick-marks-major-inner">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-major-outer">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- in -->
        <xsl:when test="c:majorTickMark/@val = 'in' ">
          <xsl:attribute name="chart:tick-marks-major-inner">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-major-outer">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- out -->
        <xsl:when test="c:majorTickMark/@val = 'out' ">
          <xsl:attribute name="chart:tick-marks-major-inner">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-major-outer">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- none -->
        <xsl:when test="c:majorTickMark/@val = 'none' ">
          <xsl:attribute name="chart:tick-marks-major-inner">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-major-outer">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>
    </xsl:if>

    <!-- minor interval marks-->
    <xsl:if test="c:minorTickMark">
      <xsl:choose>
        <!-- cross-->
        <xsl:when test="c:minorTickMark/@val = 'cross' ">
          <xsl:attribute name="chart:tick-marks-minor-inner">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-minor-outer">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- in -->
        <xsl:when test="c:minorTickMark/@val = 'in' ">
          <xsl:attribute name="chart:tick-marks-minor-inner">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-minor-outer">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <!-- out -->
        <xsl:when test="c:minorTickMark/@val = 'out' ">
          <xsl:attribute name="chart:tick-marks-minor-inner">
            <xsl:text>false</xsl:text>
          </xsl:attribute>
          <xsl:attribute name="chart:tick-marks-minor-outer">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>
    </xsl:if>

    <!-- axis at -->
    <!-- Edited by Sonata: to set Axis' start point -->
    <xsl:for-each
      select="parent::node()/child::node()[contains(name(),'Ax')]/c:axId[@val =$axisId]">
      <xsl:if test="../c:crossesAt/@val">
        <xsl:attribute name="chart:origin">
          <xsl:value-of select="../c:crossesAt/@val"/>
        </xsl:attribute>
      </xsl:if>

    </xsl:for-each>

    <!--xsl:for-each select="parent::node()/c:valAx[generate-id(.) != $id][1]">
      <xsl:choose-->
    <!-- for stock chart type 3 and 4 -->
    <!--xsl:when test="key('plotArea', ancestor::c:chartSpace/@oox:part)/c:stockChart and key('plotArea', ancestor::c:chartSpace/@oox:part)/c:barChart">
          <xsl:attribute name="chart:origin">
            <xsl:value-of select="key('plotArea', ancestor::c:chartSpace/@oox:part)/c:valAx[2]/c:scaling[1]/c:min/@val"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="c:crossesAt">
          <xsl:attribute name="chart:origin">
            <xsl:value-of select="c:crossesAt/@val"/>
          </xsl:attribute>
        </xsl:when>
      </xsl:choose>

    </xsl:for-each-->

    <!-- major unit -->
    <xsl:choose>
      <xsl:when test="c:majorUnit">
        <xsl:attribute name="chart:interval-major">
          <xsl:value-of select="c:majorUnit/@val"/>
        </xsl:attribute>
        <xsl:variable name="val">
          <xsl:value-of select="c:majorUnit/@val div c:minorUnit/@val"/>
        </xsl:variable>
        <xsl:if test="c:minorUnit and $val != 'NaN'">
          <xsl:attribute name="chart:interval-minor-divisor">
            <xsl:value-of select="c:majorUnit/@val div c:minorUnit/@val"/>
          </xsl:attribute>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>
        <!-- when there is no majorUnit it has to be maximum of scale to calculate minor-divisor -->
        <xsl:if test="c:minorUnit and c:scaling/c:max">
          <xsl:attribute name="chart:interval-minor-divisor">
            <xsl:value-of select="c:scaling/c:max/@val div c:minorUnit/@val"/>
          </xsl:attribute>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="InsertDefaultChartSeriesColor">
    <xsl:param name="number"/>
    
    <xsl:variable name="colorNum">
      <xsl:value-of select="$number mod 6"/>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="$colorNum = 0">
        <xsl:text>#345b89</xsl:text>
      </xsl:when>
      <xsl:when test="$colorNum = 1">
        <xsl:text>#8b3533</xsl:text>
      </xsl:when>
      <xsl:when test="$colorNum = 2">
        <xsl:text>#6f873c</xsl:text>
      </xsl:when>
      <xsl:when test="$colorNum = 3">
        <xsl:text>#5a4474</xsl:text>
      </xsl:when>
      <xsl:when test="$colorNum = 4">
        <xsl:text>#2e7c90</xsl:text>
      </xsl:when>
      <xsl:when test="$colorNum = 5">
        <xsl:text>#b56a2c</xsl:text>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CheckIfSeriesReversed">
    <!-- @Description: Checks if data series should be reversed  -->
    <!-- @Context: inside input chart file -->

    <xsl:for-each select="c:chartSpace/c:chart/c:plotArea">
      <xsl:choose>
        <!--added condition by sonata for bug no:2567199-->
        <xsl:when
          test="(c:barChart/c:barDir/@val = 'bar' and c:barChart/c:grouping/@val = 'clustered' ) 
          or (c:barChart/c:barDir/@val = 'bar' and c:barChart/c:grouping/@val = 'stacked' )
          or c:areaChart/c:grouping/@val = 'standard' or c:doughnutChart">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="CheckIfCategoriesReversed">
    <!-- @Description: Checks if data categories should be reversed  -->
    <!-- @Context: inside input chart file -->

    <xsl:for-each select="c:chartSpace/c:chart/c:plotArea">
      <xsl:choose>
        <!--changed by sonata for bug no:2557071-->
        <!--<xsl:when
          test="c:barChart/c:barDir/@val = 'bar' or c:pieChart or c:pie3DChart or c:ofPieChart">-->
        <xsl:when
        test="c:barChart/c:barDir/@val = 'bar' or c:ofPieChart">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <!--end-->
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertDataPointLabelAttributes">
    <!-- value and/or percentage -->
    <xsl:choose>
      <xsl:when test="c:showVal/@val = 1 and c:showPercent/@val=1 ">
        <xsl:attribute name="chart:data-label-number">
          <xsl:text>value-and-percentage</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="c:showVal/@val = 1">
        <xsl:attribute name="chart:data-label-number">
          <xsl:text>value</xsl:text>
        </xsl:attribute>
      </xsl:when>
      <xsl:when test="c:showPercent/@val=1">
        <xsl:attribute name="chart:data-label-number">
          <xsl:text>percentage</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <!-- name -->
    <xsl:if test="c:showCatName/@val = 1 ">
      <xsl:attribute name="chart:data-label-text">
        <xsl:text>true</xsl:text>
      </xsl:attribute>
    </xsl:if>
    <!-- legend icon -->
    <xsl:if test="c:showLegendKey/@val = 1 ">
      <xsl:attribute name="chart:data-label-symbol">
        <xsl:text>true</xsl:text>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>
<!-- Defect:  2682600 , 2186921 
     Code Changes By : Vijayeta
      Desc:          : On Reverse converssion, the source data range is not retained in ODF
                        Additional attributes added to maintain the link between chart abd source data.
  -->
	<xsl:template name ="GetStartMultipleRanges">
		<xsl:param name ="multRanges"/>
		<xsl:param name ="startRange">
			<xsl:value-of select ="''"/>
		</xsl:param>
		<xsl:variable name ="newRange">
			<xsl:choose>
				<xsl:when test ="starts-with($multRanges,'(')">
					<xsl:value-of select ="substring($multRanges,2)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select ="$multRanges"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="finalRange">
			<xsl:value-of select ="$startRange"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$newRange!=''">
				<xsl:call-template name ="GetStartMultipleRanges">
					<xsl:with-param name ="multRanges" >
						<xsl:value-of select ="substring-after($newRange,',')"/>
					</xsl:with-param>
					<xsl:with-param name ="startRange" >
						<xsl:choose>
							<xsl:when test ="contains($newRange,',')">
								<xsl:value-of select ="concat($finalRange,substring-before(substring-before($newRange,','),':'),'|')"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select ="concat($finalRange,substring-before($newRange,':'),'|')"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select ="$startRange"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name ="GetEndMultipleRanges">
		<xsl:param name ="multRanges"/>
		<xsl:param name ="endRange">
			<xsl:value-of select ="''"/>
		</xsl:param>
		<xsl:variable name ="newRange">
			<xsl:choose>
				<xsl:when test ="starts-with($multRanges,'(')">
					<xsl:value-of select ="substring($multRanges,2)"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select ="$multRanges"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="finalRange">
			<xsl:value-of select ="$endRange"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$newRange!=''">
				<xsl:call-template name ="GetEndMultipleRanges">
					<xsl:with-param name ="multRanges" >
						<xsl:value-of select ="substring-after($newRange,',')"/>
					</xsl:with-param>
					<xsl:with-param name ="endRange" >
						<xsl:choose>
							<xsl:when test ="contains($newRange,',')">
								<xsl:value-of select ="concat($finalRange,substring-after(substring-before($newRange,','),':'),'|')"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<xsl:when test ="substring($newRange,string-length($newRange))=')'">
										<xsl:value-of select ="concat($finalRange,substring-after(substring($newRange,0,string-length($newRange)),':'),'|')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select ="concat($finalRange,substring-after($newRange,':'),'|')"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select ="$endRange"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name ="getCompleteRange">
		<xsl:param name ="startRange"/>
		<xsl:param name ="endRange"/>
		<xsl:param name ="finalRange"/>
		<xsl:variable name ="space">
			<xsl:value-of select ="' '"/>
		</xsl:variable>
		<xsl:variable name ="finalRange1">
			<xsl:value-of select ="$finalRange"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$startRange!='' and $endRange!=''">
				<xsl:call-template name ="getCompleteRange">
					<xsl:with-param name ="startRange" >
						<xsl:value-of select ="substring-after($startRange,'|')"/>
					</xsl:with-param>
					<xsl:with-param name ="endRange" >
						<xsl:value-of select ="substring-after($endRange,'|')"/>
					</xsl:with-param>
					<xsl:with-param name ="finalRange" >
						<xsl:value-of select ="concat($finalRange,$space,substring-before($startRange,'|'),':.',substring-before($endRange,'|'))"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select ="translate(substring-after($finalRange,$space),'!','.')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="getChartCategoriesMultipleRanges">
		<xsl:param name="cellRange"/>
		<xsl:param name ="finalRange"/>
		<xsl:variable name ="space">
			<xsl:value-of select ="' '"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$cellRange!=''">
				<xsl:call-template name ="getChartCategoriesMultipleRanges">
					<xsl:with-param name ="cellRange" >
						<xsl:value-of select ="substring-after($cellRange,';')"/>
					</xsl:with-param>
					<xsl:with-param name ="finalRange">
						<xsl:choose>
							<xsl:when test ="contains($cellRange,';')">
								<xsl:value-of select ="concat($finalRange,$space,concat(substring-before(substring-before($cellRange,';'),':'),'.:',substring-after(substring-before($cellRange,';'),':')))"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select ="concat($finalRange,$space,concat(substring-before($cellRange,':'),'.:',substring-after($cellRange,':')))"/>
							</xsl:otherwise>
						</xsl:choose>						
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>				
				<xsl:value-of select ="substring-after($finalRange,' ')"/>					
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
<!--End of code change for defects  2682600 and  2186921 -->
</xsl:stylesheet>
