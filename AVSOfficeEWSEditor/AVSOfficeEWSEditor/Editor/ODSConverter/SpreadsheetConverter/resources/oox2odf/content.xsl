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
RefNo-1 22-Jan-2008 Sandeep S     1833074   Changes for fixing Cell Content missing and 1832335 New line inserted in note content after roundtrip conversions                                              
RefNo-2 23-May-2008 Sandeep S     1898009   Changes for fixing:XLSX borders in grouped columns lost
RefNo-3 20-Jun-2008 Sandeep S     1906975   Changes done to fix deffect Sheet not printed after conversion(issue with the sheet names)
RefNo-4 24-sep-2008 Sandeep s     Added some more invalid special charecters to the list (issue with the sheet names) 
RefNo-5 12-Jan-2009 Sandeep S     ODF1.1    Changes done for ODF1.1 conformance
RefNo-6 12-Jan-2009 Sandeep S     ODF1.1   Changes done for ODF1.1 conformance   
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:shared-strings"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:svg="urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:number="urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:dc="http://purl.org/dc/elements/1.1/"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:chart="urn:oasis:names:tc:opendocument:xmlns:chart:1.0"
  xmlns:dr3d="urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0"
  xmlns:math="http://www.w3.org/1998/Math/MathML"
  xmlns:form="urn:oasis:names:tc:opendocument:xmlns:form:1.0"
  xmlns:script="urn:oasis:names:tc:opendocument:xmlns:script:1.0"
  xmlns:ooo="http://openoffice.org/2004/office" xmlns:ooow="http://openoffice.org/2004/writer"
  xmlns:oooc="http://openoffice.org/2004/calc" xmlns:dom="http://www.w3.org/2001/xml-events"
  xmlns:xforms="http://www.w3.org/2002/xforms" xmlns:xsd="http://www.w3.org/2001/XMLSchema"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" office:version="1.0"
  xmlns:e="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:oox="urn:oox"
  xmlns:msxsl="urn:schemas-microsoft-com:xslt"
  xmlns:x14="http://schemas.microsoft.com/office/spreadsheetml/2009/9/main"
  exclude-result-prefixes="e r pxsi oox msxsl">

  <!--<xsl:import href="relationships.xsl"/>
  <xsl:import href="database-ranges.xsl"/>
  <xsl:import href="styles.xsl"/>
  <xsl:import href="table_body.xsl"/>
  <xsl:import href="number.xsl"/>
  <xsl:import href="picture.xsl"/>
  <xsl:import href="note.xsl"/>
  <xsl:import href="conditional.xsl"/>
  <xsl:import href="validation.xsl"/>
  <xsl:import href="elements.xsl"/>
  <xsl:import href="measures.xsl"/>
  <xsl:import href="ole_objects.xsl"/>
  <xsl:import href="connections.xsl"/>
  <xsl:import href="groups.xsl"/>
  <xsl:import href="scenario.xsl"/>
  <xsl:import href="change_tracking.xsl"/>
  <xsl:import href="pivot_tables.xsl"/>-->


  <!--xsl:key name="Sst" match="e:si" use="''"/-->
  <xsl:key name="SheetFormatPr" match="e:sheetFormatPr" use="@oox:part"/>
  <xsl:key name="Col" match="e:col" use="@oox:part"/>
  <xsl:key name="ConditionalFormatting" match="e:worksheet/e:conditionalFormatting" use="@oox:part"/>
  <xsl:key name="ConditionalFormattingO14" match="e:worksheet/e:extLst/e:ext/x14:conditionalFormattings/x14:conditionalFormatting" 
		   use="@oox:part"/>	
  <xsl:key name="ConditionalInheritance" match="e:worksheet/e:conditionalFormatting" use="@oox:id"/>

  <!-- recursive search and replace -->
  <xsl:template name="recursive">
    <xsl:param name="oldString"/>
    <xsl:param name="newString"/>
    <xsl:param name="wholeText"/>
    <xsl:choose>
      <xsl:when test="contains($wholeText, $oldString)">
        <xsl:value-of select="concat(substring-before($wholeText, $oldString), $newString)"/>
        <xsl:call-template name="recursive">
          <xsl:with-param name="oldString" select="$oldString"/>
          <xsl:with-param name="newString" select="$newString"/>
          <xsl:with-param name="wholeText" select="substring-after($wholeText, $oldString)"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$wholeText"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!--RefNo-6:ODF1.1:Added office:version="1.1" attribute-->
  <xsl:template name="content">
	  <office:document-content office:version="1.1">
      <office:scripts/>
      <office:font-face-decls>
        <xsl:call-template name="InsertFonts"/>
      </office:font-face-decls>
      <office:automatic-styles>
        <xsl:call-template name="InsertColumnStyles"/>

        <!--xsl:comment>Row Styles</xsl:comment-->
        <xsl:call-template name="InsertRowStyles"/>

        <!--xsl:comment>Number Styles</xsl:comment-->
        <xsl:call-template name="InsertNumberStyles"/>

        <!--xsl:comment>Cell Styles</xsl:comment-->
        <xsl:call-template name="InsertCellStyles"/>

        <!--xsl:comment>Merged Cell Styles</xsl:comment-->
        <xsl:call-template name="InsertMergeCellStyles"/>

        <!--xsl:comment>Horizontal Cell Styles</xsl:comment-->
        <xsl:if test="key('Part', 'xl/styles.xml')/e:styleSheet/e:cellXfs/e:xf/e:alignment/@horizontal = 'centerContinuous'">
          <xsl:call-template name="InsertHorizontalCellStyles"/>
        </xsl:if>

        <!--xsl:comment>Table Styles</xsl:comment-->
        <xsl:call-template name="InsertStyleTableProperties"/>

        <!--xsl:comment>Text Styles</xsl:comment-->
        <xsl:call-template name="InsertTextStyles"/>

        <!--xsl:comment>Text Box Styles</xsl:comment-->
        <xsl:call-template name="InsertTextBoxTextStyles"/>
        <!-- Insert Picture properties -->

        <!--xsl:comment>Picture Styles</xsl:comment-->
        <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]" mode="PictureStyle">
          <xsl:with-param name="number">1</xsl:with-param>
        </xsl:apply-templates>

        <!-- Insert Conditional Properties -->
        <!--xsl:comment>Conditional Styles</xsl:comment-->
        <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]" mode="ConditionalStyle">
          <xsl:with-param name="number">1</xsl:with-param>
        </xsl:apply-templates>
       
		<!-- Insert Conditional Inheritance -->
        <xsl:if test="key('ConditionalInheritance', 0)">
        <xsl:call-template name="ConditionalInheritance">
          <xsl:with-param name="id">
            <xsl:text>0</xsl:text>
          </xsl:with-param>
        </xsl:call-template>
        </xsl:if>
          
       

        <!-- Insert Scenario properties -->
        <!--xsl:comment>Scenario Styles</xsl:comment-->
        <xsl:call-template name="InsertScenarioStyles"/>
        <!-- Insert Note Shape properties -->
        <!--xsl:comment>Note Styles</xsl:comment-->
		   <!-- Perofomance-->
		  <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook">
			  <xsl:for-each select="e:sheets/e:sheet">
          <xsl:call-template name="InsertNoteStyles">
            <xsl:with-param name="sheetNr">
              <xsl:value-of select="position()"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
		  </xsl:for-each>
      </office:automatic-styles>
      <xsl:call-template name="InsertSheets"/>
    </office:document-content>
  </xsl:template>

  <xsl:template name="InsertSheets">

    <office:body>
      <office:spreadsheet>

        <!--Insert Change Tracking -->
        <xsl:call-template name="InsertChangeTracking"/>

        <xsl:variable name="rSheredStrings">
          <xsl:call-template name="rSheredStrings"/>
        </xsl:variable>

        <!--RefNo-5:ODF1.1:Added element <table:content-validations> to avoid repetion-->
        <xsl:variable name="contentValidation">
        <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]"
          mode="Validation">
          <xsl:with-param name="number">1</xsl:with-param>
          <xsl:with-param name="rSheredStrings">
            <xsl:value-of select="$rSheredStrings"/>
          </xsl:with-param>
        </xsl:apply-templates>
        </xsl:variable>
        
        <!--Start of RefNo-5:ODF1.1:Added element <table:content-validations> to avoid repetion-->
        <xsl:if test="msxsl:node-set($contentValidation)/table:content-validation">
          <table:content-validations>
            <xsl:copy-of select="$contentValidation"/>
          </table:content-validations>
        </xsl:if>
        <!--End of RefNo-5-->

        <!-- insert strings from sharedStrings to be moved later by post-processor-->
		    <!-- Perofomance-->
		  <xsl:for-each select="key('Part', 'xl/sharedStrings.xml')">
			  <xsl:for-each select="e:sst">
          <pxsi:sst xmlns:pxsi="urn:cleverage:xmlns:post-processings:shared-strings">
            <xsl:for-each select="e:si">
              <xsl:call-template name="e:si"/>
            </xsl:for-each>
          </pxsi:sst>
        </xsl:for-each>
		  </xsl:for-each>

        <!--xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet">
          <xsl:call-template name="gaga" >
            <xsl:with-param name="number">1</xsl:with-param>
            <xsl:with-param name="rSheredStrings" select="$rSheredStrings"/>
          </xsl:call-template>
        </xsl:for-each-->

        <xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]">
          <xsl:with-param name="number">1</xsl:with-param>
          <xsl:with-param name="rSheredStrings">
            <xsl:value-of select="$rSheredStrings"/>
          </xsl:with-param>
        </xsl:apply-templates>

        
		  <!--
		  Feature: Named Ranges
		  By     : Vijayeta
		  Date   :11th Sept '08
		  -->
		  <!--<table:named-expressions>
			  <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName">
				  <xsl:if test ="not(contains(@name,'_xlnm.Print_Area')) and not(contains(@name, '_xlnm.Print_Titles'))">
					  <xsl:if test ="not(contains(.,'#REF'))">
						  <xsl:variable name ="range">
							  <xsl:value-of select ="."/>
						  </xsl:variable>
						  <xsl:variable name ="isSingleRange">
							  <xsl:variable name ="sheet">
								  <xsl:value-of select ="substring-before($range,'!')"/>
							  </xsl:variable>
							  <xsl:if test ="contains(substring-after($range,'!'),$sheet)">
								  <xsl:value-of select ="'false'"/>
							  </xsl:if>
							  <xsl:if test ="not(contains(substring-after($range,'!'),$sheet))">
								  <xsl:value-of select ="'true'"/>
							  </xsl:if>
						  </xsl:variable>
						  <xsl:variable name ="isFunction">
							  <xsl:choose>
								  <xsl:when test="not(contains($range,'(') and contains($range,')')) and (contains($range,':') or $isSingleRange='true')  ">
									  <xsl:value-of select ="'false'"/>
								  </xsl:when>
								  <xsl:when test="contains($range,'(') and contains($range,')')">
									  <xsl:value-of select ="'true'"/>
								  </xsl:when>
								  <xsl:otherwise>
									  <xsl:value-of select ="''"/>
								  </xsl:otherwise>
							  </xsl:choose>
						  </xsl:variable>
						  <xsl:variable name ="cellRangeAddress">
							  <xsl:choose>
								  <xsl:when test ="substring-after($range,':')!=''">
									  <xsl:variable name ="part1">
										  <xsl:choose>
											  <xsl:when test="$isFunction='false'">
												  <xsl:value-of select ="translate(substring-before($range,':'),'!','.')"/>
											  </xsl:when>
											  <xsl:when test="$isFunction='true'">
												  <xsl:value-of select ="translate(substring-after(substring-before($range,':'),'('),'!','.')"/>
											  </xsl:when>
										  </xsl:choose>
									  </xsl:variable>
									  <xsl:variable name ="part2">
										  <xsl:variable name ="endRange">
											  <xsl:value-of select ="substring-after($range,':')"/>
										  </xsl:variable>
										  <xsl:choose>
											  <xsl:when test ="contains($endRange,'!')">
												  <xsl:choose>
													  <xsl:when test="$isFunction='false'">
														  <xsl:value-of select ="concat('.',translate(substring-after($endRange,'!'),'!',''))"/>
													  </xsl:when>
													  <xsl:when test="$isFunction='true'">
														  <xsl:value-of select ="concat('.',translate(substring-before(substring-after($endRange,'!'),')'),'!',''))"/>
													  </xsl:when>
												  </xsl:choose>
											  </xsl:when>
											  <xsl:otherwise>
												  <xsl:choose>
													  <xsl:when test="$isFunction='false'">
														  <xsl:value-of select ="concat('.',$endRange)"/>
													  </xsl:when>
													  <xsl:when test="$isFunction='true'">
														  <xsl:value-of select ="concat('.',substring-before($endRange,')'))"/>
													  </xsl:when>
												  </xsl:choose>
											  </xsl:otherwise>
										  </xsl:choose>
										  -->
		  <!--<xsl:value-of select ="concat('.',translate(substring-after($range,':'),'!','.'))"/>-->
		  <!--
									  </xsl:variable>
									  <xsl:value-of select ="concat($part1,':',$part2)"/>
								  </xsl:when>
								  <xsl:otherwise>
									  <xsl:choose>
										  <xsl:when test="$isFunction='false'">
											  <xsl:value-of select ="translate($range,'!','.')"/>
										  </xsl:when>
										  <xsl:when test="$isFunction='true'">
											  <xsl:value-of select ="translate(substring-after(substring-before($range,')'),'('),'!','.')"/>
										  </xsl:when>
									  </xsl:choose>
								  </xsl:otherwise>
							  </xsl:choose>
						  </xsl:variable>
						  <xsl:choose>
							  <xsl:when test="$isFunction='false'">
								  <xsl:variable name ="baseAddress">
									  <xsl:choose>
										  <xsl:when test ="substring-after($range,':')!=''">
											  <xsl:value-of select ="translate(substring-before($range,':'),'!','.')"/>
										  </xsl:when>
										  <xsl:otherwise>
											  <xsl:value-of select ="translate($range,'!','.')"/>
										  </xsl:otherwise>
									  </xsl:choose>
								  </xsl:variable>
								  <table:named-range>
									  <xsl:attribute name ="table:name">
										  <xsl:value-of select ="@name"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:base-cell-address">
										  <xsl:value-of select ="concat('$',$baseAddress)"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:cell-range-address">
										  <xsl:value-of select ="concat('$',$cellRangeAddress)"/>
									  </xsl:attribute>
								  </table:named-range>
							  </xsl:when>
							  <xsl:when test="$isFunction='true'">
								  <xsl:variable name ="function">
									  <xsl:value-of select ="substring-before($range,'(')"/>
								  </xsl:variable>
								  <table:named-expression>
									  <xsl:attribute name ="table:name">
										  <xsl:value-of select ="@name"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:base-cell-address">
										  <xsl:value-of select ="'$Sheet1.$A$1'"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:expression">
										  <xsl:value-of select ="concat($function,'([$',$cellRangeAddress,'])')"/>
										  -->
		  <!--<xsl:value-of select ="concat('sonataOoxFormula',$function,'([$',$cellRangeAddress,'])','##shtName##',$sheetNames)"/>-->
		  <!--
									  </xsl:attribute>
								  </table:named-expression>
							  </xsl:when>
						  </xsl:choose>
					  </xsl:if>
				  </xsl:if>
			  </xsl:for-each>
		  </table:named-expressions>-->
				<!-- ODF1.1 Conformance
			    range in form $Sheet1.$A$1:.$C$1
		        -->				
				<table:named-expressions>
			  <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName">
				  <xsl:if test ="not(contains(@name,'_xlnm.Print_Area')) and not(contains(@name, '_xlnm.Print_Titles'))">
							<!--<xsl:if test ="not(contains(.,'#REF')) and not(contains(.,'OFFSET'))">-->
							<xsl:if test ="not(contains(.,'#REF'))">
						  <xsl:variable name ="range">
									<xsl:if test ="contains(.,':')">
										<xsl:value-of select ="concat(substring-before(.,':'),':.',substring-after(.,':'))"/>
									</xsl:if>
									<xsl:if test ="not(contains(.,':'))">
							  <xsl:value-of select ="."/>
									</xsl:if>
						  </xsl:variable>
						  <xsl:variable name ="isFunction">
							  <xsl:choose>
								  <xsl:when test="not(contains($range,'(') and contains($range,')'))">
											<xsl:choose >
												<xsl:when test="contains($range,',')">
													<xsl:value-of select ="'true'"/>
												</xsl:when>
												<xsl:otherwise>
									  <xsl:value-of select ="'false'"/>
												</xsl:otherwise>
											</xsl:choose>
								  </xsl:when>
								  <xsl:when test="contains($range,'(') and contains($range,')')">
									  <xsl:variable name ="parenthesisContents">
										  <xsl:value-of select ="substring-after($range,'!')"/>
									  </xsl:variable>
									  <xsl:choose>
										  <xsl:when test ="contains($parenthesisContents,')')">
											  <xsl:value-of select ="'true'"/>
										  </xsl:when>
										  <xsl:when test ="not(contains($parenthesisContents,')'))">
											  <xsl:value-of select ="'false'"/>
										  </xsl:when>
									  </xsl:choose>
								  </xsl:when>
								  <xsl:otherwise>
									  <xsl:value-of select ="''"/>
								  </xsl:otherwise>
							  </xsl:choose>
						  </xsl:variable>
						  <xsl:variable name ="cellRangeAddress">
							  <xsl:choose>
								  <xsl:when test="$isFunction='false'">
									  <xsl:value-of select ="$range"/>
								  </xsl:when>
								  <xsl:when test="$isFunction='true'">
											<xsl:choose >
												<xsl:when test="contains($range,',')">
													<xsl:value-of select ="''"/>
												</xsl:when>
												<xsl:otherwise>
									  <xsl:value-of select ="concat(substring-after(substring-before($range,'!'),'('),'!',substring-before(substring-after($range,'!'),')'))"/>
												</xsl:otherwise>
											</xsl:choose>
								  </xsl:when>
							  </xsl:choose>
						  </xsl:variable>
						  <xsl:variable name ="NamedRange">
									<xsl:if  test ="$cellRangeAddress!=''">
							  <xsl:value-of select ="substring-after($cellRangeAddress,'!')"/>
									</xsl:if>
						  </xsl:variable>
						  <xsl:variable name ="sheetName">
									<xsl:if  test ="$cellRangeAddress!=''">
							  <xsl:value-of select ="substring-before($cellRangeAddress,'!')"/>
									</xsl:if>
						  </xsl:variable>
						  <xsl:variable name ="apos">
							  <xsl:text>&apos;</xsl:text>
						  </xsl:variable>
						  <xsl:variable name="checkedName">
									<xsl:if  test ="$cellRangeAddress!=''">
							  <xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($sheetName,$apos,'')]">
								  <xsl:call-template name="CheckSheetName">
									  <xsl:with-param name="sheetNumber">
										  <xsl:for-each
											select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[@name = translate($sheetName,$apos,'')]">
											  <xsl:value-of select="count(preceding-sibling::e:sheet) + 1"/>
										  </xsl:for-each>
									  </xsl:with-param>
									  <xsl:with-param name="name">
										  <xsl:value-of select="translate(@name,'!-$#():,.+','')"/>
									  </xsl:with-param>
								  </xsl:call-template>
							  </xsl:for-each>
									</xsl:if>
						  </xsl:variable>
						  <xsl:variable name ="correctName">
									<xsl:if  test ="$cellRangeAddress!=''">
							  <xsl:choose>
								  <xsl:when test="starts-with($sheetName,$apos) and $checkedName!=''">
									  <xsl:value-of select="concat($apos,$checkedName,$apos)"/>
								  </xsl:when>
								  <xsl:when test="not(starts-with($sheetName,$apos)) and $checkedName!=''">
										  <xsl:value-of select="$checkedName"/>
								  </xsl:when>
							  </xsl:choose>
									</xsl:if>
						  </xsl:variable>
						  <xsl:if test ="$correctName != ''">							  
						  <xsl:choose>
							  <xsl:when test="$isFunction='false'">
								  <xsl:variable name ="baseAddress">
									  <xsl:choose>
										  <xsl:when test ="substring-after($NamedRange,':')!=''">
														<xsl:variable name ="beginColOrRow">
															<xsl:choose >
																<xsl:when test ="contains(substring-after(substring-before($NamedRange,':'),'$'),'$')">
																	<xsl:value-of select ="'RCpresent'"/>
																</xsl:when>
																<xsl:otherwise >
																	<xsl:variable name ="character">
																		<xsl:value-of select ="substring-after(substring-before($NamedRange,':'),'$')"/>
																	</xsl:variable>
																	<xsl:choose>
                                    <xsl:when test="contains($character,'A') or contains($character,'B') or contains($character,'C') or contains($character,'D') or contains($character,'E') or contains($character,'F') or contains($character,'G') 
							  or contains($character,'H') or contains($character,'I') or contains($character,'J') or contains($character,'K') or contains($character,'L') or contains($character,'M') or contains($character,'N') or contains($character,'O') 
            				  or contains($character,'P') or contains($character,'Q') or contains($character,'R') or contains($character,'S') or contains($character,'T') or contains($character,'U') or contains($character,'V') or contains($character,'W') 
							  or contains($character,'X') or contains($character,'Y') or contains($character,'Z') or contains($character,'a') or contains($character,'b') or contains($character,'c') or contains($character,'d') or contains($character,'e')
							  or contains($character,'f') or contains($character,'g') or contains($character,'h') or contains($character,'i') or contains($character,'j') or contains($character,'k') or contains($character,'l') or contains($character,'m')
							  or contains($character,'n') or contains($character,'o') or contains($character,'p') or contains($character,'q') or contains($character,'r') or contains($character,'s') or contains($character,'t') or contains($character,'u')
							  or contains($character,'v') or contains($character,'w') or contains($character,'x') or contains($character,'y') or contains($character,'z')">
																			<xsl:value-of select ="'true'"/>
																		</xsl:when>
																		<xsl:otherwise>
																			<xsl:value-of select ="'false'"/>
																		</xsl:otherwise>
																	</xsl:choose>
																</xsl:otherwise>
															</xsl:choose>
														</xsl:variable>
														<xsl:variable name ="beginCol">
															<xsl:choose>
																<xsl:when test ="$beginColOrRow='RCpresent'">
																	<xsl:value-of select ="substring-before(substring-after(substring-before($NamedRange,':'),'$'),'$')"/>
																</xsl:when>
																<xsl:when test ="$beginColOrRow='true'">
																	<xsl:value-of select ="substring-after(substring-before($NamedRange,':'),'$')"/>
																</xsl:when>
																<xsl:when test ="$beginColOrRow='false'">
																	<xsl:value-of select ="'A'"/>
																</xsl:when>
															</xsl:choose>
														</xsl:variable>
														<xsl:variable name ="beginRow">
															<xsl:choose>
																<xsl:when test ="$beginColOrRow='RCpresent'">
																	<xsl:value-of select ="substring-after(substring-after(substring-before($NamedRange,':'),'$'),'$')"/>
																</xsl:when>
																<xsl:when test ="$beginColOrRow='true'">
																	<xsl:value-of select ="1"/>
																</xsl:when>
																<xsl:when test ="$beginColOrRow='false'">
																	<xsl:value-of select ="substring-after(substring-before($NamedRange,':'),'$')"/>
																</xsl:when>
															</xsl:choose>
														</xsl:variable>
														<xsl:value-of select ="concat(concat(&quot;&apos;&quot;,translate($correctName,&quot;&apos;&quot;,''),&quot;&apos;&quot;),'.$',$beginCol,'$',$beginRow)"/>
														<!--<xsl:value-of select ="concat(concat(&quot;&apos;&quot;,translate($correctName,&quot;&apos;&quot;,''),&quot;&apos;&quot;),'.',substring-before($NamedRange,':'),$endRow)"/>-->
										  </xsl:when>
										  <xsl:otherwise>
											  <xsl:value-of select ="concat($correctName,'.',$NamedRange)"/>
										  </xsl:otherwise>
									  </xsl:choose>
								  </xsl:variable>
								  <table:named-range>
									  <xsl:attribute name ="table:name">
										  <xsl:value-of select ="@name"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:base-cell-address">
										  <xsl:value-of select ="concat('$',$baseAddress)"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:cell-range-address">
                                                                                                            <xsl:variable name ="FinalRange">
														<xsl:call-template name="translate-expression">
															<xsl:with-param name="isRangeAddress" select="true()"/>
															<xsl:with-param name="cell-row-pos" select="0"/>
															<xsl:with-param name="cell-column-pos" select="0"/>
															<xsl:with-param name="expression" select="."/>
															<xsl:with-param name="return-value" select="''"/>
														</xsl:call-template>
													</xsl:variable>
                      <!--If Multiple named ranges-->
                      <xsl:if test ="contains($FinalRange,':')">
                        <xsl:variable name ="beginEndRowCol">
                          <xsl:call-template name ="translateRangetoODF">
                            <xsl:with-param name ="range" select ="$range"/>
                          </xsl:call-template>
													</xsl:variable>
                        <xsl:variable name ="sheetNameCorrect">
                          <xsl:value-of select="concat($apos,translate(substring-before($range,'!'),$apos,''),$apos)"/>
													</xsl:variable>
													<xsl:variable name ="beginCol">
                          <xsl:value-of select ="substring-before($beginEndRowCol,'|')"/>
													</xsl:variable>
													<xsl:variable name ="beginRow">
                          <xsl:value-of select ="substring-before(substring-after($beginEndRowCol,concat($beginCol,'|')),'|')"/>
                        </xsl:variable>
                        <xsl:variable name ="endCol">
                          <xsl:value-of select ="substring-before(substring-after($beginEndRowCol,concat($beginCol,'|',$beginRow,'|')),'|')"/>
													</xsl:variable>
													<xsl:variable name ="endRow">
                          <xsl:value-of select ="substring-after($beginEndRowCol,concat($beginCol,'|',$beginRow,'|',$endCol,'|'))"/>
													</xsl:variable>
														<xsl:value-of select ="concat('$',$sheetNameCorrect,'.$',$beginCol,'$',$beginRow,':.$',$endCol,'$',$endRow)"/>
													<!--<xsl:value-of select ="concat(substring-before($FinalRange,':'),':.',substring-after($FinalRange,':'))"/>-->
													</xsl:if>
													<xsl:if test ="not(contains($FinalRange,':'))">
														<xsl:value-of select ="$FinalRange"/>
													</xsl:if>
									  </xsl:attribute>
								  </table:named-range>
							  </xsl:when>
							  <xsl:when test="$isFunction='true'">								  
								  <table:named-expression>
									  <xsl:attribute name ="table:name">
										  <xsl:value-of select ="@name"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:base-cell-address">
										  <xsl:value-of select ="'$Sheet1.$A$1'"/>
									  </xsl:attribute>
									  <xsl:attribute name ="table:expression">
													<xsl:choose>
														<xsl:when test ="contains($range,',')">
															<xsl:variable name ="correctRange">
																<xsl:call-template name ="getCorrectNamedRange">
																	<xsl:with-param  name ="NamedRange" select="$range"/>
																</xsl:call-template>
															</xsl:variable>
															<xsl:value-of select ="$correctRange"/>
														</xsl:when>
														<xsl:otherwise>
													<xsl:call-template name="translate-expression">
														<xsl:with-param name="isRangeAddress" select="true()"/>
														<xsl:with-param name="cell-row-pos" select="0"/>
														<xsl:with-param name="cell-column-pos" select="0"/>
														<xsl:with-param name="expression" select="."/>
														<xsl:with-param name="return-value" select="''"/>
													</xsl:call-template>
														</xsl:otherwise>
													</xsl:choose>
									  </xsl:attribute>
								  </table:named-expression>
							  </xsl:when>
						  </xsl:choose>							  
						  </xsl:if>						 
								<xsl:if test ="$correctName = '' and contains($range,',') ">
									<table:named-expression>
										<xsl:attribute name ="table:name">
											<xsl:value-of select ="@name"/>
										</xsl:attribute>
										<xsl:attribute name ="table:base-cell-address">
											<xsl:value-of select ="'$Sheet1.$A$1'"/>
										</xsl:attribute>
										<xsl:attribute name ="table:expression">
											<xsl:variable name ="correctRange">
												<xsl:call-template name ="getCorrectNamedRange">
													<xsl:with-param  name ="NamedRange" select="$range"/>
												</xsl:call-template>
											</xsl:variable>
											<xsl:value-of select ="$correctRange"/>											
										</xsl:attribute>
									</table:named-expression>
								</xsl:if>
					  </xsl:if>
				  </xsl:if>
			  </xsl:for-each>
				</table:named-expressions>
		  <!-- end of code for the feature 'Named Ranges'-->

				<!--ODF1.1 Conformance 
				   'table:database-ranges' shd come only after 'table:named-expressions'
				-->
				<table:database-ranges>
					<xsl:apply-templates select="key('Part', 'xl/workbook.xml')/e:workbook/e:sheets/e:sheet[1]">
						<xsl:with-param name="number">1</xsl:with-param>
						<xsl:with-param name="mode" select="'database'"/>
						<xsl:with-param name="rSheredStrings">
							<xsl:value-of select="$rSheredStrings"/>
						</xsl:with-param>
					</xsl:apply-templates>
				</table:database-ranges>
        <!--xsl:variable name="pivotTables"-->
        <xsl:call-template name="InsertPilotTables"/>
        <!--/xsl:variable-->

      </office:spreadsheet>
    </office:body>
  </xsl:template>

  <xsl:template match="e:sheet">
    <xsl:param name="number"/>
    <xsl:param name="mode"/>
    <xsl:param name="rSheredStrings"/>

    <xsl:variable name="target">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id" select="@r:id"/>
        <xsl:with-param name="document">
          <xsl:text>xl/workbook.xml</xsl:text>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <!-- insert tables -->
      <xsl:when test="$mode = '' ">

        <xsl:choose>
          <!-- when sheet is a chartsheet -->
          <xsl:when test="starts-with($target,'chartsheets/')">
            <xsl:call-template name="InsertChartsheet">
              <xsl:with-param name="number" select="$number"/>
              <xsl:with-param name="sheet" select="$target"/>
            </xsl:call-template>
          </xsl:when>
          <!-- when sheet is a worksheet -->
          <xsl:otherwise>
            <xsl:call-template name="InsertWorksheet">
              <xsl:with-param name="number" select="$number"/>
              <xsl:with-param name="rSheredStrings">
                <xsl:value-of select="$rSheredStrings"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>

      <!-- insert database ranges -->
      <xsl:otherwise>

        <!-- if there is filter or sort -->
        <xsl:if
          test="key('Part', concat('xl/',$target))/e:worksheet/e:autoFilter or key('Part', concat('xl/',$target))/e:worksheet/e:sortState/e:sortCondition[not(@customList)]">

          <xsl:variable name="checkedName">
            <xsl:call-template name="CheckSheetName">
              <xsl:with-param name="sheetNumber">
                <xsl:value-of select="$number"/>
              </xsl:with-param>
              <xsl:with-param name="name">
                <xsl:value-of select="translate(@name,'!-$#():,.+','')"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:variable>

          <xsl:for-each select="key('Part', concat('xl/',$target))/e:worksheet">
            <xsl:call-template name="InsertDatabaseRange">
              <xsl:with-param name="number" select="$number"/>
              <xsl:with-param name="checkedName" select="$checkedName"/>
            </xsl:call-template>
          </xsl:for-each>
        </xsl:if>

      </xsl:otherwise>
    </xsl:choose>

    <!-- Insert next Table -->
    <xsl:choose>
      <xsl:when test="$number &gt; 255">
        <xsl:message terminate="no">translation.oox2odf.SheetNumber</xsl:message>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="following-sibling::e:sheet[1]">
          <xsl:with-param name="number">
            <xsl:value-of select="$number + 1"/>
          </xsl:with-param>
          <xsl:with-param name="mode" select="$mode"/>
          <xsl:with-param name="rSheredStrings">
            <xsl:value-of select="$rSheredStrings"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="InsertWorksheet">
    <xsl:param name="number"/>
    <xsl:param name="rSheredStrings"/>

    <xsl:variable name="Id">
      <xsl:call-template name="GetTarget">
        <xsl:with-param name="id">
          <xsl:value-of select="@r:id"/>
        </xsl:with-param>
        <xsl:with-param name="document">xl/workbook.xml</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

	  <!--Start of RefNo-3-->
    <xsl:variable name="strSheetName" select="@name"/>    
    <!--End of RefNo-3-->
	  <!-- Defect :1803593, file '03706191.CONFIDENTIAL.xlsx 
		   Changes by: Vijayeta
		   Desc:â€?-â€? is removed from the list of symbols to get the value of â€?checkedNameâ€™.,line 342 
		   This is done because some of the sheets in the file â€?03706191.CONFIDENTIAL.xlsxâ€™ have names such as â€?E3-SITESâ€™, N2-L, and so on
      -->
    <xsl:variable name="checkedName">
      <xsl:call-template name="CheckSheetName">
        <xsl:with-param name="sheetNumber">
          <xsl:value-of select="$number"/>
        </xsl:with-param>
        <xsl:with-param name="name">
          <!--RefNo-3-->
					<xsl:value-of select="translate(@name,'!-$#():,.+','')"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
	  <!-- Perofomance-->
    <xsl:variable name="BigMergeCell">
		<xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet">
			<xsl:for-each select="e:mergeCells">
        <xsl:apply-templates select="e:mergeCell[1]" mode="BigMergeColl"/>
      </xsl:for-each>
		</xsl:for-each>
    </xsl:variable>
	    <!-- Perofomance-->
    <xsl:variable name="BigMergeRow">
		<xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet">
			<xsl:for-each select="e:mergeCells">
        <xsl:apply-templates select="e:mergeCell[1]" mode="BigMergeRow"/>
      </xsl:for-each>
		</xsl:for-each>
    </xsl:variable>

    <!-- variable with merge cell -->
	   <!-- Perofomance-->
    <xsl:variable name="MergeCell">
		<xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet">
			<xsl:for-each select="e:mergeCells">
        <xsl:apply-templates select="e:mergeCell[1]" mode="merge"/>
      </xsl:for-each>
		</xsl:for-each>
    </xsl:variable>


    <!-- Check If Picture are in this sheet  -->
    <xsl:variable name="PictureCell">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="PictureCell">
          <xsl:with-param name="sheet">
            <xsl:value-of select="substring-after($Id, '/')"/>
          </xsl:with-param>
          <xsl:with-param name="mode">
            <xsl:value-of select="substring-before($Id, '/')"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <!-- Check if notes are in this sheet -->
    <xsl:variable name="NoteCell">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="NoteCell">
          <xsl:with-param name="sheetNr" select="$number"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <!-- Check If Conditionals are in this sheet -->

    <xsl:variable name="ConditionalCell">
      <!--xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ConditionalCell"/>
      </xsl:for-each-->
    </xsl:variable>

    <xsl:variable name="ConditionalRow">
      <!--xsl:call-template name="ConditionalRow">
        <xsl:with-param name="ConditionalCell">
          <xsl:value-of select="$ConditionalCell"/>
        </xsl:with-param>
      </xsl:call-template-->
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
    
    <xsl:variable name="ConditionalCellCol">
      <!--xsl:for-each select="key('Part', concat('xl/',$Id))">
          <xsl:call-template name="ConditionalCellCol">        
          </xsl:call-template>
       </xsl:for-each-->
    </xsl:variable>
    
    <xsl:variable name="ConditionalCellAll">
      <!--xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ConditionalCellAll">          
        </xsl:call-template>
      </xsl:for-each-->
    </xsl:variable>
    
    <xsl:variable name="ConditionalCellSingle">
      <!--xsl:call-template name="ConditionalCellSingle">
        <xsl:with-param name="sqref">
          <xsl:value-of select="$ConditionalCellAll"/>
        </xsl:with-param>
      </xsl:call-template-->
    </xsl:variable>
    
    
    <xsl:variable name="ConditionalCellMultiple">
      <!--xsl:call-template name="ConditionalCellMultiple">
        <xsl:with-param name="sqref">
          <xsl:value-of select="$ConditionalCellAll"/>
        </xsl:with-param>
      </xsl:call-template-->
    </xsl:variable>
    

    <!-- Check If Data Validation are in this sheet -->
    <xsl:variable name="ValidationCell">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ValidationCell"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="ValidationRow">
      <xsl:call-template name="ValidationRow">
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="ValidationCellStyle">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ValidationCell">
          <xsl:with-param name="document">
            <xsl:text>style</xsl:text>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <!-- Check if Scenario are in this sheet -->
    <xsl:variable name="ScenarioCell">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="ScenarioCell"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="PictureRow">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="PictureRow">
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="NoteRow">
      <xsl:for-each select="key('Part', concat('xl/',$Id))">
        <xsl:call-template name="NoteRow">
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

	    <!-- Perofomance-->
    <xsl:variable name="removeFilter">
      <xsl:if test="key('Part', concat('xl/',$Id))/e:worksheet/e:autoFilter">
			<xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet">
				<xsl:for-each select="e:autoFilter">
          <xsl:variable name="filtersNum">
            <xsl:value-of select="count(e:filterColumn/e:filters)"/>
          </xsl:variable>
          <xsl:variable name="customFiltersNum">
            <xsl:value-of select="count(e:filterColumn/e:customFilters)"/>
          </xsl:variable>
          <xsl:variable name="topFiltersNum">
            <xsl:value-of select="count(e:filterColumn/e:top10)"/>
          </xsl:variable>

          <xsl:if
            test="e:filterColumn/e:filters/e:filter[position() = 2] and $filtersNum + $customFiltersNum + $topFiltersNum &gt; 1">
            <xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell" select="substring-before(@ref,':')"/>
            </xsl:call-template>
            <xsl:text>:</xsl:text>
            <xsl:call-template name="GetRowNum">
              <xsl:with-param name="cell" select="substring-after(@ref,':')"/>
            </xsl:call-template>
          </xsl:if>
				</xsl:for-each>
        </xsl:for-each>
      </xsl:if>
    </xsl:variable>

    <xsl:variable name="ConnectionCell">
      <xsl:call-template name="ConnectionsCell">
        <xsl:with-param name="number">
          <xsl:value-of select="$number - 1"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!-- variable with values of all manual row breaks-->
    <xsl:variable name="AllRowBreakes">
      <xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet/e:rowBreaks/e:brk">
        <xsl:value-of select="concat(@id + 1,';')"/>
      </xsl:for-each>
    </xsl:variable>

    <table:table>

      <xsl:variable name="GroupCell">
        <xsl:apply-templates select="key('Part', concat('xl/',$Id))/e:worksheet/e:cols/e:col[1]"
          mode="groupTag"/>
      </xsl:variable>


      <!-- Insert Table (Sheet) Name -->
      <xsl:attribute name="table:name">
        <!--        <xsl:value-of select="@name"/>-->
        <xsl:value-of select="$checkedName"/>
      </xsl:attribute>

      <!-- Insert Table Style Name (style:table-properties) -->

      <xsl:attribute name="table:style-name">
        <xsl:value-of select="generate-id()"/>
      </xsl:attribute>

      <!-- insert Print Range -->
      <xsl:variable name="apostrof">
        <xsl:text>&apos;</xsl:text>
      </xsl:variable>
		  <!-- Perofomance-->
		<xsl:for-each select="key('Part', 'xl/workbook.xml')/e:workbook">
			<xsl:for-each select="e:definedNames/e:definedName">
        <!-- for the current sheet -->
        <xsl:if test="not(contains(self::node(),'#REF'))">
          <!-- if print range with apostrophes -->
          <!--
		         Defect    :1954140
		         Changes by: Vijayeta
		         Desc      :Print ranges expressed in diff manner. All the diff types handelled.
           -->
          <xsl:variable name ="sheetNamePrntRnge">
            <xsl:value-of select ="substring-before(./self::node(),'!')"/>
          </xsl:variable>          
			<!--<xsl:if test="($sheetNamePrntRnge = concat($apostrof, $checkedName,$apostrof) or $sheetNamePrntRnge = $checkedName) and (@name = '_xlnm.Print_Area' or @name='_xlnm._FilterDatabase' or contains(@name,'Excel_BuiltIn__FilterDatabase'))">-->
          <!--RefNo-3:Replaced $checkedName with $strSheetName(contains the original sheet name.)-->
          <!--<xsl:if test="($sheetNamePrntRnge = concat($apostrof, $checkedName,$apostrof) or $sheetNamePrntRnge = $checkedName) and (@name = '_xlnm.Print_Area')">-->
          <xsl:if test="($sheetNamePrntRnge = concat($apostrof, $strSheetName,$apostrof) or $sheetNamePrntRnge = $strSheetName) and (@name = '_xlnm.Print_Area')">
            <!-- one print range with apostrophes -->
            <xsl:choose>
								<!--Vijayeta,SP2,PrintArea-->
								<!-- multiple print ranges with apostrophes -->
								<!--RefNo-3:Replaced $checkedName with $strSheetName(contains the original sheet name.)-->
								<xsl:when test="contains(./self::node(),concat(',', $apostrof, $strSheetName))">
									<!--<xsl:if test="contains(./self::node(),concat(',', $apostrof, $checkedName))">-->
									<xsl:variable name ="GetPrintRange">
										<xsl:value-of select="./self::node()"/>
									</xsl:variable>
									<xsl:variable name ="newRange">
										<!--Start of RefNo-3: Replace the sheet name with special charecters with checked name.-->
										<xsl:choose>
											<xsl:when test="$strSheetName != $checkedName">
												<xsl:if test="not(contains($strSheetName, '!') or contains($strSheetName, ','))">
													<xsl:variable name="strReplaceSheetName">
														<xsl:call-template name="tmpReplaceSheetName">
															<xsl:with-param name="GetPrintRange" select="$GetPrintRange"/>
															<xsl:with-param name="checkedName" select="concat($apostrof,$checkedName,$apostrof,'.')"/>
															<xsl:with-param name="strBuildPrtRng" select="''"/>
														</xsl:call-template>
													</xsl:variable>
													<xsl:value-of select ="translate($strReplaceSheetName,'$','')"/>
												</xsl:if>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select ="translate(translate(translate($GetPrintRange,'!','.'),'$',''),',',' ')"/>
											</xsl:otherwise>
										</xsl:choose>
										<!--<xsl:value-of select ="translate(translate(translate($GetPrintRange,'!','.'),'$',''),',',' ')"/>-->
										<!--End fo RefNo-3-->
									</xsl:variable>
									<xsl:attribute name="table:print-ranges">
										<xsl:value-of select ="$newRange"/>
									</xsl:attribute>
									<!--</xsl:if>-->
								</xsl:when>
								<!-- multiple print ranges with apostrophes -->
								<!--RefNo-3:Replaced $checkedName with $strSheetName(contains the original sheet name.)-->
								<xsl:when test="contains(./self::node(),concat(',',$strSheetName))">
									<!--<xsl:if test="contains(./self::node(),concat(',', $apostrof, $checkedName))">-->
									<xsl:variable name ="GetPrintRange">
										<xsl:value-of select="./self::node()"/>
									</xsl:variable>
									<xsl:variable name ="newRange">
										<!--Start of RefNo-3: Replace the sheet name with special charecters with checked name.-->
										<xsl:choose>
											<xsl:when test="$strSheetName != $checkedName">
												<xsl:if test="not(contains($strSheetName, '!') or contains($strSheetName, ','))">
													<xsl:variable name="strReplaceSheetName">
														<xsl:call-template name="tmpReplaceSheetName">
															<xsl:with-param name="GetPrintRange" select="$GetPrintRange"/>
															<xsl:with-param name="checkedName" select="concat($apostrof,$checkedName,$apostrof,'.')"/>
															<xsl:with-param name="strBuildPrtRng" select="''"/>
														</xsl:call-template>
													</xsl:variable>
													<xsl:value-of select ="translate($strReplaceSheetName,'$','')"/>
												</xsl:if>
											</xsl:when>
											<xsl:otherwise>
												<xsl:variable name ="correctRange">
													<xsl:call-template name ="getCorrectRange">
														<xsl:with-param  name ="PrintRange" select="$GetPrintRange"/>
													</xsl:call-template>
												</xsl:variable>
												<xsl:value-of select ="$correctRange"/>
												<!--<xsl:value-of select ="translate(translate(translate($GetPrintRange,'!','.'),'$',''),',',' ')"/>-->
											</xsl:otherwise>
										</xsl:choose>
										<!--<xsl:value-of select ="translate(translate(translate($GetPrintRange,'!','.'),'$',''),',',' ')"/>-->
										<!--End fo RefNo-3-->
									</xsl:variable>
									<xsl:attribute name="table:print-ranges">
										<xsl:value-of select ="$newRange"/>
									</xsl:attribute>
									<!--</xsl:if>-->
								</xsl:when>
              <!--RefNo-3:Replaced $checkedName with $strSheetName(contains the original sheet name.)-->
              <!--<xsl:when test="not(contains(./self::node(),concat(',', $apostrof, $checkedName)))">-->
								<xsl:when test="not(contains(./self::node(),concat(',', $apostrof, $strSheetName))) or not(contains(./self::node(),concat(',', $strSheetName)))">
                <!--<xsl:if test="not(contains(./self::node(),concat(',', $apostrof, $checkedName)))">-->
				<xsl:variable name ="prntRnge">
					<xsl:call-template name="recursive">
					  <xsl:with-param name="oldString" select="':'"/>
					  <xsl:with-param name="newString" select="concat(':', $apostrof, $checkedName, $apostrof, '.')"/>
                     <!--RefNo-3--> 
                     <!--<xsl:with-param name="wholeText" select="translate(./self::node(), '!', '.')"/>-->
                      <xsl:with-param name="wholeText" select="concat($apostrof, $checkedName, $apostrof,'.',substring-after(./self::node(), '!'))"/>                                            
					</xsl:call-template>
				  </xsl:variable>
				<xsl:variable name ="PartOne">
					<xsl:value-of select ="substring-before($prntRnge,':')"/>
				  </xsl:variable>
				<xsl:variable name ="PartTwo">
					<xsl:value-of select ="substring-after($prntRnge,':')"/>
				  </xsl:variable>
				<xsl:variable name ="beginSheet">
					<xsl:value-of select ="substring-before($PartOne,'.')"/>
				</xsl:variable>
				<xsl:variable name ="endSheet">
								<xsl:value-of select ="substring-before($PartTwo,'.')"/>
				</xsl:variable>
				<xsl:variable name ="beginColOrRow">
					<xsl:choose>
						<xsl:when test ="contains(substring-after($PartOne,'$'),'$')">
							<xsl:value-of select ="'RCpresent'"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:variable name ="character">
								<xsl:value-of select ="substring-after($PartOne,'$')"/>
							</xsl:variable>
							<xsl:choose>
								<!--<xsl:when test="contains('ABCDEFGHIJKLMNOPQRSTUVWXYZ',$character)">
								  <xsl:value-of select ="'true'"/>
							  </xsl:when>-->
								<xsl:when test="contains('A',$character) or contains('B',$character) or contains('C',$character) or contains('D',$character) or contains('E',$character) or contains('F',$character) or contains('G',$character) 
							  or contains('H',$character) or contains('I',$character) or contains('J',$character) or contains('K',$character) or contains('L',$character) or contains('M',$character) or contains('N',$character) or contains('O',$character) 
            				  or contains('P',$character) or contains('Q',$character) or contains('R',$character) or contains('S',$character) or contains('T',$character) or contains('U',$character) or contains('V',$character) or contains('W',$character) 
							  or contains('X',$character) or contains('Y',$character) or contains('Z',$character) or contains('a',$character) or contains('b',$character) or contains('c',$character) or contains('d',$character) or contains('e',$character)
							  or contains('f',$character) or contains('g',$character) or contains('h',$character) or contains('i',$character) or contains('j',$character) or contains('k',$character) or contains('l',$character) or contains('m',$character)
							  or contains('n',$character) or contains('o',$character) or contains('p',$character) or contains('q',$character) or contains('r',$character) or contains('s',$character) or contains('t',$character) or contains('u',$character)
							  or contains('v',$character) or contains('w',$character) or contains('x',$character) or contains('y',$character) or contains('z',$character)">
									<xsl:value-of select ="'true'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select ="'false'"/>
								</xsl:otherwise>
							</xsl:choose>
							<!--<xsl:value-of select ="substring-after($PartOne,'$')"/>-->
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="endColOrRow">
					<xsl:choose >
						<xsl:when test ="contains(substring-after($PartTwo,'$'),'$')">
							<xsl:value-of select ="'RCpresent'"/>
						</xsl:when>
						<xsl:otherwise >
							<xsl:variable name ="character">
								<xsl:value-of select ="substring-after($PartTwo,'$')"/>
							</xsl:variable>
							<xsl:choose>
								<xsl:when test="contains('A',$character) or contains('B',$character) or contains('C',$character) or contains('D',$character) or contains('E',$character) or contains('F',$character) or contains('G',$character) 
							  or contains('H',$character) or contains('I',$character) or contains('J',$character) or contains('K',$character) or contains('L',$character) or contains('M',$character) or contains('N',$character) or contains('O',$character) 
            				  or contains('P',$character) or contains('Q',$character) or contains('R',$character) or contains('S',$character) or contains('T',$character) or contains('U',$character) or contains('V',$character) or contains('W',$character) 
							  or contains('X',$character) or contains('Y',$character) or contains('Z',$character) or contains('a',$character) or contains('b',$character) or contains('c',$character) or contains('d',$character) or contains('e',$character)
							  or contains('f',$character) or contains('g',$character) or contains('h',$character) or contains('i',$character) or contains('j',$character) or contains('k',$character) or contains('l',$character) or contains('m',$character)
							  or contains('n',$character) or contains('o',$character) or contains('p',$character) or contains('q',$character) or contains('r',$character) or contains('s',$character) or contains('t',$character) or contains('u',$character)
							  or contains('v',$character) or contains('w',$character) or contains('x',$character) or contains('y',$character) or contains('z',$character)">
									<xsl:value-of select ="'true'"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select ="'false'"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="beginCol">
					<xsl:choose>
						<xsl:when test ="$beginColOrRow='RCpresent'">
							<xsl:value-of select ="substring-before(substring-after($PartOne,'$'),'$')"/>
						</xsl:when>
						<xsl:when test ="$beginColOrRow='true'">
							<xsl:value-of select ="substring-after($PartOne,'$')"/>
						</xsl:when>
						<xsl:when test ="$beginColOrRow='false'">
							<xsl:value-of select ="'A'"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="endCol">
					<xsl:choose>
						<xsl:when test ="$endColOrRow='RCpresent'">
							<xsl:value-of select ="substring-before(substring-after($PartTwo,'$'),'$')"/>
						</xsl:when>
						<xsl:when test ="$endColOrRow='true'">
							<xsl:value-of select ="substring-after($PartTwo,'$')"/>
						</xsl:when>
						<xsl:when test ="$endColOrRow='false'">
							<xsl:value-of select ="'IV'"/>
						</xsl:when>
					</xsl:choose>
				</xsl:variable>
				<xsl:variable name ="beginRow">
					<xsl:choose>
						<xsl:when test ="$beginColOrRow='RCpresent'">
							<xsl:value-of select ="substring-after(substring-after($PartOne,'$'),'$')"/>
						</xsl:when>
						<xsl:when test ="$beginColOrRow='true'">
							<xsl:value-of select ="1"/>
						</xsl:when>
						<xsl:when test ="$beginColOrRow='false'">
							<xsl:value-of select ="substring-after($PartOne,'$')"/>
						</xsl:when>
					</xsl:choose>					
				</xsl:variable>
				<xsl:variable name ="endRow">
					<xsl:choose>
						<xsl:when test ="$endColOrRow='RCpresent'">
							<xsl:value-of select ="substring-after(substring-after($PartTwo,'$'),'$')"/>
						</xsl:when>
						<xsl:when test ="$endColOrRow='true'">
							<xsl:value-of select ="65536"/>
						</xsl:when>
						<xsl:when test ="$endColOrRow='false'">
							<xsl:value-of select ="substring-after($PartTwo,'$')"/>
						</xsl:when>
					</xsl:choose>					
				</xsl:variable>
				<xsl:variable name ="newPrntRnge">
					<xsl:if test ="$endSheet!=''">
						<xsl:value-of select ="concat($beginSheet,'.',$beginCol,$beginRow,':',$endSheet,'.',$endCol,$endRow)"/>
					</xsl:if>
					<xsl:if test ="$endSheet=''">
						<xsl:value-of select ="concat($beginSheet,'.',$beginCol,$beginRow,':',$beginSheet,'.',$endCol,$endRow)"/>
					</xsl:if>
				</xsl:variable>
				<xsl:attribute name="table:print-ranges">
					<xsl:value-of select ="$newPrntRnge"/>
				</xsl:attribute>
                <!--</xsl:if>-->
              </xsl:when>
								<!--Vijayeta,SP2,PrintArea,End-->
            </xsl:choose>
            <!-- End of fix for Bug 1803593, for the file U S Extreme Temperature, on round trip-->
          </xsl:if>
        </xsl:if>
      </xsl:for-each>
		</xsl:for-each>
      
      <xsl:variable name="GroupRowStart">
        <xsl:call-template name="GroupRow">
          <xsl:with-param name="Id">
            <xsl:value-of select="$Id"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="GroupRowEnd">
        <xsl:call-template name="GroupRowEnd">
          <xsl:with-param name="Id">
            <xsl:value-of select="$Id"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:apply-templates
        select="key('Part', 'xl/workbook.xml')/e:workbook/e:definedNames/e:definedName[1]"
        mode="PrintArea">
        <xsl:with-param name="name">
          <xsl:value-of select="@name"/>
        </xsl:with-param>
        <xsl:with-param name="checkedName">
          <xsl:value-of select="$checkedName"/>
        </xsl:with-param>
      </xsl:apply-templates>

      <xsl:call-template name="InsertSheetContent">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$Id"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeCell">
          <xsl:value-of select="$BigMergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeRow">
          <xsl:value-of select="$BigMergeRow"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="NameSheet">
          <xsl:value-of select="@name"/>
        </xsl:with-param>
        <xsl:with-param name="sheetNr">
          <xsl:value-of select="$number"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCell">
          <xsl:value-of select="$ConditionalCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteCell">
          <xsl:value-of select="$NoteCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteRow">
          <xsl:value-of select="$NoteRow"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellStyle">
          <xsl:value-of select="$ConditionalCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalRow">
          <xsl:value-of select="$ConditionalRow"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellCol">
          <xsl:value-of select="$ConditionalCellCol"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellAll">
          <xsl:value-of select="$ConditionalCellAll"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellSingle">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellMultiple">
          <xsl:value-of select="$ConditionalCellMultiple"/>
        </xsl:with-param>
        <xsl:with-param name="PictureCell">
          <xsl:value-of select="$PictureCell"/>
        </xsl:with-param>
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$PictureRow"/>
        </xsl:with-param>
        <xsl:with-param name="removeFilter" select="$removeFilter"/>
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationRow">
          <xsl:value-of select="$ValidationRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCellStyle">
          <xsl:value-of select="$ValidationCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="ConnectionsCell">
          <xsl:value-of select="$ConnectionCell"/>
        </xsl:with-param>
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupCell"/>
        </xsl:with-param>
        <xsl:with-param name="GroupRowStart">
          <xsl:value-of select="$GroupRowStart"/>
        </xsl:with-param>
        <xsl:with-param name="GroupRowEnd">
          <xsl:value-of select="$GroupRowEnd"/>
        </xsl:with-param>
        <xsl:with-param name="AllRowBreakes">
          <xsl:value-of select="$AllRowBreakes"/>
        </xsl:with-param>
        <xsl:with-param name="rSheredStrings">
          <xsl:value-of select="$rSheredStrings"/>
        </xsl:with-param>
      </xsl:call-template>

    </table:table>

    <xsl:for-each select="key('Part', concat('xl/',$Id))/e:worksheet/e:scenarios">
      <xsl:call-template name="Scenarios"/>
    </xsl:for-each>

    <xsl:if test="key('Part', concat('xl/',$Id))/e:worksheet/e:dataConsolidate">
      <xsl:message terminate="no">translation.oox2odf.DataConsolidation</xsl:message>
    </xsl:if>

  </xsl:template>

  <!--Start of RefNo-3: Template to replace old sheet name with the new one.-->
  <xsl:template name="tmpReplaceSheetName">
    <xsl:param name="GetPrintRange"/>
    <xsl:param name="checkedName"/>
    <xsl:param name="strBuildPrtRng"/>

    <xsl:choose>
      <xsl:when test="$GetPrintRange != ''">
        <xsl:choose>
          <xsl:when test="contains($GetPrintRange,',')">
            <xsl:variable name="strPrntRange" select="substring-before($GetPrintRange,',')"/>            
            <xsl:call-template name="tmpReplaceSheetName">
              <xsl:with-param name="GetPrintRange" select="substring-after($GetPrintRange,',')"/>
              <xsl:with-param name="checkedName" select="$checkedName"/>
              <xsl:with-param name="strBuildPrtRng">
                <xsl:choose>
                  <xsl:when test="$strBuildPrtRng = ''">
                    <xsl:value-of select="concat($checkedName, substring-after($strPrntRange,'!'))"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="concat($strBuildPrtRng,' ',$checkedName, substring-after($strPrntRange,'!'))"/>                    
                  </xsl:otherwise>                    
                </xsl:choose>                        
              </xsl:with-param> 
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="concat($strBuildPrtRng,' ',$checkedName, substring-after($GetPrintRange,'!'))"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$strBuildPrtRng"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  <!--End of RefNo-3-->

  <xsl:template match="e:definedName" mode="PrintArea">
    <xsl:param name="name"/>
    <xsl:param name="checkedName"/>

    <xsl:variable name="value">
      <xsl:value-of select="."/>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="contains($value, $name) and @name = '_xlnm.Print_Area' ">

        <!--         
        <xsl:variable name="apos">
          <xsl:text>&apos;</xsl:text>
        </xsl:variable>
        
        <xsl:variable name="sheetName">
          <xsl:choose>
            <xsl:when test="starts-with(text(),$apos)">
              <xsl:value-of select="$apos"/>
              <xsl:value-of select="substring-before(substring-after(text(),$apos),$apos)"/>
              <xsl:value-of select="$apos"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="substring-before(text(),'!')"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
     
        <xsl:variable name="printArea">
          <xsl:value-of select="$sheetName"/>
          <xsl:value-of select="substring-after(text(),$sheetName)"/>
        </xsl:variable>
