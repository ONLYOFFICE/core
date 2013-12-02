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
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="a e oox r number">


  <xsl:template name="InsertBorder">
    <xsl:param name="Id"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="LastHorizontalCellStyle"/>
  
    <xsl:variable name="BorderId">
      <xsl:choose>
        <xsl:when test="@borderId != ''">
          <xsl:value-of select="@borderId"/>
        </xsl:when>
        <xsl:otherwise/>
      </xsl:choose>
    </xsl:variable>


    <xsl:choose>
      <xsl:when test="$BorderId != '' and $MergeCell != ''">
        <xsl:variable name="LastMergeCell">
          <xsl:value-of select="substring-after(substring-before($MergeCell, ';'), ':')"/>
        </xsl:variable>

        <xsl:variable name="NrStyleMergeEnd">
          <xsl:value-of select="key('Cell', $LastMergeCell)[ancestor::e:worksheet/@oox:part = key('Part', concat('xl/', $Id))/e:worksheet/@oox:part]/@s"/>
        </xsl:variable>

        <xsl:variable name="LastMergeCellBorderId">
          <xsl:value-of select="key('Xf', $NrStyleMergeEnd)/@borderId"/>
        </xsl:variable>

        <xsl:for-each select="key('Border', $BorderId)">
          <xsl:call-template name="InsertBorderProperties">
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="thisborderId">
              <xsl:value-of select="$BorderId"/>
            </xsl:with-param>
            <xsl:with-param name="LastMergeCellBorderId">
              <xsl:if test="$BorderId != $LastMergeCellBorderId">
                <xsl:value-of select="$LastMergeCellBorderId"/>
              </xsl:if>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <xsl:when test="$BorderId != '' and $LastHorizontalCellStyle != ''">
        <xsl:for-each select="key('Border', $BorderId)">
          <xsl:call-template name="InsertBorderProperties">
            <xsl:with-param name="thisborderId">
              <xsl:value-of select="$BorderId"/>
            </xsl:with-param>
            <xsl:with-param name="LastHorizontalCellStyle">
              <xsl:value-of select="$LastHorizontalCellStyle"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>

      </xsl:when>

      <xsl:when test="$BorderId != 0 and $BorderId != ''">
        <xsl:for-each select="key('Border', $BorderId)">
          <xsl:call-template name="InsertBorderProperties">
            <xsl:with-param name="thisborderId">
              <xsl:value-of select="$BorderId"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>

      <xsl:otherwise>
        <xsl:for-each select="e:border">
          <xsl:call-template name="InsertBorderProperties">
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="thisborderId">
              <xsl:value-of select="$BorderId"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="PreprocessMergedCellsForBorders">
    <xsl:text>,</xsl:text>
    <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet">
      <xsl:variable name="workingpath">
        <xsl:value-of select="concat('xl/worksheets/sheet', @sheetId,'.xml')"/>
      </xsl:variable>
      <xsl:for-each select="key('Part', concat('xl/worksheets/sheet', @sheetId,'.xml'))/e:worksheet/e:mergeCells/e:mergeCell">
        <xsl:variable name="range">
          <xsl:value-of select="@ref"/>
        </xsl:variable>
        <xsl:variable name="strBef">
          <xsl:value-of select="number(key('Part', $workingpath)/e:worksheet/e:sheetData/e:row/e:c[@r= substring-before($range,':')]/@s)"/>
        </xsl:variable>
        <xsl:variable name="strAft">
          <xsl:value-of select="number(key('Part', $workingpath)/e:worksheet/e:sheetData/e:row/e:c[@r= substring-after($range,':')]/@s)"/>
        </xsl:variable>
        <xsl:value-of select="string(number(key('Part', 'xl/styles.xml')/e:styleSheet/e:cellXfs/e:xf[$strBef]/@borderId))"/>
        <xsl:text>:</xsl:text>
        <xsl:value-of select="string(number(key('Part', 'xl/styles.xml')/e:styleSheet/e:cellXfs/e:xf[$strAft]/@borderId))"/>
        <xsl:text>,</xsl:text>
      </xsl:for-each>
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="InsertBottomAndRightBorderProperties">
    <xsl:param name="LastMergeCellBorderId"/>
    <xsl:param name="LastHorizontalCellStyle"/>


    <xsl:attribute name="fo:border-bottom">
      <xsl:variable name="BorderStyleBottom">
        <xsl:choose>
          <xsl:when test="e:bottom/@style and e:bottom/@style != 'none'">
            <xsl:call-template name="GetBorderStyle">
              <xsl:with-param name="style">
                <xsl:value-of select="e:bottom/@style"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="$LastMergeCellBorderId != ''">
            <xsl:for-each select="key('Border', $LastMergeCellBorderId)">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:value-of select="e:bottom/@style"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$LastHorizontalCellStyle != ''">
            <xsl:for-each select="key('Border', $LastHorizontalCellStyle)">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:value-of select="e:bottom/@style"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="GetBorderStyle">
              <xsl:with-param name="style">
                <xsl:value-of select="e:bottom/@style"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <xsl:variable name="BorderColorBottom">
        <xsl:choose>
          <xsl:when test="e:bottom/e:color and e:bottom/e:color != 'none'">
            <xsl:for-each select="e:bottom/e:color">
              <xsl:call-template name="InsertColor"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$LastMergeCellBorderId != ''">
            <xsl:for-each select="key('Border', $LastMergeCellBorderId)">
              <xsl:for-each select="e:bottom/e:color">
                <xsl:call-template name="InsertColor"/>
              </xsl:for-each>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$LastHorizontalCellStyle != ''">
            <xsl:for-each select="key('Border', $LastHorizontalCellStyle)">
              <xsl:call-template name="GetBorderStyle">
                <xsl:with-param name="style">
                  <xsl:value-of select="e:bottom/@style"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each select="e:bottom/e:color">
              <xsl:call-template name="InsertColor"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$BorderStyleBottom != 'none'">
          <xsl:value-of select="concat($BorderStyleBottom, concat(' ', $BorderColorBottom))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$BorderStyleBottom"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>

    <xsl:attribute name="fo:border-right">
      <xsl:variable name="BorderStyleRight">
        <xsl:call-template name="GetBorderStyle">
          <xsl:with-param name="style">
            <xsl:choose>
              <xsl:when test="e:right/@style and e:right/@style != 'none'">
                <xsl:value-of select="e:right/@style"/>
              </xsl:when>
              <xsl:when test="$LastMergeCellBorderId != ''">
                <xsl:for-each select="key('Border', $LastMergeCellBorderId)">
                  <xsl:value-of select="e:right/@style"/>
                </xsl:for-each>
              </xsl:when>
              <xsl:when test="$LastHorizontalCellStyle != ''">
                <xsl:variable name="LastHorizontalCellStyleBorderId">
                  <xsl:value-of select="key('Xf', $LastHorizontalCellStyle)/@borderId"/>
                </xsl:variable>
                <xsl:for-each select="key('Border', $LastHorizontalCellStyleBorderId)">
                  <xsl:value-of select="e:right/@style"/>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="e:right/@style"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="BorderColorRight">
        <xsl:choose>
          <xsl:when test="e:right/e:color and e:right/e:color != 'none'">
            <xsl:for-each select="e:right/e:color">
              <xsl:call-template name="InsertColor"/>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$LastMergeCellBorderId != ''">
            <xsl:for-each select="key('Border', $LastMergeCellBorderId)">
              <xsl:for-each select="e:right/e:color">
                <xsl:call-template name="InsertColor"/>
              </xsl:for-each>
            </xsl:for-each>
          </xsl:when>
          <xsl:when test="$LastHorizontalCellStyle != ''">
            <xsl:variable name="LastHorizontalCellStyleBorderId">
              <xsl:value-of select="key('Xf', $LastHorizontalCellStyle)/@borderId"/>
            </xsl:variable>
            <xsl:for-each select="key('Border', $LastHorizontalCellStyleBorderId)">
              <xsl:for-each select="e:right/e:color">
                <xsl:call-template name="InsertColor"/>
              </xsl:for-each>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each select="e:right/e:color">
              <xsl:call-template name="InsertColor"/>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$BorderStyleRight != 'none'">
          <xsl:value-of select="concat($BorderStyleRight, concat(' ', $BorderColorRight))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$BorderStyleRight"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template name="InsertBorderProperties">
    <xsl:param name="LastMergeCellBorderId"/>
    <xsl:param name="LastHorizontalCellStyle"/>


    <xsl:call-template name="InsertBottomAndRightBorderProperties">
      <xsl:with-param name="LastMergeCellBorderId">
        <xsl:value-of select="$LastMergeCellBorderId"/>
      </xsl:with-param>
      <xsl:with-param name="LastHorizontalCellStyle">
        <xsl:value-of select="$LastHorizontalCellStyle"/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:attribute name="fo:border-left">
      <xsl:variable name="BorderStyleLeft">
        <xsl:call-template name="GetBorderStyle">
          <xsl:with-param name="style">
            <xsl:choose>
              <xsl:when test ="e:start/@style and e:start/@style!='none'">
              <xsl:value-of select="e:start/@style"/>
              </xsl:when>
              <xsl:otherwise>
            <xsl:value-of select="e:left/@style"/>
              </xsl:otherwise> 
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="BorderColorLeft">
        <xsl:choose>
          <xsl:when test ="e:start/e:color and e:start/e:color!='none'">
            <xsl:for-each select="e:start/e:color">
              <xsl:call-template name="InsertColor"/>
            </xsl:for-each>
          </xsl:when>
            <xsl:otherwise>
        <xsl:for-each select="e:left/e:color">
          <xsl:call-template name="InsertColor"/>
        </xsl:for-each>
            </xsl:otherwise>
          </xsl:choose>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$BorderStyleLeft != 'none'">
          <xsl:value-of select="concat($BorderStyleLeft, concat(' ', $BorderColorLeft))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$BorderStyleLeft"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>


    <xsl:attribute name="fo:border-top">
      <xsl:variable name="BorderStyleTop">
        <xsl:call-template name="GetBorderStyle">
          <xsl:with-param name="style">
            <xsl:value-of select="e:top/@style"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>
      <xsl:variable name="BorderColorTop">
        <xsl:for-each select="e:top/e:color">
          <xsl:call-template name="InsertColor"/>
        </xsl:for-each>
      </xsl:variable>
      <xsl:choose>
        <xsl:when test="$BorderStyleTop != 'none'">
          <xsl:value-of select="concat($BorderStyleTop, concat(' ', $BorderColorTop))"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$BorderStyleTop"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>

    <xsl:if test="@diagonalUp = '1'">
      <xsl:attribute name="style:diagonal-bl-tr">
        <xsl:variable name="BorderStyleTop">
          <xsl:call-template name="GetBorderStyle">
            <xsl:with-param name="style">
              <xsl:value-of select="e:diagonal/@style"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="BorderColorTop">
          <xsl:for-each select="e:diagonal/e:color">
            <xsl:call-template name="InsertColor"/>
          </xsl:for-each>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$BorderStyleTop != 'none'">
            <xsl:value-of select="concat($BorderStyleTop, concat(' ', $BorderColorTop))"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$BorderStyleTop"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>

    <xsl:if test="@diagonalDown = '1'">
      <xsl:attribute name="style:diagonal-tl-br">
        <xsl:variable name="BorderStyleTop">
          <xsl:call-template name="GetBorderStyle">
            <xsl:with-param name="style">
              <xsl:value-of select="e:diagonal/@style"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:variable>
        <xsl:variable name="BorderColorTop">
          <xsl:for-each select="e:diagonal/e:color">
            <xsl:call-template name="InsertColor"/>
          </xsl:for-each>
        </xsl:variable>
        <xsl:choose>
          <xsl:when test="$BorderStyleTop != 'none'">
            <xsl:value-of select="concat($BorderStyleTop, concat(' ', $BorderColorTop))"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$BorderStyleTop"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>

  </xsl:template>

  <xsl:template name="GetBorderStyle">
    <xsl:param name="style"/>

    <xsl:choose>
      <xsl:when test="$style = ''">
        <xsl:text>none</xsl:text>
      </xsl:when>
      <xsl:when
        test="$style = 'thin' or $style = 'hair' or $style = 'dotted' and $style = 'dashed' and $style = 'dashDot' and $style = 'dashDotDot'">
        <xsl:text>0.002cm solid</xsl:text>
      </xsl:when>
      <xsl:when test="$style = 'double'">
        <xsl:text>0.039cm double</xsl:text>
      </xsl:when>
      <xsl:when test="$style = 'medium'">
        <xsl:text>0.088cm solid</xsl:text>
      </xsl:when>
      <xsl:when test="$style = 'thick'">
        <xsl:text>0.141cm solid</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>0.002cm solid</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


</xsl:stylesheet>
