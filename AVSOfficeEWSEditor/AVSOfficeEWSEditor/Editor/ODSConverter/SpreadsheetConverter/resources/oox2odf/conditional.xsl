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
RefNo-1 5-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance                                              
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main" 
  xmlns:oox="urn:oox"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:x14="http://schemas.microsoft.com/office/spreadsheetml/2009/9/main"
  xmlns:xm="http://schemas.microsoft.com/office/excel/2006/main"
				 xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  exclude-result-prefixes="e oox r">

 <!-- <xsl:import href="common.xsl"/>
  <xsl:import href="relationships.xsl"/>
  <xsl:import href="border.xsl"/>
  <xsl:import href="styles.xsl"/>-->
  
  <xsl:key name="XfStyle" match="e:xf" use="@oox:id"/>

  <!-- Get Row with Conditional -->
  <xsl:template name="ConditionalRow">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="Result"/>
    <xsl:choose>
      <xsl:when test="$ConditionalCell != ''">
        <xsl:call-template name="ConditionalRow">
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="substring-after($ConditionalCell, ';')"/>
          </xsl:with-param>
          <xsl:with-param name="Result">
            <xsl:value-of
              select="concat($Result,  concat(substring-before($ConditionalCell, ':'), ';'))"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$Result"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

 

  <xsl:template match="e:conditionalFormatting">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="document"/>
	  <xsl:choose>
		  <!--Defect Id: 1803593, file 'ilas_EVE_Database_V0.02.xlsx'
		      Changes Done by: Vijayeta
			  Date: 10th Jan '08
			  Databar, colourscales and iconsets not supported in Open Office, hence do not execute this part of code-->
		  <xsl:when test ="./e:cfRule and not(./e:cfRule[@type= 'dataBar'] or ./e:cfRule[@type= 'colorScale'] or ./e:cfRule[@type= 'iconSet'])">
    <xsl:variable name="colNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:choose>
            <xsl:when test="contains(@sqref, ':')">
              <xsl:value-of select="substring-before(@sqref, ':')"/>
            </xsl:when>
            <xsl:when test="contains(@sqref, ' ')">
              <xsl:value-of select="substring-before(@sqref, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@sqref"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:choose>
            <xsl:when test="contains(@sqref, ':')">
              <xsl:value-of select="substring-before(@sqref, ':')"/>
            </xsl:when>
            <xsl:when test="contains(@sqref, ' ')">
              <xsl:value-of select="substring-before(@sqref, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@sqref"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="dxfIdStyle">
      <xsl:value-of select="count(preceding-sibling::e:conditionalFormatting)"/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="contains(@sqref, ':')">
        <xsl:choose>
          <xsl:when test="following-sibling::e:conditionalFormatting">
            <xsl:apply-templates select="following-sibling::e:conditionalFormatting[1]">
              <xsl:with-param name="ConditionalCell">
                <xsl:call-template name="InsertConditionalCell">
                  <xsl:with-param name="ConditionalCell">
                    <xsl:value-of select="$ConditionalCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="StartCell">
                    <xsl:value-of select="substring-before(@sqref, ':')"/>
                  </xsl:with-param>
                  <xsl:with-param name="EndCell">
                    <xsl:value-of select="substring-after(@sqref, ':')"/>
                  </xsl:with-param>
                  <xsl:with-param name="document">
                    <xsl:value-of select="$document"/>
                  </xsl:with-param>
                  <xsl:with-param name="dxfIdStyle">
                    <xsl:value-of select="$dxfIdStyle"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:with-param>
              <xsl:with-param name="document">
                <xsl:value-of select="$document"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="InsertConditionalCell">
              <xsl:with-param name="ConditionalCell">
                <xsl:value-of select="$ConditionalCell"/>
              </xsl:with-param>
              <xsl:with-param name="StartCell">
                <xsl:value-of select="substring-before(@sqref, ':')"/>
              </xsl:with-param>
              <xsl:with-param name="EndCell">
                <xsl:value-of select="substring-after(@sqref, ':')"/>
              </xsl:with-param>
              <xsl:with-param name="document">
                <xsl:value-of select="$document"/>
              </xsl:with-param>
              <xsl:with-param name="dxfIdStyle">
                <xsl:value-of select="$dxfIdStyle"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="following-sibling::e:conditionalFormatting">
            <xsl:apply-templates select="following-sibling::e:conditionalFormatting[1]">
              <xsl:with-param name="ConditionalCell">
                <xsl:choose>
                  <xsl:when test="$document='style'">
                    <xsl:value-of
                      select="concat($rowNum, ':', $colNum, ';', '-', $dxfIdStyle, ';', $ConditionalCell)"
                    />
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="concat($rowNum, ':', $colNum, ';', $ConditionalCell)"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
              <xsl:with-param name="document">
                <xsl:value-of select="$document"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <xsl:when test="$document='style'">
                <xsl:value-of
                  select="concat($rowNum, ':', $colNum, ';', '-', $dxfIdStyle, ';', $ConditionalCell)"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="concat($rowNum, ':', $colNum, ';', $ConditionalCell)"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
		  </xsl:when >

		  </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertConditionalCell">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="StartCell"/>
    <xsl:param name="EndCell"/>
    <xsl:param name="document"/>
    <xsl:param name="dxfIdStyle"/>

    <xsl:variable name="StartColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="StartRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$StartCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndColNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="EndRowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="$EndCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="RepeatRowConditional">
      <xsl:call-template name="RepeatRowConditional">
        <xsl:with-param name="StartColNum">
          <xsl:value-of select="$StartColNum"/>
        </xsl:with-param>
        <xsl:with-param name="EndColNum">
          <xsl:value-of select="$EndColNum"/>
        </xsl:with-param>
        <xsl:with-param name="StartRowNum">
          <xsl:value-of select="$StartRowNum"/>
        </xsl:with-param>
        <xsl:with-param name="EndRowNum">
          <xsl:value-of select="$EndRowNum"/>
        </xsl:with-param>
        <xsl:with-param name="document">
          <xsl:value-of select="$document"/>
        </xsl:with-param>
        <xsl:with-param name="dxfIdStyle">
          <xsl:value-of select="$dxfIdStyle"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:value-of select="concat($ConditionalCell, $RepeatRowConditional)"/>

  </xsl:template>

  <xsl:template name="RepeatRowConditional">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="StartColNum"/>
    <xsl:param name="StartRowNum"/>
    <xsl:param name="EndColNum"/>
    <xsl:param name="EndRowNum"/>
    <xsl:param name="document"/>
    <xsl:param name="dxfIdStyle"/>

    <xsl:choose>
      <xsl:when test="$StartRowNum &lt;= $EndRowNum">

        <xsl:call-template name="RepeatColConditional">
          <xsl:with-param name="StartColNum">
            <xsl:value-of select="$StartColNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndColNum">
            <xsl:value-of select="$EndColNum"/>
          </xsl:with-param>
          <xsl:with-param name="StartRowNum">
            <xsl:value-of select="$StartRowNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndRowNum">
            <xsl:value-of select="$EndRowNum"/>
          </xsl:with-param>
          <xsl:with-param name="document">
            <xsl:value-of select="$document"/>
          </xsl:with-param>
          <xsl:with-param name="dxfIdStyle">
            <xsl:value-of select="$dxfIdStyle"/>
          </xsl:with-param>
        </xsl:call-template>

        <xsl:call-template name="RepeatRowConditional">
          <xsl:with-param name="ConditionalCell">
            <xsl:choose>
              <xsl:when test="$document='style'">
                <xsl:value-of
                  select="concat($StartRowNum, ':', $StartColNum, ';', '-', $dxfIdStyle, ';',$ConditionalCell)"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of
                  select="concat($StartRowNum, ':', $StartColNum, ';', $ConditionalCell)"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="StartRowNum">
            <xsl:value-of select="$StartRowNum + 1"/>
          </xsl:with-param>
          <xsl:with-param name="StartColNum">
            <xsl:value-of select="$StartColNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndRowNum">
            <xsl:value-of select="$EndRowNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndColNum">
            <xsl:value-of select="$EndColNum"/>
          </xsl:with-param>
          <xsl:with-param name="document">
            <xsl:value-of select="$document"/>
          </xsl:with-param>
          <xsl:with-param name="dxfIdStyle">
            <xsl:value-of select="$dxfIdStyle"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="RepeatColConditional">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="StartColNum"/>
    <xsl:param name="StartRowNum"/>
    <xsl:param name="EndColNum"/>
    <xsl:param name="EndRowNum"/>
    <xsl:param name="document"/>
    <xsl:param name="dxfIdStyle"/>

    <xsl:choose>
      <xsl:when test="$StartColNum != $EndColNum">
        <xsl:call-template name="RepeatColConditional">
          <xsl:with-param name="ConditionalCell">
            <xsl:choose>
              <xsl:when test="$document='style'">
                <xsl:value-of
                  select="concat($StartRowNum, ':', $StartColNum, ';', '-', $dxfIdStyle, ';',$ConditionalCell)"
                />
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of
                  select="concat($StartRowNum, ':', $StartColNum, ';', $ConditionalCell)"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="StartRowNum">
            <xsl:value-of select="$StartRowNum "/>
          </xsl:with-param>
          <xsl:with-param name="StartColNum">
            <xsl:value-of select="$StartColNum + 1"/>
          </xsl:with-param>
          <xsl:with-param name="EndRowNum">
            <xsl:value-of select="$EndRowNum"/>
          </xsl:with-param>
          <xsl:with-param name="EndColNum">
            <xsl:value-of select="$EndColNum"/>
          </xsl:with-param>
          <xsl:with-param name="document">
            <xsl:value-of select="$document"/>
          </xsl:with-param>
          <xsl:with-param name="dxfIdStyle">
            <xsl:value-of select="$dxfIdStyle"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$document='style'">
            <xsl:value-of
              select="concat($StartRowNum, ':', $StartColNum, ';', '-', $dxfIdStyle, ';',$ConditionalCell)"
            />
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($StartRowNum, ':', $StartColNum, ';', $ConditionalCell)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="e:sheet" mode="ConditionalStyle">
    <xsl:param name="number"/>

    <xsl:variable name="Id">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!--Reduntant Code-->
    <!--<xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>-->
    <!--end-->

    <!-- Check If Conditionals are in this sheet -->

    <xsl:variable name="ConditionalCell">
      <!--xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ConditionalCell"/>
      </xsl:for-each-->
    </xsl:variable>

    <xsl:variable name="ConditionalCellStyle">
      <!--xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ConditionalCell">
          <xsl:with-param name="document">
            <xsl:text>style</xsl:text>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each-->
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/',$Id))">

      <xsl:apply-templates select="e:worksheet/e:conditionalFormatting" mode="ConditionalStyle">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
      </xsl:apply-templates>
		<!-- Defect: 2948277
		     Desc  :Cross Sheet Conditional Formatting for Office 2010
         -->
		<xsl:apply-templates select="e:worksheet/e:extLst/e:ext/x14:conditionalFormattings/x14:conditionalFormatting" mode="ConditionalStyle">
			<xsl:with-param name="sheet">
				<xsl:value-of select="$Id"/>
			</xsl:with-param>
		</xsl:apply-templates>
		<!--End of Fetaure Cond Formatting 2010-->
      <xsl:apply-templates select="e:worksheet/e:sheetData/e:row/e:c" mode="ConditionalAndCellStyle">
        <xsl:with-param name="ConditionalCell">
          <xsl:value-of select="$ConditionalCell"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellStyle">
          <xsl:value-of select="$ConditionalCellStyle"/>
        </xsl:with-param>
      </xsl:apply-templates>

    </xsl:for-each>

    <!-- Insert next Table -->

    <xsl:apply-templates select="following-sibling::e:sheet[1]" mode="ConditionalStyle">
      <xsl:with-param name="number">
        <xsl:value-of select="$number + 1"/>
      </xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="e:conditionalFormatting" mode="ConditionalStyle">
    <xsl:call-template name="InsertConditionalProperties"/>
  </xsl:template>
	<!-- Defect: 2948277
		     Desc  :Cross Sheet Conditional Formatting for Office 2010
         -->
	<xsl:template match="x14:conditionalFormatting" mode="ConditionalStyle">
		<xsl:call-template name="InsertConditionalProperties">
			<xsl:with-param name ="Office14" select ="'True'"/>
		</xsl:call-template>			
	</xsl:template>
	<!--End of Fetaure Cond Formatting 2010-->

  <xsl:template name="InsertConditionalProperties">
		<xsl:param name ="Office14"/>
    
    <style:style style:name="{generate-id(.)}" style:family="table-cell"
      style:parent-style-name="Default">
		<xsl:call-template name="InsertConditional">
			<xsl:with-param name ="Office14" select ="'True'"/>
		</xsl:call-template>
    </style:style>
  </xsl:template>

  <!-- Insert Conditional -->
  <xsl:template name="InsertConditional">
	  <xsl:param name ="Office14" select ="'False'"/>
    <!--RefNO-1:ODF1.1:to aviod text-properties if alerady inserted-->
    <xsl:param name="FrmCellFormat" select="'false'"/>

    <xsl:variable name="sqref">
		<xsl:choose>
			<xsl:when test ="$Office14='True'">
				<xsl:value-of select="xm:sqref"/>
			</xsl:when>
			<xsl:otherwise>
      <xsl:value-of select="@sqref"/>
			</xsl:otherwise>
		</xsl:choose>
      
    </xsl:variable>
    
    <xsl:variable name="s">
		<xsl:choose>
			<xsl:when test ="not(contains($sqref,':'))">
      <xsl:value-of select="parent::e:worksheet/e:sheetData/e:row/e:c[@r = $sqref]/@s"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="''"/>
			</xsl:otherwise>
		</xsl:choose>      	
    </xsl:variable>
    
    <!--RefNo-1:ODF1.1:Moved text-properties to follow map and added condition  and $FrmCellFormat = 'false' to chk if text-properties alerady inserted-->
    
    <xsl:if test="$s != '' and $FrmCellFormat = 'false'">
   
      <style:text-properties>
        <xsl:apply-templates select="key('Part', 'xl/styles.xml')/e:styleSheet/e:fonts/e:font[@oox:id = key('Xf', $s)/@fontId]"
          mode="style"/>
      </style:text-properties>
      
    </xsl:if>
    
    <xsl:for-each select="e:cfRule">
      <xsl:sort select="@priority"/>
      <!-- Formulas are not implemented, skip conditional with formulas-->
      <xsl:if test="not(@operator='containsText')">
      <style:map>
        <xsl:attribute name="style:apply-style-name">
          <xsl:variable name="PositionStyle">
            <xsl:value-of select="@dxfId"/>
          </xsl:variable>
          <xsl:choose>
            <!-- if there is a specified style for cells fullfilling certain condition -->
            <xsl:when test="@dxfId != ''">
              <xsl:value-of select="generate-id(key('Dxf', @dxfId))"/>
            </xsl:when>
            <!-- default style -->
            <xsl:otherwise>
              <xsl:text>Default</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
	    <!--<xsl:variable name ="sheetNameFormula1">
			  <xsl:value-of select ="substring-after(e:formula,'!')"/>
		  </xsl:variable>-->
		<xsl:variable name ="formula1">
			<xsl:value-of select ="e:formula"/>						 
		  </xsl:variable>		 
        <xsl:choose>
          <xsl:when test="@operator='equal'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()=</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='lessThanOrEqual'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()&lt;=</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='lessThan'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()&lt;</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='greaterThan'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()&gt;</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='greaterThanOrEqual'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()&gt;=</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='notEqual'">
            <xsl:attribute name="style:condition">
              <xsl:text>cell-content()!=</xsl:text>
              <xsl:value-of select="$formula1"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='between'">
			  <xsl:variable name ="sheetNameFormula2">
				  <xsl:value-of select ="substring-after(e:formula[2],'!')"/>
			  </xsl:variable>
			  <xsl:variable name ="formula2">
				  <xsl:value-of select ="e:formula[2]"/>							
			  </xsl:variable>			 
            <xsl:attribute name="style:condition">
              <xsl:value-of
                select="concat('cell-content-is-between(',$formula1, ',', $formula2, ')') "/>
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='notBetween'">
			  <xsl:variable name ="sheetNameFormula2">
				  <xsl:value-of select ="substring-after(e:formula[2],'!')"/>
			  </xsl:variable>
			  <xsl:variable name ="formula2">
			         <xsl:value-of select ="e:formula[2]"/>						
			  </xsl:variable>			  
            <xsl:attribute name="style:condition">
              <xsl:value-of
                select="concat('cell-content-is-not-between(', $formula1, ',', $formula2, ')') "
              />
            </xsl:attribute>
          </xsl:when>
          <xsl:when test="@operator='containsText'">
            <xsl:attribute name="style:condition">             
              <xsl:value-of select="concat('is-true-formula(', substring-before($formula1, '&quot;,'), '&quot;;', substring-after($formula1, '&quot;,'), ')')"
              />
            </xsl:attribute>
            <xsl:attribute name="style:base-cell-address">
              <xsl:value-of select="substring-before(substring-after($formula1, '&quot;,'), ')')"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="style:condition">
              <xsl:text>is-true-formula(FALSE)</xsl:text>
              <!--for cfRules not supported by converter;-->
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>
      </style:map>
      </xsl:if>
    </xsl:for-each>

	  <!-- Defect: 2948277
		     Desc  :Cross Sheet Conditional Formatting for Office 2010
         -->	 
	  <xsl:for-each select ="x14:cfRule">
		  <xsl:if test ="not(@type= 'dataBar' or @type= 'colorScale' or @type= 'iconSet')">			  
		  <xsl:variable name ="genId1">
				  <!--<xsl:value-of
				   select="concat(position(),'_',generate-id(key('ConditionalFormattingO14', ancestor::e:worksheet/@oox:part)))"
                  />-->
				  <xsl:value-of
					   select="concat(@priority,'_',generate-id(key('ConditionalFormattingO14', ancestor::e:worksheet/@oox:part)))"
                  />
		  </xsl:variable>
		  <!-- Formulas are not implemented, skip conditional with formulas-->
			  <xsl:if test="not(@operator='containsText')">
			  <style:map>
				  <xsl:attribute name="style:apply-style-name">
					  <xsl:variable name="PositionStyle">
						  <xsl:value-of select="@dxfId"/>
					  </xsl:variable>
					  <xsl:choose>
						  <!-- if there is a specified style for cells fullfilling certain condition -->
							  <xsl:when test="@id != ''">
							  <!--msxsl:node-set($animationVal)/p:par-->
							  <!--<xsl:value-of select="$genId1"/>-->
							  <xsl:value-of select="$genId1"/>
						  </xsl:when>
						  <!-- default style -->
						  <xsl:otherwise>
							  <xsl:text>Default</xsl:text>
						  </xsl:otherwise>
					  </xsl:choose>
				  </xsl:attribute>
				  <xsl:attribute name="style:base-cell-address">
						  <xsl:value-of select ="translate(translate(xm:f,'!','.'),'$','')"/>
				  </xsl:attribute>
			     <xsl:variable name ="formula1"> 
					<xsl:value-of select ="concat('[$',translate(xm:f,'!','.'),']')"/>						 
				  </xsl:variable>
				  <xsl:choose>
						  <xsl:when test="@operator='equal'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()=</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='lessThanOrEqual'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()&lt;=</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='lessThan'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()&lt;</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='greaterThan'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()&gt;</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='greaterThanOrEqual'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()&gt;=</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='notEqual'">
						  <xsl:attribute name="style:condition">
							  <xsl:text>cell-content()!=</xsl:text>
							  <xsl:value-of select="$formula1"/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='between'">
						  <xsl:variable name ="formula2">
								  <xsl:value-of select ="concat('[$',translate(xm:f[2],'!','.'),']')"/>
						  </xsl:variable>
						  <xsl:attribute name="style:condition">
							  <xsl:value-of
								select="concat('cell-content-is-between(', $formula1, ',', $formula2, ')') "/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='notBetween'">
						  <xsl:variable name ="formula2">
								  <xsl:value-of select ="concat('[$',translate(xm:f[2],'!','.'),']')"/>								  
						  </xsl:variable>
						  <xsl:attribute name="style:condition">
							  <xsl:value-of
								select="concat('cell-content-is-not-between(', $formula1,',', $formula2,')') "/>
						  </xsl:attribute>
					  </xsl:when>
						  <xsl:when test="@operator='containsText'">
						  <xsl:attribute name="style:condition">
							  <xsl:value-of select="concat('is-true-formula(', substring-before($formula1, '&quot;,'), '&quot;;', substring-after($formula1, '&quot;,'), ')')"/>
						  </xsl:attribute>
						  <xsl:attribute name="style:base-cell-address">
								  <xsl:value-of select="substring-before(substring-after(xm:f, '&quot;,'), ')')"/>
						  </xsl:attribute>
					  </xsl:when>
					  <xsl:otherwise>
						  <xsl:attribute name="style:condition">
							  <xsl:text>is-true-formula(FALSE)</xsl:text>
							  <!--for cfRules not supported by converter;-->
						  </xsl:attribute>
					  </xsl:otherwise>
				  </xsl:choose>
			  </style:map>
		  </xsl:if>
		</xsl:if>
	  </xsl:for-each>
	  <!-- End of feature Cond Formatting, cross sheet ref-->
	  
  </xsl:template>

  <!-- Insert Coditional Styles -->

  <xsl:template name="InsertConditionalStyles">
    <xsl:for-each select="key('Part', 'xl/styles.xml')">
      <xsl:apply-templates select="e:styleSheet/e:dxfs/e:dxf"/>
    </xsl:for-each>
		<xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet">
			<xsl:variable  name="sheet">
				<xsl:call-template name="GetTarget">
					<xsl:with-param name="id">
						<xsl:value-of select="@r:id"/>
					</xsl:with-param>
					<xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>				
			<xsl:apply-templates select="key('Part', concat('xl/',$sheet))//x14:cfRule/x14:dxf" mode="Insertoffice14Styles"/>			
			</xsl:for-each>
  </xsl:template>

  <xsl:template match="e:dxf">
    <style:style style:name="{generate-id(.)}" style:family="table-cell">
      <style:table-cell-properties>
        <xsl:variable name="this" select="."/>
        <xsl:apply-templates select="e:fill" mode="style"/>
        <xsl:call-template name="InsertBorder"/>
      </style:table-cell-properties>
      <!--RefNo-1:ODF1.1:Moved text-properties to follow table-cell-properties-->
      <style:text-properties>
        <xsl:apply-templates select="e:font[1]" mode="style"/>
      </style:text-properties>
    </style:style>
  </xsl:template>
      
	<!-- Defect: 2948277
		     Desc  :Cross Sheet Conditional Formatting for Office 2010
         -->
  <xsl:template match="x14:dxf" mode="Insertoffice14Styles">	
		<xsl:variable name ="genId1">
			<!--msxsl:node-set($animationVal)/p:par-->
			<!--<xsl:value-of select="generate-id(msxsl:node-set($genId))"/>-->
			<!--<xsl:value-of
				   select="concat(position(),'_',generate-id(key('ConditionalFormattingO14', ancestor::e:worksheet/@oox:part)))"
                  />-->
			<xsl:value-of
				   select="concat(./parent::node()/@priority,'_',generate-id(key('ConditionalFormattingO14', ancestor::e:worksheet/@oox:part)))"
                  />
			</xsl:variable>
			<style:style style:name="{$genId1}" style:family="table-cell">
      <style:table-cell-properties>
        <xsl:variable name="this" select="."/>
        <xsl:apply-templates select="e:fill" mode="style"/>
        <xsl:call-template name="InsertBorder"/>
      </style:table-cell-properties>
      <!--RefNo-1:ODF1.1:Moved text-properties to follow table-cell-properties-->
      <style:text-properties>
        <xsl:apply-templates select="e:font[1]" mode="style"/>
      </style:text-properties>
    </style:style>
  </xsl:template>
	<!-- End of feature Cond Formatting, cross sheet ref-->

  <!-- Insert Cell Style and Conditional Style -->
  <xsl:template match="e:c" mode="ConditionalAndCellStyle">
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>

    <xsl:variable name="colNum">
      <xsl:call-template name="GetColNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="@r"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="rowNum">
      <xsl:call-template name="GetRowNum">
        <xsl:with-param name="cell">
          <xsl:value-of select="@r"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if
      test="contains(concat(';', $ConditionalCellStyle), concat(';', $rowNum, ':', $colNum, ';')) and @s != ''">

      <xsl:variable name="CellStyleNumber">
        <xsl:value-of select="@s"/>
      </xsl:variable>

      <style:style style:family="table-cell">

        <xsl:attribute name="style:name">
          <xsl:variable name="CellStyleId">
            <xsl:for-each select="key('Part', 'xl/styles.xml')/e:styleSheet">
              <xsl:for-each select="key('Xf', $CellStyleNumber)">
                <xsl:value-of select="generate-id(.)"/>
              </xsl:for-each>
            </xsl:for-each>
          </xsl:variable>
          <xsl:variable name="ConditionalStyleId">
            <xsl:for-each
              select="key('ConditionalFormatting', ancestor::e:worksheet/@oox:part)[@oox:id = substring-before(substring-after(concat(';', $ConditionalCellStyle), concat(';', $rowNum, ':', $colNum, ';-')), ';')]">
              <xsl:value-of select="generate-id(.)"/>
            </xsl:for-each>
          </xsl:variable>
          <xsl:value-of select="concat($CellStyleId, $ConditionalStyleId)"/>
        </xsl:attribute>

        <xsl:for-each select="key('Xf', $CellStyleNumber)">
          <xsl:call-template name="InsertCellFormat"/>
        </xsl:for-each>

        <xsl:for-each
          select="key('ConditionalFormatting', ancestor::e:worksheet/@oox:part)[@oox:id = substring-before(substring-after(concat(';', $ConditionalCellStyle), concat(';', $rowNum, ':', $colNum, ';-')), ';')]">
          <xsl:call-template name="InsertConditional">
            <!--RefNO-1:ODF1.1:to aviod text-properties if alerady inserted-->
            <xsl:with-param name="FrmCellFormat" select="'true'"/>
          </xsl:call-template>
        </xsl:for-each>
      </style:style>

    </xsl:if>

  </xsl:template>
  
  
  <xsl:template name="ConditionalInheritance">
    <xsl:param name="id"/>
    
    <xsl:for-each select="key('ConditionalInheritance', $id)">
      <xsl:if test="@oox:ConditionalInheritance != ''">
        <xsl:call-template name="ConditionalInheritanceStyle">
          <xsl:with-param name="id">
            <xsl:value-of select="$id"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalInheritance">
            <xsl:value-of select="@oox:ConditionalInheritance"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:if>
    </xsl:for-each>
    
    <xsl:if test="key('ConditionalInheritance', $id+1)">
      <xsl:call-template name="ConditionalInheritance">
        <xsl:with-param name="id">
          <xsl:value-of select="$id+1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    
  </xsl:template>
  
  <xsl:template name="ConditionalInheritanceStyle">
    <xsl:param name="id"/>
    <xsl:param name="ConditionalInheritance"/>
    
    <xsl:if test="substring-before($ConditionalInheritance, '|') != ''">            
      <style:style style:family="table-cell"
      style:parent-style-name="Default">
        <xsl:attribute name="style:name">
          <xsl:value-of select="concat(generate-id(.), substring-before($ConditionalInheritance, '|'))"/>  
        </xsl:attribute>
        
        <xsl:for-each select="key('Xf', substring-before($ConditionalInheritance, '|'))">
          <xsl:call-template name="InsertCellFormat">
            <xsl:with-param name="CheckConditional">
              <xsl:text>true</xsl:text>
            </xsl:with-param>
          </xsl:call-template>
          </xsl:for-each>
      
        <xsl:call-template name="InsertConditional">
          <!--RefNO-1:ODF1.1:to aviod text-properties if alerady inserted-->
          <xsl:with-param name="FrmCellFormat" select="'true'"/>
        </xsl:call-template>      
      </style:style>
      
    </xsl:if>
    
    <xsl:if test="substring-after($ConditionalInheritance, '|') != ''">
      <xsl:call-template name="ConditionalInheritanceStyle">
        <xsl:with-param name="id">
          <xsl:value-of select="$id"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalInheritance">
          <xsl:value-of select="substring-after($ConditionalInheritance, '|')"/>
        </xsl:with-param>
      </xsl:call-template>
    
    </xsl:if>
    
  </xsl:template>

</xsl:stylesheet>
