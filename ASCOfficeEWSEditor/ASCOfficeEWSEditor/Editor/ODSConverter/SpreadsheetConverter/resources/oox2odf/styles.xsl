<?xml version="1.0" encoding="utf-8"?>
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
RefNo-1 22-Oct-2007 Sandeep S     1812102   Added condition to check for "alignment" child element.                                              
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-2 22-Oct-2007 Sandeep S     1810604   Changes for default page settings.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-3 26-Oct-2007 Sandeep S     1757322   Modification done to get the styled column with column break. Column 
                                            breaks have 0 based indexing, where as styled columns indexing starts with 1.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
RefNo-4 12-Nov-2007 Sandeep S     1790019   Modification done to get the default row style for each sheet correctly.
                                            (Bug:Row break:-Round trip Conversion not proper for xlsx sheet)
RefNo-5 12-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance   
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
				xmlns:x14="http://schemas.microsoft.com/office/spreadsheetml/2009/9/main"
  xmlns:v="urn:schemas-microsoft-com:vml"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:oox="urn:oox"
  exclude-result-prefixes="a e oox r number v">

  <!--<xsl:import href="relationships.xsl"/>
  <xsl:import href="border.xsl"/>
  <xsl:import href="headers.xsl"/>
  <xsl:import href="insert_cols.xsl"/>
  <xsl:import href="note.xsl"/>-->

  <xsl:key name="numFmtId" match="e:styleSheet/e:numFmts/e:numFmt" use="@numFmtId"/>
  <xsl:key name="Font" match="e:styleSheet/e:fonts/e:font" use="@oox:id"/>
  <xsl:key name="Fill" match="e:styleSheet/e:fills/e:fill" use="@oox:id"/>
  <xsl:key name="Border" match="e:styleSheet/e:borders/e:border" use="@oox:id"/>
  <xsl:key name="Xf" match="e:styleSheet/e:cellXfs/e:xf" use="@oox:id"/>
  <xsl:key name="Dxf" match="e:styleSheet/e:dxfs/e:dxf" use="@oox:id"/>
  <xsl:key name="CellStylesId" match="e:cellStyle" use="@xfId"/>
  <xsl:key name="Cell" match="e:c" use="@r"/>

  <xsl:template name="styles">

    <xsl:variable name="activeTab">
      <xsl:for-each select="key('Part', 'xl/workbook.xml')">
        <xsl:choose>
          <xsl:when test="e:workbook/e:bookViews/e:workbookView/@activeTab">
            <xsl:value-of select="e:workbook/e:bookViews/e:workbookView/@activeTab"/>
          </xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:variable>
	<!--RefNo-5:ODF1.1:Added office:version="1.1"-->
	 <office:document-styles office:version="1.1">
      <office:font-face-decls>
        <xsl:call-template name="InsertFonts"/>
        <xsl:call-template name="InsertHeaderFooterFonts">
          <xsl:with-param name="activeTab" select="$activeTab"/>
        </xsl:call-template>
      </office:font-face-decls>
      <office:styles>
        <xsl:call-template name="InsertDefaultTableCellStyle"/>
        <xsl:call-template name="InsertCellStyle"/>
        <!-- if there is at least one comment with border style defined -->
        <xsl:if test="key('Part', 'xl/drawings/vmlDrawing1.vml')/xml[1]/v:shape/v:stroke">
          <xsl:call-template name="InsertCommentsBorderStyles"/>
        </xsl:if>
        <!-- Insert Conditional Styles-->
        <xsl:call-template name="InsertConditionalStyles"/>
      </office:styles>
      <office:automatic-styles>
        <xsl:call-template name="InsertPageLayout">
          <xsl:with-param name="activeTab" select="$activeTab"/>
        </xsl:call-template>
        <xsl:call-template name="InsertHeaderFooterStyles">
          <xsl:with-param name="activeTab" select="$activeTab"/>
        </xsl:call-template>
      </office:automatic-styles>
      <office:master-styles>
        <style:master-page style:name="Default" style:page-layout-name="pm1">
          <xsl:call-template name="InsertHeaderFooter">
            <xsl:with-param name="activeTab" select="$activeTab"/>
          </xsl:call-template>
        </style:master-page>
      </office:master-styles>
    </office:document-styles>
  </xsl:template>

  <xsl:template name="InsertFonts">
	  <!-- Perofomance-->
	  <!--
	      Changes by: Vijayeta
	      Desc      : Fix for the defect 2633527, unknown chr appears in roundtrip
		              as the font style is enclosed in apos, which is removed in the fix		  
      -->
	  <xsl:variable name ="apos">
		  <xsl:text >&apos;</xsl:text>
	  </xsl:variable>
    <xsl:for-each select="key('Part', 'xl/styles.xml')">
		<xsl:for-each select ="e:styleSheet/e:fonts/e:font[e:name]">
			<!--<style:font-face style:name="{e:name/@val}" svg:font-family="{e:name/@val}"/>-->
			<style:font-face style:name="{translate(e:name/@val,$apos,'')}" svg:font-family="{translate(e:name/@val,$apos,'')}"/>
			<!--End of the fix for 2633527-->
    </xsl:for-each>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertDefaultTableCellStyle">
    <style:style style:name="Default" style:family="table-cell">
      <style:text-properties>
        <xsl:apply-templates select="key('Part', 'xl/styles.xml')/e:styleSheet/e:fonts/e:font[1]"
          mode="style"/>
      </style:text-properties>
    </style:style>
  </xsl:template>

  <!-- insert column styles from all sheets -->
	  <!-- Perofomance-->
  <xsl:template name="InsertColumnStyles">
	  <xsl:for-each select="key('Part', 'xl/workbook.xml')">
		  <xsl:for-each select="e:workbook/e:sheets/e:sheet">
      <xsl:call-template name="InsertSheetColumnStyles">
        <xsl:with-param name="sheet">
          <xsl:call-template name="GetTarget">
            <xsl:with-param name="id">
              <xsl:value-of select="@r:id"/>
            </xsl:with-param>
	    <xsl:with-param name="document" select="string('xl/workbook.xml')"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:for-each>
	  </xsl:for-each>
  </xsl:template>

  <!-- insert column styles from selected sheet -->
  <xsl:template name="InsertSheetColumnStyles">
    <xsl:param name="sheet"/>
    <xsl:variable name="ManualColBreaks">
      <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks/e:brk">
        <xsl:value-of select="concat(@id,';')"/>
      </xsl:for-each>
    </xsl:variable>
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
    <xsl:if test="key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks">
      <style:style
        style:name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)}"
        style:family="table-column">
        <style:table-column-properties fo:break-before="page">
			<xsl:attribute name="style:column-width">
				<xsl:choose>
					<xsl:when
					  test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultColWidth">
						<xsl:value-of select="concat('sonataColumnWidth:','True|',$defaultFontStyle,'|',$defaultFontSize,'|',key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultColWidth)"/>
					</xsl:when>
					<xsl:otherwise>
						<!-- Here To calculate the def col width formula used is 
						     width=Truncate([{Number of Characters} * {Maximum Digit Width} + {5 pixel padding}]/{Maximum Digit Width}*256)/256
							 Column Width =Truncate(((256 * {width} + Truncate(128/{Maximum Digit Width}))/256)*{Maximum Digit Width})-->
						<xsl:value-of select="concat('sonataColumnWidth:','False|',$defaultFontStyle,'|',$defaultFontSize)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:attribute>
		</style:table-column-properties>
	</style:style>
