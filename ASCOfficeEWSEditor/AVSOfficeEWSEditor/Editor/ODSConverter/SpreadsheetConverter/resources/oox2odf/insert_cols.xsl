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
RefNo-1 26-Oct-2007 Sandeep S     1757322   Modification done to handle column break within the styled columns 
                                            and to insert column breaks after the last styled columns.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:v="urn:schemas-microsoft-com:vml" xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" 
  xmlns:oox="urn:oox" 
  exclude-result-prefixes="e oox r v">


  <xsl:template match="e:col" mode="header">
    <xsl:param name="number"/>
    <xsl:param name="sheet"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="headerColsStart"/>
    <xsl:param name="headerColsEnd"/>
    <xsl:param name="GroupCell"/>
    <xsl:param name="ManualColBreaks"/>


    <!-- if there were columns with default properties before this column then insert default columns-->
    <xsl:choose>
      <!-- when this column is the first non-default one but it's not the column A -->
      <xsl:when
        test="$number = 1 and @min &gt; 1 and ($headerColsStart= '' or $headerColsStart &gt; 1)">
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <!-- when there is a header -->
              <xsl:when test="$headerColsStart != '' ">
                <xsl:choose>
                  <xsl:when test="@min &lt; $headerColsStart">
                    <xsl:value-of select="@min - 1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$headerColsStart - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <xsl:otherwise>
                <xsl:value-of select="@min - 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>

          <!-- Possible are nesesary code -->
          <!--xsl:if test="@style">            
                      <xsl:variable name="position">
                      <xsl:value-of select="$this/@style + 1"/>
                      </xsl:variable>
                      
                      <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="generate-id(key('Xf', $position))"/>
                      </xsl:attribute>
                      </xsl:if-->

        </table:table-column>

      </xsl:when>
      <!-- when this column is not first non-default one and there were default columns after previous non-default column (if there was a gap between this and previous column)-->
      <xsl:when test="preceding-sibling::e:col[1]/@max &lt; @min - 1 ">
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <!-- when there is a header -->
              <xsl:when test="$headerColsStart != '' ">
                <xsl:choose>
                  <!-- insert empty columns before header -->
                  <xsl:when
                    test="@min &gt; $headerColsStart and preceding-sibling::e:col[1]/@max &lt; $headerColsStart">
                    <xsl:value-of select="$headerColsStart - preceding-sibling::e:col[1]/@max - 1"/>
                  </xsl:when>
                  <!-- insert empty columns after header -->
                  <xsl:when
                    test="@min &gt; $headerColsEnd and preceding-sibling::e:col[1]/@max &lt; $headerColsEnd">
                    <xsl:value-of select="@max - $headerColsEnd - 1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@min - preceding-sibling::e:col[1]/@max - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <xsl:otherwise>
                <xsl:value-of select="@min - preceding-sibling::e:col[1]/@max - 1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

        </table:table-column>
      </xsl:when>
    </xsl:choose>

    <!-- insert this column -->
    <xsl:call-template name="InsertThisColumn">
      <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
      <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
      <xsl:with-param name="GroupCell">
        <xsl:value-of select="$GroupCell"/>
      </xsl:with-param>
      <xsl:with-param name="ManualColBreaks">
        <xsl:value-of select="$ManualColBreaks"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- insert next column -->
    <xsl:choose>
      <!-- calc supports only 256 columns -->
      <xsl:when test="$number &gt; 255">
        <xsl:message terminate="no">translation.oox2odf.ColNumber</xsl:message>
      </xsl:when>

      <xsl:when
        test="following-sibling::e:col[1][@min &lt;= $headerColsEnd and @min &lt; 256]">
        <xsl:apply-templates select="following-sibling::e:col[1]" mode="header">
          <xsl:with-param name="number" select="@max + 1"/>
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="sheet" select="$sheet"/>
          <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
          <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="e:col">
    <xsl:param name="number"/>
    <xsl:param name="sheet"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="headerColsStart"/>
    <xsl:param name="headerColsEnd"/>
    <xsl:param name="GroupCell"/>
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="prevManualBreak" select="0"/>
    <xsl:param name="AfterRow"/>

    <xsl:variable name="GetMinManualColBreak">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:choose>
            <xsl:when test="preceding-sibling::e:col/@max">
              <xsl:value-of select="preceding-sibling::e:col/@max"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>1</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!-- if there were columns with default properties before this column then insert default columns-->
    <xsl:choose>
      <!-- when this column is the first non-default one but it's not the column A -->
      <xsl:when
        test="$number = 1 and @min &gt; 1 and ($headerColsStart= '' or $headerColsStart &gt; 1)">
        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetMinManualColBreak"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>

        </xsl:call-template>

        <xsl:variable name="ManualCol">
          <xsl:call-template name="GetMaxValueBetweenTwoValues">
            <xsl:with-param name="min">
              <!--Start of RefNo-1-->
              <!--<xsl:value-of select="preceding-sibling::e:col/@max +1"/>-->
              <xsl:choose>
                <xsl:when test="preceding-sibling::e:col">
                  <xsl:value-of select="preceding-sibling::e:col/@max +1"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="1"/>
                </xsl:otherwise>
              </xsl:choose>
              <!--End of RefNo-1-->
            </xsl:with-param>
            <xsl:with-param name="max">
              <!--RefNo-1-->
              <xsl:value-of select="@min - 2"/>
            </xsl:with-param>
            <xsl:with-param name="value">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <!--Start of RefNo-1-->
        <!--  <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:for-each select="key('Part', concat('xl/',$sheet))">
              <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
            </xsl:for-each>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
					  <xsl:choose>-->
        <!-- when there is a header -->
        <!-- <xsl:when test="$headerColsStart != '' ">
                <xsl:choose>
                  <xsl:when test="@min &lt; $headerColsStart">
                    <xsl:value-of select="@min - 1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$headerColsStart - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="@min &gt; $GetMinManualColBreak">
                    <xsl:value-of select="@min - ($ManualCol + 1) - 1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@min - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>
				  </xsl:attribute>-->
        <xsl:choose>
          <!-- when there is a header -->
          <xsl:when test="$headerColsStart != '' ">
            <xsl:choose>
              <xsl:when test="@min &lt; $headerColsStart">
                <xsl:if test="@min - 1 &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="@min - 1"/>
                    </xsl:attribute>
                  </table:table-column>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="$headerColsStart - 1 &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="$headerColsStart - 1"/>
                    </xsl:attribute>
                  </table:table-column>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="@min &gt; $GetMinManualColBreak">
                <xsl:if test="(@min - ($ManualCol + 2) - 1) &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="@min - ($ManualCol + 2) - 1"/>
                    </xsl:attribute>
                  </table:table-column>

                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="(@min - 1) &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="@min - 1"/>
                    </xsl:attribute>
                  </table:table-column>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
        <!--End fo RefNo-1-->

        <!-- Possible are nesesary code -->
        <!--xsl:if test="@style">            
            <xsl:variable name="position">
            <xsl:value-of select="$this/@style + 1"/>
            </xsl:variable>
            
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="generate-id(key('Xf', $position))"/>
            </xsl:attribute>
            </xsl:if-->
        <!--</table:table-column>-->

      </xsl:when>
      <!-- when this column is not first non-default one and there were default columns after previous non-default column (if there was a gap between this and previous column)-->
      <xsl:when test="preceding-sibling::e:col[1]/@max &lt; @min - 1">

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetMinManualColBreak"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>

        </xsl:call-template>

        <xsl:variable name="ManualCol">
          <xsl:call-template name="GetMaxValueBetweenTwoValues">
            <xsl:with-param name="min">
              <xsl:text>1</xsl:text>
            </xsl:with-param>
            <xsl:with-param name="max">
              <xsl:value-of select="@min"/>
            </xsl:with-param>
            <xsl:with-param name="value">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
          <!-- when there is a header -->
          <xsl:when test="$headerColsStart != '' ">
            <xsl:choose>
              <xsl:when test="preceding-sibling::e:col/@max + 1 = $headerColsStart"/>
              <!-- insert empty columns before header -->
              <xsl:when
                test="@min &gt; $headerColsStart and preceding-sibling::e:col[1]/@max &lt; $headerColsStart">
                <!--RefNo-1-->
                <xsl:if test="$headerColsStart - preceding-sibling::e:col[1]/@max - 1 &gt; 1">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:if test="$headerColsStart - preceding-sibling::e:col[1]/@max - 1 &gt; 1">
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of select="$headerColsStart - preceding-sibling::e:col[1]/@max - 1"/>
                      </xsl:attribute>
                    </xsl:if>
                  </table:table-column>
                </xsl:if>
              </xsl:when>
              <xsl:when test="@min = $headerColsEnd + 1"/>
              <!-- insert empty columns after header -->
              <xsl:when
                test="@min &gt; $headerColsEnd and preceding-sibling::e:col[1]/@max &lt; $headerColsEnd">
                <!--RefNo-1-->
                <xsl:if test="@min - $headerColsEnd - 1 &gt; 1">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:if test="@min - $headerColsEnd - 1 &gt; 1">
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of select="@min - $headerColsEnd - 1"/>
                      </xsl:attribute>
                    </xsl:if>
                  </table:table-column>
                </xsl:if>
              </xsl:when>
              <!-- insert simple empty rows -->
              <xsl:otherwise>
                <!--RefNo-1-->
                <xsl:if test="@min - preceding-sibling::e:col[1]/@max - 1 &gt; 1">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>

                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>

                    <xsl:if test="@min - preceding-sibling::e:col[1]/@max - 1 &gt; 1">
                      <xsl:attribute name="table:number-columns-repeated">
                        <xsl:value-of select="@min - preceding-sibling::e:col[1]/@max - 1"/>
                      </xsl:attribute>
                    </xsl:if>
                  </table:table-column>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:when>

          <xsl:otherwise>
            <!--Start of RefNo-1
            <table:table-column>
              <xsl:attribute name="table:style-name">
                <xsl:for-each select="key('Part', concat('xl/',$sheet))">
                  <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
                </xsl:for-each>
              </xsl:attribute>

              <xsl:attribute name="table:number-columns-repeated">
                <xsl:choose>
                  <xsl:when test="@min &gt; $GetMinManualColBreak">
                    <xsl:value-of select="@min - ($ManualCol + 1) - 1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@min - preceding-sibling::e:col[1]/@max - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>

              <xsl:attribute name="table:default-cell-style-name">
                <xsl:value-of select="$DefaultCellStyleName"/>
              </xsl:attribute>
            </table:table-column> 