-->

        <xsl:call-template name="InsertRanges">
          <xsl:with-param name="ranges" select="text()"/>
          <xsl:with-param name="mode" select="substring-after(text(),',')"/>
          <xsl:with-param name="checkedName" select="$checkedName"/>
        </xsl:call-template>
      </xsl:when>

      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="following-sibling::e:definedName">
            <xsl:apply-templates select="following-sibling::e:definedName[1]" mode="PrintArea">
              <xsl:with-param name="name">
                <xsl:value-of select="$name"/>
              </xsl:with-param>
              <xsl:with-param name="checkedName">
                <xsl:value-of select="$checkedName"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertRanges">
    <xsl:param name="ranges"/>
    <xsl:param name="mode"/>
    <xsl:param name="checkedName"/>
    <!-- if print ranges attribute does not contain '#REF' -->
    <xsl:for-each select="key('Part', 'xl/workbook.xml')">
      <xsl:if
        test="not(contains(//workbook/definedNames/definedName[attribute::name = '_xlnm.Print_Area' ],'#REF'))">
        <xsl:variable name="apos">
          <xsl:text>&apos;</xsl:text>
        </xsl:variable>
        <!-- take sheet name from <definedName> (can be inside apostrophes and can be distinct from $checkedName) 
           it is needed for <definedName> processing -->
        <xsl:variable name="sheetName">
          <xsl:choose>
            <xsl:when test="starts-with($ranges,$apos)">
              <xsl:value-of select="$apos"/>
              <xsl:value-of select="substring-before(substring-after($ranges,$apos),$apos)"/>
              <xsl:value-of select="$apos"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="substring-before($ranges,'!')"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:choose>
          <!-- when there are more than one range -->
          <xsl:when test="$mode != '' ">

            <!-- single-cell range can be defined either as Sheet1!$A$2:$A$2 or as Sheet1!$A$2-->
            <xsl:variable name="startRange">
              <xsl:choose>
                <xsl:when
                  test="contains(substring-before(substring-after($ranges, concat($sheetName,'!') ),','), ':' )">
                  <xsl:value-of
                    select="substring-before(substring-after($ranges,concat($sheetName,'!')),':' )"
                  />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of
                    select="substring-before(substring-after($ranges,concat($sheetName,'!')),',' )"
                  />
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <!-- single-cell range can be defined either as Sheet1!$A$2:$A$2 or as Sheet1!$A$2-->
            <xsl:variable name="endRange">
              <xsl:choose>
                <xsl:when
                  test="contains(substring-before(substring-after($ranges, concat($sheetName,'!') ),','), ':' )">
                  <xsl:value-of select="substring-before(substring-after($ranges,':'),',' )"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of
                    select="substring-before(substring-after($ranges,concat($sheetName,'!')),',' )"
                  />
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="start">
              <xsl:choose>
                <!-- when print range is defined for whole rows -->
                <xsl:when test="number(translate($startRange,'$',''))">
                  <xsl:value-of select="concat('A',translate($startRange,'$',''))"/>
                </xsl:when>
                <!-- when print range is defined for whole columns -->
                <xsl:when test="$startRange = translate($startRange,'1234567890','')">
                  <xsl:value-of select="concat(translate($startRange,'$',''),'1')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="translate($startRange,'$','')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="end">
              <xsl:choose>
                <!-- when print range is defined for whole rows -->
                <xsl:when test="number(translate($endRange,'$',''))">
                  <xsl:value-of select="concat('IV',translate($endRange,'$',''))"/>
                </xsl:when>
                <!-- when print range is defined for whole columns -->
                <xsl:when test="$endRange = translate($endRange,'1234567890','')">
                  <xsl:value-of select="concat(translate($endRange,'$',''),'65536')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="translate($endRange,'$','')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <!--  
           <xsl:value-of
              select="concat($apos,$checkedName,$apos,'.',$start,':',$apos,$checkedName,$apos,'.',$end)"/>
            <xsl:text> </xsl:text>