</xsl:if>
<style:style
  style:name="{concat('co', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
      style:family="table-column">
      <style:table-column-properties fo:break-before="auto">
		  <xsl:attribute name="style:column-width">
			  <xsl:choose>
				  <xsl:when
					test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultColWidth">
					  <xsl:value-of select="concat('sonataColumnWidth:','True|',$defaultFontStyle,'|',$defaultFontSize,'|',key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultColWidth)"/>
				  </xsl:when>
				  <xsl:otherwise>
					  <!-- Excel application default-->
					  <!-- Here To calculate the def col width formula used is 
				           width=Truncate([{Number of Characters} * {Maximum Digit Width} + {5 pixel padding}]/{Maximum Digit Width}*256)/256
                           Column Width =Truncate(((256 * {width} + Truncate(128/{Maximum Digit Width}))/256)*{Maximum Digit Width})-->
					  <xsl:value-of select="concat('sonataColumnWidth:','False|',$defaultFontStyle,'|',$defaultFontSize)"/>
				  </xsl:otherwise>
			  </xsl:choose>
		  </xsl:attribute>
	  </style:table-column-properties>
  </style:style>

  <xsl:apply-templates select="key('Part', concat('xl/',$sheet))/e:worksheet/e:cols/e:col[1]"
      mode="automaticstyles">
      <xsl:with-param name="manualBreakes" select="$ManualColBreaks"/>
    </xsl:apply-templates>
  </xsl:template>

  <xsl:template name="colBreakId">
    <xsl:variable name="sheet">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <xsl:attribute name="table:style-name">
      <xsl:value-of select="generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:colBreaks)"/>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="e:col" mode="automaticstyles">
    <xsl:param name="manualBreakes"/>

    <xsl:variable name="breakes">
      <xsl:choose>
        <xsl:when test="substring-before($manualBreakes,';') &lt;= @max">
          <xsl:call-template name="CutBreaks">
            <xsl:with-param name="breakes" select="$manualBreakes"/>
            <!--RefNo-3-->
            <xsl:with-param name="max" select="@max - 1"/>
            <xsl:with-param name="mode">
              <xsl:text>style</xsl:text>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$manualBreakes"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!-- insert column style with break -->
    <xsl:if test="substring-before($manualBreakes,';') &lt;= @max">
      <style:style style:name="{concat(generate-id(.),'-break')}" style:family="table-column">
        <style:table-column-properties fo:break-before="page">
          <xsl:if test="@width">
            <xsl:attribute name="style:column-width">
              <xsl:call-template name="ConvertFromCharacters">
                <xsl:with-param name="value" select="@width"/>
              </xsl:call-template>
            </xsl:attribute>
          </xsl:if>
        </style:table-column-properties>
      </style:style>
    </xsl:if>

    <!-- TO DO: do not create column style without break when this is e:col for single column and it has break -->
    <!--xsl:if test="not(@min != @max and substring-before($manualBreakes,';') &lt;= @max)"-->
    <style:style style:name="{generate-id(.)}" style:family="table-column">
      <style:table-column-properties>
        <xsl:if test="@width">
          <xsl:attribute name="style:column-width">
            <xsl:call-template name="ConvertFromCharacters">
              <xsl:with-param name="value" select="@width"/>
            </xsl:call-template>
          </xsl:attribute>
        </xsl:if>
      </style:table-column-properties>
    </style:style>
    <!--/xsl:if-->

    <xsl:if test="following-sibling::e:col[1]">
      <xsl:apply-templates select="following-sibling::e:col[1]" mode="automaticstyles">
        <xsl:with-param name="manualBreakes" select="$breakes"/>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>
	  <!-- Perofomance-->
  <!-- insert row styles from all sheets -->
  <xsl:template name="InsertRowStyles">
		<xsl:for-each select="key('Part', 'xl/workbook.xml')">
			<xsl:for-each select="e:workbook/e:sheets/e:sheet">
      <xsl:call-template name="InsertSheetRowStyles">
        <xsl:with-param name="sheet">
          <xsl:call-template name="GetTarget">
            <xsl:with-param name="id">
              <xsl:value-of select="@r:id"/>
            </xsl:with-param>
            <xsl:with-param name="document" select="string('xl/workbook.xml')"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:for-each>
		</xsl:for-each>
  </xsl:template>

  <!-- insert row styles from selected sheet -->
  <xsl:template name="InsertSheetRowStyles">
    <xsl:param name="sheet"/>

    <!-- default & row Break style -->

    <!--Start of RefNo-4: Commeted the code to get the correct default row style for the sheet with row breaks.
    <xsl:choose>
      <xsl:when test="key('Part', concat('xl/',$sheet))/e:worksheet/e:rowBreaks">-->
    <!--To fix internal defect :3207: Added condition to get the row style for the sheet with row breaks-->
    <xsl:if test="key('Part', concat('xl/',$sheet))/e:worksheet/e:rowBreaks">
        <style:style
          style:name="{generate-id(key('Part', concat('xl/',$sheet))/e:worksheet/e:rowBreaks)}"
          style:family="table-row">
          <style:table-row-properties fo:break-before="page">
            <xsl:attribute name="style:row-height">
              <xsl:choose>
                <xsl:when
                  test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultRowHeight">
                  <xsl:call-template name="ConvertToCentimeters">
                    <xsl:with-param name="length">
                      <xsl:value-of
                        select="concat(key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultRowHeight,'pt')"
                      />
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
    <!-- Excel application default-->
                  <xsl:call-template name="ConvertToCentimeters">
                    <xsl:with-param name="length" select="'20px'"/>
                  </xsl:call-template>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:attribute>
          </style:table-row-properties>
        </style:style>
    </xsl:if>
      <!--</xsl:when>
      <xsl:otherwise>-->
      <!--End of RefNo-4-->
    <style:style
      style:name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
      style:family="table-row">
      <style:table-row-properties fo:break-before="auto">
        <xsl:attribute name="style:row-height">
          <xsl:choose>
            <xsl:when
              test="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultRowHeight">
              <xsl:call-template name="ConvertToCentimeters">
                <xsl:with-param name="length">
                  <xsl:value-of
                    select="concat(key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetFormatPr/@defaultRowHeight,'pt')"
                      />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:when>
            <xsl:otherwise>
              <!-- Excel application default-->
              <xsl:call-template name="ConvertToCentimeters">
                <xsl:with-param name="length" select="'20px'"/>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </style:table-row-properties>
    </style:style>
    <!--RefNo-4
      </xsl:otherwise>
    </xsl:choose>-->

    <xsl:apply-templates select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetData"
      mode="automaticstyles"/>
  </xsl:template>

  <xsl:template match="e:row" mode="automaticstyles">
    <xsl:if test="@ht">
      <xsl:variable name ="currentRow">
        <xsl:value-of  select ="@r"/>
      </xsl:variable>
      <xsl:variable name ="rowBrk">
        <xsl:choose >
          <xsl:when test ="parent::node()/parent::node()/e:rowBreaks">
            <xsl:for-each select ="parent::node()/parent::node()/e:rowBreaks/e:brk">
              <xsl:if test ="@id=$currentRow - 1">
                <xsl:value-of select ="'page'"/>
              </xsl:if>
            </xsl:for-each>
          </xsl:when>
        </xsl:choose>
      </xsl:variable>
      <style:style style:name="{generate-id(.)}" style:family="table-row">
        <style:table-row-properties>
          <xsl:variable name ="pageBreak">
            <xsl:choose >
              <xsl:when test ="contains($rowBrk,'page')">
                <xsl:value-of select ="'page'"/>
              </xsl:when>
              <xsl:otherwise >
                <xsl:value-of select ="'auto'"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:variable>
          <xsl:attribute name ="fo:break-before">
            <xsl:value-of select ="$pageBreak"/>
          </xsl:attribute>
          <xsl:attribute name="style:row-height">
            <xsl:call-template name="ConvertToCentimeters">
              <xsl:with-param name="length" select="concat(@ht,'pt')"/>
            </xsl:call-template>
          </xsl:attribute>
          <xsl:attribute name="style:use-optimal-row-height">
            <xsl:choose> 
            <!--XSLT Best Practice-->                        
              <xsl:when test="@customHeight or @thickTop or @thickBot">
                <xsl:text>false</xsl:text>
              </xsl:when>              
              <xsl:otherwise>
                <xsl:text>true</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </style:table-row-properties>
      </style:style>
    </xsl:if>
  </xsl:template>

  <!--  Insert Table Properties -->
	  <!-- Perofomance-->
  <xsl:template name="InsertStyleTableProperties">
		<xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook">
			<xsl:for-each select="e:sheets/e:sheet">
      <style:style>
        <xsl:attribute name="style:name">
          <xsl:value-of select="generate-id()"/>
        </xsl:attribute>
        <xsl:attribute name="style:family">
          <xsl:text>table</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="style:master-page-name">
          <xsl:text>Default</xsl:text>
        </xsl:attribute>
        <style:table-properties>
          <xsl:if test="@state='hidden'">
            <xsl:attribute name="table:display">
              <xsl:text>false</xsl:text>
            </xsl:attribute>
          </xsl:if>

          <xsl:variable name="sheet">
            <xsl:call-template name="GetTarget">
              <xsl:with-param name="id">
                <xsl:value-of select="@r:id"/>
              </xsl:with-param>
              <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet/e:sheetViews/e:sheetView">
            <xsl:if test="@rightToLeft = 1">
              <xsl:attribute name="style:writing-mode">
                <xsl:value-of select="'rl-tb'"/>
              </xsl:attribute>
              <xsl:attribute name="sheet">
                <xsl:value-of select="$sheet"/>
              </xsl:attribute>
            </xsl:if>
          </xsl:for-each>

        </style:table-properties>
      </style:style>
    </xsl:for-each>
		</xsl:for-each>
  </xsl:template>

  <!-- insert number styles-->

  <xsl:template name="InsertNumberStyles">

    <xsl:apply-templates select="key('Part', 'xl/styles.xml')/e:styleSheet/e:cellXfs"
      mode="fixedNumFormat"/>
    <xsl:apply-templates select="key('Part', 'xl/styles.xml')/e:styleSheet/e:numFmts"
      mode="automaticstyles"/>
  </xsl:template>

  <xsl:template name="InsertCellStyles">
	    <!-- Perofomance-->
	  <xsl:for-each select="key('Part', 'xl/styles.xml')/e:styleSheet">
		  <xsl:for-each select="e:cellXfs/e:xf">
      <style:style style:name="{generate-id(.)}" style:family="table-cell">
        <xsl:call-template name="InsertCellFormat"/>
      </style:style>
    </xsl:for-each>
	  </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertMergeCellStyles">

    <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]" mode="MergeStyle">
      <xsl:with-param name="number">1</xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="e:sheet" mode="MergeStyle">
    <xsl:param name="number"/>

    <xsl:variable name="Id">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>

    <!-- get a ';'-separated list of all merged cells for sheet $Id -->
	   <!-- Perofomance-->
    <xsl:variable name="MergeCell">
		  <xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet">
			  <xsl:for-each select="e:mergeCells">
        <xsl:apply-templates select="e:mergeCell[1]" mode="merge"/>
      </xsl:for-each>
		  </xsl:for-each>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData">
      <xsl:call-template name="InsertMergeCellStyleProperties">
        <xsl:with-param name="Id">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:for-each>

    <!-- Insert next Table -->

    <xsl:apply-templates select="following-sibling::e:sheet[1]" mode="MergeStyle">
      <xsl:with-param name="number">
        <xsl:value-of select="$number + 1"/>
      </xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template name="InsertMergeCellStyleProperties">
    <xsl:param name="MergeCell"/>
    <xsl:param name="Id"/>

    <xsl:if test="$MergeCell != ''">

      <xsl:variable name="NrStyleMergeStart">
        <xsl:value-of select="key('Cell', substring-before($MergeCell, ':'))[ancestor::e:worksheet/@oox:part = key('Part', concat('xl/', $Id))/e:worksheet/@oox:part]/@s"/>
      </xsl:variable>

      <!--xsl:comment>Merged Cell Styles
        Node = <xsl:value-of select="name()"/>
        @MergeCell = <xsl:value-of select="$MergeCell"/>
        @Id = <xsl:value-of select="$Id"/>
        $NrStyleMergeStart = <xsl:value-of select="$NrStyleMergeStart"/>
      </xsl:comment-->

      <xsl:for-each select="key('Xf', $NrStyleMergeStart)">
        <style:style style:name="{concat(generate-id(.), generate-id(.))}" style:family="table-cell">
          <xsl:call-template name="InsertCellFormat">
            <xsl:with-param name="Id">
              <xsl:value-of select="$Id"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
          </xsl:call-template>
        </style:style>
      </xsl:for-each>

      <xsl:call-template name="InsertMergeCellStyleProperties">
        <xsl:with-param name="Id">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="substring-after($MergeCell, ';')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>


  </xsl:template>

  <xsl:template name="InsertHorizontalCellStyles">

    <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]"
      mode="HorizontalStyle">
      <xsl:with-param name="number">1</xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="e:sheet" mode="HorizontalStyle">
    <xsl:param name="number"/>

    <xsl:variable name="Id">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>

    <xsl:apply-templates select="key('Part', concat('xl/',$Id))/e:worksheet/e:sheetData/e:row[1]" mode="HorizontalStyle"/>

    <!-- Insert next Table -->

    <xsl:apply-templates select="following-sibling::e:sheet[1]" mode="HorizontalStyle">
      <xsl:with-param name="number">
        <xsl:value-of select="$number + 1"/>
      </xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template name="ConvertHorizontalStyleForRow" match="e:row" mode="HorizontalStyle">

    <xsl:apply-templates select="e:c[1]" mode="HorizontalStyle"/>

    <xsl:if test="following-sibling::e:row[1]">
      <xsl:for-each select="following-sibling::e:row[1]">
        <xsl:call-template name="ConvertHorizontalStyleForRow"/>
      </xsl:for-each>
      <!--xsl:apply-templates select="following-sibling::e:row[1]" mode="HorizontalStyle"/-->
    </xsl:if>

  </xsl:template>

  <xsl:template match="e:c" mode="HorizontalStyle">

    <xsl:if test="e:v">

      <xsl:variable name="horizontal">
        <xsl:value-of select="key('Xf', @s)/e:alignment/@horizontal" />
      </xsl:variable>

      <xsl:variable name="continuous">
        <xsl:call-template name="CountContinuous"/>
      </xsl:variable>

      <xsl:if test="$horizontal = 'centerContinuous'">


        <xsl:call-template name="InsertHorizontalCellStylesProperties">
          <xsl:with-param name="FirstHorizontalCellStyle">
            <xsl:value-of select="@s"/>
          </xsl:with-param>
          <xsl:with-param name="LastHorizontalCellStyle">
            <xsl:value-of select="following-sibling::e:c[position() = $continuous - 1]/@s"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:if>

      <xsl:if test="following-sibling::e:c[1]">
        <xsl:apply-templates select="following-sibling::e:c[1]" mode="HorizontalStyle"/>
      </xsl:if>

    </xsl:if>

    <xsl:if test="not(e:v)">

      <xsl:if test="following-sibling::e:c[1]">
        <xsl:apply-templates select="following-sibling::e:c[1]" mode="HorizontalStyle"/>
      </xsl:if>

    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertHorizontalCellStylesProperties">
    <xsl:param name="FirstHorizontalCellStyle"/>
    <xsl:param name="LastHorizontalCellStyle"/>


    <xsl:for-each select="key('Xf', $FirstHorizontalCellStyle)">
      <style:style style:name="{concat(generate-id(.), generate-id(.))}" style:family="table-cell">
        <xsl:call-template name="InsertCellFormat">
          <xsl:with-param name="LastHorizontalCellStyle">
            <xsl:value-of select="$LastHorizontalCellStyle"/>
          </xsl:with-param>
        </xsl:call-template>
      </style:style>
    </xsl:for-each>

  </xsl:template>

  <!-- cell formats -->

  <xsl:template name="InsertCellFormat">
    <xsl:param name="Id"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="LastHorizontalCellStyle"/>

    <xsl:choose>

      <!-- existing number format -->
      <xsl:when test="key('numFmtId',@numFmtId)">
        <xsl:attribute name="style:data-style-name">
          <xsl:value-of select="generate-id(key('numFmtId',@numFmtId))"/>
        </xsl:attribute>
      </xsl:when>

      <!-- fixed number format -->
      <xsl:when test="@numFmtId &gt; 0">
        <xsl:attribute name="style:data-style-name">
          <xsl:value-of select="concat('N',@numFmtId)"/>
        </xsl:attribute>
      </xsl:when>

    </xsl:choose>

    <xsl:attribute name="style:parent-style-name">
      <xsl:text>Default</xsl:text>
    </xsl:attribute>

    <!--RefNo-1
    <xsl:if
      test="@applyAlignment = 1 or @applyBorder = 1 or (@applyProtection=1) or  @borderId != '0' or @fillId!='0' or @applyFill= 1">-->
    <!--Code added by Sandeep : BugNo.1812102: 22-oct-2007 :Added condition to check for "alignment" child element-->
    <xsl:if
		test="@applyAlignment = 1 or @applyBorder = 1 or (@applyProtection=1) or  @borderId != '0' or @fillId!='0' or @applyFill= 1 or e:alignment">

      <style:table-cell-properties>
        <!--RefNo-1
        <xsl:if test="@applyAlignment = 1">-->
        <xsl:if test="@applyAlignment = 1 or e:alignment">
          <!-- vertical-align -->
          <xsl:attribute name="style:vertical-align">
            <xsl:choose>
              <xsl:when test="e:alignment/@vertical = 'center' ">
                <xsl:text>middle</xsl:text>
              </xsl:when>
              <xsl:when test="not(e:alignment/@vertical)">
                <xsl:text>bottom</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="e:alignment/@vertical"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="e:alignment/@horizontal = 'fill' ">
          <xsl:attribute name="style:repeat-content">
            <xsl:text>true</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if
          test="e:alignment/@horizontal = 'distributed' or e:alignment/@vertical = 'distributed' ">
          <xsl:attribute name="fo:wrap-option">
            <xsl:text>wrap</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="e:alignment/@wrapText">
          <xsl:attribute name="fo:wrap-option">
            <xsl:text>wrap</xsl:text>
          </xsl:attribute>
        </xsl:if>

        <!-- text orientation -->
        <xsl:if test="e:alignment/@textRotation">
          <xsl:choose>
            <xsl:when test="e:alignment/@textRotation = 255">
              <xsl:attribute name="style:direction">
                <xsl:text>ttb</xsl:text>
              </xsl:attribute>
            </xsl:when>
            <xsl:otherwise>
              <xsl:attribute name="style:rotation-angle">
                <xsl:choose>
                  <xsl:when
                    test="e:alignment/@textRotation &lt; 90 or e:alignment/@textRotation = 90">
                    <xsl:value-of select="e:alignment/@textRotation"/>
                  </xsl:when>
                  <xsl:when test="e:alignment/@textRotation &gt; 90">
                    <xsl:value-of select="450 - e:alignment/@textRotation"/>
                  </xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:attribute name="style:rotation-align">
            <xsl:text>none</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@applyBorder = 1 or  @borderId != '0' or $MergeCell != ''">
          <xsl:call-template name="InsertBorder">
            <xsl:with-param name="Id">
              <xsl:value-of select="$Id"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="LastHorizontalCellStyle">
              <xsl:value-of select="$LastHorizontalCellStyle"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>

        <xsl:if test="@applyProtection=1 and e:protection">
          <xsl:attribute name="style:cell-protect">
            <xsl:choose>
              <xsl:when test="e:protection/@locked=0 and e:protection/@hidden=1">
                <xsl:text>formula-hidden</xsl:text>
              </xsl:when>
              <xsl:when test="e:protection/@hidden= 1">
                <xsl:text>protected formula-hidden</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>protected</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
        </xsl:if>

        <xsl:if test="@applyFill=1 or @fillId!='0'">
          <xsl:apply-templates select="key('Fill', @fillId)" mode="style"/>
        </xsl:if>

      </style:table-cell-properties>

      <!-- default horizontal alignment when text has angle orientation  -->
      <xsl:if test="not(e:alignment/@horizontal) and e:alignment/@textRotation">
        <style:paragraph-properties>
          <xsl:attribute name="fo:text-align">
            <xsl:choose>
              <xsl:when
                test="e:alignment/@textRotation &lt; 90 or e:alignment/@textRotation = 180">
                <xsl:text>start</xsl:text>
              </xsl:when>
              <xsl:when
                test="e:alignment/@textRotation &gt; 89 and e:alignment/@textRotation &lt; 180">
                <xsl:text>end</xsl:text>
              </xsl:when>
              <xsl:when test="e:alignment/@textRotation = 255">
                <xsl:text>center</xsl:text>
              </xsl:when>
            </xsl:choose>
          </xsl:attribute>
        </style:paragraph-properties>
      </xsl:if>

      <xsl:if test="e:alignment/@horizontal">
        <style:paragraph-properties>
          <!-- horizontal-align -->
          <xsl:attribute name="fo:text-align">
            <xsl:choose>
              <xsl:when test="e:alignment/@horizontal = 'left' ">
                <xsl:text>start</xsl:text>
              </xsl:when>
              <xsl:when test="e:alignment/@horizontal = 'right' ">
                <xsl:text>end</xsl:text>
              </xsl:when>
              <xsl:when test="e:alignment/@horizontal = 'centerContinuous' ">
                <xsl:text>center</xsl:text>
              </xsl:when>
              <xsl:when test="e:alignment/@horizontal = 'distributed' ">
                <xsl:text>center</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="e:alignment/@horizontal"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>

          <xsl:if test="e:alignment/@indent and e:alignment/@indent != '0'">
            <xsl:attribute name="fo:margin-left">
              <xsl:variable name="indennt">
                <xsl:value-of select="e:alignment/@indent"/>
              </xsl:variable>
              <xsl:value-of select="format-number($indennt*0.353,'##.###cm') "/>
            </xsl:attribute>
          </xsl:if>

          <xsl:if test="e:alignment/@indent and e:alignment/@indent != '0'">
            <xsl:attribute name="fo:margin-right">
              <xsl:variable name="indennnt">
                <xsl:value-of select="e:alignment/@indent"/>
              </xsl:variable>
              <xsl:value-of select="format-number($indennnt*0.353,'##.###cm') "/>
            </xsl:attribute>
          </xsl:if>

        </style:paragraph-properties>
      </xsl:if>
    </xsl:if>

    <xsl:if test="@applyFont = 1 or  @fontId != ''">
      <style:text-properties>
        <xsl:apply-templates select="key('Font', @fontId)" mode="style"/>
      </style:text-properties>
    </xsl:if>
  </xsl:template>

  <!-- ignore text -->
  <xsl:template match="text()" mode="style"/>

  <!-- convert font name-->
  <xsl:template match="e:rFont" mode="style">
    <xsl:attribute name="style:font-name">
      <xsl:value-of select="@val"/>
    </xsl:attribute>
  </xsl:template>

  <!-- bold -->
  <!--<xsl:template match="e:b" mode="style">
    <xsl:attribute name="fo:font-weight">
      <xsl:text>bold</xsl:text>
    </xsl:attribute>
  </xsl:template>-->
