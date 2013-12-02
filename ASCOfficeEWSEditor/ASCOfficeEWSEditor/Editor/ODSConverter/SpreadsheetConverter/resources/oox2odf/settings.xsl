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
RefNo-1 16-May-2008 Sandeep S     1777584   Changes done to implement Freeze Pane                                              
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main"
  xmlns:ooo="http://openoffice.org/2004/office" office:version="1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:rels="http://schemas.openxmlformats.org/package/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  exclude-result-prefixes="e r w">

  <!--<xsl:import href="relationships.xsl"/>
  <xsl:import href="common.xsl"/>-->
  <!--RefNo-2:ODF1.1:Added office:version="1.1" attribute:06-feb-09-->
  <xsl:template name="InsertSettings">
	  <office:document-settings office:version="1.1">
      <office:settings>
        <config:config-item-set config:name="ooo:view-settings">
          <config:config-item-map-indexed config:name="Views">
            <config:config-item-map-entry>

              <!-- settings based on default active table -->
              <xsl:for-each select="key('Part', 'xl/workbook.xml')">
                <xsl:variable name="ActiveTabNumber">
                  <xsl:choose>
                    <xsl:when test="e:workbook/e:bookViews/e:workbookView/@activeTab">
                      <xsl:value-of select="e:workbook/e:bookViews/e:workbookView/@activeTab"/>
                    </xsl:when>
                    <xsl:otherwise>0</xsl:otherwise>
                  </xsl:choose>
                </xsl:variable>

                <!-- Set default Active Table (Sheet) -->
                <config:config-item config:name="ActiveTable" config:type="string">
                  <xsl:for-each
                    select="e:workbook/e:sheets/e:sheet[position() = $ActiveTabNumber + 1]/@name">
                    <!--Start of RefNo-1: To remove special charecters in the sheet name.-->
                    <xsl:variable name="checkedName">
                      <xsl:call-template name="CheckSheetName">
                        <xsl:with-param name="sheetNumber">
                          <xsl:value-of select="$ActiveTabNumber + 1"/>
                        </xsl:with-param>
                        <xsl:with-param name="name">
                          <xsl:value-of select="translate(.,'!$#():,.+','')"/>
                        </xsl:with-param>
                      </xsl:call-template>
                    </xsl:variable>
                    <!--<xsl:value-of select="."/>-->
                    <xsl:value-of select="$checkedName"/>
                    <!--End of RefNo-1-->
                  </xsl:for-each>
                </config:config-item>
                <!-- Defect : 1877279 
				             File Name: mastereval-v3.xlsx (on roundtrip)
					           Fix By: Vijayeta
					           Desc: to make sure that the zoom value is taken from the active tab, 
					                 and also the fact is considered that the active sheet not necessarily be a worksheet, can also  be a chartsheet, with the path different from the worksheet.
			          -->
                <config:config-item config:name="ZoomValue" config:type="int">
                  <xsl:variable name ="rId">
                    <xsl:value-of select="key('Part', 'xl/workbook.xml')//e:workbook/e:sheets/e:sheet[position() = $ActiveTabNumber + 1]/@r:id"/>
                  </xsl:variable>
                  <xsl:variable name ="sheetNamePath">
                    <xsl:for-each select ="key('Part', 'xl/_rels/workbook.xml.rels')//rels:Relationships/rels:Relationship[@Id=$rId]">
                      <xsl:value-of select ="@Target"/>
                    </xsl:for-each>
                  </xsl:variable>
                  <xsl:choose >
                    <xsl:when test ="contains($sheetNamePath,'chartsheets')">
                      <xsl:for-each select="key('Part', concat('xl/',$sheetNamePath))/e:chartsheet/e:sheetViews/e:sheetView">
                        <xsl:choose>
                          <xsl:when test="not(@view = 'pageBreakPreview') and @zoomScale">
                            <xsl:value-of select="@zoomScale"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:text>100</xsl:text>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:for-each>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:for-each select="key('Part', concat('xl/',$sheetNamePath))/e:worksheet/e:sheetViews/e:sheetView">
                        <xsl:choose>
                          <xsl:when test="not(@view = 'pageBreakPreview') and @zoomScale">
                            <xsl:value-of select="@zoomScale"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:text>100</xsl:text>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:for-each>
                    </xsl:otherwise>
                  </xsl:choose>
                  <!--<xsl:for-each
                    select="key('Part', concat('xl/worksheets/sheet', $ActiveTabNumber + 1,'.xml'))/e:worksheet/e:sheetViews/e:sheetView">
                    <xsl:choose>
                      <xsl:when test="not(@view = 'pageBreakPreview') and @zoomScale">
                        <xsl:value-of select="@zoomScale"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text>100</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:for-each>-->
                </config:config-item>

                <config:config-item config:name="PageViewZoomValue" config:type="int">
                  <xsl:for-each
                    select="key('Part', concat('xl/worksheets/sheet', $ActiveTabNumber + 1,'.xml'))/e:worksheet/e:sheetViews/e:sheetView">
                    <xsl:choose>
                      <xsl:when test="@view = 'pageBreakPreview' and @zoomScale">
                        <xsl:value-of select="@zoomScale"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text>100</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:for-each>
                </config:config-item>

                <config:config-item config:name="ShowPageBreakPreview" config:type="boolean">
                  <xsl:for-each
                    select="key('Part', concat('xl/worksheets/sheet', $ActiveTabNumber + 1,'.xml'))/e:worksheet/e:sheetViews/e:sheetView">
                    <xsl:choose>
                      <xsl:when test="@view = 'pageBreakPreview' ">
                        <xsl:text>true</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>false</xsl:otherwise>
                    </xsl:choose>
                  </xsl:for-each>
                </config:config-item>

                <config:config-item config:name="HasColumnRowHeaders" config:type="boolean">
                  <xsl:for-each
                    select="key('Part', concat('xl/worksheets/sheet', $ActiveTabNumber + 1,'.xml'))/e:worksheet/e:sheetViews/e:sheetView">
                    <xsl:choose>
                      <xsl:when test="@showRowColHeaders = 0">
                        <xsl:text>false</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>true</xsl:otherwise>
                    </xsl:choose>
                  </xsl:for-each>
                </config:config-item>
              </xsl:for-each>

              <config:config-item-map-named config:name="Tables">
                <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet">
                  <xsl:call-template name="InsertCursorPosition">
                    <xsl:with-param name="sheet">
                      <xsl:call-template name="GetTarget">
                        <xsl:with-param name="id">
                          <xsl:value-of select="@r:id"/>
                        </xsl:with-param>
                        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
                      </xsl:call-template>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:for-each>
              </config:config-item-map-named>
            </config:config-item-map-entry>
          </config:config-item-map-indexed>
        </config:config-item-set>
      </office:settings>
    </office:document-settings>
  </xsl:template>

  <xsl:template name="InsertCursorPosition">
    <xsl:param name="sheet"/>
    <!--Start of RefNo-1-->
    <xsl:variable name="checkedName">
      <xsl:call-template name="CheckSheetName">
        <xsl:with-param name="sheetNumber">
          <xsl:value-of select="position()"/>
        </xsl:with-param>
        <xsl:with-param name="name">
          <xsl:value-of select="translate(@name,'!$#():,.+','')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <!--<config:config-item-map-entry config:name="{@name}">-->
    <config:config-item-map-entry config:name="{$checkedName}">
      <xsl:variable name="HSplitPossition">
        <xsl:choose>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@xSplit">
            <xsl:value-of
               select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@xSplit"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="VSplitPossition">
        <xsl:choose>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@ySplit">
            <xsl:value-of
               select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@ySplit"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="actPane">
        <xsl:choose>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@activePane">
            <xsl:value-of
               select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:pane/@activePane"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="actCell">
        <xsl:choose>
          <xsl:when test="$actPane != ''">
            <xsl:choose>
              <xsl:when test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection[@pane = $actPane]/@activeCell">
                <xsl:value-of
                 select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection[@pane = $actPane]/@activeCell"
                  />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="'A1'"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="''"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="actCellX">
        <xsl:choose>
          <xsl:when test="$HSplitPossition != '' or $VSplitPossition != ''">
            <xsl:variable name="col">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell" select="$actCell"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$col - 1"/>
          </xsl:when>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell">
            <xsl:variable name="col">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell"
                  />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$col - 1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="actCellY">
        <xsl:choose>
          <xsl:when test="$HSplitPossition != '' or $VSplitPossition != ''">
            <xsl:variable name="row">
              <xsl:call-template name="GetRowNum">
                <xsl:with-param name="cell" select="$actCell"/>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$row - 1"/>
          </xsl:when>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell">
            <xsl:variable name="row">
              <xsl:call-template name="GetRowNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell"
                  />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$row - 1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <config:config-item config:name="CursorPositionX" config:type="int">
        <xsl:choose>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell">
            <xsl:variable name="col">
              <xsl:call-template name="GetColNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell"
                  />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$col - 1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </config:config-item>
      <config:config-item config:name="CursorPositionY" config:type="int">
        <xsl:choose>
          <xsl:when
            test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell">
            <xsl:variable name="row">
              <xsl:call-template name="GetRowNum">
                <xsl:with-param name="cell">
                  <xsl:value-of
                    select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView/e:selection/@activeCell"
                  />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:variable>
            <xsl:value-of select="$row - 1"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </config:config-item>
      <xsl:if test="$HSplitPossition != '' or $VSplitPossition != ''">
        <config:config-item config:name="HorizontalSplitMode" config:type="short">
          <xsl:choose>
            <xsl:when
              test="$HSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>2</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="VerticalSplitMode" config:type="short">
          <xsl:choose>
            <xsl:when
              test="$VSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>2</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="HorizontalSplitPosition" config:type="int">
          <xsl:choose>
            <xsl:when
              test="$HSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$HSplitPossition"/>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="VerticalSplitPosition" config:type="int">
          <xsl:choose>
            <xsl:when
              test="$VSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$VSplitPossition"/>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="ActiveSplitRange" config:type="short">
          <xsl:choose>
            <xsl:when test="$actCell != ''">
              <xsl:choose>
                <xsl:when test="$VSplitPossition = ''">
                  <xsl:choose>
                    <xsl:when test="$actCellY &gt;= $VSplitPossition">
                      <xsl:text>3</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:text>2</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:when>
                <xsl:when test="$HSplitPossition = ''">
                  <xsl:choose>
                    <xsl:when test="$actCellX &gt;= $HSplitPossition">
                      <xsl:text>2</xsl:text>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:text>0</xsl:text>
                    </xsl:otherwise>
                  </xsl:choose>
                </xsl:when>
                <xsl:when test="$actCellY &gt;= $VSplitPossition and $actCellX &gt;= $HSplitPossition">
                  <xsl:text>3</xsl:text>
                </xsl:when>
                <xsl:when test="$actCellY &gt;= $VSplitPossition and $actCellX &lt; $HSplitPossition">
                  <xsl:text>1</xsl:text>
                </xsl:when>
                <xsl:when test="$actCellY &lt; $VSplitPossition and $actCellX &lt; $HSplitPossition">
                  <xsl:text>0</xsl:text>
                </xsl:when>
                <xsl:when test="$actCellY &lt; $VSplitPossition and $actCellX &gt;= $HSplitPossition">
                  <xsl:text>2</xsl:text>
                </xsl:when>
              </xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>3</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="PositionLeft" config:type="int">0</config:config-item>
        <config:config-item config:name="PositionRight" config:type="int">
          <xsl:choose>
            <xsl:when
              test="$HSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$HSplitPossition"/>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
        <config:config-item config:name="PositionTop" config:type="int">0</config:config-item>
        <config:config-item config:name="PositionBottom" config:type="int">
          <xsl:choose>
            <xsl:when
              test="$VSplitPossition = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$VSplitPossition"/>
            </xsl:otherwise>
          </xsl:choose>
        </config:config-item>
      </xsl:if>
      <!--End of RefNo-1-->
    </config:config-item-map-entry>
  </xsl:template>
</xsl:stylesheet>