-->
            <xsl:call-template name="InsertRanges">
              <xsl:with-param name="ranges" select="substring-after($ranges,',')"/>
              <xsl:with-param name="mode" select="substring-after(substring-after($ranges,','),',')"/>
              <xsl:with-param name="checkedName" select="$checkedName"/>
            </xsl:call-template>
          </xsl:when>

          <!-- this is the last range -->
          <xsl:otherwise>
            <!-- single-cell range can be defined either as Sheet1!$A$2:$A$2 or as Sheet1!$A$2-->
            <xsl:variable name="startRange">
              <xsl:choose>
                <xsl:when test="contains(substring-after($ranges, concat($sheetName,'!') ), ':' )">
                  <xsl:value-of
                    select="substring-before(substring-after($ranges,concat($sheetName,'!')),':' )"
                  />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after($ranges,concat($sheetName,'!'))"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <!-- single-cell range can be defined either as Sheet1!$A$2:$A$2 or as Sheet1!$A$2-->
            <xsl:variable name="endRange">
              <xsl:choose>
                <xsl:when test="contains(substring-after($ranges, concat($sheetName,'!') ), ':' )">
                  <xsl:value-of select="substring-after($ranges,':')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after($ranges,concat($sheetName,'!'))"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="start">
              <xsl:choose>
                <!-- when print range is defined for whole rows -->
                <xsl:when test="number(translate($startRange,'$',''))">
                  <xsl:value-of select="concat('A',translate($startRange,'$',''))"/>
                </xsl:when>
                <!-- when print range is defined for whole columns -->
                <xsl:when test="$startRange = translate($startRange,'1234567890','')">
                  <xsl:value-of select="concat(translate($startRange,'$',''),'1')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="translate($startRange,'$','')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <xsl:variable name="end">
              <xsl:choose>
                <!-- when print range is defined for whole rows -->
                <xsl:when test="number(translate($endRange,'$',''))">
                  <xsl:value-of select="concat('IV',translate($endRange,'$',''))"/>
                </xsl:when>
                <!-- when print range is defined for whole columns -->
                <xsl:when test="$endRange = translate($endRange,'1234567890','')">
                  <xsl:value-of select="concat(translate($endRange,'$',''),'65536')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="translate($endRange,'$','')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>

            <!--          <xsl:value-of
              select="concat($apos,$checkedName,$apos,'.',$start,':',$apos,$checkedName,$apos,'.',$end)"/>