<!--Vijayeta/Chavi,SP2,Font asian-->
  <xsl:template match="e:b" mode="style">
    <xsl:choose>
      <xsl:when test="not(@val)">
        <!--added by chhavi for sp2-->
        <xsl:attribute name="style:font-weight-asian">
          <xsl:text>bold</xsl:text>
        </xsl:attribute>
        <xsl:attribute name="fo:font-weight">
          <xsl:text>bold</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
	<!--Vijayeta/Chavi,SP2,Font asian,End-->
  <!-- italic -->
  <xsl:template match="e:i" mode="style">
    <xsl:choose>
      <xsl:when test="not(@val)">
        <xsl:attribute name="fo:font-style">
          <xsl:text>italic</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- insert underline -->
  <xsl:template match="e:u" mode="style">
    <xsl:call-template name="InsertUnderline"/>
  </xsl:template>

  <!-- superscript/subscript-->
  <xsl:template match="e:vertAlign" mode="style">
    <xsl:attribute name="style:text-position">
      <xsl:choose>
        <xsl:when test="@val = 'superscript' ">
          <xsl:text>super 58%</xsl:text>
        </xsl:when>
        <xsl:when test="@val = 'subscript' ">
          <xsl:text>sub 58%</xsl:text>
        </xsl:when>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template name="InsertTextStyles">

    <!-- @Description: inserts text styles -->
    <!-- @Context: none -->

    <xsl:apply-templates select="key('Part', 'xl/sharedStrings.xml')/e:sst/e:si/e:r[e:rPr]" mode="automaticstyles"/>
	    <!-- Perofomance-->
	  <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook">
		  <xsl:for-each select="e:sheets/e:sheet">
      <xsl:apply-templates select="key('Part', concat('xl/comments', position(), '.xml'))/e:comments">
        <xsl:with-param name="number">
          <xsl:value-of select="position()"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:for-each>
	  </xsl:for-each>
  </xsl:template>

  <xsl:template match="e:comments">

    <!-- @Description: inserts text styles in notes -->
    <!-- @Context: none -->

    <xsl:param name="number"/>
    <!--(int) number of sheet/comments file -->
    <xsl:apply-templates select="e:commentList/e:comment/e:text/e:r[e:rPr]" mode="automaticstyles"/>
    <xsl:apply-templates select="key('Part', concat('xl/comments',$number + 1,'.xml'))">
      <xsl:with-param name="number">
        <xsl:value-of select="$number+1"/>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <!-- convert run properties into span style -->
  <xsl:template match="e:r" mode="automaticstyles">
    <style:style style:name="{generate-id(.)}" style:family="text">
      <style:text-properties fo:font-weight="normal" fo:font-style="normal"
        style:text-underline-type="none" style:text-position="0% 100%">
        <xsl:apply-templates select="e:rPr" mode="style"/>
      </style:text-properties>
    </style:style>
  </xsl:template>

  <xsl:template name="InsertUnderline">
    <xsl:choose>
      <xsl:when test="@val = 'double'">
        <xsl:attribute name="style:text-underline-type">double</xsl:attribute>
        <xsl:attribute name="style:text-underline-style">solid</xsl:attribute>
        <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
      </xsl:when>
      <xsl:when test="@val = 'single'">
        <xsl:attribute name="style:text-underline-type">single</xsl:attribute>
        <xsl:attribute name="style:text-underline-style">solid</xsl:attribute>
        <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
      </xsl:when>
      <xsl:when test="@val = 'singleAccounting'">
        <xsl:attribute name="style:text-underline-type">single</xsl:attribute>
        <xsl:attribute name="style:text-underline-style">accounting</xsl:attribute>
        <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
      </xsl:when>
      <xsl:when test="@val = 'doubleAccounting'">
        <xsl:attribute name="style:text-underline-type">double</xsl:attribute>
        <xsl:attribute name="style:text-underline-style">accounting</xsl:attribute>
        <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
      </xsl:when>
      <xsl:when test="@val = 'none'">
        <xsl:attribute name="style:text-underline-type">none</xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="style:text-underline-type">single</xsl:attribute>
        <xsl:attribute name="style:text-underline-style">solid</xsl:attribute>
        <xsl:attribute name="style:text-underline-width">normal</xsl:attribute>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:if test="@color">
      <xsl:attribute name="style:text-underline-color">
        <xsl:choose>
          <xsl:when test="@color = 'auto'">font-color</xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat('#',@color)"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <!-- convert font size -->
  <xsl:template match="e:sz" mode="style">
    <xsl:attribute name="fo:font-size">
      <xsl:value-of select="round(@val)"/>
    </xsl:attribute>
    <!-- Fix for the bug 1802600 by chhavi -->
    <xsl:attribute name="style:font-size-asian">
      <xsl:value-of select="concat(round(./parent::node()/e:sz/@val),'pt')"/>
    </xsl:attribute>
    <xsl:attribute name="style:font-size-complex">
      <xsl:value-of select="concat(round(./parent::node()/e:sz/@val),'pt')"/>
    </xsl:attribute>
    <xsl:attribute name="style:font-name-asian">
      <xsl:value-of select="./parent::node()/e:name/@val"/>
    </xsl:attribute>
  </xsl:template>

  <!-- Fix for the bug 1802600
	     Template added by Vijayeta:Add an additional attribute 'style:font-size-asian', if charset is of type Japaneese
		 Date: 5th Nov '07-->
  <xsl:template match="e:charset" mode="style">
    <xsl:if test ="round(@val)=128">
      <xsl:attribute name="style:font-size-asian">
        <xsl:value-of select="concat(round(./parent::node()/e:sz/@val),'pt')"/>
      </xsl:attribute>
      <xsl:attribute name="style:font-size-complex">
        <xsl:value-of select="concat(round(./parent::node()/e:sz/@val),'pt')"/>
      </xsl:attribute>
      <xsl:attribute name="style:font-name-asian">
        <xsl:value-of select="./parent::node()/e:name/@val"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>
  <!-- End of fix for the bug 1802600-->

  <xsl:template match="e:name" mode="style">
    <xsl:attribute name="style:font-name">
      <xsl:value-of select="@val"/>
    </xsl:attribute>
  </xsl:template>

  <!-- insert strike-through -->
  <xsl:template match="e:strike" mode="style">
    <xsl:choose>
      <xsl:when test="not(@val)">
        <xsl:attribute name="style:text-line-through-style">
          <xsl:text>solid</xsl:text>
        </xsl:attribute>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- cell color fill -->
  <xsl:template match="e:fgColor" mode="style">
    <xsl:attribute name="fo:background-color">
      <xsl:call-template name="InsertColor"/>
    </xsl:attribute>
  </xsl:template>

  <!-- cell color fill in conditional -->
