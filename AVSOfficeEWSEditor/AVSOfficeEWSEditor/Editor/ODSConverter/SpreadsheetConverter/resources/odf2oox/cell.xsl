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
RefNo-1 06-Nov-2007 Sandeep S     1757284   Changes done to get the correct Used area-sheet after conversion. 
RefNo-2 26-Dec-2007 Sandeep S     1805556   Changes done to include a condition to retain row grouping in case of blank cell
RefNo-3 08-Feb-2008 Sandeep S     1738259  Changes done to Bug:Hyperlink text color is not retained after conversion
RefNo-4 23-May-2008 Sandeep S     1898009   Changes for fixing:XLSX borders in grouped columns lost
                                  1877455   Changes done to retain the cell style in case of blank cell with column style.
RefNo-5 03-Jun-2008 Sandeep S     1780009   Parts of borders of merged cells are not retained(Grouped rows)
RefNo-6 03-Jun-2008 sandeep S     1704269   Changes done to retain the cell style in case of merged cell.
RefNo-7 04-Jun-2008 Sandeep S     1780009   Changes done to get the correct merged cell style i.e. To deferentiate b/w A3 and AA3
RefNo-8 08-sep-2008 sandeep s     New feature Changes for formula implementation.
RefNo-9 14-oct-2008 sandeep s     2149116  Changes done to retain Time&Date format in case of formula.
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns="http://schemas.openxmlformats.org/spreadsheetml/2006/main"
  xmlns:pzip="urn:cleverage:xmlns:post-processings:zip"
  xmlns:table="urn:oasis:names:tc:opendocument:xmlns:table:1.0"
  xmlns:r="http://schemas.openxmlformats.org/officeDocument/2006/relationships"
  xmlns:config="urn:oasis:names:tc:opendocument:xmlns:config:1.0"
  xmlns:text="urn:oasis:names:tc:opendocument:xmlns:text:1.0"
  xmlns:style="urn:oasis:names:tc:opendocument:xmlns:style:1.0"
  xmlns:pxsi="urn:cleverage:xmlns:post-processings:pivotTable"
  xmlns:fo="urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0"
  xmlns:draw="urn:oasis:names:tc:opendocument:xmlns:drawing:1.0"
  xmlns:meta="urn:oasis:names:tc:opendocument:xmlns:meta:1.0"
  xmlns:office="urn:oasis:names:tc:opendocument:xmlns:office:1.0" exclude-result-prefixes="table r">

  <!--<xsl:import href="measures.xsl"/>-->

  <!-- insert column properties into sheet -->
  <xsl:template match="table:table-column" mode="sheet">
    <xsl:param name="colNumber"/>
    <xsl:param name="defaultFontSize"/>
	  <xsl:param name="defaultFontSizeForDefColWid"/>
	  <xsl:param name="defaultFontStyleForDefColWid"/>

    <!-- tableId required for CheckIfColumnStyle template -->
    <xsl:variable name="tableId">
      <xsl:value-of select="generate-id(ancestor::table:table)"/>
    </xsl:variable>

		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<!--<xsl:variable name="max">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated &gt; 0">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$colNumber"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="max">
			<xsl:variable name ="tempOfficeVersion">
				<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			</xsl:variable>
			<!--<xsl:choose>
				 <xsl:when test="@table:number-columns-repeated &gt; 0">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:choose >
								<xsl:when test ="(256 - ((16384 - @table:number-columns-repeated) - 1)) &lt; 0">
									<xsl:value-of select ="$colNumber + ((16384 - @table:number-columns-repeated) - 1) - 256"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select ="$colNumber + (256 - ((16384 - @table:number-columns-repeated) - 1))"/>
								</xsl:otherwise>
							</xsl:choose>							
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                           <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
                       </xsl:when>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
				  <xsl:value-of select="$colNumber"/>
				</xsl:otherwise>
			  </xsl:choose>-->
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated &gt; 0">
					<xsl:choose >
						<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
							<xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
						</xsl:when>
						<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
							<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:value-of select ="$colNumber + (256 - ((16384 - @table:number-columns-repeated) - 1))"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
        </xsl:when>
					</xsl:choose>
				</xsl:when>
        <xsl:otherwise>
							<xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
          <xsl:value-of select="$colNumber"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->

    <xsl:if test="not($max = '256' and @table:visibility = 'collapse')">
      <col min="{$colNumber}">
        <xsl:attribute name="max">
          <xsl:value-of select="$max"/>
        </xsl:attribute>
        <!-- insert column width -->
		  <!--Changes By: Vijayeta
		      Code Change :Checking for condition if attr 'style:column-width' is present or not, if not, set 'width' as default column width
			  Desc        :On conversion of KSpread.ods, xlsx file loses some columns as the ODS file does not have attriute 'style:column-width', to set the custom width
			               Hence, if custom width not present in 'table:column' set 'width' to default width-->		 
          <xsl:attribute name="width">
			  <xsl:choose>
				  <xsl:when
					test="key('style',@table:style-name)/style:table-column-properties/@style:column-width">
            <xsl:call-template name="ConvertToCharacters">
              <xsl:with-param name="width">
                <xsl:value-of
								select="key('style',@table:style-name)/style:table-column-properties/@style:column-width"/>
              </xsl:with-param>
              <xsl:with-param name="defaultFontSize" select="$defaultFontSize"/>
            </xsl:call-template>
				  </xsl:when>
				  <xsl:otherwise>
					  <xsl:value-of select="concat('sonataColumnWidth:',$defaultFontStyleForDefColWid,'|',$defaultFontSizeForDefColWid)"/>					 
				  </xsl:otherwise>
				  </xsl:choose>
          </xsl:attribute>
          <xsl:attribute name="customWidth">1</xsl:attribute>
        <!-- get parent table:table-row id-->
        <xsl:variable name="columnId">
          <xsl:value-of select="generate-id(.)"/>
        </xsl:variable>

        <!-- go to first table:table-column-->
        <xsl:for-each select="ancestor::table:table/descendant::table:table-column[1]">
          <xsl:variable name="number">
            <xsl:call-template name="GetColNumber">
              <xsl:with-param name="columnId" select="$columnId"/>
              <xsl:with-param name="tableId" select="generate-id(ancestor::table:table)"/>
            </xsl:call-template>
          </xsl:variable>
        </xsl:for-each>

        <xsl:if test="ancestor::table:table-column-group != 0">
          <xsl:attribute name="outlineLevel">
            <xsl:value-of select="count(ancestor::table:table-column-group)"/>
          </xsl:attribute>
        </xsl:if>

        <xsl:if test="@table:visibility = 'collapse'">
          <xsl:attribute name="hidden">1</xsl:attribute>
        </xsl:if>
        <!--commented by sonata for bug no:2559854-->

        <!--<xsl:if test="@table:default-cell-style-name != 'Default' ">
          
          -->
				<!--by sonata for bug no:2165930-->
				<!--
          <xsl:if test="@table:default-cell-style-name != 'Default' and @table:number-columns-repeated">
          <xsl:for-each select="key('style',@table:default-cell-style-name)">
            <xsl:attribute name="style">
              <xsl:value-of
                select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
            </xsl:attribute>
          </xsl:for-each>
          </xsl:if>
        </xsl:if>-->
          <!--end-->
          
          <!--commented by sonata for bug no:2165930-->
          
          <!-- column style is when in all posible rows there is a cell in this column -->
        <!--changed by sonata for bug no:2559854-->
        <xsl:if test="@table:default-cell-style-name != 'Default' ">
          <xsl:variable name="checkColumnStyle">
            <xsl:for-each select="ancestor::table:table/descendant::table:table-row[1]">
              <xsl:call-template name="CheckIfColumnStyle">
                <xsl:with-param name="number" select="$colNumber"/>
                <xsl:with-param name="table" select="$tableId"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:variable>

          <xsl:if test="$checkColumnStyle = 'true' ">
            <xsl:for-each select="key('style',@table:default-cell-style-name)">
              <xsl:attribute name="style">
                <xsl:value-of
                  select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
              </xsl:attribute>
            </xsl:for-each>
          </xsl:if>
          <!--end-->
          
        </xsl:if>
      </col>
    </xsl:if>

    <!-- insert next column -->
    <xsl:choose>
      <!-- next column is sibling of this one -->
			<!--Vijayeta,SP2,@table:number-columns-repeated-->
			<!--<xsl:when test="following::table:table-column[generate-id(ancestor::table:table) = $tableId]">
        <xsl:apply-templates
          select="following::table:table-column[generate-id(ancestor::table:table) = $tableId][1]"
          mode="sheet">
          <xsl:with-param name="colNumber">
            <xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="defaultFontSize" select="$defaultFontSize"/>
        </xsl:apply-templates>
      </xsl:when>-->
      <xsl:when test="following::table:table-column[generate-id(ancestor::table:table) = $tableId]">
        <xsl:apply-templates
          select="following::table:table-column[generate-id(ancestor::table:table) = $tableId][1]"
          mode="sheet">
          <xsl:with-param name="colNumber">
						<xsl:variable name ="tempOfficeVersion">
							<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
						</xsl:variable>
						<!--<xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
								<xsl:choose >
									<xsl:when test ="@table:number-columns-repeated &gt; 256">
										<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
									</xsl:when>
									<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
              </xsl:when>
								</xsl:choose>
							</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>-->
						<xsl:choose>
							<xsl:when test="@table:number-columns-repeated">
								<xsl:choose>
									<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
										<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
									</xsl:when>
									<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
										<xsl:choose>
											<xsl:when test ="@table:number-columns-repeated &gt; 256">
												<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$colNumber+1"/>
							</xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="defaultFontSize" select="$defaultFontSize"/>
			<xsl:with-param name="defaultFontSizeForDefColWid" select="$defaultFontSizeForDefColWid"/>
			<xsl:with-param name="defaultFontStyleForDefColWid" select="$defaultFontStyleForDefColWid"/>
        </xsl:apply-templates>
      </xsl:when>

			<!--Vijayeta,SP2,@table:number-columns-repeated,End-->

      <!-- this is the last column inside header -->
      <xsl:when
        test="not(following-sibling::node()[1][name() = 'table:table-column' ]) and parent::node()[name() = 'table:table-header-columns' ] and parent::node()/following-sibling::table:table-column[1]">
        <xsl:apply-templates select="parent::node()/following-sibling::table:table-column[1]"
          mode="sheet">
					<!--<xsl:with-param name="colNumber">
            <xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>-->
          <xsl:with-param name="colNumber">
						<!--<xsl:choose>
              <xsl:when test="@table:number-columns-repeated">
								<xsl:choose >
									<xsl:when test ="@table:number-columns-repeated &gt; 256">
										<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
									</xsl:when>
									<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
              </xsl:when>
								</xsl:choose>
							</xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$colNumber+1"/>
              </xsl:otherwise>
            </xsl:choose>-->
						<xsl:variable name ="tempOfficeVersion">
							<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
						</xsl:variable>
						<xsl:choose>
							<xsl:when test="@table:number-columns-repeated">
								<xsl:choose>
									<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
										<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
									</xsl:when>
									<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
										<xsl:choose>
											<xsl:when test ="@table:number-columns-repeated &gt; 256">
												<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
									</xsl:otherwise>
								</xsl:choose>
							</xsl:when>
							<xsl:otherwise>
								<xsl:value-of select="$colNumber+1"/>
							</xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
					<!--Vijayeta,SP2,@table:number-columns-repeated,End-->

          <xsl:with-param name="defaultFontSize" select="$defaultFontSize"/>
			<xsl:with-param name="defaultFontSizeForDefColWid" select="$defaultFontSizeForDefColWid"/>
			<xsl:with-param name="defaultFontStyleForDefColWid" select="$defaultFontStyleForDefColWid"/>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <!-- Check if 256 column are hidden -->

  <xsl:template match="table:table-column" mode="defaultColWidth">
    <xsl:param name="colNumber"/>
    <xsl:param name="result"/>
		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<!--<xsl:variable name="max">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$colNumber"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="max">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
					<!--<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:value-of select ="$colNumber + (256 - (16384 - (@table:number-columns-repeated - 1)))"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
        </xsl:when>
		</xsl:choose>-->
					<!--<xsl:choose >
				<xsl:when test ="@table:number-columns-repeated &gt; 256">
					<xsl:choose >
						<xsl:when test ="(256 - ((16384 - @table:number-columns-repeated) - 1)) &lt; 0">
							<xsl:value-of select ="$colNumber + ((16384 - @table:number-columns-repeated) - 1) - 256"/>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select ="$colNumber + (256 - ((16384 - @table:number-columns-repeated) - 1))"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:when test ="@table:number-columns-repeated &lt;= 256">
					<xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
				</xsl:when>
			</xsl:choose>-->
					<xsl:variable name ="tempOfficeVersion">
						<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
					</xsl:variable>
					<xsl:choose>
						<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
							<xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
						</xsl:when>
						<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:value-of select ="$colNumber + (256 - (16384 - (@table:number-columns-repeated - 1)))"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
        </xsl:when>
					</xsl:choose>
				</xsl:when>
        <xsl:otherwise>
							<xsl:value-of select="$colNumber+@table:number-columns-repeated - 1"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
          <xsl:value-of select="$colNumber"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <xsl:choose>
      <xsl:when test="not($max = '256' and @table:visibility = 'collapse')">
        <!-- insert next column -->
        <xsl:choose>
          <!-- next column is a sibling of this one -->
          <xsl:when test="following-sibling::node()[1][name() = 'table:table-column' ]">
            <xsl:apply-templates select="following-sibling::table:table-column[1]"
              mode="defaultColWidth">
							<!--Vijayeta,SP2,@table:number-columns-repeated-->
							<!--<xsl:with-param name="colNumber">
                <xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>-->
              <xsl:with-param name="colNumber">
								<!--<xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
										<xsl:choose >
											<xsl:when test ="@table:number-columns-repeated &gt; 256">
												<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
											</xsl:when>
											<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
										</xsl:choose>
									</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
									</xsl:choose>-->
								<xsl:variable name ="tempOfficeVersion">
									<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
								</xsl:variable>
								<xsl:choose>
									<xsl:when test="@table:number-columns-repeated">
										<xsl:choose>
											<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:when>
											<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
												<xsl:choose>
													<xsl:when test ="@table:number-columns-repeated &gt; 256">
														<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
													</xsl:when>
													<xsl:otherwise>
														<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
													</xsl:otherwise>
												</xsl:choose>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$colNumber+1"/>
									</xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
							<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
              <xsl:with-param name="result">
                <xsl:text>false</xsl:text>
              </xsl:with-param>
            </xsl:apply-templates>

            <xsl:variable name="tableId">
              <xsl:value-of select="generate-id(ancestor::table:table)"/>
            </xsl:variable>

          </xsl:when>
          <!-- this is the last column before header -->
          <xsl:when test="following-sibling::node()[1][name() = 'table:table-header-columns' ]">
            <xsl:apply-templates select="following-sibling::node()[1]/table:table-column[1]"
              mode="defaultColWidth">
							<!--Vijayeta,SP2,@table:number-columns-repeated-->
							<!--<xsl:with-param name="colNumber">
                <xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>-->
              <xsl:with-param name="colNumber">
								<!--<xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
										<xsl:choose >
											<xsl:when test ="@table:number-columns-repeated &gt; 256">
												<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
											</xsl:when>
											<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
										</xsl:choose>
									</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>-->
								<xsl:variable name ="tempOfficeVersion">
									<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
								</xsl:variable>
								<xsl:choose>
									<xsl:when test="@table:number-columns-repeated">
										<xsl:choose>
											<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:when>
											<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
												<xsl:choose>
													<xsl:when test ="@table:number-columns-repeated &gt; 256">
														<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
													</xsl:when>
													<xsl:otherwise>
														<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
													</xsl:otherwise>
												</xsl:choose>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$colNumber+1"/>
									</xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
							<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
              <xsl:with-param name="result">
                <xsl:text>false</xsl:text>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <!-- this is the last column inside header -->
          <xsl:when
            test="not(following-sibling::node()[1][name() = 'table:table-column' ]) and parent::node()[name() = 'table:table-header-columns' ]">
            <xsl:apply-templates select="parent::node()/following-sibling::table:table-column[1]"
              mode="defaultColWidth">
							<!--Vijayeta,SP2,@table:number-columns-repeated-->
							<!--<xsl:with-param name="colNumber">
                <xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>-->
              <xsl:with-param name="colNumber">
								<!--<xsl:choose>
                  <xsl:when test="@table:number-columns-repeated">
										<xsl:choose >
											<xsl:when test ="@table:number-columns-repeated &gt; 256">
												<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
											</xsl:when>
											<xsl:when test ="@table:number-columns-repeated &lt;= 256">
                    <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
                  </xsl:when>
										</xsl:choose>
									</xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$colNumber+1"/>
                  </xsl:otherwise>
                </xsl:choose>-->
								<xsl:variable name ="tempOfficeVersion">
									<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
								</xsl:variable>
								<xsl:choose>
									<xsl:when test="@table:number-columns-repeated">
										<xsl:choose>
											<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:when>
											<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
												<xsl:choose>
													<xsl:when test ="@table:number-columns-repeated &gt; 256">
														<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
													</xsl:when>
													<xsl:otherwise>
														<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
													</xsl:otherwise>
												</xsl:choose>
											</xsl:when>
											<xsl:otherwise>
												<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
											</xsl:otherwise>
										</xsl:choose>
									</xsl:when>
									<xsl:otherwise>
										<xsl:value-of select="$colNumber+1"/>
									</xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
							<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
              <xsl:with-param name="result">
                <xsl:text>false</xsl:text>
              </xsl:with-param>
            </xsl:apply-templates>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$result"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>true</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  <!-- Create Variable with "Default Style Name" -->
  <xsl:template name="CreateDefaultTag">
    <xsl:param name="colNumber"/>
    <xsl:param name="TagNumber"/>

    <xsl:param name="Tag"/>
    <xsl:param name="repeat"/>
		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<xsl:variable name ="tableNumberColumnsRepeated">
			<!--<xsl:choose >
				<xsl:when test ="@table:number-columns-repeated &gt; 256">
					<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
				</xsl:when>
				<xsl:when test ="@table:number-columns-repeated &lt;= 256">
					<xsl:value-of select="@table:number-columns-repeated"/>
				</xsl:when>
			</xsl:choose>-->
			<xsl:variable name ="tempOfficeVersion">
				<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			</xsl:variable>
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
		</xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <xsl:choose>
			<!--<xsl:when test="@table:number-columns-repeated &gt; $repeat">-->
			<xsl:when test="$tableNumberColumnsRepeated &gt; $repeat">
        <xsl:call-template name="CreateDefaultTag">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:with-param>
          <xsl:with-param name="TagNumber">
            <xsl:value-of select="$TagNumber"/>
          </xsl:with-param>
          <xsl:with-param name="Tag">
            <xsl:value-of
              select="concat(concat(concat($colNumber, @table:default-cell-style-name),';'), $Tag)"
            />
          </xsl:with-param>
          <xsl:with-param name="repeat">
            <xsl:value-of select="$repeat + 1"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of
          select="concat(concat(concat($colNumber, @table:default-cell-style-name),';'), $Tag)"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="CreateStyleTags">
    <xsl:param name="repeat"/>
    <xsl:param name="colNumber"/>
    <xsl:param name="TagNumber"/>

    <xsl:param name="Tag"/>
    <xsl:param name="count" select="1"/>

    <xsl:choose>
      <xsl:when test="$repeat &gt; $count">
        <xsl:call-template name="CreateStyleTags">
          <xsl:with-param name="repeat" select="$repeat"/>
          <xsl:with-param name="colNumber" select="$colNumber + 1"/>
          <xsl:with-param name="TagNumber" select="$TagNumber"/>
          <xsl:with-param name="Tag">
            <xsl:value-of
              select="concat(concat(concat ('K', (concat($colNumber, ':')), @table:default-cell-style-name),';'), $Tag)"
            />
          </xsl:with-param>
          <xsl:with-param name="count" select="$count + 1"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of
          select="concat(concat(concat ('K', (concat($colNumber, ':')), @table:default-cell-style-name),';'), $Tag)"
        />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="table:table-column" mode="tag">
    <xsl:param name="colNumber"/>
    <xsl:param name="Tag"/>

    <xsl:variable name="TagNumber">
      <xsl:value-of select="concat('Tag', position())"/>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<!--<xsl:variable name="NextColl">
      <xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$colNumber+1"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>-->
    <xsl:variable name="NextColl">
			<!--<xsl:choose>
        <xsl:when test="@table:number-columns-repeated">
					<xsl:choose >
						<xsl:when test ="@table:number-columns-repeated &gt; 256">
							<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
						</xsl:when>
						<xsl:when test ="@table:number-columns-repeated &lt;= 256">
          <xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
        </xsl:when>
					</xsl:choose>
				</xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$colNumber+1"/>
        </xsl:otherwise>
      </xsl:choose>-->
			<xsl:variable name ="tempOfficeVersion">
				<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			</xsl:variable>
			<xsl:choose>
				<xsl:when test="@table:number-columns-repeated">
					<xsl:choose>
						<xsl:when test ="starts-with($tempOfficeVersion,'OpenOffice.org')">
							<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
						</xsl:when>
						<xsl:when test ="starts-with($tempOfficeVersion,'MicrosoftOffice')">
							<xsl:choose>
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="$colNumber + (256 - (16384 - @table:number-columns-repeated))"/>
								</xsl:when>
								<xsl:otherwise>
									<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
								</xsl:otherwise>
							</xsl:choose>
						</xsl:when>
						<xsl:otherwise>
							<xsl:value-of select="$colNumber+@table:number-columns-repeated"/>
						</xsl:otherwise>
					</xsl:choose>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$colNumber+1"/>
				</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
    <!-- style char for this table:table-column tag -->
    <xsl:variable name="TagChar">
      <xsl:choose>
        <!-- when this tag describes group of columns with changed default-cell-style-name -->
        <xsl:when
          test="@table:number-columns-repeated and @table:default-cell-style-name != 'Default' ">
          <xsl:call-template name="CreateStyleTags">
            <xsl:with-param name="colNumber">
              <xsl:value-of select="$colNumber"/>
            </xsl:with-param>
            <xsl:with-param name="TagNumber">
              <xsl:value-of select="$TagNumber"/>
            </xsl:with-param>
            <xsl:with-param name="Tag">
              <xsl:value-of select="$Tag"/>
            </xsl:with-param>
            <xsl:with-param name="repeat">
							<!--Vijayeta,SP2,@table:number-columns-repeated-->
							<!--<xsl:choose >
								<xsl:when test ="@table:number-columns-repeated &gt; 256">
									<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
								</xsl:when>
								<xsl:when test ="@table:number-columns-repeated &lt;= 256">
              <xsl:value-of select="@table:number-columns-repeated"/>
								</xsl:when>
							</xsl:choose>-->
							<xsl:variable name ="tempOfficeVersion">
								<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
							</xsl:variable>
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
							<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
							<!--<xsl:value-of select="@table:number-columns-repeated"/>-->
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <!-- when this tag describes column(s) with default default-cell-style-name -->
        <xsl:when test="@table:default-cell-style-name = 'Default' ">
          <xsl:call-template name="CreateDefaultTag">
            <xsl:with-param name="colNumber">
              <xsl:value-of select="$colNumber"/>
            </xsl:with-param>
            <xsl:with-param name="TagNumber">
              <xsl:value-of select="$TagNumber"/>
            </xsl:with-param>
            <xsl:with-param name="Tag">
              <xsl:value-of select="$Tag"/>
            </xsl:with-param>
            <xsl:with-param name="repeat">1</xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <!-- when this tag describes single column with changed default-cell-style-name -->
        <xsl:otherwise>
          <xsl:value-of
            select="concat(concat(concat ('K', (concat($colNumber, ':')), @table:default-cell-style-name),';'), $Tag)"
          />
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <!--check next column -->
    <xsl:choose>
      <!-- next column is sibling of this one -->
      <xsl:when test="following-sibling::node()[1][name() = 'table:table-column' ]">
        <xsl:apply-templates select="following-sibling::table:table-column[1]" mode="tag">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$NextColl"/>
          </xsl:with-param>
          <xsl:with-param name="Tag">
            <xsl:value-of select="$TagChar"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <!-- this is the last column before header  -->
      <xsl:when test="following-sibling::node()[1][name() = 'table:table-header-columns' ]">
        <xsl:apply-templates select="following-sibling::node()[1]/table:table-column[1]" mode="tag">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$NextColl"/>
          </xsl:with-param>
          <xsl:with-param name="Tag">
            <xsl:value-of select="$TagChar"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <!-- this is the last column inside header  -->
      <xsl:when
        test="not(following-sibling::node()[1][name() = 'table:table-column' ]) and parent::node()[name() = 'table:table-header-columns' ] and parent::node()/following-sibling::table:table-column[1]">
        <xsl:apply-templates select="parent::node()/following-sibling::table:table-column[1]"
          mode="tag">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$NextColl"/>
          </xsl:with-param>
          <xsl:with-param name="Tag">
            <xsl:value-of select="$TagChar"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$TagChar"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- Check if 65536 rows are hidden -->

  <xsl:template match="table:table-row" mode="zeroHeight">
    <xsl:param name="rowNumber"/>
    <xsl:param name="collapse"/>

    <xsl:choose>
      <xsl:when
        test="not(following-sibling::table:table-row or following-sibling::table:table-header-rows or following-sibling::table:table-row-group) and @table:visibility='collapse'">
				<!--Vijayeta,SP2,@table:number-rows-repeated-->
				<!--<xsl:variable name="CheckNumber">
          <xsl:choose>
            <xsl:when test="@table:number-rows-repeated">
              <xsl:value-of select="$rowNumber+@table:number-rows-repeated"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$rowNumber+1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>-->
        <xsl:variable name="CheckNumber">
          <xsl:choose>
            <xsl:when test="@table:number-rows-repeated">
							<xsl:choose >
								<xsl:when test ="@table:number-rows-repeated &gt; 65536">
									<xsl:value-of select ="$rowNumber + (65536 - (1048576 - @table:number-rows-repeated))"/>
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
        </xsl:variable>
				<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
        <xsl:choose>
          <xsl:when test="$CheckNumber = '65536'">
            <xsl:text>true</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>false</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:when
        test="following-sibling::table:table-row[@table:visibility='collapse'] or parent::node()/following-sibling::table:table-row[@table:visibility='collapse']">
        <xsl:choose>
          <xsl:when test="name(following-sibling::node()[1]) = 'table:table-row'">
            <xsl:apply-templates select="following-sibling::table:table-row[1]" mode="zeroHeight">
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
            </xsl:apply-templates>
          </xsl:when>
          <xsl:when
            test="name(following-sibling::node()[1]) = 'table:table-header-rows' or name(following-sibling::node()[1]) = 'table:table-row-group'">
            <xsl:apply-templates
              select="following-sibling::table:table-header-rows/table:table-row[1]|following-sibling::table:table-row-group/table:table-row[1]"
              mode="zeroHeight">
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
            </xsl:apply-templates>
          </xsl:when>
          <xsl:when
            test="(name(parent::node()) = 'table:table-header-rows' or name(parent::node()) = 'table:table-row-group') and parent::node()/following-sibling::table:table-row">
            <xsl:apply-templates select="parent::node()/following-sibling::table:table-row[1]"
              mode="zeroHeight">
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
            </xsl:apply-templates>
          </xsl:when>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <!-- insert row into sheet -->
  <xsl:template match="table:table-row" mode="sheet">
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="defaultRowHeight"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="CheckRowHidden"/>
    <xsl:param name="CheckIfDefaultBorder"/>
    <xsl:param name="ignoreFilter"/>
    <xsl:param name="tableId"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <xsl:variable name="height">
      <xsl:call-template name="point-measure">
        <xsl:with-param name="length">
          <xsl:value-of
            select="key('style',@table:style-name)/style:table-row-properties/@style:row-height"/>
        </xsl:with-param>
        <xsl:with-param name="round">false</xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:if
      test="table:table-cell or @table:visibility='collapse' or  @table:visibility='filter' or ($height != $defaultRowHeight and following-sibling::table:table-row/table:table-cell/text:p|text:span) or table:covered-table-cell">
      <!--RefNo-1-->
      <!--RefNo-2: Changed count(following-sibling::table:table-row) &lt;= 1 to (count(following-sibling::table:table-row) &lt;= 1  and parent::table:table)-->
     <!--  Commented by Vipul to fix defect <xsl:if test="descendant::office:annotation or not((count(following-sibling::table:table-row) &lt;= 1  and parent::table:table) and (count(child::table:table-cell/child::text:p) = 0  and (not(child::table:table-cell/attribute::table:style-name))  and not(following-sibling::table:table-row/table:table-cell/child::text:p)))"> -->