-->
          </xsl:otherwise>
        </xsl:choose>

      </xsl:if>
    </xsl:for-each>
    <!-- if print ranges attribute contains '#REF' then there should be 'table:print' attribute put instead with 'false' value -->
    <xsl:for-each select="key('Part', 'xl/workbook.xml')">
      <xsl:if
        test="contains(//workbook/definedNames/definedName[attribute::name = '_xlnm.Print_Area' ],'#REF')">
        <xsl:attribute name="table:print">
          <xsl:value-of select="'false'"/>
        </xsl:attribute>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>


  <!-- insert string -->
  <xsl:template name="e:si">
    <xsl:if test="not(e:r)">
      <pxsi:si pxsi:number="{position() - 1}">
        <xsl:apply-templates/>
      </pxsi:si>
    </xsl:if>

  </xsl:template>

  <xsl:template name="InsertSheetContent">
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="BigMergeRow"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalRow"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="removeFilter"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="ConnectionsCell"/>
    <xsl:param name="GroupCell"/>
    <xsl:param name="GroupRowStart"/>
    <xsl:param name="GroupRowEnd"/>
    <xsl:param name="AllRowBreakes"/>
    <xsl:param name="rSheredStrings"/>

    
	  <!-- Perofomance-->
	  <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet">
		  <xsl:for-each select="e:oleObjects">
      <xsl:call-template name="InsertOLEObjects"/>
    </xsl:for-each>
	  </xsl:for-each>

	   <!-- Perofomance-->
    <xsl:variable name="ManualColBreaks">
		  <xsl:for-each select="key('Part', concat('xl/',$sheet))/e:worksheet">
			  <xsl:for-each select="e:colBreaks/e:brk">
        <xsl:value-of select="concat(@id,';')"/>
      </xsl:for-each>
		  </xsl:for-each>
    </xsl:variable>


    <xsl:call-template name="InsertColumns">
      <xsl:with-param name="sheet" select="$sheet"/>
      <xsl:with-param name="GroupCell">
        <xsl:value-of select="$GroupCell"/>
      </xsl:with-param>
      <xsl:with-param name="ManualColBreaks">
        <xsl:value-of select="$ManualColBreaks"/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:variable name="sheetName">
      <xsl:value-of select="@name"/>
    </xsl:variable>

    <xsl:for-each select="key('Part', concat('xl/',$sheet))">

      <xsl:variable name="apos">
        <xsl:text>&apos;</xsl:text>
      </xsl:variable>

      <xsl:variable name="headerRowsStart">
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
                    select="substring-before(substring-after(substring-after(substring-after(text(),','),$apos),concat($apos,'!$')),':')"
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
                    select="substring-before(substring-after(substring-after(text(),','),'$'),':')"
                  />
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-before(substring-after(text(),'$'),':')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </xsl:when>
        </xsl:choose>

      </xsl:variable>

      <xsl:variable name="headerRowsEnd">
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
                    select="substring-after(substring-after(substring-after(text(),','),':'),'$')"/>
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
                    select="substring-after(substring-after(substring-after(text(),','),':'),'$')"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="substring-after(substring-after(text(),':'),'$')"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:for-each>
          </xsl:when>
        </xsl:choose>
      </xsl:variable>

      <!-- Insert Row  -->
      <xsl:choose>

        <!-- if there are header rows -->
        <xsl:when test="$headerRowsStart != '' and number($headerRowsStart)">

          <!-- insert rows before header rows -->
          <xsl:apply-templates
            select="e:worksheet/e:sheetData/e:row[@r &lt; $headerRowsStart and @r &lt; 65537]"
            mode="headers">
            <xsl:with-param name="BigMergeCell">
              <xsl:value-of select="$BigMergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="BigMergeRow">
              <xsl:value-of select="$BigMergeRow"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="headerRowsStart" select="$headerRowsStart"/>
            <xsl:with-param name="headerRowsEnd" select="$headerRowsEnd"/>
            <xsl:with-param name="sheetNr" select="$sheetNr"/>
            <xsl:with-param name="removeFilter" select="$removeFilter"/>
            <xsl:with-param name="rSheredStrings">
              <xsl:value-of select="$rSheredStrings"/>
            </xsl:with-param>
          </xsl:apply-templates>

          <!-- insert empty rows before header -->
          <xsl:if
            test="$headerRowsStart &gt; 1 and not(e:worksheet/e:sheetData/e:row[@r = $headerRowsStart - 1 and @r &lt; 65537])">
            <xsl:choose>
              <!-- when there aren't any rows before at all -->
              <xsl:when
                test="not(e:worksheet/e:sheetData/e:row[@r &lt; $headerRowsStart and @r &lt; 65537])">
                <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-rows-repeated="{$headerRowsStart - 1}">
                  <table:table-cell table:number-columns-repeated="256"/>
                </table:table-row>
              </xsl:when>
              <!-- if there was a row before header -->
              <xsl:otherwise>
                <xsl:for-each
                  select="e:worksheet/e:sheetData/e:row[@r &lt; $headerRowsStart and @r &lt; 65537][last()]">
                  <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                    table:number-rows-repeated="{$headerRowsStart - @r - 1}">
                    <table:table-cell table:number-columns-repeated="256"/>
                  </table:table-row>
                </xsl:for-each>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>

          <!-- insert header rows -->
          <table:table-header-rows>
            <xsl:apply-templates
              select="e:worksheet/e:sheetData/e:row[@r &gt;= $headerRowsStart and @r &lt;= $headerRowsEnd and @r &lt; 65537]"
              mode="headers">
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of select="$BigMergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeRow">
                <xsl:value-of select="$BigMergeRow"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="$MergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="headerRowsStart" select="$headerRowsStart"/>
              <xsl:with-param name="headerRowsEnd" select="$headerRowsEnd"/>
              <xsl:with-param name="sheetNr" select="$sheetNr"/>
              <xsl:with-param name="removeFilter" select="$removeFilter"/>
              <xsl:with-param name="rSheredStrings">
                <xsl:value-of select="$rSheredStrings"/>
              </xsl:with-param>
            </xsl:apply-templates>

            <!-- if header is empty -->
            <xsl:choose>
              <xsl:when test="not(e:worksheet/e:sheetData/e:row/e:c/e:v) and $BigMergeCell != '' ">
                <xsl:call-template name="InsertEmptySheet">
                  <xsl:with-param name="sheet">
                    <xsl:value-of select="$sheet"/>
                  </xsl:with-param>
                  <xsl:with-param name="BigMergeCell">
                    <xsl:value-of select="$BigMergeCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="BigMergeRow">
                    <xsl:value-of select="$BigMergeRow"/>
                  </xsl:with-param>
                  <xsl:with-param name="MergeCell">
                    <xsl:value-of select="$MergeCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="RowNumber">
                    <xsl:value-of select="e:worksheet/e:sheetData/e:row[position() = last()]/@r"/>
                  </xsl:with-param>
                  <xsl:with-param name="PictureCell">
                    <xsl:value-of select="$PictureCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="PictureRow">
                    <xsl:value-of select="$PictureRow"/>
                  </xsl:with-param>
                  <xsl:with-param name="NoteCell">
                    <xsl:value-of select="$NoteCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="NoteRow">
                    <xsl:value-of select="$NoteRow"/>
                  </xsl:with-param>
                  <xsl:with-param name="NameSheet">
                    <xsl:value-of select="$sheetName"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCell">
                    <xsl:value-of select="$ConditionalCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCellStyle">
                    <xsl:value-of select="$ConditionalCellStyle"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalRow">
                    <xsl:value-of select="$ConditionalRow"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCellCol">
                    <xsl:value-of select="$ConditionalCellCol"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCellAll">
                    <xsl:value-of select="$ConditionalCellAll"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCellSingle">
                    <xsl:value-of select="$ConditionalCellSingle"/>
                  </xsl:with-param>
                  <xsl:with-param name="ConditionalCellMultiple">
                    <xsl:value-of select="$ConditionalCellMultiple"/>
                  </xsl:with-param>
                  <xsl:with-param name="sheetNr" select="$sheetNr"/>
                  <xsl:with-param name="ValidationCell">
                    <xsl:value-of select="$ValidationCell"/>
                  </xsl:with-param>
                  <xsl:with-param name="ValidationRow">
                    <xsl:value-of select="$ValidationRow"/>
                  </xsl:with-param>
                  <xsl:with-param name="ValidationCellStyle">
                    <xsl:value-of select="$ValidationCellStyle"/>
                  </xsl:with-param>
                  <xsl:with-param name="AllRowBreakes">
                    <xsl:value-of select="$AllRowBreakes"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>

              <xsl:when
                test="not(e:worksheet/e:sheetData/e:row[@r &gt;= $headerRowsStart and @r &lt;= $headerRowsEnd and @r &lt; 65537])">
                <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-rows-repeated="{$headerRowsEnd - $headerRowsStart + 1}">
                  <table:table-cell table:number-columns-repeated="256"/>
                </table:table-row>
              </xsl:when>
            </xsl:choose>

            <!-- if there are empty rows at the end of the header -->
            <xsl:for-each
              select="e:worksheet/e:sheetData/e:row[@r &gt;= $headerRowsStart and @r &lt;= $headerRowsEnd and @r &lt; 65537][last()]">
              <xsl:if test="@r &lt; $headerRowsEnd">
                <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
                  table:number-rows-repeated="{$headerRowsEnd - @r}">
                  <table:table-cell table:number-columns-repeated="256"/>
                </table:table-row>
              </xsl:if>
            </xsl:for-each>
          </table:table-header-rows>

          <!-- insert rows after header rows -->
          <xsl:apply-templates
            select="e:worksheet/e:sheetData/e:row[@r &gt; $headerRowsEnd and @r &lt; 65537]"
            mode="headers">
            <xsl:with-param name="BigMergeCell">
              <xsl:value-of select="$BigMergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="BigMergeRow">
              <xsl:value-of select="$BigMergeRow"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <!-- Code Added By Sateesh Reddy Date:01-Feb-2008 -->
            <xsl:with-param name="sheetNr" select="$sheetNr"/>
            <!-- End -->
            <xsl:with-param name="headerRowsStart" select="$headerRowsStart"/>
            <xsl:with-param name="headerRowsEnd" select="$headerRowsEnd"/>
            <xsl:with-param name="removeFilter" select="$removeFilter"/>
            <xsl:with-param name="rSheredStrings">
              <xsl:value-of select="$rSheredStrings"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:when>

        <!-- if there aren't any header rows -->
        <xsl:otherwise>

          <xsl:choose>
            <!--RefNo-5:ODF1.1:Added conditio to chk for rows lessthan 65537 index-->
            <xsl:when test="e:worksheet/e:sheetData/e:row[@r &lt; 65537]">
              <xsl:apply-templates select="e:worksheet/e:sheetData/e:row[@r &lt; 65537]">
                <xsl:with-param name="BigMergeCell">
                  <xsl:value-of select="$BigMergeCell"/>
                </xsl:with-param>
                <xsl:with-param name="BigMergeRow">
                  <xsl:value-of select="$BigMergeRow"/>
                </xsl:with-param>
                <xsl:with-param name="MergeCell">
                  <xsl:value-of select="$MergeCell"/>
                </xsl:with-param>
                <xsl:with-param name="PictureCell">
                  <xsl:value-of select="$PictureCell"/>
                </xsl:with-param>
                <xsl:with-param name="PictureRow">
                  <xsl:value-of select="$PictureRow"/>
                </xsl:with-param>
                <xsl:with-param name="NoteCell">
                  <xsl:value-of select="$NoteCell"/>
                </xsl:with-param>
                <xsl:with-param name="NoteRow">
                  <xsl:value-of select="$NoteRow"/>
                </xsl:with-param>
                <xsl:with-param name="sheet">
                  <xsl:value-of select="$sheet"/>
                </xsl:with-param>
                <xsl:with-param name="NameSheet">
                  <xsl:value-of select="$NameSheet"/>
                </xsl:with-param>
                <xsl:with-param name="sheetNr" select="$sheetNr"/>
                <xsl:with-param name="ConditionalCell">
                  <xsl:value-of select="$ConditionalCell"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalRow">
                  <xsl:value-of select="$ConditionalRow"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellStyle">
                  <xsl:value-of select="$ConditionalCellStyle"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellCol">
                  <xsl:value-of select="$ConditionalCellCol"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellAll">
                  <xsl:value-of select="$ConditionalCellAll"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellSingle">
                  <xsl:value-of select="$ConditionalCellSingle"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellMultiple">
                  <xsl:value-of select="$ConditionalCellMultiple"/>
                </xsl:with-param>
                <xsl:with-param name="removeFilter" select="$removeFilter"/>
                <xsl:with-param name="ValidationCell">
                  <xsl:value-of select="$ValidationCell"/>
                </xsl:with-param>
                <xsl:with-param name="ValidationRow">
                  <xsl:value-of select="$ValidationRow"/>
                </xsl:with-param>
                <xsl:with-param name="ValidationCellStyle">
                  <xsl:value-of select="$ValidationCellStyle"/>
                </xsl:with-param>
                <xsl:with-param name="ConnectionsCell">
                  <xsl:value-of select="$ConnectionsCell"/>
                </xsl:with-param>
                <xsl:with-param name="GroupRowStart">
                  <xsl:value-of select="$GroupRowStart"/>
                </xsl:with-param>
                <xsl:with-param name="GroupRowEnd">
                  <xsl:value-of select="$GroupRowEnd"/>
                </xsl:with-param>
                <xsl:with-param name="AllRowBreakes">
                  <xsl:value-of select="$AllRowBreakes"/>
                </xsl:with-param>
                <xsl:with-param name="rSheredStrings">
                  <xsl:value-of select="$rSheredStrings"/>
                </xsl:with-param>
              </xsl:apply-templates>
            </xsl:when>

            <xsl:otherwise>
              <!-- Insert sheet without text -->
              <xsl:call-template name="InsertEmptySheet">
                <xsl:with-param name="sheet">
                  <xsl:value-of select="$sheet"/>
                </xsl:with-param>
                <xsl:with-param name="BigMergeCell">
                  <xsl:value-of select="$BigMergeCell"/>
                </xsl:with-param>
                <xsl:with-param name="BigMergeRow">
                  <xsl:value-of select="$BigMergeRow"/>
                </xsl:with-param>
                <xsl:with-param name="MergeCell">
                  <xsl:value-of select="$MergeCell"/>
                </xsl:with-param>
                <xsl:with-param name="RowNumber">
                  <xsl:value-of select="e:worksheet/e:sheetData/e:row[position() = last()]/@r"/>
                </xsl:with-param>
                <xsl:with-param name="PictureCell">
                  <xsl:value-of select="$PictureCell"/>
                </xsl:with-param>
                <xsl:with-param name="PictureRow">
                  <xsl:value-of select="$PictureRow"/>
                </xsl:with-param>
                <xsl:with-param name="NoteCell">
                  <xsl:value-of select="$NoteCell"/>
                </xsl:with-param>
                <xsl:with-param name="NoteRow">
                  <xsl:value-of select="$NoteRow"/>
                </xsl:with-param>
                <xsl:with-param name="NameSheet">
                  <xsl:value-of select="$sheetName"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCell">
                  <xsl:value-of select="$ConditionalCell"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellStyle">
                  <xsl:value-of select="$ConditionalCellStyle"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalRow">
                  <xsl:value-of select="$ConditionalRow"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellCol">
                  <xsl:value-of select="$ConditionalCellCol"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellAll">
                  <xsl:value-of select="$ConditionalCellAll"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellSingle">
                  <xsl:value-of select="$ConditionalCellSingle"/>
                </xsl:with-param>
                <xsl:with-param name="ConditionalCellMultiple">
                  <xsl:value-of select="$ConditionalCellMultiple"/>
                </xsl:with-param>
                <xsl:with-param name="sheetNr" select="$sheetNr"/>
                <xsl:with-param name="ValidationCell">
                  <xsl:value-of select="$ValidationCell"/>
                </xsl:with-param>
                <xsl:with-param name="ValidationRow">
                  <xsl:value-of select="$ValidationRow"/>
                </xsl:with-param>
                <xsl:with-param name="ValidationCellStyle">
                  <xsl:value-of select="$ValidationCellStyle"/>
                </xsl:with-param>
                <xsl:with-param name="AllRowBreakes">
                  <xsl:value-of select="$AllRowBreakes"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:otherwise>
          </xsl:choose>

        </xsl:otherwise>
      </xsl:choose>

      <!-- OpenOffice calc supports only 65536 rows -->
      <xsl:if test="e:worksheet/e:sheetData/e:row[@r &gt; 65536]">
        <xsl:message terminate="no">translation.oox2odf.RowNumber</xsl:message>
      </xsl:if>

    </xsl:for-each>

  </xsl:template>

  <xsl:template match="e:row">
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="BigMergeRow"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalRow"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="removeFilter"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="ConnectionsCell"/>
    <xsl:param name="outlineLevel"/>
    <xsl:param name="GroupRowStart"/>
    <xsl:param name="GroupRowEnd"/>
    <xsl:param name="AllRowBreakes"/>
    <xsl:param name="rSheredStrings"/>

    <xsl:variable name="this" select="."/>

    <xsl:variable name="lastCellColumnNumber">
      <xsl:choose>
        <xsl:when test="e:c[last()]/@r">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell">
              <xsl:value-of select="e:c[last()]/@r"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="CheckIfBigMerge">
      <xsl:call-template name="CheckIfBigMergeRow">
        <xsl:with-param name="RowNum">
          <xsl:value-of select="@r"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeCellRow">
          <xsl:value-of select="$BigMergeRow"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="GetMinRowWithPicture">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="concat($PictureRow, $NoteRow)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="GetMinRowWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of
            select="concat($PictureRow, $NoteRow, $ConditionalRow, $ValidationRow, $AllRowBreakes)"
          />
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:choose>
            <xsl:when test="preceding-sibling::e:row[1]/@r = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="preceding-sibling::e:row[1]/@r"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!-- if there were empty rows before this one then insert empty rows -->
    <xsl:choose>
      <!-- when this row is the first non-empty one but not row 1 and there aren't Big Merge Coll and Pictures-->
      <xsl:when
        test="position()=1 and @r>1 and $BigMergeCell = '' and ($GetMinRowWithElement = '' or ($GetMinRowWithElement) &gt;= @r)">

        <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
          table:number-rows-repeated="{@r - 1}">
          <table:table-cell table:number-columns-repeated="256"/>
        </table:table-row>

      </xsl:when>

      <!-- when this row is the first non-empty one but not row 1 and there aren't Big Merge Coll, and there are Pictures before this row-->
      <xsl:when
        test="position()=1 and @r>1 and $BigMergeCell = '' and $GetMinRowWithElement &lt; @r">

        <xsl:call-template name="InsertElementsBetwenTwoRows">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$PictureRow"/>
          </xsl:with-param>
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
          <xsl:with-param name="NoteRow">
            <xsl:value-of select="$NoteRow"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="$ConditionalCell"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellStyle">
            <xsl:value-of select="$ConditionalCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalRow">
            <xsl:value-of select="$ConditionalRow"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellCol">
            <xsl:value-of select="$ConditionalCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellAll">
            <xsl:value-of select="$ConditionalCellAll"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellSingle">
            <xsl:value-of select="$ConditionalCellSingle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellMultiple">
            <xsl:value-of select="$ConditionalCellMultiple"/>
          </xsl:with-param>
          <xsl:with-param name="sheetNr" select="$sheetNr"/>
          <xsl:with-param name="EndRow">
            <xsl:value-of select="@r - 1"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationCell">
            <xsl:value-of select="$ValidationCell"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationRow">
            <xsl:value-of select="$ValidationRow"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationCellStyle">
            <xsl:value-of select="$ValidationCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="AllRowBreakes">
            <xsl:value-of select="$AllRowBreakes"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>



      <!-- when this row is the first non-empty one but not row 1 and there aren't Big Merge Coll-->

      <xsl:when test="position()=1 and @r>1 and $BigMergeCell != ''">

        <xsl:call-template name="InsertBigMergeFirstRowEmpty">
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="1"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
        </xsl:call-template>

        <xsl:if test="@r - 2 &gt; 0"/>
        <xsl:call-template name="InsertBigMergeRowEmpty">
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="2"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="Repeat">
            <xsl:value-of select="@r - 2"/>
          </xsl:with-param>

        </xsl:call-template>

      </xsl:when>

      <!-- when this row is not first one and there were pictures rows after previous non-empty row-->
      <xsl:when
        test="preceding-sibling::e:row[1]/@r &lt;  @r - 1 and $GetMinRowWithElement &gt; preceding-sibling::e:row[1]/@r and $GetMinRowWithElement &lt; @r ">

        <xsl:call-template name="InsertElementsBetwenTwoRows">
          <xsl:with-param name="sheet">
            <xsl:value-of select="$sheet"/>
          </xsl:with-param>
          <xsl:with-param name="NameSheet">
            <xsl:value-of select="$NameSheet"/>
          </xsl:with-param>
          <xsl:with-param name="PictureCell">
            <xsl:value-of select="$PictureCell"/>
          </xsl:with-param>
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="$PictureRow"/>
          </xsl:with-param>
          <xsl:with-param name="NoteCell">
            <xsl:value-of select="$NoteCell"/>
          </xsl:with-param>
          <xsl:with-param name="NoteRow">
            <xsl:value-of select="$NoteRow"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCell">
            <xsl:value-of select="$ConditionalCell"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellStyle">
            <xsl:value-of select="$ConditionalCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalRow">
            <xsl:value-of select="$ConditionalRow"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellCol">
            <xsl:value-of select="$ConditionalCellCol"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellAll">
            <xsl:value-of select="$ConditionalCellAll"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellSingle">
            <xsl:value-of select="$ConditionalCellSingle"/>
          </xsl:with-param>
          <xsl:with-param name="ConditionalCellMultiple">
            <xsl:value-of select="$ConditionalCellMultiple"/>
          </xsl:with-param>
          <xsl:with-param name="sheetNr" select="$sheetNr"/>
          <xsl:with-param name="EndRow">
            <xsl:value-of select="@r - 1"/>
          </xsl:with-param>
          <xsl:with-param name="prevRow">
            <xsl:value-of select="preceding-sibling::e:row[1]/@r"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationCell">
            <xsl:value-of select="$ValidationCell"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationRow">
            <xsl:value-of select="$ValidationRow"/>
          </xsl:with-param>
          <xsl:with-param name="ValidationCellStyle">
            <xsl:value-of select="$ValidationCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="AllRowBreakes">
            <xsl:value-of select="$AllRowBreakes"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:when>

      <xsl:otherwise>

        <!-- when this row is not first one and there were empty rows after previous non-empty row -->
        <xsl:if test="preceding-sibling::e:row[1]/@r &lt;  @r - 1">
          <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
            table:number-rows-repeated="{@r -1 - preceding-sibling::e:row[1]/@r}">
            <table:table-cell table:number-columns-repeated="256"/>
          </table:table-row>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:if test="contains(concat(':', $GroupRowStart), concat(':', @r, ':'))">
      <xsl:call-template name="InsertRowGroupStart">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupRowStart"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <!-- insert this row -->
    <xsl:call-template name="InsertThisRow">
      <xsl:with-param name="BigMergeCell">
        <xsl:value-of select="$BigMergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="BigMergeRow">
        <xsl:value-of select="$BigMergeRow"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="lastCellColumnNumber">
        <xsl:value-of select="$lastCellColumnNumber"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfBigMerge">
        <xsl:value-of select="$CheckIfBigMerge"/>
      </xsl:with-param>
      <xsl:with-param name="this" select="$this"/>
      <xsl:with-param name="PictureRow">
        <xsl:value-of select="$PictureRow"/>
      </xsl:with-param>
      <xsl:with-param name="PictureCell">
        <xsl:value-of select="$PictureCell"/>
      </xsl:with-param>
      <xsl:with-param name="NoteRow">
        <xsl:value-of select="$NoteRow"/>
      </xsl:with-param>
      <xsl:with-param name="NoteCell">
        <xsl:value-of select="$NoteCell"/>
      </xsl:with-param>
      <xsl:with-param name="sheet">
        <xsl:value-of select="$sheet"/>
      </xsl:with-param>
      <xsl:with-param name="NameSheet">
        <xsl:value-of select="$NameSheet"/>
      </xsl:with-param>
      <xsl:with-param name="sheetNr" select="$sheetNr"/>
      <xsl:with-param name="ConditionalCell">
        <xsl:value-of select="$ConditionalCell"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellStyle">
        <xsl:value-of select="$ConditionalCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellCol">
        <xsl:value-of select="$ConditionalCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellAll">
        <xsl:value-of select="$ConditionalCellAll"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellSingle">
        <xsl:value-of select="$ConditionalCellSingle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellMultiple">
        <xsl:value-of select="$ConditionalCellMultiple"/>
      </xsl:with-param>
      <xsl:with-param name="removeFilter" select="$removeFilter"/>
      <xsl:with-param name="ValidationCell">
        <xsl:value-of select="$ValidationCell"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationRow">
        <xsl:value-of select="$ValidationRow"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCellStyle">
        <xsl:value-of select="$ValidationCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConnectionsCell">
        <xsl:value-of select="$ConnectionsCell"/>
      </xsl:with-param>
      <xsl:with-param name="outlineLevel"/>
      <xsl:with-param name="AllRowBreakes">
        <xsl:value-of select="$AllRowBreakes"/>
      </xsl:with-param>
      <xsl:with-param name="rSheredStrings">
        <xsl:value-of select="$rSheredStrings"/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:if test="contains(concat(':', $GroupRowEnd), concat(':', @r, ':'))">
      <xsl:call-template name="InsertRowGroupEnd">
        <xsl:with-param name="GroupCell">
          <xsl:value-of select="$GroupRowEnd"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
