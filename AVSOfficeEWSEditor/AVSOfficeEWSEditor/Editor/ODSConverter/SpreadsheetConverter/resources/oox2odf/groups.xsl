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
    xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" exclude-result-prefixes="e r v">


  <!-- Insert col groups -->

  <xsl:template match="e:col" mode="groupTag">
    <xsl:param name="level" select="0"/>


    <xsl:variable name="min">
      <xsl:value-of select="@min"/>
    </xsl:variable>
    <xsl:variable name="max">
      <xsl:value-of select="@max"/>
    </xsl:variable>
    <xsl:variable name="outlineLevel">
      <xsl:value-of select="@outlineLevel"/>
    </xsl:variable>

    <!-- when group starts -->
    <xsl:if
        test="@outlineLevel &gt; $level and not(preceding-sibling::e:col[1][@max = $min - 1 and @outlineLevel &gt; $level])">

      <xsl:variable name="groupEnd">
        <xsl:call-template name="FindGroupEnd">
          <xsl:with-param name="level" select="$level + 1"/>
        </xsl:call-template>
      </xsl:variable>

      <xsl:value-of select="concat(@min,':',$groupEnd,';')"/>

      <!-- search for subgroups-->
      <xsl:apply-templates select="." mode="groupTag">
        <xsl:with-param name="level" select="$level + 1"/>
      </xsl:apply-templates>
    </xsl:if>

    <xsl:choose>
      <!-- stop search for subgroups when group ends -->
      <xsl:when
          test="$level &gt; 0 and not(following-sibling::e:col[1][@outlineLevel &gt;= $level and @min = $max + 1])"/>
      <xsl:otherwise>
        <xsl:apply-templates select="following-sibling::e:col[1]" mode="groupTag">
          <xsl:with-param name="level" select="$level"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="FindGroupEnd">
    <xsl:param name="level"/>

    <xsl:variable name="max">
      <xsl:value-of select="@max"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when
          test="following-sibling::e:col[1][@min = $max +1 and @outlineLevel &gt;= $level]">
        <xsl:for-each select="following-sibling::e:col[1]">
          <xsl:call-template name="FindGroupEnd">
            <xsl:with-param name="level" select="$level"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="@max"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  <xsl:template name="InsertColumnGroupStart">
    <xsl:param name="GroupCell"/>

    <xsl:if test="$GroupCell != '' and contains(concat(';', $GroupCell), concat(';', @min, ':'))">
      <table:table-column-group-start/>
      <xsl:call-template name="InsertColumnGroupStart">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="substring-after(substring-after(concat(';', $GroupCell), concat(';', @min, ':')), ';')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertColumnGroupEnd">
    <xsl:param name="GroupCell"/>

    <xsl:if test="$GroupCell != '' and contains(concat(';', $GroupCell), concat(':', @max, ';')) and (preceding-sibling::e:col[1]/@max != @max or not(preceding-sibling::e:col[1]/@max))">
      <table:table-column-group-end/>
      <xsl:call-template name="InsertColumnGroupEnd">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="substring-after(concat(';', $GroupCell), concat(':', @min, ';'))"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <!-- Insert start row groups -->

  <xsl:template name="GroupRow">
    <xsl:param name="Id"/>
    <xsl:param name="Result"/>

    <xsl:if test="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData/e:row/@outlineLevel">
      <xsl:apply-templates select="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData/e:row[1]" mode="groupTag">
        <xsl:with-param name="Id">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
        <xsl:with-param name="Result">
          <xsl:value-of select="$Result"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <xsl:template match="e:row" mode="groupTag">
    <xsl:param name="Id"/>
    <xsl:param name="Result"/>
    <xsl:param name="outlineLevel"/>

    <xsl:variable name="StartGroupRow">
      <xsl:if test="(not(preceding-sibling::e:row[1]/@outlineLevel) or @r != preceding-sibling::e:row[1]/@r +1) or (@r = preceding-sibling::e:row[1]/@r +1 and @outlineLevel -  preceding-sibling::e:row[1]/@outlineLevel &gt; 0) and @outlineLevel != ''">
        <xsl:call-template name="RepeatRow">
          <xsl:with-param name="Value">
            <xsl:value-of select="@r"/>
          </xsl:with-param>
          <xsl:with-param name="Repeat">
            <xsl:choose>
              <xsl:when test="not(preceding-sibling::e:row[1]/@outlineLevel) or @r != preceding-sibling::e:row[1]/@r +1">
                <xsl:value-of select="@outlineLevel"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="@outlineLevel - preceding-sibling::e:row[1]/@outlineLevel"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::e:row[1]">
        <xsl:apply-templates select="following-sibling::e:row[1]" mode="groupTag">
          <xsl:with-param name="Id">
            <xsl:value-of select="$Id"/>
          </xsl:with-param>
          <xsl:with-param name="Result">
            <xsl:value-of select="concat($StartGroupRow, $Result)"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat($StartGroupRow, $Result)"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

  <!-- Insert end row groups -->

  <xsl:template name="GroupRowEnd">
    <xsl:param name="Id"/>
    <xsl:param name="Result"/>

    <xsl:if test="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData/e:row/@outlineLevel">
      <xsl:apply-templates select="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData/e:row[1]" mode="groupTagEnd">
        <xsl:with-param name="Id">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
        <xsl:with-param name="Result">
          <xsl:value-of select="$Result"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <xsl:template match="e:row" mode="groupTagEnd">
    <xsl:param name="Id"/>
    <xsl:param name="Result"/>
    <xsl:param name="outlineLevel"/>

    <xsl:variable name="EndGroupRow">
      <!--<xsl:if test="(not(preceding-sibling::e:row[1]/@outlineLevel) or @r != preceding-sibling::e:row[1]/@r +1) or (@r = preceding-sibling::e:row[1]/@r +1 and @outlineLevel -  preceding-sibling::e:row[1]/@outlineLevel &gt; 0) and @outlineLevel != ''">
                <xsl:call-template name="RepeatRow">
                    <xsl:with-param name="Value">
                        <xsl:value-of select="@r"/>
                    </xsl:with-param>
                    <xsl:with-param name="Repeat">
                        <xsl:choose>
                            <xsl:when test="not(preceding-sibling::e:row[1]/@outlineLevel) or @r != preceding-sibling::e:row[1]/@r +1">
                                <xsl:value-of select="@outlineLevel"/>
                            </xsl:when>                            
                            <xsl:otherwise>
                                <xsl:value-of select="@outlineLevel - preceding-sibling::e:row[1]/@outlineLevel"/>
                            </xsl:otherwise>
                        </xsl:choose>
                    </xsl:with-param>                    
                </xsl:call-template>
                </xsl:if>-->

      <xsl:call-template name="RepeatRow">
        <xsl:with-param name="Value">
          <xsl:choose>
            <xsl:when test="@outlineLevel and not(following-sibling::e:row)">
              <xsl:value-of select="@r"/>
            </xsl:when>
            <xsl:when test="@outlineLevel and not(following-sibling::e:row[2])">
              <xsl:value-of select="@r"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="preceding-sibling::e:row[1]/@r"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
        <xsl:with-param name="Repeat">
          <xsl:choose>
            <xsl:when test="preceding-sibling::e:row[1]/@outlineLevel &gt; @outlineLevel and @r = preceding-sibling::e:row[1]/@r + 1">
              <xsl:choose>
                <xsl:when test="not(following-sibling::e:row)">
                  <xsl:value-of select="@outlineLevel"/>
                </xsl:when>
                <xsl:otherwise>
              <xsl:value-of select="preceding-sibling::e:row[1]/@outlineLevel - @outlineLevel"/>
                </xsl:otherwise>
              </xsl:choose>
              <!--<xsl:value-of select="preceding-sibling::e:row[1]/@outlineLevel - @outlineLevel"/>-->
            </xsl:when>
            <xsl:when test="(not(@outlineLevel) and preceding-sibling::e:row[1]/@outlineLevel) or ( @r &gt; preceding-sibling::e:row[1]/@r + 1 and preceding-sibling::e:row[1]/@outlineLevel)">
              <xsl:value-of select="preceding-sibling::e:row[1]/@outlineLevel"/>
            </xsl:when>
            <xsl:when test="@outlineLevel and not(following-sibling::e:row)">
              <xsl:choose>                
                <xsl:when test="(preceding-sibling::e:row[1]/@outlinelevel) and (@outlinelevel &gt; preceding-sibling::e:row[1]/@outlinelevel) and (@r = preceding-sibling::e:row[1]/@r + 1)" >
                  <xsl:value-of select="@outlinelevel + 1"/>
                </xsl:when>
                <xsl:otherwise>
              <xsl:value-of select="@outlineLevel"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:when>
            <!--<xsl:value-of select="@outlineLevel"/>-->            
            <xsl:otherwise>
              <xsl:if test="@outlineLevel and not(following-sibling::e:row[2]) and following-sibling::e:row[1]/@outlineLevel">
                <xsl:value-of select="@outlineLevel - following-sibling::e:row[1]/@outlineLevel"/>
              </xsl:if>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>

    </xsl:variable>

    <xsl:choose>
      <xsl:when test="following-sibling::e:row[1]">
        <xsl:apply-templates select="following-sibling::e:row[1]" mode="groupTagEnd">
          <xsl:with-param name="Id">
            <xsl:value-of select="$Id"/>
          </xsl:with-param>
          <xsl:with-param name="Result">
            <xsl:value-of select="concat($EndGroupRow, $Result)"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="concat($EndGroupRow, $Result)"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>



  <xsl:template name="RepeatRow">
    <xsl:param name="Repeat"/>
    <xsl:param name="Value"/>
    <xsl:param name="Result"/>

    <xsl:choose>
      <xsl:when test="$Repeat &gt; 0">

        <xsl:call-template name="RepeatRow">
          <xsl:with-param name="Repeat">
            <xsl:value-of select="$Repeat - 1"/>
          </xsl:with-param>
          <xsl:with-param name="Value">
            <xsl:value-of select="$Value"/>
          </xsl:with-param>
          <xsl:with-param name="Result">
            <xsl:value-of select="concat($Value, ':', $Result)"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

  <xsl:template name="InsertRowGroupStart">
    <xsl:param name="GroupCell"/>

    <xsl:if test="contains(concat(':', $GroupCell), concat(':', @r, ':'))">
      <table:table-row-group-start/>
      <xsl:call-template name="InsertRowGroupStart">
        <xsl:with-param name="GroupCell">
          <!--RefNo-1:ODF1.1:Added ':' at the end-->
          <xsl:value-of select="concat(substring-after(concat(':', $GroupCell), concat(':', @r, ':')), substring-before(concat(':', $GroupCell), concat(':', @r, ':')),':')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertRowGroupEnd">
    <xsl:param name="GroupCell"/>

    <xsl:if test="contains(concat(':', $GroupCell), concat(':', @r, ':'))">
      <table:table-row-group-end/>
      <!--RefNo-1:ODF1.1:Changed the template name called.-->
      <!--<xsl:call-template name="InsertRowGroupStart">-->
      <xsl:call-template name="InsertRowGroupEnd">
        <xsl:with-param name="GroupCell">
          <!--RefNo-1:ODF1.1:Added ':' at the end-->
          <xsl:value-of select="concat(substring-after(concat(':', $GroupCell), concat(':', @r, ':')), substring-before(concat(':', $GroupCell), concat(':', @r, ':')),':')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>


</xsl:stylesheet>