<xsl:if test="descendant::office:annotation or descendant::draw:frame or
      not(
        (count(following-sibling::table:table-row) &lt;= 1  and parent::table:table) 
        and (count(child::table:table-cell/child::text:p) = 0  and 
          (not(child::table:table-cell/attribute::table:style-name))
        and not(following-sibling::table:table-row/table:table-cell/child::text:p) or following-sibling::table:table-row/descendant::draw:frame))">
      <row r="{$rowNumber}">

        <!-- insert row height -->
        <xsl:if test="$height">
          <!--added condition by sonata for bug no:2107295-->
        <xsl:if test="$height!=0">
          <xsl:attribute name="ht">
            <xsl:value-of select="$height"/>
          </xsl:attribute>
          <xsl:if
            test="not(key('style',@table:style-name)/style:table-row-properties/@style:use-optimal-row-height = 'true' ) or table:covered-table-cell">
            <xsl:attribute name="customHeight">1</xsl:attribute>
          </xsl:if>
        </xsl:if>
        </xsl:if>
        <!--changed by sonata for bug no:2165930-->
        <!--changed by sonata for regression bug no:2559854-->
        <xsl:variable name="checkStyle">
          <xsl:for-each select="table:table-cell">
            <xsl:choose>
              <xsl:when test="@table:style-name or @table:number-columns-repeated">
                <xsl:value-of select="'true'"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="'false'"/>
              </xsl:otherwise>
            </xsl:choose>
            </xsl:for-each>
        </xsl:variable>
        <xsl:variable name="totalCell">
          <xsl:value-of select="count(table:table-cell)"/>
        </xsl:variable>
        <xsl:variable name="countColRepeted">
          <xsl:value-of select="count(table:table-cell[@table:number-columns-repeated])"/>
        </xsl:variable>
        <xsl:variable name="sumColRepeted">
          <xsl:value-of select="sum(table:table-cell/@table:number-columns-repeated)"/>
        </xsl:variable>
      
        <xsl:variable name="totalColCount">
          <xsl:value-of select=" $totalCell + $sumColRepeted - $countColRepeted"/>
        </xsl:variable>
       
      
        <!--<xsl:if test="table:table-cell[last()]/@table:style-name and table:table-cell[last()]/@table:number-columns-repeated">-->
        <xsl:if test="not(contains($checkStyle,'false')) and $totalColCount =256">
          <xsl:if test="table:table-cell[last()]/@table:style-name">
          <xsl:variable name="stylerepeated">
            <xsl:value-of select="table:table-cell[last()]/@table:style-name"/>
          </xsl:variable>
          <xsl:for-each select="key('style',$stylerepeated)">
            <xsl:attribute name="s">
              <xsl:value-of
                select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
            </xsl:attribute>
          </xsl:for-each>
          <xsl:attribute name="customFormat">
            <xsl:value-of select="1"/>
          </xsl:attribute>
        </xsl:if>
        </xsl:if>


        <!--end-->
        <!--get parent table:table-row id-->
        <xsl:variable name="rowId">
          <xsl:value-of select="generate-id(.)"/>
        </xsl:variable>

        <!-- go to first table:table-row-->
        <xsl:for-each select="ancestor::table:table/descendant::table:table-row[1]">
          <xsl:variable name="number">
            <xsl:call-template name="GetRowNumber">
              <xsl:with-param name="rowId" select="$rowId"/>
              <xsl:with-param name="tableId" select="$tableId"/>
            </xsl:call-template>
          </xsl:variable>
        </xsl:for-each>

        <xsl:if test="ancestor::table:table-row-group">
          <xsl:attribute name="outlineLevel">
            <xsl:value-of select="count(ancestor::table:table-row-group)"/>
          </xsl:attribute>
        </xsl:if>

        <xsl:if
          test="@table:visibility = 'collapse' or (@table:visibility = 'filter' and $ignoreFilter = '')">
          <xsl:attribute name="hidden">1</xsl:attribute>
        </xsl:if>

        <!-- insert first cell -->
        <xsl:apply-templates select="node()[1]" mode="sheet">
          <xsl:with-param name="colNumber">0</xsl:with-param>
          <xsl:with-param name="rowNumber" select="$rowNumber"/>
          <xsl:with-param name="cellNumber" select="$cellNumber"/>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="ignoreFilter" select="$ignoreFilter"/>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </row>


      <!-- insert repeated rows -->
      <xsl:if test="@table:number-rows-repeated">
        <xsl:if
          test="not(@table:visibility='collapse') or not(following-sibling::table:table-row[@table:visibility = 'collapse']) or following-sibling::table:table-row/table:table-cell/text:p or $CheckRowHidden != 'true' or contains($CheckIfDefaultBorder, 'true')">
          <xsl:call-template name="InsertRepeatedRows">
							<!--Vijayeta,SP2,@table:number-rows-repeated-->
							<!--<xsl:with-param name="numberRowsRepeated">
								<xsl:value-of select="@table:number-rows-repeated"/>
							</xsl:with-param>-->
            <xsl:with-param name="numberRowsRepeated">
								<xsl:choose >
									<xsl:when test ="@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
              <xsl:value-of select="@table:number-rows-repeated"/>
									</xsl:when>
								</xsl:choose>
            </xsl:with-param>
							<!--<xsl:with-param name="numberOfAllRowsRepeated">
								<xsl:value-of select="@table:number-rows-repeated"/>
							</xsl:with-param>-->
            <xsl:with-param name="numberOfAllRowsRepeated">
								<xsl:choose >
									<xsl:when test ="@table:number-rows-repeated &gt; 65536">
										<xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
									</xsl:when>
									<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
              <xsl:value-of select="@table:number-rows-repeated"/>
									</xsl:when>
								</xsl:choose>
            </xsl:with-param>
							<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
            <xsl:with-param name="rowNumber">
              <xsl:value-of select="$rowNumber + 1"/>
            </xsl:with-param>
            <xsl:with-param name="cellNumber">
              <xsl:value-of select="$cellNumber"/>
            </xsl:with-param>
            <xsl:with-param name="height">
              <xsl:value-of select="$height"/>
            </xsl:with-param>
            <xsl:with-param name="defaultRowHeight">
              <xsl:value-of select="$defaultRowHeight"/>
            </xsl:with-param>
            <xsl:with-param name="TableColumnTagNum">
              <xsl:value-of select="$TableColumnTagNum"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCell">
              <xsl:value-of select="$MergeCell"/>
            </xsl:with-param>
            <xsl:with-param name="MergeCellStyle">
              <xsl:value-of select="$MergeCellStyle"/>
            </xsl:with-param>
            <xsl:with-param name="CheckIfDefaultBorder">
              <xsl:value-of select="$CheckIfDefaultBorder"/>
            </xsl:with-param>
            <xsl:with-param name="ignoreFilter" select="$ignoreFilter"/>
            <xsl:with-param name="tableId">
              <xsl:value-of select="$tableId"/>
            </xsl:with-param>
            <xsl:with-param name="multilines">
              <xsl:value-of select="$multilines"/>
            </xsl:with-param>
            <xsl:with-param name="hyperlinkStyle">
              <xsl:value-of select="$hyperlinkStyle"/>
            </xsl:with-param>
            <xsl:with-param name="contentFontsCount">
              <xsl:value-of select="$contentFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="styleFontsCount">
              <xsl:value-of select="$styleFontsCount"/>
            </xsl:with-param>
            <xsl:with-param name="cellFormats">
              <xsl:value-of select="$cellFormats"/>
            </xsl:with-param>
            <xsl:with-param name="cellStyles">
              <xsl:value-of select="$cellStyles"/>
            </xsl:with-param>
            <xsl:with-param name="CheckIfConditional">
              <xsl:value-of select="$CheckIfConditional"/>
            </xsl:with-param>
            <xsl:with-param name="pivotCells">
              <xsl:value-of select="$pivotCells"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:if>
      </xsl:if>
      </xsl:if>
    </xsl:if>


    <!-- insert next row -->
    <xsl:choose>
      <!-- next row is a sibling -->
      <xsl:when test="following::table:table-row[generate-id(ancestor::table:table) = $tableId]">
        <xsl:apply-templates
          select="following::table:table-row[generate-id(ancestor::table:table) = $tableId][1]"
          mode="sheet">
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
            <!-- last or is for cells with error -->
            <xsl:value-of
              select="$cellNumber + count(child::table:table-cell[text:p and not(@office:value-type='float') and (@office:value-type='string' or @office:value-type='boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency' or @office:value-type='date' or @office:value-type='time')))])"
            />
          </xsl:with-param>
          <xsl:with-param name="defaultRowHeight" select="$defaultRowHeight"/>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="CheckRowHidden">
            <xsl:value-of select="$CheckRowHidden"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfDefaultBorder">
            <xsl:value-of select="$CheckIfDefaultBorder"/>
          </xsl:with-param>
          <xsl:with-param name="ignoreFilter" select="$ignoreFilter"/>
          <xsl:with-param name="tableId">
            <xsl:value-of select="$tableId"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <!-- next row is inside header rows -->
      <xsl:when
        test="following-sibling::node()[1][name() = 'table:table-header-rows' ] or following-sibling::node()[1][name() = 'table:table-row-group' ]">
        <xsl:apply-templates
          select="following-sibling::table:table-header-rows/table:table-row[1]|following-sibling::table:table-row-group/table:table-row[1]"
          mode="sheet">
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
            <!-- last or is for cells with error -->
            <xsl:value-of
              select="$cellNumber + count(child::table:table-cell[text:p and not(@office:value-type='float') and (@office:value-type='string' or @office:value-type='boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency' or @office:value-type='date'  or @office:value-type='time')))])"
            />
          </xsl:with-param>
          <xsl:with-param name="defaultRowHeight" select="$defaultRowHeight"/>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="CheckRowHidden">
            <xsl:value-of select="$CheckRowHidden"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfDefaultBorder">
            <xsl:value-of select="$CheckIfDefaultBorder"/>
          </xsl:with-param>
          <xsl:with-param name="ignoreFilter" select="$ignoreFilter"/>
          <xsl:with-param name="tableId">
            <xsl:value-of select="$tableId"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <!-- this is last row inside header rows, next row is outside -->
      <xsl:when
        test="(parent::node()[name()='table:table-header-rows'] or parent::node()[name()='table:table-row-group']) and not(following-sibling::node()[1][name() = 'table:table-row' ])">
        <xsl:apply-templates select="parent::node()/following-sibling::table:table-row[1]"
          mode="sheet">
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
            <!-- last or is for cells with error -->
            <xsl:value-of
              select="$cellNumber + count(child::table:table-cell[text:p and not(@office:value-type='float') and (@office:value-type='string' or @office:value-type='boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency' or @office:value-type='date' or @office:value-type='time')))])"
            />
          </xsl:with-param>
          <xsl:with-param name="defaultRowHeight" select="$defaultRowHeight"/>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="CheckRowHidden">
            <xsl:value-of select="$CheckRowHidden"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfDefaultBorder">
            <xsl:value-of select="$CheckIfDefaultBorder"/>
          </xsl:with-param>
          <xsl:with-param name="ignoreFilter" select="$ignoreFilter"/>
          <xsl:with-param name="tableId">
            <xsl:value-of select="$tableId"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <!-- template which inserts repeated rows -->
  <xsl:template name="InsertRepeatedRows">
    <xsl:param name="numberRowsRepeated"/>
    <xsl:param name="numberOfAllRowsRepeated"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="height"/>
    <xsl:param name="defaultRowHeight"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="CheckIfDefaultBorder"/>
    <xsl:param name="ignoreFilter"/>
    <xsl:param name="tableId"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <!-- first 'or' is for empty cells with formatting, e.g. background -->
  <!--  Commented by Vipul to fix defect <xsl:if
      test="(@table:number-rows-repeated and @table:style-name) or table:table-cell/text:p or table:table-cell/@table:style-name or @table:visibility='collapse' or  @table:visibility='filter' or ($height != $defaultRowHeight and following-sibling::table:table-row/table:table-cell/text:p|text:span) or contains($CheckIfDefaultBorder, 'true') and @ table:table-row[@table:number-rows-repeated] or parent::table:table-row-group"> -->
    <xsl:if
      test="(@table:number-rows-repeated and @table:style-name) or 
            table:table-cell/text:p or 
            count(table:table-cell/child) > 0 or
            table:table-cell/@table:style-name or 
            @table:visibility='collapse' or  
            @table:visibility='filter' or 
            ($height != $defaultRowHeight and following-sibling::table:table-row/table:table-cell/text:p|text:span) 
            or contains($CheckIfDefaultBorder, 'true') 
            and @table:table-row[@table:number-rows-repeated] 
            or parent::table:table-row-group">
      <xsl:choose>
        <xsl:when test="$numberRowsRepeated &gt; 1">
          <!--RefNo-1-->
          <!--RefNo-2: Changed count(following-sibling::table:table-row) &lt;= 1 to (count(following-sibling::table:table-row) &lt;= 1  and parent::table:table)-->
          <xsl:if test="not((count(following-sibling::table:table-row) &lt;= 1 and parent::table:table) and (count(child::table:table-cell/child::text:p) = 0  and (not(child::table:table-cell/attribute::table:style-name))  and not(following-sibling::table:table-row/table:table-cell/child::text:p)))">

            <row>
            <xsl:attribute name="r">
              <xsl:value-of select="$rowNumber"/>
            </xsl:attribute>

            <!-- insert row height -->
            <xsl:if test="$height">
              <xsl:attribute name="ht">
                <xsl:value-of select="$height"/>
              </xsl:attribute>
              <xsl:attribute name="customHeight">1</xsl:attribute>
            </xsl:if>

            <xsl:if test="parent::table:table-row-group and ancestor::table:table-row-group">
              <xsl:attribute name="outlineLevel">
                <xsl:value-of select="count(ancestor::table:table-row-group)"/>
              </xsl:attribute>
            </xsl:if>

            <xsl:if
              test="@table:visibility = 'collapse' or (@table:visibility = 'filter' and $ignoreFilter = '' )">
              <xsl:attribute name="hidden">1</xsl:attribute>
            </xsl:if>

            <!-- insert first cell -->
            <xsl:apply-templates select="node()[1]" mode="sheet">
              <xsl:with-param name="colNumber">0</xsl:with-param>
              <xsl:with-param name="rowNumber" select="$rowNumber"/>
              <xsl:with-param name="cellNumber" select="$cellNumber"/>
              <xsl:with-param name="TableColumnTagNum">
                <xsl:value-of select="$TableColumnTagNum"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="$MergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCellStyle">
                <xsl:value-of select="$MergeCellStyle"/>
              </xsl:with-param>
              <xsl:with-param name="tableId">
                <xsl:value-of select="$tableId"/>
              </xsl:with-param>
              <xsl:with-param name="multilines">
                <xsl:value-of select="$multilines"/>
              </xsl:with-param>
              <xsl:with-param name="hyperlinkStyle">
                <xsl:value-of select="$hyperlinkStyle"/>
              </xsl:with-param>
              <xsl:with-param name="contentFontsCount">
                <xsl:value-of select="$contentFontsCount"/>
              </xsl:with-param>
              <xsl:with-param name="styleFontsCount">
                <xsl:value-of select="$styleFontsCount"/>
              </xsl:with-param>
              <xsl:with-param name="cellFormats">
                <xsl:value-of select="$cellFormats"/>
              </xsl:with-param>
              <xsl:with-param name="cellStyles">
                <xsl:value-of select="$cellStyles"/>
              </xsl:with-param>
              <xsl:with-param name="CheckIfConditional">
                <xsl:value-of select="$CheckIfConditional"/>
              </xsl:with-param>
              <xsl:with-param name="pivotCells">
                <xsl:value-of select="$pivotCells"/>
              </xsl:with-param>
            </xsl:apply-templates>
          </row>


          <!-- insert repeated rows -->
          <xsl:if test="@table:number-rows-repeated">
            <xsl:call-template name="InsertRepeatedRows">
              <xsl:with-param name="numberRowsRepeated">
                <xsl:choose>
                  <xsl:when test="$numberRowsRepeated - 1 &gt; 180">
                    <xsl:text>49</xsl:text>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="$numberRowsRepeated - 1"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
								<!--Vijayeta,SP2,@table:number-rows-repeated-->
								<!--<xsl:with-param name="numberOfAllRowsRepeated">
									<xsl:value-of select="@table:number-rows-repeated"/>
								</xsl:with-param>-->
              <xsl:with-param name="numberOfAllRowsRepeated">
									<xsl:choose >
										<xsl:when test ="@table:number-rows-repeated &gt; 65536">
											<xsl:value-of select ="65536 - (1048576 - @table:number-rows-repeated)"/>
										</xsl:when>
										<xsl:when test ="@table:number-rows-repeated &lt;= 65536">
                <xsl:value-of select="@table:number-rows-repeated"/>
										</xsl:when>
									</xsl:choose>
              </xsl:with-param>
								<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
              <xsl:with-param name="rowNumber">
                <xsl:value-of select="$rowNumber + 1"/>
              </xsl:with-param>
              <xsl:with-param name="cellNumber">
                <xsl:value-of select="$cellNumber"/>
              </xsl:with-param>
              <xsl:with-param name="height">
                <xsl:value-of select="$height"/>
              </xsl:with-param>
              <xsl:with-param name="defaultRowHeight">
                <xsl:value-of select="$defaultRowHeight"/>
              </xsl:with-param>
              <xsl:with-param name="TableColumnTagNum">
                <xsl:value-of select="$TableColumnTagNum"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCell">
                <xsl:value-of select="$MergeCell"/>
              </xsl:with-param>
              <xsl:with-param name="MergeCellStyle">
                <xsl:value-of select="$MergeCellStyle"/>
              </xsl:with-param>
              <xsl:with-param name="CheckIfDefaultBorder">
                <xsl:value-of select="$CheckIfDefaultBorder"/>
              </xsl:with-param>
              <xsl:with-param name="tableId">
                <xsl:value-of select="$tableId"/>
              </xsl:with-param>
              <xsl:with-param name="multilines">
                <xsl:value-of select="$multilines"/>
              </xsl:with-param>
              <xsl:with-param name="hyperlinkStyle">
                <xsl:value-of select="$hyperlinkStyle"/>
              </xsl:with-param>
              <xsl:with-param name="contentFontsCount">
                <xsl:value-of select="$contentFontsCount"/>
              </xsl:with-param>
              <xsl:with-param name="styleFontsCount">
                <xsl:value-of select="$styleFontsCount"/>
              </xsl:with-param>
              <xsl:with-param name="cellFormats">
                <xsl:value-of select="$cellFormats"/>
              </xsl:with-param>
              <xsl:with-param name="cellStyles">
                <xsl:value-of select="$cellStyles"/>
              </xsl:with-param>
              <xsl:with-param name="CheckIfConditional">
                <xsl:value-of select="$CheckIfConditional"/>
              </xsl:with-param>
              <xsl:with-param name="pivotCells">
                <xsl:value-of select="$pivotCells"/>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:if>
          </xsl:if>
        </xsl:when>
      </xsl:choose>
    </xsl:if>

  </xsl:template>

  <!-- insert cell into row -->
  <xsl:template match="table:table-cell|table:covered-table-cell" mode="sheet">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <xsl:message terminate="no">progress:table:table-cell</xsl:message>
    <!--RefNo-1-->
    <!--Start of RefNo-4: 16-Jul-2008-->
    <xsl:variable name="columnCellStyle">
      <xsl:call-template name="GetColumnCellStyle">
        <xsl:with-param name="colNum">
          <xsl:value-of select="$colNumber"/>
        </xsl:with-param>
        <xsl:with-param name="TableColumnTagNum">
          <xsl:value-of select="$TableColumnTagNum"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>
    <!--End of RefNo-4-->
    <!--RefNo-4:added columnCellStyle condition-->
    <!--RefNo-6: or following-sibling::table:covered-table-cell or name()='table:covered-table-cell'-->
		<!--Vijayeta,SP2,@table:number-columns-repeated-->
		<xsl:variable name ="tableNumberColumnsRepeated">
			<!--<xsl:choose >
				<xsl:when test ="@table:number-columns-repeated &gt; 256">
					<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
				</xsl:when>
				<xsl:when test ="@table:number-columns-repeated &lt;= 256">
					<xsl:value-of select="@table:number-columns-repeated"/>
				</xsl:when>
			</xsl:choose>-->
			<xsl:variable name ="tempOfficeVersion">
				<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			</xsl:variable>
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
		</xsl:variable>
		<!--Vijayeta,SP2,@table:number-columns-repeated,End-->
		<xsl:if test="not(not(following-sibling::table:table-cell) and not(child::text:p) and (not(attribute::table:style-name or $columnCellStyle != '')
			or (256 = ($colNumber + $tableNumberColumnsRepeated)))) 
			or following-sibling::table:covered-table-cell or name()='table:covered-table-cell'">
    
    <xsl:call-template name="InsertConvertCell">
      <xsl:with-param name="colNumber">
        <xsl:value-of select="$colNumber"/>
      </xsl:with-param>
      <xsl:with-param name="rowNumber">
        <xsl:value-of select="$rowNumber"/>
      </xsl:with-param>
      <xsl:with-param name="cellNumber">
        <xsl:value-of select="$cellNumber"/>
      </xsl:with-param>
      <xsl:with-param name="ColumnRepeated">1</xsl:with-param>
      <xsl:with-param name="TableColumnTagNum">
        <xsl:value-of select="$TableColumnTagNum"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCellStyle">
        <xsl:value-of select="$MergeCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="CountStyleTableCell">
        <xsl:value-of select="$cellFormats - 1"/>
      </xsl:with-param>
      <xsl:with-param name="multilines">
        <xsl:value-of select="$multilines"/>
      </xsl:with-param>
      <xsl:with-param name="hyperlinkStyle">
        <xsl:value-of select="$hyperlinkStyle"/>
      </xsl:with-param>
      <xsl:with-param name="contentFontsCount">
        <xsl:value-of select="$contentFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleFontsCount">
        <xsl:value-of select="$styleFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="cellFormats">
        <xsl:value-of select="$cellFormats"/>
      </xsl:with-param>
      <xsl:with-param name="cellStyles">
        <xsl:value-of select="$cellStyles"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfConditional">
        <xsl:value-of select="$CheckIfConditional"/>
      </xsl:with-param>
      <xsl:with-param name="pivotCells">
        <xsl:value-of select="$pivotCells"/>
      </xsl:with-param>
    </xsl:call-template>
    </xsl:if>
    <xsl:apply-templates mode="cell">
      <xsl:with-param name="colNumber">
        <xsl:value-of select="$colNumber"/>
      </xsl:with-param>
      <xsl:with-param name="rowNumber">
        <xsl:value-of select="$rowNumber"/>
      </xsl:with-param>
    </xsl:apply-templates>

  </xsl:template>

  <xsl:template match="office:annotation" mode="cell">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:variable name="colChar">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num" select="$colNumber"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="noteId">
      <xsl:value-of select="count(preceding::office:annotation)+1"/>
    </xsl:variable>
    <pxsi:commentmark xmlns:pxsi="urn:cleverage:xmlns:post-processings:comments"
      ref="{concat($colChar,$rowNumber)}" noteId="{$noteId}"/>

    <pxsi:commentDrawingMark xmlns:x="urn:schemas-microsoft-com:office:excel"
      xmlns:pxsi="urn:cleverage:xmlns:post-processings:drawings" noteId="{$noteId}">
      <x:Row>
        <xsl:value-of select="$rowNumber - 1"/>
      </x:Row>
      <x:Column>
        <xsl:value-of select="$colNumber"/>
      </x:Column>
    </pxsi:commentDrawingMark>

  </xsl:template>

  <!-- insert cell -->
  <xsl:template name="InsertNextCell">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <xsl:choose>
      <!-- Checks if  next index is table:table-cell-->
      <xsl:when test="following-sibling::node()[1][name()='table:table-cell']">
        <xsl:apply-templates select="following-sibling::table:table-cell[1]" mode="sheet">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$colNumber+1"/>
          </xsl:with-param>
          <xsl:with-param name="rowNumber" select="$rowNumber"/>
          <!-- if this node was table:table-cell with string than increase cellNumber-->
          <xsl:with-param name="cellNumber">
            <xsl:choose>
              <xsl:when
                test="name()='table:table-cell' and child::text:p and not(@office:value-type='float') and (@office:value-type='string' or @office:value-type='boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency' or @office:value-type='date' or @office:value-type='time')))">
                <xsl:value-of select="$cellNumber + 1"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$cellNumber"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
      <!--  Checks if next index is table:covered-table-cell-->
      <xsl:when test="following-sibling::node()[1][name()='table:covered-table-cell']">
        <xsl:apply-templates select="following-sibling::table:covered-table-cell[1]" mode="sheet">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$colNumber+1"/>
          </xsl:with-param>
          <xsl:with-param name="rowNumber" select="$rowNumber"/>
          <!-- if this node was table:table-cell with string than increase cellNumber-->
          <xsl:with-param name="cellNumber">
            <xsl:choose>
              <xsl:when
                test="name()='table:table-cell' and child::text:p and not(@office:value-type='float') and (@office:value-type='string' or @office:value-type='boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency' or @office:value-type='date' or @office:value-type='time')))">
                <xsl:value-of select="$cellNumber + 1"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$cellNumber"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
          <xsl:with-param name="ColumnRepeated">1</xsl:with-param>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:apply-templates>
      </xsl:when>
    </xsl:choose>

  </xsl:template>


  <!-- Insert Cell for "@table:number-columns-repeated"-->
  <xsl:template name="InsertConvertCell">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="ColumnRepeated"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="CountStyleTableCell"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <!-- do not show covered cells content -->

    <xsl:variable name="columnCellStyle">
      <xsl:call-template name="GetColumnCellStyle">
        <xsl:with-param name="colNum">
          <xsl:value-of select="$colNumber + 1"/>
        </xsl:with-param>
        <xsl:with-param name="TableColumnTagNum">
          <xsl:value-of select="$TableColumnTagNum"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:call-template name="InsertCell">
      <xsl:with-param name="colNumber">
        <xsl:value-of select="$colNumber"/>
      </xsl:with-param>
      <xsl:with-param name="rowNumber">
        <xsl:value-of select="$rowNumber"/>
      </xsl:with-param>
      <xsl:with-param name="cellNumber">
        <xsl:value-of select="$cellNumber"/>
      </xsl:with-param>
      <xsl:with-param name="TableColumnTagNum">
        <xsl:value-of select="$TableColumnTagNum"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCell">
        <xsl:value-of select="$MergeCell"/>
      </xsl:with-param>
      <xsl:with-param name="MergeCellStyle">
        <xsl:value-of select="$MergeCellStyle"/>
      </xsl:with-param>
      <xsl:with-param name="CountStyleTableCell">
        <xsl:value-of select="$CountStyleTableCell"/>
      </xsl:with-param>
      <xsl:with-param name="multilines">
        <xsl:value-of select="$multilines"/>
      </xsl:with-param>
      <xsl:with-param name="hyperlinkStyle">
        <xsl:value-of select="$hyperlinkStyle"/>
      </xsl:with-param>
      <xsl:with-param name="contentFontsCount">
        <xsl:value-of select="$contentFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="styleFontsCount">
        <xsl:value-of select="$styleFontsCount"/>
      </xsl:with-param>
      <xsl:with-param name="cellFormats">
        <xsl:value-of select="$cellFormats"/>
      </xsl:with-param>
      <xsl:with-param name="cellStyles">
        <xsl:value-of select="$cellStyles"/>
      </xsl:with-param>
      <xsl:with-param name="CheckIfConditional">
        <xsl:value-of select="$CheckIfConditional"/>
      </xsl:with-param>
      <xsl:with-param name="pivotCells">
        <xsl:value-of select="$pivotCells"/>
      </xsl:with-param>
    </xsl:call-template>
		<!--Vijayeta,SP2-->
		<xsl:variable name ="tableNumberColumnsRepeated">
			<!--<xsl:choose >
				<xsl:when test ="@table:number-columns-repeated &gt; 256">
					<xsl:value-of select ="256 - (16384 - @table:number-columns-repeated)"/>
				</xsl:when>
				<xsl:when test ="@table:number-columns-repeated &lt;= 256">
					<xsl:value-of select="@table:number-columns-repeated"/>
				</xsl:when>
			</xsl:choose>-->
			<xsl:variable name ="tempOfficeVersion">
				<xsl:value-of select ="document('meta.xml')/office:document-meta/office:meta/meta:generator"/>
			</xsl:variable>
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

		</xsl:variable>
    <!-- Insert cells if "@table:number-columns-repeated"  > 1 -->
    <xsl:choose>
      <!-- do not output unnecessary last cells in a row (when last cell is table:table-cell with default style, repeated columns and without content)  -->
      <!-- explenation of last 'and':
                  If column has got changed default-cell-style-name then in $TableColumnTagNum string there is entry 'K' col_number ':' default-cell-style-name ';' if not there is no 'K'. 
                  So if inside cell range defined by table:table-cell with repeated columns attribute there is column that has changed default-cell-style-name then before ';'col_number 
                  in $TableColumnTagNum there should be 'K' ($TableColumnTagNum contains listed default-cell-style-name from backward) -->
      <xsl:when test="not(@table:style-name) and not(contains(substring-before($TableColumnTagNum,';$colNumber:'),'K') 
        or contains($TableColumnTagNum,concat('K',$colNumber))) and not(following-sibling::node()[1]) and name() = 'table:table-cell' and not(text:p)"> </xsl:when>
			<!--Vijayeta,SP2End-->
      <!--RefNo-4:added $columnCellStyle-->
      <!--RefNo-6:added condition or name()='table:covered-table-cell'-->
      <xsl:when
			  test="@table:number-columns-repeated and number($tableNumberColumnsRepeated) &gt; $ColumnRepeated and (@table:style-name or following-sibling::node() or text:p or $columnCellStyle != '' or name()='table:covered-table-cell')">
   
        <!--RefNo-1-->
        <!--RefNo-6: added or following-sibling::table:covered-table-cell or name()='table:covered-table-cell'-->
        <xsl:if test="not(not(following-sibling::table:table-cell) and not(child::text:p) and (not(attribute::table:style-name) or (256 = ($colNumber + @table:number-columns-repeated)))) or following-sibling::table:covered-table-cell or name()='table:covered-table-cell'">
        <xsl:call-template name="InsertConvertCell">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$colNumber + 1"/>
          </xsl:with-param>
          <xsl:with-param name="rowNumber" select="$rowNumber"/>
          <xsl:with-param name="cellNumber">
            <xsl:value-of select="$cellNumber"/>
          </xsl:with-param>
          <xsl:with-param name="ColumnRepeated">
            <xsl:value-of select="$ColumnRepeated + 1"/>
          </xsl:with-param>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="CountStyleTableCell">
            <xsl:value-of select="$CountStyleTableCell"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:call-template>
        </xsl:if>
      </xsl:when>
      <xsl:otherwise>

        <!-- insert next cell -->
        <xsl:call-template name="InsertNextCell">
          <xsl:with-param name="colNumber">
            <xsl:value-of select="$colNumber"/>
          </xsl:with-param>
          <xsl:with-param name="rowNumber">
            <xsl:value-of select="$rowNumber"/>
          </xsl:with-param>
          <xsl:with-param name="cellNumber">
            <xsl:value-of select="$cellNumber"/>
          </xsl:with-param>
          <xsl:with-param name="TableColumnTagNum">
            <xsl:value-of select="$TableColumnTagNum"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCell">
            <xsl:value-of select="$MergeCell"/>
          </xsl:with-param>
          <xsl:with-param name="MergeCellStyle">
            <xsl:value-of select="$MergeCellStyle"/>
          </xsl:with-param>
          <xsl:with-param name="multilines">
            <xsl:value-of select="$multilines"/>
          </xsl:with-param>
          <xsl:with-param name="hyperlinkStyle">
            <xsl:value-of select="$hyperlinkStyle"/>
          </xsl:with-param>
          <xsl:with-param name="contentFontsCount">
            <xsl:value-of select="$contentFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="styleFontsCount">
            <xsl:value-of select="$styleFontsCount"/>
          </xsl:with-param>
          <xsl:with-param name="cellFormats">
            <xsl:value-of select="$cellFormats"/>
          </xsl:with-param>
          <xsl:with-param name="cellStyles">
            <xsl:value-of select="$cellStyles"/>
          </xsl:with-param>
          <xsl:with-param name="CheckIfConditional">
            <xsl:value-of select="$CheckIfConditional"/>
          </xsl:with-param>
          <xsl:with-param name="pivotCells">
            <xsl:value-of select="$pivotCells"/>
          </xsl:with-param>
        </xsl:call-template>

      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:variable name="StyleApplyStyleName">
    <xsl:value-of select="@style:apply-style-name"/>
  </xsl:variable>

  <xsl:template name="InsertCell">
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="cellNumber"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:param name="MergeCell"/>
    <xsl:param name="MergeCellStyle"/>
    <xsl:param name="CountStyleTableCell"/>
    <xsl:param name="multilines"/>
    <xsl:param name="hyperlinkStyle"/>
    <xsl:param name="contentFontsCount"/>
    <xsl:param name="styleFontsCount"/>
    <xsl:param name="cellFormats"/>
    <xsl:param name="cellStyles"/>
    <xsl:param name="CheckIfConditional"/>
    <xsl:param name="pivotCells"/>

    <xsl:variable name="columnCellStyle">
      <xsl:call-template name="GetColumnCellStyle">
        <xsl:with-param name="colNum">
          <xsl:value-of select="$colNumber + 1"/>
        </xsl:with-param>
        <xsl:with-param name="TableColumnTagNum">
          <xsl:value-of select="$TableColumnTagNum"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="CheckIfMerge">
      <xsl:call-template name="CheckIfMergeColl">
        <xsl:with-param name="MergeCell">
          <xsl:value-of select="$MergeCell"/>
        </xsl:with-param>
        <xsl:with-param name="MergeCellStyle">
          <xsl:value-of select="$MergeCellStyle"/>
        </xsl:with-param>
        <xsl:with-param name="colNumber">
          <xsl:value-of select="$colNumber"/>
        </xsl:with-param>
        <xsl:with-param name="rowNumber">
          <xsl:value-of select="$rowNumber"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="colChar">
      <xsl:call-template name="NumbersToChars">
        <xsl:with-param name="num" select="$colNumber"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:variable name="cellNum">
      <xsl:value-of select="concat($colChar, $rowNumber)"/>
    </xsl:variable>


    <!--RefNo-4: Added condition or $columnCellStyle != ''-->
    <xsl:if
      test="parent::node()/@table:number-rows-repeated or child::text:p or name() = 'table:covered-table-cell' or $CheckIfMerge = 'start' or @table:style-name != '' or $columnCellStyle != ''">

      <c>

        <xsl:attribute name="r">
          <xsl:value-of select="$cellNum"/>
        </xsl:attribute>

        <!-- insert cell style number -->
        <xsl:choose>
          <!-- if it is a multiline cell -->
          <xsl:when test="text:p[2] and not(name() = 'table:covered-table-cell')">

            <xsl:variable name="multilineNumber">
              <xsl:for-each select="text:p[2]">
                <xsl:number count="table:table-cell[text:p[2]]" level="any"/>
              </xsl:for-each>
            </xsl:variable>
            <!--Start of RefNo-3:checking if hyperlink inside multiline cell-->
            <xsl:choose>
              <xsl:when test="descendant::text:a[not(ancestor::office:annotation)]">
                <xsl:variable name="hyperlinkId">
                  <xsl:number count="table:table-cell[descendant::text:a]"
                    level="any"/>
                </xsl:variable>
         <!-- Code Added By Sateesh Reddy Date:01-Feb-2008  -->
                <!--Start of RefNo-9-->
                <!--<xsl:if test="($numCellFormats + $multilines + $numStyles + $hyperlinkId - 1)">
            <xsl:attribute name="s">
                          <xsl:value-of select="$numCellFormats + $multilines + $numStyles + $hyperlinkId - 1"/>
                  </xsl:attribute>
                </xsl:if>-->
                <xsl:if test="($CountStyleTableCell + $styleFontsCount + $multilines + $hyperlinkId)!=''">
            <xsl:attribute name="s">
                          <!--<xsl:value-of select="$cellFormats + $cellStyles + $hyperlinkId - 1"/>-->
                    <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $multilines + $hyperlinkId"/>
            </xsl:attribute>
            </xsl:if>
                <!--End of RefNo-9-->
          </xsl:when>
              <xsl:otherwise>
                <!--Start of RefNo-9-->
                <!--<xsl:if test="($cellFormats + $cellStyles + $multilineNumber + count(ancestor::table:table/descendant::text:a[not(ancestor::office:annotation)])- 1) != ''">
            <xsl:attribute name="s">
                    <xsl:value-of select="$cellFormats + $cellStyles + $multilineNumber + count(ancestor::table:table/descendant::text:a[not(ancestor::office:annotation)]) - 1"/>
            </xsl:attribute>
                </xsl:if>-->
                <xsl:if test="($CountStyleTableCell + $styleFontsCount + $multilineNumber) != ''">
                  <xsl:attribute name="s">
                    <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $multilineNumber"/>
                  </xsl:attribute>
            </xsl:if>
                <!--End of RefNo-9-->
              </xsl:otherwise>
            </xsl:choose>
            <!--End of RefNo-3-->
          </xsl:when>

          <!-- if it is a hyperlink  in the cell-->
          <xsl:when test="descendant::text:a[not(ancestor::office:annotation)]">

              <xsl:choose>
                <!-- if there is no 'Hyperlink' style -->
                <xsl:when
                  test="not(document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = 'Hyperlink' ])">
                  <xsl:variable name="TableStyleName">
                    <xsl:value-of select="@table:style-name"/>
                  </xsl:variable>   
                  <xsl:choose>
                    <xsl:when test="$TableStyleName != ''">
                      <xsl:attribute name="style-number-change-post">
                        <xsl:value-of select="concat($TableStyleName, 'h')"/>
                      </xsl:attribute>                      
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:attribute name="style-number-change-post">
                        <xsl:value-of select="concat('Default', 'h')"/>
                      </xsl:attribute>
                    </xsl:otherwise>
                  </xsl:choose>
                 
                </xsl:when>
                <xsl:otherwise>
                  <xsl:variable name="hyperlinkId">
                    <xsl:number count="table:table-cell[descendant::text:a]"
                      level="any"/>
                  </xsl:variable>
                  <!-- Code Added By Sateesh Reddy Date:01-Feb-2008  -->
                  <xsl:if test="($CountStyleTableCell + $styleFontsCount + $hyperlinkId)!= ''">
                  <xsl:attribute name="s">
                      <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $hyperlinkId"/>
                  </xsl:attribute>
                  </xsl:if>
                </xsl:otherwise>
              </xsl:choose>

          </xsl:when>

          <xsl:otherwise>
            <xsl:choose>
              <!--RefNo-7:Changed condition to check for the correct merged cell To deferentiate b/w A3 and AA3
              substring-before(substring-after(concat(';',$MergeCellStyle), concat(';',$cellNum, ':')), ';') != ''-->
              <xsl:when
                test="(name() = 'table:covered-table-cell'  or name() = 'table:table-cell' )and (substring-before(substring-after(concat(';',$MergeCellStyle), concat(';',$cellNum, ':')), ';') != '')">
                <xsl:variable name="style">
                  <!--Start of RefNo-7
                  <xsl:value-of
                    select="substring-before(substring-after($MergeCellStyle, concat($cellNum, ':')), ';')"
                  />-->
                  <xsl:value-of
                      select="substring-before(substring-after(concat(';',$MergeCellStyle), concat(';',$cellNum, ':')), ';')"/>
                  <!--End of RefNo-7-->
                </xsl:variable>
                <xsl:choose>
                  <xsl:when test="key('style', $style)">
                    <xsl:attribute name="style-number-change-post">
                      <xsl:value-of select="$style"/>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:variable name="TableStyleName">
                      <xsl:value-of select="@table:style-name"/>
                    </xsl:variable>
                    <xsl:if
                      test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $TableStyleName ]">
                      <xsl:attribute name="style-number-change-post">
                        <xsl:value-of select="$TableStyleName"/>
                      </xsl:attribute>
                    </xsl:if>
                  </xsl:otherwise>
                </xsl:choose>

              </xsl:when>
              <!-- when style is specified in cell -->
              <xsl:when test="@table:style-name and not(name() ='table:covered-table-cell')">

                <xsl:variable name="StyleApplyStyleName">
                  <xsl:value-of select="@style:apply-style-name"/>
                </xsl:variable>
				  <!-- 
				  Code Changes by: Vijayeta
				  Defect         : 2563110 , Strike Through Lost
				 -->
				  <xsl:variable name ="tableStyleName">
					  <xsl:value-of select="@table:style-name"/>
				  </xsl:variable>
                <!--xsl:choose>
                  <xsl:when test="key('style',@table:style-name)">
                    <xsl:for-each select="key('style',@table:style-name)">
                      <xsl:attribute name="s">
                        <xsl:value-of select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
                      </xsl:attribute>
                    </xsl:for-each>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:variable name="TableStyleName">
                      <xsl:value-of select="@table:style-name"/>
                    </xsl:variable>
                    <xsl:for-each select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $TableStyleName ]">
                        <xsl:variable name="CountTableCell">
                          <xsl:value-of select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
                        </xsl:variable>
                        <xsl:attribute name="s">
                          <xsl:value-of select="$CountStyleTableCell+$CountTableCell"/>
                        </xsl:attribute>
                    </xsl:for-each>
                  </xsl:otherwise>
                  </xsl:choose-->
                <!--Code Added By Sateesh Reddy Date:01-Feb-2008 -->
                <!--RefNo-5: Changed if condition to chk for the styles in case of grouping.-->
                <!--<xsl:if test="parent::node()/parent::node()/parent::node()/parent::node()/parent::node()/office:automatic-styles/style:style/@style:name=@table:style-name and @table:style-name !=''">-->
				  <!-- 
				  Code Changes by: Vijayeta
				  Defect         : 2563110 , Strike Through Lost
				 -->
				  <xsl:choose>
					  <xsl:when test="//office:automatic-styles/style:style/@style:name=@table:style-name and @table:style-name !=''">
                <xsl:attribute name="style-number-change-post">
                  <xsl:value-of select="@table:style-name"/>
                </xsl:attribute>
					  </xsl:when>
					  <xsl:when test="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $tableStyleName]">
						  <xsl:attribute name="style-number-change-post">
							  <xsl:value-of select="@table:style-name"/>
						  </xsl:attribute>
					  </xsl:when>
				  </xsl:choose>				  
              </xsl:when>
              <!-- when style is specified in column -->
              <xsl:when test="$columnCellStyle != '' ">
                <!--xsl:choose>
                  <xsl:when test="key('style',$columnCellStyle)">
                    <xsl:for-each select="key('style',$columnCellStyle)">
                      <xsl:attribute name="s">
                        <xsl:value-of select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
                      </xsl:attribute>
                    </xsl:for-each>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:for-each select="document('styles.xml')/office:document-styles/office:styles/style:style[@style:name = $columnCellStyle ]">
                        <xsl:variable name="CountTableCell">
                          <xsl:value-of select="count(preceding-sibling::style:style[@style:family='table-cell']) + 1"/>
                        </xsl:variable>
                        <xsl:attribute name="s">
                          <xsl:value-of select="$CountStyleTableCell+$CountTableCell"/>
                        </xsl:attribute>
                    </xsl:for-each>
                  </xsl:otherwise>
                  </xsl:choose-->
                <xsl:attribute name="style-number-change-post">
                  <xsl:value-of select="$columnCellStyle"/>
                </xsl:attribute>
              </xsl:when>
              <!--Start of RefNo-9-->
              <xsl:when test="@table:formula">                
                <xsl:variable name="hyperlinkCnt">
                  <xsl:value-of select="count(/descendant::text:a[not(ancestor::office:annotation)])" />
                </xsl:variable>
                <xsl:choose>
                  <xsl:when test="@office:value-type='date'">
                    <xsl:choose>
                      <xsl:when test="contains(@office:date-value,'T')">
                        <xsl:attribute name="s">
                          <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $multilines + $hyperlinkCnt + 3"/>
                        </xsl:attribute>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:attribute name="s">
                          <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $multilines + $hyperlinkCnt + 1"/>
                        </xsl:attribute>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:when>
                  <xsl:when test="@office:value-type = 'time'">
                    <xsl:attribute name="s">
                      <xsl:value-of select="$CountStyleTableCell + $styleFontsCount + $multilines + $hyperlinkCnt + 2"/>
                    </xsl:attribute>
                  </xsl:when>
                </xsl:choose>
              </xsl:when>
              <!--End of RefNo-9-->
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
        <!-- convert cell type -->
        <xsl:if test="child::text:p and not(name() = 'table:covered-table-cell')">
          <xsl:choose>
            <xsl:when
              test="@office:value-type='float' or (@office:value-type!='string' and @office:value-type != 'percentage' and @office:value-type != 'currency' and @office:value-type != 'date' and @office:value-type != 'time' and @office:value-type!='boolean' and (number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%')))">
              <xsl:variable name="Type">
                <xsl:call-template name="ConvertTypes">
                  <xsl:with-param name="type">
                    <xsl:value-of select="@office:value-type"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:variable>

              <xsl:attribute name="t">
                <xsl:value-of select="$Type"/>
              </xsl:attribute>
              <!--Strat of RefNo-8: Check for formula -->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:choose>
                  <xsl:when test="$Type = 'n' ">
                    <xsl:choose>
                      <xsl:when test="@office:value != ''">
                        <xsl:value-of select="@office:value"/>
                      </xsl:when>
                      <xsl:otherwise>
                        <xsl:value-of select="translate(text:p,',','.')"/>
                      </xsl:otherwise>
                    </xsl:choose>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="text:p"/>
                  </xsl:otherwise>
                </xsl:choose>
              </v>
            </xsl:when>

            <!-- percentage -->
            <xsl:when test="@office:value-type = 'percentage'">
              <!--Strat of RefNo-8: Check for formula -->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:choose>
                  <xsl:when test="@office:value">
                    <xsl:value-of select="@office:value"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="translate(substring-before(text:p, '%'), ',', '.')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </v>
            </xsl:when>

            <!-- currency -->
            <xsl:when test="@office:value-type = 'currency'">
              <!--Strat of RefNo-8: Check for formula -->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:value-of select="@office:value"/>
              </v>
            </xsl:when>

            <!-- date -->
            <xsl:when test="@office:value-type='date'">
              <!--Strat of RefNo-8: Check for formula -->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:call-template name="DateToNumber">
                  <xsl:with-param name="value">
                    <xsl:value-of select="@office:date-value"/>
                  </xsl:with-param>
                </xsl:call-template>
              </v>
            </xsl:when>

            <!-- time-->
            <xsl:when test="@office:value-type = 'time'">
              <!--Strat of RefNo-8: Check for formula -->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:call-template name="TimeToNumber">
                  <xsl:with-param name="value">
                    <xsl:value-of select="@office:time-value"/>
                  </xsl:with-param>
                </xsl:call-template>
              </v>
            </xsl:when>

            <!--Start of RefNo-8: For bollean value-->
            <xsl:when test="@office:value-type = 'boolean'">
              <xsl:attribute name="t">b</xsl:attribute>
              <!--<xsl:if test="not(@office:boolean-value = 'true' or @office:boolean-value = 'false')">
                <xsl:attribute name="t">e</xsl:attribute>
              </xsl:if>-->
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <v>
                <xsl:choose>
                  <!--<xsl:when test="@office:boolean-value='false'">
                    <xsl:value-of select="'0'"/>
                  </xsl:when>
                  <xsl:when test="@office:boolean-value='true'">
                    <xsl:value-of select="'1'"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="text:p"/>
                  </xsl:otherwise>-->
                  <xsl:when test="text:p='TRUE'">
                    <xsl:value-of select="'1'"/>
                  </xsl:when>
                  <!--<xsl:when test="text:p='FALSE'">
                    <xsl:value-of select="'0'"/>
                  </xsl:when>-->
                  <xsl:otherwise>
					  <xsl:value-of select="'0'"/>
                  </xsl:otherwise>
                </xsl:choose>                
              </v>
            </xsl:when>
            <!--End of RefNo-8-->

            <!-- last or when number cell has error -->
            <!--Strat of RefNo-8: Check for formula -->
            <xsl:when
              test="not(@office:value-type='float') and @office:value-type = 'string' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency'))">
            <!--<xsl:when
              test="not(@office:value-type='float') and @office:value-type = 'string' or @office:value-type = 'boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency'))">-->
              <xsl:attribute name="t">s</xsl:attribute>
              
              <xsl:if test="@table:formula">
                <xsl:call-template name="tmpFormula">
                  <xsl:with-param name="cellNum" select="$cellNum"/>
                  <xsl:with-param name="colNumber" select="$colNumber"/>
                  <xsl:with-param name="rowNumber" select="$rowNumber"/>
                  <xsl:with-param name="colChar" select="$colChar"/>
                </xsl:call-template>
              </xsl:if>
              <!--End of RefNo-8: Check for formula -->
              <v>
                <xsl:value-of select="number($cellNumber)"/>
              </v>
            </xsl:when>
          </xsl:choose>

        </xsl:if>

        <!-- cell info for pivotTable postprocessor -->
        <xsl:if test="contains($pivotCells,concat(';',$colNumber + 1,':',$rowNumber,';'))">
          <pxsi:pivotCell>
            <xsl:attribute name="pxsi:col">            
              <xsl:value-of select="$colNumber + 1"/>
            </xsl:attribute>
            <xsl:attribute name="pxsi:row">
              <xsl:value-of select="$rowNumber"/>
            </xsl:attribute>
            <xsl:attribute name="pxsi:sheetNum">
              <xsl:value-of select="substring-before($pivotCells,'#')"/>
            </xsl:attribute>

            <!-- cell value -->
            <pxsi:val>
            <!-- number-->
            <xsl:choose>
              <xsl:when test="@office:value-type='float' ">
                <xsl:choose>
                  <xsl:when test="@office:value != ''">
                    <xsl:value-of select="@office:value"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="translate(text:p,',','.')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <!-- percentage -->
              <xsl:when test="@office:value-type = 'percentage'">
                <xsl:choose>
                  <xsl:when test="@office:value">
                    <xsl:value-of select="@office:value"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="translate(substring-before(text:p, '%'), ',', '.')"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:when>

              <!-- currency -->
              <xsl:when test="@office:value-type = 'currency'">
                <xsl:value-of select="@office:value"/>
              </xsl:when>

              <!-- date -->
              <xsl:when test="@office:value-type='date'">
                <xsl:call-template name="DateToNumber">
                  <xsl:with-param name="value">
                    <xsl:value-of select="@office:date-value"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>

              <!-- time-->
              <xsl:when test="@office:value-type = 'time'">
                <xsl:call-template name="TimeToNumber">
                  <xsl:with-param name="value">
                    <xsl:value-of select="@office:time-value"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>

              <!-- text -->
              <xsl:when
                test="not(@office:value-type='float') and @office:value-type = 'string' or @office:value-type = 'boolean' or not((number(text:p) or text:p = 0 or contains(text:p,',') or contains(text:p,'%') or @office:value-type='currency'))">
                <xsl:apply-templates mode="pivot" xml:space="preserve"/>
              </xsl:when>
            </xsl:choose>
            </pxsi:val>
            
            <!-- text value (including number formating) -->
            <pxsi:text>
              <xsl:apply-templates mode="pivot" xml:space="preserve"/>
            </pxsi:text>
            
          </pxsi:pivotCell>
        </xsl:if>

      </c>
    </xsl:if>
  </xsl:template>

  <xsl:template name="GetColumnCellStyle">
    <xsl:param name="colNum"/>
    <xsl:param name="table-column_tagNum" select="1"/>
    <xsl:param name="column" select="1"/>
    <xsl:param name="TableColumnTagNum"/>
    <xsl:value-of
      select="substring-before(substring-after($TableColumnTagNum, concat(concat('K', $colNum), ':')), ';')"
    />
  </xsl:template>

  <xsl:template name="GetColNumber">
    <xsl:param name="position"/>
    <xsl:param name="count" select="1"/>
    <xsl:param name="value" select="1"/>

    <xsl:choose>
      <xsl:when test="$count &lt; $position">
				<!--<xsl:variable name="columns">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated">
              <xsl:value-of select="@table:number-columns-repeated"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>1</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>-->
				<!--Vijayeta,SP2,@table:number-columns-repeated-->
        <xsl:variable name="columns">
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
        <xsl:for-each
          select="following-sibling::node()[name() = 'table:table-cell' or name() = 'table:covered-table-cell' ][1]">
          <xsl:call-template name="GetColNumber">
            <xsl:with-param name="position">
              <xsl:value-of select="$position"/>
            </xsl:with-param>
            <xsl:with-param name="count">
              <xsl:value-of select="$count + 1"/>
            </xsl:with-param>
            <xsl:with-param name="value">
              <xsl:value-of select="$value + $columns"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="GetRowNumber">
    <xsl:param name="rowId"/>
    <xsl:param name="tableId"/>
    <xsl:param name="value" select="1"/>

    <xsl:choose>
      <xsl:when test="generate-id(.) != $rowId">
        <xsl:variable name="rows">
          <xsl:choose>
						<!--Vijayeta,SP2,@table:number-rows-repeated-->
						<!--<xsl:when test="@table:number-rows-repeated">
							<xsl:value-of select="@table:number-rows-repeated"/>
						</xsl:when>-->
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
						<!--Vijayeta,SP2,@table:number-rows-repeated,End-->
            <xsl:otherwise>
              <xsl:text>1</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:for-each
          select="following::table:table-row[generate-id(ancestor::table:table) = $tableId][1]">
          <xsl:call-template name="GetRowNumber">
            <xsl:with-param name="rowId" select="$rowId"/>
            <xsl:with-param name="tableId" select="$tableId"/>
            <xsl:with-param name="value">
              <xsl:value-of select="$value + $rows"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>

      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$value"/>
      </xsl:otherwise>
    </xsl:choose>
    <!--<xsl:value-of select="$rows"/>-->
  </xsl:template>

  <xsl:template name="GetLinkNumber">
    <xsl:param name="position"/>
    <xsl:param name="count" select="1"/>
    <xsl:param name="value" select="1"/>
    <xsl:choose>
      <xsl:when test="$count &lt; $position">
        <xsl:variable name="linkNum"/>

        <xsl:for-each select="following-sibling::text:a[1]">
          <xsl:call-template name="GetLinkNumber">
            <xsl:with-param name="position">
              <xsl:value-of select="$position"/>
            </xsl:with-param>
            <xsl:with-param name="count">
              <xsl:value-of select="$count + 1"/>
            </xsl:with-param>
            <xsl:with-param name="value">
              <xsl:value-of select="$value + $linkNum"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="CheckIfColumnStyle">
    <xsl:param name="number"/>
    <xsl:param name="table"/>
    <xsl:param name="count" select="0"/>
    <xsl:param name="value"/>

    <xsl:variable name="newValue">
      <xsl:for-each select="table:table-cell[1]">
        <xsl:call-template name="CheckIfCellNonEmpty">
          <xsl:with-param name="number">
            <xsl:value-of select="$number"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:variable>

    <xsl:choose>
      <xsl:when test="$newValue != 'end' ">
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
        <xsl:variable name="table2">
          <xsl:for-each select="following::table:table-row[1]">
            <xsl:value-of select="generate-id(ancestor::table:table)"/>
          </xsl:for-each>
        </xsl:variable>

        <!-- go to the next row in the same table-->
        <xsl:choose>
          <xsl:when test="$table = $table2">
            <xsl:for-each select="following::table:table-row[1]">
              <xsl:call-template name="CheckIfColumnStyle">
                <xsl:with-param name="number" select="$number"/>
                <xsl:with-param name="table" select="$table"/>
                <xsl:with-param name="count" select="$count + $rows"/>
                <xsl:with-param name="value" select="$newValue"/>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <xsl:otherwise>
            <xsl:choose>
              <!-- when last possible row was reached -->
              <!--RefNo-4(1898009):Added greater than condition-->
              <xsl:when test="$count + $rows &gt;= 65536">
                <xsl:text>true</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>false</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <!-- when cell in column $number didn't occur in the row-->
      <xsl:otherwise>
        <xsl:text>false</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- "true" when cell is non-empty, "false" when empty, "end" when after the end of a row -->
  <xsl:template name="CheckIfCellNonEmpty">
    <xsl:param name="number"/>
    <xsl:param name="count" select="1"/>
    <xsl:param name="value"/>

    <xsl:choose>
      <xsl:when test="$count &lt; $number">
				<!--<xsl:variable name="columns">
          <xsl:choose>
            <xsl:when test="@table:number-columns-repeated">
              <xsl:value-of select="@table:number-columns-repeated"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>1</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>-->
				<!--Vijayeta,SP2,@table:number-columns-repeated-->
        <xsl:variable name="columns">
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

        <xsl:variable name="newValue">
          <xsl:choose>
            <xsl:when test="text:p">
              <xsl:text>true</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>false</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>

        <xsl:choose>
          <xsl:when test="following-sibling::table:table-cell[1]">
            <xsl:for-each select="following-sibling::table:table-cell[1]">
              <xsl:call-template name="CheckIfCellNonEmpty">
                <xsl:with-param name="number">
                  <xsl:value-of select="$number"/>
                </xsl:with-param>
                <xsl:with-param name="count">
                  <xsl:value-of select="$count + $columns"/>
                </xsl:with-param>
                <xsl:with-param name="value">
                  <xsl:value-of select="$newValue"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:for-each>
          </xsl:when>
          <!-- when cell is in last table:table-cell tag and it has columns-repeated attribute -->
          <xsl:when test="$columns &gt; 1 and $count + $columns &gt;= $number">
            <xsl:value-of select="$newValue"/>
          </xsl:when>
          <!-- when cell is after the end of the row -->
          <xsl:otherwise>
            <xsl:text>end</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:choose>
          <xsl:when test="$count = $number">
            <xsl:variable name="newValue">
              <xsl:choose>
                <xsl:when test="text:p">
                  <xsl:text>true</xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:text>false</xsl:text>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:variable>
            <xsl:value-of select="$newValue"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$value"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="text()" mode="cell"/>

  <!--Strat of RefNo-8: to get formula : Sanjay-->
  <xsl:template name="tmpFormula">
    <xsl:param name="cellNum"/>
    <xsl:param name="colNumber"/>
    <xsl:param name="rowNumber"/>
    <xsl:param name="colChar"/>
    <xsl:variable name="extLink">
      <xsl:text>[&apos;file:///</xsl:text>
    </xsl:variable>      
    <xsl:variable name="tblRef">
      <xsl:text>&apos;)</xsl:text>
    </xsl:variable>    
    <xsl:choose>
      <xsl:when test="not(
                        (contains(@table:formula,$extLink))                   
                    or (contains(@table:formula,'ARABIC(')) 
                    or (contains(@table:formula,'DDE('))
                    or (contains(@table:formula,'JIS('))
                    or (contains(@table:formula,'SHEET('))
                    or (contains(@table:formula,'SHEETS('))
                    or (contains(@table:formula,'STYLE('))
                    or (contains(@table:formula,'MUNIT(')) 
                    or (contains(@table:formula,'DURATION(') and not(contains(@table:formula,'getDuration(')))                    
                    or (contains(@table:formula,'ACOT('))
                    or (contains(@table:formula,'ACOTH('))
                    or (contains(@table:formula,'COT('))
                    or (contains(@table:formula,'COTH('))
                    or (contains(@table:formula,'BASE(')) 
                    or (contains(@table:formula,'B(') and not(contains(@table:formula,'DB(')) and not(contains(@table:formula,'DDB(')) and not(contains(@table:formula,'PROB(')))
                    or (contains(@table:formula,'COMBINA('))
                    or (contains(@table:formula,'CURRENT(')) 
                    or (contains(@table:formula,'DAYS(') and not(contains(@table:formula,'Networkdays(')))
                    or (contains(@table:formula,'getDaysInMonth(')) 
                    or (contains(@table:formula,'getDaysInYear(')) 
                    or (contains(@table:formula,'DDE('))
                    or (contains(@table:formula,'EASTERSUNDAY(')) 
                    or (contains(@table:formula,'EFFECTIVE(')) 
                    or (contains(@table:formula,'FORMULA('))
                    or (contains(@table:formula,'GAUSS(')) 
                    or (contains(@table:formula,'ISFORMULA(')) 
                    or (contains(@table:formula,'getIsLeapYear('))
                    or (contains(@table:formula,'getDiffMonths('))
                    or (contains(@table:formula,'PERMUTATIONA(')) 
                    or (contains(@table:formula,'PHI('))
                    or (contains(@table:formula,'getRot13('))
                    or (contains(@table:formula,'CONVERT(') and not(contains(@table:formula,'getConvert(')))
                    or (contains(@table:formula,'WEEKNUM(') and not(contains(@table:formula,'getWeeknum(')))
                    or (contains(@table:formula,'getDiffWeeks('))
                    or (contains(@table:formula,'getWeeksInYear('))
                    or (contains(@table:formula,'getDiffYears('))
                    or (contains(@table:formula,'DECIMAL('))
                    or (contains(@table:formula,'ZGZ('))
                    or ((contains(@table:formula,'IF(') and (contains(@table:formula,' AND ') or contains(@table:formula,')AND(') or contains(@table:formula,' OR ') or contains(@table:formula,')OR(')))and not(contains(@table:formula,'COUNTIF(')) and not(contains(@table:formula,'SUMIF(')))
                    or (contains(@table:formula,'TABLE('))                    
                    or (contains(@table:formula,$tblRef))                    
                    or (contains(@office:string-value,'#NAME?'))
                    or (contains(@office:string-value,'#N/A'))                                        
				
				    or (contains(@office:string-value,'#N/D'))
				
                    or (text:p='Err:511')
                    or (text:p='#VALUE!')
                    or (text:p='Err:504')
                    or (text:p='Err:502')
                    or (text:p='Err:501')
                    or (text:p='Err:501')
                    or (text:p='Err:502')
                    or (text:p='Err:503')
                    or (text:p='Err:504')
                    or (text:p='Err:508')
                    or (text:p='Err:509')
                    or (text:p='Err:510')
                    or (text:p='Err:511')
                    or (text:p='Err:512')
                    or (text:p='Err:513')
                    or (text:p='Err:514')
                    or (text:p='Err:516')
                    or (text:p='Err:517')
                    or (text:p='Err:518')
                    or (text:p='Err:519')
                    or (text:p='Err:520')
                    or (text:p='Err:521')
                    or (text:p='Err:522')
                    or (text:p='Err:523')
                    or (text:p='Err:524')
                    or (text:p='Err:525')
                    or (text:p='Err:526')
                    or (text:p='Err:527')
                    or (text:p='Err:532')
                    or (text:p='#REF!')
                    or (text:p='#N/A')
				
					or (text:p='#N/D')
				
                    or (text:p='#NAME?')
                    or (text:p='#NOM ?')
                    or (text:p='#DIV/0!')
                    or (text:p='#NUM!')
                    or (contains(@table:formula,'ASCIITOCHAR(')) 
                    or (contains(@table:formula,'CARX('))
                    or (contains(@table:formula,'[.BOOL2]INT('))
                    or (contains(@table:formula,'[.BOOL2]STRING('))
                    or (contains(@table:formula,'CARY('))
                    or (contains(@table:formula,'CHARTOASCII('))
                    or (contains(@table:formula,'COMPARE(')) 
                    or (contains(@table:formula,'COMPOUND(')) 
                    or (contains(@table:formula,'CONTINUOUS('))
                    or (contains(@table:formula,'CUR('))
                    or (contains(@table:formula,'CURRENTDATE('))
                    or (contains(@table:formula,'CURRENTDATETIME('))
                    or (contains(@table:formula,'CURRENTTIME('))
                    or (contains(@table:formula,'DAYNAME(')) 
                    or (contains(@table:formula,'DAYOFYEAR(')) 
                    or (contains(@table:formula,'DAYSINMONTH('))
                    or (contains(@table:formula,'DAYSINYEAR('))
                    or (contains(@table:formula,'DECSEX('))
                    or (contains(@table:formula,'DIV('))
                    or (contains(@table:formula,'EPS('))
                    or (contains(@table:formula,'EURO('))     
                    or (contains(@table:formula,'FV_ANNUITY('))
                    or (contains(@table:formula,'GPRODUCT('))
                    or (contains(@table:formula,'INV(')) 
                    or (contains(@table:formula,'KPRODUCT(')) 
                    or (contains(@table:formula,'[.LOG2]('))
                    or (contains(@table:formula,'LOGn('))
                    or (contains(@table:formula,'MULTIPLY('))
                    or (contains(@table:formula,'POW('))
                    or (contains(@table:formula,'RANDBERNOULLI('))
                    or (contains(@table:formula,'RANDBINOM('))
                    or (contains(@table:formula,'RANDEXP('))
                    or (contains(@table:formula,'RANDNEGBINOM('))
                    or (contains(@table:formula,'RANDNORM('))
                    or (contains(@table:formula,'RANDPOISSON(')) 
                    or (contains(@table:formula,'ROOTN(')) 
                    or (contains(@table:formula,'SUMA('))
                    or (contains(@table:formula,'BINO('))
                    or (contains(@table:formula,'HOURS('))
                    or (contains(@table:formula,'NVBINO('))
                    or (contains(@table:formula,'ISDATE('))
                    or (contains(@table:formula,'ISLEAPYEAR('))     
                    or (contains(@table:formula,'ISNOTTEXT('))
                    or (contains(@table:formula,'ISNUM('))
                    or (contains(@table:formula,'ISOWEEKNUM(')) 
                    or (contains(@table:formula,'ISTIME(')) 
                    or (contains(@table:formula,'KURTP('))
                    or (contains(@table:formula,'LEVEL_COUPON('))
                    or (contains(@table:formula,'MINUTES('))
                    or (contains(@table:formula,'MONTHNAME('))
                    or (contains(@table:formula,'MONTHS('))
                    or (contains(@table:formula,'NAND('))
                    or (contains(@table:formula,'NOR('))
                    or (contains(@table:formula,'[.NUM2]STRING('))
                    or (contains(@table:formula,'POLA('))
                    or (contains(@table:formula,'POLR('))
                    or (contains(@table:formula,'PV_ANNUITY('))     
                    or (contains(@table:formula,'REGEXP('))
                    or (contains(@table:formula,'REGEXPRE('))
                    or (contains(@table:formula,'ROT(')) 
                    or (contains(@table:formula,'SECONDS(')) 
                    or (contains(@table:formula,'SEXDEC('))
                    or (contains(@table:formula,'SKEWP('))
                    or (contains(@table:formula,'SLEEK('))
                    or (contains(@table:formula,'STRING('))
                    or (contains(@table:formula,'[.SUM2]XMY('))
                    or (contains(@table:formula,'TOGGLE('))
                    or (contains(@table:formula,'VARIANCE('))
                    or (contains(@table:formula,'WEEKS('))
                    or (contains(@table:formula,'WEEKSINYEAR(')) 
                    or (contains(@table:formula,'XOR(')) 
                    or (contains(@table:formula,'YEARS('))
                    or (contains(@table:formula,'ZERO_COUPON(')))">
        <f>
          <xsl:choose>
            <xsl:when test="@table:number-matrix-columns-spanned or @table:number-matrix-rows-spanned">
              <xsl:variable name="tmpColumnSpanNo">
                <xsl:value-of select="@table:number-matrix-columns-spanned"/>
              </xsl:variable>
              <xsl:variable name="tmpRowSpanNo">
                <xsl:value-of select="@table:number-matrix-rows-spanned"/>
              </xsl:variable>
              <xsl:attribute name="t">
                <xsl:value-of select="'array'"/>
              </xsl:attribute>
              <xsl:attribute name="ref">
                <xsl:choose>
                  <xsl:when test="($tmpColumnSpanNo='1') and ($tmpRowSpanNo='1')">
                    <xsl:value-of select="$cellNum"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:variable name="tmpColChar">
                      <xsl:choose>
                        <xsl:when test="$tmpColumnSpanNo &gt; 1">
                          <xsl:call-template name="NumbersToChars">
                            <xsl:with-param name="num" select="($colNumber + $tmpColumnSpanNo - 1)"/>
                          </xsl:call-template>
                        </xsl:when>
                        <xsl:otherwise>
                          <xsl:value-of select="$colChar"/>
                        </xsl:otherwise>
                      </xsl:choose>
                    </xsl:variable>
                    <xsl:variable name="tmpCR1">
                      <xsl:value-of select="($rowNumber + $tmpRowSpanNo) - 1 "/>
                    </xsl:variable>
                    <xsl:value-of select="concat($cellNum,':',(concat($tmpColChar,$tmpCR1)))"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:attribute>
            </xsl:when>
          </xsl:choose>
          <xsl:value-of select="concat('sonataOdfFormula', @table:formula)"/>
        </f>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
  <!--End of RefNo-8-->

</xsl:stylesheet>