<!--By sateesh, Changed Condition if to when-->
	  <xsl:choose>
		  <xsl:when
      test="not(following-sibling::e:row) and ($PictureRow != '' or $NoteRow != '' or $ConditionalRow != '' or $ValidationRow != '' or $AllRowBreakes!='' )">

      <xsl:variable name="GetMinRowWithElementAfterLastRow">
        <xsl:call-template name="GetMinRowWithPicture">
          <xsl:with-param name="PictureRow">
            <xsl:value-of select="concat($PictureRow, $NoteRow, $ConditionalRow, $AllRowBreakes)"/>
          </xsl:with-param>
          <xsl:with-param name="AfterRow">
            <xsl:value-of select="@r"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:call-template name="InsertElementsBetwenTwoRows">
        <xsl:with-param name="sheet">
          <xsl:value-of select="$sheet"/>
        </xsl:with-param>
        <xsl:with-param name="NameSheet">
          <xsl:value-of select="$NameSheet"/>
        </xsl:with-param>
        <xsl:with-param name="PictureCell">
          <xsl:value-of select="$PictureCell"/>
        </xsl:with-param>
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="$PictureRow"/>
        </xsl:with-param>
        <xsl:with-param name="NoteCell">
          <xsl:value-of select="$NoteCell"/>
        </xsl:with-param>
        <xsl:with-param name="NoteRow">
          <xsl:value-of select="$NoteRow"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCell">
          <xsl:value-of select="$ConditionalCell"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellStyle">
          <xsl:value-of select="$ConditionalCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalRow">
          <xsl:value-of select="$ConditionalRow"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellCol">
          <xsl:value-of select="$ConditionalCellCol"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellAll">
          <xsl:value-of select="$ConditionalCellAll"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellSingle">
          <xsl:value-of select="$ConditionalCellSingle"/>
        </xsl:with-param>
        <xsl:with-param name="ConditionalCellMultiple">
          <xsl:value-of select="$ConditionalCellMultiple"/>
        </xsl:with-param>
        <xsl:with-param name="sheetNr" select="$sheetNr"/>
        <!--value changed to 65536 from 65535 for file name Testfeatures XLSX V2.0 all.xlsx: bug no:2559854 -->
        <xsl:with-param name="EndRow">
          <xsl:value-of select="65536"/>
        </xsl:with-param>
        <!--end-->
        <xsl:with-param name="prevRow">
          <xsl:value-of select="@r"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCell">
          <xsl:value-of select="$ValidationCell"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationRow">
          <xsl:value-of select="$ValidationRow"/>
        </xsl:with-param>
        <xsl:with-param name="ValidationCellStyle">
          <xsl:value-of select="$ValidationCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="AllRowBreakes">
          <xsl:value-of select="$AllRowBreakes"/>
        </xsl:with-param>
      </xsl:call-template>
	  </xsl:when>
    <!--Start of RefNo-2-->
		<xsl:when
		  test="not(following-sibling::e:row) and (../preceding-sibling::e:cols/e:col[@style])">
      <table:table-row table:style-name="{concat('ro', key('Part', concat('xl/',$sheet))/e:worksheet/@oox:part)}"
      table:number-rows-repeated="65535">
        <table:table-cell table:number-columns-repeated="256"/>
      </table:table-row>
		</xsl:when>
    <!--End of RefNo-2-->
	  </xsl:choose>
  </xsl:template>