-->
            <xsl:choose>
              <xsl:when test="@min &gt; $GetMinManualColBreak">
                <xsl:if test="(@min - ($ManualCol + 1) - 1) &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="@min - ($ManualCol + 1) - 1"/>
                    </xsl:attribute>
                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>
                  </table:table-column>
                </xsl:if>
              </xsl:when>
              <xsl:otherwise>
                <xsl:if test="(@min - preceding-sibling::e:col[1]/@max - 1) &gt; 0">
                  <table:table-column>
                    <xsl:attribute name="table:style-name">
                      <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                    </xsl:attribute>
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:value-of select="@min - preceding-sibling::e:col[1]/@max - 1"/>
                    </xsl:attribute>
                    <xsl:attribute name="table:default-cell-style-name">
                      <xsl:value-of select="$DefaultCellStyleName"/>
                    </xsl:attribute>
                  </table:table-column>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
            <!--End of RefNo-1-->
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
    </xsl:choose>
    <!-- insert this column -->
    <xsl:choose>
      <!-- if this is the first column after beginning of the header -->
      <xsl:when
        test="$headerColsStart != '' and (@max &gt;= $headerColsStart and not(preceding-sibling::e:col[1]/@max &gt;= $headerColsStart))">

        <!-- insert part of a column range (range: @max > @min) before header-->
        <xsl:if test="@min &lt; $headerColsStart and @max &gt;= $headerColsStart">
          <xsl:call-template name="InsertThisColumn">
            <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
            <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
            <xsl:with-param name="beforeHeader" select="'true'"/>
            <xsl:with-param name="GroupCell">
              <xsl:value-of select="$GroupCell"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>

        <table:table-header-columns>

          <!-- insert column settings at the beginning of the header -->
          <xsl:choose>
            <!-- insert previous column settings -->
            <xsl:when test="preceding-sibling::e:col[1][@max &gt;= $headerColsStart]">
              <xsl:variable name="preceding">
                <xsl:value-of select="preceding-sibling::node()[1]"/>
              </xsl:variable>

              <table:table-column table:style-name="{generate-id(preceding-sibling::node()[1])}">
                <xsl:attribute name="table:number-columns-repeated">
                  <xsl:value-of select="preceding-sibling::node()[1]/@max - $headerColsStart - 1"/>
                </xsl:attribute>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>

                <xsl:if test="preceding-sibling::e:col[1]/@hidden=1">
                  <xsl:attribute name="table:visibility">
                    <xsl:text>collapse</xsl:text>
                  </xsl:attribute>
                </xsl:if>
                <xsl:if test="preceding-sibling::e:col[1]/@style">
                  <xsl:variable name="position">
                    <xsl:value-of select="preceding-sibling::e:col[1]/@style"/>
                  </xsl:variable>
                  <xsl:attribute name="table:default-cell-style-name">
                    <xsl:value-of select="generate-id(key('Xf', $position))"/>
                  </xsl:attribute>
                </xsl:if>
              </table:table-column>
            </xsl:when>
            <xsl:when test="@min &gt; $headerColsStart">
              <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
                <xsl:if test="@min - $headerColsStart &gt; 0">
                  <xsl:attribute name="table:number-columns-repeated">
                    <xsl:choose>
                      <xsl:when test="@min &lt;= $headerColsEnd">
                        <xsl:value-of select="@min - $headerColsStart"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="$headerColsEnd - $headerColsStart + 1"/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:attribute>
                </xsl:if>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>
              </table:table-column>
            </xsl:when>
          </xsl:choose>

          <xsl:if test="@min &lt;= $headerColsEnd">
            <xsl:call-template name="InsertThisColumn">
              <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
              <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
              <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
              <xsl:with-param name="GroupCell">
                <xsl:value-of select="$GroupCell"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:if>

          <!-- insert next header-column -->
          <xsl:if
            test="following-sibling::e:col[@min &lt;= $headerColsEnd and @min &lt; 256][1]">
            <xsl:apply-templates
              select="following-sibling::e:col[@min &lt;= $headerColsEnd and @min &lt; 256][1]"
              mode="header">
              <xsl:with-param name="number" select="@max + 1"/>
              <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
              <xsl:with-param name="sheet" select="$sheet"/>
              <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
              <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
              <xsl:with-param name="GroupCell">
                <xsl:value-of select="$GroupCell"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:if>

          <!-- insert empty columns at the end of the header -->
          <xsl:for-each
            select="parent::node()/e:col[@min &lt;= 256 and @max &gt;= $headerColsStart and @min &lt;= $headerColsEnd][last()]">
            <xsl:if test="@max &lt; $headerColsEnd">
              <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
                <xsl:attribute name="table:number-columns-repeated">
                  <xsl:value-of select="$headerColsEnd - @max"/>
                </xsl:attribute>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>
              </table:table-column>
            </xsl:if>
          </xsl:for-each>

        </table:table-header-columns>

        <!-- if this column range starts inside header, but ends outside write rest of the columns outside header -->
        <xsl:if test="@min &lt;= $headerColsEnd and @max &gt; $headerColsEnd">
          <xsl:call-template name="InsertThisColumn">
            <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
            <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
            <xsl:with-param name="afterHeader" select="'true'"/>
            <xsl:with-param name="GroupCell">
              <xsl:value-of select="$GroupCell"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>

        <!-- if further there is column range starts inside header, but ends outside write rest of the columns outside header -->
        <xsl:for-each
          select="following-sibling::e:col[@min &lt;= $headerColsEnd and @max &gt; $headerColsEnd]">
          <xsl:call-template name="InsertThisColumn">
            <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
            <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
            <xsl:with-param name="afterHeader" select="'true'"/>
            <xsl:with-param name="GroupCell">
              <xsl:value-of select="$GroupCell"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>

        <!-- if header is empty -->
        <xsl:if test="@min &gt; $headerColsEnd">

          <xsl:choose>
            <!-- first row after start of the header is right after end of the header -->
            <xsl:when test="@min = $headerColsEnd + 1">
              <xsl:call-template name="InsertThisColumn">
                <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
                <xsl:with-param name="GroupCell">
                  <xsl:value-of select="$GroupCell"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <!-- insert default columns between header and column -->
              <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
                <xsl:if test="@min &gt; $headerColsEnd + 2">
                  <xsl:attribute name="table:number-columns-repeated">
                    <xsl:value-of select="@min - $headerColsEnd - 1"/>
                  </xsl:attribute>
                </xsl:if>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>
              </table:table-column>

              <xsl:call-template name="InsertThisColumn">
                <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
                <xsl:with-param name="GroupCell">
                  <xsl:value-of select="$GroupCell"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:if>

      </xsl:when>
      <!-- if header is after the last column -->
      <xsl:when test="not(following-sibling::e:col) and @max &lt; $headerColsStart">

        <xsl:call-template name="InsertThisColumn">
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
        </xsl:call-template>

        <!-- insert default columns before header -->
        <xsl:if test="$headerColsStart &gt; @max + 1">
          <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
            <xsl:if test="$headerColsStart &gt; @max + 2">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="$headerColsStart - @max - 1"/>
              </xsl:attribute>
            </xsl:if>

            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:if>

        <table:table-header-columns>
          <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}">
            <xsl:if test="$headerColsEnd  - $headerColsStart &gt; 1">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="$headerColsEnd  - $headerColsStart + 1"/>
              </xsl:attribute>
            </xsl:if>

            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </table:table-header-columns>

      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="InsertThisColumn">
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
          <!--Start of RefNo-1-->
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <!--End of RefNo-1-->
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:choose>

      <!-- calc supports only 256 columns -->
      <xsl:when test="$number &gt; 255">
        <xsl:message terminate="no">translation.oox2odf.ColNumber</xsl:message>
      </xsl:when>

      <xsl:when test="$headerColsStart = '' ">
        <xsl:apply-templates select="following-sibling::e:col[1]">
          <xsl:with-param name="number" select="@max + 1"/>
          <xsl:with-param name="sheet" select="$sheet"/>
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
          <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>

      <!-- if next is the first header column -->
      <xsl:when
        test="@max &lt; $headerColsStart and following-sibling::e:col[1][@min &lt;= $headerColsEnd and @max &gt;= $headerColsStart]">
        <xsl:apply-templates select="following-sibling::e:col[1]">
          <xsl:with-param name="number" select="@max + 1"/>
          <xsl:with-param name="sheet" select="$sheet"/>
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
          <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>

      <!-- jump over the header -->
      <xsl:when
        test="following-sibling::e:col[@min &lt; $headerColsStart or @min &gt; $headerColsEnd]">
        <xsl:apply-templates
          select="following-sibling::e:col[@min &lt; $headerColsStart or @min &gt; $headerColsEnd][1]">
          <xsl:with-param name="number" select="@max + 1"/>
          <xsl:with-param name="sheet" select="$sheet"/>
          <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
          <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
          <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
          <xsl:with-param name="GroupCell">
            <xsl:value-of select="$GroupCell"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>


  <xsl:template name="InsertColBreak">
    <xsl:param name="GetMinManualColBreak"/>
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="sheet"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="prevManualBreak" select="0"/>
    <xsl:param name="GetFirstManualColBreakAfterColWithStyle"/>
    <xsl:param name="GetLastManualBreak"/>

    <xsl:choose>

      <!-- if manual colBreak ist  first after default cols -->
      <xsl:when test="@min &gt; $GetMinManualColBreak and not(preceding-sibling::e:col)">
        <!--RefNo-1-->
        <xsl:if test="$GetMinManualColBreak - $prevManualBreak &gt; 0">
          <table:table-column>
            <xsl:attribute name="table:style-name">
              <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
            </xsl:attribute>

            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>

            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:if>

        <!-- RefNo-1 -->
        <xsl:if test="(@min - 1) != ($GetMinManualColBreak)">
          <table:table-column
            table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:if>

        <xsl:variable name="GetTemplate_GetMinRowWithPicture">
          <xsl:call-template name="GetMinRowWithPicture">
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="AfterRow">
              <xsl:value-of select="$GetMinManualColBreak + 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetTemplate_GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>
          <xsl:with-param name="prevManualBreak">
            <xsl:value-of select="$GetMinManualColBreak + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>


      <!-- if manual colBreak ist  greater than @max-->
      <!--RefNo-1
      <xsl:when
        test="@min &lt; $GetMinManualColBreak and $GetMinManualColBreak &gt; @max and not(preceding-sibling::e:col)">

        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:for-each select="key('Part', concat('xl/',$sheet))">
              <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
            </xsl:for-each>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>
          </xsl:attribute>
        </table:table-column>

        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>

        <xsl:variable name="GetTemplate_GetMinRowWithPicture">
          <xsl:call-template name="GetMinRowWithPicture">
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="AfterRow">
              <xsl:value-of select="$GetMinManualColBreak + 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetTemplate_GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>
          <xsl:with-param name="prevManualBreak">
            <xsl:value-of select="$GetMinManualColBreak + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