<!-- Defect: 2948277
Desc  : Also handling Cross Sheet Conditional Formatting for Office 2010.
 -->
  <xsl:template match="e:bgColor" mode="style">
    <xsl:if test="ancestor::e:dxf or ancestor::x14:dxf ">	 
      <xsl:attribute name="fo:background-color">
        <xsl:call-template name="InsertColor"/>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template match="e:color" mode="style">

    <xsl:attribute name="fo:color">
      <xsl:call-template name="InsertColor"/>
    </xsl:attribute>
  </xsl:template>

  <xsl:template name="InsertPageLayout">
    <xsl:variable name="ActiveTabNumber">
      <xsl:for-each select="key('Part', 'xl/workbook.xml')">
        <xsl:choose>
          <xsl:when test="e:workbook/e:bookViews/e:workbookView/@activeTab">
            <xsl:value-of select="e:workbook/e:bookViews/e:workbookView/@activeTab"/>
          </xsl:when>
          <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>
      </xsl:for-each>
    </xsl:variable>

    <xsl:for-each
      select="key('Part', concat('xl/worksheets/sheet', $ActiveTabNumber + 1,'.xml'))/e:worksheet">
      <style:page-layout style:name="pm1">
        <style:page-layout-properties>

          <!-- paper size -->
          <xsl:for-each select="e:pageSetup">
            <xsl:choose>
              <xsl:when test="@paperSize">
                <xsl:call-template name="InsertPaperSize">
                  <xsl:with-param name="paperSize" select="@paperSize"/>
                  <xsl:with-param name="orientation">
                    <xsl:choose>
                      <xsl:when test="@orientation = 'landscape' ">
                        <xsl:text>landscape</xsl:text>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:text>portrait</xsl:text>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>
              <!-- Letter -->
              <xsl:otherwise>
                <!-- Start of RefNo-2-Code inseretd by Sandeep, Fix for the bug 1810604 Date:22nd Oct '07-->
                <xsl:choose>
                  <xsl:when test="@orientation = 'landscape' ">
                    <xsl:attribute name="fo:page-width">
                      <xsl:text>27.94cm</xsl:text>
                    </xsl:attribute>
                    <xsl:attribute name="fo:page-height">
                      <xsl:text>21.59cm</xsl:text>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:attribute name="fo:page-width">
                      <xsl:text>21.59cm</xsl:text>
                    </xsl:attribute>
                    <xsl:attribute name="fo:page-height">
                      <xsl:text>27.94cm</xsl:text>
                    </xsl:attribute>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:otherwise>
              <!--End of RefNo-2-End of Code inseretd by Sandeep, Fix for the bug 1810604 Date:22nd Oct '07-->
            </xsl:choose>

            <!-- paper orientation -->
            <xsl:if test="@orientation">
              <xsl:attribute name="style:print-orientation">
                <xsl:value-of select="@orientation"/>
              </xsl:attribute>
            </xsl:if>

            <!-- page order -->
            <xsl:if test="@pageOrder = 'overThenDown' ">
              <xsl:attribute name="style:print-page-order">
                <xsl:text>ltr</xsl:text>
              </xsl:attribute>
            </xsl:if>

            <!-- first page number -->
            <xsl:if test="@useFirstPageNumber = 1 and @firstPageNumber">
              <xsl:attribute name="style:first-page-number">
                <xsl:value-of select="@firstPageNumber"/>
              </xsl:attribute>
            </xsl:if>

            <!-- fit on pages wide / pages tall -->
            <xsl:if test="parent::node()/e:sheetPr/e:pageSetUpPr/@fitToPage = 1">
              <xsl:attribute name="style:scale-to-X">
                <xsl:choose>
                  <xsl:when test="@fitToWidth">
                    <xsl:value-of select="@fitToWidth"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text>1</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>

              <xsl:attribute name="style:scale-to-Y">
                <xsl:choose>
                  <xsl:when test="@fitToHeight">
                    <xsl:value-of select="@fitToHeight"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text>1</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>

            <!-- scale printout -->
            <xsl:if test="@scale">
              <xsl:attribute name="style:scale-to">
                <xsl:value-of select="@scale"/>
              </xsl:attribute>
            </xsl:if>
          </xsl:for-each>

          <xsl:call-template name="InsertPageMargins"/>

          <xsl:for-each select="e:printOptions">
            <!-- table alignment -->
            <xsl:if test="@horizontalCentered = 1 or @verticalCentered = 1">
              <xsl:attribute name="style:table-centering">
                <xsl:choose>
                  <xsl:when test="@horizontalCentered = 1 and @verticalCentered = 1">
                    <xsl:text>both</xsl:text>
                  </xsl:when>
                  <xsl:when test="@horizontalCentered = 1">
                    <xsl:text>horizontal</xsl:text>
                  </xsl:when>
                  <xsl:when test="@verticalCentered = 1">
                    <xsl:text>vertical</xsl:text>
                  </xsl:when>
                </xsl:choose>
              </xsl:attribute>
            </xsl:if>

            <!-- print -->
            <xsl:if
              test="@headings = 1 or @gridLines = 1 or (parent::node()/e:pageSetup/@cellComments and parent::node()/e:pageSetup/@cellComments != 'none') ">
              <!-- notes -->
              <xsl:variable name="printNotes">
                <xsl:choose>
                  <xsl:when test="parent::node()/e:pageSetup/@cellComments != 'none' ">
                    <xsl:text>annotations charts drawings objects zero-values</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:text>charts drawings objects zero-values</xsl:text>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:variable>

              <!-- gridlines-->
              <xsl:variable name="printGrid">
                <xsl:choose>
                  <xsl:when test="@gridLines = 1">
                    <xsl:value-of select="concat($printNotes,' grid')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$printNotes"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:variable>

              <!-- row and column headings -->
              <xsl:variable name="printHeaders">
                <xsl:choose>
                  <xsl:when test="@headings = 1">
                    <xsl:value-of select="concat($printGrid,' headers')"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$printGrid"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:variable>

              <xsl:attribute name="style:print">
                <xsl:value-of select="$printHeaders"/>
              </xsl:attribute>
            </xsl:if>

          </xsl:for-each>

        </style:page-layout-properties>
        <style:header-style>
          <style:header-footer-properties fo:min-height="0.751cm" fo:margin-left="0cm"
            fo:margin-right="0cm" fo:margin-bottom="0.25cm" fo:margin-top="0cm"/>
        </style:header-style>
        <style:footer-style>
          <style:header-footer-properties fo:min-height="0.751cm" fo:margin-left="0cm"
            fo:margin-right="0cm" fo:margin-top="0.25cm" fo:margin-bottom="0cm"/>
        </style:footer-style>
      </style:page-layout>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertPageMargins">
    <xsl:for-each select="e:pageMargins">
      <xsl:if test="(@top and not(@header)) or (@top and @header = 0)">
        <xsl:attribute name="fo:margin-top">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@top,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@header and @header != 0">
        <xsl:attribute name="fo:margin-top">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@header,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="(@bottom and not(@footer) or (@bottom and @footer = 0))">
        <xsl:attribute name="fo:margin-bottom">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@bottom,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@footer and @footer != 0">
        <xsl:attribute name="fo:margin-bottom">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@footer,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@left">
        <xsl:attribute name="fo:margin-left">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@left,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="@right">
        <xsl:attribute name="fo:margin-right">
          <xsl:call-template name="ConvertToCentimeters">
            <xsl:with-param name="length" select="concat(@right,'in')"/>
          </xsl:call-template>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>


  <xsl:template name="InsertCellStyle">
    <xsl:for-each select="key('Part', 'xl/styles.xml')/e:styleSheet/e:cellXfs">
      <xsl:apply-templates select="e:xf" mode="stylesandformating"/>
    </xsl:for-each>
  </xsl:template>

	<!-- ODF1.1 Conformance
			   style:name has a value Normal 2/Heading 1 etc.. replaced as Normal to match the type NCName
    -->
  <xsl:template match="e:xf" mode="stylesandformating">
    <xsl:variable name="Xfid">
      <xsl:value-of select="@xfId"/>
    </xsl:variable>
    <xsl:if
      test="$Xfid != '0' and not(preceding-sibling::e:xf/@xfId = $Xfid) and key('CellStylesId', @xfId)/@name != 'Default'">
      <style:style>
        <xsl:attribute name="style:name">
					<xsl:value-of select="translate(translate(key('CellStylesId', @xfId)/@name,' ',''),'0123456789%!-$#():,.+','')"/>
        </xsl:attribute>
        <xsl:attribute name="style:family">
          <xsl:text>table-cell</xsl:text>
        </xsl:attribute>
        <xsl:call-template name="InsertCellFormat"/>
      </style:style>
    </xsl:if>
  </xsl:template>

  <xsl:template name="InsertCommentsBorderStyles">
    <draw:stroke-dash draw:name="round_20_dotted" draw:display-name="round dotted" draw:style="rect" draw:dots1="1" draw:dots2="1" draw:distance="0cm"/>

    <draw:stroke-dash draw:name="square_20_dotted" draw:display-name="square dotted"
      draw:style="rect" draw:dots1="1" draw:dots2="1" draw:distance="0.05cm" />

    <draw:stroke-dash draw:name="dashed" draw:style="rect" draw:dots1="1" draw:dots1-length="0.15cm" draw:dots2="1" draw:dots2-length="0.15cm" draw:distance="0.05cm" />

    <draw:stroke-dash draw:name="dash_20_dot" draw:display-name="dash dot" draw:style="rect"
      draw:dots1="1" draw:dots1-length="0.1cm" draw:dots2="1" draw:distance="0.05cm"/>

    <draw:stroke-dash draw:name="long_20_dash" draw:display-name="long dash" draw:style="rect"
      draw:dots1="1" draw:dots1-length="0.2cm" draw:dots2="1" draw:dots2-length="0.2cm"
      draw:distance="0.05cm"/>

    <draw:stroke-dash draw:name="long_20_dash_20_dot" draw:display-name="long dash dot"
      draw:style="rect" draw:dots1="1" draw:dots1-length="0.2cm" draw:dots2="1" draw:distance="0.05cm"/>

    <draw:stroke-dash draw:name="long_20_dash_20_dot_20_dot" draw:display-name="long dash dot dot" draw:style="rect" draw:dots1="1" draw:dots1-length="0.2cm" draw:dots2="2"
      draw:distance="0.1cm"/>
  </xsl:template>

</xsl:stylesheet>