<!--By sateesh, Changed Condition if to when-->
  <xsl:template match="e:row" mode="headers">
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="BigMergeRow"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="headerRowsStart"/>
    <xsl:param name="headerRowsEnd"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="removeFilter"/>
    <xsl:param name="rSheredStrings"/>

    <xsl:variable name="this" select="."/>

    <xsl:variable name="lastCellColumnNumber">
      <xsl:choose>
        <xsl:when test="e:c[last()]/@r">
          <xsl:call-template name="GetColNum">
            <xsl:with-param name="cell">
              <xsl:value-of select="e:c[last()]/@r"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>1</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="CheckIfBigMerge">
      <xsl:call-template name="CheckIfBigMergeRow">
        <xsl:with-param name="RowNum">
          <xsl:value-of select="@r"/>
        </xsl:with-param>
        <xsl:with-param name="BigMergeCellRow">
          <xsl:value-of select="$BigMergeRow"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:choose>
      <!-- when this row is the first non-empty one before header but not row 1 and there aren't Big Merge Coll -->
      <xsl:when
        test="position()=1 and @r > 1 and @r &lt; $headerRowsStart and $BigMergeCell = '' ">
        <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}"
          table:number-rows-repeated="{@r - 1}">
          <table:table-cell table:number-columns-repeated="256"/>
        </table:table-row>
      </xsl:when>

      <!-- if this is a header row -->
      <xsl:when
        test="$headerRowsStart != ''  and @r &gt;= $headerRowsStart and @r &lt;= $headerRowsEnd">
        <xsl:choose>
          <!-- when this row is the first non-empty one but not row 1 and there are Big Merge Coll-->
          <xsl:when test="position()=1 and @r>1 and $BigMergeCell != '' ">
            <xsl:call-template name="InsertBigMergeFirstRowEmpty">
              <xsl:with-param name="RowNumber">
                <xsl:value-of select="1"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of select="$BigMergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="$MergeCell"/>
              </xsl:with-param>
            </xsl:call-template>
            <xsl:if test="@r - 2 &gt; 0"/>
            <xsl:call-template name="InsertBigMergeRowEmpty">
              <xsl:with-param name="RowNumber">
                <xsl:value-of select="2"/>
              </xsl:with-param>
              <xsl:with-param name="BigMergeCell">
                <xsl:value-of select="$BigMergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="$MergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="Repeat">
                <xsl:value-of select="@r - 2"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:when>

          <!-- if the first non-empty header row isn't the first header row -->
          <xsl:when test="position() = 1 and @r &gt; $headerRowsStart">
            <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}"
              table:number-rows-repeated="{@r - $headerRowsStart}">
              <table:table-cell table:number-columns-repeated="256"/>
            </table:table-row>
          </xsl:when>

          <!-- if there are empty header rows before this one header row-->
          <xsl:when test="position() &gt; 1 and @r - 1 != preceding-sibling::e:row[1]/@r">
            <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}"
              table:number-rows-repeated="{@r - preceding-sibling::e:row[1]/@r - 1}">
              <table:table-cell table:number-columns-repeated="256"/>
            </table:table-row>
          </xsl:when>
        </xsl:choose>
      </xsl:when>

      <!-- when this row is the first non-empty one after header rows and there aren't Big Merge Coll -->
      <xsl:when test="position()=1 and @r &gt; $headerRowsEnd + 1 and $BigMergeCell = '' ">
        <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}"
          table:number-rows-repeated="{@r - $headerRowsEnd - 1}">
          <table:table-cell table:number-columns-repeated="256"/>
        </table:table-row>
      </xsl:when>

      <!-- when this row is the first non-empty one but not row 1 and there are Big Merge Coll (this is not a header row) -->
      <xsl:when test="position()=1 and @r>1 and $BigMergeCell != '' ">
        <xsl:call-template name="InsertBigMergeFirstRowEmpty">
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="1"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:if test="@r - 2 &gt; 0"/>
        <xsl:call-template name="InsertBigMergeRowEmpty">
          <xsl:with-param name="RowNumber">
            <xsl:value-of select="2"/>
          </xsl:with-param>
          <xsl:with-param name="BigMergeCell">
            <xsl:value-of select="$BigMergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="Repeat">
            <xsl:value-of select="@r - 2"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>

      <!-- when this row is not first one and there were empty rows after previous non-empty row -->
      <xsl:when test="position() != 1 and @r != preceding-sibling::e:row[1]/@r + 1">
        <table:table-row table:style-name="{concat('ro', ancestor::e:worksheet/@oox:part)}"
          table:number-rows-repeated="{@r -1 - preceding-sibling::e:row[1]/@r}">
          <table:table-cell table:number-columns-repeated="256"/>
        </table:table-row>
      </xsl:when>

    </xsl:choose>

    <!-- insert this row -->
    <xsl:call-template name="InsertThisRow">
      <xsl:with-param name="BigMergeCell">
        <xsl:value-of select="$BigMergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="BigMergeRow">
        <xsl:value-of select="$BigMergeRow"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="lastCellColumnNumber">
        <xsl:value-of select="$lastCellColumnNumber"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfBigMerge">
        <xsl:value-of select="$CheckIfBigMerge"/>
      </xsl:with-param>
      <xsl:with-param name="rSheredStrings">
        <xsl:value-of select="$rSheredStrings"/>
      </xsl:with-param>

      <xsl:with-param name="this" select="$this"/>
      <xsl:with-param name="headerRowsStart" select="$headerRowsStart"/>
      <xsl:with-param name="sheetNr" select="$sheetNr"/>
      <xsl:with-param name="removeFilter" select="$removeFilter"/>
    </xsl:call-template>

  </xsl:template>

  <xsl:template name="ConvertCell" match="e:c">
    <xsl:param name="BeforeMerge"/>
    <xsl:param name="prevCellCol"/>
    <xsl:param name="BigMergeCell"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="PictureCell"/>
    <xsl:param name="PictureRow"/>
    <xsl:param name="NoteCell"/>
    <xsl:param name="NoteRow"/>
    <xsl:param name="sheet"/>
    <xsl:param name="NameSheet"/>
    <xsl:param name="sheetNr"/>
    <xsl:param name="ConditionalCell"/>
    <xsl:param name="ConditionalCellStyle"/>
    <xsl:param name="ConditionalCellCol"/>
    <xsl:param name="ConditionalCellAll"/>
    <xsl:param name="ConditionalCellSingle"/>
    <xsl:param name="ConditionalCellMultiple"/>
    <xsl:param name="ValidationCell"/>
    <xsl:param name="ValidationRow"/>
    <xsl:param name="ValidationCellStyle"/>
    <xsl:param name="ConnectionsCell"/>
    <xsl:param name="rSheredStrings"/>

    <xsl:variable name="this" select="."/>
<!--
	Changes By : Vijayeta,SP2
    File       : Testfeatures Extended Limits.xlsx, xlsx->Trans->ods->SP2->XLSX
	Desc       : The ODS file converted contains col repeated=16379 which is not supported by ODS which hence fails in SP2 conversion
	-->
    <!--
    <xsl:variable name="colNum" >
      <xsl:call-template name="GetColNum2">
        <xsl:with-param name="cell">
          <xsl:value-of select="@oox:p"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
	-->
	  <xsl:variable name="colNum" >
		  <xsl:variable name="temp" >
			  <xsl:call-template name="GetColNum2">
				  <xsl:with-param name="cell">
					  <xsl:value-of select="@oox:p"/>
				  </xsl:with-param>
			  </xsl:call-template>
		  </xsl:variable>
		  <xsl:choose >
			  <xsl:when test ="$temp &gt; 256">
				  <xsl:value-of select="256 - (16384 - $temp)"/>
			  </xsl:when>
			  <xsl:otherwise>
				  <xsl:value-of select="$temp"/>
			  </xsl:otherwise>
		  </xsl:choose>
	  </xsl:variable>