-->
      <!-- manual colBreak between styled cols -->
      <xsl:when test="@min &gt; $GetMinManualColBreak and preceding-sibling::e:col">
        <!--Start of RefNo-1
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:for-each select="key('Part', concat('xl/',$sheet))">
              <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
            </xsl:for-each>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <xsl:when test="$prevManualBreak &gt; 0">
                <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:choose>
                  <xsl:when test="$GetMinManualColBreak - preceding-sibling::e:col[1]/@max &lt; 0">
                    <xsl:value-of select="preceding-sibling::e:col[1]/@max - $GetMinManualColBreak"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$GetMinManualColBreak - preceding-sibling::e:col[1]/@max"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </table:table-column>
-->
        <xsl:choose>
          <xsl:when test="$prevManualBreak &gt; 0">
            <xsl:if test="($GetMinManualColBreak - $prevManualBreak) &gt; 0">
              <table:table-column>
                <xsl:attribute name="table:style-name">
                  <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                </xsl:attribute>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>

                <xsl:attribute name="table:number-columns-repeated">

                  <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>

                </xsl:attribute>
              </table:table-column>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="($GetMinManualColBreak - preceding-sibling::e:col[1]/@max) &gt; 0">
              <table:table-column>
                <xsl:attribute name="table:style-name">
                  <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
                </xsl:attribute>

                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>

                <xsl:attribute name="table:number-columns-repeated">

                  <xsl:value-of select="$GetMinManualColBreak - preceding-sibling::e:col[1]/@max"/>

                </xsl:attribute>
              </table:table-column>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>

        <!--End of RefNo-1-->
        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>

        <xsl:variable name="GetTemplate_GetMinRowWithPicture">
          <xsl:call-template name="GetMinRowWithPicture">
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="AfterRow">
              <xsl:value-of select="$GetMinManualColBreak + 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetTemplate_GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>
          <xsl:with-param name="prevManualBreak">
            <xsl:value-of select="$GetMinManualColBreak + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <!--RefNo-1
      <xsl:when test="@min = $GetMinManualColBreak and preceding-sibling::e:col">
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:for-each select="key('Part', concat('xl/',$sheet))">
              <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
            </xsl:for-each>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <xsl:when test="$prevManualBreak &gt; 0">
                <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$GetMinManualColBreak - preceding-sibling::e:col[1]/@max"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </table:table-column>

        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>

        <xsl:variable name="GetTemplate_GetMinRowWithPicture">
          <xsl:call-template name="GetMinRowWithPicture">
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="AfterRow">
              <xsl:value-of select="$GetMinManualColBreak + 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetTemplate_GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>
          <xsl:with-param name="prevManualBreak">
            <xsl:value-of select="$GetMinManualColBreak + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
