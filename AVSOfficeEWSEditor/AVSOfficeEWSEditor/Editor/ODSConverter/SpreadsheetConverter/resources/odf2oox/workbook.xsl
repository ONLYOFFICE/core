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
RefNo-1 21-Oct-2008 Sandeep s     2171834   Changes done to fix frezpane deffect(Configuraion details retrived using name, insted of possition)
RefNo-2 23-Jan-2009 Sandeep S     1828899       Changes done to replace Named range 'C' and 'R' with _C and _R. 
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:pivotTable"
  exclude-result-prefixes="office style table config text pxsi">

  <!--<xsl:import href="worksheets.xsl"/>
  <xsl:import href="cell.xsl"/>
  <xsl:import href="sharedStrings.xsl"/>
  <xsl:import href="common.xsl"/>
  <xsl:import href="ole_objects.xsl"/>
  <xsl:import href="change_tracking.xsl"/>-->

  <xsl:key name="ConfigItem" match="config:config-item" use="@config:name"/>
  <xsl:key name="style" match="style:style" use="@style:name"/>
  <xsl:key name="pageStyle" match="style:page-layout" use="@style:name"/>
  <xsl:key name="styleMap" match="style:map" use="''"/>
  <xsl:key name="table" match="table:table" use="@table:name"/>

  <!-- main workbook template-->
  <xsl:template name="InsertWorkbook">
    <xsl:apply-templates select="document('content.xml')/office:document-content"/>
  </xsl:template>

  <!-- workbook body template -->
  <xsl:template match="office:body">
    <workbook conformance="transitional">
      <xsl:call-template name="WorkbookView"/>
      <xsl:apply-templates select="office:spreadsheet"/>
    </workbook>
  </xsl:template>

  <!-- workbook  view template-->
  <xsl:template name="WorkbookView">
    <bookViews>
      <workbookView>

        <!-- Insert firstSheet attribute when first sheet is hidden -->
        <xsl:if
          test="key('style',office:spreadsheet/table:table[position()=1]/@table:style-name)/style:table-properties/@table:display = 'false'">
        
            <xsl:variable name="TableStyleName">
              <xsl:value-of
                select="office:spreadsheet/table:table[key('style',@table:style-name)/style:table-properties/@table:display != 'false']/@table:style-name"
              />
            </xsl:variable>
            <xsl:variable name="TableCount">
            <xsl:for-each
              select="office:spreadsheet/table:table[@table:style-name=$TableStyleName][position()=1]">
              <xsl:value-of select="count(preceding-sibling::table:table)"/>
            </xsl:for-each>
            </xsl:variable>
            <xsl:if test="$TableCount != ''">
              <xsl:attribute name="firstSheet">
                    <xsl:value-of select="$TableCount"/>
              </xsl:attribute>
            </xsl:if>
        </xsl:if>

        <!-- Insert activeTab (Active sheet after open the file) -->
        <xsl:attribute name="activeTab">
          <xsl:variable name="ActiveTable">
            <xsl:for-each select="document('settings.xml')">
              <xsl:value-of select="key('ConfigItem', 'ActiveTable')"/>
            </xsl:for-each>
          </xsl:variable>
          <xsl:choose>
            <xsl:when test="office:spreadsheet/table:table[@table:name=$ActiveTable]">
              <xsl:for-each select="office:spreadsheet/table:table[@table:name=$ActiveTable]">
                <xsl:value-of select="count(preceding-sibling::table:table)"/>
              </xsl:for-each>
            </xsl:when>
            <xsl:otherwise>
              <!--SP2MustFix condition added by sonata for bug no:2654544 -->
              <xsl:choose>
                <xsl:when test="key('style',office:spreadsheet/table:table[position()=1]/@table:style-name)/style:table-properties/@table:display = 'false'">
                  <xsl:value-of select="1"/>
                </xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
          </xsl:choose>
              <!--end-->
            
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>

      </workbookView>
    </bookViews>
  </xsl:template>

  <!-- insert references to all sheets -->
  <xsl:template match="office:spreadsheet">

    <!-- Add-in message about sheet name length -->

    <xsl:for-each select="table:table">

      <xsl:variable name="SheetName">
        <xsl:call-template name="CheckSheetName">
          <xsl:with-param name="sheetNumber">
            <xsl:value-of select="position()"/>
          </xsl:with-param>
          <xsl:with-param name="name">
            <xsl:value-of
              select="translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;)"
            />
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="SheetNameWithForbiddenChars">
        <xsl:call-template name="CheckSheetName">
          <xsl:with-param name="sheetNumber">
            <xsl:value-of select="position()"/>
          </xsl:with-param>
          <xsl:with-param name="name">
            <xsl:value-of select="@table:name"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:if
        test="(string-length($SheetNameWithForbiddenChars) - string-length($SheetName) &gt; 0) or (string-length($SheetName) &gt; 31)">
        <xsl:message terminate="no">translation.odf2oox.SheetNameLength</xsl:message>
      </xsl:if>

    </xsl:for-each>

    <sheets>
      <xsl:for-each select="table:table">
        <xsl:if test="not(table:scenario)">
          <sheet>
            <!-- characters "*\/[];'?" can not occur in sheet name -->
            <xsl:attribute name="name">
              <!-- if there is a sheet with the same name modify name -->
              <xsl:call-template name="CheckSheetName">
                <xsl:with-param name="sheetNumber">
                  <xsl:value-of select="position()"/>
                </xsl:with-param>
                <xsl:with-param name="name">
                  <xsl:value-of
                    select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
                  />
                </xsl:with-param>
              </xsl:call-template>
            </xsl:attribute>
            <xsl:attribute name="sheetId">
              <xsl:value-of select="position()"/>
            </xsl:attribute>
            <xsl:attribute name="r:id">
              <xsl:value-of select="generate-id(.)"/>
            </xsl:attribute>
            <xsl:if
              test="key('style',@table:style-name)/style:table-properties/@table:display = 'false'">
              <xsl:attribute name="state">
                <xsl:text>hidden</xsl:text>
              </xsl:attribute>
            </xsl:if>
          </sheet>
        </xsl:if>
      </xsl:for-each>
    </sheets>

    <!--<xsl:if test="table:table/table:shapes/draw:frame/draw:object">-->
	  <!--Defect Id       :1784784
			* Code Changed by :Vijayeta Tilak
			* Date            :26th Dec '07
			* Description     :This part of code( an additional '/') was added because when a notepad is inserted as an object, an addional elemnt
			*                  'table:shapes' is present which is absent when anyother object such as a word doc is inseretd
	  -->
	  <xsl:if test="table:table//draw:frame/draw:object">
		  <xsl:call-template name="ExternalReference"/>
	  </xsl:if>

    <definedNames>
      <xsl:call-template name="InsertPrintRanges"/>
      <xsl:call-template name="InsertHeaders"/>
      <xsl:for-each select="table:table">
        <xsl:call-template name="InsertDefineConnection"/>
      </xsl:for-each>
      <!--definedName name="praca" localSheetId="0">Sheet1!$C$11:$D$11</definedName-->
			
			<!--Defect Id     : 1768233
			* Code Changed by : Sateesh
			* Date            : 17th Jun '08
			* Bug Description :Filters dropdown list conversion not proper-office XP/2003-->

			<xsl:for-each
			  select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:database-ranges/table:database-range ">
				<definedName name="_xlnm._FilterDatabase" localSheetId="0" hidden="1">
					<!--Changes By : Vijayeta
						Date       : 28th Jan '09
						Desc       : SP2,If @table:target-range-address does not contain ':'
						File       : Uni_Chart_Privatization.ods
					 -->					
					<!--<xsl:variable name="rangeAddress">
						<xsl:value-of select="@table:target-range-address"/>
					</xsl:variable>
					<xsl:variable name="partOne">
						<xsl:value-of select="substring-before(substring-before(@table:target-range-address,':'),'.')"/>	
					</xsl:variable>
					<xsl:variable name="partTwo">
						<xsl:value-of select="substring-after(substring-before(@table:target-range-address,':'),'.')"/>
					</xsl:variable>
					<xsl:variable name="partThree">
						<xsl:value-of select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
					</xsl:variable>
					
					<xsl:variable name="firstColValue">
						<xsl:value-of select="translate($partTwo,'1234567890','')"/>
					</xsl:variable>
					<xsl:variable name="firstRowValue">
						<xsl:value-of select="translate($partTwo,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','')"/>
					</xsl:variable>

					<xsl:variable name="secondColValue">
						<xsl:value-of select="translate($partThree,'1234567890','')"/>
					</xsl:variable>
					<xsl:variable name="secondRowValue">
						<xsl:value-of select="translate($partThree,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','')"/>
					</xsl:variable>
					
					<xsl:value-of select="concat($partOne,'!','$',$firstColValue,'$',$firstRowValue,':','$',$secondColValue,'$',$secondRowValue)"/>-->
					<xsl:variable name ="apos">
						<xsl:text>&apos;</xsl:text>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test ="contains(@table:target-range-address,':')">
							<xsl:variable name="partOne">
								<xsl:value-of select="translate(substring-before(substring-before(@table:target-range-address,':'),'.'),$apos,'')"/>
							</xsl:variable>
							<xsl:variable name="partTwo">
								<xsl:value-of select="translate(substring-after(substring-before(@table:target-range-address,':'),'.'),$apos,'')"/>
							</xsl:variable>
							<xsl:variable name="partThree">
								<xsl:value-of select="substring-after(substring-after(@table:target-range-address,':'),'.')"/>
							</xsl:variable>
							<xsl:variable name="firstColValue">
								<xsl:value-of select="translate($partTwo,'1234567890','')"/>
							</xsl:variable>
							<xsl:variable name="firstRowValue">
								<xsl:value-of select="translate($partTwo,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','')"/>
							</xsl:variable>
							<xsl:variable name="secondColValue">
								<xsl:value-of select="translate($partThree,'1234567890','')"/>
							</xsl:variable>
							<xsl:variable name="secondRowValue">
								<xsl:value-of select="translate($partThree,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','')"/>
							</xsl:variable>
					<xsl:value-of select="concat($apos,$partOne,$apos,'!','$',$firstColValue,'$',$firstRowValue,':','$',$secondColValue,'$',$secondRowValue)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:variable name="partOne">
								<xsl:value-of select="translate(substring-before(@table:target-range-address,'.'),$apos,'')"/>
							</xsl:variable>
							<xsl:variable name="partTwo">
								<xsl:value-of select="substring-after(@table:target-range-address,'.')"/>
							</xsl:variable>
							<xsl:variable name="firstColValue">
								<xsl:value-of select="translate($partTwo,'1234567890','')"/>
							</xsl:variable>
							<xsl:variable name="firstRowValue">
								<xsl:value-of select="translate($partTwo,'ABCDEFGHIJKLMNOPQRSTUVWXYZ','')"/>
							</xsl:variable>
							<xsl:value-of select="concat($apos,$partOne,$apos,'!','$',$firstColValue,'$',$firstRowValue)"/>
						</xsl:otherwise>
					</xsl:choose>					
				</definedName>
			</xsl:for-each>
		<!--
		  Feature: Named Ranges
		  By     : Vijayeta
		  Date   :11th Sept '08
		  -->
		<xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:named-expressions">
			<xsl:for-each select ="child::node()">
				<!--<xsl:if test ="not(contains(@table:name,'Excel_BuiltIn_Print_Area')) and not(contains(@table:name,'Excel_BuiltIn_Print_Titles_1 1'))  and @table:name!='C' and @table:name!='c' and @table:name!='R' and @table:name!='r' ">-->
				<xsl:variable name ="definedTableName">
					<xsl:choose>
						<xsl:when test ="contains(@table:name,'Excel_BuiltIn_Print_Area')">
							<xsl:value-of select ="concat('_',position(),@table:name)"/>
						</xsl:when>
						<xsl:when test ="@table:name = 'C' or @table:name = 'c' or @table:name = 'R' or @table:name = 'r'">
							<!--RefNo-2:Removed possition() from string:To replace named range C n R with _C n _R-->
							<xsl:value-of select ="concat('_',@table:name)"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="@table:name"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
					<xsl:choose>
						<xsl:when test ="not(contains(@table:name,'Excel_BuiltIn_Print_Area')) and not(contains(@table:name,'Excel_BuiltIn_Print_Titles_1 1')) and not(contains(@table:cell-range-address,'http://'))">
					<xsl:variable name ="isFunction">
						<xsl:choose>
							<xsl:when test="name()='table:named-range'">
								<xsl:value-of select ="'false'"/>
							</xsl:when>
							<xsl:when test ="name()='table:named-expression'">
								<xsl:value-of select ="'true'"/>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name ="range">
						<xsl:choose>
							<xsl:when test="$isFunction='false'">
								<xsl:choose>
									<xsl:when test ="starts-with(@table:cell-range-address,'$')">
										<xsl:value-of select ="substring-after(@table:cell-range-address,'$')"/>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select ="@table:cell-range-address"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:when test="$isFunction='true'">
								<xsl:value-of select ="@table:expression"/>
							</xsl:when>
						</xsl:choose>
					</xsl:variable>
					<xsl:variable name ="namedRange">
						<xsl:choose>
							<xsl:when test ="substring-after($range,':')!='' and not(contains($range,'file:///')) ">
								<xsl:variable name ="part1">
									<xsl:choose>
										<xsl:when test="$isFunction='false'">
											<xsl:value-of select ="translate(substring-before($range,':'),'.','!')"/>
										</xsl:when>
										<xsl:when test="$isFunction='true'">
											<xsl:choose>
												<xsl:when test ="contains($range,'(')  and contains($range,')')">
													<xsl:value-of select ="concat(translate(substring-before($range,'$'),'[',''),translate(substring-after(substring-before($range,':'),'$'),'.','!'))"/>
												</xsl:when>
												<xsl:otherwise>

												</xsl:otherwise>
											</xsl:choose>
										</xsl:when>
									</xsl:choose>
								</xsl:variable>
								<xsl:variable name ="part2">
									<xsl:choose>
										<xsl:when test="$isFunction='false'">
											<xsl:choose>
												<xsl:when test ="contains(substring-after($range,':'),'.')">
													<xsl:value-of select ="substring-after(substring-after($range,':'),'.')"/>
												</xsl:when>
												<xsl:otherwise>
													<xsl:value-of select ="substring-after($range,':')"/>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:when>
										<xsl:when test="$isFunction='true'">
											<xsl:choose>
												<xsl:when test ="contains(substring-after($range,':'),'.')">
													<xsl:value-of select ="translate(substring-after(substring-after($range,':'),'.'),']','')"/>
												</xsl:when>
												<xsl:otherwise>
													<xsl:value-of select ="translate(substring-after($range,':'),']','')"/>
												</xsl:otherwise>
											</xsl:choose>
										</xsl:when>
									</xsl:choose>
								</xsl:variable>
								<!--
									Defect : 2165767
									Desc:    Name ranges not retained as the input file had no proper representtaion
								-->
								<xsl:choose>
									<xsl:when test ="contains($part1,'#') or contains($part1,'file:///')">
										<xsl:text>"</xsl:text>
										<xsl:value-of select ="concat($part1,':',$part2)"/>
										<xsl:text>"</xsl:text>
									</xsl:when>
									<xsl:when test ="not(contains($range,'#') or contains($part1,'file:///'))">
										<xsl:choose>
											<xsl:when test ="substring-before($part1,'!')!=''">
										<xsl:value-of select ="concat($part1,':',$part2)"/>
									</xsl:when>
											<xsl:otherwise>
												<xsl:variable name ="baseSheet">
													<xsl:value-of select ="translate(substring-before(@table:base-cell-address,'.'),'$','')"/>
												</xsl:variable>
												<xsl:value-of select ="concat($baseSheet,$part1,':',$part2)"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:choose>
									<xsl:when test ="contains($range,'#') or contains($range,'file:///')">
										<xsl:text>"</xsl:text>
										<xsl:value-of select ="translate($range,'.','!')"/>
										<xsl:text>"</xsl:text>
									</xsl:when>
									<xsl:when test ="not(contains($range,'#') or contains($range,'file:///'))">
										<xsl:value-of select ="translate($range,'.','!')"/>
									</xsl:when>
								</xsl:choose>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:variable>
					<xsl:if 
						test ="$isFunction='false' or ($isFunction='true' and contains(@table:expression,'(') and contains(@table:expression,')')
						and not((contains(@table:expression,'ARABIC(')) 
                    or (contains(@table:expression,'DDE('))
                    or (contains(@table:expression,'JIS('))
                    or (contains(@table:expression,'SHEET('))
                    or (contains(@table:expression,'SHEETS('))
                    or (contains(@table:expression,'STYLE('))
                    or (contains(@table:expression,'MUNIT(')) 
                    or (contains(@table:expression,'DURATION(') and not(contains(@table:expression,'getDuration(')))                    
                    or (contains(@table:expression,'ACOT('))
                    or (contains(@table:expression,'ACOTH('))
                    or (contains(@table:expression,'COT('))
                    or (contains(@table:expression,'COTH('))
                    or (contains(@table:expression,'BASE(')) 
                    or (contains(@table:expression,'B(') and not(contains(@table:expression,'DB(')) and not(contains(@table:expression,'DDB(')) and not(contains(@table:expression,'PROB(')))
                    or (contains(@table:expression,'COMBINA('))
                    or (contains(@table:expression,'CURRENT(')) 
                    or (contains(@table:expression,'DAYS('))
                    or (contains(@table:expression,'getDaysInMonth(')) 
                    or (contains(@table:expression,'getDaysInYear(')) 
                    or (contains(@table:expression,'DDE('))
                    or (contains(@table:expression,'EASTERSUNDAY(')) 
                    or (contains(@table:expression,'EFFECTIVE(')) 
                    or (contains(@table:expression,'FORMULA('))
                    or (contains(@table:expression,'GAUSS(')) 
                    or (contains(@table:expression,'ISFORMULA(')) 
                    or (contains(@table:expression,'getIsLeapYear('))
                    or (contains(@table:expression,'getDiffMonths('))
                    or (contains(@table:expression,'PERMUTATIONA(')) 
                    or (contains(@table:expression,'PHI('))
                    or (contains(@table:expression,'getRot13('))
                    or (contains(@table:expression,'CONVERT(') and not(contains(@table:expression,'getConvert(')))
                    or (contains(@table:expression,'WEEKNUM(') and not(contains(@table:expression,'getWeeknum(')))
                    or (contains(@table:expression,'getDiffWeeks('))
                    or (contains(@table:expression,'getWeeksInYear('))
                    or (contains(@table:expression,'getDiffYears('))
                    or (contains(@table:expression,'DECIMAL('))
                    or (contains(@table:expression,'ZGZ('))
					or (contains(@table:expression,'IF('))
					or (contains(@table:expression,'OFFSET('))
                    or (contains(@table:expression,'NA('))))">
						<definedName>						
							<xsl:attribute name ="name">
								<xsl:value-of select ="$definedTableName"/>
							</xsl:attribute>
							<!--<xsl:value-of select ="$namedRange"/>-->
							<xsl:choose>
								<xsl:when test="$isFunction='false'">
									<xsl:value-of select ="$namedRange"/>
								</xsl:when>
								<xsl:when test="$isFunction='true'">
									<xsl:variable name ="namedRange1">
										<!--<xsl:choose >
										<xsl:when test ="contains(@table:expression,'(') and contains(@table:expression,')')">-->
										<xsl:value-of select ="concat(substring-before(@table:expression,'$'),substring-after(@table:expression,'$'))"/>
										<!--</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select ="''"/>
										</xsl:otherwise>
									</xsl:choose>-->
									</xsl:variable>
									<!--<xsl:if test ="	$namedRange1!=''">-->

									<!--</xsl:if>-->
									<!--<xsl:value-of select="concat('sonataOdfFormulaoooc:=', $namedRange1)"/>-->
									<xsl:value-of select="concat('sonataOdfFormulaoooc:=', $namedRange1)"/>
								</xsl:when>
							</xsl:choose>
						</definedName>
					</xsl:if>
						</xsl:when>
						<xsl:when test ="contains(@table:cell-range-address,'http://')">
							<xsl:if test ="contains(@table:cell-range-address,'#')">
								<xsl:variable name ="apos">
									<xsl:text>&apos;</xsl:text>
								</xsl:variable>
								<xsl:variable name ="url">
									<xsl:value-of select ="translate(substring-before(@table:cell-range-address,'#'),$apos,'')"/>
								</xsl:variable>
								<xsl:variable name ="fileName">
									<xsl:call-template name ="getFileNameFromUrl">
										<xsl:with-param name ="url" select ="substring-after($url,'http://')"/>
									</xsl:call-template>
								</xsl:variable>
								<xsl:variable name ="urlNoFileName">
									<xsl:value-of select ="substring-before($url,$fileName)"/>
								</xsl:variable>															
								<xsl:variable name ="cellRange">
									<xsl:value-of select ="substring-after(@table:cell-range-address,'#')"/>
								</xsl:variable>
								<definedName>
									<xsl:attribute name ="name">
										<xsl:value-of select ="$definedTableName"/>
									</xsl:attribute>
									<xsl:choose >
										<xsl:when test ="contains($cellRange,':')">
											<xsl:value-of select ="concat($apos,$urlNoFileName,'[',$fileName,']',concat(substring-before($cellRange,'.'),concat($apos,'!'),
														  substring-before(substring-after($cellRange,'.'),':'),':',substring-after(substring-after($cellRange,':'),'.')))"/>
										</xsl:when>
										<xsl:otherwise>
											<xsl:value-of select ="concat($apos,$urlNoFileName,'[',$fileName,']',concat(substring-before($cellRange,'.'),concat($apos,'!'),substring-after($cellRange,'.')))"/>
										</xsl:otherwise>
									</xsl:choose>
									
								</definedName>
				</xsl:if>
						</xsl:when>
					</xsl:choose>

			</xsl:for-each>
		</xsl:for-each>
		<!-- end of code for the feature 'Named Ranges'-->
		<!--End-->
    </definedNames>

    <xsl:for-each select="table:data-pilot-tables">

      <pivotCaches>

        <xsl:for-each select="table:data-pilot-table">
          <xsl:variable name="apos">'</xsl:variable>
          <xsl:variable name="sheetName">
            <xsl:value-of select="translate(substring-before(table:source-cell-range/@table:cell-range-address,'.'),$apos,'')"/>
          </xsl:variable>
          <xsl:variable name="cellAddress">
            <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
          </xsl:variable>
		<!--
		Defect: 1894250
		Fixed By: Vijayeta
		Desc : Performance related defect, where in the pivot range of the source table(table:cell-range-address) in input is row num 65536,
			   where as the source table spans only upto 1089 rows. Hence, the code iterates 65536 times, and resulting in long conversion time.
			   Here this part of code takes a count of rows in source table and compares it with the value of attribute table:cell-range-address.
		Date: 10th Sep '08	
		variables: rowCount,columnForRowCount,rowNumCellRange
		-->
			<xsl:variable name ="rowCount">
				<xsl:call-template name ="getTableRowCount">
					<xsl:with-param name="sheetName">
						<xsl:value-of select="$sheetName"/>
					</xsl:with-param>
					<xsl:with-param name="cellAddress">
						<xsl:value-of select="$cellAddress"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:variable>
			<xsl:variable name ="columnForRowCount">
						<xsl:if test ="$cellAddress != ''">
				<xsl:call-template name="NumbersToChars">
					<xsl:with-param name="num">
						<xsl:variable name ="number">
							<xsl:call-template name="GetColNum">
								<xsl:with-param name="cell" select="substring-after(substring-after($cellAddress,':'),'.')"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:value-of select ="$number - 1"/>
					</xsl:with-param>
				</xsl:call-template>
						</xsl:if>
			</xsl:variable>
			<xsl:variable name ="rowNumCellRange">
						<xsl:if test ="$cellAddress != ''">
				<xsl:call-template name="GetRowNum">
					<xsl:with-param name="cell" select="substring-after(substring-after($cellAddress,':'),'.')"/>
				</xsl:call-template>
						</xsl:if>
			</xsl:variable>
          <xsl:variable name="CreatePivotTable">
            <xsl:for-each select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[@table:name=$sheetName]">
              <xsl:apply-templates select="table:table-row[1]" mode="checkPivotCells">
                <xsl:with-param name="rowNumber">1</xsl:with-param>
                <xsl:with-param name="cellStart">
                  <xsl:value-of select="substring-before(substring-after($cellAddress,'.'),':')"/>
                </xsl:with-param>
                <xsl:with-param name="cellEnd">
					  <xsl:choose>
						  <xsl:when test ="$rowNumCellRange &gt; $rowCount ">
							  <xsl:value-of select ="concat($columnForRowCount,$rowCount)"/>
						  </xsl:when>
						  <xsl:otherwise>
                  <xsl:value-of select="substring-after(substring-after($cellAddress,':'),'.')"/>
						  </xsl:otherwise>
					  </xsl:choose>
					  <!--<xsl:value-of select="substring-after(substring-after($cellAddress,':'),'.')"/>-->
                </xsl:with-param>
              </xsl:apply-templates>
            </xsl:for-each>
          </xsl:variable>
          <xsl:if test="$CreatePivotTable != 'false'">
            <xsl:variable name="pivotSource">
              <xsl:value-of select="table:source-cell-range/@table:cell-range-address"/>
            </xsl:variable>
            
            <!-- do not duplicate the same source range cache -->
            <xsl:if
              test="not(preceding-sibling::table:data-pilot-table[table:source-cell-range/@table:cell-range-address = $pivotSource])">
              
              <pivotCache cacheId="{count(preceding-sibling::table:data-pilot-table)}"
                r:id="{generate-id()}"/>
              
            </xsl:if>
          </xsl:if>
        </xsl:for-each>
      </pivotCaches>

    </xsl:for-each>

  </xsl:template>

  <!-- insert all sheets -->
  <xsl:template name="InsertSheets">
	  <xsl:for-each select="document('content.xml')">
    <xsl:variable name="multilines">
      <xsl:for-each
				select="office:document-content/office:body/office:spreadsheet">
        <xsl:value-of select="count(descendant::table:table-cell/text:p[2])"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="hyperlinkStyle">
      <xsl:for-each
        select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = 'Hyperlink' ]">
        <xsl:value-of select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"
        />
      </xsl:for-each>
    </xsl:variable>


    <xsl:variable name="cellFormats">
      <xsl:value-of
				select="count(office:document-content/office:automatic-styles/style:style[@style:family='table-cell']) + 1"
      />
    </xsl:variable>

    <xsl:variable name="cellStyles">
      <xsl:value-of
				select="count(office:document-styles/office:styles/style:style[@style:family='table-cell'])"
      />
    </xsl:variable>

    <xsl:variable name="CheckIfConditional">
      <xsl:choose>
        <xsl:when
					test="office:document-content/office:automatic-styles/style:style/style:map[@style:condition != '']">
          <xsl:text>true</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>false</xsl:text>
        </xsl:otherwise>
      </xsl:choose>

    </xsl:variable>
    
    <xsl:variable name="contentFontsCount">
      <xsl:value-of
				select="count(office:document-content/office:automatic-styles/style:style)"
      />
    </xsl:variable>
    
    <xsl:variable name="styleFontsCount">
      <xsl:value-of
        select="count(document('styles.xml')/office:document-styles/office:styles/style:style)"
      />
    </xsl:variable>

    <!-- convert first table -->
    <xsl:apply-templates
			select="office:document-content/office:body/office:spreadsheet/table:table[1]"
      mode="sheet">
		  <xsl:with-param name="cellNumber" select ="number(0)"/>
		  <xsl:with-param name="sheetId" select ="number(1)"/>
		<xsl:with-param name="multilines" select ="$multilines"/>
		<xsl:with-param name="hyperlinkStyle" select ="$hyperlinkStyle"/>
		<xsl:with-param name="contentFontsCount" select ="$hyperlinkStyle"/>
		<xsl:with-param name="styleFontsCount" select ="$styleFontsCount"/>
		<xsl:with-param name="cellFormats" select ="$cellFormats"/>
		<xsl:with-param name="cellStyles" select ="$cellStyles"/>
		<xsl:with-param name="CheckIfConditional" select ="$CheckIfConditional"/>
      <!--RefNo-1:Added parm strSheetName-->
    <xsl:with-param name="strSheetName" select ="office:document-content/office:body/office:spreadsheet/table:table[1]/@table:name"/>
    </xsl:apply-templates>
	  </xsl:for-each>
  </xsl:template>

  <xsl:template name="CheckSheetName">
    <xsl:param name="name"/>
    <xsl:param name="sheetNumber"/>

    <xsl:choose>
      <!-- when there are at least 2 sheets with the same name after removal of forbidden characters and cutting to 31 characters (name correction) -->
      <xsl:when
        test="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31) = $name][2]">
        <xsl:variable name="nameConflictsBefore">
          <xsl:for-each
            select="document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[position() = $sheetNumber]">
            <!-- count sheets before this one whose name (after correction) collide with this sheet name (after correction) -->
            <xsl:value-of
              select="count(document('content.xml')/office:document-content/office:body/office:spreadsheet/table:table[substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31) = $name and position() &lt; $sheetNumber])"
            />
          </xsl:for-each>
        </xsl:variable>
        <!-- cut name and add "(N)" at the end where N is seqential number of duplicated name -->
        <xsl:value-of
          select="concat(substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31 - 2 - string-length($nameConflictsBefore + 1)),'_',$nameConflictsBefore + 1)"
        />
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$name"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertPrintRanges">
    <xsl:for-each select="table:table">
      <xsl:if test="@table:print-ranges and key('style',@table:style-name)/style:table-properties/@table:display != 'false'">
        <definedName name="_xlnm.Print_Area">
          <xsl:attribute name="localSheetId">
            <xsl:value-of select="position() - 1"/>
          </xsl:attribute>
          <xsl:call-template name="InsertRange">
            <xsl:with-param name="range" select="@table:print-ranges"/>
          </xsl:call-template>
        </definedName>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertRange">
    <xsl:param name="result"/>
    <xsl:param name="range"/>

    
    <xsl:variable name="PrintRange">
      <xsl:choose>
        <xsl:when test="contains(substring-after($range, '.'), ' ')">
          <xsl:value-of select="concat(substring-before($range, '.'), '.', substring-before(substring-after($range, '.'), ' '))"/>    
        </xsl:when>
        <xsl:when test="$range != ''">
          <xsl:value-of select="$range"/>
        </xsl:when>
      </xsl:choose>
    </xsl:variable>
    
   <xsl:choose>
     <xsl:when test="contains($PrintRange, ':')">
       
       <xsl:variable name="row1">
         <xsl:call-template name="GetRowNum">
           <xsl:with-param name="cell"
             select="concat(substring-after($range,'.'),substring-before($range,':'))"/>
         </xsl:call-template>
       </xsl:variable>

       <xsl:variable name="col1">
         <xsl:value-of select="substring-before(substring-after($range,'.'),$row1)"/>
       </xsl:variable>
       <!-- 
       Defect :1803593, file '03706191.CONFIDENTIAL.xlsx 
		   Changes by: Vijayeta
		   Desc:Many a times, the print range is not present in the correct format, because of which the template 'GetRowNum' for the variable 'endCell' goes into infinite loop,
            and col2 is also not  correctly found. Hence, additional changes to check for
       -->
       <xsl:variable name="PrintRange1">
         <xsl:choose>
           <xsl:when test="contains(substring-after($range, '.'), ' ')">
             <xsl:value-of select="substring-before($range, ' ')"/>
           </xsl:when>
           <xsl:when test="$range != ''">
             <xsl:value-of select="$range"/>
           </xsl:when>
         </xsl:choose>
       </xsl:variable>
       <xsl:variable name ="sheet">
         <xsl:value-of select ="substring-before($PrintRange1,'.')"/>
       </xsl:variable>
       <xsl:variable name="row2">
         <xsl:variable name="endCell">
           <xsl:choose>
             <xsl:when test ="contains(substring-after($PrintRange1,':'),$sheet)">
               <xsl:choose>
                 <!-- when there is next range there is a space before -->
                 <xsl:when test="contains(substring-after(substring-after($range,'.'),'.'),' ')">
                   <xsl:value-of
                     select="substring-before(substring-after(substring-after($range,'.'),'.'),' ')"/>
                 </xsl:when>
                 <xsl:otherwise>
                   <xsl:value-of select="substring-after(substring-after($range,'.'),'.')"/>
                 </xsl:otherwise>
               </xsl:choose>
             </xsl:when>
             <xsl:otherwise >
               <xsl:choose>
                 <!-- when there is next range there is a space before -->
                 <xsl:when test="contains(substring-after(substring-after($range,'.'),':'),' ')">
                   <xsl:value-of
                     select="substring-before(substring-after(substring-after($range,'.'),':'),' ')"/>
                 </xsl:when>
                 <xsl:otherwise>
                   <xsl:value-of select="substring-after(substring-after($range,'.'),':')"/>
                 </xsl:otherwise>
               </xsl:choose>
             </xsl:otherwise>
           </xsl:choose>
         </xsl:variable>
         <xsl:call-template name="GetRowNum">
           <xsl:with-param name="cell" select="$endCell"/>
         </xsl:call-template>
       </xsl:variable>
       <xsl:variable name="col2">
         <xsl:choose>
           <xsl:when test ="contains(substring-after($PrintRange1,':'),$sheet)">
             <xsl:value-of
        select="substring-before(substring-after(substring-after($range,'.'),'.'),$row2)"/>
           </xsl:when>
           <xsl:otherwise >
             <xsl:value-of
        select="substring-before(substring-after(substring-after($range,'.'),':'),$row2)"/>
           </xsl:otherwise>
         </xsl:choose>
       </xsl:variable>       
       <!-- if sheet name contains space then name has to be inside apostrophes -->
       <xsl:variable name="sheetName">
         <xsl:text>'</xsl:text>
         <xsl:call-template name="CheckSheetName">
           <xsl:with-param name="sheetNumber">
             <xsl:value-of select="position()"/>
           </xsl:with-param>
           <xsl:with-param name="name">
             <xsl:value-of
               select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
             />
           </xsl:with-param>
         </xsl:call-template>
         <xsl:text>'</xsl:text>
       </xsl:variable>
		 <!--
		     Defect    :1954140
		     Changes by: Sateesh Reddy		   
         -->
       <xsl:variable name="StringRange">
       <xsl:if test="not(contains($range, '$'))">
         <xsl:variable name="val1">
           <xsl:value-of select="substring-before(substring-after($range,'.'),$row1)"/>
         </xsl:variable>
         <xsl:choose>
           <xsl:when test="val1!=''">
             <xsl:value-of
               select="concat($sheetName,'!$',substring-before(substring-after($range,'.'),$row1),'$', $row1,':','$',$col2,'$', $row2)"/>
           </xsl:when>
           <xsl:otherwise>
             <xsl:variable name="val2">
               <xsl:if test="contains($row1,'.')">
                 <xsl:choose>
                   <xsl:when test="substring-before($row1,'.')!=''">
                     <xsl:value-of select="substring-before($row1,'.')"/>
                   </xsl:when>
                   <xsl:when test="substring-after($row1,'.')!=''">
                     <xsl:value-of select="substring-after($row1,'.')"/>
                   </xsl:when>
                 </xsl:choose>
               </xsl:if>
             </xsl:variable>
             <xsl:if test="$val2 !=''">
               <xsl:value-of
                 select="concat($sheetName,'!$', $val2,':','$', $row2)"/>
             </xsl:if>
             <xsl:if test="$val2 =''">
         <xsl:value-of
           select="concat($sheetName,'!$',substring-before(substring-after($range,'.'),$row1),'$', $row1,':','$',$col2,'$', $row2)"/>
             </xsl:if>
           </xsl:otherwise>
         </xsl:choose>         
       </xsl:if>
       <xsl:if test="contains($range, '$')">
         <xsl:value-of
           select="concat($sheetName,'!',substring-before(substring-after($range,'.'),$row1),'', $row1,':','',$col2,'', $row2)"
         />
       </xsl:if>
       </xsl:variable>
       
       <!-- if there is next range there is a space before -->
       

         <xsl:call-template name="InsertRange">
           <xsl:with-param name="result">
             <xsl:choose>
               <xsl:when test="$result != ''">
                 <xsl:value-of select="concat($result, ',', $StringRange)"/>    
               </xsl:when>
               <xsl:otherwise>
                 <xsl:value-of select="$StringRange"/>
               </xsl:otherwise>
             </xsl:choose>             
           </xsl:with-param>
           <xsl:with-param name="range">
             <xsl:value-of
               select="substring-after(substring-after($range,'.'), ' ')"/>
           </xsl:with-param>
         </xsl:call-template>
       
       
     </xsl:when>
     
     <xsl:when test="$PrintRange != ''">
       
       <!-- if sheet name contains space then name has to be inside apostrophes -->
       <xsl:variable name="sheetName">
         <xsl:text>'</xsl:text>
         <xsl:call-template name="CheckSheetName">
           <xsl:with-param name="sheetNumber">
             <xsl:value-of select="position()"/>
           </xsl:with-param>
           <xsl:with-param name="name">
             <xsl:value-of
               select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
             />
           </xsl:with-param>
         </xsl:call-template>
         <xsl:text>'</xsl:text>
       </xsl:variable>
       
       <xsl:variable name="row1">
         <xsl:call-template name="GetRowNum">
           <xsl:with-param name="cell"
             select="substring-after($PrintRange,'.')"/>
         </xsl:call-template>
       </xsl:variable>

       <xsl:variable name="StringRange">
         <xsl:if test="not(contains($PrintRange, '$'))">
           <xsl:value-of
             select="concat($sheetName,'!$',substring-before(substring-after($PrintRange,'.'),$row1),'$', $row1)"/>
           
         </xsl:if>
         <xsl:if test="contains($PrintRange, '$')">
           <xsl:choose>
             <xsl:when test="not(contains($PrintRange,':'))">
             <!-- Defect : 1877279 
				          File Name: _TestSuite - DB_Shipwreck.xlsx (on roundtrip)
					        Fix By: Vijayeta
					        Desc: This gives the right print range in case of single cell as print range
             -->
               <!--<xsl:value-of
                 select="concat($sheetName,'!',substring-before(substring-after($PrintRange,'.'),$row1),'', $row1,':$',$row1)"/>-->
               <xsl:value-of 
                 select="concat($sheetName,'!',substring-before(substring-after($PrintRange,'.'),$row1),'', $row1)"/>
             </xsl:when>
             <xsl:otherwise>
           <xsl:value-of
             select="concat($sheetName,'!',substring-before(substring-after($PrintRange,'.'),$row1),'', $row1)"/>
             </xsl:otherwise>
           </xsl:choose>
         </xsl:if>
       </xsl:variable>
  
       
       <!-- if there is next range there is a space before -->
       
       
       <xsl:call-template name="InsertRange">
         <xsl:with-param name="result">
           <xsl:choose>
             <xsl:when test="$result != ''">
               <xsl:value-of select="concat($result, ',', $StringRange)"/>    
             </xsl:when>
             <xsl:otherwise>
               <xsl:value-of select="$StringRange"/>
             </xsl:otherwise>
           </xsl:choose>
         </xsl:with-param>
         <xsl:with-param name="range">
           <xsl:value-of
               select="substring-after(substring-after($range,'.'), ' ')"/>
         </xsl:with-param>
       </xsl:call-template>
              
       
     </xsl:when>
     
     <xsl:otherwise>
		 <!--Defect Id    :1838572
			* Code Added by   :Sateesh Reddy
            * Date            :03 Jan '08
			* Description     :This part of code was added because when printarea value is not coming with '$' sign then file is crashing.
                         To avoid crash I added this code.Here I am checking $sign.
		 -->
       <xsl:choose>
         <xsl:when test="contains($result,',')">
           <!--<xsl:value-of select="substring-before($result,',')"/>-->
           <xsl:variable name="var_Res">
             <xsl:value-of select="substring-after($result,',')"/>
           </xsl:variable>
           <xsl:variable name="var_Result">
             <xsl:value-of select="substring-after($var_Res,':')"/>
           </xsl:variable>
           <xsl:variable name="var_Result1">
             <xsl:value-of select="substring-after($var_Result,'$')"/>
           </xsl:variable>
           <xsl:variable name="var_Result2">
             <xsl:value-of select="substring-after($var_Result1,'$')"/>
           </xsl:variable>
           <xsl:if test="$var_Result2 != ''">
             <xsl:value-of select="$result"/>
           </xsl:if>
           <xsl:if test="$var_Result2 = ''">
           <xsl:value-of select="substring-before($result,',')"/>    
           </xsl:if>
         </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$result"/>
         </xsl:otherwise>
       </xsl:choose>      
       
       </xsl:otherwise>
     
   </xsl:choose>
    
   
    
  </xsl:template>

  <xsl:template name="InsertHeaders">

    <xsl:for-each select="table:table">
      <xsl:if test="table:table-header-rows or table:table-header-columns">
        <definedName name="_xlnm.Print_Titles">
          <xsl:attribute name="localSheetId">
            <xsl:value-of select="position() - 1"/>
          </xsl:attribute>

          <xsl:choose>
            <!-- when there are only header columns -->
            <xsl:when test="table:table-header-columns and not(table:table-header-rows)">
              <xsl:call-template name="InsertHeaderCols"/>
            </xsl:when>
            <!-- when there are only header rows -->
            <xsl:when test="table:table-header-rows and not(table:table-header-columns)">
              <xsl:call-template name="InsertHeaderRows"/>
            </xsl:when>
            <!-- when there are both: header rows and header columns -->
            <xsl:when test="table:table-header-rows and table:table-header-columns">
              <xsl:call-template name="InsertHeaderCols"/>
              <xsl:text>,</xsl:text>
              <xsl:call-template name="InsertHeaderRows"/>
            </xsl:when>
          </xsl:choose>

        </definedName>
      </xsl:if>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="InsertHeaderRows">

    <xsl:variable name="headerRowStart">
      <xsl:variable name="count">
        <xsl:for-each select="descendant::table:table-row[1]">
          <xsl:call-template name="CountHeaderRowsStart"/>
        </xsl:for-each>
      </xsl:variable>

      <xsl:choose>
        <xsl:when test="$count = '' ">
          <xsl:text>1</xsl:text>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$count"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="headerRows">
      <xsl:for-each select="table:table-header-rows/table:table-row[1]">
        <xsl:call-template name="CountRows"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="sheetName">
      <xsl:text>&apos;</xsl:text>
      <xsl:call-template name="CheckSheetName">
        <xsl:with-param name="sheetNumber">
          <xsl:value-of select="position()"/>
        </xsl:with-param>
        <xsl:with-param name="name">
          <xsl:value-of
            select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
          />
        </xsl:with-param>
      </xsl:call-template>
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <xsl:value-of
      select="concat($sheetName,'!$',$headerRowStart,':$',$headerRowStart + $headerRows  - 1)"/>

  </xsl:template>

  <xsl:template name="InsertHeaderCols">

    <xsl:variable name="headerColStart">
      <xsl:for-each select="child::node()[name() != 'office:forms' ][1]">
        <xsl:call-template name="CountHeaderColsStart"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="headerCols">
      <xsl:for-each select="table:table-header-columns/table:table-column[1]">
        <xsl:call-template name="CountCols"/>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="charHeaderColStart">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num" select="$headerColStart - 1"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="charHeaderColEnd">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num" select="$headerColStart + $headerCols  - 2"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="sheetName">
      <xsl:text>&apos;</xsl:text>
      <xsl:call-template name="CheckSheetName">
        <xsl:with-param name="sheetNumber">
          <xsl:value-of select="position()"/>
        </xsl:with-param>
        <xsl:with-param name="name">
          <xsl:value-of
            select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
          />
        </xsl:with-param>
      </xsl:call-template>
      <xsl:text>&apos;</xsl:text>
    </xsl:variable>

    <xsl:value-of select="concat($sheetName,'!$',$charHeaderColStart,':$',$charHeaderColEnd)"/>

  </xsl:template>

  <xsl:template name="CountHeaderRowsStart">
    <xsl:param name="value" select="0"/>
		<!--Vijayeta,SP2,@table:number-rows-repeated-->
		<!--<xsl:variable name="rows">
      <xsl:choose>
        <xsl:when test="@table:number-rows-repeated">
          <xsl:value-of select="@table:number-rows-repeated"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="rows">
      <xsl:choose>
        <xsl:when test="@table:number-rows-repeated">
					<xsl:choose >
						<xsl:when test ="@table:number-rows-repeated &gt; 65536">
							<xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
						</xsl:when>
						<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
          <xsl:value-of select="@table:number-rows-repeated"/>
        </xsl:when>
					</xsl:choose>
				</xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
    <xsl:choose>
      <xsl:when test="name() = 'table:table-row' ">
        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="CountHeaderRowsStart">
            <xsl:with-param name="value" select="$value + $rows"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value + 1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CountHeaderColsStart">
    <xsl:param name="value" select="0"/>
		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<!--<xsl:variable name="cols">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
          <xsl:value-of select="@table:number-columns-repeated"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="cols">
      <!--<xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="@table:number-columns-repeated"/>
        </xsl:when>
					</xsl:choose>
				</xsl:when>
        <xsl:otherwise>
          <xsl:text>1</xsl:text>
        </xsl:otherwise>
      </xsl:choose>-->
		<xsl:variable name ="tempOfficeVersion">
			<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test="@table:number-columns-repeated">
		      <xsl:choose>
			<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
				<xsl:value-of select="@table:number-columns-repeated"/>
			</xsl:when>
			<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
				<xsl:choose>
					<xsl:when test ="@table:number-columns-repeated &gt; 256">
						<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of select="@table:number-columns-repeated"/>
					</xsl:otherwise>
				</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="@table:number-columns-repeated"/>
			</xsl:otherwise>
		</xsl:choose>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>1</xsl:text>
			</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <xsl:choose>
      <xsl:when test="name() = 'table:table-column' ">
        <xsl:for-each select="following-sibling::node()[1]">
          <xsl:call-template name="CountHeaderColsStart">
            <xsl:with-param name="value" select="$value + $cols"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value + 1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="InsertDefineConnection">


    <!-- if there is a shheet with the same name modify name -->
    <xsl:variable name="SheetName">
      <xsl:call-template name="CheckSheetName">
        <xsl:with-param name="sheetNumber">
          <xsl:value-of select="position()"/>
        </xsl:with-param>
        <xsl:with-param name="name">
          <xsl:value-of
            select="substring(translate(@table:name,&quot;*\/[]:&apos;?&quot;,&quot;&quot;),1,31)"
          />
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="SheetId">
      <xsl:value-of select="position() - 1"/>
    </xsl:variable>


    <xsl:apply-templates select="table:table-row[1]" mode="connection">
      <xsl:with-param name="rowNumber">
        <xsl:text>1</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="cellNumber">
        <xsl:text>0</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="SheetId">
        <xsl:value-of select="$SheetId"/>
      </xsl:with-param>
      <xsl:with-param name="tableName" select="@table:name"/>
    </xsl:apply-templates>


  </xsl:template>

  <!-- search coditional -->
  <xsl:template match="table:table-row" mode="connection">
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="SheetId"/>
    <xsl:param name="tableName"/>

    <xsl:apply-templates
      select="child::node()[name() = 'table:table-cell' or name()= 'table:covered-table-cell'][1]"
      mode="connection">
      <xsl:with-param name="colNumber">
        <xsl:text>0</xsl:text>
      </xsl:with-param>
      <xsl:with-param name="rowNumber" select="$rowNumber"/>
      <xsl:with-param name="SheetId">
        <xsl:value-of select="$SheetId"/>
      </xsl:with-param>
      <xsl:with-param name="tableName">
        <xsl:value-of select="$tableName"/>
      </xsl:with-param>
    </xsl:apply-templates>

    <!-- check next row -->
    <xsl:choose>
      <!-- next row is a sibling -->
      <xsl:when test="following-sibling::table:table-row">
        <xsl:apply-templates select="following-sibling::table:table-row[1]" mode="connection">
					<!--Vijayeta,SP2,@table:number-rows-repeated-->
					<!--<xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when test="@table:number-rows-repeated">
                <xsl:value-of select="$rowNumber+@table:number-rows-repeated"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
          <xsl:with-param name="rowNumber">
            <xsl:choose>
              <xsl:when test="@table:number-rows-repeated">
								<xsl:choose >
									<xsl:when test ="@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="$rowNumber+(65536 - (1048576 - @table:number-rows-repeated))"/>
									</xsl:when>
									<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
                <xsl:value-of select="$rowNumber+@table:number-rows-repeated"/>
              </xsl:when>
								</xsl:choose>
							</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$rowNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
          <xsl:with-param name="cellNumber">
            <xsl:text>0</xsl:text>
          </xsl:with-param>
          <xsl:with-param name="SheetId">
            <xsl:value-of select="$SheetId"/>
          </xsl:with-param>
          <xsl:with-param name="tableName">
            <xsl:value-of select="$tableName"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="table:covered-table-cell" mode="connection">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="SheetId"/>
    <xsl:param name="tableName"/>


    <xsl:if test="following-sibling::table:table-cell">
      <xsl:apply-templates
        select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ][1]"
        mode="connection">
				<!--Vijayeta,SP2,@table:number-columns-repeated-->
				<!--<xsl:with-param name="colNumber">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
              <xsl:value-of select="number($colNumber) + number(@table:number-columns-repeated)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$colNumber + 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>-->
        <xsl:with-param name="colNumber">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
				<!--<xsl:choose >
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
								</xsl:when>
								<xsl:when test ="@table:number-columns-repeated &lt;= 256">
									<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
								</xsl:when>
				</xsl:choose>-->
				<xsl:variable name ="tempOfficeVersion">
					<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
					</xsl:when>
					<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						<xsl:choose>
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
					</xsl:otherwise>
							</xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$colNumber + 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
				<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNumber"/>
        </xsl:with-param>
        <xsl:with-param name="SheetId">
          <xsl:value-of select="$SheetId"/>
        </xsl:with-param>
        <xsl:with-param name="tableName">
          <xsl:value-of select="$tableName"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>

  </xsl:template>

  <!-- insert define connection -->
  <xsl:template match="table:table-cell" mode="connection">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="SheetId"/>
    <xsl:param name="tableName"/>


    <xsl:for-each select="table:cell-range-source">
      <xsl:variable name="ColConnectionStart">
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:value-of select="$colNumber"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <xsl:variable name="ColConnectionEnd">
        <xsl:call-template name="NumbersToChars">
          <xsl:with-param name="num">
            <xsl:value-of select="$colNumber + @table:last-column-spanned - 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:variable>

      <definedName name="{generate-id()}" localSheetId="{$SheetId}">
        <xsl:value-of
          select="concat($tableName, '!$', $ColConnectionStart, '$', $rowNumber, ':$', $ColConnectionEnd, '$', $rowNumber + @table:last-row-spanned - 1)"
        />
      </definedName>
    </xsl:for-each>

    <xsl:if test="following-sibling::table:table-cell">
      <xsl:apply-templates
        select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell'][1]"
        mode="connection">
				<!--Vijayeta,SP2,@table:number-columns-repeated-->
				<!--<xsl:with-param name="colNumber">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
              <xsl:value-of select="number($colNumber) + number(@table:number-columns-repeated)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$colNumber + 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>-->
        <xsl:with-param name="colNumber">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated != ''">
							<!--<xsl:choose >
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
								</xsl:when>
								<xsl:when test ="@table:number-columns-repeated &lt;= 256">
									<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
								</xsl:when>
							</xsl:choose>-->
				<xsl:variable name ="tempOfficeVersion">
					<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
				</xsl:variable>
				<xsl:choose>
					<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
						<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
					</xsl:when>
					<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
						<xsl:choose>
							<xsl:when test ="@table:number-columns-repeated &gt; 256">
								<xsl:value-of select ="number($colNumber) + number(256 - (16384 - number(@table:number-columns-repeated)))"/>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:when>
					<xsl:otherwise>
						<xsl:value-of
								select="number($colNumber) + number(@table:number-columns-repeated)"/>
					</xsl:otherwise>
							</xsl:choose>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$colNumber + 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
				<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNumber"/>
        </xsl:with-param>
        <xsl:with-param name="SheetId">
          <xsl:value-of select="$SheetId"/>
        </xsl:with-param>
        <xsl:with-param name="tableName">
          <xsl:value-of select="$tableName"/>
        </xsl:with-param>
      </xsl:apply-templates>
    </xsl:if>
	</xsl:template>

	<xsl:template name ="getFileNameFromUrl">
		<xsl:param name ="url"/>
		<xsl:variable name ="newUrl">
			<xsl:value-of select ="$url"/>
		</xsl:variable>
		<xsl:choose>
			<xsl:when test ="substring-after($newUrl,'/')!=''">
				<xsl:call-template name ="getFileNameFromUrl">
					<xsl:with-param name ="url">
						<xsl:value-of select ="substring-after($newUrl,'/')"/>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select ="$newUrl"/>
			</xsl:otherwise>
		</xsl:choose>


  </xsl:template>

</xsl:stylesheet>