<!--Vijayeta, SP2, End-->
    <xsl:variable name="rowNum" >
      <xsl:call-template name="GetRowNum2">
        <xsl:with-param name="cell">
          <xsl:value-of select="@oox:p"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="CheckIfMerge">
      <xsl:call-template name="CheckIfMerge">
        <xsl:with-param name="colNum">
          <xsl:value-of select="$colNum"/>
        </xsl:with-param>
        <xsl:with-param name="rowNum">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="PictureColl">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $PictureCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="NoteColl">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $NoteCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="ConditionalColl">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $ConditionalCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="ValidationColl">
      <xsl:call-template name="GetCollsWithElement">
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNum"/>
        </xsl:with-param>
        <xsl:with-param name="ElementCell">
          <xsl:value-of select="concat(';', $ValidationCell)"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="GetMinCollWithElement">
      <xsl:call-template name="GetMinRowWithPicture">
        <xsl:with-param name="PictureRow">
          <xsl:value-of select="concat($PictureColl, $NoteColl, $ValidationColl, $ConditionalColl)"/>
        </xsl:with-param>
        <xsl:with-param name="AfterRow">
          <xsl:choose>
            <xsl:when test="$prevCellCol = ''">
              <xsl:text>0</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$prevCellCol"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <!-- Insert Empty Cell -->
    <xsl:call-template name="InsertEmptyCell">
      <xsl:with-param name="BeforeMerge">
        <xsl:value-of select="$BeforeMerge"/>
      </xsl:with-param>
      <xsl:with-param name="BigMergeCell">
        <xsl:value-of select="$BigMergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="prevCellCol">
        <xsl:value-of select="$prevCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="this" select="$this"/>
      <xsl:with-param name="colNum">
        <xsl:value-of select="$colNum"/>
      </xsl:with-param>
      <xsl:with-param name="rowNum">
        <xsl:value-of select="$rowNum"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfMerge">
        <xsl:value-of select="$CheckIfMerge"/>
      </xsl:with-param>
      <xsl:with-param name="PictureRow">
        <xsl:value-of select="$PictureRow"/>
      </xsl:with-param>
      <xsl:with-param name="PictureCell">
        <xsl:value-of select="$PictureCell"/>
      </xsl:with-param>
      <xsl:with-param name="NoteRow">
        <xsl:value-of select="$NoteRow"/>
      </xsl:with-param>
      <xsl:with-param name="NoteCell">
        <xsl:value-of select="$NoteCell"/>
      </xsl:with-param>
      <xsl:with-param name="sheet">
        <xsl:value-of select="$sheet"/>
      </xsl:with-param>
      <xsl:with-param name="NameSheet">
        <xsl:value-of select="$NameSheet"/>
      </xsl:with-param>
      <xsl:with-param name="PictureColl">
        <xsl:value-of select="$PictureColl"/>
      </xsl:with-param>
      <xsl:with-param name="NoteColl">
        <xsl:value-of select="$NoteColl"/>
      </xsl:with-param>
      <xsl:with-param name="sheetNr" select="$sheetNr"/>
      <xsl:with-param name="ConditionalCell">
        <xsl:value-of select="$ConditionalCell"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellStyle">
        <xsl:value-of select="$ConditionalCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCol">
        <xsl:value-of select="$ConditionalColl"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellCol">
        <xsl:value-of select="$ConditionalCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellAll">
        <xsl:value-of select="$ConditionalCellAll"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellSingle">
        <xsl:value-of select="$ConditionalCellSingle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellMultiple">
        <xsl:value-of select="$ConditionalCellMultiple"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCell">
        <xsl:value-of select="$ValidationCell"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationRow">
        <xsl:value-of select="$ValidationRow"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCellStyle">
        <xsl:value-of select="$ValidationCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationColl">
        <xsl:value-of select="$ValidationColl"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- Insert this cell or covered cell if this one is Merge Cell -->
    <xsl:call-template name="InsertThisCell">
      <xsl:with-param name="BeforeMerge">
        <xsl:value-of select="$BeforeMerge"/>
      </xsl:with-param>
      <xsl:with-param name="BigMergeCell">
        <xsl:value-of select="$BigMergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="prevCellCol">
        <xsl:value-of select="$prevCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="this" select="$this"/>
      <xsl:with-param name="colNum">
        <xsl:value-of select="$colNum"/>
      </xsl:with-param>
      <xsl:with-param name="rowNum">
        <xsl:value-of select="$rowNum"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfMerge">
        <xsl:value-of select="$CheckIfMerge"/>
      </xsl:with-param>
      <xsl:with-param name="PictureRow">
        <xsl:value-of select="$PictureRow"/>
      </xsl:with-param>
      <xsl:with-param name="PictureCell">
        <xsl:value-of select="$PictureCell"/>
      </xsl:with-param>
      <xsl:with-param name="PictureColl">
        <xsl:value-of select="$PictureColl"/>
      </xsl:with-param>
      <xsl:with-param name="NoteRow">
        <xsl:value-of select="$NoteRow"/>
      </xsl:with-param>
      <xsl:with-param name="NoteCell">
        <xsl:value-of select="$NoteCell"/>
      </xsl:with-param>
      <xsl:with-param name="NoteColl">
        <xsl:value-of select="$NoteColl"/>
      </xsl:with-param>
      <xsl:with-param name="sheet">
        <xsl:value-of select="$sheet"/>
      </xsl:with-param>
      <xsl:with-param name="NameSheet">
        <xsl:value-of select="$NameSheet"/>
      </xsl:with-param>
      <xsl:with-param name="GetMinCollWithPicture">
        <xsl:value-of select="$GetMinCollWithElement"/>
      </xsl:with-param>
      <xsl:with-param name="sheetNr" select="$sheetNr"/>
      <xsl:with-param name="ConditionalCell">
        <xsl:value-of select="$ConditionalCell"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellStyle">
        <xsl:value-of select="$ConditionalCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellCol">
        <xsl:value-of select="$ConditionalCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellAll">
        <xsl:value-of select="$ConditionalCellAll"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellSingle">
        <xsl:value-of select="$ConditionalCellSingle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellMultiple">
        <xsl:value-of select="$ConditionalCellMultiple"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCell">
        <xsl:value-of select="$ValidationCell"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationRow">
        <xsl:value-of select="$ValidationRow"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCellStyle">
        <xsl:value-of select="$ValidationCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConnectionsCell">
        <xsl:value-of select="$ConnectionsCell"/>
      </xsl:with-param>
      <xsl:with-param name="rSheredStrings">
        <xsl:value-of select="$rSheredStrings"/>
      </xsl:with-param>
    </xsl:call-template>

    <!-- Insert next coll -->
    <xsl:call-template name="InsertNextCell">
      <xsl:with-param name="BeforeMerge">
        <xsl:value-of select="$BeforeMerge"/>
      </xsl:with-param>
      <xsl:with-param name="BigMergeCell">
        <xsl:value-of select="$BigMergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="prevCellCol">
        <xsl:value-of select="$colNum"/>
      </xsl:with-param>
      <xsl:with-param name="this" select="$this"/>
      <xsl:with-param name="colNum">
        <xsl:value-of select="$colNum"/>
      </xsl:with-param>
      <xsl:with-param name="rowNum">
        <xsl:value-of select="$rowNum"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfMerge">
        <xsl:value-of select="$CheckIfMerge"/>
      </xsl:with-param>
      <xsl:with-param name="PictureRow">
        <xsl:value-of select="$PictureRow"/>
      </xsl:with-param>
      <xsl:with-param name="PictureCell">
        <xsl:value-of select="$PictureCell"/>
      </xsl:with-param>
      <xsl:with-param name="PictureColl">
        <xsl:value-of select="$PictureColl"/>
      </xsl:with-param>
      <xsl:with-param name="NoteRow">
        <xsl:value-of select="$NoteRow"/>
      </xsl:with-param>
      <xsl:with-param name="NoteCell">
        <xsl:value-of select="$NoteCell"/>
      </xsl:with-param>
      <xsl:with-param name="NoteColl">
        <xsl:value-of select="$NoteColl"/>
      </xsl:with-param>
      <xsl:with-param name="sheet">
        <xsl:value-of select="$sheet"/>
      </xsl:with-param>
      <xsl:with-param name="NameSheet">
        <xsl:value-of select="$NameSheet"/>
      </xsl:with-param>
      <xsl:with-param name="GetMinCollWithElement">
        <xsl:value-of select="$GetMinCollWithElement"/>
      </xsl:with-param>
      <xsl:with-param name="sheetNr" select="$sheetNr"/>
      <xsl:with-param name="ConditionalCell">
        <xsl:value-of select="$ConditionalCell"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellStyle">
        <xsl:value-of select="$ConditionalCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ConnectionsCell">
        <xsl:value-of select="$ConnectionsCell"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellCol">
        <xsl:value-of select="$ConditionalCellCol"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellAll">
        <xsl:value-of select="$ConditionalCellAll"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellSingle">
        <xsl:value-of select="$ConditionalCellSingle"/>
      </xsl:with-param>
      <xsl:with-param name="ConditionalCellMultiple">
        <xsl:value-of select="$ConditionalCellMultiple"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCell">
        <xsl:value-of select="$ValidationCell"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationRow">
        <xsl:value-of select="$ValidationRow"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationCellStyle">
        <xsl:value-of select="$ValidationCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="ValidationColl">
        <xsl:value-of select="$ValidationColl"/>
      </xsl:with-param>
      <xsl:with-param name="rSheredStrings">
        <xsl:value-of select="$rSheredStrings"/>
      </xsl:with-param>
    </xsl:call-template>

  </xsl:template>


  <!-- convert run into span -->
  <xsl:template match="e:r">
    <!--RefNo-1: Changes for fixing 1833074 XLSX: Cell Content missing-->
    <!--<xsl:param name ="textp"/>-->
    <!-- 
	        Bug No.          :1805599
		    Code Modified By:Vijayeta
			Date            :6th Nov '07
			Description     :New Line to be added, for which the comment is checked for new lines.
			                If present Post Processor in the CS file 'OdfSharedStringsPostProcessor.cs' is called
	   -->
    <xsl:variable name="textContent">
      <xsl:value-of select="./e:t"/>
    </xsl:variable>
    <xsl:choose >
      <xsl:when test ="contains($textContent, '&#xA;')">
        <xsl:variable name ="Id" >
          <xsl:value-of select ="generate-id(.)"/>
        </xsl:variable>
        <xsl:value-of select ="concat('SonataAnnotation|',$textContent,'|',$Id)"/>
      </xsl:when>
      <!--Start of RefNo-1-->
      <!--<xsl:when test ="$textp='T'">
        <text:span>
          <xsl:if test="e:rPr">
            <xsl:attribute name="text:style-name">
              <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:apply-templates/>
        </text:span>
      </xsl:when>-->
      <!--End of RefNo-1-->
      <xsl:otherwise >
        <!--RefNo-1:Commented
        <text:p text:style-name="{generate-id(./parent::node())}">-->
        <text:span>
          <xsl:if test="e:rPr">
            <xsl:attribute name="text:style-name">
              <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:apply-templates/>
        </text:span>
        <!--</text:p >-->
      </xsl:otherwise>
    </xsl:choose>
    <!-- End of modification for the bug 1805599-->

    <!--<text:span>
      <xsl:if test="e:rPr">
        <xsl:attribute name="text:style-name">
          <xsl:value-of select="generate-id(.)"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:apply-templates/>
    </text:span>-->
  </xsl:template>

  <!-- convert run into span in hyperlink-->
  <xsl:template match="e:r" mode="hyperlink">
    <xsl:param name="XlinkHref"/>
    <xsl:param name="position"/>

    <xsl:choose>
      <xsl:when test="$position = '1'">
        <text:span>
          <xsl:if test="e:rPr">
            <xsl:attribute name="text:style-name">
              <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
          </xsl:if>
          <text:a>
            <xsl:attribute name="xlink:href">
              <xsl:value-of select="$XlinkHref"/>
            </xsl:attribute>
            <xsl:apply-templates/>
            <xsl:if test="following-sibling::e:r">
              <xsl:apply-templates select="following-sibling::e:r[1]" mode="hyperlink">
                <xsl:with-param name="XlinkHref">
                  <xsl:value-of select="$XlinkHref"/>
                </xsl:with-param>
                <xsl:with-param name="position">
                  <xsl:value-of select="$position+1"/>
                </xsl:with-param>
              </xsl:apply-templates>
            </xsl:if>
          </text:a>
        </text:span>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates/>
        <xsl:if test="following-sibling::e:r">
          <xsl:apply-templates select="following-sibling::e:r[1]" mode="hyperlink">
            <xsl:with-param name="XlinkHref">
              <xsl:value-of select="$XlinkHref"/>
            </xsl:with-param>
            <xsl:with-param name="position">
              <xsl:value-of select="$position+1"/>
            </xsl:with-param>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>

  <xsl:template name="CheckSheetName">
    <xsl:param name="name"/>
    <xsl:param name="sheetNumber"/>

    <!--Start of RefNo-3: code to chk for the invalid charecters-->
    <!--Start of RefNo-4: Added some more invalid special charecters to the list-->    
    <xsl:variable name="invalidChars">
      <xsl:text>&apos;!-$#():,.+^&amp;=&lt;&gt;%{};"</xsl:text>
    </xsl:variable>

    <xsl:variable name="strTrnName" select="translate($name,$invalidChars,'')"/>
    <!--End of RefNo-3: code to chk for the invalid charecters-->
    
    <xsl:choose>
      <!-- when there are at least 2 sheets with the same name after removal of forbidden characters and cutting to 31 characters (name correction) -->
      <!--RefNo-3:Replaced $name with $strTrnName-->
      <xsl:when test="parent::node()/e:sheet[translate(@name,$invalidChars,'') = $strTrnName][2]">
        <xsl:variable name="nameConflictsBefore">
          <!-- count sheets before this one whose name (after correction) collide with this sheet name (after correction) -->
          <!--RefNo-3:Replaced $name with $strTrnName-->
          <xsl:value-of
            select="count(parent::node()/e:sheet[translate(@name,$invalidChars,'') = $strTrnName and position() &lt; $sheetNumber])"
          />
        </xsl:variable>
        <!-- cut name and add "(N)" at the end where N is seqential number of duplicated name -->
        <xsl:value-of select="concat(translate(@name,$invalidChars,''),'_',$nameConflictsBefore + 1)"/>
      </xsl:when>
      <xsl:otherwise>
        <!--RefNo-3:Replaced $name with $strTrnName-->
        <xsl:value-of select="$strTrnName"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="rSheredStrings">
    <xsl:param name="result"/>
	    <!-- Perofomance-->
	  <xsl:for-each select="key('Part', 'xl/sharedStrings.xml')">
		  <xsl:for-each select="e:sst">
      <xsl:apply-templates select="e:si[1]" mode="r">
        <xsl:with-param name="result">
          <xsl:choose>
            <xsl:when test="e:r">
              <xsl:value-of select="concat($result, '0', ';')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$result"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
        <xsl:with-param name="position">
          <xsl:text>0</xsl:text>
        </xsl:with-param>
      </xsl:apply-templates>
		  </xsl:for-each>
    </xsl:for-each>

  </xsl:template>


  <xsl:template match="e:si" mode="r">
    <xsl:param name="result"/>
    <xsl:param name="position"/>

    <xsl:choose>
      <xsl:when test="following-sibling::e:si/e:r">
        <xsl:apply-templates select="following-sibling::e:si[1]" mode="r">
          <xsl:with-param name="result">
            <xsl:choose>
              <xsl:when test="e:r">
                <xsl:value-of select="concat($result, $position, ';')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$result"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="position">
            <xsl:value-of select="$position + 1"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="e:r">
            <xsl:value-of select="concat($result, $position, ';')"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$result"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

	<!-- Vijayeta-->
	<xsl:template name="translate-expression">
		<!--  return position or range for formula or other -->
		<xsl:param name="cell-row-pos"/>
		<!-- the position in row (vertical) of cell -->
		<xsl:param name="cell-column-pos"/>
		<!-- the position in column (horizontal of cell -->
		<xsl:param name="expression"/>
		<!-- recomposed expression containing cell positions after every conversion -->
		<xsl:param name="is-range-mode" select="false()"/>
		<!-- as mode changes a '[.' resp. ']' is written out  -->
		<xsl:param name="return-value"/>
		<!-- expression of table:cell-range-address is different than formula (e.g. no prefix)  -->
		<xsl:param name="isRangeAddress"/>
		<!-- determines if the currently processed expression is relative -->
		<xsl:param name="isRelative" select="false()" />

		<!-- value to be given out later -->
		<!-- to judge whether this input expression contains any cell position to convert -->
		<xsl:variable name="temp-range">
			<xsl:choose>
				<xsl:when test="$expression != ''">
					<xsl:call-template name="parse-range-name">
						<xsl:with-param name="expression" select="$expression"/>
						<xsl:with-param name="return-value" select="''"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="''"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- if $range-type = 1, then range is representing a sheet, function's name or separated symbol, but not cell position,
			 or if $range-type = 2, range should be handled because it contains certain cell position.
			 The first character marks the type of that expression. -->
		<xsl:variable name="range-type">
			<xsl:choose>
				<xsl:when test="substring($temp-range, 1, 1) = '1'">
					<xsl:value-of select="1"/>
				</xsl:when>
				<xsl:when test="substring($temp-range, 1, 1) = '2'">
					<xsl:value-of select="2"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="2"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<!-- remove that added range type token -->
		<xsl:variable name="current-range">
			<xsl:value-of select="substring($temp-range, 2)"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$range-type = 1">
				<!-- Nothing to convert, so just join the front and behind strings. -->
				<xsl:call-template name="translate-expression">
					<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
					<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
					<xsl:with-param name="expression">
						<!-- get current converting position from $temp-token or $current-range, then join the expression. -->
						<xsl:choose>
							<xsl:when test="contains($current-range, '#$')">
								<!-- because of recomposing of string, the $current-range may not be the pit
							of $expression, so the char #$ should not be used for nominal -->
								<xsl:variable name="temp-token">
									<xsl:choose>
										<xsl:when test="contains($current-range, '\')">
											<xsl:value-of select="concat(']', substring-after($current-range, '#$'), &quot;&apos;&quot;)"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="substring-after($current-range, '#$')"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:value-of select="substring-after($expression, $temp-token)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="substring-after($expression, $current-range)"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="return-value">
						<!-- react on range mode change (when to insert closing ']' or in case of '!' change the mode to RANGE and create open '[' -->
						<xsl:choose>
							<xsl:when test="$current-range = '=' and $return-value = '' and not($isRangeAddress)">
								<xsl:text>oooc:=</xsl:text>
							</xsl:when>
							<xsl:when test="contains($current-range, '!') and not($isRangeAddress)">
								<xsl:value-of select="concat($return-value, '[', $current-range)"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<xsl:when test="$is-range-mode = 'true' and $current-range != ':' and not($isRangeAddress)">
										<xsl:value-of select="concat($return-value, ']', substring-before($expression, $current-range), $current-range)"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="concat($return-value, substring-before($expression, $current-range), $current-range)"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="is-range-mode">
						<xsl:choose>
							<!-- ! is the separator of worksheet and range
								 : is the separator for a cell range -->
							<xsl:when test="contains($current-range, '!') or $current-range = ':'">
								<xsl:value-of select="true()"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="false()"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:with-param>
					<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<!-- types of range to handle in $current-range, i.e. the cell position expression to convert
					1: special cell including row and column; e.g. R4C5
					2: whole row; e.g. R3
					3: whole column; e.g. C5
					4: other name not for cell or row/column; e.g. RANDOM() or something unknown
				-->
				<xsl:variable name="handle-type">
					<xsl:choose>
						<xsl:when test="starts-with($current-range, 'R')">
							<!-- It's type 1 or type 2 or 4/unknown cell position. -->
							<xsl:choose>
								<xsl:when test="contains($current-range, 'C')">
									<!-- It's type 1, specifying the cell position or 4/unknown -->
									<xsl:variable name="part-type-r">
										<xsl:call-template name="handle-type-number">
											<xsl:with-param name="t-part" select="substring-before( substring-after($current-range, 'R'), 'C')"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:variable name="part-type-c">
										<xsl:call-template name="handle-type-number">
											<xsl:with-param name="t-part" select="substring-after($current-range, 'C')"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:choose>
										<xsl:when test="($part-type-r = 1) and ($part-type-c = 1)">
											<xsl:value-of select="1"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="4"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:when>
								<xsl:otherwise>
									<!-- It's type 2 specifying the cell position, or 4/unknown. -->
									<xsl:variable name="part-type">
										<xsl:call-template name="handle-type-number">
											<xsl:with-param name="t-part" select="substring-after($current-range, 'R')"/>
										</xsl:call-template>
									</xsl:variable>
									<xsl:choose>
										<xsl:when test="$part-type = 1">
											<xsl:value-of select="2"/>
										</xsl:when>
										<xsl:when test="$part-type = 2">
											<xsl:value-of select="4"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="4"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:when test="starts-with($current-range, 'C')">
							<!-- It's type 3 of cell position, or 4/unknown -->
							<xsl:variable name="part-type">
								<xsl:call-template name="handle-type-number">
									<xsl:with-param name="t-part" select="substring-after($current-range, 'C')"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:choose>
								<xsl:when test="$part-type = 1">
									<xsl:value-of select="3"/>
								</xsl:when>
								<xsl:when test="$part-type = 2">
									<xsl:value-of select="4"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="4"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<!-- It's type 4, not cell position -->
							<xsl:value-of select="4"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<!-- Start to convert that cell position expression, that cell position unit -->
				<xsl:choose>
					<xsl:when test="$handle-type = 1">
						<!-- It's type 1, e.g. R1C2 -->
						<!-- process the row part -->
						<xsl:variable name="after-R">
							<xsl:value-of select="substring(substring-after($current-range,'R'),1,1)"/>
						</xsl:variable>
						<xsl:choose>
							<!-- found one cell unit -->
							<xsl:when test="$after-R='C' or $after-R='[' or $after-R='0' or $after-R='1' or $after-R='2' or $after-R='3' or $after-R='4' or $after-R='5' or $after-R='6' or $after-R='7' or $after-R='8' or $after-R='9'">
								<xsl:variable name="row-pos">
									<xsl:choose>
										<xsl:when test="$after-R='['">
											<xsl:value-of select="$cell-row-pos+substring-before( substring-after($current-range,'R['),']')"/>
										</xsl:when>
										<xsl:when test="$after-R='C'">
											<xsl:value-of select="$cell-row-pos"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="substring-before(substring-after($current-range,'R'),'C')"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="row-pos-style">
									<xsl:choose>
										<xsl:when test="$after-R='[' or $after-R='C'">relative</xsl:when>
										<xsl:otherwise>absolute</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<!-- process the column part -->
								<xsl:variable name="after-C">
									<xsl:value-of select="substring(substring-after(substring-after($current-range,'R'),'C'),1,1)"/>
								</xsl:variable>
								<xsl:variable name="column-digit-length">
									<xsl:choose>
										<xsl:when test="$after-C='0' or $after-C='1' or $after-C='2' or $after-C='3' or $after-C='4' or $after-C='5' or $after-C='6' or $after-C='7' or $after-C='8' or $after-C='9'">
											<xsl:call-template name="get-digit-length">
												<xsl:with-param name="complexive-string" select="substring-after(substring-after($current-range,'R'),'C')"/>
											</xsl:call-template>
										</xsl:when>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="column-pos">
									<xsl:choose>
										<xsl:when test="$after-C='['">
											<xsl:value-of select="$cell-column-pos + substring-before(substring-after(substring-after($current-range,'R'),'C['),']')"/>
										</xsl:when>
										<xsl:when test="$after-C='0' or $after-C='1' or $after-C='2' or $after-C='3' or $after-C='4' or $after-C='5' or $after-C='6' or $after-C='7' or $after-C='8' or $after-C='9'">
											<xsl:value-of select="substring(substring-after(substring-after($current-range,'R'),'C'),1,$column-digit-length)"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$cell-column-pos"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="column-pos-style">
									<xsl:choose>
										<xsl:when test="$after-C='0' or $after-C='1' or $after-C='2' or $after-C='3' or $after-C='4' or $after-C='5' or $after-C='6' or $after-C='7' or $after-C='8' or $after-C='9'">absolute</xsl:when>
										<xsl:otherwise>relative</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="trans-unit">
									<xsl:call-template name="translate-unit">
										<xsl:with-param name="column-number" select="$column-pos"/>
										<xsl:with-param name="row-number" select="$row-pos"/>
										<xsl:with-param name="column-pos-style" select="$column-pos-style"/>
										<xsl:with-param name="row-pos-style" select="$row-pos-style"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), $trans-unit)"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($expression, $current-range)"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), translate( substring-before(substring-after($expression, '('),'R'),',!', ';.'))"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($current-range,'R')"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="$handle-type = 2">
						<!-- It's type 2, e.g. R3 -->
						<!-- process the range only including a whole row -->
						<xsl:variable name="after-R">
							<xsl:value-of select="substring(substring-after($current-range,'R'),1,1)"/>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="$after-R='[' or $after-R='0' or $after-R='1' or $after-R='2' or $after-R='3' or $after-R='4' or $after-R='5' or $after-R='6' or $after-R='7' or $after-R='8' or $after-R='9'">
								<xsl:variable name="row-number">
									<xsl:choose>
										<xsl:when test="$after-R = '['">
											<xsl:value-of select="substring-before(substring-after($current-range, 'R['), ']')"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="substring-after($current-range, 'R')"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="row-pos">
									<xsl:choose>
										<xsl:when test="$after-R='['">
											<xsl:value-of select="$cell-row-pos + $row-number"/>
										</xsl:when>
										<xsl:when test="$after-R='0' or $after-R='1' or $after-R='2' or $after-R='3' or $after-R='4' or $after-R='5' or $after-R='6' or $after-R='7' or $after-R='8' or $after-R='9'">
											<xsl:value-of select="$row-number"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$cell-row-pos"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="trans-unit1">
									<xsl:call-template name="translate-unit">
										<xsl:with-param name="column-number" select="1"/>
										<xsl:with-param name="row-number" select="$row-pos"/>
										<xsl:with-param name="column-pos-style" select="'relative'"/>
										<xsl:with-param name="row-pos-style" select="'relative'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="trans-unit2">
									<xsl:call-template name="translate-unit">
										<xsl:with-param name="column-number" select="256"/>
										<xsl:with-param name="row-number" select="$row-pos"/>
										<xsl:with-param name="column-pos-style" select="'relative'"/>
										<xsl:with-param name="row-pos-style" select="'relative'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), $trans-unit1, ':', $trans-unit2)"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($expression, $current-range)"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), translate( substring-before($current-range,'R'),',!', ';.'),'R')"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($current-range,'R')"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:when test="$handle-type = 3">
						<!-- It's type 3, e.g. C4 -->
						<!-- process the range only including a whole column -->
						<xsl:variable name="after-C">
							<xsl:value-of select="substring(substring-after($current-range,'C'),1,1)"/>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="$after-C='[' or $after-C='0' or $after-C='1' or $after-C='2' or $after-C='3' or $after-C='4' or $after-C='5' or $after-C='6' or $after-C='7' or $after-C='8' or $after-C='9'">
								<xsl:variable name="column-number">
									<xsl:choose>
										<xsl:when test="$after-C = '['">
											<xsl:value-of select="substring-before(substring-after($current-range, 'C['), ']')"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="substring-after($current-range, 'C')"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="column-pos">
									<xsl:choose>
										<xsl:when test="$after-C='['">
											<xsl:value-of select="$cell-column-pos + $column-number"/>
										</xsl:when>
										<xsl:when test="$after-C='0' or $after-C='1' or $after-C='2' or $after-C='3' or $after-C='4' or $after-C='5' or $after-C='6' or $after-C='7' or $after-C='8' or $after-C='9'">
											<xsl:value-of select="$column-number"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select="$cell-column-pos"/>
										</xsl:otherwise>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name="trans-unit1">
									<xsl:call-template name="translate-unit">
										<xsl:with-param name="column-number" select="$column-pos"/>
										<xsl:with-param name="row-number" select="1"/>
										<xsl:with-param name="column-pos-style" select="'relative'"/>
										<xsl:with-param name="row-pos-style" select="'relative'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="trans-unit2">
									<xsl:call-template name="translate-unit">
										<xsl:with-param name="column-number" select="$column-pos"/>
										<xsl:with-param name="row-number" select="65565"/>
										<xsl:with-param name="column-pos-style" select="'relative'"/>
										<xsl:with-param name="row-pos-style" select="'relative'"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), $trans-unit1, ':', $trans-unit2)"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($expression, $current-range)"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<xsl:variable name="name-unit" select="concat(substring-before($expression, $current-range), translate( substring-before($current-range,'C'),',!', ';.'),'C')"/>
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($current-range,'C')"/>
									<xsl:with-param name="return-value">
										<xsl:choose>
											<xsl:when test="$is-range-mode = 'true'">
												<xsl:value-of select="concat($return-value, $name-unit)"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="concat($return-value, '[.', $name-unit)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:with-param>
									<xsl:with-param name="is-range-mode" select="true()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<!-- It's unknown, so just jump over it -->
						<xsl:variable name="next-pit" select="substring-after($expression, $current-range)"/>
						<xsl:choose>
							<xsl:when test="contains($next-pit, '+') or contains($next-pit, '-') or contains($next-pit, '*') or contains($next-pit, '/') or contains($next-pit, ')') or contains($next-pit, '^') or contains($next-pit, ':') or contains($next-pit, '&quot;') or contains($next-pit, ';') or contains($next-pit, ',') or contains($next-pit, '[')">
								<xsl:call-template name="translate-expression">
									<xsl:with-param name="cell-row-pos" select="$cell-row-pos"/>
									<xsl:with-param name="cell-column-pos" select="$cell-column-pos"/>
									<xsl:with-param name="expression" select="substring-after($expression, $current-range)"/>
									<xsl:with-param name="return-value" select="concat($return-value, substring-before($expression, $current-range), $current-range)"/>
									<xsl:with-param name="is-range-mode" select="false()"/>
									<xsl:with-param name="isRangeAddress" select="$isRangeAddress"/>
								</xsl:call-template>
							</xsl:when>
							<xsl:otherwise>
								<!-- return the final range or formula -->
								<xsl:choose>
									<!-- in case the closing bracket of the range wasn't set, do it now  -->
									<xsl:when test="$is-range-mode = 'true' and $current-range = ''">
										<xsl:value-of select="translate( concat($return-value, ']'),',!', ';.')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:if test ="starts-with($return-value,'OFFSET') or starts-with($return-value,'offset') or contains($return-value,'OFFSET') or contains($return-value,'offset')">
										<xsl:value-of select="translate( concat($return-value, substring-before($expression, $current-range), $current-range),',!', ';.')"/>
										</xsl:if>
										<xsl:if test ="not(starts-with($return-value,'OFFSET') or starts-with($return-value,'offset') or contains($return-value,'OFFSET') or contains($return-value,'offset'))">
											<xsl:variable name ="tempReturnValue">
												<xsl:value-of select ="concat(&quot;&apos;&quot;,translate(substring-before($return-value,'!'),&quot;&apos;&quot;,''),&quot;&apos;&quot;)"/>
											</xsl:variable>
											<xsl:value-of select="translate(concat($tempReturnValue,'!',substring-after($return-value,'!'), substring-before($expression, $current-range), $current-range),',!', ';.')"/>
										</xsl:if>										
									</xsl:otherwise>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="parse-range-name">
		<!-- return the string or name for next handle. the type 1 is names of function, sheet, special separated symbol, not to parse as range; type 2 is the range including R/C to be parsed -->
		<xsl:param name="expression"/>
		<xsl:param name="return-value"/>
		<xsl:variable name="first-one" select="substring($expression,1,1)"/>
		<xsl:choose>
			<xsl:when test="$first-one = '='">
				<xsl:choose>
					<xsl:when test="string-length(normalize-space($return-value)) &gt; 0">
						<xsl:value-of select="concat('2', $return-value)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:text>1=</xsl:text>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$first-one='(' or $first-one='!' or $first-one='&amp;'">
				<xsl:value-of select="concat('1', $return-value, $first-one)"/>
			</xsl:when>
			<xsl:when test="$first-one='['">
				<xsl:choose>
					<xsl:when test="starts-with(substring-after($expression, ']'), 'C')">
						<xsl:call-template name="parse-range-name">
							<xsl:with-param name="expression" select="substring-after($expression, ']')"/>
							<xsl:with-param name="return-value" select="concat($return-value, substring-before($expression, ']'), ']')"/>
						</xsl:call-template>
					</xsl:when>
					<xsl:when test="contains(substring-before($expression, ']'), '.') and contains(substring-after($expression, ']'), '!')">
						<xsl:value-of select="concat('1', &quot;&apos;&quot;, substring-before(substring-after($expression, '['), ']'), &quot;&apos;&quot;, '#$', substring-before(substring-after($expression, ']'), '!'))"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat('2', $return-value, substring-before($expression, ']'), ']')"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$first-one='&quot;'">
				<xsl:value-of select="concat('1', $first-one, substring-before(substring-after($expression, '&quot;'), '&quot;'), '&quot;')"/>
			</xsl:when>
			<xsl:when test="$first-one=&quot;&apos;&quot;">
				<!-- here the string &quot;&apos;&quot; represents a char &apos;  -->
				<xsl:variable name="str-in" select="substring-before(substring-after($expression, &quot;&apos;&quot;), &quot;&apos;&quot;)"/>
				<xsl:choose>
					<!-- for file path transformation -->
					<xsl:when test="contains($str-in, '\') and contains($str-in, '[') and contains($str-in, ']')">
						<xsl:variable name="first-pos" select="substring-before($str-in, '[')"/>
						<xsl:variable name="second-pos" select="substring-before(substring-after($str-in, '['), ']')"/>
						<xsl:variable name="third-pos" select="substring-after($str-in, ']')"/>
						<xsl:value-of select="concat('1', &quot;&apos;&quot;, $first-pos, $second-pos, &quot;&apos;&quot;, '#$', $third-pos)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat('1', &quot;&apos;&quot;, $str-in, &quot;&apos;&quot;)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="$first-one='+' or $first-one='-' or $first-one='*' or $first-one='/' or $first-one=')' or $first-one='^' or $first-one=':' or $first-one='&quot;' or $first-one=';' or $first-one=',' or $first-one='&gt;' or $first-one='&lt;'">
				<xsl:choose>
					<xsl:when test="$return-value = ''">
						<xsl:value-of select="concat('1', $first-one)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="concat('2', $return-value)"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:choose>
					<xsl:when test="$expression = ''">
						<xsl:value-of select="concat('2', $return-value)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:call-template name="parse-range-name">
							<xsl:with-param name="expression" select="substring($expression, 2, string-length($expression)-1)"/>
							<xsl:with-param name="return-value" select="concat($return-value, substring($expression, 1, 1))"/>
						</xsl:call-template>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="handle-type-number">
		<!-- to handle the part between R and C, or after C in range string in translate-expression. return type: 1: number or cell range; 2: other, not for next step -->
		<xsl:param name="t-part"/>
		<xsl:choose>
			<xsl:when test="starts-with($t-part, '[')">
				<xsl:variable name="tt-str" select="substring-before( substring-after( $t-part, '['), ']')"/>
				<xsl:choose>
					<xsl:when test="($tt-str &lt; 0) or ($tt-str &gt; 0) or ($tt-str = 0)">
						<xsl:value-of select="1"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="2"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:when test="($t-part &lt; 0) or ($t-part &gt; 0) or ($t-part = 0)">
				<xsl:value-of select="1"/>
			</xsl:when>
			<xsl:when test="$t-part = ''">
				<xsl:value-of select="1"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="2"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="translate-unit">
		<!-- convert cell position expression unit, R1C1, R3, C4 -->
		<xsl:param name="column-number"/>
		<xsl:param name="row-number"/>
		<xsl:param name="column-pos-style"/>
		<xsl:param name="row-pos-style"/>
		<xsl:variable name="column-number1">
			<xsl:value-of select="floor( $column-number div 26 )"/>
		</xsl:variable>
		<xsl:variable name="column-number2">
			<xsl:value-of select="$column-number mod 26"/>
		</xsl:variable>
		<xsl:variable name="column-character1">
			<xsl:call-template name="number-to-character">
				<xsl:with-param name="number" select="$column-number1"/>
			</xsl:call-template>
		</xsl:variable>
		<xsl:variable name="column-character2">
			<xsl:call-template name="number-to-character">
				<xsl:with-param name="number" select="$column-number2"/>
			</xsl:call-template>
		</xsl:variable>
		<!-- position styles are 'absolute' or 'relative', -->
		<xsl:choose>
			<xsl:when test="$column-pos-style = 'absolute'">
				<xsl:value-of select="concat( '$', $column-character1, $column-character2)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="concat( $column-character1, $column-character2)"/>
			</xsl:otherwise>
		</xsl:choose>
		<xsl:choose>
			<xsl:when test="$row-pos-style ='absolute'">
				<xsl:value-of select="concat( '$', $row-number)"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$row-number"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="get-digit-length">
		<xsl:param name="complexive-string"/>
		<xsl:variable name="first-char">
			<xsl:value-of select="substring( $complexive-string, 1, 1)"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="$first-char = '1' or $first-char = '2' or $first-char = '3' or $first-char = '4' or $first-char = '5' or $first-char = '6' or $first-char = '7' or $first-char = '8' or $first-char = '9' or $first-char = '0' ">
				<xsl:variable name="temp">
					<xsl:call-template name="get-digit-length">
						<xsl:with-param name="complexive-string" select="substring( $complexive-string, 2)"/>
					</xsl:call-template>
				</xsl:variable>
				<xsl:value-of select="$temp+1"/>
			</xsl:when>
			<xsl:otherwise>0</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	<xsl:template name="number-to-character">
		<xsl:param name="number"/>
		<xsl:choose>
			<xsl:when test="$number = 0"/>
			<xsl:when test="$number = 1">A</xsl:when>
			<xsl:when test="$number = 2">B</xsl:when>
			<xsl:when test="$number = 3">C</xsl:when>
			<xsl:when test="$number = 4">D</xsl:when>
			<xsl:when test="$number = 5">E</xsl:when>
			<xsl:when test="$number = 6">F</xsl:when>
			<xsl:when test="$number = 7">G</xsl:when>
			<xsl:when test="$number = 8">H</xsl:when>
			<xsl:when test="$number = 9">I</xsl:when>
			<xsl:when test="$number = 10">J</xsl:when>
			<xsl:when test="$number = 11">K</xsl:when>
			<xsl:when test="$number = 12">L</xsl:when>
			<xsl:when test="$number = 13">M</xsl:when>
			<xsl:when test="$number = 14">N</xsl:when>
			<xsl:when test="$number = 15">O</xsl:when>
			<xsl:when test="$number = 16">P</xsl:when>
			<xsl:when test="$number = 17">Q</xsl:when>
			<xsl:when test="$number = 18">R</xsl:when>
			<xsl:when test="$number = 19">S</xsl:when>
			<xsl:when test="$number = 20">T</xsl:when>
			<xsl:when test="$number = 21">U</xsl:when>
			<xsl:when test="$number = 22">V</xsl:when>
			<xsl:when test="$number = 23">W</xsl:when>
			<xsl:when test="$number = 24">X</xsl:when>
			<xsl:when test="$number = 25">Y</xsl:when>
			<xsl:when test="$number = 26">Z</xsl:when>
			<xsl:otherwise/>
		</xsl:choose>
	</xsl:template>
	<!-- Vijayeta ODF 1.1 for table:cell-range-address and table:base-cell-address -->
	<xsl:template name ="getCorrectRange">
		<xsl:param name ="PrintRange"/>
		<xsl:param name ="finalRange"/>
		<xsl:variable name ="tempPrintRange">
			<xsl:value-of select ="$PrintRange"/>
		</xsl:variable>
		<xsl:variable name ="FinalPrintRange">
			<xsl:value-of select ="$finalRange"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="$tempPrintRange !=''">
				<xsl:variable name ="tempFinalRange">
					<xsl:variable name ="range">
						<xsl:choose>
							<xsl:when test ="contains($tempPrintRange,',')">
								<xsl:value-of select ="substring-before($tempPrintRange,',')"/>
							</xsl:when>
							<xsl:when test ="not(contains($tempPrintRange,','))">
								<xsl:value-of select ="$tempPrintRange"/>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test ="contains($range,':')">
							<xsl:value-of select ="concat(substring-before($range,':'),':.',substring-after($range,':'))"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="$range"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:call-template name ="getCorrectRange">
					<xsl:with-param  name ="PrintRange" select="substring-after($tempPrintRange,',')"/>
					<xsl:with-param name ="finalRange" select ="concat($FinalPrintRange,' ',$tempFinalRange)"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select ="translate(translate(substring-after($FinalPrintRange,' '),'$',''),'!','.')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
  <!--for ODF 1.1-->
	<xsl:template name ="getCorrectNamedRange">
		<xsl:param name ="NamedRange"/>
		<xsl:param name ="finalRange"/>
		<xsl:variable name ="tempNamedRange">
			<xsl:value-of select ="$NamedRange"/>
		</xsl:variable>
		<xsl:variable name ="FinalNamedRange">
			<xsl:value-of select ="$finalRange"/>
		</xsl:variable>
		<xsl:variable name ="apos">
			<xsl:text>&apos;</xsl:text>
		</xsl:variable>
		<xsl:choose>
	
			<xsl:when test ="$tempNamedRange !=''">
				<xsl:variable name ="tempFinalRange">
					<xsl:variable name ="range">
						<xsl:choose>
							<xsl:when test ="contains($tempNamedRange,';')">
								<xsl:value-of select ="substring-before($tempNamedRange,';')"/>
							</xsl:when>
							<xsl:when test ="not(contains($tempNamedRange,';'))">
								<xsl:value-of select ="$tempNamedRange"/>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test ="contains($range,':')">
							<xsl:variable name ="beginEndRowCol">
								<xsl:call-template name ="translateRangetoODF">
									<xsl:with-param name ="range" select ="$range"/>
								</xsl:call-template>
							</xsl:variable>
							<xsl:variable name ="sheetName">
								<xsl:value-of select="translate(substring-before($range,'.'),$apos,'')"/>
							</xsl:variable>
							<xsl:variable name ="beginCol">
								<xsl:value-of select ="substring-before($beginEndRowCol,'|')"/>
							</xsl:variable>
							<xsl:variable name ="beginRow">
								<xsl:value-of select ="substring-before(substring-after($beginEndRowCol,concat($beginCol,'|')),'|')"/>
							</xsl:variable>
							<xsl:variable name ="endCol">
								<xsl:value-of select ="substring-before(substring-after($beginEndRowCol,concat($beginCol,'|',$beginRow,'|')),'|')"/>
							</xsl:variable>
							<xsl:variable name ="endRow">
								<xsl:value-of select ="substring-after($beginEndRowCol,concat($beginCol,'|',$beginRow,'|',$endCol,'|'))"/>
							</xsl:variable>
							<xsl:value-of select ="concat('[$',$sheetName,'.$',$beginCol,'$',$beginRow,':.$',$endCol,'$',$endRow,']')"/>
						</xsl:when>
						<xsl:otherwise>
	
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
				<xsl:call-template name ="getCorrectNamedRange">
					<xsl:with-param  name ="NamedRange" select="substring-after($tempNamedRange,';')"/>
					<xsl:with-param name ="finalRange" select ="concat($FinalNamedRange,';',$tempFinalRange)"/>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<!--<xsl:value-of select ="translate(translate(substring-after($FinalPrintRange,' '),'$',''),'!','.')"/>-->
				<xsl:value-of select ="substring-after($FinalNamedRange,';')"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
	
  <!--for ODF 1.1-->
	<xsl:template name ="translateRangetoODF">
		<xsl:param name ="range"/>
		<xsl:variable name ="PartOne">
			<xsl:value-of select ="substring-before($range,':')"/>
		</xsl:variable>
		<xsl:variable name ="PartTwo">
			<xsl:value-of select ="substring-after($range,':')"/>
		</xsl:variable>
		<xsl:variable name ="beginColOrRow">
			<xsl:choose>
				<xsl:when test ="contains(substring-after($PartOne,'$'),'$')">
					<xsl:value-of select ="'RCpresent'"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:variable name ="character">
						<xsl:value-of select ="substring-after($PartOne,'$')"/>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="contains($character,'A') or contains($character,'B') or contains($character,'C') or contains($character,'D') or contains($character,'E') or contains($character,'F') or contains($character,'G') 
                                            or contains($character,'H') or contains($character,'I') or contains($character,'J') or contains($character,'K') or contains($character,'L') or contains($character,'M') or contains($character,'N') or contains($character,'O') 
                                            or contains($character,'P') or contains($character,'Q') or contains($character,'R') or contains($character,'S') or contains($character,'T') or contains($character,'U') or contains($character,'V') or contains($character,'W') 
                                            or contains($character,'X') or contains($character,'Y') or contains($character,'Z') or contains($character,'a') or contains($character,'b') or contains($character,'c') or contains($character,'d') or contains($character,'e')
                                            or contains($character,'f') or contains($character,'g') or contains($character,'h') or contains($character,'i') or contains($character,'j') or contains($character,'k') or contains($character,'l') or contains($character,'m')
                                            or contains($character,'n') or contains($character,'o') or contains($character,'p') or contains($character,'q') or contains($character,'r') or contains($character,'s') or contains($character,'t') or contains($character,'u')
                                            or contains($character,'v') or contains($character,'w') or contains($character,'x') or contains($character,'y') or contains($character,'z')">
							<xsl:value-of select ="'true'"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="'false'"/>
						</xsl:otherwise>
					</xsl:choose>
					<!--<xsl:value-of select ="substring-after($PartOne,'$')"/>-->
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="endColOrRow">
			<xsl:choose >
				<xsl:when test ="contains(substring-after($PartTwo,'$'),'$')">
					<xsl:value-of select ="'RCpresent'"/>
				</xsl:when>
				<xsl:otherwise >
					<xsl:variable name ="character">
						<xsl:value-of select ="substring-after($PartTwo,'$')"/>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test="contains($character,'A') or contains($character,'B') or contains($character,'C') or contains($character,'D') or contains($character,'E') or contains($character,'F') or contains($character,'G') 
                                            or contains($character,'H') or contains($character,'I') or contains($character,'J') or contains($character,'K') or contains($character,'L') or contains($character,'M') or contains($character,'N') or contains($character,'O') 
                                            or contains($character,'P') or contains($character,'Q') or contains($character,'R') or contains($character,'S') or contains($character,'T') or contains($character,'U') or contains($character,'V') or contains($character,'W') 
                                            or contains($character,'X') or contains($character,'Y') or contains($character,'Z') or contains($character,'a') or contains($character,'b') or contains($character,'c') or contains($character,'d') or contains($character,'e')
                                            or contains($character,'f') or contains($character,'g') or contains($character,'h') or contains($character,'i') or contains($character,'j') or contains($character,'k') or contains($character,'l') or contains($character,'m')
                                            or contains($character,'n') or contains($character,'o') or contains($character,'p') or contains($character,'q') or contains($character,'r') or contains($character,'s') or contains($character,'t') or contains($character,'u')
                                            or contains($character,'v') or contains($character,'w') or contains($character,'x') or contains($character,'y') or contains($character,'z')">
							<xsl:value-of select ="'true'"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="'false'"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="beginCol">
			<xsl:choose>
				<xsl:when test ="$beginColOrRow='RCpresent'">
					<xsl:value-of select ="substring-before(substring-after($PartOne,'$'),'$')"/>
				</xsl:when>
				<xsl:when test ="$beginColOrRow='true'">
					<xsl:value-of select ="substring-after($PartOne,'$')"/>
				</xsl:when>
				<xsl:when test ="$beginColOrRow='false'">
					<xsl:value-of select ="'A'"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="endCol">
			<xsl:choose>
				<xsl:when test ="$endColOrRow='RCpresent'">
					<xsl:value-of select ="substring-before(substring-after($PartTwo,'$'),'$')"/>
				</xsl:when>
				<xsl:when test ="$endColOrRow='true'">
					<xsl:value-of select ="substring-after($PartTwo,'$')"/>
				</xsl:when>
				<xsl:when test ="$endColOrRow='false'">
					<xsl:value-of select ="'IV'"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="beginRow">
			<xsl:choose>
				<xsl:when test ="$beginColOrRow='RCpresent'">
					<xsl:value-of select ="substring-after(substring-after($PartOne,'$'),'$')"/>
				</xsl:when>
				<xsl:when test ="$beginColOrRow='true'">
					<xsl:value-of select ="1"/>
				</xsl:when>
				<xsl:when test ="$beginColOrRow='false'">
					<xsl:value-of select ="substring-after($PartOne,'$')"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name ="endRow">
			<xsl:choose>
				<xsl:when test ="$endColOrRow='RCpresent'">
					<xsl:value-of select ="substring-after(substring-after($PartTwo,'$'),'$')"/>
				</xsl:when>
				<xsl:when test ="$endColOrRow='true'">
					<xsl:value-of select ="65536"/>
				</xsl:when>
				<xsl:when test ="$endColOrRow='false'">
					<xsl:value-of select ="substring-after($PartTwo,'$')"/>
				</xsl:when>
			</xsl:choose>
		</xsl:variable>
		<xsl:value-of select ="concat($beginCol,'|',$beginRow,'|',$endCol,'|',$endRow)"/>
	</xsl:template>
</xsl:stylesheet>