-->
      <!-- RefNo-1
      <xsl:when
        test="@min &lt; $GetMinManualColBreak and $GetMinManualColBreak &lt;= @max and preceding-sibling::e:col">
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:for-each select="key('Part', concat('xl/',$sheet))">
              <xsl:value-of select="generate-id(key('SheetFormatPr', ancestor-or-self::e:worksheet/@oox:part))"/>
            </xsl:for-each>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:choose>
              <xsl:when test="$prevManualBreak &gt; 0">
                <xsl:value-of select="$GetMinManualColBreak - $prevManualBreak"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$GetMinManualColBreak - preceding-sibling::e:col[1]/@max"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </table:table-column>

        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>

        <xsl:variable name="GetTemplate_GetMinRowWithPicture">
          <xsl:call-template name="GetMinRowWithPicture">
            <xsl:with-param name="PictureRow">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="AfterRow">
              <xsl:value-of select="$GetMinManualColBreak + 1"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>

        <xsl:call-template name="InsertColBreak">
          <xsl:with-param name="ManualColBreaks">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="GetMinManualColBreak">
            <xsl:value-of select="$GetTemplate_GetMinRowWithPicture"/>
          </xsl:with-param>
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="DefaultCellStyleName">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:with-param>
          <xsl:with-param name="prevManualBreak">
            <xsl:value-of select="$GetMinManualColBreak + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      -->
    </xsl:choose>
  </xsl:template>



  <xsl:template name="InsertColumns">
    <xsl:param name="sheet"/>
    <xsl:param name="GroupCell"/>
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="GetMinManualColBreak"/>

    <xsl:variable name="DefaultCellStyleName">
      <xsl:value-of select="generate-id(key('Xf', '0'))"/>
    </xsl:variable>

    <xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>

    <xsl:variable name="apos">
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <xsl:variable name="charHeaderColsStart">
      <xsl:choose>
        <!-- if sheet name in range definition is in apostrophes -->
        <xsl:when
          test="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($apos,$sheetName,$apos))]">
          <xsl:for-each
            select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($apos,$sheetName,$apos))]">
            <xsl:choose>
              <!-- when header columns are present -->
              <xsl:when test="contains(text(),',')">
                <xsl:value-of
                  select="substring-before(substring-after(substring-after(substring-before(text(),','),$apos),concat($apos,'!$')),':')"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of
                  select="substring-before(substring-after(substring-after(text(),$apos),concat($apos,'!$')),':')"
                />
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>

        <xsl:when
          test="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($sheetName,'!'))]">
          <xsl:for-each
            select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($sheetName,'!'))]">
            <xsl:choose>
              <!-- when header columns are present -->
              <xsl:when test="contains(text(),',')">
                <xsl:value-of
                  select="substring-before(substring-after(substring-before(text(),','),'$'),':')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-before(substring-after(text(),'$'),':')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="charHeaderColsEnd">
      <xsl:choose>
        <!-- if sheet name in range definition is in apostrophes -->
        <xsl:when
          test="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($apos,$sheetName,$apos))]">
          <xsl:for-each
            select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($apos,$sheetName,$apos))]">
            <xsl:choose>
              <!-- when header columns are present -->
              <xsl:when test="contains(text(),',')">
                <xsl:value-of
                  select="substring-after(substring-after(substring-before(text(),','),':'),'$')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-after(substring-after(text(),':'),'$')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>

        <xsl:when
          test="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($sheetName,'!'))]">
          <xsl:for-each
            select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[@name= '_xlnm.Print_Titles' and starts-with(text(),concat($sheetName,'!'))]">
            <xsl:choose>
              <!-- when header columns are present -->
              <xsl:when test="contains(text(),',')">
                <xsl:value-of
                  select="substring-after(substring-after(substring-before(text(),','),':'),'$')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="substring-after(substring-after(text(),':'),'$')"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:for-each>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="headerColsStart">
      <xsl:if test="$charHeaderColsStart != '' and not(number($charHeaderColsStart))">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="$charHeaderColsStart"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>

    <xsl:variable name="headerColsEnd">
      <xsl:if test="$charHeaderColsEnd != '' and not(number($charHeaderColsEnd))">
        <xsl:call-template name="GetAlphabeticPosition">
          <xsl:with-param name="literal" select="$charHeaderColsEnd"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet">

      <xsl:apply-templates select="e:cols/e:col[1]">
        <xsl:with-param name="number">1</xsl:with-param>
        <xsl:with-param name="sheet" select="$sheet"/>
        <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
        <xsl:with-param name="headerColsStart" select="$headerColsStart"/>
        <xsl:with-param name="headerColsEnd" select="$headerColsEnd"/>
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupCell"/>
        </xsl:with-param>
        <xsl:with-param name="ManualColBreaks">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
      </xsl:apply-templates>
      <xsl:variable name="LastBreakBeforeHeader">
        <!--last break between 0 and headerStart -->
        <xsl:call-template name="GetMaxValueBetweenTwoValues">
          <xsl:with-param name="min" select="0"/>
          <xsl:with-param name="max" select="$headerColsStart"/>
          <xsl:with-param name="value" select="$ManualColBreaks"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="LastBreakInsideHeader">
        <!-- last break between headerStart and headerEnd -->
        <xsl:call-template name="GetMaxValueBetweenTwoValues">
          <xsl:with-param name="min" select="$headerColsStart"/>
          <xsl:with-param name="max" select="$headerColsEnd - 1"/>
          <xsl:with-param name="value" select="$ManualColBreaks"/>
        </xsl:call-template>
      </xsl:variable>
      <!-- if all columns are default (there aren't any e:col tags) and there is a header -->
      <xsl:if test="not(e:cols/e:col) and $headerColsStart != '' ">

        <!-- insert columns before header -->
        <xsl:choose>
          <!-- if there is a break before header -->
          <xsl:when test="$ManualColBreaks != '' ">
            <!-- list of breakes before header-->
            <xsl:variable name="breakes">
              <xsl:value-of select="substring-before($ManualColBreaks,$LastBreakBeforeHeader)"/>
              <xsl:value-of select="$LastBreakBeforeHeader"/>
              <xsl:text>;</xsl:text>
            </xsl:variable>

            <xsl:if test="$LastBreakBeforeHeader !='' and $LastBreakBeforeHeader &gt;0">
              <xsl:call-template name="InsertFirstColBreaksWhenThereAreNotCols">
                <xsl:with-param name="ManualColBreaks" select="$breakes"/>
                <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
                <xsl:with-param name="sheet" select="$sheet"/>
              </xsl:call-template>
              <xsl:if test="$headerColsStart - $LastBreakBeforeHeader -1 &gt; 1">
                <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-columns-repeated="{$headerColsStart - $LastBreakBeforeHeader}">
                  <xsl:attribute name="table:default-cell-style-name">
                    <xsl:value-of select="$DefaultCellStyleName"/>
                  </xsl:attribute>
                </table:table-column>
              </xsl:if>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise>
            <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
              table:number-columns-repeated="{$headerColsStart - 1}">
              <xsl:attribute name="table:default-cell-style-name">
                <xsl:value-of select="$DefaultCellStyleName"/>
              </xsl:attribute>
            </table:table-column>
          </xsl:otherwise>
        </xsl:choose>


        <!-- insert header columns -->
        <table:table-header-columns>

          <xsl:choose>
            <!-- if there are breakes inside header -->
            <xsl:when test="$LastBreakInsideHeader !='' and $LastBreakInsideHeader &gt;0">
              <!-- list of breakes inside header-->
              <xsl:variable name="breakes">
                <xsl:choose>
                  <xsl:when test="$LastBreakBeforeHeader != 0">
                    <xsl:value-of
                      select="substring-before(substring-after($ManualColBreaks, concat($LastBreakBeforeHeader,';')), $LastBreakInsideHeader)"
                    />
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of
                      select="substring-before($ManualColBreaks, $LastBreakInsideHeader)"/>
                  </xsl:otherwise>
                </xsl:choose>
                <xsl:value-of select="$LastBreakInsideHeader"/>
                <xsl:text>;</xsl:text>
              </xsl:variable>

              <!--message will be displayed if there is more than one manual column breake inside header -->
              <xsl:if test="string-length($breakes) - string-length(translate($breakes,';','')) &gt; 1">
                <xsl:message terminate="no">translation.oox2odf.ManualColBreakeInsideHeader</xsl:message>
              </xsl:if>


              <xsl:call-template name="InsertFirstColBreaksWhenThereAreNotCols">
                <xsl:with-param name="ManualColBreaks" select="$breakes"/>
                <xsl:with-param name="colStart" select="$headerColsStart - 1"/>
                <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
                <xsl:with-param name="sheet" select="$sheet"/>
              </xsl:call-template>

              <xsl:if test="$headerColsEnd - $LastBreakInsideHeader &gt; 1">
                <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-columns-repeated="{$headerColsEnd - $LastBreakInsideHeader - 1}">
                  <xsl:attribute name="table:default-cell-style-name">
                    <xsl:value-of select="$DefaultCellStyleName"/>
                  </xsl:attribute>
                </table:table-column>
              </xsl:if>
            </xsl:when>
            <!-- if there are no breakes inside header -->
            <xsl:otherwise>
              <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                table:number-columns-repeated="{$headerColsEnd - $headerColsStart + 1}">
                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>
              </table:table-column>
            </xsl:otherwise>
          </xsl:choose>
        </table:table-header-columns>

        <!-- inserts manual breakes after header -->

        <!-- number of the first breake after header -->
        <xsl:variable name="FirstBreakeAfterHeader">
          <xsl:value-of
            select="substring-before(substring-after($ManualColBreaks, concat($LastBreakInsideHeader,';')), ';' )"
          />
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="$FirstBreakeAfterHeader !='' and $FirstBreakeAfterHeader &gt;0">
            <!-- list of breakes after header-->
            <xsl:variable name="BreakesAfterHeader">
              <xsl:choose>
                <xsl:when test="$LastBreakInsideHeader != 0">
                  <xsl:value-of
                    select="substring-after($ManualColBreaks, concat($LastBreakInsideHeader,';'))"/>
                </xsl:when>
                <xsl:when test="$LastBreakBeforeHeader != 0">
                  <xsl:value-of
                    select="substring-after($ManualColBreaks, concat($LastBreakBeforeHeader,';'))"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$ManualColBreaks"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:call-template name="InsertFirstColBreaksWhenThereAreNotCols">
              <xsl:with-param name="ManualColBreaks" select="$BreakesAfterHeader"/>
              <xsl:with-param name="colStart" select="$headerColsEnd"/>
              <xsl:with-param name="DefaultCellStyleName" select="$DefaultCellStyleName"/>
              <xsl:with-param name="sheet" select="$sheet"/>
            </xsl:call-template>
          </xsl:when>
        </xsl:choose>
      </xsl:if>

    </xsl:for-each>

    <!-- apply default column style for last columns which style wasn't changed -->
    <xsl:for-each select="key('Part', concat('xl/',$sheet))">

      <xsl:variable name="GetMinColAfterMaxColStyle">
        <xsl:value-of select="key('Col', e:worksheet/@oox:part)[last()]/@max"/>
      </xsl:variable>

      <xsl:variable name="GetFirstManualColBreakAfterColWithStyle">
        <xsl:call-template name="GetMinRowWithPicture">
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$ManualColBreaks"/>
          </xsl:with-param>
          <xsl:with-param name="AfterRow">
            <xsl:choose>
              <xsl:when test="$GetMinColAfterMaxColStyle !='' ">
                <xsl:value-of select="$GetMinColAfterMaxColStyle"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>1</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="GetLastManualBreak">
        <xsl:value-of select="e:worksheet/e:colBreaks/e:brk[last()]/@id"/>
      </xsl:variable>
      <!--Start of RefNo-1
      <xsl:if test="$GetFirstManualColBreakAfterColWithStyle &gt; $GetMinColAfterMaxColStyle">-->
      <xsl:if test="$GetFirstManualColBreakAfterColWithStyle &gt; $GetMinColAfterMaxColStyle - 1">
        <xsl:for-each select="e:worksheet/e:colBreaks/e:brk[@id &gt; $GetMinColAfterMaxColStyle - 1]">
          <xsl:choose>
            <xsl:when test="not(preceding-sibling::e:brk[@id &gt; $GetMinColAfterMaxColStyle - 1])">
              <xsl:if test="$GetFirstManualColBreakAfterColWithStyle - $GetMinColAfterMaxColStyle &gt; 0">
                <!--End of RefNo-1-->
                <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-columns-repeated="{$GetFirstManualColBreakAfterColWithStyle - $GetMinColAfterMaxColStyle}">
                  <xsl:attribute name="table:default-cell-style-name">
                    <xsl:value-of select="$DefaultCellStyleName"/>
                  </xsl:attribute>
                </table:table-column>
                <!--Start of RefNo-1-->
              </xsl:if>
            </xsl:when>
            <xsl:otherwise>
              <xsl:variable name="brkAt" select="@id"></xsl:variable>
              <xsl:variable name="brkLastAt" select="preceding-sibling::e:brk[1]/@id"></xsl:variable>
              <xsl:if test="$brkAt - $brkLastAt &gt; 1">
                <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-columns-repeated="{$brkAt - $brkLastAt - 1}">
                  <xsl:attribute name="table:default-cell-style-name">
                    <xsl:value-of select="$DefaultCellStyleName"/>
                  </xsl:attribute>
                </table:table-column>
              </xsl:if>
            </xsl:otherwise>
          </xsl:choose>
          <!--End of RefNo-1-->
          <table:table-column
            table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:for-each>

      </xsl:if>

      <xsl:choose>
        <xsl:when
          test="$headerColsStart != '' and not(key('Col', e:worksheet/@oox:part)[@max &gt; $headerColsEnd])">
          <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
            table:number-columns-repeated="{256 - $headerColsEnd}">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:when>


        <xsl:when test="not(key('Col', e:worksheet/@oox:part)) and $ManualColBreaks != ''">
          <xsl:call-template name="InsertFirstColBreaksWhenThereAreNotCols">
            <xsl:with-param name="ManualColBreaks">
              <xsl:value-of select="$ManualColBreaks"/>
            </xsl:with-param>
            <xsl:with-param name="sheet">
              <xsl:value-of select="$sheet"/>
            </xsl:with-param>
            <xsl:with-param name="DefaultCellStyleName">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>



        <xsl:when test="not(key('Col', e:worksheet/@oox:part))">
          <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
            table:number-columns-repeated="256">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="key('Col', e:worksheet/@oox:part)[last()]/@max &lt; 256">
            <table:table-column table:style-name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
              table:number-columns-repeated="{256 - key('Col', e:worksheet/@oox:part)[last()]/@max}">
              <xsl:attribute name="table:default-cell-style-name">
                <xsl:value-of select="$DefaultCellStyleName"/>
              </xsl:attribute>
            </table:table-column>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertThisColumn">
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="headerColsStart"/>
    <xsl:param name="headerColsEnd"/>
    <xsl:param name="beforeHeader" select="'false'"/>
    <xsl:param name="afterHeader" select="'false'"/>
    <xsl:param name="outlineLevel"/>
    <xsl:param name="GroupCell"/>
    <xsl:param name="ManualColBreaks"/>


    <!-- Insert Group Start -->
    <xsl:if test="contains(concat(';', $GroupCell), concat(';', @min, ':'))">
      <xsl:call-template name="InsertColumnGroupStart">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <!--
    <table:table-column table:style-name="{generate-id(.)}">

      <xsl:choose>-->
    <!-- when this is the rest of a column range after header -->
    <!--<xsl:when test="$afterHeader = 'true' ">
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="@max - $headerColsEnd"/>
          </xsl:attribute>
        </xsl:when>-->
    <!-- when this is the part of a column range before header -->
    <!--<xsl:when test="$beforeHeader = 'true'">
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="$headerColsStart - @min"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:when test="@min = $headerColsEnd"/>-->
    <!-- when this column range starts before header and ends after -->
    <!--<xsl:when
          test="$headerColsStart != '' and (@min &lt; $headerColsStart and @max &gt; $headerColsEnd)">
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="$headerColsEnd - $headerColsStart + 1"/>
          </xsl:attribute>
        </xsl:when>-->
    <!-- when this column range starts before header and ends inside -->
    <!--<xsl:when
          test="$headerColsStart != '' and @min &lt; $headerColsStart and @max &gt;= $headerColsStart">
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="@max - $headerColsStart + 1"/>
          </xsl:attribute>
        </xsl:when>-->
    <!-- when this column range starts inside header and ends outside -->
    <!--<xsl:when
          test="$headerColsEnd != '' and @min &lt; $headerColsEnd and @max &gt; $headerColsEnd">
          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="$headerColsEnd - @min + 1"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:if test="not(@min = @max)">
            <xsl:attribute name="table:number-columns-repeated">
              <xsl:choose>
                <xsl:when test="@max &gt; 256">
                  <xsl:value-of select="256 - @min + 1"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="@max - @min + 1"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </xsl:if>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:attribute name="table:default-cell-style-name">
        <xsl:value-of select="$DefaultCellStyleName"/>
      </xsl:attribute>
      <xsl:if test="@hidden=1">
        <xsl:attribute name="table:visibility">
          <xsl:text>collapse</xsl:text>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@style">
        <xsl:variable name="position">
          <xsl:value-of select="@style + 1"/>
        </xsl:variable>
        <xsl:attribute name="table:default-cell-style-name">
          <xsl:value-of select="generate-id(key('Xf', $position))"/>
        </xsl:attribute>
      </xsl:if>
    </table:table-column>
-->

    <!--Start of RefNo-1:Deleted code :code in template InsertTableCol-->
    <xsl:call-template name="InsertTableCol">
      <xsl:with-param name="afterHeader">
        <xsl:value-of select="$afterHeader"/>
      </xsl:with-param>
      <xsl:with-param name="headerColsEnd">
        <xsl:value-of select="$headerColsEnd"/>
      </xsl:with-param>
      <xsl:with-param name="beforeHeader">
        <xsl:value-of select="$beforeHeader"/>
      </xsl:with-param>
      <xsl:with-param name="headerColsStart">
        <xsl:value-of select="$headerColsStart"/>
      </xsl:with-param>
      <xsl:with-param name="DefaultCellStyleName">
        <xsl:value-of select="$DefaultCellStyleName"/>
      </xsl:with-param>
      <xsl:with-param name="ManualColBreaks">
        <xsl:value-of select="$ManualColBreaks"/>
      </xsl:with-param>
    </xsl:call-template>
    <!-- End of RefNo-1-->

    <!-- Insert Group End -->

    <xsl:if test="contains(concat(';', $GroupCell), concat(':', @max, ';'))">
      <xsl:call-template name="InsertColumnGroupEnd">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <!--Start of RefNo-1-->
  <xsl:template name="InsertTableCol">
    <xsl:param name="afterHeader"/>
    <xsl:param name="headerColsEnd"/>
    <xsl:param name="beforeHeader"/>
    <xsl:param name="headerColsStart"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="LastBreak" select="0"/>

    <xsl:variable name="firstVal" select="substring-before($ManualColBreaks,';')">

    </xsl:variable>

    <xsl:variable name="GetManualColBreakeInRange">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="@min - 1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="ManualCol">
      <xsl:call-template name="GetMaxValueBetweenTwoValues">
        <xsl:with-param name="min">
          <xsl:value-of select="@min - 1"/>
        </xsl:with-param>
        <xsl:with-param name="max">
          <xsl:value-of select="@max - 1"/>
        </xsl:with-param>
        <xsl:with-param name="value">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$GetManualColBreakeInRange = (@min - 1)">

        <table:table-column table:style-name="{concat(generate-id(.),'-break')}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>

      </xsl:when>
      <xsl:when test="$GetManualColBreakeInRange &gt; (@min - 1) and $GetManualColBreakeInRange &lt; (@max - 1)">

        <xsl:if test="($GetManualColBreakeInRange - $LastBreak) != 1">
          <table:table-column table:style-name="{generate-id(.)}">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>

            <xsl:attribute name="table:number-columns-repeated">
              <xsl:choose>
                <xsl:when test="$LastBreak &gt; 0">
                  <xsl:value-of select="$GetManualColBreakeInRange - $LastBreak"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="$GetManualColBreakeInRange - (@min - 1)"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </table:table-column>
        </xsl:if>
        <table:table-column table:style-name="{concat(generate-id(.),'-break')}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>
      </xsl:when>

      <xsl:when test="$GetManualColBreakeInRange = (@max - 1)">

        <xsl:choose>
          <xsl:when test="$LastBreak &gt; 0">
            <xsl:if test="$LastBreak != @max - 2">
              <table:table-column table:style-name="{generate-id(.)}">
                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>

                <xsl:if test="($GetManualColBreakeInRange - $LastBreak) != 1">
                  <xsl:attribute name="table:number-columns-repeated">

                    <xsl:value-of select="$GetManualColBreakeInRange - $LastBreak - 1"/>

                  </xsl:attribute>
                </xsl:if>
              </table:table-column>
            </xsl:if>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="@max != @min">
              <table:table-column table:style-name="{generate-id(.)}">
                <xsl:attribute name="table:default-cell-style-name">
                  <xsl:value-of select="$DefaultCellStyleName"/>
                </xsl:attribute>


                <xsl:attribute name="table:number-columns-repeated">

                  <xsl:value-of select="@max - @min"/>

                </xsl:attribute>

              </table:table-column>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>
        <table:table-column table:style-name="{concat(generate-id(.),'-break')}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>
      </xsl:when>

      <xsl:otherwise>

        <table:table-column table:style-name="{generate-id(.)}">
          <xsl:choose>
            <!-- when this is the rest of a column range after header -->
            <xsl:when test="$afterHeader = 'true' ">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="@max - $headerColsEnd"/>
              </xsl:attribute>
            </xsl:when>
            <!-- when this is the part of a column range before header -->
            <xsl:when test="$beforeHeader = 'true'">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="$headerColsStart - @min"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:when test="@min = $headerColsEnd"/>
            <!-- when this column range starts before header and ends after -->
            <xsl:when
						  test="$headerColsStart != '' and (@min &lt; $headerColsStart and @max &gt; $headerColsEnd)">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="$headerColsEnd - $headerColsStart + 1"/>
              </xsl:attribute>
            </xsl:when>
            <!-- when this column range starts before header and ends inside -->
            <xsl:when
						  test="$headerColsStart != '' and @min &lt; $headerColsStart and @max &gt;= $headerColsStart">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="@max - $headerColsStart + 1"/>
              </xsl:attribute>
            </xsl:when>
            <!-- when this column range starts inside header and ends outside -->
            <xsl:when
						  test="$headerColsEnd != '' and @min &lt; $headerColsEnd and @max &gt; $headerColsEnd">
              <xsl:attribute name="table:number-columns-repeated">
                <xsl:value-of select="$headerColsEnd - @min + 1"/>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:choose>
                <xsl:when test="$LastBreak = 0">
                  <xsl:if test="not(@min = @max)">
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:choose>
                        <xsl:when test="@max &gt; 256">
                          <xsl:value-of select="256 - @min + 1"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="@max - @min + 1"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:attribute>
                  </xsl:if>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:if test="not($LastBreak = (@max-1))">
                    <xsl:attribute name="table:number-columns-repeated">
                      <xsl:choose>
                        <xsl:when test="@max &gt; 256">
                          <xsl:value-of select="256 - $LastBreak - 1"/>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="@max - $LastBreak - 1"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:attribute>
                  </xsl:if>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:otherwise>
          </xsl:choose>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
          <xsl:if test="@hidden=1">
            <xsl:attribute name="table:visibility">
              <xsl:text>collapse</xsl:text>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="@style">
            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="generate-id(key('Xf', @style))"/>
            </xsl:attribute>
          </xsl:if>
        </table:table-column>
      </xsl:otherwise>

    </xsl:choose>

    <xsl:if test="$GetManualColBreakeInRange &gt;= (@min - 1) and $GetManualColBreakeInRange &lt; (@max - 1)">
      <xsl:call-template name="InsertTableCol">
        <xsl:with-param name="afterHeader">
          <xsl:value-of select="$afterHeader"/>
        </xsl:with-param>
        <xsl:with-param name="headerColsEnd">
          <xsl:value-of select="$headerColsEnd"/>
        </xsl:with-param>
        <xsl:with-param name="beforeHeader">
          <xsl:value-of select="$beforeHeader"/>
        </xsl:with-param>
        <xsl:with-param name="headerColsStart">
          <xsl:value-of select="$headerColsStart"/>
        </xsl:with-param>
        <xsl:with-param name="DefaultCellStyleName">
          <xsl:value-of select="$DefaultCellStyleName"/>
        </xsl:with-param>
        <xsl:with-param name="ManualColBreaks">
          <xsl:value-of select="substring-after($ManualColBreaks,concat($GetManualColBreakeInRange,';'))"/>
        </xsl:with-param>
        <xsl:with-param name ="LastBreak">
          <xsl:value-of select="$GetManualColBreakeInRange"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>
  <!--End of RefNo-1-->

  <xsl:template name="InsertFirstColBreaksWhenThereAreNotCols">
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="sheet"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="colStart" select="0"/>
    <xsl:param name="prevManualBreak" select="0"/>

    <xsl:variable name="GetMinManualColBreake">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="0"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$GetMinManualColBreake != ''">
        <xsl:if test="$GetMinManualColBreake - $colStart  &gt; 0">
          <table:table-column>
            <xsl:attribute name="table:style-name">
              <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
            </xsl:attribute>

            <xsl:attribute name="table:default-cell-style-name">
              <xsl:value-of select="$DefaultCellStyleName"/>
            </xsl:attribute>

            <xsl:attribute name="table:number-columns-repeated">
              <xsl:value-of select="$GetMinManualColBreake - $colStart"/>
            </xsl:attribute>
          </table:table-column>
        </xsl:if>

        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>
      </xsl:when>
    </xsl:choose>

    <xsl:if test="$ManualColBreaks !='' ">
      <xsl:call-template name="InsertNextColBreaksWhenThereAreNotCols">
        <xsl:with-param name="GetMinManualColBreake">
          <xsl:value-of select="$GetMinManualColBreake"/>
        </xsl:with-param>
        <xsl:with-param name="ManualColBreaks">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="DefaultCellStyleName">
          <xsl:value-of select="$DefaultCellStyleName"/>
        </xsl:with-param>
        <xsl:with-param name="prevManualBreak">
          <xsl:value-of select="$GetMinManualColBreake"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertNextColBreaksWhenThereAreNotCols">
    <xsl:param name="ManualColBreaks"/>
    <xsl:param name="sheet"/>
    <xsl:param name="DefaultCellStyleName"/>
    <xsl:param name="prevManualBreak"/>
    <xsl:param name="GetMinManualColBreake"/>


    <xsl:variable name="GetLastManualBreak">
      <xsl:value-of select="e:worksheet/e:colBreaks/e:brk[last()]/@id"/>
    </xsl:variable>

    <xsl:variable name="GetNextManualColBreake">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:value-of select="$GetMinManualColBreake + 1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>

      <xsl:when test="$GetNextManualColBreake - $GetMinManualColBreake - 1 = 0">
        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>
      </xsl:when>

      <xsl:when test="$GetNextManualColBreake - $GetMinManualColBreake &gt; 1">
        <table:table-column>
          <xsl:attribute name="table:style-name">
            <xsl:value-of select="concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)"/>
          </xsl:attribute>

          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>

          <xsl:attribute name="table:number-columns-repeated">
            <xsl:value-of select="$GetNextManualColBreake - $GetMinManualColBreake - 1"/>
          </xsl:attribute>
        </table:table-column>

        <table:table-column
          table:style-name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}">
          <xsl:attribute name="table:default-cell-style-name">
            <xsl:value-of select="$DefaultCellStyleName"/>
          </xsl:attribute>
        </table:table-column>
      </xsl:when>
    </xsl:choose>

    <xsl:if test="$GetNextManualColBreake - $GetMinManualColBreake &gt;= 1">
      <xsl:call-template name="InsertNextColBreaksWhenThereAreNotCols">
        <xsl:with-param name="ManualColBreaks">
          <xsl:value-of select="$ManualColBreaks"/>
        </xsl:with-param>
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="DefaultCellStyleName">
          <xsl:value-of select="$DefaultCellStyleName"/>
        </xsl:with-param>
        <xsl:with-param name="prevManualBreak">
          <xsl:value-of select="$GetNextManualColBreake + 1"/>
        </xsl:with-param>
        <xsl:with-param name="GetMinManualColBreake">
          <xsl:value-of select="$GetNextManualColBreake"/>
        </xsl:with-param>
        <xsl:with-param name="GetLastManualBreak">
          <xsl:value-of select="$GetLastManualBreak"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="GetMaxValueBetweenTwoValues">
    <xsl:param name="max"/>
    <xsl:param name="min"/>
    <xsl:param name="value"/>
    <xsl:param name="result" select="0"/>

    <xsl:variable name="FirstValue">
      <xsl:value-of select="substring-before($value, ';')"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$value = ''">
        <xsl:value-of select="$result"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="GetMaxValueBetweenTwoValues">
          <xsl:with-param name="min">
            <xsl:value-of select="$min"/>
          </xsl:with-param>
          <xsl:with-param name="max">
            <xsl:value-of select="$max"/>
          </xsl:with-param>
          <xsl:with-param name="value">
            <xsl:value-of select="substring-after($value, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="result">
            <xsl:choose>
              <xsl:when
                test="($FirstValue &gt; $result) and ($FirstValue &gt;= $min) and ($FirstValue &lt;= $max)">
                <xsl:value-of select="$FirstValue"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$result"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CutBreaks">
    <xsl:param name="breakes"/>
    <xsl:param name="max"/>
    <xsl:param name="headerStart"/>
    <xsl:param name="mode"/>

    <xsl:variable name="minCol">
      <xsl:choose>
        <!-- minCol for style:style for columns -->
        <xsl:when test="$mode = 'style' ">
          <xsl:value-of select="$max"/>
        </xsl:when>
        <!-- cut breaks before header start -->
        <xsl:when test="$headerStart">
          <xsl:value-of select="$headerStart"/>
        </xsl:when>
      </xsl:choose>

    </xsl:variable>

    <xsl:choose>
      <xsl:when test="substring-before($breakes,';') &lt;= $minCol">
        <xsl:call-template name="CutBreaks">
          <xsl:with-param name="breakes">
            <xsl:value-of select="substring-after($breakes,';')"/>
          </xsl:with-param>
          <xsl:with-param name="max" select="$max"/>
          <xsl:with-param name="mode" select="$mode"/>
          <xsl:with-param name="headerStart" select="$headerStart"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$breakes"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

</xsl:stylesheet>
